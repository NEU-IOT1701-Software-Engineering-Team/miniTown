#include"SoundManager.h"


int BackgroundMusicVol = 500;
int SoundEffectVol = 500;
bool endflag = false;

MCIDEVICEID wDevioceId;
SoundManager::~SoundManager()
{
	endflag = true;
	for (int i = 0; i < musicList.size(); i++)
	{
		musicList[i].join();
	}
	musicList.clear();
}


DWORD MCIPlay(DWORD DeviceID, MusicType musicType)
{
	MCI_PLAY_PARMS mciPP;
	DWORD opReturn;
	if (musicType == soundEffect)
	{
		opReturn = mciSendCommand(wDevioceId, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mciPP);
		return opReturn;
	}
	else if (musicType == backgroundMusic)
	{
		opReturn = mciSendCommand(wDevioceId, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD_PTR)&mciPP);
		return opReturn;
	}

}

void Close_MCI(DWORD DeviceID)
{
	DWORD retclose = mciSendCommand(DeviceID, MCI_CLOSE, NULL, NULL);
}

//调节音量
DWORD MCISetVolume(DWORD dwVol, DWORD DeviceID)
{
	MCI_DGV_SETAUDIO_PARMS mciSetAudioPara;
	mciSetAudioPara.dwCallback = NULL;
	mciSetAudioPara.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciSetAudioPara.dwValue = dwVol; //音量0-1000
	DWORD dwReturn = mciSendCommand(DeviceID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD_PTR)(LPVOID)&mciSetAudioPara);
	return dwReturn;
}



DWORD SOUND_RUN(const CString& strPath, MusicType musicType)
{
	//打开MCI设备
	MCI_OPEN_PARMS mciOP;
	DWORD opReturn;
	MCIDEVICEID wIDDevioce; //保存设备ID


	mciOP.lpstrDeviceType = NULL;
	mciOP.lpstrElementName = strPath; //设置打开的文件的名称
	opReturn = mciSendCommand
	(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD_PTR)(LPVOID)&mciOP);
	if (opReturn == 0)
	{
		wIDDevioce = mciOP.wDeviceID; //播放设备ID
		wDevioceId = mciOP.wDeviceID;

		//设置当前播放位置跳转60秒
		//MCISeek(100);


		//设置一下声音大小
		int vol;
		vol = (musicType == backgroundMusic) ? BackgroundMusicVol : SoundEffectVol;
		MCISetVolume(vol, wIDDevioce);

		//音频播放
		MCIPlay(wIDDevioce, musicType);
		while (1)
		{
			MCI_STATUS_PARMS mciStatus;
			mciStatus.dwItem = MCI_STATUS_MODE;
			DWORD sta2Return = mciSendCommand(wIDDevioce, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)(LPVOID)&mciStatus);
			DWORD music_playstyle = mciStatus.dwReturn; //获得当前播放工作模式 播放 526 暂停 525
			//cout << wIDDevioce << ":" << music_playstyle << endl;
			if (music_playstyle == 525)
			{
				if (musicType == soundEffect)
				{
					Close_MCI(wIDDevioce);
					return 0;  //播放完成
				}

			}
			//如果音乐声音大小发生更改，则修改音乐声音大小
			if (musicType == backgroundMusic)
			{
				if (vol != BackgroundMusicVol)
				{
					vol = BackgroundMusicVol;
					MCISetVolume(vol, wIDDevioce);
				}
			}
			else if (musicType == soundEffect)
			{
				if (vol != SoundEffectVol)
				{
					vol = SoundEffectVol;
					MCISetVolume(vol, wIDDevioce);
				}
			}
			if (endflag == true)  //看到终止标准位就退出
			{
				Close_MCI(wIDDevioce);
				return 0;
			}

			Sleep(100);
		}

	}
	else
		wIDDevioce = NULL;
	return opReturn;

}


void SoundManager::playSound(const CString& strPath, MusicType musicType)
{
	musicList.push_back(thread(SOUND_RUN, strPath, musicType));

}

void SoundManager::playSound(Sound& sound)
{
	musicList.push_back(thread(SOUND_RUN, sound.strPath, sound.musicType));

}


void SoundManager::SetBackgroundMusicVol(int vol)
{
	if (vol > 1000)
	{
		vol = 1000;
	}
	if (vol < 0)
	{
		vol = 0;
	}
	BackgroundMusicVol = vol;
}

void SoundManager::SetSoundEffectVol(int vol)
{
	if (vol > 1000)
	{
		vol = 1000;
	}
	if (vol < 0)
	{
		vol = 0;
	}
	SoundEffectVol = vol;
}

