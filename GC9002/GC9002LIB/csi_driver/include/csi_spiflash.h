/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_spiflash.h
 * @brief    header File for SPI FLASH Driver
 * @version  V1.0
 * @date     2023/03/01
 * @model    spiflash
 ******************************************************************************/
#ifndef __CSI_SPIFLASH_H__
#define __CSI_SPIFLASH_H__

#include <csi_common.h>
#include <csi_gpio.h>
#include <csi_qspi.h>
#include <csi_spi.h>
#if(defined CHIP_GC9002)
#include <csi_irq.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_SPIFLASH_ENABLED


#define CSI_QSPI_FLASH_DEBUG                (1U)//debug mode：init will send reset opcode to flash device
#define CSI_SFUD_DEBUG_PRINT                (0U)
#define CSI_FLASH_SFDP_CMD_ENABLE           (1U)
#define CSI_QSPI_WRITE_PROTECTION_ENABLE    (1U)
/* support maximum SFDP major revision by driver */
#define CSI_SUPPORT_MAX_SFDP_MAJOR_REV      1U
/* the JEDEC basic flash parameter table length is 9 DWORDs (288-bit) on JESD216 (V1.0) initial release standard */
#define CSI_SFDP_BASIC_TABLE_LEN            9U
#if(defined CSI_SFUD_DEBUG_PRINT && CSI_SFUD_DEBUG_PRINT == 1U && defined CONFIG_RTOS_ENABLE && CONFIG_RTOS_ENABLE == 1U)
#define SFUD_DEBUG(...)                     LOG_D((STR))
#define SFUD_INFO(...)                      LOG_I((STR))
#else
#define SFUD_DEBUG(...)                     
#define SFUD_INFO(...)
#endif


#if(defined CHIP_GC9002)
#define __CSI_FLASH_DRIVER_ATTRIBUTE 	    CONFIG_ATTRIBUTE_FLASH_DRIVER
#define CSI_AHB_MAP_ADDR_LENGTH             (0x8000000U)
#define CSI_AHB_MAP_ADDR_MASK               (0x00000000U)
#define CSI_AHB_OFF_ADDR_MASK               (0xFFFFFFFFU)
#define CSI_QSPI_FLASH_READ_444_DISABLE     (1U)
#define CSI_QSPI_FLASH_READ_144_DISABLE     (0U)
#elif(defined CHIP_GC9005)
#define __CSI_FLASH_DRIVER_ATTRIBUTE 		            
#define CSI_AHB_MAP_ADDR_LENGTH             (0x2000000U)
#define CSI_AHB_MAP_ADDR_MASK               (0xFF000000U)
#define CSI_AHB_OFF_ADDR_MASK               (0x00FFFFFFU)
#define CSI_QSPI_FLASH_READ_444_DISABLE     (1U)
#define CSI_QSPI_FLASH_READ_144_DISABLE     (0U)
#endif

#define CSI_SFDP_SIGNATURE                  (0x50444653U)   //SFDP  Serial Flash Discoverable Parameters
#define CSI_SFDP_FREQ                       CONFIG_QSPI_FLASH_FREQ
#define CSI_SFDP_ADDR_BITS                  QSPI_ADDRESS_32_BITS
#define CSI_SFDP_DUMMY_WRITE                (0x00)

#define CSI_QSPI_FLASH_CNT                  CSI_QSPI_CNT
#define CSI_SPI_FLASH_CNT                   CSI_SPI_CNT

#define CSI_TIMEOUT_CLOCKS                  (0xFFFFFFFFUL)

/* Common Flash Opcode Define -------------------------------------------------------*/
#define CSI_COM_OP_READ_DEVICE_ID           (0x9F)
#define CSI_COM_OP_READ_SFDP                (0x5A)
#define CSI_COM_OP_READ_SR1                 (0x05)
#define CSI_COM_OP_READ_SR2                 (0x35)
#define CSI_COM_OP_READ_SR3                 (0x15)
#define CSI_COM_OP_WRITE_SR1                (0x01)
#define CSI_COM_OP_WRITE_SR2                (0x31)
#define CSI_COM_OP_WRITE_SR3                (0x11)
#define CSI_COM_OP_PAGE_PROGRAM             (0x02)
#define CSI_COM_OP_CHIP_ERASE1              (0x60)
#define CSI_COM_OP_CHIP_ERASE2              (0xC7)

#define CSI_COM_OP_111_PAGE_PROGRAM         (0x02)
#define CSI_COM_OP_111_4PAGE_PROGRAM        (0x12)
#define CSI_COM_OP_114_PAGE_PROGRAM         (0x32)
#define CSI_COM_OP_114_4PAGE_PROGRAM        (0x34)
#define CSI_COM_OP_144_PAGE_PROGRAM         (0x3E)

#define CSI_COM_OP_ENTER_QPI_MODE           (0x38)
#define CSI_COM_OP_EXIT_QPI_MODE            (0xFF)

#define CSI_COM_OP_111_READ                 (0x03)

#define CSI_COM_OP_ENTER_4BADDR_MODE        (0xB7)
#define CSI_COM_OP_EXIT_4BADDR_MODE         (0xE9)
#define CSI_COM_OP_WRITE_ENABLE             (0x06)
#define CSI_COM_OP_WRITE_ENABLE_VSR         (0x50)

#define CSI_COM_OP_RESET_ENABLE             (0x66)
#define CSI_COM_OP_RESET                    (0x99)
#define CSI_COM_OP_CLEAR_SR                 (0x30)

#if(!(defined CSI_FLASH_SFDP_CMD_ENABLE) || CSI_FLASH_SFDP_CMD_ENABLE == 0U)
/* GD25Q257D Define -----------------------------------------------------------------*/
#define CSI_GD25Q257D_ID                    (0xC8UL)
#define CSI_IS_GD25Q257D_ID(ID)             (((ID) & 0xFF) == CSI_GD25Q257D_ID)
#define CSI_GD25Q257D_FREQ                  CSI_QSPI_FREQ_MAX

#define CSI_GD25Q257D_INDIRECT_INST_BUS_WIDTH            QSPI_CFG_BUS_SINGLE
#define CSI_GD25Q257D_INDIRECT_ADDR_READ_BUS_WIDTH       QSPI_CFG_BUS_QUAD
#define CSI_GD25Q257D_INDIRECT_DATA_READ_BUS_WIDTH       QSPI_CFG_BUS_QUAD
#define CSI_GD25Q257D_INDIRECT_ADDR_WRITE_BUS_WIDTH      QSPI_CFG_BUS_SINGLE
#define CSI_GD25Q257D_INDIRECT_DATA_WRITE_BUS_WIDTH      QSPI_CFG_BUS_SINGLE

#define CSI_GD25Q257D_ADDR_BITS                 QSPI_ADDRESS_32_BITS

#define CSI_GD25Q257D_OPCODE_READ               (0xEC)
#define CSI_GD25Q257D_OPCODE_WRITE              (0x02)
#define CSI_GD25Q257D_DUMMY_READ                (0x04)
#define CSI_GD25Q257D_DUMMY_WRITE               (0x00)
#define CSI_GD25Q257D_MODE_BIT_EN               (true)
#define CSI_GD25Q257D_MODE_BIT                  (0xFF)
#define CSI_GD25Q257D_PAGE_SIZE                 (256U)
#define CSI_GD25Q257D_BLOCK_SIZE                (65536U)
#endif
typedef enum
{
    FLASH_ADDR_24,      //24位地址
    FLASH_ADDR_32,      //32位地址
}csi_spiflash_addr_mode_t;
typedef enum
{
    LOCK_TP_NONE,       //无保护
    LOCK_TP_TOP,        //Upper region
    LOCK_TP_BOTTOM,     //Lower region
    LOCK_TP_DUAL,       //
}csi_spiflash_lock_region_t;

#define CSI_IS_SFDP_PTP_BASE_ADDR(ADDR)         ((ADDR) < 0xFF)

struct _FLASH_SR
{
    union
    {
        uint8_t u8_value;
        struct{
            uint8_t BUSY : 1;  /* Erase/Write In Progress */
            uint8_t WEL : 1;   /* Write Enable Latch */
            uint8_t BP0 : 1;   /* Block Protect Bits */
            uint8_t BP1 : 1;   /* Block Protect Bits */
            uint8_t BP2 : 1;   /* Block Protect Bits */
            uint8_t BP3 : 1;   /* Block Protect Bits */
            uint8_t TB : 1;    /* Top/Bottom Protect Bit */
            uint8_t SRP0 : 1;  /* Status Register Protect 0 */
        }bits;
    }SR1;
    union
    {
        uint8_t u8_value;
        struct{
            uint8_t SRP1 : 1;  /* Status Register Protect 1 */
            uint8_t QE : 1;    /* Quad Enable */
            uint8_t     : 1;   /* Reserved */
            uint8_t LB1 : 1;   /* Security Register Lock Bits */
            uint8_t LB2 : 1;   /* Security Register Lock Bits */
            uint8_t LB3 : 1;   /* Security Register Lock Bits */
            uint8_t CMP : 1;   /* Complement Protect */
            uint8_t SUS : 1;   /* Suspend Status */
        }bits;
    }SR2;
    union
    {
        uint8_t u8_value;
        struct{
            uint8_t ADS : 1;       /* Current Address Mode */
            uint8_t ADP : 1;       /* Power Up Address Mode */
            uint8_t WPS : 1;       /* Write Protect Selection */
            uint8_t     : 1;       /* Reserved */
            uint8_t     : 1;       /* Reserved */
            uint8_t DRV0 : 1;      /* Output Driver Strength */
            uint8_t DRV1 : 1;      /* Output Driver Strength */
            uint8_t HOLD_RST : 1;  /* /HOLD or /RESET Function */
        }bits;
    }SR3;
    uint8_t Reserved;
};
typedef struct
{
    char *flash_name;                           //spiflash名称
    struct _FLASH_SR SR;
    uint32_t JEDEC_PTP_base_addr : 24;               //First address of JEDEC Flash Parameter table
    uint32_t JEDEC_PTP_EN : 1;
    uint32_t addr_3_byte  : 1;
    uint32_t addr_4_byte  : 1;
    uint32_t              : 5;
    uint32_t MAKER_PTP_base_addr : 8;               //First address of manufacturer Flash Parameter table
    uint32_t MAKER_PTP_EN : 1;
    uint32_t              : 7;
    uint32_t BYTE4_PTP_base_addr : 8;               //First address of 4-Byte Address Flash Parameter table
    uint32_t BYTE4_PTP_EN : 1;
    uint32_t              : 7;
    uint32_t flash_id;                          //JEDEC ID
    uint32_t capacity;                          //flash capacity (bytes)
    uint32_t xip_addr;                          //XIP地址
    uint32_t block_size;                        //块大小
    uint32_t sector_size;                       //扇区大小
    uint32_t page_size;                         //页大小
    uint32_t protect_blk_size;                  //保护区域单位
    csi_spiflash_lock_region_t protect_type;    //保护区域类型
}csi_spiflash_info_t;
typedef enum
{
    SPIFLASH_DATA_1_LINE,       //单线模式
    SPIFLASH_DATA_2_LINES,      //双线模式
    SPIFLASH_DATA_4_LINES,      //四线模式
}csi_spiflash_data_line_t;
typedef enum
{
    SECTOR_ERASE,       //扇区擦除
    HALF_BLOCK_ERASE,   //半块擦除
    BLOCK_ERASE,        //块擦除
    CHIP_ERASE,         //全片擦除
}csi_spiflash_erase_type_t;
typedef enum
{
    SPIFLASH_BUS_SPI,
    SPIFLASH_BUS_QSPI,
}csi_spiflash_bus_type_t;
typedef struct 
{
    csi_spi_t spi;                  //spi csi Device handle
    csi_qspi_t qspi;                //qspi csi Device handle
    csi_spiflash_bus_type_t type;   //
}csi_spi_qspi_t;
typedef struct
{
    uint8_t write_enable;
    uint8_t write_enable_vsr;
    uint8_t read_sr1;
    uint8_t read_sr2;

    uint8_t read_sr3;
    uint8_t write_sr1;
    uint8_t write_sr2;
    uint8_t write_sr3;
    
    uint8_t erase_4kb;
    uint8_t erase_sector;
    uint8_t erase_half_block;
    uint8_t erase_full_block;
    
    uint8_t erase_chip;
    uint8_t enter_4bits_addr;
    uint8_t Mode_444_Disable;
    uint8_t Mode_444_Enable;

    uint8_t enter_qpi_mode;
    uint8_t exit_qpi_mode;
    uint8_t Reserved0;
    uint8_t Reserved1;
}csi_flash_opcode_t;

typedef struct _csi_spiflash
{
    csi_spi_qspi_t spi_qspi;    //csi Device unity handle
    void (*spi_cs_callback)(csi_gpio_pin_state_t value);    //软件片选控制函数
    csi_spiflash_info_t flash_prv_info;                     //flash设备信息
    csi_flash_opcode_t opcode;
    csi_error_t(*spi_send)(void *spi, uint8_t cmd, uint32_t addr, uint32_t addr_size, const void *data, uint32_t size);
    csi_error_t(*spi_receive)(void *spi, uint8_t cmd, uint32_t addr, uint32_t addr_size, void *data, uint32_t size);
    void *priv; //Device private variable
}csi_spiflash_t;



csi_error_t csi_spiflash_spi_init(csi_spiflash_t *spiflash, uint32_t spi_idx, void(*spi_cs_callback)(csi_gpio_pin_state_t value));
__CSI_FLASH_DRIVER_ATTRIBUTE csi_error_t csi_spiflash_qspi_init(csi_spiflash_t *spiflash, uint32_t qspi_idx, void (*spi_cs_callback)(csi_gpio_pin_state_t value));
void        csi_spiflash_spi_uninit(csi_spiflash_t *spiflash);
void        csi_spiflash_qspi_uninit(csi_spiflash_t *spiflash);

csi_error_t csi_spiflash_get_flash_info(csi_spiflash_t *spiflash, csi_spiflash_info_t *flash_info);
__CSI_FLASH_DRIVER_ATTRIBUTE int32_t     csi_spiflash_read(csi_spiflash_t *spiflash, uint32_t offset, void *data, uint32_t size);
__CSI_FLASH_DRIVER_ATTRIBUTE int32_t     csi_spiflash_program(csi_spiflash_t *spiflash, uint32_t offset, const void *data, uint32_t size);
__CSI_FLASH_DRIVER_ATTRIBUTE csi_error_t csi_spiflash_erase(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size);//use sector erase opcode<4kb erase>
__CSI_FLASH_DRIVER_ATTRIBUTE csi_error_t csi_spiflash_block_erase(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size);//use block erase opcode<64kb erase>
__CSI_FLASH_DRIVER_ATTRIBUTE csi_error_t csi_spiflash_chip_erase(csi_spiflash_t *spiflash);

__CSI_FLASH_DRIVER_ATTRIBUTE csi_error_t csi_spiflash_read_reg(csi_spiflash_t *spiflash, uint8_t cmd_code, uint8_t *data, uint32_t size);
__CSI_FLASH_DRIVER_ATTRIBUTE csi_error_t csi_spiflash_write_reg(csi_spiflash_t *spiflash, uint8_t cmd_code, uint8_t *data, uint32_t size);
csi_error_t csi_spiflash_lock(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size);
csi_error_t csi_spiflash_unlock(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size);
int         csi_spiflash_is_locked(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size);
csi_error_t csi_spiflash_config_data_line(csi_spiflash_t *spiflash, csi_spiflash_data_line_t line);
uint32_t    csi_spiflash_frequence(csi_spiflash_t *spiflash, uint32_t hz);
csi_error_t csi_spiflash_release_power_down(csi_spiflash_t *spiflash);
void        csi_spiflash_power_down(csi_spiflash_t *spiflash);

union SFDP_00H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x00 */
    {
        uint32_t SFDP_Signature;
    };
};
union SFDP_04H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x04 */
    {
        uint32_t SFDP_Minor_Revision    : 8;
        uint32_t SFDP_Major_Revision    : 8;
        uint32_t Para_Headers_Num       : 8;
        uint32_t                        : 8;
    };
};
union SFDP_08H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x08 */
    {
        uint32_t ID_number_JEDEC            : 8;
        uint32_t                            : 24;
    };
};
union SFDP_0CH
{
    uint32_t u32_value;
    struct /*!< Offset: 0x0C */
    {
        uint32_t Parameter_Table_Pointer    : 24;
        uint32_t                            : 8;
    };
};
union SFDP_10H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x10 */
    {
        uint32_t ID_Num_Manufacturer        : 8;
        uint32_t                            : 24;
    };
};
union SFDP_14H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x14 */
    {
        uint32_t Parameter_Table_Pointer    : 24;
        uint32_t                            : 8;
    };
};
union SFDP_18H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x18 */
    {
        uint32_t ID_Num_4Byte_Addr_Ins      : 8;
        uint32_t                            : 24;
    };
};
union SFDP_1CH
{
    uint32_t u32_value;
    struct /*!< Offset: 0x1C */
    {
        uint32_t Parameter_Table_Pointer    : 24;
        uint32_t                            : 8;
    };
};
union SFDP_JEDEC_PT_00H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x00+JEDEC_PTP_base_addr */
    {
        uint32_t Block_Sector_Erase_Size    : 2;//01b   01: 4KB erase  11: not support 4KB erase  others: Reserved
        uint32_t Write_Granularity          : 1;//0: 1Byte, 1: 64Byte or larger
        //Write Enable Instruction Requested for Writing to Volatile Status Registers
        uint32_t Write_Enable_Instruction   : 1;//0: Nonvolatile status bit   1: Volatile status bit  
        //Write Enable Opcode Select for Writing to Volatile Status Registers
        uint32_t Write_Enable_Opcode        : 1;//0: Use 50H Opcode    1: Use 06H Opcode
        uint32_t                            : 3;
        uint32_t Erase_4KB_Opcode           : 8;//20H
        uint32_t Fast_Read_1_1_2            : 1;//0=Not support, 1=Support
        uint32_t Address_Bytes_Num          : 2;//00: 3Byte only, 01: 3 or 4Byte, 10: 4Byte only, 11: Reserved
        uint32_t Double_Transfer_Rate       : 1;//0=Not support, 1=Support
        uint32_t Fast_Read_1_2_2            : 1;//0=Not support, 1=Support
        uint32_t Fast_Read_1_4_4            : 1;//0=Not support, 1=Support
        uint32_t Fast_Read_1_1_4            : 1;//0=Not support, 1=Support
        uint32_t                            : 9;
    };
};
union SFDP_JEDEC_PT_04H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x04+JEDEC_PTP_base_addr */
    {
        uint32_t Flash_Memory_Density;//0FFFFFFFH
    };
};
union SFDP_JEDEC_PT_08H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x08+JEDEC_PTP_base_addr */
    {
        uint32_t Fast_Read_Num_Wait_1_4_4       : 5;//0 0000b: Wait states (Dummy Clocks) not support
        uint32_t Fast_Read_Num_Mode_Bits_1_4_4  : 3;//000b:Mode Bits not support
        uint32_t Fast_Read_1_4_4_Opcode         : 8;//EBH
        
        uint32_t Fast_Read_Num_Wait_1_1_4       : 5;//0 0000b: Wait states (Dummy Clocks) not support
        uint32_t Fast_Read_Num_Mode_Bits_1_1_4  : 3;//000b:Mode Bits not support
        uint32_t Fast_Read_1_1_4_Opcode         : 8;//6BH
    };
};
union SFDP_JEDEC_PT_0CH
{
    uint32_t u32_value;
    struct /*!< Offset: 0x0C+JEDEC_PTP_base_addr */
    {
        uint32_t Fast_Read_Num_Wait_1_1_2       : 5;//0 0000b: Wait states (Dummy Clocks) not support
        uint32_t Fast_Read_Num_Mode_Bits_1_1_2  : 3;//000b:Mode Bits not support
        uint32_t Fast_Read_1_1_2_Opcode         : 8;//3BH
        
        uint32_t Fast_Read_Num_Wait_1_2_2       : 5;//0 0000b: Wait states (Dummy Clocks) not support
        uint32_t Fast_Read_Num_Mode_Bits_1_2_2  : 3;//000b:Mode Bits not support
        uint32_t Fast_Read_1_2_2_Opcode         : 8;//42H
    };
};
union SFDP_JEDEC_PT_10H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x10+JEDEC_PTP_base_addr */
    {
        uint32_t Fast_Read_2_2_2    : 1;//0=not support 1=support
        uint32_t                    : 3;
        uint32_t Fast_Read_4_4_4    : 1;//0=not support 1=support
        uint32_t                    : 27;
    };
};
union SFDP_JEDEC_PT_14H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x14+JEDEC_PTP_base_addr */
    {
        uint32_t                                : 16;
        uint32_t Fast_Read_Num_Wait_2_2_2       : 5;//0 0000b: Wait states (Dummy Clocks) not support
        uint32_t Fast_Read_Num_Mode_Bits_2_2_2  : 3;//000b:Mode Bits not support
        uint32_t Fast_Read_2_2_2_Opcode         : 8;//FFH
    };
};
union SFDP_JEDEC_PT_18H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x18+JEDEC_PTP_base_addr */
    {
        uint32_t                                : 16;
        uint32_t Fast_Read_Num_Wait_4_4_4       : 5;//0 0000b: Wait states (Dummy Clocks) not support
        uint32_t Fast_Read_Num_Mode_Bits_4_4_4  : 3;//000b:Mode Bits not support
        uint32_t Fast_Read_4_4_4_Opcode         : 8;//FFH
    };
};
union SFDP_JEDEC_PT_1CH
{
    uint32_t u32_value;
    struct /*!< Offset: 0x1C+JEDEC_PTP_base_addr */
    {
        uint32_t Sector_Type_1_Size             : 8;//0CH   Sector/block size=2^N Bytes  2^12 = 4096
        uint32_t Sector_Type_1_erase_Opcode     : 8;//20H
        uint32_t Sector_Type_2_Size             : 8;//0FH   Sector/block size=2^N Bytes  2^15 = 32768
        uint32_t Sector_Type_2_erase_Opcode     : 8;//52H
    };
};
union SFDP_JEDEC_PT_20H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x20+JEDEC_PTP_base_addr */
    {
        uint32_t Sector_Type_3_Size             : 8;//10H   Sector/block size=2^N Bytes  2^16 = 65536
        uint32_t Sector_Type_3_erase_Opcode     : 8;//D8H
        uint32_t Sector_Type_4_Size             : 8;//00H   Sector/block size=2^N Bytes
        uint32_t Sector_Type_4_erase_Opcode     : 8;//FFH
    };
};
union SFDP_JEDEC_PT_24H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x24+JEDEC_PTP_base_addr */
    {
        //Multiplier from typical erase time to maximum erase time
        uint32_t Typical_max_time_erase         : 4;//0010b   
        uint32_t Typical_time_erase_Type_1      : 7;//010 0100b
        uint32_t Typical_time_erase_Type_2      : 7;//010 1100b
        uint32_t Typical_time_erase_Type_3      : 7;//011 0010b
        uint32_t Typical_time_erase_Type_4      : 7;//111 1111b  Not exist
    };
};
union SFDP_JEDEC_PT_28H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x28+JEDEC_PTP_base_addr */
    {
        //Multiplier from typical time to maxtime for Page or Byte program
        uint32_t Typical_max_time_Page_program          : 4;//0010b   
        uint32_t Page_Size                              : 4;//8H        size=2^N Bytes  2^8 = 256   Page size=256Byte
        uint32_t Typical_time_Page_program              : 6;//10 1001b Page program=600us/640us
        uint32_t Typical_time_Byte_first_program        : 5;//1 0011b     First Byte program=30us/32us
        uint32_t Typical_time_Byte_additional_program   : 5;//0 0010b     Additional Byte program=2.5us/3us
        uint32_t Typical_time_Chip_Erase                : 7;//101 1000b     Chip erase typical time=100s/100s
        uint32_t                                        : 1;
    };
};
union SFDP_JEDEC_PT_2CH
{
    uint32_t u32_value;
    struct /*!< Offset: 0x2C+JEDEC_PTP_base_addr */
    {
        //Prohibited Operations During Program Suspend
        uint32_t Prohibited_Program_Suspend         : 4;//1100b
        //Prohibited Operations During Erase Suspend
        uint32_t Prohibited_Erase_Suspend           : 4;//1110b
        uint32_t                                    : 1;
        uint32_t Program_Resume_to_Suspend_Interval : 4;//0000b  Interval=64us
        //Suspend in-progress program max latency
        uint32_t Suspend_to_Program_Interval        : 7;//011 0011b  max latency=20us/20us
        //Erase Resume to Suspend latency
        uint32_t Erase_to_Suspend_Interval          : 4;//0000b  Interval=64us
        //Suspend in-progress erase max latency
        uint32_t Suspend_to_Erase_Interval          : 7;//011 0011b  max latency=20us/20us
        uint32_t                                    : 1;
    };
};
union SFDP_JEDEC_PT_30H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x30+JEDEC_PTP_base_addr */
    {
        uint32_t Program_Resume_Instruction     : 8;//7AH
        uint32_t Program_Suspend_Instruction    : 8;//75H
        uint32_t Resume_Instruction             : 8;//7AH
        uint32_t Suspend_Instruction            : 8;//75H
    };
};
union SFDP_JEDEC_PT_34H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x34+JEDEC_PTP_base_addr */
    {
        uint32_t                                        : 2;
        //Use of legacy polling is supported by reading the Status Register with 05h instruction and checking WIP bit[0] (0=ready; 1=busy)
        uint32_t Status_Register_Polling_Device_Busy    : 6;//0111101b 
        //Exit Deep Power down to next operation delay
        uint32_t Exit_Deep_next_operation_delay         : 7;//011 1101b   Delay=30us/30us
        //Exit Deep Power down Instruction
        uint32_t Exit_Deep_Power_Instruction            : 8;//1010 1011b  
        //Enter Deep Power down Instruction
        uint32_t Enter_Deep_Power_Instruction           : 8;//1011 1001b
        uint32_t Deep_Power_down_Supported              : 1;//0b
    };
};
union SFDP_JEDEC_PT_38H
{
    uint32_t u32_value;
    struct /*!< Offset: 0x38+JEDEC_PTP_base_addr */
    {
        uint32_t mode_4_4_4_disable_sequences   : 4;//Not support 4-4-4 mode
        uint32_t mode_4_4_4_enable_sequences    : 5;//Not support 4-4-4 mode
        uint32_t mode_0_4_4_supported           : 1;//1b Supported
        uint32_t mode_0_4_4_Exit_Method         : 6;//000001b
        uint32_t mode_0_4_4_Entry_Method        : 4;//0100b
        uint32_t Quad_Enable_Requirements       : 3;//100b        QE is in status register 2, bit 1
        uint32_t RESET_Disable                  : 1;//0b
        uint32_t                                : 8;
    };
};
union SFDP_JEDEC_PT_3CH
{
    uint32_t u32_value;
    struct /*!< Offset: 0x3C+JEDEC_PTP_base_addr */
    {
        //Volatile or Non-Volatile Register and Write Enable Instruction for Status Register 1
        uint32_t Write_Enable_Instruction_SR1   : 7;//0001000b
        uint32_t                                : 1;//
        //Soft Reset and Rescue Sequence Support
        uint32_t Soft_Reset_Rescue              : 6;//010000b
        uint32_t Exit_4Byte_Addressing          : 10;//00 0000 0001b
        uint32_t Enter_4Byte_Addressing         : 8;//0000 0001b
    };
};
union SFDP_BYTE4_PT_00H
{
    uint32_t u32_value;
    struct/*!< Offset: 0x30+MAKER_PTP_base_addr */
    {
        //Support for (1-1-1) READ command. Instruction = 13H
        uint32_t READ_cmd_1_1_1_Sup     : 1;//1b  0 = NOT supported  1 = supported
        //Support for (1-1-1) FAST READ command. Instruction = 0CH
        uint32_t FASTREAD_cmd_1_1_1_Sup : 1;//1b
        //Support for (1-1-2) FAST READ command. Instruction = 3CH
        uint32_t FASTREAD_cmd_1_1_2_Sup : 1;//1b
        //Support for (1-2-2) FAST READ command. Instruction = BCH
        uint32_t FASTREAD_cmd_1_2_2_Sup : 1;//1b
        //Support for (1-1-4) FAST READ command. Instruction = 6CH
        uint32_t FASTREAD_cmd_1_1_4_Sup : 1;//1b
        //Support for (1-4-4) FAST READ command. Instruction = ECH
        uint32_t FASTREAD_cmd_1_4_4_Sup : 1;//1b
        //Support for (1-1-1) Page Program command. Instruction = 12H
        uint32_t Page_Program_cmd_1_1_1_Sup : 1;//1b
        //Support for (1-1-4) Page Program command. Instruction = 34H
        uint32_t Page_Program_cmd_1_1_4_Sup : 1;//1b
        
        //Support for (1-4-4) Page Program command. Instruction = 3EH
        uint32_t Page_Program_cmd_1_4_4_Sup : 1;//0b
        //Support for Erase Command - Type 1 size. Instruction lookup in next the dword
        uint32_t Erase_cmd_Type1_size_Sup   : 1;//1b
        //Support for Erase Command - Type 2 size. Instruction lookup in next the dword
        uint32_t Erase_cmd_Type2_size_Sup   : 1;//1b
        //Support for Erase Command - Type 3 size. Instruction lookup in next the dword
        uint32_t Erase_cmd_Type3_size_Sup   : 1;//1b
        //Support for Erase Command - Type 4 size. Instruction lookup in next the dword
        uint32_t Erase_cmd_Type4_size_Sup   : 1;//0b
        //Support for (1-1-1) DTR READ command. Instruction = 0EH
        uint32_t DTRREAD_cmd_1_1_1_Sup      : 1;//0b
        //Support for (1-2-2) DTR READ command. Instruction = BEH
        uint32_t DTRREAD_cmd_1_2_2_Sup      : 1;//0b
        //Support for (1-4-4) DTR READ command. Instruction = EEH
        uint32_t DTRREAD_cmd_1_4_4_Sup      : 1;//1b.
        
        //Support for volatile individual sector lock Read command. Instruction = E0H
        uint32_t sector_lock_read_cmd_Sup       : 1;//0b
        //Support for volatile individual sector lock Write command. Instruction = E1H
        uint32_t sector_lock_Write_cmd_Sup      : 1;//0b
        //Support for non-volatile individual sector lock Read command. Instruction = E2H
        uint32_t sector_non_lock_Read_cmd_Sup   : 1;//0b
        //Support for non-volatile individual sector lock Write command. Instruction = E3H
        uint32_t sector_non_lock_Write_cmd_Sup  : 1;//0b
        uint32_t                                : 12;
    };
};


#endif /* CONFIG_CSI_DRV_SPIFLASH_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_SPIFLASH_H__ */