#pragma once

#include<Windows.h>
#include<digitalv.h>
#include<mmsystem.h>
#include<conio.h>
#include<Mmsystem.h>
#include<string>
#include<iostream>
#include<atlstr.h>
#include<thread>
#include<vector>
#pragma comment(lib,"WINMM.LIB")

//²Î¿¼×ÊÁÏ https://www.cnblogs.com/kuangke/p/6626845.html?tdsourcetag=s_pctim_aiomsg

using namespace std;
extern MCIDEVICEID wDevioceId;


extern int BackgroundMusicVol;
extern  int SoundEffectVol;
extern bool endflag;
enum MusicType { backgroundMusic, soundEffect };


struct Sound
{
	CString strPath;
	MusicType musicType;
	Sound(CString str, MusicType musicTYPE)
	{
		strPath = str;
		musicType = musicTYPE;
	}
};

class SoundManager
{
public:
	void playSound(Sound& sound);
	void playSound(const CString& strPath, MusicType musicType);
	void SetBackgroundMusicVol(int vol);
	void SetSoundEffectVol(int vol);

	vector<thread> musicList;

	~SoundManager();
};
