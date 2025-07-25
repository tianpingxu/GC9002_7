////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI小组成员
//
// Create Date:   2015/11/17
// File Name:     ahmiv3.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by ...
// Revision 1.6.0 - change the way matrix compute 2017/10/15 by zuz
//                  change into the same with FPGA 2018/12/18 by zuz
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////


// ahmiv2.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include "ahmiv3.h"
#include "publicInclude.h"
#include <windows.h>
#include <iomanip>
#include <string>
#include <sstream>
#include "trace.h"
#include "AHMIDriver.h"
#include "simulatorParameters.h"
#include "stc.h"

#ifdef PC_SIM

extern u8		 stm32info[11];
extern u8       VideoTextureBuffer[VIDEO_BUF_SIZE];
extern u8       OscilloscopeArry[SIZE_OF_SingleOscilloscope_BUF * Chart_Num_MAX];
extern u8       QRCodeArry[QRcode_BUF_SIZE * QRCode_Num_MAX];
extern u8       DataImageDataArray[SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max];
extern u8       H264PicBuffer[SIZE_OF_SingleH264Pic_BUF];
extern u8       SnapshotBuffer[SIZE_OF_Snapshot_BUF];
extern u8  TempFrameBuffer[ MAX_SCREEN_SIZE];
extern u8  GlobalPageCacheBuffer[ PageCache_Size *2];  //global page cache bufffer for AHMI to write one frame data
extern u8  GlobalPageCacheBufferSub[ PageCache_Size *2];  //global page cache bufffer for AHMI to write one sub frame data


extern AHMIREGS AHMI_REGFILE;

#define SCREEN_ROTATE 0

u32 StartAddress=0;

extern u32 TileRenderCounter;
extern u32 FrameRate;
extern u8  sourceBuffer[SoureBufferSize];
extern u8  GlobalBackgroundBuffer[ MAX_SCREEN_SIZE *2 ]; //used for background texture

extern StructFrameBuffer  GlobalFrameBuffer2[ MAX_SCREEN_SIZE *2]; //used for double buffer


#define DXT5_DIV 3 //DXT5缩减的精度

#ifdef VERSION_2

#define L1_PREFETCH_LOG2 3
#define L2_PREFETCH_LOG2 6

u32 t_l1 = 67 * (1<<L1_PREFETCH_LOG2);
u32 t_l2 = 1000 + 67 * (1<<L2_PREFETCH_LOG2);

FILE *fp_uv_division;

FILE *fp_pge;
FILE *fp_cb;
FILE *fp_uv;

int       uv_count_test = 0;

u32 count1;
u32 count2;

u32 refresh1;
u32 refresh2;

l1cacherom	l1cache;
l2cacherom	l2cache;
u64 T;

u16 lut_rom[256];

int bte_counter;
int uv_counter;
int pge_counter;
int cb_counter;

#define  w_width  15
#define  int_width 31
#define  div_width 10
unsigned int R[1024];
int r_flag = 0;

#endif

//-----------------------------
// 函数名： ahmi::ahmi
// AHMI构造函数
// 参数列表：
// @param1 int width  屏幕宽度
// @param2 int height 屏幕高度
// 备注(各个版本之间的修改):
//   无
//-----------------------------
#ifdef VERSION_1
ahmi::ahmi()
{
	AHMI_REGFILE.STATUS=1;
	screen_height = globalArgs.ScreenHeigth;
	screen_width = globalArgs.ScreenWidth;
#ifdef TEST_LOG_GEN
	RemoveDirectory(_T("testLog"));
	CreateDirectory(_T("testLog"),NULL);
#ifdef TEX_RAS_LOG_GEN
	texRasFile.open("testLog\\texRas.log",std::ios::out);
#endif
#ifdef COLOR_BLENDING_LOG_GEN
	colorBlendingFile.open("testLog\\colorBlending.log",std::ios::out);
#endif
#ifdef TILE_BUFFER_LOG_GEN
	tileBufferFile.open("testLog\\tileBuffer.log",std::ios::out);
#endif
#ifdef UV_LOG_GEN
	uvLogFile.open("testLog\\uv.log",std::ios::out);
#endif
#ifdef FRAME_BUFFER_LOG_GEN
frameLogFile.open("testLog\\frame.log",std::ios::out);
frame_log_gen_flag = 0;
#endif

#endif
}
#endif

#ifdef VERSION_2

ahmi::ahmi()
{
	AHMI_REGFILE.STATUS=1;
	screen_height = globalArgs.ScreenHeigth;
	screen_width = globalArgs.ScreenWidth;
}

#endif

//-----------------------------
// 函数名： ahmi::InitTile
// 参数列表：
// @param1 Tile* tile 当前tile指针
// @param2 u16 x      横坐标
// @param3 u16 y      纵坐标
// 备注(各个版本之间的修改):
//   无
//-----------------------------
#ifdef VERSION_1
void ahmi::InitTile(Tile* tile,u16 x,u16 y)
{
	int i=0;
	tile->x=x;
	tile->y=y;
}
#endif

#ifdef VERSION_2
void ahmi::InitTile(Tile* tile,u16 x,u16 y)
{
	tile->x=x;
	tile->y=y;
	tile->mask_usage =0;
	tile->mask_type = 0;
	tile->alpha_pixel = 0;
	for(int i = 0;i < TILESIZE * TILESIZE;i++)
	{
		tile->ColorBuffer[i].cor.x = 0;
		tile->ColorBuffer[i].cor.y = 0;
		tile->ColorBuffer[i].cor.u = 0;
		tile->ColorBuffer[i].cor.v = 0;
		tile->ColorBuffer[i].c.r = 0;
		tile->ColorBuffer[i].c.g = 0;
		tile->ColorBuffer[i].c.b = 0;
		tile->ColorBuffer[i].c.a = 0;
		tile->ColorBuffer[i].c.m = 0;
	}
}
#endif

//-----------------------------
// 函数名： InitFinalTile
// 初始化finalTile
// 参数列表：
// @param1 FinalTile* final_tile
// @param2 u16 x      横坐标
// @param3 u16 y      纵坐标
// 备注(各个版本之间的修改):
//   无
//-----------------------------
#ifdef VERSION_1
void ahmi::InitFinalTile(FinalTile* final_tile,u16 x,u16 y)
{
	int i=0;
	final_tile->x=x;
	final_tile->y=y;
//	for (i = 0; i<TilePixelSize; i++)
//	{
//		final_tile->x=x;
//		final_tile->y=y;
//		final_tile->TileBuffer[i].r=0;
//		final_tile->TileBuffer[i].g=0;
//		final_tile->TileBuffer[i].b=0;
//	}
}
#endif

#ifdef VERSION_2
void ahmi::InitFinalTile(FinalTile* final_tile,u16 x,u16 y)
{
	int i=0;
	for (i = 0; i<TILESIZE* TILESIZE; i++)
	{
		final_tile->x=x;
		final_tile->y=y;
		final_tile->TileBuffer[i].r=0;
		final_tile->TileBuffer[i].g=0;
		final_tile->TileBuffer[i].b=0;
		final_tile->TileBuffer[i].a=0;
	}
}
#endif

//-----------------------------
// 函数名： color_blending
// 颜色混合
// 参数列表：
// @param1 FinalTile* final_tile
// @param2 Tile* tile
// 备注(各个版本之间的修改):
//   无
//-----------------------------
#ifdef VERSION_1
void ahmi::color_blending(FinalTile* final_tile,Tile* tile)
{
	int alpha;
	for(int i=0;i<TilePixelSize;i++)
	{
		if(tile->ColorBuffer[i].m)
		{
			if(tile->mask_usage) alpha = (tile->ColorBuffer[i].a*(tile->AlphaBuffer[i]+1))>>8;//纹理自带alpha与外部alpha混合
			else                 alpha = tile->ColorBuffer[i].a;

			final_tile->TileBuffer[i].r=(tile->ColorBuffer[i].r*alpha + final_tile->TileBuffer[i].r*(255-alpha)+128)/256;//四舍五入 modify by xt 20150401
			final_tile->TileBuffer[i].g=(tile->ColorBuffer[i].g*alpha + final_tile->TileBuffer[i].g*(255-alpha)+128)/256;
			final_tile->TileBuffer[i].b=(tile->ColorBuffer[i].b*alpha + final_tile->TileBuffer[i].b*(255-alpha)+128)/256;				
		}
	}

}
#endif

#ifdef VERSION_2

u16 alpha_rcp(u8 a)
{
	return(lut_rom[a]);
}

u8  color_mult_8x8(u8 color, u8 alpha)
{
	int temp;
	if(alpha == 255) temp = color;
	else
	{
		temp = color*alpha*257/256/256;
		if(temp >=128) temp = temp + 1;
		if(temp>255)
			temp = 255;
	}
	return (u8)temp;
}

void ahmi::color_blending(FinalTile* final_tile,Tile* tile_in,u16 texture_ctrl)
{
	for(int i = 0;i< 32* 32;i++)
	{
		if(tile_in->ColorBuffer[i].c.m)
		{
		u16 x,y,pixel_num;
		x = tile_in->ColorBuffer[i].cor.x/16;
		y = tile_in->ColorBuffer[i].cor.y/16;
		pixel_num = (y % 32) * 32 + x%32;
		if(tile_in->alpha_pixel)
		{
			COLOR c_new;
			COLOR c_back;
			COLOR c_final;
			c_new.r = tile_in->ColorBuffer[i].c.r;
			c_new.g = tile_in->ColorBuffer[i].c.g;
			c_new.b = tile_in->ColorBuffer[i].c.b;
			c_new.a = tile_in->ColorBuffer[i].c.a;
			if(tile_in->mask_usage)
			{
				c_new.a = (final_tile->AlphaBuffer[pixel_num] * c_new.a) * 257 /256/256 + ((((final_tile->AlphaBuffer[pixel_num] * c_new.a) * 257 /256) & 128) == 128);
			}
			c_back.r = final_tile->TileBuffer[pixel_num].r;
			c_back.g = final_tile->TileBuffer[pixel_num].g;
			c_back.b = final_tile->TileBuffer[pixel_num].b;
			c_back.a = final_tile->TileBuffer[pixel_num].a;

			if((texture_ctrl &0x100) == 0x100)
			{
				c_final.r = c_new.r;
				c_final.g = c_new.g;
				c_final.b = c_new.b;
				c_final.a = c_new.a;
				final_tile->TileBuffer[pixel_num].r = c_final.r;
				final_tile->TileBuffer[pixel_num].g = c_final.g;
				final_tile->TileBuffer[pixel_num].b = c_final.b;
				final_tile->TileBuffer[pixel_num].a = c_final.a;
			}
			else
			{
				u8 a_in = c_new.a;
				u8 a_back = c_back.a;
				u8 a_final = a_in + a_back - (a_in * a_back * 257 /256/256 + ((a_in * a_back * 257 /256& 128) == 128));
				c_final.a = a_final;
				u16 c_final_rcp = alpha_rcp(a_final);
				u8 c_final_rcp_int = (c_final_rcp & 127) + 128;
				u8 c_final_rcp_exp = (c_final_rcp & 0x380)/128;
				c_final.r = (c_final.a == 0) ? 0 : (((c_back.r * c_back.a * 257 /256/256 + (c_back.r * c_back.a * 257 /256/256!=0)) * (255- c_new.a) + c_new.r * (tile_in->mask_usage ? final_tile->AlphaBuffer[pixel_num]: 255))/256 * c_final_rcp_int) >> (7 - c_final_rcp_exp);
				c_final.g = (c_final.a == 0) ? 0 : (((c_back.g * c_back.a * 257 /256/256 + (c_back.g * c_back.a * 257 /256/256!=0)) * (255- c_new.a) + c_new.g * (tile_in->mask_usage ? final_tile->AlphaBuffer[pixel_num]: 255))/256 * c_final_rcp_int) >> (7 - c_final_rcp_exp);
				c_final.b = (c_final.a == 0) ? 0 : (((c_back.b * c_back.a * 257 /256/256 + (c_back.b * c_back.a * 257 /256/256!=0)) * (255- c_new.a) + c_new.b * (tile_in->mask_usage ? final_tile->AlphaBuffer[pixel_num]: 255))/256 * c_final_rcp_int) >> (7 - c_final_rcp_exp);
				u16 test_r = (c_final.a == 0) ? 0 :((color_mult_8x8(color_mult_8x8(c_back.r,c_back.a),255-c_new.a) + color_mult_8x8(c_new.r,c_new.a)) * c_final_rcp_int) >> (7 - c_final_rcp_exp);
				u16 test_g = (c_final.a == 0) ? 0 :((color_mult_8x8(color_mult_8x8(c_back.g,c_back.a),255-c_new.a) + color_mult_8x8(c_new.g,c_new.a)) * c_final_rcp_int) >> (7 - c_final_rcp_exp);
				u16 test_b = (c_final.a == 0) ? 0 :((color_mult_8x8(color_mult_8x8(c_back.b,c_back.a),255-c_new.a) + color_mult_8x8(c_new.b,c_new.a)) * c_final_rcp_int) >> (7 - c_final_rcp_exp);
				u8 test_2_r = (c_final.a == 0) ? 0 :(((c_back.r * c_back.a  /255 ) * (255- c_new.a) + c_new.r * c_new.a)/255 * c_final_rcp_int) >> (7 - c_final_rcp_exp);
				u8 test_2_g = (c_final.a == 0) ? 0 :(((c_back.g * c_back.a  /255 ) * (255- c_new.a) + c_new.g * c_new.a)/255 * c_final_rcp_int) >> (7 - c_final_rcp_exp);
				u8 test_2_b = (c_final.a == 0) ? 0 :(((c_back.b * c_back.a  /255 ) * (255- c_new.a) + c_new.b * c_new.a)/255 * c_final_rcp_int) >> (7 - c_final_rcp_exp);	
				
				final_tile->TileBuffer[pixel_num].r = c_final.r + (c_final.r !=0);
				final_tile->TileBuffer[pixel_num].g = c_final.g + (c_final.g !=0);
				final_tile->TileBuffer[pixel_num].b = c_final.b + (c_final.b !=0);
				final_tile->TileBuffer[pixel_num].a = c_final.a;
				if( (test_r != final_tile->TileBuffer[pixel_num].r) || (test_g != final_tile->TileBuffer[pixel_num].g) || (test_b != final_tile->TileBuffer[pixel_num].b) )
				{
					int test_error = 1;
				}
			}

		}
		else
		{
			switch(tile_in->mask_type)
			{
			case(0):
				final_tile->AlphaBuffer[pixel_num] = tile_in->ColorBuffer[i].c.a;
				break;

			case(1):
				final_tile->AlphaBuffer[pixel_num] = tile_in->ColorBuffer[i].c.a * final_tile->AlphaBuffer[pixel_num] * 257/256/256 + (((tile_in->ColorBuffer[i].c.a * final_tile->AlphaBuffer[pixel_num] * 257/256) & 128) == 128);

				break;

			case(2):
				final_tile->AlphaBuffer[pixel_num] = tile_in->ColorBuffer[i].c.a | final_tile->AlphaBuffer[pixel_num];
				break;

			case(3):
				final_tile->AlphaBuffer[pixel_num] = ~tile_in->ColorBuffer[i].c.a;
				break;

				default:
					break;
			}
		}
#ifdef TEST_LOG_GEN
		if(tile_in->ColorBuffer[i].c.m)	
		{
			if(tile_in->alpha_pixel)
				fprintf(fp_cb,"%03x %02x%02x%02x%02x\n",pixel_num,final_tile->TileBuffer[pixel_num].r,final_tile->TileBuffer[pixel_num].g,final_tile->TileBuffer[pixel_num].b,final_tile->TileBuffer[pixel_num].a);
			else
				fprintf(fp_cb,"%03x %02x\n",pixel_num,final_tile->AlphaBuffer[pixel_num]);
		}
#endif
		}
	}
}
#endif

//-----------------------------
// 函数名： ahmi::cache
// 读取纹理数据
// 参数列表：
// @param1 U64 block_addr 块地址
// @param2 u16 in_block_addr 块内地址
// @param3 unsigned int * texel 纹理块
// @param4 u8 TexType 纹理类型
// 备注(各个版本之间的修改):
//   无
//-----------------------------
#ifdef VERSION_1
void ahmi::cache(U64 block_addr,u16 in_block_addr,u32 addr,unsigned int * texel,u8 TexType)
{
	U64 tex_out;
	U64 alpha_out;
	Color color0,color1,color2,color3;
	u16 BColor1;
	u16 BColor2;
	u8 alpha;
	u8 alpha_temp;
	u8 alpha0;
	u8 alpha1;
	U64 index_temp;
	u8 index;

	if((addr & START_ADDR_OF_RAM) == 0) //指向spi flash地址
		tex_out = *(U64*)((u32)TextureBuffer +addr + block_addr*8);
	else //指向 sdram地址
	{
		if(addr >= START_ADDR_OF_RAM + 0x1a80000)
			tex_out = *(U64*)(GlobalBackgroundBuffer +(addr- (START_ADDR_OF_RAM + 0x1a80000)) + block_addr*8);
		else 
			tex_out =  *(U64*)(VideoTextureBuffer +(addr-START_ADDR_OF_RAM) + block_addr*8);;
	}

	switch (TexType)
	{
	case(SIM_DXT1):
		BColor1=(tex_out&0xFFFF);
		BColor2=(tex_out&0xFFFF0000)>>16;
		index_temp=(tex_out&0xFFFFFFFF00000000)>>32;
		index=(index_temp>>(2*in_block_addr))%4;
		color0.r=((BColor1&0xF800)>>8) + ( ((BColor1&0xE000)>>13) & 0x7    ); //color0.r= {Bcolor1[15:11], Bcolor1[15:13]}
		color0.g=((BColor1&0x07E0)>>3) + ( ((BColor1 & 0x600) >> 9) & 0x03 ); //color0.g= {Bcolor1[10:5], Bcolor1[10:9]}
		color0.b=((BColor1&0x001F)<<3) + ( ((BColor1&0x1C)>>2) & 0x07      ); //color0.b= {Bcolor1[4:0], Bcolor1[4:2]}
		color0.m=1;
		color1.r=((BColor2&0xF800)>>8) + ( ((BColor2&0xE000)>>13) & 0x7    ); //color1.r= {Bcolor2[15:11], Bcolor2[15:13]}
		color1.g=((BColor2&0x07E0)>>3) + ( ((BColor2&0x600) >> 9) & 0x03 ); //color1.g= {Bcolor2[10:5], Bcolor2[10:9]}
		color1.b=((BColor2&0x001F)<<3) + ( ((BColor2&0x1C)>>2) & 0x07      ); //color1.b= {Bcolor2[4:0], Bcolor2[4:2]}
		color1.m=1;
		color2.r = ( color0.r * 3 + color1.r * 1 ) / 4;
        color2.g = ( color0.g * 3 + color1.g * 1 ) / 4;
        color2.b = ( color0.b * 3 + color1.b * 1 ) / 4;
		color2.m = 1;
        color3.r = ( color0.r*1 + color1.r * 3 ) / 4;
        color3.g = ( color0.g*1 + color1.g * 3 ) / 4;
        color3.b = ( color0.b*1 + color1.b * 3 ) / 4;
		color3.m = 1;
		switch (index)
			{
			case 0:
				*texel=((color0.r)<<16)+((color0.g)<<8)+color0.b;
				break;					
			case 1:						
				*texel=((color1.r)<<16)+((color1.g)<<8)+color1.b;
				break;					
			case 2:						
				*texel=((color2.r)<<16)+((color2.g)<<8)+color2.b;
				break;	
			case 3:
				*texel=((color3.r)<<16)+((color3.g)<<8)+color3.b;
				break;
			}
	break;
	case(SIM_DXT3):
			alpha_out=tex_out;
			tex_out = *(U64*)((u32)TextureBuffer +addr + block_addr*8 + 8);
			BColor1=(tex_out&0xFFFF);
			BColor2=(tex_out&0xFFFF0000)>>16;
			index_temp=(tex_out&0xFFFFFFFF00000000)>>32;
			index=(index_temp>>(2*in_block_addr))%4;
			color0.r=((BColor1&0xF800)>>8) + ((BColor1 & 0xE000) >> 13);
			color0.g=((BColor1&0x07E0)>>3) + ((BColor1 & 0x0600)>>9)   ;
			color0.b=((BColor1&0x001F)<<3) + ((BColor1&0x001C)>>2)     ;
			color0.m=1;
			color1.r=((BColor2&0xF800)>>8) + ((BColor2 & 0xE000) >> 13);
			color1.g=((BColor2&0x07E0)>>3) + ((BColor2 & 0x0600) >> 9) ;
			color1.b=((BColor2&0x001F)<<3) + ((BColor2 & 0x001C )>> 2) ;
			color1.m=1;
			color2.r = ( color0.r * 3 + color1.r * 1 ) / 4;
			color2.g = ( color0.g * 3 + color1.g * 1 ) / 4;
			color2.b = ( color0.b * 3 + color1.b * 1 ) / 4;
			color2.m = 1;
			color3.r = ( color0.r*1 + color1.r * 3 ) / 4;
			color3.g = ( color0.g*1 + color1.g * 3 ) / 4;
			color3.b = ( color0.b*1 + color1.b * 3 ) / 4;
			color3.m = 1;
			alpha = (alpha_out>>(4*in_block_addr))%16;
			alpha = alpha*16 + alpha;
			switch (index)
				{
				case 0:
					*texel=((color0.r)<<24)+((color0.g)<<16)+(color0.b<<8)+alpha;
					break;					
				case 1:						
					*texel=((color1.r)<<24)+((color1.g)<<16)+(color1.b<<8)+alpha;
					break;					
				case 2:						
					*texel=((color2.r)<<24)+((color2.g)<<16)+(color2.b<<8)+alpha;
					break;				
				case 3:					
					*texel=((color3.r)<<24)+((color3.g)<<16)+(color3.b<<8)+alpha;

				}

	break;

	case(SIM_DXT5):
		alpha_out=tex_out;
		alpha0 = alpha_out & 0xFF;          //第一字节，alpha0
		alpha1 = (alpha_out & 0xFF00) >> 8; //第二字节，alpha1
		index_temp=(alpha_out&0xFFFFFFFFFFFF0000)>>16; //3-8 index
		index=(index_temp>>(3*in_block_addr))%8;
		if(alpha0 > alpha1) //模拟用256精度来计算
		{
			switch(index)
			{
			case(0):
				alpha = alpha0;
				alpha_temp = alpha0;
				break;
			case(1):
				alpha = alpha1;
				alpha_temp = alpha1;
				break;
			case(2):
				alpha = (( (219 >> DXT5_DIV) * (alpha0 >> DXT5_DIV) ) + ( (37 >> DXT5_DIV) * (alpha1 >> DXT5_DIV) )) >> (8 - 2*DXT5_DIV); //alpha_2 = 6/7*alpha_0 + 1/7*alpha_1;
				alpha_temp = ((6 * alpha0) /7) + ( (1*alpha1) / 7 );
				break;
			case(3):
				alpha = (( (183 >> DXT5_DIV) * (alpha0>>DXT5_DIV) ) + ( (73>>DXT5_DIV) * (alpha1>>DXT5_DIV) )) >> (8 - 2*DXT5_DIV) ; //alpha_3 = 5/7*alpha_0 + 2/7*alpha_1;
				alpha_temp = ((5 * alpha0) /7) + ( (2*alpha1) / 7 );
				break;
			case(4):
				alpha = (( (146>>DXT5_DIV) * (alpha0>>DXT5_DIV) ) + ( (110>>DXT5_DIV) * (alpha1>>DXT5_DIV) )) >> (8 - 2*DXT5_DIV) ; //alpha_4 = 4/7*alpha_0 + 3/7*alpha_1;
				alpha_temp = ((4 * alpha0) /7) + ( (3*alpha1) / 7 );
				break;
			case(5):
				alpha = (( (110 >>DXT5_DIV) * (alpha0>>DXT5_DIV) ) + ( (146>>DXT5_DIV)*(alpha1>>DXT5_DIV) )) >> (8 - 2*DXT5_DIV); //alpha_5 = 3/7*alpha_0 + 4/7*alpha_1;
				alpha_temp = ((3 * alpha0) /7) + ( (4*alpha1) / 7 );
				break;
			case(6):
				alpha = (( (73>>DXT5_DIV) * (alpha0>>DXT5_DIV)) + ((183>>DXT5_DIV)*(alpha1>>DXT5_DIV) )) >> (8 - 2*DXT5_DIV) ; //alpha_6 = 2/7*alpha_0 + 5/7*alpha_1;
				alpha_temp = ((2 * alpha0) /7) + ( (5*alpha1) / 7 );
				break;
			case(7):
				alpha = (( (37>>DXT5_DIV) * (alpha0>>DXT5_DIV) ) + ( (219>>DXT5_DIV)*(alpha1>>DXT5_DIV) ) ) >> (8 - 2*DXT5_DIV) ; //alpha_6 = 2/7*alpha_0 + 5/7*alpha_1;
				alpha_temp = ((1 * alpha0) /7) + ( (6*alpha1) / 7 );
				break;
			default:
				alpha = 0xff;
				alpha_temp = 0xff;
				break;
			}
		}
		else 
		{
			switch(index)
			{
			case(0):
				alpha = alpha0;
				alpha_temp = alpha0;
				break;
			case(1):
				alpha = alpha1;
				alpha_temp = alpha1;
				break;
			case(2):
				alpha = ( (( (205>>DXT5_DIV) * (alpha0>>DXT5_DIV) ) + ((51>>DXT5_DIV)*(alpha1>>DXT5_DIV) ) )>> (8 - 2*DXT5_DIV) ); //alpha_2 = 4/5*alpha_0 + 1/5*alpha_1;
				alpha_temp = ((4 * alpha0) /5) + ( (1*alpha1) / 5 );
				break;
			case(3):
				alpha = ( (( (154>>DXT5_DIV) * (alpha0>>DXT5_DIV)) + ((102>>DXT5_DIV)*(alpha1>>DXT5_DIV)) ) >> (8 - 2*DXT5_DIV) ); //alpha_3 = 3/5*alpha_0 + 2/5*alpha_1;
				alpha_temp = ((3 * alpha0) /5) + ( (2*alpha1) / 5 );
				break;
			case(4):
				alpha = ( (( (102>>DXT5_DIV) * (alpha0>>DXT5_DIV) ) + ( (154>>DXT5_DIV) * (alpha1>>DXT5_DIV) )) >> (8 - 2*DXT5_DIV) ); //alpha_4 = 2/5*alpha_0 + 3/5*alpha_1;
				alpha_temp = ((2 * alpha0) /5) + ( (3*alpha1) / 5 );
				break;
			case(5):
				alpha = ( (( (51>>DXT5_DIV) * (alpha0>>DXT5_DIV) ) +  ( (205>>DXT5_DIV) * (alpha1>>DXT5_DIV) )) >> (8 - 2*DXT5_DIV) ); //alpha_5 = 1/5*alpha_0 + 4/5*alpha_1;
				alpha_temp = ((1 * alpha0) /5) + ( (4*alpha1) / 5 );
				break;
			case(6):
				alpha = 0; 
				alpha_temp = 0;
				break;
			case(7):
				alpha = 255; 
				alpha_temp = 255;
				break;
			default:
				alpha = 0xff;
				alpha_temp = 255;
			}
		}

		tex_out = *(U64*)((u32)TextureBuffer +addr + block_addr*8 + 8);
		BColor1=(tex_out&0xFFFF);
		BColor2=(tex_out&0xFFFF0000)>>16;
		index_temp=(tex_out&0xFFFFFFFF00000000)>>32;
		index=(index_temp>>(2*in_block_addr))%4;
		color0.r= ((BColor1&0xF800)>>8) + ((BColor1 & 0xE000) >> 13);
		color0.g= ((BColor1&0x07E0)>>3) + ((BColor1 & 0x0600)>>9);
		color0.b= ((BColor1&0x001F)<<3) + ((BColor1&0x001C)>>2) ;
		color0.m=1;
		color1.r=((BColor2&0xF800)>>8) + ((BColor2 & 0xE000) >> 13);
		color1.g=((BColor2&0x07E0)>>3) + ((BColor2 & 0x0600) >> 9) ;
		color1.b=((BColor2&0x001F)<<3) + ((BColor2 & 0x001C )>> 2) ;
		color1.m=1;
		color2.r = ( color0.r * 3 + color1.r * 1 ) / 4;
		color2.g = ( color0.g * 3 + color1.g * 1 ) / 4;
		color2.b = ( color0.b * 3 + color1.b * 1 ) / 4;
		color2.m = 1;
		color3.r = ( color0.r*1 + color1.r * 3 ) / 4;
		color3.g = ( color0.g*1 + color1.g * 3 ) / 4;
		color3.b = ( color0.b*1 + color1.b * 3 ) / 4;
		color3.m = 1;
		switch (index)
		{
		case 0:
			*texel=((color0.r)<<24)+((color0.g)<<16)+(color0.b<<8)+alpha;
			break;					
		case 1:						
			*texel=((color1.r)<<24)+((color1.g)<<16)+(color1.b<<8)+alpha;
			break;					
		case 2:						
			*texel=((color2.r)<<24)+((color2.g)<<16)+(color2.b<<8)+alpha;
			break;				
		case 3:					
			*texel=((color3.r)<<24)+((color3.g)<<16)+(color3.b<<8)+alpha;

		}
	break;

	case(SIM_ALPHA8):
			*texel=(tex_out>>(in_block_addr*8))%256;

	break;

	case(SIM_RGB565):
		*texel = (tex_out >> ( in_block_addr * 16))%65536;
	break;

	case(SIM_RGBA8888):
		*texel = (tex_out >> (/*32 - */in_block_addr * 32)) & 0xffffffff;
	break;

	case(SIM_Y8):
		*texel=(tex_out>>(in_block_addr*8))%256;
	break;

	case(SIM_ALPHA1):
		*texel=(tex_out>>(in_block_addr))%2 ? 255 : 0;
	break;

	case(SIM_ALPHA2):
		*texel= ((tex_out>>(in_block_addr*2))%4) * 255 / 3;
		break;
	
	case(SIM_ALPHA4):
		*texel= ((tex_out>>(in_block_addr*4))%16) * 255 / 15;
		break;

	default:
		break;
	}
}
#endif

#ifdef VERSION_2

bool read_l1(u32 addr)
{
	u16 cache_addr = (addr & 0x1ff0) >> 4; 
	u16 read_tag = l1cache.l1_cache_data[cache_addr].tag;
	bool init = l1cache.l1_cache_data[cache_addr].init;
	u16 head = (addr >> 13) & 0x7fff;
	bool hit = init && ((read_tag & 0x7fff) == head);
	return hit;
}

bool read_l2(u32 addr)
{
	u16 cache_addr = (addr &0x3ff0) >> 4; 
	u16 read_tag = l2cache.l2_cache_data[cache_addr].tag;
	bool init = l2cache.l2_cache_data[cache_addr].init;
	u16 head = (addr >>14) & 0x3fff;
	bool hit = init && ((read_tag & 0x7fff) == head);
	return hit;
}

void writel1(u32 addr)
{
	u16 l1_cache_addr = (addr & 0x1ff0) >> 4; 
	u16 l2_cache_addr = (addr &0x3ff0) >> 4; 
	l1_cache_addr = l1_cache_addr >> L1_PREFETCH_LOG2;
	l1_cache_addr = l1_cache_addr << L1_PREFETCH_LOG2;
	u32 x = (((u32)1) << L1_PREFETCH_LOG2);
	for(u32 i = 0;i < x;i++)
	{
		if(l1cache.l1_cache_data[l1_cache_addr + i].init)
		{
			refresh1 = refresh1 + 1;
		}
		l1cache.l1_cache_data[l1_cache_addr + i].tag = addr >> 13;
		l1cache.l1_cache_data[l1_cache_addr + i].init = 1; 
	}
}

void writel2(u32 addr)
{
	u16 l2_cache_addr = (addr &0x3ff0) >> 4; 
	l2_cache_addr = l2_cache_addr >> L2_PREFETCH_LOG2;
	l2_cache_addr = l2_cache_addr << L2_PREFETCH_LOG2;
	u32 x = (((u32)1) << L2_PREFETCH_LOG2);
	for(u32 i = 0;i < x;i++)
	{
		if(l2cache.l2_cache_data[l2_cache_addr + i].init)
		{
			refresh2 = refresh2 + 1;
		}
		l2cache.l2_cache_data[l2_cache_addr + i].tag = addr >> 14;
		l2cache.l2_cache_data[l2_cache_addr + i].init = 1;
	}
}

extern "C" extern int embedded_platform;
void ahmi::cache(u32 block_addr,u16 in_block_addr,u32 addr,unsigned int * texel,u8 TexType)
{
	U64 tex_out;
	U64 alpha_out;
	COLOR color0,color1,color2,color3;
	u16 BColor1;
	u16 BColor2;
	u8 alpha;
	u8 alpha0;
	u8 alpha1;
	u8 alpha_temp; 
	u64 index_temp;
	u8 index;
	u8 x = in_block_addr%4;
	u128 block_data;
	u8 y = in_block_addr/4;
	if((block_addr & START_ADDR_OF_RAM) == 0) //指向spi flash地址
	{
		tex_out = *(U64*)((u32)TextureBuffer + addr + block_addr);
	}
	else if((block_addr & START_ADDR_OF_RAM) == START_ADDR_OF_RAM )//指向 sdram地址
	{
		if(((TexType << 4) == RGB565 || (TexType << 4) == RGB565_RE || (TexType << 4) == RGBA8888_RE))
		{
			tex_out = *(U64*)((u32)VideoTextureBuffer + (block_addr-START_ADDR_OF_RAM) + addr);
		}
		else
		{
			if((block_addr-START_ADDR_OF_RAM) >= VIDEO_BUF_SIZE && (block_addr-START_ADDR_OF_RAM) < (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10))
			{
				tex_out = *(U64*)((u32)QRCodeArry + (block_addr - START_ADDR_OF_RAM - VIDEO_BUF_SIZE) + addr);
			}
			else if((block_addr-START_ADDR_OF_RAM) >= (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10) && (block_addr-START_ADDR_OF_RAM) < (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10 + Oscilloscope_BUF_SIZE*10 ))
			{
				tex_out = *(U64*)((u32)OscilloscopeArry + (block_addr - START_ADDR_OF_RAM - VIDEO_BUF_SIZE - QRcode_BUF_SIZE * 10) + addr);
			}
			else if((block_addr-START_ADDR_OF_RAM) >= (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10 + Oscilloscope_BUF_SIZE * 10) && 
				    (block_addr-START_ADDR_OF_RAM) < (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10 + Oscilloscope_BUF_SIZE * 10 + SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max))
			{
				tex_out = *(U64*)((u32)DataImageDataArray + (block_addr - START_ADDR_OF_RAM - VIDEO_BUF_SIZE - QRcode_BUF_SIZE * 10 - Oscilloscope_BUF_SIZE*10 ) + addr);
			}
			else if((block_addr-START_ADDR_OF_RAM) >= (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10 + Oscilloscope_BUF_SIZE * 10 + SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max) && 
				    (block_addr-START_ADDR_OF_RAM) < (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10 + Oscilloscope_BUF_SIZE * 10 + SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max + SIZE_OF_SingleH264Pic_BUF))
			{
				tex_out = *(U64*)((u32)H264PicBuffer + (block_addr - START_ADDR_OF_RAM - VIDEO_BUF_SIZE - QRcode_BUF_SIZE * 10 - Oscilloscope_BUF_SIZE*10 - SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max) + addr);
			}
			else if((block_addr-START_ADDR_OF_RAM) >= (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10 + Oscilloscope_BUF_SIZE * 10 + SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max + SIZE_OF_SingleH264Pic_BUF) && 
				    (block_addr-START_ADDR_OF_RAM) < (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10 + Oscilloscope_BUF_SIZE * 10 + SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max + SIZE_OF_SingleH264Pic_BUF + SIZE_OF_Snapshot_BUF))
			{
				tex_out = *(U64*)((u32)SnapshotBuffer + (block_addr - START_ADDR_OF_RAM - VIDEO_BUF_SIZE - QRcode_BUF_SIZE * 10 - Oscilloscope_BUF_SIZE*10 - SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max - SIZE_OF_SingleH264Pic_BUF) + addr);
			}
			else if((block_addr-START_ADDR_OF_RAM) >= (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10 + Oscilloscope_BUF_SIZE * 10 + SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max + SIZE_OF_SingleH264Pic_BUF + SIZE_OF_Snapshot_BUF) && 
				    (block_addr-START_ADDR_OF_RAM) < (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10 + Oscilloscope_BUF_SIZE * 10 + SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max + SIZE_OF_SingleH264Pic_BUF + SIZE_OF_Snapshot_BUF + PageCache_Size *2))
			{
				tex_out = *(U64*)((u32)GlobalPageCacheBuffer + (block_addr - START_ADDR_OF_RAM - VIDEO_BUF_SIZE - QRcode_BUF_SIZE * 10 - Oscilloscope_BUF_SIZE*10 - SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max - SIZE_OF_SingleH264Pic_BUF - SIZE_OF_Snapshot_BUF) + addr);
			}
			else if((block_addr-START_ADDR_OF_RAM) >= (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10 + Oscilloscope_BUF_SIZE * 10 + SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max + SIZE_OF_SingleH264Pic_BUF + SIZE_OF_Snapshot_BUF + PageCache_Size *2) && 
				    (block_addr-START_ADDR_OF_RAM) < (VIDEO_BUF_SIZE + QRcode_BUF_SIZE * 10 + Oscilloscope_BUF_SIZE * 10 + SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max + SIZE_OF_SingleH264Pic_BUF + SIZE_OF_Snapshot_BUF + PageCache_Size *2 + PageCache_Size *2))
			{
				tex_out = *(U64*)((u32)GlobalPageCacheBufferSub + (block_addr - START_ADDR_OF_RAM - VIDEO_BUF_SIZE - QRcode_BUF_SIZE * 10 - Oscilloscope_BUF_SIZE*10 - SIZE_OF_SingleDataImage_BUF * DataImage_Num_Max - SIZE_OF_SingleH264Pic_BUF - SIZE_OF_Snapshot_BUF - PageCache_Size *2) + addr);
			}
		}
	}

	if(read_l1(block_addr))
	{	
	}
	else
	{
		count1 = count1 + 1;
		if (read_l2(block_addr))
		{
			writel1(block_addr);
		}
		else
		{
			count2 = count2 + 1;
			writel2(block_addr);
			writel1(block_addr);
		}
	}

	if(in_block_addr == 0x400)
	{
		*texel = 0;
	}
	else
	{
	switch (TexType)
	{
	case(SIM_DXT1):
		if(embedded_platform == EMBEDDED_GC9003)
		{
			BColor1=(tex_out&0xFFFF);
			BColor2=(tex_out&0xFFFF0000)>>16;
			index_temp=(tex_out&0xFFFFFFFF00000000)>>32;
			index=(index_temp>>(2*in_block_addr))%4;
			color0.r=((BColor1&0xF800)>>8) + ((BColor1&0xE000) >> 13);
			color0.g=((BColor1&0x07E0)>>3) + ((BColor1&0x600) >> 9);
			color0.b=((BColor1&0x001F)<<3) + ((BColor1&0x1C)>>2);
			color0.a=255;
			color1.r=((BColor2&0xF800)>>8) + ((BColor2&0xE000) >> 13);
			color1.g=((BColor2&0x07E0)>>3) + ((BColor2&0x600) >> 9);
			color1.b=((BColor2&0x001F)<<3) + ((BColor2&0x1C)>>2);
			color1.a=255;
			color2.r = ( color0.r * 3 + color1.r * 1 ) / 4;
			color2.g = ( color0.g * 3 + color1.g * 1 ) / 4;
			color2.b = ( color0.b * 3 + color1.b * 1 ) / 4;
			color2.a = 255;
			color3.r = ( color0.r*1 + color1.r * 3 ) / 4;
			color3.g = ( color0.g*1 + color1.g * 3 ) / 4;
			color3.b = ( color0.b*1 + color1.b * 3 ) / 4;
			color3.a = 255;
			switch (index)
				{
				case 0:
					*texel=((color0.r)<<16)+((color0.g)<<8)+color0.b;
					break;					
				case 1:						
					*texel=((color1.r)<<16)+((color1.g)<<8)+color1.b;
					break;					
				case 2:						
					*texel=((color2.r)<<16)+((color2.g)<<8)+color2.b;
					break;	
				case 3:
					*texel=((color3.r)<<16)+((color3.g)<<8)+color3.b;
					break;
				}
		}
		else
		{
			alpha_out=tex_out;
			tex_out = *(U64*)((u32)TextureBuffer +addr + block_addr + 8);
			block_data = alpha_out;
			block_data |= tex_out;
		    stc_rgb_dec(x,y,block_data,texel);
		}
	break;
	case(SIM_DXT3):
		if(embedded_platform == EMBEDDED_GC9003)
		{
			alpha_out=tex_out;
			tex_out = *(U64*)((u32)TextureBuffer +addr + block_addr + 8);
			BColor1=(tex_out&0xFFFF);
			BColor2=(tex_out&0xFFFF0000)>>16;
			index_temp=(tex_out&0xFFFFFFFF00000000)>>32;
			index=(index_temp>>(2*in_block_addr))%4;
			color0.r=((BColor1&0xF800)>>8) + ((BColor1&0xE000) >> 13);
			color0.g=((BColor1&0x07E0)>>3) + ((BColor1&0x600) >> 9);
			color0.b=((BColor1&0x001F)<<3) + ((BColor1&0x1C)>>2);
			color0.a=255;
			color1.r=((BColor2&0xF800)>>8) + ((BColor2&0xE000) >> 13);
			color1.g=((BColor2&0x07E0)>>3) + ((BColor2&0x600) >> 9);
			color1.b=((BColor2&0x001F)<<3) + ((BColor2&0x1C)>>2);
			color1.a=255;
			color2.r = ( color0.r * 3 + color1.r * 1 ) / 4;
			color2.g = ( color0.g * 3 + color1.g * 1 ) / 4;
			color2.b = ( color0.b * 3 + color1.b * 1 ) / 4;
			color2.a = 255;
			color3.r = ( color0.r*1 + color1.r * 3 ) / 4;
			color3.g = ( color0.g*1 + color1.g * 3 ) / 4;
			color3.b = ( color0.b*1 + color1.b * 3 ) / 4;
			color3.a = 255;
			alpha = (alpha_out>>(4*in_block_addr))%16;
			alpha = alpha*16 + alpha;
			switch (index)
				{
				case 0:
					*texel=((color0.r)<<16)+((color0.g)<<8)+(color0.b)+(alpha<<24);
					break;					
				case 1:						
					*texel=((color1.r)<<16)+((color1.g)<<8)+(color1.b)+(alpha<<24);
					break;					
				case 2:						
					*texel=((color2.r)<<16)+((color2.g)<<8)+(color2.b)+(alpha<<24);
					break;				
				case 3:					
					*texel=((color3.r)<<16)+((color3.g)<<8)+(color3.b)+(alpha<<24);

				}
		}
		else
		{
			alpha_out=tex_out;
			tex_out = *(U64*)((u32)TextureBuffer +addr + block_addr + 8);
			block_data = alpha_out;
			block_data |= tex_out;
		    stc_rgba_dec(x,y,block_data,texel);
		}
	break;

	case(SIM_DXT5):
		if(embedded_platform == EMBEDDED_GC9003)
		{
			alpha_out=tex_out;
			alpha0 = alpha_out & 0xFF;          //第一字节，alpha0
			alpha1 = (alpha_out & 0xFF00) >> 8; //第二字节，alpha1
			index_temp=(alpha_out&0xFFFFFFFFFFFF0000)>>16; //3-8 index
			index=(index_temp>>(3*in_block_addr))%8;
			if(alpha0 > alpha1) //模拟用256精度来计算
			{
				switch(index)
				{
				case(0):
					alpha = alpha0;
					alpha_temp = alpha0;
					break;
				case(1):
					alpha = alpha1;
					alpha_temp = alpha1;
					break;
				case(2):
					alpha = (( (219 >> DXT5_DIV) * (alpha0 >> DXT5_DIV) ) + ( (37 >> DXT5_DIV) * (alpha1 >> DXT5_DIV) )) >> (8 - 2*DXT5_DIV); //alpha_2 = 6/7*alpha_0 + 1/7*alpha_1;
					alpha_temp = ((6 * alpha0) /7) + ( (1*alpha1) / 7 );
					break;
				case(3):
					alpha = (( (183 >> DXT5_DIV) * (alpha0>>DXT5_DIV) ) + ( (73>>DXT5_DIV) * (alpha1>>DXT5_DIV) )) >> (8 - 2*DXT5_DIV) ; //alpha_3 = 5/7*alpha_0 + 2/7*alpha_1;
					alpha_temp = ((5 * alpha0) /7) + ( (2*alpha1) / 7 );
					break;
				case(4):
					alpha = (( (146>>DXT5_DIV) * (alpha0>>DXT5_DIV) ) + ( (110>>DXT5_DIV) * (alpha1>>DXT5_DIV) )) >> (8 - 2*DXT5_DIV) ; //alpha_4 = 4/7*alpha_0 + 3/7*alpha_1;
					alpha_temp = ((4 * alpha0) /7) + ( (3*alpha1) / 7 );
					break;
				case(5):
					alpha = (( (110 >>DXT5_DIV) * (alpha0>>DXT5_DIV) ) + ( (146>>DXT5_DIV)*(alpha1>>DXT5_DIV) )) >> (8 - 2*DXT5_DIV); //alpha_5 = 3/7*alpha_0 + 4/7*alpha_1;
					alpha_temp = ((3 * alpha0) /7) + ( (4*alpha1) / 7 );
					break;
				case(6):
					alpha = (( (73>>DXT5_DIV) * (alpha0>>DXT5_DIV)) + ((183>>DXT5_DIV)*(alpha1>>DXT5_DIV) )) >> (8 - 2*DXT5_DIV) ; //alpha_6 = 2/7*alpha_0 + 5/7*alpha_1;
					alpha_temp = ((2 * alpha0) /7) + ( (5*alpha1) / 7 );
					break;
				case(7):
					alpha = (( (37>>DXT5_DIV) * (alpha0>>DXT5_DIV) ) + ( (219>>DXT5_DIV)*(alpha1>>DXT5_DIV) ) ) >> (8 - 2*DXT5_DIV) ; //alpha_6 = 2/7*alpha_0 + 5/7*alpha_1;
					alpha_temp = ((1 * alpha0) /7) + ( (6*alpha1) / 7 );
					break;
				default:
					alpha = 0xff;
					alpha_temp = 0xff;
					break;
				}
			}
			else 
			{
				switch(index)
				{
				case(0):
					alpha = alpha0;
					alpha_temp = alpha0;
					break;
				case(1):
					alpha = alpha1;
					alpha_temp = alpha1;
					break;
				case(2):
					alpha = ( (( (205>>DXT5_DIV) * (alpha0>>DXT5_DIV) ) + ((51>>DXT5_DIV)*(alpha1>>DXT5_DIV) ) )>> (8 - 2*DXT5_DIV) ); //alpha_2 = 4/5*alpha_0 + 1/5*alpha_1;
					alpha_temp = ((4 * alpha0) /5) + ( (1*alpha1) / 5 );
					break;
				case(3):
					alpha = ( (( (154>>DXT5_DIV) * (alpha0>>DXT5_DIV)) + ((102>>DXT5_DIV)*(alpha1>>DXT5_DIV)) ) >> (8 - 2*DXT5_DIV) ); //alpha_3 = 3/5*alpha_0 + 2/5*alpha_1;
					alpha_temp = ((3 * alpha0) /5) + ( (2*alpha1) / 5 );
					break;
				case(4):
					alpha = ( (( (102>>DXT5_DIV) * (alpha0>>DXT5_DIV) ) + ( (154>>DXT5_DIV) * (alpha1>>DXT5_DIV) )) >> (8 - 2*DXT5_DIV) ); //alpha_4 = 2/5*alpha_0 + 3/5*alpha_1;
					alpha_temp = ((2 * alpha0) /5) + ( (3*alpha1) / 5 );
					break;
				case(5):
					alpha = ( (( (51>>DXT5_DIV) * (alpha0>>DXT5_DIV) ) +  ( (205>>DXT5_DIV) * (alpha1>>DXT5_DIV) )) >> (8 - 2*DXT5_DIV) ); //alpha_5 = 1/5*alpha_0 + 4/5*alpha_1;
					alpha_temp = ((1 * alpha0) /5) + ( (4*alpha1) / 5 );
					break;
				case(6):
					alpha = 0; 
					alpha_temp = 0;
					break;
				case(7):
					alpha = 255; 
					alpha_temp = 255;
					break;
				default:
					alpha = 0xff;
					alpha_temp = 255;
				}
			}



			tex_out = *(U64*)((u32)TextureBuffer +addr + block_addr + 8);
			BColor1=(tex_out&0xFFFF);
			BColor2=(tex_out&0xFFFF0000)>>16;
			index_temp=(tex_out&0xFFFFFFFF00000000)>>32;
			index=(index_temp>>(2*in_block_addr))%4;
			color0.r=((BColor1&0xF800)>>8) + ((BColor1&0xE000) >> 13);
			color0.g=((BColor1&0x07E0)>>3) + ((BColor1&0x600) >> 9);
			color0.b=((BColor1&0x001F)<<3) + ((BColor1&0x1C)>>2);
			color0.a=255;
			color1.r=((BColor2&0xF800)>>8) + ((BColor2&0xE000) >> 13);
			color1.g=((BColor2&0x07E0)>>3) + ((BColor2&0x600) >> 9);
			color1.b=((BColor2&0x001F)<<3) + ((BColor2&0x1C)>>2);
			color1.a=255;
			color2.r = ( color0.r * 3 + color1.r * 1 ) / 4;
			color2.g = ( color0.g * 3 + color1.g * 1 ) / 4;
			color2.b = ( color0.b * 3 + color1.b * 1 ) / 4;
			color2.a = 255;
			color3.r = ( color0.r*1 + color1.r * 3 ) / 4;
			color3.g = ( color0.g*1 + color1.g * 3 ) / 4;
			color3.b = ( color0.b*1 + color1.b * 3 ) / 4;
			color3.a = 255;
			switch (index)
					{
					case 0:
						*texel=((color0.r)<<16)+((color0.g)<<8)+(color0.b)+(alpha<<24);
						break;					
					case 1:						
						*texel=((color1.r)<<16)+((color1.g)<<8)+(color1.b)+(alpha<<24);
						break;					
					case 2:						
						*texel=((color2.r)<<16)+((color2.g)<<8)+(color2.b)+(alpha<<24);
						break;				
					case 3:					
						*texel=((color3.r)<<16)+((color3.g)<<8)+(color3.b)+(alpha<<24);

					}
		}
		else
		{
			alpha_out=tex_out;
			tex_out = *(U64*)((u32)TextureBuffer +addr + block_addr + 8);
			block_data = alpha_out;
			block_data |= tex_out;
		    //sfbc_dec(x,y,block_data,texel);
		}
	break;

	case(SIM_ALPHA8):
			*texel=(tex_out>>(in_block_addr))%256;

	break;

	case(SIM_ALPHA4):
			*texel=((tex_out>>(in_block_addr))%16) * 255 / 15;

	break;

	case(SIM_ALPHA2):
			*texel=((tex_out>>(in_block_addr))%4) * 255 / 3;

	break;

	case(SIM_RGB565):
	case(SIM_RGB565_RE):
		*texel = (tex_out >> ( in_block_addr))%65536;
	break;

	case(SIM_RGBA1555):
		*texel = (tex_out >> ( in_block_addr))%65536;
	break;

	case(SIM_RGBA4444):
		*texel = (tex_out >> ( in_block_addr))%65536;
	break;

	case(SIM_RGBA8888):
	case(SIM_ARGB8888_RE):
		*texel = (tex_out >> (/*32 - */in_block_addr)) & 0xffffffff;
	break;

	case(SIM_Y8):
		*texel=(tex_out>>(in_block_addr))%256;
	break;

	case(SIM_ALPHA1):
		*texel=(tex_out>>(in_block_addr))%2 ? 255 : 0;
	break;

	default:
		break;
	}
	}
}
#endif


#ifdef VERSION_1

unsigned int get_bit(u16 input_num)
{
	unsigned int k = 0;
	input_num -= 1;
	while(input_num > 0)
	{
		input_num = (input_num >> 1);
		k++;
	}
	return k;
}

//-----------------------------
// name： addr_translate
// translate the address of uv into block_addr and in_block_addr
//
// @param1 FinalTile* final_tile
// @param2 u32 u      u value of texture
// @param3 u32 v      v value of texture
// @param tex_width   width of texture
// @param tex_height  height of texture
// @param tex_type    type of texture
// @param u32* block_addr    output of block address
// @param u16* in_block_addr  output of in block address
// additional comments
//   none
//-----------------------------
void addr_translate(u32 u,u32 v,u32 addr,u16 tex_width,u16 tex_height,u16 tex_type,u64* block_addr,u16* in_block_addr)
{
	unsigned int tex_width_bit;
	unsigned int tex_height_bit;
	unsigned int pixel_num = 0;
	tex_width_bit = get_bit(tex_width);
	tex_height_bit = get_bit(tex_height);
	if( (u < 0) || (v < 0) || (u >= (u32)(tex_width * 256) ) || (v >= (u32)(tex_height * 256)) )
	{
			*block_addr = 0;
			*in_block_addr = 0x40;
	}
	else
	{
		switch (tex_type)
		{

		case(SIM_DXT1):
			*block_addr = addr + (u/256/4 + v/256/4 * (tex_width/4 + (tex_width%4 == 0 ? 0 : 1))) ;
			*in_block_addr = u/256%4 + v/256%4 * 4;
			break;

		case(SIM_DXT3):
			*block_addr = addr + (u/256/4 + v/256/4 * (tex_width/4 + (tex_width%4 == 0 ? 0 : 1)));
			*in_block_addr = u/256%4 + v/256%4 * 4;
			break;
		case(SIM_DXT5):
			*block_addr = addr + (u/256/4 + v/256/4 * (tex_width/4 + (tex_width%4 == 0 ? 0 : 1)));
			*in_block_addr = u/256%4 + v/256%4 * 4;
			break;
		case(SIM_ALPHA1):
				*block_addr= (addr*8 + (u/256 + v/256 * tex_width)) /64 ;
				*in_block_addr = (u/256 + v/256 * tex_width) % 64;
			break;

		case(SIM_ALPHA2):
				*block_addr= (addr*4 + (u/256 + v/256 * tex_width)) /32 ;
				*in_block_addr = ((u/256 + v/256 * tex_width) % 32);
			break;

		case(SIM_ALPHA4):
				*block_addr= (addr*2 + (u/256 + v/256 * tex_width)) /16;
				*in_block_addr = ((u/256 + v/256 * tex_width) % 16);
			break;

		case(SIM_ALPHA8):
				*block_addr= (addr + (u/256 + v/256 * tex_width))/8;
				*in_block_addr = ((u/256 + v/256 * tex_width) % 8);
			break;

		case(SIM_RGB565):
				*block_addr= addr + (u/256 + v/256 * tex_width) /4;
				*in_block_addr = ((u/256 + v/256 * tex_width) % 4);
			break;

		case(SIM_RGBA8888):
				*block_addr= addr + (u/256 + v/256 * tex_width) /2 ;
				*in_block_addr = ((u/256 + v/256 * tex_width) % 2);
			break;

		case(SIM_Y8):
				*block_addr= addr + (u/256 + v/256 * tex_width) /8;
				*in_block_addr = ((u/256 + v/256 * tex_width) % 8);
			break;

		default:
			*block_addr= 0;
			*in_block_addr = 0;
			break;
		}
	}
}


//-----------------------------
// 函数名： ahmi::TexRaster(
// 纹理光栅化
// 参数列表：
// @param1 Tile* tile      当前tile
// @param2 Matrix* matrix  纹理矩阵
// @param3 u32 addr        纹理地址
// @param4 u16 tex_width   纹理宽
// @param5 u16 tex_height  纹理高
// @param6 u8 texture_ctrl {4:1}纹理类型,{0}是否使用alpha
// @param7 u32 InitColor   颜色
// @param8 u8 MaskType     mask类型,0覆盖，1与，2或，3非
// 备注(各个版本之间的修改):
//   无
//-----------------------------

void ahmi::TexRaster(Tile* tile,Matrix* matrix,u32 addr,u16 tex_width,u16 tex_height,u8 texture_ctrl,u32 InitColor,u8 MaskType)
{
	u8 TexType;
	long long u0;
	long long v0;
	//bool mask_usage;
	bool mask_alpha;
	int i = 0;
	U64 block_addr;
	u16 in_block_addr;
	int block_row;
	u8 pixel_u_decimal;
	u8 pixel_v_decimal;
//	u64 u_cal,v_cal;
	u32 u_int;
	u32 v_int;
	u8 alpha_texture_type = 0; //0 is not alpha texture

	COLOR color_inter[4];

	TexType = (texture_ctrl & 0x1e) >> 1;
	mask_alpha = (texture_ctrl&0x1);
	tile->mask_usage = mask_alpha;
	//u（x，y）=A(x+E) + C(y+F)；v（x，y）= B(x+E) + D(y+F)
	//u0 = (((long long)matrix->A/*1.11.20*/ * ( (tile->x/*1.11.4*/ << 9)+ (long long)matrix->E)/*1.18.13*/ + (long long)matrix->C/*1.11.20*/ * ( (tile->y/*1.11.4*/ << 9)+matrix->F)/*1.18.13*/)>>25);//1.39.8
	//v0 = (((long long)matrix->B/*1.11.20*/ * ( (tile->x/*1.11.4*/ << 9)+ (long long)matrix->E)/*1.18.13*/ + (long long)matrix->D/*1.11.20*/ * ( (tile->y/*1.11.4*/ << 9)+matrix->F)/*1.18.13*/)>>25);//1.39.8

	//change the way matrix compute by zuz 2018/10/15
	//u = Ax + Cy + E
	//v = Bx + Dy + F
#ifdef UseFloatMatrix
	u0 =  (long long)(((float)matrix->A / (1 << 20) * (float)tile->x / (1 << 4) 
		+ (float)matrix->C / (1 << 20) * (float)tile->y / (1 << 4)  
	    + (float)matrix->E / (1 << 13) )* (1 << 8)); //1.39.8
	v0 =  (long long)(((float)matrix->B / (1 << 20) * (float)tile->x / (1 << 4) 
		+ (float)matrix->D / (1 << 20) * (float)tile->y / (1 << 4)  
	    + (float)matrix->F / (1 << 13) )* (1 << 8)); //1.39.8
#else
	u0 =  ((long long)matrix->A/*1.11.20*/ * (tile->x/*1.11.4*/ )/*1.22.24*/ >> 16)  
		+ ((long long)matrix->C/*1.11.20*/ * (tile->y/*1.11.4*/ )/*1.22.24*/ >> 16)//1.39.8
	    + ((matrix->E/*1.18.13*/) >> 5); //1.39.8
	v0 =  ((long long)matrix->B/*1.11.20*/ * (tile->x/*1.11.4*/ )/*1.22.24*/ >> 16)
		+ ((long long)matrix->D/*1.11.20*/ * (tile->y/*1.11.4*/ )/*1.22.24*/ >> 16)//1.39.13
		+ ((matrix->F/*1.18.13*/) >> 5); //1.39.8
#endif
	TileRenderCounter++;
	for(i=0;i<TilePixelSize;i++)
	{
		long long u,v;
		u8 x,y;
		unsigned int alpha;
		unsigned int color;
		unsigned int Y;
		x=i%32;
		y=i/32;
#ifdef UseFloatMatrix
		u= u0+ (long long)(((float)matrix->A / (1 << 20) * x + (float)matrix->C / (1 << 20) * y) * (1 << 8));//1.17.8
		v= v0+ (long long)(((float)matrix->B / (1 << 20) * x + (float)matrix->D / (1 << 20) * y) * (1 << 8));//1.17.8
#else
		u= (u0+(( (long long)(matrix->A/*1.11.20*/ >> 6/*1.11.14*/) * x/*1.5.0*/ + (matrix->C >> 6) * y)>>6/*1.17.8*/) );//1.17.0
		v= (v0+(( (long long)(matrix->B/*1.11.20*/ >> 6/*1.11.14*/) * x/*1.5.0*/ + (matrix->D >> 6) * y)>>6/*1.17.8*/) );//1.17.0
#endif
#ifdef UV_LOG_GEN
		uvLogFile.fill('0');
		uvLogFile<< std::setw(8) << std::hex << (int)(u << 8) << " " << std::setw(8) << std::hex << (int)(v << 8) << std::endl;
#endif
#ifdef BILINEAR_FILTERING
		if(  ( ( (u0 & 0xff) == 0) && ( (v0 & 0xff) == 0) ) || u < -256 || u >= tex_width*256 || v < -256 || v >= tex_height*256 || TexType == SIM_PURECOLOR || TexType == SIM_PUREMASK)
		{
#endif
			u = u >> 8;/*1.17.0*/
			v = v >> 8;/*1.17.0*/
			switch(TexType)
			{
			case(SIM_DXT1):
				block_row = tex_width/4+(((tex_width&0x3)==0)?0:1);
				if((u<0)||(u>tex_width-1)||(v<0)||(v>tex_height-1))
				{
					tile->ColorBuffer[i].r=0;
					tile->ColorBuffer[i].g=0;
					tile->ColorBuffer[i].b=0;
					tile->ColorBuffer[i].a=0;
					tile->ColorBuffer[i].m=0;
				}
				else
				{
					block_addr=u/4+v/4 * block_row;
					in_block_addr=(u&0x3)+((v&0x3)<<2);
					ahmi::cache(block_addr,in_block_addr,addr,&color,TexType);						
					tile->ColorBuffer[i].r=(color&0xFF0000) >> 16;
					tile->ColorBuffer[i].g=(color&0xFF00) >> 8;
					tile->ColorBuffer[i].b=(color&0xFF);
					tile->ColorBuffer[i].a=255;
					tile->ColorBuffer[i].m=1;
				 }	
				tile->color_done=1;
				break;

			case(SIM_DXT3):
			case(SIM_DXT5):
				block_row = tex_width/4+(((tex_width&0x3)==0)?0:1);
				if((u<0)||(u>tex_width-1)||(v<0)||(v>tex_height-1))
				{
					tile->ColorBuffer[i].r=0;
					tile->ColorBuffer[i].g=0;
					tile->ColorBuffer[i].b=0;
					tile->ColorBuffer[i].a=0;
					tile->ColorBuffer[i].m=0;
				}
				else
				{
					block_addr=(u/4+v/4 * block_row)*2;
					in_block_addr=(u&0x3)+((v&0x3)<<2);
					ahmi::cache(block_addr,in_block_addr,addr,&color,TexType);	
					tile->ColorBuffer[i].r=(color&0xFF000000) >> 24;
					tile->ColorBuffer[i].g=(color&0xFF0000) >> 16;
					tile->ColorBuffer[i].b=(color&0xFF00) >>8;
					tile->ColorBuffer[i].a=(color&0xFF);
					tile->ColorBuffer[i].m=1;
				}
				tile->color_done=1;
				break;

			case(SIM_ALPHA8):
				if((u<0)||(u>tex_width-1)||(v<0)||(v>tex_height-1))
				{
					switch(MaskType)
					{
					case(0)://覆盖显示
						tile->AlphaBuffer[i]=0;
						break;
					case(1):
						tile->AlphaBuffer[i]=0;
						break;
					case(3)://取非显示
						tile->AlphaBuffer[i]=0xff;
						break;
					}
				}
				else
				{
					block_addr=(u+v*tex_width)/8;
					in_block_addr=(u+v*tex_width)%8;
					ahmi::cache(block_addr,in_block_addr,addr,&alpha,TexType);
					switch(MaskType)
					{
					case(0)://覆盖显示
						tile->AlphaBuffer[i]=alpha;
						break;
					case(1)://与显示
						tile->AlphaBuffer[i] = tile->AlphaBuffer[i] & alpha;
						break;
					case(2)://或显示
						tile->AlphaBuffer[i] = tile->AlphaBuffer[i] | alpha;
						break;
					case(3)://取非显示
						tile->AlphaBuffer[i]=~alpha;
						break;
					}
					tile->color_done=0;
				}
				break;
			case(SIM_ALPHA1):
				if((u<0)||(u>tex_width-1)||(v<0)||(v>tex_height-1))
				{
					switch(MaskType)
					{
					case(0)://覆盖显示
						tile->AlphaBuffer[i]=0;
						break;
					case(1):
						tile->AlphaBuffer[i]=0;
						break;
					case(3)://取非显示
						tile->AlphaBuffer[i]=0xff;
						break;
					}
				}
				else
				{
					block_addr=(u+v*tex_width)/64;
					in_block_addr=(u+v*tex_width)%64;
					ahmi::cache(block_addr,in_block_addr,addr,&alpha,TexType);						
					switch(MaskType)
					{
					case(0)://覆盖显示
						tile->AlphaBuffer[i]=alpha;
						break;
					case(1)://与显示
						tile->AlphaBuffer[i] = tile->AlphaBuffer[i] & alpha;
						break;
					case(2)://或显示
						tile->AlphaBuffer[i] = tile->AlphaBuffer[i] | alpha;
						break;
					case(3)://取非显示
						tile->AlphaBuffer[i]=~alpha;
						break;
					}
					tile->color_done=0;
				 }
				break;
			case(SIM_ALPHA2):
				if((u<0)||(u>tex_width-1)||(v<0)||(v>tex_height-1))
				{
					switch(MaskType)
					{
					case(0)://覆盖显示
						tile->AlphaBuffer[i]=0;
						break;
					case(1):
						tile->AlphaBuffer[i]=0;
						break;
					case(3)://取非显示
						tile->AlphaBuffer[i]=0xff;
						break;
					}
				}
				else
				{
					block_addr=(u+v*tex_width)/32;
					in_block_addr=(u+v*tex_width)%32;
					ahmi::cache(block_addr,in_block_addr,addr,&alpha,TexType);						
					switch(MaskType)
					{
					case(0)://覆盖显示
						tile->AlphaBuffer[i]=alpha;
						break;
					case(1)://与显示
						tile->AlphaBuffer[i] = tile->AlphaBuffer[i] & alpha;
						break;
					case(2)://或显示
						tile->AlphaBuffer[i] = tile->AlphaBuffer[i] | alpha;
						break;
					case(3)://取非显示
						tile->AlphaBuffer[i]=~alpha;
						break;
					}
					tile->color_done=0;
				 }
				break;
			case(SIM_ALPHA4):
				if((u<0)||(u>tex_width-1)||(v<0)||(v>tex_height-1))
				{
					switch(MaskType)
					{
					case(0)://覆盖显示
						tile->AlphaBuffer[i]=0;
						break;
					case(1):
						tile->AlphaBuffer[i]=0;
						break;
					case(3)://取非显示
						tile->AlphaBuffer[i]=0xff;
						break;
					}
				}
				else
				{
					block_addr=(u+v*tex_width)/16;
					in_block_addr=(u+v*tex_width)%16;
					ahmi::cache(block_addr,in_block_addr,addr,&alpha,TexType);						
					switch(MaskType)
					{
					case(0)://覆盖显示
						tile->AlphaBuffer[i]=alpha;
						break;
					case(1)://与显示
						tile->AlphaBuffer[i] = tile->AlphaBuffer[i] & alpha;
						break;
					case(2)://或显示
						tile->AlphaBuffer[i] = tile->AlphaBuffer[i] | alpha;
						break;
					case(3)://取非显示
						tile->AlphaBuffer[i]=~alpha;
						break;
					}
					tile->color_done=0;
				 }
				break;
			case(SIM_PURECOLOR):
				if((u<0)||(u>tex_width-1)||(v<0)||(v>tex_height-1))
				{
					tile->ColorBuffer[i].r=0;
					tile->ColorBuffer[i].g=0;
					tile->ColorBuffer[i].b=0;
					tile->ColorBuffer[i].a=0;
					tile->ColorBuffer[i].m=0;
				}
				else
				{
					tile->ColorBuffer[i].a=(InitColor&0xFF000000) >> 24;
					tile->ColorBuffer[i].r=(InitColor&0xFF0000) >> 16;
					tile->ColorBuffer[i].g=(InitColor&0xFF00) >> 8;
					tile->ColorBuffer[i].b=InitColor&0xFF;
					tile->ColorBuffer[i].m=1;
				}
				tile->color_done=1;
				break;
			case(SIM_RGB565):
				if((u<0)||(u>tex_width-1)||(v<0)||(v>tex_height-1))
				{
					tile->ColorBuffer[i].r=0;
					tile->ColorBuffer[i].g=0;
					tile->ColorBuffer[i].b=0;
					tile->ColorBuffer[i].a=0;
					tile->ColorBuffer[i].m=0;
				}
				else
				{
					block_addr=(u+v*tex_width)/4;
					in_block_addr=(u+v*tex_width)%4;
					ahmi::cache(block_addr,in_block_addr,addr,&color,TexType);
						tile->ColorBuffer[i].r= ((color&0xF800) >> 8) + ((color & 0xE000) >> 13);
						tile->ColorBuffer[i].g= ((color&0x7E0) >> 3 ) + ((color & 0x0600) >> 9) ;
						tile->ColorBuffer[i].b= ((color&0x1F) << 3  ) + ((color & 0x001C )>> 2) ; 
					tile->ColorBuffer[i].a=255;
					tile->ColorBuffer[i].m=1;
					
				 }
				tile->color_done=1;
				break;
			case(SIM_RGBA8888):
				if((u<0)||(u>tex_width-1)||(v<0)||(v>tex_height-1))
				{
					tile->ColorBuffer[i].r=0;
					tile->ColorBuffer[i].g=0;
					tile->ColorBuffer[i].b=0;
					tile->ColorBuffer[i].a=0;
					tile->ColorBuffer[i].m=0;
				}
				else
				{
					block_addr=(u+v*tex_width)/2;
					in_block_addr=(u+v*tex_width)%2;
					ahmi::cache(block_addr,in_block_addr,addr,&color,TexType);
					tile->ColorBuffer[i].a=(color&0xFF000000) >> 24;
					tile->ColorBuffer[i].r=(color&0xFF0000) >> 16;
					tile->ColorBuffer[i].g=(color&0xFF00) >> 8;
					tile->ColorBuffer[i].b=(color&0xFF);
					tile->ColorBuffer[i].m=1;					
				}
				tile->color_done=1;
				break;
			case(SIM_Y8):
				if((u<0)||(u>tex_width-1)||(v<0)||(v>tex_height-1))
				{
					tile->ColorBuffer[i].r=0;
					tile->ColorBuffer[i].g=0;
					tile->ColorBuffer[i].b=0;
					tile->ColorBuffer[i].a=0;
					tile->ColorBuffer[i].m=0;
				}
				else
				{
					block_addr=(u+v*tex_width)/8;
					in_block_addr=(u+v*tex_width)%8;
					ahmi::cache(block_addr,in_block_addr,addr,&Y,TexType);
					tile->ColorBuffer[i].r=Y;
					tile->ColorBuffer[i].g=Y;
					tile->ColorBuffer[i].b=Y;
					tile->ColorBuffer[i].a=255;
					tile->ColorBuffer[i].m=1;
				}
				tile->color_done=1;
				break;
			case(SIM_PUREMASK):
				if((u<0)||(u>tex_width-1)||(v<0)||(v>tex_height-1))
				{
					switch(MaskType)
					{
					case(0)://覆盖显示
						tile->AlphaBuffer[i]=0;
						break;
					case(1):
						tile->AlphaBuffer[i]=0;
						break;
					case(3)://取非显示
						tile->AlphaBuffer[i]=0xff;
						break;
					}
				}
				else
				{
					switch(MaskType)
					{
					case(0)://覆盖显示
						tile->AlphaBuffer[i]= (InitColor & 0xff000000) >> 24;
						break;
					case(1)://与显示
						tile->AlphaBuffer[i] = tile->AlphaBuffer[i] & ((InitColor & 0xff000000) >> 24);
						break;
					case(2)://或显示
						tile->AlphaBuffer[i] = tile->AlphaBuffer[i] | ((InitColor & 0xff000000) >> 24);
						break;
					case(3)://取非显示
						tile->AlphaBuffer[i] = ~((InitColor & 0xff000000) >> 24);
						break;
					}
				 }
				tile->color_done=0;
			}
#ifdef BILINEAR_FILTERING
		}//end of no bilinear
		else //bi-filter
		{
			//
			pixel_u_decimal = u & 0xff;
			pixel_v_decimal = v & 0xff;
			u16 param[4];
			//param[0] = 256 - pixel_u_decimal - pixel_v_decimal + pixel_u_decimal*pixel_v_decimal/256;
			//param[1] = pixel_u_decimal -  pixel_u_decimal*pixel_v_decimal/256;
			//param[2] = (pixel_u_decimal *  pixel_v_decimal)/256;
			//param[3] = pixel_v_decimal - pixel_u_decimal*pixel_v_decimal/256;
			param[0] = (255 - pixel_u_decimal)*(255 - pixel_v_decimal)/256;
			param[1] = (255 - pixel_v_decimal)*pixel_u_decimal/256;
			param[2] = (pixel_u_decimal*pixel_v_decimal)/256;
			param[3] = (255-pixel_u_decimal)*pixel_v_decimal/256;


			for(int j = 0;j<4;j++)
			{
				u_int = ((u >> 8) << 8) + (((j/2)^(j%2)) << 8); //j==0, (0,0); j==1 (1,0); j==2 (1,1); j==3 (0,1)
				v_int = ((v >> 8) << 8) + ((j/2)<<8);
				u8 tex_type = TexType;
				unsigned int texel;
				addr_translate(u_int,v_int,0,tex_width,tex_height,TexType,&block_addr, &in_block_addr);
				ahmi::cache(block_addr,in_block_addr,addr,&texel,TexType);
				if(in_block_addr == 0x40)
					texel = 0;
				else
					{
						if(tex_type == SIM_PURECOLOR)
						{
							texel = addr;
						}
						if(tex_type == SIM_PUREMASK)
						{
							texel = addr>>24;
						}
					}
				switch (tex_type)
				{
					case(SIM_DXT1):					
							color_inter[j].r=((texel)&0xFF0000) >> 16;
							color_inter[j].g=((texel)&0xFF00) >> 8;
							color_inter[j].b=((texel)&0xFF);
							color_inter[j].a= (in_block_addr == 0x40) ? 0: 255;
							tile->ColorBuffer[i].m=1;
							alpha_texture_type = 0;
						break;

					case(SIM_DXT3):
							color_inter[j].a=((texel)&0xFF000000) >> 24;
							color_inter[j].r=((texel)&0xFF0000) >> 16;
							color_inter[j].g=((texel)&0xFF00) >>8;
							color_inter[j].b=((texel)&0xFF);
							tile->ColorBuffer[i].m=1;
							alpha_texture_type = 0;
						break;

					case(SIM_DXT5):
							color_inter[j].a=((texel)&0xFF000000) >> 24;
							color_inter[j].r=((texel)&0xFF0000) >> 16;
							color_inter[j].g=((texel)&0xFF00) >>8;
							color_inter[j].b=((texel)&0xFF);
							tile->ColorBuffer[i].m=1;
							alpha_texture_type = 0;
					break;

					case(SIM_ALPHA1):
							color_inter[j].a=((texel)&0xFF);
							color_inter[j].r=0;
							color_inter[j].g=0;
							color_inter[j].b=0;
							tile->ColorBuffer[i].m=1;
							alpha_texture_type = 1;
						break;

					case(SIM_ALPHA2):
							color_inter[j].a=((texel)&0xFF);
							color_inter[j].r=0;
							color_inter[j].g=0;
							color_inter[j].b=0;
							tile->ColorBuffer[i].m=1;
							alpha_texture_type = 1;
						break;

					case(SIM_ALPHA4):
							color_inter[j].a=((texel)&0xFF);
							color_inter[j].r=0;
							color_inter[j].g=0;
							color_inter[j].b=0;
							tile->ColorBuffer[i].m=1;
							alpha_texture_type = 1;
						break;

					case(SIM_ALPHA8):
							color_inter[j].a=((texel)&0xFF);
							color_inter[j].r=0;
							color_inter[j].g=0;
							color_inter[j].b=0;
							tile->ColorBuffer[i].m=1;
							alpha_texture_type = 1;
						break;

					case(SIM_RGB565):
						color_inter[j].r=(((texel)&0xF800) >> 8) + (((texel)&0xF800) >> 13);
						color_inter[j].g=(((texel)&0x7E0) >> 3) + (((texel)&0x7E0) >> 9);
						color_inter[j].b=(((texel)&0x1F) << 3) + (((texel)&0x1F) >>2);
						color_inter[j].a= (in_block_addr == 0x40) ? 0: 255;
						tile->ColorBuffer[i].m=1;
						alpha_texture_type = 0;
					break;

					case(SIM_RGBA8888):
							color_inter[j].a=((texel)&0xFF000000) >> 24;
							color_inter[j].r=((texel)&0xFF0000) >> 16;
							color_inter[j].g=((texel)&0xFF00) >>8;
							color_inter[j].b=((texel)&0xFF);
							tile->ColorBuffer[i].m=1;
							alpha_texture_type = 0;
						break;

					case(SIM_Y8):
							color_inter[j].a=255;
							color_inter[j].r=((texel)&0xFF);
							color_inter[j].g=((texel)&0xFF);
							color_inter[j].b=((texel)&0xFF);
							tile->ColorBuffer[i].m=1;
							alpha_texture_type = 0;
						break;

				case(SIM_PURECOLOR):
							color_inter[j].a=((texel)&0xFF000000) >> 24;
							color_inter[j].r=((texel)&0xFF0000) >> 16;
							color_inter[j].g=((texel)&0xFF00) >>8;
							color_inter[j].b=((texel)&0xFF);
							tile->ColorBuffer[i].m=1;
							alpha_texture_type = 0;
					break;

				case(SIM_PUREMASK):
							color_inter[j].a=((texel)&0xFF);
							color_inter[j].r=0;
							color_inter[j].g=0;
							color_inter[j].b=0;
							tile->ColorBuffer[i].m=1;
							alpha_texture_type = 1;
					break;

					default:
						break;
				}
			}//end of four points

			if(alpha_texture_type == 0)//color type
			{
			    tile->ColorBuffer[i].r = (color_inter[2].r * param[2] + color_inter[3].r * param[3] + color_inter[0].r * param[0] + color_inter[1].r *param[1])/256;
			    tile->ColorBuffer[i].g = (color_inter[2].g * param[2] + color_inter[3].g * param[3] + color_inter[0].g * param[0] + color_inter[1].g *param[1])/256;
			    tile->ColorBuffer[i].b = (color_inter[2].b * param[2] + color_inter[3].b * param[3] + color_inter[0].b * param[0] + color_inter[1].b *param[1])/256;
			    tile->ColorBuffer[i].a = (color_inter[2].a * param[2] + color_inter[3].a * param[3] + color_inter[0].a * param[0] + color_inter[1].a *param[1])/256;
				tile->color_done=1;
			}
			else 
			{
				tile->AlphaBuffer[i] = (color_inter[2].a * param[2] + color_inter[3].a * param[3] + color_inter[0].a * param[0] + color_inter[1].a *param[1])/256;
				tile->color_done=0;
			}

		}//end of bi-filter
#endif
	}

}
#endif

#ifdef VERSION_2

void ahmi::bte(Tile* tile_in,VERTEX_SET* TexVertex,u16 tex_type)
{
	unsigned int i,tile_x,tile_y,j;
	short x0,y0,x1,y1,x2,y2,x3,y3;
	unsigned short x_temp,y_temp;
	int a0,b0,a1,b1,a2,b2,a3,b3;
	long long ee0_int,ee1_int,ee2_int,ee3_int;
	long long ee0,ee1,ee2,ee3;

	tile_x = tile_in->x;
	tile_y = tile_in->y;

#ifdef TEST_LOG_GEN
	/*char text[1000];
	FILE * fp;
	sscanf(text,"%d %d BTE",tile_in->x,tile_in->y);*/
	CString CSFilename;
	CreateDirectory(_T("BTE_log"),NULL);
	CSFilename.Format(_T("BTE_log\\%d_%d_%d_BTE.log"),tile_in->x,tile_in->y,bte_counter);
	std::string str((LPCSTR)CStringA(CSFilename));
	char* text = (char *)str.c_str();

	FILE * fp;
	fp = fopen(text,"w");
#endif

	x0 = TexVertex->v0.cor.x;
	y0 = TexVertex->v0.cor.y;
	x1 = TexVertex->v1.cor.x;
	y1 = TexVertex->v1.cor.y;
	x2 = TexVertex->v2.cor.x;
	y2 = TexVertex->v2.cor.y;
	x3 = TexVertex->v3.cor.x;
	y3 = TexVertex->v3.cor.y;

	a0 = y0- y1;
	b0 = x1- x0;
	a1 = y1- y2;
	b1 = x2- x1;
	a2 = y2- y3;
	b2 = x3- x2;
	a3 = y3- y0;
	b3 = x0 - x3;
	ee0_int = (long long)a0 * (long long)tile_x + (long long)b0 * (long long)tile_y + (long long)x0 * (long long)y1 - (long long)x1 * (long long)y0;
	ee1_int = (long long)a1 * (long long)tile_x + (long long)b1 * (long long)tile_y + (long long)x1 * (long long)y2 - (long long)x2 * (long long)y1;
	ee2_int = (long long)a2 * (long long)tile_x + (long long)b2 * (long long)tile_y + (long long)x2 * (long long)y3 - (long long)x3 * (long long)y2;
	ee3_int = (long long)a3 * (long long)tile_x + (long long)b3 * (long long)tile_y + (long long)x3 * (long long)y0 - (long long)x0 * (long long)y3;
	j= 0 ;
	tile_in->mask_usage = ((tex_type&0x8) == 0x8);
	tile_in->mask_type = tex_type&0x3;
	bool judge_t0 = (a0 > 0) | ( (a0 == 0) & (b0 > 0) );
	bool judge_t1 = (a1 > 0) | ( (a1 == 0) & (b1 > 0) );
	bool judge_t2 = (a2 > 0) | ( (a2 == 0) & (b2 > 0) );
	bool judge_t3 = (a3 > 0) | ( (a3 == 0) & (b3 > 0) );

	for(i=0;i<TILESIZE* TILESIZE;i++)
	{
		x_temp= i%2 + ((i & 4)>>2)*2 + ((i & 16)>>4)*4 + ((i & 64)>>6)*8 + ((i & 256)>>8)*16;
		y_temp=((i & 2)>>1) + ((i & 8)>>3)*2 + ((i & 32)>>5)*4 + ((i & 128)>>7)*8 + ((i & 512)>>9)*16;
		ee0 = ee0_int + a0 * x_temp*16 + b0 * y_temp*16 ;
		ee1 = ee1_int + a1 * x_temp*16 + b1 * y_temp*16 ;
		ee2 = ee2_int + a2 * x_temp*16 + b2 * y_temp*16 ;
		ee3 = ee3_int + a3 * x_temp*16 + b3 * y_temp*16 ;

		if( (tex_type&0x100) == 0x100)
		{
			tile_in->ColorBuffer[i].cor.x = tile_x + x_temp*16;
			tile_in->ColorBuffer[i].cor.y = tile_y + y_temp * 16;
			tile_in->ColorBuffer[i].c.m = 1;
			j++;
		}
		else if((ee0>0 || (judge_t0 && ee0 == 0)) & (ee1>0 || (judge_t1 && ee1 == 0)) & (ee2>0 || (judge_t2 && ee2 == 0)) & (ee3>0 || (judge_t3 && ee3 == 0)))
		{
			tile_in->ColorBuffer[j].cor.x = tile_x + x_temp*16;
			tile_in->ColorBuffer[j].cor.y = tile_y + y_temp * 16;
			tile_in->ColorBuffer[j].c.m = 1;
			j++;
		}

	}
	int k = j;
#ifdef TEST_LOG_GEN
	for(int i = 0;i<j;i++)
	{
		fprintf(fp,"%04x %04x\n",tile_in->ColorBuffer[i].cor.x,tile_in->ColorBuffer[i].cor.y);
	}
	fclose(fp);
#endif
	bte_counter++;
}

void param_cal(	long long u_temp,long long v_temp,int w_temp,int* u_out_fix,int* v_out_fix)
{
	union{float f;unsigned int i;} x,y;
	unsigned int x_i,x_e,x_w;
	unsigned int y_i,y_e,y_w;
	float jump = (float)1/1024;
	float test;
	float error_u_max = 0;
	float error_v_max = 0;

	float error_u;
	float error_v;

	unsigned int R_addr;

	x.f = 1 + jump/2;
	int i = 0;

	//FILE *fp;
    //fopen_s(&fp,"div_r.log","w");

//计算 1024个 R值*********************************************
	while(x.f<2 && r_flag == 0)
	{  
		y.f = (float)1 / x.f;
		int t =  (x.i&0x7f800000>>23);
		x_w = ((x.i & 0x007fffff) >> (23 - w_width));
		y_w = ((y.i & 0x007fffff) >> (23 - w_width));
		test = (float)(y_w + ((unsigned int)1 << w_width)) / ((unsigned int) 1 <<(w_width +1));
		R[i] = x_w + y_w;
		//fprintf(fp,"%x\n",R[i]);
		x.f = x.f + jump;
		i = i+1;
	}
	r_flag = 1;

/*
//精度测试*************************************
	x.f = 1;
	float jump0 = (float)1/1024/32;
	
	float result;
	float error;
	float error_max = 0;
	while(x.f<2)
	{
		y.f = (float) 1/ x.f;
		x_w = ((x.i & 0x007fffff) >> (23 - w_width));
		R_addr = (x_w >> (w_width - div_width));
		y_w = R[R_addr] - x_w;
		result = (float)((1<<w_width) + y_w)/((unsigned int) 1 <<(w_width +1));
		error  = fabs(result - y.f);
		if(error > error_max)
			error_max = error;
		x.f = x.f + jump0;
	}
//..............................................................
*/






//。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。

	unsigned long long u_temp_positive,v_temp_positive;  //33.12
	unsigned int	   w_temp_positive; // 11.20
	bool u_temp_sign,v_temp_sign,w_temp_sign;  //0 为正； 1为负

	bool  sign1 = (w_temp == 0x0100000);

//u v w to positive**************************************************

//	u_temp = (long long)1000<<12;	//////////////////////////////////////////////////////
//	v_temp = -u_temp;	//////////////////////////////////////////////////////
	//w_temp = 1<<19;	//////////////////////////////////////////////////////
	//w_temp = -w_temp;
	if(u_temp>=0)
	{
		u_temp_positive = u_temp;
		u_temp_sign = 0;						//0 为正； 1为负
	}
	else 
	{
		u_temp_positive = 0 - u_temp;
		u_temp_sign = 1;						
	}

	if(v_temp >= 0)
	{
		v_temp_positive = v_temp;
		v_temp_sign = 0;
	}
	else
	{
		v_temp_positive = 0 - v_temp;
		v_temp_sign = 1;
	}


	if(w_temp >=0)
	{
		w_temp_positive = w_temp;
		w_temp_sign = 0;
	}
	else
	{
		w_temp_positive = 0 - w_temp;
		w_temp_sign = 1;
	}
//........................................................................................


	float result_u = (float) u_temp / w_temp * pow((float)2,8);
	float result_v = (float) v_temp / w_temp * pow((float)2,8);

	unsigned int w_w;  // 0.15
	unsigned int w_shift;  // 偏移 32 ；即 w_positive = (1+w_w)*（2**（shift-32);
	unsigned int  w_temp0; // 11.20


//w_temp_Positive 转 浮点*********************
	
//	w_temp_positive = 0x00070000;			//////////////////////////

	w_temp0 = w_temp_positive;
	w_shift = 43;
	while(!(w_temp0 & 0x80000000))
	{
		w_shift = w_shift -1;
		w_temp0 = w_temp0 * 2;
	}
	w_w = (w_temp0 & 0x7fffffff)>>(31-w_width);
//.............................................

//求 w_daoshu*********************************
	float w_daoshu_test;
	unsigned int w_daoshu; 
	unsigned int w_daoshu_w;
	int w_daoshu_shift;

	R_addr = (w_w >> (w_width - div_width));
	w_daoshu_w = R[R_addr] - w_w;
	w_daoshu_test = (float)((1<<w_width) + w_daoshu_w)/((unsigned int) 1 <<(w_width +1));
	w_daoshu = ((1<<w_width) + w_daoshu_w);
	w_daoshu_shift = 64 - w_shift - w_width -1;		
//.............................................

//计算 u_out_temp v_out_temp, u_out,v_out****************
	
	unsigned long long u_out_temp,v_out_temp;  
	long long u_out,v_out;

	u_out_temp = (u_temp_positive >> 5) * w_daoshu;
	v_out_temp = (v_temp_positive >> 5) * w_daoshu;


	if((u_temp_sign&w_temp_sign)|(!u_temp_sign & !w_temp_sign))
		u_out = (u_out_temp) >>(39 - w_daoshu_shift);//*(pow((float)2,(w_daoshu_shift-32-4)));
	else 
		u_out = ((long long)((long long)0 - u_out_temp) >> (39 - w_daoshu_shift));//*(pow((float)2,(w_daoshu_shift-32-4)));

	if((v_temp_sign&w_temp_sign)|(!v_temp_sign & !w_temp_sign))
		v_out = (v_out_temp) >>(39 - w_daoshu_shift);//*(pow((float)2,(w_daoshu_shift-32-4)));
	else 
		v_out =((long long)((long long)0 - v_out_temp) >> (39 - w_daoshu_shift));//*(pow((float)2,(w_daoshu_shift-32-4)));
	//error_u = fabs(u_out - result_u);
	//error_v = fabs(v_out - result_v);
	//if(error_u > error_u_max)
	//	error_u_max = error_u;
	//
	//if(error_v > error_v_max)
	//	error_v_max = error_v;
	if(sign1)
	{
		*u_out_fix = (int)(u_temp >> 12);
		*v_out_fix = (int)(v_temp >> 12);
	}
	else
	{
		*u_out_fix = (int)(u_out);
		*v_out_fix = (int)(v_out);
	}
#ifdef TEST_LOG_GEN
	fprintf(fp_uv_division,"%08x ",*u_out_fix);
	fprintf(fp_uv_division,"%08x\n ",*v_out_fix);
#endif

}

void ahmi::uv_translate(Tile* tile_in,Matrix* matrix,u16 tex_type)
{	
	for(int i=0;i<TILESIZE* TILESIZE;i++)
		{
			if(tile_in->ColorBuffer[i].c.m)
			{
				long long w,u_temp,v_temp;
				u_temp = ((long long)matrix->A*(long long)(tile_in->ColorBuffer[i].cor.x) + (long long)matrix->D*(long long)(tile_in->ColorBuffer[i].cor.y) + (long long)((long long)matrix->G<< 11)>> 4);//1.43.20
				v_temp = ((long long)matrix->B*(long long)(tile_in->ColorBuffer[i].cor.x) + (long long)matrix->E*(long long)(tile_in->ColorBuffer[i].cor.y) + (long long)((long long)matrix->H<< 11)>> 4);
				if( u_temp >= 0x80000000000 || u_temp <= (long long)-0x80000000000 || v_temp >= 0x80000000000 || v_temp <= (long long)-0x80000000000)
				{
					int yangpengfei = 0;
				}
				w = ((long long)matrix->C*(long long)(tile_in->ColorBuffer[i].cor.x) + (long long)matrix->F*(long long)(tile_in->ColorBuffer[i].cor.y) + (long long)((long long)1<< 24) >>4);
				if(w == 0)
					w = 1;
				param_cal(u_temp,v_temp,w,&tile_in->ColorBuffer[i].cor.u,&tile_in->ColorBuffer[i].cor.v);
				uv_count_test = uv_count_test + 1;
			}
		}
#ifdef TEST_LOG_GEN
	for(int i=0;i<TILESIZE* TILESIZE;i++)
		{
			if(tile_in->ColorBuffer[i].c.m)
			fprintf(fp_uv,"%08x %08x\n",tile_in->ColorBuffer[i].cor.u,tile_in->ColorBuffer[i].cor.v);
		}
#endif
}

unsigned int get_bit(u16 input_num)
{
	unsigned int k = 0;
	input_num -= 1;
	while(input_num > 0)
	{
		input_num = (input_num >> 1);
		k++;
	}
	return k;
}

void addr_translate(u32 u,u32 v,u32 addr,u16 tex_width,u16 tex_height,u16 tex_type,u32* block_addr,u16* in_block_addr)
{
	unsigned int tex_width_bit;
	unsigned int tex_height_bit;
	unsigned int pixel_num = 0;
	tex_width_bit = get_bit(tex_width);
	tex_height_bit = get_bit(tex_height);
	if( (u < 0) || (v < 0) || (u >= (u32)(tex_width * 256) ) || (v >= (u32)(tex_height * 256)) )
	{
			*block_addr = 0;
			*in_block_addr = 0x40;
	}
	else
	{
		switch (tex_type)
		{

		case(SIM_DXT1):
			if(embedded_platform == EMBEDDED_GC9003)
			{
				*block_addr = addr + (u/256/4 + v/256/4 * (tex_width/4 + (tex_width%4 == 0 ? 0 : 1))) * 8;
				*in_block_addr = u/256%4 + v/256%4 * 4;
			}
			else
			{
				*block_addr = addr + (u/256/4 + v/256/4 * (tex_width/4 + (tex_width%4 == 0 ? 0 : 1))) * 16;
				*in_block_addr = u/256%4 + v/256%4 * 4;
			}
			break;

		case(SIM_DXT3):
			*block_addr = addr + (u/256/4 + v/256/4 * (tex_width/4 + (tex_width%4 == 0 ? 0 : 1))) * 16;
			*in_block_addr = u/256%4 + v/256%4 * 4;
			break;
		case(SIM_DXT5):
			*block_addr = addr + (u/256/4 + v/256/4 * (tex_width/4 + (tex_width%4 == 0 ? 0 : 1))) * 16;
			*in_block_addr = u/256%4 + v/256%4 * 4;
			break;
		case(SIM_ALPHA1):
				*block_addr= (addr*8 + (u/256 + v/256 * tex_width)) /64 * 8 + (addr&0x80000000);
				*in_block_addr = (u/256 + v/256 * tex_width) % 64;
			break;

		case(SIM_ALPHA2):
				*block_addr= (addr*4 + (u/256 + v/256 * tex_width)) /32 * 8 + (addr&0x80000000);
				*in_block_addr = ((u/256 + v/256 * tex_width) % 32)*2;
			break;

		case(SIM_ALPHA4):
				*block_addr= (addr*2 + (u/256 + v/256 * tex_width)) /16 * 8 + (addr&0x80000000);
				*in_block_addr = ((u/256 + v/256 * tex_width) % 16)*4;
			break;

		case(SIM_ALPHA8):
				*block_addr= (addr + (u/256 + v/256 * tex_width)) /8 * 8;
				*in_block_addr = ((u/256 + v/256 * tex_width) % 8)*8;
			break;

		case(SIM_RGB565):
				*block_addr= addr + (u/256 + v/256 * tex_width) /4 * 8;
				*in_block_addr = ((u/256 + v/256 * tex_width) % 4)*16;
			break;

		case(SIM_RGBA8888):
				*block_addr= addr + (u/256 + v/256 * tex_width) /2 * 8;
				*in_block_addr = ((u/256 + v/256 * tex_width) % 2)*32;
			break;

		case(SIM_Y8):
				*block_addr= addr + (u/256 + v/256 * tex_width) /8 * 8;
				*in_block_addr = ((u/256 + v/256 * tex_width) % 8)*8;
			break;

		case(SIM_ARGB8888_RE):
				u=u/256;
				v=v/256;
				if(tex_width_bit == tex_height_bit)
				{
					for(int k = 0;  k < tex_width_bit;k++)
					{
						pixel_num += ((((u>>k)%2) + (((v>>k)%2)<<1)) << (2*k));
					}
				}
				else if(tex_width_bit < tex_height_bit)
				{
					for(int k = 0;  k < tex_height_bit;k++)
					{
						if(k<tex_width_bit)
						{
							pixel_num += ((((u>>k)%2) + (((v>>k)%2)<<1)) << (2*k));
						}
						else
						{
							pixel_num += ((v>>k)%2) << (2*tex_width_bit + (k - tex_width_bit) );
						}
					}
				}
				else
				{
					for(int k = 0;  k < tex_width_bit;k++)
					{
						if(k<tex_height_bit)
						{
							pixel_num += ((((u>>k)%2) + (((v>>k)%2)<<1)) << (2*k));
						}
						else
						{
							pixel_num += ((u>>k)%2) << (2*tex_height_bit + (k - tex_height_bit) );
						}
					}
				}
				*block_addr = addr + pixel_num /2*8;
				*in_block_addr = pixel_num%2*32;

			break;

		case(SIM_RGB565_RE):
				u=u/256;
				v=v/256;
				if(tex_width_bit == tex_height_bit)
				{
					for(int k = 0;  k < tex_width_bit;k++)
					{
						pixel_num += ((((u>>k)%2) + (((v>>k)%2)<<1)) << (2*k));
					}
				}
				else if(tex_width_bit < tex_height_bit)
				{
					for(int k = 0;  k < tex_height_bit;k++)
					{
						if(k<tex_width_bit)
						{
							pixel_num += ((((u>>k)%2) + (((v>>k)%2)<<1)) << (2*k));
						}
						else
						{
							pixel_num += ((v>>k)%2) << (2*tex_width_bit + (k - tex_width_bit) );
						}
					}
				}
				else
				{
					for(int k = 0;  k < tex_width_bit;k++)
					{ 
						if(k<tex_height_bit)
						{
							pixel_num += ((((u>>k)%2) + (((v>>k)%2)<<1)) << (2*k));
						}
						else
						{
							pixel_num += ((u>>k)%2) << (2*tex_height_bit + (k - tex_height_bit) );
						}
					}
				}
				*block_addr = addr + pixel_num /4*8;
				*in_block_addr = pixel_num%4*16;

			break;

		//case(SIM_RGBA1555):
		//		*block_addr= addr + (u/256 + v/256 * tex_width) /4 * 8;
		//		*in_block_addr = (u/256 + v/256 * tex_width) % 4;
		//	break;

		//case(SIM_RGBA4444):
		//		*block_addr= addr + (u/256 + v/256 * tex_width) /4 * 8;
		//		*in_block_addr = ((u/256 + v/256 * tex_width) % 4)*16;
		//	break;

		default:
			*block_addr= 0;
			*in_block_addr = 0;
			break;
		}
	}
}

void ahmi::pixel_generate(Tile* tile_in,u32 addr ,u16 tex_width,u16 tex_height,u16 texture_ctrl,u32 InitColor,VERTEX_SET* tex_vertex)
{
	u8 tex_type;
	//bool mask_use;
	//u8 mask_type;
	u32 block_addr = 0;
	u16 in_block_addr = 0;
	unsigned int texel;
	//unsigned int texel_inter[4];
	COLOR color_inter[4];
	int pixel_u;
	int pixel_v;
	u32 u_int;
	u32 v_int;
	u8 pixel_u_decimal;
	u8 pixel_v_decimal;
	tex_type = (texture_ctrl & 0xf0) >> 4;
	//if(tex_type == SIM_PURECOLOR || tex_type == SIM_PUREMASK)
	//{
	//	interpolation(tile_in,tex_width,tex_height,tex_vertex,addr,tex_type);
	//	if(tex_type == SIM_PURECOLOR)
	//		tile_in->alpha_pixel = 1;
	//	else
	//		tile_in->alpha_pixel = 0;
	//}
	//else
	{
		for(int i = 0;i < TILESIZE* TILESIZE;i++)
		{
			if(tile_in->ColorBuffer[i].c.m)
			{
				pixel_u = tile_in->ColorBuffer[i].cor.u;
				pixel_v = tile_in->ColorBuffer[i].cor.v;
				if(pixel_u < -256 || pixel_u >= tex_width*256 || pixel_v < -256 || pixel_v >= tex_height*256)
				{
					tile_in->ColorBuffer[i].c.r=0;
					tile_in->ColorBuffer[i].c.g=0;
					tile_in->ColorBuffer[i].c.b=0;
					tile_in->ColorBuffer[i].c.a=0;
					//tile_in->alpha_pixel = 1;
					switch (tex_type)
					{
						case(SIM_DXT1):					
								tile_in->alpha_pixel = 1;
							break;

						case(SIM_DXT3):
								tile_in->alpha_pixel = 1;
							break;

						case(SIM_DXT5):
								tile_in->alpha_pixel = 1;
							break;

						case(SIM_ALPHA1):
							tile_in->alpha_pixel = 0;
							break;

						case(SIM_ALPHA2):
							tile_in->alpha_pixel = 0;
							break;

						case(SIM_ALPHA4):
							tile_in->alpha_pixel = 0;
							break;

						case(SIM_ALPHA8):
							tile_in->alpha_pixel = 0;
							break;

						case(SIM_RGB565):
						case(SIM_RGB565_RE):
							tile_in->alpha_pixel = 1;
							break;

						case(SIM_RGBA8888):
						case(SIM_ARGB8888_RE):
								tile_in->alpha_pixel = 1;
							break;

						case(SIM_Y8):
								tile_in->alpha_pixel = 1;
							break;

						case(SIM_RGBA1555):
								tile_in->alpha_pixel = 1;
							break;

						case(SIM_RGBA4444):
								tile_in->alpha_pixel = 1;
							break;

						case(SIM_PURECOLOR):
								tile_in->alpha_pixel = 1;
							break;

						case(SIM_PUREMASK):
								tile_in->alpha_pixel = 0;
							break;

						default:
							break;
					}
				}
				else if(( (pixel_u & 0xff) == 0) && ( (pixel_v & 0xff) == 0))
				{
					if(pixel_u < 0 || pixel_u >= (int)(tex_width*256) || pixel_v < 0 || pixel_v >= (int)(tex_height*256) )
						{
							tile_in->ColorBuffer[i].c.r=0;
							tile_in->ColorBuffer[i].c.g=0;
							tile_in->ColorBuffer[i].c.b=0;
							tile_in->ColorBuffer[i].c.a=0;
						}
					else
					{
						addr_translate(pixel_u,pixel_v,addr,tex_width,tex_height,tex_type,&block_addr, &in_block_addr);
						ahmi::cache(block_addr,in_block_addr,0,&texel,tex_type);
						if(in_block_addr == 0x40)
							texel = 0;
						else
							{
								if(tex_type == SIM_PURECOLOR)
								{
									texel = addr;
								}
								if(tex_type == SIM_PUREMASK)
								{
									texel = addr;
								}
							}
					
						switch (tex_type)
						{
							case(SIM_DXT1):					
									tile_in->ColorBuffer[i].c.r=((texel)&0xFF0000) >> 16;
									tile_in->ColorBuffer[i].c.g=((texel)&0xFF00) >> 8;
									tile_in->ColorBuffer[i].c.b=((texel)&0xFF);
									tile_in->ColorBuffer[i].c.a=(pixel_u < 0 || pixel_u > tex_width*256 || pixel_v < 0 || pixel_v > tex_height*256) ? 0 : 255;
									tile_in->alpha_pixel = 1;
								break;

							case(SIM_DXT3):
									tile_in->ColorBuffer[i].c.a=((texel)&0xFF000000) >> 24;
									tile_in->ColorBuffer[i].c.r=((texel)&0xFF0000) >> 16;
									tile_in->ColorBuffer[i].c.g=((texel)&0xFF00) >>8;
									tile_in->ColorBuffer[i].c.b=((texel)&0xFF);
									tile_in->alpha_pixel = 1;
								break;

							case(SIM_DXT5):
								tile_in->ColorBuffer[i].c.a=((texel)&0xFF000000) >> 24;
								tile_in->ColorBuffer[i].c.r=((texel)&0xFF0000) >> 16;
								tile_in->ColorBuffer[i].c.g=((texel)&0xFF00) >>8;
								tile_in->ColorBuffer[i].c.b=((texel)&0xFF);
								tile_in->alpha_pixel = 1;
							break;

							case(SIM_ALPHA1):
								tile_in->ColorBuffer[i].c.a=((texel)&0xFF);
								tile_in->ColorBuffer[i].c.r=0;
								tile_in->ColorBuffer[i].c.g=0;
								tile_in->ColorBuffer[i].c.b=0;
								tile_in->alpha_pixel = 0;
								break;

							case(SIM_ALPHA2):
								tile_in->ColorBuffer[i].c.a=((texel)&0xFF);
								tile_in->ColorBuffer[i].c.r=0;
								tile_in->ColorBuffer[i].c.g=0;
								tile_in->ColorBuffer[i].c.b=0;
								tile_in->alpha_pixel = 0;
								break;

							case(SIM_ALPHA4):
								tile_in->ColorBuffer[i].c.a=((texel)&0xFF);
								tile_in->ColorBuffer[i].c.r=0;
								tile_in->ColorBuffer[i].c.g=0;
								tile_in->ColorBuffer[i].c.b=0;
								tile_in->alpha_pixel = 0;
								break;

							case(SIM_ALPHA8):
								tile_in->ColorBuffer[i].c.a=((texel)&0xFF);
								tile_in->ColorBuffer[i].c.r=0;
								tile_in->ColorBuffer[i].c.g=0;
								tile_in->ColorBuffer[i].c.b=0;
								tile_in->alpha_pixel = 0;
								break;

							case(SIM_RGB565):
							case(SIM_RGB565_RE):
								tile_in->ColorBuffer[i].c.r=(((texel)&0xF800) >> 8) + (((texel)&0xF800) >> 13);
								tile_in->ColorBuffer[i].c.g=(((texel)&0x7E0) >> 3) + (((texel)&0x7E0) >> 9);
								tile_in->ColorBuffer[i].c.b=(((texel)&0x1F) << 3) + (((texel)&0x1F) >>2);
								tile_in->ColorBuffer[i].c.a=(pixel_u < 0 || pixel_u > tex_width*256 || pixel_v < 0 || pixel_v > tex_height*256) ? 0 : 255;
								tile_in->alpha_pixel = 1;
								break;

							case(SIM_RGBA8888):
							case(SIM_ARGB8888_RE):
									tile_in->ColorBuffer[i].c.a=((texel)&0xFF000000) >> 24;
									tile_in->ColorBuffer[i].c.r=((texel)&0xFF0000) >> 16;
									tile_in->ColorBuffer[i].c.g=((texel)&0xFF00) >>8;
									tile_in->ColorBuffer[i].c.b=((texel)&0xFF);
									tile_in->alpha_pixel = 1;
								break;

							case(SIM_Y8):
									tile_in->ColorBuffer[i].c.a=255;
									tile_in->ColorBuffer[i].c.r=((texel)&0xFF);
									tile_in->ColorBuffer[i].c.g=((texel)&0xFF);
									tile_in->ColorBuffer[i].c.b=((texel)&0xFF);
									tile_in->alpha_pixel = 1;
								break;

							case(SIM_RGBA1555):
									tile_in->ColorBuffer[i].c.a=(((texel)&0x8000) >> 15) * 255;
									tile_in->ColorBuffer[i].c.r=((texel)&0x7C00) >> 7;
									tile_in->ColorBuffer[i].c.g=((texel)&0x3E0) >>2;
									tile_in->ColorBuffer[i].c.b=((texel)&0x1F) << 3;
									tile_in->alpha_pixel = 1;
								break;

							case(SIM_RGBA4444):
									tile_in->ColorBuffer[i].c.a=(((texel)&0xF000) >> 12) * 0x11;
									tile_in->ColorBuffer[i].c.r=(((texel)&0xF00) >> 8) * 0x11;
									tile_in->ColorBuffer[i].c.g=(((texel)&0xF0) >>4) * 0x11;
									tile_in->ColorBuffer[i].c.b=((texel)&0xF) *0x11;
									tile_in->alpha_pixel = 1;
								break;
							case(SIM_PURECOLOR):
									tile_in->ColorBuffer[i].c.a=((texel)&0xFF000000) >> 24;
									tile_in->ColorBuffer[i].c.r=((texel)&0xFF0000) >> 16;
									tile_in->ColorBuffer[i].c.g=((texel)&0xFF00) >>8;
									tile_in->ColorBuffer[i].c.b=((texel)&0xFF);
									tile_in->alpha_pixel = 1;
								break;

							case(SIM_PUREMASK):
									tile_in->ColorBuffer[i].c.a=((texel)&0xFF000000)>>24;
									tile_in->ColorBuffer[i].c.r=0;
									tile_in->ColorBuffer[i].c.g=0;
									tile_in->ColorBuffer[i].c.b=0;
									tile_in->alpha_pixel = 0;
								break;
							default:
								break;
						}
									tile_in->ColorBuffer[i].c.r=(tile_in->ColorBuffer[i].c.r *tile_in->ColorBuffer[i].c.a)>>8;
									tile_in->ColorBuffer[i].c.g=(tile_in->ColorBuffer[i].c.g *tile_in->ColorBuffer[i].c.a)>>8;
									tile_in->ColorBuffer[i].c.b=(tile_in->ColorBuffer[i].c.b *tile_in->ColorBuffer[i].c.a)>>8;
					}
				}
				else
				{
					pixel_u_decimal = pixel_u & 0xff;
					pixel_v_decimal = pixel_v & 0xff;
					u8 param[4];
					param[0] = 256 - pixel_u_decimal - pixel_v_decimal + pixel_u_decimal*pixel_v_decimal/256;
					param[1] = pixel_u_decimal -  pixel_u_decimal*pixel_v_decimal/256;
					param[2] = (pixel_u_decimal *  pixel_v_decimal)/256;
					param[3] = pixel_v_decimal - pixel_u_decimal*pixel_v_decimal/256;
					for(int j = 0;j<4;j++)
					{
						u_int = ((pixel_u>>8) << 8) + (((j/2)^(j%2)) << 8);
						v_int = ((pixel_v >> 8) << 8) + ((j/2)<<8);
					//if(u_int < 0 || u_int >= tex_width*256 || v_int < 0 || v_int >= tex_height*256)
					//	{
					//	texel = 0;
					//	}
					//else
					{
						addr_translate(u_int,v_int,addr,tex_width,tex_height,tex_type,&block_addr, &in_block_addr);
						ahmi::cache(block_addr,in_block_addr,0,&texel,tex_type);
						if(in_block_addr == 0x40)
							texel = 0;
						else
							{
								if(tex_type == SIM_PURECOLOR)
								{
									texel = addr;
								}
								if(tex_type == SIM_PUREMASK)
								{
									texel = addr;
								}
							}
					}
						switch (tex_type)
						{
							case(SIM_DXT1):					
									color_inter[j].r=((texel)&0xFF0000) >> 16;
									color_inter[j].g=((texel)&0xFF00) >> 8;
									color_inter[j].b=((texel)&0xFF);
									color_inter[j].a= (in_block_addr == 0x40) ? 0: 255;
									tile_in->alpha_pixel = 1;
								break;

							case(SIM_DXT3):
									color_inter[j].a=((texel)&0xFF000000) >> 24;
									color_inter[j].r=((texel)&0xFF0000) >> 16;
									color_inter[j].g=((texel)&0xFF00) >>8;
									color_inter[j].b=((texel)&0xFF);
									tile_in->alpha_pixel = 1;
								break;

							case(SIM_DXT5):
									color_inter[j].a=((texel)&0xFF000000) >> 24;
									color_inter[j].r=((texel)&0xFF0000) >> 16;
									color_inter[j].g=((texel)&0xFF00) >>8;
									color_inter[j].b=((texel)&0xFF);
									tile_in->alpha_pixel = 1;
							break;

							case(SIM_ALPHA1):
									color_inter[j].a=((texel)&0xFF);
									color_inter[j].r=0;
									color_inter[j].g=0;
									color_inter[j].b=0;
									tile_in->alpha_pixel = 0;
								break;

							case(SIM_ALPHA2):
									color_inter[j].a=((texel)&0xFF);
									color_inter[j].r=0;
									color_inter[j].g=0;
									color_inter[j].b=0;
									tile_in->alpha_pixel = 0;
								break;

							case(SIM_ALPHA4):
									color_inter[j].a=((texel)&0xFF);
									color_inter[j].r=0;
									color_inter[j].g=0;
									color_inter[j].b=0;
									tile_in->alpha_pixel = 0;
								break;

							case(SIM_ALPHA8):
									color_inter[j].a=((texel)&0xFF);
									color_inter[j].r=0;
									color_inter[j].g=0;
									color_inter[j].b=0;
									tile_in->alpha_pixel = 0;
								break;

							case(SIM_RGB565):
							case(SIM_RGB565_RE):
								color_inter[j].r=(((texel)&0xF800) >> 8) + (((texel)&0xF800) >> 13);
								color_inter[j].g=(((texel)&0x7E0) >> 3) + (((texel)&0x7E0) >> 9);
								color_inter[j].b=(((texel)&0x1F) << 3) + (((texel)&0x1F) >>2);
								color_inter[j].a= (in_block_addr == 0x40) ? 0: 255;
								tile_in->alpha_pixel = 1;
							break;

							case(SIM_RGBA8888):
							case(SIM_ARGB8888_RE):
									color_inter[j].a=((texel)&0xFF000000) >> 24;
									color_inter[j].r=((texel)&0xFF0000) >> 16;
									color_inter[j].g=((texel)&0xFF00) >>8;
									color_inter[j].b=((texel)&0xFF);
									tile_in->alpha_pixel = 1;
								break;

							case(SIM_Y8):
									color_inter[j].a=255;
									color_inter[j].r=((texel)&0xFF);
									color_inter[j].g=((texel)&0xFF);
									color_inter[j].b=((texel)&0xFF);
									tile_in->alpha_pixel = 1;
								break;

							case(SIM_RGBA1555):
									color_inter[j].a=(((texel)&0x8000) >> 15) * 255;
									color_inter[j].r=((texel)&0x7C00) >> 7;
									color_inter[j].g=((texel)&0x3E0) >>2;
									color_inter[j].b=((texel)&0x1F) << 3;
									tile_in->alpha_pixel = 1;
								break;

							case(SIM_RGBA4444):
									color_inter[j].a=(((texel)&0xF000) >> 12) * 0x11;
									color_inter[j].r=(((texel)&0xF00) >> 8) * 0x11;
									color_inter[j].g=(((texel)&0xF0) >>4) * 0x11;
									color_inter[j].b=((texel)&0xF) *0x11;
									tile_in->alpha_pixel = 1;
								break;
						case(SIM_PURECOLOR):
									color_inter[j].a=((texel)&0xFF000000) >> 24;
									color_inter[j].r=((texel)&0xFF0000) >> 16;
									color_inter[j].g=((texel)&0xFF00) >>8;
									color_inter[j].b=((texel)&0xFF);
									tile_in->alpha_pixel = 1;
							break;

						case(SIM_PUREMASK):
									color_inter[j].a=((texel)&0xFF000000)>>24;
									color_inter[j].r=0;
									color_inter[j].g=0;
									color_inter[j].b=0;
									tile_in->alpha_pixel = 0;
							break;

							default:
								break;
						}
					}
				tile_in->ColorBuffer[i].c.r = ((color_inter[2].a * param[2] /256 +(color_inter[2].a* param[2]%256>127)) * color_inter[2].r + (color_inter[3].a * param[3] /256 +(color_inter[3].a* param[3]%256>127))* color_inter[3].r  + (color_inter[0].a * param[0]/256 +(color_inter[0].a* param[0]%256>127) )  * color_inter[0].r + (color_inter[1].a *param[1] /256 +(color_inter[1].a*param[1]%256>127))* color_inter[1].r )/256;
				tile_in->ColorBuffer[i].c.g = ((color_inter[2].a * param[2] /256 +(color_inter[2].a* param[2]%256>127)) * color_inter[2].g + (color_inter[3].a * param[3] /256 +(color_inter[3].a* param[3]%256>127))* color_inter[3].g  + (color_inter[0].a * param[0]/256 +(color_inter[0].a* param[0]%256>127) )  * color_inter[0].g + (color_inter[1].a *param[1] /256 +(color_inter[1].a*param[1]%256>127))* color_inter[1].g )/256;
				tile_in->ColorBuffer[i].c.b = ((color_inter[2].a * param[2] /256 +(color_inter[2].a* param[2]%256>127)) * color_inter[2].b + (color_inter[3].a * param[3] /256 +(color_inter[3].a* param[3]%256>127))* color_inter[3].b  + (color_inter[0].a * param[0]/256 +(color_inter[0].a* param[0]%256>127) )  * color_inter[0].b + (color_inter[1].a *param[1] /256 +(color_inter[1].a*param[1]%256>127))* color_inter[1].b )/256;
				tile_in->ColorBuffer[i].c.a = (color_inter[2].a * param[2] + color_inter[3].a * param[3] + color_inter[0].a * param[0] + color_inter[1].a *param[1])/256;
				}
			}
#ifdef TEST_LOG_GEN
			fprintf(fp_pge,"%04x %04x %02x %02x %02x %02x\n",tile_in->ColorBuffer[i].cor.x,tile_in->ColorBuffer[i].cor.y,tile_in->ColorBuffer[i].c.r,tile_in->ColorBuffer[i].c.g,tile_in->ColorBuffer[i].c.b,tile_in->ColorBuffer[i].c.a);
#endif
		}
		
	}
	
}

void ahmi::TexRaster(Tile* tile_in,Matrix* matrix,u32 addr ,u16 tex_width,u16 tex_height,u16 texture_ctrl,u32 InitColor,VERTEX_SET* tex_vertex)
{
	bte(tile_in,tex_vertex,texture_ctrl);
	uv_translate(tile_in,matrix,texture_ctrl);
	pixel_generate(tile_in,addr,tex_width,tex_height,texture_ctrl,InitColor,tex_vertex);
};
#endif

//-----------------------------
// 函数名： Write_Frame
// 写framebuffer
// 参数列表：
// @param1 FinalTile *finaltile       写回的tile
// @param2 FramBufferPtr FrameBuffer  framebuffer数组
// 备注(各个版本之间的修改):
// ...
//-----------------------------
#ifdef VERSION_1
void ahmi::Write_Frame(FinalTile *finaltile,FramBufferPtr FrameBuffer, u8 frameType, u8 bufferWidth)
{
   u32 i=0,j=0;
   u32 address;
   u32  x=(*finaltile).x>>4;
   u32  y=(*finaltile).y>>4;
   int  screen_rotate = SCREEN_ROTATE;
   int  screen_height;
   int  screen_width;
   int  x_temp;
   int  y_temp;
   u32  addr_write;
   addr_write = *( (u32*)(stm32info + addr_ahmi_0) );
#ifdef TILE_BUFFER_LOG_GEN
   tileBufferFile << " tile_x " << (int)x << " tile_y " << (int)y << std::endl;
#endif
if(addr_write == 0x1a80000) //write to static texture buffer
   {
		for (j=0;j<TILESIZE;j++)
			for (i=0;i<TILESIZE;i++)
			{
				if( (y+j >= globalArgs.ScreenHeigth) || (x+i >= globalArgs.ScreenWidth )) continue;
				address= 4 * ( (y+j)*globalArgs.ScreenWidth+x+i);
				//address=3*( (y+j) * (globalArgs.ScreenWidth)+ x + i);
				//*(FrameBuffer+address  ) = (*finaltile).TileBuffer[j*32+i].r & 0xf8;
				//*(FrameBuffer+address+1) = (*finaltile).TileBuffer[j*32+i].g & 0xfc;
				//*(FrameBuffer+address+2) = (*finaltile).TileBuffer[j*32+i].b & 0xf8;
				*(GlobalBackgroundBuffer+address+3) = 0xff;   //a
				*(GlobalBackgroundBuffer+address+2) = (*finaltile).TileBuffer[j*32+i].r;
				*(GlobalBackgroundBuffer+address+1) = (*finaltile).TileBuffer[j*32+i].g;
				*(GlobalBackgroundBuffer+address) = (*finaltile).TileBuffer[j*32+i].b;
			}
   }
   else if(frameType == 0)
   {
		for (j=0;j<TILESIZE;j++)
			for (i=0;i<TILESIZE;i++)
			{
				if(screen_rotate == 0)
				{
					screen_width = globalArgs.ScreenWidth;
					screen_height = globalArgs.ScreenHeigth;
					x_temp = x+i;
					y_temp = y + j;
				}
				else if(screen_rotate == 90)
				{
					screen_height = globalArgs.ScreenWidth;
					screen_width = globalArgs.ScreenHeigth;
					x_temp = y + j;
					y_temp =screen_height - (x + i) - 1;
				}
				if( (y_temp >= screen_height) || (x_temp >= screen_width)) continue;
				address=3*(( screen_height-1-(y_temp))*screen_width+x_temp);    //更改地址为左下角为坐标原点
				//address=3*( (y+j) * (globalArgs.ScreenWidth)+ x + i);
#ifdef FINAL_COLOR_16
				*(FrameBuffer+address  ) = (*finaltile).TileBuffer[j*32+i].r & 0xf8;
				*(FrameBuffer+address+1) = (*finaltile).TileBuffer[j*32+i].g & 0xfc;
				*(FrameBuffer+address+2) = (*finaltile).TileBuffer[j*32+i].b & 0xf8;
#endif
#ifdef FINAL_COLOR_32
				*(FrameBuffer+address  ) = (*finaltile).TileBuffer[j*32+i].r;
				*(FrameBuffer+address+1) = (*finaltile).TileBuffer[j*32+i].g;
				*(FrameBuffer+address+2) = (*finaltile).TileBuffer[j*32+i].b;
#endif
#ifdef TILE_BUFFER_LOG_GEN
				u32 finalColor;	
				finalColor = (((*finaltile).TileBuffer[j*32+i].r & 0xf8) << 16) + ( (((*finaltile).TileBuffer[j*32+i].g & 0xfc)) << 8 ) + (((*finaltile).TileBuffer[j*32+i].b & 0xf8) );
				tileBufferFile <<  std::setw(6) << std::hex << finalColor << std::endl;
#endif
				//(FrameBuffer+address)->r=(*finaltile).TileBuffer[j*32+i].r;
				//(FrameBuffer+address)->g=(*finaltile).TileBuffer[j*32+i].g;
				//(FrameBuffer+address)->b=(*finaltile).TileBuffer[j*32+i].b;
			}
   }
   else 
   {
	   for (j=0;j<TILESIZE;j++)
			for (i=0;i<TILESIZE;i++)
			{
				if( (y+j >= globalArgs.ScreenHeigth) || (x+i >= globalArgs.ScreenWidth )) continue;
				address=2 * ( (y+j)*globalArgs.ScreenWidth+x+i);
				//address=3*( (y+j) * (globalArgs.ScreenWidth)+ x + i);
#ifdef FINAL_COLOR_16
				*(u16*)( FrameBuffer + address) = (u16)(((*finaltile).TileBuffer[j*32+i].r & 0xf8) << 8);
				*(u16*)( FrameBuffer + address) += (u16)(((*finaltile).TileBuffer[j*32+i].g & 0xfc) << 3);
				*(u16*)( FrameBuffer + address) += (u16)(((*finaltile).TileBuffer[j*32+i].b & 0xfc) >> 3);
#endif
#ifdef FINAL_COLOR_32
				*(u16*)( FrameBuffer + address) = (u16)(((*finaltile).TileBuffer[j*32+i].r ) );
				*(u16*)( FrameBuffer + address) += (u16)(((*finaltile).TileBuffer[j*32+i].g) );
				*(u16*)( FrameBuffer + address) += (u16)(((*finaltile).TileBuffer[j*32+i].b) );
#endif
			}
   }
}
#endif

#ifdef VERSION_2
void ahmi::Write_Frame(FinalTile *finaltile,FramBufferPtr FrameBuffer)
{
   u32 i=0,j=0;
   u32 address;
   u32  x=(*finaltile).x>>4;
   u32  y=(*finaltile).y>>4;
   int addr;
   int addr_temp;
   int y_size;// = ((AHMI_REGFILE.FrameSize&0xffff0000)>>16)-x;
   int x_size;// = (AHMI_REGFILE.FrameSize&0xffff)-y;
   int x_cut;
   int y_cut;
   int FrameSize = AHMI_REGFILE.FrameSize;
   if(AHMI_REGFILE.OUPUT_ROT==0)
   {
   
   }
   else if(AHMI_REGFILE.OUPUT_ROT==1)
   {
	   FrameSize = ((FrameSize&0xffff)<<16) + (FrameSize>>16);
   }
   else if(AHMI_REGFILE.OUPUT_ROT==2)
   {
   
   }
   else if(AHMI_REGFILE.OUPUT_ROT==3)
   {
	   FrameSize = ((FrameSize&0xffff)<<16) + (FrameSize>>16);
   }

   if(AHMI_REGFILE.OUPUT_ROT==0)
   {
	   if(y+31 < (FrameSize&0xffff0000)>>16) 
	   {
			y_cut = 0;
	   }
	   else
		   y_cut = 1;
	   if(x+31 < (FrameSize&0xffff))
		   x_cut = 0;
	   else
		x_cut = 1;
	   {
		 y_size = y_cut ? (((FrameSize&0xffff0000)>>16)-y)%TILESIZE : TILESIZE;
		 x_size = x_cut ? ((FrameSize&0xffff)-x)%TILESIZE : TILESIZE;
	   }
   }
   else if(AHMI_REGFILE.OUPUT_ROT==1)
   {
	   if(x+31 < (FrameSize&0xffff0000)>>16) 
	   {
			y_cut = 0;
	   }
	   else
		   y_cut = 1;
	   if(y+31 < (FrameSize&0xffff))
		   x_cut = 0;
	   else
		x_cut = 1;
		 x_size = y_cut ? (((FrameSize&0xffff0000)>>16)-x)%TILESIZE : TILESIZE;
		 y_size= x_cut ? ((FrameSize&0xffff)-y)%TILESIZE : TILESIZE;
   }
   else if(AHMI_REGFILE.OUPUT_ROT==2)
   {
	   if(y+31 < (FrameSize&0xffff0000)>>16) 
	   {
			y_cut = 0;
	   }
	   else
		   y_cut = 1;
	   if(x+31 < (FrameSize&0xffff))
		   x_cut = 0;
	   else
		x_cut = 1;
		 y_size = y_cut ? (((FrameSize&0xffff0000)>>16)-y)%TILESIZE : TILESIZE;
		 x_size = x_cut ? ((FrameSize&0xffff)-x)%TILESIZE : TILESIZE;
   }
   else if(AHMI_REGFILE.OUPUT_ROT==3)
   {
	   if(x+31 < (FrameSize&0xffff0000)>>16) 
	   {
			y_cut = 0;
	   }
	   else
		   y_cut = 1;
	   if(y+31 < (FrameSize&0xffff))
		   x_cut = 0;
	   else
		x_cut = 1;
	   	 x_size = y_cut ? (((FrameSize&0xffff0000)>>16)-x)%TILESIZE : TILESIZE;
		 y_size= x_cut ? ((FrameSize&0xffff)-y)%TILESIZE : TILESIZE;
   }
   	/*for (j=0;j<TILESIZE;j++)
   		for (i=0;i<TILESIZE;i++)
		{
			if( (y+j >= globalArgs.ScreenHeigth) || (x+i >= globalArgs.ScreenWidth)) continue;
			address=3*((globalArgs.ScreenHeigth-1-(y+j))*globalArgs.ScreenWidth+x+i);
			*(FrameBuffer+address) = (*finaltile).TileBuffer[j*32+i].r   ;//& 0xf8;
			*(FrameBuffer+address+1) = (*finaltile).TileBuffer[j*32+i].g ;//& 0xfc;
			*(FrameBuffer+address+2) = (*finaltile).TileBuffer[j*32+i].b ;//& 0xf8;
			//(FrameBuffer+address)->r=(*finaltile).TileBuffer[j*32+i].r;
			//(FrameBuffer+address)->g=(*finaltile).TileBuffer[j*32+i].g;
			//(FrameBuffer+address)->b=(*finaltile).TileBuffer[j*32+i].b;
		}*/
   if(AHMI_REGFILE.OUPUT_ROT==0)
   {
   		for (j=0;j<y_size;j++)
   			for (i=0;i<x_size;i++)
			{
#ifdef OPENGL_RENDERING
				address=3*( ( ( (FrameSize&0xffff0000)>>16)-1-(y+j) ) * (FrameSize&0xffff)+x+i);
				if(AHMI_REGFILE.OUTPUFORMAT==0)
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[j*32+i].r   ;//& 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[j*32+i].g ;//& 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[j*32+i].b ;//& 0xf8;
				}
				else
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[j*32+i].r   & 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[j*32+i].g & 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[j*32+i].b & 0xf8;
				}
#endif
#ifdef OPENCV_RENDERING
				address=3*( (y+j)*(FrameSize&0xffff)+x+i );
				if(AHMI_REGFILE.OUTPUFORMAT==0)
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[j*32+i].b   ;//& 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[j*32+i].g ;//& 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[j*32+i].r ;//& 0xf8;
				}
				else
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[j*32+i].b   & 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[j*32+i].g & 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[j*32+i].r & 0xf8;
				}
#endif
			}
   }
   else if(AHMI_REGFILE.OUPUT_ROT==1)
   {
	   addr = 0;
	   for(i=0;i<y_size;i++)
		   for(j=0;j<x_size;j++)
	   {
#ifdef OPENGL_RENDERING
		   addr = i*32+j;
		   address=3*((((FrameSize&0xffff0000)>>16)-x-j-1)*(FrameSize&0xffff)+y+i);
				if(AHMI_REGFILE.OUTPUFORMAT==0)
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[addr].r   ;//& 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[addr].g ;//& 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[addr].b ;//& 0xf8;
				}
				else
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[addr].r   & 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[addr].g & 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[addr].b & 0xf8;
				}
			
#endif

#ifdef OPENCV_RENDERING
		   addr = i*32+j;
		   address=3*((((FrameSize&0xffff0000)>>16)-x-j-1)*(FrameSize&0xffff)+y+i);
				if(AHMI_REGFILE.OUTPUFORMAT==0)
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[addr].b   ;//& 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[addr].g ;//& 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[addr].r ;//& 0xf8;
				}
				else
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[addr].b   & 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[addr].g & 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[addr].r & 0xf8;
				}
#endif
	   }
   }
   else if(AHMI_REGFILE.OUPUT_ROT==2)
   {
	   for(i=0;i<y_size;i++)
		   for(j=0;j<x_size;j++) 
	   {
#ifdef OPENGL_RENDERING
		   addr = (y_size-i-1)*32+(x_size-j-1);
		    address=3*(( (FrameSize >> 16) - (y+y_size - (i)))*(FrameSize&0xffff)+((FrameSize&0xffff)-x-(x_size-j)));
				if(AHMI_REGFILE.OUTPUFORMAT==0)
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[addr].r   ;//& 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[addr].g ;//& 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[addr].b ;//& 0xf8;
				}
				else
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[addr].r   & 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[addr].g & 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[addr].b & 0xf8;
				}
#endif
#ifdef OPENCV_RENDERING
		   addr = (y_size-i-1)*32+(x_size-j-1);
		    address=3*(( (FrameSize >> 16) - (y+y_size - (i)))*(FrameSize&0xffff)+((FrameSize&0xffff)-x-(x_size-j)));
		    	if(AHMI_REGFILE.OUTPUFORMAT==0)
		    	{
		    		*(FrameBuffer+address) = (*finaltile).TileBuffer[addr].b   ;//& 0xf8;
		    		*(FrameBuffer+address+1) = (*finaltile).TileBuffer[addr].g ;//& 0xfc;
		    		*(FrameBuffer+address+2) = (*finaltile).TileBuffer[addr].r ;//& 0xf8;
		    	}
		    	else
		    	{
		    		*(FrameBuffer+address) = (*finaltile).TileBuffer[addr].b   & 0xf8;
		    		*(FrameBuffer+address+1) = (*finaltile).TileBuffer[addr].g & 0xfc;
		    		*(FrameBuffer+address+2) = (*finaltile).TileBuffer[addr].r & 0xf8;
		    	}
#endif
	   }
   }
   else if(AHMI_REGFILE.OUPUT_ROT==3)
   {
	   

	   for(i=0;i<y_size;i++)
		   for(j=0;j<x_size;j++)
	   {
#ifdef OPENGL_RENDERING
		   addr = (y_size-i-1)*32+(x_size-j-1);
			int  x_tran = (FrameSize&0xffff)-(y+(y_size-i));
			int y_tran = x+(x_size-j-1);
		    address=3*(y_tran * (FrameSize&0xffff)+x_tran);
				if(AHMI_REGFILE.OUTPUFORMAT==0)
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[addr].r   ;//& 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[addr].g ;//& 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[addr].b ;//& 0xf8;
				}
				else
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[addr].r   & 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[addr].g & 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[addr].b & 0xf8;
				}
#endif
#ifdef OPENCV_RENDERING
		   addr = (y_size-i-1)*32+(x_size-j-1);
			int  x_tran = (FrameSize&0xffff)-(y+(y_size-i));
			int y_tran = x+(x_size-j-1);
		    address=3*(y_tran * (FrameSize&0xffff)+x_tran);
				if(AHMI_REGFILE.OUTPUFORMAT==0)
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[addr].b   ;//& 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[addr].g ;//& 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[addr].r ;//& 0xf8;
				}
				else
				{
					*(FrameBuffer+address) = (*finaltile).TileBuffer[addr].b   & 0xf8;
					*(FrameBuffer+address+1) = (*finaltile).TileBuffer[addr].g & 0xfc;
					*(FrameBuffer+address+2) = (*finaltile).TileBuffer[addr].r & 0xf8;
				}

#endif
	   }
   }
#ifdef TEST_LOG_GEN
	CString CSFilename;
	CreateDirectory(_T("TB_log"),NULL);
	CSFilename.Format(_T("TB_log\\%d_%d_TB.log"),finaltile->x,finaltile->y);
	std::string str((LPCSTR)CStringA(CSFilename));
	char* text = (char *)str.c_str();
	FILE * fp;
	fp = fopen(text,"w");
	for(int i=0;i<TILESIZE* TILESIZE;i++)
		{
			fprintf(fp,"%04x %04x %02x %02x %02x %02x\n",finaltile->x + i%32 * 16,finaltile->y + i/32 * 16,finaltile->TileBuffer[i].r,finaltile->TileBuffer[i].g,finaltile->TileBuffer[i].b,finaltile->TileBuffer[i].a);
		}
	fclose(fp);
#endif
}
#endif
//-----------------------------
// 函数名： ahmi::DrawFrame
// 绘制一帧
// 参数列表：
// @param1 FramBufferPtr FrameBuffer  framebuffer数组
// 备注(各个版本之间的修改):
// ...
//-----------------------------
#ifdef VERSION_1
void ahmi::DrawFrame()//modified by darydou 
{
	//read frame buffer address from regs
	FramBufferPtr FrameBuffer = (FramBufferPtr)( AHMI_REGFILE.addr_ahmi_l + (AHMI_REGFILE.addr_ahmi_h << 16) );
	u16 tile_x = 0, tile_y = 0;
	u16 tile_x_num = 0, tile_y_num = 0;
	//for the sourcebuffer
	u8 headerfirst,headersecond,headerthird,headerfourth;
	u8  SB_texType, SB_Mask, SB_Matrix, SB_AddrType,SB_End,SB_MaskType;
	u8  texCtrl;
	u8  start_x, start_y, stop_x, stop_y;
	u8  drawingStartX, drawingStartY, drawingStopX, drawingStopY;
	u16 width, height;
	u32 addr = 0;
	u32 color = 0;
	u16 suffershift;
	//u32 addr_ahmi = ( (stm32info[Addr_combine]) * (ADDR_MULTI_BASE) );//Addr_combine[2:0] * 0x200000
	//u8 buffer_width = stm32info[BUFFER_WIDTH];
 	Tile uutTile;
	Matrix uutMatrix;
	int PRINTFLAG = 1;
	#if  __PRINTDEBUG__//add by darydou 3/4
	    int printcount = 0;
	    char textureformat[10];
	    char  buffer[200];
		ERROR_PRINT("The flash boxsize ,as follow:\r\n");
		WriteText(buffer,"tile_start_x is %d \r\n tile_end_x is %d \r\n tile_start_y is %d \r\n tile_end_y is %d \r\n",\
			stm32info[Tile_start_x],stm32info[Tile_end_x],stm32info[Tile_start_y],stm32info[Tile_end_y]);
		ERROR_PRINT(buffer);
	#endif

	drawingStartX = AHMI_REGFILE.tileStartYX % 256;
	drawingStartY = AHMI_REGFILE.tileStartYX / 256;
	drawingStopX = AHMI_REGFILE.tileEndYX % 256;
	drawingStopY = AHMI_REGFILE.tileEndYX / 256;

	for (tile_y_num = drawingStartY; tile_y_num <= drawingStopY; tile_y_num++)
		for (tile_x_num = drawingStartX; tile_x_num <= drawingStopX; tile_x_num++)
			if(tile_x_num >= 0 && tile_x_num <TILE_NUM_X && tile_y_num >= 0 && tile_y_num < TILE_NUM_Y)
			{

				//tile_x = tile_x_num << 4;//12.4定点数
				//tile_y = tile_y_num << 4;//12.4定点数
				tile_x = tile_x_num * TILESIZE * 16;//12.4定点数
				tile_y = tile_y_num * TILESIZE * 16;//12.4定点数
				InitTile(&uutTile, tile_x, tile_y);
				InitFinalTile(&theFinalTile, tile_x, tile_y);
				suffershift = 0;
				//if (flag == 1)
				//{//将已经刷新的图片作为新的纹理
				//	//由于最终存储的纹理为BMP888格式的，但AHMI没有处理BMP888格式
				//	SB_texType = 8; //1000
				//	SB_Mask = 0;
				//	SB_MaskType = 0;
				//	texCtrl = (SB_texType << 1) + SB_Mask;
				//	//modefied by xt 20150506
				//	//ABCD为1.6.9定点数 EF为12.4定点数，默认未做变化为单位矩阵
				//	uutMatrix.A = 1<<9; 
				//	uutMatrix.B = 0;
				//	uutMatrix.C = 0;
				//	uutMatrix.D = uutMatrix.A;
				//	uutMatrix.E = 0;
				//	uutMatrix.F = 0;
				//	width = screen_width;
				//	height = screen_height;
				//	color = 0;
				//	addr = (u32)FrameBuffer;
				//	TexRaster(&uutTile, &uutMatrix, addr, width, height, texCtrl, color,SB_MaskType);
				//	if (uutTile.color_done)
				//		color_blending(&theFinalTile, &uutTile);
				//}
			
				while(suffershift < SoureBufferSize)
				{
					headerfirst = sourceBuffer[suffershift++];
					headersecond = sourceBuffer[suffershift++];
					headerthird = sourceBuffer[suffershift++];
					headerfourth = sourceBuffer[suffershift++];
					//headerfirst为8bits，各比特位包含纹理的相关信息，对应关系如下
					//headerfirst： SB_texType(7:4) SB_Mask(3) SB_Matrix(2:1) SB_End(0)
					SB_texType = (headerfirst & TEXTYPEMASK) >> 4;
					SB_Mask = (headerfirst & MASKBIT) >> 3;   
					SB_Matrix = (headerfirst & ABCDEFMATRIX) >> 1;
					SB_End = headerfirst & 0x1;
					//headersecond为8bits，各比特位包含纹理的相关信息，对应关系如下
					//headersecond： Resaved(7:3) SB_MaskType(2:1) SB_AddrType(0)
					SB_AddrType = headersecond & 0x1;
					SB_MaskType = (headersecond>>1) & 0x3;
					texCtrl = (SB_texType << 1) + SB_Mask;
					
					if (SB_End == 1 || suffershift>=SoureBufferSize)
						break;
					//analyze the boxSize
					start_x = sourceBuffer[suffershift++];
					start_y = sourceBuffer[suffershift++];
					stop_x = sourceBuffer[suffershift++];
					stop_y = sourceBuffer[suffershift++];

					//analyze the texSize
					width = (s16)*((s16 *)(sourceBuffer + suffershift));
					suffershift += 2;
					height = (s16)*((s16 *)(sourceBuffer + suffershift));
					suffershift += 2;

					//analyze the matrix
					if (SB_Matrix == 0)
					{//add by xt 15/3/31
						//right
						uutMatrix.E = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.F = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.A = uutMatrix.D = 0x100000;//1.11.20
						uutMatrix.B = uutMatrix.C = 0;
					}
					else if (SB_Matrix == 1)
					{//add by xt 15/3/31
						//right
						uutMatrix.A = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.B = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.C = -uutMatrix.B;
						uutMatrix.D = uutMatrix.A ;
						uutMatrix.E = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.F = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
					}
					else if(SB_Matrix == 2)
					{//add by xt 15/3/31
						uutMatrix.A = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.B =  0;
						uutMatrix.C =  0;
						uutMatrix.D = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.E = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.F = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;					
					}
					else
					{//add by xt 15/3/31
						//right
						uutMatrix.A = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.B = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.C = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.D = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.E = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						uutMatrix.F = (s32)*((s32 *)(sourceBuffer + suffershift));
						suffershift += 4;
					}

					//analyze the addr_type
					if (SB_AddrType == 1)
					{//modified by xt 15/5/19
						addr = (u32)*((u32 *)(sourceBuffer + suffershift));
						suffershift += 4;
						color = addr;
					}

					/**************************************************************/
					//打印输入数据 add by darydou 3/4
					#if __PRINTDEBUG__
					if (PRINTFLAG){
						//打印纹理的个数
						printcount++;
						WriteText(buffer, "This is the %d texture!\r\n", printcount);
						ERROR_PRINT(buffer);
						//打印纹理的格式
						switch (SB_texType)
						{
						case SIM_DXT1:
							strcpy(textureformat, "DXT1");
							break;
						case SIM_DXT3: 
							strcpy(textureformat, "DXT3");
							break;
						case SIM_ALPHA8:
							strcpy(textureformat, "ALPHA8");
							break;
						case SIM_PURECOLOR:
							strcpy(textureformat, "COLOR");
							break;
						case SIM_RGB565:
							strcpy(textureformat, "BMP565");
							break;
						case SIM_RGBA8888:
							strcpy(textureformat, "BMP8888");
							break;
						case SIM_Y8:
							strcpy(textureformat, "Y8");
							break;
						case SIM_YUV422:
							strcpy(textureformat, "YUV422");
							break;
						case SIM_ALPHA1:
							strcpy(textureformat, "ALPHA1");
							break;
						case SIM_PUREMASK:
							strcpy(textureformat, "PUREMASK");
							break;
						default:
							strcpy(textureformat, "erro!!!");
							break;
						}
						WriteText(buffer, "The texture format is %s.\r\n", textureformat);
						ERROR_PRINT(buffer);
						//打印是否做了掩膜
						if (SB_Mask == 0)
							ERROR_PRINT("The texture have no mask.\r\n");
						else
							ERROR_PRINT("The texture have mask.\r\n");
						//打印包围盒
						ERROR_PRINT("The texture boxsize have any tile,but not one.As follows:\r\n");
						WriteText(buffer, "tile_start_x is %d,\r\n", start_x);
						ERROR_PRINT(buffer);
						WriteText(buffer, "tile_stop_x  is %d,\r\n", stop_x);
						ERROR_PRINT(buffer);
						WriteText(buffer,"tile_start_y  is %d,\r\n", start_y);
						ERROR_PRINT(buffer);
						WriteText(buffer,"tile_stop_y   is %d.\r\n", stop_y);
						ERROR_PRINT(buffer);
						//打印矩阵信息
						if (SB_Matrix == 0){
							ERROR_PRINT("The matrix only have E and F.\r\nMatrix as follows:\r\n");
						}
						else if (SB_Matrix == 1){
							ERROR_PRINT("The matrix only have A and B, E and F(.\r\nMatrix as follows:\r\n");
						}
						else if (SB_Matrix == 2){
							ERROR_PRINT("The matrix only have A and D, E and F(.\r\nMatrix as follows:\r\n");
						}
						else if (SB_Matrix == 3){
							ERROR_PRINT("The matrix have all A,B,C,D,E and F.\r\nMatrix as follows:\r\n");
						}
						//else{
						//	ERROR_PRINT("The matrix is erro !!!\r\n");
						//}
						WriteText(buffer, " A is %d \r\n B is %d\r\n C is %d\r\n D is %d\r\n E is %d \r\n F is %d .\r\n",
							uutMatrix.A, uutMatrix.B, uutMatrix.C, uutMatrix.D, uutMatrix.E, uutMatrix.F);
						ERROR_PRINT(buffer);
						////打印纹理的大小宽和高
						//if (SB_TexSize == 0){
						//	ERROR_PRINT("The texsize is lower than 256.\r\n");
						//}
						//else{
						//	ERROR_PRINT("The texsize is more than 256.\r\n");
						//}
						WriteText(buffer, "The texsize width is %d,\r\n height is %d\r\n", width, height);
						ERROR_PRINT(buffer);
						//打印是地址还是颜色纹理
						if (SB_AddrType == 0){
							ERROR_PRINT("The texture has no addresss!\r\n");
						}
						else{
							ERROR_PRINT("The texture have a addr!\r\n");
							WriteText(buffer, "The texture addr is 0x%.8x!\r\n", addr);
							ERROR_PRINT(buffer);
						}
						////简单的错误判断
						////1.一个纹理的纹理格式标记是颜色纹理，但地址标记显示它具有地址而不是具有三颜色，纹理文件头有误
						////2.一个纹理的纹理格式标记是非颜色纹理，但地址标记显示它具有三颜色而不是纹理的存储地址，纹理文件头有误
						//if (SB_AddrType&&SB_texType == 3){
						//	ERROR_PRINT("ERROR: The texture textype is  color ,but its textype is addr!!!\r\nThe textureheader may be wrong!!!\r\n	Please check it!!!");
						//}
						//if (!SB_AddrType&&SB_texType != 3){
						//	ERROR_PRINT("ERROR: The texture textype is not  color ,but its textype is color!!!\r\nThe textureheader may be wrong!!!\r\n Please check it!!!");
						//}
						//ERROR_PRINT("\r\n"); 
						//ERROR_PRINT("\r\n");
					}
					#endif
					/**************************************************************/
					//modified by xt 20150506
					if ((tile_x_num < start_x || tile_x_num > stop_x) || (tile_y_num < start_y || tile_y_num > stop_y))
						continue;

					TexRaster(&uutTile, &uutMatrix, addr, width, height, texCtrl, color,SB_MaskType);
#ifdef TEX_RAS_LOG_GEN
					texRasFile << "tile_x " << tile_x_num << " tile_y " << tile_y_num << " texCtrl " << std::setw(4) << std::hex << (int)(texCtrl) << std::endl;
					for(int i = 0; i < TilePixelSize; i++)
					{
						texRasFile << std::setw(2) << std::hex << (int)(uutTile.ColorBuffer[i].a) << " ";
						texRasFile << std::setw(2) << std::hex << (int)(uutTile.ColorBuffer[i].r) << " ";
						texRasFile << std::setw(2) << std::hex << (int)(uutTile.ColorBuffer[i].g) << " ";
						texRasFile << std::setw(2) << std::hex << (int)(uutTile.ColorBuffer[i].b) << std::endl;
					}
#endif
					if (uutTile.color_done)
						color_blending(&theFinalTile, &uutTile);
#ifdef COLOR_BLENDING_LOG_GEN
					colorBlendingFile << "tile_x " << tile_x_num << " tile_y " << tile_y_num << " texCtrl " << std::setw(4) << std::hex << (int)(texCtrl) << std::endl;
					for(int i = 0; i < TilePixelSize; i++)
					{
						colorBlendingFile << std::setw(2) << std::hex << (int)(theFinalTile.TileBuffer[i].r) << " ";
						colorBlendingFile << std::setw(2) << std::hex << (int)(theFinalTile.TileBuffer[i].g) << " ";
						colorBlendingFile << std::setw(2) << std::hex << (int)(theFinalTile.TileBuffer[i].b) << std::endl;
					}
#endif
				}//end of searching every source buffer element
				PRINTFLAG = 0;
				Write_Frame(&theFinalTile, FrameBuffer,0, 0);

#ifdef TEST_LOG_GEN
			FILE *fp_final;
			FILE *fp_vga;
			FILE *fp_vga_16bit;
			fp_final = fopen("ahmi_final.log","w");
			fp_vga   = fopen("vga.log","w");
			fp_vga_16bit   = fopen("vga_16bit.log","w");
			if(fp_final == NULL || fp_vga == NULL)
				return;
			for(int ii=0;ii<(AHMI_REGFILE.FrameSize&0xffff) * ((AHMI_REGFILE.FrameSize&0xffff0000)>>16);ii++)
			{
				int num = (ii%(AHMI_REGFILE.FrameSize&0xffff)) + (((AHMI_REGFILE.FrameSize&0xffff0000)>>16) - 1 - ii/(AHMI_REGFILE.FrameSize&0xffff))*(AHMI_REGFILE.FrameSize&0xffff);
				fprintf(fp_final,"%04x %04x %02x %02x %02x\n",(ii%(AHMI_REGFILE.FrameSize&0xffff)) * 16,ii/(AHMI_REGFILE.FrameSize&0xffff)*16,FrameBuffer[3*num+0],FrameBuffer[3*num+1],FrameBuffer[3*num+2]);
				fprintf(fp_vga,"%02x %02x %02x\n",FrameBuffer[3*num+0],FrameBuffer[3*num+1],FrameBuffer[3*num+2]);
				fprintf(fp_vga_16bit,"%02x %02x %02x\n",((FrameBuffer[3*num+0]&0xF8) | ((FrameBuffer[3*num+0]&0xE0)>>5)),((FrameBuffer[3*num+1]&0xFC) | ((FrameBuffer[3*num+1]&0xC0)>>6)),((FrameBuffer[3*num+2]&0xF8) | ((FrameBuffer[3*num+2]&0xE0)>>5)));
			}
			fclose(fp_final);
			fclose(fp_vga);
			fclose(fp_vga_16bit);
#endif

				int m=0;
			}//end of one frame

#ifdef FRAME_BUFFER_LOG_GEN

//#if (NUM_OF_CORE==1)
			if(this->frame_log_gen_flag == 0)
			{
	            //frameLogFile.open("testLog\\frame.log",std::ios::out);
	            for(int y_count = 0; y_count < globalArgs.ScreenHeigth; y_count++)
	            	for(int x_count = 0; x_count < globalArgs.ScreenWidth; x_count++)
	            	{
	            		u64 address= 3 * ( (globalArgs.ScreenHeigth - y_count - 1)*globalArgs.ScreenWidth+x_count);
	            		u32 temp_color_r = *(FrameBuffer+address  );
	            		u32 temp_color_g = *(FrameBuffer+address + 1 );
	            		u32 temp_color_b = *(FrameBuffer+address + 2 );
	            		u32 temp_color = (temp_color_r << 16) + (temp_color_g << 8) + temp_color_b;
	            		frameLogFile.width(6);
	            		frameLogFile.fill('0');
	            		frameLogFile << std::hex << temp_color << std::endl;
	            	}
	            frameLogFile.close();
			}
			
//#endif //core number 1

#if (NUM_OF_CORE==2)
			
			
		    if(this->frame_log_gen_flag == 0)
			{
				frameLogFileEven.open("testLog\\__ahmi_0.log",std::ios::out);
		    	frameLogFileOdd.open("testLog\\__ahmi_1.log",std::ios::out);
	            //frameLogFile.open("testLog\\frame.log",std::ios::out);
	            for(int y_count = 0; y_count < globalArgs.ScreenHeigth; y_count++)
	            	for(int x_count = 0; x_count < globalArgs.ScreenWidth; x_count++)
	            	{
						int tile_x_num_count = x_count/32;
						int EvenFlag = (tile_x_num_count%2 == 0);//1 is for log0
	            		u64 address= 3 * ( (globalArgs.ScreenHeigth - y_count - 1)*globalArgs.ScreenWidth+x_count);
	            		u32 temp_color_r = *(FrameBuffer+address  );
	            		u32 temp_color_g = *(FrameBuffer+address + 1 );
	            		u32 temp_color_b = *(FrameBuffer+address + 2 );
	            		u32 temp_color = (temp_color_r << 16) + (temp_color_g << 8) + temp_color_b;
						if(EvenFlag == 1)
						{
	            		    frameLogFileEven.width(6);
	            		    frameLogFileEven.fill('0');
	            		    frameLogFileEven << std::hex << temp_color << std::endl;

	            		    frameLogFileOdd << "xxxxxx" << std::endl;
						}
						else if(EvenFlag == 0)
						{
							frameLogFileOdd.width(6);
	            		    frameLogFileOdd.fill('0');
	            		    frameLogFileOdd << std::hex << temp_color << std::endl;
							frameLogFileEven << "xxxxxx" << std::endl;
						}
	            	}
	            frameLogFileEven.close();
				frameLogFileOdd.close();
			}
			this->frame_log_gen_flag = 1; 
#endif// core number 2
			this->frame_log_gen_flag = 1;

#endif//frame buffer log gen
	FrameRate++;
#ifdef TEST_LOG_GEN
#ifdef TEX_RAS_LOG_GEN
	texRasFile.close();
#endif
#ifdef COLOR_BLENDING_LOG_GEN
	colorBlendingFile.close();
#endif
#ifdef TILE_BUFFER_LOG_GEN
	tileBufferFile.close();
#endif
#ifdef UV_LOG_GEN
	uvLogFile.close();
#endif
#endif
	AHMI_REGFILE.STATUS=1;
}
#endif

#ifdef VERSION_2
void ahmi::DrawFrame()
{
	u16 tile_x = 0, tile_y = 0;
	u16 tile_x_num = 0, tile_y_num = 0;
	u8  texCtrl;
	u8  start_x, start_y, stop_x, stop_y;
	u8  headerfirst,headersecond,headerthird,headerfourth;
	u16 width, height;
	u32 addr, color;
	u32 suffershift;
 	Tile uutTile;
	Matrix uutMatrix;
	VERTEX_SET tex_vertex;
	u16 tex_width,tex_heigth;
	u32 texture_ctrl;
	u32 InitColor;
	//u8 MaskType;
	u32 AreaRcp;
	bool endflag = 0;
	float	result;
	int		int_r = 0;
	lut_rom[0] = 0;
#ifdef TEST_LOG_GEN
	fp_pge = fopen("pge.log","w");
	fp_uv_division = fopen("division.log","w");
	fp_uv = fopen("uv_translate.log","w");
	fp_cb=  fopen("cb.log","w");
#endif
	//AHMI_REGFILE.DrawBox=0x2020000;
	//AHMI_REGFILE.FB_ADDR =0;
	//AHMI_REGFILE.FrameSize=0x00400040;
	//AHMI_REGFILE.MaxSBNum = 255;
	//AHMI_REGFILE.OUPUT_ROT=0;
	//AHMI_REGFILE.OUTPUFORMAT=0;
	int draw_startx=(AHMI_REGFILE.DrawBox&0xff);
	int draw_starty=(AHMI_REGFILE.DrawBox&0xff00)>>8;
	int draw_stopx=(AHMI_REGFILE.DrawBox&0xff0000)>>16;
	int draw_stopy=(AHMI_REGFILE.DrawBox&0xff000000)>>24;
	FramBufferPtr FrameBuffer = (FramBufferPtr)AHMI_REGFILE.FB_ADDR;
	u8 *sourceBuffer = (u8*)AHMI_REGFILE.SB_ADDR;
	for(int j=1;j<256;j++)
	{
		result =(float)255 / (float)j;
		int e = 0;
		while(result>2)
		{
			result = result/2;
			e = e + 1;
		}
		float w = result -1 ;
		w = w * 128;
		int_r  = (int)w+e*128;
		lut_rom[j] = int_r;
	}
	//int PRINTFLAG = 1;
#ifdef TEST_LOG_GEN
		FILE *fp;
		fp = fopen("TB.log","w");
#endif
		for (tile_y_num = draw_starty; tile_y_num <= draw_stopy; tile_y_num++)
		for (tile_x_num = draw_startx; tile_x_num <= draw_stopx; tile_x_num++)
//		for (tile_y_num = stm32info[Tile_start_y]; tile_y_num <= stm32info[Tile_end_y]; tile_y_num++)
			if(tile_x_num >= 0 && tile_x_num <(((AHMI_REGFILE.FrameSize&0xffff) + TILESIZE-1)/TILESIZE) && tile_y_num >= 0 && tile_y_num <  ((((AHMI_REGFILE.FrameSize&0xffff0000)>>16) + TILESIZE-1)/TILESIZE))
			{
				bte_counter = 0;
				uv_counter = 0;
				pge_counter = 0;
				cb_counter = 0;

				//tile_x = tile_x_num << 4;//12.4定点数
				//tile_y = tile_y_num << 4;//12.4定点数
				tile_x = tile_x_num * TILESIZE * 16;//12.4定点数
				tile_y = tile_y_num * TILESIZE * 16;//12.4定点数
				InitTile(&uutTile, tile_x, tile_y);
				InitFinalTile(&theFinalTile, tile_x, tile_y);
				suffershift = 0;
				endflag = 0;
				while(!endflag)
				{
					headerfirst = sourceBuffer[suffershift++];
					headersecond = sourceBuffer[suffershift++];
					headerthird = sourceBuffer[suffershift++];
					headerfourth = sourceBuffer[suffershift++];
					//texture_ctrl=sourceBuffer[suffershift].tex_type;
					if((headerfourth & 0x80) == 0)
					{
						tex_vertex.v0.cor.y = sourceBuffer[suffershift++];
						tex_vertex.v0.cor.y = (sourceBuffer[suffershift++] << 8) | tex_vertex.v0.cor.y;
						tex_vertex.v0.cor.x = sourceBuffer[suffershift++];
						tex_vertex.v0.cor.x = (sourceBuffer[suffershift++] << 8) | tex_vertex.v0.cor.x; 
						tex_vertex.v1.cor.y = sourceBuffer[suffershift++];
						tex_vertex.v1.cor.y = (sourceBuffer[suffershift++] << 8) | tex_vertex.v1.cor.y;
						tex_vertex.v1.cor.x = sourceBuffer[suffershift++];
						tex_vertex.v1.cor.x = (sourceBuffer[suffershift++] << 8) | tex_vertex.v1.cor.x; 
						tex_vertex.v2.cor.y = sourceBuffer[suffershift++];
						tex_vertex.v2.cor.y = (sourceBuffer[suffershift++] << 8) | tex_vertex.v2.cor.y;
						tex_vertex.v2.cor.x = sourceBuffer[suffershift++];
						tex_vertex.v2.cor.x = (sourceBuffer[suffershift++] << 8) | tex_vertex.v2.cor.x; 
						tex_vertex.v3.cor.y = sourceBuffer[suffershift++];
						tex_vertex.v3.cor.y = (sourceBuffer[suffershift++] << 8) | tex_vertex.v3.cor.y;
						tex_vertex.v3.cor.x = sourceBuffer[suffershift++];
						tex_vertex.v3.cor.x = (sourceBuffer[suffershift++] << 8) | tex_vertex.v3.cor.x; 

						uutMatrix.A = sourceBuffer[suffershift++];
						uutMatrix.A = (sourceBuffer[suffershift++] << 8)  | uutMatrix.A;
						uutMatrix.A = (sourceBuffer[suffershift++] << 16) | uutMatrix.A;
						uutMatrix.A = (sourceBuffer[suffershift++] << 24) | uutMatrix.A;
						uutMatrix.B = sourceBuffer[suffershift++];
						uutMatrix.B = (sourceBuffer[suffershift++] << 8)  | uutMatrix.B;
						uutMatrix.B = (sourceBuffer[suffershift++] << 16) | uutMatrix.B;
						uutMatrix.B = (sourceBuffer[suffershift++] << 24) | uutMatrix.B;
						uutMatrix.C = sourceBuffer[suffershift++];
						uutMatrix.C = (sourceBuffer[suffershift++] << 8)  | uutMatrix.C;
						uutMatrix.C = (sourceBuffer[suffershift++] << 16) | uutMatrix.C;
						uutMatrix.C = (sourceBuffer[suffershift++] << 24) | uutMatrix.C;
						uutMatrix.D = sourceBuffer[suffershift++];
						uutMatrix.D = (sourceBuffer[suffershift++] << 8)  | uutMatrix.D;
						uutMatrix.D = (sourceBuffer[suffershift++] << 16) | uutMatrix.D;
						uutMatrix.D = (sourceBuffer[suffershift++] << 24) | uutMatrix.D;
						uutMatrix.E = sourceBuffer[suffershift++];
						uutMatrix.E = (sourceBuffer[suffershift++] << 8)  | uutMatrix.E;
						uutMatrix.E = (sourceBuffer[suffershift++] << 16) | uutMatrix.E;
						uutMatrix.E = (sourceBuffer[suffershift++] << 24) | uutMatrix.E;
						uutMatrix.F = sourceBuffer[suffershift++];
						uutMatrix.F = (sourceBuffer[suffershift++] << 8)  | uutMatrix.F;
						uutMatrix.F = (sourceBuffer[suffershift++] << 16) | uutMatrix.F;
						uutMatrix.F = (sourceBuffer[suffershift++] << 24) | uutMatrix.F;
						uutMatrix.G = sourceBuffer[suffershift++];
						uutMatrix.G = (sourceBuffer[suffershift++] << 8)  | uutMatrix.G;
						uutMatrix.G = (sourceBuffer[suffershift++] << 16) | uutMatrix.G;
						uutMatrix.G = (sourceBuffer[suffershift++] << 24) | uutMatrix.G;
						uutMatrix.H = sourceBuffer[suffershift++];
						uutMatrix.H = (sourceBuffer[suffershift++] << 8)  | uutMatrix.H;
						uutMatrix.H = (sourceBuffer[suffershift++] << 16) | uutMatrix.H;
						uutMatrix.H = (sourceBuffer[suffershift++] << 24) | uutMatrix.H;

						tex_heigth = sourceBuffer[suffershift++];
						tex_heigth = (sourceBuffer[suffershift++] << 8) | tex_heigth;
						tex_width = sourceBuffer[suffershift++];
						tex_width = (sourceBuffer[suffershift++] << 8) | tex_width;

						addr = sourceBuffer[suffershift++];
						addr = (sourceBuffer[suffershift++] << 8)  | addr;
						addr = (sourceBuffer[suffershift++] << 16) | addr;
						addr = (sourceBuffer[suffershift++] << 24) | addr;

						start_x = sourceBuffer[suffershift++];
						start_y = sourceBuffer[suffershift++];
						stop_x	= sourceBuffer[suffershift++];
						stop_y	= sourceBuffer[suffershift++];

						texture_ctrl = (headerfourth << 24) | (headerthird << 16) | (headersecond << 8) | (headerfirst);
						InitColor = addr;

						endflag = ((texture_ctrl & ENDFLAG_VERSION2) == ENDFLAG_VERSION2);
					}
					else
					{
						uutMatrix.A = 0x100000;
						uutMatrix.B = 0;
						uutMatrix.C = 0;
						uutMatrix.D = 0;
						uutMatrix.E = 0x100000;
						uutMatrix.F = 0;
						uutMatrix.G = sourceBuffer[suffershift++];
						uutMatrix.G = (sourceBuffer[suffershift++] << 8)  | uutMatrix.G;
						uutMatrix.G = (sourceBuffer[suffershift++] << 16) | uutMatrix.G;
						uutMatrix.G = (sourceBuffer[suffershift++] << 24) | uutMatrix.G;
						uutMatrix.H = sourceBuffer[suffershift++];
						uutMatrix.H = (sourceBuffer[suffershift++] << 8)  | uutMatrix.H;
						uutMatrix.H = (sourceBuffer[suffershift++] << 16) | uutMatrix.H;
						uutMatrix.H = (sourceBuffer[suffershift++] << 24) | uutMatrix.H;

						tex_heigth = sourceBuffer[suffershift++];
						tex_heigth = (sourceBuffer[suffershift++] << 8) | tex_heigth;
						tex_width = sourceBuffer[suffershift++];
						tex_width = (sourceBuffer[suffershift++] << 8) | tex_width;

						addr = sourceBuffer[suffershift++];
						addr = (sourceBuffer[suffershift++] << 8)  | addr;
						addr = (sourceBuffer[suffershift++] << 16) | addr;
						addr = (sourceBuffer[suffershift++] << 24) | addr;

						start_x = sourceBuffer[suffershift++];
						start_y = sourceBuffer[suffershift++];
						stop_x	= sourceBuffer[suffershift++];
						stop_y	= sourceBuffer[suffershift++];

						tex_vertex.v0.cor.x = (-uutMatrix.G >> 9);
						tex_vertex.v0.cor.y = (-uutMatrix.H >> 9);
						tex_vertex.v3.cor.x = (-uutMatrix.G >> 9);
						tex_vertex.v3.cor.y = (tex_heigth << 4) + (-uutMatrix.H >> 9);
						tex_vertex.v2.cor.x = (tex_width << 4) + (-uutMatrix.G >> 9);
						tex_vertex.v2.cor.y = (tex_heigth << 4) + (-uutMatrix.H >> 9);
						tex_vertex.v1.cor.x = (tex_width << 4) + (-uutMatrix.G >> 9);
						tex_vertex.v1.cor.y = (-uutMatrix.H >> 9);

						texture_ctrl = (headerfourth << 24) | (headerthird << 16) | (headersecond << 8) | (headerfirst);
						InitColor = addr;

						endflag = ((texture_ctrl & ENDFLAG_VERSION2) == ENDFLAG_VERSION2);
					}						
					InitTile(&uutTile, tile_x, tile_y);
					if ((tile_x_num < start_x || tile_x_num > stop_x) || (tile_y_num < start_y || tile_y_num > stop_y))
						continue;
					else 
						TexRaster(&uutTile, &uutMatrix, addr, tex_width, tex_heigth, texture_ctrl, InitColor,&tex_vertex);
					//if (uutTile.color_done)
						color_blending(&theFinalTile, &uutTile, texture_ctrl);
						//clear_tile(&uutTile);
					//suffershift++;
				}
			Write_Frame(&theFinalTile, FrameBuffer);
			FinalTile *finaltile = &theFinalTile;
#ifdef TEST_LOG_GEN
		for(int i=0;i<TILESIZE* TILESIZE;i++)
		{
			fprintf(fp,"%04x %04x %02x %02x %02x %02x\n",finaltile->x + i%32 * 16,finaltile->y + i/32 * 16,finaltile->TileBuffer[i].r,finaltile->TileBuffer[i].g,finaltile->TileBuffer[i].b,finaltile->TileBuffer[i].a);
		}
#endif
			}
			T = t_l1 * count1 + t_l2 * count2;
#ifdef TEST_LOG_GEN
			fclose(fp);

			FILE *fp_final;
			FILE *fp_vga;
			FILE *fp_vga_16bit;
			fp_final = fopen("ahmi_final.log","w");
			fp_vga   = fopen("vga.log","w");
			fp_vga_16bit   = fopen("vga_16bit.log","w");
			if(fp_final == NULL || fp_vga == NULL)
				return;
			for(int ii=0;ii<(AHMI_REGFILE.FrameSize&0xffff) * ((AHMI_REGFILE.FrameSize&0xffff0000)>>16);ii++)
			{
				int num = (ii%(AHMI_REGFILE.FrameSize&0xffff)) + (((AHMI_REGFILE.FrameSize&0xffff0000)>>16) - 1 - ii/(AHMI_REGFILE.FrameSize&0xffff))*(AHMI_REGFILE.FrameSize&0xffff);
				fprintf(fp_final,"%04x %04x %02x %02x %02x\n",(ii%(AHMI_REGFILE.FrameSize&0xffff)) * 16,ii/(AHMI_REGFILE.FrameSize&0xffff)*16,FrameBuffer[3*num+0],FrameBuffer[3*num+1],FrameBuffer[3*num+2]);
				fprintf(fp_vga,"%02x %02x %02x\n",FrameBuffer[3*num+0],FrameBuffer[3*num+1],FrameBuffer[3*num+2]);
				fprintf(fp_vga_16bit,"%02x %02x %02x\n",((FrameBuffer[3*num+0]&0xF8) | ((FrameBuffer[3*num+0]&0xE0)>>5)),((FrameBuffer[3*num+1]&0xFC) | ((FrameBuffer[3*num+1]&0xC0)>>6)),((FrameBuffer[3*num+2]&0xF8) | ((FrameBuffer[3*num+2]&0xE0)>>5)));
			}
			fclose(fp_final);
			fclose(fp_vga);
			fclose(fp_vga_16bit);
#endif
			 AHMI_REGFILE.STATUS=1;
}
#endif

void ahmi::SetAHMIRegs(AHMIDriverPtr pAHMIDriver) //set the regs for AHMI
{
	#ifdef VERSION_1
	AHMI_REGFILE.tileStartYX = (pAHMIDriver->mAHMI_InitTypeDef.YDIR_StartTile << 8) + pAHMIDriver->mAHMI_InitTypeDef.XDIR_StartTile;
	AHMI_REGFILE.tileEndYX = (pAHMIDriver->mAHMI_InitTypeDef.YDIR_EndTile << 8) + pAHMIDriver->mAHMI_InitTypeDef.XDIR_EndTile;
	AHMI_REGFILE.addr_ahmi_l = (pAHMIDriver->mAHMI_InitTypeDef.FB_BASE) % 65536;
	AHMI_REGFILE.addr_ahmi_h = (pAHMIDriver->mAHMI_InitTypeDef.FB_BASE) / 65536;
	AHMI_REGFILE.FrameSize = (uint32_t)(pAHMIDriver->mAHMI_InitTypeDef.FRAME_Hight <<16) | pAHMIDriver->mAHMI_InitTypeDef.FRAME_Width;
	AHMI_REGFILE.STATUS = 0;
	#endif

	#ifdef VERSION_2
	AHMI_REGFILE.DrawBox = (uint32_t)(pAHMIDriver->mAHMI_InitTypeDef.XDIR_StartTile | pAHMIDriver->mAHMI_InitTypeDef.YDIR_StartTile << 8 | pAHMIDriver->mAHMI_InitTypeDef.XDIR_EndTile << 16 | pAHMIDriver->mAHMI_InitTypeDef.YDIR_EndTile << 24);
	AHMI_REGFILE.FB_ADDR = pAHMIDriver->mAHMI_InitTypeDef.FB_BASE;
	AHMI_REGFILE.SB_ADDR = pAHMIDriver->mAHMI_InitTypeDef.SB_BASE;
	AHMI_REGFILE.FrameSize = (uint32_t)(pAHMIDriver->mAHMI_InitTypeDef.FRAME_Hight <<16) | pAHMIDriver->mAHMI_InitTypeDef.FRAME_Width;
	AHMI_REGFILE.MaxSBNum = pAHMIDriver->mAHMI_InitTypeDef.MAX_SB;
	AHMI_REGFILE.OUPUT_ROT = pAHMIDriver->mAHMI_InitTypeDef.OUTPUT_Rotation;
	AHMI_REGFILE.OUTPUFORMAT = pAHMIDriver->mAHMI_InitTypeDef.OUTPUT_FORMAT;
	AHMI_REGFILE.STATUS = 0;
	#endif
}

#endif
