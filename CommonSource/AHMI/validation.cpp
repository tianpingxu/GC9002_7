#include "validation.h"

void InvertUint8(unsigned char *dBuf,unsigned char *srcBuf)
{
	int i;
	unsigned char tmp[4]={0};
 
	for(i=0;i< 8;i++)
	{
		if(srcBuf[0]& (1 << i))
		tmp[0]|=1<<(7-i);
	}
	dBuf[0] = tmp[0];
	
}

void InvertUint16(unsigned short *dBuf,unsigned short *srcBuf)
{
	int i;
	unsigned short tmp[4]={0};
 
	for(i=0;i< 16;i++)
	{
		if(srcBuf[0]& (1 << i))
		tmp[0]|=1<<(15 - i);
	}
	dBuf[0] = tmp[0];
}

void InvertUint32(unsigned int *dBuf,unsigned int *srcBuf)
{
	int i;
	unsigned int tmp[4]={0};
	
	for(i=0;i< 32;i++)
	{
		if(srcBuf[0]& (1 << i))
		tmp[0]|=1<<(31 - i);
	}
	dBuf[0] = tmp[0];

}

Validator::Validator()
{
}

Validator::~Validator()
{
}

u16 Validator:: calcCRC16(u8 *data, u32 datalen)
{
	unsigned short wCRCin = 0x0000;
	unsigned short wCPoly = 0x8005;
	unsigned char wChar = 0;
	
	while (datalen--) 	
	{
		wChar = *(data++);
		InvertUint8(&wChar,&wChar);
		wCRCin ^= (wChar << 8);
		for(int i = 0;i < 8;i++)
		{
			if(wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin,&wCRCin);
	return (wCRCin);
}

u32 XORCheck(u32 *data, u32 datalen)
{
	u32 checkCode = 0xA55A5AA5;
	u32 i = 0;

	for(i = 0; i < datalen; i++)
	{
		checkCode^= *(data + i);
	}

	return checkCode;
}
