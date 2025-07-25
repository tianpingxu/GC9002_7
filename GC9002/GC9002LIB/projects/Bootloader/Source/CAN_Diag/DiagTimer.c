#include <stdio.h>
#include <stdint.h>
#include "DiagPlatform.h"
#include "DiagTimer.h"
#include "MidTimer.h"
#include "SID_10.h"
#include "SID_27.h"
#include "SID_28.h"
#include "Security.h"
#include "string.h"
#include "upgrade_c1.h"

extern st_Diag_RX_BufType *spt_Diag_Rx_Buf;
extern uint8_t ugv1_Session_status;


/* Interrupt Flag */
static uint8_t Diag_Timer_flag[NUM_TIMER] = {0};

static bool Diag_Timer_bInit;

/* When received 10 02, change MainState from 0 to 0xAA */
static uint8_t Diag_u8MainState = 0x00;

/* Timer initialization parameter, and Timer used parameter when running */
AHMITimer Diag_AHMITIM[NUM_TIMER] = 
{
    /*     mStopValue                   mCurValue       mStatus            mMode*/
    {    500,                               0,            TIMDISNABLE,    0},
    {    1000,                              0,            TIMDISNABLE,    0},
    {    1,                                 0,            TIMDISNABLE,    0},
    {    CAN_TIMER_P2CAN_SERVER_LEN,        0,            TIMDISNABLE,    0},
    {    1,                                 0,            TIMDISNABLE,    0},
    {    1,                                 0,            TIMDISNABLE,    0},
    {    200,                               0,            TIMDISNABLE,    0},
    {    1000,                              0,            TIMDISNABLE,    0},         /* used for sending MainState to customer cyclically(1s) */
};


/* Timer ISR function pointer */
const AHMITimerCB Diag_AHMITIMCB[NUM_TIMER] = 
{
    Diag_TIMER_S3_SERVER_Handler,
    Diag_TIMER_27_HEX_Handler,
    Diag_TIMER_STmin_Handler,
    Diag_TIMER_P2CAN_SERVER_Handler,
    DisplayTask_TIMER_Handler,
    Diag_TIMER_Voltage_Handler,
    Diag_AHMITIMCB6,
    Diag_AHMITIMCB7
};


/* Set Timer stop value */
void set_Diag_Timer_mStopValue( uint8_t number, uint32 stopvalue )
{
    ENTER_CRITICAL;
    Diag_AHMITIM[number].mStopValue = stopvalue;
    EXIT_CRITICAL;
}


/* Get flag of if Timer had happened */
uint8_t get_Diag_Timer_flag( uint8_t number )
{
    return Diag_Timer_flag[number];
}

/* Clear happened flag of Timer */
void clear_Diag_Timer_flag( uint8_t number )
{
    ENTER_CRITICAL;
    Diag_Timer_flag[number] = 0;
    EXIT_CRITICAL;
}


/* Update or start Timer */
void Diag_Timer_update( uint8_t number )
{
    ENTER_CRITICAL;
    Diag_AHMITIM[number].mStatus = TIMENABLE;
    Diag_AHMITIM[number].mCurValue = 0;
    EXIT_CRITICAL;
}


/* Disable Timer */
void Diag_Timer_disable( uint8_t number )
{
    ENTER_CRITICAL;
    Diag_AHMITIM[number].mStatus = TIMDISNABLE;
    Diag_AHMITIM[number].mCurValue = 0;
    EXIT_CRITICAL;
}


/* Timer ISR, 1ms trigger once */
void Diag_Timer_vIRQHandler(void)
{
    if(Diag_Timer_bInit)
    {
        Diag_AHMITimerHandlerISR();
    }
}


void Diag_Timer_vInit(void)
{
    Diag_Timer_bInit = TRUE_1;
}

/* ISR */
void Diag_TIMER_S3_SERVER_Handler()
{
    Diag_Timer_flag[Diag_TIMER_S3_SERVER] = 1;
    ugv1_Session_status = DEFAULT_SESSION ;                         // Back to default session
    Diag_Timer_disable( Diag_TIMER_S3_SERVER );

    gf_func_SID_10_term();
    //gf_func_SID_27_term();  // by Zhu Jiahai, currently no need call this function
    gf_func_SID_28_term();

    //gf_Session_term();                                            //All Diagnosis terminate
}


/* ISR */
void Diag_TIMER_27_HEX_Handler()
{
    Diag_Timer_flag[Diag_TIMER_27_HEX] = 1;
}


/* ISR */
void Diag_TIMER_STmin_Handler()
{
    //xSemaphoreGiveFromISR(xSemaphore_multy_trans, NULL);
}


/* The time from received request to respond exceed the limit, need to send 0x78 to Diagnostic instrument */
void Diag_TIMER_P2CAN_SERVER_Handler()
{
    CanFrameType TxMessage;
    /* set the timeout value to 5000ms after sending respond, set back to 50ms when respond */
    set_Diag_Timer_mStopValue( Diag_TIMER_P2CAN_SERVER, CAN_TIMER_P2CAN_SERVER_LEN );

    (void)memset(TxMessage.data, 0, 8);
    TxMessage.Id = BCM_DIAG_CAN_ID;
    TxMessage.data[0] = 0x03;
    TxMessage.data[1] = 0x7F;
    TxMessage.data[2] = spt_Diag_Rx_Buf->Rx_data[0];
    TxMessage.data[3] = 0x78;
    CAN_SendDiagMessageIsr(TxMessage.Id, TxMessage.data, 8, CAN_BUS1);
}


/* ISR */
void DisplayTask_TIMER_Handler()
{
    ;
}


/* ISR */
void Diag_TIMER_Voltage_Handler()
{
    ;
}


/* ISR */
void Diag_AHMITIMCB6()
{
    CanFrameType TxMessage;
    (void)memset(TxMessage.data, 0, 8);
    TxMessage.Id = BCM_DIAG_CAN_ID;
    TxMessage.data[0] = 0x03;
    TxMessage.data[1] = 0x7F;
    TxMessage.data[2] = 0x10;
    TxMessage.data[3] = 0x21;
    CAN_SendDiagMessageIsr(TxMessage.Id, TxMessage.data, 8, CAN_BUS1);
}

/* Interface for changing MainState value */
void Diag_vChangeMainState(uint8_t value)
{
    Diag_u8MainState = value;
}


/* ISR */
void Diag_AHMITIMCB7()
{
    CanFrameType TxMessage;
    /* Set back to 1s when respond */
    set_Diag_Timer_mStopValue( Diag_AHMITIM7, 1000);

    (void)memset(TxMessage.data, 0, 8);
    TxMessage.Id = APP_CAN_ID;
    TxMessage.data[0] = Diag_u8MainState;

    CAN_SendDiagMessageIsr(TxMessage.Id, TxMessage.data, 8, CAN_BUS1);
}


/* ISR handle enterance */
void Diag_AHMITimerHandlerISR()
{
    uint8_t i;
    
    for(i = 0; i < NUM_TIMER; i++)
    {
        if(Diag_AHMITIM[i].mStatus == TIMENABLE)
        {
            if(Diag_AHMITIM[i].mCurValue < Diag_AHMITIM[i].mStopValue - 1)
            {
                Diag_AHMITIM[i].mCurValue++;
            }
            else
            {
                Diag_AHMITIM[i].mCurValue = 0;
                Diag_AHMITIMCB[i]();
            }
        }
    }
}

