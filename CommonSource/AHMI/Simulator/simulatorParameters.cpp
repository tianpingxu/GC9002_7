////////////////////////////////////////////////////////////////////////////////
// Company:      SX
// Engineer:     AHMI Team
//
// Create Date:   2018/10/16
// File Name:     simulatorParameters.cpp
// Project Name:  AHMIGEN_V2.0
// 
// Revision:
// Revision 2.00 - File Created 2018/08/24 by Zhou Yuzhi
// Additional Comments:
//     the parameters needed for simualtor layer, only valid on PC
////////////////////////////////////////////////////////////////////////////////
#include "simulatorParameters.h"
#include "ahmiv3.h"


AHMIREGS AHMI_REGFILE;
#ifdef PC_SIM
ahmi mAHMI;
#endif
