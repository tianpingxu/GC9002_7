#ifndef __UPGRADE_BTLD_H
#define __UPGRADE_BTLD_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/

#include "upgrade_ct.h"
#include "upgrade_ci.h"
#include "upgrade_ce.h"



#define UPG_BTLD_MAGIC_NUM                                  (0x48494948)
#define UPG_FLAG_SELF_UPG_OP_FINISHED                       (0xAABBCCDD)



typedef struct 
{	
    uint32_t                        u32BtldSize;                                    /* Bootloader size(exclude head) */
    uint32_t                        u32Crc;                                         /* Bootloader CRC (exclude head) */
    uint32_t                        u32MagicNum;                                    /* Bootloader magic number */
	uint32_t                        u32Resv;                                        /* Reserved */
}UPG_tstBtldHeadInfo, *UPG_tstBtldHeadInfoPtr;

void UPG_BTLD_vSetUpgFlag(bool flag);
bool UPG_BTLD_vGetIfNeedUpgBootloader(void);
bool UPG_BTLD_bIfBootloaderValid(uint8_t * addr);
void UPG_BTLD_vUpgBootloader(uint8_t * addr);
bool UPG_BTLD_bIfSelfUpgOpFinished(void);
void UPG_BTLD_vSetSelfUpgOpFinished(void);



#ifdef __cplusplus
}
#endif

#endif /* __UPGRADE_BTLD_H */




