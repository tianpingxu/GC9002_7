/**
  ******************************************************************************
  * @file    gc90x_fixunit.c
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    15-August-2019
  * @brief   This file provides all the fix unit firmware functions.
  ******************************************************************************
	*/
#include "gc90x_fix.h"
#include <assert.h>

//-----------------------------
// name: fix_mult
// @param1 int x
// @param2 int y
// @param3 int shiftR
// addtional comments :
//		return ((x * y) >> shiftR)
//-----------------------------
int64_t fix_mult(int x, int y, int shiftR)
{
	return (int64_t)(((int64_t)x * y) >> shiftR);
}

//-----------------------------
// name: fix_mult20x20
// @param1 int x (fixed-point 1.11.20)
// @param2 int y (fixed-point 1.11.20)
// addtional comments :
//		return ((x * y) >> 20)
//-----------------------------
int64_t fix_mult20x20(int x, int y)
{
	return (int64_t)(((int64_t)x * y) >> 20);
}

//-----------------------------
// name: fix_mult20x13
// @param1 int x (fixed-point 1.11.20)
// @param2 int y (fixed-point 1.18.13)
// addtional comments :
//		return ((x * y) >> 20)
//-----------------------------
int64_t fix_mult20x13(int x, int y)
{
	return (int64_t)(((int64_t)x * y) >> 20);
}

//-----------------------------
// name: fix_mult13x20
// @param1 int x (fixed-point 1.18.13)
// @param2 int y (fixed-point 1.11.20)
// addtional comments :
//		return ((x * y) >> 13)
//-----------------------------
int64_t fix_mult13x20(int x, int y)
{
	return (int64_t)(((int64_t)x * y) >> 13);
}

//-----------------------------
// name: fix_mult13x13
// @param1 int x (fixed-point 1.18.13)
// @param2 int y (fixed-point 1.18.13)
// addtional comments :
//		return ((x * y) >> 13)
//-----------------------------
int64_t fix_mult13x13(int x, int y)
{
	return (int64_t)(((int64_t)x * y) >> 13);
}

//-----------------------------
// name: fix_div64
// @param1 int x_high
// @param2 int x_low
// @param3 int y
// addtional comments :
//		return (((x_high << 32) + x_low) / y)
//-----------------------------
int64_t fix_div64(int x_high, int x_low, int y)
{
	if(y == 0)
	{
		x_high = *(uint32_t *)(0x20000002);
	}
    assert(y!=0);
	
	GC_DIV_UNIT->divisor = y;
	GC_DIV_UNIT->dividend_high = x_high;
	GC_DIV_UNIT->dividend_low = x_low;
	GC_DIV_UNIT->ctrl = FIX_SIGN + FIX_START;
	while((GC_DIV_UNIT->ctrl & (1<<DONE_POS))==0);
	
	return *((int64_t*)&GC_DIV_UNIT->div_quotient_low);     
}

//-----------------------------
// name: fix_div
// @param1 int x
// @param2 int y
// @param3 int shiftL
// addtional comments :
//		return ((x << shiftL) / y)
//-----------------------------
int64_t fix_div(int x, int y, int shiftL)
{
	int64_t tmp = 0;
	if(y == 0)
	{
		x = *(uint32_t *)(0x20000002);
	}
    assert(y!=0);
	
	tmp = (((int64_t)x) << shiftL);
	GC_DIV_UNIT->divisor = y;
	GC_DIV_UNIT->dividend_high = (tmp>>32);
	GC_DIV_UNIT->dividend_low = (tmp);
	GC_DIV_UNIT->ctrl = FIX_SIGN + FIX_START;
	while((GC_DIV_UNIT->ctrl & (1<<DONE_POS))==0);
	
	return *((int64_t*)&GC_DIV_UNIT->div_quotient_low);  
}

//-----------------------------
// name: fix_mod64
// @param1 int x_high
// @param2 int x_low
// @param3 int y
// addtional comments :
//		return (((x_high << 32) + x_low) % y)
//-----------------------------
int fix_mod64(int x_high, int x_low, int y)
{
	assert(y!=0);

	GC_DIV_UNIT->divisor = y;
	GC_DIV_UNIT->dividend_high = x_high;
	GC_DIV_UNIT->dividend_low = x_low;
	GC_DIV_UNIT->ctrl = FIX_SIGN + FIX_START;
	while((GC_DIV_UNIT->ctrl & (1<<DONE_POS))==0);
	
	return GC_DIV_UNIT->div_reminder;
}

//-----------------------------
// name: fix_mod
// @param1 int x
// @param2 int y
// @param3 int shiftL
// addtional comments :
//		return ((x << shiftL) % y)
//-----------------------------
int fix_mod(int x, int y, int shiftL)    
{
	int64_t tmp = 0;
	if(y == 0)
	{
		x = *(uint32_t *)(0x20000002);
	}
    assert(y!=0);
	
	tmp = (((int64_t)x) << shiftL);
	GC_DIV_UNIT->divisor = y;
	GC_DIV_UNIT->dividend_high = (tmp>>32);
	GC_DIV_UNIT->dividend_low = (tmp);
	GC_DIV_UNIT->ctrl = FIX_SIGN + FIX_START;
	while((GC_DIV_UNIT->ctrl & (1<<DONE_POS))==0);
	
	return GC_DIV_UNIT->div_reminder;
}

//-----------------------------
// name: fix_mult_vec20x20
// @param1 int result[]
// @param2 int x[]
// @param3 int y
// @param4 int acc
// addtional comments :
//		when acc = 0, it means vector multiplication start(result[0] = 0 + ((x[0] * y) >> 20)...)
//		when acc = 1, it means cumulative last result(result[0] = lastresult[0] + ((x[0] * y) >> 20)...)
//-----------------------------
void fix_mult_vec20x20(int result[], int x[], int y, int acc)
{
	int i;

	if(acc==0)
	{
		result[0]=result[1]=result[2]=0;
	}	
	if(y==0)
	{
		return;
	}
	else if(y == 0x100000)
	{
		result[0] += x[0] ;
		result[1] += x[1] ;
		result[2] += x[2] ;			
	}	
	else
	{
		for(i=0;i<3;i++)
		{
			if(x[i] == 0) ;
			else if(x[i] == 0x100000) result[i] += y;
			else
			{
				result[i] += (((int64_t)x[i] * y) >> 20);
			}
		}	
	}
}

//-----------------------------
// name: fix_mult_vec20x13
// @param1 int result[]
// @param2 int x[]
// @param3 int y
// @param4 int acc
// addtional comments :
//		when acc = 0, it means vector multiplication start(result[0] = 0 + ((x[0] * y) >> 20)...)
//		when acc = 1, it means cumulative last result(result[0] = lastresult[0] + ((x[0] * y) >> 20)...)
//-----------------------------
void fix_mult_vec20x13(int result[], int x[],int y,int acc)
{
	int i;
	if(acc==0)
	{
		result[0]=result[1]=result[2]=0;
	}	

	if(y==0)
	{
		return;
	}
	else if(y == 0x2000)
	{
		result[0] += x[0]>>7 ;
		result[1] += x[1]>>7 ;
		result[2] += x[2]>>7 ;
	}	
	else
	{
		for(i=0;i<3;i++)
		{
			if(x[i] == 0) result[i] += 0;
			else if(x[i] == 0x100000) result[i] += y;
			else
			{
				result[i] += (((int64_t)x[i] * y) >> 20);
			}
		}	
	}
}

//-----------------------------
// name: fix_mult_vec13x20
// @param1 int result[]
// @param2 int x[]
// @param3 int y
// @param4 int acc
// addtional comments :
//		when acc = 0, it means vector multiplication start(result[0] = 0 + ((x[0] * y) >> 13)...)
//		when acc = 1, it means cumulative last result(result[0] = lastresult[0] + ((x[0] * y) >> 13)...)
//-----------------------------
void fix_mult_vec13x20(int result[], int x[],int y,int acc)
{
	int i;
	if(acc==0)
	{
		result[0]=result[1]=result[2]=0;
	}

	if(y==0)
	{
		return;
	}
	else if(y == 0x100000)
	{
		result[0] += x[0]<<7 ;
		result[1] += x[1]<<7 ;
		result[2] += x[2]<<7 ;
	}	
	else
	{
		for(i=0;i<3;i++)
		{
			if(x[i] == 0) result[i] += 0;
			else if(x[i] == 0x2000) result[i] += y;
			else
			{
				result[i] += (((int64_t)x[i] * y) >> 13);
			}
		}	
	}
}

//-----------------------------
// name: fix_mult_vec13x13
// @param1 int result[]
// @param2 int x[]
// @param3 int y
// @param4 int acc
// addtional comments :
//		when acc = 0, it means vector multiplication start(result[0] = 0 + ((x[0] * y) >> 13)...)
//		when acc = 1, it means cumulative last result(result[0] = lastresult[0] + ((x[0] * y) >> 13)...)
//-----------------------------
void fix_mult_vec13x13(int result[], int x[],int y,int acc)
{
	int i;
	if(acc==0)
	{
		result[0]=result[1]=result[2]=0;
	}

	if(y==0)
	{
		return;
	}
	else if(y == 0x2000)
	{
		result[0] += x[0] ;
		result[1] += x[1] ;
		result[2] += x[2] ;
	}	
	else
	{
		for(i=0;i<3;i++)
		{
			if(x[i] == 0) result[i] += 0;
			else if(x[i] == 0x2000) result[i] += y;
			else
			{
				result[i] += (((int64_t)x[i] * y) >> 13);
			}
		}	
	}
}

//-----------------------------
// name: fix_CORDIC
// @param1 int Radian 1.27.4
// @param2 int* cos
// @param3 int* sin
// addtional comments :
//-----------------------------
void fix_CORDIC(int Radian, int* cos, int* sin)
{
	GC_CORDIC_UNIT->x_i = *cos;
	GC_CORDIC_UNIT->y_i = *sin;
	GC_CORDIC_UNIT->z_i = (Radian << 20);
	GC_CORDIC_UNIT->ctrl = CORDIC_ROTATION + FIX_START;
	while((GC_CORDIC_UNIT->ctrl & (1<<DONE_POS))==0);
	
	*cos = GC_CORDIC_UNIT->x_o;
	*sin = GC_CORDIC_UNIT->y_o;
}

//-----------------------------
// name: fix_CORDIC_GetAngleFromTan
// @param1 int angleTan 1.11.20
// addtional comments :
//		return arctan(angleTan) 1.27.4
//-----------------------------
int fix_CORDIC_GetAngleFromTan(int angleTan)
{
	GC_CORDIC_UNIT->x_i = 0x10000;
	GC_CORDIC_UNIT->y_i = (angleTan >> 4);
	GC_CORDIC_UNIT->z_i = 0;
	GC_CORDIC_UNIT->ctrl = CORDIC_VECTOR + FIX_START;
	while((GC_CORDIC_UNIT->ctrl & (1<<DONE_POS))==0);
	
	return (GC_CORDIC_UNIT->z_o >> 20);
}