#ifndef __FIX_UNIT_H__
#define __FIX_UNIT_H__

//#include "CPU1_CMSDK_CM0.h"
#include "core_rv32.h"

#ifdef __cplusplus
extern "C" {
#endif

#define   FIX_USIGN (0 << 1)
#define   FIX_SIGN  (1 << 1)
#define   DONE_POS  2
#define   FIX_START 1

typedef struct 
{
    __IO uint32_t ctrl;
    __IO uint32_t divisor;
    __IO uint32_t dividend_low;
    __IO uint32_t dividend_high;
    __IO uint32_t div_quotient_low;
    __IO uint32_t div_quotient_high;
    __IO uint32_t div_reminder;
} DIV_UNIT_TypeDef;

#define   CORDIC_ROTATION 	(0 << 1)
#define   CORDIC_VECTOR  	(1 << 1)

typedef struct 
{
    __IO uint32_t ctrl;
    __IO uint32_t iter;
    __IO uint32_t x_i;
    __IO uint32_t y_i;
    __IO uint32_t z_i;
    __IO uint32_t x_o;
    __IO uint32_t y_o;
	__IO uint32_t z_o;
} CORDIC_UNIT_TypeDef;   


#define   DIV_UNIT_BASE 		0x50100000
#define   CORDIC_UNIT_BASE 		0x50100040
#define   GC_DIV_UNIT 		((DIV_UNIT_TypeDef*)DIV_UNIT_BASE)
#define   GC_CORDIC_UNIT 	((CORDIC_UNIT_TypeDef*)CORDIC_UNIT_BASE) 

int64_t fix_mult(int x, int y, int shiftR);
int64_t fix_mult20x20(int x, int y);
int64_t fix_mult20x13(int x, int y);
int64_t fix_mult13x20(int x, int y);
int64_t fix_mult13x13(int x, int y);
int64_t fix_div64(int x_high, int x_low, int y);
int64_t fix_div(int x, int y, int shiftL);
int fix_mod64(int x_high, int x_low, int y);
int fix_mod(int x, int y, int shiftL);

void fix_mult_vec13x13(int result[], int x[],int y,int acc);
void fix_mult_vec20x20(int result[], int x[],int y,int acc);
void fix_mult_vec20x13(int result[], int x[],int y,int acc);
void fix_mult_vec13x20(int result[], int x[],int y,int acc);

void fix_CORDIC(int Radian, int* cos, int* sin);
int fix_CORDIC_GetAngleFromTan(int angleTan);
#ifdef __cplusplus
}
#endif

#endif
