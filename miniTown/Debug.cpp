#include "Debug.h"

char header[] = "调试信息：";
char _Buffer[2048] = "调试信息：";

//Description:
//	输出调试调试信息，要要输出的内容长度不能超过2048字节(带换行符)
//Paramter: 
//	char const* const _Format 格式化字符串
//	... 可变参数
//Return Value:
//	<0  错误
//	>=0	输出的字符串长度
int DebugPrintln(_In_z_ _Printf_format_string_ char const* const _Format,...) {
	int _Result;
	va_list _ArgList;
	__crt_va_start(_ArgList, _Format);
	_Result = vsprintf(_Buffer+strlen(header), _Format, _ArgList);
	if (_Result < 0) {
		__crt_va_end(_ArgList);
		//缓冲区溢出
		return _Result;
	}
	strcat(_Buffer, "\n");
	OutputDebugString(_Buffer);
	__crt_va_end(_ArgList);
	return _Result;
}

//Description:
//	输出调试调试信息，要要输出的内容长度不能超过2048字节(带换行符)
//Paramter: 
//	char const* const _Format 格式化字符串
//	... 可变参数
//Return Value:
//	<0  错误
//	>=0	输出的字符串长度
int DebugPrint(_In_z_ _Printf_format_string_ char const* const _Format, ...) {
	int _Result;
	va_list _ArgList;
	__crt_va_start(_ArgList, _Format);
	_Result = vsprintf(_Buffer + strlen(header), _Format, _ArgList);
	if (_Result < 0) {
		__crt_va_end(_ArgList);
		//缓冲区溢出
		return _Result;
	}
	OutputDebugString(_Buffer);
	__crt_va_end(_ArgList);
	return _Result;
}