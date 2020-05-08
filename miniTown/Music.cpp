#include "Music.h"

Musci::Musci(char* tszFileName, unsigned short tVolume)
{
	fileName = tszFileName;
	volume = tVolume;
	MciOpen.lpstrElementName = fileName.c_str();
	isExit = false;
	isPlay = false;
	isLoop = false;//�Ƿ�ѭ������
}
Musci::Musci(const char* tszFileName, unsigned short tVolume)
{
	fileName = tszFileName;
	volume = tVolume;
	MciOpen.lpstrElementName = fileName.c_str();
	isExit = false;
	isPlay = false;
	isLoop = false;//�Ƿ�ѭ������
}

Musci::~Musci()
{
	this->isExit = true;
	this->isLoop = false;
	this->isPlay = false;
	WaitForSingleObject(hThread, 30*1000);
	CloseHandle(hThread);
}

void Musci::play()
{
	this->isPlay = true;
}

void Musci::stop()
{
	this->isPlay = false;
}

bool Musci::operator==(time_t id)
{
	return this->id == id;
}

bool Musci::operator==(HANDLE hThread)
{
	return this->hThread == hThread;
}

bool Musci::operator==(Musci* pMusic)
{
	return this->id == pMusic->id;
}

inline void Musci::_init()
{
	;
}
