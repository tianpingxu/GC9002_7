#include "stdlib.h"
#include "gc90x_touch.h"
#include "publicInclude.h"
#include "AHMIInterface.h"
#include "gc90xx_hal.h"
#include "adcshare_c1.h"
#include "cfg_c1.h"


#define DELAY_TIME 2000

uint32_t ADC_DelayTimer = DELAY_TIME;


void get_rtpX_value(int x[],int times);
void get_rtpY_value(int x[],int times);
void get_rtpZ_value(int *z1,int *z2,int times);
void gc90x_touch_delay(int i);



/*********************Add************************************/
extern ADC_HandleTypeDef hAdc;
uint32_t channel = 0;

void GCADCTouch_vGpioSetAdc(GPIO_TypeDef * GPIO, uint32_t PIN)
{
	
    /* */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.SchmidtTrigger = GPIO_SCHMIDT_TRIGGER_DISABLE;
    GPIO_InitStruct.DriverStrength = GPIO_DRIVER_STRENGTH_0;
    GPIO_InitStruct.Alternate = GPIO_AF0;//general IO
    HAL_GPIO_Init(GPIO, &GPIO_InitStruct);

//    memset(&hAdc, 0, sizeof(ADC_HandleTypeDef));
//    hAdc.Instance = ADC;
//    HAL_ADC_StructInit(&hAdc);

    switch(PIN)
    {
        case GPIO_PIN_14:
            channel = ADC_CHANNEL_5;
            break;

        case GPIO_PIN_15:
            channel = ADC_CHANNEL_4;
            break;
		
		case GPIO_PIN_12:
            channel = ADC_CHANNEL_7;
            break;

        case GPIO_PIN_13:
            channel = ADC_CHANNEL_6;
            break;
			
		case GPIO_PIN_16:
            channel = ADC_CHANNEL_3;
            break;

        case GPIO_PIN_17:
            channel = ADC_CHANNEL_2;
            break;
			
		case GPIO_PIN_18:
            channel = ADC_CHANNEL_1;
            break;

        case GPIO_PIN_19:
            channel = ADC_CHANNEL_0;
            break;

        default:
            break;
    }
	
//	ADCEnableChannel(channel);
//	ADCStartChannel(channel);
//    HAL_ADC_Init(&hAdc);
}

void GCADCTouch_vGpioSetOutput(GPIO_TypeDef * GPIO, uint32_t PIN, GPIO_PinState value)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    //HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Pin = PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.SchmidtTrigger = GPIO_SCHMIDT_TRIGGER_DISABLE;
    GPIO_InitStruct.DriverStrength = GPIO_DRIVER_STRENGTH_0;
    GPIO_InitStruct.Alternate = GPIO_AF0;//general IO
    
    HAL_GPIO_Init(GPIO, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIO, PIN, value);
    
}


void GCADCTouch_vGpioSetInput(GPIO_TypeDef * GPIO, uint32_t PIN)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    //HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Pin = PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.SchmidtTrigger = GPIO_SCHMIDT_TRIGGER_DISABLE;
    GPIO_InitStruct.DriverStrength = GPIO_DRIVER_STRENGTH_0;
    GPIO_InitStruct.Alternate = GPIO_AF0;//general IO
    
    HAL_GPIO_Init(GPIO, &GPIO_InitStruct);    
}

uint8_t GCADCTouch_u8AdcGetValue(uint16_t *buf, uint8_t size)
{
	ADCSHR_u32PrioGetChannelData(channel, buf, size);
//	ADCStopChannel(channel);
//	ADCDisableChannel(channel);
//	HAL_ADC_GetValue(&hAdc, buf, size);
	return 1;
//    uint8_t tryTime = 5;
//    uint8_t res;
//    while(tryTime > 0)
//    {
//        res = HAL_ADC_GetValue(&hAdc, buf, size);
//        if(res == size)
//        {
//            return res;
//        }
//        else
//        {
//            tryTime--;
//        }
//    }
//    return res;
}


void GCADCTouch_vAdcStart(void)
{
//    HAL_ADC_Start(&hAdc);
//	__HAL_ADC_WAIT_DONE(&hAdc);
	
//	ADCStartChannel(channel);
}


void GCADCTouch_vTest(void)
{
    u16 value;
    int valueX[12];
    int valueY[12];
    int valueZ;
    uint8_t ret;
    uint32_t i = 0;

    
    
    while(1)
    {
        
        /* get Z */
        GCADCTouch_vGpioSetOutput(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
        GCADCTouch_vGpioSetOutput(GPIOB, GPIO_PIN_16, GPIO_PIN_RESET);
        GCADCTouch_vGpioSetAdc(GPIOB, GPIO_PIN_15);
        GCADCTouch_vGpioSetInput(GPIOB, GPIO_PIN_9);
        gc90x_touch_delay(ADC_DelayTimer);
        GCADCTouch_vAdcStart();
        ret = GCADCTouch_u8AdcGetValue(&value, 1);
        if(ret != 1)
        {
            i++;
        }
        else
        {
            i = 0;
        }

        get_rtpX_value(valueX, 12);
        get_rtpY_value(valueY, 12);
        get_rtpZ_value(&valueZ, 0, 12);
        
    }

}


#define XPLUS_GPIO_PIN     					g_touchX_PlusPin
#define YPLUS_GPIO_PIN     					g_touchY_PlusPin
#define XMINUS_GPIO_PIN     				g_touchX_SubPin //GPIO_PIN_16 For gc9002_deb20230210_v1_1_0
#define YMINUS_GPIO_PIN     				g_touchY_SubPin //GPIO_PIN_9 For gc9002_deb20230210_v1_1_0

#define XPLUS_GPIO_PORT     			    g_touchX_PlusPort
#define YPLUS_GPIO_PORT     				g_touchY_PlusPort
#define XMINUS_GPIO_PORT     				g_touchX_SubPort
#define YMINUS_GPIO_PORT     				g_touchY_SubPort


/* X+ */
#define XPLUS_INPUT_MODE                    GCADCTouch_vGpioSetAdc(XPLUS_GPIO_PORT, XPLUS_GPIO_PIN)           
#define XPLUS_OUTPUT_MODE     
#define XPLUS_OUTPUT_1                      GCADCTouch_vGpioSetOutput(XPLUS_GPIO_PORT, XPLUS_GPIO_PIN, GPIO_PIN_SET)
#define XPLUS_OUTPUT_0                      GCADCTouch_vGpioSetOutput(XPLUS_GPIO_PORT, XPLUS_GPIO_PIN, GPIO_PIN_RESET)
/* Y+ */
#define YPLUS_INPUT_MODE                    GCADCTouch_vGpioSetAdc(YPLUS_GPIO_PORT, YPLUS_GPIO_PIN)           
#define YPLUS_OUTPUT_MODE     
#define YPLUS_OUTPUT_1                      GCADCTouch_vGpioSetOutput(YPLUS_GPIO_PORT, YPLUS_GPIO_PIN, GPIO_PIN_SET)
#define YPLUS_OUTPUT_0                      GCADCTouch_vGpioSetOutput(YPLUS_GPIO_PORT, YPLUS_GPIO_PIN, GPIO_PIN_RESET)
/* X- */
#define XMINUS_INPUT_MODE                   GCADCTouch_vGpioSetInput(XMINUS_GPIO_PORT, XMINUS_GPIO_PIN)           
#define XMINUS_OUTPUT_MODE     
#define XMINUS_OUTPUT_1                     GCADCTouch_vGpioSetOutput(XMINUS_GPIO_PORT, XMINUS_GPIO_PIN, GPIO_PIN_SET)
#define XMINUS_OUTPUT_0                     GCADCTouch_vGpioSetOutput(XMINUS_GPIO_PORT, XMINUS_GPIO_PIN, GPIO_PIN_RESET)
/* Y- */
#define YMINUS_INPUT_MODE                   GCADCTouch_vGpioSetInput(YMINUS_GPIO_PORT, YMINUS_GPIO_PIN)           
#define YMINUS_OUTPUT_MODE     
#define YMINUS_OUTPUT_1                     GCADCTouch_vGpioSetOutput(YMINUS_GPIO_PORT, YMINUS_GPIO_PIN, GPIO_PIN_SET)
#define YMINUS_OUTPUT_0                     GCADCTouch_vGpioSetOutput(YMINUS_GPIO_PORT, YMINUS_GPIO_PIN, GPIO_PIN_RESET)
/*********************End************************************/


/*******************************************************************************
* Function Name  : gc90x_touch_delay
* Description    : Delay for ADC sample time.
* Input          : i, number of cycles
* Output         : None
* Return         : None
*******************************************************************************/
void gc90x_touch_delay(int i)
{
    while(i > 0)
    {
        i--;
    }
}


/*
       PB9  Y-
       PB16 X-
ADC3   PB15  X+
ADC4   PB14  Y+
*/
/*******************************************************************************
* Function Name  : get_rtpX_value
* Description    : read x channel data.
* Input          : x, pointer to data buffer
*                : times,sample times
* Output         : None
* Return         : None
*******************************************************************************/
void get_rtpX_value(int x[],int times)
{
	//ADC_InitTypeDef ADC_InitStruct;
	uint16_t adcValue[12];
	int i;
	
    // SET X+  VCC
    XPLUS_OUTPUT_MODE; 
	XPLUS_OUTPUT_1;    
    // SET X-  GND
    XMINUS_OUTPUT_MODE;
	XMINUS_OUTPUT_0;  
    // SET Y+  INPUT
    YPLUS_INPUT_MODE;
    // SET Y-  INPUT
	YMINUS_INPUT_MODE;
    // read Y+
    #if 0
	ADC_InitStruct.APBCLKPRE = APBtoADC_PRE_8;
	ADC_InitStruct.ADC_ACQT = ADC_ACQT_2ClockCycles;
	ADC_InitStruct.ADC_CHS = ADC_CHS_4;
	ADC_Init(GC_ADC, &ADC_InitStruct);
    #endif
    gc90x_touch_delay(ADC_DelayTimer);
	
//	GCADCTouch_vAdcStart();
//	ADCChannelWaitDone(channel);
    /* times must <= 12 */
    ADCSHR_u32PrioGetChannelData(channel, adcValue, times);
	for(i=0;i<times;i++)
	{
		x[i] = adcValue[i];
	}
//	ADCStopChannel(channel);
//	ADCDisableChannel(channel);
	
    XPLUS_OUTPUT_0;
}


/*******************************************************************************
* Function Name  : get_rtpY_value
* Description    : read y channel data.
* Input          : y, pointer to data buffer
*                : times,sample times
* Output         : None
* Return         : None
*******************************************************************************/
void get_rtpY_value(int y[],int times)
{
	//ADC_InitTypeDef ADC_InitStruct;
	uint16_t adcValue[12];
	int i;
	
    // SET Y+  VCC
    YPLUS_OUTPUT_MODE; 
	YPLUS_OUTPUT_1;    
    // SET Y-  GND
    YMINUS_OUTPUT_MODE;
	YMINUS_OUTPUT_0;  
    // SET X+  INPUT
    XPLUS_INPUT_MODE;
    // SET X-  INPUT
	XMINUS_INPUT_MODE;
    // read X+
    #if 0
	ADC_InitStruct.APBCLKPRE = APBtoADC_PRE_8;
	ADC_InitStruct.ADC_ACQT = ADC_ACQT_2ClockCycles;
	ADC_InitStruct.ADC_CHS = ADC_CHS_3;
	ADC_Init(GC_ADC, &ADC_InitStruct);
	#endif
	gc90x_touch_delay(ADC_DelayTimer);
	
//	GCADCTouch_vAdcStart();
//	ADCChannelWaitDone(channel);
	/* times must <= 12 */
    ADCSHR_u32PrioGetChannelData(channel, adcValue, times);
	for(i=0;i<times;i++)
	{
		y[i] = adcValue[i];
	}
//	ADCStopChannel(channel);
//	ADCDisableChannel(channel);
    
	YPLUS_OUTPUT_0;
}

/*******************************************************************************
* Function Name  : get_rtpZ_value
* Description    : read z1 channel data.
* Input          : z1, pointer to data buffer
*                : times,sample times
* Output         : None
* Return         : None
*******************************************************************************/
void get_rtpZ_value(int *z1,int *z2,int times)
{
    //ADC_InitTypeDef ADC_InitStruct;
    uint16_t adcValue[12];
    int i;
    uint32_t sum;
    
    // SET Y+  VCC
    YPLUS_OUTPUT_MODE; 
	YPLUS_OUTPUT_1;    
    // SET X-  GND
    XMINUS_OUTPUT_MODE;
	XMINUS_OUTPUT_0;  
    // SET X+  INPUT
    XPLUS_INPUT_MODE;
    // SET Y-  INPUT
	YMINUS_INPUT_MODE;
    // read X+
    #if 0
	ADC_InitStruct.APBCLKPRE = APBtoADC_PRE_8;
	ADC_InitStruct.ADC_ACQT = ADC_ACQT_2ClockCycles;
	ADC_InitStruct.ADC_CHS = ADC_CHS_3;
	ADC_Init(GC_ADC, &ADC_InitStruct);
    #endif
	gc90x_touch_delay(ADC_DelayTimer);
	
	sum = 0;

//	GCADCTouch_vAdcStart();
//	ADCChannelWaitDone(channel);
    /* times must <= 12 */
    ADCSHR_u32PrioGetChannelData(channel, adcValue, times);
	for(i=0; i< times; i++)
	{
		sum += adcValue[i];
	}
//	ADCStopChannel(channel);
//	ADCDisableChannel(channel);
	*z1 = sum/times;
    
    YPLUS_OUTPUT_0;
// read Y-	
//	ADC_InitStruct.APBCLKPRE = APBtoADC_PRE_8;
//	ADC_InitStruct.ADC_ACQT = ADC_ACQT_2ClockCycles;
//	ADC_InitStruct.ADC_CHS = ADC_CHS_4;
//	ADC_Init(GC_ADC, &ADC_InitStruct);
//	
//	*z2=0;
//	for(i=0;i<times;i++)
//	{
//		ADC_StartCAP(GC_ADC);
//		*z2+= ADC_GetADCRES(GC_ADC);
//	}
//	*z2 = *z2/times;
}

/*******************************************************************************
* Function Name  : GC90xTouch_Sort
* Description    : sort data in buffer.
* Input          : queue, pointer to data buffer.
*                : length,buffer length.
* Output         : None
* Return         : None
*******************************************************************************/
void GC90xTouch_Sort(int* queue, int length)
{
	int tmp;
	int i,j;
	
	for(i=0;i<length-1;i++)
		for(j=0;j<length-1-i;j++)
			if(queue[j]>queue[j+1])
			{
				tmp = queue[j];
				queue[j] = queue[j+1];
				queue[j+1] = tmp;
			}
}

/*******************************************************************************
* Function Name  : Touch_Test
* Description    : Touch function test.
* Input          : x_o, pointer to x channel data.
*                : y_o, pointer to y channel data.
*                : touch, pointer to the touch state.
* Output         : None
* Return         : None
*******************************************************************************/
void Touch_Test(int *x_o,int *y_o,int *touch)
{
	int x[12];
	int y[12];
	int z1;
	int z2;
	int x_avg,y_avg;
	int x_var,y_var;
	int i;
	
	
	get_rtpZ_value(&z1,&z2,4);
	
	if(z1>10)
	{	
		get_rtpX_value(x,12);
		get_rtpY_value(y,12);
		
		GC90xTouch_Sort(x,12);
		GC90xTouch_Sort(y,12);
		
		x_avg = y_avg=0;
		
		for(i=2;i<10;i++)
		{
			x_avg+=x[i];
			y_avg+=y[i];
		}
		x_avg/=8;
		y_avg/=8;
		
		x_var = y_var = 0;
		
		for(i=2;i<10;i++)
		{
			x_var+=abs(x_avg-x[i]);
			y_var+=abs(y_avg-y[i]);
		}
		
		get_rtpZ_value(&z1,&z2,4);
		
		if(z1 > GCADC_Z_TOUTH_THREHOLD && x_var < GCADC_X_TOUTH_THREHOLD && y_var < GCADC_Y_TOUTH_THREHOLD)
		{
			*x_o = x_avg;
			*y_o = y_avg;
			*touch = 1;
		}
		else
		{
			*x_o = 0;
			*y_o = 0;
			*touch = 0;
		}
	}
	else
	{
			*x_o = 0;
			*y_o = 0;
			*touch = 0;
	}
}

/*******************************************************************************
* Function Name  : GC90xTouch_GetTouchState
* Description    : Get touch state.
* Input          : None
* Output         : Touch or Notouch
* Return         : None
*******************************************************************************/
uint32_t GC90xTouch_GetTouchState(void)
{
	int z1;
	int z2;
	
	get_rtpZ_value(&z1,&z2,4);
	
	if(z1 > GCADC_Z_TOUTH_THREHOLD)
		return 0;
	else
		return 1;
}

/*******************************************************************************
* Function Name  : GC90xTouch_ReadPosition
* Description    : Read touch information interface.
* Input          : pPosition, pointer to touch information.
* Output         : None
* Return         : None
*******************************************************************************/
void GC90xTouch_ReadPosition(Position* pPosition)
{
	int x[12];
	int y[12];
	int z1;
	int z2;
	int x_avg,y_avg;
	int x_var,y_var;
	int i;
    int x_o;
    int y_o;
    int touch;
	
	if(GC90xTouch_GetTouchState() == 0)
	{	
		get_rtpX_value(x,12);
		get_rtpY_value(y,12);
		
		GC90xTouch_Sort(x,12);
		GC90xTouch_Sort(y,12);
		
		x_avg = y_avg=0;
		
		for(i=4;i<8;i++)
		{
			x_avg+=x[i];
			y_avg+=y[i];
		}
		x_avg/=4;
		y_avg/=4;
		
		x_var = y_var = 0;
		
		for(i=4;i<8;i++)
		{
			x_var+=abs(x_avg-x[i]);
			y_var+=abs(y_avg-y[i]);
		}
		
		get_rtpZ_value(&z1,&z2,4);
		
		if(z1 > GCADC_Z_TOUTH_THREHOLD && x_var < GCADC_X_TOUTH_THREHOLD && y_var < GCADC_Y_TOUTH_THREHOLD)
		{
			x_o = x_avg;
			y_o = y_avg;
			touch = TOUCHED;
		}
		else
		{
//			x_o = 0;
//			y_o = 0;
//			touch = NOTOUCH;
            x_o = pPosition->pos_x;
			y_o = pPosition->pos_y;
			touch = TOUCHED;
		}
	}
	else
	{
			x_o = 0;
			y_o = 0;
			touch = NOTOUCH;
	}
    
    pPosition->pos_x = x_o;
    pPosition->pos_y = y_o;
    pPosition->event = touch;
}

GCADCTouch_tsWorkbuf ADCTouch_tsWorkbuf;
extern TouchFSM g_TM_stTouchFsm;

void Touch_Sort(int* queue, int length)
{
	int tmp;
	int i,j;
	
	for(i=0;i<length-1;i++)
    {
		for(j=0;j<length-1-i;j++)
        {
			if(queue[j]>queue[j+1])
			{
				tmp = queue[j];
				queue[j] = queue[j+1];
				queue[j+1] = tmp;
			}
        }
     }
}

void GCADCTouch_vRtpGPIOInit(GCADCTouch_tsWorkbuf *ptsWorkbuf)
{
    //ADC_InitTypeDef ADC_InitStruct;
    
    switch (ptsWorkbuf->teReadDirect)
    {
        case GCADCTouch_enZ:
			// SET Y+  VCC
			YPLUS_OUTPUT_MODE; 
			YPLUS_OUTPUT_1;    
			// SET X-  GND
			XMINUS_OUTPUT_MODE;
			XMINUS_OUTPUT_0;  
			// SET X+  INPUT
			XPLUS_INPUT_MODE;
			// SET Y-  INPUT
			YMINUS_INPUT_MODE;
			// read X+
			#if 0
			ADC_InitStruct.APBCLKPRE = APBtoADC_PRE_8;
			ADC_InitStruct.ADC_ACQT = ADC_ACQT_2ClockCycles;
			ADC_InitStruct.ADC_CHS = ADC_CHS_3;
			ADC_Init(GC_ADC, &ADC_InitStruct); 
            #endif
            break;
        case GCADCTouch_enX:
			// SET X+  VCC
			XPLUS_OUTPUT_MODE; 
			XPLUS_OUTPUT_1;    
			// SET X-  GND
			XMINUS_OUTPUT_MODE;
			XMINUS_OUTPUT_0;  
			// SET Y+  INPUT
			YPLUS_INPUT_MODE;
			// SET Y-  INPUT
			YMINUS_INPUT_MODE;
			// read Y+
			#if 0
			ADC_InitStruct.APBCLKPRE = APBtoADC_PRE_8;
			ADC_InitStruct.ADC_ACQT = ADC_ACQT_2ClockCycles;
			ADC_InitStruct.ADC_CHS = ADC_CHS_4;
			ADC_Init(GC_ADC, &ADC_InitStruct);
            #endif
            break;
        case GCADCTouch_enY:
			// SET Y+  VCC
			YPLUS_OUTPUT_MODE; 
			YPLUS_OUTPUT_1;    
			// SET Y-  GND
			YMINUS_OUTPUT_MODE;
			YMINUS_OUTPUT_0;  
			// SET X+  INPUT
			XPLUS_INPUT_MODE;
			// SET X-  INPUT
			XMINUS_INPUT_MODE;
			// read X+
			#if 0
			ADC_InitStruct.APBCLKPRE = APBtoADC_PRE_8;
			ADC_InitStruct.ADC_ACQT = ADC_ACQT_2ClockCycles;
			ADC_InitStruct.ADC_CHS = ADC_CHS_3;
			ADC_Init(GC_ADC, &ADC_InitStruct);
            #endif
            break;
    }
}

void GCADCTouch_vRtpGPIOReset(GCADCTouch_tsWorkbuf *ptsWorkbuf)
{
    switch (ptsWorkbuf->teReadDirect)
    {
        case GCADCTouch_enZ:           
            YPLUS_OUTPUT_0; 
            break;
        case GCADCTouch_enX:
            XPLUS_OUTPUT_0;
            break;
        case GCADCTouch_enY:
            YPLUS_OUTPUT_0;
            break;  
        default:
            break;
    }
}

void GCADCTouch_vRtpGetValue(GCADCTouch_tsWorkbuf *ptsWorkbuf, int *pBuf, uint8_t times)
{
    uint32_t i=0;
    uint16_t adcValue[12];

     for (i=0; i<GCADCTOUCH_VALUE_BUFFER_LENGTH; i++)
    {
		*(pBuf+i) = 0;
    } 

//	GCADCTouch_vAdcStart();

    /* times must <= 12 */
    ADCSHR_u32PrioGetChannelData(channel, adcValue, times);
	for(i=0; i<times; i++)
	{

       *(pBuf+i) =  (int)adcValue[i];
	}

    
    GCADCTouch_vRtpGPIOReset(ptsWorkbuf);
    
    switch (ptsWorkbuf->teReadDirect)
    {
        case GCADCTouch_enZ:           
            ptsWorkbuf->int32ZValue = 0; 
            for (i=0; i<times; i++)
            {
                ptsWorkbuf->int32ZValue += pBuf[i];         
            }
            ptsWorkbuf->int32ZValue /= times;
            break;
        case GCADCTouch_enX:
            Touch_Sort(pBuf,times);
            ptsWorkbuf->int32XValue = 0;
            for(i=4; i<8; i++)
            {
                ptsWorkbuf->int32XValue += ptsWorkbuf->int16X[i];
            }
            ptsWorkbuf->int32XValue /= 4;
            
            ptsWorkbuf->int32tempXValue = 0;
            for(i=4;i<8;i++)
            {
                ptsWorkbuf->int32tempXValue += abs(ptsWorkbuf->int32XValue-ptsWorkbuf->int16X[i]);
            }
            break;
        case GCADCTouch_enY:
            Touch_Sort(pBuf,times);
            ptsWorkbuf->int32YValue = 0;
            for(i=4; i<8; i++)
            {
                ptsWorkbuf->int32YValue += ptsWorkbuf->int16Y[i];
            }
            ptsWorkbuf->int32YValue /= 4;

            ptsWorkbuf->int32tempYValue = 0;
            for(i=4;i<8;i++)
            {
                ptsWorkbuf->int32tempYValue += abs(ptsWorkbuf->int32YValue-ptsWorkbuf->int16Y[i]);
            }
            break;         
    }
}

extern uint32_t AHMITick;
void GCADCtouch_vMain(void)
{    
    switch(ADCTouch_tsWorkbuf.teRtpManSM)
    {
        case GCADCTouch_VarInit:
            ADCTouch_tsWorkbuf.teReadDirect = GCADCTouch_enZ;        
            GCADCTouch_vRtpGPIOInit(&ADCTouch_tsWorkbuf);   
            ADCTouch_tsWorkbuf.u32PreTick =  AHMITick;
            ADCTouch_tsWorkbuf.teRtpManSM = GCADCTouch_GPIOInitDelay;
            break;
        case GCADCTouch_GPIOInitDelay:
            if (AHMITick-ADCTouch_tsWorkbuf.u32PreTick > GCADCTOUCH_DELAY_VALUE)
            {
                if (ADCTouch_tsWorkbuf.teReadDirect == GCADCTouch_enZ)
                {
//					ADCGetChannelState(channel, &state);
//					if(state != ADCStateDataReady)
//						return;
						
                    GCADCTouch_vRtpGetValue(&ADCTouch_tsWorkbuf, ADCTouch_tsWorkbuf.int16Z, 4);
                    
                    if (ADCTouch_tsWorkbuf.u32Debounce == 1)
                    {
                        ADCTouch_tsWorkbuf.u32Debounce = 0;
                        
                        if (ADCTouch_tsWorkbuf.int32ZValue > GCADC_Z_TOUTH_THREHOLD)
                        {
                            if (ADCTouch_tsWorkbuf.int32tempXValue < GCADC_X_TOUTH_THREHOLD && ADCTouch_tsWorkbuf.int32tempYValue < GCADC_Y_TOUTH_THREHOLD)
                            {
                                g_TM_stTouchFsm.mPosi.pos_x = ADCTouch_tsWorkbuf.int32XValue;
                                g_TM_stTouchFsm.mPosi.pos_y = ADCTouch_tsWorkbuf.int32YValue;
                                g_TM_stTouchFsm.mPosi.event = TOUCHED;
                            }
                        }
                        else
                        {
							g_TM_stTouchFsm.mPosi.event = NOTOUCH;
                        }     
                        
                        ADCTouch_tsWorkbuf.teReadDirect = GCADCTouch_enZ;
                        GCADCTouch_vRtpGPIOInit(&ADCTouch_tsWorkbuf);   
                        ADCTouch_tsWorkbuf.u32PreTick =  AHMITick;
                        ADCTouch_tsWorkbuf.teRtpManSM = GCADCTouch_GPIOInitDelay;
                    }
                    else
                    {
                        if (ADCTouch_tsWorkbuf.int32ZValue > GCADC_Z_TOUTH_THREHOLD)
                        {
                            ADCTouch_tsWorkbuf.teReadDirect = GCADCTouch_enX;
                            GCADCTouch_vRtpGPIOInit(&ADCTouch_tsWorkbuf);   
                            ADCTouch_tsWorkbuf.u32PreTick =  AHMITick;
                            ADCTouch_tsWorkbuf.teRtpManSM = GCADCTouch_GPIOInitDelay;
                            
                        }
                        else
                        {
                            g_TM_stTouchFsm.mPosi.event = NOTOUCH;
                            
                            ADCTouch_tsWorkbuf.teReadDirect = GCADCTouch_enZ;
                            GCADCTouch_vRtpGPIOInit(&ADCTouch_tsWorkbuf);   
                            ADCTouch_tsWorkbuf.u32PreTick =  AHMITick;
                            ADCTouch_tsWorkbuf.teRtpManSM = GCADCTouch_GPIOInitDelay;
                        }
                    }                     
                }
                else if (ADCTouch_tsWorkbuf.teReadDirect == GCADCTouch_enX)
                {
//					ADCGetChannelState(channel, &state);
//					if(state != ADCStateDataReady)
//						return;
						
                      GCADCTouch_vRtpGetValue(&ADCTouch_tsWorkbuf, ADCTouch_tsWorkbuf.int16X, GCADCTOUCH_VALUE_BUFFER_LENGTH);
                    
                      ADCTouch_tsWorkbuf.teReadDirect = GCADCTouch_enY;
                      GCADCTouch_vRtpGPIOInit(&ADCTouch_tsWorkbuf);   
                      ADCTouch_tsWorkbuf.u32PreTick =  AHMITick;
                      ADCTouch_tsWorkbuf.teRtpManSM = GCADCTouch_GPIOInitDelay;
                }
                else if (ADCTouch_tsWorkbuf.teReadDirect == GCADCTouch_enY)
                {
//					ADCGetChannelState(channel, &state);
//					if(state != ADCStateDataReady)
//						return;
						
                    GCADCTouch_vRtpGetValue(&ADCTouch_tsWorkbuf, ADCTouch_tsWorkbuf.int16Y, GCADCTOUCH_VALUE_BUFFER_LENGTH);

                    ADCTouch_tsWorkbuf.teReadDirect = GCADCTouch_enZ;                    
                    GCADCTouch_vRtpGPIOInit(&ADCTouch_tsWorkbuf);   
                    ADCTouch_tsWorkbuf.u32PreTick =  AHMITick;
                    ADCTouch_tsWorkbuf.teRtpManSM = GCADCTouch_GPIOInitDelay;                    
                    
                    ADCTouch_tsWorkbuf.u32Debounce = 1;
                }
            }
            break;
        default:
            break;
    }
}
