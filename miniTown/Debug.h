#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <cstdio>

//Description:
	//	输出调试调试信息，要要输出的内容长度不能超过2048字节(带换行符)
	//Paramter: 
	//	char const* const _Format 格式化字符串
	//	... 可变参数
	//Return Value:
	//	<0  错误
	//	>=0	输出的字符串长度
int DebugPrintln(_In_z_ _Printf_format_string_ char const* const _Format, ...);

//Description:
	//	输出调试调试信息，要要输出的内容长度不能超过2048字节
	//Paramter: 
	//	char const* const _Format 格式化字符串
	//	... 可变参数
	//Return Value:
	//	<0  错误
	//	>=0	输出的字符串长度
int DebugPrint(_In_z_ _Printf_format_string_ char const* const _Format, ...);
