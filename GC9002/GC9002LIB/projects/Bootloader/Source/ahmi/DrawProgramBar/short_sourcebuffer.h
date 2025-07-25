/**
  ******************************************************************************
  * @file    short_sourcebuff.h
  * @author  
  * @version V1.0.0
  * @date    16-06-2023
  * @brief   The head file of Short Source Buffer.
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SHORT_SB_H
#define __SHORT_SB_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/

#include "stdio.h"
#include "string.h"


#ifndef YES
#define YES                         (1)
#endif

#ifndef NO
#define NO                          (0)
#endif

#ifndef TILESIZE
#define TILESIZE                    (32)
#endif

extern uint32_t SCREEN_WIDTH;
extern uint32_t SCREEN_HEIGHT;

/* Get Display Width from System Config or global variable */
#define SYN_GET_DISPLAY_WIDTH       (SCREEN_WIDTH)

/* Get Display Height from System Config or global variable */
#define SYN_GET_DISPLAY_HEIGHT      (SCREEN_HEIGHT)

typedef enum
{
    SB_enTxtrTypeDxt1 = 0,
    SB_enTxtrTypeDxt3,
    SB_enTxtrTypeAlpha1,
    SB_enTxtrTypePureColor,
    SB_enTxtrTypeRgb565,
    SB_enTxtrTypeArgb888,
    SB_enTxtrTypeY8,
    SB_enTxtrTypeRgb565_RE,
    SB_enTxtrTypePureMask,
	SB_enTxtrTypeAphha2,
    SB_enTxtrTypeAphha8,
    SB_enTxtrTypeAphha4,
    SB_enTxtrTypeArgb1555,
    SB_enTxtrTypeArgb4444,
    SB_enTxtrTypeDxt5,
    SB_enTxtrTypeArgb8888_RE,
    SB_enTxtrTypeTotalItems,
   
}SB_tenTextureType;


typedef enum
{
    SB_enAlphaMixTypeOver = 0,
    SB_enAlphaMixTypeIn,
    SB_enAlphaMixTypeAnd,
    SB_enAlphaMixTypeNot,
    SB_enAlphaMixTypeTotalItems,
   
}SB_tenAlphaMixType;

typedef struct{
    uint32_t alphaMixType       : 2;                            /* Alpha mix type */
    uint32_t fishEye            : 1;                            /* Not used */
    uint32_t useBuffuredAlpha   : 1;                            /* Indicate if use the buffered Alpha */
    uint32_t textureType        : 4;                            /* Indicate the input Texture Type */
    
    uint32_t forceRender        : 1;                            /* Force to cover Alpha buffer and Color buffer of Tile */
    uint32_t res1               : 6;                            /* Reserved */
    uint32_t isLastTexture      : 1;                            /* Indicate if this source buffer is the last one */

    uint32_t res2               : 15;                           /* Reserved */
    uint32_t isShortSB          : 1;                            /* Indicate if this source buffer is short */

}SB_tstTextureAttrBits;


typedef union
{
    volatile uint32_t u32Val;
    SB_tstTextureAttrBits bits;
}SB_tunTextureAttr, *SB_tunTextureAttrPtr;


typedef struct{
    uint8_t                         u8StartTileX;                   /* Start Tile X coordinate value */
    uint8_t                         u8StartTileY;                   /* Start Tile X coordinate value */
    uint8_t                         u8EndTileX;                     /* End Tile X coordinate value */
    uint8_t                         u8EndTileY;                     /* End Tile X coordinate value */
}SB_tstTileInfoBytes;


typedef union
{
    volatile uint32_t u32Val;
    SB_tstTileInfoBytes bytes;
}SB_tunTileInfo, *SB_tunTileInfoPtr;


typedef struct 
{	
    SB_tunTextureAttr               unTextureAttr;                  /* Texture attibute (4 bytes)  */
    uint32_t                        u32MatrixG;                     /* Matrix G */
    uint32_t                        u32MatrixH;                     /* Matrix H */

    uint16_t                        u16TextureHeight;               /* Texture Width */
    uint16_t                        u16TextureWidth;                /* Texture Height */
    uint32_t                        u32TextureAddrOrColor;          /* If Texture type is pure color, it will save PureColor(0xAARRGGBB), otherwise save Texture Address */

    SB_tunTileInfo                  unTileInfo;                     /* Tile information */
}SB_tstShortSB, *SB_tstShortSBPtr;

#define BIT_SET_LAST_SB             (0x00008000)
#define SB_SET_LAST_SB(pSB)         ((pSB)->unTextureAttr.u32Val |= BIT_SET_LAST_SB)

void SB_vCalculateMatricsAndTile(SB_tstShortSBPtr pShortSB, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height);
void SB_vInitStructByPureColor_Force(SB_tstShortSBPtr pShortSB, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, uint32_t pureColor);
void SB_vInitStructByPureColor_Noforce(SB_tstShortSBPtr pShortSB, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, uint32_t pureColor);
void SB_vInitStructByAlpha1_Noforce(SB_tstShortSBPtr pShortSB, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, uint32_t textureAddr);
void SB_vInitStructByAlpha1_End(SB_tstShortSBPtr pShortSB, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, uint8_t alpha, uint32_t color);
void SB_vInitStructByLastFrameBuffer_Force(SB_tstShortSBPtr pShortSB, uint32_t frameBufAddr, SB_tenTextureType displayOutputType);
void AHMI_SetPara(uint16_t startX, uint16_t startY, uint16_t width, uint16_t height);



	 

#ifdef __cplusplus
}
#endif

#endif /* __SHORT_SB_H */





