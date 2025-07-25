
#include "platform.h"
#include "publicInclude.h"
#include "publicDefine.h"
#include "WriteTexture.h"
#include "qrencode.h"
#include "stdlib.h"
#include "drawImmediately_cd.h"
#include "widgetClass_cd.h"
#if defined(EMBEDDED) && defined(VERSION_2)
#include "gc90x_heap.h"
#endif

#ifndef CHIP_GC9002_NoHyper
#define WriteTexture_Alpha1

const int ControlPointNum = 3 ; //���Ƶ���Ŀ
uint32_t olddata[Chart_Num_MAX] = {0};
uint32_t olddataNum[Chart_Num_MAX] = {0};
Point ControlPoint[ControlPointNum]; //���Ƶ�
Point DataPoint[Chart_Num_MAX][ControlPointNum]; //���Ƶ�
Point WorkPoint[ControlPointNum];  //һ�����ڴ����ʱ���ݵĿռ�?
Point DataBuffer[800];
uint32_t pointnumber = 0;

#ifdef PC_SIM
extern u8       OscilloscopeArry[SIZE_OF_SingleOscilloscope_BUF * Chart_Num_MAX];//the buffer to be displayed
extern u8       QRCodeArry[QRcode_BUF_SIZE * QRCode_Num_MAX]; //the buffer to be displayed
extern u8       DataImageDataArray[SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max];
#endif

#if defined(VERSION_2) && defined(EMBEDDED)
uint32_t  START_ADDR_OF_QRCode_FROM_GCHEAP = 0; //the start address of QRCode ram
#if defined(CHIP_GC9002)
uint32_t  START_ADDR_OFFSET_FROM_GCHEAP = 0;
#elif defined (CHIP_GC9005)
uint32_t  START_ADDR_OFFSET_FROM_GCHEAP = 0x60000000;
#else
uint32_t  START_ADDR_OFFSET_FROM_GCHEAP = 0x20000000;
#endif
#endif

void add_to_bitarray(uint8_t *bitarr, int32_t num)//num means data to be inset
{
	#ifdef WriteTexture_Alpha1
	bitarr[num >> 3] |= (1 << (num & 0x7));
	#endif
	
	#ifdef WriteTexture_Alpha2
	bitarr[num >> 2] |= (3 << ((num & 0x3) << 1));
	#endif
	
	#ifdef WriteTexture_Alpha4
	bitarr[num >> 1] |= (0xf << ((num & 0x1) << 2));
	#endif
}

uint8_t get_from_bitarray(uint8_t *bitarr, int32_t num)
{
	#ifdef WriteTexture_Alpha1
	return bitarr[num >> 3] & (1 << (num & 0x7));
	#endif
	
	#ifdef WriteTexture_Alpha2
	return bitarr[num >> 2] & (3 << ((num & 0x3) << 1));
	#endif
	
	#ifdef WriteTexture_Alpha4
	return bitarr[num >> 1] & (0xf << ((num & 0x1) << 2));
	#endif
}

#if defined(EMBEDDED) && defined(VERSION_2)
uint8_t* p_Oscilloscopetexture[Chart_Num_MAX] = {0};
#endif
#ifdef PC_SIM
uint8_t p_OscilloscopeInitFlag[Chart_Num_MAX] = {0};
#endif
void initOscilloscopeTextureRAM(uint32_t OscilloscopeID, uint32_t OscilloscopeWidth, uint32_t OscilloscopeHeight)
{
	#ifdef PC_SIM
	if(p_OscilloscopeInitFlag[OscilloscopeID] == 0)
	{
		p_OscilloscopeInitFlag[OscilloscopeID] = 1;
		memset(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, 0, SIZE_OF_SingleOscilloscope_BUF);
	}
	#endif
	#ifdef EMBEDDED
	if(p_Oscilloscopetexture[OscilloscopeID] == NULL)
	{
		if(OscilloscopeWidth * OscilloscopeHeight % 8 != 0)
		{
			p_Oscilloscopetexture[OscilloscopeID] = (uint8_t *)gcMalloc(((OscilloscopeWidth * OscilloscopeHeight + 8) >> 3));
			
			if(p_Oscilloscopetexture[OscilloscopeID] != NULL)
			{
				memset(p_Oscilloscopetexture[OscilloscopeID], 0, ((OscilloscopeWidth * OscilloscopeHeight + 8) >> 3));
			}
		}
		else
		{
			p_Oscilloscopetexture[OscilloscopeID] = (uint8_t *)gcMalloc((OscilloscopeWidth * OscilloscopeHeight >> 3));
			
			if(p_Oscilloscopetexture[OscilloscopeID] != NULL)
			{
				memset(p_Oscilloscopetexture[OscilloscopeID], 0, (OscilloscopeWidth * OscilloscopeHeight >> 3));
			}
		}
	}
	#endif 
}

void updataOscilloscopeTexture_alpha1(uint32_t OscilloscopeID, uint16_t Data, uint16_t DataNum, uint32_t DataMax, uint32_t DataNumMax, uint8_t showMode, uint8_t linewidth)
{
	int32_t i = 0, j = 0, k = 0;
	int32_t value = 0;
	int32_t differenceData = 0;

	if((linewidth == 1) || (showMode == 1))
	{
		#ifdef PC_SIM
		memset((OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF + (DataMax >> 3) * (DataNum)), 0, (DataMax >> 3));
		#endif
		
		#if defined(EMBEDDED) && defined(VERSION_2)
		memset((p_Oscilloscopetexture[OscilloscopeID] + (DataMax >> 3) * (DataNum)), 0, (DataMax >> 3));
		#endif

		differenceData = Data - olddata[OscilloscopeID];

		for(j = 1; j < (int32_t)DataMax; j++)
		{
			if (showMode != 0)
			{
				if(j >= (Data))
				{
					break;
				}
				else
				{
					#ifdef PC_SIM
					add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, (DataNum) * DataMax + j);
					#endif
					#if defined(EMBEDDED) && defined(VERSION_2)
		            add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], (DataNum) * DataMax + j);
					#endif
				}
			}else
			{
				if( j == (Data) || j == (Data - 1))
				{
					#ifdef PC_SIM
					add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, (DataNum) * DataMax + j);
					#endif
					#if defined(EMBEDDED) && defined(VERSION_2)
					add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], (DataNum) * DataMax + j);
					#endif
					break;
				}
			}
		}

		if(differenceData >= 0)
		{
			if (showMode != 0)
			{
				for(i = 0; i < Data; i++)
				{
					#ifdef PC_SIM
					if(DataNum == 0)
					{
						add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, (DataNum) * DataMax + i);
					}
					else
					{
						add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, (DataNum - 1) * DataMax + i);
					}
	
					#endif
					#if defined(EMBEDDED) && defined(VERSION_2)
					if(DataNum == 0)
					{
						add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], (DataNum) * DataMax + i);
					}
					else
					{
						add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], (DataNum - 1) * DataMax + i);
					}
					#endif
				}
			}
			else
			{
				for(i = olddata[OscilloscopeID]; i < Data; i++)
				{
					#ifdef PC_SIM
					if(DataNum == 0)
					{
						add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, (DataNum) * DataMax + i);
					}
					else
					{
						add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, (DataNum - 1) * DataMax + i);
					}
	
					#endif
					#if defined(EMBEDDED) && defined(VERSION_2)
					if(DataNum == 0)
					{
						add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], (DataNum) * DataMax + i);
					}
					else
					{
						add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], (DataNum - 1) * DataMax + i);
					}
					#endif
				}
			}
		}
		else if(differenceData < 0)
		{
			if (showMode != 0)
			{
				for(i = 0; i < (int32_t)olddata[OscilloscopeID]; i++)
				{
					#ifdef PC_SIM
					if(DataNum == 0)
					{
						add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, (DataNum) * DataMax + i);
					}
					else
					{
						add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, (DataNum - 1) * DataMax + i);
					}
	
					#endif
					#if defined(EMBEDDED) && defined(VERSION_2)
					if(DataNum == 0)
					{
						add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], (DataNum) * DataMax + i);
					}
					else
					{
						add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], (DataNum - 1) * DataMax + i);
					}
					#endif
				}
			}
			else
			{
				for(i = Data; i < (int32_t)olddata[OscilloscopeID]; i++)
				{
					#ifdef PC_SIM
					if(DataNum == 0)
					{
						add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, (DataNum) * DataMax + i);
					}
					else
					{
						add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, (DataNum - 1) * DataMax + i);
					}
	
					#endif
					#if defined(EMBEDDED) && defined(VERSION_2)
					if(DataNum == 0)
					{
						add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], (DataNum) * DataMax + i);
					}
					else
					{
						add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], (DataNum - 1) * DataMax + i);
					}
					#endif
				}
			}
		}
	}
	else
	{
		#ifdef PC_SIM
		if(DataNum > olddataNum[OscilloscopeID])
		{
			for(k = 0; k <= (DataNum - olddataNum[OscilloscopeID]); k++)
			{
				value = olddataNum[OscilloscopeID] + linewidth + k;
				if(value >= DataNumMax)
				{
					value = value - DataNumMax;
					memset((OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF + (DataMax >> 3) * (value)), 0, (DataMax >> 3));
					continue;
				}
				memset((OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF + (DataMax >> 3) * (olddataNum[OscilloscopeID] + linewidth + k)), 0, (DataMax >> 3));
			}
		}
		#endif
		
		#if defined(EMBEDDED) && defined(VERSION_2)
		if(DataNum > olddataNum[OscilloscopeID])
		{
			for(k = 0; k <= (int32_t)(DataNum - olddataNum[OscilloscopeID]); k++)
			{
				value = olddataNum[OscilloscopeID] + linewidth + k;
				if(value >= (int32_t)DataNumMax)
				{
					value = value - DataNumMax;
					memset((p_Oscilloscopetexture[OscilloscopeID] + (DataMax >> 3) * (value)), 0, (DataMax >> 3));
					continue;
				}
				memset((p_Oscilloscopetexture[OscilloscopeID] + (DataMax >> 3) * (olddataNum[OscilloscopeID] + linewidth + k)), 0, (DataMax >> 3));
			}
		}
		#endif

		differenceData = Data - olddata[OscilloscopeID];

		for(j = 1; j < (int32_t)DataMax; j++)
		{
			for(k = 0; k < linewidth; k++)
			{
				value = DataNum + k;
				if(value < 0)
				{
					value = value + DataNumMax;
				}
				else if(value >= (int32_t)DataNumMax)
				{
					value = value - DataNumMax;
				}

				if(k != linewidth/2)
				{
					if(k < linewidth/2)
					{
						if( j <= (s32)(Data + k + 1) && j >= (s32)(Data - k - 1))
						{
							#ifdef PC_SIM
							add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, value * DataMax + j);
							#endif
							#if defined(EMBEDDED) && defined(VERSION_2)
							add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], value * DataMax + j);
							#endif
						}
					}
					else if(k > linewidth/2)
					{
						if( j <= (s32)(Data + (linewidth - k)) && j >= (s32)(Data - (linewidth - k)))
						{
							#ifdef PC_SIM
							add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, value * DataMax + j);
							#endif
							#if defined(EMBEDDED) && defined(VERSION_2)
							add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], value * DataMax + j);
							#endif
						}
					}
				}
				else
				{
					if( j <= (s32)(Data + linewidth/2) && j >= (s32)(Data - linewidth/2))
					{
						#ifdef PC_SIM
						add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, value * DataMax + j);
						#endif
						#if defined(EMBEDDED) && defined(VERSION_2)
						add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], value * DataMax + j);
						#endif
					}
				}

				if(differenceData > 0)
				{
					if(k != linewidth - 1)
					{
						if( j <= (s32)(Data) && j >= (s32)(olddata[OscilloscopeID]))
						{
							#ifdef PC_SIM
							add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, value * DataMax + j);
							#endif
							#if defined(EMBEDDED) && defined(VERSION_2)
							add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], value * DataMax + j);
							#endif
						}
					
					}
				}
				else if(differenceData < 0)
				{
					if(k != linewidth - 1)
					{
						if( j <= (s32)(olddata[OscilloscopeID]) && j >= (s32)(Data))
						{
							#ifdef PC_SIM
							add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, value * DataMax + j);
							#endif
							#if defined(EMBEDDED) && defined(VERSION_2)
							add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], value * DataMax + j);
							#endif
						}
					
					}
				}
			}
		}
	}

	olddata[OscilloscopeID] = Data;
	olddataNum[OscilloscopeID] = DataNum;
}

#if defined(EMBEDDED) && defined(VERSION_2)
uint8_t* p_DataImagetexture[DataImage_Num_Max] = {0};
#endif
#ifdef PC_SIM
uint8_t p_DataImageInitFlags[DataImage_Num_Max] = {0};
#endif
void initDataImageTextureRAM(uint32_t dataImageID, uint32_t width, uint32_t height)
{
	#ifdef PC_SIM
	if(p_DataImageInitFlags[dataImageID] == 0)
	{
		p_DataImageInitFlags[dataImageID] = 1;
		memset(DataImageDataArray + dataImageID * SIZE_OF_SingleDataImage_BUF, 0, SIZE_OF_SingleDataImage_BUF);
	}
	#endif
	
	#if defined(EMBEDDED) && defined(VERSION_2)
	if(p_DataImagetexture[dataImageID] == NULL)
	{
		if((width * height) != 0)
		{
			p_DataImagetexture[dataImageID] = (uint8_t *)gcMalloc(width * height * 4);
			
			if(p_DataImagetexture[dataImageID] != NULL)
			{
				memset(p_DataImagetexture[dataImageID], 0, (width * height * 4));
			}
		}
		else
		{
			return;
		}
	}
	#endif 
}

void transColorFromRGB565(uint32_t value, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a)
{
	*a = 0xFF;
	*r = ((value >> 11)<<3) + 7;
    *g = (((value >> 5) & 0x3F)<<2) + 3;
    *b = ((value & 0x1F)<<3)+7;
}

void transColorFromRGBA8888(uint32_t value, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a)
{
	*a = value & 0xFF;
	value = value >> 8;
	*b = value & 0xFF;
	value = value >> 8;
	*g = value & 0xFF;
	value = value >> 8;
	*r = value & 0xFF;
}

void updataDataImageTexture(uint32_t dataImageID, uint8_t* rawDataArr, uint16_t rawDataNum, uint8_t rawDataByteLen, uint8_t rawDataColorType, uint16_t lineID, uint16_t width, uint16_t height, uint8_t rowOrColumn, uint8_t shouldShift)
{
	uint32_t i = 0, j = 0;
	uint32_t value = 0;
	uint8_t r,g,b,a;
	uint32_t pos;
	uint32_t srcPos;
	uint32_t totallinebytes;

	if (rowOrColumn)
	{
		//row
		if (lineID > height)
		{
			return;
		}
	}
	else
	{
		//column
		if (lineID > width)
		{
			return;
		}
	}

	//if (shouldShift)
	//{
	//	if (rowOrColumn)
	//	{
	//		for (i = 0; i < lineID; i++)
	//		{
	//			for (j = 0; j < width; j++)
	//			{
	//				pos = i * width + j;
	//				srcPos = (i+1) * width + j;
	//				//copy
	//				#ifdef PC_SIM
	//				DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + pos*4] = DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + srcPos*4];
	//				DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + pos*4 + 1] = DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + srcPos*4 + 1];
	//				DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + pos*4 + 2] = DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + srcPos*4 + 2];
	//				DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + pos*4 + 3] = DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + srcPos*4 + 3];
	//				#endif
	//	
	//				#if defined(EMBEDDED) && defined(VERSION_2)
	//				(p_DataImagetexture[dataImageID])[pos*4] = (p_DataImagetexture[dataImageID])[srcPos*4];
	//				(p_DataImagetexture[dataImageID])[pos*4 + 1] = (p_DataImagetexture[dataImageID])[srcPos*4 + 1];
	//				(p_DataImagetexture[dataImageID])[pos*4 + 2] = (p_DataImagetexture[dataImageID])[srcPos*4 + 2];
	//				(p_DataImagetexture[dataImageID])[pos*4 + 3] = (p_DataImagetexture[dataImageID])[srcPos*4 + 3];
	//				#endif
	//			}
	//		}
	//	}
	//	else
	//	{
	//		for (i = 0; i < lineID; i++)
	//		{
	//			for (j = 0; j < height; j++)
	//			{
	//				pos = j * width + i;
	//				srcPos = j * width + i+1;
	//				//copy
	//				#ifdef PC_SIM
	//				DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + pos*4] = DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + srcPos*4];
	//				DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + pos*4 + 1] = DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + srcPos*4 + 1];
	//				DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + pos*4 + 2] = DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + srcPos*4 + 2];
	//				DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + pos*4 + 3] = DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + srcPos*4 + 3];
	//				#endif
	//	
	//				#if defined(EMBEDDED) && defined(VERSION_2)
	//				(p_DataImagetexture[dataImageID])[pos*4] = (p_DataImagetexture[dataImageID])[srcPos*4];
	//				(p_DataImagetexture[dataImageID])[pos*4 + 1] = (p_DataImagetexture[dataImageID])[srcPos*4 + 1];
	//				(p_DataImagetexture[dataImageID])[pos*4 + 2] = (p_DataImagetexture[dataImageID])[srcPos*4 + 2];
	//				(p_DataImagetexture[dataImageID])[pos*4 + 3] = (p_DataImagetexture[dataImageID])[srcPos*4 + 3];
	//				#endif
	//			}
	//		}
	//	}
	//}
	
	if (rawDataNum % rawDataByteLen)
	{
		rawDataNum -= rawDataNum % rawDataByteLen;
	}

	if (rowOrColumn)
	{
		//row
		if(rawDataNum > (width * rawDataByteLen))
			rawDataNum = (width * rawDataByteLen);

		//get totallinebytes
		totallinebytes = width * rawDataByteLen;
	}
	else
	{
		//column
		if(rawDataNum > (height * rawDataByteLen))
			rawDataNum = (height * rawDataByteLen);

		//get totallinebytes
		totallinebytes = height * rawDataByteLen;
	}
	
	for (i = 0; i < totallinebytes; i+=rawDataByteLen)
	{
		if(i < rawDataNum)
		{
			value = 0;
			for (j = 0; j < rawDataByteLen; j++)
			{
				value = (value << 8) + rawDataArr[i+j];
			}
			if (rawDataColorType == 1)
			{
				//rgb565
				transColorFromRGB565(value, &r, &g, &b, &a);
			}
			else
			{
				transColorFromRGBA8888(value, &r, &g, &b, &a);
			}
			//set pixel
			if (rowOrColumn)
			{
				//row
				pos = lineID * width + i/rawDataByteLen;
			}
			else
			{
				//column
				pos = i/rawDataByteLen * width  + lineID;
			}
			
			#ifdef PC_SIM
			DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + 4*pos] = b;
			DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + 4*pos + 1] = g;
			DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + 4*pos + 2] = r;
			DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + 4*pos + 3] = a;
			#endif
			
			#if defined(EMBEDDED) && defined(VERSION_2)
			(p_DataImagetexture[dataImageID])[4*pos] = b;
			(p_DataImagetexture[dataImageID])[4*pos + 1] = g;
			(p_DataImagetexture[dataImageID])[4*pos + 2] = r;
			(p_DataImagetexture[dataImageID])[4*pos + 3] = a;
			#endif
		}
		else
		{
			//set zero
			if (rowOrColumn)
			{
				//row
				pos = lineID * width + i/rawDataByteLen;
			}
			else
			{
				//column
				pos = i/rawDataByteLen * width  + lineID;
			}

			#ifdef PC_SIM
			DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + 4*pos] = 0;
			DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + 4*pos + 1] = 0;
			DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + 4*pos + 2] = 0;
			DataImageDataArray[dataImageID * SIZE_OF_SingleDataImage_BUF + 4*pos + 3] = 0;
			#endif
			
			#if defined(EMBEDDED) && defined(VERSION_2)
			(p_DataImagetexture[dataImageID])[4*pos] = 0;
			(p_DataImagetexture[dataImageID])[4*pos + 1] = 0;
			(p_DataImagetexture[dataImageID])[4*pos + 2] = 0;
			(p_DataImagetexture[dataImageID])[4*pos + 3] = 0;
			#endif
		}
	}
}

void drawPointTexture_alpha1(uint32_t OscilloscopeID, uint16_t x, uint16_t y, uint32_t width, uint32_t height, uint8_t linewidth)
{
	int i = 0;
	s16 cur_x, cur_y, dif_x, dif_y;

	for(i = 0; i < linewidth * linewidth; i++)
	{
		cur_x = x - linewidth /2;
		cur_y = y - linewidth /2;

		cur_x += (i % linewidth);
		cur_y += (i / linewidth);

		dif_x = cur_x - x;
		dif_y = cur_y - y;

		if(dif_x < 0)
		{
			dif_x = -dif_x;
		}

		if(dif_y < 0)
		{
			dif_y = -dif_y;
		}

		if((dif_x + dif_y) > (linewidth/2 + 1))
		{
			continue;
		}
		else
		{
			if(cur_x < 0 || cur_x >= (s16)width || cur_y < 0 || cur_y >= (s16)height)
			{
				continue;
			}
			else
			{
				#ifdef PC_SIM
				add_to_bitarray(OscilloscopeArry + OscilloscopeID * SIZE_OF_SingleOscilloscope_BUF, (cur_y) * width + cur_x);
				#endif
				#if defined(EMBEDDED) && defined(VERSION_2)
				add_to_bitarray(p_Oscilloscopetexture[OscilloscopeID], (cur_y) * width + cur_x);
				#endif
			}
		}
	}
}

int min_main(int x1, int x2)
{
	if(x1 <= x2)
		return x1;
	else
		return x2;
}

int max_main(int x1, int x2)
{
	if(x1 <= x2)
		return x2;
	else
		return x1;
}

void Bezier(uint32_t OscilloscopeID, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t width, uint32_t height, uint8_t linewidth)
{
    if((x1 == x2 && y1 == y2) || (x2 == x3 && y2 ==y3) || (x1 == x3 && y1 ==y3))
        return;

    /********* Getting the midpoint of the data point ************/
    ControlPoint[0].X = (((x1 + x2) >> 1) << 4);
    ControlPoint[0].Y = (((y1 + y2) >> 1) << 4);
    ControlPoint[1].X = (x2 << 4);
    ControlPoint[1].Y = (y2 << 4);
    ControlPoint[2].X = (((x2 + x3) >> 1) << 4);
    ControlPoint[2].Y = (((y2 + y3) >> 1) << 4);

    /********* Midpoint optimization of second-order Bessel curve ************/
    GetAllPoint_ForTouchTrack(ControlPoint[0], ControlPoint[1], ControlPoint[2]); //all output points in DataBuffer[] arry

    for(uint32_t i = 0;i < pointnumber; i++)
    {
        drawPointTexture_alpha1(OscilloscopeID,DataBuffer[i].X>>4,DataBuffer[i].Y>>4,width,height,linewidth);
    }
    pointnumber = 0;
}

void Bresenham(uint32_t OscilloscopeID, int x1, int y1, int x2, int y2, uint32_t width, uint32_t height, uint8_t linewidth) 
{
	int x, y, dx, dy, p1, i;
	dx = x2 - x1;                        
	dy = y2 - y1;
	if (abs(dx) > abs(dy))
	{
		if (x1 < x2)
		{
			x = x1;
			y = y1;
		}
		else
		{
			x = x2;
			y = y2;
		}   
		drawPointTexture_alpha1(OscilloscopeID,x,y,width,height,linewidth);

		p1 = 2 * abs(dy) - abs(dx);
		for (i = min_main(x1, x2); i < max_main(x1, x2); i++)
		{
			x = x + 1;
			if (p1 >= 0) 
			{
				if (dx*dy >= 0)
				{
					y = y + 1;
				}
				else
				{
					y = y - 1;
				}
				p1 = p1 + 2 * (abs(dy) - abs(dx));
			}
			else 
			{
				y = y;
				p1 = p1 + 2 * abs(dy);
			}
			drawPointTexture_alpha1(OscilloscopeID,x,y,width,height,linewidth);
		}
	}
	else
	{
		if (y1 < y2) 
		{    
			x = x1;
			y = y1;
		}
		else 
		{
			x = x2;
			y = y2;
		}
		drawPointTexture_alpha1(OscilloscopeID,x,y,width,height,linewidth);
		p1 = 2 * abs(dx) - abs(dy);
		for (i = min_main(y1, y2); i < max_main(y1, y2); i++) 
		{
			y = y + 1;
			if (p1 >= 0) 
			{
				if (dx*dy >= 0)
				{
					x = x + 1;
				}
				else
				{
					x = x - 1;
				}
				p1 = p1 + 2 * (abs(dx) - abs(dy));
			}
			else 
			{
				x = x;
				p1 = p1 + 2 * abs(dx);
			}
			drawPointTexture_alpha1(OscilloscopeID,x,y,width,height,linewidth);
		}
	}
}

Point GetFinalPoint_s16(int16_t t, int n)//t 1.2.13
{
    if (n == 1)
    {
        return WorkPoint[0];
    }
    else
    {
        //������һ�׵ı��������Ƶ�
        for (int i = 0; i < n - 1; i++)
        {
            WorkPoint[i].X = ((int)((WorkPoint[i].X * ((1 << 13) - t)) + (WorkPoint[i + 1].X * t)) >> 13);
            WorkPoint[i].Y = ((int)((WorkPoint[i].Y * ((1 << 13) - t)) + (WorkPoint[i + 1].Y * t)) >> 13);
        }
        //������һ��
        return GetFinalPoint_s16(t, n - 1);
    }
}

Point GetCenterPoint(Point a, Point b, Point c)
{
	Point ab,bc,abc;

	ab.X = ((a.X + b.X) >> 1);
	ab.Y = ((a.Y + b.Y) >> 1);

	bc.X = ((b.X + c.X) >> 1);
	bc.Y = ((b.Y + c.Y) >> 1);

	abc.X = ((ab.X + bc.X) >> 1);
	abc.Y = ((ab.Y + bc.Y) >> 1);

	if((b.X - a.X) <= (1 << 3))
	{
		return b;
	}

	return abc;
}

void GetAllPoint(Point a, Point b, Point c)// .4
{
	Point ab,bc;

	if((b.X -a.X) <= (1 << 3))
		return;

	WorkPoint[0] = a;
	WorkPoint[1] = b;
	WorkPoint[2] = c;

	Point FinalPoint0 = GetCenterPoint(a, b, c);
	DataBuffer[pointnumber++] = FinalPoint0;

	ab.X = ((a.X + b.X) >> 1);
	ab.Y = ((a.Y + b.Y) >> 1);

	bc.X = ((b.X + c.X) >> 1);
	bc.Y = ((b.Y + c.Y) >> 1);

	GetAllPoint(a, ab, FinalPoint0);
	GetAllPoint(FinalPoint0, bc, c);

}

Point GetCenterPoint_ForTouchTrack(Point a, Point b, Point c)
{
    Point ab,bc,abc;

    ab.X = ((a.X + b.X) >> 1);
    ab.Y = ((a.Y + b.Y) >> 1);

    bc.X = ((b.X + c.X) >> 1);
    bc.Y = ((b.Y + c.Y) >> 1);

    abc.X = ((ab.X + bc.X) >> 1);
    abc.Y = ((ab.Y + bc.Y) >> 1);

    if(((b.X -a.X) <= (1 << 3) && (b.X -a.X) >= -(1 << 3)) && ((b.Y -a.Y) <= (1 << 3) && (b.Y -a.Y) >= -(1 << 3)))
    {
        return b;
    }

    return abc;
}

void GetAllPoint_ForTouchTrack(Point a, Point b, Point c)// .4
{
    Point ab,bc;

    if(((b.X -a.X) <= (1 << 3) && (b.X -a.X) >= -(1 << 3)) && ((b.Y -a.Y) <= (1 << 3) && (b.Y -a.Y) >= -(1 << 3)))
        return;

    WorkPoint[0] = a;
    WorkPoint[1] = b;
    WorkPoint[2] = c;

    Point FinalPoint0 = GetCenterPoint_ForTouchTrack(a, b, c);
    DataBuffer[pointnumber++] = FinalPoint0;

    ab.X = ((a.X + b.X) >> 1);
    ab.Y = ((a.Y + b.Y) >> 1);

    bc.X = ((b.X + c.X) >> 1);
    bc.Y = ((b.Y + c.Y) >> 1);

    GetAllPoint_ForTouchTrack(a, ab, FinalPoint0);
    GetAllPoint_ForTouchTrack(FinalPoint0, bc, c);
}

void HandleAllPoint(void)
{
	uint32_t i = 0;

	for(i = 0; i < pointnumber + 1; i++)
	{
		if( ((DataBuffer[i].X & 0xf) >= 0x8) && ((DataBuffer[i].Y & 0xf) >= 0x8) )
		{
			DataBuffer[i].X = (DataBuffer[i].X >> 4) + 0x1;
			DataBuffer[i].Y = (DataBuffer[i].Y >> 4) + 0x1;
		}
		else if( ((DataBuffer[i].X & 0xf) < 0x8) && ((DataBuffer[i].Y & 0xf) >= 0x8) )
		{
			DataBuffer[i].X = (DataBuffer[i].X >> 4);
			DataBuffer[i].Y = (DataBuffer[i].Y >> 4) + 0x1;
		}
		else if( ((DataBuffer[i].X & 0xf) >= 0x8) && ((DataBuffer[i].Y & 0xf) < 0x8) )
		{
			DataBuffer[i].X = (DataBuffer[i].X >> 4) + 0x1;
			DataBuffer[i].Y = (DataBuffer[i].Y >> 4);
		}
		else if( ((DataBuffer[i].X & 0xf) < 0x8) && ((DataBuffer[i].Y & 0xf) < 0x8) )
		{
			DataBuffer[i].X = (DataBuffer[i].X >> 4);
			DataBuffer[i].Y = (DataBuffer[i].Y >> 4);
		}
	}

}

#if defined(EMBEDDED) && defined(VERSION_2)
GCHeap* qrheap = NULL;
uint8_t* p_QRtexture[QRCode_Num_MAX] = {0};
#endif

void writeQRCode_alpha1(uint32_t qrID, uint32_t qrwidth, char *QRSourceString)
{
    #ifdef VERSION_2
	uint32_t i,j;
    int x_temp,y_temp;
    QRcode*	pQRC;
    uint8_t* p;
	
	#ifdef EMBEDDED
	uint32_t QRCodeMemorySize = 0;
    if(qrheap == NULL)
    {
        qrheap = (GCHeap*)gcMalloc(sizeof(GCHeap));
		
		p = (uint8_t *)gcMalloc(125*1024);
		qrheap->init(p, 125*1024);
		memset(p, 0, 125*1024);
	}
		
	QRCodeMemorySize = ((qrwidth * qrwidth) >> 3) + 1;

	if(p_QRtexture[qrID] == NULL)
	{		
		p_QRtexture[qrID] = (uint8_t *)gcMalloc(QRCodeMemorySize);
		
		if(p_QRtexture[qrID] != NULL)
		{
			memset(p_QRtexture[qrID], 0, QRCodeMemorySize);
		}
	}
	#endif 

    pQRC = QRcode_encodeString(QRSourceString, 0, QR_ECLEVEL_H, QR_MODE_8, 1);
    
    if(pQRC == NULL)
	{
        return;
	}
	
	#ifdef PC_SIM
	memset(QRCodeArry + qrID * QRcode_BUF_SIZE, 0, QRcode_BUF_SIZE);
	#endif
	#if defined(EMBEDDED) && defined(VERSION_2)
	memset(p_QRtexture[qrID], 0, QRCodeMemorySize);
	#endif
	
    
    
    for(i = 0; i < qrwidth; i++)
    {
        x_temp = i * pQRC->width / qrwidth;
        
        for(j = 0; j < qrwidth; j++)
        {
			y_temp = j * pQRC->width / qrwidth;

            p = pQRC->data;
			p = p + x_temp * pQRC->width + y_temp;
			
            if(*p & 1)
			{
				#ifdef PC_SIM
				add_to_bitarray(QRCodeArry + qrID * QRcode_BUF_SIZE, i * qrwidth + j);
				#endif
				#if defined(EMBEDDED) && defined(VERSION_2)
				add_to_bitarray(p_QRtexture[qrID], i * qrwidth + j);
				#endif
			}
            else
			{
				//p++;
			}
        }
    }
		
    if(pQRC != 0)
    {
        if(pQRC->data != 0)
            qr_free(pQRC->data);
        
        qr_free(pQRC);
    }
    
    #endif
}
#endif
