#include "gc9002_power.h"


void ldo_calibration(void)
{
	
}

void init_ldo_and_hyper(void)
{
	HYPER_HandleTypeDef handle_hyper;
	
	ldo_calibration();
	
	if(HAL_HYPER_Init(&handle_hyper) != HAL_OK)
    {
        while (1);
    }
}