#include "stc.h"
void interpolate(u8 a0,u8 a1,u8 weight,u8* out)
{
    u8 true_weight = weight + weight/8;
	sc16 da = a1 - a0;
	sc16 da_x_w = da * true_weight;
    sc16 final_a = a0 + da_x_w/16;
	*out = final_a < 0 ? 0 : final_a > 255 ? 255 : final_a;
}

void interpolate_32(u8 a0,u8 a1,u8 weight,u8* out)
{
    u8 true_weight = weight + weight/16;
	sc16 da = a1 - a0;
	sc16 da_x_w = da * true_weight;
    sc16 final_a = a0 + da_x_w/32;
	*out = final_a < 0 ? 0 : final_a > 255 ? 255 : final_a;
}

void weight_16(u8 weight_in,u8* weight_out)
{
	if(weight_in == 0)
		*weight_out = 0;
	else if(weight_in == 1)
		*weight_out = 4;
	else if(weight_in == 2)
		*weight_out = 11;
	else
		*weight_out = 15;
}

void weight_32(u8 weight_in,u8* weight_out)
{
	if(weight_in == 0)
		*weight_out = 0;
	else if(weight_in == 1)
		*weight_out = 8;
	else if(weight_in == 2)
		*weight_out = 23;
	else
		*weight_out = 31;
}

void weight_32_2(u8 weight_in,u8* weight_out)
{
	if(weight_in == 0)
		*weight_out = 0;
	else if(weight_in == 1)
		*weight_out = 4;
	else if(weight_in == 2)
		*weight_out = 8;
	else if(weight_in == 3)
		*weight_out = 12;
	else if(weight_in == 4)
		*weight_out = 19;
	else if(weight_in == 5)
		*weight_out = 23;
	else if(weight_in == 6)
		*weight_out = 27;
	else
		*weight_out = 31;
}

void stc_rgba_two_partition_dec(u8 x,u8 y,u128 block_data,u32* out_color)
{
    u8 p0_r0;
    u8 p0_g0;
	u8 p0_b0;
	u8 p0_a0;
    u8 p0_r1;
    u8 p0_g1;
	u8 p0_b1;
	u8 p0_a1;
	u8 p1_r0;
    u8 p1_g0;
	u8 p1_b0;
	u8 p1_a0;
    u8 p1_r1;
    u8 p1_g1;
	u8 p1_b1;
	u8 p1_a1;
	u32 seed = block_data>>86;
	seed = seed%1024;
	//seed = seed + 2*1024;
	u8 cur_par=select_partition(seed,x,y,0,2, 1);
	p0_r0 = (block_data>>2 ) %64; 
	p0_r1 = (block_data>>8 ) %64; 
	p0_g0 = (block_data>>14) %64; 
    p0_g1 = (block_data>>20) %64; 
    p0_b0 = (block_data>>26) %64; 
	p0_b1 = (block_data>>32) %64; 
	p0_a0 = (block_data>>38) %8; 
    p0_a1 = (block_data>>41) %8; 
	p1_r0 = (block_data>>44) %64; 
	p1_r1 = (block_data>>50) %64; 
	p1_g0 = (block_data>>56) %64; 
    p1_g1 = (block_data>>62) %64; 
    p1_b0 = (block_data>>68) %64; 
	p1_b1 = (block_data>>74) %64; 
	p1_a0 = (block_data>>80) %8; 
    p1_a1 = (block_data>>83) %8; 

	p0_r0 = p0_r0*4+p0_r0/16; 
	p0_r1 = p0_r1*4+p0_r1/16; 
	p0_g0 = p0_g0*4+p0_g0/16; 
    p0_g1 = p0_g1*4+p0_g1/16; 
    p0_b0 = p0_b0*4+p0_b0/16; 
	p0_b1 = p0_b1*4+p0_b1/16; 
	p0_a0 = p0_a0 * 32 + p0_a0 * 4 + p0_a0/2; 
    p0_a1 = p0_a1 * 32 + p0_a1 * 4 + p0_a1/2; 
	p1_r0 = p1_r0*4+p1_r0/16; 
	p1_r1 = p1_r1*4+p1_r1/16; 
	p1_g0 = p1_g0*4+p1_g0/16; 
    p1_g1 = p1_g1*4+p1_g1/16; 
    p1_b0 = p1_b0*4+p1_b0/16; 
	p1_b1 = p1_b1*4+p1_b1/16; 
	p1_a0 = p1_a0 * 32 + p1_a0 * 4 + p1_a0/2;
    p1_a1 = p1_a1 * 32 + p1_a1 * 4 + p1_a1/2;
	u64 weight_table = block_data>>96;

    u64 weight_total = weight_table>>(2*(y*4+x));
	u8 weight = weight_total%4;
    weight_16(weight,&weight);
	u8 r,g,b,a;
	if(cur_par == 0)
	{
	interpolate(p0_r0,p0_r1,weight,&r);
	interpolate(p0_g0,p0_g1,weight,&g);
	interpolate(p0_b0,p0_b1,weight,&b);
	interpolate(p0_a0,p0_a1,weight,&a);
	}
	else
	{
    interpolate(p1_r0,p1_r1,weight,&r);
	interpolate(p1_g0,p1_g1,weight,&g);
	interpolate(p1_b0,p1_b1,weight,&b);
	interpolate(p1_a0,p1_a1,weight,&a);
	}
	*out_color = (a<<24) + (r<<16) + (g<<8) + b;
    
}

void stc_rgba_two_partition_two_plane_dec(u8 x,u8 y,u128 block_data,u32* out_color)
{
    u8 p0_r0;
    u8 p0_g0;
	u8 p0_b0;
	u8 p0_a0;
    u8 p0_r1;
    u8 p0_g1;
	u8 p0_b1;
	u8 p0_a1;
	u8 p1_r0;
    u8 p1_g0;
	u8 p1_b0;
	u8 p1_a0;
    u8 p1_r1;
    u8 p1_g1;
	u8 p1_b1;
	u8 p1_a1;
	u32 seed = block_data>>71;
	seed = seed%1024;
	//seed = seed + 2*1024;
	u8 cur_par=select_partition(seed,x,y,0,2, 1);
	u32 offset_p0 = (block_data>>3)%2;
    u32 offset_p1 = (block_data>>4)%2;
	if((offset_p0 & 0x1) == 0x0 )
	{
	p0_r0 = (block_data>>7 ) %16; 
	p0_r1 = (block_data>>11) %16; 
	p0_g0 = (block_data>>15) %16; 
    p0_g1 = (block_data>>19) %16; 
    p0_b0 = (block_data>>23) %16; 
	p0_b1 = (block_data>>27) %16; 
	p0_a0 = (block_data>>31) %16;
    p0_a1 = (block_data>>35) %16;
	p0_r0 = p0_r0*16+p0_r0; 
	p0_r1 = p0_r1*16+p0_r1; 
	p0_g0 = p0_g0*16+p0_g0; 
    p0_g1 = p0_g1*16+p0_g1; 
    p0_b0 = p0_b0*16+p0_b0; 
	p0_b1 = p0_b1*16+p0_b1; 
	p0_a0 = p0_a0 * 16 + p0_a0; 
    p0_a1 = p0_a1 * 16 + p0_a1; 
	}
	else
	{
	u32 offset_p0_r01_u = (block_data>>12)%8;
	u32 offset_p0_g01_u = (block_data>>20)%8;
    u32 offset_p0_b01_u = (block_data>>28)%8;
	u32 offset_p0_a01_u = (block_data>>36)%8;
	s8 offset_p0_r01 = (offset_p0_r01_u & 0x4) == 0x4 ? (offset_p0_r01_u+0xf8): offset_p0_r01_u;
	s8 offset_p0_g01 = (offset_p0_g01_u & 0x4) == 0x4 ? (offset_p0_g01_u+0xf8): offset_p0_g01_u;
    s8 offset_p0_b01 = (offset_p0_b01_u & 0x4) == 0x4 ? (offset_p0_b01_u+0xf8): offset_p0_b01_u;
	s8 offset_p0_a01 = (offset_p0_a01_u & 0x4) == 0x4 ? (offset_p0_a01_u+0xf8): offset_p0_a01_u;
	p0_r0 = (block_data>>7 ) %32; 
	p0_r1 = p0_r0 + offset_p0_r01; 
	p0_g0 = (block_data>>15) %32; 
    p0_g1 = p0_g0 + offset_p0_g01; 
    p0_b0 = (block_data>>23) %32; 
	p0_b1 = p0_b0 + offset_p0_b01; 
	p0_a0 = (block_data>>31) %32; 
	p0_a1 = p0_a0 + offset_p0_a01; 
	p0_r0 =  p0_r0 * 8 + p0_r0/4;
	p0_r1 =  p0_r1 * 8 + p0_r1/4;
	p0_g0 =  p0_g0 * 8 + p0_g0/4;
    p0_g1 =  p0_g1 * 8 + p0_g1/4;
    p0_b0 =  p0_b0 * 8 + p0_b0/4;
	p0_b1 =  p0_b1 * 8 + p0_b1/4;
	p0_a0 =  p0_a0 * 8 + p0_a0/4;
	p0_a1 =  p0_a1 * 8 + p0_a1/4;

	}

	if((offset_p1 & 0x1) == 0x0 )
	{
	p1_r0 = (block_data>>39) %16; 
	p1_r1 = (block_data>>43) %16; 
	p1_g0 = (block_data>>47) %16; 
    p1_g1 = (block_data>>51) %16; 
    p1_b0 = (block_data>>55) %16; 
	p1_b1 = (block_data>>59) %16; 
	p1_a0 = (block_data>>63) %16;
    p1_a1 = (block_data>>67) %16;
	p1_r0 = p1_r0*16+p1_r0; 
	p1_r1 = p1_r1*16+p1_r1; 
	p1_g0 = p1_g0*16+p1_g0; 
    p1_g1 = p1_g1*16+p1_g1; 
    p1_b0 = p1_b0*16+p1_b0; 
	p1_b1 = p1_b1*16+p1_b1; 
	p1_a0 = p1_a0 * 16 + p1_a0;
    p1_a1 = p1_a1 * 16 + p1_a1;
	}
	else
	{
    u32 offset_p1_r01_u = (block_data>>44)%8;
	u32 offset_p1_g01_u = (block_data>>52)%8;
    u32 offset_p1_b01_u = (block_data>>60)%8;
	u32 offset_p1_a01_u = (block_data>>68)%8;
	s8 offset_p1_r01 = (offset_p1_r01_u & 0x4) == 0x4 ? (offset_p1_r01_u+0xf8): offset_p1_r01_u;
	s8 offset_p1_g01 = (offset_p1_g01_u & 0x4) == 0x4 ? (offset_p1_g01_u+0xf8): offset_p1_g01_u;
    s8 offset_p1_b01 = (offset_p1_b01_u & 0x4) == 0x4 ? (offset_p1_b01_u+0xf8): offset_p1_b01_u;
	s8 offset_p1_a01 = (offset_p1_a01_u & 0x4) == 0x4 ? (offset_p1_a01_u+0xf8): offset_p1_a01_u;
	p1_r0 = (block_data>>39) %32; 
	p1_r1 = p1_r0 + offset_p1_r01; 
	p1_g0 = (block_data>>47) %32; 
    p1_g1 = p1_g0 + offset_p1_g01; 
    p1_b0 = (block_data>>55) %32; 
	p1_b1 = p1_b0 + offset_p1_b01; 
	p1_a0 = (block_data>>63) %32; 
	p1_a1 = p1_a0 + offset_p1_a01; 
	p1_r0 =  p1_r0 * 8 + p1_r0/4;
	p1_r1 =  p1_r1 * 8 + p1_r1/4;
	p1_g0 =  p1_g0 * 8 + p1_g0/4;
    p1_g1 =  p1_g1 * 8 + p1_g1/4;
    p1_b0 =  p1_b0 * 8 + p1_b0/4;
	p1_b1 =  p1_b1 * 8 + p1_b1/4;
	p1_a0 =  p1_a0 * 8 + p1_a0/4;
	p1_a1 =  p1_a1 * 8 + p1_a1/4;
	}

	u8 type = (block_data>>5)%4;

	u64 weight_table = block_data>>81;

    u8 w0 = weight_table%16;
    u8 w1 = (weight_table>> (2  *2))%16;
	u8 w2 = (weight_table>> (4  *2))%16;
	u8 w3 = (weight_table>> (6  *2))%16;
	u8 w4 = (weight_table>> (8  *2))%16;
	u8 w5 = (weight_table>> (10 *2))%16;
	u8 w6 = (weight_table>> (12 *2))%16;
	u8 w7 = (weight_table>> (14 *2))%16;
	u8 w8 = (weight_table>> (16 *2))%16;
	u8 w9 = (weight_table>> (18 *2))%16;
	u8 w10 = (weight_table>>(20*2))%16;
	u8 w11 = (weight_table>>(22*2))%16;
	u8 p0_w0 = w0 %4;
    u8 p0_w1 = w1 %4;
	u8 p0_w2 = w2 %4;
	u8 p0_w3 = w3 %4;
	u8 p0_w4 = w4 %4;
	u8 p0_w5 = w5 %4;
	u8 p0_w6 = w6 %4;
	u8 p0_w7 = w7 %4;
	u8 p0_w8 = w8 %4;
	u8 p0_w9 = w9 %4;
	u8 p0_w10 =w10%4;
	u8 p0_w11 =w11%4;
	u8 p1_w0 = w0 /4;
    u8 p1_w1 = w1 /4;
	u8 p1_w2 = w2 /4;
	u8 p1_w3 = w3 /4;
	u8 p1_w4 = w4 /4;
	u8 p1_w5 = w5 /4;
	u8 p1_w6 = w6 /4;
	u8 p1_w7 = w7 /4;
	u8 p1_w8 = w8 /4;
	u8 p1_w9 = w9 /4;
	u8 p1_w10 =w10/4;
	u8 p1_w11 =w11/4;


	u8 pos = y*4+x;
	u8 w_p0;
    u8 w_p1;
	if(pos == 0)
        w_p0 = p0_w0;
	else if(pos == 1)
		w_p0 = (p0_w0+p0_w1)/2 + ((p0_w0+p0_w1)%2);
	else if(pos == 2)
		w_p0 = (p0_w1+p0_w2)/2 + ((p0_w1+p0_w2)%2);
	else if(pos == 3)
		w_p0 = p0_w2;
	else if(pos == 4)
		w_p0 = p0_w3;
	else if(pos == 5)
		w_p0 = (p0_w3+p0_w4)/2 + ((p0_w3+p0_w4)%2);
	else if(pos == 6)
		w_p0 = (p0_w4+p0_w5)/2 + ((p0_w4+p0_w5)%2);
	else if(pos == 7)
		w_p0 = p0_w5;
	else if(pos == 8)
		w_p0 = p0_w6;
	else if(pos == 9)
		w_p0 = (p0_w6+p0_w7)/2 + ((p0_w6+p0_w7)%2);
	else if(pos == 10)
		w_p0 = (p0_w7+p0_w8)/2 + ((p0_w7+p0_w8)%2);
	else if(pos == 11)
		w_p0 = p0_w8;
	else if(pos == 12)
		w_p0 = p0_w9;
	else if(pos == 13)
		w_p0 = (p0_w9+p0_w10)/2 + ((p0_w9+p0_w10)%2);
	else if(pos == 14)
		w_p0 = (p0_w10+p0_w11)/2 + ((p0_w10+p0_w11)%2);
	else if(pos == 15)
		w_p0 = p0_w11;
    if(pos == 0)
        w_p1 = p1_w0;
	else if(pos == 1)
		w_p1 = (p1_w0+p1_w1)/2 + ((p1_w0+p1_w1)%2);
	else if(pos == 2)
		w_p1 = (p1_w1+p1_w2)/2 + ((p1_w1+p1_w2)%2);
	else if(pos == 3)
		w_p1 = p1_w2;
	else if(pos == 4)
		w_p1 = p1_w3;
	else if(pos == 5)
		w_p1 = (p1_w3+p1_w4)/2 + ((p1_w3+p1_w4)%2);
	else if(pos == 6)
		w_p1 = (p1_w4+p1_w5)/2 + ((p1_w4+p1_w5)%2);
	else if(pos == 7)
		w_p1 = p1_w5;
	else if(pos == 8)
		w_p1 = p1_w6;
	else if(pos == 9)
		w_p1 = (p1_w6+p1_w7)/2 + ((p1_w6+p1_w7)%2);
	else if(pos == 10)
		w_p1 = (p1_w7+p1_w8)/2 + ((p1_w7+p1_w8)%2);
	else if(pos == 11)
		w_p1 = p1_w8;
	else if(pos == 12)
		w_p1 = p1_w9;
	else if(pos == 13)
		w_p1 = (p1_w9+p1_w10)/2 + ((p1_w9+p1_w10)%2);
	else if(pos == 14)
		w_p1 = (p1_w10+p1_w11)/2 + ((p1_w10+p1_w11)%2);
	else if(pos == 15)
		w_p1 = p1_w11;
	u8 wr,wg,wb,wa;
	weight_16(w_p0,&w_p0);
	weight_16(w_p1,&w_p1);
	wr = type == 3 ? w_p0 : w_p1;
	wg = type == 2 ? w_p0 : w_p1;
	wb = type == 1 ? w_p0 : w_p1;
	wa = type == 0 ? w_p0 : w_p1;

    
	u8 r,g,b,a;
	if(cur_par == 0)
	{
	interpolate(p0_r0,p0_r1,wr,&r);
	interpolate(p0_g0,p0_g1,wg,&g);
	interpolate(p0_b0,p0_b1,wb,&b);
	interpolate(p0_a0,p0_a1,wa,&a);
	}
	else
	{
    interpolate(p1_r0,p1_r1,wr,&r);
	interpolate(p1_g0,p1_g1,wg,&g);
	interpolate(p1_b0,p1_b1,wb,&b);
	interpolate(p1_a0,p1_a1,wa,&a);
	}
	*out_color = (a<<24) + (r<<16) + (g<<8) + b;
    
}

void stc_rgba_three_partition_dec(u8 x,u8 y,u128 block_data,u32* out_color)
{
    u8 p0_r0;
    u8 p0_g0;
	u8 p0_b0;
	u8 p0_a0;
    u8 p0_r1;
    u8 p0_g1;
	u8 p0_b1;
	u8 p0_a1;
	u8 p1_r0;
    u8 p1_g0;
	u8 p1_b0;
	u8 p1_a0;
    u8 p1_r1;
    u8 p1_g1;
	u8 p1_b1;
	u8 p1_a1;
	u8 p2_r0;
    u8 p2_g0;
	u8 p2_b0;
	u8 p2_a0;
    u8 p2_r1;
    u8 p2_g1;
	u8 p2_b1;
	u8 p2_a1;
	u32 seed = block_data>>95;
	seed = seed%1024;
	//seed = seed + 3*1024;
	u8 cur_par=select_partition(seed,x,y,0,3, 1);
	u32 offset_mode = (block_data>>4)%2;
	if((offset_mode & 0x1) == 0x0)
	{
	p0_r0 = (block_data>>5 ) %16; 
	p0_r1 = (block_data>>9 ) %16; 
	p0_g0 = (block_data>>13) %16; 
    p0_g1 = (block_data>>17) %16; 
    p0_b0 = (block_data>>21) %16; 
	p0_b1 = (block_data>>25) %16; 
	p0_a0 = (block_data>>29) %8; 
    p0_a1 = (block_data>>32) %8; 
	p1_r0 = (block_data>>35) %16; 
	p1_r1 = (block_data>>39) %16; 
	p1_g0 = (block_data>>43) %16; 
    p1_g1 = (block_data>>47) %16; 
    p1_b0 = (block_data>>51) %16; 
	p1_b1 = (block_data>>55) %16; 
	p1_a0 = (block_data>>59) %8; 
    p1_a1 = (block_data>>62) %8; 
	p2_r0 = (block_data>>65) %16; 
	p2_r1 = (block_data>>69) %16; 
	p2_g0 = (block_data>>73) %16; 
    p2_g1 = (block_data>>77) %16; 
    p2_b0 = (block_data>>81) %16; 
	p2_b1 = (block_data>>85) %16; 
	p2_a0 = (block_data>>89) %8; 
    p2_a1 = (block_data>>92) %8;
	p0_r0 = p0_r0*16+p0_r0; 
	p0_r1 = p0_r1*16+p0_r1; 
	p0_g0 = p0_g0*16+p0_g0; 
    p0_g1 = p0_g1*16+p0_g1; 
    p0_b0 = p0_b0*16+p0_b0; 
	p0_b1 = p0_b1*16+p0_b1; 
	p0_a0 = p0_a0 * 32 + p0_a0 * 4 + p0_a0/2; 
    p0_a1 = p0_a1 * 32 + p0_a1 * 4 + p0_a1/2; 
	p1_r0 = p1_r0*16+p1_r0; 
	p1_r1 = p1_r1*16+p1_r1; 
	p1_g0 = p1_g0*16+p1_g0; 
    p1_g1 = p1_g1*16+p1_g1; 
    p1_b0 = p1_b0*16+p1_b0; 
	p1_b1 = p1_b1*16+p1_b1; 
	p1_a0 = p1_a0 * 32 + p1_a0 * 4 + p1_a0/2;
    p1_a1 = p1_a1 * 32 + p1_a1 * 4 + p1_a1/2;
	p2_r0 = p2_r0*16+p2_r0; 
	p2_r1 = p2_r1*16+p2_r1; 
	p2_g0 = p2_g0*16+p2_g0; 
    p2_g1 = p2_g1*16+p2_g1; 
    p2_b0 = p2_b0*16+p2_b0; 
	p2_b1 = p2_b1*16+p2_b1; 
	p2_a0 = p2_a0 * 32 + p2_a0 * 4 + p2_a0/2;
    p2_a1 = p2_a1 * 32 + p2_a1 * 4 + p2_a1/2;
	}
	else
	{
    u32 offset_p0_r01_u = (block_data>>10)%8;
	u32 offset_p0_g01_u = (block_data>>18)%8;
    u32 offset_p0_b01_u = (block_data>>26)%8;
	u32 offset_p1_r01_u = (block_data>>10)%8;
	u32 offset_p1_g01_u = (block_data>>18)%8;
    u32 offset_p1_b01_u = (block_data>>26)%8;
	u32 offset_p2_r01_u = (block_data>>10)%8;
	u32 offset_p2_g01_u = (block_data>>18)%8;
    u32 offset_p2_b01_u = (block_data>>26)%8;
	s8 offset_p0_r01 = (offset_p0_r01_u & 0x4) == 0x4 ? (offset_p0_r01_u+0xf8): offset_p0_r01_u;
	s8 offset_p0_g01 = (offset_p0_g01_u & 0x4) == 0x4 ? (offset_p0_g01_u+0xf8): offset_p0_g01_u;
    s8 offset_p0_b01 = (offset_p0_b01_u & 0x4) == 0x4 ? (offset_p0_b01_u+0xf8): offset_p0_b01_u;
	s8 offset_p1_r01 = (offset_p1_r01_u & 0x4) == 0x4 ? (offset_p1_r01_u+0xf8): offset_p1_r01_u;
	s8 offset_p1_g01 = (offset_p1_g01_u & 0x4) == 0x4 ? (offset_p1_g01_u+0xf8): offset_p1_g01_u;
    s8 offset_p1_b01 = (offset_p1_b01_u & 0x4) == 0x4 ? (offset_p1_b01_u+0xf8): offset_p1_b01_u;
	s8 offset_p2_r01 = (offset_p2_r01_u & 0x4) == 0x4 ? (offset_p2_r01_u+0xf8): offset_p2_r01_u;
	s8 offset_p2_g01 = (offset_p2_g01_u & 0x4) == 0x4 ? (offset_p2_g01_u+0xf8): offset_p2_g01_u;
    s8 offset_p2_b01 = (offset_p2_b01_u & 0x4) == 0x4 ? (offset_p2_b01_u+0xf8): offset_p2_b01_u;
    p0_r0 = (block_data>>5 ) %32; 
	p0_r1 = p0_r0 + offset_p0_r01; 
	p0_g0 = (block_data>>13) %32; 
    p0_g1 = p0_g0 + offset_p0_g01; 
    p0_b0 = (block_data>>21) %32; 
	p0_b1 = p0_b0 + offset_p0_b01; 
	p0_a0 = (block_data>>29) %8; 
    p0_a1 = (block_data>>32) %8; 
	p1_r0 = (block_data>>35) %32; 
	p1_r1 = p1_r0 + offset_p1_r01; 
	p1_g0 = (block_data>>43) %32; 
    p1_g1 = p1_g0 + offset_p1_g01; 
    p1_b0 = (block_data>>51) %32; 
	p1_b1 = p1_b0 + offset_p1_b01; 
	p1_a0 = (block_data>>59) %8; 
    p1_a1 = (block_data>>62) %8; 
	p2_r0 = (block_data>>65) %32; 
	p2_r1 = p2_r0 + offset_p2_r01; 
	p2_g0 = (block_data>>73) %32; 
    p2_g1 = p2_g0 + offset_p2_g01; 
    p2_b0 = (block_data>>81) %32; 
	p2_b1 = p2_b0 + offset_p2_b01; 
	p2_a0 = (block_data>>89) %8; 
    p2_a1 = (block_data>>92) %8;

	p0_r0 = p0_r0 * 8 + p0_r0/4;
	p0_r1 =	p0_r1 * 8 + p0_r1/4;
	p0_g0 =	p0_g0 * 8 + p0_g0/4;
    p0_g1 =	p0_g1 * 8 + p0_g1/4;
    p0_b0 =	p0_b0 * 8 + p0_b0/4;
	p0_b1 =	p0_b1 * 8 + p0_b1/4;	
	p1_r0 =	p1_r0 * 8 + p1_r0/4;
	p1_r1 =	p1_r1 * 8 + p1_r1/4;
	p1_g0 =	p1_g0 * 8 + p1_g0/4;
    p1_g1 =	p1_g1 * 8 + p1_g1/4;
    p1_b0 =	p1_b0 * 8 + p1_b0/4;
	p1_b1 =	p1_b1 * 8 + p1_b1/4;
	p2_r0 =	p2_r0 * 8 + p2_r0/4;
	p2_r1 =	p2_r1 * 8 + p2_r1/4;
	p2_g0 =	p2_g0 * 8 + p2_g0/4;
    p2_g1 =	p2_g1 * 8 + p2_g1/4;
    p2_b0 =	p2_b0 * 8 + p2_b0/4;
	p2_b1 =	p2_b1 * 8 + p2_b1/4;
	p0_a0 =	p0_a0 * 32 + p0_a0 * 4 + p0_a0/2;
    p0_a1 =	p0_a1 * 32 + p0_a1 * 4 + p0_a1/2;
	p1_a0 =	p1_a0 * 32 + p1_a0 * 4 + p1_a0/2;
    p1_a1 =	p1_a1 * 32 + p1_a1 * 4 + p1_a1/2;
	p2_a0 =	p2_a0 * 32 + p2_a0 * 4 + p2_a0/2;
    p2_a1 =	p2_a1 * 32 + p2_a1 * 4 + p2_a1/2;
	}
	u64 weight_table = block_data>>105;

    u8 w0 = weight_table%4;
    u8 w1 = (weight_table>>2)%4;
	u8 w2 = (weight_table>>4)%4;
	u8 w3 = (weight_table>>6)%4;
	u8 w4 = (weight_table>>8)%4;
	u8 w5 = (weight_table>>10)%4;
	u8 w6 = (weight_table>>12)%4;
	u8 w7 = (weight_table>>14)%4;
	u8 w8 = (weight_table>>16)%4;
	u8 w9 = (weight_table>>18)%4;
	u8 w10 = (weight_table>>20)%4;
	u8 w11 = (weight_table>>22)%4;
	u8 pos = y*4+x;
	u8 weight;
	if(pos == 0)
        weight = w0;
	else if(pos == 1)
		weight = (w0+w1)/2 + ((w0+w1)%2);
	else if(pos == 2)
		weight = (w1+w2)/2 + ((w1+w2)%2);
	else if(pos == 3)
		weight = w2;
	else if(pos == 4)
		weight = w3;
	else if(pos == 5)
		weight = (w3+w4)/2 + ((w3+w4)%2);
	else if(pos == 6)
		weight = (w4+w5)/2 + ((w4+w5)%2);
	else if(pos == 7)
		weight = w5;
	else if(pos == 8)
		weight = w6;
	else if(pos == 9)
		weight = (w6+w7)/2 + ((w6+w7)%2);
	else if(pos == 10)
		weight = (w7+w8)/2 + ((w7+w8)%2);
	else if(pos == 11)
		weight = w8;
	else if(pos == 12)
		weight = w9;
	else if(pos == 13)
		weight = (w9+w10)/2 + ((w9+w10)%2);
	else if(pos == 14)
		weight = (w10+w11)/2 + ((w10+w11)%2);
	else if(pos == 15)
		weight = w11;


    weight_16(weight,&weight);
	u8 r,g,b,a;
	if(cur_par == 0)
	{
	interpolate(p0_r0,p0_r1,weight,&r);
	interpolate(p0_g0,p0_g1,weight,&g);
	interpolate(p0_b0,p0_b1,weight,&b);
	interpolate(p0_a0,p0_a1,weight,&a);
	}
	else if(cur_par == 1)
	{
    interpolate(p1_r0,p1_r1,weight,&r);
	interpolate(p1_g0,p1_g1,weight,&g);
	interpolate(p1_b0,p1_b1,weight,&b);
	interpolate(p1_a0,p1_a1,weight,&a);
	}
	else
	{
    interpolate(p2_r0,p2_r1,weight,&r);
	interpolate(p2_g0,p2_g1,weight,&g);
	interpolate(p2_b0,p2_b1,weight,&b);
	interpolate(p2_a0,p2_a1,weight,&a);
	}
	*out_color = (a<<24) + (r<<16) + (g<<8) + b;
    
}


void stc_rgba_one_partition_dec(u8 x,u8 y,u128 block_data,u32* out_color)
{
    u8 r0;
    u8 g0;
	u8 b0;
	u8 a0;
    u8 r1;
    u8 g1;
	u8 b1;
	u8 a1;
	r0 = (block_data>>1 ) %256; 
	r1 = (block_data>>9 ) %256; 
	g0 = (block_data>>17) %256; 
    g1 = (block_data>>25) %256; 
    b0 = (block_data>>33) %256; 
	b1 = (block_data>>41) %256; 
	a0 = (block_data>>49) %256; 
    a1 = (block_data>>57) %256; 
	u64 weight_table = block_data>>65;

    u64 weight_total = weight_table>>(4*(y*4+x));
	u8 weight = weight_total%16;
	u8 r,g,b,a;
	interpolate(r0,r1,weight,&r);
	interpolate(g0,g1,weight,&g);
	interpolate(b0,b1,weight,&b);
	interpolate(a0,a1,weight,&a);
	*out_color = (a<<24) + (r<<16) + (g<<8) + b;
    
}

void stc_rgba_one_partition_two_plane_dec(u8 x,u8 y,u128 block_data,u32* out_color)
{
    u8 r0;
    u8 g0;
	u8 b0;
	u8 a0;
    u8 r1;
    u8 g1;
	u8 b1;
	u8 a1;
	r0 = (block_data>>(5+1 )) %256; 
	r1 = (block_data>>(5+9 )) %256; 
	g0 = (block_data>>(5+17)) %256; 
    g1 = (block_data>>(5+25)) %256; 
    b0 = (block_data>>(5+33)) %256; 
	b1 = (block_data>>(5+41)) %256; 
	a0 = ((block_data>>(5+49)) %64)*4; 
    a1 = ((block_data>>(5+55)) %64)*4; 
	a0 = a0 + a0/64;
	a1 = a1 + a1/64;
	u64 weight_table = block_data>>66;
	u8 type = (block_data>>4)%4;

    u64 weight_total = weight_table>>(4*(y*4+x));
	u8 weight = weight_total%16;
	if(x==3&&y==3)
		weight = weight/2 * 4 + (weight%2);
	u8 w0 = weight%4;
	u8 w1 = weight/4;
	weight_16(w0,&w0);
	weight_16(w1,&w1);
	u8 r,g,b,a;
	u8 wr,wg,wb,wa;
	wr = type == 3 ? w0 : w1;
	wg = type == 2 ? w0 : w1;
	wb = type == 1 ? w0 : w1;
	wa = type == 0 ? w0 : w1;
	interpolate(r0,r1,wr,&r);
	interpolate(g0,g1,wg,&g);
	interpolate(b0,b1,wb,&b);
	interpolate(a0,a1,wa,&a);
	*out_color = (a<<24) + (r<<16) + (g<<8) + b;
    
}


void stc_rgba_dec(u8 x,u8 y,u128 block_data,u32* out_color)
{
	u32 test = block_data>>0;
	if((test & 0x1) == 0)   //one partition
		stc_rgba_one_partition_dec(x,y,block_data,out_color);
	else if((test & 0x3) == 0x1)  //two partition
		stc_rgba_two_partition_dec(x,y,block_data,out_color);
	else if((test & 0x7) == 0x3)  //two plane two partition
		stc_rgba_two_partition_two_plane_dec(x,y,block_data,out_color);
	else if((test & 0xf) == 0x7)  //two plane one partition
		stc_rgba_one_partition_two_plane_dec(x,y,block_data,out_color);
	else                              //three partition
		stc_rgba_three_partition_dec(x,y,block_data,out_color);
	//if((*out_color & 0xff000000) == 0x0)
	//    *out_color = *out_color | 0xffff0000;
}

void stc_rgb_one_partition_dec(u8 x,u8 y,u128 block_data,u32* out_color)
{
    u8 r0;
    u8 g0;
	u8 b0;
    u8 r1;
    u8 g1;
	u8 b1;
	r0 = (block_data>>1 ) %256; 
	r1 = (block_data>>9 ) %256; 
	g0 = (block_data>>17) %256; 
    g1 = (block_data>>25) %256; 
    b0 = (block_data>>33) %256; 
	b1 = (block_data>>41) %256; 
	u128 weight_table = block_data||49;

    u64 weight_total = weight_table>>(5*(y*4+x));
	u8 weight = weight_total%32;
	u8 r,g,b,a;
	interpolate_32(r0,r1,weight,&r);
	interpolate_32(g0,g1,weight,&g);
	interpolate_32(b0,b1,weight,&b);
	*out_color = (0xff<<24) + (r<<16) + (g<<8) + b;
    
}

void stc_rgb_two_partition_dec(u8 x,u8 y,u128 block_data,u32* out_color)
{
    u8 p0_r0;
    u8 p0_g0;
	u8 p0_b0;
    u8 p0_r1;
    u8 p0_g1;
	u8 p0_b1;
	u8 p1_r0;
    u8 p1_g0;
	u8 p1_b0;
    u8 p1_r1;
    u8 p1_g1;
	u8 p1_b1;
	u32 seed = block_data>>87;
	seed = seed%1024;
	//seed = seed + 2*1024;
	u8 cur_par=select_partition(seed,x,y,0,2, 1);
	p0_r0 = (block_data>>3 ) %128; 
	p0_r1 = (block_data>>10) %128; 
	p0_g0 = (block_data>>17) %128; 
    p0_g1 = (block_data>>24) %128; 
    p0_b0 = (block_data>>31) %128; 
	p0_b1 = (block_data>>38) %128; 
	p1_r0 = (block_data>>45) %128; 
	p1_r1 = (block_data>>52) %128; 
	p1_g0 = (block_data>>59) %128; 
    p1_g1 = (block_data>>66) %128; 
    p1_b0 = (block_data>>73) %128; 
	p1_b1 = (block_data>>80) %128; 

	p0_r0 = p0_r0*2+p0_r0/64; 
	p0_r1 = p0_r1*2+p0_r1/64; 
	p0_g0 = p0_g0*2+p0_g0/64; 
    p0_g1 = p0_g1*2+p0_g1/64; 
    p0_b0 = p0_b0*2+p0_b0/64; 
	p0_b1 = p0_b1*2+p0_b1/64; 
	p1_r0 = p1_r0*2+p1_r0/64; 
	p1_r1 = p1_r1*2+p1_r1/64; 
	p1_g0 = p1_g0*2+p1_g0/64; 
    p1_g1 = p1_g1*2+p1_g1/64; 
    p1_b0 = p1_b0*2+p1_b0/64; 
	p1_b1 = p1_b1*2+p1_b1/64; 
	u64 weight_table = block_data>>97;

    u64 weight_total = weight_table>>(2*(y*4+x));
	u8 weight = weight_total%4;
    weight_32(weight,&weight);
	u8 r,g,b,a;
	if(cur_par == 0)
	{
	interpolate_32(p0_r0,p0_r1,weight,&r);
	interpolate_32(p0_g0,p0_g1,weight,&g);
	interpolate_32(p0_b0,p0_b1,weight,&b);
	}
	else
	{
    interpolate_32(p1_r0,p1_r1,weight,&r);
	interpolate_32(p1_g0,p1_g1,weight,&g);
	interpolate_32(p1_b0,p1_b1,weight,&b);
	}
	*out_color = (0xff<<24) + (r<<16) + (g<<8) + b;
    
}

void stc_rgb_one_partition_two_plane_dec(u8 x,u8 y,u128 block_data,u32* out_color)
{
    u8 r0;
    u8 g0;
	u8 b0;
    u8 r1;
    u8 g1;
	u8 b1;
	r0 = (block_data>>(5+1 )) %128; 
	r1 = (block_data>>(5+8 )) %128; 
	g0 = (block_data>>(5+15)) %128; 
    g1 = (block_data>>(5+22)) %128; 
    b0 = (block_data>>(5+29)) %128; 
	b1 = (block_data>>(5+36)) %128; 
    r0 = r0* 2 + r0/64; 
	r1 = r1* 2 + r1/64; 
	g0 = g0* 2 + g0/64; 
    g1 = g1* 2 + g1/64; 
    b0 = b0* 2 + b0/64; 
	b1 = b1* 2 + b1/64; 
	u128 weight_table;
	weight_table = block_data||48;
	u8 type = (block_data>>4)%4;

    u64 weight_total = weight_table>>(5*(y*4+x));
	u8 weight = weight_total%32;
	u8 w0 = weight%4;
	u8 w1 = weight/4;
	weight_32(w0,&w0);
	weight_32_2(w1,&w1);
	u8 r,g,b,a;
	u8 wr,wg,wb,wa;
	wr = type == 2 ? w0 : w1;
	wg = type == 1 ? w0 : w1;
	wb = type == 0 ? w0 : w1;
	interpolate_32(r0,r1,wr,&r);
	interpolate_32(g0,g1,wg,&g);
	interpolate_32(b0,b1,wb,&b);
	*out_color = (0xff<<24) + (r<<16) + (g<<8) + b;
    
}

void stc_rgb_three_partition_dec(u8 x,u8 y,u128 block_data,u32* out_color)
{
    u8 p0_r0;
    u8 p0_g0;
	u8 p0_b0;
    u8 p0_r1;
    u8 p0_g1;
	u8 p0_b1;
	u8 p1_r0;
    u8 p1_g0;
	u8 p1_b0;
    u8 p1_r1;
    u8 p1_g1;
	u8 p1_b1;
	u8 p2_r0;
    u8 p2_g0;
	u8 p2_b0;
    u8 p2_r1;
    u8 p2_g1;
	u8 p2_b1;
	u32 seed = block_data>>86;
	seed = seed%1024;
	//seed = seed + 3*1024;
	u8 cur_par=select_partition(seed,x,y,0,3, 1);
	u32 offset_mode_p0_c0 = (block_data>>4)%2;
    u32 offset_mode_p1_c0 = (block_data>>5)%2;
	u32 offset_mode_p2_c0 = (block_data>>6)%2;
	u32 offset_mode_p0_c1 = (block_data>>7)%2;
	u32 offset_mode_p1_c1 = (block_data>>8)%2;
	u32 offset_mode_p2_c1 = (block_data>>9)%2;
	u32 offset_mode_p0_c2 = (block_data>>10)%2;
	u32 offset_mode_p1_c2 = (block_data>>11)%2;
	u32 offset_mode_p2_c2 = (block_data>>12)%2;
	if((offset_mode_p0_c0 & 0x1) == 0x0 )
	{
	p0_r0 = (block_data>>14 ) %16; 
	p0_r1 = (block_data>>18 ) %16; 
	p0_r0 = p0_r0*16+p0_r0; 
	p0_r1 = p0_r1*16+p0_r1; 
	}
	else
	{
    p0_r0 = (block_data>>14 ) %32; 
	u32 offset_p0_r_u = (block_data>>19 ) %8; 
	s8 offset_p0_r = (offset_p0_r_u & 0x4) == 0x4 ? offset_p0_r_u+0xf8 : offset_p0_r_u;
	p0_r1 = p0_r0 + offset_p0_r; 
	p0_r0 = p0_r0*8+p0_r0/4; 
	p0_r1 = p0_r1*8+p0_r1/4; 
	}
	if((offset_mode_p0_c1 & 0x1) == 0x0 )
	{
	p0_g0 = (block_data>>22 ) %16; 
	p0_g1 = (block_data>>26 ) %16; 
	p0_g0 = p0_g0*16+p0_g0; 
	p0_g1 = p0_g1*16+p0_g1; 
	}
	else
	{
    p0_g0 = (block_data>>22 ) %32; 
	u32 offset_p0_g_u = (block_data>>27 ) %8; 
	s8 offset_p0_g = (offset_p0_g_u & 0x4) == 0x4 ? offset_p0_g_u+0xf8 : offset_p0_g_u;
	p0_g1 = p0_g0 + offset_p0_g; 
	p0_g0 = p0_g0*8+p0_g0/4; 
	p0_g1 = p0_g1*8+p0_g1/4; 
	}
	if((offset_mode_p0_c2 & 0x1) == 0x0 )
	{
	p0_b0 = (block_data>>30 ) %16; 
	p0_b1 = (block_data>>34 ) %16;  
    p0_b0 = p0_b0*16+p0_b0; 
	p0_b1 = p0_b1*16+p0_b1;
	}
	else
	{
    p0_b0 = (block_data>>30 ) %32; 
	u32 offset_p0_b_u = (block_data>>35 ) %8; 
	s8 offset_p0_b = (offset_p0_b_u & 0x4) == 0x4 ? offset_p0_b_u+0xf8 : offset_p0_b_u;
	p0_b1 = p0_b0 + offset_p0_b; 
	p0_b0 = p0_b0*8+p0_b0/4; 
	p0_b1 = p0_b1*8+p0_b1/4; 
	}

	if((offset_mode_p1_c0 & 0x1) == 0x0 )
	{
	p1_r0 = (block_data>>38 ) %16; 
	p1_r1 = (block_data>>42 ) %16; 
	p1_r0 = p1_r0*16+p1_r0; 
	p1_r1 = p1_r1*16+p1_r1; 
	}
	else
	{
    p1_r0 = (block_data>>38 ) %32; 
	u32 offset_p1_r_u = (block_data>>43 ) %8; 
	s8 offset_p1_r = (offset_p1_r_u & 0x4) == 0x4 ? offset_p1_r_u+0xf8 : offset_p1_r_u;
	p1_r1 = p1_r0 + offset_p1_r; 
	p1_r0 = p1_r0*8+p1_r0/4; 
	p1_r1 = p1_r1*8+p1_r1/4; 
	}
	if((offset_mode_p1_c1 & 0x1) == 0x0 )
	{
	p1_g0 = (block_data>>46 ) %16; 
	p1_g1 = (block_data>>50 ) %16; 
	p1_g0 = p1_g0*16+p1_g0; 
	p1_g1 = p1_g1*16+p1_g1; 
	}
	else
	{
    p1_g0 = (block_data>>46 ) %32; 
	u32 offset_p1_g_u = (block_data>>51 ) %8; 
	s8 offset_p1_g = (offset_p1_g_u & 0x4) == 0x4 ? offset_p1_g_u+0xf8 : offset_p1_g_u;
	p1_g1 = p1_g0 + offset_p1_g; 
	p1_g0 = p1_g0*8+p1_g0/4; 
	p1_g1 = p1_g1*8+p1_g1/4; 
	}
	if((offset_mode_p1_c2 & 0x1) == 0x0 )
	{
	p1_b0 = (block_data>>54 ) %16; 
	p1_b1 = (block_data>>58 ) %16;  
    p1_b0 = p1_b0*16+p1_b0; 
	p1_b1 = p1_b1*16+p1_b1;
	}
	else
	{
    p1_b0 = (block_data>>54 ) %32; 
	u32 offset_p1_b_u = (block_data>>59 ) %8; 
	s8 offset_p1_b = (offset_p1_b_u & 0x4) == 0x4 ? offset_p1_b_u+0xf8 : offset_p1_b_u;
	p1_b1 = p1_b0 + offset_p1_b; 
	p1_b0 = p1_b0*8+p1_b0/4; 
	p1_b1 = p1_b1*8+p1_b1/4; 
	}

	if((offset_mode_p2_c0 & 0x1) == 0x0 )
	{
	p2_r0 = (block_data>>62 ) %16; 
	p2_r1 = (block_data>>66 ) %16; 
	p2_r0 = p2_r0*16+p2_r0; 
	p2_r1 = p2_r1*16+p2_r1; 
	}
	else
	{
    p2_r0 = (block_data>>62 ) %32; 
	u32 offset_p2_r_u = (block_data>>67 ) %8; 
	s8 offset_p2_r = (offset_p2_r_u & 0x4) == 0x4 ? offset_p2_r_u+0xf8 : offset_p2_r_u;
	p2_r1 = p2_r0 + offset_p2_r; 
	p2_r0 = p2_r0*8+p2_r0/4; 
	p2_r1 = p2_r1*8+p2_r1/4; 
	}
	if((offset_mode_p2_c1 & 0x1) == 0x0 )
	{
	p2_g0 = (block_data>>70 ) %16; 
	p2_g1 = (block_data>>74 ) %16; 
	p2_g0 = p2_g0*16+p2_g0; 
	p2_g1 = p2_g1*16+p2_g1; 
	}
	else
	{
    p2_g0 = (block_data>>70 ) %32; 
	u32 offset_p2_g_u = (block_data>>75 ) %8; 
	s8 offset_p2_g = (offset_p2_g_u & 0x4) == 0x4 ? offset_p2_g_u+0xf8 : offset_p2_g_u;
	p2_g1 = p2_g0 + offset_p2_g; 
	p2_g0 = p2_g0*8+p2_g0/4; 
	p2_g1 = p2_g1*8+p2_g1/4; 
	}
	if((offset_mode_p2_c2 & 0x1) == 0x0 )
	{
	p2_b0 = (block_data>>78 ) %16; 
	p2_b1 = (block_data>>82 ) %16;  
    p2_b0 = p2_b0*16+p2_b0; 
	p2_b1 = p2_b1*16+p2_b1;
	}
	else
	{
    p2_b0 = (block_data>>78 ) %32; 
	u32 offset_p2_b_u = (block_data>>83 ) %8; 
	s8 offset_p2_b = (offset_p2_b_u & 0x4) == 0x4 ? offset_p2_b_u+0xf8 : offset_p2_b_u;
	p2_b1 = p2_b0 + offset_p2_b; 
	p2_b0 = p2_b0*8+p2_b0/4; 
	p2_b1 = p2_b1*8+p2_b1/4; 
	}

 

	u64 weight_table = block_data>>96;
    u8 weight = weight_table%4;
    weight_32(weight,&weight);
	u8 r,g,b,a;
	if(cur_par == 0)
	{
	interpolate_32(p0_r0,p0_r1,weight,&r);
	interpolate_32(p0_g0,p0_g1,weight,&g);
	interpolate_32(p0_b0,p0_b1,weight,&b);
	}
	else if(cur_par == 1)
	{
    interpolate_32(p1_r0,p1_r1,weight,&r);
	interpolate_32(p1_g0,p1_g1,weight,&g);
	interpolate_32(p1_b0,p1_b1,weight,&b);
	}
	else
	{
    interpolate_32(p2_r0,p2_r1,weight,&r);
	interpolate_32(p2_g0,p2_g1,weight,&g);
	interpolate_32(p2_b0,p2_b1,weight,&b);
	}
	*out_color = (0xff<<24) + (r<<16) + (g<<8) + b;
    
}

void stc_rgb_two_partition_two_plane_dec(u8 x,u8 y,u128 block_data,u32* out_color)
{
    u8 p0_r0;
    u8 p0_g0;
	u8 p0_b0;
    u8 p0_r1;
    u8 p0_g1;
	u8 p0_b1;
	u8 p1_r0;
    u8 p1_g0;
	u8 p1_b0;
    u8 p1_r1;
    u8 p1_g1;
	u8 p1_b1;
	u32 seed = block_data>>54;
	seed = seed%1024;
	//seed = seed + 2*1024;
	u8 cur_par=select_partition(seed,x,y,0,2, 1);
	u32 offset_p0 = (block_data>>2)%2;
    u32 offset_p1 = (block_data>>3)%2;
	if((offset_p0 & 0x1) == 0x0)
	{
	p0_r0 = (block_data>>6 ) %16; 
	p0_r1 = (block_data>>10) %16; 
	p0_g0 = (block_data>>14) %16; 
    p0_g1 = (block_data>>18) %16; 
    p0_b0 = (block_data>>22) %16; 
	p0_b1 = (block_data>>26) %16; 
	p0_r0 = p0_r0*16+p0_r0; 
	p0_r1 = p0_r1*16+p0_r1; 
	p0_g0 = p0_g0*16+p0_g0; 
    p0_g1 = p0_g1*16+p0_g1; 
    p0_b0 = p0_b0*16+p0_b0; 
	p0_b1 = p0_b1*16+p0_b1; 
	}
	else
	{
	u32 offset_p0_r01_u = (block_data>>11)%8;
	u32 offset_p0_g01_u = (block_data>>19)%8;
    u32 offset_p0_b01_u = (block_data>>27)%8;
	s8 offset_p0_r01 = (offset_p0_r01_u & 0x4) == 0x4 ? (offset_p0_r01_u+0xf8): offset_p0_r01_u;
	s8 offset_p0_g01 = (offset_p0_g01_u & 0x4) == 0x4 ? (offset_p0_g01_u+0xf8): offset_p0_g01_u;
    s8 offset_p0_b01 = (offset_p0_b01_u & 0x4) == 0x4 ? (offset_p0_b01_u+0xf8): offset_p0_b01_u;
	p0_r0 = (block_data>>6 ) %32; 
	p0_r1 = p0_r0 + offset_p0_r01; 
	p0_g0 = (block_data>>14) %32; 
    p0_g1 = p0_g0 + offset_p0_g01; 
    p0_b0 = (block_data>>22) %32; 
	p0_b1 = p0_b0 + offset_p0_b01;  
	p0_r0 = p0_r0 * 8 + p0_r0/4;
	p0_r1 = p0_r1 * 8 + p0_r1/4;
	p0_g0 = p0_g0 * 8 + p0_g0/4;
    p0_g1 = p0_g1 * 8 + p0_g1/4;
    p0_b0 = p0_b0 * 8 + p0_b0/4;
	p0_b1 = p0_b1 * 8 + p0_b1/4;
	}

	if((offset_p1 & 0x1) == 0x0)
	{
	p1_r0 = (block_data>>30) %16; 
	p1_r1 = (block_data>>34) %16; 
	p1_g0 = (block_data>>38) %16; 
    p1_g1 = (block_data>>42) %16; 
    p1_b0 = (block_data>>46) %16; 
	p1_b1 = (block_data>>50) %16; 
	p1_r0 = p1_r0*16+p1_r0; 
	p1_r1 = p1_r1*16+p1_r1; 
	p1_g0 = p1_g0*16+p1_g0; 
    p1_g1 = p1_g1*16+p1_g1; 
    p1_b0 = p1_b0*16+p1_b0; 
	p1_b1 = p1_b1*16+p1_b1; 
	}
	else
	{
    u8 offset_p1_r01_u = (block_data>>35)%8;
	u8 offset_p1_g01_u = (block_data>>43)%8;
    u8 offset_p1_b01_u = (block_data>>51)%8;
	s8 offset_p1_r01 = (offset_p1_r01_u & 0x4) == 0x4 ? (offset_p1_r01_u+0xf8): offset_p1_r01_u;
	s8 offset_p1_g01 = (offset_p1_g01_u & 0x4) == 0x4 ? (offset_p1_g01_u+0xf8): offset_p1_g01_u;
    s8 offset_p1_b01 = (offset_p1_b01_u & 0x4) == 0x4 ? (offset_p1_b01_u+0xf8): offset_p1_b01_u;
	p1_r0 = (block_data>>30) %32; 
	p1_r1 = p1_r0 + offset_p1_r01; 
	p1_g0 = (block_data>>38) %32; 
    p1_g1 = p1_g0 + offset_p1_g01; 
    p1_b0 = (block_data>>46) %32; 
	p1_b1 = p1_b0 + offset_p1_b01; 
	p1_r0 = p1_r0 * 8 + p1_r0/4;
	p1_r1 = p1_r1 * 8 + p1_r1/4;
	p1_g0 = p1_g0 * 8 + p1_g0/4;
    p1_g1 = p1_g1 * 8 + p1_g1/4;
    p1_b0 = p1_b0 * 8 + p1_b0/4;
	p1_b1 = p1_b1 * 8 + p1_b1/4;
	}

	u8 type = (block_data>>4)%4;

	u64 weight_table = block_data>>64;
	u8 weight = (weight_table >> ((y*4+x)*4))%16;
	u8 w_p0 = weight%4;
	u8 w_p1 = weight/4;
    
	weight_32(w_p0,&w_p0);
	weight_32(w_p1,&w_p1);
	u8 wr = type == 2 ? w_p0 : w_p1;
	u8 wg = type == 1 ? w_p0 : w_p1;
	u8 wb = type == 0 ? w_p0 : w_p1;

    
	u8 r,g,b,a;
	if(cur_par == 0)
	{
	interpolate_32(p0_r0,p0_r1,wr,&r);
	interpolate_32(p0_g0,p0_g1,wg,&g);
	interpolate_32(p0_b0,p0_b1,wb,&b);
	}
	else
	{
    interpolate_32(p1_r0,p1_r1,wr,&r);
	interpolate_32(p1_g0,p1_g1,wg,&g);
	interpolate_32(p1_b0,p1_b1,wb,&b);
	}
	*out_color = (0xff<<24) + (r<<16) + (g<<8) + b;
    
}

void stc_rgb_dec(u8 x,u8 y,u128 block_data,u32* out_color)
{
	u32 test = block_data>>0;
	if((test & 0x1) == 0)   //one partition
		stc_rgb_one_partition_dec(x,y,block_data,out_color);
	else if((test & 0x3) == 0x1)  //two plane two partition
		stc_rgb_two_partition_two_plane_dec(x,y,block_data,out_color);
	else if((test & 0x7) == 0x3)  //two partition
		stc_rgb_two_partition_dec(x,y,block_data,out_color);
	else if((test & 0xf) == 0x7)  //two plane one partition
		stc_rgb_one_partition_two_plane_dec(x,y,block_data,out_color);
	else                              //three partition
		stc_rgb_three_partition_dec(x,y,block_data,out_color);
}