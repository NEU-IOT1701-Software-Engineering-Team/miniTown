#include"Player.h"

static std::vector<Musci*>_listMusic;
std::mutex m_listMusic;//��_listPlay�Ļ�����

Player::Player()
{
}

Player::~Player()
{
	while (!_listMusic.empty()) {
		;
	}
}

void Player::addMusci(Musci* music)
{
	music->id = time(NULL);
	
	HANDLE hThread = CreateThread(NULL, 0,Player::_Play, (LPVOID)music->id, CREATE_SUSPENDED/*������������ִ��*/, NULL);
	music->hThread = hThread;
	
	m_listMusic.lock();
	_listMusic.push_back(music);
	m_listMusic.unlock();

	Sleep(timeout);//��Ӳ��������Ҫʱ��

	ResumeThread(music->hThread);//�ָ���������̣߳����俪ʼִ��
}

DWORD __stdcall Player::_Play(LPVOID lpParameter)
{
	std::cout << (time_t)lpParameter << std::endl;
	Musci* pMusic = NULL;
	m_listMusic.lock();
	std::vector<Musci*>::iterator it = _listMusic.begin();
	for (; it != _listMusic.end(); ++it) {
		if ((*(*it)) == (time_t)lpParameter) {
			pMusic = (*it);
			break;
		}
	}
	m_listMusic.unlock();

	DWORD ret = mciSendCommand(0, MCI_OPEN,
		MCI_OPEN_ELEMENT, (DWORD_PTR)&pMusic->MciOpen);//��һ���豸 ׼�����Ÿ���Ч
	_print(pMusic, " MCI_OPEN ret:%d\n", ret);
	
	pMusic->MciStatus.dwItem = MCI_STATUS_LENGTH;//Ҫ��ȡ���� ��ȡ����
	ret = mciSendCommand(pMusic->MciOpen.wDeviceID, MCI_STATUS,
		MCI_STATUS_ITEM, (DWORD_PTR)(LPVOID)&pMusic->MciStatus);//��ȡ��Чʱ��  ��λ����
	_print(pMusic, " MCI_STATUS ret:%d length:%d\n", ret, pMusic->MciStatus.dwReturn);

	pMusic->nLength = pMusic->MciStatus.dwReturn;


	pMusic->MciSetAudio.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	pMusic->MciSetAudio.dwValue = pMusic->volume;
	ret = mciSendCommand(pMusic->MciOpen.wDeviceID, MCI_SETAUDIO,
		MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD_PTR)&pMusic->MciSetAudio);//��������
	_print(pMusic, " MCI_SETAUDIO ret:%d\n", ret);

	while (!pMusic->isExit) {
		while (pMusic->isPlay) {
			ret = mciSendCommand(pMusic->MciOpen.wDeviceID, MCI_PLAY,
				MCI_NOTIFY, (DWORD_PTR)(LPVOID)&pMusic->MciPlay);//��ʼ����
			_print(pMusic, " MCI_PLAY ret:%d\n", ret);

			if (pMusic->nLength < ResponseTime) {
				Sleep(pMusic->nLength);
			}
			else {
				for (int i = 0; pMusic->isPlay && i < pMusic->nLength; i += ResponseTime) {
					Sleep(ResponseTime);
				}
			}
			
			
			if (pMusic->isPlay) {
				if (pMusic->isLoop) {
					pMusic->isPlay = true;
				}
				else {
					pMusic->isPlay = false;
				}
			}
			
			ret = mciSendCommand(pMusic->MciOpen.wDeviceID, MCI_SEEK,
				MCI_SEEK_TO_START, (DWORD_PTR)&pMusic->MciSeek);//����ͷ
			_print(pMusic, " MCI_SEEK ret:%d\n", ret);
		}
	}
	ret = mciSendCommand(pMusic->MciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);//�ر��豸
	_print(pMusic, " MCI_CLOSE ret:%d\n", ret);

	it = _listMusic.begin();
	for (; it != _listMusic.end(); ++it) {
		if ((*(*it)) == (time_t)lpParameter) {
			_listMusic.erase(it);
			break;
		}
	}
	
	return 0;
}

void Player::_print(Musci* pMusic, char const* const _Format, ...)
{
	char _Buffer[512] = { 0 };
	//const char header
	int _Result;
	va_list _ArgList;
	__crt_va_start(_ArgList, _Format);
	sprintf(_Buffer, "%s hThread:%x DevID:%d", pMusic->fileName.c_str(), pMusic->hThread, pMusic->MciOpen.wDeviceID);
	_Result = vsprintf(_Buffer + strlen(_Buffer), _Format, _ArgList);
	if (_Result < 0) {
		__crt_va_end(_ArgList);
		//���������
		return;
	}
	std::cout << _Buffer;
	//OutputDebugString(_Buffer);
	__crt_va_end(_ArgList);
}
