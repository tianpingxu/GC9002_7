////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     modbus_task.h
// Project Name:  AHMISimulator
//
// Revision:
// Revision 2.00 - File Created 2018/08/22 by Yu Chunying
// Revision 2.00 - add the modbus tasks function
//
////////////////////////////////////////////////////////////////////////////////
#ifndef MODBUS_TASH_H
#define MODBUS_TASH_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "AHMICfgDefinition.h"
#include "AHMIBasicStructure.h"
#include "trace.h"

#ifdef __cplusplus 
extern "C" {
#endif
    
void ModBusTaskCreate(void);    
    
#ifdef __cplusplus 
}
#endif

#endif
