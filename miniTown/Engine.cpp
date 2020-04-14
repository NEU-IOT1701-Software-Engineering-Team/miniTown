#include "miniTown.h"

const char* title = "miniTown";
 int nScreenWidth = SCREEN_WIDTH;
 int nScreenHeight = SCREEN_HEIGHT;
HWND hWnd = NULL;
HDC hMemDC = NULL;
HBITMAP hBitmapBuffer = NULL;
HBITMAP hBitmapDevice = NULL;
BYTE* buffer = NULL;
BYTE screen_keys[512] = { 0 };
Player player;

Object* drawList[MaxDrawObjectSum];

//std::vector<Player::_Sound> _listPlay;
std::list<Player::_Sound*> _listPlay;
std::vector<Player::_MSG> _listMsg;
const int timeout = 1400;//���ż�� ����Ƶ���������Ҫʱ����
const int step = 200;//��Ӧʱ��
std::mutex m_listPlay;//��_listPlay�Ļ�����
std::mutex m_listMsg;//��_listMsg�Ļ�����
Player::Player() {
	_StaticStructure();
}

Player::~Player() {
	//_StaticDestruct();
}

void Player::playSound(const char* szFileNameTemp, unsigned short volume) {
	Player::_MSG msg;
	msg.type = PLAYER_MSG_PLAY;
	msg.szFileName = (char*)szFileNameTemp;
	msg.volume = volume;

	m_listMsg.lock();
	_listMsg.push_back(msg);//��������Ϣ������Ϣ����
	m_listMsg.unlock();
}

void Player::playSound(char* szFileNameTemp, unsigned short volume) {
	Player::_MSG msg;
	msg.type = PLAYER_MSG_PLAY;
	msg.szFileName = szFileNameTemp;
	msg.volume = volume;

	m_listMsg.lock();
	_listMsg.push_back(msg);//��������Ϣ������Ϣ����
	m_listMsg.unlock();
}

void Player::playSoundLoop(const char* szFileNameTemp, unsigned short volume) {
	Player::_MSG msg;
	msg.type = PLAYER_MSG_LOOP;
	msg.szFileName = (char*)szFileNameTemp;
	msg.volume = volume;

	m_listMsg.lock();
	_listMsg.push_back(msg);//��������Ϣ������Ϣ����
	m_listMsg.unlock();
}

void Player::playSoundLoop(char* szFileNameTemp, unsigned short volume) {
	Player::_MSG msg;
	msg.type = PLAYER_MSG_LOOP;
	msg.szFileName = szFileNameTemp;
	msg.volume = volume;

	m_listMsg.lock();
	_listMsg.push_back(msg);//��������Ϣ������Ϣ����
	m_listMsg.unlock();
}

void Player::stopPlay(int hThread) {
	Player::_Sound* pSound = NULL;
	m_listPlay.lock();
	std::list<Player::_Sound*>::iterator it = _listPlay.begin();
	for (; it != _listPlay.end(); ++it) {
		if ((*(*it)) == hThread) {
			pSound = (*it);
			break;
		}
	}
	m_listPlay.unlock();
	if (pSound == NULL) {
		return;
	}

	pSound->stop();//֪ͨ�߳̽���
	WaitForSingleObject(pSound->hThread, step * 2);//�ȴ�����
	CloseHandle(pSound->hThread);
	pSound->hThread = NULL;

	//���б����Ƴ�
	_clearPlayList();

	_MyFree(pSound);//�ͷ���Դ
}
void Player::stopPlay(Player::_Sound* pSound) {
	if (pSound == NULL) {
		return;
	}
	Player::_MSG msg;
	msg.type = PLAYER_MSG_STOP;
	msg.pSound = pSound;

	m_listMsg.lock();
	_listMsg.push_back(msg);//��ֹͣ������Ϣ�������
	m_listMsg.unlock();
}
void Player::stopPlayAll() {
	Player::_MSG msg;
	msg.type = PLAYER_MSG_STOPALL;

	m_listMsg.lock();
	_listMsg.push_back(msg);//��ֹͣ���в�����Ϣ�������
	m_listMsg.unlock();
}

HANDLE Player::_hThread = NULL;

DWORD __stdcall Player::_Run(LPVOID lpParameter)
{
	bool isExit = false;
	while (!isExit) {
		m_listMsg.lock();
		if (_listMsg.empty()) {
			m_listMsg.unlock();
			continue;
		}
		//get message
		Player::_MSG msg = _listMsg[0];
		_listMsg.erase(_listMsg.begin());
		m_listMsg.unlock();

		switch (msg.type) {
		case PLAYER_MSG_EXIT: {//�˳���Ϣ
			m_listMsg.lock();
			_listMsg.clear();//�����Ϣ����
			m_listMsg.unlock();
			isExit = true;
			break;
		}
		case PLAYER_MSG_PLAY: {//������Ϣ
			Player::_Sound* pSound = _MyMalloc();//���ڱ���ôβ��ŵ������Ϣ
			pSound->id = time(NULL);
			pSound->szFileName = msg.szFileName;
			pSound->MciOpen.lpstrElementName = msg.szFileName;
			pSound->volume = msg.volume;
			pSound->isStop = false;
			pSound->hThread = CreateThread(NULL, 0, Player::_Play, (LPVOID)pSound->id, CREATE_SUSPENDED/*������������ִ��*/, NULL);//�����߳��첽����
			//std::cout << "pSound:"<<pSound<<" handle:"<<pSound->hThread << std::endl;
			if (pSound->hThread == NULL) {
				//GetLastError();
				break;//return -1;//��η���
			}

			m_listPlay.lock();
			_listPlay.push_back(pSound);//��������Ϣ�������
			m_listPlay.unlock();

			Sleep(timeout);//��Ӳ��������Ҫʱ��
			ResumeThread(pSound->hThread);//�ָ���������̣߳����俪ʼִ��
			//Sleep(timeout);//��Ӳ��������Ҫʱ��
			break;
		}
		case PLAYER_MSG_LOOP: {
			Player::_Sound* pSound = _MyMalloc();//���ڱ���ôβ��ŵ������Ϣ
			pSound->id = time(NULL);
			pSound->szFileName = msg.szFileName;
			pSound->MciOpen.lpstrElementName = msg.szFileName;
			pSound->volume = msg.volume;
			pSound->isStop = false;
			pSound->hThread = CreateThread(NULL, 0, Player::_PlayLoop, (LPVOID)pSound->id, CREATE_SUSPENDED/*������������ִ��*/, NULL);//�����߳��첽����
			//std::cout << "pSound:" << pSound << " handle:" << pSound->hThread << std::endl;
			if (pSound->hThread == NULL) {
				//GetLastError();
				break;//return -1;//��η���
			}

			m_listPlay.lock();
			_listPlay.push_back(pSound);//��������Ϣ�������
			m_listPlay.unlock();
			Sleep(timeout);//��Ӳ��������Ҫʱ��
			ResumeThread(pSound->hThread);//�ָ���������̣߳����俪ʼִ��
			//Sleep(timeout);//��Ӳ��������Ҫʱ��
			break;
		}
		case PLAYER_MSG_STOP: {
			msg.pSound->stop();//֪ͨ�߳̽���
			WaitForSingleObject(msg.pSound->hThread, step * 2);//�ȴ�����
			CloseHandle(msg.pSound->hThread);
			msg.pSound->hThread = NULL;

			//���б����Ƴ�
			_clearPlayList();//�������ͷ�pSound  ��Ӧ��_MyMalloc();��������ڴ�
			break;
		}
		case PLAYER_MSG_STOPALL: {
			Player::_Sound* pSound = NULL;

			m_listPlay.lock();
			std::list<Player::_Sound*>::iterator it = _listPlay.begin();
			for (; it != _listPlay.end(); ) {
				pSound = *it;
				std::list<Player::_Sound*>::iterator tmp;
				//erase֮��itָ���ʧЧ ������Ҫ����erase֮ǰ��ָ��
				if (it != _listPlay.begin()) {
					tmp = --it;
					++it;
					m_listPlay.unlock();

					pSound->stop();
					WaitForSingleObject(pSound->hThread, step * 200);//itʧЧ
					CloseHandle(pSound->hThread);

					pSound->hThread = NULL;
					_clearPlayList();//�������ͷ�����pSound->hThread==NULL����Դ  ��Ӧ��_MyMalloc();��������ڴ�


					m_listPlay.lock();
					it = tmp;
				}
				else {
					m_listPlay.unlock();

					pSound->stop();
					WaitForSingleObject(pSound->hThread, step * 200);//itʧЧ
					CloseHandle(pSound->hThread);
					//Sleep(1000);

					pSound->hThread = NULL;
					_clearPlayList();//�������ͷ�����pSound->hThread==NULL����Դ  ��Ӧ��_MyMalloc();��������ڴ�

					m_listPlay.lock();
					it = _listPlay.begin();
					continue;//it�Ѿ�ָ����һ����
				}
				++it;
			}
			m_listPlay.unlock();
			
			m_listMsg.lock();//ɾ��stop��Ϣ
			{
				std::vector<Player::_MSG>::iterator it = _listMsg.begin();
				Player::_MSG msg;
				for (; it != _listMsg.end();) {
					msg = (*it);
					//m_listMsg.unlock();
					if (msg.type == PLAYER_MSG_STOP) {
						
						//erase֮��itָ���ʧЧ ������Ҫ����erase֮ǰ��ָ��
						if (it != _listMsg.begin()) {
							std::vector<Player::_MSG>::iterator tmp = --it;
							++it;
							_listMsg.erase(it);
							it = tmp;
							++it;
						}
						else {
							_listMsg.erase(it);
							it = _listMsg.begin();//it�Ѿ�ָ�����¸�
						}
					}
					else {
						++it;
					}

					//m_listMsg.lock();
				}
			}
			m_listMsg.unlock();

			break;
		}
		default:
			break;
		}
	}
	return 0;
}

void Player::_StaticStructure()
{
	_hThread = CreateThread(NULL, 0, Player::_Run, NULL, 0, NULL);
}

void Player::_StaticDestruct() {
	stopPlayAll();//ֹͣ�������ڲ��ţ�������Ӧ�߳�

	Player::_MSG msg;
	msg.type = PLAYER_MSG_EXIT;

	m_listMsg.lock();
	//m_listMsg.try_lock();
	_listMsg.push_back(msg);//���˳���Ϣ�������
	m_listMsg.unlock();
	//m_listPlay.
	WaitForSingleObject(_hThread, step * 200);
	CloseHandle(_hThread);
}

DWORD WINAPI Player::_Play(LPVOID lpParameter) {
	Player::_Sound* pSound = NULL;
	//��������Ѱ�� ���ɵ�ǰ�߳���ά���Ĳ�����Ϣ

	m_listPlay.lock();
	std::list<Player::_Sound*>::iterator it = _listPlay.begin();
	for (; it != _listPlay.end(); ++it) {
		if ((*(*it)) == (time_t)lpParameter) {
			pSound = (*it);
			break;
		}
	}
	m_listPlay.unlock();

	if (pSound == NULL) {
		stopPlay(pSound);//�ͷ���Դ
		return -1;
	}

	//�򿪲���Ҫ������������ͬһ�߳�.
	DWORD ret = mciSendCommand(0, MCI_OPEN,
		MCI_OPEN_ELEMENT, (DWORD_PTR)&pSound->MciOpen);//��һ���豸 ׼�����Ÿ���Ч
	if (ret != 0) {
		_print(pSound, " MCI_OPEN ret:%d\n", ret);
		//std::cout <<pSound->szFileName << " MCI_OPEN ret:" << ret << std::endl;
		stopPlay(pSound);//�ͷ���Դ
		return ret;
	}
	//std::cout <<pSound->szFileName << " id:" <<pSound->MciOpen.wDeviceID << std::endl;

	ret = mciSendCommand(pSound->MciOpen.wDeviceID, MCI_PLAY,
		MCI_NOTIFY, (DWORD_PTR)(LPVOID)&pSound->MciPlay);//��ʼ����
	_print(pSound, " MCI_PLAY ret:%d\n", ret);
	//std::cout <<pSound->szFileName << " MCI_PLAY ret" <<ret << std::endl;

	pSound->MciStatus.dwItem = MCI_STATUS_LENGTH;//Ҫ��ȡ���� ��ȡ����
	ret = mciSendCommand(pSound->MciOpen.wDeviceID, MCI_STATUS,
		MCI_STATUS_ITEM, (DWORD_PTR)(LPVOID)&pSound->MciStatus);//��ȡ��Чʱ��  ��λ����
	pSound->nLength = pSound->MciStatus.dwReturn;
	_print(pSound, " MCI_STATUS ret:%d\n", ret);
	//std::cout <<pSound->szFileName << " MCI_STATUS ret" << ret << std::endl;
	_print(pSound, " Length:%d\n", pSound->nLength);
	//std::cout <<pSound->szFileName << " Length:" <<pSound->nLength << std::endl;


	pSound->MciSetAudio.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	pSound->MciSetAudio.dwValue = pSound->volume;
	ret = mciSendCommand(pSound->MciOpen.wDeviceID, MCI_SETAUDIO,
		MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD_PTR)&pSound->MciSetAudio);//��������
	//std::cout <<pSound->szFileName << " MCI_SET ret" << ret << std::endl;
	_print(pSound, " MCI_SET ret:%d\n", ret);

	for (int i = 0; (!pSound->isStop) && (i < pSound->nLength); i += step) {//�ȴ��������
		Sleep(step);
	}

	ret = mciSendCommand(pSound->MciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);//�ر��豸
	//std::cout <<pSound->szFileName << " MCI_CLOSE ret" << ret << std::endl;
	_print(pSound, " MCI_CLOSE ret:%d\n", ret);
	stopPlay(pSound);//�ͷ���Դ
	std::cout << "End" << std::endl;
	return 0L;
}

DWORD WINAPI Player::_PlayLoop(LPVOID lpParameter) {
	Player::_Sound* pSound = NULL;
	//��������Ѱ�� ���ɵ�ǰ�߳���ά���Ĳ�����Ϣ

	m_listPlay.lock();
	std::list<Player::_Sound*>::iterator it = _listPlay.begin();
	for (; it != _listPlay.end(); ++it) {
		if ((*(*it)) == (time_t)lpParameter) {
			pSound = (*it);
			break;
		}
	}
	m_listPlay.unlock();

	if (pSound == NULL) {
		stopPlay(pSound);//�ͷ���Դ
		return -1;
	}

	DWORD ret;
	//ѭ������
	while (!pSound->isStop) {
		ret = mciSendCommand(0, MCI_OPEN,
			MCI_OPEN_ELEMENT, (DWORD_PTR)(LPVOID)&pSound->MciOpen);//��һ���豸 ׼�����Ÿ���Ч
		if (ret != 0) {
			_print(pSound, " MCI_OPEN ret:%d\n", ret);
			//std::cout <<pSound->szFileName << " MCI_OPEN ret:" << ret << std::endl;
			stopPlay(pSound);//�ͷ���Դ
		}
		//std::cout <<pSound->szFileName << " id:" <<pSound->MciOpen.wDeviceID << std::endl;

		ret = mciSendCommand(pSound->MciOpen.wDeviceID, MCI_PLAY,
			MCI_NOTIFY, (DWORD_PTR)(LPVOID)&pSound->MciPlay);//��ʼ����
		_print(pSound, " MCI_PLAY ret:%d\n", ret);
		//std::cout <<pSound->szFileName << " MCI_PLAY ret" <<ret << std::endl;

		pSound->MciStatus.dwItem = MCI_STATUS_LENGTH;//Ҫ��ȡ���� ��ȡ����
		ret = mciSendCommand(pSound->MciOpen.wDeviceID, MCI_STATUS,
			MCI_STATUS_ITEM, (DWORD_PTR)(LPVOID)&pSound->MciStatus);//��ȡ��Чʱ��  ��λ����
		pSound->nLength = pSound->MciStatus.dwReturn;
		_print(pSound, " MCI_STATUS ret:%d\n", ret);
		//std::cout <<pSound->szFileName << " MCI_STATUS ret" << ret << std::endl;
		_print(pSound, " Length:%d\n", pSound->nLength);
		//std::cout <<pSound->szFileName << " Length:" <<pSound->nLength << std::endl;

		pSound->MciSetAudio.dwItem = MCI_DGV_SETAUDIO_VOLUME;
		pSound->MciSetAudio.dwValue = pSound->volume;
		ret = mciSendCommand(pSound->MciOpen.wDeviceID, MCI_SETAUDIO,
			MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD_PTR)&pSound->MciSetAudio);//��������
		//std::cout <<pSound->szFileName << " MCI_SET ret" << ret << std::endl;
		_print(pSound, " MCI_SET ret:%d\n", ret);

		for (int i = 0; (!pSound->isStop) && (i < pSound->nLength); i += step) {
			Sleep(step);
		}

		ret = mciSendCommand(pSound->MciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);//�ر��豸
		//std::cout <<pSound->szFileName << " MCI_CLOSE ret" << ret << std::endl;
		_print(pSound, " MCI_CLOSE ret:%d\n", ret);
	}
	stopPlay(pSound);
	return 0L;
}

void Player::_clearPlayList() {
	m_listPlay.lock();
	std::list<Player::_Sound*>::iterator it = _listPlay.begin();
	for (; it != _listPlay.end(); ) {
		if ((*it)->hThread == NULL) {
			std::list<Player::_Sound*>::iterator tmp ;
			//erase֮��itָ���ʧЧ ������Ҫ����erase֮ǰ��ָ��
			if (it != _listPlay.begin()) {
				tmp = --it;
				++it;

				_MyFree((*it));
				_listPlay.erase(it);
				it = tmp;
			}
			else {
				_MyFree((*it));
				_listPlay.erase(it);
				it= _listPlay.begin();
				continue;//it�Ѿ�ָ����һ����
			}
		}
		++it;
	}
	m_listPlay.unlock();
}

Player::_Sound* Player::_MyMalloc() {
	return new Player::_Sound;
}

void Player::_MyFree(Player::_Sound* ptr) {
	delete ptr;
}

void Player::_print(Player::_Sound* pSound, char const* const _Format, ...)
{
	char _Buffer[512] = { 0 };
	//const char header
	int _Result;
	va_list _ArgList;
	__crt_va_start(_ArgList, _Format);
	sprintf(_Buffer, "%s hThread:%x DevID:%d", pSound->szFileName, pSound->hThread, pSound->MciOpen.wDeviceID);
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


int drawSum = 0;
void AddDrawObject(Object* object)
{
	bool findFlag = false;
	for (int i = 0; i < drawSum; i++)
	{
		if (drawList[i] == object)
		{
			findFlag = true;
		}
	}
	if (!findFlag)
	{
		drawList[drawSum] = object;
		drawSum++;
	}
}

void RemoveDrawObecjt(Object* object)
{
	int findIndex = -1;
	for (int i = 0; i < drawSum; i++)
	{
		if (drawList[i] == object)
		{
			findIndex = i;
			break;
		}
	}
	if (findIndex != -1)
	{
		for (int i = findIndex; i < drawSum - 1; i++)
		{
			drawList[i] = drawList[i + 1];
		}
		drawSum--;
	}
}

inline void DrawPoint(int nPos, Color c) {
	if (!c.isTransparent()) {
		buffer[nPos++] = c.b;
		buffer[nPos++] = c.g;
		buffer[nPos++] = c.r;
	}
}

inline void DrawPoint(int nPos, BYTE r, BYTE g, BYTE b, BYTE a = 0) {
	if (!(r == 255 && g == 255 && b == 255)) {
		buffer[nPos++] = b;
		buffer[nPos++] = g;
		buffer[nPos++] = r;
	}
}

inline void DrawPoint(int x, int y, Color c) {
	if (!c.isTransparent()) {
		int nPos = (y * nScreenWidth + x) * 4;
		buffer[nPos++] = c.b;
		buffer[nPos++] = c.g;
		buffer[nPos++] = c.r;
	}
}

inline void DrawPoint(int x, int y, BYTE r, BYTE g, BYTE b) {
	if (!(r == 255 && g == 255 && b == 255)) {
		int nPos = (y * nScreenWidth + x) * 4;
		buffer[nPos++] = b;
		buffer[nPos++] = g;
		buffer[nPos++] = r;
	}
}
inline void DrawRect(int x, int y, int width, int height, BYTE r, BYTE g, BYTE b) {
	for (int i = 0; i < height; ++i) {
		int posBuffer = (nScreenWidth * (y + i) + x) * 4;//����ÿ�е����
		for (int j = 0; j < width; ++j) {
			DrawPoint(posBuffer, r, g, b);
			posBuffer += 4;
		}
	}
}

//uncheck
inline void DrawPic(int x, int y, Picture* pic, RECT rect) {
	int xBuffer = 0, yBuffer = 0;
	int xPic, yPic;

	for (yPic = rect.top; yPic < rect.bottom; ++yPic) {
		int posBuffer = (nScreenWidth * (y + yBuffer++) + x) * 4;
		for (xPic = rect.left; xPic < rect.right; ++xPic) {
			int posPic = pic->getWidth() * yPic + xPic;
			DrawPoint(posBuffer, pic->pChannelR[posPic], pic->pChannelG[posPic], pic->pChannelB[posPic]);
			posBuffer += 4;
			++posPic;
		}
	}
}
inline void DrawPic(int x, int y, Picture* pic) {
	RECT rect;
	int _x, _y;
	//������
	if ((y + pic->getHeight()) < 0) {
		//��ȫ�����ϱ߽�
		//������
		return;
	}
	else if (y < 0) {
		//���ֳ����ϱ߽�
		rect.top = -y;
		rect.bottom = pic->getHeight();
		//_x = x;
		_y = 0;
	}
	else if ((y + pic->getHeight()) < nScreenHeight) {
		//��ȫ�ڴ�����
		rect.top = 0;
		rect.bottom = pic->getHeight();
		//_x = x;
		_y = y;
	}
	else if (y < nScreenHeight) {
		//���ֳ����±߽�
		rect.top = 0;
		rect.bottom = nScreenHeight - y;
		//_x = x;
		_y = y;
	}
	else {
		//��ȫ�����±߽�
		//������
		return;
	}

	//������
	if ((x + pic->getWidth()) < 0) {
		//��ȫ������߽�
		//������
		return;
	}
	else if (x < 0) {
		//���ֳ�����߽�
		rect.left = -x;
		rect.right = pic->getWidth();
		_x = 0;
		//_y = y;
	}
	else if ((x + pic->getWidth()) < nScreenWidth) {
		//��ȫ�ڴ�����
		rect.left = 0;
		rect.right = pic->getWidth();
		_x = x;
		//_y = y;
	}
	else if (x < nScreenWidth) {
		//���ֳ����ұ߽�
		rect.left = 0;
		rect.right = nScreenWidth - x;
		_x = x;
		//_y = y;
	}
	else {
		//��ȫ�����±߽�
		//������
		return;
	}

	DrawPic(_x, _y, pic, rect);
}

//Description:
//	P����P0˳ʱ����תangle
//Paramter: 
//	POINT p		����ת��
//	POINT p0	���ĵ�
//	float angle	��ת�Ƕ�
//	POINT& p_	��ת���
//Return:
//	NONE
inline void spin(Point p, Point p0, float angle, Point& p_) {
	float sinAngle = sin(angle);
	float cosAngle = cos(angle);


	p_.x = p.x * cosAngle + p.y * sinAngle + p0.x * (1 - cosAngle) - p0.y * sinAngle;


	p_.y = -p.x * sinAngle + p.y * cosAngle + p0.y * (1 - cosAngle) + p0.x * sinAngle;
}

inline void DrawObject(Object* obj) {
	//���ڿ��� ��Ϊobject�ĳ�Ա����
	float angle = obj->getAngle(1);

	if (angle == 0)//Ӧ�ÿ���
	{
		//����Ҫ��ת
		DrawPic(obj->point.x, obj->point.y, obj->pic);
	}
	else {
		int nWidth = obj->pic->getWidth();
		int nHeight = obj->pic->getHeight();

		//����ͼ��
		for (int i = 0; i < nHeight; ++i) {
			for (int j = 0; j < nWidth; ++j) {
				//������ת������
				Point p_;

				//��������õ���p_���Ե�ǰ�������Ͻ�Ϊԭ�������ϵ ��Ҫӳ�䵽��������ϵ��
				spin({ j,i }, obj->pic->centerPoint, obj->getAngle(1), p_);

				//ӳ��
				p_ += obj->point;

				if (p_.x < 0 || p_.y < 0) {
					continue;//����ת��������ĵ�����
				}
				else if (p_.x > nScreenWidth || p_.y > nScreenHeight) {
					continue;//����ת��������ĵ�����
				}
				int posBuffer = (p_.y * nScreenWidth + p_.x) * 4;
				int posPic = i * nWidth + j;

				DrawPoint(posBuffer, obj->pic->pChannelR[posPic], obj->pic->pChannelG[posPic], obj->pic->pChannelB[posPic]);
				posBuffer += 4;
				++posPic;
			}
		}
	}

}

//Description:
//	�����ػ�����
//	������Ϣ
//Paramter: 
//	
//Return:
//	
void Draw() {
	void getMessage();

	DrawRect(0, 0, nScreenWidth, nScreenHeight, 192, 224, 0);

	/*
	Object obj;
	obj.point.x = 10;
	obj.point.y = 10;
	obj.z = 1;
	obj.setAngle(45);
	obj.pic = &picField;

	Object obj2;
	obj2.point.x = 30;
	obj2.point.y = 30;
	obj2.z = 2;
	obj2.setAngle(45);
	obj2.pic = &picField1;

	Object obj3;
	obj3.point.x = 50;
	obj3.point.y = 50;
	obj3.setAngle(45);
	obj3.z = 0;
	obj3.pic = &picField2;

	Object* _drawList[MaxDrawObjectSum];
	_drawList[0] = &obj;
	_drawList[1] = &obj2;
	_drawList[2] = &obj3;
	std::sort(*_drawList, *(_drawList + 2));
	for (int i = 0; i < 3; ++i) {
		DrawObject(_drawList[i]);
	}*/

	for (int i = 0; i < drawSum; i++)
	{
		//DebugPrint("��%d��objetc ��ʼ����\t",i);
		Object* drawNowObject = drawList[i];
		//DrawBmp(drawNowObject->x, drawNowObject->y, drawNowObject->pic);
		drawNowObject->updatePoint();
		DrawObject(drawNowObject);
		//DebugPrintln("�������");
	}

	HDC hDC = GetDC(hWnd);
	BitBlt(hDC, 0, 0, nScreenWidth, nScreenHeight, hMemDC, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hDC);
	getMessage();
}

void getMessage() {
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void freeSomethingForEngine() {
	if (hMemDC) {
		if (hBitmapDevice) {
			SelectObject(hMemDC, hBitmapDevice);
			hBitmapDevice = NULL;
		}
		DeleteDC(hMemDC);
		hMemDC = NULL;
	}
	if (hBitmapBuffer) {
		DeleteObject(hBitmapBuffer);
		hBitmapBuffer = NULL;
	}
	if (hWnd) {
		CloseWindow(hWnd);
		hWnd = NULL;
	}

	Player::_StaticDestruct();
}
//������Ϣ
static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CLOSE: {
		//DebugPrintln("%d\t%d", wParam, wParam & 511);
		break;
	}
	case WM_KEYDOWN: {
		//DebugPrintln("%d\t%d", wParam, wParam & 511);
		screen_keys[wParam & 511] = 1;
		break;
	}
	case WM_KEYUP: {
		screen_keys[wParam & 511] = 0;
		break;
	}
	default: {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
}

//Description:
//	������ά��һ������,Ŀǰ���Բ����Ϸ��Խ��м��.
//	�ô�����ʾ������������window�ṹ������ָ���img
//Paramter: 
//	LPVOID windowһ��ָ��Window�ṹ���ָ��
//Return:
//	0:		����
//	other:	��GetLastError()���صĴ������
int _CreateWindow(const char* title, int nWidth, int nHeight) {
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_BYTEALIGNCLIENT;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.hInstance = hInstance;//GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = (char*)title;
	wc.lpszMenuName = (char*)title;

	RegisterClass(&wc);

	//Create a new window
	hWnd = CreateWindow(
		(char*)title,//class name
		(char*)title,//title
		WS_OVERLAPPEDWINDOW,//style
		CW_USEDEFAULT, CW_USEDEFAULT,//position
		nWidth, nHeight,//size
		NULL,//parent window
		NULL,//menu
		hInstance,//application instance
		NULL//prarameters
	);
	if (hWnd == 0) {
		return GetLastError();
	}

	//�����ڴ滺�廭��
	HDC hDC = GetDC(hWnd);
	hMemDC = CreateCompatibleDC(hDC);//�����봰�ڼ��ݵ��ڴ��豸�����Ļ���(�����ڴ滺�廭��)
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), nWidth, -nHeight, 1, 32, BI_RGB,
		nWidth * nHeight * 4, 0, 0, 0, 0 } };
	LPVOID ptr;
	hBitmapBuffer = CreateDIBSection(hMemDC, &bi, DIB_RGB_COLORS, &ptr, 0, 0);//��������������
	hBitmapDevice = (HBITMAP)SelectObject(hMemDC, hBitmapBuffer);//ѡ�񻺳������������������壬������ԭʼ����
	buffer = (BYTE*)ptr;
	ReleaseDC(hWnd, hDC);

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	return 0;
}

