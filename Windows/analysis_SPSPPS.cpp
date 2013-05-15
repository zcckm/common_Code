
/*=======================================================================
��������    :2012 /12 /25
�ļ���      : analysis_SPSPPS.cpp
����        : zcckm
�汾        : 1.0.0.0
�ļ�ʵ�ֹ��� :
���ܽ���:����H264������ SPS PPS ��ȡ���еĿ����Ϣ
ע�⣺SPS PPS����Ϣ�����ǰ����������뷽ʽ�洢���ݶ���ͨ�����ײ�����
�����Ҫ����SPS PPS�洢�ṹͨ�����ײ������������ҳ���Ҫ������
���ײ�����

b(8):������ʽ��8�����ֽڣ�����Ϊ��˵���﷨Ԫ����Ϊ8�����أ�û���﷨�ϵĺ��壩
f(n):nλ�̶�ģʽ���ش�����ֵ�̶�����forbidden_zero_bit��ֵ��Ϊ0��
i(n):ʹ��n���ص��з����������﷨��û�в��ô˸�ʽ��
u(n):nλ�޷�������

ָ�����ײ����룺
ue(v):�޷�������ָ�����ײ��������﷨Ԫ��
se(v):�з�������ָ�����ײ�������﷨Ԫ�أ���λ����
te(v):��λָ�����ײ�������﷨Ԫ�أ���λ����

  sps pps �ṹ��14496-part10����˵����
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
=======================================================================*/
#include "stdafx.h"
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef struct 
{
	unsigned int m_width;
	unsigned int m_height;
}FRAMEINFO;

bool getBit(u8 * data,u8 offset)
{
	u8 offset_byte = 0;
	if (offset == 0)
	{
		offset = 1;
	}
	offset_byte = (offset-1) % 8;
	return (data[(offset-1)/8] & (0x80 >> offset_byte))? true:false;
}

unsigned int columbusDecode(u8 * data, u8 start_pos,u8 * offset)
{
	u8 start = start_pos;
	u8 zeroCount = 0;
	u32 aft1VAL = 0;
	u32 pre1VAL = 1;
	while(!getBit(data,start))
	{
		++start;
		++zeroCount;
	}
	if (offset != NULL)
	{
		*offset =  (2 * zeroCount) +1;
	}
	for (int a = zeroCount; a>0 ;a--)
	{
		pre1VAL *=2;
		aft1VAL += ((0x01 <<(a -1)) * getBit(data, start + zeroCount - a + 1));
	}
	return pre1VAL + aft1VAL -1;
}

bool getWHInfo(u8 * data,FRAMEINFO * info)
{
	u8 mainOffset = 33;
	u8 offset;
	int currentflag = 0;
	u8 judge_flag = 0;
	int a = 0;
	judge_flag = data[1];//profile_idc;
	currentflag = columbusDecode(data, mainOffset,&offset);
	mainOffset += offset;
	
	if(judge_flag == 100 || judge_flag == 110 || judge_flag == 122 || judge_flag == 244
		|| judge_flag == 44 || judge_flag == 83 ||judge_flag == 86)
	{
		currentflag = columbusDecode(data, mainOffset,&offset);
		mainOffset += offset;
		if (currentflag == 3)
		{
			++mainOffset;
		}
		for (a = 0; a< 2;a++)
		{
			columbusDecode(data, mainOffset,&offset);
			mainOffset += offset;
		}
		++mainOffset;
		if(getBit(data,mainOffset))
		{
			mainOffset += (currentflag != 3?8:12);
		}

	}

	for (a = 0; a< 2;a++)
	{
		currentflag = columbusDecode(data, mainOffset,&offset);
		mainOffset += offset;
	}

	if (currentflag == 1) //pic_order_cnt_type
	{
		++mainOffset;
		for (a = 0; a < 3; a++)
		{
			currentflag = columbusDecode(data, mainOffset,&offset);
			mainOffset += offset;
		}
		for (a = 0; a < currentflag; a++)
		{
			columbusDecode(data, mainOffset,&offset);
			mainOffset += offset;
		}
	}
	else if(currentflag == 0)
	{
		currentflag = columbusDecode(data, mainOffset,&offset);
		mainOffset += offset;
	}
	currentflag = columbusDecode(data, mainOffset,&offset);
	mainOffset += offset;
	mainOffset += 1;
	
	if (info != NULL)
	{
		info->m_width = (columbusDecode(data,mainOffset,&offset)+1)*16 ;
		mainOffset += offset;
		info->m_height = (columbusDecode(data,mainOffset,&offset)+1)*16;
		if (info->m_height == 1088)
		{
			info->m_height = 1080;
		}
		printf("width = %d,height = %d\n",info->m_width,info->m_height);
		return true;
	}
	
	return false;
}

int main(int argc, char* argv[])
{

	u8  buf[] = {0X27,0X42,0X00,0X1E,0XF4,0X03,0XC0,0X11,0X2F,0X2A};//{0X27,0X42,0X00,0X1E,0XF4,0X03,0XC0,0X11,0X2F,0X2A};
	u8 offset;
	printf("data = %d\n",columbusDecode(buf,41,&offset));
	printf("offset = %d\n",offset);
	FRAMEINFO info;
	getWHInfo(buf,&info);
	return 0;
}

