#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>
#include <mutex>
#include"Music.h"

#define timeout 1000
#define ResponseTime 100

class Player
{
public:
	
	
	Player();
	~Player();

	static void addMusci(Musci* music);

private:
	
	static DWORD WINAPI _Play(LPVOID lpParameter);
	static void _print(Musci* pMusic, _In_z_ _Printf_format_string_ char const* const _Format, ...);
};
