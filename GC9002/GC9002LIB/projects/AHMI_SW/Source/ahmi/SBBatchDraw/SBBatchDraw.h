
#ifndef SBBATCHDRAW__H
#define SBBATCHDRAW__H
#include "publicDefine.h"
#include "drawImmediately_cd.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BatchDraw_SB	0
#define BatchDraw_FB	1

//#define SBBatchDraw
////#define FBBatchDraw
//
//#if defined(SBBatchDraw) && defined(FBBatchDraw)
//#error "Please select only one from SBBatchDraw or FBBatchDraw"
//#endif

#define SBBatchDrawBackgroundSBLength 	0x40

#define SBBatchDrawSBPosStackNumber 	8
#define SBBatchDrawSBCacheSize			(SBBatchDrawSBAddrMax - SBBatchDrawBackgroundSBLength)//(0x800 - 0x40)
#define SBBatchDrawSBAddrMax			0x800

typedef enum
{
	SBBatchDraw_Idle = 0,
    SBBatchDraw_CopySBBatch,
	SBBatchDraw_DrawSBBatch,
	SBBatchDraw_WaitDrawDone
}SBBatchDraw_teMainSM;

void BatchDraw_vSetBatchDrawType(uint8_t type);
uint8_t BatchDraw_u8GetBatchDrawType(void);


void SBBatchDraw_vSetFlag(void);
void SBBatchDraw_vClearFlag(void);
uint8_t SBBatchDraw_u8GetFlag(void);
void SBBatchDraw_vSetDrawDone(void);
void SBBatchDraw_vClearDrawDone(void);
uint8_t SBBatchDraw_u8GetDrawDone(void);
uint32_t SBBatchDraw_u32GetSBBatchDrawLastSBPos(void);
uint32_t SBBatchDraw_u32GetSBBatchDrawMaxSize(void);
void SBBatchDraw_vStartSBBatchDraw(uint32_t SBPos);
void SBBatchDraw_vClearRefreshDone(void);
void SBBatchDraw_vSetRefreshDone(void);
void SBBatchDraw_vMain(void);
	
uint8_t BatchDraw_u8JudgeSBPos(void);

typedef struct FBBatchDraw_Box
{
	uint16_t startX;
	uint16_t startY;
	uint16_t width;
	uint16_t height;
	uint16_t sbpos;
}FBBatchDraw_Box;

#define FBBatchDrawMagicNum 	0xC3A55A3C
void FBBatchDraw_vCopyWidgetSB(uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t SBPos, uint16_t *pSBLength);

#ifdef __cplusplus
}
#endif

#endif

