#include "platform.h"
#include "gc90x_fix.h"

#ifdef __cplusplus
extern "C" {
#endif

int64_t csi_fix_mult(int x, int y, int shiftR);
int64_t csi_fix_mult20x20(int x, int y);
int64_t csi_fix_mult20x13(int x, int y);
int64_t csi_fix_mult13x20(int x, int y);
int64_t csi_fix_mult13x13(int x, int y);
int64_t csi_fix_div64(int x_high, int x_low, int y);
int64_t csi_fix_div(int x, int y, int shiftL);
int csi_fix_mod64(int x_high, int x_low, int y);
int csi_fix_mod(int x, int y, int shiftL);

void csi_fix_mult_vec13x13(int result[], int x[],int y,int acc);
void csi_fix_mult_vec20x20(int result[], int x[],int y,int acc);
void csi_fix_mult_vec20x13(int result[], int x[],int y,int acc);
void csi_fix_mult_vec13x20(int result[], int x[],int y,int acc);

#if defined(CHIP_GC9002)
void csi_fix_CORDIC(int Radian, int* cos, int* sin);
int csi_fix_CORDIC_GetAngleFromTan(int angleTan);
#endif

#ifdef __cplusplus
}
#endif
