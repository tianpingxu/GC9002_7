////////////////////////////////////////////////////////////////////////////////
// Company:      SuXianwei
// Engineer:     AHMI 
//
// Create Date:   2023/06/16
// File Name:     short_sourcebuffer.c
// Project Name:  Bootloader_Redesign
// 
// Revision:
// Revision 1.0.0 - File Created 2023/06/16
////////////////////////////////////////////////////////////////////////////////

#include "short_sourcebuffer.h"
#include "gc90x_ahmi.h"

#define TILESIZE_16             (16)



__attribute__((optimize(0))) void AHMI_SetPara(uint16_t startX, uint16_t startY, uint16_t width, uint16_t height)
{
    uint16_t validWidth, validHeight;
    uint16_t displayWidth, displayHeight;
    uint8_t startTileX, startTileY, endTileX, endTileY;

    displayWidth = (uint16_t)SYN_GET_DISPLAY_WIDTH;
    displayHeight = (uint16_t)SYN_GET_DISPLAY_HEIGHT;
    
    validWidth = (startX + width) <= displayWidth ? width : (displayWidth - startX);
    //validWidth = displayWidth;
    validHeight = (startY + height) <= displayHeight ? height : (displayHeight - startY);

#if 0
    DISPLAY->timing_param0 &= 0x0000FFFF;
	DISPLAY->timing_param0 |= (validWidth << 16);
	
	DISPLAY->timing_param2 &= 0x0000FFFF;
	DISPLAY->timing_param2 |= (validHeight << 16);
    

    DISPLAY->col_update_area = ((startX + validWidth - 1) << 16) + startX;

    DISPLAY->row_update_area = ((startY + validHeight - 1) << 16) + startY;
	
	DISPLAY->total_byte_cnt = validWidth * validHeight * 2;



    startTileX = ((DISPLAY->col_update_area & 0xFFFF) + 1) / 16;
	startTileY = ((DISPLAY->row_update_area & 0xFFFF) + 1) / 16;
	endTileX = startTileX + ((((DISPLAY->timing_param0 & 0xFFFF0000) >> 16)+15)/16 - 1);
	endTileY = startTileY + ((((DISPLAY->timing_param2 & 0xFFFF0000) >> 16)+15)/16 - 1);
#endif
    startTileX = startX  / TILESIZE_16;
    startTileY = startY  / TILESIZE_16;
    endTileX = (startX + validWidth + TILESIZE_16 - 1) / TILESIZE_16 - 1;
    endTileY = (startY + validHeight + TILESIZE_16 - 1) / TILESIZE_16 - 1;    

    
    /* set X/Y direction start and end tile according to XDIR_StartTile,YDIR_StartTile,XDIR_EndTile,YDIR_EndTile */
	AHMI->DTBR = (uint32_t)(startTileX | (startTileY << 8)
										| (endTileX << 16) | endTileY << 24);


    /* set frame width and hight according to FRAME_Width and FRAME_Hight */
	//AHMI->FSR = (uint32_t)(validWidth | (validHeight << 16));
}

/**
  * @name       SB_vCalculateMatricsAndTile
  * @brief      Calculate Short SourceBuffer Matrics and Tile
  * @note       
  * @param      pShortSB: pointer to Short SourceBuffer
  * @param      startX: start X coordinate value
  * @param      startY: start Y coordinate value
  * @param      width: width of the pure color area
  * @param      height: height of the pure color area 
  * @retval     void
  * @author     
  * @date       2023-06-16
  */
void SB_vCalculateMatricsAndTile(SB_tstShortSBPtr pShortSB, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height)
{
    uint16_t validWidth, validHeight;
    uint16_t displayWidth, displayHeight;

    displayWidth = (uint16_t)SYN_GET_DISPLAY_WIDTH;
    displayHeight = (uint16_t)SYN_GET_DISPLAY_HEIGHT;
    
    //validWidth = (startX + width) <= displayWidth ? width : (displayWidth - startX);
    validWidth = width;
    validHeight = (startX + height) <= displayHeight ? height : (displayHeight - startY);
    
    pShortSB->u16TextureWidth = validWidth;
    pShortSB->u16TextureHeight = validHeight;

    if(0 == startX)
    {
        pShortSB->u32MatrixG = 0;
    }
    else
    {
        pShortSB->u32MatrixG = -(startX << 13);
    }
    if(0 == startY)
    {
        pShortSB->u32MatrixH = 0;
    }
    else
    {
        pShortSB->u32MatrixH = -(startY << 13);
    }

    pShortSB->unTileInfo.bytes.u8StartTileX = startX  / TILESIZE;
    pShortSB->unTileInfo.bytes.u8StartTileY = startY / TILESIZE;
    pShortSB->unTileInfo.bytes.u8EndTileX = (startX + validWidth + TILESIZE - 1) / TILESIZE - 1;
    pShortSB->unTileInfo.bytes.u8EndTileY = (startY + validHeight + TILESIZE - 1) / TILESIZE - 1;
    
}

/**
  * @name       SB_vInitStructByPureColor_Force
  * @brief      Initialization function to prepare pure color background
  * @note       Usually as the first Short SourceBuffer to make
  * @param      pShortSB: pointer to Short SourceBuffer
  * @param      startX: start X coordinate value
  * @param      startY: start Y coordinate value
  * @param      width: width of the pure color area
  * @param      height: height of the pure color area
  * @param      pureColor: pure color
  * @retval     void
  * @author     
  * @date       2023-06-16
  */
void SB_vInitStructByPureColor_Force(SB_tstShortSBPtr pShortSB, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, uint32_t pureColor)
{
    memset(pShortSB, 0, sizeof(SB_tstShortSB));
    pShortSB->unTextureAttr.bits.isShortSB = YES;
    /* If as first SourceBuffer , forceRender must be 1, it will cover old Alpha Buffer and Color Buffer */
    pShortSB->unTextureAttr.bits.forceRender = YES;
    pShortSB->unTextureAttr.bits.textureType = SB_enTxtrTypePureColor;
    pShortSB->u32TextureAddrOrColor = 0xFF000000 | pureColor;
    
    SB_vCalculateMatricsAndTile(pShortSB, startX, startY, width, height);
}

/**
  * @name       SB_vInitStructByPureColor_Noforce
  * @brief      Initialization function to prepare pure color background with no force rendering
  * @note       Usually as the first Short SourceBuffer to make
  * @param      pShortSB: pointer to Short SourceBuffer
  * @param      startX: start X coordinate value
  * @param      startY: start Y coordinate value
  * @param      width: width of the pure color area
  * @param      height: height of the pure color area
  * @param      pureColor: pure color
  * @retval     void
  * @author     
  * @date       2023-06-16
  */
void SB_vInitStructByPureColor_Noforce(SB_tstShortSBPtr pShortSB, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, uint32_t pureColor)
{
    memset(pShortSB, 0, sizeof(SB_tstShortSB));
    //pShortSB->unTextureAttr.bits.forceRender = YES;
    pShortSB->unTextureAttr.bits.isShortSB = YES;
    /* If as first SourceBuffer , forceRender must be 1, it will cover old Alpha Buffer and Color Buffer */
    pShortSB->unTextureAttr.bits.textureType = SB_enTxtrTypePureColor;
    pShortSB->u32TextureAddrOrColor = 0xFF000000 | pureColor;
    
    SB_vCalculateMatricsAndTile(pShortSB, startX, startY, width, height);
    //AHMI_SetPara(startX, startY, width, height);
}


/**
  * @name       SB_vInitStructByAlpha1_Noforce
  * @brief      Apply Alpha1(ASCII Font texture) to GPU buffer
  * @note       Important: need to use the alpha and color in Buffer , otherwise no effect
  * @param      pShortSB: pointer to Short SourceBuffer
  * @param      startX: start X coordinate value
  * @param      startY: start Y coordinate value
  * @param      width: width of the pure color area
  * @param      height: height of the pure color area
  * @param      textureAddr: texture address
  * @retval     void
  * @author     
  * @date       2023-06-16
  */
void SB_vInitStructByAlpha1_Noforce(SB_tstShortSBPtr pShortSB, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, uint32_t textureAddr)
{
    memset(pShortSB, 0, sizeof(SB_tstShortSB));
    pShortSB->unTextureAttr.bits.isShortSB = YES;
    pShortSB->unTextureAttr.bits.textureType = SB_enTxtrTypeAlpha1;
    pShortSB->u32TextureAddrOrColor = textureAddr;
    
    SB_vCalculateMatricsAndTile(pShortSB, startX, startY, width, height);
}


/**
  * @name       SB_vInitStructByAlpha1_End
  * @brief      This will make the SourceBuffer as the last one, so GPU will end draw
  * @note       
  * @param      pShortSB: pointer to Short SourceBuffer
  * @param      startX: start X coordinate value
  * @param      startY: start Y coordinate value
  * @param      width: width of the pure color area
  * @param      height: height of the pure color area
  * @param      alpha: alpha value to use
  * @param      color: combine with alpha
  * @retval     void
  * @author     
  * @date       2023-06-16
  */
void SB_vInitStructByAlpha1_End(SB_tstShortSBPtr pShortSB, uint16_t startX, uint16_t startY, uint16_t width, uint16_t height, uint8_t alpha, uint32_t color)
{
    memset(pShortSB, 0, sizeof(SB_tstShortSB));
    pShortSB->unTextureAttr.bits.isShortSB = YES;
    pShortSB->unTextureAttr.bits.textureType = SB_enTxtrTypePureColor;
    pShortSB->unTextureAttr.bits.useBuffuredAlpha = YES;
    pShortSB->unTextureAttr.bits.alphaMixType = SB_enAlphaMixTypeOver;
    pShortSB->unTextureAttr.bits.isLastTexture = YES;
    pShortSB->u32TextureAddrOrColor = (alpha << 24) | (color & 0x00FFFFFF);
    
    SB_vCalculateMatricsAndTile(pShortSB, startX, startY, width, height);
}


/**
  * @name       SB_vInitStructByLastFrameBuffer_Force
  * @brief      Use last frame Buffer data as background
  * @note       
  * @param      pShortSB: pointer to Short SourceBuffer
  * @param      frameBufAddr: Frame Buffer start Address
  * @param      displayOutputType: Display output type, RGB565/ARGB8888/..
  * @retval     void
  * @author     
  * @date       2023-06-17
  */
void SB_vInitStructByLastFrameBuffer_Force(SB_tstShortSBPtr pShortSB, uint32_t frameBufAddr, SB_tenTextureType displayOutputType)
{
    memset(pShortSB, 0, sizeof(SB_tstShortSB));
    pShortSB->unTextureAttr.bits.isShortSB = YES;
    /* If as first SourceBuffer , forceRender must be 1, it will cover old Alpha Buffer and Color Buffer */
    pShortSB->unTextureAttr.bits.forceRender = YES;
    pShortSB->unTextureAttr.bits.textureType = displayOutputType;
    pShortSB->u32TextureAddrOrColor = frameBufAddr;
    
    SB_vCalculateMatricsAndTile(pShortSB, 0, 0, SYN_GET_DISPLAY_WIDTH, SYN_GET_DISPLAY_HEIGHT);
}


