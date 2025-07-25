#include "DiagPlatform.h"
#include "DiagNWL.h"
//#include "core_cm0.h"
#include "csi_irq.h"

#ifdef __cplusplus
extern "C" {
#endif
 
#define ENTER_CRITICAL                  __disable_irq()
#define EXIT_CRITICAL                   __enable_irq() 
/* 定时器个�? */    
#define NUM_TIMER 8

/* 定时器的宏定义，对应每个定时器的序号 */
typedef enum
{
	Diag_TIMER_S3_SERVER = 0,
	Diag_TIMER_27_HEX,
	Diag_TIMER_STmin,
	Diag_TIMER_P2CAN_SERVER,
	DisplayTask_TIMER,
	Diag_TIMER_Voltage,
	Diag_AHMITIM6,
	Diag_AHMITIM7
}DIAG_TIMER_ENUM;

void Diag_AHMITimerHandlerISR( void );
void Diag_Timer_update( uint8 number );
void Diag_Timer_disable( uint8 number );

void set_Diag_Timer_mStopValue( uint8 number, uint32 stopvalue );
uint8 get_Diag_Timer_flag( uint8 number );
void clear_Diag_Timer_flag( uint8 number );

void Diag_TIMER_S3_SERVER_Handler( void );
void Diag_TIMER_27_HEX_Handler( void );
void Diag_TIMER_STmin_Handler( void );
void Diag_TIMER_P2CAN_SERVER_Handler( void );
void DisplayTask_TIMER_Handler( void );
void Diag_TIMER_Voltage_Handler( void );
void Diag_AHMITIMCB6( void );
void Diag_AHMITIMCB7( void );
/* for GEELY Requirement */
void Diag_vChangeMainState(uint8 value);
void Diag_Timer_vIRQHandler(void);
void Diag_Timer_vInit(void);


#ifdef __cplusplus
}

#endif
