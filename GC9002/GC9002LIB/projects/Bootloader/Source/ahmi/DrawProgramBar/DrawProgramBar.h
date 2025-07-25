/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRAW_PROGRAM_BAR_H
#define __DRAW_PROGRAM_BAR_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "short_sourcebuffer.h"
#include "cfg_c1.h"

/* Specify customer upgrading hint string, default value is "Information" */
#define CONFIG_CUSTOMER_INFO_HINT_STRING                                "Information"

/* Specify customer upgrading hint string color: 0xXXXXXX, e.g. PINK: 0x0xFF00FF */
#define CONFIG_CUSTOMER_INFO_HINT_STRING_COLOR                          (0xFF00FF)

/* Specify customer info start row (uint: 16pixel), default value is 0 */
#define CONFIG_CUSTOMER_INFO_START_ROW                                  (0)

/* Specify customer info start column (uint: 8pixel), default value is 10  */
#define CONFIG_CUSTOMER_INFO_START_COLUMN                               (10)

/* Config the background color, default color is blue:0x0000FF */
#define CONFIG_BACKGROUND_COLOR                                         (0x0000FF)

#define CONFIG_UPG_STEP                                                 (5)


#define CPU0BIN 				0
#define TEXTURE 				1
#define CPU1BIN 				2
#define PRODUCT 				3
#define CALIBRATION 			4
#define HWCONFIG 				5 
#define EraseStoreTagSpace		6 
#define FILETYPE_END            7  //the last one

#define VERIFY_FILE 			0
#define ERASE 					1
#define WRITEFILE 				2
#define VERIFY_DATA 			3
#define UPGRADE_OK 			    4

#define UPGRADE_OPTYPE_END      5  //the last one

//#define TILESIZE 				32 //the size of TILE

#define ErrorBackground_YELLOW          (0xFFFFFF00)
#define ErrorBackground_BLUE            (0xFF0000FF)
#define ErrorBackground_GREEN           (0xFF00FF00)
#define ErrorBackground_RED             (0xFFFF0000)

#define FONT_WIDTH                      (0x8)
#define FONT_HEIGHT                     (0x10)
/* Indicate Line number decided by DRAW module */
#define LINE_NUM_AUTO                   (0xFE)

/* Indicate this isn't Upgrade File, no need to look up the saved line number */
#define NOT_UPG_FILE                    (0xFE)

/* Config the size of short SourceBuffer(one size is 24Byte) */
#define CONFIG_SHORT_SB_SIZE            (6)
/* Config the buffer size used to record cur line of the special upgrade file */
#define CONFIG_LINE_INFO_SIZE           (10)

/* These two config depend on free memory size(CONFIG_STR_TEXTURE_WIDTH x CONFIG_STR_TEXTURE_HEIGHT / 8 = occupied memory size), default size is 0x200 */
#define CONFIG_STR_TEXTURE_WIDTH        (0x100)
#define CONFIG_STR_TEXTURE_HEIGHT       (0x10)


#define CONFIG_FRAME_BUF_ADDR           (FRAME_BASE_ADDR)
#define CONFIG_TEXTURE_OUTPUT_TYPE      (SB_enTxtrTypeRgb565)




/* Synchronization with CFG, if interface changed, only need to redefine macro in .h file */
#define SYN_GET_CTRL_BL_PORT                                (CFG_pCfg2Port(CFG_Get_sys_ctrl_bl_port()))
#define SYN_GET_CTRL_BL_PIN                                 (CFG_u32Cfg2Pin(CFG_Get_sys_ctrl_bl_pin()))
#define SYN_GET_SYS_IF_BL_ALL_ON                            (CFG_Get_sys_if_bl_all_on())



typedef struct 
{	
    bool                            bInited;                        /* Indicate if module initialized  */
    uint8_t                         u8CurLineNum;                   /* Current line number */    
    uint8_t                         u8ShortSBIndex;                 /* Index of Short SourceBuffer */
    uint8_t                         u8TextureOutputType;            /* Indicate Texture output type */
    
    uint8_t                         u8ShortSBTotalNum;              /* Total number of Short SourceBuffer */
    uint8_t                         u8UpgFileTotalNum;              /* Total number of  */
    uint8_t                         res1[2];

    uint16_t                        u16StrTextureWidth;             /* the created String Texture Width */
    uint16_t                        u16StrTextureHeight;            /* the created String Texture Height */
    uint32_t                        u32StrTextureAddr;              /* the address of created String Texture */
    uint32_t                        u32FrameBufAddr;                /* the FrameBuffer address */

    SB_tstShortSBPtr                pstShortSB;                     /* Tile information */
    uint8_t*                        pu8UpgFileLineInfo;             /* used to record the line infomation of each Upgrade file */
}DRAW_tstDrawWorkBuf, *DRAW_tstDrawWorkBufPtr;




void DrawProgressBar(uint32_t width, uint32_t height, uint32_t filetype, uint32_t optype, uint32_t value);
void DrawErrorBackground(uint32_t width, uint32_t height, uint8_t level, const uint8_t* pChar);
void SetString(uint32_t screenwidth, uint32_t screenheight, uint16_t posX, uint16_t posY, uint16_t FontWidth, uint16_t FontHeight, uint32_t FontStartAddr, uint32_t FontCode, uint8_t type);
void DisplayString(uint32_t screenwidth, uint32_t screenheight, uint16_t LineNo, uint8_t* pStrFontCode, uint16_t strFontCodeLen);
void ClearAllString(void);
void InitAHMI(void);
void DRAW_vInit(void);
void DRAW_vDeinit(void);
void DRAW_vDrawString(uint8_t upgFileIndex, uint8_t startFontX, uint8_t startFontY, uint32_t color, const char* pStr, bool forceErase);
void DRAW_vBackLightOpen(void);
void DRAW_vBackLightClose(void);
void DRAW_vSetFramebufState(bool framebufState);
bool DRAW_bGetFramebufState(void);





#ifdef __cplusplus
}
#endif

#endif //__DRAW_PROGRAM_BAR_H

