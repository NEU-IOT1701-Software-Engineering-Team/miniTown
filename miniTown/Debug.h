#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <cstdio>

//Description:
	//	������Ե�����Ϣ��ҪҪ��������ݳ��Ȳ��ܳ���2048�ֽ�(�����з�)
	//Paramter: 
	//	char const* const _Format ��ʽ���ַ���
	//	... �ɱ����
	//Return Value:
	//	<0  ����
	//	>=0	������ַ�������
int DebugPrintln(_In_z_ _Printf_format_string_ char const* const _Format, ...);

//Description:
	//	������Ե�����Ϣ��ҪҪ��������ݳ��Ȳ��ܳ���2048�ֽ�
	//Paramter: 
	//	char const* const _Format ��ʽ���ַ���
	//	... �ɱ����
	//Return Value:
	//	<0  ����
	//	>=0	������ַ�������
int DebugPrint(_In_z_ _Printf_format_string_ char const* const _Format, ...);
