#include "miniTown.h"

const char* WindowTitle = "miniTown";
int nScreenWidth = SCREEN_WIDTH;
int nScreenHeight = SCREEN_HEIGHT;
HWND hWnd = NULL;
HDC hMemDC = NULL;
HBITMAP hBitmapBuffer = NULL;
HBITMAP hBitmapDevice = NULL;
BYTE* buffer = NULL;
BYTE screen_keys[512] = { 0 };
Player player;

//Object* drawList[MaxDrawObjectSum];
//���ƶ���
std::vector<ObjectPointer> drawList;
std::vector<Button*> listButton;
std::vector<Label*> listLabel;
std::vector<EditBox*> listEditBox;

std::list<Player::_Sound*> _listPlay;
std::vector<Player::_MSG> _listMsg;
const int timeout = 1000;//���ż�� ����Ƶ���������Ҫʱ����
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
			WaitForSingleObject(msg.pSound->hThread, step * 2000);//�ȴ�����
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
					WaitForSingleObject(pSound->hThread, step * 2000);//itʧЧ
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
	WaitForSingleObject(_hThread, step * 2000);
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
			std::list<Player::_Sound*>::iterator tmp;
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
				it = _listPlay.begin();
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

//Description:
//	����ƶ����м���һ���¶���,����ö����ѱ����룬�򲻻��ظ�����.
//Paramter: 
//	Object* object ��Ҫ�������ĵ�ַ
//Return Value:
//	NONE
void AddDrawObject(Object* object)
{
	ObjectPointer op(object);
	bool isExist = false;//�ö����Ƿ��Ѵ���
	for (std::vector<ObjectPointer>::iterator it = drawList.begin();
		it != drawList.end(); ++it) {
		if ((*it) == op) {
			isExist = true;
			break;
		}
	}
	if (!isExist) {
		//��������Ҫ���
		drawList.push_back(op);
	}
}

//Description:
//	�ӻ��ƶ�����ɾ��һ������
//Paramter: 
//	Object* object ��Ҫɾ���Ķ����ַ
//Return Value:
//	NONE
void RemoveDrawObecjt(Object* object)
{
	ObjectPointer op(object);
	for (std::vector<ObjectPointer>::iterator it = drawList.begin();
		it != drawList.end(); ++it) {
		if ((*it) == op) {
			drawList.erase(it);
			break;
		}
	}
}


//base
inline void DrawPoint(int nPos, BYTE r, BYTE g, BYTE b, BYTE a = 0) {

	float rate = (float)a / 255;
	float rate_ = 1 - rate;
	byte oldR, oldG, oldB;

	oldB = buffer[nPos];
	oldG = buffer[nPos + 1];
	oldR = buffer[nPos + 2];


	byte Nowb, Nowr, Nowg;
	Nowb = (oldB * rate) + b * rate_;
	Nowg = (oldG * rate) + g * rate_;
	Nowr = (oldR * rate) + r * rate_;
	
	
	//Nowb = ((buffer[nPos] * a) >> 8) + ((b * (255 - a)) >> 8);
	//Nowg= ((buffer[nPos+1] * a) >> 8) + ((g * (255 - a)) >> 8);
	//Nowr = ((buffer[nPos+2] * a) >> 8) + ((r * (255 - a)) >> 8);
	buffer[nPos] = Nowb;
	buffer[nPos + 1] = Nowg;
	buffer[nPos + 2] = Nowr;
}

inline void DrawPoint(int nPos, Color c) {
	DrawPoint(nPos, c.r, c.g, c.b, c.a);
}

inline void DrawPoint(int x, int y, BYTE r, BYTE g, BYTE b, BYTE a = 0) {
	if (x > nScreenWidth || x<0 || y>nScreenHeight || y < 0) {
		return;
	}
	int nPos = (y * nScreenWidth + x) * 4;
	DrawPoint(nPos, r, g, b, a);
}

inline void DrawPoint(int x, int y, Color c) {
	DrawPoint(x,y, c.r, c.g, c.b, c.a);
}

//base
inline void DrawLine(int x1, int y1, int x2, int y2, BYTE r, BYTE g, BYTE b, BYTE a = 0) {
	int x, y, dx, dy;
	float k, e;
	if (x1 > x2) {
		x = x1; x1 = x2; x2 = x;
		y = y1; y1 = y2; y2 = y;
	}
	if (x1 == x2) {
		if (y1 > y2) {
			y = y1; y1 = y2; y2 = y;
		}
		x = x1;
		for (y = y1; y < y2; ++y) {
			DrawPoint(x, y, r, g, b, a);
		}
		return ;
	}
	dx = x2 - x1;
	dy = y2 - y1;
	k = dy / dx;
	
	x = x1;
	y = y1;
	int driect = signbit(k) ? -1 : 1;
	e = -driect*0.5;
	for (int i = 0; i < dx; ++i) {
		DrawPoint(x, y, r, g, b);
		x += 1;
		e += driect * k;
		
		if (e >= 0) {
			y += driect * 1;
			e -= 1;
		}
		
	}
}

inline void DrawLine(Point p1, Point p2, BYTE r, BYTE g, BYTE b, BYTE a = 0) {
	DrawLine(p1.x, p1.y, p2.x, p2.y, r, g, b,a);
}

inline void DrawLine(Point p1, Point p2, Color color) {
	DrawLine(p1.x, p1.y, p2.x, p2.y, color.r, color.g, color.b, color.a);
}

//base
inline void DrawRect(int x, int y, int width, int height, BYTE r, BYTE g, BYTE b, BYTE a = 0) {
	for (int i = 0; i < height; ++i) {
		int posBuffer = (nScreenWidth * (y + i) + x) * 4;//����ÿ�е����
		for (int j = 0; j < width; ++j) {
			DrawPoint(posBuffer, r, g, b);
			posBuffer += 4;
		}
	}
}

inline void DrawRect(RECT rect, Color color) {
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	DrawRect(rect.left, rect.top, width, height, color.r, color.g, color.b, color.a);
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

//uncheck
inline void DrawPicA(int x, int y, Picture* pic, RECT rect) {
	int xBuffer = 0, yBuffer = 0;
	int xPic, yPic;

	for (yPic = rect.top; yPic < rect.bottom; ++yPic) {
		int posBuffer = (nScreenWidth * (y + yBuffer++) + x) * 4;
		for (xPic = rect.left; xPic < rect.right; ++xPic) {
			int posPic = pic->getWidth() * yPic + xPic;

			DrawPoint(posBuffer, pic->pChannelR[posPic], pic->pChannelG[posPic], pic->pChannelB[posPic], pic->pChannelA[posPic]);
			
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
	if (pic->isIncludeAlpha()) {
		DrawPicA(_x, _y, pic, rect);
	}
	else {
		DrawPic(_x, _y, pic, rect);
	}
	
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

	float tempX = p.x * cosAngle + p.y * sinAngle + p0.x * (1 - cosAngle) - p0.y * sinAngle;
	p_.x = tempX + 1;

	float tempY = -p.x * sinAngle + p.y * cosAngle + p0.y * (1 - cosAngle) + p0.x * sinAngle;
	p_.y = tempY + 1;
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


		int count = 0;
		int size = nWidth * nHeight;

		float co = cos(angle);
		float si = sin(angle);
		int rotateW, rotateH; //��ת��Ŀ��

		int xMin, xMax, yMin, yMax;
		float xSrc = 0;
		float ySrc = 0;  //�任��ͼ���������ԭͼ�е�����

		int xOff = nWidth / 2; //x���ĵ�ƫ����
		int yOff = nHeight / 2; //y���ĵ�ƫ����

		int nchannel = 3;  //��ɫͨ��


		float a1, a2, a3, a4;


		//��ת������귶Χ
		rotateH = nWidth * fabs(si) + nHeight * fabs(co);
		rotateW = nWidth * fabs(co) + nHeight * fabs(si);

		yMin = (nHeight - rotateH) / 2;
		yMax = yMin + rotateH + 1;
		xMin = (nWidth - rotateW) / 2;
		xMax = xMin + rotateW + 1;

		int x, y;
		for (y = yMin; y <= yMax; y++)
		{
			for (x = xMin; x < xMax; x++)
			{
				//����ԭͼ�е�����
				ySrc = si * float(x - xOff) + co * float(y - yOff) + float(int(nHeight / 2));
				xSrc = co * float(x - xOff) - si * float(y - yOff) + float(int(nWidth / 2));

				//�����ԭͼ��Χ��
				if (ySrc >= 0. && ySrc < nHeight - 0.5 && xSrc >= 0. && xSrc < nWidth - 0.5)
				{
					//��ʼ��ֵ
					int xSmall = floor(xSrc);
					int xBig = ceil(xSrc);
					int ySmall = floor(ySrc);
					int yBig = ceil(ySrc);

					float ux = xSrc - xSmall;
					float uy = ySrc - ySmall;
					float valueTempR;
					float valueTempG;
					float valueTempB;
					float valueTempA;

					Point DrawNowPoint = Point(x, y);
					DrawNowPoint += obj->point;
					int DrawNowPointMemLocation = (DrawNowPoint.y * nScreenWidth + DrawNowPoint.x) * 4;

					a1 = (xSmall >= 0 && ySmall >= 0 ? obj->pic->pChannelR[ySmall * nWidth + xSmall] : 0);
					a2 = (xBig < nWidth&& ySmall >= 0 ? obj->pic->pChannelR[ySmall * nWidth + xBig] : 0);
					a3 = (xSmall >= 0 && yBig < nHeight ? obj->pic->pChannelR[yBig * nWidth + xSmall] : 0);
					a4 = (xBig < nWidth&& yBig < nHeight ? obj->pic->pChannelR[yBig * nWidth + xBig] : 0);

					valueTempR = (1 - ux) * (1 - uy) * a1 + (1 - ux) * uy * a3 + (1 - uy) * ux * a2 + ux * uy * a4;

					a1 = (xSmall >= 0 && ySmall >= 0 ? obj->pic->pChannelG[ySmall * nWidth + xSmall] : 0);
					a2 = (xBig < nWidth&& ySmall >= 0 ? obj->pic->pChannelG[ySmall * nWidth + xBig] : 0);
					a3 = (xSmall >= 0 && yBig < nHeight ? obj->pic->pChannelG[yBig * nWidth + xSmall] : 0);
					a4 = (xBig < nWidth&& yBig < nHeight ? obj->pic->pChannelG[yBig * nWidth + xBig] : 0);

					valueTempG = (1 - ux) * (1 - uy) * a1 + (1 - ux) * uy * a3 + (1 - uy) * ux * a2 + ux * uy * a4+1; //��������

					a1 = (xSmall >= 0 && ySmall >= 0 ? obj->pic->pChannelB[ySmall * nWidth + xSmall] : 0);
					a2 = (xBig < nWidth&& ySmall >= 0 ? obj->pic->pChannelB[ySmall * nWidth + xBig] : 0);
					a3 = (xSmall >= 0 && yBig < nHeight ? obj->pic->pChannelB[yBig * nWidth + xSmall] : 0);
					a4 = (xBig < nWidth&& yBig < nHeight ? obj->pic->pChannelB[yBig * nWidth + xBig] : 0);

					valueTempB = (1 - ux) * (1 - uy) * a1 + (1 - ux) * uy * a3 + (1 - uy) * ux * a2 + ux * uy * a4+1; //��������

					a1 = (xSmall >= 0 && ySmall >= 0 ? obj->pic->pChannelA[ySmall * nWidth + xSmall] : 0);
					a2 = (xBig < nWidth && ySmall >= 0 ? obj->pic->pChannelA[ySmall * nWidth + xBig] : 0);
					a3 = (xSmall >= 0 && yBig < nHeight ? obj->pic->pChannelA[yBig * nWidth + xSmall] : 0);
					a4 = (xBig < nWidth && yBig < nHeight ? obj->pic->pChannelA[yBig * nWidth + xBig] : 0);

					if (a1 == 255 || a2 == 255 || a3 == 255 || a4 == 255)
					{
						valueTempA = 255;
					}
					else
					{
						valueTempA = (1 - ux) * (1 - uy) * a1 + (1 - ux) * uy * a3 + (1 - uy) * ux * a2 + ux * uy * a4; //��������

					}

					Color c = Color(valueTempR, valueTempG, valueTempB,valueTempA);
					//std::cout << valueTempA << std::endl;

					//c = Color(50, 50, 50);
					//std::cout << "DrawPos x" << DrawNowPoint.x << " y " << DrawNowPoint.y << std::endl;
					DrawPoint(DrawNowPointMemLocation, c);
				}
			}
		}
		
		count = size;

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

	//DrawLine(300, 300, 0, 0, 255, 0, 0);
	//DrawLine(300, 300, 600, 0, 255, 0, 0);
	//DrawLine(300,300, 600, 600, 255, 0, 0);
	//DrawLine(300, 300, 0, 600, 255, 0, 0);

	/*
	Object obj;
	obj.x = 50;
	obj.y = 50;
	obj.z = 1;
	//obj.setAngle(20);
	obj.pic = &picTest;
	drawList.clear();
	AddDrawObject(&obj);
	*/
	/*
	Object obj;
	obj.point.x = 10;
	obj.point.y = 10;
	obj.z = 1;
	//obj.setAngle(20);
	obj.pic = &picField;

	Object obj2;
	obj2.point.x = 10;
	obj2.point.y = 20;
	obj2.z = 2;
	obj2.setAngle(120);
	obj2.pic = &picLove;

	Object obj3;
	obj3.point.x = 10;
	obj3.point.y = 20;
	//obj3.setAngle(-60);
	obj3.z = 1;
	obj3.pic = &picField2;

	drawList.clear();
	AddDrawObject(&obj);
	AddDrawObject(&obj2);
	AddDrawObject(&obj3);
	std::sort(drawList.begin(), drawList.end());
	for (int i = 0; i < drawList.size(); ++i) {
		DrawObject(drawList[i].pObject);
	}*/
	std::sort(drawList.begin(), drawList.end());
	for (int i = 0; i < drawList.size(); ++i) {
		drawList[i].pObject->updatePoint();
		DrawObject(drawList[i].pObject);
	}

	int nOldBkMode = SetBkMode(hMemDC, TRANSPARENT);//��������ʱ����͸��
	//����Button
	{
		for (int i = 0; i < listButton.size(); ++i) {
			//EDGE_ETCHED
			DrawRect(listButton[i]->rect, listButton[i]->currentBackgroundColor);//���Ʊ���
			DrawEdge(hMemDC, &listButton[i]->rect, EDGE_ETCHED, BF_RECT);//��ͳ��ť���
			COLORREF nOldTextColor = SetTextColor(hMemDC, listButton[i]->getForegroundColor().getColorRef());
			DrawText(hMemDC, listButton[i]->title, -1, &listButton[i]->rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			SetTextColor(hMemDC, nOldTextColor);
		}

	}

	//����Label
	{
		for (int i = 0; i < listLabel.size(); ++i) {
			DrawRect(listLabel[i]->rect, listLabel[i]->currentBackgroundColor);
			COLORREF nOldTextColor = SetTextColor(hMemDC, listLabel[i]->getForegroundColor().getColorRef());
			//DrawTextEx(hMemDC, listLabel[i]->title, -1, &listLabel[i]->rect, DT_CENTER, NULL);
			DrawText(hMemDC, listLabel[i]->title, -1, &listLabel[i]->rect, DT_CENTER | DT_EXPANDTABS );
			SetTextColor(hMemDC, nOldTextColor);
		}
	}

	//����EditBox
	{
		for (int i = 0; i < listEditBox.size(); ++i) {
			DrawRect(listEditBox[i]->rect, listEditBox[i]->currentBackgroundColor);
			DrawEdge(hMemDC, &listEditBox[i]->rect, EDGE_SUNKEN, BF_RECT);//��ͳ�༭����

			COLORREF nOldTextColor = SetTextColor(hMemDC, listEditBox[i]->getForegroundColor().getColorRef());
			DrawText(hMemDC, listEditBox[i]->text.c_str(), -1, &listEditBox[i]->rect, DT_VCENTER | DT_SINGLELINE);
			SetTextColor(hMemDC, nOldTextColor);

			//�ж��Ƿ���Ҫ���ƹ��
			if (listEditBox[i]->isShowCaret) {
				if ((listEditBox[i]->sumFPS++) > (frame * 5)) {
					listEditBox[i]->stateCaret = !listEditBox[i]->stateCaret;
				}
				//�����˸
				if (listEditBox[i]->stateCaret) {
					//��ʾ
					
					DrawLine(listEditBox[i]->pointCaret.x, listEditBox[i]->pointCaret.y- listEditBox[i]->getHeight(),
						listEditBox[i]->pointCaret.x, listEditBox[i]->pointCaret.y,
						0, 0, 0);
				}
				else {
					//
				}
			}
		}
	}

	SetBkMode(hMemDC, nOldBkMode);//�ָ�ģʽ

	//���ڴ��豸����������
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
	//DestroyCaret();//���ٹ��
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


//UI Button

//Description:
//	����һ����ť����,����ö����ѱ����룬�򲻻��ظ�����.
//Paramter: 
//	Button * button ��Ҫ�������ĵ�ַ
//Return Value:
//	NONE
void AddButton(Button* button) {
	bool isExist = false;
	for (int i = 0; i < listButton.size(); ++i) {
		if (listButton[i] == button) {
			isExist = true;
			break;
		}
	}
	if (!isExist) {
		listButton.push_back(button);
	}
}

//Description:
//	�Ӱ�ť�����б���ɾ��һ������
//Paramter: 
//	Button * button ��Ҫɾ���Ķ����ַ
//Return Value:
//	NONE
void RemoveButton(Button* button)
{
	std::vector<Button*>::iterator it;
	for (it = listButton.begin(); it != listButton.end(); ++it) {
		if ((*it) == button) {
			listButton.erase(it);
			break;
		}
	}
}

//Description:
//	����һ����ǩ����,����ö����ѱ����룬�򲻻��ظ�����.
//Paramter: 
//	Label* label ��Ҫ�������ĵ�ַ
//Return Value:
//	NONE
void AddLabel(Label* label)
{
	bool isExist = false;
	for (int i = 0; i < listLabel.size(); ++i) {
		if (listLabel[i] == label) {
			isExist = true;
			break;
		}
	}
	if (!isExist) {
		listLabel.push_back(label);
	}
}

//Description:
//	�ӱ�ǩ�����б���ɾ��һ������
//Paramter: 
//	Label* label ��Ҫɾ���Ķ����ַ
//Return Value:
//	NONE
void RemoveLabel(Label* label)
{
	std::vector<Label*>::iterator it;
	for (it = listLabel.begin(); it != listLabel.end(); ++it) {
		if ((*it) == label) {
			listLabel.erase(it);
			break;
		}
	}
}

//Description:
//	����һ���༭�����,����ö����ѱ����룬�򲻻��ظ�����.
//Paramter: 
//	Label* label ��Ҫ�������ĵ�ַ
//Return Value:
//	NONE
void AddEditBox(EditBox* editBox)
{
	bool isExist = false;
	for (int i = 0; i < listEditBox.size(); ++i) {
		if (listEditBox[i] == editBox) {
			isExist = true;
			break;
		}
	}
	if (!isExist) {
		listEditBox.push_back(editBox);
	}
}

//Description:
//	�ӱ༭������б���ɾ��һ������
//Paramter: 
//	EditBox* editBox ��Ҫɾ���Ķ����ַ
//Return Value:
//	NONE
void RemoveEditBox(EditBox* editBox)
{
	std::vector<EditBox*>::iterator it;
	for (it = listEditBox.begin(); it != listEditBox.end(); ++it) {
		if ((*it) == editBox) {
			listEditBox.erase(it);
			break;
		}
	}
}



//������Ϣ
static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CLOSE: {
		//DebugPrintln("%d\t%d", wParam, wParam & 511);
		break;
	}
	case WM_CHAR: {
		Point p(lParam & 0x0000ffff, lParam >> 16);

		TCHAR code = wParam;

		//����EditBox
		for (int i = 0; i < listEditBox.size(); ++i) {
			if ((*listEditBox[i]) == p) {//Ѱ��������ĵ����ڵ�EditBox
				listEditBox[i]->text.insert(listEditBox[i]->nPosCaret, 1, (char)code);
				listEditBox[i]->moveCaret(1);

				break;
			}
		}
		break;
	}
	case WM_KEYDOWN: {
		//����EditBox
		for (int i = 0; i < listEditBox.size(); ++i) {
			if (listEditBox[i]->isShowCaret) {//Ѱ��������ĵ����ڵ�EditBox
				return 0;//������Ӧ�ȼ���Ϣ
			}
		}

		screen_keys[wParam & 511] = 1;
		break;
	}
	case WM_KEYUP: {
		//����EditBox
		for (int i = 0; i < listEditBox.size(); ++i) {
			if (listEditBox[i]->isShowCaret) {//Ѱ��������ĵ����ڵ�EditBox
				switch (wParam)
				{
				case VK_LEFT: {
					listEditBox[i]->moveCaret(-1);
					break;
				}
				case VK_RIGHT: {
					listEditBox[i]->moveCaret(1);
					break;
				}
				case VK_DELETE: {
					if (listEditBox[i]->text.length() == 0) {
						break;
					}
					if (listEditBox[i]->nPosCaret < listEditBox[i]->text.length()) {
						listEditBox[i]->text.erase(listEditBox[i]->nPosCaret, 1);
					}
					break;
				}
				case VK_BACK: {
					if (listEditBox[i]->text.length() == 0) {
						break;
					}
					if (listEditBox[i]->nPosCaret!=0) {
						listEditBox[i]->text.erase(--listEditBox[i]->nPosCaret, 1);
					}
					break;
				}
				default:
					switch (listEditBox[i]->getInputType())
					{
					case EB_IT_TEXT: {
						//�ı�����
						if (wParam <= 'Z' && wParam >= 'A') {
							listEditBox[i]->text.insert(listEditBox[i]->nPosCaret, 1, (char)wParam);
							listEditBox[i]->moveCaret(1);
							if (listEditBox[i]->lpValueChange != NULL) {
								listEditBox[i]->lpValueChange();
							}
						}
					}
					case EB_IT_NUMBER: {
						//��������
						if (wParam <= '9' && wParam >= '0') {
							listEditBox[i]->text.insert(listEditBox[i]->nPosCaret, 1, (char)wParam);
							listEditBox[i]->moveCaret(1);
							if (listEditBox[i]->lpValueChange != NULL) {
								listEditBox[i]->lpValueChange();
							}
						}
					}
					default:
						break;
					}
					
				}
				//break;//������Ӧ�ȼ���Ϣ


				HDC hDC = GetDC(hWnd);
				int strLength = listEditBox[i]->text.length();
				SIZE size = { 0 };
				int nPosStr = 0;
				for (nPosStr = 0; nPosStr <= strLength; ++nPosStr) {
					GetTextExtentPoint(hDC, listEditBox[i]->text.c_str(), nPosStr, &size);//���ص�ǰ�ַ�����ʼλ��
					if (listEditBox[i]->nPosCaret <= nPosStr) {
						break;
					}
				}

				size.cy = listEditBox[i]->getHeight();
				if (nPosStr == 0) {
					size.cx = 0;
				}

				Point newPoint = { size.cx  ,size.cy };
				newPoint + listEditBox[i]->getRect();
				listEditBox[i]->pointCaret = newPoint;//���ù��λ��
				//listEditBox[i]->isShowCaret = true;//��ʾ���

				ReleaseDC(hWnd, hDC);
				return 0;//������Ӧ�ȼ���Ϣ
			}
		}
		
		screen_keys[wParam & 511] = 0;
		break;
	}
	case WM_MOUSEMOVE: {
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_HOVER;
		tme.dwHoverTime = 1;
		tme.hwndTrack = hWnd;
		TrackMouseEvent(&tme);//ע�������ͣ��Ϣ

		//tme.dwFlags = TME_LEAVE;
		//TrackMouseEvent(&tme);//ע������뿪��Ϣ  ����ûʲô��
		//DebugPrintln("WM_MOUSEMOVE");
		break;
	}
	case WM_MOUSEHOVER: {
		//�ж��Ƿ���ROI
		Point p(lParam & 0x0000ffff, lParam >> 16);

		for (int i = 0; i < listButton.size(); ++i) {
			if ((*listButton[i]) == p) {//Ѱ�����ͣ�������ڵİ�ť
				listButton[i]->currentBackgroundColor = DEFAULT_FOCUS_BC;//��ͣ�ڰ�ť��ʱ  �޸ı�����ɫ
			}
			else {
				listButton[i]->currentBackgroundColor = listButton[i]->getBackgroundColor();//�뿪ʱ�ָ�
			}
		}
		//DebugPrintln("WM_MOUSEHOVER %d %d ",p.x,p.y);
		break;
	}
	case WM_NCMOUSEHOVER: {
		DebugPrintln("WM_NCMOUSEHOVER");
		break;
	}
	case WM_LBUTTONDOWN: {
		//DebugPrintln("WM_LBUTTONDOWN");
		//����  ��lparam ��
		//��16λ��y ��16λ��x
		//DebugPrintln("%d %d", lParam >> 16, lParam & 0x0000ffff);
		Point p(lParam & 0x0000ffff, lParam >> 16);

		//����Button
		for (int i = 0; i < listButton.size(); ++i) {
			if ((*listButton[i]) == p) {//Ѱ��������ĵ����ڵİ�ť
				listButton[i]->isClick = true;
				listButton[i]->currentBackgroundColor = DEFAULT_DOWN_BC;
				//DebugPrintln("WM_LBUTTONDOWN");
				//break;
				return 0;
			}
		}

		//����EditBox
		for (int i = 0; i < listEditBox.size(); ++i) {
			if ((*listEditBox[i]) == p) {//Ѱ��������ĵ����ڵ�EditBox
				//������� �ñ༭�� ������λ��
				Point pointForRect = p;
				pointForRect - listEditBox[i]->rect;//�����RECT������

				HDC hDC = GetDC(hWnd);
				int strLength = listEditBox[i]->text.length();
				SIZE size = { 0 };
				int nPosStr = 0;
				for (nPosStr = 0; nPosStr < strLength; ++nPosStr) {
					GetTextExtentPoint(hDC, listEditBox[i]->text.c_str(), nPosStr, &size);//���ص�ǰ�ַ�����ʼλ��
					if (pointForRect.x < size.cx) {
						++nPosStr;
						break;
					}
				}

				size.cy = listEditBox[i]->getHeight();
				if (nPosStr == 0) {
					size.cx = 0;
					
				}
				else {
					SIZE size2 = { 0 };
					GetTextExtentPoint(hDC, listEditBox[i]->text.c_str(), nPosStr, &size2);
					//�ж������λ�þ����ĸ��ַ�����
					if ((pointForRect.x - size.cx) < (abs(size2.cx - pointForRect.x))) {
						//ǰ��
						listEditBox[i]->nPosCaret = nPosStr-1;
					}
					else {
						//������߸���
						size.cx = size2.cx;
						listEditBox[i]->nPosCaret = nPosStr ;
					}
				}
				pointForRect.x = size.cx;
				pointForRect.y = size.cy;
				pointForRect + listEditBox[i]->rect;//����ڵ�����
				
				
				listEditBox[i]->pointCaret = pointForRect;//���ù��λ��
				listEditBox[i]->isShowCaret = true;//��ʾ���

				ReleaseDC(hWnd, hDC);
			}
			else {
				listEditBox[i]->isShowCaret = false;//���ع��
			}
		}
		break;
	}
	case WM_LBUTTONUP: {
		//DebugPrintln("WM_LBUTTONUP");
		//����  ��lparam ��
		//��16λ��y ��16λ��x
		//DebugPrintln("%d %d", lParam >> 16, lParam & 0x0000ffff);
		Point p(lParam & 0x0000ffff, lParam >> 16);

		for (int i = 0; i < listButton.size(); ++i) {
			if ((*listButton[i]) == p) {//Ѱ�ҵ�ǰ������Ӧ��button
				if (listButton[i]->isClick) {//�ж� down �� up ��ͬһ��button
					if (listButton[i]->lpClickL != NULL) {//�жϸ��¼��Ƿ񱻶���
						//DebugPrintln("WM_LBUTTONUP");
						listButton[i]->lpClickL();//���øð�ť������ĺ���
					}
					listButton[i]->isClick = false;
					listButton[i]->currentBackgroundColor = listButton[i]->getBackgroundColor();//����֮��ָ�
					break;
				}
				break;//up ��Ϣ�� down ����ͬһ��button
			}
		}
		break;
	}
	
	default: {
		//DebugPrintln("%x", message);
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
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), nWidth, -nHeight, 1, 32, BI_RGB,//BI_BITFIELDS �޷�ʹ��
		nWidth * nHeight * 4, 0, 0, 0, 0 } };
	LPVOID ptr;
	hBitmapBuffer = CreateDIBSection(hMemDC, &bi, DIB_RGB_COLORS, &ptr, 0, 0);//��������������
	DebugPrintln("%d",GetLastError());

	hBitmapDevice = (HBITMAP)SelectObject(hMemDC, hBitmapBuffer);//ѡ�񻺳������������������壬������ԭʼ����
	buffer = (BYTE*)ptr;
	ReleaseDC(hWnd, hDC);

	//CreateCaret(hWnd, NULL, 1, 10);

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	return 0;
}

