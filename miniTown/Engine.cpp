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
//std::vector<Object> listObject;

std::vector<_Sound> _listPlay;
Player::Player() {
	;
}

Player::~Player() {
	_free();
}

int Player::playSound(const char* szFileNameTemp) {
	//����Ҫ���浽������
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startUpInfo;
	memset(&processInfo, 0, sizeof(processInfo));
	memset(&startUpInfo, 0, sizeof(startUpInfo));

	char cmd[512] = { 0 };
	sprintf(cmd, "player.exe -P %s", szFileNameTemp);
	if (!CreateProcess(NULL, cmd, NULL, NULL, false,
		0, NULL, NULL, &startUpInfo, &processInfo)) {
		//DWORD ret = GetLastError();
		return -1;
	}
	//WaitForSingleObject(processInfo.hProcess, INFINITE);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	return 0;
}

int Player::playSound(char* szFileNameTemp) {
	//����Ҫ���浽������
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startUpInfo;
	memset(&processInfo, 0, sizeof(processInfo));
	memset(&startUpInfo, 0, sizeof(startUpInfo));

	char cmd[512] = { 0 };
	sprintf(cmd, "player.exe -P %s", szFileNameTemp);
	if (!CreateProcess(NULL, cmd, NULL, NULL, false,
		0, NULL, NULL, &startUpInfo, &processInfo)) {
		//DWORD ret = GetLastError();
		return -1;
	}
	//WaitForSingleObject(processInfo.hProcess, INFINITE);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	return 0;
}

int Player::playSoundLoop(const char* szFileNameTemp) {
	_Sound sound;
	memset(&sound, 0, sizeof(sound));
	char cmd[512] = { 0 };
	sprintf(cmd, "player.exe -L %s", szFileNameTemp);
	if (!CreateProcess(NULL, cmd, NULL, NULL, false,
		0, NULL, NULL, &sound.startUpInfo, &sound.processInfo)) {
		//DWORD ret = GetLastError();
		return -1;
	}
	sound.szFileName = (char *)szFileNameTemp;
	_listPlay.push_back(sound);

	return sound.processInfo.dwProcessId;
}

int Player::playSoundLoop(char* szFileNameTemp) {
	_Sound sound;
	memset(&sound, 0, sizeof(sound));
	char cmd[512] = { 0 };
	sprintf(cmd, "player.exe -L %s", szFileNameTemp);
	if (!CreateProcess(NULL, cmd, NULL, NULL, false,
		0, NULL, NULL, &sound.startUpInfo, &sound.processInfo)) {
		//DWORD ret = GetLastError();
		return -1;
	}
	sound.szFileName = szFileNameTemp;
	_listPlay.push_back(sound);
	return sound.processInfo.dwProcessId;
}

void Player::stopPlay(char* szTargetFileName) {
	Player::_freeProcess(szTargetFileName);
}
void Player::stopPlay(DWORD dwProcessId) {
	Player::_freeProcess(dwProcessId);
}

void Player::_freeProcess(char* szFileNameTemp) {
	std::vector<_Sound>::iterator it = _listPlay.begin();
	for (; it != _listPlay.end(); ++it) {
		if ((*it) == szFileNameTemp) {
			(*it).stop();
			_listPlay.erase(it);
			break;
		}
	}
}
void Player::_freeProcess(DWORD dwProcessId) {
	std::vector<_Sound>::iterator it = _listPlay.begin();
	for (; it != _listPlay.end(); ++it) {
		if ((*it) == dwProcessId) {
			(*it).stop();
			_listPlay.erase(it);
			break;
		}
	}
}
void Player::_free() {
	while (!_listPlay.empty()) {
		_listPlay[0].stop();
		_listPlay.erase(_listPlay.begin());
	}
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

inline void DrawPoint(int nPos, BYTE r, BYTE g, BYTE b,BYTE a=0) {
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
	if (!(r ==255 && g ==255 && b == 255)) {
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

inline void DrawPic(int x, int y, Picture* pic) {
	int nWidth = pic->getWidth();
	int nHeight = pic->getHeight();
	if (x < 0)
	{
		std::cout << 233 << std::endl;
	}

	int posPic = 0;
	for (int i = 0; i < nHeight; ++i) {
		//����б߽� ���ϱ߽�
		if (y < 0) {
			continue;
		}
		//����б߽� ���±߽�
		if (y >= nScreenHeight) {
			break;//�ö���ʣ�²��ֲ���Ҫ����
		}
		int posBuffer = (nScreenWidth * (y + i) + x) * 4;//����ÿ�е����
		for (int j = 0; j < nWidth; ++j) {
			//����б߽� ��߽�
			if (posBuffer < 0) {
				continue;
			}
			//����б߽� �ұ߽�
			if ((posBuffer%(nScreenWidth * 4)) >= nScreenWidth*4) {
				break;//����ʣ�²��ֲ���Ҫ����
			}
			DrawPoint(posBuffer, pic->pChannelR[posPic], pic->pChannelG[posPic], pic->pChannelB[posPic]);
			posBuffer+=4;
			++posPic;
		}
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

	
	p_.x = p.x * cosAngle + p.y * sinAngle + p0.x * (1 - cosAngle) - p0.y * sinAngle;

	
	p_.y = -p.x * sinAngle + p.y * cosAngle + p0.y * (1 - cosAngle) + p0.x * sinAngle;
}

inline void DrawObject( Object* obj) {
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
				int posBuffer = (p_.y * nScreenWidth + p_.x)*4;
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
	
	
	Object obj;
	obj.point.x = 60;
	obj.point.y = 30;
	obj.setAngle(45);
	obj.pic = &picField;
	DrawObject(&obj);//����
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

void playMusic(char *fileName) {
	bool ret=PlaySound(fileName, NULL, SND_FILENAME | SND_ASYNC);
	//DebugPrintln("%d %d", ret, GetLastError());
}
void playMusic(const char* fileName) {
	bool ret = PlaySound(fileName, NULL, SND_FILENAME | SND_ASYNC);
	//DebugPrintln("%d %d", ret, GetLastError());
}