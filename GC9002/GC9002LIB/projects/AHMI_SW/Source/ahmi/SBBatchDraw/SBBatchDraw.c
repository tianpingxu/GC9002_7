#include "SBBatchDraw.h"
#include "csi_ahmi.h"
#include "display_conf.h"

extern uint32_t SBAddress;
extern uint32_t sourceshift;
extern u8 initAHMI;
extern uint32_t AHMIOutputRotation;

static volatile uint8_t BatchDrawType = BatchDraw_SB;

FBBatchDraw_Box mFBBatchDraw_Box[4] = {0};
uint32_t StartAddrOfSourcebufferForFBBatch = 0;
static uint8_t FBBatchCurBox = 0;

void BatchDraw_vSetBatchDrawType(uint8_t type)
{
	static uint8_t FBBatchInitFlag = 1;
	BatchDrawType = type;
	
	if(FBBatchInitFlag && (type == BatchDraw_FB))
	{
		FBBatchInitFlag = 0;
		switch (sizeof(mFBBatchDraw_Box)/sizeof(FBBatchDraw_Box))
		{
			case 1:
				mFBBatchDraw_Box[0].startX = 0;
				mFBBatchDraw_Box[0].startY = 0;
				mFBBatchDraw_Box[0].width = SCREEN_WIDTH;
				mFBBatchDraw_Box[0].height = SCREEN_HEIGHT;
			break;
			case 2:
				mFBBatchDraw_Box[0].startX = 0;
				mFBBatchDraw_Box[0].startY = 0;
				mFBBatchDraw_Box[0].width = ((SCREEN_WIDTH/2 + 15) / 16) * 16;
				mFBBatchDraw_Box[0].height = SCREEN_HEIGHT;
				mFBBatchDraw_Box[1].startX = mFBBatchDraw_Box[0].width;
				mFBBatchDraw_Box[1].startY = 0;
				mFBBatchDraw_Box[1].width = (SCREEN_WIDTH - mFBBatchDraw_Box[0].width);
				mFBBatchDraw_Box[1].height = SCREEN_HEIGHT;
			break;
			case 3:
				mFBBatchDraw_Box[0].startX = 0;
				mFBBatchDraw_Box[0].startY = 0;
				mFBBatchDraw_Box[0].width = ((SCREEN_WIDTH/3 + 15) / 16) * 16;
				mFBBatchDraw_Box[0].height = SCREEN_HEIGHT;
				mFBBatchDraw_Box[1].startX = mFBBatchDraw_Box[0].width;
				mFBBatchDraw_Box[1].startY = 0;
				mFBBatchDraw_Box[1].width = mFBBatchDraw_Box[0].width;
				mFBBatchDraw_Box[1].height = SCREEN_HEIGHT;
				mFBBatchDraw_Box[2].startX = mFBBatchDraw_Box[0].width * 2;
				mFBBatchDraw_Box[2].startY = 0;
				mFBBatchDraw_Box[2].width = (SCREEN_WIDTH - mFBBatchDraw_Box[0].width * 2);
				mFBBatchDraw_Box[2].height = SCREEN_HEIGHT;
			break;
			case 4:
				mFBBatchDraw_Box[0].startX = 0;
				mFBBatchDraw_Box[0].startY = 0;
				mFBBatchDraw_Box[0].width = ((SCREEN_WIDTH/2 + 15) / 16) * 16;
				mFBBatchDraw_Box[0].height = ((SCREEN_HEIGHT/2 + 15) / 16) * 16;
				mFBBatchDraw_Box[1].startX = mFBBatchDraw_Box[0].width;
				mFBBatchDraw_Box[1].startY = 0;
				mFBBatchDraw_Box[1].width = (SCREEN_WIDTH - mFBBatchDraw_Box[0].width);
				mFBBatchDraw_Box[1].height = mFBBatchDraw_Box[0].height;
				mFBBatchDraw_Box[2].startX = mFBBatchDraw_Box[1].startX;
				mFBBatchDraw_Box[2].startY = mFBBatchDraw_Box[1].height;
				mFBBatchDraw_Box[2].width = mFBBatchDraw_Box[1].width;
				mFBBatchDraw_Box[2].height = (SCREEN_HEIGHT - mFBBatchDraw_Box[1].height);
				mFBBatchDraw_Box[3].startX = 0;
				mFBBatchDraw_Box[3].startY = mFBBatchDraw_Box[2].startY;
				mFBBatchDraw_Box[3].width = mFBBatchDraw_Box[2].startX;
				mFBBatchDraw_Box[3].height = mFBBatchDraw_Box[2].height;
			break;
			default:
				mFBBatchDraw_Box[0].startX = 0;
				mFBBatchDraw_Box[0].startY = 0;
				mFBBatchDraw_Box[0].width = SCREEN_WIDTH;
				mFBBatchDraw_Box[0].height = SCREEN_HEIGHT;
			break;
		}
	}
}

uint8_t BatchDraw_u8GetBatchDrawType(void)
{
	return BatchDrawType;
}


uint32_t SBBatchDrawLastSBPos = 0;
uint32_t SBBatchDrawCurSBPos = 0;
uint32_t SBBatchDrawSBPosStack[SBBatchDrawSBPosStackNumber] = {0};
static volatile uint8_t SBBatchDrawFlag = 0;
static volatile uint8_t SBBatchDrawDone = 1;
static volatile uint8_t SBBatchRefreshDone = 1;
SBBatchDraw_teMainSM mSBBatchDraw_teMainSM = SBBatchDraw_Idle;

void SBBatchDraw_vSetFlag(void)
{
	SBBatchDrawFlag = 1;
}

void SBBatchDraw_vClearFlag(void)
{
	if(BatchDrawType == BatchDraw_SB)
	{
		SBBatchDrawLastSBPos = 0;
	}
	SBBatchDrawFlag = 0;
}

uint8_t SBBatchDraw_u8GetFlag(void)
{
	return SBBatchDrawFlag;
}

void SBBatchDraw_vSetDrawDone(void)
{
	SBBatchDrawDone = 1;
}

void SBBatchDraw_vClearDrawDone(void)
{
	if(BatchDrawType == BatchDraw_SB)
	{
		for(uint8_t i = 0; i < (SBBatchDrawSBPosStackNumber - 1); i++)
		{
			SBBatchDrawSBPosStack[i] = SBBatchDrawSBPosStack[i+1];
		}
		SBBatchDrawSBPosStack[SBBatchDrawSBPosStackNumber-1] = 0;
	}
	SBBatchDrawDone = 0;
}

uint8_t SBBatchDraw_u8GetDrawDone(void)
{
	return SBBatchDrawDone;
}

uint32_t SBBatchDraw_u32GetSBBatchDrawLastSBPos(void)
{
	return SBBatchDrawLastSBPos;
}

uint32_t SBBatchDraw_u32GetSBBatchDrawMaxSize(void)
{
	if(SBBatchDraw_u8GetFlag())
	{
		return SBBatchDrawSBCacheSize;
	}
	else
	{
		return SBBatchDrawSBAddrMax;
	}
}

volatile uint32_t old_SBPos = 0;
void SBBatchDraw_vStartSBBatchDraw(uint32_t SBPos)
{
	#ifdef CHIP_GC9002_NoHyper
	return;
	#endif

	//if((DISPLAY_GetMode() != DISPLAY_Mode_MasterActive) && (DISPLAY_GetMode() != DISPLAY_Mode_MasterPassive))
	if(BatchDrawType == BatchDraw_FB)
	{
		return;
	}
	
    uint32_t irq;
	irq = csi_irq_save();
    
	if(!SBBatchDraw_u8GetFlag())
	{
		SBBatchDraw_vSetFlag();
		SBBatchDrawCurSBPos = SBPos;
		SBBatchDrawSBPosStack[0] = SBPos;
		mSBBatchDraw_teMainSM = SBBatchDraw_CopySBBatch;
	}
	else
	{
		uint32_t tmp = SBBatchDrawLastSBPos;
		uint8_t i = 0;
		
		for(i = 0; i < SBBatchDrawSBPosStackNumber; i++)
		{
			if(SBBatchDrawSBPosStack[i] == 0)
			{
				break;
			}
			tmp = SBBatchDrawSBPosStack[i];
		}
		if((SBPos - tmp) > SBBatchDrawSBCacheSize)
		{
			SBBatchDrawSBPosStack[i] = old_SBPos;
		}
	}
	old_SBPos = SBPos;
    csi_irq_restore(irq);
}

void SBBatchDraw_vClearRefreshDone(void)
{
    uint32_t irq;
    
    irq = csi_irq_save();
	if(BatchDrawType == BatchDraw_FB)
	{
		FBBatchCurBox = 0;
		for(int i = 0; i < sizeof(mFBBatchDraw_Box)/sizeof(FBBatchDraw_Box); i++)
		{
			mFBBatchDraw_Box[i].sbpos = 0;
		}
	}
	SBBatchRefreshDone = 0;
    csi_irq_restore(irq);
}

void SBBatchDraw_vSetRefreshDone(void)
{
    uint32_t irq;

    irq = csi_irq_save();
    
	if(BatchDrawType == BatchDraw_SB)
	{
		
		if(SBBatchDraw_u8GetFlag())
		{
			for(uint8_t i = 0; i < SBBatchDrawSBPosStackNumber; i++)
			{
				uint32_t tmp = SBBatchDrawLastSBPos;
				if(SBBatchDrawSBPosStack[i] == 0)
				{
					if(i > 0)
						tmp = SBBatchDrawSBPosStack[i - 1];
						
					if((sourceshift - tmp) > SBBatchDrawSBCacheSize)
					{
						SBBatchDrawSBPosStack[i] = old_SBPos;
						SBBatchDrawSBPosStack[i + 1] = sourceshift;
					}
					else
					{
						SBBatchDrawSBPosStack[i] = sourceshift;
					}
					break;
				}
			}
			
			SBBatchRefreshDone = 1;
		}
		
	}
	else if(BatchDrawType == BatchDraw_FB)
	{
		if(initAHMI)
		{
			if(sourceshift > SBBatchDrawSBAddrMax)
			{
				SBBatchDrawFlag = 1;
				mSBBatchDraw_teMainSM = SBBatchDraw_CopySBBatch;
			}
			else
			{
				FBBatchCurBox = 0;
				for(int i = 0; i < sizeof(mFBBatchDraw_Box)/sizeof(FBBatchDraw_Box); i++)
				{
					mFBBatchDraw_Box[i].sbpos = 0;
				}
			}
		}
	}
    
    csi_irq_restore(irq);
}

extern void csi_ahmi_ClearSourceShift(void);
uint8_t SBBatchDraw_u8GetRefreshDone(void)
{
	return SBBatchRefreshDone;
}

uint8_t BatchDraw_u8JudgeSBPos(void)
{
	if(BatchDrawType == BatchDraw_FB)
	{
		for(int i = 0; i < (sizeof(mFBBatchDraw_Box)/sizeof(FBBatchDraw_Box)); i++)
		{
			if(mFBBatchDraw_Box[i].sbpos > SBBatchDrawSBAddrMax)
			{
				DISPLAY_SetUpdateRange(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
				return 1;
			}
		}
	}
	return 0;
}

void SBBatchDraw_vMain(void)
{
	if(BatchDrawType == BatchDraw_SB)
	{
		uint32_t BackgroundSB[16];
		uint32_t FBAddressForSlaveMode = StartAddrOfDisplayBuffer;
		extern void AHMI_SetFBAddress(uint32_t address);
		
		switch(mSBBatchDraw_teMainSM)
		{
			case SBBatchDraw_Idle:
			
			break;
			
			case SBBatchDraw_CopySBBatch:
				if(AHMI_GetDrawDoneFlag())
				{
					if(SBBatchDrawLastSBPos == 0)
					{
						if((DISPLAY_GetMode() != DISPLAY_Mode_MasterActive) && (DISPLAY_GetMode() != DISPLAY_Mode_MasterPassive))
						{
							AHMI_SetFBAddress(FBAddressForSlaveMode);
						}
						AHMI_SetSBAdressForSBBatchDraw(0, SBAddress, (SBBatchDrawCurSBPos - SBBatchDrawLastSBPos));
					}
					else
					{
						if(AHMI_GetOutputFormat() == OUTPUTFORMAT_RGBA8888)
						{
							BackgroundSB[0] = (0x00000100 | RGBA8888);
						}
						else if((AHMI_GetOutputFormat() == OUTPUTFORMAT_RGB565) || (AHMI_GetOutputFormat() == OUTPUTFORMAT_RGB888) || (AHMI_GetOutputFormat() == OUTPUTFORMAT_SFBC))
						{
							BackgroundSB[0] = (0x00000100 | RGB565);
						}
						else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_Y8)
						{
							BackgroundSB[0] = (0x00000100 | Y8);
						}
						
						switch(AHMIOutputRotation)
						{
							case DEGREE0:
								BackgroundSB[1] = 0;
								BackgroundSB[2] = ((SCREEN_WIDTH) << 20);
								BackgroundSB[3] = ((SCREEN_WIDTH) << 20) | ((SCREEN_HEIGHT) << 4);
								BackgroundSB[4] = ((SCREEN_HEIGHT) << 4);
								BackgroundSB[5] = 0x100000;
								BackgroundSB[6] = 0;
								BackgroundSB[7] = 0;
								BackgroundSB[8] = 0;
								BackgroundSB[9] = 0x100000;
								BackgroundSB[10] = 0;
								BackgroundSB[11] = 0;
								BackgroundSB[12] = 0;
								BackgroundSB[13] = (SCREEN_WIDTH << 16) | SCREEN_HEIGHT;
								break;
							case DEGREE180:
								BackgroundSB[1] = 0;
								BackgroundSB[2] = ((SCREEN_WIDTH) << 20);
								BackgroundSB[3] = ((SCREEN_WIDTH) << 20) | ((SCREEN_HEIGHT) << 4);
								BackgroundSB[4] = ((SCREEN_HEIGHT) << 4);
								BackgroundSB[5] = 0xFFF00000;
								BackgroundSB[6] = 0;
								BackgroundSB[7] = 0;
								BackgroundSB[8] = 0;
								BackgroundSB[9] = 0xFFF00000;
								BackgroundSB[10] = 0;
								BackgroundSB[11] = ((SCREEN_WIDTH - 1) << 13);
								BackgroundSB[12] = ((SCREEN_HEIGHT - 1) << 13);
								BackgroundSB[13] = (SCREEN_WIDTH << 16) | SCREEN_HEIGHT;
								break;
							case DEGREE90:
								BackgroundSB[1] = 0;
								BackgroundSB[2] = ((SCREEN_HEIGHT) << 20);
								BackgroundSB[3] = ((SCREEN_HEIGHT) << 20) | ((SCREEN_WIDTH) << 4);
								BackgroundSB[4] = ((SCREEN_WIDTH) << 4);
								BackgroundSB[5] = 0;
								BackgroundSB[6] = 0x100000;
								BackgroundSB[7] = 0;
								BackgroundSB[8] = 0xFFF00000;
								BackgroundSB[9] = 0;
								BackgroundSB[10] = 0;
								BackgroundSB[11] = ((SCREEN_HEIGHT - 1) << 13);
								BackgroundSB[12] = 0;
								BackgroundSB[13] = (SCREEN_HEIGHT << 16) | SCREEN_WIDTH;
								break;
							case DEGREE270:
								BackgroundSB[1] = 0;
								BackgroundSB[2] = ((SCREEN_HEIGHT) << 20);
								BackgroundSB[3] = ((SCREEN_HEIGHT) << 20) | ((SCREEN_WIDTH) << 4);
								BackgroundSB[4] = ((SCREEN_WIDTH) << 4);
								BackgroundSB[5] = 0;
								BackgroundSB[6] = 0xFFF00000;
								BackgroundSB[7] = 0;
								BackgroundSB[8] = 0x100000;
								BackgroundSB[9] = 0;
								BackgroundSB[10] = 0;
								BackgroundSB[11] = 0;
								BackgroundSB[12] = ((SCREEN_WIDTH - 1) << 13);
								BackgroundSB[13] = (SCREEN_HEIGHT << 16) | SCREEN_WIDTH;
								break;
								
							default:
								BackgroundSB[1] = 0;
								BackgroundSB[2] = ((SCREEN_WIDTH) << 20);
								BackgroundSB[3] = ((SCREEN_WIDTH) << 20) | ((SCREEN_HEIGHT) << 4);
								BackgroundSB[4] = ((SCREEN_HEIGHT) << 4);
								BackgroundSB[5] = 0x100000;
								BackgroundSB[6] = 0;
								BackgroundSB[7] = 0;
								BackgroundSB[8] = 0;
								BackgroundSB[9] = 0x100000;
								BackgroundSB[10] = 0;
								BackgroundSB[11] = 0;
								BackgroundSB[12] = 0;
								BackgroundSB[13] = (SCREEN_WIDTH << 16) | SCREEN_HEIGHT;
								break;
						}

						//BackgroundSB[13] = (SCREEN_WIDTH << 16) | SCREEN_HEIGHT;
						if((DISPLAY_GetMode() != DISPLAY_Mode_MasterActive) && (DISPLAY_GetMode() != DISPLAY_Mode_MasterPassive))
						{
							BackgroundSB[14] = FBAddressForSlaveMode;
						}
						else
						{
							BackgroundSB[14] = AHMI_GetFBAdress();
						}
						BackgroundSB[15] = ((((SCREEN_WIDTH + TILESIZE - 1) / TILESIZE) - 1) << 16) | ((((SCREEN_HEIGHT + TILESIZE - 1) / TILESIZE) - 1) << 24);
						AHMI_SetSBAdressForSBBatchDraw(0, (uint32_t)BackgroundSB, SBBatchDrawBackgroundSBLength);
						AHMI_SetSBAdressForSBBatchDraw(SBBatchDrawBackgroundSBLength, SBAddress + SBBatchDrawLastSBPos, (SBBatchDrawCurSBPos - SBBatchDrawLastSBPos));
						
//						BackgroundSB[1] = 0;
//						BackgroundSB[2] = 0;
//						BackgroundSB[3] = (SCREEN_WIDTH << 16) | (SCREEN_HEIGHT);
//						BackgroundSB[4] = AHMI_GetFBAdress();
//						BackgroundSB[5] = ((((SCREEN_WIDTH + TILESIZE - 1) / TILESIZE) - 1) << 16) | ((((SCREEN_HEIGHT + TILESIZE - 1) / TILESIZE) - 1) << 24);
//						AHMI_SetSBAdressForSBBatchDraw(0, (uint32_t)BackgroundSB, 24);
//						AHMI_SetSBAdressForSBBatchDraw(24, SBAddress + SBBatchDrawLastSBPos, (SBBatchDrawCurSBPos - SBBatchDrawLastSBPos));
					}
					
					SBBatchDrawLastSBPos = SBBatchDrawCurSBPos;
					
					if((SBBatchDrawSBPosStack[0] == sourceshift) && SBBatchRefreshDone == 1)
					{
						SBBatchDrawSBPosStack[0] = 0;
						mSBBatchDraw_teMainSM = SBBatchDraw_WaitDrawDone;
					}
					else
					{
						mSBBatchDraw_teMainSM = SBBatchDraw_DrawSBBatch;
					}
				}
			break;
			
			case SBBatchDraw_DrawSBBatch:
			
				SBBatchDraw_vClearDrawDone();
				csi_ahmi_Draw_ForSBBatch(NULL, 0);
				
				mSBBatchDraw_teMainSM = SBBatchDraw_WaitDrawDone;
			break;
			
			case SBBatchDraw_WaitDrawDone:
				if(SBBatchDraw_u8GetDrawDone())
				{
					if(SBBatchDrawSBPosStack[0] == 0)
					{
						if(SBBatchDraw_u8GetRefreshDone())
						{
							if((DISPLAY_GetMode() != DISPLAY_Mode_MasterActive) && (DISPLAY_GetMode() != DISPLAY_Mode_MasterPassive))
							{
								AHMI_SetFBAddress(DRV_DISPLAY_AHB_SLAVE_BASE_ADDR);
							}
							csi_ahmi_ClearSourceShift();
						
							SBBatchDraw_vClearFlag();
							mSBBatchDraw_teMainSM = SBBatchDraw_Idle;
						}
					}
					else
					{
						SBBatchDrawCurSBPos = SBBatchDrawSBPosStack[0];
						mSBBatchDraw_teMainSM = SBBatchDraw_CopySBBatch;
					}
				}
			break;
			
			default:
			
			break;
		}
	}
	else if(BatchDrawType == BatchDraw_FB)
	{
		switch(mSBBatchDraw_teMainSM)
		{
			case SBBatchDraw_Idle:
			
			break;
			
			case SBBatchDraw_CopySBBatch:
				//AHMI_SetSBAdressForSBBatchDraw(0, SBAddress, sourceshift);
				if(mFBBatchDraw_Box[FBBatchCurBox].sbpos != 0)
				{
					AHMI_SetSBAdressForSBBatchDraw(0, StartAddrOfSourcebufferForFBBatch + SBBatchDrawSBAddrMax * FBBatchCurBox, mFBBatchDraw_Box[FBBatchCurBox].sbpos);
				}
				mSBBatchDraw_teMainSM = SBBatchDraw_DrawSBBatch;
			break;
			
			case SBBatchDraw_DrawSBBatch:
			
				SBBatchDraw_vClearDrawDone();
				csi_ahmi_Draw_ForSBBatch(mFBBatchDraw_Box, FBBatchCurBox++);
				
				mSBBatchDraw_teMainSM = SBBatchDraw_WaitDrawDone;
			break;
			
			case SBBatchDraw_WaitDrawDone:
				if(SBBatchDraw_u8GetDrawDone())
				{
					if(FBBatchCurBox < (sizeof(mFBBatchDraw_Box)/sizeof(FBBatchDraw_Box)))
					{
						mSBBatchDraw_teMainSM = SBBatchDraw_CopySBBatch;
					}
					else
					{
						csi_ahmi_ClearSourceShift();
						
						SBBatchDraw_vClearFlag();
						mSBBatchDraw_teMainSM = SBBatchDraw_Idle;
					}
				}
			break;
			
			default:
			
			break;
		}
	}
}


//uint8_t FBBatchDraw_JudgePoint(uint32_t x, uint32_t y, uint32_t vertex0_x, uint32_t vertex0_y, uint32_t vertex1_x, uint32_t vertex1_y)
//{
//	int t;
//	
//	if(vertex0_x>vertex1_x) {t=vertex0_x;vertex0_x=vertex1_x;vertex1_x=t;}
//	if(vertex0_y>vertex1_y) {t=vertex0_y;vertex0_y=vertex1_y;vertex1_y=t;}
//
//	if(x>=vertex0_x&&x<=vertex1_x&&y>=vertex0_y&&y<=vertex1_y)
//	{
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//}

uint8_t FBBatchDraw_JudgePoint(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t vertex0_x, int16_t vertex0_y, int16_t vertex1_x, int16_t vertex1_y)
{
	int flag = 0; //设置标记值，默认为重叠
	
	if (x0 > vertex1_x || y0 > vertex1_y) flag++; //初始点在3,6,7,8,9区域
	else if (x1 < vertex0_x || y1 < vertex0_y) flag++; //初始点在1,2,4区域
	//else if (x0 > vertex0_x && y0 > vertex0_y && x1 < vertex1_x && y1 < vertex1_y) flag++; //初始点在5区域
	
	if (flag == 0)
		return 1;
	else 
		return 0;
}

void FBBatchDraw_vCopyWidgetSB(uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t SBPos, uint16_t *pSBLength)
{
	u8* srcAddr = 0;
    u8* dstAddr = 0;
	u32 backgroundSBLength = 0;
    uint32_t irq;

    
	
	if((p0 == 0 && p1 == 0 && p2 == 0 && p3 == 0) || (((*pSBLength) & 0xFFF) == 0))
	{
		return;
	}

    irq = csi_irq_save();
	if(p0 == FBBatchDrawMagicNum && p1 == FBBatchDrawMagicNum && p2 == FBBatchDrawMagicNum && p3 == FBBatchDrawMagicNum)
	{
		for(int i = 0; i < sizeof(mFBBatchDraw_Box)/sizeof(FBBatchDraw_Box); i++)
		{
			if(((*pSBLength) >> (15 - i)) & 0x1)
			{
				if(mFBBatchDraw_Box[i].sbpos == 0)
				{
					if((*(uint32_t *)SBAddress) & 0x80000000)
						backgroundSBLength = 24;
					else
						backgroundSBLength = 64;
						
					srcAddr = (uint8_t *)(SBAddress);
					dstAddr = (uint8_t *)(StartAddrOfSourcebufferForFBBatch + SBBatchDrawSBAddrMax * i);
					for(int j = 0; j < backgroundSBLength; j++)
					{
						*(dstAddr++) = *(srcAddr++);
					}
					mFBBatchDraw_Box[i].sbpos += backgroundSBLength;
				}
				
				srcAddr = (uint8_t *)(SBAddress + SBPos);
				dstAddr = (uint8_t *)(StartAddrOfSourcebufferForFBBatch + SBBatchDrawSBAddrMax * i + mFBBatchDraw_Box[i].sbpos);
				for(int j = 0; j < (*pSBLength & 0xFFF); j++)
				{
					*(dstAddr++) = *(srcAddr++);
				}
				mFBBatchDraw_Box[i].sbpos += ((*pSBLength) & 0xFFF);
			}
		}
	}
	else
	{
		int16_t max_X, max_Y, min_X, min_Y;
		
		min_X = (int16_t)(p0 >> 16);
		max_X = (int16_t)(p0 >> 16);
		min_Y = (int16_t)(p0 & 0xFFFF);
		max_Y = (int16_t)(p0 & 0xFFFF);

		if((int16_t)(p1 >> 16) > max_X)
			max_X = (int16_t)(p1 >> 16);
		else if((int16_t)(p1 >> 16) < min_X)
			min_X = (int16_t)(p1 >> 16);
		if((int16_t)(p2 >> 16) > max_X)
			max_X = (int16_t)(p2 >> 16);
		else if((int16_t)(p2 >> 16) < min_X)
			min_X = (int16_t)(p2 >> 16);
		if((int16_t)(p3 >> 16) > max_X)
			max_X = (int16_t)(p3 >> 16);
		else if((int16_t)(p3 >> 16) < min_X)
			min_X = (int16_t)(p3 >> 16);

		if((int16_t)(p1 & 0xFFFF) > max_Y)
			max_Y = (int16_t)(p1 & 0xFFFF);
		else if((int16_t)(p1 & 0xFFFF) < min_Y)
			min_Y = (int16_t)(p1 & 0xFFFF);
		if((int16_t)(p2 & 0xFFFF) > max_Y)
			max_Y = (int16_t)(p2 & 0xFFFF);
		else if((int16_t)(p2 & 0xFFFF) < min_Y)
			min_Y = (int16_t)(p2 & 0xFFFF);
		if((int16_t)(p3 & 0xFFFF) > max_Y)
			max_Y = (int16_t)(p3 & 0xFFFF);
		else if((int16_t)(p3 & 0xFFFF) < min_Y)
			min_Y = (int16_t)(p3 & 0xFFFF);
			
		for(int i = 0; i < sizeof(mFBBatchDraw_Box)/sizeof(FBBatchDraw_Box); i++)
		{
			if(FBBatchDraw_JudgePoint(min_X, min_Y, max_X, max_Y, mFBBatchDraw_Box[i].startX, mFBBatchDraw_Box[i].startY, 
			mFBBatchDraw_Box[i].startX + mFBBatchDraw_Box[i].width, mFBBatchDraw_Box[i].startY + mFBBatchDraw_Box[i].height))
			{
				if(mFBBatchDraw_Box[i].sbpos == 0)
				{
					if((*(uint32_t *)SBAddress) & 0x80000000)
						backgroundSBLength = 24;
					else
						backgroundSBLength = 64;
						
					srcAddr = (uint8_t *)(SBAddress);
					dstAddr = (uint8_t *)(StartAddrOfSourcebufferForFBBatch + SBBatchDrawSBAddrMax * i);
					for(int j = 0; j < backgroundSBLength; j++)
					{
						*(dstAddr++) = *(srcAddr++);
					}
					mFBBatchDraw_Box[i].sbpos += backgroundSBLength;
				}
			
				srcAddr = (uint8_t *)(SBAddress + SBPos);
				dstAddr = (uint8_t *)(StartAddrOfSourcebufferForFBBatch + SBBatchDrawSBAddrMax * i + mFBBatchDraw_Box[i].sbpos);
				for(int j = 0; j < (*pSBLength & 0xFFF); j++)
				{
					*(dstAddr++) = *(srcAddr++);
				}
				mFBBatchDraw_Box[i].sbpos += ((*pSBLength) & 0xFFF);
				
				*pSBLength |= (1 << (15 - i));
			}
		}

//		uint32_t cur_x = 0, cur_y = 0;
//		uint8_t box_flag = 0;
//		*pSBLength &= 0x0FFF;
//		for(int p = 0; p < 4; p++)
//		{
//			switch(p)
//			{
//				case 0:
//					cur_x = p0 >> 16;
//					cur_y = p0 & 0xFFFF;
//				break;
//				case 1:
//					cur_x = p1 >> 16;
//					cur_y = p1 & 0xFFFF;
//				break;
//				case 2:
//					cur_x = p2 >> 16;
//					cur_y = p2 & 0xFFFF;
//				break;
//				case 3:
//					cur_x = p3 >> 16;
//					cur_y = p3 & 0xFFFF;
//				break;
//				default:
//				break;
//			}
//			
//			for(int i = 0; i < sizeof(mFBBatchDraw_Box)/sizeof(FBBatchDraw_Box); i++)
//			{
//				if(box_flag & (1 << i))
//					continue;
//					
//				if(FBBatchDraw_JudgePoint(cur_x, cur_y, mFBBatchDraw_Box[i].startX, mFBBatchDraw_Box[i].startY, 
//				mFBBatchDraw_Box[i].startX + mFBBatchDraw_Box[i].width, mFBBatchDraw_Box[i].startY + mFBBatchDraw_Box[i].height))
//				{
//					box_flag |= (1 << i);
//					if(mFBBatchDraw_Box[i].sbpos == 0)
//					{
//						if((*(uint32_t *)SBAddress) & 0x80000000)
//							backgroundSBLength = 24;
//						else
//							backgroundSBLength = 64;
//							
//						srcAddr = (uint8_t *)(SBAddress);
//						dstAddr = (uint8_t *)(StartAddrOfSourcebufferForFBBatch + SBBatchDrawSBAddrMax * i);
//						for(int j = 0; j < backgroundSBLength; j++)
//						{
//							*(dstAddr++) = *(srcAddr++);
//						}
//						mFBBatchDraw_Box[i].sbpos += backgroundSBLength;
//					}
//				
//					srcAddr = (uint8_t *)(SBAddress + SBPos);
//					dstAddr = (uint8_t *)(StartAddrOfSourcebufferForFBBatch + SBBatchDrawSBAddrMax * i + mFBBatchDraw_Box[i].sbpos);
//					for(int j = 0; j < (*pSBLength & 0xFFF); j++)
//					{
//						*(dstAddr++) = *(srcAddr++);
//					}
//					mFBBatchDraw_Box[i].sbpos += ((*pSBLength) & 0xFFF);
//					
//					*pSBLength |= (1 << (15 - i));
//					break;
//				}
//			}
//		}
	}
    csi_irq_restore(irq);
}
