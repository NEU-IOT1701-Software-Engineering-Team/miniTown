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

	//�洢������Ϣ
	struct _Sound {
		HANDLE hThread;
		MCI_OPEN_PARMS MciOpen;
		MCI_PLAY_PARMS MciPlay;
		MCI_STATUS_PARMS MciStatus;
		//MCI_SET_PARMS MciSet;
		MCI_DGV_SETAUDIO_PARMS MciSetAudio;
		DWORD nLength;//��Ч���� ��λms
		bool isStop;//�Ƿ�ֹͣ����
		unsigned short volume;//����0-1000
		char* szFileName;
		time_t id;
		struct _Sound() {
			memset(this, 0, sizeof(struct _Sound));
		}
		//�Ƽ�ʹ��
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

	//Player���õ�����Ϣ�ṹ��
	//�������߳���Player����Ϣ����
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
	//	������Ч,������һ��.
	//Paramter: 
	//	const char* szFileNameTemp Ҫ���ŵ��ļ������ļ����в����пո�����пո����ʹ��˫����
	//	unsigned short volume=800 ������ֵ[0,1000]��wav��ʽ��֧����������
	//Return Value:
	//	-1		�����߳�ʧ��
	//	�Ǹ���	�߳�int���͵��߳̾����ʹ��ʱ��ǿתΪHANDLE
	static void playSound(const char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	������Ч,������һ��
	//Paramter: 
	//	char * szFileNameTemp Ҫ���ŵ��ļ������ļ����в����пո�����пո����ʹ��˫����
	//	unsigned short volume=800 ������ֵ[0,1000]��wav��ʽ��֧����������
	//Return Value:
	//	-1		�����߳�ʧ��
	//	�Ǹ���	�߳�int���͵��߳̾����ʹ��ʱ��ǿתΪHANDLE
	static void playSound(char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	������Ч,ѭ������
	//Paramter: 
	//	const char* szFileNameTemp Ҫ���ŵ��ļ������ļ����в����пո�����пո����ʹ��˫����
	//	unsigned short volume=800 ������ֵ[0,1000]��wav��ʽ��֧����������
	//Return Value:
	//	-1		�����߳�ʧ��
	//	�Ǹ���	�߳�int���͵��߳̾����ʹ��ʱ��ǿתΪHANDLE.
	static void playSoundLoop(const char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	������Ч,ѭ������
	//Paramter: 
	//	char* szFileNameTemp Ҫ���ŵ��ļ������ļ����в����пո�����пո����ʹ��˫����
	//	unsigned short volume=800 ������ֵ[0,1000]��wav��ʽ��֧����������
	//Return Value:
	//	-1		�����߳�ʧ��
	//	�Ǹ���	�߳�int���͵��߳̾����ʹ��ʱ��ǿתΪHANDLE.
	static void playSoundLoop(char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	ͨ���ļ�����ֹѭ������(���Ƽ�)
	//Paramter: 
	//	const char* szTargetFileName Ҫͣ������Ч�ļ������ļ����в����пո�����пո����ʹ��˫����
	//Return Value:
	//	NONE
	//static void stopPlay(const char* szTargetFileName);

	//Description:
	//	ͨ���ļ�����ֹѭ������(���Ƽ�)
	//Paramter: 
	//	char* szTargetFileName Ҫͣ������Ч�ļ������ļ����в����пո�����пո����ʹ��˫����
	//Return Value:
	//	NONE
	//static void stopPlay(char* szTargetFileName);

	//Description:
	//	ͨ���߳̾����ֹѭ������(�Ƽ�),��ѭ������ʱ�᷵��һ���߳̾��
	//Paramter: 
	//	int hThread Ҫͣ�����߳̾��
	//Return Value:
	//	NONE
	static void stopPlay(int hThread);

	//Description:
	//	ͨ���洢������Ϣָ����ֹ����
	//Paramter: 
	//	Player::_Sound* pSound �洢������Ϣָ��
	//Return Value:
	//	NONE
	static void stopPlay(Player::_Sound* pSound);

	//Description:
	//	ֹͣ����������Ч
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
		//����
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
	float x;//Ϊ�˼���֮ǰ�İ汾 ���Ƽ�ʹ��23333
	float y;//Ϊ�˼���֮ǰ�İ汾 ���Ƽ�ʹ��23333
	Point point;

	/*
	zֵԽС��Խ�Ȼ���
	zֵ��ͬ��yԽСԽ�Ȼ���
	*/
	int z;
	Picture* pic;//�ö����ͼ��

	Object() {
		memset(this, 0, sizeof(Object));
	}

	void WalkTo(Object* object);

	//Description:
	//	����ͼƬ��ת�Ƕ�
	//Paramter: 
	//	int angle ��Ҫ��ת�ĽǶ�.(�Ƕ���)
	//Return Value:
	//	NONE
	void setAngle(int angle) {

		if (abs(angle) >= 360) {
			angle %= 360;
		}
		this->angle = ((float)angle) * PI / 180;
	}

	//Description:
	//	����ͼƬ��ת�Ƕ�
	//Paramter: 
	//	float radian ��Ҫ��ת�ĽǶ�.(������)
	//Return Value:
	//	NONE
	void setAngle(float radian) {
		if (abs(radian) >= 2 * PI) {//�Ƕȴ���2pi ��Ҫ��������
			//1������ת�Ƕ�
			//2���Ƕ�����
			//3���Ƕ�ת����
			angle = (float(((int(radian * 180 / PI))) % 360)) * PI / 180;
		}
		else {
			angle = radian;
		}
	}

	//Description:
	//	����ͼƬ��ת�Ƕ�(�Ƕ���).
	//Paramter: 
	//	NONE
	//Return Value:
	//	int ����ͼƬ��ת�Ƕ�(�Ƕ���).
	int getAngle() {
		return int(angle * 180 / PI);
	}

	//Description:
	//	����ͼƬ��ת�Ƕ�(�Ƕ���).
	//Paramter: 
	//	BYTE none �޹ز��������Լ��ɡ���Ϊ������.
	//Return Value:
	//	float ����ͼƬ��ת�Ƕ�(�Ƕ���).
	float getAngle(BYTE none) {
		return angle;
	}

	//Ϊ�˼���֮ǰ�İ汾
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
	float angle = 0.0f;//ͼ�����ת�Ƕ�(������)
};

struct ObjectPointer {
public:
	Object* pObject;//һ��ָ��ʵ��object�����ָ��

	struct ObjectPointer() {
		pObject = NULL;
	}

	struct ObjectPointer(Object* tpObject) {
		pObject = tpObject;
	}

	//Description:
	//	����<��������ʵ�ֶ�object�İ�Zֵ������
	//Paramter: 
	//	class ObjectPointer obj2 ��һ���Ƚ϶���
	//Return Value:
	//	bool �ȽϽ��
	bool operator<(struct ObjectPointer obj2) {
		if (this->pObject == NULL || obj2.pObject == NULL) {
			return false;
		}
		this->pObject->updatePoint();
		obj2.pObject->updatePoint();
		
		//��Ⱦ����
		//obj1 obj2 ��z������-1ʱ		yԽСԽ�Ȼ�Խ���ײ�
		//obj1 obj2 ֻ��һ��z���� - 1ʱ	z= - 1���Ȼ�
		//obj1 obj2 ��z�������� - 1ʱ		yԽСԽ�Ȼ�Խ���ײ� y��ͬʱ zԽСԽ�Ȼ�Խ���ײ�

		//�жϵ�һ�������zֵ�Ƿ�Ϊ-1
		if (this->pObject->z == -1) {
			//�жϵڶ��������zֵ�Ƿ�Ϊ-1
			if (obj2.pObject->z == -1) {
				return this->pObject->point.y < obj2.pObject->point.y;
			}
			else {
				//z1==-1 && z2!=-1
				return true;//this �� obj2��� this�Ȼ�
			}
		}
		else if (obj2.pObject->z == -1) {
			if (this->pObject->z == -1) {
				return this->pObject->point.y < obj2.pObject->point.y;
			}
			else {
				//z1!=-1 && z2==-1
				return false;//this �� obj2��� obj�Ȼ�
			}
		}
		else {
			//������߶�ûz=-1  //y����
			if (this->pObject->point.y == obj2.pObject->point.y) {
				//yֵ��ͬ
				return this->pObject->z < obj2.pObject->z;
			}
			else {
				return this->pObject->point.y < obj2.pObject->point.y;
			}
		}
	}

	//Description:
	//	����==��������ʵ�ֶ�object�Ƚϣ��ж��Ƿ���ͬһ������
	//Paramter: 
	//	class ObjectPointer obj2 ��һ���Ƚ϶���
	//Return Value:
	//	bool �ȽϽ��
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
	int nDataSize = 0;//���ݴ�С
	int nImgSize = 0;
	int nWidth = 0;
	int nHeight = 0;
	int nWidthBytes = 0;//ÿ���ֽ���
	int nBitsPixel = 0;//ÿ��������ռλ��
	bool isBackward = true;
	Point centerPoint;

	/*
	���� BGRA��˳������
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
	//	�ӱ��ؼ���ͼƬ���ڴ�
	//Paramter: 
	//	char* szPathҪ���ص�ͼƬ·��,���Բ����Ϸ��Խ��м��.
	//Return Value:
	//	0  ����
	//	-1 ��ȡ��Ϣͷ����!
	//	-2 λͼ�ļ����ͷǷ�biCompression
	//	-3 λͼ�ļ���ʧ��
	//	-4 �ڴ治��
	//	-5 ��ȡ���ݴ���
	//	-6  ��ȡ�ļ�ͷ����!
	int loadImage(char* szPath) {
		return _loadImage(szPath);
	}
	//Description:
	//	�ӱ��ؼ���ͼƬ���ڴ�
	//Paramter: 
	//	char* szPathҪ���ص�ͼƬ·��,���Բ����Ϸ��Խ��м��.
	//Return Value:
	//	0  ����
	//	-1 ��ȡ��Ϣͷ����!
	//	-2 λͼ�ļ����ͷǷ�biCompression
	//	-3 λͼ�ļ���ʧ��
	//	-4 �ڴ治��
	//	-5 ��ȡ���ݴ���
	//	-6  ��ȡ�ļ�ͷ����!
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
			return pChannelA[0];//��Ӧ�ó���
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
			return pChannelA[0];//��Ӧ�ó���
		}
	}
private:
	int _loadImage(char* szPath) {
		/*
		if (hImage != NULL) {
			DeleteObject(hImage);
		}
		//���ļ�����λͼ
		hImage = (HBITMAP)LoadImage(0, szPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		//��ȡ����
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
			//����
			isBackward = false;
			nHeight = -infoHeader.biHeight;
		}
		else {
			//����
			isBackward = true;
			nHeight = infoHeader.biHeight;
		}
		//����ͼ�����ĵ�
		centerPoint.x = nWidth / 2;
		centerPoint.y = nHeight / 2;

		switch (infoHeader.biCompression) {
		case BI_RGB: {
			//��ѹ��
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
				//����
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
			//8bit rleѹ��
			//infoHeader.biBitCount=8
			break;
		}
		case BI_RLE4: {
			//4bit rleѹ��
			//infoHeader.biBitCount=4
			break;
		}
		case BI_BITFIELDS: {
			//����������16/32λ
			break;
		}
		case BI_JPEG: {
			//����jpegͼ��
			break;
		}
		case BI_PNG: {
			//����pngͼ��
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
//	������ά��һ������,Ŀǰ���Բ����Ϸ��Խ��м��.
//	�ô�����ʾ������������window�ṹ������ָ���img
//Paramter: 
//	LPVOID windowһ��ָ��Window�ṹ���ָ��
//Return:
//	0:		����
//	other:	��GetLastError()���صĴ������
int _CreateWindow(const char* title, int nWidth, int nHeight);

//Description:
//	�����ػ�����
//	������Ϣ
//Paramter: 
//	NONE
//Return:
//	NONE
void Draw();

//Description:
//	����ƶ����м���һ���¶���,����ö����ѱ����룬�򲻻��ظ�����.
//Paramter: 
//	Object* object ��Ҫ�������ĵ�ַ
//Return Value:
//	NONE
void AddDrawObject(Object* object);

//Description:
//	�ӻ��ƶ�����ɾ��һ������
//Paramter: 
//	Object* object ��Ҫɾ���Ķ����ַ
//Return Value:
//	NONE
void RemoveDrawObecjt(Object* object);


#define DEFAULT_BC			COLOR_WHITE
#define DEFAULT_FC			COLOR_BLACK
#define DEFAULT_FOCUS_BC	{201,218,248}
#define DEFAULT_DOWN_BC		{109,158,235}
//UI ��ť
struct Button
{
	char* title;
	RECT rect;
	Color currentBackgroundColor;
	void (*lpDoubleClickL)(void);
	void (*lpClickL)(void);
	void (*lpClickR)(void);
	bool isClick;
	bool enabled;//��ť�Ƿ����
	bool isVisible;//��ť�Ƿ����

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

	//�޸İ�ťλ��
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

	//�жϱ�����ĵ��Ƿ���rect��
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
//	����һ����ť����,����ö����ѱ����룬�򲻻��ظ�����.
//Paramter: 
//	Button * button ��Ҫ�������ĵ�ַ
//Return Value:
//	NONE
void AddButton(Button* button);

//Description:
//	�Ӱ�ť�����б���ɾ��һ������
//Paramter: 
//	Button * button ��Ҫɾ���Ķ����ַ
//Return Value:
//	NONE
void RemoveButton(Button* button);


//UI ��ǩ
struct Label {
	char* title;
	RECT rect;
	Color currentBackgroundColor;
	//��ʱ����Ҫ��Ӧ�¼�

	bool enabled;//�Ƿ����
	bool isVisible;//�Ƿ�ɼ�

	//Description:
	//	Ĭ�Ϲ��캯����
	//Paramter: 
	//	NONE
	//Return Value:
	//	NONE
	struct Label() {
		_initALL();
	}
	
	//Description:
	//	Ĭ�Ϲ��캯����
	//Paramter: 
	//	char* tTitle ��ǩ����
	//	RECT tRect ��ǩ��С�Լ�λ��
	//Return Value:
	//	NONE
	struct Label(char* tTitle, RECT tRect) {
		_initALL();
		title = tTitle;
		rect = tRect;
	}

	//Description:
	//	Ĭ�Ϲ��캯����
	//Paramter: 
	//	char* tTitle ��ǩ����
	//	int x, int y ��ǩλ��
	//	int width, int height ��ǩ��С
	//Return Value:
	//	NONE
	struct Label(char* tTitle, int x, int y, int width, int height) {
		_initALL();
		title = tTitle;
		rect = { x,y,x + width,y + height };
	}

	//Description:
	//	Ĭ�Ϲ��캯����
	//Paramter: 
	//	char* tTitle ��ǩ����
	//	RECT tRect ��ǩ��С�Լ�λ��
	//	Color tBackgroundColor ����ɫ
	//	Color tForegroundColor ǰ��ɫ��������ɫ��
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
	//	Ĭ�Ϲ��캯����
	//Paramter: 
	//	char* tTitle ��ǩ����
	//	int x, int y ��ǩλ��
	//	int width, int height ��ǩ��С
	//	Color tBackgroundColor ����ɫ
	//	Color tForegroundColor ǰ��ɫ��������ɫ��
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
	//	���ñ�����ɫ��
	//Paramter: 
	//	Color color ������ɫ
	//Return Value:
	//	NONE
	void setBackgroundColor(Color color) {
		backgroundColor = color;
		currentBackgroundColor = color;
	}

	//Description:
	//	���ñ�����ɫ��
	//Paramter: 
	//	BYTE r, BYTE g, BYTE b ������ɫ
	//Return Value:
	//	NONE
	void setBackgroundColor(BYTE r, BYTE g, BYTE b) {
		backgroundColor = { r,g,b };
		currentBackgroundColor = { r,g,b };
	}

	//Description:
	//	��ȡ������ɫ��
	//Paramter: 
	//	NONE
	//Return Value:
	//	Color ������ɫ
	Color getBackgroundColor() {
		return backgroundColor;
	}

	//Description:
	//	��ȡǰ����ɫ��
	//Paramter: 
	//	NONE
	//Return Value:
	//	Color ǰ����ɫ
	void setForegroundColor(Color color) {
		foregroundColor = color;
	}

	//Description:
	//	����ǰ����ɫ��
	//Paramter: 
	//	BYTE r, BYTE g, BYTE b ��ɫ
	//Return Value:
	//	NONE
	void setForegroundColor(BYTE r, BYTE g, BYTE b) {
		foregroundColor = { r,g,b };
	}

	//Description:
	//	��ȡǰ����ɫ��
	//Paramter: 
	//	NONE
	//Return Value:
	//	Color ǰ����ɫ
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
//	����һ����ǩ����,����ö����ѱ����룬�򲻻��ظ�����.
//Paramter: 
//	Label* label ��Ҫ�������ĵ�ַ
//Return Value:
//	NONE
void AddLabel(Label* label);

//Description:
//	�ӱ�ǩ�����б���ɾ��һ������
//Paramter: 
//	Label* label ��Ҫɾ���Ķ����ַ
//Return Value:
//	NONE
void RemoveLabel(Label* label);


//UI �༭��
struct EditBox : public Label
{
	std::string text;
	int nPosCaret;//���λ��
	Point pointCaret;//�������
	bool isShowCaret;//�Ƿ���ʾ���
	int sumFPS;//�����˸
	bool stateCaret;//��굱ǰ״̬
	
	//Description:
	//	Ĭ�Ϲ��캯����
	//Paramter: 
	//	NONE
	//Return Value:
	//	NONE
	EditBox() {
		_initAll();
	}

	//Description:
	//	��ȡ�༭������ ����ָ���ַ������׵�ַ
	//Paramter: 
	//	NONE
	//Return Value:
	//	const char *
	const char * getText() {
		return text.c_str();
	}

	//Description:
	//	��ȡ�༭������ ͨ����������ָ���ַ������׵�ַ
	//Paramter: 
	//	char ** pValue
	//Return Value:
	//	int �ַ�������
	int getText(char ** pValue) {
		*pValue = (char*)text.c_str();
		return text.length();
	}

	//Description:
	//	��ȡ�༭������ �����ѷ���õ�ַ
	//Paramter: 
	//	char* value ������׵�ַ
	//	int length ����ĳ���
	//Return Value:
	//	int ʵ�ʸ����ַ�������
	int getText(char* value, int length) {
		if (length >= text.length()) {
			//�����ַ�ĳ����㹻
			memcpy(value, text.c_str(), text.length() *sizeof(char));
			return  text.length();
		}
		else {
			//����
			memcpy(value, text.c_str(), length * sizeof(char));
			return  length;
		}
		
	}

	//Description:
	//	��ȡ�༭�����ݳ���
	//Paramter: 
	//	NONE
	//Return Value:
	//	int ����
	int getLength() {
		return  text.length();
	}

	//Description:
	//	��ձ༭������
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

	//�жϱ�����ĵ��Ƿ���rect��
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
//	����һ���༭�����,����ö����ѱ����룬�򲻻��ظ�����.
//Paramter: 
//	EditBox* editBox ��Ҫ�������ĵ�ַ
//Return Value:
//	NONE
void AddEditBox(EditBox* editBox);

//Description:
//	�ӱ༭������б���ɾ��һ������
//Paramter: 
//	EditBox* editBox ��Ҫɾ���Ķ����ַ
//Return Value:
//	NONE
void RemoveEditBox(EditBox* editBox);


void freeSomethingForEngine();