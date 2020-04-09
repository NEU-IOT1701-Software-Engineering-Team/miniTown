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
		DWORD nLength;//��Ч���� ��λms
		bool isStop;//�Ƿ�ֹͣ����
		unsigned short volume;//����0-1000
		char* szFileName;
		_Sound() {
			memset(this, 0, sizeof(struct _Sound));
		}
		//���Ƽ�ʹ��
		bool operator==(char* szTargetFileName) {
			return (strcmp(szFileName, szTargetFileName) == 0);
		}
		//�Ƽ�ʹ��
		bool operator==(int hThread) {
			return this->hThread == (HANDLE)hThread;
		}
		void stop() {
			isStop = true;
		}
	};
	typedef struct _Sound _Sound;

	//Description:
	//	������Ч,������һ��.
	//Paramter: 
	//	const char* szFileNameTemp Ҫ���ŵ��ļ������ļ����в����пո�����пո����ʹ��˫����
	//	unsigned short volume=800 ������ֵ[0,1000]��wav��ʽ��֧����������
	//Return Value:
	//	-1		�����߳�ʧ��
	//	�Ǹ���	�߳�int���͵��߳̾����ʹ��ʱ��ǿתΪHANDLE
	static int playSound(const char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	������Ч,������һ��
	//Paramter: 
	//	char * szFileNameTemp Ҫ���ŵ��ļ������ļ����в����пո�����пո����ʹ��˫����
	//	unsigned short volume=800 ������ֵ[0,1000]��wav��ʽ��֧����������
	//Return Value:
	//	-1		�����߳�ʧ��
	//	�Ǹ���	�߳�int���͵��߳̾����ʹ��ʱ��ǿתΪHANDLE
	static int playSound(char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	������Ч,ѭ������
	//Paramter: 
	//	const char* szFileNameTemp Ҫ���ŵ��ļ������ļ����в����пո�����пո����ʹ��˫����
	//	unsigned short volume=800 ������ֵ[0,1000]��wav��ʽ��֧����������
	//Return Value:
	//	-1		�����߳�ʧ��
	//	�Ǹ���	�߳�int���͵��߳̾����ʹ��ʱ��ǿתΪHANDLE.
	static int playSoundLoop(const char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	������Ч,ѭ������
	//Paramter: 
	//	char* szFileNameTemp Ҫ���ŵ��ļ������ļ����в����пո�����пո����ʹ��˫����
	//	unsigned short volume=800 ������ֵ[0,1000]��wav��ʽ��֧����������
	//Return Value:
	//	-1		�����߳�ʧ��
	//	�Ǹ���	�߳�int���͵��߳̾����ʹ��ʱ��ǿתΪHANDLE.
	static int playSoundLoop(char* szFileNameTemp, unsigned short volume = 800);

	//Description:
	//	ͨ���ļ�����ֹѭ������(���Ƽ�)
	//Paramter: 
	//	const char* szTargetFileName Ҫͣ������Ч�ļ������ļ����в����пո�����пո����ʹ��˫����
	//Return Value:
	//	NONE
	static void stopPlay(const char* szTargetFileName);

	//Description:
	//	ͨ���ļ�����ֹѭ������(���Ƽ�)
	//Paramter: 
	//	char* szTargetFileName Ҫͣ������Ч�ļ������ļ����в����пո�����пո����ʹ��˫����
	//Return Value:
	//	NONE
	static void stopPlay(char* szTargetFileName);

	//Description:
	//	ͨ���߳̾����ֹѭ������(�Ƽ�),��ѭ������ʱ�᷵��һ���߳̾��
	//Paramter: 
	//	int hThread Ҫͣ�����߳̾��
	//Return Value:
	//	NONE
	static void stopPlay(int hThread);

	//Description:
	//	ֹͣ����������Ч
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
	int z;
	Picture* pic;//�ö����ͼ��

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

void AddDrawObject(Object* object);
void RemoveDrawObecjt(Object* object);
void freeSomethingForEngine();
void playMusic(const char* fileName);
void playMusic(char* fileName);