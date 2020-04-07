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
	//不需要保存到链表中
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
	//不需要保存到链表中
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
		int posBuffer = (nScreenWidth * (y + i) + x) * 4;//计算每行的起点
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
		//检查行边界 的上边界
		if (y < 0) {
			continue;
		}
		//检查行边界 的下边界
		if (y >= nScreenHeight) {
			break;//该对象剩下部分不需要画了
		}
		int posBuffer = (nScreenWidth * (y + i) + x) * 4;//计算每行的起点
		for (int j = 0; j < nWidth; ++j) {
			//检查列边界 左边界
			if (posBuffer < 0) {
				continue;
			}
			//检查列边界 右边界
			if ((posBuffer%(nScreenWidth * 4)) >= nScreenWidth*4) {
				break;//该行剩下部分不需要画了
			}
			DrawPoint(posBuffer, pic->pChannelR[posPic], pic->pChannelG[posPic], pic->pChannelB[posPic]);
			posBuffer+=4;
			++posPic;
		}
	}
}

//Description:
//	P点绕P0顺时针旋转angle
//Paramter: 
//	POINT p		被旋转点
//	POINT p0	中心点
//	float angle	旋转角度
//	POINT& p_	旋转后点
//Return:
//	NONE
inline void spin(Point p, Point p0, float angle, Point& p_) {
	float sinAngle = sin(angle);
	float cosAngle = cos(angle);

	
	p_.x = p.x * cosAngle + p.y * sinAngle + p0.x * (1 - cosAngle) - p0.y * sinAngle;

	
	p_.y = -p.x * sinAngle + p.y * cosAngle + p0.y * (1 - cosAngle) + p0.x * sinAngle;
}

inline void DrawObject( Object* obj) {
	//后期考虑 作为object的成员函数
	float angle = obj->getAngle(1);

	if (angle == 0)//应该可以
	{
		//不需要旋转
		DrawPic(obj->point.x, obj->point.y, obj->pic);
	}
	else {
		int nWidth = obj->pic->getWidth();
		int nHeight = obj->pic->getHeight();

		//遍历图像
		for (int i = 0; i < nHeight; ++i) {
			for (int j = 0; j < nWidth; ++j) {
				//计算旋转后坐标
				Point p_;
				
				//这样计算得到的p_是以当前对象左上角为原点的坐标系 需要映射到窗口坐标系中
				spin({ j,i }, obj->pic->centerPoint, obj->getAngle(1), p_);

				//映射
				p_ += obj->point;

				if (p_.x < 0 || p_.y < 0) {
					continue;//将旋转到窗体外的点抛弃
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
//	更新重画窗口
//	接收消息
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
	DrawObject(&obj);//测试
	for (int i = 0; i < drawSum; i++)
	{
		//DebugPrint("第%d个objetc 开始绘制\t",i);
		Object* drawNowObject = drawList[i];
		//DrawBmp(drawNowObject->x, drawNowObject->y, drawNowObject->pic);
		drawNowObject->updatePoint();
		DrawObject(drawNowObject);
		//DebugPrintln("绘制完成");
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
//处理消息
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
//	创建并维护一个窗口,目前不对参数合法性进行检查.
//	该窗口显示的内容依赖于window结构体里所指向的img
//Paramter: 
//	LPVOID window一个指向Window结构体的指针
//Return:
//	0:		正常
//	other:	由GetLastError()返回的错误代码
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

	//创建内存缓冲画板
	HDC hDC = GetDC(hWnd);
	hMemDC = CreateCompatibleDC(hDC);//创建与窗口兼容的内存设备上下文环境(创建内存缓冲画板)
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), nWidth, -nHeight, 1, 32, BI_RGB,
		nWidth * nHeight * 4, 0, 0, 0, 0 } };
	LPVOID ptr;
	hBitmapBuffer = CreateDIBSection(hMemDC, &bi, DIB_RGB_COLORS, &ptr, 0, 0);//创建缓冲区画布
	hBitmapDevice = (HBITMAP)SelectObject(hMemDC, hBitmapBuffer);//选择缓冲区画布到缓冲区画板，并保留原始画布
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