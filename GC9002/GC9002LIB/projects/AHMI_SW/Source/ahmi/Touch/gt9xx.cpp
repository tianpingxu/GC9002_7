
//#include "gc90x_gpio.h"
#include "gt9xx.h"
#include "i2c_aw2083.h"
#include "AHMITouch.h"
#include "gc90xx_hal.h"

#ifdef TOUCH_IC_GT9XX
int32_t input_x = 0;
int32_t input_y = 0;
int32_t input_w = 0;//ï¿½ï¿½ï¿½ï¿½size


//// 5ï¿½ï¿½ï¿½ï¿½GT9157ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//const uint8_t CTP_CFG_GT9157[] ={ 
//	0x00,0x20,0x03,0xE0,0x01,0x05,0x3C,0x00,0x01,0x08,
//	0x28,0x0C,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
//	0x00,0x00,0x00,0x17,0x19,0x1E,0x14,0x8B,0x2B,0x0D,
//	0x33,0x35,0x0C,0x08,0x00,0x00,0x00,0x9A,0x03,0x11,
//	0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x32,0x00,0x00,
//	0x00,0x20,0x58,0x94,0xC5,0x02,0x00,0x00,0x00,0x04,
//	0xB0,0x23,0x00,0x93,0x2B,0x00,0x7B,0x35,0x00,0x69,
//	0x41,0x00,0x5B,0x4F,0x00,0x5B,0x00,0x00,0x00,0x00,
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//	0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,
//	0x12,0x14,0x16,0x18,0x1A,0xFF,0x00,0x00,0x00,0x00,
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//	0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0F,
//	0x10,0x12,0x13,0x16,0x18,0x1C,0x1D,0x1E,0x1F,0x20,
//	0x21,0x22,0x24,0x26,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
//	0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
//	0xFF,0xFF,0xFF,0xFF,0x48,0x01
//};

//// 7ï¿½ï¿½ï¿½ï¿½GT911ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//const uint8_t CTP_CFG_GT911[] =  {
//	//Ô­ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//0x43,0x00,0x04,0x58,0x02,0x0A,0x3e,0x00,0x01,0x08,
//0x28,0x05,0x2a,0x16,0x03,0x05,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x10,0x11,0x14,0x0F,0x8C,0x2E,0x0E,
//0x4B,0x50,0xB5,0x06,0x00,0x00,0x00,0x00,0x02,0x1D,
//0x00,0x00,0x00,0x00,0x00,0x03,0x64,0x32,0x00,0x00,
//0x00,0x3C,0x78,0x94,0xC5,0x02,0x00,0x00,0x00,0x00,
//0x92,0x40,0x00,0x81,0x4A,0x00,0x73,0x55,0x00,0x68,
//0x61,0x00,0x5E,0x70,0x00,0x5E,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,
//0x12,0x14,0x16,0x18,0x1A,0x1C,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0F,
//0x10,0x12,0x13,0x14,0x16,0x18,0x1C,0x1D,0x1E,0x1F,
//0x20,0x21,0x22,0x24,0x26,0x28,0x29,0x2A,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0xF3,0x00
//};

const uint8_t CTP_CFG_GT9157[] ={ 
	0x45,0x20,0x03,0xe0,0x01,0x05,0x0e,0x00,0x01,0x0a,
	0x28,0x0f,0x2a,0x16,0x03,0x05,0x00,0x00,0x00,0x00,
	0x00,0x00,0x08,0x17,0x19,0x1c,0x14,0x87,0x29,0x0a,
	0x66,0x68,0xeb,0x04,0x00,0x00,0x02,0x00,0x02,0x11,
	0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x5a,0x8c,0x94,0xC5,0x02,0x07,0x19,0x00,0x04,
	0x93,0x5e,0x00,0x87,0x66,0x00,0x7c,0x70,0x00,0x72,
	0x7a,0x00,0x6B,0x86,0x00,0x6a,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,
	0x12,0x14,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x1d,
	0x1e,0x1f,0x20,0x21,0x22,0x24,0x26,0x28,0xff,0xff,
	0xff,0xff,0xff,0xff,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xb7,0x00
};


const uint8_t CTP_CFG_GT911[] =  {
	//Ô­ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
0x65,0x20,0x03,0xe0,0x01,0x0A,0x0e,0x20,0x01,0x08,
0x28,0x05,0x50,0x3C,0x03,0x05,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x89,0x2A,0x0B,
0x2D,0x2B,0x0F,0x0A,0x00,0x00,0x01,0xA9,0x03,0x2D,
0x00,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,
0x00,0x21,0x59,0x94,0xC5,0x02,0x07,0x00,0x00,0x04,
0x93,0x24,0x00,0x7D,0x2C,0x00,0x6B,0x36,0x00,0x5D,
0x42,0x00,0x53,0x50,0x00,0x53,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,
0x12,0x14,0x16,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x02,0x04,0x06,0x08,0x0A,0x0F,0x10,
0x12,0x16,0x18,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,
0x24,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xD5,0x01
};

const uint8_t CTP_CFG_GT9110[] =  {
	//Ô­ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
0x00,0x56,0x05,0x00,0x03,0x01,0x0e,0x00,0x01,0x08,
0x28,0x05,0x6e,0x50,0x03,0x05,0x00,0x00,0x00,0x00,    
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x95,0x35,0xff,
0x5b,0x59,0xc4,0x09,0x00,0x00,0x01,0xdc,0x02,0x10,
0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,
0x00,0x28,0x78,0x94,0x40,0x05,0x00,0x1e,0x00,0x04,   
0x87,0x40,0x00,0x83,0x4a,0x00,0x80,0x55,0x00,0x7e,   
0x61,0x00,0x7f,0x70,0x00,0x7f,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   
0x00,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x00,0x00,    
0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,   
0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,
0x1c,0x1d,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,
0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1b,0x1c,
0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,
0x27,0x28,0x29,0x2a,0x6d,0x00
};


const uint8_t CTP_CFG_GT967[] =  {
0x42,0x40,0x01,0xE0,0x01,0x05,0x06,0x00,0x01,0x08, /* 7th value changed from 0x05 to 0x06 (INT tigger mode: low power lookup) */
0x28,0x05,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x87,0x27,0x45,
0x27,0x29,0xD6,0x09,0x00,0x00,0x00,0x9A,0x03,0x1D,
0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x23,0x3C,0x94,0xC5,0x01,0x07,0x00,0x00,0x04,
0x89,0x24,0x00,0x7A,0x29,0x00,0x70,0x2D,0x00,0x65,
0x33,0x00,0x5D,0x38,0x00,0x5D,0x00,0x00,0x00,0x00,
0x78,0x50,0x35,0xCF,0xFF,0x27,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,
0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
0x00,0x00,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x24,
0x25,0x26,0x27,0x28,0x29,0x2A,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x56,0x01
};


const uint8_t CTP_CFG_GT967_4p3Inch[] =  {
0x45,0xE0,0x01,0x10,0x01,0x05,0x0E,0x00,0x01,0x08, /* 7th value changed from 0x0D to 0x0E (INT tigger mode: low power lookup) */
0x28,0x05,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x87,0x27,0x55,
0x27,0x29,0xD6,0x09,0x00,0x00,0x00,0x9A,0x03,0x1D,
0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x23,0x3C,0x94,0xC5,0x01,0x07,0x00,0x00,0x04,
0x89,0x24,0x00,0x7A,0x29,0x00,0x70,0x2D,0x00,0x65,
0x33,0x00,0x5D,0x38,0x00,0x5D,0x00,0x00,0x00,0x00,
0x78,0x50,0x35,0xCF,0xFF,0x27,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
0x08,0x09,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
0x00,0x00,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x24,
0x25,0x26,0x27,0x28,0x29,0x2A,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x61,0x01
};


//const uint8_t CTP_CFG_GT9110[] =  {
//	//Ô­ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//    0x5B,0x56,0x05,0x00,0x03,0x01,0x3D,0x00,0x01,0x0F,
//    0x1E,0x0F,0x50,0x32,0x03,0x05,0x00,0x00,0x00,0x00,
//    0x00,0x00,0x05,0x17,0x1A,0x1D,0x14,0x95,0x35,0xFF,
//    0x28,0x2A,0xD3,0x07,0x00,0x00,0x00,0xB9,0x33,0x2D,
//    0x41,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//    0x00,0x13,0x40,0x94,0xC5,0x02,0x07,0x00,0x00,0x04,
//    0xE0,0x15,0x00,0xB3,0x1B,0x00,0x8F,0x23,0x00,0x76,
//    0x2C,0x00,0x61,0x39,0x00,0x61,0x00,0x00,0x00,0x00,
//    0xF0,0x4A,0x3A,0x9F,0xFF,0x27,0x00,0x00,0x00,0x00,
//    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x19,0x00,0x00,
//    0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
//    0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,
//    0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,
//    0x1C,0x1D,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
//    0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,
//    0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1B,0x1C,
//    0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,
//    0x27,0x28,0x29,0x2A,0x4E,0x01
//};


uint8_t config[GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH]
                = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};

TOUCH_IC touchIC;								
uint8_t GTP_bCommunicateOK = 0;
				
static int8_t GTP_I2C_Test(void);
				
__attribute__((optimize(0))) static void Delay(uint32_t nCount)	 //ï¿½òµ¥µï¿½ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½
{
	for(; nCount != 0; nCount--);
}


/**
  * @brief   Ê¹ï¿½ï¿½IICï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý´ï¿½ï¿½ï¿½
  * @param
  *		@arg i2c_msg:ï¿½ï¿½ï¿½Ý´ï¿½ï¿½ï¿½á¹¹ï¿½ï¿?
  *		@arg num:ï¿½ï¿½ï¿½Ý´ï¿½ï¿½ï¿½á¹¹ï¿½ï¿½Ä¸ï¿½ï¿½ï¿½
  * @retval  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÉµÄ´ï¿½ï¿½ï¿½á¹¹ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½0xff
  */
__attribute__((optimize(0))) static int I2C_Transfer( struct i2c_msg *msgs,int num)
{
    int im = 0;
    int ret = 0;

    for (im = 0; ret == 0 && im != num; im++)
    {
        if ((msgs[im].flags&I2C_M_RD))																//ï¿½ï¿½ï¿½ï¿½flagï¿½Ð¶ï¿½ï¿½Ç¶ï¿½ï¿½ï¿½ï¿½Ý»ï¿½ï¿½ï¿½Ð´ï¿½ï¿½ï¿½ï¿½
        {
            ret = Touch_I2C_ReadBytes(msgs[im].addr, msgs[im].buf, msgs[im].len);		//IICï¿½ï¿½È¡ï¿½ï¿½ï¿½ï¿½
        }
        else
        {
            ret = Touch_I2C_WriteBytes(msgs[im].addr,  msgs[im].buf, msgs[im].len);	//IICÐ´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        }
    }

	if(ret)
		return ret;

	return im;   													//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÉµÄ´ï¿½ï¿½ï¿½á¹¹ï¿½ï¿½ï¿½ï¿½
}

/**
  * @brief   ï¿½ï¿½IICï¿½è±¸ï¿½Ð¶ï¿½È¡ï¿½ï¿½ï¿½ï¿½
  * @param
  *		@arg client_addr:ï¿½è±¸ï¿½ï¿½Ö·
  *		@arg  buf[0~1]: ï¿½ï¿½È¡ï¿½ï¿½ï¿½Ý¼Ä´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½Ö·
  *		@arg buf[2~len-1]: ï¿½æ´¢ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÝµÄ»ï¿½ï¿½ï¿½buffer
  *		@arg len:    GTP_ADDR_LENGTH + read bytes countï¿½ï¿½ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Ö·ï¿½ï¿½ï¿½ï¿½+ï¿½ï¿½È¡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö½ï¿½ï¿½ï¿½ï¿½ï¿½
  * @retval  i2c_msgsï¿½ï¿½ï¿½ï¿½á¹¹ï¿½ï¿½Ä¸ï¿½ï¿½ï¿½ï¿½ï¿½2Îªï¿½É¹ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÎªÊ§ï¿½ï¿½
  */
__attribute__((optimize(0))) static int32_t GTP_I2C_Read(uint8_t client_addr, uint8_t *buf, int32_t len)
{
    struct i2c_msg msgs[2];
    int32_t ret=-1;
    int32_t retries = 0;

    /*Ò»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÝµÄ¹ï¿½ï¿½Ì¿ï¿½ï¿½Ô·ï¿½Îªï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
     * 1. IIC  Ð´ï¿½ï¿½ Òªï¿½ï¿½È¡ï¿½Ä¼Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Ö·
     * 2. IIC  ï¿½ï¿½È¡  ï¿½ï¿½ï¿½ï¿½
     * */

    msgs[0].flags = !I2C_M_RD;					//Ð´ï¿½ï¿½
    msgs[0].addr  = client_addr;					//IICï¿½è±¸ï¿½ï¿½Ö·
    msgs[0].len   = GTP_ADDR_LENGTH;	//ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Ö·Îª2ï¿½Ö½ï¿½(ï¿½ï¿½Ð´ï¿½ï¿½ï¿½ï¿½ï¿½Ö½Úµï¿½ï¿½ï¿½ï¿½ï¿½)
    msgs[0].buf   = &buf[0];						//buf[0~1]ï¿½æ´¢ï¿½ï¿½ï¿½ï¿½Òªï¿½ï¿½È¡ï¿½Ä¼Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Ö·
    
    msgs[1].flags = I2C_M_RD;					//ï¿½ï¿½È¡
    msgs[1].addr  = client_addr;					//IICï¿½è±¸ï¿½ï¿½Ö·
    msgs[1].len   = len - GTP_ADDR_LENGTH;	//Òªï¿½ï¿½È¡ï¿½ï¿½ï¿½ï¿½ï¿½Ý³ï¿½ï¿½ï¿½
    msgs[1].buf   = &buf[GTP_ADDR_LENGTH];	//buf[GTP_ADDR_LENGTH]Ö®ï¿½ï¿½Ä»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½æ´¢ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?

    while(retries < 5)
    {
        ret = I2C_Transfer( msgs, 2);					//ï¿½ï¿½ï¿½ï¿½IICï¿½ï¿½ï¿½Ý´ï¿½ï¿½ï¿½ï¿½ï¿½Ìºï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
        if(ret == 2)break;
        retries++;
    }
    if((retries >= 5))
    {
        return -1;
    }
    return ret;
}



/**
  * @brief   ï¿½ï¿½IICï¿½è±¸Ð´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
  * @param
  *		@arg client_addr:ï¿½è±¸ï¿½ï¿½Ö·
  *		@arg  buf[0~1]: ÒªÐ´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý¼Ä´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½Ö?
  *		@arg buf[2~len-1]: ÒªÐ´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
  *		@arg len:    GTP_ADDR_LENGTH + write bytes countï¿½ï¿½ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Ö·ï¿½ï¿½ï¿½ï¿½+Ð´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö½ï¿½ï¿½ï¿½ï¿½ï¿?
  * @retval  i2c_msgsï¿½ï¿½ï¿½ï¿½á¹¹ï¿½ï¿½Ä¸ï¿½ï¿½ï¿½ï¿½ï¿½1Îªï¿½É¹ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÎªÊ§ï¿½ï¿½
  */
__attribute__((optimize(0))) static int32_t GTP_I2C_Write(uint8_t client_addr,uint8_t *buf,int32_t len)
{
    struct i2c_msg msg;
    int32_t ret = -1;
    int32_t retries = 0;

    /*Ò»ï¿½ï¿½Ð´ï¿½ï¿½ï¿½ÝµÄ¹ï¿½ï¿½ï¿½Ö»ï¿½ï¿½ÒªÒ»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
     * 1. IICï¿½ï¿½ï¿½ï¿½ Ð´ï¿½ï¿½ ï¿½ï¿½ï¿½Ý¼Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Ö·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
     * */
    msg.flags = !I2C_M_RD;			//Ð´ï¿½ï¿½
    msg.addr  = client_addr;			//ï¿½ï¿½ï¿½è±¸ï¿½ï¿½Ö·
    msg.len   = len;							//ï¿½ï¿½ï¿½ï¿½Ö±ï¿½Óµï¿½ï¿½ï¿½(ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Ö·ï¿½ï¿½ï¿½ï¿½+Ð´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö½ï¿½ï¿½ï¿?
    msg.buf   = buf;						//Ö±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð´ï¿½ë»ºï¿½ï¿½ï¿½ï¿½ï¿½Ðµï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½ï¿½ï¿½Ë¼Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Ö·)

    while(retries < 5)
    {
        ret = I2C_Transfer(&msg, 1);	//ï¿½ï¿½ï¿½ï¿½IICï¿½ï¿½ï¿½Ý´ï¿½ï¿½ï¿½ï¿½ï¿½Ìºï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
        if (ret == 1)break;
        retries++;
    }
    if((retries >= 5))
    {
    }
    return ret;
}



/**
  * @brief   Ê¹ï¿½ï¿½IICï¿½ï¿½È¡ï¿½Ù´ï¿½ï¿½ï¿½ï¿½Ý£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç·ï¿½ï¿½ï¿½ï¿½ï¿½
  * @param
  *		@arg client:ï¿½è±¸ï¿½ï¿½Ö·
  *		@arg  addr: ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Ö·
  *		@arg rxbuf: ï¿½æ´¢ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
  *		@arg len:    ï¿½ï¿½È¡ï¿½ï¿½ï¿½Ö½ï¿½ï¿½ï¿½
  * @retval
  * 	@arg GTP_FAIL
  * 	@arg GTP_SUCCESS
  */
__attribute__((optimize(0)))  int32_t GTP_I2C_Read_dbl_check(uint8_t client_addr, uint16_t addr, uint8_t *rxbuf, int len)
{
    uint8_t buf[16] = {0};
    uint8_t confirm_buf[16] = {0};
    uint8_t retry = 0;

    while (retry++ < 3)
    {
        memset(buf, 0xAA, 16);
        buf[0] = (uint8_t)(addr >> 8);
        buf[1] = (uint8_t)(addr & 0xFF);
        GTP_I2C_Read(client_addr, buf, len + 2);
        
        memset(confirm_buf, 0xAB, 16);
        confirm_buf[0] = (uint8_t)(addr >> 8);
        confirm_buf[1] = (uint8_t)(addr & 0xFF);
        GTP_I2C_Read(client_addr, confirm_buf, len + 2);

      
        if (!memcmp(buf, confirm_buf, len+2))
        {
            memcpy(rxbuf, confirm_buf+2, len);
            return GTP_SUCCESS;
        }
    }    
    return GTP_FAIL;
}

/**
  * @brief  ??? I/O??
  * @param  ?
  * @retval ?
  */
__attribute__((optimize(0))) static void GTP_GPIO_Config(void)
{    
    Touch_I2C_ConfI2C();
	  
	  //INT
//    PACLK_ON;
//    PA10_ALTF_CLR;
//    PA10_INPUT_MODE;
    
//    PACLK_ON;
//	//INT
//    PA10_InitStruct.GPIOCLKPRE = PRE_2;
//    PA10_InitStruct.GPIO_Pin = GPIO_Pin_10;
//    PA10_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIOA_Init(&PA10_InitStruct);
//    GPIOA_ResetBits(GPIO_Pin_10); 

	GPIO_InitTypeDef GPIO_InitStruct;
	HAL_GPIO_StructInit(&GPIO_InitStruct);

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin = TOUCH_IRQ_GPIO_PIN;
	HAL_GPIO_Init(TOUCH_IRQ_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(TOUCH_IRQ_GPIO_PORT, TOUCH_IRQ_GPIO_PIN, GPIO_PIN_RESET);
    
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = TOUCH_RST_GPIO_PIN;
	HAL_GPIO_Init(TOUCH_RST_GPIO_PORT, &GPIO_InitStruct);

//	HAL_GPIO_WritePin(TOUCH_RST_GPIO_PORT, TOUCH_RST_GPIO_PIN, GPIO_PIN_RESET);
//    while(1)
//		{
//			GPIOA_ResetBits(GPIO_Pin_10); 
//			for (i=0; i<10000; i++);
//			GPIOA_SetBits(GPIO_Pin_10); 
//			for (i=0; i<10000; i++);
//		}
		
    //rst
//    PA2_InitStruct.GPIOCLKPRE = PRE_2;
//    PA2_InitStruct.GPIO_Pin = GPIO_Pin_2;
//    PA2_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIOA_Init(&PA2_InitStruct);
//	GPIO_InitStruct.Pin = GPIO_PIN_2;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief  ?GT91xx??????
  * @param  ?
  * @retval ?
  */
__attribute__((optimize(0))) void GTP_ResetChip(void)
{
    Delay(I2C_DELAY_1_MS);
	HAL_GPIO_WritePin(TOUCH_RST_GPIO_PORT, TOUCH_RST_GPIO_PIN, GPIO_PIN_RESET); 
    Delay(I2C_DELAY_1_MS);
    
	HAL_GPIO_WritePin(TOUCH_RST_GPIO_PORT, TOUCH_RST_GPIO_PIN, GPIO_PIN_SET); 
    Delay(5 * I2C_DELAY_1_MS);
}



/**
  * @brief  I2C ??(GT91xx)???
  * @param  ?
  * @retval ?
  */
__attribute__((optimize(0))) void GTP_Touch_Init(void)
{
    GTP_GPIO_Config(); 
   
    GTP_ResetChip();


	GPIO_InitTypeDef GPIO_InitStruct;
	HAL_GPIO_StructInit(&GPIO_InitStruct);
	
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pin = TOUCH_IRQ_GPIO_PIN;
	HAL_GPIO_Init(TOUCH_IRQ_GPIO_PORT, &GPIO_InitStruct);
    


    Touch_I2C_Stop();
}


/**
  * @brief   ï¿½ï¿½ï¿½Ú´ï¿½ï¿½ï¿½ò±¨¸æ´¥ï¿½ï¿½ï¿½ï¿½âµ½ï¿½ï¿½ï¿½ï¿½
  * @param
  *    @arg     id: ï¿½ï¿½ï¿½ï¿½Ë³ï¿½ï¿½trackID
  *    @arg     x:  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ x ï¿½ï¿½ï¿½ï¿½
  *    @arg     y:  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ y ï¿½ï¿½ï¿½ï¿½
  *    @arg     w:  ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ð¡
  * @retval ï¿½ï¿½
  */
/*ï¿½ï¿½ï¿½Ú¼ï¿½Â¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê±(ï¿½ï¿½ï¿½ï¿½)ï¿½ï¿½ï¿½ï¿½Ò»ï¿½Î´ï¿½ï¿½ï¿½Î»ï¿½Ã£ï¿½ï¿½ï¿½ï¿½ï¿½Öµï¿½ï¿½Ê¾ï¿½ï¿½Ò»ï¿½ï¿½ï¿½Þ´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½*/
//static int16_t pre_x[GTP_MAX_TOUCH] ={-1,-1,-1,-1,-1};
//static int16_t pre_y[GTP_MAX_TOUCH] ={-1,-1,-1,-1,-1};

//static void GTP_Touch_Down(int32_t id,int32_t x,int32_t y,int32_t w)
//{
//  
////	GTP_DEBUG_FUNC();

////	/*È¡xï¿½ï¿½yï¿½ï¿½Ê¼Öµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä»ï¿½ï¿½ï¿½ï¿½Öµ*/
////    GTP_DEBUG("ID:%d, X:%d, Y:%d, W:%d", id, x, y, w);

////	
////    /* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Å¥ï¿½ï¿½ï¿½ï¿½ï¿½Ú´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
////    Touch_Button_Down(x,y); 
////	

////    /*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì¼£ï¿½ï¿½ï¿½ï¿½ï¿½Ú´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
////    Draw_Trail(pre_x[id],pre_y[id],x,y,&brush);
////	
////		/************************************/
////		/*ï¿½Ú´Ë´ï¿½ï¿½ï¿½ï¿½ï¿½Ô¼ï¿½ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ã°´ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì¼ï¿½ï¿½ï¿½*/
////		/* (x,y) ï¿½ï¿½Îªï¿½ï¿½ï¿½ÂµÄ´ï¿½ï¿½ï¿½ï¿½ï¿½ *************/
////		/************************************/
////	
////		/*prex,preyï¿½ï¿½ï¿½ï¿½æ´¢ï¿½ï¿½Ò»ï¿½Î´ï¿½ï¿½ï¿½ï¿½ï¿½Î»ï¿½Ã£ï¿½idÎªï¿½ì¼£ï¿½ï¿½ï¿?ï¿½ï¿½ã´¥ï¿½ï¿½Ê±ï¿½Ð¶ï¿½ì¼£)*/
////    pre_x[id] = x; pre_y[id] =y;
//	
//}


/**
  * @brief   ï¿½ï¿½ï¿½Ú´ï¿½ï¿½ï¿½ò±¨¸æ´¥ï¿½ï¿½ï¿½Í·ï¿?
  * @param ï¿½Í·Åµï¿½ï¿½idï¿½ï¿½
  * @retval ï¿½ï¿½
  */
//static void GTP_Touch_Up( int32_t id)
//{
////	

////    /*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í·ï¿½,ï¿½ï¿½ï¿½Ú´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½*/
////    Touch_Button_Up(pre_x[id],pre_y[id]);

////		/*****************************************/
////		/*ï¿½Ú´Ë´ï¿½ï¿½ï¿½ï¿½ï¿½Ô¼ï¿½ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í·ï¿½Ê±ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Ì¼ï¿½ï¿½ï¿½*/
////		/* pre_x[id],pre_y[id] ï¿½ï¿½Îªï¿½ï¿½ï¿½Âµï¿½ï¿½Í·Åµï¿½ ****/
////		/*******************************************/	
////		/***idÎªï¿½ì¼£ï¿½ï¿½ï¿?ï¿½ï¿½ã´¥ï¿½ï¿½Ê±ï¿½Ð¶ï¿½ì¼£)********/
////	
////	
////    /*ï¿½ï¿½ï¿½ï¿½ï¿½Í·Å£ï¿½ï¿½ï¿½pre xy ï¿½ï¿½ï¿½ï¿½Îªï¿½ï¿½*/
////	  pre_x[id] = -1;
////	  pre_y[id] = -1;		
////  
////    GTP_DEBUG("Touch id[%2d] release!", id);

//}


/**
  * @brief   ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ñ¯ï¿½ï¿½ï¿½ï¿½ï¿½Ú´ï¿½ï¿½ï¿½ï¿½Ð¶Ïµï¿½ï¿½ï¿½
  * @param ï¿½ï¿½
  * @retval ï¿½ï¿½
  */
#if 0
uint8_t Goodix_TS_Work_Func(void)
{
		uint8_t  point_data[2 + 1 + 8 * GTP_MAX_TOUCH + 1]={GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
    uint8_t  finger = 0;
    uint8_t  end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
    uint8_t  touch_num = 0;
    static uint16_t pre_touch = 0;
    static uint8_t pre_id[GTP_MAX_TOUCH] = {0};

    uint8_t client_addr=GTP_ADDRESS;
    uint8_t* coor_data = NULL;
    uint8_t id = 0;
 
    int32_t i  = 0;
    int32_t ret = -1;

    ret = GTP_I2C_Read(client_addr, point_data, 12);//10ï¿½Ö½Ú¼Ä´ï¿½ï¿½ï¿½ï¿½ï¿½2ï¿½Ö½Úµï¿½Ö·
    if (ret < 0)
    {
				input_x = 0;
				input_y = 0;
        return 0;
    }
    
    finger = point_data[GTP_ADDR_LENGTH];//×´Ì¬ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

    if (finger == 0x00)		//Ã»ï¿½ï¿½ï¿½ï¿½ï¿½Ý£ï¿½ï¿½Ë³ï¿½
    {
       return 0;
    }

    if((finger & 0x80) == 0)//ï¿½Ð¶ï¿½buffer statusÎ»
    {
         ret = GTP_I2C_Write(client_addr, end_cmd, 3);
				 if (ret < 0)
				 {
							
				 }
				 return 0;//ï¿½ï¿½ï¿½ï¿½Î´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð§
    }

    touch_num = finger & 0x0f;//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
    if (touch_num > GTP_MAX_TOUCH)
    {
         ret = GTP_I2C_Write(client_addr, end_cmd, 3);//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö§ï¿½Öµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ë³ï¿?
				 if (ret < 0)
				 {
							
				 }
				 return 0;//ï¿½ï¿½ï¿½ï¿½Î´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð§			
    }

    if (touch_num > 1)//ï¿½ï¿½Ö¹Ò»ï¿½ï¿½ï¿½ï¿½
    {
        uint8_t buf[8 * GTP_MAX_TOUCH] = {(GTP_READ_COOR_ADDR + 10) >> 8, (GTP_READ_COOR_ADDR + 10) & 0xff};

        ret = GTP_I2C_Read(client_addr, buf, 2 + 8 * (touch_num - 1));
        memcpy(&point_data[12], &buf[2], 8 * (touch_num - 1));			//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ýµï¿½point_data
    }
    
    if (pre_touch>touch_num)				//pre_touch>touch_num,ï¿½ï¿½Ê¾ï¿½ÐµÄµï¿½ï¿½Í·ï¿½ï¿½ï¿½
    {
        for (i = 0; i < pre_touch; i++)						//Ò»ï¿½ï¿½ï¿½ï¿½Ò»ï¿½ï¿½ï¿½ã´¦ï¿½ï¿½
         {
            uint8_t j;
           for(j=0; j<touch_num; j++)
           {
               coor_data = &point_data[j * 8 + 3];
               id = coor_data[0] & 0x0F;									//track id
              if(pre_id[i] == id)
                break;

              if(j >= touch_num-1)											//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç°ï¿½ï¿½ï¿½ï¿½idï¿½ï¿½ï¿½Ò²ï¿½ï¿½ï¿½pre_id[i]ï¿½ï¿½ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½Í·ï¿½
              {
                 GTP_Touch_Up( pre_id[i]);
              }
           }
       }
    }

    if (touch_num)
    {
        for (i = 0; i < touch_num; i++)						//Ò»ï¿½ï¿½ï¿½ï¿½Ò»ï¿½ï¿½ï¿½ã´¦ï¿½ï¿½
        {
            coor_data = &point_data[i * 8 + 3];

            id = coor_data[0] & 0x0F;									//track id
            pre_id[i] = id;

            input_x  = coor_data[1] | (coor_data[2] << 8);	//xï¿½ï¿½ï¿½ï¿½
            input_y  = coor_data[3] | (coor_data[4] << 8);	//yï¿½ï¿½ï¿½ï¿½
            input_w  = coor_data[5] | (coor_data[6] << 8);	//size
//            {
//                GTP_Touch_Down( id, input_x, input_y, input_w);//ï¿½ï¿½ï¿½Ý´ï¿½ï¿½ï¿½
//            }
        }
    }
//    else if (pre_touch)		//touch_ num=0 ï¿½ï¿½pre_touchï¿½ï¿½=0
//    {
//      for(i=0;i<pre_touch;i++)
//      {
//          GTP_Touch_Up(pre_id[i]);
//      }
//    }


//    pre_touch = touch_num;
		 ret = GTP_I2C_Write(client_addr, end_cmd, 3);//ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾Î?
		 if (ret < 0)
		 {
					
		 }
		 return 1;
}
#else//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
__attribute__((optimize(0))) uint8_t Goodix_TS_Work_Func(void)
{
	uint8_t  point_data[2 + 1 + 8 * GTP_MAX_TOUCH + 1]={GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
    uint8_t  finger = 0;
    uint8_t  end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
    uint8_t  touch_num = 0;
    //static uint16_t pre_touch = 0;
    //static uint8_t pre_id[GTP_MAX_TOUCH] = {0};

    uint8_t client_addr=GTP_ADDRESS;
    uint8_t* coor_data = NULL;
    //uint8_t id = 0;
 
    int32_t i  = 0;
    int32_t ret = -1;
    int retry = 0;

    while((finger == 0) && (retry < 20))
    {
        ret = GTP_I2C_Read(client_addr, point_data, 12);//10ï¿½Ö½Ú¼Ä´ï¿½ï¿½ï¿½ï¿½ï¿½2ï¿½Ö½Úµï¿½Ö·
        if (ret < 0)
        {
            return 0;
        }

        finger = point_data[GTP_ADDR_LENGTH];//×´Ì¬ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        retry++;
    }

    if((finger & 0x80) == 0)//ï¿½Ð¶ï¿½buffer statusÎ»
    {
        return 0;//ï¿½ï¿½ï¿½ï¿½Î´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð§
    }
		
	ret = GTP_I2C_Write(client_addr, end_cmd, 3);//ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾Î?
	if (ret < 0)
	{
			
	}
    
	touch_num = 1;
    if (touch_num)
    {
        for (i = 0; i < touch_num; i++)						//Ò»ï¿½ï¿½ï¿½ï¿½Ò»ï¿½ï¿½ï¿½ã´¦ï¿½ï¿½
        {
            coor_data = &point_data[i * 8 + 3];

            //id = coor_data[0] & 0x0F;									//track id
            //pre_id[i] = id;

            input_x  = coor_data[1] | (coor_data[2] << 8);	//xï¿½ï¿½ï¿½ï¿½
            input_y  = coor_data[3] | (coor_data[4] << 8);	//yï¿½ï¿½ï¿½ï¿½
            input_w  = coor_data[5] | (coor_data[6] << 8);	//size
        }
    }

    return 1;
}

GT9xxTouch_teRtpMainSM mGT9xxTouch_teRtpMainSM = GT9xxTouch_GetTouchState;
extern TouchFSM g_TM_stTouchFsm;
uint8_t GT9xxTouch_vMain(uint32_t *px, uint32_t *py, uint32_t *state)
{
    static uint32_t err_count=0;

    uint8_t touch_status[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
    uint8_t point_data[2 + 1 + 8 * GTP_MAX_TOUCH + 1] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
    uint8_t end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
    uint8_t touch_valid = 0;
    uint8_t touch_finger_num = 0;
    uint8_t i = 0;
    uint8_t *pPointData = 0;

    uint8_t ret = 0;

    if(GTP_bCommunicateOK == 1)
    {
        if (GTP_I2C_Read(GTP_ADDRESS, touch_status, 3))
        {
            touch_valid = touch_status[GTP_ADDR_LENGTH] & 0x80;
            touch_finger_num = touch_status[GTP_ADDR_LENGTH] & 0x0F;

            if(touch_valid &&  touch_finger_num)
            {
                if (GTP_I2C_Read(GTP_ADDRESS, point_data, sizeof(point_data)))
                {
                    pPointData = &point_data[3];
                    for (i=0; i<GTP_MAX_TOUCH; i++)/*now only get the first point data,GTP_MAX_TOUCH==1*/
                    {
                        *px = pPointData[8*i+1] | (pPointData[8*i+2] << 8);
                        *py = pPointData[8*i+3] | (pPointData[8*i+4] << 8);
                    }

                    *state = TOUCHED;
                }
                else
                {
                    err_count++;
                }
            }
            else
            {
                *px = 0;
                *py = 0;
                *state = NOTOUCH;
            }

            if(touch_valid)
            {
                if(GTP_I2C_Write(GTP_ADDRESS, end_cmd, 3) < 0)
                {
                    err_count++;
                }
            }

            ret = 1;
        }
        else
        {
            err_count++;
        }
    }

    return ret;
}

uint32_t GTP_GetTouchState(void)
{
//	return GPIO_ReadInputDataBit(GPIO_PORT_AW2083_PENIRQN, AW2083_PENIRQN_PIN);
    
    return HAL_GPIO_ReadPin(TOUCH_IRQ_GPIO_PORT, TOUCH_IRQ_GPIO_PIN);//GPIOA_ReadInputDataBit(GPIO_Pin_10);
}

void GTP_ReadPosition(Position* CurPosition)
{
    uint32_t ret = 0;
    
    if(GTP_GetTouchState() == 0)
//    if(1)
    {
    
        ret = Goodix_TS_Work_Func();    //Get position
        if(ret == 1)
        {
            CurPosition->pos_x = GetXCoordinate();
            CurPosition->pos_y = GetYCoordinate();
            CurPosition->event = TOUCHED;
        }
        else
        {
            CurPosition->pos_x = GetXCoordinate();
            CurPosition->pos_y = GetYCoordinate();
            CurPosition->event = TOUCHED;
        }
    }
    else
    {
        CurPosition->pos_x = 0;
        CurPosition->pos_y = 0;
        CurPosition->event = NOTOUCH;
    }
    
}

//extern uint32_t AHMITick;

//Position GTxPosition = {0, 0, 0};
//uint32_t GTxOldAHMITick = 0;

//void GTP_ReadPosition(Position* CurPosition)
//{
//    uint32_t ret = 0;
//    
//    if(AHMITick < GTxOldAHMITick)
//    {
//        GTxOldAHMITick = AHMITick;
//    }
//    
//    if(AHMITick >= (GTxOldAHMITick + 10))
//    {
//    
//        ret = Goodix_TS_Work_Func();    //Get position
//        if(ret == 1)
//        {
//            GTxPosition.pos_x = GetXCoordinate();
//            GTxPosition.pos_y = GetYCoordinate();
//            GTxPosition.event = TOUCHED;
//            
//            GTxOldAHMITick = AHMITick;
//        }
//        else
//        {
//            GTxPosition.pos_x = 0;
//            GTxPosition.pos_y = 0;
//            GTxPosition.event = NOTOUCH;
//        }
//    }

//    CurPosition->pos_x = GTxPosition.pos_x;
//    CurPosition->pos_y = GTxPosition.pos_y;
//    CurPosition->event = GTxPosition.event;

//    
//}
#endif

/**
  * @brief   ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð¾Æ¬ï¿½ï¿½ï¿½Â¸ï¿½Î»
  * @param ï¿½ï¿½
  * @retval ï¿½ï¿½
  */
 int8_t GTP_Reset_Guitar(void)
{
#if 1
    GTP_ResetChip();
    return 0;
#else 		//ï¿½ï¿½ï¿½ï¿½ï¿½Î?
    int8_t ret = -1;
    int8_t retry = 0;
    uint8_t reset_command[3]={(uint8_t)GTP_REG_COMMAND>>8,(uint8_t)GTP_REG_COMMAND&0xFF,2};

    //Ð´ï¿½ë¸´Î»ï¿½ï¿½ï¿½ï¿½
    while(retry++ < 5)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, reset_command, 3);
        if (ret > 0)
        {
            return ret;
        }

    }
    return ret;
#endif

}

//static int32_t GTP_Get_Info(void)
//{
//    uint8_t opr_buf[6] = {0};
//    int32_t ret = 0;

////    uint16_t abs_x_max = GTP_MAX_WIDTH;
////    uint16_t abs_y_max = GTP_MAX_HEIGHT;
////    uint8_t int_trigger_type = GTP_INT_TRIGGER;
//        
//    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+1) >> 8);
//    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+1) & 0xFF);
//    
//    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 6);
//    if (ret < 0)
//    {
//        return GTP_FAIL;
//    }
//    
////    abs_x_max = (opr_buf[3] << 8) + opr_buf[2];
////    abs_y_max = (opr_buf[5] << 8) + opr_buf[4];
//    
//    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+6) >> 8);
//    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+6) & 0xFF);
//    
//    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 3);
//    if (ret < 0)
//    {
//        return GTP_FAIL;
//    }
////    int_trigger_type = opr_buf[2] & 0x03;
//        
//    return GTP_SUCCESS;    
//}

__attribute__((optimize(0))) int32_t General_I2C_Read(uint8_t client_addr, uint8_t *buf, int32_t len)
{
    struct i2c_msg msgs[2];
    int32_t ret=-1;
    int32_t retries = 0;
	uint8_t addrLength = 1;

    msgs[0].flags = !I2C_M_RD;					
    msgs[0].addr  = client_addr;					
    msgs[0].len   = addrLength;	
    msgs[0].buf   = &buf[0];						
    
    msgs[1].flags = I2C_M_RD;					
    msgs[1].addr  = client_addr;					
    msgs[1].len   = len - addrLength;	
    msgs[1].buf   = &buf[addrLength];	

    while(retries < 5)
    {
        ret = I2C_Transfer( msgs, 2);					
        if(ret == 2)break;
        retries++;
    }
    if((retries >= 5))
    {
        return -1;
    }
    return ret;
}

__attribute__((optimize(0))) int32_t General_I2C_Write(uint8_t client_addr,uint8_t *buf,int32_t len)
{
    struct i2c_msg msg;
    int32_t ret = -1;
    int32_t retries = 0;

    msg.flags = !I2C_M_RD;			
    msg.addr  = client_addr;			
    msg.len   = len;							
    msg.buf   = buf;						

    while(retries < 5)
    {
        ret = I2C_Transfer(&msg, 1);	
        if (ret == 1)break;
        retries++;
    }
    if((retries >= 5))
    {
    }
    return ret;
}



__attribute__((optimize(0))) void GM7150Init(void)
{
	uint8_t temp[4] = {0};

    temp[0] = 0;/*addr*/
	temp[1] = 0;/*data*/
	General_I2C_Write(0xB8,temp,2);

    temp[0] = 0x03;/*addr*/
	temp[1] = 0x09;/*data*/
	General_I2C_Write(0xB8,temp,2);

}

__attribute__((optimize(0))) void GM7150ReadStatus(void)
{
	uint8_t temp[4] = {0};
	
	temp[0] = 0x88;	
	Touch_I2C_WriteBytes(0xB8,  temp, 1);
	Touch_I2C_ReadBytes(0xB8,temp, 1);
}


/*******************************************************
Function:
    Initialize gtp.
Input:
    ts: goodix private data
Output:
    Executive outcomes.
        0: succeed, otherwise: failed
*******************************************************/
__attribute__((optimize(0))) int32_t GTP_Init_Panel(void)
{
    int32_t ret = -1;

    int32_t i = 0;
    uint8_t check_sum = 0;
    int32_t retry = 0;

    uint8_t* cfg_info;
    uint8_t cfg_info_len  ;

    uint8_t cfg_num =184;//0x80FE-0x8047+1 ;		//ï¿½ï¿½Òªï¿½ï¿½ï¿½ÃµÄ¼Ä´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    bool bNeedWriteConf = FALSE;


    GTP_Touch_Init();
    
//	Delay(0xfffff);	//ï¿½ï¿½Òªï¿½Ó¸ï¿½ï¿½ï¿½Ê±ï¿½È´ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½ï¿?
	
    ret = GTP_I2C_Test();
    if (ret < 0)
    {
		return ret;
    } 
	else
	{
		GTP_bCommunicateOK = 1;
	}
	
	//ï¿½ï¿½È¡ï¿½ï¿½ï¿½ï¿½ICï¿½ï¿½ï¿½Íºï¿½
    GTP_Read_Version(); 
		
    //ï¿½ï¿½ï¿½ï¿½ICï¿½ï¿½ï¿½Íºï¿½Ö¸ï¿½ï¿½Í¬ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    if(touchIC == GT9157)
    {
        cfg_info =  (uint8_t*)CTP_CFG_GT9157; //Ö¸ï¿½ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
        cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT9157);//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ã±ï¿½Ä´ï¿½Ð?
    }
    else if(touchIC == GT9110)
    {
        cfg_info =  (uint8_t*)CTP_CFG_GT9110;//Ö¸ï¿½ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
        cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT9110) ;//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ã±ï¿½Ä´ï¿½Ð?
    }
    else if(touchIC == GT911)
    {
        cfg_info =  (uint8_t*)CTP_CFG_GT911;//Ö¸ï¿½ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
        cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT911) ;//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ã±ï¿½Ä´ï¿½Ð?
    }    
    else if(touchIC == GT967)
    {
        uint8_t confVer;
        GTP_Read_ConfigVersionAndResolution(&confVer, NULL, NULL);
        if(0x42 == confVer)
        {
            cfg_info = (uint8_t*)CTP_CFG_GT967;
            
        }
        else if(0x45 == confVer)
        {
            cfg_info = (uint8_t*)CTP_CFG_GT967_4p3Inch;
        }
        else
        {
            cfg_info = (uint8_t*)CTP_CFG_GT967;
        }
        
//        cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT967);
//        cfg_num = cfg_info_len - 2;
//        bNeedWriteConf = TRUE;
    }  
    
    for (i=0; i<GTP_CONFIG_MAX_LENGTH; i++)
    {
        config[GTP_ADDR_LENGTH+i]						 = 0;
    }
    for (i=0; i<cfg_info_len; i++)
    {
        config[GTP_ADDR_LENGTH+i]						 = cfg_info[i];
    }
		
    //ï¿½ï¿½ï¿½ï¿½ÒªÐ´ï¿½ï¿½checksumï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Öµ
    check_sum = 0;
    for (i = GTP_ADDR_LENGTH; i < cfg_num+GTP_ADDR_LENGTH; i++)
    {
        check_sum += config[i];
    }
    config[ cfg_num+GTP_ADDR_LENGTH] = (~check_sum) + 1; 	//checksum
    config[ cfg_num+GTP_ADDR_LENGTH+1] =  1; 						//refresh ï¿½ï¿½ï¿½Ã¸ï¿½ï¿½Â±ï¿½Ö¾

    if(bNeedWriteConf)
    {
        Delay(50 * I2C_DELAY_1_MS);
        for (retry = 0; retry < 5; retry++)
        {
            ret = GTP_I2C_Write(GTP_ADDRESS, config , cfg_num + GTP_ADDR_LENGTH+2);
            if (ret > 0)
            {
                break;
            }
        }
        //Delay(0xfffff);	
    }
	
#if 0	//ï¿½ï¿½ï¿½ï¿½Ð´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý£ï¿½ï¿½ï¿½ï¿½ï¿½Ç·ï¿½ï¿½ï¿½ï¿½ï¿½Ð´ï¿½ï¿½
    //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð´ï¿½ï¿½ï¿½ï¿½Ç·ï¿½ï¿½ï¿½Í¬
	{
        uint16_t i;
        uint8_t buf[240];
        buf[0] = config[0];
        buf[1] =config[1];    //ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Ö·
        
        ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));

        //ï¿½æ±¾ï¿½ï¿½Ð´ï¿½ï¿½0x00ï¿½ó£¬»ï¿½ï¿½ï¿½Ð¸ï¿½Î»ï¿½ï¿½ï¿½ï¿½Î»Î?x41
        config[GTP_ADDR_LENGTH] = 0x41;

        for(i=0;i<cfg_num+GTP_ADDR_LENGTH;i++)
        {
        
            if(config[i] != buf[i])
            {
                //GTP_ERROR("Config fail ! i = %d ",i);
                return -1;
            }
        }
    	if(i==cfg_num+GTP_ADDR_LENGTH)
			{
			}
	    	//GTP_DEBUG("Config success ! i = %d ",i);
	}
#endif
	
    return 0;
}


int32_t GTP_Read_ConfigVersionAndResolution(uint8_t* pConfigVer, uint16_t* pXMax, uint16_t* pYMax)
{
    uint8_t retry = 0;
    int8_t ret = -1;
    uint8_t test[7] = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};
  
    while(retry++ < 5)
    {
        ret = GTP_I2C_Read(GTP_ADDRESS, test, 7);
        if (ret > 0)
        {
            if(pConfigVer)
            {
                *pConfigVer = test[2];
            }
            if(pXMax)
            {
                *pXMax = (uint16_t)(test[3] | (test[4] << 8));
            }
            if(pYMax)
            {
                *pXMax = (uint16_t)(test[5] | (test[6] << 8));
            }
            return ret;
        }
    }
    
    return ret;
}


/*******************************************************
Function:
    Read chip version.
Input:
    client:  i2c device
    version: buffer to keep ic firmware version
Output:
    read operation return.
        2: succeed, otherwise: failed
*******************************************************/
__attribute__((optimize(0))) int32_t GTP_Read_Version(void)
{
    int32_t ret = -1;
    uint8_t buf[8] = {GTP_REG_VERSION >> 8, GTP_REG_VERSION & 0xff};    //ï¿½Ä´ï¿½ï¿½ï¿½ï¿½ï¿½Ö·

    ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));
    if (ret < 0)
    {
        return ret;
    }

    if (buf[5] == 0x00)
    {				
		//GT911Ð¾Æ¬
		if(buf[2] == '9' && buf[3] == '1' && buf[4] == '1')
            touchIC = GT911;
        //GT967
        else if(buf[2] == '9' && buf[3] == '6' && buf[4] == '7')
        {
            touchIC = GT967;
        }
    }
    else if(buf[2] == '9' && buf[3] == '1' && buf[4] == '1' && buf[5] == '0')
    {
        touchIC = GT9110;
    }
    //GT9157Ð¾Æ¬
    else if(buf[2] == '9' && buf[3] == '1' && buf[4] == '5' && buf[5] == '7')
    {
		touchIC = GT9157;
    }
    else
    {
        touchIC = GT911;
    }
    return ret;
}

/*******************************************************
Function:
    I2c test Function.
Input:
    client:i2c client.
Output:
    Executive outcomes.
        2: succeed, otherwise failed.
*******************************************************/
__attribute__((optimize(0))) static int8_t GTP_I2C_Test( void)
{
    uint8_t retry = 0;
    int8_t ret = -1;
    uint8_t test[3] = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};
  
    while(retry++ < 5)
    {
        ret = GTP_I2C_Read(GTP_ADDRESS, test, 3);
        if (ret > 0)
        {
            return ret;
        }
    }
    
    return ret;
}

int32_t  GetXCoordinate(void)
{
    return input_x;
}

int32_t  GetYCoordinate(void)
{
    return input_y;
}

//ï¿½ï¿½âµ½ï¿½ï¿½ï¿½ï¿½ï¿½Ð¶ï¿½Ê±ï¿½ï¿½ï¿½Ã£ï¿?
void GTP_TouchProcess(void)
{
    Goodix_TS_Work_Func();
}

void GetPosition()
{
    Goodix_TS_Work_Func();
}
#endif
