#ifndef __SOFT_LIB_H__
#define __SOFT_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
/* @note If not use soft float function, please set it to 0
 */
#define CONFIG_CSI_FLOAT_SOFT_ENABLE    1U

#define itofix(i_x, q)          ((unsigned int)(i_x) << (q))                            // int to Fixed point
#define Mulfix(fx_x, fx_y, q)   (((unsigned int)(fx_x) * (unsigned int)(fx_y)) >> (q))  // Fixed*Fixed
#define Divfix(fx_x, fx_y, q)   (((unsigned int)(fx_x) << (q)) / (unsigned int)(fx_y))  // Fixed/Fixed

void bubble_sort_u32(unsigned int arr[], unsigned int length);
void bubble_sort_u16(unsigned short arr[], unsigned int length);
void bubble_sort_u8(unsigned char arr[], unsigned int length);
void bubble_sort_s32(int arr[], unsigned int length);
void bubble_sort_s16(short arr[], unsigned int length);
void bubble_sort_s8(char arr[], unsigned int length);

#if(defined CONFIG_CSI_FLOAT_SOFT_ENABLE && CONFIG_CSI_FLOAT_SOFT_ENABLE == 1U)
int float_to_int(float x, uint8_t Q);
float int_to_float(int xq, uint8_t Q);
#endif /* CONFIG_CSI_FLOAT_SOFT_ENABLE */

unsigned short multip_fixed_s16(unsigned short a, unsigned short b, unsigned char q);
unsigned char multip_fixed_signed(unsigned char a, unsigned char b, unsigned char q);
unsigned char multip_fixed_unsigned(unsigned char a, unsigned char b, unsigned char q);

unsigned char normalization_fixed_signed(unsigned char a, unsigned char q, unsigned char min, unsigned char max);
unsigned char normalization_fixed_unsigned(unsigned char a, unsigned char q, unsigned char min, unsigned char max);

#define DEFARG(para, defval) ((#para[0]) ? (para + 0) : defval)

unsigned char crc4_itu(unsigned char *data, unsigned int length, unsigned char crc_i);
unsigned char crc5_epc(unsigned char *data, unsigned int length, unsigned char crc_i);
unsigned char crc5_itu(unsigned char *data, unsigned int length, unsigned char crc_i);
unsigned char crc5_usb(unsigned char *data, unsigned int length, unsigned char crc_i);
unsigned char crc6_itu(unsigned char *data, unsigned int length, unsigned char crc_i);
unsigned char crc7_mmc(unsigned char *data, unsigned int length, unsigned char crc_i);
unsigned char crc8(unsigned char *data, unsigned int length, unsigned char crc_i);
unsigned char crc8_itu(unsigned char *data, unsigned int length, unsigned char crc_i);
unsigned char crc8_rohc(unsigned char *data, unsigned int length, unsigned char crc_i);
unsigned char crc8_maxim(unsigned char *data, unsigned int length, unsigned char crc_i);
unsigned short crc16_ibm(unsigned char *data, unsigned int length, unsigned short crc_i);
unsigned short crc16_maxim(unsigned char *data, unsigned int length, unsigned short crc_i);
unsigned short crc16_usb(unsigned char *data, unsigned int length, unsigned short crc_i);
unsigned short crc16_modbus(unsigned char *data, unsigned int length, unsigned short crc_i);
unsigned short crc16_ccitt(unsigned char *data, unsigned int length, unsigned short crc_i);
unsigned short crc16_ccitt_false(unsigned char *data, unsigned int length, unsigned short crc_i);
unsigned short crc16_x25(unsigned char *data, unsigned int length, unsigned short crc_i);
unsigned short crc16_xmodem(unsigned char *data, unsigned int length, unsigned short crc_i);
unsigned short crc16_dnp(unsigned char *data, unsigned int length, unsigned short crc_i);
unsigned int crc32_le(unsigned char *data, unsigned int length, unsigned int crc_i);
unsigned int crc32_mpeg_2(unsigned char *data, unsigned int length, unsigned int crc_i);

#ifdef __cplusplus
}
#endif

#endif /* __SOFT_LIB_H__ */
