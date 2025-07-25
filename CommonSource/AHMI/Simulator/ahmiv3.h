////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI小组成员
//
// Create Date:   2015/11/17
// File Name:     ahmiv3.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by Arthas 
// Additional Comments:
//    
// 
//////////////////////////////////
//////////////////////////////////////////////
#ifndef AHMIV3__H
#define AHMIV3__H

#include "AHMICfgDefinition.h"
#ifdef PC_SIM
#include "stdafx.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#endif
#include "publicType.h"
#include "publicDefine.h"
#include "ahmiv3_vd.h"
#include "AHMIDriver.h"
#define __PRINTDEBUG__ 0

#ifdef VERSION_2

typedef u8 StructFrameBuffer;

typedef StructFrameBuffer *FramBufferPtr;
#endif

#ifdef PC_SIM
extern"C"
{
extern struct globalArgs_t globalArgs;
}
extern u16 gScreenWidth;
extern u16 gScreenHeight;
extern u8           TextureBuffer[TEXBUFSIZE];

#ifdef VERSION_2

typedef u8 StructFrameBuffer;

typedef StructFrameBuffer *FramBufferPtr;

typedef struct _l1cachedata {
	bool init;
	u16 tag;
	u64	data[2];
	}l1cachedata;

typedef struct _l1cacherom {
	l1cachedata  l1_cache_data[512];
	}l1cacherom;

typedef struct _l2cachedata {
	bool init;
	u16 tag;			// 13位
	u64	data[2];
	}l2cachedata;

typedef struct _l2cacherom {
	l2cachedata  l2_cache_data[1024];
	}l2cacherom;

typedef struct _COLOR{
	u8 a;
	u8 r;
	u8 g;
	u8 b;

	bool m;
} COLOR;

typedef struct _CORDINATE{
	s16 y; //1.11.4
	s16 x; //1.11.4
	int u;
	int v;
} CORDINATE;//96bit

typedef struct _TILE_VERTEX{
	CORDINATE cor;
	COLOR c;
} TILE_VERTEX;

typedef struct _FinalTile {
	u16 x;
	u16 y;
	COLOR TileBuffer[TILESIZE* TILESIZE];
	Alpha AlphaBuffer[TILESIZE * TILESIZE];//for alpha
	} FinalTile;

typedef struct AHMI_Tile{
	u16 x;  //coordinate of left-bottom of the tile 
	u16 y;  //coordinate of left-bottom of the tile
	TILE_VERTEX ColorBuffer[TILESIZE * TILESIZE];//for color
	bool mask_usage;//是否使用mask和alpha
	//bool color_done;
	bool alpha_pixel;//输出为颜色还是纯alpha，1表示为颜色
	u8 mask_type;
	}Tile;

//Color16 FrameBuffer[ScreenWidth* ScreenHeight];



class ahmi{

	public:	
		//function name: ahmi
		//function description: constructor
		//@param : int screen_width // the width of screen
		//@param : int screen_height // the height of screen
		//output : null
		ahmi();
			//void DrawFrame(U64* TextureIndex ,ROMInfo* ,Color16* FrameBuffer);//绘制帧

		/**
        * @brief  start one draw-call
        * @param  null
        * @retval None
        */
		void DrawFrame();
		void SetAHMIRegs(AHMIDriverPtr pAHMIDriver); //set the regs for AHMI
	private:
		// void TileInfoRead(U64* TextureIndex);   //读取tileinfo并计算tile的x，y	
			void pixel_generate(Tile* ,u32 addr ,u16 tex_width,u16 tex_height,u16 texture_ctrl,u32 InitColor,VERTEX_SET* tex_vertex);
			void TexRaster(Tile* ,Matrix* ,u32 addr ,u16 tex_width,u16 tex_height,u16 texture_ctrl,u32 InitColor,VERTEX_SET* tex_vertex);//纹理光栅化
			void InitTile(Tile*,u16 x,u16 y);
			void cache(u32 block_addr, u16 in_block_addr, u32 addr, unsigned int* texel, u8 texture_ctrl);//纹理读取 modified by darydou
			void InitFinalTile(FinalTile*,u16 x,u16 y);
			void color_blending(FinalTile* ,Tile*,u16 texture_ctrl);//颜色混合
			void Write_Frame(FinalTile* ,FramBufferPtr FrameBuffer);//将tile最终颜色写入frame buffer	
			void Write_back_finaltile(FinalTile *finaltile, FramBufferPtr FrameBuffer);//add by darydou writebackframe to finaltile
			void uv_translate(Tile* tile_in,Matrix* matrix,u16 tex_type);
			void interpolation(Tile* tile_in,u16 tex_width,u16 tex_height,VERTEX_SET* tex_vertex,u32 AreaRcp,u8 tex_type);
			void bte(Tile* tile_in,VERTEX_SET* TexVertex,u16 tex_type);
			u32 screen_height;//屏幕高
			u32 screen_width;//屏幕宽
			int tile_x;
			int tile_y;
			FinalTile theFinalTile;//tile最终结果
			//u32 StartAddress;
			FramBufferPtr pFrameBuffer;
};

#endif


#endif


#endif

