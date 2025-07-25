/**
  ******************************************************************************
  * @file    gc90xx_ll_sdmmc.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2022-10-18
  * @brief   This file contains all the functions prototypes for the SDMMC
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_LL_SDMMC_H__

#define __GC90XX_LL_SDMMC_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal_def.h>
#ifdef HAL_SDMMC_MODULE_ENABLED

/** @addtogroup gc9005_Driver
  * @{
  */
/* Exported types ------------------------------------------------------------*/ 
/** @defgroup SDMMC_LL_Exported_Types SDMMC_LL Exported Types
  * @{
  */
 /**
   * @brief  SDMMC Host Configuration Structure definition
   */
typedef struct
{
    uint32_t DDRModeEn[4];      /*!< Specifies DDR mode enable on Slotx.
                                        This parameter can be a value of @ref SDIO_DISABLE/SDIO_ENABLE              */
    uint32_t MMC8ModeEn[4];     /*!< Specifies MMC-8 mode enable on Slotx. if set disable, 1/4 bit mode is enable;
                                        This parameter can be a value of @ref SDIO_DISABLE/SDIO_ENABLE              */
    uint32_t DebouncePeriod;    /*!< Specifies Debounce Period.Typically, DP value should be chosen to obtain the
                                        period of 20ms.
                                        This parameter can be a value of @ref 20                                    */
    uint32_t BurstLength;       /*!< Specifies The maximum number of beats definition within single DMA burst.
                                        This parameter can be a value of @ref SDIO_LL_Burst_Length                  */
    uint32_t CPRMClkEn;         /*!< Specifies CPRM clock enable.
                                        This parameter can be a value of @ref SDIO_DISABLE/SDIO_ENABLE              */
    uint32_t CPRMEn;            /*!< Specifies CPRM enable.
                                        This parameter can be a value of @ref SDIO_DISABLE/SDIO_ENABLE              */
}SDIO_HostInitTypeDef;
/* HWInit SRS16 configuration for slot #x */
typedef struct
{
    union
    {
        uint32_t value;
        struct{
            uint32_t TCF        : 6;//Timeout Clock Frequency
            uint32_t            : 1;
            uint32_t TCU        : 1;//Timeout Clock Unit,0 – kHz, 1 – MHz
            uint32_t BCSDCLK    : 8;//Base Clock Frequency For SD Clock
            uint32_t MBL        : 2;//Max Block Length
            uint32_t _8EDS      : 1;//8-bit Embedded Device Support
            uint32_t ADMA2S     : 1;//ADMA2 Support
            uint32_t ADMA1S     : 1;//ADMA1 Support
            uint32_t HSS        : 1;//High Speed Support
            uint32_t DMAS       : 1;//SDMA Support
            uint32_t SRS        : 1;//Suspend/Resume Support
            uint32_t VS33       : 1;//Voltage Support 3.3V
            uint32_t VS30       : 1;//Voltage Support 3.0V
            uint32_t VS18       : 1;//Voltage Support 1.8V
            uint32_t            : 1;
            uint32_t _64BS      : 1;//64-bit System Bus Support
            uint32_t AIS        : 1;//Asynchronous Interrupt Support
            uint32_t SLT        : 2;//<31..30>Slot Type
        };
    };
}SDIO_HwInitCR1;
/* HWInit SRS17 configuration for slot #x */
typedef struct
{
    union
    {
        uint32_t value;
        struct{
            uint32_t SDR50      : 1;//SDR50 Supported
            uint32_t SDR104     : 1;//SDR104 Supported
            uint32_t DDR50      : 1;//DDR50 Supported
            uint32_t            : 1;
            uint32_t DRVA       : 1;//1.8V Line Driver Type A Supported
            uint32_t DRVC       : 1;//1.8V Line Driver Type C Supported
            uint32_t DRVD       : 1;//1.8V Line Driver Type D Supported
            uint32_t            : 1;
            uint32_t RTNGCNT    : 4;//Timer Count for Re-Tuning,2^(n-1) second,if set 0,disable
            uint32_t RESERVED1  : 1;
            uint32_t UTSM50     : 1;//Use Tuning for SDR50
            uint32_t RTNGM      : 2;//Re-Tuning Modes
            uint32_t CLKMPR     : 8;//Clock Multiplier
            uint32_t            : 8;
        };
    };
}SDIO_HwInitCR2;
/* HWInit SRS18 configuration for slot #x */
typedef struct
{
    union
    {
        uint32_t value;
        struct{
            uint32_t MC33       : 8;//Maximum Current for 3.3V
            uint32_t MC30       : 8;//Maximum Current for 3.0V
            uint32_t MC18       : 8;//Maximum Current for 1.8V
            uint32_t            : 8;
        };
    };
}SDIO_HwInitCR3;
typedef struct
{
    SDIO_HwInitCR1 mCr1;
    SDIO_HwInitCR2 mCr2;
    SDIO_HwInitCR3 mCr3;
}SDIO_HwInitType;
/** 
  * @brief  SDMMC Configuration Structure definition  
  */
typedef struct
{
    uint32_t SpeedMode;            /*!< Specifies the clock transition on which the bit capture is made.
                                      This parameter can be a value of @ref SDIO_LL_Clock_Edge                 */
    uint32_t ClockMode;            /*!< Specifies Clock Generator Select
                                      This parameter can be a value of @ref SDIO_LL_Clock_Mode                 */
    uint32_t BusWide;              /*!< Specifies the SDMMC bus width.
                                      This parameter can be a value of @ref SDIO_LL_Bus_Wide                   */
    uint32_t ClockDiv;             /*!< Specifies the clock frequency of the SDMMC controller.
                                      This parameter can be a value between Min_Data = 0 and Max_Data = 0x3FF  */
}SDIO_InitTypeDef;

enum SDIO_LL_Burst_Length
{
    SDIO_Beats_default = 0x0UL,
    SDIO_Beats_1 = 0x1UL,
    SDIO_Beats_2,
    SDIO_Beats_4,
    SDIO_Beats_8,
    SDIO_Beats_16,
    SDIO_Beats_32,
    SDIO_Beats_64,
    SDIO_Beats_128,
    SDIO_Beats_256,
    SDIO_Beats_512,
    SDIO_Beats_1024,
    SDIO_Beats_2048,
};

#define SD_HCSR0_WRITE_MASK   0x070F0FFFUL
#define SD_HCSR1_WRITE_MASK   0x070FFFE5UL
#define SD_HCSR2_WRITE_MASK   0xC0FF0000UL
typedef struct
{
    union
    {
        uint32_t value;
        struct{
            uint32_t LEDC       : 1;//LED Control.
            uint32_t DTW        : 1;//Data Transfer Width.
            uint32_t HSE        : 1;//High Speed Enable.
            uint32_t DMASEL     : 2;//DMA Select.
            uint32_t EDTW       : 1;//Extended Data Transfer Width.
            uint32_t CDTL       : 1;//Card Detect Test Level.
            uint32_t CDSS       : 1;//<!-for test purposes-!>Card Detect Signal Selection.
            uint32_t BP         : 1;//SD Bus Power.
            uint32_t BVS        : 3;//SD Bus Voltage Select.
            uint32_t            : 4;
            uint32_t SBGR       : 1;//Stop At Block Gap Request.
            uint32_t CR         : 1;//Continue Request.
            uint32_t RWC        : 1;//Read Wait Control.
            uint32_t IBG        : 1;//Interrupt At Block Gap.
            uint32_t            : 4;
            uint32_t WCINT      : 1;//Wakeup Event Enable On Card Interrupt.
            uint32_t WCI        : 1;//Wakeup Event Enable On Card Inserted.
            uint32_t WCR        : 1;//Wakeup Event Enable On SD Card Removal.
            uint32_t            : 5;
        };
    };
}SDIO_HCSR0Type;
typedef struct
{
    union
    {
        uint32_t value;
        struct{
            uint32_t ICE        : 1;//Internal Clock Enable.
            uint32_t ICS        : 1;//<!- Read Only -!>Internal Clock Stable.
            uint32_t SDCE       : 1;//SD Clock Enable.
            uint32_t            : 2;
            uint32_t CGS        : 1;//Clock Generator Select.
            uint32_t USDCLKFS   : 2;//SDCLK Frequency Select (upper).
            uint32_t SDCLKFS    : 8;//SDCLK Frequency Select (lower).
            uint32_t DTCV       : 4;//Data Timeout Counter Value.
            uint32_t            : 4;
            uint32_t SRFA       : 1;//Software Reset For All.
            uint32_t SRCMD      : 1;//Software Reset For CMD Line.
            uint32_t SRDAT      : 1;//Software Reset For DAT Line.
            uint32_t            : 5;
        };
    };
}SDIO_HCSR1Type;
typedef struct
{
    union
    {
        uint32_t value;
        struct{
            uint32_t ACNE       : 1;//<!- Read Only -!>Auto CMD12 Not Executed.
            uint32_t ACTE       : 1;//<!- Read Only -!>Auto CMD Timeout Error.
            uint32_t ACCE       : 1;//<!- Read Only -!>Auto CMD CRC Error.
            uint32_t ACEBE      : 1;//<!- Read Only -!>Auto CMD End Bit Error.
            uint32_t ACIE       : 1;//<!- Read Only -!>Auto CMD Index Error.
            uint32_t            : 2;
            uint32_t CNIACE     : 1;//<!- Read Only -!>Command Not Issued By Auto CMD12 Error.
            uint32_t            : 8;
            uint32_t UHSMS      : 3;//UHS Mode Select.
            uint32_t _1V8SE     : 1;//1.8V Signaling Enable.
            uint32_t DSS        : 2;//Driver Strength Select.
            uint32_t EXTNG      : 1;//Execute Tuning.
            uint32_t SCS        : 1;//Sampling Clock Select.
            uint32_t            : 6;
            uint32_t AIE        : 1;//Asynchronous Interrupt Enable.
            uint32_t PVE        : 1;//Preset Value Enable.
        };
    };
}SDIO_HCSR2Type;
/** 
  * @brief  SDMMC Command Control structure 
  */
typedef struct                                                                                            
{
    uint32_t Argument;              /*!< Specifies the SDMMC command argument which is sent
                                         to a card as part of a command message. If a command
                                         contains an argument, it must be loaded into this register
                                         before writing the command to the command register.                */
    uint32_t CmdIndex;              /*!< Specifies the SDMMC command index.
                                         This parameter can be a value of @ref SDIO_II_CMD_Index_Type       */
    uint32_t CmdType;               /*!< Specifies the SDMMC command type.
                                         This parameter can be a value of @ref SDIO_II_CMD_Type_Type        */
    uint32_t CmdIndexCheckEn;       /*!< Specifies the SDMMC command index Check Enable.
                                         This parameter can be a value of SDIO_DISABLE/SDIO_ENABLE          */
    uint32_t CmdCRCCheckEn;         /*!< Specifies the SDMMC command CRC Check Enable.
                                         This parameter can be a value of SDIO_DISABLE/SDIO_ENABLE          */
    uint32_t DataEn;                /*!< Specifies the Data Present Select.
                                         This parameter can be a value of SDIO_DISABLE/SDIO_ENABLE          */
    uint32_t TransferDir;           /*!< Specifies the data transfer direction, whether the transfer
                                         is a read or write.
                                         This parameter can be a value of @ref SDIO_LL_Transfer_Direction   */
    uint32_t DMAEn;                 /*!< Specifies the SDMMC DMA Enable
                                         This parameter can be a value of @ref SDIO_DISABLE/SDIO_ENABLE
                                         ADMA2: the starting address of the data page must by aligned to
                                         the 4B boundary (the 2 least significant bits set to 0).           */
    uint32_t Response;              /*!< Specifies the SDMMC response type.
                                         This parameter can be a value of @ref SDIO_LL_Response_Type        */
    uint32_t DataBlockSelect;       /*!< Specifies the SDMMC Multi/Single Block Select.
                                         This parameter can be a value of @ref SDIO_LL_Data_Block_Select    */
    uint32_t AutoCmdSelect;         /*!< Specifies the SDMMC Auto CMD Enable
                                         This parameter can be a value of @ref SDMMC_LL_Auto_Cmd_Select     */
    uint32_t BlockCountEn;          /*!< Specifies the SDMMC Block Count Enable
                                         This parameter can be a value of @ref SDIO_DISABLE/SDIO_ENABLE     */
}SDIO_CmdInitTypeDef;
/**
  * @brief  SDMMC Data Control structure
  */
typedef struct
{
    uint32_t DataTimeOut;       /*!< Specifies the data timeout period in card bus clock periods.  */
    uint32_t DataLength;        /*!< Specifies the number of data bytes to be transferred.         */
    uint32_t DataBlockSize;     /*!< Specifies the data block size for block transfer.
                                    This parameter can be a value of @ref SDIO_LL_Data_Block_Size    */
}SDIO_DataInitTypeDef;
//DMA Select
enum SDIO_DMA_Select
{
    SDIO_SDMA = 0x0UL,
    SDIO_ADMA1 = 0x1UL,
    SDIO_ADMA2 = 0x2UL,
};
/**
  * @brief  SDMA Descriptor struct
  */
typedef struct
{
    uint32_t Address;               /*!< Simple DMA System Address.                                                 */
    uint32_t BlockSize;             /*!< Size of DMA Transfer Block.                                                */
    uint32_t BlockCount;            /*!< Count of DMA Transfer Block.                                               */
    uint32_t BurstLength;           /*!< Specifies The maximum number of beats definition within single DMA burst.
                                        This parameter can be a value of @ref SDIO_LL_Burst_Length                  */
    uint32_t BufferBoundary;        /*!< Host DMA Buffer Boundary.                                                  */
}SDIO_SDMA_Descriptor;
/**
  * @brief  ADMA1 Descriptor struct
  */
typedef struct
{
    union
    {
        uint32_t PageAddress;   /*!< The field contains data page address, data page length, or next
                                      descriptor list address, depending on the descriptor type.        */
        struct
        {
            uint32_t VAL         : 1;
            uint32_t END         : 1;
            uint32_t INT         : 1;
            uint32_t             : 1;
            uint32_t ACT         : 2;    /*!< The field defines the type of the descriptor.                     */
            uint32_t             : 6;
            uint32_t Length      : 16;   /*!< The field contains data page address, data page length, or next
                                          descriptor list address, depending on the descriptor type.        */
            uint32_t             : 4;
        };
    };
}SDIO_ADMA1_Descriptor;
/**
  * @brief  ADMA2 Descriptor struct
  */
typedef struct
{
    uint64_t VAL         : 1;
    uint64_t END         : 1;
    uint64_t INT         : 1;
    uint64_t             : 1;
    uint64_t ACT         : 2;   /*!< The field defines the type of the descriptor.
                                     This parameter can be a value of @ref SDIO_ADMA_Descriptor_Type    */
    uint64_t             : 10;
    uint64_t PageLength  : 16;  /*!< The field contains data page length in bytes.                      */
    uint64_t PageAddress : 32;  /*!< The field contains data page address or next descriptor
                                         list address depending on the descriptor type.                 */
}SDIO_ADMA2_Descriptor;
/**
  * @brief  DMAx config struct
  */
typedef struct
{
    enum SDIO_DMA_Select DMAx;
    uint32_t table_size;
    SDIO_SDMA_Descriptor *SDMA_Descriptor;
    SDIO_ADMA1_Descriptor *ADMA1_DescriptorTable;
    SDIO_ADMA2_Descriptor *ADMA2_DescriptorTable;
}SDIO_DMAInitTypedef;
/**
  * @brief  The type of ADMAx Descriptor
  */
enum SDIO_ADMA_Descriptor_Type
{
    ADMA_NOP = 0,
    ADMA_SET = 1,
    ADMA_TRAN = 2,
    ADMA_LINK = 3,
};


/****************************** SDIO Instances ********************************/
#define IS_SDIO_HOST_INSTANCE(INSTANCE) ((INSTANCE) == SDIO_Host)
#define IS_SDIO_SLOT_INSTANCE(INSTANCE) ((INSTANCE) == SDIO_Slot0 || \
                                         (INSTANCE) == SDIO_Slot1 || \
                                         (INSTANCE) == SDIO_Slot2 || \
                                         (INSTANCE) == SDIO_Slot3)

/**
  * @brief  The type of CMD
  */
enum SDIO_II_CMD_Index_Type
{
    SDIO_CMD0 = 0x0U,
    SDIO_CMD1,
    SDIO_CMD2,
    SDIO_CMD3,
    SDIO_CMD4,
    SDIO_CMD5,
    SDIO_CMD6,
    SDIO_CMD7,
    SDIO_CMD8,
    SDIO_CMD9,
    SDIO_CMD10,
    SDIO_CMD11,
    SDIO_CMD12,
    SDIO_CMD13,
    SDIO_CMD14,
    SDIO_CMD15,
    SDIO_CMD16,
    SDIO_CMD17,
    SDIO_CMD18,
    SDIO_CMD19,
    SDIO_CMD20,
    SDIO_CMD21,
    SDIO_CMD22,
    SDIO_CMD23,
    SDIO_CMD24,
    SDIO_CMD25,
    SDIO_CMD26,
    SDIO_CMD27,
    SDIO_CMD28,
    SDIO_CMD29,
    SDIO_CMD30,
    SDIO_CMD31,
    SDIO_CMD32,
    SDIO_CMD33,
    SDIO_CMD34,
    SDIO_CMD35,
    SDIO_CMD36,
    SDIO_CMD37,
    SDIO_CMD38,
    SDIO_CMD39,
    SDIO_CMD40,
    SDIO_CMD41,
    SDIO_CMD42,
    SDIO_CMD43,
    SDIO_CMD44,
    SDIO_CMD45,
    SDIO_CMD46,
    SDIO_CMD47,
    SDIO_CMD48,
    SDIO_CMD49,
    SDIO_CMD50,
    SDIO_CMD51,
    SDIO_CMD52,
    SDIO_CMD53,
    SDIO_CMD54,
    SDIO_CMD55,
    SDIO_CMD56,
    SDIO_CMD57,
    SDIO_CMD58,
    SDIO_CMD59,
    SDIO_CMD60,
    SDIO_CMD61,
    SDIO_CMD62,
    SDIO_CMD63,
};


#define IS_SDIO_CMD_INDEX(INDEX)    (INDEX >= SDIO_CMD0 && INDEX <= SDIO_CMD63)
enum
{
    SDIO_DISABLE = 0x0UL,
    SDIO_ENABLE,
};
#define IS_SDIO_EN(EN)     ((EN) == SDIO_DISABLE || (EN) == SDIO_ENABLE)
enum
{
    SDIO_Slot_Unavailable = 0x0UL,
    SDIO_Slot_Available,
};

/** @defgroup SDIO_LL_Clock_Mode Clock Mode
  *    Clock Generator Select
  */
enum
{
    SDIO_Clock_Mode_10Bit_Divider = 0x0UL,
    SDIO_Clock_Mode_Programmable,
};
#define IS_SDIO_CLOCK_MODE(BYPASS)    (((BYPASS) == SDIO_Clock_Mode_10Bit_Divider) || \
                                       ((BYPASS) == SDIO_Clock_Mode_Programmable))
/** @defgroup SDIO_LL_Speed_Mode Speed Mode
  *
  */
enum
{
    SDIO_Speed_Mode_Full = 0x0UL,
    SDIO_Speed_Mode_High,
};
#define IS_SDIO_SPEED_MODE(MODE) (((MODE) == SDIO_Speed_Mode_Full) || \
                                   ((MODE) == SDIO_Speed_Mode_High))
/**
  * @brief  MMC-8 mode on slot
  */
enum
{
    SDIO_MMC8_Mode_Bits1or4 = 0x0UL,
    SDIO_MMC8_Mode_Bits8,
};
enum
{
    SDIO_Soft_Reset = 0x1UL,
};
/**
  * @brief  Programmable Burst Length
  */
enum
{
    SDIO_Burst_Beat1 = 0x1UL,
    SDIO_Burst_Beats2,
    SDIO_Burst_Beats4,
    SDIO_Burst_Beats8,
    SDIO_Burst_Beats16,
    SDIO_Burst_Beats32,
    SDIO_Burst_Beats64,
    SDIO_Burst_Beats128,
    SDIO_Burst_Beats256,
    SDIO_Burst_Beats512 = 0xAUL,
    SDIO_Burst_Beats1024 = 0xBUL,
    SDIO_Burst_Beats2048 = 0xFUL,
};
/**
  * @brief  CPRM busy
  */
enum
{
   SDIO_CPRM_Ready = 0x0UL,
   SDIO_CPRM_Busy,
};
/**
  * @brief  CPRM clock stable
  *         Card State stable
  *         Internal Clock Stable
  */
enum
{
   SDIO_State_Unstable = 0x0UL,
   SDIO_State_Stable,
};
/**
  * @brief  CPRM halt
  */
enum
{
   SDIO_CPRM_Op_Normal = 0x0UL,
   SDIO_CPRM_Op_Halt,
};
/**
  * @brief  CPRM command
  */
enum CRPM_CMD_Enum
{
   SDIO_NOP = 0x0U,
   SDIO_PROCESS_MKB = 0x1U,
   SDIO_PROCESS_MKB_EXT,
   SDIO_CALC_KMU,
   SDIO_AKE,
   SDIO_SET_BIND_ID,
   SDIO_GEN_KT,
   SDIO_CALC_KT,
   SDIO_GEN_KT_BIND,
   SDIO_CALC_KT_BIND,
   SDIO_DECRYPT_USER,
   SDIO_ENCRYPT_USER,
   SDIO_DECRYPT_PROT,
   SDIO_ENCRYPT_PROT,
};
/**
  * @brief  Host DMA Buffer Boundary
  * @mark：SRS1.HDMABB set 000(SDIO_DMA_Buff_Boundary_4kB)
  *         if SRS0.SDMA_SA input 4kB (not transmit 0x1000Bytes), SDMA will stop transmit, trigger DMA interrupt(SRS12.DMAINT)
  *     eg：SRS0.SDMA_SA set 0x20200F80, SRS0.SDMA_SA increase when DMA transmit, if add to 0x20201000,
  *     Host judge that DMA arrive buffer bound, then stop transmit and trigger interrupt
  */
enum
{
   SDIO_DMA_Buff_Boundary_4kB = 0x0UL,
   SDIO_DMA_Buff_Boundary_8kB,
   SDIO_DMA_Buff_Boundary_16kB,
   SDIO_DMA_Buff_Boundary_32kB,
   SDIO_DMA_Buff_Boundary_64kB,
   SDIO_DMA_Buff_Boundary_128kB,
   SDIO_DMA_Buff_Boundary_256kB,
   SDIO_DMA_Buff_Boundary_512kB,
};
/**
  * @brief  Command Type
  */
enum SDIO_II_CMD_Type_Type
{
   SDIO_CMD_Type_Normal = 0x0U,
   SDIO_CMD_Type_Suspend,
   SDIO_CMD_Type_Resume,
   SDIO_CMD_Type_Abort,
};
#define IS_SDIO_CMD_TYPE(TYPE)  (TYPE == SDIO_CMD_Type_Normal || \
                                 TYPE == SDIO_CMD_Type_Suspend || \
                                 TYPE == SDIO_CMD_Type_Resume || \
                                 TYPE == SDIO_CMD_Type_Abort)
/**
  * @brief  Data Present Select
  */
enum
{
   SDIO_DPS_CMD_Others = 0x0UL,
   SDIO_DPS_TFR_Data,
};
/** @defgroup SDIO_LL_Response_Type Response Type
 * @{
 */
enum
{
   SDIO_Response_No = 0x0UL,
   SDIO_Response_Bit136,
   SDIO_Response_Bit48,
   SDIO_Response_Bit48_Busy,
};
#define IS_SDIO_RESPONSE(RESPONSE) (((RESPONSE) == SDIO_Response_No)      || \
                                    ((RESPONSE) == SDIO_Response_Bit136)  || \
                                    ((RESPONSE) == SDIO_Response_Bit48)   || \
                                    ((RESPONSE) == SDIO_Response_Bit48_Busy))
/** @defgroup SDIO_LL_Data_Block_Select Multi/Single Block Select
  * @{
  */
enum
{
   SDIO_Block_Single = 0x0UL,
   SDIO_Block_Multi,
};
#define IS_SDIO_DATA_BLK_SEL(SEL)  (SEL == SDIO_Block_Single || SEL == SDIO_Block_Multi)
/** @defgroup SDIO_LL_Transfer_Direction Transfer Direction
  * @{
  */
enum
{
   SDIO_Data_TFR_Dir_Write = 0x0UL,
   SDIO_Data_TFR_Dir_Read,
};
#define IS_SDIO_TRANSFER_DIR(DIR) (((DIR) == SDIO_Data_TFR_Dir_Write) || \
                                   ((DIR) == SDIO_Data_TFR_Dir_Read))

/** @defgroup SDMMC_LL_Auto_Cmd_Select Auto CMD Enable
  * @{
  */
enum SDMMC_LL_Auto_Cmd_Select
{
   SDIO_Auto_CMD_No = 0x0UL,
   SDIO_Auto_CMD12,
   SDIO_Auto_CMD23,
};
#define IS_SDIO_AUTO_CMD_SEL(SEL)  (SEL == SDIO_Auto_CMD_No || \
                                    SEL == SDIO_Auto_CMD12  || \
                                    SEL == SDIO_Auto_CMD23)
/**
  * @brief  Line Signal Level
  */
enum
{
   SDIO_Line_Signal_Level_Low = 0x0UL,
   SDIO_Line_Signal_Level_High,
};
/**
  * @brief  Card Inserted
  */
enum
{
   SDIO_Card_No = 0x0UL,
   SDIO_Card_Inserted,
};
/**
  * @brief  Read/Write Transfer Active
  */
enum
{
   SDIO_Data_TFR_No = 0x0UL,
   SDIO_Data_TFR_Ing,
};
/**
  * @brief  DAT Line Active.
  */
enum
{
   SDIO_Data_Line_Released = 0x0UL,
   SDIO_Data_Line_Active,
};
/**
  * @brief  Command Inhibit DAT/CMD
  */
enum
{
   SDIO_Can_Sent = 0x0UL,
   SDIO_Cannot_Sent,
};
/**
  * @brief  SD Bus Voltage Select
  */
enum SDIO_Bus_Vol_SelectType
{
   SDIO_Bus_Vol_1_8V = 0x5UL,
   SDIO_Bus_Vol_3_0V,
   SDIO_Bus_Vol_3_3V,
};
#define IS_BUS_VOLTAGE(VOL)     ((VOL == SDIO_Bus_Vol_1_8V) || \
                                 (VOL == SDIO_Bus_Vol_3_0V) || \
                                 (VOL == SDIO_Bus_Vol_3_3V))
/**
  * @brief  SD Bus Power
  */
enum
{
   SDIO_Bus_Power_Down = 0x0UL,
   SDIO_Bus_Power_On,
};
/**
  * @brief  High Speed Enable
  */
enum
{
   SDIO_Speed_Default = 0x0UL,   //0-25MHz
   SDIO_Speed_High,              //0-50MHz
};
/** @defgroup SDIO_LL_Bus_Wide Bus Width
  * @{ Data Transfer Width
  */
enum SDIO_LL_Bus_Wide
{
   SDIO_BUS_WIDE_1B = 0x0UL,
   SDIO_BUS_WIDE_4B,
   SDIO_BUS_WIDE_8B,
};
#define IS_SDIO_BUS_WIDE(WIDE) (((WIDE) == SDIO_BUS_WIDE_1B) || \
                                ((WIDE) == SDIO_BUS_WIDE_4B) || \
                                ((WIDE) == SDIO_BUS_WIDE_8B))
/**
  * @brief  LED Control
  */
enum
{
   SDIO_LED_ShutDown = 0x0UL,
   SDIO_LED_Open,
};
/**
  * @brief  Driver Strength Select
  */
enum
{
    SDIO_Driver_Type_B = 0x0UL,//default
    SDIO_Driver_Type_A,
    SDIO_Driver_Type_C,
    SDIO_Driver_Type_D,
    SDIO_Driver_Type_Default = SDIO_Driver_Type_B,
};
/**
  * @brief  UHS Mode Select
  */
enum
{
   SDIO_UHS_Mode_SDR12 = 0x0UL,
   SDIO_UHS_Mode_SDR25,
   SDIO_UHS_Mode_SDR50,
   SDIO_UHS_Mode_SDR104,
   SDIO_UHS_Mode_DDR50,
};
/**
  * @brief  Slot Type
  */
enum
{
   SDIO_Slot_Type_RemovableCard = 0x0UL,
   SDIO_Slot_Type_Embedded,
   SDIO_Slot_Type_SharedBus,
};
/**
  * @brief  Max Block Length
  */
enum
{
   SDIO_Block_Bytes_512 = 0x0UL,
   SDIO_Block_Bytes_1024,
   SDIO_Block_Bytes_2048,
};
/**
  * @brief  Timeout Clock Unit
  */
enum
{
   SDIO_Timeout_Clock_Unit_kHz = 0x0UL,
   SDIO_Timeout_Clock_Unit_MHz,
};
/**
  * @brief  Re-Tuning Modes
  */
enum
{
   SDIO_Re_Tuning_Mode1 = 0x0UL,
   SDIO_Re_Tuning_Mode2,
   SDIO_Re_Tuning_Mode3,
};
/**
  * @brief  Re-Tuning Modes
  */
enum HAL_ADMA_Err_State
{
   SDIO_ADMA_State_Stopped = 0x0UL,
   SDIO_ADMA_State_Fetching_Descriptor = 0x1UL,
   SDIO_ADMA_State_Transfer_Data = 0x3UL,
};
/**
  * @brief  BEPC：Back-End Power Control <bit #include <gc9005.h>>
  *         Interrupt Pin Select
  *         BWP：Bus Width Preset <bit #include <gc9005.h>>
  */
enum
{
   SDIO_Device1 = 0x1UL,
   SDIO_Device2,
   SDIO_Device3,
   SDIO_Device4,
   SDIO_Device5,
   SDIO_Device6,
   SDIO_Device7,
};
/**
  * @brief  Number of Interrupt Input Pins
  */
enum
{
   SDIO_Support_INT_A = 0x1UL,
   SDIO_Support_INT_A_B,
   SDIO_Support_INT_A_B_C,
};
/**
  * @brief  Number of Clock Pins
  */
enum
{
   SDIO_Shared_Bus_not_supported = 0x0UL,
   SDIO_Support_sdclk_1 = 0x1UL,
   SDIO_Support_sdclk_2,
   SDIO_Support_sdclk_3,
   SDIO_Support_sdclk_4,
   SDIO_Support_sdclk_5,
   SDIO_Support_sdclk_6,
   SDIO_Support_sdclk_7,
};
/**
  * @brief  Interrupt Signal For Each Slot
  */
enum
{
   SDIO_Int_Signal_Slot0 = 0x1UL,
   SDIO_Int_Signal_Slot1 = 0x2UL,
   SDIO_Int_Signal_Slot2 = 0x4UL,
   SDIO_Int_Signal_Slot3 = 0x8UL,
};

/** @defgroup SDIO_LL_Data_Block_Size  Data Block Size
  * @{
  */
enum
{
    SDIO_DATABLOCK_SIZE_0B = 0x00000000U,
    SDIO_DATABLOCK_SIZE_1B = 0x1UL,
    SDIO_DATABLOCK_SIZE_2B = 0x2UL,
    SDIO_DATABLOCK_SIZE_4B = 0x4UL,
    SDIO_DATABLOCK_SIZE_8B = 0x8UL,
    SDIO_DATABLOCK_SIZE_16B = 0x10UL,
    SDIO_DATABLOCK_SIZE_32B = 0x20UL,
    SDIO_DATABLOCK_SIZE_64B = 0x40UL,
    SDIO_DATABLOCK_SIZE_128B = 0x80UL,
    SDIO_DATABLOCK_SIZE_256B = 0x100UL,
    SDIO_DATABLOCK_SIZE_512B = 0x200UL,
    SDIO_DATABLOCK_SIZE_MAX = SDIO_DATABLOCK_SIZE_512B,
};
#define IS_SDIO_BLOCK_SIZE(SIZE) (((SIZE) > SDIO_DATABLOCK_SIZE_0B)    || \
                                  ((SIZE) <= SDIO_DATABLOCK_SIZE_MAX))

/* Exported constants --------------------------------------------------------*/
/** @defgroup SDMMC_LL_Exported_Constants SDMMC_LL Exported Constants
  * @{
  */
/**
  * @brief  hardware error
  */
#define SDMMC_ERROR_HW_HARDWARE                 0x00000001U   /*!< Hardware Error         */
#define SDMMC_ERROR_HW_CMD_TIMEOUT              0x00000003U   /*!< Command Timeout Error  */
#define SDMMC_ERROR_HW_CMD_CRC                  0x00000005U   /*!< Command CRC Error      */
#define SDMMC_ERROR_HW_CMD_END_BIT              0x00000009U   /*!< Command End Bit Error  */
#define SDMMC_ERROR_HW_CMD_Index                0x00000011U   /*!< Command Index Error    */
#define SDMMC_ERROR_HW_DATA_TIMEOUT             0x00000021U   /*!< Data Timeout Error     */
#define SDMMC_ERROR_HW_DATA_CRC                 0x00000041U   /*!< Data CRC Error         */
#define SDMMC_ERROR_HW_DATA_END_BIT             0x00000081U   /*!< Data End Bit Error     */
#define SDMMC_ERROR_HW_Current_Limit            0x00000101U   /*!< Current Limit Error    */
#define SDMMC_ERROR_HW_AUTO_CMD                 0x00000201U   /*!< Auto CMD Error         */
#define SDMMC_ERROR_HW_ADMA                     0x00000401U   /*!< ADMA Error             */
#define SDMMC_ERROR_HW_TUNING                   0x00000801U   /*!< Tuning Error           */
#define SDMMC_ERROR_HW_OTHER_DMA_NOT_SUP        0x00001001U   /*!< Other DMA Not Support      */
/**
  * @brief  soft error
  */
#define SDMMC_ERROR_NONE                     0x00000000U   /*!< No error                                                      */
#define SDMMC_ERROR_HARDWARE                 0x00000001U   /*!< Hardware Error, See HardwareErrCode for details.              */
#define SDMMC_ERROR_DATA_CRC_FAIL            0x00000002U   /*!< Data block sent/received (CRC check failed)                   */
#define SDMMC_ERROR_CMD_RSP_TIMEOUT          0x00000004U   /*!< Command response timeout                                      */
#define SDMMC_ERROR_DATA_TIMEOUT             0x00000008U   /*!< Data timeout                                                  */
#define SDMMC_ERROR_TX_UNDERRUN              0x00000010U   /*!< Transmit FIFO underrun                                        */
#define SDMMC_ERROR_RX_OVERRUN               0x00000020U   /*!< Receive FIFO overrun                                          */
#define SDMMC_ERROR_ADDR_MISALIGNED          0x00000040U   /*!< Misaligned address                                            */
#define SDMMC_ERROR_BLOCK_LEN_ERR            0x00000080U   /*!< Transferred block length is not allowed for the card or the 
                                                                 number of transferred bytes does not match the block length   */
#define SDMMC_ERROR_ERASE_SEQ_ERR            0x00000100U   /*!< An error in the sequence of erase command occurs              */
#define SDMMC_ERROR_BAD_ERASE_PARAM          0x00000200U   /*!< An invalid selection for erase groups                         */
#define SDMMC_ERROR_WRITE_PROT_VIOLATION     0x00000400U   /*!< Attempt to program a write protect block                      */
#define SDMMC_ERROR_LOCK_UNLOCK_FAILED       0x00000800U   /*!< Sequence or password error has been detected in unlock 
                                                                command or if there was an attempt to access a locked card    */
#define SDMMC_ERROR_COM_CRC_FAILED           0x00001000U   /*!< CRC check of the previous command failed                      */
#define SDMMC_ERROR_ILLEGAL_CMD              0x00002000U   /*!< Command is not legal for the card state                       */
#define SDMMC_ERROR_CARD_ECC_FAILED          0x00004000U   /*!< Card internal ECC was applied but failed to correct the data  */
#define SDMMC_ERROR_CC_ERR                   0x00008000U   /*!< Internal card controller error                                */
#define SDMMC_ERROR_GENERAL_UNKNOWN_ERR      0x00010000U   /*!< General or unknown error                                      */
#define SDMMC_ERROR_STREAM_READ_UNDERRUN     0x00020000U   /*!< The card could not sustain data reading in stream rmode       */
#define SDMMC_ERROR_STREAM_WRITE_OVERRUN     0x00040000U   /*!< The card could not sustain data programming in stream mode    */
#define SDMMC_ERROR_CID_CSD_OVERWRITE        0x00080000U   /*!< CID/CSD overwrite error                                       */
#define SDMMC_ERROR_WP_ERASE_SKIP            0x00100000U   /*!< Only partial address space was erased                         */
#define SDMMC_ERROR_CARD_ECC_DISABLED        0x00200000U   /*!< Command has been executed without using internal ECC          */
#define SDMMC_ERROR_ERASE_RESET              0x00400000U   /*!< Erase sequence was cleared before executing because an out 
                                                                of erase sequence command was received                        */
#define SDMMC_ERROR_AKE_SEQ_ERR              0x00800000U   /*!< Error in sequence of authentication                           */
#define SDMMC_ERROR_INVALID_VOLTRANGE        0x01000000U   /*!< Error in case of invalid voltage range                        */
#define SDMMC_ERROR_ADDR_OUT_OF_RANGE        0x02000000U   /*!< Error when addressed block is out of range                    */
#define SDMMC_ERROR_REQUEST_NOT_APPLICABLE   0x04000000U   /*!< Error when command request is not applicable                  */
#define SDMMC_ERROR_INVALID_PARAMETER        0x08000000U   /*!< the used parameter is not valid                               */
#define SDMMC_ERROR_UNSUPPORTED_FEATURE      0x10000000U   /*!< Error when feature is not insupported                         */
#define SDMMC_ERROR_BUSY                     0x20000000U   /*!< Error when transfer process is busy                           */
#define SDMMC_ERROR_DMA                      0x40000000U   /*!< Error while DMA transfer                                      */
#define SDMMC_ERROR_TIMEOUT                  0x80000000U   /*!< Timeout error                                                 */


/** 
  * @brief SDMMC Commands Index 
  */
#define SDMMC_CMD_GO_IDLE_STATE                       ((uint8_t)0)   /*!< Resets the SD memory card.                                                               */
#define SDMMC_CMD_SEND_OP_COND                        ((uint8_t)1)   /*!< Sends host capacity support information and activates the card's initialization process. */
#define SDMMC_CMD_ALL_SEND_CID                        ((uint8_t)2)   /*!< Asks any card connected to the host to send the CID numbers on the CMD line.             */
#define SDMMC_CMD_SET_REL_ADDR                        ((uint8_t)3)   /*!< Asks the card to publish a new relative address (RCA).                                   */
#define SDMMC_CMD_SET_DSR                             ((uint8_t)4)   /*!< Programs the DSR of all cards.                                                           */
#define SDMMC_CMD_SDMMC_SEN_OP_COND                   ((uint8_t)5)   /*!< Sends host capacity support information (HCS) and asks the accessed card to send its 
                                                                       operating condition register (OCR) content in the response on the CMD line.                  */
#define SDMMC_CMD_HS_SWITCH                           ((uint8_t)6)   /*!< Checks switchable function (mode 0) and switch card function (mode 1).                   */
#define SDMMC_CMD_SEL_DESEL_CARD                      ((uint8_t)7)   /*!< Selects the card by its own relative address and gets deselected by any other address    */
#define SDMMC_CMD_HS_SEND_EXT_CSD                     ((uint8_t)8)   /*!< Sends SD Memory Card interface condition, which includes host supply voltage information 
                                                                       and asks the card whether card supports voltage.                                             */
#define SDMMC_CMD_SEND_CSD                            ((uint8_t)9)   /*!< Addressed card sends its card specific data (CSD) on the CMD line.                       */
#define SDMMC_CMD_SEND_CID                            ((uint8_t)10)  /*!< Addressed card sends its card identification (CID) on the CMD line.                      */
#define SDMMC_CMD_READ_DAT_UNTIL_STOP                 ((uint8_t)11)  /*!< SD card doesn't support it.                                                              */
#define SDMMC_CMD_STOP_TRANSMISSION                   ((uint8_t)12)  /*!< Forces the card to stop transmission.                                                    */
#define SDMMC_CMD_SEND_STATUS                         ((uint8_t)13)  /*!< Addressed card sends its status register.                                                */
#define SDMMC_CMD_HS_BUSTEST_READ                     ((uint8_t)14)  /*!< Reserved                                                                                 */
#define SDMMC_CMD_GO_INACTIVE_STATE                   ((uint8_t)15)  /*!< Sends an addressed card into the inactive state.                                         */
#define SDMMC_CMD_SET_BLOCKLEN                        ((uint8_t)16)  /*!< Sets the block length (in bytes for SDSC) for all following block commands 
                                                                           (read, write, lock). Default block length is fixed to 512 Bytes. Not effective 
                                                                           for SDHS and SDXC.                                                                       */
#define SDMMC_CMD_READ_SINGLE_BLOCK                   ((uint8_t)17)  /*!< Reads single block of size selected by SET_BLOCKLEN in case of SDSC, and a block of 
                                                                           fixed 512 bytes in case of SDHC and SDXC.                                                */
#define SDMMC_CMD_READ_MULT_BLOCK                     ((uint8_t)18)  /*!< Continuously transfers data blocks from card to host until interrupted by 
                                                                           STOP_TRANSMISSION command.                                                               */
#define SDMMC_CMD_HS_BUSTEST_WRITE                    ((uint8_t)19)  /*!< 64 bytes tuning pattern is sent for SDR50 and SDR104.                                    */
#define SDMMC_CMD_WRITE_DAT_UNTIL_STOP                ((uint8_t)20)  /*!< Speed class control command.                                                             */
#define SDMMC_CMD_SET_BLOCK_COUNT                     ((uint8_t)23)  /*!< Specify block count for CMD18 and CMD25.                                                 */
#define SDMMC_CMD_WRITE_SINGLE_BLOCK                  ((uint8_t)24)  /*!< Writes single block of size selected by SET_BLOCKLEN in case of SDSC, and a block of 
                                                                           fixed 512 bytes in case of SDHC and SDXC.                                                */
#define SDMMC_CMD_WRITE_MULT_BLOCK                    ((uint8_t)25)  /*!< Continuously writes blocks of data until a STOP_TRANSMISSION follows.                    */
#define SDMMC_CMD_PROG_CID                            ((uint8_t)26)  /*!< Reserved for manufacturers.                                                              */
#define SDMMC_CMD_PROG_CSD                            ((uint8_t)27)  /*!< Programming of the programmable bits of the CSD.                                         */
#define SDMMC_CMD_SET_WRITE_PROT                      ((uint8_t)28)  /*!< Sets the write protection bit of the addressed group.                                    */
#define SDMMC_CMD_CLR_WRITE_PROT                      ((uint8_t)29)  /*!< Clears the write protection bit of the addressed group.                                  */
#define SDMMC_CMD_SEND_WRITE_PROT                     ((uint8_t)30)  /*!< Asks the card to send the status of the write protection bits.                           */
#define SDMMC_CMD_SD_ERASE_GRP_START                  ((uint8_t)32)  /*!< Sets the address of the first write block to be erased. (For SD card only).              */
#define SDMMC_CMD_SD_ERASE_GRP_END                    ((uint8_t)33)  /*!< Sets the address of the last write block of the continuous range to be erased.           */
#define SDMMC_CMD_ERASE_GRP_START                     ((uint8_t)35)  /*!< Sets the address of the first write block to be erased. Reserved for each command 
                                                                           system set by switch function command (CMD6).                                            */
#define SDMMC_CMD_ERASE_GRP_END                       ((uint8_t)36)  /*!< Sets the address of the last write block of the continuous range to be erased. 
                                                                           Reserved for each command system set by switch function command (CMD6).                  */
#define SDMMC_CMD_ERASE                               ((uint8_t)38)  /*!< Reserved for SD security applications.                                                   */
#define SDMMC_CMD_FAST_IO                             ((uint8_t)39)  /*!< SD card doesn't support it (Reserved).                                                   */
#define SDMMC_CMD_GO_IRQ_STATE                        ((uint8_t)40)  /*!< SD card doesn't support it (Reserved).                                                   */
#define SDMMC_CMD_LOCK_UNLOCK                         ((uint8_t)42)  /*!< Sets/resets the password or lock/unlock the card. The size of the data block is set by 
                                                                           the SET_BLOCK_LEN command.                                                               */
#define SDMMC_CMD_APP_CMD                             ((uint8_t)55)  /*!< Indicates to the card that the next command is an application specific command rather 
                                                                           than a standard command.                                                                 */
#define SDMMC_CMD_GEN_CMD                             ((uint8_t)56)  /*!< Used either to transfer a data block to the card or to get a data block from the card 
                                                                           for general purpose/application specific commands.                                       */
#define SDMMC_CMD_NO_CMD                              ((uint8_t)64)  /*!< No command                                                                               */ 

/** 
  * @brief Following commands are SD Card Specific commands.
  *        SDMMC_APP_CMD should be sent before sending these commands. 
  */
#define SDMMC_CMD_APP_SD_SET_BUSWIDTH                 ((uint8_t)6)   /*!< (ACMD6) Defines the data bus width to be used for data transfer. The allowed data bus 
                                                                            widths are given in SCR register.                                                       */
#define SDMMC_CMD_SD_APP_STATUS                       ((uint8_t)13)  /*!< (ACMD13) Sends the SD status.                                                            */
#define SDMMC_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22)  /*!< (ACMD22) Sends the number of the written (without errors) write blocks. Responds with 
                                                                           32bit+CRC data block.                                                                    */
#define SDMMC_CMD_SD_APP_OP_COND                      ((uint8_t)41)  /*!< (ACMD41) Sends host capacity support information (HCS) and asks the accessed card to 
                                                                           send its operating condition register (OCR) content in the response on the CMD line.     */
#define SDMMC_CMD_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42)  /*!< (ACMD42) Connect/Disconnect the 50 KOhm pull-up resistor on CD/DAT3 (pin 1) of the card  */
#define SDMMC_CMD_SD_APP_SEND_SCR                     ((uint8_t)51)  /*!< Reads the SD Configuration Register (SCR).                                               */
#define SDMMC_CMD_SDMMC_RW_DIRECT                     ((uint8_t)52)  /*!< For SD I/O card only, reserved for security specification.                               */
#define SDMMC_CMD_SDMMC_RW_EXTENDED                   ((uint8_t)53)  /*!< For SD I/O card only, reserved for security specification.                               */

/** 
  * @brief Following commands are SD Card Specific security commands.
  *        SDMMC_CMD_APP_CMD should be sent before sending these commands. 
  */
#define SDMMC_CMD_SD_APP_GET_MKB                      ((uint8_t)43)
#define SDMMC_CMD_SD_APP_GET_MID                      ((uint8_t)44)
#define SDMMC_CMD_SD_APP_SET_CER_RN1                  ((uint8_t)45)
#define SDMMC_CMD_SD_APP_GET_CER_RN2                  ((uint8_t)46)
#define SDMMC_CMD_SD_APP_SET_CER_RES2                 ((uint8_t)47)
#define SDMMC_CMD_SD_APP_GET_CER_RES1                 ((uint8_t)48)
#define SDMMC_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint8_t)18)
#define SDMMC_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint8_t)25)
#define SDMMC_CMD_SD_APP_SECURE_ERASE                 ((uint8_t)38)
#define SDMMC_CMD_SD_APP_CHANGE_SECURE_AREA           ((uint8_t)49)
#define SDMMC_CMD_SD_APP_SECURE_WRITE_MKB             ((uint8_t)48)

/** 
  * @brief  Masks for errors Card Status R1 (OCR Register) 
  */
#define SDMMC_OCR_ADDR_OUT_OF_RANGE        0x80000000U
#define SDMMC_OCR_ADDR_MISALIGNED          0x40000000U
#define SDMMC_OCR_BLOCK_LEN_ERR            0x20000000U
#define SDMMC_OCR_ERASE_SEQ_ERR            0x10000000U
#define SDMMC_OCR_BAD_ERASE_PARAM          0x08000000U
#define SDMMC_OCR_WRITE_PROT_VIOLATION     0x04000000U
#define SDMMC_OCR_LOCK_UNLOCK_FAILED       0x01000000U
#define SDMMC_OCR_COM_CRC_FAILED           0x00800000U
#define SDMMC_OCR_ILLEGAL_CMD              0x00400000U
#define SDMMC_OCR_CARD_ECC_FAILED          0x00200000U
#define SDMMC_OCR_CC_ERROR                 0x00100000U
#define SDMMC_OCR_GENERAL_UNKNOWN_ERROR    0x00080000U
#define SDMMC_OCR_STREAM_READ_UNDERRUN     0x00040000U
#define SDMMC_OCR_STREAM_WRITE_OVERRUN     0x00020000U
#define SDMMC_OCR_CID_CSD_OVERWRITE        0x00010000U
#define SDMMC_OCR_WP_ERASE_SKIP            0x00008000U
#define SDMMC_OCR_CARD_ECC_DISABLED        0x00004000U
#define SDMMC_OCR_ERASE_RESET              0x00002000U
#define SDMMC_OCR_AKE_SEQ_ERROR            0x00000008U
#define SDMMC_OCR_ERRORBITS                0xFDFFE008U

/** 
  * @brief  Masks for R6 Response 
  */
#define SDMMC_R6_GENERAL_UNKNOWN_ERROR     0x00002000U
#define SDMMC_R6_ILLEGAL_CMD               0x00004000U
#define SDMMC_R6_COM_CRC_FAILED            0x00008000U

#define SDMMC_VOLTAGE_WINDOW_SD            0x80100000U
#define SDMMC_HIGH_CAPACITY                0x40000000U
#define SDMMC_STD_CAPACITY                 0x00000000U
#define SDMMC_CHECK_PATTERN                0x000001AAU

#define SDMMC_MAX_VOLT_TRIAL               0x0000FFFFU
    
#define SDMMC_MAX_TRIAL               0x0000FFFFU
    
#define SDMMC_ALLZERO                      0x00000000U

#define SDMMC_WIDE_BUS_SUPPORT             0x00040000U
#define SDMMC_SINGLE_BUS_SUPPORT           0x00010000U
#define SDMMC_CARD_LOCKED                  0x02000000U

#define SDMMC_DATATIMEOUT                  0xEU//tsdmclk x 2^27

#define SDMMC_0TO7BITS                     0x000000FFU
#define SDMMC_8TO15BITS                    0x0000FF00U
#define SDMMC_16TO23BITS                   0x00FF0000U
#define SDMMC_24TO31BITS                   0xFF000000U
#define SDMMC_MAX_DATA_LENGTH              0x01FFFFFFU

#define SDMMC_HALFFIFO                     0x00000008U
#define SDMMC_HALFFIFOBYTES                0x00000020U

/** 
  * @brief  Command Class supported
  */
#define SDIO_CCCC_ERASE                   0x00000020U

#define SDIO_CMDTIMEOUT                   5000U        /* Command send and response timeout */
#define SDIO_MAXERASETIMEOUT              63000U       /* Max erase Timeout 63 s            */

  
/** @defgroup SDIO_LL_Clock_Division Clock Division
  * @{
  */
#define IS_SDIO_CLKDIV(DIV)   ((DIV) <= 0x3FFU)

/** @defgroup SDIO_LL_Wait_Interrupt_State Wait Interrupt
  * @{
  */
#define SDIO_WAIT_NO                        0x00000000U
#define SDIO_WAIT_IT                        SDIO_CMD_WAITINT 
#define SDIO_WAIT_PEND                      SDIO_CMD_WAITPEND

#define IS_SDIO_WAIT(WAIT) (((WAIT) == SDIO_WAIT_NO) || \
                             ((WAIT) == SDIO_WAIT_IT) || \
                             ((WAIT) == SDIO_WAIT_PEND))


/** @defgroup SDIO_LL_Response_Registers Response Register
  * @{
  */
#define SDIO_RESP0                          0x00000000U
#define SDIO_RESP1                          0x00000004U
#define SDIO_RESP2                          0x00000008U
#define SDIO_RESP3                          0x0000000CU

#define IS_SDIO_RESP(RESP) (((RESP) == SDIO_RESP0) || \
                            ((RESP) == SDIO_RESP1) || \
                            ((RESP) == SDIO_RESP2) || \
                            ((RESP) == SDIO_RESP3))

/** @defgroup SDIO_LL_Data_Length Data Lenght
  * @{
  */
#define IS_SDIO_DATA_LENGTH(LENGTH) ((LENGTH) <= 0x01FFFFFFU)

/** @defgroup SDIO_LL_Read_Wait_Mode Read Wait Mode
  * @{
  */
#define SDIO_READ_WAIT_MODE_DATA2                0x00000000U
#define SDIO_READ_WAIT_MODE_CLK                  (SDIO_DCTRL_RWMOD)

#define IS_SDIO_READWAIT_MODE(MODE) (((MODE) == SDIO_READ_WAIT_MODE_CLK) || \
                                     ((MODE) == SDIO_READ_WAIT_MODE_DATA2))

#define SDIO_STATE_AUTO_CMD_ACNE    SD_AutoCMD12_ESR_ACNE           //Auto CMD12 Not Executed
#define SDIO_STATE_AUTO_CMD_ACTE    SD_AutoCMD12_ESR_ACTE           //Auto CMD Timeout Error
#define SDIO_STATE_AUTO_CMD_ACCE    SD_AutoCMD12_ESR_ACCE           //Auto CMD CRC Error
#define SDIO_STATE_AUTO_CMD_ACEBE   SD_AutoCMD12_ESR_ACEBE          //Auto CMD End Bit Error
#define SDIO_STATE_AUTO_CMD_ACIE    SD_AutoCMD12_ESR_ACIE           //Auto CMD Index Error
#define SDIO_STATE_AUTO_CMD_CNIACE  SD_AutoCMD12_ESR_CNIACE         //Command Not Issued By Auto CMD12 Error

#define SDIO_STATE_CICMD            SD_PSR_CICMD                    //Command Inhibit CMD.
#define SDIO_STATE_CIDAT            SD_PSR_CIDAT                    //Command Inhibit DAT.
#define SDIO_STATE_DATLA            SD_PSR_DATLA                    //DAT Line Active.
#define SDIO_STATE_RTNGR            SD_PSR_RTNGR                    //Re-Tuning Request
#define SDIO_STATE_WTA              SD_PSR_WTA                      //Write Transfer Active
#define SDIO_STATE_RTA              SD_PSR_RTA                      //Read Transfer Active
#define SDIO_STATE_BWE              SD_PSR_BWE                      //Buffer Write Enable
#define SDIO_STATE_BRE              SD_PSR_BRE                      //Buffer Read Enable
#define SDIO_STATE_CI               SD_PSR_CI                       //Card Inserted
#define SDIO_STATE_CSS              SD_PSR_CSS                      //Card State Stable
#define SDIO_STATE_CDL              SD_PSR_CDL                      //Card Detect Pin Level
#define SDIO_STATE_WPSL             SD_PSR_WPSL                     //Write Protect Switch Pin Level
#define SDIO_STATE_DATSL0           (0x1UL << (SD_PSR_DATSL_Pos))   //DAT[0] Line Signal Level
#define SDIO_STATE_DATSL1           (0x1UL << (SD_PSR_DATSL_Pos+1)) //DAT[1] Line Signal Level
#define SDIO_STATE_DATSL2           (0x1UL << (SD_PSR_DATSL_Pos+2)) //DAT[2] Line Signal Level
#define SDIO_STATE_DATSL3           (0x1UL << (SD_PSR_DATSL_Pos+3)) //DAT[3] Line Signal Level
#define SDIO_STATE_CMDSL            SD_PSR_CMDSL                    //CMD Line Signal Level

/** @defgroup SDIO_LL_Interrupt_sources_state Interrupt Sources state
  * @{
  */
#define SDIO_IT_STATE_CC                        SD_ISER_CC       //Command Complete
#define SDIO_IT_STATE_TC                        SD_ISER_TC       //Transfer Complete
#define SDIO_IT_STATE_BGE                       SD_ISER_BGE      //Block Gap Event
#define SDIO_IT_STATE_DMAINT                    SD_ISER_DMAINT   //DMA Interrupt
#define SDIO_IT_STATE_BWR                       SD_ISER_BWR      //Buffer Write Ready
#define SDIO_IT_STATE_BRR                       SD_ISER_BRR      //Buffer Read Ready
#define SDIO_IT_STATE_CIN                       SD_ISER_CIN      //Card Insertion
#define SDIO_IT_STATE_CR                        SD_ISER_CR       //Card Removal
#define SDIO_IT_STATE_CINT                      SD_ISER_CINT     //Card Interrupt
#define SDIO_IT_STATE_INT_A                     SD_ISER_INT_A    //Interrupt on line A
#define SDIO_IT_STATE_INT_B                     SD_ISER_INT_B    //Interrupt on line B
#define SDIO_IT_STATE_INT_C                     SD_ISER_INT_C    //Interrupt on line C
#define SDIO_IT_STATE_RTNGE                     SD_ISER_RTNGE    //Re-Tuning Event
#define SDIO_IT_STATE_EINT                      SD_ISER_EINT     //Error Interrupt
#define SDIO_IT_STATE_ECT                       SD_ISER_ECT      //Command Timeout Error
#define SDIO_IT_STATE_ECCRC                     SD_ISER_ECCRC    //Command CRC Error
#define SDIO_IT_STATE_ECEB                      SD_ISER_ECEB     //Command End Bit Error
#define SDIO_IT_STATE_ECI                       SD_ISER_ECI      //Command Index Error
#define SDIO_IT_STATE_EDT                       SD_ISER_EDT      //Data Timeout Error
#define SDIO_IT_STATE_EDCRC                     SD_ISER_EDCRC    //Data CRC Error
#define SDIO_IT_STATE_EDEB                      SD_ISER_EDEB     //Data End Bit Error
#define SDIO_IT_STATE_ECL                       SD_ISER_ECL      //Current Limit Error
#define SDIO_IT_STATE_EAC                       SD_ISER_EAC      //Auto CMD Error
#define SDIO_IT_STATE_EADMA                     SD_ISER_EADMA    //ADMA Error
#define SDIO_IT_STATE_ETNG                      SD_ISER_ETNG     //Tuning Error

/** @defgroup SDIO_LL_Interrupt_sources Interrupt Sources
  * @{
  */
#define SDIO_IT_CC                        SD_ISR_CC       //Command Complete
#define SDIO_IT_TC                        SD_ISR_TC       //Transfer Complete
#define SDIO_IT_BGE                       SD_ISR_BGE      //Block Gap Event
#define SDIO_IT_DMAINT                    SD_ISR_DMAINT   //DMA Interrupt
#define SDIO_IT_BWR                       SD_ISR_BWR      //Buffer Write Ready
#define SDIO_IT_BRR                       SD_ISR_BRR      //Buffer Read Ready
#define SDIO_IT_CIN                       SD_ISR_CIN      //Card Insertion
#define SDIO_IT_CR                        SD_ISR_CR       //Card Removal
#define SDIO_IT_CINT                      SD_ISR_CINT     //Card Interrupt
#define SDIO_IT_INT_A                     SD_ISR_INT_A    //Interrupt on line A
#define SDIO_IT_INT_B                     SD_ISR_INT_B    //Interrupt on line B
#define SDIO_IT_INT_C                     SD_ISR_INT_C    //Interrupt on line C
#define SDIO_IT_RTNGE                     SD_ISR_RTNGE    //Re-Tuning Event
#define SDIO_IT_EINT                      SD_ISR_EINT     //Error Interrupt
#define SDIO_IT_ECT                       SD_ISR_ECT      //Command Timeout Error
#define SDIO_IT_ECCRC                     SD_ISR_ECCRC    //Command CRC Error
#define SDIO_IT_ECEB                      SD_ISR_ECEB     //Command End Bit Error
#define SDIO_IT_ECI                       SD_ISR_ECI      //Command Index Error
#define SDIO_IT_EDT                       SD_ISR_EDT      //Data Timeout Error
#define SDIO_IT_EDCRC                     SD_ISR_EDCRC    //Data CRC Error
#define SDIO_IT_EDEB                      SD_ISR_EDEB     //Data End Bit Error
#define SDIO_IT_ECL                       SD_ISR_ECL      //Current Limit Error
#define SDIO_IT_EAC                       SD_ISR_EAC      //Auto CMD Error
#define SDIO_IT_EADMA                     SD_ISR_EADMA    //ADMA Error
#define SDIO_IT_ETNG                      SD_ISR_ETNG     //Tuning Error

/** @defgroup SDIO_LL_Flags Flags
  * @{
  */
#define SDIO_FLAG_CC                        SD_ISR_CC       //Command Complete
#define SDIO_FLAG_TC                        SD_ISR_TC       //Transfer Complete
#define SDIO_FLAG_BGE                       SD_ISR_BGE      //Block Gap Event
#define SDIO_FLAG_DMAINT                    SD_ISR_DMAINT   //DMA Interrupt
#define SDIO_FLAG_BWR                       SD_ISR_BWR      //Buffer Write Ready
#define SDIO_FLAG_BRR                       SD_ISR_BRR      //Buffer Read Ready
#define SDIO_FLAG_CIN                       SD_ISR_CIN      //Card Insertion
#define SDIO_FLAG_CR                        SD_ISR_CR       //Card Removal
#define SDIO_FLAG_CINT                      SD_ISR_CINT     //Card Interrupt
#define SDIO_FLAG_INT_A                     SD_ISR_INT_A    //Interrupt on line A
#define SDIO_FLAG_INT_B                     SD_ISR_INT_B    //Interrupt on line B
#define SDIO_FLAG_INT_C                     SD_ISR_INT_C    //Interrupt on line C
#define SDIO_FLAG_RTNGE                     SD_ISR_RTNGE    //Re-Tuning Event
#define SDIO_FLAG_EINT                      SD_ISR_EINT     //Error Interrupt
#define SDIO_FLAG_ECT                       SD_ISR_ECT      //Command Timeout Error
#define SDIO_FLAG_ECCRC                     SD_ISR_ECCRC    //Command CRC Error
#define SDIO_FLAG_ECEB                      SD_ISR_ECEB     //Command End Bit Error
#define SDIO_FLAG_ECI                       SD_ISR_ECI      //Command Index Error
#define SDIO_FLAG_EDT                       SD_ISR_EDT      //Data Timeout Error
#define SDIO_FLAG_EDCRC                     SD_ISR_EDCRC    //Data CRC Error
#define SDIO_FLAG_EDEB                      SD_ISR_EDEB     //Data End Bit Error
#define SDIO_FLAG_ECL                       SD_ISR_ECL      //Current Limit Error
#define SDIO_FLAG_EAC                       SD_ISR_EAC      //Auto CMD Error
#define SDIO_FLAG_EADMA                     SD_ISR_EADMA    //ADMA Error
#define SDIO_FLAG_ETNG                      SD_ISR_ETNG     //Tuning Error
#define SDIO_STATIC_FLAGS                   ((uint32_t)(SDIO_FLAG_CC      | SDIO_FLAG_TC     | SDIO_FLAG_ECT |\
                                                        SDIO_FLAG_ECCRC   | SDIO_FLAG_ECEB   | SDIO_FLAG_ECI |\
                                                        SDIO_FLAG_EDT     | SDIO_FLAG_EDCRC  | SDIO_FLAG_EDEB|\
                                                        SDIO_FLAG_ECL     | SDIO_FLAG_EADMA  | SDIO_FLAG_ETNG))
#define SDIO_FLAG_OTHER_ERRORS      (SD_ISR_ECT  | SD_ISR_ECCRC | \
                                     SD_ISR_ECEB | SD_ISR_ECI   | \
                                     SD_ISR_EDT  | SD_ISR_EDCRC | \
                                     SD_ISR_EDEB | SD_ISR_ECL   | \
                                     SD_ISR_EAC  | SD_ISR_EADMA | \
                                     SD_ISR_ETNG)
#define SDIO_FLAG_IT_ERROR          SDIO_FLAG_EINT
#define SDIO_IT_STATE_ALL           0x07FF00FFUL
#define SDIO_IT_STATE_OTHER_ERRORS  SDIO_FLAG_OTHER_ERRORS
#define SDIO_IT_STATE_ERROR         SDIO_FLAG_IT_ERROR
#define SDIO_IT_OTHER_ERRORS        SDIO_FLAG_OTHER_ERRORS
#define SDIO_IT_ERROR               SDIO_FLAG_IT_ERROR


#define SDIO_ADMA_ADMAL             SD_ADMA_ESR_ADMAL   //ADMA Length Mismatch Error
#define SDIO_ADMA_STATE             SD_ADMA_ESR_ADMAES  //ADMA Error State

/** @defgroup SDIO_LL_Register Bits And Addresses Definitions
  * @brief SDIO_LL registers bit address in the alias region
  * @{
  */

/* sdclk = sdmclk; when (N=0)  sdclk= sdmclk/2N; when (N>0) */
/* SDIO Initialization Frequency (400KHz min) */
#define SDIO_INIT_CLK_DIV     ((uint8_t)0x3C)

/* SDIO Data Transfer Frequency (24MHz max) */
#define SDIO_TRANSFER_CLK_DIV ((uint8_t)0x1)

/** @defgroup SDIO_LL_Interrupt_Clock Interrupt And Clock Configuration
 *  @brief macros to handle interrupts and specific clock configurations
 * @{
 */
#define __SDIO_MASTER_CLK_ENABLE(__INSTANCE__) \
do{ \
    (__INSTANCE__)->Slot->HCSR1 |= SD_HCSR1_ICE;\
    while(((__INSTANCE__)->Slot->HCSR1 & SD_HCSR1_ICS) == RESET);\
}while(0)

/**
  * @brief  Internal Clock Disable
  */
#define __SDIO_MASTER_CLK_DISABLE(__INSTANCE__)  ((__INSTANCE__)->Slot->HCSR1 &= ~SD_HCSR1_ICE)
/**
  * @brief  Enable the SDIO device.
  * @param  __INSTANCE__ SDIO Instance  
  * @retval None
  */ 
#define __SDIO_ENABLE(__INSTANCE__)  ((__INSTANCE__)->Slot->HCSR1 |= SD_HCSR1_SDCE)

/**
  * @brief  Disable the SDIO device.
  * @param  __INSTANCE__ SDIO Instance  
  * @retval None
  */
#define __SDIO_DISABLE(__INSTANCE__)   ((__INSTANCE__)->Slot->HCSR1 &= ~SD_HCSR1_SDCE)

/**
  * @brief  Enable the SDIO DMA transfer.
  * @param  __INSTANCE__ SDIO Instance  
  * @retval None
  */ 
#define __SDIO_DMA_ENABLE(__INSTANCE__)
/**
  * @brief  Disable the SDIO DMA transfer.
  * @param  __INSTANCE__ SDIO Instance   
  * @retval None
  */
#define __SDIO_DMA_DISABLE(__INSTANCE__)

/**
  * @brief  Get the SDIO DMAx select.
  * @param  __INSTANCE__ SDIO Instance
  * @retval None
  */
#define __SDIO_GET_DMA_SELECT(__INSTANCE__)  (((__INSTANCE__)->Slot->HCSR0 & SD_HCSR0_DMASEL) >> SD_HCSR0_DMASEL_Pos)
/**
  * @brief  SDIO select DMAx.
  * @param  __INSTANCE__ SDIO Instance
  * @param  __DMAX__ DMAx
  * @retval None
  */
#define __SDIO_DMA_SELECT(__INSTANCE__, __DMAX__)  MODIFY_REG((__INSTANCE__)->Slot->HCSR0, SD_HCSR0_DMASEL_Msk, (__DMAX__) << SD_HCSR0_DMASEL_Pos)
/**
  * @brief  Set the SDIO SDMA Addr.
  * @param  __INSTANCE__ SDIO Instance
  * @param  __ADDR__ SDMA System Address(Read/Write Buffer Address)
  * @retval None
  */
#define __SDIO_SDMA_SET_ADDR(__INSTANCE__, __ADDR__)  ((__INSTANCE__)->Slot->SAR = (__ADDR__))
/**
  * @brief  Set the SDIO ADMA Addr.
  * @param  __INSTANCE__ SDIO Instance
  * @param  __ADDR__ ADMA System Address(Read/Write Buffer Address)
  * @retval None
  */
#define __SDIO_ADMA_SET_ADDR(__INSTANCE__, __ADDR__)   ((__INSTANCE__)->Slot->ADMA_SAR = (__ADDR__))
/**
  * @brief  Set SDIO Host DMA Buffer Boundary
  * @param  __INSTANCE__ SDIO Instance
  * @param  __BOUNDARY__ Host DMA Buffer Boundary
  * @retval None
  */
#define __SDIO_SDMA_SET_BOUNDARY(__INSTANCE__, __BOUNDARY__)  MODIFY_REG((__INSTANCE__)->Slot->BLOCK_CSR, SD_BLOCK_CSR_HDMABB_Msk, ((__BOUNDARY__) << SD_BLOCK_CSR_HDMABB_Pos))
/**
  * @brief  Set SDIO Host SDMA Burst Length
  * @param  __INSTANCE__ SDIO Instance
  * @param  __BURST_LENGTH__ Host DMA Burst Length
  * @retval None
  */
#define __SDIO_DMA_SET_BURST_LENGTH(__INSTANCE__, __BURST_LENGTH__)  MODIFY_REG((__INSTANCE__)->Host->BSR, SD_BSR_PBL_Msk, (__BURST_LENGTH__))
/**
  * @brief  Continue Request
  * @param  __INSTANCE__ SDIO Instance
  * @retval None
  */
#define __SDIO_TRANSFER_CONTINUE_REQUEST(__INSTANCE__) \
do \
{  \
    ((__INSTANCE__)->Slot->HCSR0 &= ~SD_HCSR0_SBGR); \
    ((__INSTANCE__)->Slot->HCSR0 |= SD_HCSR0_CR); \
    ((__INSTANCE__)->Slot->HCSR0 |= SD_HCSR0_SBGR); \
}while(0)
/**
 * @brief  DMA Continue Request
 * @param  __INSTANCE__ SDIO Instance
 * @retval None
 */
#define __SDIO_DMA_CONTINUE_REQUEST(__INSTANCE__)  ((__INSTANCE__)->Slot->SAR = (__INSTANCE__)->Slot->SAR)
/**
  * @brief  Enable the SDIO device interrupt.
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @param  __STATE__  specifies the SDIO ADMA State.
  *         This parameter can be one or a combination of the following values:
  *            @arg SDIO_ADMA_ADMAL:           ADMA Length Mismatch Error
  *            @arg SDIO_ADMA_STATE:           ADMA Error State
  * @retval None
  */
#define __SDIO_DMA_GET_STATE(__INSTANCE__, __STATE__)        ((__INSTANCE__)->Slot->ADMA_ESR & (__STATE__))

/**
  * @brief  Enable the SDIO device interrupt.
  * @param  __INSTANCE__  Pointer to SDIO register base  
  * @param  __INTERRUPT__  specifies the SDIO interrupt sources to be enabled.
  *         This parameter can be one or a combination of the following values:
  *            @arg SDIO_IT_CC:           Command Complete
  *            @arg SDIO_IT_TC :          Transfer Complete
  *            @arg SDIO_IT_BGE:          Block Gap Event
  *            @arg SDIO_IT_DMAINT:       DMA Interrupt
  *            @arg SDIO_IT_BWR:          Buffer Write Ready
  *            @arg SDIO_IT_BRR:          Buffer Read Ready
  *            @arg SDIO_IT_CIN:          Card Insertion
  *            @arg SDIO_IT_CR:           Card Removal
  *            @arg SDIO_IT_CINT:         Card Interrupt
  *            @arg SDIO_IT_INT_A:        Interrupt on line A
  *            @arg SDIO_IT_INT_B:        Interrupt on line B
  *            @arg SDIO_IT_INT_C:        Interrupt on line C
  *            @arg SDIO_IT_RTNGE:        Re-Tuning Event
  *            @arg SDIO_IT_ECT:          Command Timeout Error
  *            @arg SDIO_IT_ECCRC:        Command CRC Error
  *            @arg SDIO_IT_ECEB:         Command End Bit Error
  *            @arg SDIO_IT_ECI:          Command Index Error
  *            @arg SDIO_IT_EDT:          Data Timeout Error
  *            @arg SDIO_IT_EDCRC:        Data CRC Error
  *            @arg SDIO_IT_EDEB:         Data End Bit Error
  *            @arg SDIO_IT_ECL:          Current Limit Error
  *            @arg SDIO_IT_EAC:          Auto CMD Error
  *            @arg SDIO_IT_EADMA:        ADMA Error
  *            @arg SDIO_IT_ETNG:         Tuning Error
  * @retval None
  */
#define __SDIO_ENABLE_IT(__INSTANCE__, __INTERRUPT__)   \
do{                                                     \
    (__INSTANCE__)->Slot->IER |= (__INTERRUPT__);       \
    (__INSTANCE__)->Slot->ISER |= (__INTERRUPT__);      \
}while(0)

/**
  * @brief  Disable the SDIO device interrupt.
  * @param  __INSTANCE__  Pointer to SDIO register base   
  * @param  __INTERRUPT__  specifies the SDIO interrupt sources to be disabled.
  *          This parameter can be one or a combination of the following values:
  *            @arg SDIO_IT_CC:           Command Complete
  *            @arg SDIO_IT_TC :          Transfer Complete
  *            @arg SDIO_IT_BGE:          Block Gap Event
  *            @arg SDIO_IT_DMAINT:       DMA Interrupt
  *            @arg SDIO_IT_BWR:          Buffer Write Ready
  *            @arg SDIO_IT_BRR:          Buffer Read Ready
  *            @arg SDIO_IT_CIN:          Card Insertion
  *            @arg SDIO_IT_CR:           Card Removal
  *            @arg SDIO_IT_CINT:         Card Interrupt
  *            @arg SDIO_IT_INT_A:        Interrupt on line A
  *            @arg SDIO_IT_INT_B:        Interrupt on line B
  *            @arg SDIO_IT_INT_C:        Interrupt on line C
  *            @arg SDIO_IT_RTNGE:        Re-Tuning Event
  *            @arg SDIO_IT_ECT:          Command Timeout Error
  *            @arg SDIO_IT_ECCRC:        Command CRC Error
  *            @arg SDIO_IT_ECEB:         Command End Bit Error
  *            @arg SDIO_IT_ECI:          Command Index Error
  *            @arg SDIO_IT_EDT:          Data Timeout Error
  *            @arg SDIO_IT_EDCRC:        Data CRC Error
  *            @arg SDIO_IT_EDEB:         Data End Bit Error
  *            @arg SDIO_IT_ECL:          Current Limit Error
  *            @arg SDIO_IT_EAC:          Auto CMD Error
  *            @arg SDIO_IT_EADMA:        ADMA Error
  *            @arg SDIO_IT_ETNG:         Tuning Error
  * @retval None
  */
#define __SDIO_DISABLE_IT(__INSTANCE__, __INTERRUPT__)  \
do{                                                     \
    (__INSTANCE__)->Slot->IER &= ~(__INTERRUPT__);      \
}while(0)

/**
  * @brief  Get the SDIO device interrupt Whether or not enable.
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @param  __INTERRUPT__  specifies the SDIO interrupt sources to be getted.
  *          This parameter can be one or a combination of the following values:
  *            @arg SDIO_IT_CC:           Command Complete
  *            @arg SDIO_IT_TC :          Transfer Complete
  *            @arg SDIO_IT_BGE:          Block Gap Event
  *            @arg SDIO_IT_DMAINT:       DMA Interrupt
  *            @arg SDIO_IT_BWR:          Buffer Write Ready
  *            @arg SDIO_IT_BRR:          Buffer Read Ready
  *            @arg SDIO_IT_CIN:          Card Insertion
  *            @arg SDIO_IT_CR:           Card Removal
  *            @arg SDIO_IT_CINT:         Card Interrupt
  *            @arg SDIO_IT_INT_A:        Interrupt on line A
  *            @arg SDIO_IT_INT_B:        Interrupt on line B
  *            @arg SDIO_IT_INT_C:        Interrupt on line C
  *            @arg SDIO_IT_RTNGE:        Re-Tuning Event
  *            @arg SDIO_IT_ECT:          Command Timeout Error
  *            @arg SDIO_IT_ECCRC:        Command CRC Error
  *            @arg SDIO_IT_ECEB:         Command End Bit Error
  *            @arg SDIO_IT_ECI:          Command Index Error
  *            @arg SDIO_IT_EDT:          Data Timeout Error
  *            @arg SDIO_IT_EDCRC:        Data CRC Error
  *            @arg SDIO_IT_EDEB:         Data End Bit Error
  *            @arg SDIO_IT_ECL:          Current Limit Error
  *            @arg SDIO_IT_EAC:          Auto CMD Error
  *            @arg SDIO_IT_EADMA:        ADMA Error
  *            @arg SDIO_IT_ETNG:         Tuning Error
  * @retval None
  */
#define __SDIO_GET_IT_EN(__INSTANCE__, __INTERRUPT__)  (((__INSTANCE__)->Slot->IER & (__INTERRUPT__)) != RESET)

/**
  * @brief  Get the SDIO device interrupt state Whether or not enable.
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @param  __STATE__  specifies the SDIO interrupt state to be getted.
  *          This parameter can be one or a combination of the following values:
  *            @arg SDIO_IT_STATE_CC:           Command Complete
  *            @arg SDIO_IT_STATE_TC :          Transfer Complete
  *            @arg SDIO_IT_STATE_BGE:          Block Gap Event
  *            @arg SDIO_IT_STATE_DMAINT:       DMA Interrupt
  *            @arg SDIO_IT_STATE_BWR:          Buffer Write Ready
  *            @arg SDIO_IT_STATE_BRR:          Buffer Read Ready
  *            @arg SDIO_IT_STATE_CIN:          Card Insertion
  *            @arg SDIO_IT_STATE_CR:           Card Removal
  *            @arg SDIO_IT_STATE_CINT:         Card Interrupt
  *            @arg SDIO_IT_STATE_INT_A:        Interrupt on line A
  *            @arg SDIO_IT_STATE_INT_B:        Interrupt on line B
  *            @arg SDIO_IT_STATE_INT_C:        Interrupt on line C
  *            @arg SDIO_IT_STATE_RTNGE:        Re-Tuning Event
  *            @arg SDIO_IT_STATE_ECT:          Command Timeout Error
  *            @arg SDIO_IT_STATE_ECCRC:        Command CRC Error
  *            @arg SDIO_IT_STATE_ECEB:         Command End Bit Error
  *            @arg SDIO_IT_STATE_ECI:          Command Index Error
  *            @arg SDIO_IT_STATE_EDT:          Data Timeout Error
  *            @arg SDIO_IT_STATE_EDCRC:        Data CRC Error
  *            @arg SDIO_IT_STATE_EDEB:         Data End Bit Error
  *            @arg SDIO_IT_STATE_ECL:          Current Limit Error
  *            @arg SDIO_IT_STATE_EAC:          Auto CMD Error
  *            @arg SDIO_IT_STATE_EADMA:        ADMA Error
  *            @arg SDIO_IT_STATE_ETNG:         Tuning Error
  * @retval None
  */
#define __SDIO_GET_IT_STATE_EN(__INSTANCE__, __STATE__)  (((__INSTANCE__)->Slot->ISER & (__STATE__)) != RESET)
/**
  * @brief  Enable the SDIO device interrupt state Whether or not enable.
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @param  __STATE__  specifies the SDIO interrupt state to be Enabled.
  *          This parameter can be one or a combination of the following values:
  *            @arg SDIO_IT_STATE_CC:           Command Complete
  *            @arg SDIO_IT_STATE_TC :          Transfer Complete
  *            @arg SDIO_IT_STATE_BGE:          Block Gap Event
  *            @arg SDIO_IT_STATE_DMAINT:       DMA Interrupt
  *            @arg SDIO_IT_STATE_BWR:          Buffer Write Ready
  *            @arg SDIO_IT_STATE_BRR:          Buffer Read Ready
  *            @arg SDIO_IT_STATE_CIN:          Card Insertion
  *            @arg SDIO_IT_STATE_CR:           Card Removal
  *            @arg SDIO_IT_STATE_CINT:         Card Interrupt
  *            @arg SDIO_IT_STATE_INT_A:        Interrupt on line A
  *            @arg SDIO_IT_STATE_INT_B:        Interrupt on line B
  *            @arg SDIO_IT_STATE_INT_C:        Interrupt on line C
  *            @arg SDIO_IT_STATE_RTNGE:        Re-Tuning Event
  *            @arg SDIO_IT_STATE_ECT:          Command Timeout Error
  *            @arg SDIO_IT_STATE_ECCRC:        Command CRC Error
  *            @arg SDIO_IT_STATE_ECEB:         Command End Bit Error
  *            @arg SDIO_IT_STATE_ECI:          Command Index Error
  *            @arg SDIO_IT_STATE_EDT:          Data Timeout Error
  *            @arg SDIO_IT_STATE_EDCRC:        Data CRC Error
  *            @arg SDIO_IT_STATE_EDEB:         Data End Bit Error
  *            @arg SDIO_IT_STATE_ECL:          Current Limit Error
  *            @arg SDIO_IT_STATE_EAC:          Auto CMD Error
  *            @arg SDIO_IT_STATE_EADMA:        ADMA Error
  *            @arg SDIO_IT_STATE_ETNG:         Tuning Error
  * @retval None
  */
#define __SDIO_IT_STATE_ENABLE(__INSTANCE__, __STATE__)  ((__INSTANCE__)->Slot->ISER |= (__STATE__))
/**
  * @brief  Disable the SDIO device interrupt state Whether or not enable.
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @param  __STATE__  specifies the SDIO interrupt state to be Disable.
  *          This parameter can be one or a combination of the following values:
  *            @arg SDIO_IT_STATE_CC:           Command Complete
  *            @arg SDIO_IT_STATE_TC :          Transfer Complete
  *            @arg SDIO_IT_STATE_BGE:          Block Gap Event
  *            @arg SDIO_IT_STATE_DMAINT:       DMA Interrupt
  *            @arg SDIO_IT_STATE_BWR:          Buffer Write Ready
  *            @arg SDIO_IT_STATE_BRR:          Buffer Read Ready
  *            @arg SDIO_IT_STATE_CIN:          Card Insertion
  *            @arg SDIO_IT_STATE_CR:           Card Removal
  *            @arg SDIO_IT_STATE_CINT:         Card Interrupt
  *            @arg SDIO_IT_STATE_INT_A:        Interrupt on line A
  *            @arg SDIO_IT_STATE_INT_B:        Interrupt on line B
  *            @arg SDIO_IT_STATE_INT_C:        Interrupt on line C
  *            @arg SDIO_IT_STATE_RTNGE:        Re-Tuning Event
  *            @arg SDIO_IT_STATE_ECT:          Command Timeout Error
  *            @arg SDIO_IT_STATE_ECCRC:        Command CRC Error
  *            @arg SDIO_IT_STATE_ECEB:         Command End Bit Error
  *            @arg SDIO_IT_STATE_ECI:          Command Index Error
  *            @arg SDIO_IT_STATE_EDT:          Data Timeout Error
  *            @arg SDIO_IT_STATE_EDCRC:        Data CRC Error
  *            @arg SDIO_IT_STATE_EDEB:         Data End Bit Error
  *            @arg SDIO_IT_STATE_ECL:          Current Limit Error
  *            @arg SDIO_IT_STATE_EAC:          Auto CMD Error
  *            @arg SDIO_IT_STATE_EADMA:        ADMA Error
  *            @arg SDIO_IT_STATE_ETNG:         Tuning Error
  * @retval None
  */
#define __SDIO_IT_STATE_DISABLE(__INSTANCE__, __STATE__)  ((__INSTANCE__)->Slot->ISER &= ~(__STATE__))
/**
  * @brief  Checks whether the specified SDIO state is set or not.
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @param  __STATE__ specifies the state to check.
  *          This parameter can be one of the following values:
  *            @arg SDIO_STATE_CICMD:         Command Inhibit CMD.
  *            @arg SDIO_STATE_CIDAT:         Command Inhibit DAT.
  *            @arg SDIO_STATE_DATLA:         DAT Line Active.
  *            @arg SDIO_STATE_RTNGR:         Re-Tuning Request
  *            @arg SDIO_STATE_WTA:           Write Transfer Active
  *            @arg SDIO_STATE_RTA:           Read Transfer Active
  *            @arg SDIO_STATE_BWE:           Buffer Write Enable
  *            @arg SDIO_STATE_BRE:           Buffer Read Enable
  *            @arg SDIO_STATE_CI:            Card Inserted
  *            @arg SDIO_STATE_CSS:           Card State Stable
  *            @arg SDIO_STATE_CDL:           Card Detect Pin Level
  *            @arg SDIO_STATE_WPSL:          Write Protect Switch Pin Level
  *            @arg SDIO_STATE_DATSL0:        DAT[0] Line Signal Level
  *            @arg SDIO_STATE_DATSL1:        DAT[1] Line Signal Level
  *            @arg SDIO_STATE_DATSL2:        DAT[2] Line Signal Level
  *            @arg SDIO_STATE_DATSL3:        DAT[3] Line Signal Level
  *            @arg SDIO_STATE_CMDSL:         CMD Line Signal Level
  * @retval The new state of SDIO_FLAG (SET or RESET).
  */
#define __SDIO_GET_STATE(__INSTANCE__, __STATE__)  (((__INSTANCE__)->Slot->PSR & (__STATE__)) != RESET)
/**
  * @brief  Checks whether the specified SDIO flag is set or not. 
  * @param  __INSTANCE__  Pointer to SDIO register base   
  * @param  __FLAG__ specifies the flag to check. 
  *          This parameter can be one of the following values:
  *            @arg SDIO_FLAG_CC:           Command Complete
  *            @arg SDIO_FLAG_TC :          Transfer Complete
  *            @arg SDIO_FLAG_BGE:          Block Gap Event
  *            @arg SDIO_FLAG_DMAINT:       DMA Interrupt
  *            @arg SDIO_FLAG_BWR:          Buffer Write Ready
  *            @arg SDIO_FLAG_BRR:          Buffer Read Ready
  *            @arg SDIO_FLAG_CIN:          Card Insertion
  *            @arg SDIO_FLAG_CR:           Card Removal
  *            @arg SDIO_FLAG_CINT:         Card Interrupt
  *            @arg SDIO_FLAG_INT_A:        Interrupt on line A
  *            @arg SDIO_FLAG_INT_B:        Interrupt on line B
  *            @arg SDIO_FLAG_INT_C:        Interrupt on line C
  *            @arg SDIO_FLAG_RTNGE:        Re-Tuning Event
  *            @arg SDIO_FLAG_EINT          Error Interrupt
  *            @arg SDIO_FLAG_ECT:          Command Timeout Error
  *            @arg SDIO_FLAG_ECCRC:        Command CRC Error
  *            @arg SDIO_FLAG_ECEB:         Command End Bit Error
  *            @arg SDIO_FLAG_ECI:          Command Index Error
  *            @arg SDIO_FLAG_EDT:          Data Timeout Error
  *            @arg SDIO_FLAG_EDCRC:        Data CRC Error
  *            @arg SDIO_FLAG_EDEB:         Data End Bit Error
  *            @arg SDIO_FLAG_ECL:          Current Limit Error
  *            @arg SDIO_FLAG_EAC:          Auto CMD Error
  *            @arg SDIO_FLAG_EADMA:        ADMA Error
  *            @arg SDIO_FLAG_ETNG:         Tuning Error
  * @retval The new state of SDIO_FLAG (SET or RESET).
  */
#define __SDIO_GET_FLAG(__INSTANCE__, __FLAG__)  (((__INSTANCE__)->Slot->ISR & (__FLAG__)) != RESET)

/**
  * @brief  Clears the SDIO pending flags.
  * @param  __INSTANCE__  Pointer to SDIO register base  
  * @param  __FLAG__ specifies the flag to clear.  
  *          This parameter can be one or a combination of the following values:
  *            @arg SDIO_FLAG_CC:           Command Complete
  *            @arg SDIO_FLAG_TC :          Transfer Complete
  *            @arg SDIO_FLAG_BGE:          Block Gap Event
  *            @arg SDIO_FLAG_DMAINT:       DMA Interrupt
  *            @arg SDIO_FLAG_BWR:          Buffer Write Ready
  *            @arg SDIO_FLAG_BRR:          Buffer Read Ready
  *            @arg SDIO_FLAG_CIN:          Card Insertion
  *            @arg SDIO_FLAG_CR:           Card Removal
  *            @arg SDIO_FLAG_ECT:          Command Timeout Error
  *            @arg SDIO_FLAG_ECCRC:        Command CRC Error
  *            @arg SDIO_FLAG_ECEB:         Command End Bit Error
  *            @arg SDIO_FLAG_ECI:          Command Index Error
  *            @arg SDIO_FLAG_EDT:          Data Timeout Error
  *            @arg SDIO_FLAG_EDCRC:        Data CRC Error
  *            @arg SDIO_FLAG_EDEB:         Data End Bit Error
  *            @arg SDIO_FLAG_ECL:          Current Limit Error
  *            @arg SDIO_FLAG_EAC:          Auto CMD Error
  *            @arg SDIO_FLAG_EADMA:        ADMA Error
  *            @arg SDIO_FLAG_ETNG:         Tuning Error
  * @retval None
  */
#define __SDIO_CLEAR_FLAG(__INSTANCE__, __FLAG__)  ((__INSTANCE__)->Slot->ISR = (__FLAG__))

/**
  * @brief  Checks whether the specified SDIO interrupt has occurred or not.
  * @param  __INSTANCE__  Pointer to SDIO register base   
  * @param  __INTERRUPT__ specifies the SDIO interrupt source to check. 
  *          This parameter can be one of the following values:
  *            @arg SDIO_IT_CC:           Command Complete
  *            @arg SDIO_IT_TC :          Transfer Complete
  *            @arg SDIO_IT_BGE:          Block Gap Event
  *            @arg SDIO_IT_DMAINT:       DMA Interrupt
  *            @arg SDIO_IT_BWR:          Buffer Write Ready
  *            @arg SDIO_IT_BRR:          Buffer Read Ready
  *            @arg SDIO_IT_CIN:          Card Insertion
  *            @arg SDIO_IT_CR:           Card Removal
  *            @arg SDIO_IT_ECT:          Command Timeout Error
  *            @arg SDIO_IT_ECCRC:        Command CRC Error
  *            @arg SDIO_IT_ECEB:         Command End Bit Error
  *            @arg SDIO_IT_ECI:          Command Index Error
  *            @arg SDIO_IT_EDT:          Data Timeout Error
  *            @arg SDIO_IT_EDCRC:        Data CRC Error
  *            @arg SDIO_IT_EDEB:         Data End Bit Error
  *            @arg SDIO_IT_ECL:          Current Limit Error
  *            @arg SDIO_IT_EAC:          Auto CMD Error
  *            @arg SDIO_IT_EADMA:        ADMA Error
  *            @arg SDIO_IT_ETNG:         Tuning Error
  * @retval The new state of SDIO_IT (SET or RESET).
  */
#define __SDIO_GET_IT(__INSTANCE__, __INTERRUPT__)  (((__INSTANCE__)->Slot->ISR & (__INTERRUPT__)) != RESET)

/**
  * @brief  Clears the SDIO's interrupt pending bits.
  * @param  __INSTANCE__  Pointer to SDIO register base 
  * @param  __INTERRUPT__ specifies the interrupt pending bit to clear. 
  *          This parameter can be one or a combination of the following values:
  *            @arg SDIO_IT_CC:           Command Complete
  *            @arg SDIO_IT_TC :          Transfer Complete
  *            @arg SDIO_IT_BGE:          Block Gap Event
  *            @arg SDIO_IT_DMAINT:       DMA Interrupt
  *            @arg SDIO_IT_BWR:          Buffer Write Ready
  *            @arg SDIO_IT_BRR:          Buffer Read Ready
  *            @arg SDIO_IT_CIN:          Card Insertion
  *            @arg SDIO_IT_CR:           Card Removal
  *            @arg SDIO_IT_ECT:          Command Timeout Error
  *            @arg SDIO_IT_ECCRC:        Command CRC Error
  *            @arg SDIO_IT_ECEB:         Command End Bit Error
  *            @arg SDIO_IT_ECI:          Command Index Error
  *            @arg SDIO_IT_EDT:          Data Timeout Error
  *            @arg SDIO_IT_EDCRC:        Data CRC Error
  *            @arg SDIO_IT_EDEB:         Data End Bit Error
  *            @arg SDIO_IT_ECL:          Current Limit Error
  *            @arg SDIO_IT_EAC:          Auto CMD Error
  *            @arg SDIO_IT_EADMA:        ADMA Error
  *            @arg SDIO_IT_ETNG:         Tuning Error
  * @retval None
  */
#define __SDIO_CLEAR_IT(__INSTANCE__, __INTERRUPT__)  ((__INSTANCE__)->Slot->ISR = (__INTERRUPT__))

/**
  * @brief  Set the SDIO Blocks Count For Current Transfer(SRS1.BCCT).
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @param  __COUNT__ specifies the interrupt pending bit to clear.
  *          This parameter can be one or a combination of the following values:0~0xFFFF
  * @retval None
  */
#define __SDIO_SET_BLOCK_COUNT(__INSTANCE__, __COUNT__)  MODIFY_REG((__INSTANCE__)->Slot->BLOCK_CSR, SD_BLOCK_CSR_BCCT_Msk, (__COUNT__) << SD_BLOCK_CSR_BCCT_Pos)

/**
  * @brief  Get the SDIO Blocks Count For Current Transfer(SRS1.BCCT).
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @retval Blocks Count For Current Transfer
  */
#define __SDIO_Get_BLOCK_COUNT(__INSTANCE__)  (((__INSTANCE__)->Slot->BLOCK_CSR & SD_BLOCK_CSR_BCCT) >> SD_BLOCK_CSR_BCCT_Pos)

/**
  * @brief  Get the SDIO Blocks Count For Current Transfer(SRS1.BCCT).
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @param  __STATE__  Led ctrl
  * @retval void
  */
#define __SDIO_LED_CTRL(__INSTANCE__, __STATE__)  MODIFY_REG((__INSTANCE__)->Slot->HCSR0, SD_HCSR0_LEDC_Msk, (__STATE__) << SD_HCSR0_LEDC_Pos)


/**
  * @brief  Enable Read Wait.
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @retval None
  */
#define __SDIO_READ_WAIT_ENABLE(__INSTANCE__)       ((__INSTANCE__)->Slot->HCSR0 |= SD_HCSR0_RWC)
/**
  * @brief  Disable Read Wait.
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @retval None
  */
#define __SDIO_READ_WAIT_DISABLE(__INSTANCE__)       ((__INSTANCE__)->Slot->HCSR0 &= ~SD_HCSR0_RWC)

/**
  * @brief  Reset Data Line.
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @retval None
  */
#define __SDIO_SOFT_RESET_DAT(__INSTANCE__)  \
do{ \
    (__INSTANCE__)->Slot->HCSR1 |= SD_HCSR1_SRDAT;\
    while(((__INSTANCE__)->Slot->HCSR1 & SD_HCSR1_SRDAT) != RESET);\
}while(0)

/**
  * @brief  Reset CMD Line.
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @retval None
  */
#define __SDIO_SOFT_RESET_CMD(__INSTANCE__)  \
do{ \
    (__INSTANCE__)->Slot->HCSR1 |= SD_HCSR1_SRCMD;\
    while(((__INSTANCE__)->Slot->HCSR1 & SD_HCSR1_SRCMD) != RESET);\
}while(0)

/**
  * @brief  Reset All (CMD and Data Line).
  * @param  __INSTANCE__  Pointer to SDIO register base
  * @retval None
  */
#define __SDIO_SOFT_RESET_ALL(__INSTANCE__)  \
do{ \
    (__INSTANCE__)->Slot->HCSR1 |= SD_HCSR1_SRFA;\
    while(((__INSTANCE__)->Slot->HCSR1 & SD_HCSR1_SRFA) != RESET);\
}while(0)


/* Exported functions --------------------------------------------------------*/
/** @addtogroup SDMMC_LL_Exported_Functions
  * @{
  */
  
/* Initialization/de-initialization functions  **********************************/
/** @addtogroup HAL_SDMMC_LL_Group1
  * @{
  */
void SDIO_MspInit(SDIO_TypeDef *SDIOx);

void SDIO_ResetDATLine(SDIO_TypeDef *SDIOx);
void SDIO_ResetCMDLine(SDIO_TypeDef *SDIOx);
void SDIO_ResetALL(SDIO_TypeDef *SDIOx);
HAL_StatusTypeDef SDIO_HostReset(SDIO_HostTypeDef *Host);
HAL_StatusTypeDef SDIO_SlotReset(SDIO_TypeDef *SDIOx);
void SDIO_Host_Struct_Init(SDIO_HostInitTypeDef *init);
HAL_StatusTypeDef SDIO_HostInit(SDIO_HostTypeDef *Host, SDIO_HostInitTypeDef init);
HAL_StatusTypeDef SDIO_Init(SDIO_TypeDef *SDIOx, SDIO_InitTypeDef Init);
/**
  * @}
  */
  
/* I/O operation functions  *****************************************************/
/** @addtogroup HAL_SDMMC_LL_Group2
  * @{
  */
HAL_StatusTypeDef SDIO_ReadFIFO(SDIO_TypeDef *SDIOx, uint32_t *pReadData);
HAL_StatusTypeDef SDIO_WriteFIFO(SDIO_TypeDef *SDIOx, uint32_t *pWriteData);

/**
  * @}
  */
  
/* Peripheral Control functions  ************************************************/
/** @addtogroup HAL_SDMMC_LL_Group3
  * @{
  */
//HAL_StatusTypeDef SDIO_PowerState_UP(SDIO_TypeDef *SDIOx);
#define SDIO_PowerState_UP  SDIO_PowerState_ON
HAL_StatusTypeDef SDIO_PowerState_ON(SDIO_TypeDef *SDIOx);
HAL_StatusTypeDef SDIO_PowerState_OFF(SDIO_TypeDef *SDIOx);
uint32_t          SDIO_GetPowerState(SDIO_TypeDef *SDIOx);

/* Command path state machine (CPSM) management functions */
HAL_StatusTypeDef SDIO_SendCommand(SDIO_TypeDef *SDIOx, SDIO_CmdInitTypeDef *Command);
uint8_t           SDIO_GetCommandResponse(SDIO_TypeDef *SDIOx);
/**
  * @brief  Return the response received from the card for the last command
  * @param  SDIOx Pointer to SDMMC register base
  * @param  Response Specifies the SDMMC response register.
  *          This parameter can be one of the following values:
  *            @arg SDIO_RESP0: Response Register 1
  *            @arg SDIO_RESP0: Response Register 2
  *            @arg SDIO_RESP0: Response Register 3
  *            @arg SDIO_RESP0: Response Register 4
  * @retval The Corresponding response register value
  */
static inline uint32_t SDIO_GetResponse(SDIO_TypeDef *SDIOx, uint32_t Response)
{
    return (*(__IO uint32_t *)((uint32_t)&(SDIOx->Slot->RESP0) + Response));
}
void SDIO_GetResponse136(SDIO_TypeDef *SDIOx, uint32_t csd[]);

/* Data path state machine (DPSM) management functions */
HAL_StatusTypeDef SDIO_ConfigData(SDIO_TypeDef *SDIOx, SDIO_DataInitTypeDef* Data);
uint32_t          SDIO_GetDataCounter(SDIO_TypeDef *SDIOx);

/* SDMMC Cards mode management functions */
HAL_StatusTypeDef SDIO_SetSDMMCReadWaitMode(SDIO_TypeDef *SDIOx, uint32_t SDIO_ReadWaitMode);

/* SDMMC Commands management functions */
uint32_t SDMMC_CmdBlockCount(SDIO_TypeDef *SDIOx, uint32_t BlockCount);
uint32_t SDMMC_CmdBlockLength(SDIO_TypeDef *SDIOx, uint32_t BlockSize);
uint32_t SDMMC_CmdReadSingleBlock(SDIO_TypeDef *SDIOx, uint32_t ReadAdd);
uint32_t SDMMC_CmdReadSingleBlockDMA(SDIO_TypeDef *SDIOx, uint32_t ReadAdd);
uint32_t SDMMC_CmdReadMultiBlock(SDIO_TypeDef *SDIOx, uint32_t ReadAdd);
uint32_t SDMMC_CmdReadMultiBlockDMA(SDIO_TypeDef *SDIOx, uint32_t ReadAdd);
uint32_t SDMMC_CmdWriteSingleBlock(SDIO_TypeDef *SDIOx, uint32_t WriteAdd);
uint32_t SDMMC_CmdWriteSingleBlockDMA(SDIO_TypeDef *SDIOx, uint32_t WriteAdd);

#define USE_AUTO_CMD12  1
uint32_t SDMMC_CmdWriteMultiBlock(SDIO_TypeDef *SDIOx, uint32_t WriteAdd);
uint32_t SDMMC_CmdWriteMultiBlockDMA(SDIO_TypeDef *SDIOx, uint32_t WriteAdd);
uint32_t SDMMC_CmdSDEraseStartAdd(SDIO_TypeDef *SDIOx, uint32_t StartAdd);
uint32_t SDMMC_CmdSDEraseEndAdd(SDIO_TypeDef *SDIOx, uint32_t EndAdd);
uint32_t SDMMC_CmdErase(SDIO_TypeDef *SDIOx);
uint32_t SDMMC_CmdStopTransfer(SDIO_TypeDef *SDIOx);
uint32_t SDMMC_CmdSelDesel(SDIO_TypeDef *SDIOx, uint64_t Addr);
uint32_t SDMMC_CmdGoIdleState(SDIO_TypeDef *SDIOx);
uint32_t SDMMC_CmdOperCond(SDIO_TypeDef *SDIOx);
uint32_t SDMMC_CmdAppCommand(SDIO_TypeDef *SDIOx, uint32_t Argument);
uint32_t SDMMC_CmdAppOperCommand(SDIO_TypeDef *SDIOx, uint32_t SdType);
uint32_t SDMMC_CmdBusWidth(SDIO_TypeDef *SDIOx, uint32_t BusWidth);
uint32_t SDMMC_CmdSendSCR(SDIO_TypeDef *SDIOx);
uint32_t SDMMC_CmdSendCID(SDIO_TypeDef *SDIOx);
uint32_t SDMMC_CmdSendCSD(SDIO_TypeDef *SDIOx, uint32_t Argument);
uint32_t SDMMC_CmdSetRelAdd(SDIO_TypeDef *SDIOx, uint16_t *pRCA);
uint32_t SDMMC_CmdSendStatus(SDIO_TypeDef *SDIOx, uint32_t Argument);
uint32_t SDMMC_CmdStatusRegister(SDIO_TypeDef *SDIOx);

uint32_t SDMMC_CmdOpCondition(SDIO_TypeDef *SDIOx, uint32_t Argument);
uint32_t SDMMC_CmdSwitch(SDIO_TypeDef *SDIOx, uint32_t Argument);
uint32_t SDMMC_CmdEraseStartAdd(SDIO_TypeDef *SDIOx, uint32_t StartAdd);
uint32_t SDMMC_CmdEraseEndAdd(SDIO_TypeDef *SDIOx, uint32_t EndAdd);

#endif /* HAL_SDMMC_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC90XX_LL_SDMMC_H__ */
