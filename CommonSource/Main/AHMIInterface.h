////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     Trace.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/07/27  by YuChunYing 
// Additional Comments:
// Revision:
// Revision 1.6.2 - Modify 2019/07/16 by Yu ChunYing
// Additional Comments:
//   修改嵌入式软件和FPGA版本获取函数声明
//    
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef USER_INTERFACE
#define USER_INTERFACE

#ifdef __cplusplus 
extern "C" {
#endif

typedef enum
{
    EM_ErrRetInit_InitTextureFlashErr = 1,
    EM_ErrRetInit_GetProductDataErr,
    EM_ErrRetInit_CreateTaskErr,
    EM_ErrRetInit_LoadConfigDataErr,
    EM_ErrRetInit_MallocSpaceErr,
    EM_ErrRetInit_NULLPointerErr,
	EM_ErrRetInit_DataOverrun,
	EM_ErrRetInit_ReadWaveFilterErr,
	EM_ErrRetInit_ReadWaveFilterStatusErr,
	EM_ErrRetInit_ReadSoundErr,
	EM_ErrRetInit_ReadTagErr,
	EM_ErrRetInit_ReadStringTagErr,
	EM_ErrRetInit_ReadArryDataErr,
	EM_ErrRetInit_ReadFontRangesErr,
	EM_ErrRetInit_ReadHintErr,
	EM_ErrRetInit_InitAnimationMsgErr,
	EM_ErrRetInit_LoadFirstPageErr
   
}EM_ErrRetInit;

void initQueue(void);
void initSemaphore(void);
int startAHMI( void );
unsigned int getTagValue(unsigned int ID);
void setTagValue(unsigned int ID, unsigned int value);
void setRegValue(unsigned int ID, unsigned int value);
unsigned short int regID2TagID(unsigned int reg_id);
unsigned int setArrRegValue(unsigned int ID, char* arr, unsigned short size);
void getArrRegValue(unsigned int ID, char* arr, unsigned short size);
unsigned int setStrRegValue(unsigned int ID, char* str, unsigned short size);
void getStrRegValue(unsigned int ID, char* str);
void setStrTagValue(unsigned int ID, char* str);
void setStringTag(unsigned int ID, char* str, unsigned short size);

void playSoundIF(unsigned int id);
void pauseSoundIF(unsigned int id);
void resumeSoundIF(unsigned int id);
void stopSoundIF(unsigned int id);

void CopyPaste_background(unsigned int copy1_IMM, unsigned int copy2_IMM, unsigned short widgetID, unsigned int paste_IMM);

#ifdef EMBEDDED
#if defined(EMBEDDED) && defined(VERSION_2)
#define StoreTagMaxSizeOfWord              		(1024)
#define StoreTagMaxSizeOfByte              		(4096)

#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#if defined(CHIP_GC9002)
#define ROM_CONFIG_BASE_ADDR      						((uint32_t)0x080F0000)
#elif defined(CHIP_GC9005)
#define ROM_CONFIG_BASE_ADDR                            ((uint32_t)0xA0070000)
#else
#endif

#define BANK_WRITE_START_ADDR_PRODUCT_PARA              ((uint32_t)ROM_CONFIG_BASE_ADDR + 0xF000)
#define BANK_WRITE_END_ADDR_PRODUCT_PARA                ((uint32_t)ROM_CONFIG_BASE_ADDR + 0xFFFC)

#define BANK_WRITE_START_ADDR_HWCONFIG                  ((uint32_t)ROM_CONFIG_BASE_ADDR + 0xE000)
#define BANK_WRITE_END_ADDR_HWCONFIG                    ((uint32_t)ROM_CONFIG_BASE_ADDR + 0xEFFC)

#define BANK_WRITE_START_ADDR_STORETAG_PARA      		((uint32_t)ROM_CONFIG_BASE_ADDR + 0xD000)
#define BANK_WRITE_AND_ADDR_STORETAG_PARA      	 		((uint32_t)ROM_CONFIG_BASE_ADDR + 0xDFFC)

#define BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA      	((uint32_t)ROM_CONFIG_BASE_ADDR + 0xC000)
#define BANK_WRITE_AND_ADDR_STORESTRINGTAG_PARA      	((uint32_t)ROM_CONFIG_BASE_ADDR + 0xCFFC)

#define BANK_WRITE_START_ADDR_FOR_TOUCH                 ((uint32_t)ROM_CONFIG_BASE_ADDR + 0xB000)
#define BANK_WRITE_END_ADDR_FOR_TOUCH                   ((uint32_t)ROM_CONFIG_BASE_ADDR + 0xBFFC)

#define BANK_WRITE_START_ADDR_ERRMESSAGE_PARA      		((uint32_t)ROM_CONFIG_BASE_ADDR + 0xA000)
#define BANK_WRITE_AND_ADDR_ERRMESSAGE_PARA      	 	((uint32_t)ROM_CONFIG_BASE_ADDR + 0xAFFC)
#else
#define BANK_WRITE_START_ADDR_STORETAG_PARA      		((uint32_t)0x0807D000)
#define BANK_WRITE_AND_ADDR_STORETAG_PARA      	 		((uint32_t)0x0807DFFC)

#define BANK_WRITE_START_ADDR_STORESTRINGTAG_PARA      	((uint32_t)0x0807C000)
#define BANK_WRITE_AND_ADDR_STORESTRINGTAG_PARA      	((uint32_t)0x0807CFFC)

#define BANK_WRITE_START_ADDR_ERRMESSAGE_PARA      		((uint32_t)0x0807A000)
#define BANK_WRITE_AND_ADDR_ERRMESSAGE_PARA      	 	((uint32_t)0x0807AFFC)
#endif

#endif
void DrawFivePointPage(
	uint16_t ScreenWidth,
	uint16_t ScreenHeight,
	uint32_t BackGroundColor,
	uint16_t PointSize,
	//LeftTop position of LeftTop Point
	uint16_t LeftTopX, 
	uint16_t LeftTopY,
	uint32_t Point0Color,
	//LeftTop position of RightTop Point
	uint16_t RightTopX,
	uint16_t RightTopY,
	uint32_t Point1Color,	
	//LeftTop position of LeftBottum Point
	uint16_t LeftBottumX,
	uint16_t LeftBottumY,
	uint32_t Point2Color,
	//LeftTop position of RightBottum Point
	uint16_t RightBottumX,
	uint16_t RightBottumY,
	uint32_t Point3Color,
	//LeftTop position of Center Point
	uint16_t CenterX,
	uint16_t CenterY,
	uint32_t Point4Color
);

#endif
#if defined(EMBEDDED) && defined(VERSION_2) 
uint32_t MallocFromGCHeap(uint32_t size);
unsigned int GetTagValueC(unsigned int ID);
void SetTagValueC(unsigned int ID, unsigned int value);
void SetStrRegValueC(unsigned int ID, char* str);
void SetStrTagValueC(unsigned int ID, char* str);
#endif
	
#ifdef __cplusplus 
}
#endif

#endif
