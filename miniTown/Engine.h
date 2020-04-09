#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <cstdlib>
#include <mmsystem.h>
#include <vector>
#include <list>
#include <mutex>
#include <Digitalv.h>
#include "Debug.h"
#pragma comment(lib, "WINMM.LIB")

struct Point;
class Object;
class Color;
class Picture;

extern const char* title;
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
		_Sound() {
			memset(this, 0, sizeof(struct _Sound));
		}
		//不推荐使用
		bool operator==(char* szTargetFileName) {
			return (strcmp(szFileName, szTargetFileName) == 0);
		}
		//推荐使用
		bool operator==(int hThread) {
			return this->hThread == (HANDLE)hThread;
		}
		void stop() {
			isStop = true;
		}
	};
	typedef struct _Sound _Sound;

	//Description:
	//	播放音效,仅播放一次.
	//Paramter: 
	//	const char* szFileNameTemp 要播放的文件名，文件名中不能有空格，如果有空格必须使用双引号
	//	unsigned short volume=800 音量数值[0,1000]，wav格式不支持设置音量
	//Return Value:
	//	-1		创建线程失败
	//	非负数	线程int类型的线程句柄，使用时请强转为HANDLE
	static int playSound(const char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	播放音效,仅播放一次
	//Paramter: 
	//	char * szFileNameTemp 要播放的文件名，文件名中不能有空格，如果有空格必须使用双引号
	//	unsigned short volume=800 音量数值[0,1000]，wav格式不支持设置音量
	//Return Value:
	//	-1		创建线程失败
	//	非负数	线程int类型的线程句柄，使用时请强转为HANDLE
	static int playSound(char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	播放音效,循环播放
	//Paramter: 
	//	const char* szFileNameTemp 要播放的文件名，文件名中不能有空格，如果有空格必须使用双引号
	//	unsigned short volume=800 音量数值[0,1000]，wav格式不支持设置音量
	//Return Value:
	//	-1		创建线程失败
	//	非负数	线程int类型的线程句柄，使用时请强转为HANDLE.
	static int playSoundLoop(const char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	播放音效,循环播放
	//Paramter: 
	//	char* szFileNameTemp 要播放的文件名，文件名中不能有空格，如果有空格必须使用双引号
	//	unsigned short volume=800 音量数值[0,1000]，wav格式不支持设置音量
	//Return Value:
	//	-1		创建线程失败
	//	非负数	线程int类型的线程句柄，使用时请强转为HANDLE.
	static int playSoundLoop(char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	通过文件名终止循环播放(不推荐)
	//Paramter: 
	//	const char* szTargetFileName 要停播的音效文件名，文件名中不能有空格，如果有空格必须使用双引号
	//Return Value:
	//	NONE
	static void stopPlay(const char* szTargetFileName);

	//Description:
	//	通过文件名终止循环播放(不推荐)
	//Paramter: 
	//	char* szTargetFileName 要停播的音效文件名，文件名中不能有空格，如果有空格必须使用双引号
	//Return Value:
	//	NONE
	static void stopPlay(char* szTargetFileName);

	//Description:
	//	通过线程句柄终止循环播放(推荐),在循环播放时会返回一个线程句柄
	//Paramter: 
	//	int hThread 要停播的线程句柄
	//Return Value:
	//	NONE
	static void stopPlay(int hThread);

	//Description:
	//	停止播放所有音效
	//Paramter: 
	//	NONE
	//Return Value:
	//	NONE
	static void stopPlayAll();
private:
	//static std::vector<_Sound> _listPlay;

	static DWORD WINAPI _Play(LPVOID lpParameter);
	static DWORD WINAPI _PlayLoop(LPVOID lpParameter);

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
	int z;
	Picture* pic;//该对象的图标

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

void AddDrawObject(Object* object);
void RemoveDrawObecjt(Object* object);
void freeSomethingForEngine();
void playMusic(const char* fileName);
void playMusic(char* fileName);