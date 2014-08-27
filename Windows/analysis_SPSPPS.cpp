
/*=======================================================================
创建日期    :2012 /12 /25
文件名      : analysis_SPSPPS.cpp
作者        : zcckm
版本        : 1.0.0.0
文件实现功能 :
功能介绍:分析H264码流中 SPS PPS 获取其中的宽高信息
注意：SPS PPS中信息并不是按照正常编码方式存储数据而是通过哥伦布编码
因此需要按照SPS PPS存储结构通过哥伦布编码规则才能找出需要的数据
哥伦布编码

b(8):任意形式的8比特字节（就是为了说明语法元素是为8个比特，没有语法上的含义）
f(n):n位固定模式比特串（其值固定，如forbidden_zero_bit的值恒为0）
i(n):使用n比特的有符号整数（语法中没有采用此格式）
u(n):n位无符号整数

指数哥伦布编码：
ue(v):无符号整数指数哥伦布码编码的语法元素
se(v):有符号整数指数哥伦布编码的语法元素，左位在先
te(v):舍位指数哥伦布码编码语法元素，左位在先

  sps pps 结构在14496-part10中有说明。
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
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

/*====================================================================
  函数名：getBit
  功能：获取指定位置字节 0 OR 1
  算法实现：
  引用全局变量：
  输入参数说明：
			u8 * pBuffer 需要处理的数据数组
			u32 dwOffset 指定字节起始于pBuffer间的偏移
  返回值说明: 当前位置是 0 OR 1
  ====================================================================*/
bool getBit(u8 * pBuffer,u32 dwOffset)
{
	return pBuffer[dwOffset/8] & ( 0x80 >> (dwOffset%8) )?true:false;

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

