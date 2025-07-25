#include "platform.h"
#include "gc90x_ahmi.h"
#include "gc90x_display.h"
#include "gc90x_cache.h"
#include "gc90x_initial.h"
#include "MidTimer.h"
#include "drawImmediately_cd.h"
#include "csi_hal_core.h"

#if defined(EMBEDDED) && defined(VERSION_2)
#include "AHMIInterface.h"
#if (defined CHIP_GC9005)
#include "gc9005_flash.h"
#include "HUDDraw.h"
#include "SBBatchDraw.h"
#elif (defined CHIP_GC9002)
#include "SBBatchDraw.h"
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

#if (defined VERSION_2) && (defined EMBEDDED) 
#if (defined CHIP_GC9002)
#define TEXTUREFLASH_START_ADDR 	(ROM_CONFIG_BASE_ADDR + 0x10000)
#define TEXTURE_START_ADDR_FOR_GPU  (ROM_CONFIG_BASE_ADDR + 0x10000)
#define TEXTURE_ADDR_OFFSET 		(TEXTUREFLASH_START_ADDR - 0x08000000)


#define AHMI_PARTIAL_DRAW_NONE				0   //do not use partial draw
#define AHMI_PARTIAL_DRAW_DOUBLE_BUFFER		1   //use partial draw with bouble framebuffer for RGB screen
#define AHMI_PARTIAL_DRAW_SINGLE_BUFFER		2   //use partial draw with single framebuffer for RGB screen
#define AHMI_PARTIAL_DRAW_WITHOUT_BUFFER	3   //use partial draw without framebuffer for spical screen
#define AHMI_PARTIAL_DRAW_TYPE_END			4   //end of the type list
uint32_t GetAHMIParticalDrawType(void);
void SetAHMIParticalDrawType(uint32_t type);

#elif (defined CHIP_GC9005)
#define TEXTUREFLASH_START_ADDR     (0xA0000000 + TextureDataStartAddr)
#define TEXTURE_START_ADDR_FOR_GPU  (0xA0000000)
#define TEXTURE_ADDR_OFFSET         0
#else
#define TEXTUREFLASH_START_ADDR 	(0xA0000000)
#define TEXTURE_START_ADDR_FOR_GPU  (0xA0000000)
#define TEXTURE_ADDR_OFFSET 		0
#endif
#endif

void csi_ahmi_DrawForFivePoint(void);
void csi_ahmi_SwitchBuffer(void);
#if defined(CHIP_GC9005)
void csi_ahmi_SetFBAdress(uint8_t type);
void csi_ahmi_Draw_ForSBBatch(const FBBatchDraw_Box *pFBBatchDraw_Box, uint8_t boxID);
#ifdef HUDDraw_ENABLE
void csi_ahmi_Draw_ForHUD(uint32_t tmpFBAddr, HUDDraw_teQuadrant qID);
#endif
#endif
#if (defined CHIP_GC9002)
void csi_ahmi_Draw_ForSBBatch(const FBBatchDraw_Box *pFBBatchDraw_Box, uint8_t boxID);
#endif
void csi_ahmi_Draw(void);
void csi_ahmi_DrawDoneHandler(void);
void csi_ahmi_DisplayDoneHandler(void);

void csi_ahmi_StartDrawingTimer(void);
void csi_ahmi_StopDrawingTimer(void);
void csi_ahmi_DrawingTimeoutHandler(void);

void csi_ahmi_EnableDisplay(void);
void csi_ahmi_DisableDisplay(void);
void csi_ahmi_SetDisplayContrast(uint32_t contrast);
void csi_ahmi_SetDisplaySaturation(uint32_t saturation);
void csi_ahmi_SetDisplayBrightness(uint32_t brightness);
void csi_ahmi_SetDisplayPriParamForRotate(void);

void csi_ahmi_ReadConfigData(void* data_addr, uint32_t len);
void csi_ahmi_SeekReadConfigDataAddr(uint32_t addr);
uint32_t csi_ahmi_GetCurConfigDataAddr(void);

uint32_t csi_ahmi_GetTagValue(unsigned int tagID);
uint32_t csi_ahmi_GetRegValue(unsigned int regID);
void csi_ahmi_SetTagValue(uint32_t ID, uint32_t value);
void csi_ahmi_SetRegValue(unsigned int ID, unsigned int value);
unsigned short int csi_ahmi_RegID2TagID(unsigned int reg_id);
unsigned int csi_ahmi_SetStrRegValue(unsigned int ID, char* str, unsigned short size);
unsigned int csi_ahmi_SetArrRegValue(unsigned int ID, char* arr, unsigned short size);
void csi_ahmi_GetStrRegValue(unsigned int ID, char* str);
void csi_ahmi_SetStrTagValue(unsigned int ID, char* str);
void csi_ahmi_SetStringTag(unsigned int ID, char* str, unsigned short size);




#ifdef __cplusplus
}
#endif
