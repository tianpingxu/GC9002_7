#ifndef REPRO_H
#define REPRO_H
#include "DiagPlatform.h"
typedef enum
{
	NO_ERROR_CODE,
	FLASH_WRITE_FAILED,
	FLASH_CLEAR_FAILED,
}FlashStatus;

typedef enum
{
    REPRO_HAS_NOT_STARTED,
    REPRO_REQUEST_DOWNLOAD_DRIVER,
    REPRO_TRANSFERDATA_DRIVER,
    REPRO_REQUEST_TRANSFER_EXIT_DRIVER,
    REPRO_CHECK_MEMORY_DRIVER_CRC,
    REPRO_WRITE_FINGER_PRINT,
    REPRO_START_ERASE_MEMORY,
    REPRO_REQUEST_EXIT_ONE_PACKET,
    REPRO_CHECK_APP_CRC,
    REPRO_GET_EXIT,
}REPROStatus;


uint8 ReproDataToFlash( uint8 *t_Data, uint32 length, uint32 t_SrcAddress);

uint8 CheckAppExist( void );
void SetReproStartAddressAndSize( uint32 t_StartAddress, uint32 t_Size );
void ChangeSize(uint32 length);
void ChangeStartAddress(uint32 length);
void SetAppNotExist(void);
void SetAppExist(void);
uint32 GetDownloadStartAddress( void );
uint32 GetDownloadSize( void );
void SetReroStage(uint8 stage);
uint8 GetReproStage(void);
void CheckFromApp( void );

#endif
