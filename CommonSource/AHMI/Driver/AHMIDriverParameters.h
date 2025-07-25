////////////////////////////////////////////////////////////////////////////////
// Company:      SX
// Engineer:     AHMI小组成员
//
// Create Date:   2018/10/16
// File Name:     AHMIDriverParameters.h
// Project Name:  AHMIGEN_V2.0
// 
// Revision:
// Revision 2.00 - File Created 2018/10/16 by Zhou Yuzhi
// Additional Comments:
//    the definition of parameters for driver layer
////////////////////////////////////////////////////////////////////////////////

#ifndef AHMI_DRIVER_PARAMETERS__H
#define AHMI_DRIVER_PARAMETERS__H

#include "publicType.h"

typedef struct _AHMI_InitTypeDef
{
	uint32_t OUTPUT_Rotation;	/* output rotation angle */
	uint8_t XDIR_StartTile;		/* X direction of start tile */
	uint8_t YDIR_StartTile;		/* Y direction of start tile */
	uint8_t XDIR_EndTile;		/* X direction of end tile */													
	uint8_t YDIR_EndTile;		/* Y direction of end tile */
	uint32_t MAX_SB;			/* maximum elements of source buffer */
	uint32_t SB_BASE;			/* source buffer base address */
	uint32_t FB_BASE;			/* frame buffer base address */
	uint16_t FRAME_Width;		/* frame width */
	uint16_t FRAME_Hight;		/* frame hight */
	uint32_t OUTPUT_FORMAT;		/*the format of output, 0 is rgb 565 and 1 is rgb 8888*/
} AHMI_InitTypeDef;


/**
	*	@}
	*/

#ifdef VERSION_1
typedef struct _Matrix{
	s32 A;
	s32 B;
	s32 C;
	s32 D;
	s32 E;
	s32 F;
} Matrix;

typedef struct _SOURCE_BOX{
	u8 startx;
	u8 starty;
	u8 stopx;
	u8 stopy;
} SOURCE_BOX;//32bit

typedef struct _TEXTURE_WH{
	u16 width;
	u16 height;
} TEXTURE_WH;//32bit

typedef struct _NewSourceBuffer
{
	u32 tex_type;//32bit = 4byte 纹理控制信号，
	             //tex_type[31:11]:Reserved
				 //tex_type[10:9]:MaskType
				 //tex_type[8]:AddrType 地址类型
				 //tex_type[7:4]:TextureType 纹理类型
		         //tex_type[3]:Mask 表示该纹理是否使用alpha缓存区中的alpha来作为自己的透明度
				 //tex_type[2:1]:Matrix 2b'3' for ABCDEF/ 2b'0'for EF
				 //tex_type[0]:End 表示最后一张纹理
	SOURCE_BOX BoxSize;   //32bit = 4byte 包围盒
	TEXTURE_WH TexSize;   //32bit = 4byte 纹理宽高
	Matrix Matrix;        //192bit = 24byte 矩阵
	u32 Addr;             //32bit = 4byte 纹理地址
} NewSourceBuffer;   //total 38byte

typedef NewSourceBuffer*  NewSourceBufferPtr;

#endif

#ifdef VERSION_2

//  A  B  C
//  D  E  F
//  G  H  1
typedef struct _Matrix{
	int A;//1.11.20
	int B;
	int C;
	int D;
	int E;
	int F;
	int G; //1.18.13
	int H;
} Matrix; //32byte

typedef struct _POLCORDINATE{
	s16 y; //1.11.4
	s16 x; //1.11.4
} POLCORDINATE;//4byte

typedef struct _POLVERTEX{
	POLCORDINATE cor; //32bit
} POLVERTEX;//32bit = 4byte

typedef struct _VERTEX_SET{
	POLVERTEX v0;
	POLVERTEX v1;
	POLVERTEX v2;
	POLVERTEX v3;
} VERTEX_SET; //16byte

typedef struct _TEXTURE_WH{
	u16 height;
	u16 width;
} TEXTURE_WH;//4byte

typedef struct _SOURCE_BOX{
	u8 startx;
	u8 starty;
	u8 stopx;
	u8 stopy;
} SOURCE_BOX;//4byte

typedef struct _FISH_EYE_PARAM{
	u16 v_in_center;
	u16 u_in_center;
	u16 fv_recip;
	u16 fu_recip;
	u16 k1;
	u16 k2;
	u16 k3;
	u16 Reserved;
	u32 v_out_center;
	u32 u_out_center;
} FISH_EYE_PARAM;//24byte

typedef struct _NewSourceBuffer
{
	u32 tex_type;//16bit = 2byte 纹理控制信号，
	             //tex_type[15]:表示最后一张纹理
	             //tex_type[14]:仅在软件使用，表示需要绘制纹理
				 //tex_type[10:9]:colorblending模式
				 //tex_type[8]:直接overlap，bte默认overlap
				 //tex_type[7:4]:纹理类型
		         //tex_type[3]:第0bit表示是否使用缓存的alpha				
				 //tex_type[1:0]:mask_type，表示alpha纹理的混合模式
	VERTEX_SET TexVertex; //32bit *4 = 16byte      顶点的坐标
	Matrix mMatrix;        //256bit = 32byte 矩阵
	TEXTURE_WH TexSize;   //32bit = 4byte 纹理宽高
	u32 Addr;             //32bit = 4byte 纹理地址
	SOURCE_BOX BoxSize;   //32bit = 4byte 包围盒
	//FISH_EYE_PARAM fish_eye_param; //24byte
} NewSourceBuffer;   //total 88byte

typedef NewSourceBuffer*  NewSourceBufferPtr;

#endif

#endif

