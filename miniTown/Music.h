#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <ctime>
#include <string>
#include <mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib, "WINMM.LIB")
class Musci
{
public:
	std::string fileName;
	DWORD nLength;//音效长度 单位ms
	unsigned short volume;//音量0-1000
	bool isExit;
	bool isPlay;
	bool isLoop;//是否循环播放

	time_t id;
	HANDLE hThread;
	MCI_OPEN_PARMS MciOpen;
	MCI_PLAY_PARMS MciPlay;
	MCI_STATUS_PARMS MciStatus;
	MCI_DGV_SETAUDIO_PARMS MciSetAudio;
	MCI_SEEK_PARMS MciSeek;

	Musci(char* tszFileName, unsigned short tVolume=800);
	Musci(const char* tszFileName, unsigned short tVolume = 800);
	~Musci();

	void play();
	void stop();
	
	bool operator==(time_t id);
	bool operator==(HANDLE hThread);
	bool operator==(Musci* pMusic);
private:
	inline void _init();
};
