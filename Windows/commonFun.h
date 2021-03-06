/*=======================================================================
创建日期    :2013/04/10
文件名      : commonFun.h
作者        : zcckm
版本        : 1.0.0.1
文件实现功能 :
整合开发中常见功能，同时也可能是技术点实现的范例而非具体功能
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/04/10	1.0.0.1		zcckm		增加不定参数打印功能
=======================================================================*/
#ifndef _COMMONFUN_H_
#define _COMMONFUN_H_

void mutParam_ptf(BOOL ptf,const char * param,...)
{
	va_list arg_ptr;
	va_start(arg_ptr,param);
	char s[255];
	_vsnprintf(s,strlen(s),param,arg_ptr);
	printf("%s",s);
}
#endif

//#define CSTDBG 
#ifdef CSTDBG
#include <io.h>//Rain  用来输出调试信息
#include <fcntl.h>//Rain
#endif // CSTDBG

#ifdef CSTDBG
int nCrt = 0;
FILE* fp;
AllocConsole();
nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
fp = _fdopen(nCrt, "w");
*stdout = *fp;
setvbuf(stdout, NULL, _IONBF, 0);
#endif

