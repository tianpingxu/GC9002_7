
#ifndef __GC90x_AHMICLOCK_H
#define __GC90x_AHMICLOCK_H

//#include "core_cm0.h"  
//#include "CPU1_CMSDK_CM0.h"

#ifdef __cplusplus
 extern "C" {
#endif
	 
#define  AHMI_ClockIn  (48UL)
	 
#define  AHMI_CLOCK_BASE_ADDR   ((uint32_t)0x50003000)
#define  AHMI_CLOCK_REG    			((AHMI_CLOCK_REG_Type*)  AHMI_CLOCK_BASE_ADDR)
	 
typedef struct
{
__IO  uint32_t   Enable;
__IO  uint32_t   CLK_PAR; 		
}AHMI_CLOCK_REG_Type;

typedef struct
{
uint32_t	Enable;
uint32_t  VCO_MULT_FACTOR;
uint32_t  CLKIN_DIVIDE_FACTOR;	
uint32_t  OUTPUT_DIVIDE_FACTOR;	
uint32_t  CLOCK_IN;	
}CLK_PAR_Type;
	 
void AHMIClock_StructInit(CLK_PAR_Type* AHMIClock_InitStruct);
ITStatus AHMIClock_Init(CLK_PAR_Type* AHMIClock_InitStruct);

#ifdef __cplusplus
}
#endif

#endif /* __GC90x_AHMICLOCK_H */
