#include "miniTown.h"

const char* title = "Title_";
const int nScreenWidth = 600;
const int nScreenHeight = 600;
HWND hWnd = NULL;
HDC hMemDC = NULL;
HBITMAP hBitmapBuffer = NULL;
HBITMAP hBitmapDevice = NULL;
BYTE* buffer = NULL;
BYTE screen_keys[512] = { 0 };

Object* drawList[MaxDrawObjectSum];

//std::vector<Player::_Sound> _listPlay;
std::list<Player::_Sound> _listPlay;
const int timeout = 300;//���ż�� ����Ƶ���������Ҫʱ����
Player::Player() {
	;
}

Player::~Player() {
	Player::stopPlayAll();
}

int Player::playSound(const char* szFileNameTemp, unsigned short volume) {
	Player::_Sound sound;//���ڱ���ôβ��ŵ������Ϣ
	sound.szFileName = (char*)szFileNameTemp;
	sound.MciOpen.lpstrElementName = szFileNameTemp;
	sound.volume = volume;
	sound.isStop = false;

	sound.hThread = CreateThread(NULL, 0, Player::_Play, (LPVOID)szFileNameTemp, 0, NULL);//�����߳��첽����
	if (sound.hThread == NULL) {
		//GetLastError();
		return -1;
	}
	_listPlay.push_back(sound);//������Ϣ
	CloseHandle(sound.hThread);
	Sleep(timeout);
	return (int)sound.hThread;//����һ����ǿת���߳̾��
}

int Player::playSound(char* szFileNameTemp, unsigned short volume) {
	Player::_Sound sound;//���ڱ���ôβ��ŵ������Ϣ
	sound.szFileName = szFileNameTemp;
	sound.MciOpen.lpstrElementName = szFileNameTemp;
	sound.volume = volume;
	sound.isStop = false;

	sound.hThread = CreateThread(NULL, 0, Player::_Play, (LPVOID)szFileNameTemp, 0, NULL);//�����߳��첽����
	if (sound.hThread == NULL) {
		//GetLastError();
		return -1;
	}
	_listPlay.push_back(sound);//������Ϣ
	CloseHandle(sound.hThread);
	Sleep(timeout);
	return (int)sound.hThread;//����һ����ǿת���߳̾��
}

int Player::playSoundLoop(const char* szFileNameTemp, unsigned short volume) {
	Player::_Sound sound;//���ڱ���ôβ��ŵ������Ϣ
	sound.szFileName = (char*)szFileNameTemp;
	sound.MciOpen.lpstrElementName = szFileNameTemp;
	sound.volume = volume;
	sound.isStop = false;

	sound.hThread = CreateThread(NULL, 0, Player::_PlayLoop, (LPVOID)szFileNameTemp, 0, NULL);//�����߳��첽����
	if (sound.hThread == NULL) {
		//GetLastError();
		return -1;
	}
	_listPlay.push_back(sound);//������Ϣ
	CloseHandle(sound.hThread);
	Sleep(timeout);
	return (int)sound.hThread;//����һ����ǿת���߳̾��
}

int Player::playSoundLoop(char* szFileNameTemp, unsigned short volume) {
	Player::_Sound sound;//���ڱ���ôβ��ŵ������Ϣ
	sound.szFileName = szFileNameTemp;
	sound.MciOpen.lpstrElementName = szFileNameTemp;
	sound.volume = volume;
	sound.isStop = false;

	sound.hThread = CreateThread(NULL, 0, Player::_PlayLoop, (LPVOID)szFileNameTemp, 0, NULL);//�����߳��첽����
	if (sound.hThread == NULL) {
		//GetLastError();
		return -1;
	}
	_listPlay.push_back(sound);//������Ϣ
	CloseHandle(sound.hThread);
	Sleep(timeout);
	return (int)sound.hThread;//����һ����ǿת���߳̾��
}
void Player::stopPlay(const char* szTargetFileName) {
	std::list<_Sound>::iterator it = _listPlay.begin();
	for (; it != _listPlay.end(); ++it) {
		if ((*it) == (char*)szTargetFileName) {
			(*it).stop();
			break;
		}
	}
}
void Player::stopPlay(char* szTargetFileName) {
	std::list<_Sound>::iterator it = _listPlay.begin();
	for (; it != _listPlay.end(); ++it) {
		if ((*it) == szTargetFileName) {
			(*it).stop();
			break;
		}
	}
}
void Player::stopPlay(int hThread) {
	std::list<_Sound>::iterator it = _listPlay.begin();
	for (; it != _listPlay.end(); ++it) {
		if ((*it) == hThread) {
			(*it).stop();
			break;
		}
	}
}
void Player::stopPlayAll() {
	std::list<Player::_Sound>::iterator it = _listPlay.begin();
	for (; it != _listPlay.end(); ++it) {
		(*it).stop();
	}
}

DWORD WINAPI Player::_Play(LPVOID lpParameter) {
	//��������Ѱ�� ���ɵ�ǰ�߳���ά���Ĳ�����Ϣ
	std::list<Player::_Sound>::iterator it = _listPlay.begin();
	for (; it != _listPlay.end(); ++it) {
		if ((*it) == (char*)lpParameter) {
			//�򿪲���Ҫ������������ͬһ�߳�.
			DWORD ret = mciSendCommand(0, MCI_OPEN,
				MCI_OPEN_ELEMENT, (DWORD_PTR)(LPVOID) & (*it).MciOpen);//��һ���豸 ׼�����Ÿ���Ч
			if (ret != 0) {
				//std::cout << (*it).szFileName << " MCI_OPEN ret:" << ret << std::endl;
				return ret;
			}
			//std::cout << (*it).szFileName << " id:" << (*it).MciOpen.wDeviceID << std::endl;

			ret = mciSendCommand((*it).MciOpen.wDeviceID, MCI_PLAY,
				MCI_NOTIFY, (DWORD_PTR)(LPVOID) & (*it).MciPlay);//��ʼ����
			//std::cout << (*it).szFileName << " MCI_PLAY ret" <<ret << std::endl;

			(*it).MciStatus.dwItem = MCI_STATUS_LENGTH;//Ҫ��ȡ���� ��ȡ����
			ret = mciSendCommand((*it).MciOpen.wDeviceID, MCI_STATUS,
				MCI_STATUS_ITEM, (DWORD_PTR)(LPVOID) & (*it).MciStatus);//��ȡ��Чʱ��  ��λ����
			(*it).nLength = (*it).MciStatus.dwReturn;
			//std::cout << (*it).szFileName << " MCI_STATUS ret" << ret << std::endl;
			//std::cout << (*it).szFileName << " Length:" << (*it).nLength << std::endl;


			(*it).MciSetAudio.dwItem = MCI_DGV_SETAUDIO_VOLUME;
			(*it).MciSetAudio.dwValue = (*it).volume;
			ret = mciSendCommand((*it).MciOpen.wDeviceID, MCI_SETAUDIO,
				MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD_PTR) & (*it).MciSetAudio);//��������
			//std::cout << (*it).szFileName << " MCI_SET ret" << ret << std::endl;

			int step = 500;//��Ӧʱ��
			for (int i = 0; (!(*it).isStop) && (i < (*it).nLength); i += step) {//�ȴ��������
				Sleep(step);
			}
			//Sleep((*it).nLength);//�ȴ��������
			ret = mciSendCommand((*it).MciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);//�ر��豸
			//std::cout << (*it).szFileName << " MCI_CLOSE ret" << ret << std::endl;
			_listPlay.erase(it);//��������ɾ��
			break;
		}
	}
	return 0L;
}

DWORD WINAPI Player::_PlayLoop(LPVOID lpParameter) {
	//��������Ѱ�� ���ɵ�ǰ�߳���ά���Ĳ�����Ϣ
	std::list<Player::_Sound>::iterator it = _listPlay.begin();
	for (; it != _listPlay.end(); ++it) {
		if ((*it) == (char*)lpParameter) {
			DWORD ret;
			//ѭ������
			while (!(*it).isStop) {
				ret = mciSendCommand(0, MCI_OPEN,
					MCI_OPEN_ELEMENT, (DWORD_PTR)(LPVOID) & (*it).MciOpen);//��һ���豸 ׼�����Ÿ���Ч
				if (ret != 0) {
					return ret;
				}
				//std::cout << (*it).szFileName << " id:" << (*it).MciOpen.wDeviceID << std::endl;

				ret = mciSendCommand((*it).MciOpen.wDeviceID, MCI_PLAY,
					MCI_NOTIFY, (DWORD_PTR)(LPVOID) & (*it).MciPlay);//��ʼ����
				//std::cout << (*it).szFileName << " MCI_PLAY ret" << ret << std::endl;

				(*it).MciStatus.dwItem = MCI_STATUS_LENGTH;//Ҫ��ȡ���� ��ȡ����
				ret = mciSendCommand((*it).MciOpen.wDeviceID, MCI_STATUS,
					MCI_STATUS_ITEM, (DWORD_PTR)(LPVOID) & (*it).MciStatus);//��ȡ��Чʱ��  ��λ����
				(*it).nLength = (*it).MciStatus.dwReturn;
				//std::cout << (*it).szFileName << " MCI_STATUS ret" << ret << std::endl;
				//std::cout << (*it).szFileName << " Length:" << (*it).MciStatus.dwReturn << std::endl;

				(*it).MciSetAudio.dwItem = MCI_DGV_SETAUDIO_VOLUME;
				(*it).MciSetAudio.dwValue = (*it).volume;
				ret = mciSendCommand((*it).MciOpen.wDeviceID, MCI_SETAUDIO,
					MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD_PTR) & (*it).MciSetAudio);//��������
				//std::cout << (*it).szFileName << " MCI_SET ret" << ret << std::endl;

				int step = 500;//��Ӧʱ��
				for (int i = 0; (!(*it).isStop) && (i < (*it).nLength); i += step) {
					Sleep(step);
				}

				ret = mciSendCommand((*it).MciOpen.wDeviceID, MCI_CLOSE, NULL, NULL);//�ر��豸
				//std::cout << (*it).szFileName << " MCI_CLOSE ret" << ret << std::endl;
			}
			_listPlay.erase(it);//��������ɾ��
			break;
		}
	}

	return 0L;
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
	obj.point.x = -30;
	obj.point.y = -30;
	obj.setAngle(45);
	obj.pic = &picField;
	DrawObject(&obj);//����


	Object obj2;
	obj2.point.x = nScreenWidth - 30;
	obj2.point.y = nScreenHeight - 30;
	obj2.setAngle(45);
	obj2.pic = &picField;
	DrawObject(&obj2);//����*/


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

	Player::stopPlayAll();
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

void loadPicture() {
	picLand.loadImage("pic\\land.bmp");
	picHouse.loadImage("pic\\house.bmp");
	picHouse1.loadImage("pic\\house1.bmp");
	picFarmer.loadImage("pic\\farmer.bmp");
	picBuilder.loadImage("pic\\builder.bmp");
	picField.loadImage("pic\\field.bmp");
	picTree.loadImage("pic\\tree.bmp");
	picKing.loadImage("pic\\king.bmp");
	picField1.loadImage("pic\\field1.bmp");
	picField2.loadImage("pic\\field2.bmp");
	picRice.loadImage("pic\\rice.bmp");
	picWood.loadImage("pic\\wood.bmp");
	picSun.loadImage("pic\\sun.bmp");
	picBackground.loadImage("pic\\background.bmp");
	picBackground1.loadImage("pic\\background1.bmp");
	picMoon.loadImage("pic\\moon.bmp");
	picKingHouse.loadImage("pic\\kinghouse.bmp");
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

	loadPicture();
	return 0;
}

void playMusic(char* fileName) {
	bool ret = PlaySound(fileName, NULL, SND_FILENAME | SND_ASYNC);
	//DebugPrintln("%d %d", ret, GetLastError());
}
void playMusic(const char* fileName) {
	bool ret = PlaySound(fileName, NULL, SND_FILENAME | SND_ASYNC);
	//DebugPrintln("%d %d", ret, GetLastError());
}