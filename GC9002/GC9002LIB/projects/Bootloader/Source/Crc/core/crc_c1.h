/**
  ******************************************************************************
  * @file    crc_c1.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    21-06-2021
  * @brief   This file contains all the functions prototypes for the ShareMemory 
  *          firmware library.
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRC_C1_H
#define __CRC_C1_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/



void CRC_vInit(void);
unsigned int CRC_u32Crc32(unsigned int crc,unsigned char *buffer, unsigned int size);
unsigned int CRC_u32XorCheck(unsigned int *datain, unsigned int *data, unsigned int datalen);



	 

#ifdef __cplusplus
}
#endif

#endif /* __CRC_C1_H */




