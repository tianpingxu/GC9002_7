////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI 
//
// Create Date:   2021/09/06
// File Name:     sm_c1.h
// 
// Revision:
// Revision 1.0.0 - File Created 2021/09/06 by Yu Ning
////////////////////////////////////////////////////////////////////////////////
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SM_C1_H
#define __SM_C1_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/

#define StoreNumberTag 0
#define StoreStringTag 1

#define MaxStoreTagNum 500
#define MaxStoreStringTagWordSize 1000

typedef struct 
{
	uint32_t                        u32RegID;					/* Store Tag's Reg ID */
	uint32_t                        u32TagValue;				/* tore Tag Value */
}SM_tstStoreItem;


void SM_vInit(void);
void SM_vClear(uint8_t TagType);
void SM_vUpgrade(uint8_t TagType);
void SM_vStoreNumberTag(uint32_t u32RegID, uint32_t u32TagValue);
void SM_vStoreStringTag(uint32_t u32RegID, uint32_t u32StringLength, uint8_t *pData);
#ifdef __cplusplus
}
#endif

#endif /* __SM_C1_H */




