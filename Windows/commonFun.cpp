/*=======================================================================
创建日期    :2014 /08 /27
文件名      : commonFun.cpp
作者        : zcckm
版本        : 1.0.0.0
文件实现功能 :
功能介绍:记录一些常用的算法
注意：（NULL）

添加函数:
//获取指定位置指定bit长度的值
u32 GetBitS(u8 * pBuffer,u8 byByteOffset,u8 dwBitsLen)

//获取指定位置字节 0 OR 1
bool getBit(u8 * pBuffer,u32 dwOffset)

//判断当前系统大端还是小端
BOOL IsBigEndian()
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
=======================================================================*/




/*====================================================================
  函数名：GetBitS
  功能：获取指定字节长度表示的值，最高32位
  算法实现：注意大小端问题：
			在大端系统中，字符串指针可以直接转为u32 而小端不行
			可以借助字符串指针类似大端，与网络序列相同的性质。
  注意：使用时需要包含:
  #include <WINSOCK2.H>
  #pragma comment(lib,"ws2_32.lib")
  
  输入参数说明：
			u8 * pBuffer 需要处理的数据数组
			u8 byByteOffset 指定字节起始于pBuffer间的偏移
			u8 dwBitsLen 需要截取字节的长度
  返回值说明: 获取字节长度表示值 如果参数不符合，则返回 0xFFFFFFFF
  ====================================================================*/
u32 GetBitS(u8 * pBuffer,u8 byByteOffset,u8 dwBitsLen)
{
	if (byByteOffset + dwBitsLen > 32) return (u32)-1;
	u32 dwTmpCmp = ntohl(*(u32 *)pBuffer);
	dwTmpCmp <<= byByteOffset;
	dwTmpCmp >>=(32 - dwBitsLen);
	return dwTmpCmp;
}

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
/*====================================================================
  函数名：IsBigEndian
  功能：判断当前系统大端还是小端
  算法实现：利用大小端地址分配时高低字节位置的原理判断
  输入参数说明：
			u8 * pBuffer 需要处理的数据数组
			u8 byByteOffset 指定字节起始于pBuffer间的偏移
			u8 dwBitsLen 需要截取字节的长度
  返回值说明: TRUE 大端 FALSE 小端
  ====================================================================*/
BOOL IsBigEndian()
{
	u16 dwTeEndian = 0x0102;
	u8 * pTeEndian = (u8 *)&dwTeEndian;
	if (pTeEndian[0] == 0x01)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
