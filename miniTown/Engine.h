#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <cstdlib>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include "Debug.h"

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
	int RotationDirection = 0; //走路旋转摇晃的方向
	/*
	z值越小，越先绘制
	z值相同，y越小越先绘制
	*/
	int z;
	Picture* pic;//该对象的图标

	Object() {
		memset(this, 0, sizeof(Object));
	}

	bool WalkTo(Object* object);

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
	/*
	bool isTransparent() {
		return (r == 255 && g == 255 && b == 255);
	}*/
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
	int nBitCount = 0;//每个像素所占比特数
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
		return nBitCount;
	}
	int getWidthBytes() {
		return nWidthBytes;
	}
	int getErrorCode() {
		return nErrorCode;
	}

	bool isIncludeAlpha() {
		return pChannelA != NULL;
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

		nBitCount = infoHeader.biBitCount;

		nImgSize = nWidth * nHeight;

		nWidthBytes = (((nWidth * nBitCount) + 31) >> 5) << 2;

		switch (infoHeader.biCompression) {
		case BI_RGB: {
			//不压缩
			if (nBitCount == 24) {
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
						fseek(fp, nWidthBytes - nWidth * nBitCount / 8, SEEK_CUR);
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
						fseek(fp, nWidthBytes - nWidth * nBitCount / 8, SEEK_CUR);
					}
				}
			}
			else {
				//32bit
				pChannelR = new BYTE[nImgSize];
				pChannelG = new BYTE[nImgSize];
				pChannelB = new BYTE[nImgSize];
				pChannelA = new BYTE[nImgSize];
				if (pChannelR == NULL || pChannelG == NULL || pChannelB == NULL || pChannelA == NULL) {
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
							if (fread(&pChannelA[i * nWidth + j], sizeof(BYTE), 1, fp) != 1) {
								nErrorCode = -5;
								return nErrorCode;
							}
						}
						fseek(fp, nWidthBytes - nWidth * nBitCount / 8, SEEK_CUR);
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
							if (fread(&pChannelA[i * nWidth + j], sizeof(BYTE), 1, fp) != 1) {
								nErrorCode = -5;
								return nErrorCode;
							}
						}
						fseek(fp, nWidthBytes - nWidth * nBitCount / 8, SEEK_CUR);
					}
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
			if (nBitCount == 32) {
				//32bit B8 G8 R8 A8
				pChannelR = new BYTE[nImgSize];
				pChannelG = new BYTE[nImgSize];
				pChannelB = new BYTE[nImgSize];
				pChannelA = new BYTE[nImgSize];
				if (pChannelR == NULL || pChannelG == NULL || pChannelB == NULL || pChannelA == NULL) {
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
							if (fread(&pChannelA[i * nWidth + j], sizeof(BYTE), 1, fp) != 1) {
								nErrorCode = -5;
								return nErrorCode;
							}
						}
						fseek(fp, nWidthBytes - nWidth * nBitCount / 8, SEEK_CUR);
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
							if (fread(&pChannelA[i * nWidth + j], sizeof(BYTE), 1, fp) != 1) {
								nErrorCode = -5;
								return nErrorCode;
							}
						}
						fseek(fp, nWidthBytes - nWidth * nBitCount / 8, SEEK_CUR);
					}
				}
			}
			
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

class BaseUI {
public:
	std::string title;
	RECT rect;
	Color currentBackgroundColor;

	bool enabled;//UI是否可用
	bool isVisible;//UI是否可用

	BaseUI() {
		_initALL();
	}

	BaseUI(char* tTitle, int x, int y, int width, int height) {
		_initALL();
		title = tTitle;
		rect = { x,y,x + width,y + height };
	}
	BaseUI(char* tTitle, RECT tRect) {
		_initALL();
		title = tTitle;
		rect = tRect;
	}
	BaseUI(char* tTitle, RECT tRect, Color tBackgroundColor, Color tForegroundColor) {
		_initALL();
		title = tTitle;
		rect = tRect;
		backgroundColor = tBackgroundColor;
		currentBackgroundColor = tBackgroundColor;
		foregroundColor = tForegroundColor;
	}
	BaseUI(char* tTitle, int x, int y, int width, int height, Color tBackgroundColor, Color tForegroundColor) {
		_initALL();
		title = tTitle;
		rect = { x,y,x + width,y + height };
		backgroundColor = tBackgroundColor;
		currentBackgroundColor = tBackgroundColor;
		foregroundColor = tForegroundColor;
	}


	//Description:
	//	设置设置UI标题,如果字符串以\0结束则可以忽略参数int length
	//Paramter: 
	//	char * szTitle	字符串首地址
	//	int length=-1	字符串长度
	//Return Value:
	//	NONE
	void setTitle(char * szTitle,int length=-1) {
		if (szTitle == NULL) {
			return;
		}
		title.clear();
		if (length != -1) {
			title.append(szTitle, length);
		}
		else {
			title.append(szTitle);
		}
	}
	//Description:
	//	设置设置UI标题,如果字符串以\0结束则可以忽略参数int length
	//Paramter: 
	//	const char * szTitle	字符串首地址
	//	int length=-1	字符串长度
	//Return Value:
	//	NONE
	void setTitle(const char* szTitle, int length = -1) {
		setTitle((char*)szTitle, length);
	}

	//Description:
	//	获取UI标题 返回指向字符串的首地址
	//Paramter: 
	//	NONE
	//Return Value:
	//	const char *
	const char* getTitle() {
		return title.c_str();
	}

	//Description:
	//	获取UI标题 通过参数返回指向字符串的首地址
	//Paramter: 
	//	char** pTitle
	//Return Value:
	//	int 标题长度
	int getTitle(char** pTitle) {
		*pTitle = (char*)title.c_str();
		return title.length();
	}

	//Description:
	//	获取UI标题 事先已分配好地址
	//Paramter: 
	//	char* szTitle 分配的首地址
	//	int length 分配的长度
	//Return Value:
	//	int 实际复制字符串长度
	int getTitle(char* szTitle, int length) {
		if (length >= title.length()) {
			//传入地址的长度足够
			memcpy(szTitle, title.c_str(), title.length() * sizeof(char));
			return  title.length();
		}
		else {
			//不足
			memcpy(szTitle, title.c_str(), length * sizeof(char));
			return  length;
		}

	}

	//Description:
	//	获取UI标题长度
	//Paramter: 
	//	NONE
	//Return Value:
	//	int 长度
	int getTitleLength() {
		return  title.length();
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

	void setPoint(Point point) {
		rect = { point.x,point.y, rect.right ,rect.bottom };
	}
	Point getPoint() {
		return Point(rect.left, rect.top);
	}

	void setRect(RECT tRect) {
		rect = tRect;
	}
	void setRect(int width, int height) {
		rect = { rect.left,rect.top,rect.left + width,rect.top + height };
	}
	void setRect(int x, int y, int width, int height) {
		rect = { x,y,x + width,y + height };
	}
	RECT getRect() {
		return rect;
	}
	
	void setWidth(int width) {
		rect.right = rect.left + width;
	}
	int getWidth() {
		return rect.right - rect.left;
	}

	void setHeight(int height) {
		rect.bottom = rect.top + height;
	}
	int getHeight() {
		return rect.bottom - rect.top;
	}

	virtual void Draw(HDC hMemDC) {
		;
	}

	//判断被点击的点是否再rect内
	bool operator==(Point point) {
		return point.x <= rect.right && point.x >= rect.left && point.y <= rect.bottom && point.y >= rect.top;
	}

private:
	Color foregroundColor;
	Color backgroundColor;

	inline void _initALL() {
		//memset(this, 0, sizeof(*this));
		currentBackgroundColor = DEFAULT_BC;
		foregroundColor = DEFAULT_FC;
		backgroundColor = DEFAULT_BC;
		enabled = true;
		isVisible = true;
		rect = { 0,0,100,50 };
	}
};

//UI:按钮
class Button :public BaseUI{
public:
	void (*lpDoubleClickL)(void);
	void (*lpClickL)(void);//左键单击事件
	void (*lpClickR)(void);
	bool isClick;

	Button() {
		_initALL();
	}
	Button(char* tTitle, int x, int y, int width, int height) :
		BaseUI(tTitle,x,y,width, height){
		_initALL();
	}
	Button(char* tTitle, RECT tRect):
		BaseUI(tTitle,tRect) {
		_initALL();
	}
	Button(char* tTitle, RECT tRect, Color tBackgroundColor, Color tForegroundColor) :
		BaseUI(tTitle, tRect, tBackgroundColor, tForegroundColor){
		_initALL();
	}
	Button(char* tTitle, int x, int y, int width, int height, Color tBackgroundColor, Color tForegroundColor):
		BaseUI(tTitle,x, y, width, height, tBackgroundColor, tForegroundColor) {
		_initALL();
	}
	Button(char* tTitle, RECT tRect, Color tBackgroundColor, Color tForegroundColor, void (*tlpClickL)(void)):
		BaseUI(tTitle, tRect, tBackgroundColor, tForegroundColor) {
		_initALL();
		lpClickL = tlpClickL;
	}

	void Draw(HDC hMemDC);
private:

	inline void _initALL() {
		lpDoubleClickL = NULL;
		lpClickL = NULL;
		lpClickR = NULL;
		isClick = false;
	}
};

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

//UI: 标签
class Label : public BaseUI {
public :
	//Description:
	//	默认构造函数。
	//Paramter: 
	//	NONE
	//Return Value:
	//	NONE
	Label() {
		_initALL();
	}

	//Description:
	//	默认构造函数。
	//Paramter: 
	//	char* tTitle 标签标题
	//	RECT tRect 标签大小以及位置
	//Return Value:
	//	NONE
	Label(char* tTitle, RECT tRect) :
		BaseUI(tTitle, tRect) {
		_initALL();
	}

	//Description:
	//	默认构造函数。
	//Paramter: 
	//	char* tTitle 标签标题
	//	int x, int y 标签位置
	//	int width, int height 标签大小
	//Return Value:
	//	NONE
	Label(char* tTitle, int x, int y, int width, int height) :
		BaseUI(tTitle, x, y, width, height) {
		_initALL();
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
	Label(char* tTitle, RECT tRect, Color tBackgroundColor, Color tForegroundColor) :
		BaseUI(tTitle, tRect, tBackgroundColor, tForegroundColor) {
		_initALL();
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
	Label(char* tTitle, int x, int y, int width, int height, Color tBackgroundColor, Color tForegroundColor) :
		BaseUI(tTitle, x, y, width, height, tBackgroundColor, tForegroundColor) {
		_initALL();
	}
	void Draw(HDC hMemDC);
private:
	inline void _initALL() {
		;
	}
};

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

constexpr auto EB_DEFAULT_TITLE = "EditBox";

//UI: 编辑框
class EditBox :public BaseUI {
public :
	std::string text;//编辑框内容
	int nPosCaret;//光标位置
	Point pointCaret;//光标坐标
	bool isShowCaret;//是否显示光标
	int sumFPS;//光标闪烁
	bool stateCaret;//光标当前状态
#define EB_IT_TEXT		0
#define EB_IT_NUMBER	1

#define EB_IT_RANGE		2//不使用
	int inputType;
	void (*lpValueChange)(void);//当编辑框内容改变时 调用
	
	//Description:
	//	默认构造函数。
	//Paramter: 
	//	NONE
	//Return Value:
	//	NONE
	EditBox() {
		_initALL();
	}

	//Description:
	//	默认构造函数。
	//Paramter: 
	//	RECT tRect UI大小以及位置
	//Return Value:
	//	NONE
	EditBox(RECT tRect) :
		BaseUI((char*)EB_DEFAULT_TITLE, tRect) {
		_initALL();
	}

	//Description:
	//	默认构造函数。
	//Paramter: 
	//	int x, int y UI位置
	//	int width, int height UI大小
	//Return Value:
	//	NONE
	EditBox( int x, int y, int width, int height) :
		BaseUI((char*)EB_DEFAULT_TITLE, x, y, width, height) {
		_initALL();
	}

	//Description:
	//	默认构造函数。
	//Paramter: 
	//	RECT tRect UI大小以及位置
	//	Color tBackgroundColor 背景色
	//	Color tForegroundColor 前景色（文字颜色）
	//Return Value:
	//	NONE
	EditBox( RECT tRect, Color tBackgroundColor, Color tForegroundColor) :
		BaseUI((char*)EB_DEFAULT_TITLE, tRect, tBackgroundColor, tForegroundColor) {
		_initALL();
	}

	//Description:
	//	默认构造函数。
	//Paramter: 
	//	int x, int y UI位置
	//	int width, int height UI大小
	//	Color tBackgroundColor 背景色
	//	Color tForegroundColor 前景色（文字颜色）
	//Return Value:
	//	NONE
	EditBox( int x, int y, int width, int height, Color tBackgroundColor, Color tForegroundColor) :
		BaseUI((char*)EB_DEFAULT_TITLE, x, y, width, height, tBackgroundColor, tForegroundColor) {
		_initALL();
	}

	//Description:
	//	默认构造函数。
	//Paramter: 
	//	RECT tRect UI大小以及位置
	//	Color tBackgroundColor 背景色
	//	Color tForegroundColor 前景色（文字颜色）
	//	void (*tlpValueChange)(void) UI事件
	//Return Value:
	//	NONE
	EditBox(RECT tRect, Color tBackgroundColor, Color tForegroundColor, void (*tlpValueChange)(void)) :
		BaseUI((char*)EB_DEFAULT_TITLE, tRect, tBackgroundColor, tForegroundColor) {
		_initALL();
		lpValueChange = tlpValueChange;
	}

	//Description:
	//	设置编辑框输入类型
	//Paramter: 
	//	int tInputType 详见EB_IT_
	//Return Value:
	//	NONE
	void setInputType(int tInputType) {
		if (tInputType < EB_IT_RANGE) {
			inputType = tInputType;
		}
	}

	//Description:
	//	获取编辑框输入类型
	//Paramter: 
	//	NONE
	//Return Value:
	//	int 详见EB_IT_
	int getInputType() {
		return inputType;
	}

	//Description:
	//	获取编辑框内容 返回指向字符串的首地址
	//Paramter: 
	//	NONE
	//Return Value:
	//	const char *
	const char* getText() {
		return text.c_str();
	}

	//Description:
	//	获取编辑框内容 通过参数返回指向字符串的首地址
	//Paramter: 
	//	char ** pValue
	//Return Value:
	//	int 字符串长度
	int getText(char** pValue) {
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
			memcpy(value, text.c_str(), text.length() * sizeof(char));
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
	int getTextLength() {
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
		text.append(str);
	}
	void setText(const char* str) {
		text.clear();
		text.append(str);
	}
	void moveCaret(int nOff) {
		nPosCaret += nOff;
		if (nPosCaret < 0) {
			nPosCaret = 0;
			//pointCaret.x = 0;
		}
		else if (nPosCaret > text.length()) {
			nPosCaret = text.length();
		}
	}

	void Draw(HDC hMemDC);
private:
	void _initALL() {
		nPosCaret = 0;//光标位置
		isShowCaret = true;//是否显示光标
		sumFPS = 0;//光标闪烁
		stateCaret = false;//光标当前状态
		inputType = 0;
		lpValueChange = NULL;
	}
};


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