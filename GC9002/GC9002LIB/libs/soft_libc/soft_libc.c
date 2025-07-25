#include "soft_libc.h"


void bubble_sort_u32(unsigned int arr[], unsigned int length)
{
    int flag = 1;
    unsigned int temp = 0;
    while(length-- && flag)
    {
        flag = 0;
        for(unsigned int i = 0; i < length; i++)
        {
            if(arr[i + 1] < arr[i])
            {
                flag = 1;
                temp = arr[i + 1];
                arr[i + 1] = arr[i];
                arr[i] = temp;
            }
        }
    }
}
void bubble_sort_u16(unsigned short arr[], unsigned int length)
{
    int flag = 1;
    unsigned short temp = 0;
    while(length-- && flag)
    {
        flag = 0;
        for(unsigned int i = 0; i < length; i++)
        {
            if(arr[i + 1] < arr[i])
            {
                flag = 1;
                temp = arr[i + 1];
                arr[i + 1] = arr[i];
                arr[i] = temp;
            }
        }
    }
}
void bubble_sort_u8(unsigned char arr[], unsigned int length)
{
    int flag = 1;
    unsigned char temp = 0;
    while(length-- && flag)
    {
        flag = 0;
        for(unsigned int i = 0; i < length; i++)
        {
            if(arr[i + 1] < arr[i])
            {
                flag = 1;
                temp = arr[i + 1];
                arr[i + 1] = arr[i];
                arr[i] = temp;
            }
        }
    }
}
void bubble_sort_s32(int arr[], unsigned int length)
{
    int flag = 1;
    int temp = 0;
    while(length-- && flag)
    {
        flag = 0;
        for(unsigned int i = 0; i < length; i++)
        {
            if(arr[i + 1] < arr[i])
            {
                flag = 1;
                temp = arr[i + 1];
                arr[i + 1] = arr[i];
                arr[i] = temp;
            }
        }
    }
}
void bubble_sort_s16(short arr[], unsigned int length)
{
    int flag = 1;
    short temp = 0;
    while(length-- && flag)
    {
        flag = 0;
        for(unsigned int i = 0; i < length; i++)
        {
            if(arr[i + 1] < arr[i])
            {
                flag = 1;
                temp = arr[i + 1];
                arr[i + 1] = arr[i];
                arr[i] = temp;
            }
        }
    }
}
void bubble_sort_s8(char arr[], unsigned int length)
{
    int flag = 1;
    char temp = 0;
    while(length-- && flag)
    {
        flag = 0;
        for(unsigned int i = 0; i < length; i++)
        {
            if(arr[i + 1] < arr[i])
            {
                flag = 1;
                temp = arr[i + 1];
                arr[i + 1] = arr[i];
                arr[i] = temp;
            }
        }
    }
}
#if(defined CONFIG_CSI_FLOAT_SOFT_ENABLE && CONFIG_CSI_FLOAT_SOFT_ENABLE == 1U)
/*
 *  浮点数(x)转换为定点数(xq)：xq=(int)x*2^Q
 *  定点数(xq)转换为浮点数(x)：x=(float)xq*2^-Q
 */
int float_to_int(float x, uint8_t Q)
{
    int xq = (0x1 << Q);
    xq = (int)(x * (float)xq);
    return xq;
}

float int_to_float(int xq, uint8_t Q)
{
    int val = (0x1 << Q);
    float x = (float)((float)xq/(float)val);
    return x;
}
#endif /* CONFIG_CSI_FLOAT_SOFT_ENABLE */


/*
 a = 0x40   // 0.5
 b = 0x80   //-128
 mul = 0xC0 //-64


*/
unsigned short multip_fixed_s16(unsigned short a, unsigned short b, unsigned char q)
{
    unsigned short sign = 0x8000;
    unsigned int mul = 0;
    unsigned int u32_a = 0;
    unsigned int u32_b = 0;
    if(((a & 0x8000) == 0 && (b & 0x8000) == 0) || 
       ((a & 0x8000) != 0 && (b & 0x8000) != 0))
    {
        sign = 0x0000;
    }
    if(a & 0x8000) {
        u32_a = 0x7FFF - (unsigned int)(a & 0x7FFF);
    } else {
        u32_a = (unsigned int)(a & 0x7FFF);
    }
    if(b & 0x8000) {
        u32_b = 0x7FFF - (unsigned int)(b & 0x7FFF);
    } else {
        u32_b = (unsigned int)(b & 0x7FFF);
    }
    mul = Mulfix(u32_a, u32_b, q);
    if(sign)
    {
        mul = 0x7FFF - mul;
    }
    return (sign | (mul & 0x7FFF));
}
unsigned char multip_fixed_signed(unsigned char a, unsigned char b, unsigned char q)
{
    unsigned char sign = 0x80;
    unsigned int mul = 0;
    unsigned int u32_a = 0;
    unsigned int u32_b = 0;
    if(((a & 0x80) == 0 && (b & 0x80) == 0) || 
       ((a & 0x80) != 0 && (b & 0x80) != 0))
    {
        sign = 0x00;
    }
    if((a & 0x80)) {
        u32_a = 0x7F - (unsigned int)(a & 0x7F);
    } else {
        u32_a = (unsigned int)(a & 0x7F);
    }
    if((a & 0x80)) {
        u32_b = 0x7F - (unsigned int)(b & 0x7F);
    } else {
        u32_b = (unsigned int)(b & 0x7F);
    }
    
    mul = Mulfix(u32_a, u32_b, q);
    if(sign)
    {
        mul = 0x7F - mul;
    }
    return (sign | (mul & 0x7F));
}


/**
  * @brief  定点数乘积运算
  * @note 当积大于0xFF，返回0xFF
  * @retval 定点数积
  */
unsigned char multip_fixed_unsigned(unsigned char a, unsigned char b, unsigned char q)
{
    unsigned int mul = 0;
    unsigned int u32_a = 0;
    unsigned int u32_b = 0;
    u32_a = (short)(a << 8);
    u32_b = (short)(b << 8);
    mul = (u32_a*u32_b >> 16);
    mul >>= q;
    if(mul > 0xFF)//溢出
    {
        return 0xFF;
    }
    return (mul & 0xFF);
}

/**
  * @brief  定点数除法运算
  * @note 当商大于0xFF，返回0xFF
  * @retval 定点数商
  */
unsigned char division_fixed_unsigned(unsigned char a, unsigned char b, unsigned char q)
{
    unsigned int mul = 0;
    unsigned int u32_a = 0;
    unsigned int u32_b = 0;
    u32_a = (unsigned int)a;
    u32_b = (unsigned int)b;
    mul = u32_a*u32_b;
    mul <<= q;
    if(mul > 0xFF)//溢出
    {
        return 0xFF;
    }
    return mul;
}

unsigned char normalization_fixed_signed(unsigned char a, unsigned char q, unsigned char min, unsigned char max)
{
    unsigned char ret = 0;
    unsigned char max_min = 0;
    unsigned char a_min = 0;
    if((min & 0x80) != 0)//min < 0
    {
        max_min = (max & 0x7F) + (min & 0x7F);
        if((a & 0x80) != 0)//a < 0
        {
            a_min = (min & 0x7F) - (a & 0x7F);
        }
        else
        {
            a_min = (a & 0x7F) + (min & 0x7F);
        }
    }
    else//min > 0
    {
        max_min = (max & 0x7F) - (min & 0x7F);
        a_min = (a & 0x7F) - (min & 0x7F);
    }
    ret = (unsigned char)Divfix(a_min, max_min, q);
    return ret;
}

unsigned char normalization_fixed_unsigned(unsigned char a, unsigned char q, unsigned char min, unsigned char max)
{
    unsigned char ret = 0;
    unsigned char max_min = 0;
    unsigned char a_min = 0;
    max_min = max - min;
    a_min = a - min;
    ret = (unsigned char)Divfix(a_min, max_min, q);
    return ret;
}

/******************************************************************************
 * Name:    CRC-4/ITU           x4+x+1
 * Poly:    0x03
 * Init:    0x00
 * Refin:   True
 * Refout:  True
 * Xorout:  0x00
 * Note:
 *****************************************************************************/
unsigned char crc4_itu(unsigned char *data, unsigned int length, unsigned char crc_i)
{
    unsigned char i;
    unsigned char crc = crc_i;                // Initial value
    while(length--)
    {
        crc ^= *data++;                 // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x0C;// 0x0C = (reverse 0x03)>>(8-4)
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-5/EPC           x5+x3+1
 * Poly:    0x09
 * Init:    0x09
 * Refin:   False
 * Refout:  False
 * Xorout:  0x00
 * Note:
 *****************************************************************************/
unsigned char crc5_epc(unsigned char *data, unsigned int length, unsigned char crc_i)
{
    unsigned char i;
    unsigned char crc = crc_i;        // Initial value: 0x48 = 0x09<<(8-5)
    while(length--)
    {
        crc ^= *data++;        // crc ^= *data; data++;
        for ( i = 0; i < 8; i++ )
        {
            if ( crc & 0x80 )
                crc = (crc << 1) ^ 0x48;        // 0x48 = 0x09<<(8-5)
            else
                crc <<= 1;
        }
    }
    return crc >> 3;
}

/******************************************************************************
 * Name:    CRC-5/ITU           x5+x4+x2+1
 * Poly:    0x15
 * Init:    0x00
 * Refin:   True
 * Refout:  True
 * Xorout:  0x00
 * Note:
 *****************************************************************************/
unsigned char crc5_itu(unsigned char *data, unsigned int length, unsigned char crc_i)
{
    unsigned char i;
    unsigned char crc = crc_i;                // Initial value
    while(length--)
    {
        crc ^= *data++;                 // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x15;// 0x15 = (reverse 0x15)>>(8-5)
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-5/USB           x5+x2+1
 * Poly:    0x05
 * Init:    0x1F
 * Refin:   True
 * Refout:  True
 * Xorout:  0x1F
 * Note:
 *****************************************************************************/
unsigned char crc5_usb(unsigned char *data, unsigned int length, unsigned char crc_i)
{
    unsigned char i;
    unsigned char crc = crc_i;                // Initial value: 0x1F
    while(length--)
    {
        crc ^= *data++;                 // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x14;// 0x14 = (reverse 0x05)>>(8-5)
            else
                crc = (crc >> 1);
        }
    }
    return crc ^ 0x1F;
}

/******************************************************************************
 * Name:    CRC-6/ITU           x6+x+1
 * Poly:    0x03
 * Init:    0x00
 * Refin:   True
 * Refout:  True
 * Xorout:  0x00
 * Note:
 *****************************************************************************/
unsigned char crc6_itu(unsigned char *data, unsigned int length, unsigned char crc_i)
{
    unsigned char i;
    unsigned char crc = crc_i;         // Initial value
    while(length--)
    {
        crc ^= *data++;        // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x30;// 0x30 = (reverse 0x03)>>(8-6)
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-7/MMC           x7+x3+1
 * Poly:    0x09
 * Init:    0x00
 * Refin:   False
 * Refout:  False
 * Xorout:  0x00
 * Use:     MultiMediaCard,SD,ect.
 *****************************************************************************/
unsigned char crc7_mmc(unsigned char *data, unsigned int length, unsigned char crc_i)
{
    unsigned char i;
    unsigned char crc = crc_i;        // Initial value
    while(length--)
    {
        crc ^= *data++;        // crc ^= *data; data++;
        for ( i = 0; i < 8; i++ )
        {
            if ( crc & 0x80 )
                crc = (crc << 1) ^ 0x12;        // 0x12 = 0x09<<(8-7)
            else
                crc <<= 1;
        }
    }
    return crc >> 1;
}

/******************************************************************************
 * Name:    CRC-8               x8+x2+x+1
 * Poly:    0x07
 * Init:    0x00
 * Refin:   False
 * Refout:  False
 * Xorout:  0x00
 * Note:
 *****************************************************************************/
unsigned char crc8(unsigned char *data, unsigned int length, unsigned char crc_i)
{
    unsigned char i;
    unsigned char crc = crc_i;        // Initial value
    while(length--)
    {
        crc ^= *data++;        // crc ^= *data; data++;
        for ( i = 0; i < 8; i++ )
        {
            if ( crc & 0x80 )
                crc = (crc << 1) ^ 0x07;
            else
                crc <<= 1;
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-8/ITU           x8+x2+x+1
 * Poly:    0x07
 * Init:    0x00
 * Refin:   False
 * Refout:  False
 * Xorout:  0x55
 * Alias:   CRC-8/ATM
 *****************************************************************************/
unsigned char crc8_itu(unsigned char *data, unsigned int length, unsigned char crc_i)
{
    unsigned char i;
    unsigned char crc = crc_i;        // Initial value
    while(length--)
    {
        crc ^= *data++;        // crc ^= *data; data++;
        for ( i = 0; i < 8; i++ )
        {
            if ( crc & 0x80 )
                crc = (crc << 1) ^ 0x07;
            else
                crc <<= 1;
        }
    }
    return crc ^ 0x55;
}

/******************************************************************************
 * Name:    CRC-8/ROHC          x8+x2+x+1
 * Poly:    0x07
 * Init:    0xFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0x00
 * Note:
 *****************************************************************************/
unsigned char crc8_rohc(unsigned char *data, unsigned int length, unsigned char crc_i)
{
    unsigned char i;
    unsigned char crc = crc_i;         // Initial value : 0xFF
    while(length--)
    {
        crc ^= *data++;            // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xE0;        // 0xE0 = reverse 0x07
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-8/MAXIM         x8+x5+x4+1
 * Poly:    0x31
 * Init:    0x00
 * Refin:   True
 * Refout:  True
 * Xorout:  0x00
 * Alias:   DOW-CRC,CRC-8/IBUTTON
 * Use:     Maxim(Dallas)'s some devices,e.g. DS18B20
 *****************************************************************************/
unsigned char crc8_maxim(unsigned char *data, unsigned int length, unsigned char crc_i)
{
    unsigned char i;
    unsigned char crc = crc_i;         // Initial value
    while(length--)
    {
        crc ^= *data++;        // crc ^= *data; data++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x8C;        // 0x8C = reverse 0x31
            else
                crc >>= 1;
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-16/IBM          x16+x15+x2+1
 * Poly:    0x8005
 * Init:    0x0000
 * Refin:   True
 * Refout:  True
 * Xorout:  0x0000
 * Alias:   CRC-16,CRC-16/ARC,CRC-16/LHA
 *****************************************************************************/
unsigned short crc16_ibm(unsigned char *data, unsigned int length, unsigned short crc_i)
{
    unsigned char i;
    unsigned short crc = crc_i;        // Initial value
    while(length--)
    {
        crc ^= *data++;        // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-16/MAXIM        x16+x15+x2+1
 * Poly:    0x8005
 * Init:    0x0000
 * Refin:   True
 * Refout:  True
 * Xorout:  0xFFFF
 * Note:
 *****************************************************************************/
unsigned short crc16_maxim(unsigned char *data, unsigned int length, unsigned short crc_i)
{
    unsigned char i;
    unsigned short crc = crc_i;        // Initial value
    while(length--)
    {
        crc ^= *data++;        // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;    // crc^0xffff
}

/******************************************************************************
 * Name:    CRC-16/USB          x16+x15+x2+1
 * Poly:    0x8005
 * Init:    0xFFFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0xFFFF
 * Note:
 *****************************************************************************/
unsigned short crc16_usb(unsigned char *data, unsigned int length, unsigned short crc_i)
{
    unsigned char i;
    unsigned short crc = crc_i;        // Initial value : 0xffff
    while(length--)
    {
        crc ^= *data++;            // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;    // crc^0xffff
}

/******************************************************************************
 * Name:    CRC-16/MODBUS       x16+x15+x2+1
 * Poly:    0x8005
 * Init:    0xFFFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0x0000
 * Note:
 *****************************************************************************/
unsigned short crc16_modbus(unsigned char *data, unsigned int length, unsigned short crc_i)
{
    unsigned char i;
    unsigned short crc = crc_i;        // Initial value : 0xffff
    while(length--)
    {
        crc ^= *data++;            // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-16/CCITT        x16+x12+x5+1
 * Poly:    0x1021
 * Init:    0x0000
 * Refin:   True
 * Refout:  True
 * Xorout:  0x0000
 * Alias:   CRC-CCITT,CRC-16/CCITT-TRUE,CRC-16/KERMIT
 *****************************************************************************/
unsigned short crc16_ccitt(unsigned char *data, unsigned int length, unsigned short crc_i)
{
    unsigned char i;
    unsigned short crc = crc_i;        // Initial value
    while(length--)
    {
        crc ^= *data++;        // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x8408;        // 0x8408 = reverse 0x1021
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-16/CCITT-FALSE   x16+x12+x5+1
 * Poly:    0x1021
 * Init:    0xFFFF
 * Refin:   False
 * Refout:  False
 * Xorout:  0x0000
 * Note:
 *****************************************************************************/
unsigned short crc16_ccitt_false(unsigned char *data, unsigned int length, unsigned short crc_i)
{
    unsigned char i;
    unsigned short crc = crc_i;        //Initial value : 0xffff
    while(length--)
    {
        crc ^= (unsigned short)(*data++) << 8; // crc ^= (uint6_t)(*data)<<8; data++;
        for (i = 0; i < 8; ++i)
        {
            if ( crc & 0x8000 )
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-16/X25/ITU          x16+x12+x5+1
 * Poly:    0x1021
 * Init:    0xFFFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0XFFFF
 * Note:
 *****************************************************************************/
unsigned short crc16_x25(unsigned char *data, unsigned int length, unsigned short crc_i)
{
    unsigned char i;
    unsigned short crc = crc_i;        // Initial value ： 0xffff
    while(length--)
    {
        crc ^= *data++;            // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0x8408;        // 0x8408 = reverse 0x1021
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;                // crc^Xorout
}

/******************************************************************************
 * Name:    CRC-16/XMODEM       x16+x12+x5+1
 * Poly:    0x1021
 * Init:    0x0000
 * Refin:   False
 * Refout:  False
 * Xorout:  0x0000
 * Alias:   CRC-16/ZMODEM,CRC-16/ACORN
 *****************************************************************************/
unsigned short crc16_xmodem(unsigned char *data, unsigned int length, unsigned short crc_i)
{
    unsigned char i;
    unsigned short crc = crc_i;            // Initial value
    while(length--)
    {
        crc ^= (unsigned short)(*data++) << 8; // crc ^= (unsigned short)(*data)<<8; data++;
        for (i = 0; i < 8; ++i)
        {
            if ( crc & 0x8000 )
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

/******************************************************************************
 * Name:    CRC-16/DNP          x16+x13+x12+x11+x10+x8+x6+x5+x2+1
 * Poly:    0x3D65
 * Init:    0x0000
 * Refin:   True
 * Refout:  True
 * Xorout:  0xFFFF
 * Use:     M-Bus,ect.
 *****************************************************************************/
unsigned short crc16_dnp(unsigned char *data, unsigned int length, unsigned short crc_i)
{
    unsigned char i;
    unsigned short crc = crc_i;            // Initial value
    while(length--)
    {
        crc ^= *data++;            // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA6BC;        // 0xA6BC = reverse 0x3D65
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;                // crc^Xorout
}

/******************************************************************************
 * Name:    CRC-32/le  x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 * Poly:    0x4C11DB7
 * Init:    0xFFFFFFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0xFFFFFFF
 * Alias:   CRC_32/ADCCP
 * Use:     WinRAR,ect.
 *****************************************************************************/
unsigned int crc32_le(unsigned char *data, unsigned int length, unsigned int crc_i)
{
    unsigned char i;
    unsigned int crc = crc_i;        // Initial value : 0xffffffff
    while(length--)
    {
        crc ^= *data++;                // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;// 0xEDB88320= reverse 0x04C11DB7
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;
}

/******************************************************************************
 * Name:    CRC-32/MPEG-2/be  x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 * Poly:    0x4C11DB7
 * Init:    0xFFFFFFF
 * Refin:   False
 * Refout:  False
 * Xorout:  0x0000000
 * Note:
 *****************************************************************************/
unsigned int crc32_mpeg_2(unsigned char *data, unsigned int length, unsigned int crc_i)
{
    unsigned char i;
    unsigned int crc = crc_i;  // Initial value : 0xFFFFFFFF
    while(length--)
    {
        crc ^= (unsigned int)(*data++) << 24;// crc ^=(unsigned int)(*data)<<24; data++;
        for (i = 0; i < 8; ++i)
        {
            if ( crc & 0x80000000 )
                crc = (crc << 1) ^ 0x04C11DB7;
            else
                crc <<= 1;
        }
    }
    return crc;
}
