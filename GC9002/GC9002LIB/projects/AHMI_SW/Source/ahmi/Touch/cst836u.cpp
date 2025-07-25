/* Includes ------------------------------------------------------------------*/
#include "CST836u.h"
#include <stdlib.h>
#include "AHMITouch.h"

#ifdef TOUCH_IC_CST836U
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t CST836U_bCommunicateOK = 0;

static void CST836U__vGPIOInit(void);
static void CST836U__Reset(void);
static uint8_t CST836U__u8GetChipID( void );
uint8_t CST836U_vJudgeComIsNormal(void);
static uint32_t CST836U__u32GetTouchState( void );

__attribute__((optimize(0))) static void Delay(uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}

static int CST836U__I2C_Transfer( struct CST836U_i2c_msg *msgs,int num)
{
    int im = 0;
    int ret = 0;

    for (im = 0; ret == 0 && im != num; im++)
    {
        if ((msgs[im].flags&CST836U_I2C_M_RD))                                                              //����flag�ж��Ƕ����ݻ���д����
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
int32_t CST836U__I2C_Read(uint8_t client_addr, uint8_t *buf, int32_t len)
{
    struct CST836U_i2c_msg msgs[2];
    int32_t ret=-1;
    int32_t retries = 0;

    msgs[0].flags = !CST836U_I2C_M_RD;
    msgs[0].addr  = client_addr;
    msgs[0].len   = CST836U_ADDR_LENGTH;
    msgs[0].buf   = &buf[0];

    msgs[1].flags = CST836U_I2C_M_RD;
    msgs[1].addr  = client_addr;
    msgs[1].len   = len - CST836U_ADDR_LENGTH;
    msgs[1].buf   = &buf[CST836U_ADDR_LENGTH];

    while(retries < 5)
    {
        ret = CST836U__I2C_Transfer( msgs, 2);
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
int32_t CST836U__I2C_Write(uint8_t client_addr,uint8_t *buf,int32_t len)
{
    struct CST836U_i2c_msg msg;
    int32_t ret = -1;
    int32_t retries = 0;

    msg.flags = !CST836U_I2C_M_RD;
    msg.addr  = client_addr;
    msg.len   = len;
    msg.buf   = buf;

    while(retries < 5)
    {
        ret = CST836U__I2C_Transfer(&msg, 1);
        if (ret == 1)break;
        retries++;
    }
    if((retries >= 5))
    {
    }
    return ret;
}

__attribute__((optimize(0))) static void CST836U__vRegConfig( void )
{
    uint8_t buf[10] = {0};

    buf[0] = 0xF9;
    buf[1] = 0xFF;  //255s enter sleep mode
    buf[2] = 0x40;
    buf[3] = 0x05;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x01;  // disable auto enter sleep mode
    CST836U__I2C_Write(CST836U_IIC_WR_ADDR, buf, 7);
}


uint8_t CST836U_vInit(void)
{
    uint8_t ret = 0;

    CST836U__vGPIOInit();
    CST836U__Reset();
    if (CST836U_vJudgeComIsNormal())
    {
        CST836U_bCommunicateOK = 1;
		CST836U__vRegConfig();
        ret = 1;
    }

    return ret;
}

uint8_t CST836U_vJudgeComIsNormal(void)
{
    uint8_t ret = 0;

    if (CST836U__u8GetChipID())
    {
        ret = 1;
    }

    return ret;
}

uint8_t CST836U_u8GetConState(void)
{
    return CST836U_bCommunicateOK;
}


/**
  * @brief configure io
  * @param
  * @retval
  * @note
  */
static void CST836U__vGPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    Delay(10*30000);
    Touch_I2C_ConfI2C();

    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin = TOUCH_IRQ_GPIO_PIN;
    HAL_GPIO_Init(TOUCH_IRQ_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(TOUCH_IRQ_GPIO_PORT, TOUCH_IRQ_GPIO_PIN, GPIO_PIN_RESET);

#if (CST836U_RESET_PIN_SUPPORT == 1)
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = TOUCH_RST_GPIO_PIN;
    HAL_GPIO_Init(TOUCH_RST_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(TOUCH_RST_GPIO_PORT, TOUCH_RST_GPIO_PIN, GPIO_PIN_SET);
#endif
}

/**
  * @brief  reset cst816 chip by reset pin
  * @param
  * @retval
  * @note
  */
static void CST836U__Reset(void)
{
    uint32_t i;

#if (CST836U_RESET_PIN_SUPPORT == 1)
    Delay(100*30000);
    HAL_GPIO_WritePin(TOUCH_IRQ_GPIO_PORT, TOUCH_IRQ_GPIO_PIN, GPIO_PIN_RESET);
    Delay(50*30000);
    HAL_GPIO_WritePin(TOUCH_IRQ_GPIO_PORT, TOUCH_IRQ_GPIO_PIN, GPIO_PIN_SET);
    Delay(50*30000);
    HAL_GPIO_WritePin(TOUCH_RST_GPIO_PORT, TOUCH_RST_GPIO_PIN, GPIO_PIN_RESET);
    Delay(50*30000);
    HAL_GPIO_WritePin(TOUCH_RST_GPIO_PORT, TOUCH_RST_GPIO_PIN, GPIO_PIN_SET);
    Delay(50*30000);
    HAL_GPIO_WritePin(TOUCH_IRQ_GPIO_PORT, TOUCH_IRQ_GPIO_PIN, GPIO_PIN_RESET);
    Delay(50*30000);
    HAL_GPIO_WritePin(TOUCH_IRQ_GPIO_PORT, TOUCH_IRQ_GPIO_PIN, GPIO_PIN_SET);
    Delay(50*30000);
#endif

    GPIO_InitTypeDef GPIO_InitStruct;

    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin = TOUCH_IRQ_GPIO_PIN;
    HAL_GPIO_Init(TOUCH_IRQ_GPIO_PORT, &GPIO_InitStruct);
}

__attribute__((optimize(0))) static uint8_t CST836U__u8GetChipID( void )
{
    int32_t res = -1;
    uint8_t buf[3] = {CST836U_REG_CHIP_ID, 0,0};

    res = CST836U__I2C_Read(CST836U_IIC_WR_ADDR, buf, sizeof(buf));
    if (res < 0)
    {
        return 0;
    }

    return 1;
}



__attribute__((optimize(0))) uint8_t CST836U_vGetTouchInfo(uint32_t *px, uint32_t *py, uint32_t *state)
{
    uint8_t point_data[8] = {0x02};
    uint8_t touch_finger_num = 0;
    uint8_t ret = 0;

    if (CST836U_u8GetConState() == 1)
    {
        if(CST836U__I2C_Read(CST836U_IIC_WR_ADDR, point_data, 7))
        {
            touch_finger_num = point_data[1];

            if (touch_finger_num)
            {
                *px =  (point_data[2] & 0x0F)*256 + point_data[3];
                *py =  (point_data[4] & 0x0F)*256 + point_data[5];
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
    }

    return ret;
}

#endif