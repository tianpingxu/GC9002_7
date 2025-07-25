////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     TextureClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Revision 1.5.0 - change the definition of sourcebuffer
// Additional Comments:
//    
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include "TextureClass.h"
#include "publicInclude.h"
#include "drawImmediately_cd.h"
#include "myMathClass.h"
#include "string.h"
#if (defined VERSION_2) && (defined EMBEDDED) 
#include "csi_ahmi.h"
#include "csi_ahmi_fix.h"
#endif
#ifdef AHMI_CORE

#ifdef PC_SIM
#define TEXTURE_ADDR_OFFSET 		0
extern u8       TextureBuffer[TEXBUFSIZE];          //global buffer to store texture data
extern u8       sourceBuffer[SoureBufferSize];
#endif

#if defined(CHIP_GC9005)
#define ALIGNMENT 64
#define AHHI_ENTER_CRITICAL rt_enter_critical();
#define AHHI_EXIT_CRITICAL  rt_exit_critical();
#else
#define AHHI_ENTER_CRITICAL
#define AHHI_EXIT_CRITICAL
#endif

extern u32*		gBackgroundTextureAddrPtr;// start pointer of Background Texture address arry
//extern NewSourceBuffer mNewSourceBuffer[200]; //the maxium of source buffer
//extern u8       sourceBuffer[SoureBufferSize];
extern DynamicPageClassPtr gPagePtr;
//extern u16             WorkingPageID;
extern s16 screenratio; //Solve the problem of inconsistency between length and width,(wight / high) * 512.
extern float _20_To_float(s32 A);
extern float _13_To_float(s32 A);
extern float _4_To_float(s32 A);
extern s32 float_To_20(float A);
extern s32 float_To_13(float A);
extern s32 float_To_4(float A);

uint32_t gNewSoureBufferSize = 0;
uint32_t NumberOFTexture = 0;
uint32_t NumberOFLongTexture = 0;
uint32_t NumberOFShortTexture = 0;

TextureClass::TextureClass(void)
{
}

TextureClass::~TextureClass(void)
{
}

unsigned int log2(unsigned int num)
{
	unsigned int exp = 0;
    while(num > 1)
	{
	    num = num >> 1;
        exp = exp + 1;		
	}
	return exp;
}

unsigned int img_align(unsigned int origin_width,unsigned int origin_height,int tex_type)
{
    if(tex_type == STCRGB888 || tex_type == STCRGBA888 || tex_type == SFBC)
	{
		if((origin_width+3)/4*4 * (origin_height+3)/4*4 < 16 * 1024 - 64)
			return origin_width;
		else
		{
			unsigned int nline = (16 * 1024 )/((origin_width+3)/4*4*4  + 64);
			unsigned int nlinelog2 = log2(nline);
			unsigned int width_align = 16*1024/(1<<nlinelog2)/4;
			return width_align;
		}
	}
    if(tex_type == RGBA8888 || tex_type == RGBA8888_RE)
	{
		if(origin_width * origin_height *4 < 16 * 1024 - 64)
			return origin_width;
		else
		{
		    unsigned int nline = (16 * 1024 )/(origin_width * 4 + 64);
			unsigned int nlinelog2 = log2(nline);
			unsigned int width_align = 16*1024/(1<<nlinelog2)/4;
			return width_align;
		}
	}
	if(tex_type == RGB565 || tex_type == RGB565_RE)
	{
		if(origin_width * origin_height *2 < 16 * 1024 - 64)
			return origin_width;
		else
		{
			unsigned int nline = (16 * 1024 )/(origin_width * 2 + 64);
			unsigned int nlinelog2 = log2(nline);
			unsigned int width_align = 16*1024/(1<<nlinelog2)/2;
			return width_align;
		}
	}
	if(tex_type == Y8 || tex_type == ALPHA8)
	{
		if(origin_width * origin_height < 16 * 1024 - 64)
			return origin_width;
		else
		{
			unsigned int nline = (16 * 1024 )/(origin_width  + 64);
			unsigned int nlinelog2 = log2(nline);
			unsigned int width_align = 16*1024/(1<<nlinelog2);
			return width_align;
		}
	}
	if(tex_type == ALPHA4)
	{
		if(origin_width * origin_height/2 < 16 * 1024 - 64)
			return origin_width;
		else
		{
			unsigned int nline = (16 * 1024 )/(origin_width/2 + 64);
			unsigned int nlinelog2 = log2(nline);
			unsigned int width_align = 16*1024/(1<<nlinelog2)*2;
			return width_align;
		}
	}
	if(tex_type == ALPHA2)
	{
		if(origin_width * origin_height/4 < 16 * 1024 - 64)
			return origin_width;
		else
		{
			unsigned int nline = (16 * 1024 )/(origin_width / 4 + 64);
			unsigned int nlinelog2 = log2(nline);
			unsigned int width_align = 16*1024/(1<<nlinelog2)*4;
			return width_align;
		}
	}
	if(tex_type == ALPHA1)
	{
		if(origin_width * origin_height/8 < 16 * 1024 - 64)
			return origin_width;
		else
		{
			unsigned int nline = (16 * 1024 )/(origin_width / 8 + 64);
			unsigned int nlinelog2 = log2(nline);
			unsigned int width_align = 16*1024/(1<<nlinelog2)*8;
			return width_align;
		}
	}
	return origin_width;
}

#if (defined VERSION_2) && (defined EMBEDDED)   
extern uint32_t SBAddress;
#endif
//-----------------------------
// function name��  MoveNewSourceBufferToSourcebufferArray
// Move the data from the sourcebuffer structure to the SourceBuffer array.
// param list��
//	@param1 u32 *psourceshift��					  //sourcebuffer offset
//	@param1 NewSourceBufferPtr psourcebufferaddr, //NewSourceBuffer structure pointer
// Remarks(Revision between versions):
//    added by yn 20181019
//-----------------------------
u32 TextureClass::MoveNewSourceBufferToSourcebufferArray(u32 *psourceshift, NewSourceBufferPtr psourcebufferaddr)
{
    uint8_t* srcAddr = 0;
    uint8_t* dstAddr = 0;
    uint32_t i = 0;
	#if (defined PC_SIM)   
	u32 enter_shift = 0;
	u8 *sourcebufferaddr;
	sourcebufferaddr = (u8 *)SoureBufferAddr;
	enter_shift = (*psourceshift);				//Record current offset
	int TexAttr = psourcebufferaddr->tex_type;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)  
//	u32 enter_shift;
	u8 *sourcebufferaddr = 0;				
	int TexAttr;
	#endif 

	if( NULL == psourceshift )
	{
		ERROR_PRINT("ERROR: data is NULL pointer");
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_NULLPointerErr);
	}
    
	#if (defined VERSION_2) && (defined EMBEDDED)
	#ifdef CHIP_GC9002_NoHyper
    sourcebufferaddr = (u8 *)StartAddrOfSourcebuffer;
	AHMI_WaitDrawDone();
	#else
	sourcebufferaddr = (u8 *)SBAddress;
	#endif
//    enter_shift = (*psourceshift);//Record current offset
    TexAttr = psourcebufferaddr->tex_type;
	#endif

	#ifdef VERSION_2
	if((TexAttr & ABCDEFGHMATRIX) != 0)
	{
		*(sourcebufferaddr + (*psourceshift)++) = (u8)(TexAttr & 0xff);        //texfisrtheader
		*(sourcebufferaddr + (*psourceshift)++) = (u8)((TexAttr >> 8) & 0xff); //texsecondheader
		*(sourcebufferaddr + (*psourceshift)++) = (u8)((TexAttr >> 16) & 0xff);//texthirdheader
		*(sourcebufferaddr + (*psourceshift)++) = (u8)((TexAttr >> 24) & 0xff);//texfourthheader

//		memcpy(sourcebufferaddr + (*psourceshift), &(psourcebufferaddr->Matrix.G), 20);
        srcAddr = (uint8_t*)(&(psourcebufferaddr->mMatrix.G));
        dstAddr = sourcebufferaddr + (*psourceshift);
        for(i = 0; i < 20; i++)
        {
            *(dstAddr++) = *(srcAddr++);
        }
		*psourceshift += 20;

		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.G & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.G >> 8 & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.G >> 16) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.G >> 24) & 0xff); 
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.H & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.H >> 8 & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.H >> 16) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.H >> 24) & 0xff); 

		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexSize.height & 0xff);//texture width
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexSize.height >> 8 & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexSize.width & 0xff);//texture height
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexSize.width >> 8 & 0xff);

		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Addr & 0xff);//texture address
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Addr >> 8) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Addr >> 16) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Addr >> 24) & 0xff);

		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->BoxSize.startx);//startx
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->BoxSize.starty);//starty
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->BoxSize.stopx); //stopx
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->BoxSize.stopy); //stopy

		gNewSoureBufferSize = 24;
		NumberOFShortTexture++;
	}
	else
	{
//		memcpy(sourcebufferaddr + (*psourceshift), &(psourcebufferaddr->tex_type), 64);
        srcAddr = (uint8_t*)(&(psourcebufferaddr->tex_type));
        dstAddr = sourcebufferaddr + (*psourceshift);
        for(i = 0; i < 64; i++)
        {
            *(dstAddr++) = *(srcAddr++);
        }
		*psourceshift += 64;

		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(TexAttr & 0xff);       //texfisrtheader
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((TexAttr >> 8) & 0xff);//texsecondheader
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((TexAttr >> 16) & 0xff);//texthirdheader
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((TexAttr >> 24) & 0xff);//texfourthheader

		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexVertex.v0.cor.y & 0xff);//four coordinate points
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->TexVertex.v0.cor.y >> 8) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexVertex.v0.cor.x & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->TexVertex.v0.cor.x >> 8) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexVertex.v1.cor.y & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->TexVertex.v1.cor.y >> 8) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexVertex.v1.cor.x & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->TexVertex.v1.cor.x >> 8) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexVertex.v2.cor.y & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->TexVertex.v2.cor.y >> 8) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexVertex.v2.cor.x & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->TexVertex.v2.cor.x >> 8) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexVertex.v3.cor.y & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->TexVertex.v3.cor.y >> 8) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexVertex.v3.cor.x & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->TexVertex.v3.cor.x >> 8) & 0xff);

		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.A & 0xff);//matrix
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.A >> 8) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.A >> 16) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.A >> 24) & 0xff); 
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.B & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.B >> 8 & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.B >> 16) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.B >> 24) & 0xff); 
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.C & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.C >> 8 & 0xff); 
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.C >> 16) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.C >> 24) & 0xff); 
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.D & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.D >> 8 & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.D >> 16) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.D >> 24) & 0xff); 
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.E & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.E >> 8 & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.E >> 16) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.E >> 24) & 0xff); 
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.F & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.F >> 8 & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.F >> 16) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.F >> 24) & 0xff); 
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.G & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.G >> 8 & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.G >> 16) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.G >> 24) & 0xff); 
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.H & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Matrix.H >> 8 & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.H >> 16) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Matrix.H >> 24) & 0xff); 

		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexSize.height & 0xff);//texture width
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexSize.height >> 8 & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexSize.width & 0xff);//texture height
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->TexSize.width >> 8 & 0xff);

		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->Addr & 0xff);//texture address
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Addr >> 8) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Addr >> 16) & 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->Addr >> 24) & 0xff);

		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->BoxSize.startx);//startx
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->BoxSize.starty);//starty
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->BoxSize.stopx); //stopx
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->BoxSize.stopy); //stopy

		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->fish_eye_param.v_in_center && 0xff);//fish_eye
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.v_in_center >> 8) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->fish_eye_param.u_in_center && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.u_in_center >> 8) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->fish_eye_param.fv_recip && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.fv_recip >> 8) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->fish_eye_param.fu_recip && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.fu_recip >> 8) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->fish_eye_param.k1 && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.k1 >> 8) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->fish_eye_param.k2 && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.k2 >> 8) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->fish_eye_param.k3 && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.k3 >> 8) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->fish_eye_param.Reserved && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.Reserved >> 8) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->fish_eye_param.v_out_center && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.v_out_center >> 8) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.v_out_center >> 16) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.v_out_center >> 24) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)(psourcebufferaddr->fish_eye_param.u_out_center && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.u_out_center >> 8) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.u_out_center >> 16) && 0xff);
		//*(sourcebufferaddr + (*psourceshift)++) = (u8)((psourcebufferaddr->fish_eye_param.u_out_center >> 24) && 0xff);

		gNewSoureBufferSize = 64;
		NumberOFLongTexture++;
	}
	#endif

	NumberOFTexture++;
	return AHMI_FUNC_SUCCESS;
}



u32 readU32FromTextureBuffer(u32 addr)
{
    u32 temp = 0;
	#ifdef PC_SIM
	u8* targetAddr = TextureBuffer + addr;
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)
	u8* targetAddr = (u8 *)(TEXTURE_START_ADDR_FOR_GPU + addr);
	#endif

#if defined(CHIP_GC9005)
    if(((uintptr_t)targetAddr & (ALIGNMENT - 1)) != 0)    // 64 字节对齐直接返回数据
    {
        // 对齐地址到64字节,读取一次64字节对齐地址
        uintptr_t alignedAddr = (uintptr_t)targetAddr & ~(ALIGNMENT - 1);

        AHHI_ENTER_CRITICAL
        uint8_t region = ((uint8_t*)alignedAddr)[0];
        AHHI_EXIT_CRITICAL
    }
#endif

	u16* arr = (u16*)targetAddr;

	AHHI_ENTER_CRITICAL
    temp = ((arr[0]<<16) + arr[1]);
	AHHI_EXIT_CRITICAL

    return temp;
}

u8 readBitFromTextureBuffer(u32 addr, u16 bitPos)
{
    u8 temp = 0;
	#ifdef PC_SIM
	u8* targetAddr = TextureBuffer + addr;
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)
	u8* targetAddr = (u8 *)(TEXTURE_START_ADDR_FOR_GPU + addr);
	#endif

#if defined(CHIP_GC9005)
    if(((uintptr_t)targetAddr & (ALIGNMENT - 1)) != 0)    // 64 字节对齐直接返回数据
    {
        // 对齐地址到64字节,读取一次64字节对齐地址
        uintptr_t alignedAddr = (uintptr_t)targetAddr & ~(ALIGNMENT - 1);

        AHHI_ENTER_CRITICAL
        uint8_t region = ((uint8_t*)alignedAddr)[0];
        AHHI_EXIT_CRITICAL
    }
#endif


	u16* arr = (u16*)targetAddr;
	u16 bitU16Pos = bitPos / 16;
	u8 bitOffset = bitPos % 16;

	AHHI_ENTER_CRITICAL
    temp = (arr[bitU16Pos] >> (15-bitOffset)) & 1;
	AHHI_EXIT_CRITICAL

    return temp;

}

s32 getSliceRealOffsetWithFlags(u32 flagsAddr, u16 bitPos, u8 imgOrColor)
{
	#ifdef PC_SIM
	u8* targetAddr = TextureBuffer + flagsAddr;
	#endif
	#if (defined VERSION_2) && (defined EMBEDDED)
	u8* targetAddr = (u8 *)(TEXTURE_START_ADDR_FOR_GPU + flagsAddr);
	#endif

#if defined(CHIP_GC9005)
    if(((uintptr_t)targetAddr & (ALIGNMENT - 1)) != 0)    // 64 字节对齐直接返回数据
    {
        // 对齐地址到64字节,读取一次64字节对齐地址
        uintptr_t alignedAddr = (uintptr_t)targetAddr & ~(ALIGNMENT - 1);

        AHHI_ENTER_CRITICAL
        uint8_t region = ((uint8_t*)alignedAddr)[0];
        AHHI_EXIT_CRITICAL
    }
#endif

	u16* arr = (u16*)targetAddr;
	u16 bitU16Pos = bitPos / 16;
	u8 bitOffset = bitPos % 16;
	s32 count = 0;
	u16 val = 0;
	for (int i = 0; i < bitU16Pos; i++)
	{
	    AHHI_ENTER_CRITICAL
		val = arr[i];
		AHHI_EXIT_CRITICAL

		for (int j = 0; j < 16; j++)
		{
			if (imgOrColor)
			{
				if (val & 1)
				{
					count++;
				}
			}
			else
			{
				if ((val & 1) == 0)
				{
					count++;
				}
			}
			val = val >> 1;
		}
	}

	AHHI_ENTER_CRITICAL
	val = arr[bitU16Pos];
	AHHI_EXIT_CRITICAL

	for (int j = 0; j <= bitOffset; j++)
	{
		if (imgOrColor)
		{
			if (((val >> (15-j)) & 1) == 1)
			{
				count++;
			}
		}
		else
		{
			if (((val >> (15-j)) & 1) == 0)
			{
				count++;
			}
		}
	}
	
	return count-1;
}

//-----------------------------
// function name��  WriteSourceBuffer
// write data to sourcebuffer structure, and move the data from the sourcebuffer structure to the SourceBuffer array.
// param list��
//	@param1 u32 *psourceshift��			//sourcebuffer offset
//	@param1 NewMatrix* Mw��				//All matrix operations except texture matrix.
//	@param1 TextureClass* textureInfo��	//textureclass  pointer
//	@param1 WidgetClassPtr p_wptr��		//widgetclass pointer
// Remarks(Revision between versions):
//    added by yn 20181017
//-----------------------------
extern void Point_Sort(PointClass* pPoint, uint32_t PointNum);
funcStatus TextureClass::WriteNewSourceBuffer(u32 *psourceshift, NewMatrix* Mw, TextureClass* textureInfo, WidgetClassPtr p_wptr)
{
	funcStatus status;
	NewSourceBuffer mNewSourceBuffer[1];
	NewSourceBufferPtr sourcebufferaddr;
	//PointClass p0(0,0),p1(textureInfo->TexWidth << 4,0),p2(textureInfo->TexWidth << 4, textureInfo->TexHeight << 4),p3(0,textureInfo->TexHeight << 4);
	//Vertex Extend 1 pixel for project transform edge display sawtooth problem
	PointClass p0(((-1) << 4),((-1) << 4)),p1((textureInfo->TexWidth << 4) + 16,((-1) << 4)),p2((textureInfo->TexWidth << 4) + 16, (textureInfo->TexHeight << 4) + 16),p3(((-1) << 4),(textureInfo->TexHeight << 4) + 16);
	PointClass Point_All[12];
	NewMatrix textureMatrix;
//	s32 temp;
	u32 buffersourceshift = 0, i = 0;
	u32 addr = 0;
	u32 attr = 0;
	u32 pointNumAfterTailoring = 0;
	u8 lastTexture = 0;
	u8 forceRedering = 0;
	u8 textureType = 0;
	u8 useAlphabuffer = 0;
	u8 masktype = 0;
	u8 foregroundflag = 0;
	u8 backgroundflag = 0;
	u8 backgroundfilterflag = 0;
	u8 forcelongsourcebufferflag = 0;
	
	u8 isImageSlice = 1;

	sourcebufferaddr = mNewSourceBuffer;
	
	#ifdef VERSION_2

	u32 flagsStartAddr;
	u32 colorsAddr;
	s32 realOffset;

	if (textureInfo->SliceFlagsHeaderAddr != 0)
	{
		//has color header
		//read color header
		flagsStartAddr = readU32FromTextureBuffer(textureInfo->SliceFlagsHeaderAddr);
		colorsAddr = readU32FromTextureBuffer(textureInfo->SliceFlagsHeaderAddr+sizeof(u32));
		isImageSlice = readBitFromTextureBuffer(flagsStartAddr, textureInfo->FocusedSlice);
	}

	#if defined(CHIP_GC9002)
	foregroundflag = (u8)((textureInfo->mTexAttr & FOREGROUND_NOT) >> 16);
	backgroundflag = (u8)((textureInfo->mTexAttr & BACKGROUND_NOT) >> 17);
	backgroundfilterflag = (u8)((textureInfo->mTexAttr & BACKGROUND_FILTER) >> 18);
	#endif

	masktype = (u8)((textureInfo->mTexAttr & 0x600) >> 9);
	useAlphabuffer = (u8)((textureInfo->mTexAttr & 0x8) >> 3);
	textureType = (u8)((textureInfo->mTexAttr & 0xf0) >> 4);
	
	#ifdef CHIP_GC9002_NoHyper
	if((textureInfo->mTexAttr & 0xf0) == ALPHA1)
		return AHMI_FUNC_SUCCESS;
	#endif //endof #ifndef CHIP_GC9002_NoHyper
	
	if (!isImageSlice)
	{
		//color
		textureType = (u8)((((PURECOLOR | NONMASKTEX | EFMATRIX | ADDRTEXTURE) & 0xf0) >> 4));
	}
	if(*psourceshift == 0)
	{
		forceRedering = 1;
	}
	lastTexture = (u8)(textureInfo->mTexAttr & 0x01);

	attr = (masktype | (useAlphabuffer << 3) | (textureType << 4) | (forceRedering << 8) | (foregroundflag << 11) | (backgroundflag << 12) | (lastTexture << 15) | (backgroundfilterflag << 24));
    #ifdef CHIP_GC9005
	attr |= (1 << 18);//CHIP_GC9005 sourcebuffer tilesize 16x16 {18bit: 0 means 32x32; 1 means 16x16}
	#endif

	sourcebufferaddr[buffersourceshift].tex_type = attr;//texture attribute
	#endif

	status = textureInfo->computeTextureMatrixAndFinalMatrix(&textureMatrix, p_wptr, Mw);//Calculation of transformation matrix based on parameters
	if(status != AHMI_FUNC_SUCCESS)
	{
		return status;
	}

	if(textureInfo->useCanvasPoint)
	{
		forcelongsourcebufferflag = 1;
		p0.mPointX = p_wptr->mROI->lt.mPointX;
		p0.mPointY = p_wptr->mROI->lt.mPointY;
		p1.mPointX = p_wptr->mROI->rt.mPointX;
		p1.mPointY = p_wptr->mROI->rt.mPointY;
		p2.mPointX = p_wptr->mROI->rb.mPointX;
		p2.mPointY = p_wptr->mROI->rb.mPointY;
		p3.mPointX = p_wptr->mROI->lb.mPointX;
		p3.mPointY = p_wptr->mROI->lb.mPointY;
		textureInfo->computeTextureBoxAndPointByROI(&p0,&p1,&p2,&p3);
	}
	else
	{
		if(textureInfo->computeTextureBoxAndPoint(&textureMatrix, &p0,&p1,&p2,&p3) != AHMI_FUNC_SUCCESS)//Judging the range of tile boxes according to coordinate points
		{
			forcelongsourcebufferflag = 1;
			p0.mPointX = p_wptr->mROI->lt.mPointX;
			p0.mPointY = p_wptr->mROI->lt.mPointY;
			p1.mPointX = p_wptr->mROI->rt.mPointX;
			p1.mPointY = p_wptr->mROI->rt.mPointY;
			p2.mPointX = p_wptr->mROI->rb.mPointX;
			p2.mPointY = p_wptr->mROI->rb.mPointY;
			p3.mPointX = p_wptr->mROI->lb.mPointX;
			p3.mPointY = p_wptr->mROI->lb.mPointY;
			textureInfo->computeTextureBoxAndPointByROI(&p0,&p1,&p2,&p3);
		}
		else
		{
			Point_All[0] = p0;
			Point_All[1] = p1;
			Point_All[2] = p2;
			Point_All[3] = p3;
			Point_Sort(Point_All, 4);
			p0 = Point_All[0];
			p1 = Point_All[1];
			p2 = Point_All[2];
			p3 = Point_All[3];
		}
	}

	if((p_wptr != NULL) && (textureType != (ALPHA1 >> 4)))
	{
		//textureInfo->tailoringTexturePoint(&p0,&p1,&p2,&p3,&p_wptr->mROI->lt,&p_wptr->mROI->rt,&p_wptr->mROI->rb,&p_wptr->mROI->lb,Point_All,&pointNumAfterTailoring);
		if((p0.mPointX >= p_wptr->mROI->lt.mPointX) && (p0.mPointX <= p_wptr->mROI->rb.mPointX) && (p0.mPointY >= p_wptr->mROI->lt.mPointY) && (p0.mPointY <= p_wptr->mROI->rb.mPointY) &&
		   (p1.mPointX >= p_wptr->mROI->lt.mPointX) && (p1.mPointX <= p_wptr->mROI->rb.mPointX) && (p1.mPointY >= p_wptr->mROI->lt.mPointY) && (p1.mPointY <= p_wptr->mROI->rb.mPointY) &&
		   (p2.mPointX >= p_wptr->mROI->lt.mPointX) && (p2.mPointX <= p_wptr->mROI->rb.mPointX) && (p2.mPointY >= p_wptr->mROI->lt.mPointY) && (p2.mPointY <= p_wptr->mROI->rb.mPointY) && 
		   (p3.mPointX >= p_wptr->mROI->lt.mPointX) && (p3.mPointX <= p_wptr->mROI->rb.mPointX) && (p3.mPointY >= p_wptr->mROI->lt.mPointY) && (p3.mPointY <= p_wptr->mROI->rb.mPointY))
		{
			sourcebufferaddr[buffersourceshift].TexVertex.v0.cor.x = p0.mPointX;
			sourcebufferaddr[buffersourceshift].TexVertex.v0.cor.y = p0.mPointY;
			sourcebufferaddr[buffersourceshift].TexVertex.v1.cor.x = p1.mPointX;
			sourcebufferaddr[buffersourceshift].TexVertex.v1.cor.y = p1.mPointY;
			sourcebufferaddr[buffersourceshift].TexVertex.v2.cor.x = p2.mPointX;
			sourcebufferaddr[buffersourceshift].TexVertex.v2.cor.y = p2.mPointY;
			sourcebufferaddr[buffersourceshift].TexVertex.v3.cor.x = p3.mPointX;
			sourcebufferaddr[buffersourceshift].TexVertex.v3.cor.y = p3.mPointY;
		}
		else
		{
			forcelongsourcebufferflag = 1;
			sourcebufferaddr[buffersourceshift].TexVertex.v0.cor.x = p_wptr->mROI->lt.mPointX;
			sourcebufferaddr[buffersourceshift].TexVertex.v0.cor.y = p_wptr->mROI->lt.mPointY;
			sourcebufferaddr[buffersourceshift].TexVertex.v1.cor.x = p_wptr->mROI->rt.mPointX;
			sourcebufferaddr[buffersourceshift].TexVertex.v1.cor.y = p_wptr->mROI->rt.mPointY;
			sourcebufferaddr[buffersourceshift].TexVertex.v2.cor.x = p_wptr->mROI->rb.mPointX;
			sourcebufferaddr[buffersourceshift].TexVertex.v2.cor.y = p_wptr->mROI->rb.mPointY;
			sourcebufferaddr[buffersourceshift].TexVertex.v3.cor.x = p_wptr->mROI->lb.mPointX;
			sourcebufferaddr[buffersourceshift].TexVertex.v3.cor.y = p_wptr->mROI->lb.mPointY;
		}
	}
	else
	{
		sourcebufferaddr[buffersourceshift].TexVertex.v0.cor.x = p0.mPointX;
		sourcebufferaddr[buffersourceshift].TexVertex.v0.cor.y = p0.mPointY;
		sourcebufferaddr[buffersourceshift].TexVertex.v1.cor.x = p1.mPointX;
		sourcebufferaddr[buffersourceshift].TexVertex.v1.cor.y = p1.mPointY;
		sourcebufferaddr[buffersourceshift].TexVertex.v2.cor.x = p2.mPointX;
		sourcebufferaddr[buffersourceshift].TexVertex.v2.cor.y = p2.mPointY;
		sourcebufferaddr[buffersourceshift].TexVertex.v3.cor.x = p3.mPointX;
		sourcebufferaddr[buffersourceshift].TexVertex.v3.cor.y = p3.mPointY;
	}

	#ifdef VERSION_2
	if(textureInfo->ShearAngleX >= 0x520 || textureInfo->ShearAngleY >= 0x520)
	{
		p0.mPointX = 0;
		p0.mPointY = 0;
		p1.mPointX = (p_wptr->WidgetWidth << 4);
		p1.mPointY = 0;
		p2.mPointX = (p_wptr->WidgetWidth << 4);
		p2.mPointY = (p_wptr->WidgetHeight << 4);
		p3.mPointX = 0;
		p3.mPointY = (p_wptr->WidgetHeight << 4);
		textureInfo->computeTextureBoxAndPoint(Mw, &p0,&p1,&p2,&p3);//Judging the range of tile boxes according to coordinate points
	}
	#endif

	sourcebufferaddr[buffersourceshift].BoxSize.startx = textureInfo->TexLeftTileBox;//texture tile box
	sourcebufferaddr[buffersourceshift].BoxSize.starty = textureInfo->TexTopTileBox;
	sourcebufferaddr[buffersourceshift].BoxSize.stopx = textureInfo->TexRightTileBox;
	sourcebufferaddr[buffersourceshift].BoxSize.stopy = textureInfo->TexButtomTileBox;

	if(textureInfo->staticTexture == 0)
	{
		sourcebufferaddr[buffersourceshift].TexSize.width = textureInfo->TexWidth;//texture width
	}
	else
	{
		forcelongsourcebufferflag = 1;
		sourcebufferaddr[buffersourceshift].TexSize.width = img_align(textureInfo->TexWidth,textureInfo->TexHeight,(textureType << 4));//texture width
	}
	sourcebufferaddr[buffersourceshift].TexSize.height = textureInfo->TexHeight;//texture height

	#ifdef VERSION_2
	sourcebufferaddr[buffersourceshift].mMatrix.A = textureMatrix.Inverse_A;//texture matrix
	sourcebufferaddr[buffersourceshift].mMatrix.B = textureMatrix.Inverse_B;
	sourcebufferaddr[buffersourceshift].mMatrix.C = textureMatrix.Inverse_C;
	sourcebufferaddr[buffersourceshift].mMatrix.D = textureMatrix.Inverse_D;
	sourcebufferaddr[buffersourceshift].mMatrix.E = textureMatrix.Inverse_E;
	sourcebufferaddr[buffersourceshift].mMatrix.F = textureMatrix.Inverse_F;
	sourcebufferaddr[buffersourceshift].mMatrix.G = textureMatrix.Inverse_G;
	sourcebufferaddr[buffersourceshift].mMatrix.H = textureMatrix.Inverse_H;

	if(forcelongsourcebufferflag != 1)
	{
		if(textureMatrix.Inverse_A == 0x100000 && textureMatrix.Inverse_E == 0x100000 && textureMatrix.Inverse_B == 0 && textureMatrix.Inverse_D == 0)
		{
			sourcebufferaddr[buffersourceshift].tex_type |= ABCDEFGHMATRIX;
		}
	}
	#endif

	//compute addr
	if (SliceFlagsHeaderAddr)
	{
		if (isImageSlice)
		{
			realOffset = getSliceRealOffsetWithFlags(flagsStartAddr, textureInfo->FocusedSlice, 1);
			if (realOffset < 0)
			{
				//error
				//temp set to 0
				realOffset = 0;
			}
			if(textureInfo->staticTexture == 0)
			{
				addr = (u32)( realOffset * textureInfo->SingleSliceSize);
			}
			else
			{
				addr = (u32)(0x4000 * (realOffset / textureInfo->staticTexture) + (realOffset % textureInfo->staticTexture) * textureInfo->SingleSliceSize);
			}
			
			sourcebufferaddr[buffersourceshift].Addr = (textureInfo->TexAddr + addr) + TEXTURE_ADDR_OFFSET;//texture address
		}
		else
		{
			realOffset = getSliceRealOffsetWithFlags(flagsStartAddr, textureInfo->FocusedSlice, 0);
			if (realOffset < 0)
			{
				//error
				//temp set to 0
				realOffset = 0;
			}
			addr = readU32FromTextureBuffer(colorsAddr +  realOffset*sizeof(u32)); //color
			sourcebufferaddr[buffersourceshift].Addr = addr;//texture address
		}
	}
	else
	{
		if(textureInfo->staticTexture == 0)
		{
			addr = (u32)(textureInfo->FocusedSlice * textureInfo->SingleSliceSize);
		}
		else
		{
			addr = (u32)(0x4000 * (textureInfo->FocusedSlice / textureInfo->staticTexture) + (textureInfo->FocusedSlice % textureInfo->staticTexture) * textureInfo->SingleSliceSize);
		}
		if((textureInfo->TexAddr & 0xF0000000) == 0)
		{
			sourcebufferaddr[buffersourceshift].Addr = (textureInfo->TexAddr + addr) + TEXTURE_ADDR_OFFSET;//texture address
		}
		else
		{
			sourcebufferaddr[buffersourceshift].Addr = (textureInfo->TexAddr + addr);//texture address
		}
	}
	
	

	if((textureInfo->mTexAttr & DRAWING) == 0)
	{
		return AHMI_FUNC_SUCCESS; //TexAttr[15]is 0,The texture does not need to be drawn.
	}

	if(pointNumAfterTailoring != 0)
	{
		//clip texture with roi, get new p0, p1, p2, p3
		for(i = 0; i < (pointNumAfterTailoring/4); i++)
		{
			sourcebufferaddr[buffersourceshift].TexVertex.v0.cor.x = Point_All[0 + 4 * i].mPointX;
			sourcebufferaddr[buffersourceshift].TexVertex.v0.cor.y = Point_All[0 + 4 * i].mPointY;
			sourcebufferaddr[buffersourceshift].TexVertex.v1.cor.x = Point_All[1 + 4 * i].mPointX;
			sourcebufferaddr[buffersourceshift].TexVertex.v1.cor.y = Point_All[1 + 4 * i].mPointY;
			sourcebufferaddr[buffersourceshift].TexVertex.v2.cor.x = Point_All[2 + 4 * i].mPointX;
			sourcebufferaddr[buffersourceshift].TexVertex.v2.cor.y = Point_All[2 + 4 * i].mPointY;
			sourcebufferaddr[buffersourceshift].TexVertex.v3.cor.x = Point_All[3 + 4 * i].mPointX;
			sourcebufferaddr[buffersourceshift].TexVertex.v3.cor.y = Point_All[3 + 4 * i].mPointY;

			this->MoveNewSourceBufferToSourcebufferArray(psourceshift,sourcebufferaddr);//move the data from the sourcebuffer structure to the SourceBuffer array
		}
	}
	else
	{
		this->MoveNewSourceBufferToSourcebufferArray(psourceshift,sourcebufferaddr);//move the data from the sourcebuffer structure to the SourceBuffer array
	}

	//(*psourceshift) = (*psourceshift) + 1;
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name��  computeTextureMatrixAndFinalMatrix
// compute texture matrix and final matrix
// param list��
//	@param1 NewMatrix* M2��			//texture matrix
//	@param1 WidgetClassPtr p_wptr��	//widgetclass pointer
//	@param1 NewMatrix* Mw��			//All matrix operations except texture matrix.
// Remarks(Revision between versions):
//  compute the texture's matrix
//	compute the final matrix M_p^(-1)*M_C^(-1)*M_SC^(-1)*M_W^(-1) M_T^(-1)
//    added by yn 20181017
//-----------------------------
funcStatus TextureClass::computeTextureMatrixAndFinalMatrix(NewMatrix* M2, WidgetClassPtr p_wptr, NewMatrix* Mw)
{
	funcStatus status;
	s32 tempX = 0, tempY = 0;
	s16 tempX_s16 = 0, tempY_s16 = 0;

	M2->NewMatrix_init();
	M2->NewMatrixInv_init();
	M2->scale(int(this->ScalerX), int(this->ScalerY));
	if(p_wptr != NULL)
	{
		if((p_wptr->ADAPTIVE_SCALE_LEVEL != (1<<9)) && (p_wptr->ADAPTIVE_SCALE_LEVEL != 0) && (((this->mTexAttr & 0xf0) == STCRGB888) || ((this->mTexAttr & 0xf0) == STCRGBA888) || ((this->mTexAttr & 0xf0) == RGB565) || ((this->mTexAttr & 0xf0) == RGBA8888)))
		{
			M2->scale(p_wptr->ADAPTIVE_SCALE_LEVEL, p_wptr->ADAPTIVE_SCALE_LEVEL);
		}
	}
	M2->shear(int(this->ShearAngleX), int(this->ShearAngleY));

	tempX = this->OffsetX;
	tempY = this->OffsetY;

	if(p_wptr == NULL)
	{
		M2->translate(tempX << 9, tempY << 9);
	}
	else
	{
		//tempX_s16 = p_wptr->WidgetOffsetX;
		//tempY_s16 = p_wptr->WidgetOffsetY;
		//tempX_s16 = tempX_s16 << 4;
		//tempY_s16 = tempY_s16 << 4;
		//M2->translate(int(this->OffsetX - tempX_s16) << 9, int(this->OffsetY - tempY_s16) << 9);//Calculate the relative offset of texture relative to widget.
		M2->translate(tempX << 9, tempY << 9);//Calculate the relative offset of texture relative to widget.
	}

	if((this->RotateAngle) != 0)
	{
		//Arbitrary point rotation(translate into left point -> rotate-> translate)
		if(p_wptr != NULL)
		{
			//TODO��Pre translation of center rotation  when "p_wptr->RotationPointX" is a negative number(e.g 0xffce),"int(p_wptr->RotationPointX)" is not 0xffffffce but 0x0000ffce!
			tempX_s16 = (s16)(p_wptr->RotationPointX);
			tempY_s16 = (s16)(p_wptr->RotationPointY);
			tempX = -(tempX_s16);
			tempY = -(tempY_s16);
			tempX = (tempX << 4) << 9;
			tempY = (tempY << 4) << 9;
			M2->translate(tempX, tempY);
		}

		M2->rotate(this->RotateAngle);

		//Repair matrix repair for calculating shrinkage (for screen inconsistencies)
		if(this->mTexAttr & TEXTURE_CENTRAL_ROTATE)
		{
			M2->scale(s16(1 << 9), screenratio);
		}

		if(p_wptr != NULL)
		{
			//TODO��Opposite translation of center rotation  when "p_wptr->RotationPointX" is a negative number(e.g 0xffce),"int(p_wptr->RotationPointX)" is not 0xffffffce but 0x0000ffce!
			tempX_s16 = (s16)(p_wptr->RotationPointX);
			tempY_s16 = (s16)(p_wptr->RotationPointY);
			tempX = (tempX_s16);
			tempY = (tempY_s16);
			tempX = (tempX << 4) << 9;
			tempY = (tempY << 4) << 9;
			M2->translate(tempX, tempY);
		}
	}

	#ifdef VERSION_2
	if(p_wptr != NULL)
	{
		if (this->ShearAngleZX !=0 || this->ShearAngleZY != 0)
		{
			M2->textureProject(this->ShearAngleZX, this->ShearAngleZY, this->TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512, this->TexHeight * p_wptr->ADAPTIVE_SCALE_LEVEL / 512, this->OffsetX, this->OffsetY);
		}
	}
	#endif

	//Calculating final transformation matrix
	M2->LeftMultiMatrix(Mw);

	#ifdef VERSION_2
	status = M2->Matrix_I_Normalization();
	#endif

	return status;
}


//-----------------------------
// function name��  computeTextureBoxAndPoint
// compute texture tile box and point
// param list��
//	@param1 NewMatrix* M2��			//texture matrix
//	@param1 PointClass* p0��		//left top point
//	@param1 PointClass* p1��		//right top point
//	@param1 PointClass* p2��		//right bottom point
//	@param1 PointClass* p3��		//left bottom point
// Remarks(Revision between versions):
//	compute the texture's box and point
//    added by yn 20181017
//-----------------------------
u32 TextureClass::computeTextureBoxAndPoint(NewMatrix* M2, PointClass* p0, PointClass* p1, PointClass* p2, PointClass* p3)
{
	
#ifdef UseFloatMatrix
	long long x0,y0,x1,y1,x2,y2,x3,y3,z0,z1,z2,z3;
	int Ox0,Oy0,Ox1,Oy1,Ox2,Oy2,Ox3,Oy3;
	long long x_min, x_max, y_min, y_max;
	z0 = float_To_4(_20_To_float(M2->C) * _4_To_float(p0->mPointX) + _20_To_float(M2->F) * _4_To_float(p0->mPointY)+ _13_To_float(M2->I) * 1);
	z1 = float_To_4(_20_To_float(M2->C) * _4_To_float(p1->mPointX) + _20_To_float(M2->F) * _4_To_float(p1->mPointY)+ _13_To_float(M2->I) * 1);
	z2 = float_To_4(_20_To_float(M2->C) * _4_To_float(p2->mPointX) + _20_To_float(M2->F) * _4_To_float(p2->mPointY)+ _13_To_float(M2->I) * 1);
	z3 = float_To_4(_20_To_float(M2->C) * _4_To_float(p3->mPointX) + _20_To_float(M2->F) * _4_To_float(p3->mPointY)+ _13_To_float(M2->I) * 1);
	x0 = float_To_4(_20_To_float(M2->A) * _4_To_float(p0->mPointX) + _20_To_float(M2->D) * _4_To_float(p0->mPointY)+ _13_To_float(M2->G) * 1);
	y0 = float_To_4(_20_To_float(M2->B) * _4_To_float(p0->mPointX) + _20_To_float(M2->E) * _4_To_float(p0->mPointY)+ _13_To_float(M2->H) * 1);
	x1 = float_To_4(_20_To_float(M2->A) * _4_To_float(p1->mPointX) + _20_To_float(M2->D) * _4_To_float(p1->mPointY)+ _13_To_float(M2->G) * 1);
	y1 = float_To_4(_20_To_float(M2->B) * _4_To_float(p1->mPointX) + _20_To_float(M2->E) * _4_To_float(p1->mPointY)+ _13_To_float(M2->H) * 1);
	x2 = float_To_4(_20_To_float(M2->A) * _4_To_float(p2->mPointX) + _20_To_float(M2->D) * _4_To_float(p2->mPointY)+ _13_To_float(M2->G) * 1);
	y2 = float_To_4(_20_To_float(M2->B) * _4_To_float(p2->mPointX) + _20_To_float(M2->E) * _4_To_float(p2->mPointY)+ _13_To_float(M2->H) * 1);
	x3 = float_To_4(_20_To_float(M2->A) * _4_To_float(p3->mPointX) + _20_To_float(M2->D) * _4_To_float(p3->mPointY)+ _13_To_float(M2->H) * 1);
	y3 = float_To_4(_20_To_float(M2->B) * _4_To_float(p3->mPointX) + _20_To_float(M2->E) * _4_To_float(p3->mPointY)+ _13_To_float(M2->H) * 1);
#else
	int x0,y0,x1,y1,x2,y2,x3,y3,z0,z1,z2,z3;
	int Ox0,Oy0,Ox1,Oy1,Ox2,Oy2,Ox3,Oy3;
	s32 x_min, x_max, y_min, y_max;
	#if (defined VERSION_2) && (defined PC_SIM) 
	z0 = (((long long)(M2->C) * (p0->mPointX)) + ((long long)(M2->F) * (p0->mPointY)) + ((long long)(M2->I) << 11)) >> 12;
	z1 = (((long long)(M2->C) * (p1->mPointX)) + ((long long)(M2->F) * (p1->mPointY)) + ((long long)(M2->I) << 11)) >> 12;
	z2 = (((long long)(M2->C) * (p2->mPointX)) + ((long long)(M2->F) * (p2->mPointY)) + ((long long)(M2->I) << 11)) >> 12;
	z3 = (((long long)(M2->C) * (p3->mPointX)) + ((long long)(M2->F) * (p3->mPointY)) + ((long long)(M2->I) << 11)) >> 12;
	#endif
	#if (defined PC_SIM) 
	x0 = (((long long)(M2->A) * (p0->mPointX)) + ((long long)(M2->D) * (p0->mPointY)) + ((long long)(M2->G) << 11)) >> 20;
	y0 = (((long long)(M2->B) * (p0->mPointX)) + ((long long)(M2->E) * (p0->mPointY)) + ((long long)(M2->H) << 11)) >> 20;
	x1 = (((long long)(M2->A) * (p1->mPointX)) + ((long long)(M2->D) * (p1->mPointY)) + ((long long)(M2->G) << 11)) >> 20;
	y1 = (((long long)(M2->B) * (p1->mPointX)) + ((long long)(M2->E) * (p1->mPointY)) + ((long long)(M2->H) << 11)) >> 20;
	x2 = (((long long)(M2->A) * (p2->mPointX)) + ((long long)(M2->D) * (p2->mPointY)) + ((long long)(M2->G) << 11)) >> 20;
	y2 = (((long long)(M2->B) * (p2->mPointX)) + ((long long)(M2->E) * (p2->mPointY)) + ((long long)(M2->H) << 11)) >> 20;
	x3 = (((long long)(M2->A) * (p3->mPointX)) + ((long long)(M2->D) * (p3->mPointY)) + ((long long)(M2->G) << 11)) >> 20;
	y3 = (((long long)(M2->B) * (p3->mPointX)) + ((long long)(M2->E) * (p3->mPointY)) + ((long long)(M2->H) << 11)) >> 20;
#endif
#if (defined VERSION_2) && (defined EMBEDDED)    
	z0 = (csi_fix_mult((M2->C), (p0->mPointX), 0) + csi_fix_mult((M2->F), (p0->mPointY), 0) + ((long long)(M2->I) << 11)) >> 12;
	z1 = (csi_fix_mult((M2->C), (p1->mPointX), 0) + csi_fix_mult((M2->F), (p1->mPointY), 0) + ((long long)(M2->I) << 11)) >> 12;
	z2 = (csi_fix_mult((M2->C), (p2->mPointX), 0) + csi_fix_mult((M2->F), (p2->mPointY), 0) + ((long long)(M2->I) << 11)) >> 12;
	z3 = (csi_fix_mult((M2->C), (p3->mPointX), 0) + csi_fix_mult((M2->F), (p3->mPointY), 0) + ((long long)(M2->I) << 11)) >> 12;
			
    x0 = (csi_fix_mult((M2->A), (p0->mPointX), 0) + csi_fix_mult((M2->D), (p0->mPointY), 0) + ((long long)(M2->G) << 11)) >> 20;
    y0 = (csi_fix_mult((M2->B), (p0->mPointX), 0) + csi_fix_mult((M2->E), (p0->mPointY), 0) + ((long long)(M2->H) << 11)) >> 20;
    x1 = (csi_fix_mult((M2->A), (p1->mPointX), 0) + csi_fix_mult((M2->D), (p1->mPointY), 0) + ((long long)(M2->G) << 11)) >> 20;
    y1 = (csi_fix_mult((M2->B), (p1->mPointX), 0) + csi_fix_mult((M2->E), (p1->mPointY), 0) + ((long long)(M2->H) << 11)) >> 20;
    x2 = (csi_fix_mult((M2->A), (p2->mPointX), 0) + csi_fix_mult((M2->D), (p2->mPointY), 0) + ((long long)(M2->G) << 11)) >> 20;
    y2 = (csi_fix_mult((M2->B), (p2->mPointX), 0) + csi_fix_mult((M2->E), (p2->mPointY), 0) + ((long long)(M2->H) << 11)) >> 20;
    x3 = (csi_fix_mult((M2->A), (p3->mPointX), 0) + csi_fix_mult((M2->D), (p3->mPointY), 0) + ((long long)(M2->G) << 11)) >> 20;
    y3 = (csi_fix_mult((M2->B), (p3->mPointX), 0) + csi_fix_mult((M2->E), (p3->mPointY), 0) + ((long long)(M2->H) << 11)) >> 20;
#endif
#endif

	#ifdef VERSION_2
	if(z0 != 1 << 12)
	{
		if(z0 == 0)
			z0 = 1;

		x0 = (x0 << 12)/ z0;
		y0 = (y0 << 12)/ z0;
	}
	if(z1 != 1 << 12)
	{
		if(z1 == 0)
			z1 = 1;

		x1 = (x1 << 12)/ z1;
		y1 = (y1 << 12)/ z1;
	}
	if(z2 != 1 << 12)
	{
		if(z2 == 0)
			z2 = 1;

		x2 = (x2 << 12)/ z2;
		y2 = (y2 << 12)/ z2;
	}
	if(z3 != 1 << 12)
	{
		if(z3 == 0)
			z3 = 1;

		x3 = (x3 << 12)/ z3;
		y3 = (y3 << 12)/ z3;
	}

	//When the translation is large, repair upper right coordinate points.
	//if( /*(y2 > y3) && */((z0 != 1 << 4)||(z1 != 1 << 4)||(z2 != 1 << 4)||(z3 != 1 << 4)) )
	//{
	//	y1 = y0 - (y2 - y3);
	//}
	#endif

	Ox0 = x0;
	Ox1 = x1;
	Ox2 = x2;
	Ox3 = x3;

	Oy0 = y0;
	Oy1 = y1;
	Oy2 = y2;
	Oy3 = y3;

	//Recompute the point when it's overlapped
	if(x0 <= -2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//x0 = -2048 * 16 + 1;
	}
	if(x0 >= 2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//x0 = 2048 * 16 - 1;
	}
	if(y0 <= -2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//y0 = -2048 * 16 + 1;
	}
	if(y0 >= 2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//y0 = 2048 * 16 - 1;
	}

	if(x1 <= -2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//x1 = -2048 * 16 + 1;
	}
	if(x1 >= 2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//x1 = 2048 * 16 - 1;
	}
	if(y1 <= -2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//y1 = -2048 * 16 + 1;
	}
	if(y1 >= 2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//y1 = 2048 * 16 - 1;
	}

	if(x2 <= -2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//x2 = -2048 * 16 + 1;
	}
	if(x2 >= 2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//x2 = 2048 * 16 - 1;
	}
	if(y2 <= -2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//y2 = -2048 * 16 + 1;
	}
	if(y2 >= 2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//y2 = 2048 * 16 - 1;
	}

	if(x3 <= -2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//x3 = -2048 * 16 + 1;
	}
	if(x3 >= 2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//x3 = 2048 * 16 - 1;
	}
	if(y3 <= -2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//y3 = -2048 * 16 + 1;
	}
	if(y3 >= 2048 * 16)
	{
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
		//y3 = 2048 * 16 - 1;
	}

	//for clockwise meter puremask shear 
	if((x2 == (-2048 * 16 + 1)) && (x3 == (-2048 * 16 + 1)))
	{
		y3 = (Ox3) * (Oy3 - Oy0) / (Ox0 - Ox3) + Oy3;
		x3 = 0;

		y2 = (Ox2) * (Oy2 - Oy1) / (Ox1 - Ox2) + Oy2;
		x2 = 0;
	}

	if((y2 == (-2048 * 16 + 1)) && (y3 == (-2048 * 16 + 1)))
	{
		x3 = (-Oy3) * (Ox0 - Ox3) / (Oy0 - Oy3) + Ox3;
		y3 = 0;

		x2 = (-Oy2) * (Ox1 - Ox2) / (Oy1 - Oy2) + Ox2;
		y2 = 0;
	}

	if((x2 == (2048 * 16 - 1)) && (x3 == (2048 * 16 - 1)))
	{
		y3 = (Ox0 - (gScreenWidth << 4)) * (Oy0 - Oy3) / (Ox3 - Ox0) + Oy0;
		x3 = gScreenWidth << 4;

		y2 = (Ox1 - (gScreenWidth << 4)) * (Oy1 - Oy2) / (Ox2 - Ox1) + Oy1;
		x2 = gScreenWidth << 4;
	}

	if((y2 == (2048 * 16 - 1)) && (y3 == (2048 * 16 - 1)))
	{
		x3 = (Oy3 - (gScreenHeight << 4)) * (Ox0 - Ox3) / (Oy3 - Oy0) + Ox3;
		y3 = gScreenHeight << 4;

		x2 = (Oy2 - (gScreenHeight << 4)) * (Ox1 - Ox2) / (Oy2 - Oy1) + Ox2;
		y2 = gScreenHeight << 4;
	}

	//for anti-clockwise meter puremask shear 
	if((x2 == (-2048 * 16 + 1)) && (x1 == (-2048 * 16 + 1)))
	{
		y1 = (Ox1) * (Oy1 - Oy0) / (Ox0 - Ox1) + Oy1;
		x1 = 0;

		y2 = (Ox2) * (Oy2 - Oy3) / (Ox3 - Ox2) + Oy2;
		x2 = 0;
	}

	if((y2 == (-2048 * 16 + 1)) && (y1 == (-2048 * 16 + 1)))
	{
		x1 = (-Oy1) * (Ox0 - Ox1) / (Oy0 - Oy1) + Ox1;
		y1 = 0;

		x2 = (-Oy2) * (Ox3 - Ox2) / (Oy3 - Oy2) + Ox2;
		y2 = 0;
	}

	if((x2 == (2048 * 16 - 1)) && (x1 == (2048 * 16 - 1)))
	{
		y1 = (Ox0 - (gScreenWidth << 4)) * (Oy0 - Oy1) / (Ox1 - Ox0) + Oy0;
		x1 = gScreenWidth << 4;

		y2 = (Ox3 - (gScreenWidth << 4)) * (Oy3 - Oy2) / (Ox2 - Ox3) + Oy3;
		x2 = gScreenWidth << 4;
	}

	if((y2 == (2048 * 16 - 1)) && (y1 == (2048 * 16 - 1)))
	{
		x1 = ((gScreenHeight << 4) - Oy0) * (Ox1 - Ox0) / (Oy1 - Oy0) + Ox0;
		y1 = gScreenHeight << 4;

		x2 = ((gScreenHeight << 4) - Oy3) * (Ox2 - Ox3) / (Oy2 - Oy3) + Ox3;
		y2 = gScreenHeight << 4;
	}

	p0->mPointX = x0;
	p0->mPointY = y0;
	p1->mPointX = x1;
	p1->mPointY = y1;
	p2->mPointX = x2;
	p2->mPointY = y2;
	p3->mPointX = x3;
	p3->mPointY = y3;

	//compute Coordinate point range
	x_min = x0;
	x_max = x0;
	y_min = y0;
	y_max = y0;

	if(x1 > x_max)
		x_max = x1;
	else if(x1 < x_min)
		x_min = x1;
	if(x2 > x_max)
		x_max = x2;
	else if(x2 < x_min)
		x_min = x2;
	if(x3 > x_max)
		x_max = x3;
	else if(x3 < x_min)
		x_min = x3;

	if(y1 > y_max)
		y_max = y1;
	else if(y1 < y_min)
		y_min = y1;
	if(y2 > y_max)
		y_max = y2;
	else if(y2 < y_min)
		y_min = y2;
	if(y3 > y_max)
		y_max = y3;
	else if(y3 < y_min)
		y_min = y3;

	//Calculation of tile boxes based on min/max coordinate points
	this->TexLeftTileBox   = (x_min >> 4) / TILESIZE; 
	this->TexTopTileBox    = (y_min >> 4) / TILESIZE;
	this->TexRightTileBox  = ((x_max >> 4) + TILESIZE - 1) / TILESIZE - 1;
	this->TexButtomTileBox = ((y_max >> 4) + TILESIZE - 1) / TILESIZE - 1;

	//Source box range judgement
	if(this->TexRightTileBox < 0 || this->TexLeftTileBox >= TILE_NUM_X || this->TexButtomTileBox < 0 || this->TexTopTileBox >= TILE_NUM_Y)
	{
		this->mTexAttr &= ~DRAWING;
	}
	else
	{
		this->mTexAttr |= DRAWING;

		if(this->TexLeftTileBox < 0)
		{
			this->TexLeftTileBox = 0;
		}
		if(this->TexTopTileBox < 0)
		{
			this->TexTopTileBox = 0;
		}
		if(this->TexRightTileBox >= TILE_NUM_X)
		{
			this->TexRightTileBox = TILE_NUM_X - 1;
		}
		if(this->TexButtomTileBox >= TILE_NUM_Y)
		{
			this->TexButtomTileBox = TILE_NUM_Y - 1;
		}
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name��  computeTextureBoxAndPoint
// compute texture tile box and point
// param list��
//	@param1 NewMatrix* M2��			//texture matrix
//	@param1 PointClass* p0��		//left top point
//	@param1 PointClass* p1��		//right top point
//	@param1 PointClass* p2��		//right bottom point
//	@param1 PointClass* p3��		//left bottom point
// Remarks(Revision between versions):
//	compute the texture's box and point
//    added by yn 20181017
//-----------------------------
funcStatus TextureClass::computeTextureBoxAndPointByROI(PointClass* p0, PointClass* p1, PointClass* p2, PointClass* p3)
{
	int x0,y0,x1,y1,x2,y2,x3,y3;
	s32 x_min, x_max, y_min, y_max;

	x0 = p0->mPointX;
	y0 = p0->mPointY;
	x1 = p1->mPointX;
	y1 = p1->mPointY;
	x2 = p2->mPointX;
	y2 = p2->mPointY;
	x3 = p3->mPointX;
	y3 = p3->mPointY;

	//compute Coordinate point range
	x_min = x0;
	x_max = x0;
	y_min = y0;
	y_max = y0;

	if(x1 > x_max)
		x_max = x1;
	else if(x1 < x_min)
		x_min = x1;
	if(x2 > x_max)
		x_max = x2;
	else if(x2 < x_min)
		x_min = x2;
	if(x3 > x_max)
		x_max = x3;
	else if(x3 < x_min)
		x_min = x3;

	if(y1 > y_max)
		y_max = y1;
	else if(y1 < y_min)
		y_min = y1;
	if(y2 > y_max)
		y_max = y2;
	else if(y2 < y_min)
		y_min = y2;
	if(y3 > y_max)
		y_max = y3;
	else if(y3 < y_min)
		y_min = y3;

	//Calculation of tile boxes based on min/max coordinate points
	this->TexLeftTileBox   = (x_min >> 4) / TILESIZE; 
	this->TexTopTileBox    = (y_min >> 4) / TILESIZE;
	this->TexRightTileBox  = ((x_max >> 4) + TILESIZE - 1) / TILESIZE - 1;
	this->TexButtomTileBox = ((y_max >> 4) + TILESIZE - 1) / TILESIZE - 1;

	//Source box range judgement
	if(this->TexRightTileBox < 0 || this->TexLeftTileBox >= TILE_NUM_X || this->TexButtomTileBox < 0 || this->TexTopTileBox >= TILE_NUM_Y)
	{
		this->mTexAttr &= ~DRAWING;
	}
	else
	{
		this->mTexAttr |= DRAWING;

		if(this->TexLeftTileBox < 0)
		{
			this->TexLeftTileBox = 0;
		}
		if(this->TexTopTileBox < 0)
		{
			this->TexTopTileBox = 0;
		}
		if(this->TexRightTileBox >= TILE_NUM_X)
		{
			this->TexRightTileBox = TILE_NUM_X - 1;
		}
		if(this->TexButtomTileBox >= TILE_NUM_Y)
		{
			this->TexButtomTileBox = TILE_NUM_Y - 1;
		}
	}

	return AHMI_FUNC_SUCCESS;
}

void Point_Sort(PointClass* pPoint, uint32_t PointNum)
{
	PointClass Point_All[8];
	uint32_t i, numberofline = 0, j = 0;
	int Point_Pos[8] = {0,1,2,3,4,5,6,7};
	int find_flag = 0, last_min_pos = 0xffffffff;
	s32 x_min, y;
	s32 tan_value[7] = {0};  //.10

	x_min = pPoint[0].mPointX;
	y = pPoint[0].mPointY;

	//Get the leftmost and lowest point
	for(i = 0; i < PointNum; i++)
	{
		Point_All[i].mPointX = pPoint[i].mPointX;
		Point_All[i].mPointY = pPoint[i].mPointY;

		if(Point_All[i].mPointX < x_min)
		{
			x_min = Point_All[i].mPointX;
			y = Point_All[i].mPointY;
		}
		else if(Point_All[i].mPointX == x_min)
		{
			if(Point_All[i].mPointY > y)
			{
				x_min = Point_All[i].mPointX;
				y = Point_All[i].mPointY;
			}
		}
	}

	//Determine whether the polygon contains the left most vertical boundary
	for(i = 0; i < PointNum; i++)
	{
		if(Point_All[i].mPointX == x_min)
		{
			find_flag++;
		}
	}

	//The slope of the two-point connection is calculated and stored in tan_value[] array
	for(i = 0; i < PointNum; i++)
	{
		if(x_min == Point_All[i].mPointX)
		{
			if(last_min_pos != (int)0xffffffff)
			{
				if(Point_All[i].mPointY < y)
				{
					Point_Pos[1] = i;
					continue;
				}
				else
				{
					Point_Pos[1] = last_min_pos;
					Point_Pos[0] = i;
					continue;
				}
			}
			else
			{
				Point_Pos[0] = i;
				last_min_pos = i;
				continue;
			}
		}
		else
		{
			Point_Pos[j + find_flag] = i;
			tan_value[j++] = ((Point_All[i].mPointY - y) << 10) / (Point_All[i].mPointX - x_min);
		}
	}

	//Determine whether the polygon contains the left most vertical boundary
	if(find_flag == 2)
	{
		numberofline = PointNum - 2;
	}
	else
	{
		numberofline = PointNum - 1;
	}

	//tan_value[] array and Point_Pos[] array sort
	for(i = 0; i < numberofline - 1; ++i)
	{
		for(j = i + 1;j < numberofline; ++j)
		{
			if(tan_value[i] > tan_value[j])
			{
				int t = tan_value[i];
				tan_value[i] = tan_value[j];
				tan_value[j] = t;

				t = Point_Pos[i + find_flag];
				Point_Pos[i + find_flag] = Point_Pos[j + find_flag];
				Point_Pos[j + find_flag] = t;
			}
		}
	}

	//Rearrange coordinate points
	for(i = 0; i < PointNum; i++)
	{
		pPoint[i].mPointX = Point_All[Point_Pos[i]].mPointX;
		pPoint[i].mPointY = Point_All[Point_Pos[i]].mPointY;
	}
}

#if 0
/* software cliping */
bool judge_PointInQuadrilateral(PointClass* x, PointClass* aa, PointClass* bb, PointClass* cc, PointClass* dd)//Judge whether point x in quadrilateral
{
	int a, b, c, d;

	a = (bb->mPointX - aa->mPointX) * (x->mPointY - aa->mPointY) - (bb->mPointY - aa->mPointY) * (x->mPointX - aa->mPointX);
	b = (cc->mPointX - bb->mPointX) * (x->mPointY - bb->mPointY) - (cc->mPointY - bb->mPointY) * (x->mPointX - bb->mPointX);
	c = (dd->mPointX - cc->mPointX) * (x->mPointY - cc->mPointY) - (dd->mPointY - cc->mPointY) * (x->mPointX - cc->mPointX);
	d = (aa->mPointX - dd->mPointX) * (x->mPointY - dd->mPointY) - (aa->mPointY - dd->mPointY) * (x->mPointX - dd->mPointX);

	if((a > 0 && b > 0 && c > 0 && d > 0) || (a < 0 && b < 0 && c < 0 && d < 0))
	{
		return true;
	}

	return false;
}

bool judge_intersect(PointClass* aa, PointClass* bb, PointClass* cc, PointClass* dd)//Judge whether ab and cd intersect
{  
	PointClass ac(cc->mPointX - aa->mPointX, cc->mPointY - aa->mPointY), ab(bb->mPointX - aa->mPointX, bb->mPointY - aa->mPointY), ad(dd->mPointX - aa->mPointX, dd->mPointY - aa->mPointY);

	if((aa->mPointX < cc->mPointX) && (aa->mPointX < dd->mPointX) && (bb->mPointX < cc->mPointX) && (bb->mPointX < dd->mPointX))
	{
		return false;
	}
	if((aa->mPointX > cc->mPointX) && (aa->mPointX > dd->mPointX) && (bb->mPointX > cc->mPointX) && (bb->mPointX > dd->mPointX))
	{
		return false;
	}
	if((aa->mPointY < cc->mPointY) && (aa->mPointY < dd->mPointY) && (bb->mPointY < cc->mPointY) && (bb->mPointY < dd->mPointY))
	{
		return false;
	}
	if((aa->mPointY > cc->mPointY) && (aa->mPointY > dd->mPointY) && (bb->mPointY > cc->mPointY) && (bb->mPointY > dd->mPointY))
	{
		return false;
	}

	//ac x ab																	//ab x ad
	if((long long)((long long)(ac.mPointX * ab.mPointY - ab.mPointX * ac.mPointY) * (long long)(ab.mPointX * ad.mPointY - ad.mPointX * ab.mPointY)) < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}  

PointClass Get_Intersect_Point(PointClass* aa, PointClass* bb, PointClass* cc, PointClass* dd)
{
	PointClass intersect_point(0,0);

	if(cc->mPointX == dd->mPointX)
	{
		intersect_point.mPointX = cc->mPointX;
		intersect_point.mPointY = -(intersect_point.mPointX - aa->mPointX)*(aa->mPointY - bb->mPointY)/(bb->mPointX - aa->mPointX) + aa->mPointY;
	}
	else if(cc->mPointY == dd->mPointY)
	{
		intersect_point.mPointY = cc->mPointY;
		intersect_point.mPointX = (intersect_point.mPointY - aa->mPointY)*(bb->mPointX - aa->mPointX)/(bb->mPointY - aa->mPointY) + aa->mPointX;
	}

	return intersect_point;
}

uint32_t Get_All_Point(PointClass* p0, PointClass* p1, PointClass* p2, PointClass* p3,PointClass* Boundary_p0, PointClass* Boundary_p1, PointClass* Boundary_p2, PointClass* Boundary_p3, PointClass* pPoint)
{
	PointClass Point_All[8];
	int pointNum = 0;

	//get p0 p1 p2 p3 which in Boundary window
	if((p0->mPointX >= Boundary_p0->mPointX) && (p0->mPointX <= Boundary_p2->mPointX) && (p0->mPointY >= Boundary_p0->mPointY) && (p0->mPointY <= Boundary_p2->mPointY))
	{
		Point_All[pointNum].mPointX = p0->mPointX;
		Point_All[pointNum++].mPointY = p0->mPointY;
	}

	if((p1->mPointX >= Boundary_p0->mPointX) && (p1->mPointX <= Boundary_p2->mPointX) && (p1->mPointY >= Boundary_p0->mPointY) && (p1->mPointY <= Boundary_p2->mPointY))
	{
		Point_All[pointNum].mPointX = p1->mPointX;
		Point_All[pointNum++].mPointY = p1->mPointY;
	}

	if((p2->mPointX >= Boundary_p0->mPointX) && (p2->mPointX <= Boundary_p2->mPointX) && (p2->mPointY >= Boundary_p0->mPointY) && (p2->mPointY <= Boundary_p2->mPointY))
	{
		Point_All[pointNum].mPointX = p2->mPointX;
		Point_All[pointNum++].mPointY = p2->mPointY;
	}

	if((p3->mPointX >= Boundary_p0->mPointX) && (p3->mPointX <= Boundary_p2->mPointX) && (p3->mPointY >= Boundary_p0->mPointY) && (p3->mPointY <= Boundary_p2->mPointY))
	{
		Point_All[pointNum].mPointX = p3->mPointX;
		Point_All[pointNum++].mPointY = p3->mPointY;
	}

	//get Boundary_p0 Boundary_p1 Boundary_p2 Boundary_p3 which in quadrilateral
	if(judge_PointInQuadrilateral(Boundary_p0, p0, p1, p2, p3) == true)
	{
		Point_All[pointNum].mPointX = Boundary_p0->mPointX;
		Point_All[pointNum++].mPointY = Boundary_p0->mPointY;
	}

	if(judge_PointInQuadrilateral(Boundary_p1, p0, p1, p2, p3) == true)
	{
		Point_All[pointNum].mPointX = Boundary_p1->mPointX;
		Point_All[pointNum++].mPointY = Boundary_p1->mPointY;
	}

	if(judge_PointInQuadrilateral(Boundary_p2, p0, p1, p2, p3) == true)
	{
		Point_All[pointNum].mPointX = Boundary_p2->mPointX;
		Point_All[pointNum++].mPointY = Boundary_p2->mPointY;
	}

	if(judge_PointInQuadrilateral(Boundary_p3, p0, p1, p2, p3) == true)
	{
		Point_All[pointNum].mPointX = Boundary_p3->mPointX;
		Point_All[pointNum++].mPointY = Boundary_p3->mPointY;
	}

	//get the intersection point of p0p1 and the four sides of quadrilateral
	if((judge_intersect(p0, p1, Boundary_p0, Boundary_p1) == true))
	{
		PointClass a = Get_Intersect_Point(p0, p1, Boundary_p0, Boundary_p1);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}
	if((judge_intersect(p0, p1, Boundary_p1, Boundary_p2) == true))
	{
		PointClass a = Get_Intersect_Point(p0, p1, Boundary_p1, Boundary_p2);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}
	if((judge_intersect(p0, p1, Boundary_p2, Boundary_p3) == true))
	{
		PointClass a = Get_Intersect_Point(p0, p1, Boundary_p2, Boundary_p3);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}
	if((judge_intersect(p0, p1, Boundary_p3, Boundary_p0) == true))
	{
		PointClass a = Get_Intersect_Point(p0, p1, Boundary_p3, Boundary_p0);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}

	//get the intersection point of p1p2 and the four sides of quadrilateral
	if((judge_intersect(p1, p2, Boundary_p0, Boundary_p1) == true))
	{
		PointClass a = Get_Intersect_Point(p1, p2, Boundary_p0, Boundary_p1);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}
	if((judge_intersect(p1, p2, Boundary_p1, Boundary_p2) == true))
	{
		PointClass a = Get_Intersect_Point(p1, p2, Boundary_p1, Boundary_p2);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}
	if((judge_intersect(p1, p2, Boundary_p2, Boundary_p3) == true))
	{
		PointClass a = Get_Intersect_Point(p1, p2, Boundary_p2, Boundary_p3);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}
	if((judge_intersect(p1, p2, Boundary_p3, Boundary_p0) == true))
	{
		PointClass a = Get_Intersect_Point(p1, p2, Boundary_p3, Boundary_p0);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}

	//get the intersection point of p2p3 and the four sides of quadrilateral
	if((judge_intersect(p2, p3, Boundary_p0, Boundary_p1) == true))
	{
		PointClass a = Get_Intersect_Point(p2, p3, Boundary_p0, Boundary_p1);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}
	if((judge_intersect(p2, p3, Boundary_p1, Boundary_p2) == true))
	{
		PointClass a = Get_Intersect_Point(p2, p3, Boundary_p1, Boundary_p2);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}
	if((judge_intersect(p2, p3, Boundary_p2, Boundary_p3) == true))
	{
		PointClass a = Get_Intersect_Point(p2, p3, Boundary_p2, Boundary_p3);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}
	if((judge_intersect(p2, p3, Boundary_p3, Boundary_p0) == true))
	{
		PointClass a = Get_Intersect_Point(p2, p3, Boundary_p3, Boundary_p0);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}

	//get the intersection point of p3p0 and the four sides of quadrilateral
	if((judge_intersect(p3, p0, Boundary_p0, Boundary_p1) == true))
	{
		PointClass a = Get_Intersect_Point(p3, p0, Boundary_p0, Boundary_p1);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}
	if((judge_intersect(p3, p0, Boundary_p1, Boundary_p2) == true))
	{
		PointClass a = Get_Intersect_Point(p3, p0, Boundary_p1, Boundary_p2);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}
	if((judge_intersect(p3, p0, Boundary_p2, Boundary_p3) == true))
	{
		PointClass a = Get_Intersect_Point(p3, p0, Boundary_p2, Boundary_p3);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}
	if((judge_intersect(p3, p0, Boundary_p3, Boundary_p0) == true))
	{
		PointClass a = Get_Intersect_Point(p3, p0, Boundary_p3, Boundary_p0);
		Point_All[pointNum].mPointX = a.mPointX;
		Point_All[pointNum++].mPointY = a.mPointY;
	}

	//fill in the final coordinate point result array
	for(int i = 0; i < pointNum; i++)
	{
		pPoint[i].mPointX = Point_All[i].mPointX;
		pPoint[i].mPointY = Point_All[i].mPointY;
	}

	//returns the number of coordinate points obtained
	return pointNum;
}

void Point_Segmentation(PointClass* pInPoint, PointClass* pOutPoint, uint32_t PointNum, uint32_t* FinalPointNum)
{
	PointClass Point_Final[12];
//	int i = 0;

	if(PointNum == 3)
	{
		*FinalPointNum = 4;

		if(pInPoint[0].mPointX == pInPoint[1].mPointX)
		{
			pOutPoint[0].mPointX = pInPoint[0].mPointX;
			pOutPoint[0].mPointY = pInPoint[0].mPointY;
			pOutPoint[1].mPointX = pInPoint[1].mPointX;
			pOutPoint[1].mPointY = pInPoint[1].mPointY;
			pOutPoint[2].mPointX = pInPoint[2].mPointX;
			pOutPoint[2].mPointY = pInPoint[2].mPointY;
			pOutPoint[3].mPointX = pInPoint[2].mPointX;
			pOutPoint[3].mPointY = pInPoint[2].mPointY + 1;
		}
		else if(pInPoint[0].mPointY == pInPoint[1].mPointY)
		{
			pOutPoint[0].mPointX = pInPoint[0].mPointX;
			pOutPoint[0].mPointY = pInPoint[0].mPointY;
			pOutPoint[1].mPointX = pInPoint[1].mPointX;
			pOutPoint[1].mPointY = pInPoint[1].mPointY;
			pOutPoint[2].mPointX = pInPoint[2].mPointX;
			pOutPoint[2].mPointY = pInPoint[2].mPointY;
			pOutPoint[3].mPointX = pInPoint[2].mPointX - 1;
			pOutPoint[3].mPointY = pInPoint[2].mPointY;
		}
		else if(pInPoint[1].mPointX == pInPoint[2].mPointX)
		{
			pOutPoint[0].mPointX = pInPoint[0].mPointX;
			pOutPoint[0].mPointY = pInPoint[0].mPointY;
			pOutPoint[1].mPointX = pInPoint[1].mPointX;
			pOutPoint[1].mPointY = pInPoint[1].mPointY;
			pOutPoint[2].mPointX = pInPoint[2].mPointX;
			pOutPoint[2].mPointY = pInPoint[2].mPointY;
			pOutPoint[3].mPointX = pInPoint[0].mPointX;
			pOutPoint[3].mPointY = pInPoint[0].mPointY + 1;
		}
		else if(pInPoint[0].mPointY == pInPoint[2].mPointY)
		{
			pOutPoint[0].mPointX = pInPoint[0].mPointX;
			pOutPoint[0].mPointY = pInPoint[0].mPointY;
			pOutPoint[1].mPointX = pInPoint[1].mPointX;
			pOutPoint[1].mPointY = pInPoint[1].mPointY;
			pOutPoint[2].mPointX = pInPoint[1].mPointX + 1;
			pOutPoint[2].mPointY = pInPoint[1].mPointY;
			pOutPoint[3].mPointX = pInPoint[2].mPointX;
			pOutPoint[3].mPointY = pInPoint[2].mPointY;
		}
	}
	else if(PointNum == 4)
	{
		*FinalPointNum = 4;

		pOutPoint[0].mPointX = pInPoint[0].mPointX;
		pOutPoint[0].mPointY = pInPoint[0].mPointY;
		pOutPoint[1].mPointX = pInPoint[1].mPointX;
		pOutPoint[1].mPointY = pInPoint[1].mPointY;
		pOutPoint[2].mPointX = pInPoint[2].mPointX;
		pOutPoint[2].mPointY = pInPoint[2].mPointY;
		pOutPoint[3].mPointX = pInPoint[3].mPointX;
		pOutPoint[3].mPointY = pInPoint[3].mPointY;
	}
	else if(PointNum == 5)
	{
		*FinalPointNum = 8;

		pOutPoint[0].mPointX = pInPoint[0].mPointX;
		pOutPoint[0].mPointY = pInPoint[0].mPointY;
		pOutPoint[1].mPointX = pInPoint[1].mPointX;
		pOutPoint[1].mPointY = pInPoint[1].mPointY;
		pOutPoint[2].mPointX = pInPoint[2].mPointX;
		pOutPoint[2].mPointY = pInPoint[2].mPointY;
		pOutPoint[3].mPointX = pInPoint[3].mPointX;
		pOutPoint[3].mPointY = pInPoint[3].mPointY;

		pOutPoint[4].mPointX = pInPoint[0].mPointX;
		pOutPoint[4].mPointY = pInPoint[0].mPointY;
		pOutPoint[5].mPointX = pInPoint[2].mPointX;
		pOutPoint[5].mPointY = pInPoint[2].mPointY;
		pOutPoint[6].mPointX = pInPoint[3].mPointX;
		pOutPoint[6].mPointY = pInPoint[3].mPointY;
		pOutPoint[7].mPointX = pInPoint[4].mPointX;
		pOutPoint[7].mPointY = pInPoint[4].mPointY;
	}
	else if(PointNum == 6)
	{
		*FinalPointNum = 8;

		pOutPoint[0].mPointX = pInPoint[0].mPointX;
		pOutPoint[0].mPointY = pInPoint[0].mPointY;
		pOutPoint[1].mPointX = pInPoint[1].mPointX;
		pOutPoint[1].mPointY = pInPoint[1].mPointY;
		pOutPoint[2].mPointX = pInPoint[2].mPointX;
		pOutPoint[2].mPointY = pInPoint[2].mPointY;
		pOutPoint[3].mPointX = pInPoint[3].mPointX;
		pOutPoint[3].mPointY = pInPoint[3].mPointY;

		pOutPoint[4].mPointX = pInPoint[0].mPointX;
		pOutPoint[4].mPointY = pInPoint[0].mPointY;
		pOutPoint[5].mPointX = pInPoint[3].mPointX;
		pOutPoint[5].mPointY = pInPoint[3].mPointY;
		pOutPoint[6].mPointX = pInPoint[4].mPointX;
		pOutPoint[6].mPointY = pInPoint[4].mPointY;
		pOutPoint[7].mPointX = pInPoint[5].mPointX;
		pOutPoint[7].mPointY = pInPoint[5].mPointY;
	}
	else if(PointNum == 7)
	{
		*FinalPointNum = 12;

		pOutPoint[0].mPointX = pInPoint[0].mPointX;
		pOutPoint[0].mPointY = pInPoint[0].mPointY;
		pOutPoint[1].mPointX = pInPoint[1].mPointX;
		pOutPoint[1].mPointY = pInPoint[1].mPointY;
		pOutPoint[2].mPointX = pInPoint[2].mPointX;
		pOutPoint[2].mPointY = pInPoint[2].mPointY;
		pOutPoint[3].mPointX = pInPoint[3].mPointX;
		pOutPoint[3].mPointY = pInPoint[3].mPointY;

		pOutPoint[4].mPointX = pInPoint[0].mPointX;
		pOutPoint[4].mPointY = pInPoint[0].mPointY;
		pOutPoint[5].mPointX = pInPoint[3].mPointX;
		pOutPoint[5].mPointY = pInPoint[3].mPointY;
		pOutPoint[6].mPointX = pInPoint[4].mPointX;
		pOutPoint[6].mPointY = pInPoint[4].mPointY;
		pOutPoint[7].mPointX = pInPoint[5].mPointX;
		pOutPoint[7].mPointY = pInPoint[5].mPointY;

		pOutPoint[8].mPointX = pInPoint[0].mPointX;
		pOutPoint[8].mPointY = pInPoint[0].mPointY;
		pOutPoint[9].mPointX = pInPoint[4].mPointX;
		pOutPoint[9].mPointY = pInPoint[4].mPointY;
		pOutPoint[10].mPointX = pInPoint[5].mPointX;
		pOutPoint[10].mPointY = pInPoint[5].mPointY;
		pOutPoint[11].mPointX = pInPoint[6].mPointX;
		pOutPoint[11].mPointY = pInPoint[6].mPointY;
	}
	else if(PointNum == 8)
	{
		*FinalPointNum = 12;

		pOutPoint[0].mPointX = pInPoint[0].mPointX;
		pOutPoint[0].mPointY = pInPoint[0].mPointY;
		pOutPoint[1].mPointX = pInPoint[1].mPointX;
		pOutPoint[1].mPointY = pInPoint[1].mPointY;
		pOutPoint[2].mPointX = pInPoint[2].mPointX;
		pOutPoint[2].mPointY = pInPoint[2].mPointY;
		pOutPoint[3].mPointX = pInPoint[3].mPointX;
		pOutPoint[3].mPointY = pInPoint[3].mPointY;

		pOutPoint[4].mPointX = pInPoint[0].mPointX;
		pOutPoint[4].mPointY = pInPoint[0].mPointY;
		pOutPoint[5].mPointX = pInPoint[3].mPointX;
		pOutPoint[5].mPointY = pInPoint[3].mPointY;
		pOutPoint[6].mPointX = pInPoint[4].mPointX;
		pOutPoint[6].mPointY = pInPoint[4].mPointY;
		pOutPoint[7].mPointX = pInPoint[5].mPointX;
		pOutPoint[7].mPointY = pInPoint[5].mPointY;

		pOutPoint[8].mPointX = pInPoint[0].mPointX;
		pOutPoint[8].mPointY = pInPoint[0].mPointY;
		pOutPoint[9].mPointX = pInPoint[5].mPointX;
		pOutPoint[9].mPointY = pInPoint[5].mPointY;
		pOutPoint[10].mPointX = pInPoint[6].mPointX;
		pOutPoint[10].mPointY = pInPoint[6].mPointY;
		pOutPoint[11].mPointX = pInPoint[7].mPointX;
		pOutPoint[11].mPointY = pInPoint[7].mPointY;
	}
	else
	{
		ERROR_PRINT("Error Point Segmentation.\r\n");
	}

}

//-----------------------------
// function name��  tailoringTexturePoint
// tailoring texture point by boundary point
// param list��
//	@param PointClass* p0��			//left top point
//	@param PointClass* p1��			//right top point
//	@param PointClass* p2��			//right bottom point
//	@param PointClass* p3��			//left bottom point
//	@param PointClass* Boundary_p0��	//left boundary top point
//	@param PointClass* Boundary_p1��	//right boundary top point
//	@param PointClass* Boundary_p2��	//right boundary bottom point
//	@param PointClass* Boundary_p3��	//left boundary bottom point
//	@param PointClass* pPoint��			//final point array start addr
//	@param uint32_t PointNum��			//final point number
// Remarks(Revision between versions):
//	tailoring texture point by boundary point
//    added by yn 20200902
//-----------------------------
funcStatus TextureClass::tailoringTexturePoint(PointClass* p0, PointClass* p1, PointClass* p2, PointClass* p3,PointClass* Boundary_p0, PointClass* Boundary_p1, PointClass* Boundary_p2, PointClass* Boundary_p3, PointClass* pPoint, uint32_t* PointNum)
{
	PointClass Point_All[8];
	int x0,y0,x1,y1,x2,y2,x3,y3,pointNumWithinRange = 0;
	s32 x_min, x_max, y_min, y_max;
	u8 Flag0 = 0, Flag1 = 0, Flag2 = 0, Flag3 = 0;

	x0 = p0->mPointX;
	y0 = p0->mPointY;
	x1 = p1->mPointX;
	y1 = p1->mPointY;
	x2 = p2->mPointX;
	y2 = p2->mPointY;
	x3 = p3->mPointX;
	y3 = p3->mPointY;

	//rectangle
	if(((x0 == x3) && (y0 == y1) && (x1 == x2) && (y2 == y3)) || ((x2 == x3) && (y0 == y3) && (x1 == x0) && (y2 == y1)))
	{
		//compute Coordinate point range
		x_min = x0;
		x_max = x0;
		y_min = y0;
		y_max = y0;

		if(x1 > x_max)
			x_max = x1;
		else if(x1 < x_min)
			x_min = x1;
		if(x2 > x_max)
			x_max = x2;
		else if(x2 < x_min)
			x_min = x2;
		if(x3 > x_max)
			x_max = x3;
		else if(x3 < x_min)
			x_min = x3;

		if(y1 > y_max)
			y_max = y1;
		else if(y1 < y_min)
			y_min = y1;
		if(y2 > y_max)
			y_max = y2;
		else if(y2 < y_min)
			y_min = y2;
		if(y3 > y_max)
			y_max = y3;
		else if(y3 < y_min)
			y_min = y3;

		p0->mPointX = x_min;
		p0->mPointY = y_min;
		p1->mPointX = x_max;
		p1->mPointY = y_min;
		p2->mPointX = x_max;
		p2->mPointY = y_max;
		p3->mPointX = x_min;
		p3->mPointY = y_max;

		if((x0 >= Boundary_p0->mPointX) && (x0 <= Boundary_p2->mPointX) && (y0 >= Boundary_p0->mPointY) && (y0 <= Boundary_p2->mPointY))
		{
			pointNumWithinRange++;
		}
		else
		{
			Flag0 = 1;
		}
		if((x1 >= Boundary_p0->mPointX) && (x1 <= Boundary_p2->mPointX) && (y1 >= Boundary_p0->mPointY) && (y1 <= Boundary_p2->mPointY))
		{
			pointNumWithinRange++;
		}
		else
		{
			Flag1 = 1;
		}
		if((x2 >= Boundary_p0->mPointX) && (x2 <= Boundary_p2->mPointX) && (y2 >= Boundary_p0->mPointY) && (y2 <= Boundary_p2->mPointY))
		{
			pointNumWithinRange++;
		}
		else
		{
			Flag2 = 1;
		}
		if((x3 >= Boundary_p0->mPointX) && (x3 <= Boundary_p2->mPointX) && (y3 >= Boundary_p0->mPointY) && (y3 <= Boundary_p2->mPointY))
		{
			pointNumWithinRange++;
		}
		else
		{
			Flag3 = 1;
		}

		if(pointNumWithinRange == 4)
		{
			return AHMI_FUNC_SUCCESS;
		}
		else if(pointNumWithinRange == 3)
		{
			return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateNumErr);
		}
		else if(pointNumWithinRange == 2)
		{
			if((Flag0 == 1) && (Flag1 == 1) && (Flag2 == 0) && (Flag3 == 0))
			{
				p0->mPointY = Boundary_p0->mPointY;
				p1->mPointY = Boundary_p0->mPointY;
			}
			else if((Flag1 == 1) && (Flag2 == 1) && (Flag3 == 0) && (Flag0 == 0))
			{
				p1->mPointX = Boundary_p1->mPointX;
				p2->mPointX = Boundary_p1->mPointX;
			}
			else if((Flag2 == 1) && (Flag3 == 1) && (Flag0 == 0) && (Flag1 == 0))
			{
				p2->mPointY = Boundary_p2->mPointY;
				p3->mPointY = Boundary_p2->mPointY;
			}
			else if((Flag3 == 1) && (Flag0 == 1) && (Flag1 == 0) && (Flag2 == 0))
			{
				p3->mPointX = Boundary_p3->mPointX;
				p0->mPointX = Boundary_p3->mPointX;
			}
		}
		else if(pointNumWithinRange == 1)
		{
			if((Flag0 == 0) && (Flag1 == 1) && (Flag2 == 1) && (Flag3 == 1))
			{
				p1->mPointX = Boundary_p1->mPointX;
				p3->mPointY = Boundary_p3->mPointY;
				p2->mPointX = Boundary_p2->mPointX;
				p2->mPointY = Boundary_p2->mPointY;
			}
			else if((Flag1 ==0) && (Flag2 == 1) && (Flag3 == 1) && (Flag0 == 1))
			{
				p0->mPointX = Boundary_p0->mPointX;
				p2->mPointY = Boundary_p2->mPointY;
				p3->mPointX = Boundary_p3->mPointX;
				p3->mPointY = Boundary_p3->mPointY;
			}
			else if((Flag2 ==0) && (Flag3 == 1) && (Flag0 == 1) && (Flag1 == 1))
			{
				p3->mPointX = Boundary_p3->mPointX;
				p1->mPointY = Boundary_p1->mPointY;
				p0->mPointX = Boundary_p0->mPointX;
				p0->mPointY = Boundary_p0->mPointY;
			}
			else if((Flag3 ==0) && (Flag0 == 1) && (Flag1 == 1) && (Flag2 == 1))
			{
				p2->mPointX = Boundary_p2->mPointX;
				p0->mPointY = Boundary_p0->mPointY;
				p1->mPointX = Boundary_p1->mPointX;
				p1->mPointY = Boundary_p1->mPointY;
			}
		}
		else if(pointNumWithinRange == 0)
		{
			if((p0->mPointX > Boundary_p1->mPointX) || (p1->mPointX < Boundary_p0->mPointX) || (p2->mPointY < Boundary_p0->mPointY) || (p0->mPointY > Boundary_p2->mPointY))
			{
				return (EM_enErrRetModularTexture | EM_ErrRetTexture_CoordinateOverrun);
			}
			else
			{
				if(p0->mPointX < Boundary_p0->mPointX)
				{
					p0->mPointX = Boundary_p0->mPointX;
				}
				if(p0->mPointY < Boundary_p0->mPointY)
				{
					p0->mPointY = Boundary_p0->mPointY;
				}
				if(p1->mPointX > Boundary_p1->mPointX)
				{
					p1->mPointX = Boundary_p1->mPointX;
				}
				if(p1->mPointY < Boundary_p1->mPointY)
				{
					p1->mPointY = Boundary_p1->mPointY;
				}
				if(p2->mPointX > Boundary_p2->mPointX)
				{
					p2->mPointX = Boundary_p2->mPointX;
				}
				if(p2->mPointY > Boundary_p2->mPointY)
				{
					p2->mPointY = Boundary_p2->mPointY;
				}
				if(p3->mPointX < Boundary_p3->mPointX)
				{
					p3->mPointX = Boundary_p3->mPointX;
				}
				if(p3->mPointY > Boundary_p3->mPointY)
				{
					p3->mPointY = Boundary_p3->mPointY;
				}
			}
		}

		return AHMI_FUNC_SUCCESS;
	}

	*PointNum = Get_All_Point(p0, p1, p2, p3, Boundary_p0, Boundary_p1, Boundary_p2, Boundary_p3, Point_All);
	Point_Sort(Point_All, *PointNum);
	Point_Segmentation(Point_All, pPoint, *PointNum, PointNum);
	return AHMI_FUNC_SUCCESS;
}
#endif

#ifndef CHIP_GC9002_NoHyper
extern CopyPaste_Struct mCopyPaste_Struct[CopyPaste_Number_MAX];

void TransformTextureFromPoint(u32* pdata, u16 PageID, u16 LTPointX, u16 LTPointY, u16 RBPointX, u16 RBPointY, u16 PastePointX, u16 PastePointY)
{
	s32 Texture_OffsetX, Texture_OffsetY;
	u32 Texture_Width, Texture_Height, TexLeftTileBox, TexTopTileBox, TexRightTileBox, TexButtomTileBox;
	u32 TextureAddr = 0;

	Texture_Width = RBPointX - LTPointX;
	Texture_Height = RBPointY - LTPointY;
	Texture_OffsetX = (PastePointX - LTPointX);
	Texture_OffsetY = (PastePointY - LTPointY);

	//Calculation of tile boxes based on min/max coordinate points
	TexLeftTileBox   = (PastePointX) / TILESIZE; 
	TexTopTileBox    = (PastePointY) / TILESIZE;
	TexRightTileBox  = ((PastePointX + Texture_Width) + TILESIZE - 1) / TILESIZE - 1;
	TexButtomTileBox = ((PastePointY + Texture_Height) + TILESIZE - 1) / TILESIZE - 1;

	if(gBackgroundTextureAddrPtr[PageID - 1] == 0x5A5A5A5A)
	{
		return;
	}
	if((gBackgroundTextureAddrPtr[PageID - 1] & 0xFF000000) == 0xFF000000)
	{
		*pdata++ = 0x00000030;
		TextureAddr = gBackgroundTextureAddrPtr[PageID - 1];
	}
	else if((gBackgroundTextureAddrPtr[PageID - 1] & 0xC0000000) == 0x80000000)
	{
		*pdata++ = 0x000000E0;
		TextureAddr = (gBackgroundTextureAddrPtr[PageID - 1] & 0x3FFFFFFF);
	}
	else
	{
		*pdata++ = 0x00000050;
		TextureAddr = gBackgroundTextureAddrPtr[PageID - 1];
	}

	#if (defined CHIP_GC9005)
	pdata--;
	*(pdata++) |= (1 << 18);//CHIP_GC9005 sourcebuffer tilesize 16x16 {18bit: 0 means 32x32; 1 means 16x16};
	#endif

	*pdata++ = (PastePointX << 20) | (PastePointY << 4);
	*pdata++ = ((PastePointX + Texture_Width) << 20) | (PastePointY << 4);
	*pdata++ = ((PastePointX + Texture_Width) << 20) | ((PastePointY + Texture_Height) << 4);
	*pdata++ = (PastePointX << 20) | ((PastePointY + Texture_Height) << 4);
	*pdata++ = (1 << 20);
	*pdata++ = 0x0;
	*pdata++ = 0x0;
	*pdata++ = 0x0;
	*pdata++ = (1 << 20);
	*pdata++ = 0x0;
	*pdata++ = (-Texture_OffsetX) << 13;
	*pdata++ = (-Texture_OffsetY) << 13;
	*pdata++ = (gScreenWidth << 16) | gScreenHeight;
	*pdata++ = TextureAddr;
	*pdata++ = (TexLeftTileBox & 0xFF) | ((TexTopTileBox & 0xFF) << 8) | ((TexRightTileBox & 0xFF) << 16) | ((TexButtomTileBox & 0xFF) << 24);
}

void AddTextureFromCopyPasteAction(u32 *psourceshift, u16 widgetID)
{
	uint32_t i = 0, j = 0;
	uint32_t sourcebufferarry[16] = {0};
	
	#if (defined PC_SIM)   
	u8 *sourcebufferaddr;
	sourcebufferaddr = (u8 *)SoureBufferAddr;
	#endif

	#if (defined VERSION_2) && (defined EMBEDDED)  
	u8 *sourcebufferaddr = 0;	
	sourcebufferaddr = (u8 *)SBAddress;	
	#endif 
	
	for(i = 0; i < CopyPaste_Number_MAX; i++)
	{
		//if((mCopyPaste_Struct[i].SourcePageID & 0x8000) == 0x8000)
		{
			if(mCopyPaste_Struct[i].DestWidgetID == widgetID)
			{
				TransformTextureFromPoint((u32*)sourcebufferarry, mCopyPaste_Struct[i].SourcePageID & 0x7FFF, mCopyPaste_Struct[i].LTPointX, mCopyPaste_Struct[i].LTPointY, mCopyPaste_Struct[i].RBPointX, mCopyPaste_Struct[i].RBPointY, mCopyPaste_Struct[i].PastePointX, mCopyPaste_Struct[i].PastePointY);
				
				if(sourcebufferarry[0] != 0)
				{
					for(j = 0; j < 64; j++)
					{
						*(sourcebufferaddr + (*psourceshift)++) = ((sourcebufferarry[j/4] >> (8*(j%4))) & 0xFF);//texture attribute
					}
					gNewSoureBufferSize = 64;
					NumberOFLongTexture++;
				}
				return;
			}
		}
	}
}

#if (defined VERSION_2) && (defined EMBEDDED)
extern u32 StartAddrOfSnapshotBuffer;
extern uint32_t  START_ADDR_OFFSET_FROM_GCHEAP;
#endif
void AddTextureFromSnapshot(u32 *psourceshift, WidgetClassPtr p_wptr)
{
	uint32_t i = 0, j = 0;
	uint32_t sourcebufferarry[16] = {0};
	
	#if (defined PC_SIM)   
	u8 *sourcebufferaddr;
	sourcebufferaddr = (u8 *)SoureBufferAddr;
	#endif

	#if (defined VERSION_2) && (defined EMBEDDED)  
	u8 *sourcebufferaddr = 0;	
	sourcebufferaddr = (u8 *)SBAddress;	
	#endif 

	s32 Texture_OffsetX, Texture_OffsetY;
	u32 Texture_Width, Texture_Height, TexLeftTileBox, TexTopTileBox, TexRightTileBox, TexButtomTileBox;
	u32 TextureAddr = 0;

	#if (defined PC_SIM)   
	TextureAddr = START_ADDR_OF_Snapshot + START_ADDR_OFFSET;
	#endif

	#if (defined VERSION_2) && (defined EMBEDDED)
	if(StartAddrOfSnapshotBuffer == 0)
		return;
		
	TextureAddr = StartAddrOfSnapshotBuffer + START_ADDR_OFFSET_FROM_GCHEAP;
	#endif

	Texture_Width = SCREEN_WIDTH;
	Texture_Height = SCREEN_HEIGHT;
	Texture_OffsetX = p_wptr->WidgetOffsetX;
	Texture_OffsetY = p_wptr->WidgetOffsetY;

	//Calculation of tile boxes based on min/max coordinate points
	TexLeftTileBox   = (Texture_OffsetX) / TILESIZE; 
	TexTopTileBox    = (Texture_OffsetY) / TILESIZE;
	TexRightTileBox  = ((Texture_OffsetX + p_wptr->WidgetWidth) + TILESIZE - 1) / TILESIZE - 1;
	TexButtomTileBox = ((Texture_OffsetY + p_wptr->WidgetHeight) + TILESIZE - 1) / TILESIZE - 1;

	#if (defined PC_SIM) 
	sourcebufferarry[0] = RGBA8888;
	#endif

	#if (defined VERSION_2) && (defined EMBEDDED)  
	if(AHMI_GetOutputFormat() == OUTPUTFORMAT_RGBA8888)
	{
		sourcebufferarry[0] = (RGBA8888);
	}
	else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_RGB565)
	{
		sourcebufferarry[0] = (RGB565);
	}
	else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_Y8)
	{
		sourcebufferarry[0] = (Y8);
	}
	else if(AHMI_GetOutputFormat() == OUTPUTFORMAT_SFBC)
	{
		sourcebufferarry[0] = (SFBC);
	}
	else
	{
		return;
	}

	#if (defined CHIP_GC9005) 
	sourcebufferarry[0] |= (1 << 18);//CHIP_GC9005 sourcebuffer tilesize 16x16 {18bit: 0 means 32x32; 1 means 16x16};
	#endif
	#endif 

	sourcebufferarry[1] = (Texture_OffsetX << 20) | (Texture_OffsetY << 4);
	sourcebufferarry[2] = ((Texture_OffsetX + p_wptr->WidgetWidth) << 20) | (Texture_OffsetY << 4);
	sourcebufferarry[3] = ((Texture_OffsetX + p_wptr->WidgetWidth) << 20) | ((Texture_OffsetY + p_wptr->WidgetHeight) << 4);
	sourcebufferarry[4] = (Texture_OffsetX << 20) | ((Texture_OffsetY + p_wptr->WidgetHeight) << 4);
	sourcebufferarry[5] = (1 << 20);
	sourcebufferarry[6] = 0x0;
	sourcebufferarry[7] = 0x0;
	sourcebufferarry[8] = 0x0;
	sourcebufferarry[9] = (1 << 20);
	sourcebufferarry[10] = 0x0;
	sourcebufferarry[11] = (-Texture_OffsetX) << 13;
	sourcebufferarry[12] = (-Texture_OffsetY) << 13;
	sourcebufferarry[13] = (gScreenWidth << 16) | gScreenHeight;
	sourcebufferarry[14] = TextureAddr;
	sourcebufferarry[15] = (TexLeftTileBox & 0xFF) | ((TexTopTileBox & 0xFF) << 8) | ((TexRightTileBox & 0xFF) << 16) | ((TexButtomTileBox & 0xFF) << 24);

	for(int j = 0; j < 64; j++)
	{
		*(sourcebufferaddr + (*psourceshift)++) = ((sourcebufferarry[j/4] >> (8*(j%4))) & 0xFF);//texture attribute
	}
	gNewSoureBufferSize = 64;
	NumberOFLongTexture++;
}
#endif //endof #ifndef CHIP_GC9002_NoHyper

void ChangeTextureBox(u32 sourceshift, u8 startX, u8 startY, u8 stopX, u8 stopY)
{
	u8* sourcebufferaddr;
	
	#ifdef CHIP_GC9002_NoHyper
    sourcebufferaddr = (u8 *)StartAddrOfSourcebuffer;
	#elif (defined PC_SIM)   
	sourcebufferaddr = (u8 *)SoureBufferAddr;
	#else
	sourcebufferaddr = (u8 *)SBAddress;
	#endif
	
	sourcebufferaddr[sourceshift++] = startX;//texture tile box
	sourcebufferaddr[sourceshift++] = startY;
	sourcebufferaddr[sourceshift++] = stopX;
	sourcebufferaddr[sourceshift++] = stopY;
}
#endif
