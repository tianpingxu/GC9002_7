#include "csi_ahmi_fix.h"

int64_t csi_fix_mult(int x, int y, int shiftR)
{
	return fix_mult(x, y, shiftR);
}

int64_t csi_fix_mult20x20(int x, int y)
{
	return fix_mult20x20(x, y);
}

int64_t csi_fix_mult20x13(int x, int y)
{
	return fix_mult20x13(x, y);
}

int64_t csi_fix_mult13x20(int x, int y)
{
	return fix_mult13x20(x, y);
}

int64_t csi_fix_mult13x13(int x, int y)
{
	return fix_mult13x13(x, y);
}

int64_t csi_fix_div64(int x_high, int x_low, int y)
{
	return fix_div64(x_high, x_low, y);     
}

int64_t csi_fix_div(int x, int y, int shiftL)
{
	return fix_div(x, y, shiftL);  
}

int csi_fix_mod64(int x_high, int x_low, int y)
{
	return fix_mod64(x_high, x_low, y);
}

int csi_fix_mod(int x, int y, int shiftL)
{
	return fix_mod(x, y, shiftL);
}


void csi_fix_mult_vec20x20(int result[], int x[], int y, int acc)
{
	fix_mult_vec20x20(result, x, y, acc);
}

void csi_fix_mult_vec20x13(int result[], int x[],int y,int acc)
{
	fix_mult_vec20x13(result, x, y, acc);
}

void csi_fix_mult_vec13x20(int result[], int x[],int y,int acc)
{
	fix_mult_vec13x20(result, x, y, acc);
}

void csi_fix_mult_vec13x13(int result[], int x[],int y,int acc)
{
	fix_mult_vec13x13(result, x, y, acc);
}

#if defined(CHIP_GC9002)
void csi_fix_CORDIC(int Radian, int* cos, int* sin)
{
	fix_CORDIC(Radian, cos, sin);
}

int csi_fix_CORDIC_GetAngleFromTan(int angleTan)
{
	return fix_CORDIC_GetAngleFromTan(angleTan);
}
#endif
