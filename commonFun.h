/*=======================================================================
��������    :2013/04/10
�ļ���      : commonFun.h
����        : zcckm
�汾        : 1.0.0.1
�ļ�ʵ�ֹ��� :
���Ͽ����г������ܣ�ͬʱҲ�����Ǽ�����ʵ�ֵķ������Ǿ��幦��
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/04/10	1.0.0.1		zcckm		���Ӳ���������ӡ����
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