

#include "ahmi_flash_driver_interface.h"

/**
  * @name       ahmi_internal_flash_SetWriteProtect
  * @brief      Flash interface: set write protect for flash with semaphore
  * @note        
  * @param      intflag: intrrupt flag,1: call the function in interrut function, 0: call the function dont not in interrut function
  * @retval     none
  * @author     Yu Chunying
  * @date       2024-02-28
  */
void ahmi_internal_flash_SetWriteProtect(uint32_t intflag)
{
	csi_hal_internal_flash_take(intflag);
	csi_hal_internal_flash_SetWriteProtect();
	csi_hal_internal_flash_give(intflag);
}

/**
  * @name       ahmi_internal_flash_ClearWriteProtect
  * @brief      Flash interface: clear write protect for flash with semaphore
  * @note        
  * @param      intflag: intrrupt flag,1: call the function in interrut function, 0: call the function dont not in interrut function
  * @retval     none
  * @author     Yu Chunying
  * @date       2024-02-28
  */
void ahmi_internal_flash_ClearWriteProtect(uint32_t intflag)
{
	csi_hal_internal_flash_take(intflag);
	csi_hal_internal_flash_ClearWriteProtect();
	csi_hal_internal_flash_give(intflag);
}

/**
  * @name       ahmi_internal_flash_read
  * @brief      Flash interface: read data from flash with semaphore
  * @note 
  * @param      addr: start address of flash 
  * @param      len: length of data
  * @param      buffer: pointer for the data buffer       
  * @param      intflag: intrrupt flag,1: call the function in interrut function, 0: call the function dont not in interrut function
  * @retval     result
  * @author     Yu Chunying
  * @date       2024-02-28
  */
uint32_t ahmi_internal_flash_read(uint32_t addr, uint32_t len, uint8_t* buffer, uint32_t intflag)
{
	uint32_t rcv = 0;
	csi_hal_internal_flash_take(intflag);
	rcv =  csi_hal_internal_flash_read(addr, len, buffer);
	csi_hal_internal_flash_give(intflag);
	return rcv;
}

/**
  * @name       ahmi_internal_flash_write
  * @brief      Flash interface: write data to flash with semaphore
  * @note 
  * @param      addr: start address of flash 
  * @param      len: length of data
  * @param      buffer: pointer for the data buffer       
  * @param      intflag: intrrupt flag,1: call the function in interrut function, 0: call the function dont not in interrut function
  * @retval     none
  * @author     Yu Chunying
  * @date       2024-02-28
  */
void ahmi_internal_flash_write(uint32_t addr, uint32_t len, uint8_t* buffer, uint32_t intflag)
{
	csi_hal_internal_flash_take(intflag);
	csi_hal_internal_flash_write(addr, len, buffer);
	csi_hal_internal_flash_give(intflag);
}

/**
  * @name       ahmi_interal_flash_sector_erase
  * @brief      Flash interface: scetor(4KB) erase flash with semaphore
  * @note 
  * @param      addr: start address of flash       
  * @param      intflag: intrrupt flag,1: call the function in interrut function, 0: call the function dont not in interrut function
  * @retval     none
  * @author     Yu Chunying
  * @date       2024-02-28
  */
void ahmi_interal_flash_sector_erase(uint32_t addr, uint32_t intflag)
{
	csi_hal_internal_flash_take(intflag);
	csi_hal_interal_flash_sector_erase(addr);
	csi_hal_internal_flash_give(intflag);
}


/**
  * @name       ahmi_interal_flash_block_erase
  * @brief      Flash interface: block(64KB) erase flash with semaphore
  * @note 
  * @param      addr: start address of flash       
  * @param      intflag: intrrupt flag,1: call the function in interrut function, 0: call the function dont not in interrut function
  * @retval     none
  * @author     Yu Chunying
  * @date       2024-02-28
  */
void ahmi_internal_flash_block_erase(uint32_t addr, uint32_t intflag)
{
	csi_hal_internal_flash_take(intflag);
	csi_hal_internal_flash_block_erase(addr);
	csi_hal_internal_flash_give(intflag);
}
