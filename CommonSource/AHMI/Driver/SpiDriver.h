////////////////////////////////////////////////////////////////////////////////
// Company:      SX
// Engineer:     AHMI小组成员
//
// Create Date:   2018/10/16
// File Name:     SpiDriver.h
// Project Name:  AHMIGEN_V2.0
// 
// Revision:
// Revision 2.00 - File Created 2018/10/18 by Zhou Yuzhi
// Additional Comments:
//     the driver layer for spi. only used for 1.x
////////////////////////////////////////////////////////////////////////////////

#ifndef SPI_DRIVER__H
#define SPI_DRIVER__H

#include "publicDefine.h"


class SpiDriver
{
	/**
  * @brief  initialize all the regs of AHMI
  * @param  Mode: DISPLAY output mode select.
  *         This parameter can be: DISP_OUTPUT_PVGA, DISP_OUTPUT_PLVDS,
	*					DISP_OUTPUT_PDLVDS or DISP_OUTPUT_PSLVDS.
  * @retval AHMI_FUNC_SUCCESS or AHMI_FUNC_FAILURE
  */
	funcStatus initialAllSPIRegs();
};

#endif