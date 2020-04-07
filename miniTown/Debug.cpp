#include "Debug.h"

char header[] = "������Ϣ��";
char _Buffer[2048] = "������Ϣ��";

//Description:
//	������Ե�����Ϣ��ҪҪ��������ݳ��Ȳ��ܳ���2048�ֽ�(�����з�)
//Paramter: 
//	char const* const _Format ��ʽ���ַ���
//	... �ɱ����
//Return Value:
//	<0  ����
//	>=0	������ַ�������
int DebugPrintln(_In_z_ _Printf_format_string_ char const* const _Format,...) {
	int _Result;
	va_list _ArgList;
	__crt_va_start(_ArgList, _Format);
	_Result = vsprintf(_Buffer+strlen(header), _Format, _ArgList);
	if (_Result < 0) {
		__crt_va_end(_ArgList);
		//���������
		return _Result;
	}
	strcat(_Buffer, "\n");
	OutputDebugString(_Buffer);
	__crt_va_end(_ArgList);
	return _Result;
}

//Description:
//	������Ե�����Ϣ��ҪҪ��������ݳ��Ȳ��ܳ���2048�ֽ�(�����з�)
//Paramter: 
//	char const* const _Format ��ʽ���ַ���
//	... �ɱ����
//Return Value:
//	<0  ����
//	>=0	������ַ�������
int DebugPrint(_In_z_ _Printf_format_string_ char const* const _Format, ...) {
	int _Result;
	va_list _ArgList;
	__crt_va_start(_ArgList, _Format);
	_Result = vsprintf(_Buffer + strlen(header), _Format, _ArgList);
	if (_Result < 0) {
		__crt_va_end(_ArgList);
		//���������
		return _Result;
	}
	OutputDebugString(_Buffer);
	__crt_va_end(_ArgList);
	return _Result;
}