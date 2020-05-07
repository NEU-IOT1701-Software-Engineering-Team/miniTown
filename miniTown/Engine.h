#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <cstdlib>
#include <mmsystem.h>
#include <vector>
#include <list>
#include <mutex>
#include <string>
#include <algorithm>
#include <Digitalv.h>
#include "Debug.h"
#pragma comment(lib, "WINMM.LIB")

struct Point;
class Object;
class Color;
class Picture;

extern const char* WindowTitle;
extern  int nScreenWidth;
extern  int nScreenHeight;
extern BYTE screen_keys[512];
extern HWND hWnd;


//extern Object* drawList[MaxDrawObjectSum];
extern int drawSum;

constexpr auto PI = 3.1415926;

class Player {
public:
	Player();
	~Player();

	//存储播放信息
	struct _Sound {
		HANDLE hThread;
		MCI_OPEN_PARMS MciOpen;
		MCI_PLAY_PARMS MciPlay;
		MCI_STATUS_PARMS MciStatus;
		//MCI_SET_PARMS MciSet;
		MCI_DGV_SETAUDIO_PARMS MciSetAudio;
		DWORD nLength;//音效长度 单位ms
		bool isStop;//是否停止播放
		unsigned short volume;//音量0-1000
		char* szFileName;
		time_t id;
		struct _Sound() {
			memset(this, 0, sizeof(struct _Sound));
		}
		//推荐使用
		bool operator==(int hThread) {
			return this->hThread == (HANDLE)hThread;
		}
		bool operator==(struct _Sound* pSound) {
			//return this->hThread == pSound->hThread;
			return this->id == pSound->id;
		}
		bool operator==(time_t id) {
			return this->id == id;
		}
		void stop() {
			isStop = true;
		}
	};
	typedef struct _Sound _Sound;

	//Player所用到的消息结构体
	//用于主线程与Player的消息传递
	struct _MSG
	{
#define PLAYER_MSG_PLAY		0x00
#define PLAYER_MSG_LOOP		0x01
#define PLAYER_MSG_STOP		0x02
#define PLAYER_MSG_STOPALL	0x03
#define PLAYER_MSG_EXIT		0xff
		BYTE type;
		char* szFileName;
		unsigned short volume;
		Player::_Sound* pSound;
		struct _MSG() {
			memset(this, 0, sizeof(struct _MSG));
		}
		struct _MSG& operator=(struct _MSG srcMsg) {
			this->type = srcMsg.type;
			this->szFileName = srcMsg.szFileName;
			this->volume = srcMsg.volume;
			this->pSound = srcMsg.pSound;
			return *this;
		}

	};
	typedef struct _MSG _MSG;

	//Description:
	//	播放音效,仅播放一次.
	//Paramter: 
	//	const char* szFileNameTemp 要播放的文件名，文件名中不能有空格，如果有空格必须使用双引号
	//	unsigned short volume=800 音量数值[0,1000]，wav格式不支持设置音量
	//Return Value:
	//	-1		创建线程失败
	//	非负数	线程int类型的线程句柄，使用时请强转为HANDLE
	static void playSound(const char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	播放音效,仅播放一次
	//Paramter: 
	//	char * szFileNameTemp 要播放的文件名，文件名中不能有空格，如果有空格必须使用双引号
	//	unsigned short volume=800 音量数值[0,1000]，wav格式不支持设置音量
	//Return Value:
	//	-1		创建线程失败
	//	非负数	线程int类型的线程句柄，使用时请强转为HANDLE
	static void playSound(char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	播放音效,循环播放
	//Paramter: 
	//	const char* szFileNameTemp 要播放的文件名，文件名中不能有空格，如果有空格必须使用双引号
	//	unsigned short volume=800 音量数值[0,1000]，wav格式不支持设置音量
	//Return Value:
	//	-1		创建线程失败
	//	非负数	线程int类型的线程句柄，使用时请强转为HANDLE.
	static void playSoundLoop(const char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	播放音效,循环播放
	//Paramter: 
	//	char* szFileNameTemp 要播放的文件名，文件名中不能有空格，如果有空格必须使用双引号
	//	unsigned short volume=800 音量数值[0,1000]，wav格式不支持设置音量
	//Return Value:
	//	-1		创建线程失败
	//	非负数	线程int类型的线程句柄，使用时请强转为HANDLE.
	static void playSoundLoop(char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	通过文件名终止循环播放(不推荐)
	//Paramter: 
	//	const char* szTargetFileName 要停播的音效文件名，文件名中不能有空格，如果有空格必须使用双引号
	//Return Value:
	//	NONE
	//static void stopPlay(const char* szTargetFileName);

	//Description:
	//	通过文件名终止循环播放(不推荐)
	//Paramter: 
	//	char* szTargetFileName 要停播的音效文件名，文件名中不能有空格，如果有空格必须使用双引号
	//Return Value:
	//	NONE
	//static void stopPlay(char* szTargetFileName);

	//Description:
	//	通过线程句柄终止循环播放(推荐),在循环播放时会返回一个线程句柄
	//Paramter: 
	//	int hThread 要停播的线程句柄
	//Return Value:
	//	NONE
	static void stopPlay(int hThread);

	//Description:
	//	通过存储播放信息指针终止播放
	//Paramter: 
	//	Player::_Sound* pSound 存储播放信息指针
	//Return Value:
	//	NONE
	static void stopPlay(Player::_Sound* pSound);

	//Description:
	//	停止播放所有音效
	//Paramter: 
	//	NONE
	//Return Value:
	//	NONE
	static void stopPlayAll();
	static void _StaticDestruct();
private:



	static HANDLE _hThread;
	static DWORD WINAPI _Run(LPVOID lpParameter);
	static void _StaticStructure();
	//static void _StaticDestruct();
	static DWORD WINAPI _Play(LPVOID lpParameter);
	static DWORD WINAPI _PlayLoop(LPVOID lpParameter);

	static void _clearPlayList();
	static Player::_Sound* _MyMalloc();
	static void _MyFree(Player::_Sound* ptr);
	static void _print(Player::_Sound* pSound, _In_z_ _Printf_format_string_ char const* const _Format, ...);
};

struct Point {
	int x;
	int y;
	Point(int tx = 0, int ty = 0) {
		x = tx;
		y = ty;
	}
	void operator+(struct Point p2) {
		this->x += p2.x;
		this->y += p2.y;
	}
	void operator-(struct Point p2) {
		this->x -= p2.x;
		this->y -= p2.y;
	}
	void operator-(RECT rect) {
		this->x -= rect.left;
		this->y -= rect.top;
	}
	void operator+(RECT rect) {
		this->x += rect.left;
		this->y += rect.top;
	}
	struct Point& operator+=(const struct Point& p2) {
		//很迷
		this->x += p2.x;
		this->y += p2.y;
		return *this;
	}
	struct Point& operator=(const struct Point& p2) {
		this->x = p2.x;
		this->y = p2.y;
		return *this;
	}
};
typedef struct Point Point;



class Object {
public:
	float x;//为了兼容之前的版本 不推荐使用23333
	float y;//为了兼容之前的版本 不推荐使用23333
	Point point;

	/*
	z值越小，越先绘制
	z值相同，y越小越先绘制
	*/
	int z;
	Picture* pic;//该对象的图标

	Object() {
		memset(this, 0, sizeof(Object));
	}

	void WalkTo(Object* object);

	//Description:
	//	设置图片旋转角度
	//Paramter: 
	//	int angle 将要旋转的角度.(角度制)
	//Return Value:
	//	NONE
	void setAngle(int angle) {

		if (abs(angle) >= 360) {
			angle %= 360;
		}
		this->angle = ((float)angle) * PI / 180;
	}

	//Description:
	//	设置图片旋转角度
	//Paramter: 
	//	float radian 将要旋转的角度.(弧度制)
	//Return Value:
	//	NONE
	void setAngle(float radian) {
		if (abs(radian) >= 2 * PI) {//角度大于2pi 需要进行缩放
			//1、弧度转角度
			//2、角度缩放
			//3、角度转弧度
			angle = (float(((int(radian * 180 / PI))) % 360)) * PI / 180;
		}
		else {
			angle = radian;
		}
	}

	//Description:
	//	返回图片旋转角度(角度制).
	//Paramter: 
	//	NONE
	//Return Value:
	//	int 返回图片旋转角度(角度制).
	int getAngle() {
		return int(angle * 180 / PI);
	}

	//Description:
	//	返回图片旋转角度(角度制).
	//Paramter: 
	//	BYTE none 无关参数，忽略即可。仅为了重载.
	//Return Value:
	//	float 返回图片旋转角度(角度制).
	float getAngle(BYTE none) {
		return angle;
	}

	//为了兼容之前的版本
	void updatePoint() {
		point.x = x;
		point.y = y;
	}
	void setPoint(int tx, int ty) {
		x = tx;
		point.x = tx;
		y = ty;
		point.y = ty;
	}
	void setPoint(const Point& point) {
		this->point = point;
	}

private:
	float angle = 0.0f;//图像的旋转角度(弧度制)
};

struct ObjectPointer {
public:
	Object* pObject;//一个指向实际object对象的指针

	struct ObjectPointer() {
		pObject = NULL;
	}

	struct ObjectPointer(Object* tpObject) {
		pObject = tpObject;
	}

	//Description:
	//	重载<操作符以实现对object的按Z值的排序
	//Paramter: 
	//	class ObjectPointer obj2 另一个比较对象
	//Return Value:
	//	bool 比较结果
	bool operator<(struct ObjectPointer obj2) {
		if (this->pObject == NULL || obj2.pObject == NULL) {
			return false;
		}
		this->pObject->updatePoint();
		obj2.pObject->updatePoint();
		
		//渲染规则
		//obj1 obj2 的z都等于-1时		y越小越先画越靠底层
		//obj1 obj2 只有一个z等于 - 1时	z= - 1的先画
		//obj1 obj2 的z都不等于 - 1时		y越小越先画越靠底层 y相同时 z越小越先画越靠底层

		//判断第一个对象的z值是否为-1
		if (this->pObject->z == -1) {
			//判断第二个对象的z值是否为-1
			if (obj2.pObject->z == -1) {
				return this->pObject->point.y < obj2.pObject->point.y;
			}
			else {
				//z1==-1 && z2!=-1
				return true;//this 和 obj2相比 this先画
			}
		}
		else if (obj2.pObject->z == -1) {
			if (this->pObject->z == -1) {
				return this->pObject->point.y < obj2.pObject->point.y;
			}
			else {
				//z1!=-1 && z2==-1
				return false;//this 和 obj2相比 obj先画
			}
		}
		else {
			//两者里边都没z=-1  //y优先
			if (this->pObject->point.y == obj2.pObject->point.y) {
				//y值相同
				return this->pObject->z < obj2.pObject->z;
			}
			else {
				return this->pObject->point.y < obj2.pObject->point.y;
			}
		}
	}

	//Description:
	//	重载==操作符以实现对object比较，判断是否是同一个对象
	//Paramter: 
	//	class ObjectPointer obj2 另一个比较对象
	//Return Value:
	//	bool 比较结果
	bool operator==(struct ObjectPointer obj2) {
		return this->pObject == obj2.pObject;
	}
private:
};
typedef struct ObjectPointer ObjectPoint;

#define COLOR_WHITE	{255,255,254}
#define COLOR_BLACK	{  0,  0,  0}
#define COLOR_RED	{255,  0,  0}
#define COLOR_GREEN	{  0,255,  0}
#define COLOR_BLUE	{  0,  0,255}
class Color {
public:
	BYTE r, g, b, a;
	Color() {
		r = 0, g = 0, b = 0, a = 0;
	}
	Color(int R, int G, int B, int A = 0) {
		r = R, g = G, b = B, a = A;
	}
	bool isTransparent() {
		return (r == 255 && g == 255 && b == 255);
	}
	COLORREF getColorRef() {
		return RGB(r, g, b);
	}
};

class Picture {
public:
	int nErrorCode = 0;
	int nDataSize = 0;//数据大小
	int nImgSize = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nWidthBytes = 0;//每行字节数
	int nBitsPixel = 0;//每个像素所占位数
	bool isBackward = true;
	Point centerPoint;

	/*
	按照 BGRA的顺序排列
	*/
	//BYTE* pData = NULL;
	RGBQUAD* pColorPal = NULL;

	BYTE* pChannelR = NULL;
	BYTE* pChannelG = NULL;
	BYTE* pChannelB = NULL;
	BYTE* pChannelA = NULL;

	Picture() {
		;
	}
	Picture(const char* szPath) {
		loadImage((char*)szPath);
	}
	Picture(char* szPath) {
		loadImage(szPath);
	}
	~Picture() {
		delete[] pChannelR;
		pChannelR = NULL;
		delete[] pChannelG;
		pChannelG = NULL;
		delete[] pChannelB;
		pChannelB = NULL;
		delete[] pChannelA;
		pChannelA = NULL;
		//delete[] pData;
		//pData = NULL;
		delete[] pColorPal;
		pColorPal = NULL;
	}

	//Description:
	//	从本地加载图片到内存
	//Paramter: 
	//	char* szPath要加载的图片路径,不对参数合法性进行检查.
	//Return Value:
	//	0  正常
	//	-1 读取信息头错误!
	//	-2 位图文件类型非法biCompression
	//	-3 位图文件打开失败
	//	-4 内存不足
	//	-5 读取数据错误
	//	-6  读取文件头错误!
	int loadImage(char* szPath) {
		return _loadImage(szPath);
	}
	//Description:
	//	从本地加载图片到内存
	//Paramter: 
	//	char* szPath要加载的图片路径,不对参数合法性进行检查.
	//Return Value:
	//	0  正常
	//	-1 读取信息头错误!
	//	-2 位图文件类型非法biCompression
	//	-3 位图文件打开失败
	//	-4 内存不足
	//	-5 读取数据错误
	//	-6  读取文件头错误!
	int loadImage(const char* szPath) {
		return _loadImage((char*)szPath);
	}

	int getWidth() {
		return nWidth;
	}
	int getHeight() {
		return nHeight;
	}
	int getBitsPixel() {
		return nBitsPixel;
	}
	int getWidthBytes() {
		return nWidthBytes;
	}
	int getErrorCode() {
		return nErrorCode;
	}

#define Ch_R 0
#define Ch_G 1
#define Ch_B 2
#define Ch_A 3
	BYTE& operator()(int nPos, int Channel) {
		switch (Channel) {
		case Ch_R: {
			return pChannelR[nPos];
		}
		case Ch_G: {
			return pChannelG[nPos];
		}
		case Ch_B: {
			return pChannelB[nPos];
		}
		case Ch_A: {
			return pChannelA[nPos];
		}
		default:
			return pChannelA[0];//不应该出现
		}
	}
	BYTE& operator()(int row, int column, int Channel) {
		switch (Channel) {
		case Ch_R: {
			return pChannelR[row * nWidth + column];
		}
		case Ch_G: {
			return pChannelG[row * nWidth + column];
		}
		case Ch_B: {
			return pChannelB[row * nWidth + column];
		}
		case Ch_A: {
			return pChannelA[row * nWidth + column];
		}
		default:
			return pChannelA[0];//不应该出现
		}
	}
private:
	int _loadImage(char* szPath) {
		/*
		if (hImage != NULL) {
			DeleteObject(hImage);
		}
		//从文件加载位图
		hImage = (HBITMAP)LoadImage(0, szPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		//读取属性
		GetObject(hImage, sizeof(BITMAP), &bm);*/

		FILE* fp = fopen(szPath, "rb");
		if (fp == NULL) {
			nErrorCode = -3;
			return nErrorCode;
		}
		BITMAPFILEHEADER fileHeader;
		if (fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp) != 1) {
			fclose(fp);
			nErrorCode = -6;
			return nErrorCode;
		}
		BITMAPINFOHEADER infoHeader;
		if (fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp) != 1) {
			fclose(fp);
			nErrorCode = -1;
			return nErrorCode;
		}

		nWidth = infoHeader.biWidth;
		if (infoHeader.biHeight < 0) {
			//正向
			isBackward = false;
			nHeight = -infoHeader.biHeight;
		}
		else {
			//倒向
			isBackward = true;
			nHeight = infoHeader.biHeight;
		}
		//计算图像中心点
		centerPoint.x = nWidth / 2;
		centerPoint.y = nHeight / 2;

		switch (infoHeader.biCompression) {
		case BI_RGB: {
			//不压缩
			//nDataSize = nWidth * nHeight * 4;
			nImgSize = nWidth * nHeight;
			nWidthBytes = ((nWidth * 3 + 3) / 4 * 4);
			//pData = new BYTE[nDataSize];
			pChannelR = new BYTE[nImgSize];
			pChannelG = new BYTE[nImgSize];
			pChannelB = new BYTE[nImgSize];
			//pChannelA = new BYTE[nImgSize];
			if (pChannelR == NULL || pChannelG == NULL || pChannelB == NULL /*|| pChannelA==NULL*/) {
				fclose(fp);
				nErrorCode = -4;
				return nErrorCode;
			}
			if (isBackward) {
				//倒立
				for (int i = nHeight - 1; i >= 0; --i) {
					for (int j = 0; j < nWidth; ++j) {
						if (fread(&pChannelB[i * nWidth + j], sizeof(BYTE), 1, fp) != 1) {
							nErrorCode = -5;
							return nErrorCode;
						}
						if (fread(&pChannelG[i * nWidth + j], sizeof(BYTE), 1, fp) != 1) {
							nErrorCode = -5;
							return nErrorCode;
						}
						if (fread(&pChannelR[i * nWidth + j], sizeof(BYTE), 1, fp) != 1) {
							nErrorCode = -5;
							return nErrorCode;
						}
					}
					fseek(fp, nWidthBytes - nWidth * 3, SEEK_CUR);
				}
			}
			else {
				for (int i = 0; i < nHeight; ++i) {
					for (int j = 0; j < nWidth; ++j) {
						if (fread(&pChannelB[i * nWidth + j], sizeof(BYTE), 1, fp) != 1) {
							nErrorCode = -5;
							return nErrorCode;
						}
						if (fread(&pChannelG[i * nWidth + j], sizeof(BYTE), 1, fp) != 1) {
							nErrorCode = -5;
							return nErrorCode;
						}
						if (fread(&pChannelR[i * nWidth + j], sizeof(BYTE), 1, fp) != 1) {
							nErrorCode = -5;
							return nErrorCode;
						}
					}
					fseek(fp, nWidthBytes - nWidth * 3, SEEK_CUR);
				}
			}
			break;
		}
		case BI_RLE8: {
			//8bit rle压缩
			//infoHeader.biBitCount=8
			break;
		}
		case BI_RLE4: {
			//4bit rle压缩
			//infoHeader.biBitCount=4
			break;
		}
		case BI_BITFIELDS: {
			//比特域用于16/32位
			break;
		}
		case BI_JPEG: {
			//包含jpeg图像
			break;
		}
		case BI_PNG: {
			//包含png图像
			break;
		}
		default: {
			nErrorCode = -2;
			return nErrorCode;
		}
		}//end switch


		fclose(fp);
		nErrorCode = 0;
		return nErrorCode;
	}
};


//Description:
//	创建并维护一个窗口,目前不对参数合法性进行检查.
//	该窗口显示的内容依赖于window结构体里所指向的img
//Paramter: 
//	LPVOID window一个指向Window结构体的指针
//Return:
//	0:		正常
//	other:	由GetLastError()返回的错误代码
int _CreateWindow(const char* title, int nWidth, int nHeight);

//Description:
//	更新重画窗口
//	接收消息
//Paramter: 
//	NONE
//Return:
//	NONE
void Draw();

//Description:
//	向绘制队列中加入一个新对象,如果该对象已被加入，则不会重复加入.
//Paramter: 
//	Object* object 将要加入对象的地址
//Return Value:
//	NONE
void AddDrawObject(Object* object);

//Description:
//	从绘制队列中删掉一个对象。
//Paramter: 
//	Object* object 将要删除的对象地址
//Return Value:
//	NONE
void RemoveDrawObecjt(Object* object);


#define DEFAULT_BC			COLOR_WHITE
#define DEFAULT_FC			COLOR_BLACK
#define DEFAULT_FOCUS_BC	{201,218,248}
#define DEFAULT_DOWN_BC		{109,158,235}
//UI 按钮
struct Button
{
	char* title;
	RECT rect;
	Color currentBackgroundColor;
	void (*lpDoubleClickL)(void);
	void (*lpClickL)(void);
	void (*lpClickR)(void);
	bool isClick;
	bool enabled;//按钮是否可用
	bool isVisible;//按钮是否可用

	Button() {
		_initALL();
	}
	Button(char* tTitle, int x,int y,int width,int height) {
		_initALL();
		title = tTitle;
		rect = { x,y,x + width,y + height };
	}
	Button(char* tTitle, RECT tRect) {
		_initALL();
		title = tTitle;
		rect = tRect;
	}
	Button(char* tTitle, RECT tRect, Color tBackgroundColor, Color tForegroundColor) {
		_initALL();
		title = tTitle;
		rect = tRect;
		backgroundColor = tBackgroundColor;
		currentBackgroundColor = tBackgroundColor;
		foregroundColor = tForegroundColor;
	}
	Button(char* tTitle, int x, int y, int width, int height, Color tBackgroundColor, Color tForegroundColor) {
		_initALL();
		title = tTitle;
		rect = { x,y,x + width,y + height };
		backgroundColor = tBackgroundColor;
		currentBackgroundColor = tBackgroundColor;
		foregroundColor = tForegroundColor;
	}
	Button(char* tTitle, RECT tRect, Color tBackgroundColor, Color tForegroundColor, void (*tlpClickL)(void)) {
		_initALL();
		title = tTitle;
		rect = tRect;
		backgroundColor = tBackgroundColor;
		currentBackgroundColor = tBackgroundColor;
		foregroundColor = tForegroundColor;
		lpClickL = tlpClickL;
	}

	void setBackgroundColor(Color color) {
		backgroundColor = color;
		currentBackgroundColor = color;
	}
	void setBackgroundColor(BYTE r,BYTE g ,BYTE b) {
		backgroundColor = { r,g,b };
		currentBackgroundColor = {r,g,b};
	}
	Color getBackgroundColor() {
		return backgroundColor;
	}

	void setForegroundColor(Color color) {
		foregroundColor = color;
	}
	void setForegroundColor(BYTE r, BYTE g, BYTE b) {
		foregroundColor = { r,g,b };
	}
	Color getForegroundColor() {
		return foregroundColor;
	}

	//修改按钮位置
	void setRect(RECT tRect) {
		rect = tRect;
	}
	void setRect(Point point) {
		rect = { point.x,point.y, rect.right ,rect.bottom };
	}
	void setRect(int width, int height) {
		rect = { rect.left,rect.top,rect.left + width,rect.top + height };
	}
	void setRect(int x, int y, int width, int height) {
		rect = { x,y,x + width,y + height };
	}

	Point getPoint() {
		return Point(rect.left, rect.top);
	}
	RECT getRect() {
		return rect;
	}

	//判断被点击的点是否再rect内
	bool operator==(Point point) {
		return point.x <= rect.right && point.x >= rect.left && point.y <= rect.bottom && point.y >= rect.top;
	}

private:
	Color foregroundColor;
	Color backgroundColor;

	inline void _initALL() {
		memset(this, 0, sizeof(*this));
		currentBackgroundColor = DEFAULT_BC;
		backgroundColor = DEFAULT_BC;
		lpDoubleClickL = NULL;
		lpClickL = NULL;
		lpClickR = NULL;
		enabled = true;
		isVisible = true;
	}
};
typedef struct Button Button;

//Description:
//	增加一个按钮对象,如果该对象已被加入，则不会重复加入.
//Paramter: 
//	Button * button 将要加入对象的地址
//Return Value:
//	NONE
void AddButton(Button* button);

//Description:
//	从按钮对象列表中删掉一个对象。
//Paramter: 
//	Button * button 将要删除的对象地址
//Return Value:
//	NONE
void RemoveButton(Button* button);


//UI 标签
struct Label {
	char* title;
	RECT rect;
	Color currentBackgroundColor;
	//暂时不需要响应事件

	bool enabled;//是否可用
	bool isVisible;//是否可见

	//Description:
	//	默认构造函数。
	//Paramter: 
	//	NONE
	//Return Value:
	//	NONE
	struct Label() {
		_initALL();
	}
	
	//Description:
	//	默认构造函数。
	//Paramter: 
	//	char* tTitle 标签标题
	//	RECT tRect 标签大小以及位置
	//Return Value:
	//	NONE
	struct Label(char* tTitle, RECT tRect) {
		_initALL();
		title = tTitle;
		rect = tRect;
	}

	//Description:
	//	默认构造函数。
	//Paramter: 
	//	char* tTitle 标签标题
	//	int x, int y 标签位置
	//	int width, int height 标签大小
	//Return Value:
	//	NONE
	struct Label(char* tTitle, int x, int y, int width, int height) {
		_initALL();
		title = tTitle;
		rect = { x,y,x + width,y + height };
	}

	//Description:
	//	默认构造函数。
	//Paramter: 
	//	char* tTitle 标签标题
	//	RECT tRect 标签大小以及位置
	//	Color tBackgroundColor 背景色
	//	Color tForegroundColor 前景色（文字颜色）
	//Return Value:
	//	NONE
	struct Label(char* tTitle, RECT tRect, Color tBackgroundColor, Color tForegroundColor) {
		_initALL();
		title = tTitle;
		rect = tRect;
		backgroundColor = tBackgroundColor;
		currentBackgroundColor = tBackgroundColor;
		foregroundColor = tForegroundColor;
	}

	//Description:
	//	默认构造函数。
	//Paramter: 
	//	char* tTitle 标签标题
	//	int x, int y 标签位置
	//	int width, int height 标签大小
	//	Color tBackgroundColor 背景色
	//	Color tForegroundColor 前景色（文字颜色）
	//Return Value:
	//	NONE
	struct Label(char* tTitle, int x, int y, int width, int height, Color tBackgroundColor, Color tForegroundColor) {
		_initALL();
		title = tTitle;
		rect = { x,y,x + width,y + height };
		backgroundColor = tBackgroundColor;
		currentBackgroundColor = tBackgroundColor;
		foregroundColor = tForegroundColor;
	}

	//Description:
	//	设置背景颜色。
	//Paramter: 
	//	Color color 背景颜色
	//Return Value:
	//	NONE
	void setBackgroundColor(Color color) {
		backgroundColor = color;
		currentBackgroundColor = color;
	}

	//Description:
	//	设置背景颜色。
	//Paramter: 
	//	BYTE r, BYTE g, BYTE b 背景颜色
	//Return Value:
	//	NONE
	void setBackgroundColor(BYTE r, BYTE g, BYTE b) {
		backgroundColor = { r,g,b };
		currentBackgroundColor = { r,g,b };
	}

	//Description:
	//	获取背景颜色。
	//Paramter: 
	//	NONE
	//Return Value:
	//	Color 背景颜色
	Color getBackgroundColor() {
		return backgroundColor;
	}

	//Description:
	//	获取前景颜色。
	//Paramter: 
	//	NONE
	//Return Value:
	//	Color 前景颜色
	void setForegroundColor(Color color) {
		foregroundColor = color;
	}

	//Description:
	//	设置前景颜色。
	//Paramter: 
	//	BYTE r, BYTE g, BYTE b 颜色
	//Return Value:
	//	NONE
	void setForegroundColor(BYTE r, BYTE g, BYTE b) {
		foregroundColor = { r,g,b };
	}

	//Description:
	//	获取前景颜色。
	//Paramter: 
	//	NONE
	//Return Value:
	//	Color 前景颜色
	Color getForegroundColor() {
		return foregroundColor;
	}

	void setRect(RECT tRect) {
		rect = tRect;
	}
	void setRect(Point point) {
		rect = { point.x,point.y, rect.right ,rect.bottom };
	}
	void setRect(int width, int height) {
		rect = { rect.left,rect.top,rect.left + width,rect.top + height };
	}
	void setRect(int x, int y, int width, int height) {
		rect = { x,y,x + width,y + height };
	}

	Point getPoint() {
		return Point(rect.left, rect.top);
	}
	RECT getRect() {
		return rect;
	}
	int getWidth() {
		return rect.right - rect.left;
	}
	int getHeight() {
		return rect.bottom - rect.top;
	}
private:
	Color foregroundColor;
	Color backgroundColor;

	inline void _initALL() {
		memset(this, 0, sizeof(*this));
		currentBackgroundColor = DEFAULT_BC;
		backgroundColor = DEFAULT_BC;
		enabled = true;
		isVisible = true;
	}
};
typedef struct Label Label;

//Description:
//	增加一个标签对象,如果该对象已被加入，则不会重复加入.
//Paramter: 
//	Label* label 将要加入对象的地址
//Return Value:
//	NONE
void AddLabel(Label* label);

//Description:
//	从标签对象列表中删掉一个对象。
//Paramter: 
//	Label* label 将要删除的对象地址
//Return Value:
//	NONE
void RemoveLabel(Label* label);


//UI 编辑框
struct EditBox : public Label
{
	std::string text;
	int nPosCaret;//光标位置
	Point pointCaret;//光标坐标
	bool isShowCaret;//是否显示光标
	int sumFPS;//光标闪烁
	bool stateCaret;//光标当前状态
	
	//Description:
	//	默认构造函数。
	//Paramter: 
	//	NONE
	//Return Value:
	//	NONE
	EditBox() {
		_initAll();
	}

	//Description:
	//	获取编辑框内容 返回指向字符串的首地址
	//Paramter: 
	//	NONE
	//Return Value:
	//	const char *
	const char * getText() {
		return text.c_str();
	}

	//Description:
	//	获取编辑框内容 通过参数返回指向字符串的首地址
	//Paramter: 
	//	char ** pValue
	//Return Value:
	//	int 字符串长度
	int getText(char ** pValue) {
		*pValue = (char*)text.c_str();
		return text.length();
	}

	//Description:
	//	获取编辑框内容 事先已分配好地址
	//Paramter: 
	//	char* value 分配的首地址
	//	int length 分配的长度
	//Return Value:
	//	int 实际复制字符串长度
	int getText(char* value, int length) {
		if (length >= text.length()) {
			//传入地址的长度足够
			memcpy(value, text.c_str(), text.length() *sizeof(char));
			return  text.length();
		}
		else {
			//不足
			memcpy(value, text.c_str(), length * sizeof(char));
			return  length;
		}
		
	}

	//Description:
	//	获取编辑框内容长度
	//Paramter: 
	//	NONE
	//Return Value:
	//	int 长度
	int getLength() {
		return  text.length();
	}

	//Description:
	//	清空编辑框内容
	//Paramter: 
	//	NONE
	//Return Value:
	//	NONE
	void clear() {
		text.clear();
		nPosCaret = 0;
		pointCaret.x = 0;
	}


	void setText(char* str) {
		text.clear();
		text.insert(0,str);
	}
	void moveCaret(int nOff) {
		/*
		nPosCaret += nOff;
		if (nPosCaret < 0) {
			nPosCaret = 0;
			pointCaret.x =0;
		}
		else if(nPosCaret>text.length()){
			nPosCaret = text.length();
			pointCaret.x = text.length() * 4;
		}
		else {
			pointCaret.x += 4 * nOff;
		}
		*/
		nPosCaret += nOff;
		if (nPosCaret < 0) {
			nPosCaret = 0;
			//pointCaret.x = 0;
		}
		else if (nPosCaret > text.length()) {
			nPosCaret = text.length();
			//pointCaret.x = text.length() * 4;
		}
		else {
			//pointCaret.x += 4 * nOff;
		}
	}

	//判断被点击的点是否再rect内
	bool operator==(Point point) {
		return point.x <= rect.right && point.x >= rect.left && point.y <= rect.bottom && point.y >= rect.top;
	}
private:
	void _initAll() {
		nPosCaret = 0;
		sumFPS = 0;
		isShowCaret = false;
		stateCaret = false;
	}
};
typedef struct EditBox EditBox;

//Description:
//	增加一个编辑框对象,如果该对象已被加入，则不会重复加入.
//Paramter: 
//	EditBox* editBox 将要加入对象的地址
//Return Value:
//	NONE
void AddEditBox(EditBox* editBox);

//Description:
//	从编辑框对象列表中删掉一个对象。
//Paramter: 
//	EditBox* editBox 将要删除的对象地址
//Return Value:
//	NONE
void RemoveEditBox(EditBox* editBox);


void freeSomethingForEngine();