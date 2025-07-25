////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     Type.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by Zhou Yuzhi 
// Additional Comments:
//    the fundamental type
// 
////////////////////////////////////////////////////////////////////////////////

#include "AHMICfgDefinition.h"


#ifndef PUBLICTYPE__H
#define PUBLICTYPE__H

#ifdef CHIP_GC9005
#include "gc90x_config.h"
#endif

#if (defined(PC_SIM) || defined(PC_SIM_Gen) || defined(VERSION_2)) && (!defined(CHIP_GC9005))

#include <stdint.h>

typedef int     s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t  sc8;   /*!< Read Only */


typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const int16_t  uc16;  /*!< Read Only */
typedef const uint8_t  uc8;   /*!< Read Only */

#endif


#endif
