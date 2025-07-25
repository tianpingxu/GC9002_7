////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMIÐ¡×é³ÉÔ±
//
// Create Date:   2017/11/27
// File Name:     main.c
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2017/11/27 by Yu Chunying
////////////////////////////////////////////////////////////////////////////////

#ifndef SPI_SDCARD__H
#define SPI_SDCARD__H

#include "stdint.h"
//#include "stm32f10x.h"
//#include "spi_basic.h"
#include "upgrade_ci.h"
#include "gc90xx.h"

#ifdef __cplusplus 
extern "C" {
#endif

#define SPI_FLAG_TFE               						((uint16_t)0x0001)
#define SPI_FLAG_TNF               						((uint16_t)0x0002)
#define SPI_FLAG_RNE               						((uint16_t)0x0004)
#define SPI_FLAG_RFF               						((uint16_t)0x0008)

/* Exported types ------------------------------------------------------------*/
typedef enum
{
/** 
  * @brief  Standard error defines   
  */ 
  SD_INTERNAL_ERROR = (34), 
  SD_NOT_CONFIGURED,
  SD_REQUEST_PENDING, 
  SD_REQUEST_NOT_APPLICABLE, 
  SD_INVALID_PARAMETER,  
  SD_UNSUPPORTED_FEATURE,  
  SD_UNSUPPORTED_HW,  
  SD_ERROR,  
  SD_OK = 0 
} SD_Error;


/** 
  * @brief  SD Card States 
  */   
typedef enum
{
  SD_CARD_READY                  = ((uint32_t)0x00000001),
  SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
  SD_CARD_STANDBY                = ((uint32_t)0x00000003),
  SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
  SD_CARD_SENDING                = ((uint32_t)0x00000005),
  SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
  SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
  SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
  SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
}SDCardState;


/** 
  * @brief  Card Specific Data: CSD Register   
  */ 
typedef struct
{
  volatile uint8_t  CSDStruct;            /*!< CSD structure */
  volatile uint8_t  SysSpecVersion;       /*!< System specification version */
  volatile uint8_t  Reserved1;            /*!< Reserved */
  volatile uint8_t  TAAC;                 /*!< Data read access-time 1 */
  volatile uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
  volatile uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
  volatile uint16_t CardComdClasses;      /*!< Card command classes */
  volatile uint8_t  RdBlockLen;           /*!< Max. read data block length */
  volatile uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
  volatile uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
  volatile uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
  volatile uint8_t  DSRImpl;              /*!< DSR implemented */
  volatile uint8_t  Reserved2;            /*!< Reserved */
  volatile uint32_t DeviceSize;           /*!< Device Size */
  volatile uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
  volatile uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
  volatile uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
  volatile uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
  volatile uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
  volatile uint8_t  EraseGrSize;          /*!< Erase group size */
  volatile uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
  volatile uint8_t  WrProtectGrSize;      /*!< Write protect group size */
  volatile uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
  volatile uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
  volatile uint8_t  WrSpeedFact;          /*!< Write speed factor */
  volatile uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
  volatile uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
  volatile uint8_t  Reserved3;            /*!< Reserded */
  volatile uint8_t  ContentProtectAppli;  /*!< Content protection application */
  volatile uint8_t  FileFormatGrouop;     /*!< File format group */
  volatile uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
  volatile uint8_t  PermWrProtect;        /*!< Permanent write protection */
  volatile uint8_t  TempWrProtect;        /*!< Temporary write protection */
  volatile uint8_t  FileFormat;           /*!< File Format */
  volatile uint8_t  ECC;                  /*!< ECC code */
  volatile uint8_t  CSD_CRC;              /*!< CSD CRC */
  volatile uint8_t  Reserved4;            /*!< always 1*/
} SD_CSD;

/** 
  * @brief  Card Identification Data: CID Register   
  */
typedef struct
{
  volatile uint8_t  ManufacturerID;       /*!< ManufacturerID */
  volatile uint16_t OEM_AppliID;          /*!< OEM/Application ID */
  volatile uint32_t ProdName1;            /*!< Product Name part1 */
  volatile uint8_t  ProdName2;            /*!< Product Name part2*/
  volatile uint8_t  ProdRev;              /*!< Product Revision */
  volatile uint32_t ProdSN;               /*!< Product Serial Number */
  volatile uint8_t  Reserved1;            /*!< Reserved1 */
  volatile uint16_t ManufactDate;         /*!< Manufacturing Date */
  volatile uint8_t  CID_CRC;              /*!< CID CRC */
  volatile uint8_t  Reserved2;            /*!< always 1 */
} SD_CID;

/** 
  * @brief SD Card Status 
  */
typedef struct
{
  volatile uint8_t DAT_BUS_WIDTH;
  volatile uint8_t SECURED_MODE;
  volatile uint16_t SD_CARD_TYPE;
  volatile uint32_t SIZE_OF_PROTECTED_AREA;
  volatile uint8_t SPEED_CLASS;
  volatile uint8_t PERFORMANCE_MOVE;
  volatile uint8_t AU_SIZE;
  volatile uint16_t ERASE_SIZE;
  volatile uint8_t ERASE_TIMEOUT;
  volatile uint8_t ERASE_OFFSET;
} SD_CardStatus;


/** 
  * @brief SD Card information 
  */
typedef struct
{
  SD_CSD SD_csd;
  SD_CID SD_cid;
  uint32_t CardCapacity;  /*!< Card Capacity */
  uint32_t CardBlockSize; /*!< Card Block Size */
  uint16_t RCA;
  uint8_t CardType;
} SD_CardInfo;


/** 
  * @brief  SPI Intialization Frequency (400KHz max)
  */
#define SPI_INIT_CLK_DIV                SPI_BaudRatePrescaler_128
/** 
  * @brief  SPI Data Transfer Frequency (25MHz max) 
  */
/*!< SPIOCLK = HCLK, SDIO_CK = HCLK/(2 + SDIO_TRANSFER_CLK_DIV) */
#define SPI_TRANSFER_CLK_DIV            SPI_BaudRatePrescaler_2 

	  
/** 
  * @brief SDIO Commands  Index 
  */
#define SD_CMD_GO_IDLE_STATE                       ((uint8_t)0)
#define SD_CMD_SEND_OP_COND                        ((uint8_t)1)
#define SD_CMD_ALL_SEND_CID                        ((uint8_t)2)
#define SD_CMD_SET_REL_ADDR                        ((uint8_t)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                             ((uint8_t)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((uint8_t)5)
#define SD_CMD_HS_SWITCH                           ((uint8_t)6)
#define SD_CMD_SEL_DESEL_CARD                      ((uint8_t)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((uint8_t)8)
#define SD_CMD_SEND_CSD                            ((uint8_t)9)
#define SD_CMD_SEND_CID                            ((uint8_t)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((uint8_t)11) /*!< SD Card doesn't support it */
#define SD_CMD_STOP_TRANSMISSION                   ((uint8_t)12)
#define SD_CMD_SEND_STATUS                         ((uint8_t)13)
#define SD_CMD_HS_BUSTEST_READ                     ((uint8_t)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((uint8_t)15)
#define SD_CMD_SET_BLOCKLEN                        ((uint8_t)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((uint8_t)17)
#define SD_CMD_READ_MULT_BLOCK                     ((uint8_t)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((uint8_t)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((uint8_t)20) /*!< SD Card doesn't support it */
#define SD_CMD_SET_BLOCK_COUNT                     ((uint8_t)23) /*!< SD Card doesn't support it */
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((uint8_t)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((uint8_t)25)
#define SD_CMD_PROG_CID                            ((uint8_t)26) /*!< reserved for manufacturers */
#define SD_CMD_PROG_CSD                            ((uint8_t)27)
#define SD_CMD_SET_WRITE_PROT                      ((uint8_t)28)
#define SD_CMD_CLR_WRITE_PROT                      ((uint8_t)29)
#define SD_CMD_SEND_WRITE_PROT                     ((uint8_t)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((uint8_t)32) /*!< To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                    ((uint8_t)33) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                     ((uint8_t)35) /*!< To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define SD_CMD_ERASE_GRP_END                       ((uint8_t)36) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define SD_CMD_ERASE                               ((uint8_t)38)
#define SD_CMD_FAST_IO                             ((uint8_t)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((uint8_t)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((uint8_t)42)
#define SD_CMD_APP_CMD                             ((uint8_t)55)
#define SD_CMD_GEN_CMD                             ((uint8_t)56)
#define SD_CMD_NO_CMD                              ((uint8_t)64)

/** 
  * @brief Following commands are SD Card Specific commands.
  *        SDIO_APP_CMD :CMD55 should be sent before sending these commands. 
  */
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((uint8_t)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((uint8_t)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((uint8_t)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((uint8_t)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                      ((uint8_t)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                    ((uint8_t)53) /*!< For SD I/O Card only */

/** 
  * @brief Following commands are SD Card Specific security commands.
  *        SDIO_APP_CMD should be sent before sending these commands. 
  */
#define SD_CMD_SD_APP_GET_MKB                      ((uint8_t)43) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_MID                      ((uint8_t)44) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RN1                  ((uint8_t)45) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RN2                  ((uint8_t)46) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RES2                 ((uint8_t)47) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RES1                 ((uint8_t)48) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint8_t)18) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint8_t)25) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_ERASE                 ((uint8_t)38) /*!< For SD Card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((uint8_t)49) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((uint8_t)48) /*!< For SD Card only */
  
/* Uncomment the following line to select the SDIO Data transfer mode */  
//#define SD_DMA_MODE                                ((uint32_t)0x00000000)
/*#define SD_POLLING_MODE                            ((uint32_t)0x00000002)*/

/**
  * @brief  SD detection on its memory slot
  */
#define SD_PRESENT                                 ((uint8_t)0x01)
#define SD_NOT_PRESENT                             ((uint8_t)0x00)

/** 
  * @brief Supported SD Memory Cards 
  */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((uint32_t)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((uint32_t)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((uint32_t)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((uint32_t)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((uint32_t)0x00000007)


/* Exported functions ------------------------------------------------------- */
uint8_t SDCardSPI_CheckInsert(SPI_TypeDef* SPI);
void SPI_Hword_read(SPI_TypeDef* SPIx, uint8_t* pBuffer, uint16_t Length);
void ChangeSPIBaudRateForSDCard(SPI_TypeDef *SPIx);
SD_Error SDCardSPI_Init(SPI_TypeDef *SPIx);
uint8_t SDCardSPI_WaitReady(SPI_TypeDef *SPIx);
uint8_t SDCardSPI_SendCommand(SPI_TypeDef *SPIx, uint8_t cmd, uint32_t arg, uint8_t crc, uint32_t *presdata);
uint8_t SDCardSPI_GetCIDorCSD(SPI_TypeDef *SPIx, uint8_t cmd, uint32_t *presdata);
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo);
SD_Error SDCardSPI_ReceiveData(SPI_TypeDef *SPIx, uint8_t *data, uint16_t len);
SD_Error SPI_ReadSingleBlock_test(SPI_TypeDef *SPIx, uint8_t *data, uint16_t len);
SD_Error SDCardSPI_ReadSingleBlock(SPI_TypeDef *SPIx, uint32_t sectorAddress, uint8_t *buffer, uint16_t blocksize);
SD_Error SDCardSPI_ReadSingleBlockAPI(uint32_t sectorAddress, uint8_t *buffer, uint16_t blocksize);
SD_Error SDCardSPI_ReadMultiBlock(SPI_TypeDef *SPIx, uint32_t sectorAddress, uint8_t *buffer, uint16_t blocksize, uint32_t NumberOfBlocks);
SD_Error SDCardSPI_ReadMultiBlockAPI(uint32_t sectorAddress, uint8_t *buffer, uint16_t blocksize, uint32_t NumberOfBlocks);
SD_Error SDCardSPI_WriteSingleBlock(SPI_TypeDef *SPIx, uint32_t sectorAddress, const uint8_t *buffer, uint16_t blocksize);
SD_Error SDCardSPI_WriteSingleBlockAPI(uint32_t sectorAddress, const uint8_t *buffer, uint16_t blocksize);
SD_Error SDCardSPI_WriteMultiBlock(SPI_TypeDef *SPIx, uint32_t sectorAddress, const uint8_t *buffer, uint16_t blocksize, uint32_t NumberOfBlocks);
SD_Error SDCardSPI_WriteMultiBlockAPI(uint32_t sectorAddress, const uint8_t *buffer, uint16_t blocksize, uint32_t NumberOfBlocks);

#ifdef __cplusplus 
}
#endif



#endif
