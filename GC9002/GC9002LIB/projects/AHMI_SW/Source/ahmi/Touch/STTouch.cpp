#include "STTouch.h"

#ifdef TOUCH_IC_ST
uint16_t x_res;
uint16_t y_res;
uint16_t max_num;
uint16_t x_coordi;
uint16_t y_coordi;
uint8_t ST_bCommunicateOK = 0;

int STTouch_get_version(void);
int STTouch_get_max_touchNum(void);
int STTouch_get_resolution(uint16_t* x_res, uint16_t* y_res);

static void Delay(uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}

static int ST__I2C_Transfer( struct ST_i2c_msg *msgs,int num)
{
    int im = 0;
    int ret = 0;

    for (im = 0; ret == 0 && im != num; im++)
    {
        if ((msgs[im].flags&ST_I2C_M_RD))                                                              //����flag�ж��Ƕ����ݻ���д����
        {
            ret = Touch_I2C_ReadBytes(msgs[im].addr, msgs[im].buf, msgs[im].len);       //IIC��ȡ����
        }
        else
        {
            ret = Touch_I2C_WriteBytes(msgs[im].addr,  msgs[im].buf, msgs[im].len); //IICд������
        }
    }

    if(ret)
        return ret;

    return im;                                                      //������ɵĴ���ṹ����
}

/**
  * @brief   CST836U__I2C_Read
  * @param
  *     @arg client_addr:i2c device addr
  *     @arg  buf[0]: reg addr
  *     @arg buf[1~len-1]: data buffer read from device
  *     @arg len:    GTP_ADDR_LENGTH + read bytes count
  * @retval  0 success 1 failed
  */
int32_t ST__I2C_Read(uint8_t client_addr, uint8_t *buf, int32_t len)
{
    struct ST_i2c_msg msgs[2];
    int32_t ret=-1;
    int32_t retries = 0;

    msgs[0].flags = !ST_I2C_M_RD;
    msgs[0].addr  = client_addr;
    msgs[0].len   = ST_ADDR_LENGTH;
    msgs[0].buf   = &buf[0];

    msgs[1].flags = ST_I2C_M_RD;
    msgs[1].addr  = client_addr;
    msgs[1].len   = len - ST_ADDR_LENGTH;
    msgs[1].buf   = &buf[ST_ADDR_LENGTH];

    while(retries < 5)
    {
        ret = ST__I2C_Transfer( msgs, 2);
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
  * @brief   CST836U__I2C_Write
  * @param
  *     @arg client_addr:i2c device addr
  *     @arg  buf[0]: reg addr
  *     @arg buf[1~len-1]: data buffer read from device
  *     @arg len:    GTP_ADDR_LENGTH + read bytes count
  * @retval  0 success 1 failed
  */
int32_t ST__I2C_Write(uint8_t client_addr,uint8_t *buf,int32_t len)
{
    struct ST_i2c_msg msg;
    int32_t ret = -1;
    int32_t retries = 0;

    msg.flags = !ST_I2C_M_RD;
    msg.addr  = client_addr;
    msg.len   = len;
    msg.buf   = buf;

    while(retries < 5)
    {
        ret = ST__I2C_Transfer(&msg, 1);
        if (ret == 1)break;
        retries++;
    }
    if((retries >= 5))
    {
    }
    return ret;
}


void STTouch_GPIO_Config(void)
{
      GPIO_InitTypeDef GPIO_InitStruct;

      Touch_I2C_ConfI2C();

      //reset
      HAL_GPIO_StructInit(&GPIO_InitStruct);

      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Pin = TOUCH_RST_GPIO_PIN;
      HAL_GPIO_Init(TOUCH_RST_GPIO_PORT, &GPIO_InitStruct);

      GPIO_InitStruct.Pull = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Pin = TOUCH_IRQ_GPIO_PIN;
      HAL_GPIO_Init(TOUCH_IRQ_GPIO_PORT, &GPIO_InitStruct);
}

void STTouch_ResetChip(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    HAL_GPIO_WritePin(TOUCH_IRQ_GPIO_PORT, TOUCH_IRQ_GPIO_PIN, GPIO_PIN_SET);
    Delay(40000*10);

    HAL_GPIO_WritePin(TOUCH_RST_GPIO_PORT, TOUCH_RST_GPIO_PIN, GPIO_PIN_SET);
    Delay(40000*10);

    HAL_GPIO_WritePin(TOUCH_RST_GPIO_PORT, TOUCH_RST_GPIO_PIN, GPIO_PIN_RESET);
    Delay(40000*10);

    HAL_GPIO_WritePin(TOUCH_RST_GPIO_PORT, TOUCH_RST_GPIO_PIN, GPIO_PIN_SET);
    Delay(40000*150);

    //int
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = TOUCH_IRQ_GPIO_PIN;
    HAL_GPIO_Init(TOUCH_IRQ_GPIO_PORT, &GPIO_InitStruct);
}

uint8_t ST_u8GetConState(void)
{
    return ST_bCommunicateOK;
}



uint8_t STTouch_vInit(void)
{
    uint8_t ret = 0;
//    static uint32_t x,y,state;

    STTouch_GPIO_Config();

    STTouch_ResetChip();

    Touch_I2C_Stop();

    if (STTouch_get_version())
    {
        STTouch_get_resolution(&x_res, &y_res);
        STTouch_get_max_touchNum();
        ST_bCommunicateOK = 1;
        ret = 1;

//        while(1)
//        {
//            ST_vGetTouchInfo(&x, &y, &state);
//            Delay(20000);
//        }
    }

    return ret;
}

uint8_t ST_vGetTouchInfo(uint32_t *px, uint32_t *py, uint32_t *state)
{
    uint8_t buf[50]={0};
    uint8_t ret = 0;
    static uint32_t err_count = 0;

    if (ST_u8GetConState() == 1)
    {
        buf[0] = XY0_COORD_H; // Set Reg. address to 0x0 for reading FW Version.

        max_num = 1;

        if (ST__I2C_Read(ST1633I_I2C_ADDR, buf, max_num*4+1))
        {
            if (buf[1]&0x80)
            {
                *px = ((buf[1]>>4)&0x07)*256 + buf[2];
                *py = (buf[1]&0x07)*256 + buf[3];
                *state = TOUCHED;
            }
            else
            {
                *px =  0;
                *py =  0;
                *state = NOTOUCH;
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

int STTouch_get_version(void)
{
    uint8_t buf[2]={0};
    int ret = 0;

    buf[0] = FIRMWARE_VERSION;

    if(ST__I2C_Read(ST1633I_I2C_ADDR, buf, 2))
    {
        ret = 1;
    }

    return ret;
}

int STTouch_get_max_touchNum(void)
{
    uint8_t buf[2]={0};
    int ret = 0;

    buf[0] = MAX_NUM_TOUCHES;

    ST__I2C_Read(ST1633I_I2C_ADDR, buf, 2);

    max_num = buf[1];
    if(max_num > 10)
    {
        max_num = 10;
    }

    return ret;
}

int STTouch_get_resolution(uint16_t* x_res, uint16_t* y_res)
{
    uint8_t buf[4]={0};
    int ret = 0;

    if (x_res==0 || y_res==0)
    {
        return -1;
    }

    buf[0] = XY_RESOLUTION_HIGH; // Set Reg. address to 0x0 for reading FW Version.

    ST__I2C_Read(ST1633I_I2C_ADDR, buf, 4);

    *x_res = ((buf[1]>>4)&0x07)*256 + buf[2];
    *y_res = (buf[1]&0x07)*256 + buf[3];

    return ret;
}

int STTouch_get_coordinate(uint16_t* x_coordi, uint16_t* y_coordi)
{
    uint8_t buf[50]={0};
    int ret = 0;

    if (x_res==0 || y_res==0 || max_num==0)
    {
        return -1;
    }



    buf[0] = XY0_COORD_H; // Set Reg. address to 0x0 for reading FW Version.

    if(ST__I2C_Read(ST1633I_I2C_ADDR, buf, max_num*4+1))
    {
        if (buf[1]&0x80)
        {
            *x_coordi = ((buf[1]>>4)&0x07)*256 + buf[2];
            *y_coordi = (buf[1]&0x07)*256 + buf[3];
        }
        else
        {
            return -2;
        }
    }

    return ret;
}

#endif