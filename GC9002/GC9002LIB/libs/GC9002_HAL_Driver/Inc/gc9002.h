/******************************************************************************
 * @file     gc9002.h
 * @brief    NMSIS Core Peripheral Access Layer Header File
 * @version  V1.00
 * @date     2022-12-12
 ******************************************************************************
 */

#ifndef __GC9002_H__
#define __GC9002_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "core_rv32.h"

/* -------------------------  Interrupt Number Definition  ------------------------ */
typedef enum IRQn {
    NMI_EXPn                        =   -2,      /* NMI Exception */
    /* ----------------------  SmartL Specific Interrupt Numbers  --------------------- */
    Machine_Software_IRQn           =   3,      /* Machine software interrupt */
    User_Timer_IRQn                 =   4,      /* User timer interrupt */
    Supervisor_Timer_IRQn           =   5,      /* Supervisor timer interrupt */
    CORET_IRQn                      =   7,      /* core Timer Interrupt */
    Machine_External_IRQn           =   11,     /* Machine external interrupt */
    CANFD_IRQn                   	=   16,     /* canfd Interrupt */
    UART0_IRQn                      =   17,     /* uart Interrupt */
    UART1_IRQn                      =   18,     /* uart Interrupt */
    UART2_IRQn                      =   19,     /* uart Interrupt */
    TIM0_IRQn                       =   20,     /* timer0 Interrupt */
    TIM1_IRQn                       =   21,     /* timer1 Interrupt */
    SPI0_IRQn                       =   22,     /*  Interrupt */
    I2C0_IRQn                       =   23,     /*  Interrupt */
    I2C1_IRQn                       =   24,     /*  Interrupt */
    SPI1_IRQn                       =   25,     /*  Interrupt */
    I2S_IRQn                        =   26,     /*  Interrupt */
    GPIOA_IRQn                      =   27,     /*  Interrupt */
    GPIOB_IRQn                      =   28,     /*  Interrupt */
    SPI2_IRQn                       =   29,     /*  Interrupt */
    ADC_IRQn                        =   30,     /*  Interrupt */
    WDT_IRQn                        =   31,     /*  Interrupt */
    RTC_IRQn                        =   32,     /*  Interrupt */
    CQSPI_IRQn                      =   33,     /*  Interrupt */
    VIDEO_CAP_IRQn                  =   34,     /*  Interrupt */
    DMA_IRQn                        =   35,     /*  Interrupt */
    OTG_IRQn                        =   36,     /*  Interrupt (full speed) */
    AHMI_IRQn                       =   37,     /*  Interrupt */
    DISPLAY_IRQn                    =   38,     /*  Interrupt */
    CRC32_IRQn                      =   39,     /*  Interrupt */
    MOTOR_IRQn                      =   40,     /*  Interrupt */
    MAC_IRQn                        =   41,     /*  Interrupt */
    EXTENSION_IRQn                  =   42,     /* EXTENSION Interrupt */
    DQSPI_IRQn                      =   43,     /* DQSPI Interrupt */
    DEFAULT0_IRQn                   =   44,     /* DEFAULT0 Interrupt */
    DEFAULT1_IRQn                   =   45,     /* DEFAULT1 Interrupt */
    DEFAULT2_IRQn                   =   46,     /* DEFAULT2 Interrupt */
}IRQn_Type;


/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */


/* ToDo: add here your device peripherals pointer definitions
         following is an example for timer */
/** @addtogroup Device_Peripheral_declaration
  * @{
  */

#define DRIVER_GC9002
#define NUM_GPIO 32



/**
  * @brief Reset and Clock Control
  */
union _SYS_PLL_CTRL
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t sys_pll_f    : 9;
		uint32_t                   : 5;
		__IO uint32_t sys_pll_rstn : 1;
		__IO uint32_t sys_pll_en   : 1;
		__IO uint32_t sys_pll_n    : 6;
		uint32_t                   : 2;
		__IO uint32_t sys_pll_q    : 4;
		uint32_t                   : 3;
		__IO uint32_t sys_pll_lock : 1;
	}bit;
};
union _SYS_CLK_SEL
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t sys_clk_sel  : 1;
		uint32_t                   : 31;
	}bit;
};
union _SYS_CLK_PRES
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t sys_pres     : 2;
		uint32_t                   : 30;
	}bit;
};
union _DISP_CLK_PRES
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t disp_duty_h   : 9;
		uint32_t                    : 7;
		__IO uint32_t disp_duty_l   : 9;
		uint32_t                    : 6;
		__IO uint32_t disp_clken    : 1;
	}bit;
};
union _I2S_CLK_PRES
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t i2s_duty_h   : 8;
		__IO uint32_t i2s_duty_l   : 8;
		uint32_t                   : 16;
	}bit;
};
union _CQSPI_CLK_PRES
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t cqspi_pres   : 2;
		uint32_t                   : 30;
	}bit;
};
union _HYPER_CLK_PRES
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t hyper_pres       : 2;
		__IO uint32_t hyper_clken      : 1;
		__IO uint32_t dqspi_clken      : 1;
		__IO uint32_t dqspi_hyper_sel  : 1;
		uint32_t                       : 27;
	}bit;
};
union _DQSPI
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t hyper_pres       : 2;
		__IO uint32_t hyper_clken      : 1;
		__IO uint32_t dqspi_clken      : 1;
		__IO uint32_t dqspi_hyper_sel  : 1;
		uint32_t                       : 27;
	}bit;
};
union _APB_CLK_CTRL
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t apb_pres     : 2;
		__IO uint32_t can_clken    : 1;
		__IO uint32_t i2c_clken    : 1;
		__IO uint32_t spi_clken    : 1;
		__IO uint32_t usb_clken    : 1;
		uint32_t                   : 26;
	}bit;
};
union _OUT_CLK_CTRL
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t clkout_sel       : 3;
		__IO uint32_t clkout_pres      : 3;
		uint32_t                       : 26;
	}bit;
};
union _RESET_CTRL
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t i2s_rstn         : 1;
		__IO uint32_t disp_rstn        : 1;
		__IO uint32_t cqspi_rstn       : 1;
		__IO uint32_t hyper_rstn       : 1;
		__IO uint32_t usb_rstn         : 1;
		__IO uint32_t spi_rstn         : 1;
		__IO uint32_t i2c_rstn         : 1;
		__IO uint32_t can_rstn         : 1;
		__IO uint32_t disp_qspi_rstn   : 1;
		__IO uint32_t dqspi_rstn       : 1;
		uint32_t                       : 22;
	}bit;
};
union _USB_CTRL
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t otg_en           : 1;
		__IO uint32_t dischrg_vbus     : 1;
		uint32_t                       : 2;
		__IO uint32_t usb_dmpu         : 1;
		__IO uint32_t usb_dmpu_lo      : 1;
		__IO uint32_t usb_dmpulldn     : 1;
		uint32_t                       : 1;
		__IO uint32_t usb_cid_inter    : 1;
		__IO uint32_t usb_cid          : 1;
		uint32_t                       : 22;
	}bit;
};
union _WDG_CTRL
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t wdg_speed_up     : 1;
		uint32_t                       : 31;
	}bit;
};
union _DCACHE_SRAM_SHARE
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t dcache_share_en  : 1;
		uint32_t                       : 31;
	}bit;
};
union _CAN_FD_ENABLE
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t can_fd_enable    : 1;
		uint32_t                       : 3;
		__IO uint32_t can_pres         : 2;
		uint32_t                       : 26;
	}bit;
};
union _DEBUG_CTL
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t debug_enable         : 1;
		__IO uint32_t cpu_safe_rst_enable  : 1;
		uint32_t                           : 30;
	}bit;
};
union _PMU_CTL
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t byp_vo18     : 1;
		__IO uint32_t vo3318_en    : 1;
		__IO uint32_t vtr3318      : 3;
		__IO uint32_t vtr12        : 3;
		uint32_t                   : 24;
	}bit;
};
union _DISP_QSPI_CLK_CTL
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t disp_qspi_duty_h  : 9;
		uint32_t                        : 7;
		__IO uint32_t disp_qspi_duty_l  : 9;
		uint32_t                        : 6;
		__IO uint32_t disp_qspi_clken   : 1;
	}bit;
};
typedef struct
{
    union _SYS_PLL_CTRL 		SYS_PLL_CTRL;		/*!< Offset: 0x00  (R/W) */
    union _SYS_CLK_SEL 			SYS_CLK_SEL;		/*!< Offset: 0x04  (R/W) */
    union _SYS_CLK_PRES 		SYS_CLK_PRES;		/*!< Offset: 0x08  (R/W) */
    union _DISP_CLK_PRES 		DISP_CLK_PRES;		/*!< Offset: 0x0C  (R/W) */
    union _I2S_CLK_PRES 		I2S_CLK_PRES;		/*!< Offset: 0x10  (R/W) */
    union _CQSPI_CLK_PRES 		CQSPI_CLK_PRES;		/*!< Offset: 0x14  (R/W) */
    union
    {
        union _HYPER_CLK_PRES 	HYPER_CLK_PRES;		/*!< Offset: 0x18  (R/W) */
        union _DQSPI 			DQSPI_CLK_PRES;		/*!< Offset: 0x18  (R/W) */
    };
    union _APB_CLK_CTRL 		APB_CLK_CTRL;		/*!< Offset: 0x1C  (R/W) */
    union _OUT_CLK_CTRL 		OUT_CLK_CTRL;		/*!< Offset: 0x20  (R/W) */
    union _RESET_CTRL 			RESET_CTRL;			/*!< Offset: 0x24  (R/W) */
    union _USB_CTRL 			USB_CTRL;			/*!< Offset: 0x28  (R/W) */
    union _WDG_CTRL 			WDG_CTRL;			/*!< Offset: 0x2C  (R/W) */
    union _DCACHE_SRAM_SHARE 	DCACHE_SRAM_SHARE;	/*!< Offset: 0x30  (R/W) */
    union _CAN_FD_ENABLE 		CAN_FD_ENABLE;		/*!< Offset: 0x34  (R/W) */
    union _DEBUG_CTL 			DEBUG_CTL;			/*!< Offset: 0x38  (R/W) */
    union _PMU_CTL 				PMU_CTL;			/*!< Offset: 0x3C  (R/W) */
    union _DISP_QSPI_CLK_CTL    DISP_QSPI_CLK_CTL;  /*!< Offset: 0x40  (R/W) */
    __IO uint32_t               MAC_CTL;            /*!< Offset: 0x44  (R/W) */
}RCC_TypeDef;

/**
  * @brief Data Cache
  */
typedef struct
{
    __IO uint32_t  CTRL;            /*!< Offset: 0x00  (R/W) */
    __IO uint32_t  FATCH;           /*!< Offset: 0x04  (R/W) */
    __IO uint32_t  MISS;            /*!< Offset: 0x08  (R/W) */
    __IO uint32_t  PNLT;            /*!< Offset: 0x0c  (R/W) */
    __IO uint32_t  ADDR;            /*!< Offset: 0x10  (R/W) */
    __IO uint32_t  RANGE;           /*!< Offset: 0x14  (R/W) */
} DCACHE_TypeDef;


/**
  * @brief Hyper
  */
union _HYPER_RESET
{
	__IO uint32_t reg_u32val;      
	struct
	{
		__IO uint32_t SWRST     : 1;
		__IO uint32_t PD        : 1;
		uint32_t                : 30;
	}bit;
};
union _HYPER_IO
{
	__IO uint32_t reg_u32val;      
	struct
	{
		__IO uint32_t CLKINV    : 1;
		uint32_t                : 27;
		__IO uint32_t DS        : 2;
		uint32_t                : 2;
	}bit;
};
union _HYPER_TIMING
{
	__IO uint32_t reg_u32val;      
	struct
	{
		__IO uint32_t TRPL  : 8;
		__IO uint32_t TRPH  : 8;
		__IO uint32_t TRWR  : 8;
		__IO uint32_t TACC  : 8;
	}bit;
};
union _HYPER_OP
{
	__IO uint32_t reg_u32val;     
	struct
	{
		__IO uint32_t START     : 1;
		__IO uint32_t WIP       : 1;
		uint32_t                : 1;
		__IO uint32_t WR        : 1;
		__IO uint32_t DIE       : 2;
		uint32_t                : 2;
		__IO uint32_t REGION    : 8;
		__IO uint32_t REG       : 8;
		uint32_t                : 8;
	}bit;
};
union _HYPER_CTRL
{
	__IO uint32_t reg_u32val;        
	struct
	{
		__IO uint32_t RDMMY1    : 8;
		__IO uint32_t RDMMY2    : 8;
		uint32_t                : 16;
	}bit;
};
union _HYPER_CAPTURE
{
	__IO uint32_t reg_u32val;        
	struct
	{
		__IO uint32_t OP_CLK_DIV    : 4;
		__IO uint32_t CDELAY        : 4;
		__IO uint32_t CEDGE         : 1;
		uint32_t                    : 23;
	}bit;
};
typedef struct
{
    uint32_t                Reserved0;      /*!< Offset: 0x00  (R/W) */
    union _HYPER_RESET 		RESET;		    /*!< Offset: 0x04  (R/W) */
    union _HYPER_IO 		IO;		        /*!< Offset: 0x08  (R/W) */
    uint32_t                Reserved1;      /*!< Offset: 0x0C  (R/W) */
    union _HYPER_TIMING 	TIMING;	        /*!< Offset: 0x10  (R/W) */
    uint32_t                Reserved2[2];   /*!< Offset: 0x14/0x18  (R/W) */
    union _HYPER_OP 		OP; 		    /*!< Offset: 0x1C  (R/W) */
    __IO uint32_t           WDATA;          /*!< Offset: 0x20  (R/W) */
    __IO uint32_t           RDATA;          /*!< Offset: 0x24  (R/W) */
    uint32_t                Reserved3;      /*!< Offset: 0x28  (R/W) */
    __IO uint32_t			QOSID;			/*!< Offset: 0x2C  (R/W) */
    union _HYPER_CTRL  		RCR;		    /*!< Offset: 0x30  (R/W) */
    union _HYPER_CAPTURE 	CAPTURE;	    /*!< Offset: 0x34  (R/W) */
}HYPER_TypeDef;


/**
  * @brief CRC32
  */
typedef struct
{
    union
    {
        __IO uint32_t CR;
        struct
        {
            __IO uint32_t START      : 1;
            __IO uint32_t RES_VALID  : 1;/* result is valid ? */
            __IO uint32_t IDLE       : 1;
            uint32_t            	 : 29;
        };
    };
    __IO uint32_t AR;   /* base addr */
    __IO uint32_t LR;   /* length */
    __IO uint32_t DR;   /* result */
}CRC32_Typedef;


/**
  * @brief General Purpose I/O
  */
typedef struct
{
    __IO uint32_t P0;
    __IO uint32_t P1;
    __IO uint32_t P2;
    __IO uint32_t P3;
    __IO uint32_t P4;
    __IO uint32_t P5;
    __IO uint32_t P6;
    __IO uint32_t P7;
    __IO uint32_t P8;
    __IO uint32_t P9;
    __IO uint32_t P10;
    __IO uint32_t P11;
    __IO uint32_t P12;
    __IO uint32_t P13;
    __IO uint32_t P14;
    __IO uint32_t P15;
    __IO uint32_t P16;
    __IO uint32_t P17;
    __IO uint32_t P18;
    __IO uint32_t P19;
    __IO uint32_t P20;
    __IO uint32_t P21;
    __IO uint32_t P22;
    __IO uint32_t P23;
    __IO uint32_t P24;
    __IO uint32_t P25;
    __IO uint32_t P26;
    __IO uint32_t P27;
    __IO uint32_t P28;
    __IO uint32_t P29;
    __IO uint32_t P30;
    __IO uint32_t P31;
    __IO uint32_t IntrFlag;
    uint32_t RESERVED[0xDF];
    __IO uint32_t MODE;
}GPIO_TypeDef;

/**
  * @brief Universal Synchronous Asynchronous Receiver Transmitter
  */
typedef struct
{
    __IO uint32_t SR;         /*!< UART Status register,                   Address offset: 0x00 */
    __IO uint32_t DR;         /*!< UART Data register,                     Address offset: 0x04 */
    __IO uint32_t BRR;        /*!< UART Baud rate register,                Address offset: 0x08 */
    __IO uint32_t CR1;        /*!< UART Control register 1,                Address offset: 0x0C */
    __IO uint32_t CR2;        /*!< UART Control register 2,                Address offset: 0x10 */
    __IO uint32_t CR3;        /*!< UART Control register 3,                Address offset: 0x14 */
    __IO uint32_t GTPR;       /*!< UART Guard time and prescaler register, Address offset: 0x18 */
} UART_TypeDef;

/**
  * @brief MOTOR
  */
union _MOT_CTRL
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t CH_enable		:1;
		__IO uint32_t CH_newcmd		:1;
		__IO uint32_t mode			:2;
		__IO uint32_t auto_stop		:1;
		__IO uint32_t auto_restart	:1;
		uint32_t 					:2;
		__IO uint32_t total_tick	:2;
		uint32_t 					:21;
		__IO uint32_t CH_busy		:1;
	}bit;
};
union _MOT_IR
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t CH0_DONE		:1;
		__IO uint32_t CH0_WARNING	:1;
		__IO uint32_t CH0_DIV_BY_0	:1;
		__IO uint32_t CH0_TIME_END	:1;
		__IO uint32_t CH1_DONE		:1;
		__IO uint32_t CH1_WARNING	:1;
		__IO uint32_t CH1_DIV_BY_0	:1;
		__IO uint32_t CH1_TIME_END	:1;
		__IO uint32_t CH2_DONE		:1;
		__IO uint32_t CH2_WARNING	:1;
		__IO uint32_t CH2_DIV_BY_0	:1;
		__IO uint32_t CH2_TIME_END	:1;
		__IO uint32_t CH3_DONE		:1;
		__IO uint32_t CH3_WARNING	:1;
		__IO uint32_t CH3_DIV_BY_0	:1;
		__IO uint32_t CH3_TIME_END	:1;
		uint32_t 					:16;
	}bit;
};
typedef struct
{
	struct{
    	union _MOT_CTRL CTRL;		/*!< Offset: 0x00+x*0x40  (R/W) */
		__IO uint32_t TICK_DIV;		/*!< Offset: 0x04+x*0x40  (R/W) */
		__IO uint32_t ST_VALUE;		/*!< Offset: 0x08+x*0x40  (R/W) */
		__IO uint32_t Y0;			/*!< Offset: 0x0C+x*0x40  (R/W) */
		__IO uint32_t Y1;			/*!< Offset: 0x10+x*0x40  (R/W) */
		__IO uint32_t DY0;			/*!< Offset: 0x14+x*0x40  (R/W) */
		__IO uint32_t D2Y0;			/*!< Offset: 0x18+x*0x40  (R/W) */
		__IO uint32_t D3Y;			/*!< Offset: 0x1C+x*0x40  (R/W) */
		__IO uint32_t WARN_STEP_CNT;/*!< Offset: 0x20+x*0x40  (R/W) */
		__IO uint32_t WARN_STEP_REQ;/*!< Offset: 0x24+x*0x40  (R/W) */
		__IO uint32_t STEP_CNT;     /*!< Offset: 0x28+x*0x40  (R/W) */
		__IO uint32_t TOTAL_STEP_REQ;/*!< Offset: 0x2C+x*0x40  (R/W) */
		uint32_t Reserved[4];		/*!< Offset: 0x30+x*0x40~0x3C+x*0x40  (R/W) */
	}CH[8];
	uint32_t Reservedx[384];		/*!< Offset: 0x200~0x7FC  (R/W) */
	union _MOT_IR IER;			    /*!< Offset: 0x800  (R/W) */
	union _MOT_IR ISR;			    /*!< Offset: 0x804  (R/W) */
	__IO uint32_t SYS_TICK;			/*!< Offset: 0x808  (R/W) */
}MOTOR_TypeDef;

/**
  * @brief DMA
  */
union _DMA_SAR
{
    __IO uint64_t reg_u64val;      /*!< Offset: 0x0+x*0x58   (R/W) */
    struct
    {
        /*31: 0, Current Source Address of DMA transfer. Updated after each sourcetransfer.
        * The SINC field in the CTLx register determines whether the address increments, decrements,
        * or is left unchanged on every source transfer throughout the block transfer.*/
        __IO uint64_t SAR   : 32;
        uint64_t            : 32;
    }bit;
};
union _DMA_DAR
{
    __IO uint64_t reg_u64val;      /*!< Offset: 0x8+x*0x58   (R/W) */
    struct
    {
        /*31: 0, Current Destination address of DMA transfer. Updated after eachdestination transfer.
        * The DINC field in the CTLx register determines whetherthe address increments, decrements,
        * or is left unchanged on everydestination transfer throughout the block transfer.*/
        __IO uint64_t DAR   : 32;
        uint64_t            : 32;
    }bit;
};
union _DMA_LLP
{
    __IO uint64_t reg_u64val;      /*!< Offset: 0x10+x*0x58   (R/W) */
    struct
    {
        __IO uint64_t LMS   : 2;
        __IO uint64_t LOC   : 30;
        uint64_t            : 32;
    }bit;
};
union _DMA_CTL
{
    __IO uint64_t reg_u64val;      /*!< Offset: 0x18+x*0x58   (R/W) */
    struct
    {
        __IO uint64_t INT_EN            : 1;
        __IO uint64_t DST_TR_WIDTH      : 3;
        __IO uint64_t SRC_TR_WIDTH      : 3;
        __IO uint64_t DINC              : 2;
        __IO uint64_t SINC              : 2;
        __IO uint64_t DST_MSIZE         : 3;
        __IO uint64_t SRC_MSIZE         : 3;
        __IO uint64_t SRC_GATHER_EN     : 1;
        __IO uint64_t DST_SCATTER_EN    : 1;
        uint64_t                        : 1;
        __IO uint64_t TT_FC             : 3;
        __IO uint64_t DMS               : 2;
        __IO uint64_t SMS               : 2;
        __IO uint64_t LLP_DST_EN        : 1;
        __IO uint64_t LLP_SRC_EN        : 1;
        uint64_t                        : 3;
        __IO uint64_t BLOCK_TS          : 12;
        __IO uint64_t DONE              : 1;
        uint64_t                        : 19;
    }bit;
};
union _DMA_CFG
{
    __IO uint64_t reg_u64val;      /*!< Offset: 0x40+x*0x58   (R/W) */
    struct
    {
        uint64_t                    : 5;
        __IO uint64_t CH_PRIOR      : 3;
        __IO uint64_t CH_SUSP       : 1;
        __IO uint64_t FIFO_EMPTY    : 1; /* Read only */
        __IO uint64_t HS_SEL_DST    : 1;
        __IO uint64_t HS_SEL_SRC    : 1;
        __IO uint64_t LOCK_CH_L     : 2;
        __IO uint64_t LOCK_B_L      : 2;
        __IO uint64_t LOCK_CH       : 1;
        __IO uint64_t LOCK_B        : 1;
        __IO uint64_t DST_HS_POL    : 1;
        __IO uint64_t SRC_HS_POL    : 1;
        __IO uint64_t MAX_ABRST     : 10;
        __IO uint64_t RELOAD_SRC    : 1;
        __IO uint64_t RELOAD_DST    : 1;
        __IO uint64_t FCMODE        : 1;
        __IO uint64_t FIFO_MODE     : 1;
        __IO uint64_t PROTCTL       : 3;
        __IO uint64_t DS_UPD_EN     : 1;
        __IO uint64_t SS_UPD_EN     : 1;
        __IO uint64_t SRC_PER       : 4;
        __IO uint64_t DST_PER       : 4;
        uint64_t                    : 17;
    }bit;
};
union _DMA_SGR
{
    __IO uint64_t reg_u64val;      /*!< Offset: 0x48+x*0x58   (R/W) */
    struct
    {
        __IO uint64_t SGI   : 20;
        __IO uint64_t SGC   : 12;
        uint64_t            : 32;
    }bit;
};
union _DMA_DSR
{
    __IO uint64_t reg_u64val;      /*!< Offset: 0x50+x*0x58   (R/W) */
    struct
    {
        __IO uint64_t DSI   : 20;
        __IO uint64_t DSC   : 12;
        uint64_t            : 32;
    }bit;
};
/**
  * @brief DW_axi_dmac_mem_map/Channelx_Registers_Address_Block Registers
  */
typedef struct __DMAChannel_TypeDef
{
    struct
    {
        union _DMA_SAR SAR;
        union _DMA_DAR DAR;
        union _DMA_LLP LLP;
        union _DMA_CTL CTL;
        __IO uint64_t SSTAT;    /*!< Offset: 0x20+x*0x58   (R/W) */
        __IO uint64_t DSTAT;    /*!< Offset: 0x28+x*0x58   (R/W) */
        __IO uint64_t SSTATAR;  /*!< Offset: 0x30+x*0x58   (R/W) */
        __IO uint64_t DSTATAR;  /*!< Offset: 0x38+x*0x58   (R/W) */
    };
    union _DMA_CFG CFG;
    union _DMA_SGR SGR;
    union _DMA_DSR DSR;
}DMAChannel_TypeDef;


union _DMA_MaskTfr
{
	__IO uint64_t reg_u64val;          /*!< Offset: 0x310  (R/W) */
	struct
	{
		__IO uint64_t INT_MASK      : 8;
		__IO uint64_t INT_MASK_WE   : 8;
		uint64_t                    : 48;
	}bit;
};
union _DMA_MaskBlock
{
	__IO uint64_t reg_u64val;        /*!< Offset: 0x318  (R/W) */
	struct
	{
		__IO uint64_t INT_MASK      : 8;
		__IO uint64_t INT_MASK_WE   : 8;
		uint64_t                    : 48;
	}bit;
};
union _DMA_MaskSrcTran
{
	__IO uint64_t reg_u64val;      /*!< Offset: 0x320  (R/W) */
	struct
	{
		__IO uint64_t INT_MASK      : 8;
		__IO uint64_t INT_MASK_WE   : 8;
		uint64_t                    : 48;
	}bit;
};
union _DMA_MaskDstTran
{
	__IO uint64_t reg_u64val;      /*!< Offset: 0x328  (R/W) */
	struct
	{
		__IO uint64_t INT_MASK      : 8;
		__IO uint64_t INT_MASK_WE   : 8;
		uint64_t                    : 48;
	}bit;
};
union _DMA_MaskErr
{
	__IO uint64_t reg_u64val;          /*!< Offset: 0x330  (R/W) */
	struct
	{
		__IO uint64_t INT_MASK      : 8;
		__IO uint64_t INT_MASK_WE   : 8;
		uint64_t                    : 48;
	}bit;
};
union _DMA_StatusInt
{
	__IO uint64_t reg_u64val;        /*!< Offset: 0x360  (R/W) */
	struct
	{
		__IO uint64_t TFR       : 1;
		__IO uint64_t BLOCK     : 1;
		__IO uint64_t SRCT      : 1;
		__IO uint64_t DSTT      : 1;
		__IO uint64_t ERR       : 1;
		uint64_t                : 59;
	}bit;
};
union _DMA_ReqSrcReg
{
	__IO uint64_t reg_u64val;    /*!< Offset: 0x368  (R/W) */
	struct
	{
		__IO uint64_t SRC_REQ       : 8;
		__IO uint64_t SRC_REQ_WE    : 8;
		uint64_t                    : 48;
	}bit;
};
union _DMA_ReqDstReg
{
	__IO uint64_t reg_u64val;    /*!< Offset: 0x370  (R/W) */
	struct
	{
		__IO uint64_t DST_REQ       : 8;
		__IO uint64_t DST_REQ_WE    : 8;
		uint64_t                    : 48;
	}bit;
};
union _DMA_SglRqSrcReg
{
	__IO uint64_t reg_u64val;  /*!< Offset: 0x378  (R/W) */
	struct
	{
		__IO uint64_t SRC_SGLREQ    : 8;
		__IO uint64_t SRC_SGLREQ_WE : 8;
		uint64_t                    : 48;
	}bit;
};
union _DMA_SglRqDstReg
{
	__IO uint64_t reg_u64val;  /*!< Offset: 0x380  (R/W) */
	struct
	{
		__IO uint64_t DST_SGLREQ        : 8;
		__IO uint64_t DST_SGLREQ_WE     : 8;
		uint64_t                        : 48;
	}bit;
};
union _DMA_LstSrcReg
{
	__IO uint64_t reg_u64val;    /*!< Offset: 0x388  (R/W) */
	struct
	{
		__IO uint64_t LSTSRC        : 8;
		__IO uint64_t LSTSRC_WE     : 8;
		uint64_t                    : 48;
	}bit;
};
union _DMA_LstDstReg
{
	__IO uint64_t reg_u64val;    /*!< Offset: 0x390  (R/W) */
	struct
	{
		__IO uint64_t LSTDST        : 8;
		__IO uint64_t LSTDST_WE     : 8;
		uint64_t                    : 48;
	}bit;
};
union _DMA_DmaCfgReg
{
	__IO uint64_t reg_u64val;            /*!< Offset: 0x398  (R/W) */
	struct
	{
		__IO uint64_t DMA_EN    : 1;
		uint64_t                : 63;
	}bit;
};
union _DMA_ChEnReg
{
	__IO uint64_t reg_u64val;              /*!< Offset: 0x3A0  (R/W) */
	struct
	{
		__IO uint64_t CH_EN         : 8;
		__IO uint64_t CH_EN_WE      : 8;
		uint64_t                    : 48;
	}bit;
};
union _DMA_DmaIdReg
{
	__IO uint64_t reg_u64val;             /*!< Offset: 0x3A8  (R/W) */
	struct
	{
		__IO uint64_t DMA_ID    : 32;
		uint64_t                : 32;
	}bit;
};
union _DMA_DmaTestReg
{
	__IO uint64_t reg_u64val ;          /*!< Offset: 0x3B0  (R/W) */
	struct
	{
		__IO uint64_t TEST_SLV_IF    : 1;
		__IO uint64_t                : 64;
	}bit;
};
union _DMA_COMP_PARAMS_6
{
	__IO uint64_t reg_u64val;    /*!< Offset: 0x3C8  (R/W) */
	struct
	{
		uint64_t                        : 32;
		__IO uint64_t CH7_DTW           : 3;
		__IO uint64_t CH7_STW           : 3;
		__IO uint64_t CH7_STAT_DST      : 1;
		__IO uint64_t CH7_STAT_SRC      : 1;
		__IO uint64_t CH7_DST_SCA_EN    : 1;
		__IO uint64_t CH7_SRC_GAT_EN    : 1;
		__IO uint64_t CH7_LOCK_EN       : 1;
		__IO uint64_t CH7_MULTI_BLK_EN  : 1;
		__IO uint64_t CH7_CTL_WB_EN     : 1;
		__IO uint64_t CH7_HC_LLP        : 1;
		__IO uint64_t CH7_FC            : 2;
		__IO uint64_t CH7_MAX_MULT_SIZE : 3;
		__IO uint64_t CH7_DMS           : 3;
		__IO uint64_t CH7_LMS           : 3;
		__IO uint64_t CH7_SMS           : 3;
		__IO uint64_t CH7_FIFO_DEPTH    : 3;
	}bit;
};
union _DMA_COMP_PARAMS_5
{
	__IO uint64_t reg_u64val;    /*!< Offset: 0x3D0  (R/W) */
	struct
	{
		__IO uint64_t CH6_DTW            : 3;
		__IO uint64_t CH6_STW            : 3;
		__IO uint64_t CH6_STAT_DST       : 1;
		__IO uint64_t CH6_STAT_SRC       : 1;
		__IO uint64_t CH6_DST_SCA_EN     : 1;
		__IO uint64_t CH6_SRC_GAT_EN     : 1;
		__IO uint64_t CH6_LOCK_EN        : 1;
		__IO uint64_t CH6_MULTI_BLK_EN   : 1;
		__IO uint64_t CH6_CTL_WB_EN      : 1;
		__IO uint64_t CH6_HC_LLP         : 1;
		__IO uint64_t CH6_FC             : 2;
		__IO uint64_t CH6_MAX_MULT_SIZE  : 3;
		__IO uint64_t CH6_DMS            : 3;
		__IO uint64_t CH6_LMS            : 3;
		__IO uint64_t CH6_SMS            : 3;
		__IO uint64_t CH6_FIFO_DEPTH     : 3;
		uint64_t                         : 1;
		__IO uint64_t CH5_DTW            : 3;
		__IO uint64_t CH5_STW            : 3;
		__IO uint64_t CH5_STAT_DST       : 1;
		__IO uint64_t CH5_STAT_SRC       : 1;
		__IO uint64_t CH5_DST_SCA_EN     : 1;
		__IO uint64_t CH5_SRC_GAT_EN     : 1;
		__IO uint64_t CH5_LOCK_EN        : 1;
		__IO uint64_t CH5_MULTI_BLK_EN   : 1;
		__IO uint64_t CH5_CTL_WB_EN      : 1;
		__IO uint64_t CH5_HC_LLP         : 1;
		__IO uint64_t CH5_FC             : 2;
		__IO uint64_t CH5_MAX_MULT_SIZE  : 3;
		__IO uint64_t CH5_DMS            : 3;
		__IO uint64_t CH5_LMS            : 3;
		__IO uint64_t CH5_SMS            : 3;
		__IO uint64_t CH5_FIFO_DEPTH     : 3;
		uint64_t                         : 1;
	}bit;
};
union _DMA_COMP_PARAMS_4
{
	__IO uint64_t reg_u64val;    /*!< Offset: 0x3D8  (R/W) */
	struct
	{
		__IO uint64_t CH4_DTW            : 3;
		__IO uint64_t CH4_STW            : 3;
		__IO uint64_t CH4_STAT_DST       : 1;
		__IO uint64_t CH4_STAT_SRC       : 1;
		__IO uint64_t CH4_DST_SCA_EN     : 1;
		__IO uint64_t CH4_SRC_GAT_EN     : 1;
		__IO uint64_t CH4_LOCK_EN        : 1;
		__IO uint64_t CH4_MULTI_BLK_EN   : 1;
		__IO uint64_t CH4_CTL_WB_EN      : 1;
		__IO uint64_t CH4_HC_LLP         : 1;
		__IO uint64_t CH4_FC             : 2;
		__IO uint64_t CH4_MAX_MULT_SIZE  : 3;
		__IO uint64_t CH4_DMS            : 3;
		__IO uint64_t CH4_LMS            : 3;
		__IO uint64_t CH4_SMS            : 3;
		__IO uint64_t CH4_FIFO_DEPTH     : 3;
		uint64_t                         : 1;
		__IO uint64_t CH3_DTW            : 3;
		__IO uint64_t CH3_STW            : 3;
		__IO uint64_t CH3_STAT_DST       : 1;
		__IO uint64_t CH3_STAT_SRC       : 1;
		__IO uint64_t CH3_DST_SCA_EN     : 1;
		__IO uint64_t CH3_SRC_GAT_EN     : 1;
		__IO uint64_t CH3_LOCK_EN        : 1;
		__IO uint64_t CH3_MULTI_BLK_EN   : 1;
		__IO uint64_t CH3_CTL_WB_EN      : 1;
		__IO uint64_t CH3_HC_LLP         : 1;
		__IO uint64_t CH3_FC             : 2;
		__IO uint64_t CH3_MAX_MULT_SIZE  : 3;
		__IO uint64_t CH3_DMS            : 3;
		__IO uint64_t CH3_LMS            : 3;
		__IO uint64_t CH3_SMS            : 3;
		__IO uint64_t CH3_FIFO_DEPTH     : 3;
		uint64_t                         : 1;
	}bit;
};
union _DMA_COMP_PARAMS_3
{
	__IO uint64_t reg_u64val;    /*!< Offset: 0x3E0  (R/W) */
	struct
	{
		__IO uint64_t CH2_DTW            : 3;
		__IO uint64_t CH2_STW            : 3;
		__IO uint64_t CH2_STAT_DST       : 1;
		__IO uint64_t CH2_STAT_SRC       : 1;
		__IO uint64_t CH2_DST_SCA_EN     : 1;
		__IO uint64_t CH2_SRC_GAT_EN     : 1;
		__IO uint64_t CH2_LOCK_EN        : 1;
		__IO uint64_t CH2_MULTI_BLK_EN   : 1;
		__IO uint64_t CH2_CTL_WB_EN      : 1;
		__IO uint64_t CH2_HC_LLP         : 1;
		__IO uint64_t CH2_FC             : 2;
		__IO uint64_t CH2_MAX_MULT_SIZE  : 3;
		__IO uint64_t CH2_DMS            : 3;
		__IO uint64_t CH2_LMS            : 3;
		__IO uint64_t CH2_SMS            : 3;
		__IO uint64_t CH2_FIFO_DEPTH     : 3;
		uint64_t                         : 1;
		__IO uint64_t CH1_DTW            : 3;
		__IO uint64_t CH1_STW            : 3;
		__IO uint64_t CH1_STAT_DST       : 1;
		__IO uint64_t CH1_STAT_SRC       : 1;
		__IO uint64_t CH1_DST_SCA_EN     : 1;
		__IO uint64_t CH1_SRC_GAT_EN     : 1;
		__IO uint64_t CH1_LOCK_EN        : 1;
		__IO uint64_t CH1_MULTI_BLK_EN   : 1;
		__IO uint64_t CH1_CTL_WB_EN      : 1;
		__IO uint64_t CH1_HC_LLP         : 1;
		__IO uint64_t CH1_FC             : 2;
		__IO uint64_t CH1_MAX_MULT_SIZE  : 3;
		__IO uint64_t CH1_DMS            : 3;
		__IO uint64_t CH1_LMS            : 3;
		__IO uint64_t CH1_SMS            : 3;
		__IO uint64_t CH1_FIFO_DEPTH     : 3;
		uint64_t                         : 1;
	}bit;
};
union _DMA_COMP_PARAMS_2
{
	__IO uint64_t reg_u64val;    /*!< Offset: 0x3E8  (R/W) */
	struct
	{
		__IO uint64_t CH0_DTW            : 3;
		__IO uint64_t CH0_STW            : 3;
		__IO uint64_t CH0_STAT_DST       : 1;
		__IO uint64_t CH0_STAT_SRC       : 1;
		__IO uint64_t CH0_DST_SCA_EN     : 1;
		__IO uint64_t CH0_SRC_GAT_EN     : 1;
		__IO uint64_t CH0_LOCK_EN        : 1;
		__IO uint64_t CH0_MULTI_BLK_EN   : 1;
		__IO uint64_t CH0_CTL_WB_EN      : 1;
		__IO uint64_t CH0_HC_LLP         : 1;
		__IO uint64_t CH0_FC             : 2;
		__IO uint64_t CH0_MAX_MULT_SIZE  : 3;
		__IO uint64_t CH0_DMS            : 3;
		__IO uint64_t CH0_LMS            : 3;
		__IO uint64_t CH0_SMS            : 3;
		__IO uint64_t CH0_FIFO_DEPTH     : 3;
		uint64_t                         : 1;
		__IO uint64_t CHO_MULTI_BLK_TYPE : 4;
		__IO uint64_t CH1_MULTI_BLK_TYPE : 4;
		__IO uint64_t CH2_MULTI_BLK_TYPE : 4;
		__IO uint64_t CH3_MULTI_BLK_TYPE : 4;
		__IO uint64_t CH4_MULTI_BLK_TYPE : 4;
		__IO uint64_t CH5_MULTI_BLK_TYPE : 4;
		__IO uint64_t CH6_MULTI_BLK_TYPE : 4;
		__IO uint64_t CH7_MULTI_BLK_TYPE : 4;
	}bit;
};
union _DMA_COMP_PARAMS_1
{
	__IO uint64_t reg_u64val;    /*!< Offset: 0x3F0  (R/W) */
	struct
	{
		__IO uint64_t CHO_MAX_BLK_SIZE   : 4;
		__IO uint64_t CH1_MAX_BLK_SIZE   : 4;
		__IO uint64_t CH2_MAX_BLK_SIZE   : 4;
		__IO uint64_t CH3_MAX_BLK_SIZE   : 4;
		__IO uint64_t CH4_MAX_BLK_SIZE   : 4;
		__IO uint64_t CH5_MAX_BLK_SIZE   : 4;
		__IO uint64_t CH6_MAX_BLK_SIZE   : 4;
		__IO uint64_t CH7_MAX_BLK_SIZE   : 4;
		__IO uint64_t DMA_BIG_ENDIAN     : 1;
		__IO uint64_t INTR_IO            : 2;
		__IO uint64_t MAX_ABRST          : 1;
		uint64_t                         : 4;
		__IO uint64_t NUM_CHANNELS       : 3;
		__IO uint64_t NUM_MASTER_INT     : 2;
		__IO uint64_t S_HDATA_WIDTH      : 2;
		__IO uint64_t M4_HDATA_WIDTH     : 2;
		__IO uint64_t M3_HDATA_WIDTH     : 2;
		__IO uint64_t M2_HDATA_WIDTH     : 2;
		__IO uint64_t M1_HDATA_WIDTH     : 2;
		__IO uint64_t NUM_HS_INT         : 5;
		__IO uint64_t ADD_ENCODED_PARAMS : 1;
		__IO uint64_t STATIC_ENDIAN_SELECT  : 1;
		uint64_t                            : 2;
	}bit;
};
union _DMA_DmaCompsID
{
	__IO uint64_t reg_u64val;           /*!< Offset: 0x3F8  (R/W) */
	struct
	{
		__IO uint64_t DMA_COMP_TYPE : 32;
		uint64_t                    : 32;
	}bit;
};
typedef struct __DMA_TypeDef
{
    DMAChannel_TypeDef CHx[8];      /*!< Offset: 0x000~0x2B8  (R/W) */
    __IO uint64_t RawTfr;           /*!< Offset: 0x2C0  (R/W) */
    __IO uint64_t RawBlock;         /*!< Offset: 0x2C8  (R/W) */
    __IO uint64_t RawSrcTran;       /*!< Offset: 0x2D0  (R/W) */
    __IO uint64_t RawDstTran;       /*!< Offset: 0x2D8  (R/W) */
    __IO uint64_t RawErr;           /*!< Offset: 0x2E0  (R/W) */
    __IO uint64_t StatusTfr;        /*!< Offset: 0x2E8  (R/W) */
    __IO uint64_t StatusBlock;      /*!< Offset: 0x2F0  (R/W) */
    __IO uint64_t StatusSrcTran;    /*!< Offset: 0x2F8  (R/W) */
    __IO uint64_t StatusDstTran;    /*!< Offset: 0x300  (R/W) */
    __IO uint64_t StatusErr;        /*!< Offset: 0x308  (R/W) */
    union _DMA_MaskTfr 		MaskTfr;
    union _DMA_MaskBlock 	MaskBlock;
    union _DMA_MaskSrcTran 	MaskSrcTran;
    union _DMA_MaskDstTran 	MaskDstTran;
    union _DMA_MaskErr 		MaskErr;
    __IO uint64_t ClearTfr;         /*!< Offset: 0x338  (R/W) */
    __IO uint64_t ClearBlock;       /*!< Offset: 0x340  (R/W) */
    __IO uint64_t ClearSrcTran;     /*!< Offset: 0x348  (R/W) */
    __IO uint64_t ClearDstTran;     /*!< Offset: 0x350  (R/W) */
    __IO uint64_t ClearErr;         /*!< Offset: 0x358  (R/W) */
    union _DMA_StatusInt 	StatusInt;
    struct
    {
        union _DMA_ReqSrcReg 		ReqSrcReg;
        union _DMA_ReqDstReg 		ReqDstReg;
        union _DMA_SglRqSrcReg 		SglRqSrcReg;
        union _DMA_SglRqDstReg 		SglRqDstReg;
        union _DMA_LstSrcReg 		LstSrcReg;
        union _DMA_LstDstReg 		LstDstReg;
    };/*!< Offset: 0x368~0x390  (R/W) */
    struct
    {
        union _DMA_DmaCfgReg  		DmaCfgReg;
        union _DMA_ChEnReg 			ChEnReg;
        union _DMA_DmaIdReg 		DmaIdReg;
        union _DMA_DmaTestReg 		DmaTestReg;
        __IO uint64_t DmaLpTimeoutReg;      /*!< Offset: 0x3B8  (R/W) */
        __IO uint64_t Reserved;             /*!< Offset: 0x3C0  (R/W) */
        union _DMA_COMP_PARAMS_6 	DMA_COMP_PARAMS_6;
        union _DMA_COMP_PARAMS_5 	DMA_COMP_PARAMS_5;
        union _DMA_COMP_PARAMS_4 	DMA_COMP_PARAMS_4;
        union _DMA_COMP_PARAMS_3 	DMA_COMP_PARAMS_3;
        union _DMA_COMP_PARAMS_2 	DMA_COMP_PARAMS_2;
        union _DMA_COMP_PARAMS_1 	DMA_COMP_PARAMS_1;
        union _DMA_DmaCompsID 		DmaCompsID;
    };/*!< Offset: 0x398~0x3F8  (R/W) */
}DMA_TypeDef;

typedef struct
{
    __IO uint32_t GIR;              /*!< SDIO Host? General information register,             Address offset: 0x00 */
    __IO uint32_t DSR;              /*!< SDIO Host? Debounce settings register,               Address offset: 0x04 */
    __IO uint32_t BSR;              /*!< SDIO Host? Bus settings register,                    Address offset: 0x08 */
    __IO uint32_t Reserved;         /*!< SDIO Host? Reserved register,                        Address offset: 0x0C */
    __IO uint32_t HWInit_CR1_0;     /*!< SDIO Host? HWInit SRS16 configuration for slot #0    Address offset: 0x10 */
    __IO uint32_t HWInit_CR2_0;     /*!< SDIO Host? HWInit SRS17 configuration for slot #0    Address offset: 0x14 */
    __IO uint32_t HWInit_CR3_0;     /*!< SDIO Host? HWInit SRS18 configuration for slot #0    Address offset: 0x18 */
    __IO uint32_t Reserved0;        /*!< SDIO Host? Reserved0 register,                       Address offset: 0x1C */
    __IO uint32_t HWInit_CR1_1;     /*!< SDIO Host? HWInit SRS16 configuration for slot #1    Address offset: 0x20 */
    __IO uint32_t HWInit_CR2_1;     /*!< SDIO Host? HWInit SRS17 configuration for slot #1    Address offset: 0x24 */
    __IO uint32_t HWInit_CR3_1;     /*!< SDIO Host? HWInit SRS18 configuration for slot #1    Address offset: 0x28 */
    __IO uint32_t Reserved1;        /*!< SDIO Host? Reserved1 register,                       Address offset: 0x2C */
    __IO uint32_t HWInit_CR1_2;     /*!< SDIO Host? HWInit SRS16 configuration for slot #2    Address offset: 0x30 */
    __IO uint32_t HWInit_CR2_2;     /*!< SDIO Host? HWInit SRS17 configuration for slot #2    Address offset: 0x34 */
    __IO uint32_t HWInit_CR3_2;     /*!< SDIO Host? HWInit SRS18 configuration for slot #2    Address offset: 0x38 */
    __IO uint32_t Reserved2;        /*!< SDIO Host? Reserved2 register,                       Address offset: 0x3C */
    __IO uint32_t HWInit_CR1_3;     /*!< SDIO Host? HWInit SRS16 configuration for slot #3    Address offset: 0x40 */
    __IO uint32_t HWInit_CR2_3;     /*!< SDIO Host? HWInit SRS17 configuration for slot #3    Address offset: 0x44 */
    __IO uint32_t HWInit_CR3_3;     /*!< SDIO Host? HWInit SRS18 configuration for slot #3    Address offset: 0x48 */
    __IO uint32_t Reserved3;        /*!< SDIO Host? Reserved3 register,                       Address offset: 0x4C */
    __IO uint32_t CPRM_SR;          /*!< SDIO Host? CPRM information/settings register        Address offset: 0x50 */
    __IO uint32_t CPRM_CBC_SR;      /*!< SDIO Host? CPRM CBC settings register                Address offset: 0x54 */
    __IO uint32_t Reserved4[2];     /*!< SDIO Host? Reserved4 register,                       Address offset: 0x58/0x5C */
    __IO uint32_t HWInit_Preset0_0; /*!< SDIO Host? HWInit SRS24 preset for slot #0           Address offset: 0x60 */
    __IO uint32_t HWInit_Preset1_0; /*!< SDIO Host? HWInit SRS25 preset for slot #0           Address offset: 0x64 */
    __IO uint32_t HWInit_Preset2_0; /*!< SDIO Host? HWInit SRS26 preset for slot #0           Address offset: 0x68 */
    __IO uint32_t HWInit_Preset3_0; /*!< SDIO Host? HWInit SRS27 preset for slot #0           Address offset: 0x6C */
    __IO uint32_t HWInit_Preset0_1; /*!< SDIO Host? HWInit SRS24 preset for slot #1           Address offset: 0x70 */
    __IO uint32_t HWInit_Preset1_1; /*!< SDIO Host? HWInit SRS25 preset for slot #1           Address offset: 0x74 */
    __IO uint32_t HWInit_Preset2_1; /*!< SDIO Host? HWInit SRS26 preset for slot #1           Address offset: 0x78 */
    __IO uint32_t HWInit_Preset3_1; /*!< SDIO Host? HWInit SRS27 preset for slot #1           Address offset: 0x7C */
    __IO uint32_t HWInit_Preset0_2; /*!< SDIO Host? HWInit SRS24 preset for slot #2           Address offset: 0x80 */
    __IO uint32_t HWInit_Preset1_2; /*!< SDIO Host? HWInit SRS25 preset for slot #2           Address offset: 0x84 */
    __IO uint32_t HWInit_Preset2_2; /*!< SDIO Host? HWInit SRS26 preset for slot #2           Address offset: 0x88 */
    __IO uint32_t HWInit_Preset3_2; /*!< SDIO Host? HWInit SRS27 preset for slot #2           Address offset: 0x8C */
    __IO uint32_t HWInit_Preset0_3; /*!< SDIO Host? HWInit SRS24 preset for slot #3           Address offset: 0x90 */
    __IO uint32_t HWInit_Preset1_3; /*!< SDIO Host? HWInit SRS25 preset for slot #3           Address offset: 0x94 */
    __IO uint32_t HWInit_Preset2_3; /*!< SDIO Host? HWInit SRS26 preset for slot #3           Address offset: 0x98 */
    __IO uint32_t HWInit_Preset3_3; /*!< SDIO Host? HWInit SRS27 preset for slot #3           Address offset: 0x9C */
    __IO uint32_t HWInit_SBCR0;     /*!< SDIO Host? HWInit SRS56 preset for slot #0           Address offset: 0xA0 */
    __IO uint32_t HWInit_SBCR1;     /*!< SDIO Host? HWInit SRS56 preset for slot #1           Address offset: 0xA4 */
    __IO uint32_t HWInit_SBCR2;     /*!< SDIO Host? HWInit SRS56 preset for slot #2           Address offset: 0xA8 */
    __IO uint32_t HWInit_SBCR3;     /*!< SDIO Host? HWInit SRS56 preset for slot #3           Address offset: 0xAC */
} SDIO_HostTypeDef;

typedef struct
{
    union
    {
        __IO uint32_t SAR;          /*!< SDIO Slot?System address                       Address offset: Offset+0x00 */
        __IO uint32_t ARG2;         /*!< SDIO Slot?Argument #2                          Address offset: Offset+0x00 */
    };
    __IO uint32_t BLOCK_CSR;        /*!< SDIO Slot?Block count and size                 Address offset: Offset+0x04 */
    __IO uint32_t ARG1;             /*!< SDIO Slot?Argument #1 register                 Address offset: Offset+0x08 */
    union
    {
        __IO uint32_t TMR;         /*!< SDIO Slot?Transfer mode                        Address offset: Offset+0x0C */
        __IO uint32_t CIR;         /*!< SDIO Slot?command information                  Address offset: Offset+0x0C */
    };
    __IO uint32_t RESP0;            /*!< SDIO Slot?Response #0                          Address offset: Offset+0x10 */
    __IO uint32_t RESP1;            /*!< SDIO Slot?Response #1                          Address offset: Offset+0x14 */
    __IO uint32_t RESP2;            /*!< SDIO Slot?Response #2                          Address offset: Offset+0x18 */
    __IO uint32_t RESP3;            /*!< SDIO Slot?Response #3                          Address offset: Offset+0x1C */
    __IO uint32_t BDPR;             /*!< SDIO Slot?Buffer data port                     Address offset: Offset+0x20 */
    __IO uint32_t PSR;              /*!< SDIO Slot?Present state                        Address offset: Offset+0x24 */
    __IO uint32_t HCSR0;            /*!< SDIO Slot?Host control settings #0             Address offset: Offset+0x28 */
    __IO uint32_t HCSR1;            /*!< SDIO Slot?Host control settings #1             Address offset: Offset+0x2C */
    __IO uint32_t ISR;              /*!< SDIO Slot?Interrupt status                     Address offset: Offset+0x30 */
    __IO uint32_t ISER;             /*!< SDIO Slot?Interrupt Status Enable Register     Address offset: Offset+0x34 */
    __IO uint32_t IER;              /*!< SDIO Slot?Interrupt Enable Register            Address offset: Offset+0x38 */
    union
    {
        __IO uint32_t AutoCMD12_ESR; /*!< SDIO Slot?Auto CMD12 Error status               Address offset: Offset+0x3C */
        __IO uint32_t HCSR2;         /*!< SDIO Slot?Host control register #2              Address offset: Offset+0x3C */
    };
    __IO uint32_t Capabilities0;    /*!< SDIO Slot?Capabilities #0                      Address offset: Offset+0x40 */
    __IO uint32_t Capabilities1;    /*!< SDIO Slot?Capabilities #1                      Address offset: Offset+0x44 */
    __IO uint32_t Capabilities2;    /*!< SDIO Slot?Capabilities #2                      Address offset: Offset+0x48 */
    __IO uint32_t Reserved;         /*!< SDIO Slot?Reserved register                    Address offset: Offset+0x4C */
    __IO uint32_t FETR;             /*!< SDIO Slot?Event Trigger Register               Address offset: Offset+0x50 */
    __IO uint32_t ADMA_ESR;         /*!< SDIO Slot?ADMA error status                    Address offset: Offset+0x54 */
    __IO uint32_t ADMA_SAR;         /*!< SDIO Slot?ADMA system address                  Address offset: Offset+0x58 */
    __IO uint32_t Reserved1;        /*!< SDIO Slot?Reserved1 register                   Address offset: Offset+0x5C */
    __IO uint32_t PVR0;             /*!< SDIO Slot?Preset Values #0                     Address offset: Offset+0x60 */
    __IO uint32_t PVR1;             /*!< SDIO Slot?Preset Values #1                     Address offset: Offset+0x64 */
    __IO uint32_t PVR2;             /*!< SDIO Slot?Preset Values #2                     Address offset: Offset+0x68 */
    __IO uint32_t PVR3;             /*!< SDIO Slot?Preset Values #3                     Address offset: Offset+0x6C */
    __IO uint32_t Reserved2[0x1C];  /*!< SDIO Slot?Reserved2 register                   Address offset: Offset+[0x70~0xDC] */
    __IO uint32_t SBCR;             /*!< SDIO Slot?Shared Bus Control                   Address offset: Offset+0xE0 */
    __IO uint32_t Reserved3[3];     /*!< SDIO Slot?Reserved2 register                   Address offset: Offset+[0xE4~0xEC] */
    __IO uint32_t Reserved4[3];     /*!< SDIO Slot?Reserved2 register                   Address offset: Offset+[0xF0~0xF8] */
    union
    {
        __IO uint32_t C_HVR;        /*!< SDIO Common?Host version                       Address offset: Offset+0xFC */
        __IO uint32_t C_ISR;        /*!< SDIO Common?Interrupt status                   Address offset: Offset+0xFC */
    };
} SDIO_SlotTypeDef;

typedef struct
{
    SDIO_HostTypeDef *Host;/*!< SD Host registers base address      */
    SDIO_SlotTypeDef *Slot;/*!< SD Slot registers base address      */
}SDIO_TypeDef;

/**
  * @brief DW_apb_i2s_mem_map/DW_apb_i2s_addr_block1
  */
typedef struct
{
    union
    {
        __IO uint32_t LRBR;            /*!< I2S?Left Receive Buffer Register x                 Address offset: 0x020+0x40*x */
        __IO uint32_t LTHR;            /*!< I2S?Left Transmit Holding Register x               Address offset: 0x020+0x40*x */
    };
    union
    {
        __IO uint32_t RRBR;            /*!< I2S?Right Receive Buffer Register x                Address offset: 0x024+0x40*x */
        __IO uint32_t RTHR;            /*!< I2S?Right Transmit Holding Register x              Address offset: 0x024+0x40*x */
    };
    __IO uint32_t RER;              /*!< I2S?Receive Enable Register x                      Address offset: 0x028+0x40*x */
    __IO uint32_t TER;              /*!< I2S?Transmit Enable Register x                     Address offset: 0x02C+0x40*x */
    __IO uint32_t RCR;              /*!< I2S?Receive Configuration Register x               Address offset: 0x030+0x40*x */
    __IO uint32_t TCR;              /*!< I2S?Transmit Configuration Register x              Address offset: 0x034+0x40*x */
    __IO uint32_t ISR;              /*!< I2S?Interrupt status Register x                    Address offset: 0x038+0x40*x */
    __IO uint32_t IMR;              /*!< I2S?Interrupt Mask Register x                      Address offset: 0x03C+0x40*x */
    __IO uint32_t ROR;              /*!< I2S?Receive Overrun Register x                     Address offset: 0x040+0x40*x */
    __IO uint32_t TOR;              /*!< I2S?Transmit Overrun Register x                    Address offset: 0x044+0x40*x */
    __IO uint32_t RFCR;             /*!< I2S?Receive FIFO Configuration Register x          Address offset: 0x048+0x40*x */
    __IO uint32_t TFCR;             /*!< I2S?Transmit FIFO Configuration Register x         Address offset: 0x04C+0x40*x */
    __IO uint32_t RFF;              /*!< I2S?Receive FIFO Flush Register x                  Address offset: 0x050+0x40*x */
    __IO uint32_t TFF;              /*!< I2S?Transmit FIFO Flush Register x                 Address offset: 0x054+0x40*x */
    __IO uint32_t Reserved[2];      /*!< I2S?Reserved Register                              Address offset: 0x058+0x40*x ~ 0x05C+0x40*x */
}I2SChannel_TypeDef;
typedef struct
{
    __IO uint32_t IER;              /*!< I2S?DW_apb_i2s Enable Register                     Address offset: 0x00 */
    __IO uint32_t IRER;             /*!< I2S?I2S Receiver Block Enable Register             Address offset: 0x04 */
    __IO uint32_t ITER;             /*!< I2S?I2S Transmitter Block Enable Register          Address offset: 0x08 */
    __IO uint32_t CER;              /*!< I2S?Clock Enable Register                          Address offset: 0x0C */
    __IO uint32_t CCR;              /*!< I2S?Clock Configuration Register                   Address offset: 0x10 */
    __IO uint32_t RXFFR;            /*!< I2S?Receiver Block FIFO Reset Register             Address offset: 0x14 */
    __IO uint32_t TXFFR;            /*!< I2S?Transmitter Block FIFO Reset Register          Address offset: 0x18 */
    __IO uint32_t Reserved0;        /*!< I2S?Reserved0 Register                             Address offset: 0x1C */
    I2SChannel_TypeDef Chx[4];      /*!< I2S?Channelx Register                              Address offset: 0x020+0x40*x ~ 0x05C+0x40*x
                                                                                             0x020 ~ 0x11C>!*/
    __IO uint32_t Reserved1[40];    /*!< I2S?Reserved1 Register                             Address offset: 0x120~0x1BC */
    __IO uint32_t RXDMA;            /*!< I2S?Receiver Block DMA Register                    Address offset: 0x1C0 */
    __IO uint32_t RRXDMA;           /*!< I2S?Reset Receiver Block DMA Register              Address offset: 0x1C4 */
    __IO uint32_t TXDMA;            /*!< I2S?Transmitter Block DMA Register                 Address offset: 0x1C8 */
    __IO uint32_t RTXDMA;           /*!< I2S?Reset Transmitter Block DMA Register           Address offset: 0x1CC */
    __IO uint32_t Reserved2[8];     /*!< I2S?Reserved2 Register                             Address offset: 0x1D0~0x1EC */
    __IO uint32_t I2S_COMP_PARAM_2; /*!< I2S?Component Parameter Register 2                 Address offset: 0x1F0 */
    __IO uint32_t I2S_COMP_PARAM_1; /*!< I2S?Component Parameter Register 1                 Address offset: 0x1F4 */
    __IO uint32_t I2S_COMP_VERSION; /*!< I2S?I2S Component Version Register                 Address offset: 0x1F8 */
    __IO uint32_t I2S_COMP_TYPE;    /*!< I2S?I2S Component Type Register                    Address offset: 0x1FC */
    __IO uint32_t DMACR;            /*!< I2S?DMA Control Register                           Address offset: 0x200 */
    __IO uint32_t RXDMA_CHx[4];     /*!< I2S?Receiver Block DMA Register                    Address offset: 0x204 ~ 0x210 */
    __IO uint32_t TXDMA_CHx[4];     /*!< I2S?Transmitter Block DMA Register                 Address offset: 0x214 ~ 0x220 */
}I2S_TypeDef;

typedef struct
{
    __IO uint32_t CR;           /*!< I2C?Control Register.                                      Address offset: 0x00 */
    __IO uint32_t TAR;          /*!< I2C?Target Address Register.                               Address offset: 0x04 */
    __IO uint32_t SAR;          /*!< I2C?Slave Address Register.                                Address offset: 0x08 */
    __IO uint32_t HS_MADDR;     /*!< I2C?High Speed Master Mode Code Address Register.          Address offset: 0x0C */
    __IO uint32_t DATA_CMD;     /*!< I2C?Rx/Tx Data Buffer and Command Register.                Address offset: 0x10 */
    union
    {
        __IO uint32_t SS_SCL_HCNT;  /*!< I2C?Standard Speed I2C Clock SCL High Count Register.      Address offset: 0x14 */
        __IO uint32_t UFM_SCL_HCNT; /*!< I2C?Ultra-Fast Speed I2C Clock SCL High Count Register.    Address offset: 0x14 */
    };
    union
    {
        __IO uint32_t SS_SCL_LCNT;  /*!< I2C?Standard Speed I2C Clock SCL Low Count Register.       Address offset: 0x18 */
        __IO uint32_t UFM_SCL_LCNT; /*!< I2C?Ultra-Fast Speed I2C Clock SCL Low Count Register.     Address offset: 0x18 */
    };
    union
    {
        __IO uint32_t FS_SCL_HCNT;  /*!< I2C?Fast Mode or Fast Mode Plus I2C Clock SCL High Count Register.     Address offset: 0x1C */
        __IO uint32_t UFM_TBUF_CNT ;/*!< I2C?Ultra-Fast Speed mode TBuf Idle Count Register.                    Address offset: 0x1C */
    };
    __IO uint32_t FS_SCL_LCNT;  /*!< I2C?Fast Mode or Fast Mode Plus I2C Clock SCL Low Count Register.      Address offset: 0x20 */
    __IO uint32_t HS_SCL_HCNT;  /*!< I2C?High Speed I2C Clock SCL High Count Register.                      Address offset: 0x24 */
    __IO uint32_t HS_SCL_LCNT;  /*!< I2C?High Speed I2C Clock SCL Low Count Register.                       Address offset: 0x28 */
    __IO uint32_t ISR;          /*!< I2C?Interrupt Status Register.                                         Address offset: 0x2C */
    __IO uint32_t IMR;          /*!< I2C?Interrupt Mask Register.                                           Address offset: 0x30 */
    __IO uint32_t RISR;         /*!< I2C?Raw Interrupt Status Register.                                     Address offset: 0x34 */
    __IO uint32_t RX_TL;        /*!< I2C?Receive FIFO Threshold Register.                                   Address offset: 0x38 */
    __IO uint32_t TX_TL;        /*!< I2C?Transmit FIFO Threshold Register.                                  Address offset: 0x3C */
    __IO uint32_t CLR_INTR;     /*!< I2C?Clear Combined and Individual Interrupt Register.                  Address offset: 0x40 */
    __IO uint32_t CLR_RX_UNDER; /*!< I2C?Clear RX_UNDER Interrupt Register.                                 Address offset: 0x44 */
    __IO uint32_t CLR_RX_OVER;  /*!< I2C?Clear RX_OVER Interrupt Register.                                  Address offset: 0x48 */
    __IO uint32_t CLR_TX_OVER;  /*!< I2C?Clear TX_OVER Interrupt Register.                                  Address offset: 0x4C */
    __IO uint32_t CLR_RD_REQ;   /*!< I2C?Clear RD_REQ Interrupt Register.                                   Address offset: 0x50 */
    __IO uint32_t CLR_TX_ABRT;  /*!< I2C?Clear TX_ABRT Interrupt Register.                                  Address offset: 0x54 */
    __IO uint32_t CLR_RX_DONE;  /*!< I2C?Clear RX_DONE Interrupt Register.                                  Address offset: 0x58 */
    __IO uint32_t CLR_ACTIVITY; /*!< I2C?Clear ACTIVITY Interrupt Register.                                 Address offset: 0x5C */
    __IO uint32_t CLR_STOP_DET; /*!< I2C?Clear STOP_DET Interrupt Register.                                 Address offset: 0x60 */
    __IO uint32_t CLR_START_DET;/*!< I2C?Clear START_DET Interrupt Register.                                Address offset: 0x64 */
    __IO uint32_t CLR_GEN_CALL; /*!< I2C?Clear GEN_CALL Interrupt Register.                                 Address offset: 0x68 */
    __IO uint32_t ENR;          /*!< I2C?Enable Register.                                                   Address offset: 0x6C */
    __IO uint32_t SR;           /*!< I2C?Status Register.                                                   Address offset: 0x70 */
    __IO uint32_t TXFLR;        /*!< I2C?Transmit FIFO Level Register.                                      Address offset: 0x74 */
    __IO uint32_t RXFLR;        /*!< I2C?Receive FIFO Level Register.                                       Address offset: 0x78 */
    __IO uint32_t SDA_HOLD;     /*!< I2C?SDA Hold Time Length Register.                                     Address offset: 0x7C */
    __IO uint32_t TX_ABRT_SOURCE;       /*!< I2C?Transmit Abort Source Register.                            Address offset: 0x80 */
    __IO uint32_t SLV_DATA_NACK_ONLY;   /*!< I2C?Generate Slave Data NACK Register.                         Address offset: 0x84 */
    __IO uint32_t DMA_CR;               /*!< I2C?DMA Control Register.                                      Address offset: 0x88 */
    __IO uint32_t DMA_TDLR;             /*!< I2C?DMA Transmit Data Level Register.                          Address offset: 0x8C */
    __IO uint32_t DMA_RDLR;             /*!< I2C?Receive Data Level Register.                               Address offset: 0x90 */
    __IO uint32_t SDA_SETUP;            /*!< I2C?SDA Setup Register This register.                          Address offset: 0x94 */
    __IO uint32_t ACK_GENERAL_CALL;     /*!< I2C?ACK General Call Register.                                 Address offset: 0x98 */
    __IO uint32_t ENSR;                 /*!< I2C?Enable Status Register.                                    Address offset: 0x9C */
    union
    {
        __IO uint32_t FS_SPKLEN;        /*!< I2C?SS, FS or FM+ spike suppression limit Register.            Address offset: 0xA0 */
        __IO uint32_t UFM_SPKLEN;       /*!< I2C?UFM spike suppression limit Register.                      Address offset: 0xA0 */
    };
    __IO uint32_t HS_SPKLEN;            /*!< I2C?HS spike suppression limit register .                              Address offset: 0xA4 */
    __IO uint32_t CLR_RESTART_DET;      /*!< I2C?Clear RESTART_DET Interrupt Register.                              Address offset: 0xA8 */
    __IO uint32_t SCL_STUCK_AT_LOW_TIMEOUT; /*!< I2C?SCL Stuck at Low Timeout Register(Reserved).                   Address offset: 0xAC */
    __IO uint32_t SDA_STUCK_AT_LOW_TIMEOUT; /*!< I2C?SDA Stuck at Low Timeout Register(Reserved).                   Address offset: 0xB0 */
    __IO uint32_t CLR_SCL_STUCK_DET;        /*!< I2C?Clear SCL Stuck at Low Detect Interrupt Register(Reserved).    Address offset: 0xB4 */
    __IO uint32_t DEVICE_ID;                /*!< I2C?Device-ID Register(Reserved).                                  Address offset: 0xB8 */
    __IO uint32_t SMBUS_CLK_LOW_SEXT;           /*!< I2C?SMBus Slave Clock Extend Timeout Register(Reserved).           Address offset: 0xBC */
    __IO uint32_t SMBUS_CLK_LOW_MEXT;           /*!< I2C?SMBus Master Clock Extend Timeout Register(Reserved).          Address offset: 0xC0 */
    __IO uint32_t SMBUS_THIGH_MAX_IDLE_COUNT;   /*!< I2C?SMBus Master THigh MAX Bus-idle count Register(Reserved).      Address offset: 0xC4 */
    __IO uint32_t SMBUS_ISR;                    /*!< I2C?SMBUS Interrupt Status Register(Reserved).                     Address offset: 0xC8 */
    __IO uint32_t SMBUS_IMR;                    /*!< I2C?SMBus Interrupt Mask Register(Reserved).                       Address offset: 0xCC */
    __IO uint32_t SMBUS_RISR;                   /*!< I2C?SMBus Raw Interrupt Status Register(Reserved).                 Address offset: 0xD0 */
    __IO uint32_t SMBUS_CIR;                    /*!< I2C?SMBus Clear Interrupt Register(Reserved).                      Address offset: 0xD4 */
    __IO uint32_t OPTIONAL_SAR;                 /*!< I2C?Optional Slave Address Register(Reserved).                     Address offset: 0xD8 */
    union
    {
        __IO uint32_t SMBUS_UDID_LSB;           /*!< I2C?SMBUS ARP UDID LSB Register(Reserved).                         Address offset: 0xDC */
        __IO uint32_t SMBUS_UDID_WORD0;         /*!< I2C?SMBUS UDID WORD0 Register(Reserved).                           Address offset: 0xDC */
    };
    __IO uint32_t SMBUS_UDID_WORD1;             /*!< I2C?SMBUS UDID WORD1 Register(Reserved).                           Address offset: 0xE0 */
    __IO uint32_t SMBUS_UDID_WORD2;             /*!< I2C?SMBUS UDID WORD2 Register(Reserved).                           Address offset: 0xE4 */
    __IO uint32_t SMBUS_UDID_WORD3;             /*!< I2C?SMBUS UDID WORD3 Register(Reserved).                           Address offset: 0xE8 */
    __IO uint32_t Reserved;                     /*!< I2C?Reserved Register.                                             Address offset: 0xEC */
    __IO uint32_t REG_TIMEOUT_RST;  /*!< I2C?Register timeout counter reset register(Reserved).             Address offset: 0xF0 */
    __IO uint32_t COMP_PARAM_1;     /*!< I2C?Component Parameter Register 1.                                Address offset: 0xF4 */
    __IO uint32_t COMP_VERSION;     /*!< I2C?Component Version Register.                                    Address offset: 0xF8 */
    __IO uint32_t COMP_TYPE;        /*!< I2C?Component Type Register.                                       Address offset: 0xFC */
}I2C_TypeDef;

typedef struct
{
    __IO uint32_t POWER;
    __IO uint32_t ADC;
    __IO uint32_t ADC_CH1;
    __IO uint32_t ADC_CH2;
    __IO uint32_t ADC_CH3;
    __IO uint32_t DAC;
    __IO uint32_t IIS;
}CODEC_TypeDef;


typedef struct
{
    __IO uint32_t CR;
    __IO uint32_t DATE;
    __IO uint32_t TIME;
    __IO uint32_t INTR_TIME;
    __IO uint32_t IMR;
    __IO uint32_t ISR;
}RTC_TypeDef;

typedef struct
{
    __IO uint32_t DELAY_TIME;
    __IO uint32_t WEAKUP_MASK;
    __IO uint32_t RST_STATUS;
    __IO uint32_t ADJ_32K;
    __IO uint32_t Reserved;
    __IO uint32_t EXT_POWER;
    __IO uint32_t ADJ_MISC;
    __IO uint32_t ADJ_LDO;
    __IO uint32_t INTER_PWR_SEL;
}PMU_TypeDef;

typedef struct
{
    __IO uint32_t CR;           //0x00
    __IO uint32_t DRIR;         //0x04
    __IO uint32_t DWICR;        //0x08
    __IO uint32_t DDR;          //0x0C
    __IO uint32_t RDCAPR;       //0x10
    __IO uint32_t DSCR;         //0x14
    __IO uint32_t SRAMPSR;      //0x18
    __IO uint32_t IAHBATR;      //0x1C
    __IO uint32_t DMAPCR;       //0x20
    __IO uint32_t RAR;          //0x24
    __IO uint32_t MCR;          //0x28
    __IO uint32_t SRAMFLR;      //0x2C
    __IO uint32_t TXTR;         //0x30
    __IO uint32_t RXTR;         //0x34
    __IO uint32_t WCCR;         //0x38
    __IO uint32_t PER;          //0x3C
    __IO uint32_t ISR;          //0x40
    __IO uint32_t IMR;          //0x44
    __IO uint32_t Reserved0;    //0x48
    __IO uint32_t Reserved1;    //0x4C
    __IO uint32_t LWPR;         //0x50
    __IO uint32_t UWPR;         //0x54
    __IO uint32_t WPR;          //0x58
    __IO uint32_t Reserved2;    //0x5C
    __IO uint32_t IRTCR;        //0x60
    __IO uint32_t IRTWR;        //0x64
    __IO uint32_t IRTSAR;       //0x68
    __IO uint32_t IRTNR;        //0x6C
    __IO uint32_t IWTCR;        //0x70
    __IO uint32_t IWTWR;        //0x74
    __IO uint32_t IWTSAR;       //0x78
    __IO uint32_t IWTNR;        //0x7C
    __IO uint32_t ITARR;        //0x80
    __IO uint32_t Reserved3;    //0x84
    __IO uint32_t Reserved4;    //0x88
    __IO uint32_t Reserved5;    //0x8C
    __IO uint32_t FCCR;         //0x90
    __IO uint32_t FCAR;         //0x94
    __IO uint32_t Reserved6;    //0x98
    __IO uint32_t Reserved7;    //0x9C
    __IO uint32_t FCRLDR;       //0xA0
    __IO uint32_t FCRUDR;       //0xA4
    __IO uint32_t FCWLDR;       //0xA8
    __IO uint32_t FCWUDR;       //0xAC
    __IO uint32_t PFSR;         //0xB0
    __IO uint32_t Reserved8[14];//0xB4~0xF8
    __IO uint32_t MIDR;         //0xFC
}QSPI_TypeDef;
typedef struct
{
#if((defined DRAM_CACHE_ENABLED) && (DRAM_CACHE_ENABLED == 1))
    union
    {
#endif
        QSPI_TypeDef QSPI_REG;
#if((defined DRAM_CACHE_ENABLED) && (DRAM_CACHE_ENABLED == 1))
        struct{
            __IO uint32_t Reserved[48];     //0x00~0xBC
            __IO uint32_t CMDInfo;          //0xC0
            __IO uint32_t PageStartAddr;    //0xC4
            __IO uint32_t PageInfo;         //0xC8
            __IO uint32_t BaseAddr;         //0xCC
            __IO uint32_t FlashRdDelay;     //0xD0
            __IO uint32_t DramCacheFlush;   //0xD4
            __IO uint32_t Gzip_EN;          //0xD8
            __IO uint32_t RequestNUM;       //0xDC
            __IO uint32_t LastRequestAddr;  //0xE0
            __IO uint32_t SingleWay_EN;     //0xE4
            __IO uint32_t MaxFlashRDCycle;  //0xE8
            __IO uint32_t FlashRDInfTine;   //0xEC
            __IO uint32_t FlashRDBL;        //0xF0
            __IO uint32_t FlashRDErrAddr;   //0xF4
            __IO uint32_t FlashRDErrBurst;  //0xF8
            __IO uint32_t FlashRDAddrOffset;//0xFC
            __IO uint32_t DMAStart;         //0x100
            __IO uint32_t DMAStartAddr;     //0x104
            __IO uint32_t DMABurstLen;      //0x108
            __IO uint32_t DMADestinyAddr;   //0x10C
            __IO uint32_t DMAInt;           //0x110
            __IO uint32_t GzipBaseAddr;     //0x114
            __IO uint32_t OverTimeMaxCnt;   //0x118
        };
    };
#endif
}QSPI_DRAM_CACHE_TypeDef;
typedef struct
{
    uint8_t reg;
    uint8_t Reserved[3];
}CAN_REG;
typedef struct
{
    __IO CAN_REG ACR0;          /*!< CAN Filter bank register 1 */
    __IO CAN_REG ACR1;          /*!< CAN Filter bank register 1 */
    __IO CAN_REG ACR2;          /*!< CAN Filter bank register 1 */
    __IO CAN_REG ACR3;          /*!< CAN Filter bank register 1 */
    __IO CAN_REG AMR0;          /*!< CAN Filter bank register 1 */
    __IO CAN_REG AMR1;          /*!< CAN Filter bank register 1 */
    __IO CAN_REG AMR2;          /*!< CAN Filter bank register 1 */
    __IO CAN_REG AMR3;          /*!< CAN Filter bank register 1 */
} CAN_FilterRegister_TypeDef;

typedef struct
{/* 13*4 bytes */
    union
    {
        __IO CAN_REG val;
        struct{
            uint32_t DLC    : 4;
            uint32_t X      : 2;
            uint32_t RTR    : 1;
            uint32_t IDE    : 1;
            uint32_t        : 24;
        }bits;
    }FrameCtrlField;
    union
    {
        __IO CAN_REG val;
        struct{
            uint32_t ID10_3 : 8;
            uint32_t        : 24;
        }bits;
    }FrameIdentifier1;
    union
    {
        __IO CAN_REG val;
        struct{
            uint32_t        : 5;
            uint32_t ID2_0  : 3;
            uint32_t        : 24;
        }bits;
    }FrameIdentifier2;
    __IO CAN_REG DATA[8];
}CAN_StdFrame_TypeDef;
typedef struct
{/* 13*4 bytes */
    union
    {
        __IO CAN_REG val;
        struct{
            uint32_t DLC    : 4;
            uint32_t X      : 2;
            uint32_t RTR    : 1;
            uint32_t IDE    : 1;
            uint32_t        : 24;
        }bits;
    }FrameCtrlField;
    union
    {
        __IO CAN_REG val;
        struct{
            uint32_t ID28_21    : 8;
            uint32_t            : 24;
        }bits;
    }FrameIdentifier1;
    union
    {
        __IO CAN_REG val;
        struct{
            uint32_t ID20_13    : 8;
            uint32_t            : 24;
        }bits;
    }FrameIdentifier2;
    union
    {
        __IO CAN_REG val;
        struct{
            uint32_t ID12_5 : 8;
            uint32_t        : 24;
        }bits;
    }FrameIdentifier3;
    union
    {
        __IO CAN_REG val;
        struct{
            uint32_t        : 3;
            uint32_t ID4_0  : 5;
            uint32_t        : 24;
        }bits;
    }FrameIdentifier4;
    __IO CAN_REG DATA[8];
}CAN_ExtFrame_TypeDef;

typedef struct
{
    __IO CAN_REG MOD;           /* Offset: 0x000 Mode Register (R/W) */
    __IO CAN_REG CMR;           /* Offset: 0x004 Command Register (R/W) */
    __IO CAN_REG SR;            /* Offset: 0x008 Status Register (R/W) */
    __IO CAN_REG IR;            /* Offset: 0x00c Interrupt Register (R/W) */
    __IO CAN_REG IER;           /* Offset: 0x010 Interrupt Enable Register (R/W) */
    __IO CAN_REG Reserved14;    /* Offset: 0x014 */
    __IO CAN_REG BTR0;          /* Offset: 0x018 Bus Timing Register 0 (R/W) */
    __IO CAN_REG BTR1;          /* Offset: 0x01c Bus Timing Register 1 (R/W) */
    __IO CAN_REG OCR;           /* Offset: 0x020 Output Control Register (R/W) */
    __IO CAN_REG Reserved24;    /* Offset: 0x024 */
    __IO CAN_REG Reserved28;    /* Offset: 0x028 */
    __IO CAN_REG ALC;           /* Offset: 0x02C Arbitration Lost Capture Register (R/W) */
    __IO CAN_REG ECC;           /* Offset: 0x030 Error Code Capture Register (R/W) */
    __IO CAN_REG EWLR;          /* Offset: 0x034 Error Warning Limit Register (R/W) */
    __IO CAN_REG RXERR;         /* Offset: 0x038 Receive Error Counter Register (R/W) */
    __IO CAN_REG TXERR;         /* Offset: 0x03C Transmit Error Counter Register (R/W) */
    union{
        CAN_FilterRegister_TypeDef FilterRegister0; /* Offset: 0x40 ~ 0x5C Filter Register 0 */
        CAN_StdFrame_TypeDef TxBuffStd_WO;        /* Offset: 0x40 ~ 0x5C,0x60 ~ 0x68 Standard Frame Transmit Buffer(WO) */
        CAN_ExtFrame_TypeDef TxBuffExt_WO;        /* Offset: 0x40 ~ 0x5C,0x60 ~ 0x70 Extended Frame Transmit Buffer(WO) */
        CAN_StdFrame_TypeDef RxBuffStd_RO;        /* Offset: 0x40 ~ 0x5C,0x60 ~ 0x68 Standard Frame Receive Buffer(RO) */
        CAN_ExtFrame_TypeDef RxBuffExt_RO;        /* Offset: 0x40 ~ 0x5C,0x60 ~ 0x70 Extended Frame Receive Buffer(RO) */
    };
    __IO CAN_REG RMC;               /* Offset: 0x074 Receive Message Counter Register (R/W) */
    __IO CAN_REG RBSA;              /* Offset: 0x078 Receive Buffer Start Register (R/W) */
    __IO CAN_REG CDR;               /* Offset: 0x07C Clock Divider Register (R/W) */
    __IO CAN_REG RFIFO[64];         /* Offset: 0x080~0x17C */
    union
    {
        __IO CAN_REG TRANSBUFFER[13];           /* Offset: 0x180 ~ 0x1B0 (R/W) */
        CAN_StdFrame_TypeDef TxBuffStd_RO;    /* Offset: 0x180 ~ 0x1A8 Standard Frame Transmit Buffer(RO) */
        CAN_ExtFrame_TypeDef TxBuffExt_RO;    /* Offset: 0x180 ~ 0x1B0 Extended Frame Transmit Buffer(RO) */
    };
    __IO CAN_REG Reserved_1B4_1fc[19];      /* Offset: 0x1B4~0x1fc */
    CAN_FilterRegister_TypeDef FilterRegisterX[14]; /* Offset: 0x200 ~ 0x3BC Filter Register Buff */
    __IO CAN_REG Reserved_3C0_3F8[15];      /* Offset: 0x3C0~0x3F8 */
    __IO CAN_REG EX_REG_EN;         /* Offset: 0x3FC Ext REG En Register (R/W) */
    CAN_FilterRegister_TypeDef FilterRegisterX_Ext[15]; /* Offset: 0x400 ~ 0x4C0 Extended Filter Register Buff*/
}CAN_TypeDef;

union CANFD_RX_FrameIdentifier
{
    __IO uint32_t reg_u32val;
    struct{
        uint32_t ID10_0 : 11;
        uint32_t        : 20;
        uint32_t ESI    : 1;
    }std_bits;
    struct{
        uint32_t ID28_0 : 29;
        uint32_t        : 2;
        uint32_t ESI    : 1;
    }ext_bits;
};
union CANFD_RX_FrameCtrlField
{
    __IO uint32_t reg_u32val;
    struct{
        uint32_t DLC        : 4;
        uint32_t BRS        : 1;
        uint32_t FDF        : 1;
        uint32_t RTR        : 1;
        uint32_t IDE        : 1;
        uint32_t            : 4;
        uint32_t TX         : 1;/* Loopback flag */
        uint32_t KOER       : 3;
        uint32_t CYCLE_TIME : 16;
    }bits;
};
typedef struct
{/* 18*4 bytes */
    union CANFD_RX_FrameIdentifier FrameIdentifier;
    union CANFD_RX_FrameCtrlField FrameCtrlField;
    __IO uint32_t DATA[16];
    __IO uint32_t RTS_LSB;/* Reception Time Stamps Bit[31:0] */
    __IO uint32_t RTS_MSB;/* Reception Time Stamps Bit[63:32] */
}CANFD_RXFrame_TypeDef;
union CANFD_TX_FrameIdentifier
{
    __IO uint32_t reg_u32val;
    struct{
        uint32_t ID10_0 : 11;
        uint32_t        : 20;
        uint32_t TTSEN  : 1;
    }std_bits;
    struct{
        uint32_t ID28_0 : 29;
        uint32_t        : 2;
        uint32_t TTSEN  : 1;
    }ext_bits;
};
union CANFD_TX_FrameCtrlField
{
    __IO uint32_t reg_u32val;
    struct{
        uint32_t DLC : 4;
        uint32_t BRS : 1;
        uint32_t FDF : 1;
        uint32_t RTR : 1;
        uint32_t IDE : 1;
        uint32_t     : 24;
    }bits;
};
typedef struct
{/* 18*4 bytes */
    union CANFD_TX_FrameIdentifier FrameIdentifier;
    union CANFD_TX_FrameCtrlField FrameCtrlField;
    __IO uint32_t DATA[16];
}CANFD_TXFrame_TypeDef;
union _CANFD_RegA0
{
    __IO uint32_t value;              //OFFSET:0xa0
    struct
    {
        uint8_t CFG_STAT;
        uint8_t TCMD;
        uint8_t TCTRL;
        uint8_t RCTRL;
    }bits;
};
union _CANFD_RegB0
{
    __IO uint32_t value;              //OFFSET:0xb0
    struct
    {
        uint8_t EALCAP;
        uint8_t TDC;
        uint8_t RECNT;
        uint8_t TECNT;
    }bits;
};
typedef struct
{
    union
    {
        __IO uint32_t RBUF[20];             //OFFSET:0x00~0x4C,(R)
        CANFD_RXFrame_TypeDef RxBuff;
    };
    union
    {
        __IO uint32_t TBUF[18];             //OFFSET:0x50~0x94
        CANFD_TXFrame_TypeDef TxBuff;
    };
    __IO uint32_t TTS0;                     //OFFSET:0x98
    __IO uint32_t TTS1;                     //OFFSET:0x9c
    union _CANFD_RegA0 RCTRL_TCTRL_TCMD_CFGSTAT;//OFFSET:0xa0
    union
    {
        __IO uint32_t LIMIT_ERRINT_RTIF_RTIE;   //OFFSET:0xa4
        struct
        {
            uint8_t RTIE;
            uint8_t RTIF;
            uint8_t ERRINT;
            uint8_t LIMIT;
        }reg_0xA4;
    };
    union
    {
        __IO uint32_t S_PRESC_SJW_Seg2_Seg1;    //OFFSET:0xa8
        struct
        {
            uint8_t S_Seg_1;
            uint8_t S_Seg_2;
            uint8_t S_SJW;
            uint8_t S_PRESC;
        }reg_0xA8;
    };
    union
    {
        __IO uint32_t F_PRESC_SJW_Seg2_Seg1;    //OFFSET:0xac
        struct
        {
            uint8_t F_Seg_1;
            uint8_t F_Seg_2;
            uint8_t F_SJW;
            uint8_t F_PRESC;
        }reg_0xAC;
    };
    union _CANFD_RegB0 TECNT_RECNT_TDC_EALCAP;              //OFFSET:0xb0
    union
    {
        __IO uint32_t ACFEN10_TIMECFG_ACFCTRL;              //OFFSET:0xb4
        struct
        {
            uint8_t ACFCTRL;
            uint8_t TIMECFG;
            uint8_t ACF_EN_0;
            uint8_t ACF_EN_1;
        }reg_0xB4;
    };
    union
    {
        __IO uint32_t ACFx;                 //OFFSET:0xb8
        __IO uint32_t AMFx;                 //OFFSET:0xb8
        struct
        {
            uint32_t _7_0   : 8;
            uint32_t _15_8  : 8;
            uint32_t _23_16 : 8;
            uint32_t _28_24 : 5;
            uint32_t        : 3;
        }ACODE;
        struct
        {
            uint32_t _7_0   : 8;
            uint32_t _15_8  : 8;
            uint32_t _23_16 : 8;
            uint32_t _28_24 : 5;
            uint32_t AIDEE  : 1;
            uint32_t AIDE   : 1;
            uint32_t        : 1;
        }AMODE;
    };
    union
    {
        __IO uint32_t TTCFG_TBSLOT_VER1_VER0;              //OFFSET:0xbc ,TTCFG and TBSLOT Reserved
        struct
        {
            uint8_t VER0;
            uint8_t VER1;
            uint8_t TBSLOT;
            uint8_t TTCFG;
        };
    };
    union
    {
        __IO uint32_t REF_MSG_x;              //OFFSET:0xc0 ,Reserved
        struct
        {
            uint32_t REF_ID_7_0     : 8;
            uint32_t REF_ID_15_8    : 8;
            uint32_t REF_ID_23_16   : 8;
            uint8_t REF_ID_28_24    : 5;
            uint8_t                 : 2;
            uint8_t REF_IDE         : 1;
        }REF_MSG;
    };
    union
    {
        __IO uint32_t TTTRIG10_TRIGCFG10;       //OFFSET:0xc4 ,Reserved
        struct
        {
            uint8_t TRIG_CFG_0;
            uint8_t TRIG_CFG_1;
            uint8_t TT_TRIG_0;
            uint8_t TT_TRIG_1;
        };
    };
    union
    {
        __IO uint32_t TT_WTRIG10;              //OFFSET:0xc8
        struct
        {
            uint8_t TT_WTRIG_0;
            uint8_t TT_WTRIG_1;
            uint16_t Reserved;
        };
    };
}CANFD_TypeDef;

/**
  * @brief Video Cap
  */
typedef struct
{
    __IO uint32_t CR;                   /* Offset: 0x00 */
    union
    {
        __IO uint32_t IER;              /* Offset: 0x04 */
        struct
        {
            __IO uint32_t DrawDoneIT_En         : 1;
            uint32_t                            : 6;
            __IO uint32_t ADCIT_En              : 1;
            __IO uint32_t WriteFullIT_En        : 1;
            __IO uint32_t FrameOversizeErrIT_En : 1;
            uint32_t                            : 22;
        };
    };
    union
    {
        __IO uint32_t ISR;              /* Offset: 0x08 */
        struct
        {
            __IO uint32_t DrawDoneIT         : 1;
            uint32_t                         : 6;
            __IO uint32_t ADCIT              : 1;
            __IO uint32_t WriteFullIT        : 1;
            __IO uint32_t FrameOversizeErrIT : 1;
            uint32_t                         : 22;
        };
    };
    __IO uint32_t MaxSize;              /* Offset: 0x0C */
    union
    {
        __IO uint32_t CH0FieldPara;     /* Offset: 0x10 */
        struct
        {
            __IO uint32_t VideoHeight        : 16;
            __IO uint32_t VideoWidth         : 16;
        };
    };
    __IO uint32_t CH0BaseAddr;          /* Offset: 0x14 */
    __IO uint32_t FrameNumCnt;          /* Offset: 0x18 */
    __IO uint32_t MaxADCFrameNum;       /* Offset: 0x1C */
}VIDEOCAP_TypeDef;

/**
  * @brief Watch Dog Timer
  */
union _WDT_CR
{
	__IO uint32_t reg_u32val;      	
	struct
	{
		__IO uint32_t WDT_EN	: 1;	//WDT Enable
		__IO uint32_t RMOD		: 1;	//Response mode
		__IO uint32_t RPL		: 3;	//Reset Pulse Length
		uint32_t            	: 27;
	}bit;
};
union _WDT_TORR
{
	__IO uint32_t reg_u32val;      	
	struct
	{
		__IO uint32_t TOP		: 4;	//Timeout Period
		__IO uint32_t TOP_INIT	: 4;	//Timeout Period for Initialization
		uint32_t            	: 24;
	}bit;
};
union _WDT_COMP_PARAM_1
{
	__IO uint32_t reg_u32val;
	struct
	{
		__IO uint32_t ALWAYS_EN		: 1;
		__IO uint32_t DFLT_RMOD		: 1;
		__IO uint32_t DUAL_TOP		: 1;
		__IO uint32_t HC_RMOD		: 1;
		__IO uint32_t HC_RPL		: 1;
		__IO uint32_t HC_TOP		: 1;
		__IO uint32_t USE_FIX_TOP	: 1;
		__IO uint32_t PAUSE			: 1;
		__IO uint32_t APB_DATA_WIDTH: 2;
		__IO uint32_t DFLT_RPL		: 3;
		uint32_t            		: 3;
		__IO uint32_t DFLT_TOP		: 4;
		__IO uint32_t DFLT_TOP_INIT	: 4;
		__IO uint32_t CNT_WIDTH		: 4;	//WDT Counter Width
		uint32_t            		: 4;
	}bit;
};
typedef struct
{
    union _WDT_CR 			CR;             /*!< Offset: 0x00   (R/W) */
    union _WDT_TORR 		TORR;           /*!< Offset: 0x04   (R/W) */
	__IO uint32_t 			CCVR;           /*!< Offset: 0x08   (R/W) Current Counter Value Register */
	__IO uint32_t 			CRR;            /*!< Offset: 0x0C   (R/W) Counter Restart Register */
	__IO uint32_t 			ISR;            /*!< Offset: 0x10   (R/W) Interrupt Status Register */
	__IO uint32_t 			ICR;            /*!< Offset: 0x14   (R/W) Interrupt Clear Register */
	__IO uint32_t 			Reserveds[51];  
	__IO uint32_t 			COMP_PARAM_5;   /*!< Offset: 0xE4   (R/W) */
	__IO uint32_t 			COMP_PARAM_4;   /*!< Offset: 0xE8   (R/W) */
	__IO uint32_t 			COMP_PARAM_3;   /*!< Offset: 0xEC   (R/W) */
	__IO uint32_t 			COMP_PARAM_2;   /*!< Offset: 0xF0   (R/W) */
    union _WDT_COMP_PARAM_1 COMP_PARAM_1;	/*!< Offset: 0xF4   (R/W) */
	__IO uint32_t 			COMP_VERSION;   /*!< Offset: 0xF8   (R/W) */
	__IO uint32_t 			COMP_TYPE;      /*!< Offset: 0xFC   (R/W) */
}WDT_TypeDef;

/**
  * @brief ADC
  */
typedef struct
{
    __IO uint32_t START;                     /* Offset: 0x000 Start Adc Convert Register (R/W) */
    __IO uint32_t CLKDIV;                    /* Offset: 0x004 Adc Enable And Clk Div Register (R/W) */
    __IO uint32_t ADCCON;                    /* Offset: 0x008 Adc Channel And Dma Config Register (R/W) */
    __IO uint32_t TEMPCON;                   /* Offset: 0x00c Register (R/W) */
    __IO uint32_t INTR_EN;                   /* Offset: 0x010 Interrupt Enable Register (R/W) */
    __IO uint32_t INTR;                      /* Offset: 0x014 Interrupt Status Register (R/W) */
    __IO uint32_t FULLSIZE;                  /* Offset: 0x018 Full Size Register (R/W) */
    __IO uint32_t CONTSTOP;                  /* Offset: 0x01c Adc Convert Stop Register (R/W) */
    __IO uint32_t DONE;                      /* Offset: 0x020 Adc Convert Done Register (RO) */
    __IO uint32_t DOUT;                      /* Offset: 0x024 Adc Convert result Register (RO) */
    __IO uint32_t FIFOSTATUS;                /* Offset: 0x028 Fifo status Register (RO) */
} ADC_TypeDef;

/**
  * @brief SPI
  */
typedef struct
{
    __IO uint32_t CR0;                       /* Offset: 0x000 In Out Dirction Register (R/W) */
    __IO uint32_t CR1;                       /* Offset: 0x004 Output Register (R/W) */
    __IO uint32_t DR;                        /* Offset: 0x008 Input Register (R/W) */
    __IO uint32_t SR;                        /* Offset: 0x00c Pull Up Controller Register (R/W) */
    __IO uint32_t CPSR;                      /* Offset: 0x010 Pull Down Register (R/W) */
    __IO uint32_t IMSC;                      /* Offset: 0x014 In Out Dirction Register (R/W) */
    __IO uint32_t RIS;                       /* Offset: 0x018 In Out Dirction Register (R/W) */
    __IO uint32_t MIS;                       /* Offset: 0x01c In Out Dirction Register (R/W) */
    __IO uint32_t ICR;                       /* Offset: 0x020 In Out Dirction Register (R/W) */
    __IO uint32_t DMACR;                     /* Offset: 0x024 In Out Dirction Register (R/W) */
    __IO uint32_t RSV0[(0xFE0-0x024)/4-1];   /* Offset: 0x028 In Out Dirction Register (R/W) */
    __IO uint32_t PeriphID0;                 /* Offset: 0x02c IO function Register (R/W) */
    __IO uint32_t PeriphID1;                 /* Offset: 0x030 IO function Register (R/W) */
    __IO uint32_t PeriphID2;
    __IO uint32_t PeriphID3;
    __IO uint32_t PCellID0;
    __IO uint32_t PCellID1;
    __IO uint32_t PCellID2;
    __IO uint32_t PCellID3;
} SPI_TypeDef;

typedef struct
{
    __IO uint32_t MACCR;
    __IO uint32_t MACFFR;
    __IO uint32_t MACHTHR;
    __IO uint32_t MACHTLR;
    __IO uint32_t MACMIIAR;
    __IO uint32_t MACMIIDR;
    __IO uint32_t MACFCR;
    __IO uint32_t MACVLANTR;             /*    8 */
    uint32_t      MACVERR;
    uint32_t      MACDBGR;
    __IO uint32_t MACRWUFFR;             /*   11 */
    __IO uint32_t MACPMTCSR;
    uint32_t      RESERVED0;
    uint32_t      RESERVED1;
    __IO uint32_t MACSR;                 /*   15 */
    __IO uint32_t MACIMR;
    __IO uint32_t MACA0HR;
    __IO uint32_t MACA0LR;
    __IO uint32_t MACA1HR;
    __IO uint32_t MACA1LR;
    __IO uint32_t MACA2HR;
    __IO uint32_t MACA2LR;
    __IO uint32_t MACA3HR;
    __IO uint32_t MACA3LR;               /*   24 */
    uint32_t      RESERVED2[40];
    __IO uint32_t MMCCR;                 /*   65 */
    __IO uint32_t MMCRIR;
    __IO uint32_t MMCTIR;
    __IO uint32_t MMCRIMR;
    __IO uint32_t MMCTIMR;               /*   69 */
    uint32_t      RESERVED3[14];
    __IO uint32_t MMCTGFSCCR;            /*   84 */
    __IO uint32_t MMCTGFMSCCR;
    uint32_t      RESERVED4[5];
    __IO uint32_t MMCTGFCR;
    uint32_t      RESERVED5[10];
    __IO uint32_t MMCRFCECR;
    __IO uint32_t MMCRFAECR;
    uint32_t      RESERVED6[10];
    __IO uint32_t MMCRGUFCR;
    uint32_t      RESERVED7[334];
    __IO uint32_t PTPTSCR;
    __IO uint32_t PTPSSIR;
    __IO uint32_t PTPTSHR;
    __IO uint32_t PTPTSLR;
    __IO uint32_t PTPTSHUR;
    __IO uint32_t PTPTSLUR;
    __IO uint32_t PTPTSAR;
    __IO uint32_t PTPTTHR;
    __IO uint32_t PTPTTLR;
    __IO uint32_t RESERVED8;
    __IO uint32_t PTPTSSR;
    uint32_t      RESERVED9[565];
    __IO uint32_t DMABMR;                /*0x1000*/
    __IO uint32_t DMATPDR;
    __IO uint32_t DMARPDR;
    __IO uint32_t DMARDLAR;
    __IO uint32_t DMATDLAR;
    __IO uint32_t DMASR;
    __IO uint32_t DMAOMR;
    __IO uint32_t DMAIER;
    __IO uint32_t DMAMFBOCR;
    __IO uint32_t DMARSWTR;
    uint32_t      RESERVED10[8];
    __IO uint32_t DMACHTDR;
    __IO uint32_t DMACHRDR;
    __IO uint32_t DMACHTBAR;
    __IO uint32_t DMACHRBAR;
} ETH_TypeDef;


/**
  * @brief APB3 peripherals
  */
/* RCC definitions */
#define RCC_BASE            (0x40200000UL)
#define RCC                 ((RCC_TypeDef *)RCC_BASE)

/**
  * @brief APB2 peripherals
  */
/* DQSPI definitions */
#define DQSPI_BASE          (0x4010C000UL)
#define DQSPI_DATA_BASE     (0x31000000UL)
#define DQSPI               ((QSPI_DRAM_CACHE_TypeDef *)DQSPI_BASE)

/* VIDEO definitions */
#define VIDEOCAP_BASE    	(0x4010B000U)
#define VIDEOCAP          	((VIDEOCAP_TypeDef *)VIDEOCAP_BASE)

/* CRC32 definitions */
#define CRC32_BASE        	(0x40108000UL)
#define CRC32              	(CRC32_Typedef *)CRC32_BASE

/* ADC definitions */
#define ADC_BASE            (0x40107000UL)
#define ADC 				((ADC_TypeDef *)ADC_BASE)

/* WDT definitions */
#define WDT_BASE            (0x40106000UL)
#define WDT 				((WDT_TypeDef *)WDT_BASE)

/* RTC definitions */
#define RTC_BASE            (0x40105000UL)
#define RTC                 ((RTC_TypeDef *)RTC_BASE)



/* DCACHE definitions */
#define DCACHE_BASE         (0x40102000UL)
#define DCACHE              ((DCACHE_TypeDef *)DCACHE_BASE)

/* HYPER definitions */
#define HYPER_BASE          (0x40101000UL)
#define HYPER_DATA_BASE     (0x20000000UL)
#define HYPER               ((HYPER_TypeDef *)HYPER_BASE)

/* CQSPI definitions */
#define CQSPI_BASE          (0x40100000UL)
#define CQSPI_DATA_BASE     (0x08000000UL)
#define CQSPI               ((QSPI_DRAM_CACHE_TypeDef *)CQSPI_BASE)

/**
  * @brief APB1 peripherals
  */
/* UARTx(x=0,1,2) definitions */
#define UART_BASE           (0x40001000UL)
#define UART0_BASE          (UART_BASE + 0x0UL)
#define UART1_BASE          (UART_BASE + 0x1000UL)
#define UART2_BASE          (0x4000E000UL)
#define UART0               ((UART_TypeDef *)UART0_BASE)
#define UART1               ((UART_TypeDef *)UART1_BASE)
#define UART2               ((UART_TypeDef *)UART2_BASE)

/* SPIx(x=0,1,2) definitions */
#define SPI0_BASE          	(0x40007000UL)
#define SPI1_BASE          	(0x4000C000UL)
#define SPI2_BASE          	(0x4000D000UL)
#define SPI0                ((SPI_TypeDef *)SPI0_BASE)
#define SPI1                ((SPI_TypeDef *)SPI1_BASE)
#define SPI2                ((SPI_TypeDef *)SPI2_BASE)

/* GPIOx(x=A,B) definitions */
#define GPIOA_BASE          (0x4000A000UL)
#define GPIOB_BASE          (0x4000B000UL)
#define GPIOA               ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *)GPIOB_BASE)

/* I2S definitions */
#define I2S_BASE            (0x40008000UL)
#define I2S                 ((I2S_TypeDef *)I2S_BASE)

/* I2C definitions */
#define I2C0_BASE           (0x40005000UL)
#define I2C1_BASE           (0x40006000UL)
#define I2C0                ((I2C_TypeDef *)I2C0_BASE)
#define I2C1                ((I2C_TypeDef *)I2C1_BASE)

/* CANFD definitions */
#define CANFD_BASE         	(0x40000000UL)
#define CANFD               ((CANFD_TypeDef *)CANFD_BASE)

/**
  * @brief AHB peripherals
  */
/* MOTOR definitions */
#define MOTOR_BASE          (0x50101000UL)
#define MOTOR               ((MOTOR_TypeDef *)MOTOR_BASE)

/* DMA definitions */
#define DMA_BASE            (0x50200000UL)
#define DMA                 ((DMA_TypeDef *)DMA_BASE)

/* ETH definitions */
#define ETH_BASE            (0x50400000UL)
#define ETH_MAC_BASE        (ETH_BASE)
#define ETH_MMC_BASE        (ETH_BASE + 0x0100UL)
#define ETH_PTP_BASE        (ETH_BASE + 0x0700UL)
#define ETH_DMA_BASE        (ETH_BASE + 0x1000UL)
#define ETH                 ((ETH_TypeDef *)ETH_BASE)

/******************************** I2S Instances *******************************/
#define IS_I2S_INSTANCE(INSTANCE)       ((INSTANCE) == I2S)
/******************************** HYPER Instances *****************************/
#define IS_HYPER_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == HYPER)
/******************************** SPI Instances *******************************/
#define IS_SPI_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == SPI0 || \
                                        (INSTANCE) == SPI1 || \
                                        (INSTANCE) == SPI2)
/******************************** WDT Instances *******************************/
#define IS_WDT_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == WDT)
/******************************** ADC Instances *******************************/
#define IS_ADC_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == ADC)
/******************************** GPIO Instances ******************************/
#define IS_GPIO_ALL_INSTANCE(PERIPH) (((PERIPH) == GPIOA) || \
                                      ((PERIPH) == GPIOB))
/******************************** CANFD Instances *****************************/
#define IS_CANFD_ALL_INSTANCE(INSTANCE)       ((INSTANCE) == CANFD)
/******************************** UART Instances ******************************/
#define IS_UART_HALFDUPLEX_INSTANCE(INSTANCE)  (((INSTANCE) == UART0) || \
                                                ((INSTANCE) == UART1) || \
                                                ((INSTANCE) == UART2))

/******************************** DMA Instances *******************************/
#define IS_DMA_ALL_INSTANCE(INSTANCE)       ((INSTANCE) == DMA)

/******************************** I2C Instances *******************************/
#define IS_I2C_ALL_INSTANCE(INSTANCE) (((INSTANCE) == I2C0) || \
                                       ((INSTANCE) == I2C1))

/****************** TIM Instances : supporting synchronization ****************/
#define IS_TIM_SYNCHRO_INSTANCE(INSTANCE)  IS_TIM_MASTER_INSTANCE(INSTANCE)


/******************** TIM Instances : Advanced-control timers *****************/
#define IS_TIM_ADVANCED_INSTANCE(INSTANCE) (((INSTANCE) == TIM0) || \
                                            ((INSTANCE) == TIM1))

/******************* TIM Instances : Timer input XOR function *****************/
#define IS_TIM_XOR_INSTANCE(INSTANCE)   (((INSTANCE) == TIM0) || \
                                         ((INSTANCE) == TIM1))

/****************** TIM Instances : DMA requests generation (UDE) *************/
#define IS_TIM_DMA_INSTANCE(INSTANCE) (((INSTANCE) == TIM0) || \
                                       ((INSTANCE) == TIM1))

/************ TIM Instances : DMA requests generation (CCxDE) *****************/
#define IS_TIM_DMA_CC_INSTANCE(INSTANCE) (((INSTANCE) == TIM0) || \
                                          ((INSTANCE) == TIM1))

/************ TIM Instances : DMA requests generation (COMDE) *****************/
#define IS_TIM_CCDMA_INSTANCE(INSTANCE)  (((INSTANCE) == TIM0) || \
                                          ((INSTANCE) == TIM1))

/******************** TIM Instances : DMA burst feature ***********************/
#define IS_TIM_DMABURST_INSTANCE(INSTANCE)  (((INSTANCE) == TIM0) || \
                                             ((INSTANCE) == TIM1))

/****** TIM Instances : master mode available (TIMx_CR2.MMS available )********/
#define IS_TIM_MASTER_INSTANCE(INSTANCE) (((INSTANCE) == TIM0)  || \
                                          ((INSTANCE) == TIM1))

/*********** TIM Instances : Slave mode available (TIMx_SMCR available )*******/
#define IS_TIM_SLAVE_INSTANCE(INSTANCE) (((INSTANCE) == TIM0) || \
                                         ((INSTANCE) == TIM1))
/********************** TIM Instances : 32 bit Counter ************************/
#define IS_TIM_32B_COUNTER_INSTANCE(INSTANCE)  (((INSTANCE) == TIM0)  || \
                                                ((INSTANCE) == TIM1))

/***************** TIM Instances : external trigger input availabe ************/
#define IS_TIM_ETR_INSTANCE(INSTANCE)  (((INSTANCE) == TIM0)  || \
                                        ((INSTANCE) == TIM1))

/****************** TIM Instances : remapping capability **********************/
#define IS_TIM_REMAP_INSTANCE(INSTANCE) (((INSTANCE) == TIM0)  || \
                                         ((INSTANCE) == TIM1))
/****** TIM Instances : supporting external clock mode 1 for ETRF input *******/
#define IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE(INSTANCE) (((INSTANCE) == TIM0) || \
                                                        ((INSTANCE) == TIM1))
/********** TIM Instances : supporting internal trigger inputs(ITRX) *********/
#define IS_TIM_CLOCKSOURCE_ITRX_INSTANCE(INSTANCE)     (((INSTANCE) == TIM0) || \
                                                        ((INSTANCE) == TIM1))
/****** TIM Instances : supporting external clock mode 2 for ETRF input *******/
#define IS_TIM_CLOCKSOURCE_ETRMODE2_INSTANCE(INSTANCE) (((INSTANCE) == TIM0) || \
                                                        ((INSTANCE) == TIM1))
/****** TIM Instances : supporting external clock mode 1 for TIX inputs ******/
#define IS_TIM_CLOCKSOURCE_TIX_INSTANCE(INSTANCE)      (((INSTANCE) == TIM0) || \
                                                        ((INSTANCE) == TIM1))
/****************** TIM Instances : supporting commutation event generation ***/
#define IS_TIM_COMMUTATION_EVENT_INSTANCE(INSTANCE) (((INSTANCE) == TIM0)|| \
                                                     ((INSTANCE) == TIM1))
/****************** TIM Instances : supporting Hall sensor interface **********/
#define IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(INSTANCE)  (((INSTANCE) == TIM0) || \
                                                          ((INSTANCE) == TIM1))
/****************** TIM Instances : supporting OCxREF clear *******************/
#define IS_TIM_OCXREF_CLEAR_INSTANCE(INSTANCE)        (((INSTANCE) == TIM0) || \
                                                       ((INSTANCE) == TIM1))
/****************** TIM Instances : supporting encoder interface **************/
#define IS_TIM_ENCODER_INTERFACE_INSTANCE(INSTANCE)  (((INSTANCE) == TIM0) || \
                                                      ((INSTANCE) == TIM1))
/******************* TIM Instances : output(s) available **********************/
#define IS_TIM_CCX_INSTANCE(INSTANCE, CHANNEL) \
    ((((INSTANCE) == TIM0) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
    ||                                         \
    (((INSTANCE) == TIM1) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4))))







/******************************************************************************/
/*                                                                            */
/*                         Reset and Clock Control                            */
/*                                                                            */
/******************************************************************************/
#define RCC_DISP_PLL_CTRL_PLL_M_Pos        (0U)
#define RCC_DISP_PLL_CTRL_PLL_M_Msk        (0xFFUL << RCC_DISP_PLL_CTRL_PLL_M_Pos)            /*!< 0x000000FF */
#define RCC_DISP_PLL_CTRL_PLL_M            RCC_DISP_PLL_CTRL_PLL_M_Msk                        /*!<DISP PLL PLL_M control   */
#define RCC_DISP_PLL_CTRL_PLL_N_Pos        (8U)
#define RCC_DISP_PLL_CTRL_PLL_N_Msk        (0xFUL << RCC_DISP_PLL_CTRL_PLL_N_Pos)             /*!< 0x00000F00 */
#define RCC_DISP_PLL_CTRL_PLL_N            RCC_DISP_PLL_CTRL_PLL_N_Msk                        /*!<DISP PLL PLL_N control   */
#define RCC_DISP_PLL_CTRL_PLL_OD_Pos      (12U)
#define RCC_DISP_PLL_CTRL_PLL_OD_Msk      (0x3UL << RCC_DISP_PLL_CTRL_PLL_OD_Pos)           /*!< 0x00003000 */
#define RCC_DISP_PLL_CTRL_PLL_OD          RCC_DISP_PLL_CTRL_PLL_OD_Msk                      /*!<DISP PLL PLL OD control   */
#define RCC_DISP_PLL_CTRL_PLL_BP_Pos      (14U)
#define RCC_DISP_PLL_CTRL_PLL_BP_Msk      (0x1UL << RCC_DISP_PLL_CTRL_PLL_BP_Pos)           /*!< 0x00004000 */
#define RCC_DISP_PLL_CTRL_PLL_BP          RCC_DISP_PLL_CTRL_PLL_BP_Msk                      /*!<DISP PLL PLL BP control   */
#define RCC_DISP_PLL_CTRL_PLL_PDRST_Pos   (15U)
#define RCC_DISP_PLL_CTRL_PLL_PDRST_Msk   (0x1UL << RCC_DISP_PLL_CTRL_PLL_PDRST_Pos)        /*!< 0x00008000 */
#define RCC_DISP_PLL_CTRL_PLL_PDRST       RCC_DISP_PLL_CTRL_PLL_PDRST_Msk                   /*!<DISP PLL PLL PDRST control   */
#define RCC_DISP_PLL_CTRL_PLL_LOCKOUT_Pos (16U)
#define RCC_DISP_PLL_CTRL_PLL_LOCKOUT_Msk (0x1UL << RCC_DISP_PLL_CTRL_PLL_LOCKOUT_Pos)      /*!< 0x00010000 */
#define RCC_DISP_PLL_CTRL_PLL_LOCKOUT     RCC_DISP_PLL_CTRL_PLL_LOCKOUT_Msk                 /*!<DISP PLL PLL LOCKOUT control   */

#define RCC_DDR_PLL_CTRL_PLL_M_Pos        (0U)
#define RCC_DDR_PLL_CTRL_PLL_M_Msk        (0xFFUL << RCC_DDR_PLL_CTRL_PLL_M_Pos)            /*!< 0x000000FF */
#define RCC_DDR_PLL_CTRL_PLL_M            RCC_DDR_PLL_CTRL_PLL_M_Msk                        /*!<DDR PLL PLL_M control   */
#define RCC_DDR_PLL_CTRL_PLL_N_Pos        (8U)
#define RCC_DDR_PLL_CTRL_PLL_N_Msk        (0xFUL << RCC_DDR_PLL_CTRL_PLL_N_Pos)             /*!< 0x00000F00 */
#define RCC_DDR_PLL_CTRL_PLL_N            RCC_DDR_PLL_CTRL_PLL_N_Msk                        /*!<DDR PLL PLL_N control   */
#define RCC_DDR_PLL_CTRL_PLL_OD_Pos      (12U)
#define RCC_DDR_PLL_CTRL_PLL_OD_Msk      (0x3UL << RCC_DDR_PLL_CTRL_PLL_OD_Pos)           /*!< 0x00003000 */
#define RCC_DDR_PLL_CTRL_PLL_OD          RCC_DDR_PLL_CTRL_PLL_OD_Msk                      /*!<DDR PLL PLL OD control   */
#define RCC_DDR_PLL_CTRL_PLL_BP_Pos      (14U)
#define RCC_DDR_PLL_CTRL_PLL_BP_Msk      (0x1UL << RCC_DDR_PLL_CTRL_PLL_BP_Pos)           /*!< 0x00004000 */
#define RCC_DDR_PLL_CTRL_PLL_BP          RCC_DDR_PLL_CTRL_PLL_BP_Msk                      /*!<DDR PLL PLL BP control   */
#define RCC_DDR_PLL_CTRL_PLL_PDRST_Pos   (15U)
#define RCC_DDR_PLL_CTRL_PLL_PDRST_Msk   (0x1UL << RCC_DDR_PLL_CTRL_PLL_PDRST_Pos)        /*!< 0x00008000 */
#define RCC_DDR_PLL_CTRL_PLL_PDRST       RCC_DDR_PLL_CTRL_PLL_PDRST_Msk                   /*!<DDR PLL PLL PDRST control   */
#define RCC_DDR_PLL_CTRL_PLL_LOCKOUT_Pos (16U)
#define RCC_DDR_PLL_CTRL_PLL_LOCKOUT_Msk (0x1UL << RCC_DDR_PLL_CTRL_PLL_LOCKOUT_Pos)      /*!< 0x00010000 */
#define RCC_DDR_PLL_CTRL_PLL_LOCKOUT     RCC_DDR_PLL_CTRL_PLL_LOCKOUT_Msk                 /*!<DDR PLL PLL LOCKOUT control   */

#define RCC_AUD_PLL_CTRL_PLL_M_Pos        (0U)
#define RCC_AUD_PLL_CTRL_PLL_M_Msk        (0xFFUL << RCC_AUD_PLL_CTRL_PLL_M_Pos)            /*!< 0x000000FF */
#define RCC_AUD_PLL_CTRL_PLL_M            RCC_AUD_PLL_CTRL_PLL_M_Msk                        /*!<AUD PLL PLL_M control   */
#define RCC_AUD_PLL_CTRL_PLL_N_Pos        (8U)
#define RCC_AUD_PLL_CTRL_PLL_N_Msk        (0xFUL << RCC_AUD_PLL_CTRL_PLL_N_Pos)             /*!< 0x00000F00 */
#define RCC_AUD_PLL_CTRL_PLL_N            RCC_AUD_PLL_CTRL_PLL_N_Msk                        /*!<AUD PLL PLL_N control   */
#define RCC_AUD_PLL_CTRL_PLL_OD_Pos      (12U)
#define RCC_AUD_PLL_CTRL_PLL_OD_Msk      (0x3UL << RCC_AUD_PLL_CTRL_PLL_OD_Pos)           /*!< 0x00003000 */
#define RCC_AUD_PLL_CTRL_PLL_OD          RCC_AUD_PLL_CTRL_PLL_OD_Msk                      /*!<AUD PLL PLL OD control   */
#define RCC_AUD_PLL_CTRL_PLL_BP_Pos      (14U)
#define RCC_AUD_PLL_CTRL_PLL_BP_Msk      (0x1UL << RCC_AUD_PLL_CTRL_PLL_BP_Pos)           /*!< 0x00004000 */
#define RCC_AUD_PLL_CTRL_PLL_BP          RCC_AUD_PLL_CTRL_PLL_BP_Msk                      /*!<AUD PLL PLL BP control   */
#define RCC_AUD_PLL_CTRL_PLL_PDRST_Pos   (15U)
#define RCC_AUD_PLL_CTRL_PLL_PDRST_Msk   (0x1UL << RCC_AUD_PLL_CTRL_PLL_PDRST_Pos)        /*!< 0x00008000 */
#define RCC_AUD_PLL_CTRL_PLL_PDRST       RCC_AUD_PLL_CTRL_PLL_PDRST_Msk                   /*!<AUD PLL PLL PDRST control   */
#define RCC_AUD_PLL_CTRL_PLL_LOCKOUT_Pos (16U)
#define RCC_AUD_PLL_CTRL_PLL_LOCKOUT_Msk (0x1UL << RCC_AUD_PLL_CTRL_PLL_LOCKOUT_Pos)      /*!< 0x00010000 */
#define RCC_AUD_PLL_CTRL_PLL_LOCKOUT     RCC_AUD_PLL_CTRL_PLL_LOCKOUT_Msk                 /*!<AUD PLL PLL LOCKOUT control   */

#define RCC_SYS_PLL_CTRL_PLL_M_Pos        (0U)
#define RCC_SYS_PLL_CTRL_PLL_M_Msk        (0xFFUL << RCC_SYS_PLL_CTRL_PLL_M_Pos)            /*!< 0x000000FF */
#define RCC_SYS_PLL_CTRL_PLL_M            RCC_SYS_PLL_CTRL_PLL_M_Msk                        /*!<SYS PLL PLL_M control   */
#define RCC_SYS_PLL_CTRL_PLL_N_Pos        (8U)
#define RCC_SYS_PLL_CTRL_PLL_N_Msk        (0xFUL << RCC_SYS_PLL_CTRL_PLL_N_Pos)             /*!< 0x00000F00 */
#define RCC_SYS_PLL_CTRL_PLL_N            RCC_SYS_PLL_CTRL_PLL_N_Msk                        /*!<SYS PLL PLL_N control   */
#define RCC_SYS_PLL_CTRL_PLL_OD_Pos      (12U)
#define RCC_SYS_PLL_CTRL_PLL_OD_Msk      (0x3UL << RCC_SYS_PLL_CTRL_PLL_OD_Pos)           /*!< 0x00003000 */
#define RCC_SYS_PLL_CTRL_PLL_OD          RCC_SYS_PLL_CTRL_PLL_OD_Msk                      /*!<SYS PLL PLL OD control   */
#define RCC_SYS_PLL_CTRL_PLL_BP_Pos      (14U)
#define RCC_SYS_PLL_CTRL_PLL_BP_Msk      (0x1UL << RCC_SYS_PLL_CTRL_PLL_BP_Pos)           /*!< 0x00004000 */
#define RCC_SYS_PLL_CTRL_PLL_BP          RCC_SYS_PLL_CTRL_PLL_BP_Msk                      /*!<SYS PLL PLL BP control   */
#define RCC_SYS_PLL_CTRL_PLL_PDRST_Pos   (15U)
#define RCC_SYS_PLL_CTRL_PLL_PDRST_Msk   (0x1UL << RCC_SYS_PLL_CTRL_PLL_PDRST_Pos)        /*!< 0x00008000 */
#define RCC_SYS_PLL_CTRL_PLL_PDRST       RCC_SYS_PLL_CTRL_PLL_PDRST_Msk                   /*!<SYS PLL PLL PDRST control   */
#define RCC_SYS_PLL_CTRL_PLL_LOCKOUT_Pos (16U)
#define RCC_SYS_PLL_CTRL_PLL_LOCKOUT_Msk (0x1UL << RCC_SYS_PLL_CTRL_PLL_LOCKOUT_Pos)      /*!< 0x00010000 */
#define RCC_SYS_PLL_CTRL_PLL_LOCKOUT     RCC_SYS_PLL_CTRL_PLL_LOCKOUT_Msk                 /*!<SYS PLL PLL LOCKOUT control   */

#define RCC_DDR_CTRL_CLK_EN_Pos          (0U)
#define RCC_DDR_CTRL_CLK_EN_Msk          (0x1UL << RCC_DDR_CTRL_CLK_EN_Pos)               /*!< 0x00000001 */
#define RCC_DDR_CTRL_CLK_EN              RCC_DDR_CTRL_CLK_EN_Msk                          /*!<DDR clk enable control   */
#define RCC_AUD_CTRL_CLK_EN_Pos          (0U)
#define RCC_AUD_CTRL_CLK_EN_Msk          (0x1UL << RCC_AUD_CTRL_CLK_EN_Pos)               /*!< 0x00000001 */
#define RCC_AUD_CTRL_CLK_EN              RCC_AUD_CTRL_CLK_EN_Msk                          /*!<AUD clk enable control   */

#define RCC_DISP_CTRL_PRES0_Pos          (0U)
#define RCC_DISP_CTRL_PRES0_Msk          (0x07UL << RCC_DISP_CTRL_PRES0_Pos)              /*!< 0x000000007 */
#define RCC_DISP_CTRL_PRES0              RCC_DISP_CTRL_PRES0_Msk                          /*!<DISP pres0 control   */
#define RCC_DISP_CTRL_PRES1_Pos          (3U)
#define RCC_DISP_CTRL_PRES1_Msk          (0x01UL << RCC_DISP_CTRL_PRES1_Pos)              /*!< 0x00000008 */
#define RCC_DISP_CTRL_PRES1              RCC_DISP_CTRL_PRES1_Msk                          /*!<DISP pres1 control   */

#define RCC_I2S_CTRL_DIV0_Pos            (0U)
#define RCC_I2S_CTRL_DIV0_Msk            (0x03UL << RCC_I2S_CTRL_DIV0_Pos)                /*!< 0x00000003 */
#define RCC_I2S_CTRL_DIV0                RCC_I2S_CTRL_DIV0_Msk                            /*!<I2S DIV0 control   */
#define RCC_I2S_CTRL_DIV1_Pos            (2U)
#define RCC_I2S_CTRL_DIV1_Msk            (0x07UL << RCC_I2S_CTRL_DIV1_Pos)                /*!< 0x0000001C */
#define RCC_I2S_CTRL_DIV1                RCC_I2S_CTRL_DIV1_Msk                            /*!<I2S DIV1 control   */
#define RCC_I2S_CTRL_DIV2_Pos            (5U)
#define RCC_I2S_CTRL_DIV2_Msk            (0x07UL << RCC_I2S_CTRL_DIV2_Pos)                /*!< 0x000000E0 */
#define RCC_I2S_CTRL_DIV2                RCC_I2S_CTRL_DIV2_Msk                            /*!<I2S DIV2 control   */

#define RCC_SYS_CTRL_CLK_SEL_Pos         (0U)
#define RCC_SYS_CTRL_CLK_SEL_Msk         (0x01UL << RCC_SYS_CTRL_CLK_SEL_Pos)             /*!< 0x00000001 */
#define RCC_SYS_CTRL_CLK_SEL             RCC_SYS_CTRL_CLK_SEL_Msk                         /*!<SYS CLK_SEL control   */
#define RCC_SYS_CTRL_CLK_EN_Pos          (4U)
#define RCC_SYS_CTRL_CLK_EN_Msk          (0x01UL << RCC_SYS_CTRL_CLK_EN_Pos)              /*!< 0x00000010 */
#define RCC_SYS_CTRL_CLK_EN              RCC_SYS_CTRL_CLK_EN_Msk                          /*!<SYS CLK_EN control   */

#define RCC_FD_CAN0_CTRL_CLK_EN_Pos      (0U)
#define RCC_FD_CAN0_CTRL_CLK_EN_Msk      (0x01UL << RCC_FD_CAN0_CTRL_CLK_EN_Pos)          /*!< 0x00000001 */
#define RCC_FD_CAN0_CTRL_CLK_EN          RCC_FD_CAN0_CTRL_CLK_EN_Msk                      /*!<FD_CAN0 CLK_EN control   */
#define RCC_FD_CAN0_CTRL_PRES_Pos        (1U)
#define RCC_FD_CAN0_CTRL_PRES_Msk        (0x03UL << RCC_FD_CAN0_CTRL_PRES_Pos)            /*!< 0x00000006 */
#define RCC_FD_CAN0_CTRL_PRES            RCC_FD_CAN0_CTRL_PRES_Msk                        /*!<FD_CAN0 PRES control   */
#define RCC_FD_CAN0_CTRL_FD_EN_Pos       (4U)
#define RCC_FD_CAN0_CTRL_FD_EN_Msk       (0x01UL << RCC_FD_CAN0_CTRL_FD_EN_Pos)           /*!< 0x00000010 */
#define RCC_FD_CAN0_CTRL_FD_EN           RCC_FD_CAN0_CTRL_FD_EN_Msk                       /*!<FD_CAN0 FD_EN control   */
#define RCC_FD_CAN1_CTRL_CLK_EN_Pos      (0U)
#define RCC_FD_CAN1_CTRL_CLK_EN_Msk      (0x01UL << RCC_FD_CAN1_CTRL_CLK_EN_Pos)          /*!< 0x00000001 */
#define RCC_FD_CAN1_CTRL_CLK_EN          RCC_FD_CAN1_CTRL_CLK_EN_Msk                      /*!<FD_CAN1 CLK_EN control   */
#define RCC_FD_CAN1_CTRL_PRES_Pos        (1U)
#define RCC_FD_CAN1_CTRL_PRES_Msk        (0x03UL << RCC_FD_CAN1_CTRL_PRES_Pos)            /*!< 0x00000006 */
#define RCC_FD_CAN1_CTRL_PRES            RCC_FD_CAN1_CTRL_PRES_Msk                        /*!<FD_CAN1 PRES control   */
#define RCC_FD_CAN1_CTRL_FD_EN_Pos       (4U)
#define RCC_FD_CAN1_CTRL_FD_EN_Msk       (0x01UL << RCC_FD_CAN1_CTRL_FD_EN_Pos)           /*!< 0x00000010 */
#define RCC_FD_CAN1_CTRL_FD_EN           RCC_FD_CAN1_CTRL_FD_EN_Msk                       /*!<FD_CAN1 FD_EN control   */

#define RCC_H264_CTRL_CLK_EN_Pos         (0U)
#define RCC_H264_CTRL_CLK_EN_Msk         (0x01UL << RCC_H264_CTRL_CLK_EN_Pos)             /*!< 0x00000001 */
#define RCC_H264_CTRL_CLK_EN             RCC_H264_CTRL_CLK_EN_Msk                         /*!<H264 CLK_EN control   */
#define RCC_H264_CTRL_PRES_Pos           (1U)
#define RCC_H264_CTRL_PRES_Msk           (0x03UL << RCC_H264_CTRL_PRES_Pos)               /*!< 0x00000006 */
#define RCC_H264_CTRL_PRES               RCC_H264_CTRL_PRES_Msk                           /*!<H264 PRES control   */

#define RCC_GPU_CTRL_PRES_Pos            (0U)
#define RCC_GPU_CTRL_PRES_Msk            (0x03UL << RCC_GPU_CTRL_PRES_Pos)                /*!< 0x00000003 */
#define RCC_GPU_CTRL_PRES                RCC_GPU_CTRL_PRES_Msk                            /*!<GPU PRES control   */
#define RCC_CPU0_CTRL_PRES_Pos           (0U)
#define RCC_CPU0_CTRL_PRES_Msk           (0x03UL << RCC_CPU0_CTRL_PRES_Pos)               /*!< 0x00000003 */
#define RCC_CPU0_CTRL_PRES               RCC_CPU0_CTRL_PRES_Msk                           /*!<CPU0 PRES control   */
#define RCC_CPU0_CTRL_DBG_CLK_EN_Pos     (2U)
#define RCC_CPU0_CTRL_DBG_CLK_EN_Msk     (0x01UL << RCC_CPU0_CTRL_DBG_CLK_EN_Pos)         /*!< 0x00000004 */
#define RCC_CPU0_CTRL_DBG_CLK_EN         RCC_CPU0_CTRL_DBG_CLK_EN_Msk                     /*!<CPU0 DBG_CLK_EN control   */

#define RCC_CPU1_CTRL_CLK_EN_Pos         (0U)
#define RCC_CPU1_CTRL_CLK_EN_Msk         (0x01UL << RCC_CPU1_CTRL_CLK_EN_Pos)             /*!< 0x00000001 */
#define RCC_CPU1_CTRL_CLK_EN             RCC_CPU1_CTRL_CLK_EN_Msk                         /*!<CPU1 CLK_EN control   */

#define RCC_QSPI_CTRL_CLK_EN_Pos         (0U)
#define RCC_QSPI_CTRL_CLK_EN_Msk         (0x01UL << RCC_QSPI_CTRL_CLK_EN_Pos)             /*!< 0x00000001 */
#define RCC_QSPI_CTRL_CLK_EN             RCC_QSPI_CTRL_CLK_EN_Msk                         /*!<CPU1 CLK_EN control   */
#define RCC_QSPI_CTRL_PRES_Pos           (1U)
#define RCC_QSPI_CTRL_PRES_Msk           (0x03UL << RCC_QSPI_CTRL_PRES_Pos)               /*!< 0x00000006 */
#define RCC_QSPI_CTRL_PRES               RCC_QSPI_CTRL_PRES_Msk                           /*!<CPU1 PRES control   */

#define RCC_CLK48_CTRL_PRES_Pos          (0U)
#define RCC_CLK48_CTRL_PRES_Msk          (0x03UL << RCC_CLK48_CTRL_PRES_Pos)              /*!< 0x00000003 */
#define RCC_CLK48_CTRL_PRES              RCC_CLK48_CTRL_PRES_Msk                          /*!<CLK48 PRES control   */

#define RCC_SD_CTRL_CMD_S0_DELAY_Pos     (0U)
#define RCC_SD_CTRL_CMD_S0_DELAY_Msk     (0x01UL << RCC_SD_CTRL_CMD_S0_DELAY_Pos)         /*!< 0x00000001 */
#define RCC_SD_CTRL_CMD_S0_DELAY         RCC_SD_CTRL_CMD_S0_DELAY_Msk                     /*!<SD CMD_S0_DELAY control   */
#define RCC_SD_CTRL_DAT_S0_DELAY_Pos     (1U)
#define RCC_SD_CTRL_DAT_S0_DELAY_Msk     (0x01UL << RCC_SD_CTRL_DAT_S0_DELAY_Pos)         /*!< 0x00000002 */
#define RCC_SD_CTRL_DAT_S0_DELAY         RCC_SD_CTRL_DAT_S0_DELAY_Msk                     /*!<SD DAT_S0_DELAY control   */
#define RCC_SD_CTRL_CMD_S1_DELAY_Pos     (2U)
#define RCC_SD_CTRL_CMD_S1_DELAY_Msk     (0x01UL << RCC_SD_CTRL_CMD_S1_DELAY_Pos)         /*!< 0x00000004 */
#define RCC_SD_CTRL_CMD_S1_DELAY         RCC_SD_CTRL_CMD_S1_DELAY_Msk                     /*!<SD CMD_S1_DELAY control   */
#define RCC_SD_CTRL_DAT_S1_DELAY_Pos     (3U)
#define RCC_SD_CTRL_DAT_S1_DELAY_Msk     (0x01UL << RCC_SD_CTRL_DAT_S1_DELAY_Pos)         /*!< 0x00000008 */
#define RCC_SD_CTRL_DAT_S1_DELAY         RCC_SD_CTRL_DAT_S1_DELAY_Msk                     /*!<SD DAT_S1_DELAY control   */

#define RCC_USB_CTRL_CLK_EN_Pos          (0U)
#define RCC_USB_CTRL_CLK_EN_Msk          (0x01UL << RCC_USB_CTRL_CLK_EN_Pos)              /*!< 0x00000001 */
#define RCC_USB_CTRL_CLK_EN              RCC_USB_CTRL_CLK_EN_Msk                          /*!<USB CLK_EN control   */
#define RCC_USB_CTRL_DMPULLDN_Pos        (1U)
#define RCC_USB_CTRL_DMPULLDN_Msk        (0x01UL << RCC_USB_CTRL_DMPULLDN_Pos)            /*!< 0x00000002 */
#define RCC_USB_CTRL_DMPULLDN            RCC_USB_CTRL_DMPULLDN_Msk                        /*!<USB DMPULLDN control   */
#define RCC_USB_CTRL_DMPU_LO_Pos         (2U)
#define RCC_USB_CTRL_DMPU_LO_Msk         (0x01UL << RCC_USB_CTRL_DMPU_LO_Pos)             /*!< 0x00000004 */
#define RCC_USB_CTRL_DMPU_LO             RCC_USB_CTRL_DMPU_LO_Msk                         /*!<USB DMPU_LO control   */
#define RCC_USB_CTRL_DMPU_Pos            (3U)
#define RCC_USB_CTRL_DMPU_Msk            (0x01UL << RCC_USB_CTRL_DMPU_Pos)                /*!< 0x00000008 */
#define RCC_USB_CTRL_DMPU                RCC_USB_CTRL_DMPU_Msk                            /*!<USB DMPU control   */
#define RCC_USB_CTRL_CID_Pos             (4U)
#define RCC_USB_CTRL_CID_Msk             (0x01UL << RCC_USB_CTRL_CID_Pos)                 /*!< 0x00000010 */
#define RCC_USB_CTRL_CID                 RCC_USB_CTRL_CID_Msk                             /*!<USB CID control   */
#define RCC_USB_CTRL_VBUSLO_Pos          (5U)
#define RCC_USB_CTRL_VBUSLO_Msk          (0x01UL << RCC_USB_CTRL_VBUSLO_Pos)              /*!< 0x00000020 */
#define RCC_USB_CTRL_VBUSLO              RCC_USB_CTRL_VBUSLO_Msk                          /*!<USB VBUSLO control   */
#define RCC_USB_CTRL_VBUSSES_Pos         (6U)
#define RCC_USB_CTRL_VBUSSES_Msk         (0x01UL << RCC_USB_CTRL_VBUSSES_Pos)             /*!< 0x00000040 */
#define RCC_USB_CTRL_VBUSSES             RCC_USB_CTRL_VBUSSES_Msk                         /*!<USB VBUSSES control   */
#define RCC_USB_CTRL_VBUSVAL_Pos         (7U)
#define RCC_USB_CTRL_VBUSVAL_Msk         (0x01UL << RCC_USB_CTRL_VBUSVAL_Pos)             /*!< 0x00000080 */
#define RCC_USB_CTRL_VBUSVAL             RCC_USB_CTRL_VBUSVAL_Msk                         /*!<USB VBUSVAL control   */
#define RCC_USB_CTRL_PHY_MATCH_RES_Pos   (12U)
#define RCC_USB_CTRL_PHY_MATCH_RES_Msk   (0x07UL << RCC_USB_CTRL_PHY_MATCH_RES_Pos)       /*!< 0x00007000 */
#define RCC_USB_CTRL_PHY_MATCH_RES       RCC_USB_CTRL_PHY_MATCH_RES_Msk                   /*!<USB PHY_MATCH_RES control   */
#define RCC_USB_CTRL_FORCE_USB20_ID_LOW_Pos         (15U)
#define RCC_USB_CTRL_FORCE_USB20_ID_LOW_Msk         (0x01UL << RCC_USB_CTRL_FORCE_USB20_ID_LOW_Pos)             /*!< 0x00008000 */
#define RCC_USB_CTRL_FORCE_USB20_ID_LOW             RCC_USB_CTRL_FORCE_USB20_ID_LOW_Msk                         /*!<USB FORCE_USB20_ID_LOW control   */
#define RCC_USB_CTRL_12_CLK_EN_Pos       (16U)
#define RCC_USB_CTRL_12_CLK_EN_Msk       (0x01UL << RCC_USB_CTRL_12_CLK_EN_Pos)           /*!< 0x00010000 */
#define RCC_USB_CTRL_12_CLK_EN           RCC_USB_CTRL_12_CLK_EN_Msk                       /*!<USB 12_CLK_EN control   */
#define RCC_USB_CTRL_PHY_CLK_MODE_Pos    (17U)
#define RCC_USB_CTRL_PHY_CLK_MODE_Msk    (0x03UL << RCC_USB_CTRL_PHY_CLK_MODE_Pos)        /*!< 0x00060000 */
#define RCC_USB_CTRL_PHY_CLK_MODE        RCC_USB_CTRL_PHY_CLK_MODE_Msk                    /*!<USB PHY_CLK_MODE control   */
#define RCC_USB_CTRL_PHY_SQ_MODE_Pos     (19U)
#define RCC_USB_CTRL_PHY_SQ_MODE_Msk     (0x07UL << RCC_USB_CTRL_PHY_SQ_MODE_Pos)        /*!< 0x00380000 */
#define RCC_USB_CTRL_PHY_SQ_MODE         RCC_USB_CTRL_PHY_SQ_MODE_Msk                    /*!<USB PHY_SQ_MODE control   */
#define RCC_USB_CTRL_PHY_RREF_Pos        (22U)
#define RCC_USB_CTRL_PHY_RREF_Msk        (0x0FUL << RCC_USB_CTRL_PHY_RREF_Pos)           /*!< 0x03C00000 */
#define RCC_USB_CTRL_PHY_RREF            RCC_USB_CTRL_PHY_RREF_Msk                       /*!<USB PHY_RREF control   */
#define RCC_USB_CTRL_PHY_TF_SEL_Pos      (26U)
#define RCC_USB_CTRL_PHY_TF_SEL_Msk      (0x03UL << RCC_USB_CTRL_PHY_TF_SEL_Pos)         /*!< 0x0C000000 */
#define RCC_USB_CTRL_PHY_TF_SEL          RCC_USB_CTRL_PHY_TF_SEL_Msk                     /*!<USB PHY_TF_SEL control   */
#define RCC_USB_CTRL_PHY_IBIAS_SEL_Pos   (28U)
#define RCC_USB_CTRL_PHY_IBIAS_SEL_Msk   (0x03UL << RCC_USB_CTRL_PHY_IBIAS_SEL_Pos)      /*!< 0x30000000 */
#define RCC_USB_CTRL_PHY_IBIAS_SEL       RCC_USB_CTRL_PHY_IBIAS_SEL_Msk                  /*!<USB PHY_IBIAS_SEL control   */
#define RCC_USB_CTRL_OTG_SUSPENDM_Pos    (30U)
#define RCC_USB_CTRL_OTG_SUSPENDM_Msk    (0x01UL << RCC_USB_CTRL_OTG_SUSPENDM_Pos)       /*!< 0x40000000 */
#define RCC_USB_CTRL_OTG_SUSPENDM        RCC_USB_CTRL_OTG_SUSPENDM_Msk                   /*!<USB OTG_SUSPENDM control   */
#define RCC_USB_CTRL_FORCE_USB20_ID_HIGH_Pos    (31U)
#define RCC_USB_CTRL_FORCE_USB20_ID_HIGH_Msk    (0x01UL << RCC_USB_CTRL_FORCE_USB20_ID_HIGH_Pos)       /*!< 0x80000000 */
#define RCC_USB_CTRL_FORCE_USB20_ID_HIGH        RCC_USB_CTRL_FORCE_USB20_ID_HIGH_Msk                   /*!<USB FORCE_USB20_ID_HIGH control   */

#define RCC_GMAC_CTRL_INTF_SEL_Pos       (0U)
#define RCC_GMAC_CTRL_INTF_SEL_Msk       (0x01UL << RCC_GMAC_CTRL_INTF_SEL_Pos)          /*!< 0x00000001 */
#define RCC_GMAC_CTRL_INTF_SEL           RCC_GMAC_CTRL_INTF_SEL_Msk                      /*!<GMAC INTF_SEL control   */
#define RCC_GMAC_CTRL_FORCE_TCLK_Pos     (1U)
#define RCC_GMAC_CTRL_FORCE_TCLK_Msk     (0x01UL << RCC_GMAC_CTRL_FORCE_TCLK_Pos)        /*!< 0x00000002 */
#define RCC_GMAC_CTRL_FORCE_TCLK         RCC_GMAC_CTRL_FORCE_TCLK_Msk                    /*!<GMAC FORCE_TCLK control   */
#define RCC_GMAC_CTRL_TX_FORCE_NO_DELAY_Pos     (2U)
#define RCC_GMAC_CTRL_TX_FORCE_NO_DELAY_Msk     (0x01UL << RCC_GMAC_CTRL_TX_FORCE_NO_DELAY_Pos)        /*!< 0x00000004 */
#define RCC_GMAC_CTRL_TX_FORCE_NO_DELAY         RCC_GMAC_CTRL_TX_FORCE_NO_DELAY_Msk                    /*!<GMAC TX_FORCE_NO_DELAY control   */
#define RCC_GMAC_CTRL_TX_FORCE_DELAY_Pos        (3U)
#define RCC_GMAC_CTRL_TX_FORCE_DELAY_Msk        (0x01UL << RCC_GMAC_CTRL_TX_FORCE_DELAY_Pos)           /*!< 0x00000008 */
#define RCC_GMAC_CTRL_TX_FORCE_DELAY            RCC_GMAC_CTRL_TX_FORCE_DELAY_Msk                       /*!<GMAC TX_FORCE_DELAY control   */
#define RCC_GMAC_CTRL_RX_FORCE_NO_DELAY_Pos     (4U)
#define RCC_GMAC_CTRL_RX_FORCE_NO_DELAY_Msk     (0x01UL << RCC_GMAC_CTRL_RX_FORCE_NO_DELAY_Pos)        /*!< 0x00000010 */
#define RCC_GMAC_CTRL_RX_FORCE_NO_DELAY         RCC_GMAC_CTRL_RX_FORCE_NO_DELAY_Msk                    /*!<GMAC RX_FORCE_NO_DELAY control   */
#define RCC_GMAC_CTRL_RX_FORCE_DELAY_Pos        (5U)
#define RCC_GMAC_CTRL_RX_FORCE_DELAY_Msk        (0x01UL << RCC_GMAC_CTRL_RX_FORCE_DELAY_Pos)           /*!< 0x00000020 */
#define RCC_GMAC_CTRL_RX_FORCE_DELAY            RCC_GMAC_CTRL_RX_FORCE_DELAY_Msk                       /*!<GMAC RX_FORCE_DELAY control   */

#define RCC_SPI0_CTRL_PRES_Pos          (0U)
#define RCC_SPI0_CTRL_PRES_Msk          (0x07UL << RCC_SPI0_CTRL_PRES_Pos)             /*!< 0x00000007 */
#define RCC_SPI0_CTRL_PRES              RCC_SPI0_CTRL_PRES_Msk                         /*!<SPI0 PRES control   */
#define RCC_SPI1_CTRL_PRES_Pos          (0U)
#define RCC_SPI1_CTRL_PRES_Msk          (0x07UL << RCC_SPI1_CTRL_PRES_Pos)             /*!< 0x00000007 */
#define RCC_SPI1_CTRL_PRES              RCC_SPI1_CTRL_PRES_Msk                         /*!<SPI1 PRES control   */
#define RCC_SPI2_CTRL_PRES_Pos          (0U)
#define RCC_SPI2_CTRL_PRES_Msk          (0x07UL << RCC_SPI2_CTRL_PRES_Pos)             /*!< 0x00000007 */
#define RCC_SPI2_CTRL_PRES              RCC_SPI2_CTRL_PRES_Msk                         /*!<SPI2 PRES control   */
#define RCC_SPI3_CTRL_PRES_Pos          (0U)
#define RCC_SPI3_CTRL_PRES_Msk          (0x07UL << RCC_SPI3_CTRL_PRES_Pos)             /*!< 0x00000007 */
#define RCC_SPI3_CTRL_PRES              RCC_SPI3_CTRL_PRES_Msk                         /*!<SPI3 PRES control   */
#define RCC_SPI4_CTRL_PRES_Pos          (0U)
#define RCC_SPI4_CTRL_PRES_Msk          (0x07UL << RCC_SPI4_CTRL_PRES_Pos)             /*!< 0x00000007 */
#define RCC_SPI4_CTRL_PRES              RCC_SPI4_CTRL_PRES_Msk                         /*!<SPI4 PRES control   */

#define RCC_I2C0_CTRL_CLKEN_Pos          (0U)
#define RCC_I2C0_CTRL_CLKEN_Msk          (0x01UL << RCC_I2C0_CTRL_CLKEN_Pos)             /*!< 0x00000001 */
#define RCC_I2C0_CTRL_CLKEN              RCC_I2C0_CTRL_CLKEN_Msk                         /*!<I2C0 CLKEN control   */
#define RCC_I2C1_CTRL_CLKEN_Pos          (0U)
#define RCC_I2C1_CTRL_CLKEN_Msk          (0x01UL << RCC_I2C1_CTRL_CLKEN_Pos)             /*!< 0x00000001 */
#define RCC_I2C1_CTRL_CLKEN              RCC_I2C1_CTRL_CLKEN_Msk                         /*!<I2C1 CLKEN control   */
#define RCC_I2C2_CTRL_CLKEN_Pos          (0U)
#define RCC_I2C2_CTRL_CLKEN_Msk          (0x01UL << RCC_I2C2_CTRL_CLKEN_Pos)             /*!< 0x00000001 */
#define RCC_I2C2_CTRL_CLKEN              RCC_I2C2_CTRL_CLKEN_Msk                         /*!<I2C2 CLKEN control   */
#define RCC_I2C3_CTRL_CLKEN_Pos          (0U)
#define RCC_I2C3_CTRL_CLKEN_Msk          (0x01UL << RCC_I2C3_CTRL_CLKEN_Pos)             /*!< 0x00000001 */
#define RCC_I2C3_CTRL_CLKEN              RCC_I2C3_CTRL_CLKEN_Msk                         /*!<I2C3 CLKEN control   */

#define RCC_NO_DLA_RAM_CTRL_Pos          (0U)
#define RCC_NO_DLA_RAM_CTRL_Msk          (0x01UL << RCC_NO_DLA_RAM_CTRL_Pos)             /*!< 0x00000001 */
#define RCC_NO_DLA_RAM_CTRL              RCC_NO_DLA_RAM_CTRL_Msk                         /*!<NO DLA RAM  control   */

#define RCC_CAN2_CTRL_CLKEN_Pos          (0U)
#define RCC_CAN2_CTRL_CLKEN_Msk          (0x01UL << RCC_CAN2_CTRL_CLKEN_Pos)             /*!< 0x00000001 */
#define RCC_CAN2_CTRL_CLKEN              RCC_CAN2_CTRL_CLKEN_Msk                         /*!<CAN2 CLKEN  control   */
#define RCC_CAN3_CTRL_CLKEN_Pos          (0U)
#define RCC_CAN3_CTRL_CLKEN_Msk          (0x01UL << RCC_CAN3_CTRL_CLKEN_Pos)             /*!< 0x00000001 */
#define RCC_CAN3_CTRL_CLKEN              RCC_CAN3_CTRL_CLKEN_Msk                         /*!<CAN3 CLKEN  control   */

#define RCC_CPU1_NMI_CTRL_Pos            (0U)
#define RCC_CPU1_NMI_CTRL_Msk            (0x01UL << RCC_CPU1_NMI_CTRL_Pos)               /*!< 0x00000001 */
#define RCC_CPU1_NMI_CTRL                RCC_CPU1_NMI_CTRL_Msk                           /*!<CPU1 NMI  control   */

#define RCC_CLKOUT_CTRL_SEL_Pos          (0U)
#define RCC_CLKOUT_CTRL_SEL_Msk          (0x1FUL << RCC_CLKOUT_CTRL_SEL_Pos)             /*!< 0x0000001F */
#define RCC_CLKOUT_CTRL_SEL              RCC_CLKOUT_CTRL_SEL_Msk                         /*!<CLKOUT SEL  control   */
#define RCC_CLKOUT_CTRL_PRES_Pos         (16U)
#define RCC_CLKOUT_CTRL_PRES_Msk         (0x07UL << RCC_CLKOUT_CTRL_PRES_Pos)            /*!< 0x00070000 */
#define RCC_CLKOUT_CTRL_PRES             RCC_CLKOUT_CTRL_PRES_Msk                        /*!<CLKOUT PRES  control   */

#define RCC_RSTN_CTRL_DDR_Pos            (0U)
#define RCC_RSTN_CTRL_DDR_Msk            (0x01UL << RCC_RSTN_CTRL_DDR_Pos)               /*!< 0x00000001 */
#define RCC_RSTN_CTRL_DDR                RCC_RSTN_CTRL_DDR_Msk                           /*!<RSTN DDR  control   */
#define RCC_RSTN_CTRL_I2S_Pos            (1U)
#define RCC_RSTN_CTRL_I2S_Msk            (0x01UL << RCC_RSTN_CTRL_I2S_Pos)               /*!< 0x00000002 */
#define RCC_RSTN_CTRL_I2S                RCC_RSTN_CTRL_I2S_Msk                           /*!<RSTN I2S  control   */
#define RCC_RSTN_CTRL_FD_CAN0_Pos        (2U)
#define RCC_RSTN_CTRL_FD_CAN0_Msk        (0x01UL << RCC_RSTN_CTRL_FD_CAN0_Pos)           /*!< 0x00000004 */
#define RCC_RSTN_CTRL_FD_CAN0            RCC_RSTN_CTRL_FD_CAN0_Msk                       /*!<RSTN FD_CAN0  control   */
#define RCC_RSTN_CTRL_FD_CAN1_Pos        (3U)
#define RCC_RSTN_CTRL_FD_CAN1_Msk        (0x01UL << RCC_RSTN_CTRL_FD_CAN1_Pos)           /*!< 0x00000008 */
#define RCC_RSTN_CTRL_FD_CAN1            RCC_RSTN_CTRL_FD_CAN1_Msk                       /*!<RSTN FD_CAN1  control   */
#define RCC_RSTN_CTRL_H264_Pos           (4U)
#define RCC_RSTN_CTRL_H264_Msk           (0x01UL << RCC_RSTN_CTRL_H264_Pos)              /*!< 0x00000010 */
#define RCC_RSTN_CTRL_H264               RCC_RSTN_CTRL_H264_Msk                          /*!<RSTN H264  control   */
#define RCC_RSTN_CTRL_QSPI_Pos           (5U)
#define RCC_RSTN_CTRL_QSPI_Msk           (0x01UL << RCC_RSTN_CTRL_QSPI_Pos)              /*!< 0x00000020 */
#define RCC_RSTN_CTRL_QSPI               RCC_RSTN_CTRL_QSPI_Msk                          /*!<RSTN QSPI  control   */
#define RCC_RSTN_CTRL_SD_Pos             (6U)
#define RCC_RSTN_CTRL_SD_Msk             (0x01UL << RCC_RSTN_CTRL_SD_Pos)                /*!< 0x00000040 */
#define RCC_RSTN_CTRL_SD                 RCC_RSTN_CTRL_SD_Msk                            /*!<RSTN SD  control   */
#define RCC_RSTN_CTRL_USB_Pos            (7U)
#define RCC_RSTN_CTRL_USB_Msk            (0x01UL << RCC_RSTN_CTRL_USB_Pos)               /*!< 0x00000080 */
#define RCC_RSTN_CTRL_USB                RCC_RSTN_CTRL_USB_Msk                           /*!<RSTN USB  control   */
#define RCC_RSTN_CTRL_SPI0_Pos           (8U)
#define RCC_RSTN_CTRL_SPI0_Msk           (0x01UL << RCC_RSTN_CTRL_SPI0_Pos)              /*!< 0x00000100 */
#define RCC_RSTN_CTRL_SPI0               RCC_RSTN_CTRL_SPI0_Msk                          /*!<RSTN SPI0  control   */
#define RCC_RSTN_CTRL_SPI1_Pos           (9U)
#define RCC_RSTN_CTRL_SPI1_Msk           (0x01UL << RCC_RSTN_CTRL_SPI1_Pos)              /*!< 0x00000200 */
#define RCC_RSTN_CTRL_SPI1               RCC_RSTN_CTRL_SPI1_Msk                          /*!<RSTN SPI1  control   */
#define RCC_RSTN_CTRL_SPI2_Pos           (10U)
#define RCC_RSTN_CTRL_SPI2_Msk           (0x01UL << RCC_RSTN_CTRL_SPI2_Pos)              /*!< 0x00000400 */
#define RCC_RSTN_CTRL_SPI2               RCC_RSTN_CTRL_SPI2_Msk                          /*!<RSTN SPI2  control   */
#define RCC_RSTN_CTRL_SPI3_Pos           (11U)
#define RCC_RSTN_CTRL_SPI3_Msk           (0x01UL << RCC_RSTN_CTRL_SPI3_Pos)              /*!< 0x00000800 */
#define RCC_RSTN_CTRL_SPI3               RCC_RSTN_CTRL_SPI3_Msk                          /*!<RSTN SPI3  control   */
#define RCC_RSTN_CTRL_SPI4_Pos           (12U)
#define RCC_RSTN_CTRL_SPI4_Msk           (0x01UL << RCC_RSTN_CTRL_SPI4_Pos)              /*!< 0x00001000 */
#define RCC_RSTN_CTRL_SPI4               RCC_RSTN_CTRL_SPI4_Msk                          /*!<RSTN SPI4  control   */
#define RCC_RSTN_CTRL_DISP_Pos           (13U)
#define RCC_RSTN_CTRL_DISP_Msk           (0x01UL << RCC_RSTN_CTRL_DISP_Pos)              /*!< 0x00002000 */
#define RCC_RSTN_CTRL_DISP               RCC_RSTN_CTRL_DISP_Msk                          /*!<RSTN DISP  control   */
#define RCC_RSTN_CTRL_I2C0_Pos           (14U)
#define RCC_RSTN_CTRL_I2C0_Msk           (0x01UL << RCC_RSTN_CTRL_I2C0_Pos)              /*!< 0x00004000 */
#define RCC_RSTN_CTRL_I2C0               RCC_RSTN_CTRL_I2C0_Msk                          /*!<RSTN I2C0  control   */
#define RCC_RSTN_CTRL_I2C1_Pos           (15U)
#define RCC_RSTN_CTRL_I2C1_Msk           (0x01UL << RCC_RSTN_CTRL_I2C1_Pos)              /*!< 0x00008000 */
#define RCC_RSTN_CTRL_I2C1               RCC_RSTN_CTRL_I2C1_Msk                          /*!<RSTN I2C1  control   */
#define RCC_RSTN_CTRL_I2C2_Pos           (16U)
#define RCC_RSTN_CTRL_I2C2_Msk           (0x01UL << RCC_RSTN_CTRL_I2C2_Pos)              /*!< 0x00010000 */
#define RCC_RSTN_CTRL_I2C2               RCC_RSTN_CTRL_I2C2_Msk                          /*!<RSTN I2C2  control   */
#define RCC_RSTN_CTRL_I2C3_Pos           (17U)
#define RCC_RSTN_CTRL_I2C3_Msk           (0x01UL << RCC_RSTN_CTRL_I2C3_Pos)              /*!< 0x00020000 */
#define RCC_RSTN_CTRL_I2C3               RCC_RSTN_CTRL_I2C3_Msk                          /*!<RSTN I2C3  control   */
#define RCC_RSTN_CTRL_CAN2_Pos           (18U)
#define RCC_RSTN_CTRL_CAN2_Msk           (0x01UL << RCC_RSTN_CTRL_CAN2_Pos)              /*!< 0x00040000 */
#define RCC_RSTN_CTRL_CAN2               RCC_RSTN_CTRL_CAN2_Msk                          /*!<RSTN CAN2  control   */
#define RCC_RSTN_CTRL_CAN3_Pos           (19U)
#define RCC_RSTN_CTRL_CAN3_Msk           (0x01UL << RCC_RSTN_CTRL_CAN3_Pos)              /*!< 0x00080000 */
#define RCC_RSTN_CTRL_CAN3               RCC_RSTN_CTRL_CAN3_Msk                          /*!<RSTN CAN3  control   */
#define RCC_RSTN_CTRL_DC_Pos             (20U)
#define RCC_RSTN_CTRL_DC_Msk             (0x01UL << RCC_RSTN_CTRL_DC_Pos)                /*!< 0x00100000 */
#define RCC_RSTN_CTRL_DC                 RCC_RSTN_CTRL_DC_Msk                            /*!<RSTN DC  control   */
#define RCC_RSTN_CTRL_USB2_Pos           (23U)
#define RCC_RSTN_CTRL_USB2_Msk           (0x01UL << RCC_RSTN_CTRL_USB2_Pos)              /*!< 0x00800000 */
#define RCC_RSTN_CTRL_USB2               RCC_RSTN_CTRL_USB2_Msk                          /*!<RSTN USB2  control   */
#define RCC_RSTN_CTRL_CPU1_Pos           (30U)
#define RCC_RSTN_CTRL_CPU1_Msk           (0x01UL << RCC_RSTN_CTRL_CPU1_Pos)              /*!< 0x40000000 */
#define RCC_RSTN_CTRL_CPU1               RCC_RSTN_CTRL_CPU1_Msk                          /*!<RSTN CPU1  control   */
#define RCC_RSTN_CTRL_SW_Pos             (31U)
#define RCC_RSTN_CTRL_SW_Msk             (0x01UL << RCC_RSTN_CTRL_SW_Pos)                /*!< 0x80000000 */
#define RCC_RSTN_CTRL_SW                 RCC_RSTN_CTRL_SW_Msk                            /*!<RSTN SW  control   */

#define RCC_ROM_CTRL_PGEN_Pos            (0U)
#define RCC_ROM_CTRL_PGEN_Msk            (0x01UL << RCC_ROM_CTRL_PGEN_Pos)               /*!< 0x00000001 */
#define RCC_ROM_CTRL_PGEN                RCC_ROM_CTRL_PGEN_Msk                           /*!<ROM PGEN  control   */
#define RCC_WDG_CTRL_SPEEDUP_Pos         (0U)
#define RCC_WDG_CTRL_SPEEDUP_Msk         (0x01UL << RCC_WDG_CTRL_SPEEDUP_Pos)            /*!< 0x00000001 */
#define RCC_WDG_CTRL_SPEEDUP             RCC_WDG_CTRL_SPEEDUP_Msk                        /*!<WDG SPEEDUP  control   */
#define RCC_ADC0_CTRL_PRES_Pos           (0U)
#define RCC_ADC0_CTRL_PRES_Msk           (0x07UL << RCC_ADC0_CTRL_PRES_Pos)              /*!< 0x00000007 */
#define RCC_ADC0_CTRL_PRES               RCC_ADC0_CTRL_PRES_Msk                          /*!<ADC0 PRES  control   */
#define RCC_ADC1_CTRL_PRES_Pos           (0U)
#define RCC_ADC1_CTRL_PRES_Msk           (0x07UL << RCC_ADC1_CTRL_PRES_Pos)              /*!< 0x00000007 */
#define RCC_ADC1_CTRL_PRES               RCC_ADC1_CTRL_PRES_Msk                          /*!<ADC1 PRES  control   */
#define RCC_MOTOR_CTRL_PRES_Pos          (0U)
#define RCC_MOTOR_CTRL_PRES_Msk          (0x07UL << RCC_MOTOR_CTRL_PRES_Pos)             /*!< 0x00000007 */
#define RCC_MOTOR_CTRL_PRES              RCC_MOTOR_CTRL_PRES_Msk                         /*!<MOTOR PRES  control   */

#define RCC_WRITE_PROTECT_CTRL           (0x504F4646)                                    /*!< ?????*/
/******************************************************************************/
/*                                                                            */
/*                         General Purpose Input/Output                       */
/*                                                                            */
/******************************************************************************/
#define GPIO_Px_OUT_Pos               (0U)
#define GPIO_Px_OUT_Msk               (0x1UL << GPIO_Px_OUT_Pos)                /*!< 0x000000001 */
#define GPIO_Px_OUT                   GPIO_Px_OUT_Msk                           /*!<Output control   */
#define GPIO_Px_IN_Pos                (1U)
#define GPIO_Px_IN_Msk                (0x1UL << GPIO_Px_IN_Pos)                 /*!< 0x00000002 */
#define GPIO_Px_IN                    GPIO_Px_IN_Msk                            /*!<Input control    */
#define GPIO_Px_ISense_Pos            (8U)
#define GPIO_Px_ISense_Msk            (0x1UL << GPIO_Px_ISense_Pos)             /*!< 0x00000100 */
#define GPIO_Px_ISense                GPIO_Px_ISense_Msk                        /*!<ISense control   */
#define GPIO_Px_IEdge_Pos             (9U)
#define GPIO_Px_IEdge_Msk             (0x1UL << GPIO_Px_IEdge_Pos)              /*!< 0x00000200 */
#define GPIO_Px_IEdge                 GPIO_Px_IEdge_Msk                         /*!<IEdge control    */
#define GPIO_Px_Ievent_Pos            (10U)
#define GPIO_Px_Ievent_Msk            (0x1UL << GPIO_Px_Ievent_Pos)             /*!< 0x00000400 */
#define GPIO_Px_Ievent                GPIO_Px_Ievent_Msk                        /*!<Ievent control   */
#define GPIO_Px_IEn_Pos               (11U)
#define GPIO_Px_IEn_Msk               (0x1UL << GPIO_Px_IEn_Pos)                /*!< 0x00000800 */
#define GPIO_Px_IEn                   GPIO_Px_IEn_Msk                           /*!<IEn control      */
#define GPIO_Px_IClr_Pos              (12U)
#define GPIO_Px_IClr_Msk              (0x1UL << GPIO_Px_IClr_Pos)                /*!< 0x00001000 */
#define GPIO_Px_IClr                  GPIO_Px_IClr_Msk                          /*!<IClr control     */
#define GPIO_Px_DIR_Pos               (16U)
#define GPIO_Px_DIR_Msk               (0x1UL << GPIO_Px_DIR_Pos)                /*!< 0x00010000 */
#define GPIO_Px_DIR                   GPIO_Px_DIR_Msk                           /*!<Dir control      */
#define GPIO_Px_OD_Pos                (17U)
#define GPIO_Px_OD_Msk                (0x1UL << GPIO_Px_OD_Pos)                 /*!< 0x00020000 */
#define GPIO_Px_OD                    GPIO_Px_OD_Msk                            /*!<OD               */
#define GPIO_Px_PullDown_Pos          (18U)
#define GPIO_Px_PullDown_Msk          (0x1UL << GPIO_Px_PullDown_Pos)           /*!< 0x00040000 */
#define GPIO_Px_PullDown              GPIO_Px_PullDown_Msk                      /*!<PullDown         */
#define GPIO_Px_PullUp_Pos            (18U)
#define GPIO_Px_PullUp_Msk            (0x1UL << GPIO_Px_PullUp_Pos)             /*!< 0x00080000 */
#define GPIO_Px_PullUp                GPIO_Px_PullUp_Msk                        /*!<PullUp           */
#define GPIO_Px_ALTF_Pos              (24U)
#define GPIO_Px_ALTF_Msk              (0x7UL << GPIO_Px_ALTF_Pos)               /*!< 0x07000000 */
#define GPIO_Px_ALTF                  GPIO_Px_ALTF_Msk                          /*!<ALTF             */
#define GPIO_Px_ST_Pos                (28U)
#define GPIO_Px_ST_Msk                (0x1UL << GPIO_Px_ST_Pos)                 /*!< 0x10000000 */
#define GPIO_Px_ST                    GPIO_Px_ST_Msk                            /*!<Schmidt Trigger  */
#define GPIO_Px_SL_Pos                (29U)
#define GPIO_Px_SL_Msk                (0x1UL << GPIO_Px_SL_Pos)                 /*!< 0x20000000 */
#define GPIO_Px_SL                    GPIO_Px_SL_Msk                            /*!<Speed            */
#define GPIO_Px_DS_Pos                (30U)
#define GPIO_Px_DS_Msk                (0x3UL << GPIO_Px_DS_Pos)                 /*!< 0xC0000000 */
#define GPIO_Px_DS                    GPIO_Px_DS_Msk                            /*!<driver strength  */
#define GPIO_MODE_G0_TTL_Pos          (0U)
#define GPIO_MODE_G0_TTL_Msk          (0x1UL << GPIO_MODE_G0_TTL_Pos)           /*!< 0x00000001 */
#define GPIO_MODE_G0_TTL              GPIO_MODE_G0_TTL_Msk                      /*!<MODE G0 TTL  */
#define GPIO_MODE_G1_TTL_Pos          (1U)
#define GPIO_MODE_G1_TTL_Msk          (0x1UL << GPIO_MODE_G1_TTL_Pos)           /*!< 0x00000002 */
#define GPIO_MODE_G1_TTL              GPIO_MODE_G1_TTL_Msk                      /*!<MODE G1 TTL  */
#define GPIO_MODE_LVDS_Pos            (2U)
#define GPIO_MODE_LVDS_Msk            (0x1UL << GPIO_MODE_LVDS_Pos)             /*!< 0x00000004 */
#define GPIO_MODE_LVDS                GPIO_MODE_LVDS_Msk                        /*!<MODE LVDS  */
#define GPIO_MODE_CLK_SEL_Pos         (3U)
#define GPIO_MODE_CLK_SEL_Msk         (0x1UL << GPIO_MODE_CLK_SEL_Pos)          /*!< 0x00000008 */
#define GPIO_MODE_CLK_SEL             GPIO_MODE_CLK_SEL_Msk                     /*!<MODE CLK SEL  */
#define GPIO_MODE_TURENDIS_Pos        (4U)
#define GPIO_MODE_TURENDIS_Msk        (0x1UL << GPIO_MODE_TURENDIS_Pos)         /*!< 0x00000010 */
#define GPIO_MODE_TURENDIS            GPIO_MODE_TURENDIS_Msk                    /*!<MODE TURENDIS  */
#define GPIO_MODE_TXSTOP_Pos          (5U)
#define GPIO_MODE_TXSTOP_Msk          (0x1UL << GPIO_MODE_TXSTOP_Pos)           /*!< 0x00000020 */
#define GPIO_MODE_TXSTOP              GPIO_MODE_TXSTOP_Msk                      /*!<MODE TXSTOP  */
#define GPIO_MODE_FORCERX_Pos         (6U)
#define GPIO_MODE_FORCERX_Msk         (0x1UL << GPIO_MODE_FORCERX_Pos)          /*!< 0x00000040 */
#define GPIO_MODE_FORCERX             GPIO_MODE_FORCERX_Msk                     /*!<MODE FORCERX  */

/******************************************************************************/
/*                                                                            */
/*         Universal Synchronous Asynchronous Receiver Transmitter            */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for UART_SR register  *******************/
#define UART_SR_PE_Pos               (0U)
#define UART_SR_PE_Msk               (0x1UL << UART_SR_PE_Pos)                /*!< 0x00000001 */
#define UART_SR_PE                   UART_SR_PE_Msk                          /*!<Parity Error                 */
#define UART_SR_FE_Pos               (1U)
#define UART_SR_FE_Msk               (0x1UL << UART_SR_FE_Pos)                /*!< 0x00000002 */
#define UART_SR_FE                   UART_SR_FE_Msk                          /*!<Framing Error                */
#define UART_SR_NE_Pos               (2U)
#define UART_SR_NE_Msk               (0x1UL << UART_SR_NE_Pos)                /*!< 0x00000004 */
#define UART_SR_NE                   UART_SR_NE_Msk                          /*!<Noise Error Flag             */
#define UART_SR_ORE_Pos              (3U)
#define UART_SR_ORE_Msk              (0x1UL << UART_SR_ORE_Pos)               /*!< 0x00000008 */
#define UART_SR_ORE                  UART_SR_ORE_Msk                         /*!<OverRun Error                */
#define UART_SR_IDLE_Pos             (4U)
#define UART_SR_IDLE_Msk             (0x1UL << UART_SR_IDLE_Pos)              /*!< 0x00000010 */
#define UART_SR_IDLE                 UART_SR_IDLE_Msk                        /*!<IDLE line detected           */
#define UART_SR_RXNE_Pos             (5U)
#define UART_SR_RXNE_Msk             (0x1UL << UART_SR_RXNE_Pos)              /*!< 0x00000020 */
#define UART_SR_RXNE                 UART_SR_RXNE_Msk                        /*!<Read Data Register Not Empty */
#define UART_SR_TC_Pos               (6U)
#define UART_SR_TC_Msk               (0x1UL << UART_SR_TC_Pos)                /*!< 0x00000040 */
#define UART_SR_TC                   UART_SR_TC_Msk                          /*!<Transmission Complete        */
#define UART_SR_TXE_Pos              (7U)
#define UART_SR_TXE_Msk              (0x1UL << UART_SR_TXE_Pos)               /*!< 0x00000080 */
#define UART_SR_TXE                  UART_SR_TXE_Msk                         /*!<Transmit Data Register Empty */
#define UART_SR_LBD_Pos              (8U)
#define UART_SR_LBD_Msk              (0x1UL << UART_SR_LBD_Pos)               /*!< 0x00000100 */
#define UART_SR_LBD                  UART_SR_LBD_Msk                         /*!<LIN Break Detection Flag     */
#define UART_SR_CTS_Pos              (9U)
#define UART_SR_CTS_Msk              (0x1UL << UART_SR_CTS_Pos)               /*!< 0x00000200 */
#define UART_SR_CTS                  UART_SR_CTS_Msk                         /*!<CTS Flag                     */

/*******************  Bit definition for UART_DR register  *******************/
#define UART_DR_DR_Pos               (0U)
#define UART_DR_DR_Msk               (0xFFUL << UART_DR_DR_Pos)              /*!< 0x000000FF */
#define UART_DR_DR                   UART_DR_DR_Msk                          /*!<Data value */

/******************  Bit definition for UART_BRR register  *******************/
#define UART_BRR_DIV_Fraction_Pos    (0U)
#define UART_BRR_DIV_Fraction_Msk    (0xFUL << UART_BRR_DIV_Fraction_Pos)     /*!< 0x0000000F */
#define UART_BRR_DIV_Fraction        UART_BRR_DIV_Fraction_Msk               /*!<Fraction of UARTDIV */
#define UART_BRR_DIV_Mantissa_Pos    (4U)
#define UART_BRR_DIV_Mantissa_Msk    (0xFFFUL << UART_BRR_DIV_Mantissa_Pos)   /*!< 0x0000FFF0 */
#define UART_BRR_DIV_Mantissa        UART_BRR_DIV_Mantissa_Msk               /*!<Mantissa of UARTDIV */

/******************  Bit definition for UART_CR1 register  *******************/
#define UART_CR1_SBK_Pos             (0U)
#define UART_CR1_SBK_Msk             (0x1UL << UART_CR1_SBK_Pos)              /*!< 0x00000001 */
#define UART_CR1_SBK                 UART_CR1_SBK_Msk                        /*!<Send Break                             */
#define UART_CR1_RWU_Pos             (1U)
#define UART_CR1_RWU_Msk             (0x1UL << UART_CR1_RWU_Pos)              /*!< 0x00000002 */
#define UART_CR1_RWU                 UART_CR1_RWU_Msk                        /*!<Receiver wakeup                        */
#define UART_CR1_RE_Pos              (2U)
#define UART_CR1_RE_Msk              (0x1UL << UART_CR1_RE_Pos)               /*!< 0x00000004 */
#define UART_CR1_RE                  UART_CR1_RE_Msk                         /*!<Receiver Enable                        */
#define UART_CR1_TE_Pos              (3U)
#define UART_CR1_TE_Msk              (0x1UL << UART_CR1_TE_Pos)               /*!< 0x00000008 */
#define UART_CR1_TE                  UART_CR1_TE_Msk                         /*!<Transmitter Enable                     */
#define UART_CR1_IDLEIE_Pos          (4U)
#define UART_CR1_IDLEIE_Msk          (0x1UL << UART_CR1_IDLEIE_Pos)           /*!< 0x00000010 */
#define UART_CR1_IDLEIE              UART_CR1_IDLEIE_Msk                     /*!<IDLE Interrupt Enable                  */
#define UART_CR1_RXNEIE_Pos          (5U)
#define UART_CR1_RXNEIE_Msk          (0x1UL << UART_CR1_RXNEIE_Pos)           /*!< 0x00000020 */
#define UART_CR1_RXNEIE              UART_CR1_RXNEIE_Msk                     /*!<RXNE Interrupt Enable                  */
#define UART_CR1_TCIE_Pos            (6U)
#define UART_CR1_TCIE_Msk            (0x1UL << UART_CR1_TCIE_Pos)             /*!< 0x00000040 */
#define UART_CR1_TCIE                UART_CR1_TCIE_Msk                       /*!<Transmission Complete Interrupt Enable */
#define UART_CR1_TXEIE_Pos           (7U)
#define UART_CR1_TXEIE_Msk           (0x1UL << UART_CR1_TXEIE_Pos)            /*!< 0x00000080 */
#define UART_CR1_TXEIE               UART_CR1_TXEIE_Msk                      /*!<TXE Interrupt Enable                   */
#define UART_CR1_PEIE_Pos            (8U)
#define UART_CR1_PEIE_Msk            (0x1UL << UART_CR1_PEIE_Pos)             /*!< 0x00000100 */
#define UART_CR1_PEIE                UART_CR1_PEIE_Msk                       /*!<PE Interrupt Enable                    */
#define UART_CR1_PS_Pos              (9U)
#define UART_CR1_PS_Msk              (0x1UL << UART_CR1_PS_Pos)               /*!< 0x00000200 */
#define UART_CR1_PS                  UART_CR1_PS_Msk                         /*!<Parity Selection                       */
#define UART_CR1_PCE_Pos             (10U)
#define UART_CR1_PCE_Msk             (0x1UL << UART_CR1_PCE_Pos)              /*!< 0x00000400 */
#define UART_CR1_PCE                 UART_CR1_PCE_Msk                        /*!<Parity Control Enable                  */
#define UART_CR1_WAKE_Pos            (11U)
#define UART_CR1_WAKE_Msk            (0x1UL << UART_CR1_WAKE_Pos)             /*!< 0x00000800 */
#define UART_CR1_WAKE                UART_CR1_WAKE_Msk                       /*!<Wakeup method                          */
#define UART_CR1_M_Pos               (12U)
#define UART_CR1_M_Msk               (0x1UL << UART_CR1_M_Pos)                /*!< 0x00001000 */
#define UART_CR1_M                   UART_CR1_M_Msk                          /*!<Word length                            */
#define UART_CR1_UE_Pos              (13U)
#define UART_CR1_UE_Msk              (0x1UL << UART_CR1_UE_Pos)               /*!< 0x00002000 */
#define UART_CR1_UE                  UART_CR1_UE_Msk                         /*!<UART Enable                           */
//gc9005???????????
//#define UART_CR1_OVER8_Pos           (15U)
//#define UART_CR1_OVER8_Msk           (0x1UL << UART_CR1_OVER8_Pos)            /*!< 0x00008000 */
//#define UART_CR1_OVER8               UART_CR1_OVER8_Msk                      /*!<UART Oversampling by 8 enable

/******************  Bit definition for UART_CR2 register  *******************/
#define UART_CR2_ADD_Pos             (0U)
#define UART_CR2_ADD_Msk             (0xFUL << UART_CR2_ADD_Pos)              /*!< 0x0000000F */
#define UART_CR2_ADD                 UART_CR2_ADD_Msk                        /*!<Address of the UART node            */
#define UART_CR2_LBDL_Pos            (5U)
#define UART_CR2_LBDL_Msk            (0x1UL << UART_CR2_LBDL_Pos)             /*!< 0x00000020 */
#define UART_CR2_LBDL                UART_CR2_LBDL_Msk                       /*!<LIN Break Detection Length           */
#define UART_CR2_LBDIE_Pos           (6U)
#define UART_CR2_LBDIE_Msk           (0x1UL << UART_CR2_LBDIE_Pos)            /*!< 0x00000040 */
#define UART_CR2_LBDIE               UART_CR2_LBDIE_Msk                      /*!<LIN Break Detection Interrupt Enable */
#define UART_CR2_LBCL_Pos            (8U)
#define UART_CR2_LBCL_Msk            (0x1UL << UART_CR2_LBCL_Pos)             /*!< 0x00000100 */
#define UART_CR2_LBCL                UART_CR2_LBCL_Msk                       /*!<Last Bit Clock pulse                 */
#define UART_CR2_CPHA_Pos            (9U)
#define UART_CR2_CPHA_Msk            (0x1UL << UART_CR2_CPHA_Pos)             /*!< 0x00000200 */
#define UART_CR2_CPHA                UART_CR2_CPHA_Msk                       /*!<Clock Phase                          */
#define UART_CR2_CPOL_Pos            (10U)
#define UART_CR2_CPOL_Msk            (0x1UL << UART_CR2_CPOL_Pos)             /*!< 0x00000400 */
#define UART_CR2_CPOL                UART_CR2_CPOL_Msk                       /*!<Clock Polarity                       */
#define UART_CR2_CLKEN_Pos           (11U)
#define UART_CR2_CLKEN_Msk           (0x1UL << UART_CR2_CLKEN_Pos)            /*!< 0x00000800 */
#define UART_CR2_CLKEN               UART_CR2_CLKEN_Msk                      /*!<Clock Enable                         */

#define UART_CR2_STOP_Pos            (12U)
#define UART_CR2_STOP_Msk            (0x3UL << UART_CR2_STOP_Pos)             /*!< 0x00003000 */
#define UART_CR2_STOP                UART_CR2_STOP_Msk                       /*!<STOP[1:0] bits (STOP bits) */
#define UART_CR2_STOP_1              (0x0UL << UART_CR2_STOP_Pos)             /*!< 0x0000 */
#define UART_CR2_STOP_0_5            (0x1UL << UART_CR2_STOP_Pos)             /*!< 0x1000 */
#define UART_CR2_STOP_2              (0x2UL << UART_CR2_STOP_Pos)             /*!< 0x200 */
#define UART_CR2_STOP_1_5            (0x3UL << UART_CR2_STOP_Pos)             /*!< 0x3000 */


#define UART_CR2_LINEN_Pos           (14U)
#define UART_CR2_LINEN_Msk           (0x1UL << UART_CR2_LINEN_Pos)            /*!< 0x00004000 */
#define UART_CR2_LINEN               UART_CR2_LINEN_Msk                      /*!<LIN mode enable */


/******************  Bit definition for UART_CR3 register  *******************/
#define UART_CR3_EIE_Pos             (0U)
#define UART_CR3_EIE_Msk             (0x1UL << UART_CR3_EIE_Pos)              /*!< 0x00000001 */
#define UART_CR3_EIE                 UART_CR3_EIE_Msk                        /*!<Error Interrupt Enable      */
#define UART_CR3_IREN_Pos            (1U)
#define UART_CR3_IREN_Msk            (0x1UL << UART_CR3_IREN_Pos)             /*!< 0x00000002 */
#define UART_CR3_IREN                UART_CR3_IREN_Msk                       /*!<IrDA mode Enable            */
#define UART_CR3_IRLP_Pos            (2U)
#define UART_CR3_IRLP_Msk            (0x1UL << UART_CR3_IRLP_Pos)             /*!< 0x00000004 */
#define UART_CR3_IRLP                UART_CR3_IRLP_Msk                       /*!<IrDA Low-Power              */
#define UART_CR3_HDSEL_Pos           (3U)
#define UART_CR3_HDSEL_Msk           (0x1UL << UART_CR3_HDSEL_Pos)            /*!< 0x00000008 */
#define UART_CR3_HDSEL               UART_CR3_HDSEL_Msk                      /*!<Half-Duplex Selection       */
#define UART_CR3_NACK_Pos            (4U)
#define UART_CR3_NACK_Msk            (0x1UL << UART_CR3_NACK_Pos)             /*!< 0x00000010 */
#define UART_CR3_NACK                UART_CR3_NACK_Msk                       /*!<Smartcard NACK enable       */
#define UART_CR3_SCEN_Pos            (5U)
#define UART_CR3_SCEN_Msk            (0x1UL << UART_CR3_SCEN_Pos)             /*!< 0x00000020 */
#define UART_CR3_SCEN                UART_CR3_SCEN_Msk                       /*!<Smartcard mode enable       */
#define UART_CR3_DMAR_Pos            (6U)
#define UART_CR3_DMAR_Msk            (0x1UL << UART_CR3_DMAR_Pos)             /*!< 0x00000040 */
#define UART_CR3_DMAR                UART_CR3_DMAR_Msk                       /*!<DMA Enable Receiver         */
#define UART_CR3_DMAT_Pos            (7U)
#define UART_CR3_DMAT_Msk            (0x1UL << UART_CR3_DMAT_Pos)             /*!< 0x00000080 */
#define UART_CR3_DMAT                UART_CR3_DMAT_Msk                       /*!<DMA Enable Transmitter      */
#define UART_CR3_RTSE_Pos            (8U)
#define UART_CR3_RTSE_Msk            (0x1UL << UART_CR3_RTSE_Pos)             /*!< 0x00000100 */
#define UART_CR3_RTSE                UART_CR3_RTSE_Msk                       /*!<RTS Enable                  */
#define UART_CR3_CTSE_Pos            (9U)
#define UART_CR3_CTSE_Msk            (0x1UL << UART_CR3_CTSE_Pos)             /*!< 0x00000200 */
#define UART_CR3_CTSE                UART_CR3_CTSE_Msk                       /*!<CTS Enable                  */
#define UART_CR3_CTSIE_Pos           (10U)
#define UART_CR3_CTSIE_Msk           (0x1UL << UART_CR3_CTSIE_Pos)            /*!< 0x00000400 */
#define UART_CR3_CTSIE               UART_CR3_CTSIE_Msk                      /*!<CTS Interrupt Enable        */
//#define UART_CR3_ONEBIT_Pos          (11U)
//#define UART_CR3_ONEBIT_Msk          (0x1UL << UART_CR3_ONEBIT_Pos)           /*!< 0x00000800 */
//#define UART_CR3_ONEBIT              UART_CR3_ONEBIT_Msk                     /*!<UART One bit method enable */

/******************  Bit definition for UART_GTPR register  ******************/
#define UART_GTPR_PSC_Pos            (0U)
#define UART_GTPR_PSC_Msk            (0xFFUL << UART_GTPR_PSC_Pos)            /*!< 0x000000FF */
#define UART_GTPR_PSC                UART_GTPR_PSC_Msk                       /*!<PSC[7:0] bits (Prescaler value) */
#define UART_GTPR_PSC_0              (0x01UL << UART_GTPR_PSC_Pos)            /*!< 0x0001 */
#define UART_GTPR_PSC_1              (0x02UL << UART_GTPR_PSC_Pos)            /*!< 0x0002 */
#define UART_GTPR_PSC_2              (0x04UL << UART_GTPR_PSC_Pos)            /*!< 0x0004 */
#define UART_GTPR_PSC_3              (0x08UL << UART_GTPR_PSC_Pos)            /*!< 0x0008 */
#define UART_GTPR_PSC_4              (0x10UL << UART_GTPR_PSC_Pos)            /*!< 0x0010 */
#define UART_GTPR_PSC_5              (0x20UL << UART_GTPR_PSC_Pos)            /*!< 0x0020 */
#define UART_GTPR_PSC_6              (0x40UL << UART_GTPR_PSC_Pos)            /*!< 0x0040 */
#define UART_GTPR_PSC_7              (0x80UL << UART_GTPR_PSC_Pos)            /*!< 0x0080 */

#define UART_GTPR_GT_Pos             (8U)
#define UART_GTPR_GT_Msk             (0xFFUL << UART_GTPR_GT_Pos)             /*!< 0x0000FF00 */
#define UART_GTPR_GT                 UART_GTPR_GT_Msk                        /*!<Guard time value */
/******************************************************************************/
/*                                                                            */
/*                         Direct Memory Access                               */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for DMAC_CFGREG register  ******************/
#define DMA_CFG_DMAC_EN_Pos               (0U)
#define DMA_CFG_DMAC_EN_Msk               (0x01UL << DMA_CFG_DMAC_EN_Pos)               /*!< 0x00000000 00000001 */
#define DMA_CFG_DMAC_EN                   DMA_CFG_DMAC_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac. */
#define DMA_CFG_INT_EN_Pos                (1U)
#define DMA_CFG_INT_EN_Msk                (0x01UL << DMA_CFG_INT_EN_Pos)                /*!< 0x00000000 00000002 */
#define DMA_CFG_INT_EN                    DMA_CFG_INT_EN_Msk                            /*!< This bit is used to globally enable the interrupt generation. */

/******************  Bit definition for DMAC_CHENREG register  ******************/
#define DMA_CHEN_CH1_EN_Pos               (0U)
#define DMA_CHEN_CH1_EN_Msk               (0x01UL << DMA_CHEN_CH1_EN_Pos)               /*!< 0x00000000 00000001 */
#define DMA_CHEN_CH1_EN                   DMA_CHEN_CH1_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-1. */
#define DMA_CHEN_CH2_EN_Pos               (1U)
#define DMA_CHEN_CH2_EN_Msk               (0x01UL << DMA_CHEN_CH2_EN_Pos)               /*!< 0x00000000 00000002 */
#define DMA_CHEN_CH2_EN                   DMA_CHEN_CH2_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-2. */
#define DMA_CHEN_CH3_EN_Pos               (2U)
#define DMA_CHEN_CH3_EN_Msk               (0x01UL << DMA_CHEN_CH3_EN_Pos)               /*!< 0x00000000 00000004 */
#define DMA_CHEN_CH3_EN                   DMA_CHEN_CH3_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-3. */
#define DMA_CHEN_CH4_EN_Pos               (3U)
#define DMA_CHEN_CH4_EN_Msk               (0x01UL << DMA_CHEN_CH4_EN_Pos)               /*!< 0x00000000 00000008 */
#define DMA_CHEN_CH4_EN                   DMA_CHEN_CH4_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-4. */
#define DMA_CHEN_CH5_EN_Pos               (4U)
#define DMA_CHEN_CH5_EN_Msk               (0x01UL << DMA_CHEN_CH5_EN_Pos)               /*!< 0x00000000 00000010 */
#define DMA_CHEN_CH5_EN                   DMA_CHEN_CH5_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-5. */
#define DMA_CHEN_CH6_EN_Pos               (5U)
#define DMA_CHEN_CH6_EN_Msk               (0x01UL << DMA_CHEN_CH6_EN_Pos)               /*!< 0x00000000 00000020 */
#define DMA_CHEN_CH6_EN                   DMA_CHEN_CH6_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-6. */
#define DMA_CHEN_CH7_EN_Pos               (6U)
#define DMA_CHEN_CH7_EN_Msk               (0x01UL << DMA_CHEN_CH7_EN_Pos)               /*!< 0x00000000 00000040 */
#define DMA_CHEN_CH7_EN                   DMA_CHEN_CH7_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-7. */
#define DMA_CHEN_CH8_EN_Pos               (7U)
#define DMA_CHEN_CH8_EN_Msk               (0x01UL << DMA_CHEN_CH8_EN_Pos)               /*!< 0x00000000 00000080 */
#define DMA_CHEN_CH8_EN                   DMA_CHEN_CH8_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-8. */
#define DMA_CHEN_CH1_EN_WE_Pos               (8U)
#define DMA_CHEN_CH1_EN_WE_Msk               (0x01UL << DMA_CHEN_CH1_EN_WE_Pos)               /*!< 0x00000000 00000100 */
#define DMA_CHEN_CH1_EN_WE                   DMA_CHEN_CH1_EN_WE_Msk                           /*!< DW_axi_dmac Channel-1 Enable Write Enable bit. */
#define DMA_CHEN_CH2_EN_WE_Pos               (9U)
#define DMA_CHEN_CH2_EN_WE_Msk               (0x01UL << DMA_CHEN_CH2_EN_WE_Pos)               /*!< 0x00000000 00000200 */
#define DMA_CHEN_CH2_EN_WE                   DMA_CHEN_CH2_EN_WE_Msk                           /*!< DW_axi_dmac Channel-2 Enable Write Enable bit. */
#define DMA_CHEN_CH3_EN_WE_Pos               (10U)
#define DMA_CHEN_CH3_EN_WE_Msk               (0x01UL << DMA_CHEN_CH3_EN_WE_Pos)               /*!< 0x00000000 00000400 */
#define DMA_CHEN_CH3_EN_WE                   DMA_CHEN_CH3_EN_WE_Msk                           /*!< DW_axi_dmac Channel-3 Enable Write Enable bit. */
#define DMA_CHEN_CH4_EN_WE_Pos               (11U)
#define DMA_CHEN_CH4_EN_WE_Msk               (0x01UL << DMA_CHEN_CH4_EN_WE_Pos)               /*!< 0x00000000 00000800 */
#define DMA_CHEN_CH4_EN_WE                   DMA_CHEN_CH4_EN_WE_Msk                           /*!< DW_axi_dmac Channel-4 Enable Write Enable bit. */
#define DMA_CHEN_CH5_EN_WE_Pos               (12U)
#define DMA_CHEN_CH5_EN_WE_Msk               (0x01UL << DMA_CHEN_CH5_EN_WE_Pos)               /*!< 0x00000000 00001000 */
#define DMA_CHEN_CH5_EN_WE                   DMA_CHEN_CH5_EN_WE_Msk                           /*!< DW_axi_dmac Channel-5 Enable Write Enable bit. */
#define DMA_CHEN_CH6_EN_WE_Pos               (13U)
#define DMA_CHEN_CH6_EN_WE_Msk               (0x01UL << DMA_CHEN_CH6_EN_WE_Pos)               /*!< 0x00000000 00002000 */
#define DMA_CHEN_CH6_EN_WE                   DMA_CHEN_CH6_EN_WE_Msk                           /*!< DW_axi_dmac Channel-6 Enable Write Enable bit. */
#define DMA_CHEN_CH7_EN_WE_Pos               (14U)
#define DMA_CHEN_CH7_EN_WE_Msk               (0x01UL << DMA_CHEN_CH7_EN_WE_Pos)               /*!< 0x00000000 00004000 */
#define DMA_CHEN_CH7_EN_WE                   DMA_CHEN_CH7_EN_WE_Msk                           /*!< DW_axi_dmac Channel-7 Enable Write Enable bit. */
#define DMA_CHEN_CH8_EN_WE_Pos               (15U)
#define DMA_CHEN_CH8_EN_WE_Msk               (0x01UL << DMA_CHEN_CH8_EN_WE_Pos)               /*!< 0x00000000 00008000 */
#define DMA_CHEN_CH8_EN_WE                   DMA_CHEN_CH8_EN_WE_Msk                           /*!< DW_axi_dmac Channel-8 Enable Write Enable bit. */
#define DMA_CHEN_CH1_SUSP_Pos               (16U)
#define DMA_CHEN_CH1_SUSP_Msk               (0x01UL << DMA_CHEN_CH1_SUSP_Pos)               /*!< 0x00000000 00010000 */
#define DMA_CHEN_CH1_SUSP                   DMA_CHEN_CH1_SUSP_Msk                           /*!< Channel-1 Suspend Request. */
#define DMA_CHEN_CH2_SUSP_Pos               (17U)
#define DMA_CHEN_CH2_SUSP_Msk               (0x01UL << DMA_CHEN_CH2_SUSP_Pos)               /*!< 0x00000000 00020000 */
#define DMA_CHEN_CH2_SUSP                   DMA_CHEN_CH2_SUSP_Msk                           /*!< Channel-2 Suspend Request. */
#define DMA_CHEN_CH3_SUSP_Pos               (18U)
#define DMA_CHEN_CH3_SUSP_Msk               (0x01UL << DMA_CHEN_CH3_SUSP_Pos)               /*!< 0x00000000 00040000 */
#define DMA_CHEN_CH3_SUSP                   DMA_CHEN_CH3_SUSP_Msk                           /*!< Channel-3 Suspend Request. */
#define DMA_CHEN_CH4_SUSP_Pos               (19U)
#define DMA_CHEN_CH4_SUSP_Msk               (0x01UL << DMA_CHEN_CH4_SUSP_Pos)               /*!< 0x00000000 00080000 */
#define DMA_CHEN_CH4_SUSP                   DMA_CHEN_CH4_SUSP_Msk                           /*!< Channel-4 Suspend Request. */
#define DMA_CHEN_CH5_SUSP_Pos               (20U)
#define DMA_CHEN_CH5_SUSP_Msk               (0x01UL << DMA_CHEN_CH5_SUSP_Pos)               /*!< 0x00000000 00100000 */
#define DMA_CHEN_CH5_SUSP                   DMA_CHEN_CH5_SUSP_Msk                           /*!< Channel-5 Suspend Request. */
#define DMA_CHEN_CH6_SUSP_Pos               (21U)
#define DMA_CHEN_CH6_SUSP_Msk               (0x01UL << DMA_CHEN_CH6_SUSP_Pos)               /*!< 0x00000000 00200000 */
#define DMA_CHEN_CH6_SUSP                   DMA_CHEN_CH6_SUSP_Msk                           /*!< Channel-6 Suspend Request. */
#define DMA_CHEN_CH7_SUSP_Pos               (22U)
#define DMA_CHEN_CH7_SUSP_Msk               (0x01UL << DMA_CHEN_CH7_SUSP_Pos)               /*!< 0x00000000 00400000 */
#define DMA_CHEN_CH7_SUSP                   DMA_CHEN_CH7_SUSP_Msk                           /*!< Channel-7 Suspend Request. */
#define DMA_CHEN_CH8_SUSP_Pos               (23U)
#define DMA_CHEN_CH8_SUSP_Msk               (0x01UL << DMA_CHEN_CH8_SUSP_Pos)               /*!< 0x00000000 00800000 */
#define DMA_CHEN_CH8_SUSP                   DMA_CHEN_CH8_SUSP_Msk                           /*!< Channel-8 Suspend Request. */
#define DMA_CHEN_CH1_SUSP_WE_Pos               (24U)
#define DMA_CHEN_CH1_SUSP_WE_Msk               (0x01UL << DMA_CHEN_CH1_SUSP_WE_Pos)               /*!< 0x00000000 01000000 */
#define DMA_CHEN_CH1_SUSP_WE                   DMA_CHEN_CH1_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-1 Suspend bit. The read back value of this register bit is always 0. */
#define DMA_CHEN_CH2_SUSP_WE_Pos               (25U)
#define DMA_CHEN_CH2_SUSP_WE_Msk               (0x01UL << DMA_CHEN_CH2_SUSP_WE_Pos)               /*!< 0x00000000 02000000 */
#define DMA_CHEN_CH2_SUSP_WE                   DMA_CHEN_CH2_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-2 Suspend bit. The read back value of this register bit is always 0. */
#define DMA_CHEN_CH3_SUSP_WE_Pos               (26U)
#define DMA_CHEN_CH3_SUSP_WE_Msk               (0x01UL << DMA_CHEN_CH3_SUSP_WE_Pos)               /*!< 0x00000000 04000000 */
#define DMA_CHEN_CH3_SUSP_WE                   DMA_CHEN_CH3_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-3 Suspend bit. The read back value of this register bit is always 0. */
#define DMA_CHEN_CH4_SUSP_WE_Pos               (27U)
#define DMA_CHEN_CH4_SUSP_WE_Msk               (0x01UL << DMA_CHEN_CH4_SUSP_WE_Pos)               /*!< 0x00000000 08000000 */
#define DMA_CHEN_CH4_SUSP_WE                   DMA_CHEN_CH4_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-4 Suspend bit. The read back value of this register bit is always 0. */
#define DMA_CHEN_CH5_SUSP_WE_Pos               (28U)
#define DMA_CHEN_CH5_SUSP_WE_Msk               (0x01UL << DMA_CHEN_CH5_SUSP_WE_Pos)               /*!< 0x00000000 10000000 */
#define DMA_CHEN_CH5_SUSP_WE                   DMA_CHEN_CH5_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-5 Suspend bit. The read back value of this register bit is always 0. */
#define DMA_CHEN_CH6_SUSP_WE_Pos               (29U)
#define DMA_CHEN_CH6_SUSP_WE_Msk               (0x01UL << DMA_CHEN_CH6_SUSP_WE_Pos)               /*!< 0x00000000 20000000 */
#define DMA_CHEN_CH6_SUSP_WE                   DMA_CHEN_CH6_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-6 Suspend bit. The read back value of this register bit is always 0. */
#define DMA_CHEN_CH7_SUSP_WE_Pos               (30U)
#define DMA_CHEN_CH7_SUSP_WE_Msk               (0x01UL << DMA_CHEN_CH7_SUSP_WE_Pos)               /*!< 0x00000000 40000000 */
#define DMA_CHEN_CH7_SUSP_WE                   DMA_CHEN_CH7_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-7 Suspend bit. The read back value of this register bit is always 0. */
#define DMA_CHEN_CH8_SUSP_WE_Pos               (31U)
#define DMA_CHEN_CH8_SUSP_WE_Msk               (0x01UL << DMA_CHEN_CH8_SUSP_WE_Pos)               /*!< 0x00000000 80000000 */
#define DMA_CHEN_CH8_SUSP_WE                   DMA_CHEN_CH8_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-8 Suspend bit. The read back value of this register bit is always 0. */
#define DMA_CHEN_CH1_ABORT_Pos               (32U)
#define DMA_CHEN_CH1_ABORT_Msk               (0x01UL << DMA_CHEN_CH1_ABORT_Pos)               /*!< 0x00000001 00000000 */
#define DMA_CHEN_CH1_ABORT                   DMA_CHEN_CH1_ABORT_Msk                           /*!< Channel-1 Abort Request. */
#define DMA_CHEN_CH2_ABORT_Pos               (33U)
#define DMA_CHEN_CH2_ABORT_Msk               (0x01UL << DMA_CHEN_CH2_ABORT_Pos)               /*!< 0x00000002 00000000 */
#define DMA_CHEN_CH2_ABORT                   DMA_CHEN_CH2_ABORT_Msk                           /*!< This bit is used to write enable the Channel-1 Abort bit. */
#define DMA_CHEN_CH3_ABORT_Pos               (34U)
#define DMA_CHEN_CH3_ABORT_Msk               (0x01UL << DMA_CHEN_CH3_ABORT_Pos)               /*!< 0x00000004 00000000 */
#define DMA_CHEN_CH3_ABORT                   DMA_CHEN_CH3_ABORT_Msk                           /*!< Channel-3 Abort Request. */
#define DMA_CHEN_CH4_ABORT_Pos               (35U)
#define DMA_CHEN_CH4_ABORT_Msk               (0x01UL << DMA_CHEN_CH4_ABORT_Pos)               /*!< 0x00000008 00000000 */
#define DMA_CHEN_CH4_ABORT                   DMA_CHEN_CH4_ABORT_Msk                           /*!< Channel-4 Abort Request. */
#define DMA_CHEN_CH5_ABORT_Pos               (36U)
#define DMA_CHEN_CH5_ABORT_Msk               (0x01UL << DMA_CHEN_CH5_ABORT_Pos)               /*!< 0x00000010 00000000 */
#define DMA_CHEN_CH5_ABORT                   DMA_CHEN_CH5_ABORT_Msk                           /*!< Channel-5 Abort Request. */
#define DMA_CHEN_CH6_ABORT_Pos               (37U)
#define DMA_CHEN_CH6_ABORT_Msk               (0x01UL << DMA_CHEN_CH6_ABORT_Pos)               /*!< 0x00000020 00000000 */
#define DMA_CHEN_CH6_ABORT                   DMA_CHEN_CH6_ABORT_Msk                           /*!< Channel-6 Abort Request. */
#define DMA_CHEN_CH7_ABORT_Pos               (38U)
#define DMA_CHEN_CH7_ABORT_Msk               (0x01UL << DMA_CHEN_CH7_ABORT_Pos)               /*!< 0x00000040 00000000 */
#define DMA_CHEN_CH7_ABORT                   DMA_CHEN_CH7_ABORT_Msk                           /*!< Channel-7 Abort Request. */
#define DMA_CHEN_CH8_ABORT_Pos               (39U)
#define DMA_CHEN_CH8_ABORT_Msk               (0x01UL << DMA_CHEN_CH8_ABORT_Pos)               /*!< 0x00000080 00000000 */
#define DMA_CHEN_CH8_ABORT                   DMA_CHEN_CH8_ABORT_Msk                           /*!< Channel-8 Abort Request. */
#define DMA_CHEN_CH1_ABORT_WE_Pos               (40U)
#define DMA_CHEN_CH1_ABORT_WE_Msk               (0x01UL << DMA_CHEN_CH1_ABORT_WE_Pos)               /*!< 0x00000100 00000000 */
#define DMA_CHEN_CH1_ABORT_WE                   DMA_CHEN_CH1_ABORT_WE_Msk                           /*!< This bit is used to write enable the Channel-1 Abort bit. */
#define DMA_CHEN_CH2_ABORT_WE_Pos               (41U)
#define DMA_CHEN_CH2_ABORT_WE_Msk               (0x01UL << DMA_CHEN_CH2_ABORT_WE_Pos)               /*!< 0x00000200 00000000 */
#define DMA_CHEN_CH2_ABORT_WE                   DMA_CHEN_CH2_ABORT_WE_Msk                           /*!< This bit is used to write enable the Channel-2 Abort bit. */
#define DMA_CHEN_CH3_ABORT_WE_Pos               (42U)
#define DMA_CHEN_CH3_ABORT_WE_Msk               (0x01UL << DMA_CHEN_CH3_ABORT_WE_Pos)               /*!< 0x00000400 00000000 */
#define DMA_CHEN_CH3_ABORT_WE                   DMA_CHEN_CH3_ABORT_WE_Msk                           /*!< This bit is used to write enable the Channel-3 Abort bit. */
#define DMA_CHEN_CH4_ABORT_WE_Pos               (43U)
#define DMA_CHEN_CH4_ABORT_WE_Msk               (0x01UL << DMA_CHEN_CH4_ABORT_WE_Pos)               /*!< 0x00000800 00000000 */
#define DMA_CHEN_CH4_ABORT_WE                   DMA_CHEN_CH4_ABORT_WE_Msk                           /*!< This bit is used to write enable the Channel-4 Abort bit. */
#define DMA_CHEN_CH5_ABORT_WE_Pos               (44U)
#define DMA_CHEN_CH5_ABORT_WE_Msk               (0x01UL << DMA_CHEN_CH5_ABORT_WE_Pos)               /*!< 0x00001000 00000000 */
#define DMA_CHEN_CH5_ABORT_WE                   DMA_CHEN_CH5_ABORT_WE_Msk                           /*!< This bit is used to write enable the Channel-5 Abort bit. */
#define DMA_CHEN_CH6_ABORT_WE_Pos               (45U)
#define DMA_CHEN_CH6_ABORT_WE_Msk               (0x01UL << DMA_CHEN_CH6_ABORT_WE_Pos)               /*!< 0x00002000 00000000 */
#define DMA_CHEN_CH6_ABORT_WE                   DMA_CHEN_CH6_ABORT_WE_Msk                           /*!< This bit is used to write enable the Channel-6 Abort bit. */
#define DMA_CHEN_CH7_ABORT_WE_Pos               (46U)
#define DMA_CHEN_CH7_ABORT_WE_Msk               (0x01UL << DMA_CHEN_CH7_ABORT_WE_Pos)               /*!< 0x00004000 00000000 */
#define DMA_CHEN_CH7_ABORT_WE                   DMA_CHEN_CH7_ABORT_WE_Msk                           /*!< This bit is used to write enable the Channel-7 Abort bit. */
#define DMA_CHEN_CH8_ABORT_WE_Pos               (47U)
#define DMA_CHEN_CH8_ABORT_WE_Msk               (0x01UL << DMA_CHEN_CH8_ABORT_WE_Pos)               /*!< 0x00008000 00000000 */
#define DMA_CHEN_CH8_ABORT_WE                   DMA_CHEN_CH8_ABORT_WE_Msk                           /*!< This bit is used to write enable the Channel-8 Abort bit. */

/******************  Bit definition for DMAC_CHENREG2 register  ******************/
#define DMA_CHEN2_CH1_EN_Pos               (0U)
#define DMA_CHEN2_CH1_EN_Msk               (0x01UL << DMA_CHEN2_CH1_EN_Pos)               /*!< 0x00000000 00000001 */
#define DMA_CHEN2_CH1_EN                   DMA_CHEN2_CH1_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-1. */
#define DMA_CHEN2_CH2_EN_Pos               (1U)
#define DMA_CHEN2_CH2_EN_Msk               (0x01UL << DMA_CHEN2_CH2_EN_Pos)               /*!< 0x00000000 00000002 */
#define DMA_CHEN2_CH2_EN                   DMA_CHEN2_CH2_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-2. */
#define DMA_CHEN2_CH3_EN_Pos               (2U)
#define DMA_CHEN2_CH3_EN_Msk               (0x01UL << DMA_CHEN2_CH3_EN_Pos)               /*!< 0x00000000 00000004 */
#define DMA_CHEN2_CH3_EN                   DMA_CHEN2_CH3_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-3. */
#define DMA_CHEN2_CH4_EN_Pos               (3U)
#define DMA_CHEN2_CH4_EN_Msk               (0x01UL << DMA_CHEN2_CH4_EN_Pos)               /*!< 0x00000000 00000008 */
#define DMA_CHEN2_CH4_EN                   DMA_CHEN2_CH4_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-4. */
#define DMA_CHEN2_CH5_EN_Pos               (4U)
#define DMA_CHEN2_CH5_EN_Msk               (0x01UL << DMA_CHEN2_CH5_EN_Pos)               /*!< 0x00000000 00000010 */
#define DMA_CHEN2_CH5_EN                   DMA_CHEN2_CH5_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-5. */
#define DMA_CHEN2_CH6_EN_Pos               (5U)
#define DMA_CHEN2_CH6_EN_Msk               (0x01UL << DMA_CHEN2_CH6_EN_Pos)               /*!< 0x00000000 00000020 */
#define DMA_CHEN2_CH6_EN                   DMA_CHEN2_CH6_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-6. */
#define DMA_CHEN2_CH7_EN_Pos               (6U)
#define DMA_CHEN2_CH7_EN_Msk               (0x01UL << DMA_CHEN2_CH7_EN_Pos)               /*!< 0x00000000 00000040 */
#define DMA_CHEN2_CH7_EN                   DMA_CHEN2_CH7_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-7. */
#define DMA_CHEN2_CH8_EN_Pos               (7U)
#define DMA_CHEN2_CH8_EN_Msk               (0x01UL << DMA_CHEN2_CH8_EN_Pos)               /*!< 0x00000000 00000080 */
#define DMA_CHEN2_CH8_EN                   DMA_CHEN2_CH8_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-8. */
#define DMA_CHEN2_CH9_EN_Pos               (8U)
#define DMA_CHEN2_CH9_EN_Msk               (0x01UL << DMA_CHEN2_CH9_EN_Pos)               /*!< 0x00000000 00000100 */
#define DMA_CHEN2_CH9_EN                   DMA_CHEN2_CH9_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-9. */
#define DMA_CHEN2_CH10_EN_Pos              (9U)
#define DMA_CHEN2_CH10_EN_Msk              (0x01UL << DMA_CHEN2_CH10_EN_Pos)              /*!< 0x00000000 00000200 */
#define DMA_CHEN2_CH10_EN                  DMA_CHEN2_CH10_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-10. */
#define DMA_CHEN2_CH11_EN_Pos              (10U)
#define DMA_CHEN2_CH11_EN_Msk              (0x01UL << DMA_CHEN2_CH11_EN_Pos)              /*!< 0x00000000 00000400 */
#define DMA_CHEN2_CH11_EN                  DMA_CHEN2_CH11_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-11. */
#define DMA_CHEN2_CH12_EN_Pos              (11U)
#define DMA_CHEN2_CH12_EN_Msk              (0x01UL << DMA_CHEN2_CH12_EN_Pos)              /*!< 0x00000000 00000800 */
#define DMA_CHEN2_CH12_EN                  DMA_CHEN2_CH12_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-12. */
#define DMA_CHEN2_CH13_EN_Pos              (12U)
#define DMA_CHEN2_CH13_EN_Msk              (0x01UL << DMA_CHEN2_CH13_EN_Pos)              /*!< 0x00000000 00001000 */
#define DMA_CHEN2_CH13_EN                  DMA_CHEN2_CH13_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-13. */
#define DMA_CHEN2_CH14_EN_Pos              (13U)
#define DMA_CHEN2_CH14_EN_Msk              (0x01UL << DMA_CHEN2_CH14_EN_Pos)              /*!< 0x00000000 00002000 */
#define DMA_CHEN2_CH14_EN                  DMA_CHEN2_CH14_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-14. */
#define DMA_CHEN2_CH15_EN_Pos              (14U)
#define DMA_CHEN2_CH15_EN_Msk              (0x01UL << DMA_CHEN2_CH15_EN_Pos)              /*!< 0x00000000 00004000 */
#define DMA_CHEN2_CH15_EN                  DMA_CHEN2_CH15_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-15. */
#define DMA_CHEN2_CH16_EN_Pos              (15U)
#define DMA_CHEN2_CH16_EN_Msk              (0x01UL << DMA_CHEN2_CH16_EN_Pos)              /*!< 0x00000000 00008000 */
#define DMA_CHEN2_CH16_EN                  DMA_CHEN2_CH16_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-16. */
#define DMA_CHEN2_CH1_EN_WE_Pos                (16U)
#define DMA_CHEN2_CH1_EN_WE_Msk                (0x01UL << DMA_CHEN2_CH1_EN_WE_Pos)               /*!< 0x00000000 00010000 */
#define DMA_CHEN2_CH1_EN_WE                    DMA_CHEN2_CH1_EN_WE_Msk                           /*!< DW_axi_dmac Channel-1 Enable Write Enable bit. */
#define DMA_CHEN2_CH2_EN_WE_Pos                (17U)
#define DMA_CHEN2_CH2_EN_WE_Msk                (0x01UL << DMA_CHEN2_CH2_EN_WE_Pos)               /*!< 0x00000000 00020000 */
#define DMA_CHEN2_CH2_EN_WE                    DMA_CHEN2_CH2_EN_WE_Msk                           /*!< DW_axi_dmac Channel-2 Enable Write Enable bit. */
#define DMA_CHEN2_CH3_EN_WE_Pos                (18U)
#define DMA_CHEN2_CH3_EN_WE_Msk                (0x01UL << DMA_CHEN2_CH3_EN_WE_Pos)               /*!< 0x00000000 00040000 */
#define DMA_CHEN2_CH3_EN_WE                    DMA_CHEN2_CH3_EN_WE_Msk                           /*!< DW_axi_dmac Channel-3 Enable Write Enable bit. */
#define DMA_CHEN2_CH4_EN_WE_Pos                (19U)
#define DMA_CHEN2_CH4_EN_WE_Msk                (0x01UL << DMA_CHEN2_CH4_EN_WE_Pos)               /*!< 0x00000000 00080000 */
#define DMA_CHEN2_CH4_EN_WE                    DMA_CHEN2_CH4_EN_WE_Msk                           /*!< DW_axi_dmac Channel-4 Enable Write Enable bit. */
#define DMA_CHEN2_CH5_EN_WE_Pos                (20U)
#define DMA_CHEN2_CH5_EN_WE_Msk                (0x01UL << DMA_CHEN2_CH5_EN_WE_Pos)               /*!< 0x00000000 00100000 */
#define DMA_CHEN2_CH5_EN_WE                    DMA_CHEN2_CH5_EN_WE_Msk                           /*!< DW_axi_dmac Channel-5 Enable Write Enable bit. */
#define DMA_CHEN2_CH6_EN_WE_Pos                (21U)
#define DMA_CHEN2_CH6_EN_WE_Msk                (0x01UL << DMA_CHEN2_CH6_EN_WE_Pos)               /*!< 0x00000000 00200000 */
#define DMA_CHEN2_CH6_EN_WE                    DMA_CHEN2_CH6_EN_WE_Msk                           /*!< DW_axi_dmac Channel-6 Enable Write Enable bit. */
#define DMA_CHEN2_CH7_EN_WE_Pos                (22U)
#define DMA_CHEN2_CH7_EN_WE_Msk                (0x01UL << DMA_CHEN2_CH7_EN_WE_Pos)               /*!< 0x00000000 00400000 */
#define DMA_CHEN2_CH7_EN_WE                    DMA_CHEN2_CH7_EN_WE_Msk                           /*!< DW_axi_dmac Channel-7 Enable Write Enable bit. */
#define DMA_CHEN2_CH8_EN_WE_Pos                (23U)
#define DMA_CHEN2_CH8_EN_WE_Msk                (0x01UL << DMA_CHEN2_CH8_EN_WE_Pos)               /*!< 0x00000000 00800000 */
#define DMA_CHEN2_CH8_EN_WE                    DMA_CHEN2_CH8_EN_WE_Msk                           /*!< DW_axi_dmac Channel-8 Enable Write Enable bit. */
#define DMA_CHEN2_CH9_EN_WE_Pos                (24U)
#define DMA_CHEN2_CH9_EN_WE_Msk                (0x01UL << DMA_CHEN2_CH9_EN_WE_Pos)               /*!< 0x00000000 01000000 */
#define DMA_CHEN2_CH9_EN_WE                    DMA_CHEN2_CH9_EN_WE_Msk                           /*!< DW_axi_dmac Channel-9 Enable Write Enable bit. */
#define DMA_CHEN2_CH10_EN_WE_Pos               (25U)
#define DMA_CHEN2_CH10_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH10_EN_WE_Pos)              /*!< 0x00000000 02000000 */
#define DMA_CHEN2_CH10_EN_WE                   DMA_CHEN2_CH10_EN_WE_Msk                          /*!< DW_axi_dmac Channel-10 Enable Write Enable bit. */
#define DMA_CHEN2_CH11_EN_WE_Pos               (26U)
#define DMA_CHEN2_CH11_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH11_EN_WE_Pos)              /*!< 0x00000000 04000000 */
#define DMA_CHEN2_CH11_EN_WE                   DMA_CHEN2_CH11_EN_WE_Msk                          /*!< DW_axi_dmac Channel-11 Enable Write Enable bit. */
#define DMA_CHEN2_CH12_EN_WE_Pos               (27U)
#define DMA_CHEN2_CH12_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH12_EN_WE_Pos)              /*!< 0x00000000 08000000 */
#define DMA_CHEN2_CH12_EN_WE                   DMA_CHEN2_CH12_EN_WE_Msk                          /*!< DW_axi_dmac Channel-12 Enable Write Enable bit. */
#define DMA_CHEN2_CH13_EN_WE_Pos               (28U)
#define DMA_CHEN2_CH13_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH13_EN_WE_Pos)              /*!< 0x00000000 10000000 */
#define DMA_CHEN2_CH13_EN_WE                   DMA_CHEN2_CH13_EN_WE_Msk                          /*!< DW_axi_dmac Channel-13 Enable Write Enable bit. */
#define DMA_CHEN2_CH14_EN_WE_Pos               (29U)
#define DMA_CHEN2_CH14_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH14_EN_WE_Pos)              /*!< 0x00000000 20000000 */
#define DMA_CHEN2_CH14_EN_WE                   DMA_CHEN2_CH14_EN_WE_Msk                          /*!< DW_axi_dmac Channel-14 Enable Write Enable bit. */
#define DMA_CHEN2_CH15_EN_WE_Pos               (30U)
#define DMA_CHEN2_CH15_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH15_EN_WE_Pos)              /*!< 0x00000000 40000000 */
#define DMA_CHEN2_CH15_EN_WE                   DMA_CHEN2_CH15_EN_WE_Msk                          /*!< DW_axi_dmac Channel-15 Enable Write Enable bit. */
#define DMA_CHEN2_CH16_EN_WE_Pos               (31U)
#define DMA_CHEN2_CH16_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH16_EN_WE_Pos)              /*!< 0x00000000 80000000 */
#define DMA_CHEN2_CH16_EN_WE                   DMA_CHEN2_CH16_EN_WE_Msk                          /*!< DW_axi_dmac Channel-16 Enable Write Enable bit. */
#define DMA_CHEN2_CH17_EN_Pos              (32U)
#define DMA_CHEN2_CH17_EN_Msk              (0x01UL << DMA_CHEN2_CH17_EN_Pos)               /*!< 0x00000001 00000000 */
#define DMA_CHEN2_CH17_EN                  DMA_CHEN2_CH17_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-17. */
#define DMA_CHEN2_CH18_EN_Pos              (33U)
#define DMA_CHEN2_CH18_EN_Msk              (0x01UL << DMA_CHEN2_CH18_EN_Pos)               /*!< 0x00000002 00000000 */
#define DMA_CHEN2_CH18_EN                  DMA_CHEN2_CH18_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-18. */
#define DMA_CHEN2_CH19_EN_Pos              (34U)
#define DMA_CHEN2_CH19_EN_Msk              (0x01UL << DMA_CHEN2_CH19_EN_Pos)               /*!< 0x00000004 00000000 */
#define DMA_CHEN2_CH19_EN                  DMA_CHEN2_CH19_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-19. */
#define DMA_CHEN2_CH20_EN_Pos              (35U)
#define DMA_CHEN2_CH20_EN_Msk              (0x01UL << DMA_CHEN2_CH20_EN_Pos)               /*!< 0x00000008 00000000 */
#define DMA_CHEN2_CH20_EN                  DMA_CHEN2_CH20_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-20. */
#define DMA_CHEN2_CH21_EN_Pos              (36U)
#define DMA_CHEN2_CH21_EN_Msk              (0x01UL << DMA_CHEN2_CH21_EN_Pos)               /*!< 0x00000010 00000000 */
#define DMA_CHEN2_CH21_EN                  DMA_CHEN2_CH21_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-21. */
#define DMA_CHEN2_CH22_EN_Pos              (37U)
#define DMA_CHEN2_CH22_EN_Msk              (0x01UL << DMA_CHEN2_CH22_EN_Pos)               /*!< 0x00000020 00000000 */
#define DMA_CHEN2_CH22_EN                  DMA_CHEN2_CH22_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-22. */
#define DMA_CHEN2_CH23_EN_Pos              (38U)
#define DMA_CHEN2_CH23_EN_Msk              (0x01UL << DMA_CHEN2_CH23_EN_Pos)               /*!< 0x00000040 00000000 */
#define DMA_CHEN2_CH23_EN                  DMA_CHEN2_CH23_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-28. */
#define DMA_CHEN2_CH24_EN_Pos              (39U)
#define DMA_CHEN2_CH24_EN_Msk              (0x01UL << DMA_CHEN2_CH24_EN_Pos)               /*!< 0x00000080 00000000 */
#define DMA_CHEN2_CH24_EN                  DMA_CHEN2_CH24_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-29. */
#define DMA_CHEN2_CH25_EN_Pos              (40U)
#define DMA_CHEN2_CH25_EN_Msk              (0x01UL << DMA_CHEN2_CH25_EN_Pos)               /*!< 0x00000100 00000000 */
#define DMA_CHEN2_CH25_EN                  DMA_CHEN2_CH25_EN_Msk                           /*!< This bit is used to enable the DW_axi_dmac Channel-30. */
#define DMA_CHEN2_CH26_EN_Pos              (41U)
#define DMA_CHEN2_CH26_EN_Msk              (0x01UL << DMA_CHEN2_CH26_EN_Pos)              /*!< 0x00000200 00000000 */
#define DMA_CHEN2_CH26_EN                  DMA_CHEN2_CH26_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-31. */
#define DMA_CHEN2_CH27_EN_Pos              (42U)
#define DMA_CHEN2_CH27_EN_Msk              (0x01UL << DMA_CHEN2_CH27_EN_Pos)              /*!< 0x00000400 00000000 */
#define DMA_CHEN2_CH27_EN                  DMA_CHEN2_CH27_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-32. */
#define DMA_CHEN2_CH28_EN_Pos              (43U)
#define DMA_CHEN2_CH28_EN_Msk              (0x01UL << DMA_CHEN2_CH28_EN_Pos)              /*!< 0x00000800 00000000 */
#define DMA_CHEN2_CH28_EN                  DMA_CHEN2_CH28_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-33. */
#define DMA_CHEN2_CH29_EN_Pos              (44U)
#define DMA_CHEN2_CH29_EN_Msk              (0x01UL << DMA_CHEN2_CH29_EN_Pos)              /*!< 0x00001000 00000000 */
#define DMA_CHEN2_CH29_EN                  DMA_CHEN2_CH29_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-34. */
#define DMA_CHEN2_CH30_EN_Pos              (45U)
#define DMA_CHEN2_CH30_EN_Msk              (0x01UL << DMA_CHEN2_CH30_EN_Pos)              /*!< 0x00002000 00000000 */
#define DMA_CHEN2_CH30_EN                  DMA_CHEN2_CH30_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-35. */
#define DMA_CHEN2_CH31_EN_Pos              (46U)
#define DMA_CHEN2_CH31_EN_Msk              (0x01UL << DMA_CHEN2_CH31_EN_Pos)              /*!< 0x00004000 00000000 */
#define DMA_CHEN2_CH31_EN                  DMA_CHEN2_CH31_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-36. */
#define DMA_CHEN2_CH32_EN_Pos              (47U)
#define DMA_CHEN2_CH32_EN_Msk              (0x01UL << DMA_CHEN2_CH32_EN_Pos)              /*!< 0x00008000 00000000 */
#define DMA_CHEN2_CH32_EN                  DMA_CHEN2_CH32_EN_Msk                          /*!< This bit is used to enable the DW_axi_dmac Channel-37. */
#define DMA_CHEN2_CH17_EN_WE_Pos               (48U)
#define DMA_CHEN2_CH17_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH17_EN_WE_Pos)               /*!< 0x00010000 00000000 */
#define DMA_CHEN2_CH17_EN_WE                   DMA_CHEN2_CH17_EN_WE_Msk                           /*!< DW_axi_dmac Channel-17 Enable Write Enable bit. */
#define DMA_CHEN2_CH18_EN_WE_Pos               (49U)
#define DMA_CHEN2_CH18_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH18_EN_WE_Pos)               /*!< 0x00020000 00000000 */
#define DMA_CHEN2_CH18_EN_WE                   DMA_CHEN2_CH18_EN_WE_Msk                           /*!< DW_axi_dmac Channel-18 Enable Write Enable bit. */
#define DMA_CHEN2_CH19_EN_WE_Pos               (50U)
#define DMA_CHEN2_CH19_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH19_EN_WE_Pos)               /*!< 0x00040000 00000000 */
#define DMA_CHEN2_CH19_EN_WE                   DMA_CHEN2_CH19_EN_WE_Msk                           /*!< DW_axi_dmac Channel-19 Enable Write Enable bit. */
#define DMA_CHEN2_CH20_EN_WE_Pos               (51U)
#define DMA_CHEN2_CH20_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH20_EN_WE_Pos)               /*!< 0x00080000 00000000 */
#define DMA_CHEN2_CH20_EN_WE                   DMA_CHEN2_CH20_EN_WE_Msk                           /*!< DW_axi_dmac Channel-20 Enable Write Enable bit. */
#define DMA_CHEN2_CH21_EN_WE_Pos               (52U)
#define DMA_CHEN2_CH21_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH21_EN_WE_Pos)               /*!< 0x00100000 00000000 */
#define DMA_CHEN2_CH21_EN_WE                   DMA_CHEN2_CH21_EN_WE_Msk                           /*!< DW_axi_dmac Channel-21 Enable Write Enable bit. */
#define DMA_CHEN2_CH22_EN_WE_Pos               (53U)
#define DMA_CHEN2_CH22_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH22_EN_WE_Pos)               /*!< 0x00200000 00000000 */
#define DMA_CHEN2_CH22_EN_WE                   DMA_CHEN2_CH22_EN_WE_Msk                           /*!< DW_axi_dmac Channel-22 Enable Write Enable bit. */
#define DMA_CHEN2_CH23_EN_WE_Pos               (54U)
#define DMA_CHEN2_CH23_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH23_EN_WE_Pos)               /*!< 0x00400000 00000000 */
#define DMA_CHEN2_CH23_EN_WE                   DMA_CHEN2_CH23_EN_WE_Msk                           /*!< DW_axi_dmac Channel-23 Enable Write Enable bit. */
#define DMA_CHEN2_CH24_EN_WE_Pos               (55U)
#define DMA_CHEN2_CH24_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH24_EN_WE_Pos)               /*!< 0x00800000 00000000 */
#define DMA_CHEN2_CH24_EN_WE                   DMA_CHEN2_CH24_EN_WE_Msk                           /*!< DW_axi_dmac Channel-24 Enable Write Enable bit. */
#define DMA_CHEN2_CH25_EN_WE_Pos               (56U)
#define DMA_CHEN2_CH25_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH25_EN_WE_Pos)               /*!< 0x01000000 00000000 */
#define DMA_CHEN2_CH25_EN_WE                   DMA_CHEN2_CH25_EN_WE_Msk                           /*!< DW_axi_dmac Channel-25 Enable Write Enable bit. */
#define DMA_CHEN2_CH26_EN_WE_Pos               (57U)
#define DMA_CHEN2_CH26_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH26_EN_WE_Pos)              /*!< 0x02000000 00000000 */
#define DMA_CHEN2_CH26_EN_WE                   DMA_CHEN2_CH26_EN_WE_Msk                          /*!< DW_axi_dmac Channel-26 Enable Write Enable bit. */
#define DMA_CHEN2_CH27_EN_WE_Pos               (58U)
#define DMA_CHEN2_CH27_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH27_EN_WE_Pos)              /*!< 0x04000000 00000000 */
#define DMA_CHEN2_CH27_EN_WE                   DMA_CHEN2_CH27_EN_WE_Msk                          /*!< DW_axi_dmac Channel-27 Enable Write Enable bit. */
#define DMA_CHEN2_CH28_EN_WE_Pos               (59U)
#define DMA_CHEN2_CH28_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH28_EN_WE_Pos)              /*!< 0x08000000 00000000 */
#define DMA_CHEN2_CH28_EN_WE                   DMA_CHEN2_CH28_EN_WE_Msk                          /*!< DW_axi_dmac Channel-28 Enable Write Enable bit. */
#define DMA_CHEN2_CH29_EN_WE_Pos               (60U)
#define DMA_CHEN2_CH29_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH29_EN_WE_Pos)              /*!< 0x10000000 00000000 */
#define DMA_CHEN2_CH29_EN_WE                   DMA_CHEN2_CH29_EN_WE_Msk                          /*!< DW_axi_dmac Channel-29 Enable Write Enable bit. */
#define DMA_CHEN2_CH30_EN_WE_Pos               (61U)
#define DMA_CHEN2_CH30_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH30_EN_WE_Pos)              /*!< 0x20000000 00000000 */
#define DMA_CHEN2_CH30_EN_WE                   DMA_CHEN2_CH30_EN_WE_Msk                          /*!< DW_axi_dmac Channel-30 Enable Write Enable bit. */
#define DMA_CHEN2_CH31_EN_WE_Pos               (62U)
#define DMA_CHEN2_CH31_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH31_EN_WE_Pos)              /*!< 0x40000000 00000000 */
#define DMA_CHEN2_CH31_EN_WE                   DMA_CHEN2_CH31_EN_WE_Msk                          /*!< DW_axi_dmac Channel-31 Enable Write Enable bit. */
#define DMA_CHEN2_CH32_EN_WE_Pos               (63U)
#define DMA_CHEN2_CH32_EN_WE_Msk               (0x01UL << DMA_CHEN2_CH32_EN_WE_Pos)              /*!< 0x80000000 00000000 */
#define DMA_CHEN2_CH32_EN_WE                   DMA_CHEN2_CH32_EN_WE_Msk                          /*!< DW_axi_dmac Channel-32 Enable Write Enable bit. */


/******************  Bit definition for DMAC_CHSUSPREG register  ******************/
#define DMA_CHSUSP_CH1_SUSP_Pos             (0U)
#define DMA_CHSUSP_CH1_SUSP_Msk             (0x01UL << DMA_CHSUSP_CH1_SUSP_Pos)               /*!< 0x00000000 00000001 */
#define DMA_CHSUSP_CH1_SUSP                 DMA_CHSUSP_CH1_SUSP_Msk                           /*!< Channel-1 Suspend Request. */
#define DMA_CHSUSP_CH2_SUSP_Pos             (1U)
#define DMA_CHSUSP_CH2_SUSP_Msk             (0x01UL << DMA_CHSUSP_CH2_SUSP_Pos)               /*!< 0x00000000 00000002 */
#define DMA_CHSUSP_CH2_SUSP                 DMA_CHSUSP_CH2_SUSP_Msk                           /*!< Channel-2 Suspend Request. */
#define DMA_CHSUSP_CH3_SUSP_Pos             (2U)
#define DMA_CHSUSP_CH3_SUSP_Msk             (0x01UL << DMA_CHSUSP_CH3_SUSP_Pos)               /*!< 0x00000000 00000004 */
#define DMA_CHSUSP_CH3_SUSP                 DMA_CHSUSP_CH3_SUSP_Msk                           /*!< Channel-3 Suspend Request. */
#define DMA_CHSUSP_CH4_SUSP_Pos             (3U)
#define DMA_CHSUSP_CH4_SUSP_Msk             (0x01UL << DMA_CHSUSP_CH4_SUSP_Pos)               /*!< 0x00000000 00000008 */
#define DMA_CHSUSP_CH4_SUSP                 DMA_CHSUSP_CH4_SUSP_Msk                           /*!< Channel-4 Suspend Request. */
#define DMA_CHSUSP_CH5_SUSP_Pos             (4U)
#define DMA_CHSUSP_CH5_SUSP_Msk             (0x01UL << DMA_CHSUSP_CH5_SUSP_Pos)               /*!< 0x00000000 00000010 */
#define DMA_CHSUSP_CH5_SUSP                 DMA_CHSUSP_CH5_SUSP_Msk                           /*!< Channel-5 Suspend Request. */
#define DMA_CHSUSP_CH6_SUSP_Pos             (5U)
#define DMA_CHSUSP_CH6_SUSP_Msk             (0x01UL << DMA_CHSUSP_CH6_SUSP_Pos)               /*!< 0x00000000 00000020 */
#define DMA_CHSUSP_CH6_SUSP                 DMA_CHSUSP_CH6_SUSP_Msk                           /*!< Channel-6 Suspend Request. */
#define DMA_CHSUSP_CH7_SUSP_Pos             (6U)
#define DMA_CHSUSP_CH7_SUSP_Msk             (0x01UL << DMA_CHSUSP_CH7_SUSP_Pos)               /*!< 0x00000000 00000040 */
#define DMA_CHSUSP_CH7_SUSP                 DMA_CHSUSP_CH7_SUSP_Msk                           /*!< Channel-7 Suspend Request. */
#define DMA_CHSUSP_CH8_SUSP_Pos             (7U)
#define DMA_CHSUSP_CH8_SUSP_Msk             (0x01UL << DMA_CHSUSP_CH8_SUSP_Pos)               /*!< 0x00000000 00000080 */
#define DMA_CHSUSP_CH8_SUSP                 DMA_CHSUSP_CH8_SUSP_Msk                           /*!< Channel-8 Suspend Request. */
#define DMA_CHSUSP_CH9_SUSP_Pos             (8U)
#define DMA_CHSUSP_CH9_SUSP_Msk             (0x01UL << DMA_CHSUSP_CH9_SUSP_Pos)               /*!< 0x00000000 00000100 */
#define DMA_CHSUSP_CH9_SUSP                 DMA_CHSUSP_CH9_SUSP_Msk                           /*!< Channel-9 Suspend Request. */
#define DMA_CHSUSP_CH10_SUSP_Pos            (9U)
#define DMA_CHSUSP_CH10_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH10_SUSP_Pos)              /*!< 0x00000000 00000200 */
#define DMA_CHSUSP_CH10_SUSP                DMA_CHSUSP_CH10_SUSP_Msk                          /*!< Channel-10 Suspend Request. */
#define DMA_CHSUSP_CH11_SUSP_Pos            (10U)
#define DMA_CHSUSP_CH11_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH11_SUSP_Pos)              /*!< 0x00000000 00000400 */
#define DMA_CHSUSP_CH11_SUSP                DMA_CHSUSP_CH11_SUSP_Msk                          /*!< Channel-11 Suspend Request. */
#define DMA_CHSUSP_CH12_SUSP_Pos            (11U)
#define DMA_CHSUSP_CH12_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH12_SUSP_Pos)              /*!< 0x00000000 00000800 */
#define DMA_CHSUSP_CH12_SUSP                DMA_CHSUSP_CH12_SUSP_Msk                          /*!< Channel-12 Suspend Request. */
#define DMA_CHSUSP_CH13_SUSP_Pos            (12U)
#define DMA_CHSUSP_CH13_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH13_SUSP_Pos)              /*!< 0x00000000 00001000 */
#define DMA_CHSUSP_CH13_SUSP                DMA_CHSUSP_CH13_SUSP_Msk                          /*!< Channel-13 Suspend Request. */
#define DMA_CHSUSP_CH14_SUSP_Pos            (13U)
#define DMA_CHSUSP_CH14_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH14_SUSP_Pos)              /*!< 0x00000000 00002000 */
#define DMA_CHSUSP_CH14_SUSP                DMA_CHSUSP_CH14_SUSP_Msk                          /*!< Channel-14 Suspend Request. */
#define DMA_CHSUSP_CH15_SUSP_Pos            (14U)
#define DMA_CHSUSP_CH15_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH15_SUSP_Pos)              /*!< 0x00000000 00004000 */
#define DMA_CHSUSP_CH15_SUSP                DMA_CHSUSP_CH15_SUSP_Msk                          /*!< Channel-15 Suspend Request. */
#define DMA_CHSUSP_CH16_SUSP_Pos            (15U)
#define DMA_CHSUSP_CH16_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH16_SUSP_Pos)              /*!< 0x00000000 00008000 */
#define DMA_CHSUSP_CH16_SUSP                DMA_CHSUSP_CH16_SUSP_Msk                          /*!< Channel-16 Suspend Request. */
#define DMA_CHSUSP_CH1_SUSP_WE_Pos              (16U)
#define DMA_CHSUSP_CH1_SUSP_WE_Msk              (0x01UL << DMA_CHSUSP_CH1_SUSP_WE_Pos)               /*!< 0x00000000 00010000 */
#define DMA_CHSUSP_CH1_SUSP_WE                  DMA_CHSUSP_CH1_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-1 Suspend bit. */
#define DMA_CHSUSP_CH2_SUSP_WE_Pos              (17U)
#define DMA_CHSUSP_CH2_SUSP_WE_Msk              (0x01UL << DMA_CHSUSP_CH2_SUSP_WE_Pos)               /*!< 0x00000000 00020000 */
#define DMA_CHSUSP_CH2_SUSP_WE                  DMA_CHSUSP_CH2_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-2 Suspend bit. */
#define DMA_CHSUSP_CH3_SUSP_WE_Pos              (18U)
#define DMA_CHSUSP_CH3_SUSP_WE_Msk              (0x01UL << DMA_CHSUSP_CH3_SUSP_WE_Pos)               /*!< 0x00000000 00040000 */
#define DMA_CHSUSP_CH3_SUSP_WE                  DMA_CHSUSP_CH3_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-3 Suspend bit. */
#define DMA_CHSUSP_CH4_SUSP_WE_Pos              (19U)
#define DMA_CHSUSP_CH4_SUSP_WE_Msk              (0x01UL << DMA_CHSUSP_CH4_SUSP_WE_Pos)               /*!< 0x00000000 00080000 */
#define DMA_CHSUSP_CH4_SUSP_WE                  DMA_CHSUSP_CH4_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-4 Suspend bit. */
#define DMA_CHSUSP_CH5_SUSP_WE_Pos              (20U)
#define DMA_CHSUSP_CH5_SUSP_WE_Msk              (0x01UL << DMA_CHSUSP_CH5_SUSP_WE_Pos)               /*!< 0x00000000 00100000 */
#define DMA_CHSUSP_CH5_SUSP_WE                  DMA_CHSUSP_CH5_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-5 Suspend bit. */
#define DMA_CHSUSP_CH6_SUSP_WE_Pos              (21U)
#define DMA_CHSUSP_CH6_SUSP_WE_Msk              (0x01UL << DMA_CHSUSP_CH6_SUSP_WE_Pos)               /*!< 0x00000000 00200000 */
#define DMA_CHSUSP_CH6_SUSP_WE                  DMA_CHSUSP_CH6_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-6 Suspend bit. */
#define DMA_CHSUSP_CH7_SUSP_WE_Pos              (22U)
#define DMA_CHSUSP_CH7_SUSP_WE_Msk              (0x01UL << DMA_CHSUSP_CH7_SUSP_WE_Pos)               /*!< 0x00000000 00400000 */
#define DMA_CHSUSP_CH7_SUSP_WE                  DMA_CHSUSP_CH7_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-7 Suspend bit. */
#define DMA_CHSUSP_CH8_SUSP_WE_Pos              (23U)
#define DMA_CHSUSP_CH8_SUSP_WE_Msk              (0x01UL << DMA_CHSUSP_CH8_SUSP_WE_Pos)               /*!< 0x00000000 00800000 */
#define DMA_CHSUSP_CH8_SUSP_WE                  DMA_CHSUSP_CH8_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-8 Suspend bit. */
#define DMA_CHSUSP_CH9_SUSP_WE_Pos              (24U)
#define DMA_CHSUSP_CH9_SUSP_WE_Msk              (0x01UL << DMA_CHSUSP_CH9_SUSP_WE_Pos)               /*!< 0x00000000 01000000 */
#define DMA_CHSUSP_CH9_SUSP_WE                  DMA_CHSUSP_CH9_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-9 Suspend bit. */
#define DMA_CHSUSP_CH10_SUSP_WE_Pos             (25U)
#define DMA_CHSUSP_CH10_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH10_SUSP_WE_Pos)              /*!< 0x00000000 02000000 */
#define DMA_CHSUSP_CH10_SUSP_WE                 DMA_CHSUSP_CH10_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-10 Suspend bit. */
#define DMA_CHSUSP_CH11_SUSP_WE_Pos             (26U)
#define DMA_CHSUSP_CH11_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH11_SUSP_WE_Pos)              /*!< 0x00000000 04000000 */
#define DMA_CHSUSP_CH11_SUSP_WE                 DMA_CHSUSP_CH11_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-11 Suspend bit. */
#define DMA_CHSUSP_CH12_SUSP_WE_Pos             (27U)
#define DMA_CHSUSP_CH12_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH12_SUSP_WE_Pos)              /*!< 0x00000000 08000000 */
#define DMA_CHSUSP_CH12_SUSP_WE                 DMA_CHSUSP_CH12_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-12 Suspend bit. */
#define DMA_CHSUSP_CH13_SUSP_WE_Pos             (28U)
#define DMA_CHSUSP_CH13_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH13_SUSP_WE_Pos)              /*!< 0x00000000 10000000 */
#define DMA_CHSUSP_CH13_SUSP_WE                 DMA_CHSUSP_CH13_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-13 Suspend bit. */
#define DMA_CHSUSP_CH14_SUSP_WE_Pos             (29U)
#define DMA_CHSUSP_CH14_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH14_SUSP_WE_Pos)              /*!< 0x00000000 20000000 */
#define DMA_CHSUSP_CH14_SUSP_WE                 DMA_CHSUSP_CH14_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-14 Suspend bit. */
#define DMA_CHSUSP_CH15_SUSP_WE_Pos             (30U)
#define DMA_CHSUSP_CH15_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH15_SUSP_WE_Pos)              /*!< 0x00000000 40000000 */
#define DMA_CHSUSP_CH15_SUSP_WE                 DMA_CHSUSP_CH15_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-15 Suspend bit. */
#define DMA_CHSUSP_CH16_SUSP_WE_Pos             (31U)
#define DMA_CHSUSP_CH16_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH16_SUSP_WE_Pos)              /*!< 0x00000000 80000000 */
#define DMA_CHSUSP_CH16_SUSP_WE                 DMA_CHSUSP_CH16_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-16 Suspend bit. */
#define DMA_CHSUSP_CH17_SUSP_Pos            (32U)
#define DMA_CHSUSP_CH17_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH17_SUSP_Pos)               /*!< 0x00000001 00000000 */
#define DMA_CHSUSP_CH17_SUSP                DMA_CHSUSP_CH17_SUSP_Msk                           /*!< Channel-17 Suspend Request. */
#define DMA_CHSUSP_CH18_SUSP_Pos            (33U)
#define DMA_CHSUSP_CH18_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH18_SUSP_Pos)               /*!< 0x00000002 00000000 */
#define DMA_CHSUSP_CH18_SUSP                DMA_CHSUSP_CH18_SUSP_Msk                           /*!< Channel-18 Suspend Request. */
#define DMA_CHSUSP_CH19_SUSP_Pos            (34U)
#define DMA_CHSUSP_CH19_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH19_SUSP_Pos)               /*!< 0x00000004 00000000 */
#define DMA_CHSUSP_CH19_SUSP                DMA_CHSUSP_CH19_SUSP_Msk                           /*!< Channel-19 Suspend Request. */
#define DMA_CHSUSP_CH20_SUSP_Pos            (35U)
#define DMA_CHSUSP_CH20_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH20_SUSP_Pos)               /*!< 0x00000008 00000000 */
#define DMA_CHSUSP_CH20_SUSP                DMA_CHSUSP_CH20_SUSP_Msk                           /*!< Channel-20 Suspend Request. */
#define DMA_CHSUSP_CH21_SUSP_Pos            (36U)
#define DMA_CHSUSP_CH21_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH21_SUSP_Pos)               /*!< 0x00000010 00000000 */
#define DMA_CHSUSP_CH21_SUSP                DMA_CHSUSP_CH21_SUSP_Msk                           /*!< Channel-21 Suspend Request. */
#define DMA_CHSUSP_CH22_SUSP_Pos            (37U)
#define DMA_CHSUSP_CH22_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH22_SUSP_Pos)               /*!< 0x00000020 00000000 */
#define DMA_CHSUSP_CH22_SUSP                DMA_CHSUSP_CH22_SUSP_Msk                           /*!< Channel-22 Suspend Request. */
#define DMA_CHSUSP_CH23_SUSP_Pos            (38U)
#define DMA_CHSUSP_CH23_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH23_SUSP_Pos)               /*!< 0x00000040 00000000 */
#define DMA_CHSUSP_CH23_SUSP                DMA_CHSUSP_CH23_SUSP_Msk                           /*!< Channel-28 Suspend Request. */
#define DMA_CHSUSP_CH24_SUSP_Pos            (39U)
#define DMA_CHSUSP_CH24_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH24_SUSP_Pos)               /*!< 0x00000080 00000000 */
#define DMA_CHSUSP_CH24_SUSP                DMA_CHSUSP_CH24_SUSP_Msk                           /*!< Channel-29 Suspend Request. */
#define DMA_CHSUSP_CH25_SUSP_Pos            (40U)
#define DMA_CHSUSP_CH25_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH25_SUSP_Pos)               /*!< 0x00000100 00000000 */
#define DMA_CHSUSP_CH25_SUSP                DMA_CHSUSP_CH25_SUSP_Msk                           /*!< Channel-30 Suspend Request. */
#define DMA_CHSUSP_CH26_SUSP_Pos            (41U)
#define DMA_CHSUSP_CH26_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH26_SUSP_Pos)              /*!< 0x00000200 00000000 */
#define DMA_CHSUSP_CH26_SUSP                DMA_CHSUSP_CH26_SUSP_Msk                          /*!< Channel-31 Suspend Request. */
#define DMA_CHSUSP_CH27_SUSP_Pos            (42U)
#define DMA_CHSUSP_CH27_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH27_SUSP_Pos)              /*!< 0x00000400 00000000 */
#define DMA_CHSUSP_CH27_SUSP                DMA_CHSUSP_CH27_SUSP_Msk                          /*!< Channel-32 Suspend Request. */
#define DMA_CHSUSP_CH28_SUSP_Pos            (43U)
#define DMA_CHSUSP_CH28_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH28_SUSP_Pos)              /*!< 0x00000800 00000000 */
#define DMA_CHSUSP_CH28_SUSP                DMA_CHSUSP_CH28_SUSP_Msk                          /*!< Channel-33 Suspend Request. */
#define DMA_CHSUSP_CH29_SUSP_Pos            (44U)
#define DMA_CHSUSP_CH29_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH29_SUSP_Pos)              /*!< 0x00001000 00000000 */
#define DMA_CHSUSP_CH29_SUSP                DMA_CHSUSP_CH29_SUSP_Msk                          /*!< Channel-34 Suspend Request. */
#define DMA_CHSUSP_CH30_SUSP_Pos            (45U)
#define DMA_CHSUSP_CH30_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH30_SUSP_Pos)              /*!< 0x00002000 00000000 */
#define DMA_CHSUSP_CH30_SUSP                DMA_CHSUSP_CH30_SUSP_Msk                          /*!< Channel-35 Suspend Request. */
#define DMA_CHSUSP_CH31_SUSP_Pos            (46U)
#define DMA_CHSUSP_CH31_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH31_SUSP_Pos)              /*!< 0x00004000 00000000 */
#define DMA_CHSUSP_CH31_SUSP                DMA_CHSUSP_CH31_SUSP_Msk                          /*!< Channel-36 Suspend Request. */
#define DMA_CHSUSP_CH32_SUSP_Pos            (47U)
#define DMA_CHSUSP_CH32_SUSP_Msk            (0x01UL << DMA_CHSUSP_CH32_SUSP_Pos)              /*!< 0x00008000 00000000 */
#define DMA_CHSUSP_CH32_SUSP                DMA_CHSUSP_CH32_SUSP_Msk                          /*!< Channel-37 Suspend Request. */
#define DMA_CHSUSP_CH17_SUSP_WE_Pos             (48U)
#define DMA_CHSUSP_CH17_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH17_SUSP_WE_Pos)               /*!< 0x00010000 00000000 */
#define DMA_CHSUSP_CH17_SUSP_WE                 DMA_CHSUSP_CH17_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-17 Suspend bit. */
#define DMA_CHSUSP_CH18_SUSP_WE_Pos             (49U)
#define DMA_CHSUSP_CH18_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH18_SUSP_WE_Pos)               /*!< 0x00020000 00000000 */
#define DMA_CHSUSP_CH18_SUSP_WE                 DMA_CHSUSP_CH18_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-18 Suspend bit. */
#define DMA_CHSUSP_CH19_SUSP_WE_Pos             (50U)
#define DMA_CHSUSP_CH19_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH19_SUSP_WE_Pos)               /*!< 0x00040000 00000000 */
#define DMA_CHSUSP_CH19_SUSP_WE                 DMA_CHSUSP_CH19_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-19 Suspend bit. */
#define DMA_CHSUSP_CH20_SUSP_WE_Pos             (51U)
#define DMA_CHSUSP_CH20_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH20_SUSP_WE_Pos)               /*!< 0x00080000 00000000 */
#define DMA_CHSUSP_CH20_SUSP_WE                 DMA_CHSUSP_CH20_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-20 Suspend bit. */
#define DMA_CHSUSP_CH21_SUSP_WE_Pos             (52U)
#define DMA_CHSUSP_CH21_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH21_SUSP_WE_Pos)               /*!< 0x00100000 00000000 */
#define DMA_CHSUSP_CH21_SUSP_WE                 DMA_CHSUSP_CH21_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-21 Suspend bit. */
#define DMA_CHSUSP_CH22_SUSP_WE_Pos             (53U)
#define DMA_CHSUSP_CH22_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH22_SUSP_WE_Pos)               /*!< 0x00200000 00000000 */
#define DMA_CHSUSP_CH22_SUSP_WE                 DMA_CHSUSP_CH22_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-22 Suspend bit. */
#define DMA_CHSUSP_CH23_SUSP_WE_Pos             (54U)
#define DMA_CHSUSP_CH23_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH23_SUSP_WE_Pos)               /*!< 0x00400000 00000000 */
#define DMA_CHSUSP_CH23_SUSP_WE                 DMA_CHSUSP_CH23_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-23 Suspend bit. */
#define DMA_CHSUSP_CH24_SUSP_WE_Pos             (55U)
#define DMA_CHSUSP_CH24_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH24_SUSP_WE_Pos)               /*!< 0x00800000 00000000 */
#define DMA_CHSUSP_CH24_SUSP_WE                 DMA_CHSUSP_CH24_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-24 Suspend bit. */
#define DMA_CHSUSP_CH25_SUSP_WE_Pos             (56U)
#define DMA_CHSUSP_CH25_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH25_SUSP_WE_Pos)               /*!< 0x01000000 00000000 */
#define DMA_CHSUSP_CH25_SUSP_WE                 DMA_CHSUSP_CH25_SUSP_WE_Msk                           /*!< This bit is used as a write enable to the Channel-25 Suspend bit. */
#define DMA_CHSUSP_CH26_SUSP_WE_Pos             (57U)
#define DMA_CHSUSP_CH26_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH26_SUSP_WE_Pos)              /*!< 0x02000000 00000000 */
#define DMA_CHSUSP_CH26_SUSP_WE                 DMA_CHSUSP_CH26_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-26 Suspend bit. */
#define DMA_CHSUSP_CH27_SUSP_WE_Pos             (58U)
#define DMA_CHSUSP_CH27_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH27_SUSP_WE_Pos)              /*!< 0x04000000 00000000 */
#define DMA_CHSUSP_CH27_SUSP_WE                 DMA_CHSUSP_CH27_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-27 Suspend bit. */
#define DMA_CHSUSP_CH28_SUSP_WE_Pos             (59U)
#define DMA_CHSUSP_CH28_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH28_SUSP_WE_Pos)              /*!< 0x08000000 00000000 */
#define DMA_CHSUSP_CH28_SUSP_WE                 DMA_CHSUSP_CH28_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-28 Suspend bit. */
#define DMA_CHSUSP_CH29_SUSP_WE_Pos             (60U)
#define DMA_CHSUSP_CH29_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH29_SUSP_WE_Pos)              /*!< 0x10000000 00000000 */
#define DMA_CHSUSP_CH29_SUSP_WE                 DMA_CHSUSP_CH29_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-29 Suspend bit. */
#define DMA_CHSUSP_CH30_SUSP_WE_Pos             (61U)
#define DMA_CHSUSP_CH30_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH30_SUSP_WE_Pos)              /*!< 0x20000000 00000000 */
#define DMA_CHSUSP_CH30_SUSP_WE                 DMA_CHSUSP_CH30_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-30 Suspend bit. */
#define DMA_CHSUSP_CH31_SUSP_WE_Pos             (62U)
#define DMA_CHSUSP_CH31_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH31_SUSP_WE_Pos)              /*!< 0x40000000 00000000 */
#define DMA_CHSUSP_CH31_SUSP_WE                 DMA_CHSUSP_CH31_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-31 Suspend bit. */
#define DMA_CHSUSP_CH32_SUSP_WE_Pos             (63U)
#define DMA_CHSUSP_CH32_SUSP_WE_Msk             (0x01UL << DMA_CHSUSP_CH32_SUSP_WE_Pos)              /*!< 0x80000000 00000000 */
#define DMA_CHSUSP_CH32_SUSP_WE                 DMA_CHSUSP_CH32_SUSP_WE_Msk                          /*!< This bit is used as a write enable to the Channel-32 Suspend bit. */


/******************  Bit definition for DMAC_CHABORTREG register  ******************/
#define DMA_CHABORT_CH1_ABORT_Pos             (0U)
#define DMA_CHABORT_CH1_ABORT_Msk             (0x01UL << DMA_CHABORT_CH1_ABORT_Pos)               /*!< 0x00000000 00000001 */
#define DMA_CHABORT_CH1_ABORT                 DMA_CHABORT_CH1_ABORT_Msk                           /*!< Channel-1 Abort Request. */
#define DMA_CHABORT_CH2_ABORT_Pos             (1U)
#define DMA_CHABORT_CH2_ABORT_Msk             (0x01UL << DMA_CHABORT_CH2_ABORT_Pos)               /*!< 0x00000000 00000002 */
#define DMA_CHABORT_CH2_ABORT                 DMA_CHABORT_CH2_ABORT_Msk                           /*!< Channel-2 Abort Request. */
#define DMA_CHABORT_CH3_ABORT_Pos             (2U)
#define DMA_CHABORT_CH3_ABORT_Msk             (0x01UL << DMA_CHABORT_CH3_ABORT_Pos)               /*!< 0x00000000 00000004 */
#define DMA_CHABORT_CH3_ABORT                 DMA_CHABORT_CH3_ABORT_Msk                           /*!< Channel-3 Abort Request. */
#define DMA_CHABORT_CH4_ABORT_Pos             (3U)
#define DMA_CHABORT_CH4_ABORT_Msk             (0x01UL << DMA_CHABORT_CH4_ABORT_Pos)               /*!< 0x00000000 00000008 */
#define DMA_CHABORT_CH4_ABORT                 DMA_CHABORT_CH4_ABORT_Msk                           /*!< Channel-4 Abort Request. */
#define DMA_CHABORT_CH5_ABORT_Pos             (4U)
#define DMA_CHABORT_CH5_ABORT_Msk             (0x01UL << DMA_CHABORT_CH5_ABORT_Pos)               /*!< 0x00000000 00000010 */
#define DMA_CHABORT_CH5_ABORT                 DMA_CHABORT_CH5_ABORT_Msk                           /*!< Channel-5 Abort Request. */
#define DMA_CHABORT_CH6_ABORT_Pos             (5U)
#define DMA_CHABORT_CH6_ABORT_Msk             (0x01UL << DMA_CHABORT_CH6_ABORT_Pos)               /*!< 0x00000000 00000020 */
#define DMA_CHABORT_CH6_ABORT                 DMA_CHABORT_CH6_ABORT_Msk                           /*!< Channel-6 Abort Request. */
#define DMA_CHABORT_CH7_ABORT_Pos             (6U)
#define DMA_CHABORT_CH7_ABORT_Msk             (0x01UL << DMA_CHABORT_CH7_ABORT_Pos)               /*!< 0x00000000 00000040 */
#define DMA_CHABORT_CH7_ABORT                 DMA_CHABORT_CH7_ABORT_Msk                           /*!< Channel-7 Abort Request. */
#define DMA_CHABORT_CH8_ABORT_Pos             (7U)
#define DMA_CHABORT_CH8_ABORT_Msk             (0x01UL << DMA_CHABORT_CH8_ABORT_Pos)               /*!< 0x00000000 00000080 */
#define DMA_CHABORT_CH8_ABORT                 DMA_CHABORT_CH8_ABORT_Msk                           /*!< Channel-8 Abort Request. */
#define DMA_CHABORT_CH9_ABORT_Pos             (8U)
#define DMA_CHABORT_CH9_ABORT_Msk             (0x01UL << DMA_CHABORT_CH9_ABORT_Pos)               /*!< 0x00000000 00000100 */
#define DMA_CHABORT_CH9_ABORT                 DMA_CHABORT_CH9_ABORT_Msk                           /*!< Channel-9 Abort Request. */
#define DMA_CHABORT_CH10_ABORT_Pos            (9U)
#define DMA_CHABORT_CH10_ABORT_Msk            (0x01UL << DMA_CHABORT_CH10_ABORT_Pos)              /*!< 0x00000000 00000200 */
#define DMA_CHABORT_CH10_ABORT                DMA_CHABORT_CH10_ABORT_Msk                          /*!< Channel-10 Abort Request. */
#define DMA_CHABORT_CH11_ABORT_Pos            (10U)
#define DMA_CHABORT_CH11_ABORT_Msk            (0x01UL << DMA_CHABORT_CH11_ABORT_Pos)              /*!< 0x00000000 00000400 */
#define DMA_CHABORT_CH11_ABORT                DMA_CHABORT_CH11_ABORT_Msk                          /*!< Channel-11 Abort Request. */
#define DMA_CHABORT_CH12_ABORT_Pos            (11U)
#define DMA_CHABORT_CH12_ABORT_Msk            (0x01UL << DMA_CHABORT_CH12_ABORT_Pos)              /*!< 0x00000000 00000800 */
#define DMA_CHABORT_CH12_ABORT                DMA_CHABORT_CH12_ABORT_Msk                          /*!< Channel-12 Abort Request. */
#define DMA_CHABORT_CH13_ABORT_Pos            (12U)
#define DMA_CHABORT_CH13_ABORT_Msk            (0x01UL << DMA_CHABORT_CH13_ABORT_Pos)              /*!< 0x00000000 00001000 */
#define DMA_CHABORT_CH13_ABORT                DMA_CHABORT_CH13_ABORT_Msk                          /*!< Channel-13 Abort Request. */
#define DMA_CHABORT_CH14_ABORT_Pos            (13U)
#define DMA_CHABORT_CH14_ABORT_Msk            (0x01UL << DMA_CHABORT_CH14_ABORT_Pos)              /*!< 0x00000000 00002000 */
#define DMA_CHABORT_CH14_ABORT                DMA_CHABORT_CH14_ABORT_Msk                          /*!< Channel-14 Abort Request. */
#define DMA_CHABORT_CH15_ABORT_Pos            (14U)
#define DMA_CHABORT_CH15_ABORT_Msk            (0x01UL << DMA_CHABORT_CH15_ABORT_Pos)              /*!< 0x00000000 00004000 */
#define DMA_CHABORT_CH15_ABORT                DMA_CHABORT_CH15_ABORT_Msk                          /*!< Channel-15 Abort Request. */
#define DMA_CHABORT_CH16_ABORT_Pos            (15U)
#define DMA_CHABORT_CH16_ABORT_Msk            (0x01UL << DMA_CHABORT_CH16_ABORT_Pos)              /*!< 0x00000000 00008000 */
#define DMA_CHABORT_CH16_ABORT                DMA_CHABORT_CH16_ABORT_Msk                          /*!< Channel-16 Abort Request. */
#define DMA_CHABORT_CH1_ABORT_WE_Pos              (16U)
#define DMA_CHABORT_CH1_ABORT_WE_Msk              (0x01UL << DMA_CHABORT_CH1_ABORT_WE_Pos)               /*!< 0x00000000 00010000 */
#define DMA_CHABORT_CH1_ABORT_WE                  DMA_CHABORT_CH1_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-1 Abort bit. */
#define DMA_CHABORT_CH2_ABORT_WE_Pos              (17U)
#define DMA_CHABORT_CH2_ABORT_WE_Msk              (0x01UL << DMA_CHABORT_CH2_ABORT_WE_Pos)               /*!< 0x00000000 00020000 */
#define DMA_CHABORT_CH2_ABORT_WE                  DMA_CHABORT_CH2_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-2 Abort bit. */
#define DMA_CHABORT_CH3_ABORT_WE_Pos              (18U)
#define DMA_CHABORT_CH3_ABORT_WE_Msk              (0x01UL << DMA_CHABORT_CH3_ABORT_WE_Pos)               /*!< 0x00000000 00040000 */
#define DMA_CHABORT_CH3_ABORT_WE                  DMA_CHABORT_CH3_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-3 Abort bit. */
#define DMA_CHABORT_CH4_ABORT_WE_Pos              (19U)
#define DMA_CHABORT_CH4_ABORT_WE_Msk              (0x01UL << DMA_CHABORT_CH4_ABORT_WE_Pos)               /*!< 0x00000000 00080000 */
#define DMA_CHABORT_CH4_ABORT_WE                  DMA_CHABORT_CH4_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-4 Abort bit. */
#define DMA_CHABORT_CH5_ABORT_WE_Pos              (20U)
#define DMA_CHABORT_CH5_ABORT_WE_Msk              (0x01UL << DMA_CHABORT_CH5_ABORT_WE_Pos)               /*!< 0x00000000 00100000 */
#define DMA_CHABORT_CH5_ABORT_WE                  DMA_CHABORT_CH5_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-5 Abort bit. */
#define DMA_CHABORT_CH6_ABORT_WE_Pos              (21U)
#define DMA_CHABORT_CH6_ABORT_WE_Msk              (0x01UL << DMA_CHABORT_CH6_ABORT_WE_Pos)               /*!< 0x00000000 00200000 */
#define DMA_CHABORT_CH6_ABORT_WE                  DMA_CHABORT_CH6_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-6 Abort bit. */
#define DMA_CHABORT_CH7_ABORT_WE_Pos              (22U)
#define DMA_CHABORT_CH7_ABORT_WE_Msk              (0x01UL << DMA_CHABORT_CH7_ABORT_WE_Pos)               /*!< 0x00000000 00400000 */
#define DMA_CHABORT_CH7_ABORT_WE                  DMA_CHABORT_CH7_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-7 Abort bit. */
#define DMA_CHABORT_CH8_ABORT_WE_Pos              (23U)
#define DMA_CHABORT_CH8_ABORT_WE_Msk              (0x01UL << DMA_CHABORT_CH8_ABORT_WE_Pos)               /*!< 0x00000000 00800000 */
#define DMA_CHABORT_CH8_ABORT_WE                  DMA_CHABORT_CH8_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-8 Abort bit. */
#define DMA_CHABORT_CH9_ABORT_WE_Pos              (24U)
#define DMA_CHABORT_CH9_ABORT_WE_Msk              (0x01UL << DMA_CHABORT_CH9_ABORT_WE_Pos)               /*!< 0x00000000 01000000 */
#define DMA_CHABORT_CH9_ABORT_WE                  DMA_CHABORT_CH9_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-9 Abort bit. */
#define DMA_CHABORT_CH10_ABORT_WE_Pos             (25U)
#define DMA_CHABORT_CH10_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH10_ABORT_WE_Pos)              /*!< 0x00000000 02000000 */
#define DMA_CHABORT_CH10_ABORT_WE                 DMA_CHABORT_CH10_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-10 Abort bit. */
#define DMA_CHABORT_CH11_ABORT_WE_Pos             (26U)
#define DMA_CHABORT_CH11_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH11_ABORT_WE_Pos)              /*!< 0x00000000 04000000 */
#define DMA_CHABORT_CH11_ABORT_WE                 DMA_CHABORT_CH11_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-11 Abort bit. */
#define DMA_CHABORT_CH12_ABORT_WE_Pos             (27U)
#define DMA_CHABORT_CH12_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH12_ABORT_WE_Pos)              /*!< 0x00000000 08000000 */
#define DMA_CHABORT_CH12_ABORT_WE                 DMA_CHABORT_CH12_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-12 Abort bit. */
#define DMA_CHABORT_CH13_ABORT_WE_Pos             (28U)
#define DMA_CHABORT_CH13_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH13_ABORT_WE_Pos)              /*!< 0x00000000 10000000 */
#define DMA_CHABORT_CH13_ABORT_WE                 DMA_CHABORT_CH13_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-13 Abort bit. */
#define DMA_CHABORT_CH14_ABORT_WE_Pos             (29U)
#define DMA_CHABORT_CH14_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH14_ABORT_WE_Pos)              /*!< 0x00000000 20000000 */
#define DMA_CHABORT_CH14_ABORT_WE                 DMA_CHABORT_CH14_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-14 Abort bit. */
#define DMA_CHABORT_CH15_ABORT_WE_Pos             (30U)
#define DMA_CHABORT_CH15_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH15_ABORT_WE_Pos)              /*!< 0x00000000 40000000 */
#define DMA_CHABORT_CH15_ABORT_WE                 DMA_CHABORT_CH15_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-15 Abort bit. */
#define DMA_CHABORT_CH16_ABORT_WE_Pos             (31U)
#define DMA_CHABORT_CH16_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH16_ABORT_WE_Pos)              /*!< 0x00000000 80000000 */
#define DMA_CHABORT_CH16_ABORT_WE                 DMA_CHABORT_CH16_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-16 Abort bit. */
#define DMA_CHABORT_CH17_ABORT_Pos            (32U)
#define DMA_CHABORT_CH17_ABORT_Msk            (0x01UL << DMA_CHABORT_CH17_ABORT_Pos)               /*!< 0x00000001 00000000 */
#define DMA_CHABORT_CH17_ABORT                DMA_CHABORT_CH17_ABORT_Msk                           /*!< Channel-17 Abort Request. */
#define DMA_CHABORT_CH18_ABORT_Pos            (33U)
#define DMA_CHABORT_CH18_ABORT_Msk            (0x01UL << DMA_CHABORT_CH18_ABORT_Pos)               /*!< 0x00000002 00000000 */
#define DMA_CHABORT_CH18_ABORT                DMA_CHABORT_CH18_ABORT_Msk                           /*!< Channel-18 Abort Request. */
#define DMA_CHABORT_CH19_ABORT_Pos            (34U)
#define DMA_CHABORT_CH19_ABORT_Msk            (0x01UL << DMA_CHABORT_CH19_ABORT_Pos)               /*!< 0x00000004 00000000 */
#define DMA_CHABORT_CH19_ABORT                DMA_CHABORT_CH19_ABORT_Msk                           /*!< Channel-19 Abort Request. */
#define DMA_CHABORT_CH20_ABORT_Pos            (35U)
#define DMA_CHABORT_CH20_ABORT_Msk            (0x01UL << DMA_CHABORT_CH20_ABORT_Pos)               /*!< 0x00000008 00000000 */
#define DMA_CHABORT_CH20_ABORT                DMA_CHABORT_CH20_ABORT_Msk                           /*!< Channel-20 Abort Request. */
#define DMA_CHABORT_CH21_ABORT_Pos            (36U)
#define DMA_CHABORT_CH21_ABORT_Msk            (0x01UL << DMA_CHABORT_CH21_ABORT_Pos)               /*!< 0x00000010 00000000 */
#define DMA_CHABORT_CH21_ABORT                DMA_CHABORT_CH21_ABORT_Msk                           /*!< Channel-21 Abort Request. */
#define DMA_CHABORT_CH22_ABORT_Pos            (37U)
#define DMA_CHABORT_CH22_ABORT_Msk            (0x01UL << DMA_CHABORT_CH22_ABORT_Pos)               /*!< 0x00000020 00000000 */
#define DMA_CHABORT_CH22_ABORT                DMA_CHABORT_CH22_ABORT_Msk                           /*!< Channel-22 Abort Request. */
#define DMA_CHABORT_CH23_ABORT_Pos            (38U)
#define DMA_CHABORT_CH23_ABORT_Msk            (0x01UL << DMA_CHABORT_CH23_ABORT_Pos)               /*!< 0x00000040 00000000 */
#define DMA_CHABORT_CH23_ABORT                DMA_CHABORT_CH23_ABORT_Msk                           /*!< Channel-28 Abort Request. */
#define DMA_CHABORT_CH24_ABORT_Pos            (39U)
#define DMA_CHABORT_CH24_ABORT_Msk            (0x01UL << DMA_CHABORT_CH24_ABORT_Pos)               /*!< 0x00000080 00000000 */
#define DMA_CHABORT_CH24_ABORT                DMA_CHABORT_CH24_ABORT_Msk                           /*!< Channel-29 Abort Request. */
#define DMA_CHABORT_CH25_ABORT_Pos            (40U)
#define DMA_CHABORT_CH25_ABORT_Msk            (0x01UL << DMA_CHABORT_CH25_ABORT_Pos)               /*!< 0x00000100 00000000 */
#define DMA_CHABORT_CH25_ABORT                DMA_CHABORT_CH25_ABORT_Msk                           /*!< Channel-30 Abort Request. */
#define DMA_CHABORT_CH26_ABORT_Pos            (41U)
#define DMA_CHABORT_CH26_ABORT_Msk            (0x01UL << DMA_CHABORT_CH26_ABORT_Pos)              /*!< 0x00000200 00000000 */
#define DMA_CHABORT_CH26_ABORT                DMA_CHABORT_CH26_ABORT_Msk                          /*!< Channel-31 Abort Request. */
#define DMA_CHABORT_CH27_ABORT_Pos            (42U)
#define DMA_CHABORT_CH27_ABORT_Msk            (0x01UL << DMA_CHABORT_CH27_ABORT_Pos)              /*!< 0x00000400 00000000 */
#define DMA_CHABORT_CH27_ABORT                DMA_CHABORT_CH27_ABORT_Msk                          /*!< Channel-32 Abort Request. */
#define DMA_CHABORT_CH28_ABORT_Pos            (43U)
#define DMA_CHABORT_CH28_ABORT_Msk            (0x01UL << DMA_CHABORT_CH28_ABORT_Pos)              /*!< 0x00000800 00000000 */
#define DMA_CHABORT_CH28_ABORT                DMA_CHABORT_CH28_ABORT_Msk                          /*!< Channel-33 Abort Request. */
#define DMA_CHABORT_CH29_ABORT_Pos            (44U)
#define DMA_CHABORT_CH29_ABORT_Msk            (0x01UL << DMA_CHABORT_CH29_ABORT_Pos)              /*!< 0x00001000 00000000 */
#define DMA_CHABORT_CH29_ABORT                DMA_CHABORT_CH29_ABORT_Msk                          /*!< Channel-34 Abort Request. */
#define DMA_CHABORT_CH30_ABORT_Pos            (45U)
#define DMA_CHABORT_CH30_ABORT_Msk            (0x01UL << DMA_CHABORT_CH30_ABORT_Pos)              /*!< 0x00002000 00000000 */
#define DMA_CHABORT_CH30_ABORT                DMA_CHABORT_CH30_ABORT_Msk                          /*!< Channel-35 Abort Request. */
#define DMA_CHABORT_CH31_ABORT_Pos            (46U)
#define DMA_CHABORT_CH31_ABORT_Msk            (0x01UL << DMA_CHABORT_CH31_ABORT_Pos)              /*!< 0x00004000 00000000 */
#define DMA_CHABORT_CH31_ABORT                DMA_CHABORT_CH31_ABORT_Msk                          /*!< Channel-36 Abort Request. */
#define DMA_CHABORT_CH32_ABORT_Pos            (47U)
#define DMA_CHABORT_CH32_ABORT_Msk            (0x01UL << DMA_CHABORT_CH32_ABORT_Pos)              /*!< 0x00008000 00000000 */
#define DMA_CHABORT_CH32_ABORT                DMA_CHABORT_CH32_ABORT_Msk                          /*!< Channel-37 Abort Request. */
#define DMA_CHABORT_CH17_ABORT_WE_Pos             (48U)
#define DMA_CHABORT_CH17_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH17_ABORT_WE_Pos)               /*!< 0x00010000 00000000 */
#define DMA_CHABORT_CH17_ABORT_WE                 DMA_CHABORT_CH17_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-17 Abort bit. */
#define DMA_CHABORT_CH18_ABORT_WE_Pos             (49U)
#define DMA_CHABORT_CH18_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH18_ABORT_WE_Pos)               /*!< 0x00020000 00000000 */
#define DMA_CHABORT_CH18_ABORT_WE                 DMA_CHABORT_CH18_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-18 Abort bit. */
#define DMA_CHABORT_CH19_ABORT_WE_Pos             (50U)
#define DMA_CHABORT_CH19_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH19_ABORT_WE_Pos)               /*!< 0x00040000 00000000 */
#define DMA_CHABORT_CH19_ABORT_WE                 DMA_CHABORT_CH19_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-19 Abort bit. */
#define DMA_CHABORT_CH20_ABORT_WE_Pos             (51U)
#define DMA_CHABORT_CH20_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH20_ABORT_WE_Pos)               /*!< 0x00080000 00000000 */
#define DMA_CHABORT_CH20_ABORT_WE                 DMA_CHABORT_CH20_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-20 Abort bit. */
#define DMA_CHABORT_CH21_ABORT_WE_Pos             (52U)
#define DMA_CHABORT_CH21_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH21_ABORT_WE_Pos)               /*!< 0x00100000 00000000 */
#define DMA_CHABORT_CH21_ABORT_WE                 DMA_CHABORT_CH21_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-21 Abort bit. */
#define DMA_CHABORT_CH22_ABORT_WE_Pos             (53U)
#define DMA_CHABORT_CH22_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH22_ABORT_WE_Pos)               /*!< 0x00200000 00000000 */
#define DMA_CHABORT_CH22_ABORT_WE                 DMA_CHABORT_CH22_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-22 Abort bit. */
#define DMA_CHABORT_CH23_ABORT_WE_Pos             (54U)
#define DMA_CHABORT_CH23_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH23_ABORT_WE_Pos)               /*!< 0x00400000 00000000 */
#define DMA_CHABORT_CH23_ABORT_WE                 DMA_CHABORT_CH23_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-23 Abort bit. */
#define DMA_CHABORT_CH24_ABORT_WE_Pos             (55U)
#define DMA_CHABORT_CH24_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH24_ABORT_WE_Pos)               /*!< 0x00800000 00000000 */
#define DMA_CHABORT_CH24_ABORT_WE                 DMA_CHABORT_CH24_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-24 Abort bit. */
#define DMA_CHABORT_CH25_ABORT_WE_Pos             (56U)
#define DMA_CHABORT_CH25_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH25_ABORT_WE_Pos)               /*!< 0x01000000 00000000 */
#define DMA_CHABORT_CH25_ABORT_WE                 DMA_CHABORT_CH25_ABORT_WE_Msk                           /*!< This bit is used as a write enable to the Channel-25 Abort bit. */
#define DMA_CHABORT_CH26_ABORT_WE_Pos             (57U)
#define DMA_CHABORT_CH26_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH26_ABORT_WE_Pos)              /*!< 0x02000000 00000000 */
#define DMA_CHABORT_CH26_ABORT_WE                 DMA_CHABORT_CH26_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-26 Abort bit. */
#define DMA_CHABORT_CH27_ABORT_WE_Pos             (58U)
#define DMA_CHABORT_CH27_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH27_ABORT_WE_Pos)              /*!< 0x04000000 00000000 */
#define DMA_CHABORT_CH27_ABORT_WE                 DMA_CHABORT_CH27_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-27 Abort bit. */
#define DMA_CHABORT_CH28_ABORT_WE_Pos             (59U)
#define DMA_CHABORT_CH28_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH28_ABORT_WE_Pos)              /*!< 0x08000000 00000000 */
#define DMA_CHABORT_CH28_ABORT_WE                 DMA_CHABORT_CH28_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-28 Abort bit. */
#define DMA_CHABORT_CH29_ABORT_WE_Pos             (60U)
#define DMA_CHABORT_CH29_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH29_ABORT_WE_Pos)              /*!< 0x10000000 00000000 */
#define DMA_CHABORT_CH29_ABORT_WE                 DMA_CHABORT_CH29_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-29 Abort bit. */
#define DMA_CHABORT_CH30_ABORT_WE_Pos             (61U)
#define DMA_CHABORT_CH30_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH30_ABORT_WE_Pos)              /*!< 0x20000000 00000000 */
#define DMA_CHABORT_CH30_ABORT_WE                 DMA_CHABORT_CH30_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-30 Abort bit. */
#define DMA_CHABORT_CH31_ABORT_WE_Pos             (62U)
#define DMA_CHABORT_CH31_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH31_ABORT_WE_Pos)              /*!< 0x40000000 00000000 */
#define DMA_CHABORT_CH31_ABORT_WE                 DMA_CHABORT_CH31_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-31 Abort bit. */
#define DMA_CHABORT_CH32_ABORT_WE_Pos             (63U)
#define DMA_CHABORT_CH32_ABORT_WE_Msk             (0x01UL << DMA_CHABORT_CH32_ABORT_WE_Pos)              /*!< 0x80000000 00000000 */
#define DMA_CHABORT_CH32_ABORT_WE                 DMA_CHABORT_CH32_ABORT_WE_Msk                          /*!< This bit is used as a write enable to the Channel-32 Abort bit. */


/******************  Bit definition for DMAC_INTSTATUSREG register  ******************/
#define DMA_INTSTATUS_CH1_IntStat_Pos                      (0U)
#define DMA_INTSTATUS_CH1_IntStat_Msk                      (0x01UL << DMA_INTSTATUS_CH1_IntStat_Pos)                    /*!< 0x00000000 00000001 */
#define DMA_INTSTATUS_CH1_IntStat                          DMA_INTSTATUS_CH1_IntStat_Msk                                /*!< Channel-1 Interrupt Status Bit. */
#define DMA_INTSTATUS_CH2_IntStat_Pos                      (1U)
#define DMA_INTSTATUS_CH2_IntStat_Msk                      (0x01UL << DMA_INTSTATUS_CH2_IntStat_Pos)                    /*!< 0x00000000 00000002 */
#define DMA_INTSTATUS_CH2_IntStat                          DMA_INTSTATUS_CH2_IntStat_Msk                                /*!< Channel-2 Interrupt Status Bit. */
#define DMA_INTSTATUS_CH3_IntStat_Pos                      (2U)
#define DMA_INTSTATUS_CH3_IntStat_Msk                      (0x01UL << DMA_INTSTATUS_CH3_IntStat_Pos)                    /*!< 0x00000000 00000004 */
#define DMA_INTSTATUS_CH3_IntStat                          DMA_INTSTATUS_CH3_IntStat_Msk                                /*!< Channel-3 Interrupt Status Bit. */
#define DMA_INTSTATUS_CH4_IntStat_Pos                      (3U)
#define DMA_INTSTATUS_CH4_IntStat_Msk                      (0x01UL << DMA_INTSTATUS_CH4_IntStat_Pos)                    /*!< 0x00000000 00000008 */
#define DMA_INTSTATUS_CH4_IntStat                          DMA_INTSTATUS_CH4_IntStat_Msk                                /*!< Channel-4 Interrupt Status Bit. */
#define DMA_INTSTATUS_CH5_IntStat_Pos                      (4U)
#define DMA_INTSTATUS_CH5_IntStat_Msk                      (0x01UL << DMA_INTSTATUS_CH5_IntStat_Pos)                    /*!< 0x00000000 00000010 */
#define DMA_INTSTATUS_CH5_IntStat                          DMA_INTSTATUS_CH5_IntStat_Msk                                /*!< Channel-5 Interrupt Status Bit. */
#define DMA_INTSTATUS_CH6_IntStat_Pos                      (5U)
#define DMA_INTSTATUS_CH6_IntStat_Msk                      (0x01UL << DMA_INTSTATUS_CH6_IntStat_Pos)                    /*!< 0x00000000 00000020 */
#define DMA_INTSTATUS_CH6_IntStat                          DMA_INTSTATUS_CH6_IntStat_Msk                                /*!< Channel-6 Interrupt Status Bit. */
#define DMA_INTSTATUS_CH7_IntStat_Pos                      (6U)
#define DMA_INTSTATUS_CH7_IntStat_Msk                      (0x01UL << DMA_INTSTATUS_CH7_IntStat_Pos)                    /*!< 0x00000000 00000040 */
#define DMA_INTSTATUS_CH7_IntStat                          DMA_INTSTATUS_CH7_IntStat_Msk                                /*!< Channel-7 Interrupt Status Bit. */
#define DMA_INTSTATUS_CH8_IntStat_Pos                      (7U)
#define DMA_INTSTATUS_CH8_IntStat_Msk                      (0x01UL << DMA_INTSTATUS_CH8_IntStat_Pos)                    /*!< 0x00000000 00000080 */
#define DMA_INTSTATUS_CH8_IntStat                          DMA_INTSTATUS_CH8_IntStat_Msk                                /*!< Channel-8 Interrupt Status Bit. */
#define DMA_INTSTATUS_CommonReg_IntStat_Pos                (16U)
#define DMA_INTSTATUS_CommonReg_IntStat_Msk                (0x01UL << DMA_INTSTATUS_CommonReg_IntStat_Pos)              /*!< 0x00000000 00010000 */
#define DMA_INTSTATUS_CommonReg_IntStat                    DMA_INTSTATUS_CommonReg_IntStat_Msk                          /*!< Common Register Interrupt Status Bit. */


/******************  Bit definition for DMAC_INTSTATUSREG2 register  ******************/
#define DMA_INTSTATUS2_CH1_IntStat_Pos             (0U)
#define DMA_INTSTATUS2_CH1_IntStat_Msk             (0x01UL << DMA_INTSTATUS2_CH1_IntStat_Pos)               /*!< 0x00000000 00000001 */
#define DMA_INTSTATUS2_CH1_IntStat                 DMA_INTSTATUS2_CH1_IntStat_Msk                           /*!< Channel-1 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH2_IntStat_Pos             (1U)
#define DMA_INTSTATUS2_CH2_IntStat_Msk             (0x01UL << DMA_INTSTATUS2_CH2_IntStat_Pos)               /*!< 0x00000000 00000002 */
#define DMA_INTSTATUS2_CH2_IntStat                 DMA_INTSTATUS2_CH2_IntStat_Msk                           /*!< Channel-2 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH3_IntStat_Pos             (2U)
#define DMA_INTSTATUS2_CH3_IntStat_Msk             (0x01UL << DMA_INTSTATUS2_CH3_IntStat_Pos)               /*!< 0x00000000 00000004 */
#define DMA_INTSTATUS2_CH3_IntStat                 DMA_INTSTATUS2_CH3_IntStat_Msk                           /*!< Channel-3 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH4_IntStat_Pos             (3U)
#define DMA_INTSTATUS2_CH4_IntStat_Msk             (0x01UL << DMA_INTSTATUS2_CH4_IntStat_Pos)               /*!< 0x00000000 00000008 */
#define DMA_INTSTATUS2_CH4_IntStat                 DMA_INTSTATUS2_CH4_IntStat_Msk                           /*!< Channel-4 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH5_IntStat_Pos             (4U)
#define DMA_INTSTATUS2_CH5_IntStat_Msk             (0x01UL << DMA_INTSTATUS2_CH5_IntStat_Pos)               /*!< 0x00000000 00000010 */
#define DMA_INTSTATUS2_CH5_IntStat                 DMA_INTSTATUS2_CH5_IntStat_Msk                           /*!< Channel-5 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH6_IntStat_Pos             (5U)
#define DMA_INTSTATUS2_CH6_IntStat_Msk             (0x01UL << DMA_INTSTATUS2_CH6_IntStat_Pos)               /*!< 0x00000000 00000020 */
#define DMA_INTSTATUS2_CH6_IntStat                 DMA_INTSTATUS2_CH6_IntStat_Msk                           /*!< Channel-6 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH7_IntStat_Pos             (6U)
#define DMA_INTSTATUS2_CH7_IntStat_Msk             (0x01UL << DMA_INTSTATUS2_CH7_IntStat_Pos)               /*!< 0x00000000 00000040 */
#define DMA_INTSTATUS2_CH7_IntStat                 DMA_INTSTATUS2_CH7_IntStat_Msk                           /*!< Channel-7 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH8_IntStat_Pos             (7U)
#define DMA_INTSTATUS2_CH8_IntStat_Msk             (0x01UL << DMA_INTSTATUS2_CH8_IntStat_Pos)               /*!< 0x00000000 00000080 */
#define DMA_INTSTATUS2_CH8_IntStat                 DMA_INTSTATUS2_CH8_IntStat_Msk                           /*!< Channel-8 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH9_IntStat_Pos             (8U)
#define DMA_INTSTATUS2_CH9_IntStat_Msk             (0x01UL << DMA_INTSTATUS2_CH9_IntStat_Pos)               /*!< 0x00000000 00000100 */
#define DMA_INTSTATUS2_CH9_IntStat                 DMA_INTSTATUS2_CH9_IntStat_Msk                           /*!< Channel-9 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH10_IntStat_Pos            (9U)
#define DMA_INTSTATUS2_CH10_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH10_IntStat_Pos)              /*!< 0x00000000 00000200 */
#define DMA_INTSTATUS2_CH10_IntStat                DMA_INTSTATUS2_CH10_IntStat_Msk                          /*!< Channel-10 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH11_IntStat_Pos            (10U)
#define DMA_INTSTATUS2_CH11_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH11_IntStat_Pos)              /*!< 0x00000000 00000400 */
#define DMA_INTSTATUS2_CH11_IntStat                DMA_INTSTATUS2_CH11_IntStat_Msk                          /*!< Channel-11 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH12_IntStat_Pos            (11U)
#define DMA_INTSTATUS2_CH12_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH12_IntStat_Pos)              /*!< 0x00000000 00000800 */
#define DMA_INTSTATUS2_CH12_IntStat                DMA_INTSTATUS2_CH12_IntStat_Msk                          /*!< Channel-12 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH13_IntStat_Pos            (12U)
#define DMA_INTSTATUS2_CH13_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH13_IntStat_Pos)              /*!< 0x00000000 00001000 */
#define DMA_INTSTATUS2_CH13_IntStat                DMA_INTSTATUS2_CH13_IntStat_Msk                          /*!< Channel-13 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH14_IntStat_Pos            (13U)
#define DMA_INTSTATUS2_CH14_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH14_IntStat_Pos)              /*!< 0x00000000 00002000 */
#define DMA_INTSTATUS2_CH14_IntStat                DMA_INTSTATUS2_CH14_IntStat_Msk                          /*!< Channel-14 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH15_IntStat_Pos            (14U)
#define DMA_INTSTATUS2_CH15_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH15_IntStat_Pos)              /*!< 0x00000000 00004000 */
#define DMA_INTSTATUS2_CH15_IntStat                DMA_INTSTATUS2_CH15_IntStat_Msk                          /*!< Channel-15 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH16_IntStat_Pos            (15U)
#define DMA_INTSTATUS2_CH16_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH16_IntStat_Pos)              /*!< 0x00000000 00008000 */
#define DMA_INTSTATUS2_CH16_IntStat                DMA_INTSTATUS2_CH16_IntStat_Msk                          /*!< Channel-16 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH17_IntStat_Pos            (16U)
#define DMA_INTSTATUS2_CH17_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH17_IntStat_Pos)              /*!< 0x00000000 00010000 */
#define DMA_INTSTATUS2_CH17_IntStat                DMA_INTSTATUS2_CH17_IntStat_Msk                          /*!< Channel-17 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH18_IntStat_Pos            (17U)
#define DMA_INTSTATUS2_CH18_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH18_IntStat_Pos)              /*!< 0x00000000 00020000 */
#define DMA_INTSTATUS2_CH18_IntStat                DMA_INTSTATUS2_CH18_IntStat_Msk                          /*!< Channel-18 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH19_IntStat_Pos            (18U)
#define DMA_INTSTATUS2_CH19_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH19_IntStat_Pos)              /*!< 0x00000000 00040000 */
#define DMA_INTSTATUS2_CH19_IntStat                DMA_INTSTATUS2_CH19_IntStat_Msk                          /*!< Channel-19 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH20_IntStat_Pos            (19U)
#define DMA_INTSTATUS2_CH20_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH20_IntStat_Pos)              /*!< 0x00000000 00080000 */
#define DMA_INTSTATUS2_CH20_IntStat                DMA_INTSTATUS2_CH20_IntStat_Msk                          /*!< Channel-20 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH21_IntStat_Pos            (20U)
#define DMA_INTSTATUS2_CH21_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH21_IntStat_Pos)              /*!< 0x00000000 00100000 */
#define DMA_INTSTATUS2_CH21_IntStat                DMA_INTSTATUS2_CH21_IntStat_Msk                          /*!< Channel-21 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH22_IntStat_Pos            (21U)
#define DMA_INTSTATUS2_CH22_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH22_IntStat_Pos)              /*!< 0x00000000 00200000 */
#define DMA_INTSTATUS2_CH22_IntStat                DMA_INTSTATUS2_CH22_IntStat_Msk                          /*!< Channel-22 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH23_IntStat_Pos            (22U)
#define DMA_INTSTATUS2_CH23_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH23_IntStat_Pos)              /*!< 0x00000000 00400000 */
#define DMA_INTSTATUS2_CH23_IntStat                DMA_INTSTATUS2_CH23_IntStat_Msk                          /*!< Channel-23 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH24_IntStat_Pos            (23U)
#define DMA_INTSTATUS2_CH24_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH24_IntStat_Pos)              /*!< 0x00000000 00800000 */
#define DMA_INTSTATUS2_CH24_IntStat                DMA_INTSTATUS2_CH24_IntStat_Msk                          /*!< Channel-24 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH25_IntStat_Pos            (24U)
#define DMA_INTSTATUS2_CH25_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH25_IntStat_Pos)              /*!< 0x00000000 01000000 */
#define DMA_INTSTATUS2_CH25_IntStat                DMA_INTSTATUS2_CH25_IntStat_Msk                          /*!< Channel-25 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH26_IntStat_Pos            (25U)
#define DMA_INTSTATUS2_CH26_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH26_IntStat_Pos)              /*!< 0x00000000 02000000 */
#define DMA_INTSTATUS2_CH26_IntStat                DMA_INTSTATUS2_CH26_IntStat_Msk                          /*!< Channel-26 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH27_IntStat_Pos            (26U)
#define DMA_INTSTATUS2_CH27_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH27_IntStat_Pos)              /*!< 0x00000000 04000000 */
#define DMA_INTSTATUS2_CH27_IntStat                DMA_INTSTATUS2_CH27_IntStat_Msk                          /*!< Channel-27 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH28_IntStat_Pos            (27U)
#define DMA_INTSTATUS2_CH28_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH28_IntStat_Pos)              /*!< 0x00000000 08000000 */
#define DMA_INTSTATUS2_CH28_IntStat                DMA_INTSTATUS2_CH28_IntStat_Msk                          /*!< Channel-28 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH29_IntStat_Pos            (28U)
#define DMA_INTSTATUS2_CH29_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH29_IntStat_Pos)              /*!< 0x00000000 10000000 */
#define DMA_INTSTATUS2_CH29_IntStat                DMA_INTSTATUS2_CH29_IntStat_Msk                          /*!< Channel-29 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH30_IntStat_Pos            (29U)
#define DMA_INTSTATUS2_CH30_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH30_IntStat_Pos)              /*!< 0x00000000 20000000 */
#define DMA_INTSTATUS2_CH30_IntStat                DMA_INTSTATUS2_CH30_IntStat_Msk                          /*!< Channel-30 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH31_IntStat_Pos            (30U)
#define DMA_INTSTATUS2_CH31_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH31_IntStat_Pos)              /*!< 0x00000000 40000000 */
#define DMA_INTSTATUS2_CH31_IntStat                DMA_INTSTATUS2_CH31_IntStat_Msk                          /*!< Channel-31 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CH32_IntStat_Pos            (31U)
#define DMA_INTSTATUS2_CH32_IntStat_Msk            (0x01UL << DMA_INTSTATUS2_CH32_IntStat_Pos)              /*!< 0x00000000 80000000 */
#define DMA_INTSTATUS2_CH32_IntStat                DMA_INTSTATUS2_CH32_IntStat_Msk                          /*!< Channel-32 Interrupt Status Bit. */
#define DMA_INTSTATUS2_CommonReg_IntStat_Pos       (32U)
#define DMA_INTSTATUS2_CommonReg_IntStat_Msk       (0x01UL << DMA_INTSTATUS2_CommonReg_IntStat_Pos)         /*!< 0x00000001 00000000 */
#define DMA_INTSTATUS2_CommonReg_IntStat           DMA_INTSTATUS2_CommonReg_IntStat_Msk                     /*!< Common Register Interrupt Status Bit. */

/******************  Bit definition for DMAC_COMMONREG_INTCLEARREG register  ******************/
#define DMA_COM_INTCLEAR_SLVIF_DEC_ERR_IntStat_Pos            (0U)
#define DMA_COM_INTCLEAR_SLVIF_DEC_ERR_IntStat_Msk            (0x01UL << DMA_COM_INTCLEAR_SLVIF_DEC_ERR_IntStat_Pos)              /*!< 0x00000000 00000001 */
#define DMA_COM_INTCLEAR_SLVIF_DEC_ERR_IntStat                DMA_COM_INTCLEAR_SLVIF_DEC_ERR_IntStat_Msk                          /*!< Slave Interface Common Register Decode Error Interrupt clear Bit. */
#define DMA_COM_INTCLEAR_SLVIF_WR2RO_ERR_IntStat_Pos          (1U)
#define DMA_COM_INTCLEAR_SLVIF_WR2RO_ERR_IntStat_Msk          (0x01UL << DMA_COM_INTCLEAR_SLVIF_WR2RO_ERR_IntStat_Pos)            /*!< 0x00000000 00000002 */
#define DMA_COM_INTCLEAR_SLVIF_WR2RO_ERR_IntStat              DMA_COM_INTCLEAR_SLVIF_WR2RO_ERR_IntStat_Msk                        /*!< Slave Interface Common Register Write to Read only Error Interrupt clear Bit. */
#define DMA_COM_INTCLEAR_SLVIF_RD2WO_ERR_IntStat_Pos          (2U)
#define DMA_COM_INTCLEAR_SLVIF_RD2WO_ERR_IntStat_Msk          (0x01UL << DMA_COM_INTCLEAR_SLVIF_RD2WO_ERR_IntStat_Pos)            /*!< 0x00000000 00000004 */
#define DMA_COM_INTCLEAR_SLVIF_RD2WO_ERR_IntStat              DMA_COM_INTCLEAR_SLVIF_RD2WO_ERR_IntStat_Msk                        /*!< Slave Interface Common Register Read to Write only Error Interrupt clear Bit. */
#define DMA_COM_INTCLEAR_SLVIF_WrOnHold_ERR_IntStat_Pos       (3U)
#define DMA_COM_INTCLEAR_SLVIF_WrOnHold_ERR_IntStat_Msk       (0x01UL << DMA_COM_INTCLEAR_SLVIF_WrOnHold_ERR_IntStat_Pos)         /*!< 0x00000000 00000008 */
#define DMA_COM_INTCLEAR_SLVIF_WrOnHold_ERR_IntStat           DMA_COM_INTCLEAR_SLVIF_WrOnHold_ERR_IntStat_Msk                     /*!< Slave Interface Common Register Write On Hold Error Interrupt clear Bit. */
#define DMA_COM_INTCLEAR_SLVIF_UndefinedReg_DEC_IntStat_Pos   (8U)
#define DMA_COM_INTCLEAR_SLVIF_UndefinedReg_DEC_IntStat_Msk   (0x01UL << DMA_COM_INTCLEAR_SLVIF_UndefinedReg_DEC_IntStat_Pos)     /*!< 0x00000000 00000100 */
#define DMA_COM_INTCLEAR_SLVIF_UndefinedReg_DEC_IntStat       DMA_COM_INTCLEAR_SLVIF_UndefinedReg_DEC_IntStat_Msk                 /*!< Slave Interface Undefined register Decode Error Interrupt clear Bit. */
/******************  Bit definition for DMAC_COMMONREG_INTSTATUS_ENABLEREG register  ******************/
#define DMA_COM_INTSTATUS_EN_SLVIF_DEC_ERR_IntStat_Pos            (0U)
#define DMA_COM_INTSTATUS_EN_SLVIF_DEC_ERR_IntStat_Msk            (0x01UL << DMA_COM_INTSTATUS_EN_SLVIF_DEC_ERR_IntStat_Pos)              /*!< 0x00000000 00000001 */
#define DMA_COM_INTSTATUS_EN_SLVIF_DEC_ERR_IntStat                DMA_COM_INTSTATUS_EN_SLVIF_DEC_ERR_IntStat_Msk                          /*!< Slave Interface Common Register Decode Error Interrupt Enable Bit. */
#define DMA_COM_INTSTATUS_EN_SLVIF_WR2RO_ERR_IntStat_Pos          (1U)
#define DMA_COM_INTSTATUS_EN_SLVIF_WR2RO_ERR_IntStat_Msk          (0x01UL << DMA_COM_INTSTATUS_EN_SLVIF_WR2RO_ERR_IntStat_Pos)            /*!< 0x00000000 00000002 */
#define DMA_COM_INTSTATUS_EN_SLVIF_WR2RO_ERR_IntStat              DMA_COM_INTSTATUS_EN_SLVIF_WR2RO_ERR_IntStat_Msk                        /*!< Slave Interface Common Register Write to Read only Error Interrupt Enable Bit. */
#define DMA_COM_INTSTATUS_EN_SLVIF_RD2WO_ERR_IntStat_Pos          (2U)
#define DMA_COM_INTSTATUS_EN_SLVIF_RD2WO_ERR_IntStat_Msk          (0x01UL << DMA_COM_INTSTATUS_EN_SLVIF_RD2WO_ERR_IntStat_Pos)            /*!< 0x00000000 00000004 */
#define DMA_COM_INTSTATUS_EN_SLVIF_RD2WO_ERR_IntStat              DMA_COM_INTSTATUS_EN_SLVIF_RD2WO_ERR_IntStat_Msk                        /*!< Slave Interface Common Register Read to Write only Error Interrupt Enable Bit. */
#define DMA_COM_INTSTATUS_EN_SLVIF_WrOnHold_ERR_IntStat_Pos       (3U)
#define DMA_COM_INTSTATUS_EN_SLVIF_WrOnHold_ERR_IntStat_Msk       (0x01UL << DMA_COM_INTSTATUS_EN_SLVIF_WrOnHold_ERR_IntStat_Pos)         /*!< 0x00000000 00000008 */
#define DMA_COM_INTSTATUS_EN_SLVIF_WrOnHold_ERR_IntStat           DMA_COM_INTSTATUS_EN_SLVIF_WrOnHold_ERR_IntStat_Msk                     /*!< Slave Interface Common Register Write On Hold Error Interrupt Enable Bit. */
#define DMA_COM_INTSTATUS_EN_SLVIF_UndefinedReg_DEC_IntStat_Pos   (8U)
#define DMA_COM_INTSTATUS_EN_SLVIF_UndefinedReg_DEC_IntStat_Msk   (0x01UL << DMA_COM_INTSTATUS_EN_SLVIF_UndefinedReg_DEC_IntStat_Pos)     /*!< 0x00000000 00000100 */
#define DMA_COM_INTSTATUS_EN_SLVIF_UndefinedReg_DEC_IntStat       DMA_COM_INTSTATUS_EN_SLVIF_UndefinedReg_DEC_IntStat_Msk                 /*!< Slave Interface Undefined register Decode Error Interrupt Enable Bit. */
/******************  Bit definition for DMAC_COMMONREG_INTSIGNAL_ENABLEREG register  ******************/
#define DMA_COM_INTSIGNAL_EN_SLVIF_DEC_ERR_IntSignal_Pos            (0U)
#define DMA_COM_INTSIGNAL_EN_SLVIF_DEC_ERR_IntSignal_Msk            (0x01UL << DMA_COM_INTSIGNAL_EN_SLVIF_DEC_ERR_IntSignal_Pos)              /*!< 0x00000000 00000001 */
#define DMA_COM_INTSIGNAL_EN_SLVIF_DEC_ERR_IntSignal                DMA_COM_INTSIGNAL_EN_SLVIF_DEC_ERR_IntSignal_Msk                          /*!< Slave Interface Common Register Decode Error Interrupt Signal Enable Bit. */
#define DMA_COM_INTSIGNAL_EN_SLVIF_WR2RO_ERR_IntSignal_Pos          (1U)
#define DMA_COM_INTSIGNAL_EN_SLVIF_WR2RO_ERR_IntSignal_Msk          (0x01UL << DMA_COM_INTSIGNAL_EN_SLVIF_WR2RO_ERR_IntSignal_Pos)            /*!< 0x00000000 00000002 */
#define DMA_COM_INTSIGNAL_EN_SLVIF_WR2RO_ERR_IntSignal              DMA_COM_INTSIGNAL_EN_SLVIF_WR2RO_ERR_IntSignal_Msk                        /*!< Slave Interface Common Register Write to Read only Error Interrupt Signal Enable Bit. */
#define DMA_COM_INTSIGNAL_EN_SLVIF_RD2WO_ERR_IntSignal_Pos          (2U)
#define DMA_COM_INTSIGNAL_EN_SLVIF_RD2WO_ERR_IntSignal_Msk          (0x01UL << DMA_COM_INTSIGNAL_EN_SLVIF_RD2WO_ERR_IntSignal_Pos)            /*!< 0x00000000 00000004 */
#define DMA_COM_INTSIGNAL_EN_SLVIF_RD2WO_ERR_IntSignal              DMA_COM_INTSIGNAL_EN_SLVIF_RD2WO_ERR_IntSignal_Msk                        /*!< Slave Interface Common Register Read to Write only Error Interrupt Signal Enable Bit. */
#define DMA_COM_INTSIGNAL_EN_SLVIF_WrOnHold_ERR_IntSignal_Pos       (3U)
#define DMA_COM_INTSIGNAL_EN_SLVIF_WrOnHold_ERR_IntSignal_Msk       (0x01UL << DMA_COM_INTSIGNAL_EN_SLVIF_WrOnHold_ERR_IntSignal_Pos)         /*!< 0x00000000 00000008 */
#define DMA_COM_INTSIGNAL_EN_SLVIF_WrOnHold_ERR_IntSignal           DMA_COM_INTSIGNAL_EN_SLVIF_WrOnHold_ERR_IntSignal_Msk                     /*!< Slave Interface Common Register Write On Hold Error Interrupt Signal Enable Bit. */
#define DMA_COM_INTSIGNAL_EN_SLVIF_UndefinedReg_DEC_IntSignal_Pos   (8U)
#define DMA_COM_INTSIGNAL_EN_SLVIF_UndefinedReg_DEC_IntSignal_Msk   (0x01UL << DMA_COM_INTSIGNAL_EN_SLVIF_UndefinedReg_DEC_IntSignal_Pos)     /*!< 0x00000000 00000100 */
#define DMA_COM_INTSIGNAL_EN_SLVIF_UndefinedReg_DEC_IntSignal       DMA_COM_INTSIGNAL_EN_SLVIF_UndefinedReg_DEC_IntSignal_Msk                 /*!< Slave Interface Undefined register Decode Error Interrupt Signal Enable Bit. */
/******************  Bit definition for DMAC_COMMONREG_INTSTATUSREG register  ******************/
#define DMA_COM_INTSTATUS_SLVIF_DEC_ERR_IntSignal_Pos            (0U)
#define DMA_COM_INTSTATUS_SLVIF_DEC_ERR_IntSignal_Msk            (0x01UL << DMA_COM_INTSTATUS_SLVIF_DEC_ERR_IntSignal_Pos)              /*!< 0x00000000 00000001 */
#define DMA_COM_INTSTATUS_SLVIF_DEC_ERR_IntSignal                DMA_COM_INTSTATUS_SLVIF_DEC_ERR_IntSignal_Msk                          /*!< Slave Interface Common Register Decode Error Interrupt Signal Bit. */
#define DMA_COM_INTSTATUS_SLVIF_WR2RO_ERR_IntSignal_Pos          (1U)
#define DMA_COM_INTSTATUS_SLVIF_WR2RO_ERR_IntSignal_Msk          (0x01UL << DMA_COM_INTSTATUS_SLVIF_WR2RO_ERR_IntSignal_Pos)            /*!< 0x00000000 00000002 */
#define DMA_COM_INTSTATUS_SLVIF_WR2RO_ERR_IntSignal              DMA_COM_INTSTATUS_SLVIF_WR2RO_ERR_IntSignal_Msk                        /*!< Slave Interface Common Register Write to Read only Error Interrupt Signal Bit. */
#define DMA_COM_INTSTATUS_SLVIF_RD2WO_ERR_IntSignal_Pos          (2U)
#define DMA_COM_INTSTATUS_SLVIF_RD2WO_ERR_IntSignal_Msk          (0x01UL << DMA_COM_INTSTATUS_SLVIF_RD2WO_ERR_IntSignal_Pos)            /*!< 0x00000000 00000004 */
#define DMA_COM_INTSTATUS_SLVIF_RD2WO_ERR_IntSignal              DMA_COM_INTSTATUS_SLVIF_RD2WO_ERR_IntSignal_Msk                        /*!< Slave Interface Common Register Read to Write only Error Interrupt Signal Bit. */
#define DMA_COM_INTSTATUS_SLVIF_WrOnHold_ERR_IntSignal_Pos       (3U)
#define DMA_COM_INTSTATUS_SLVIF_WrOnHold_ERR_IntSignal_Msk       (0x01UL << DMA_COM_INTSTATUS_SLVIF_WrOnHold_ERR_IntSignal_Pos)         /*!< 0x00000000 00000008 */
#define DMA_COM_INTSTATUS_SLVIF_WrOnHold_ERR_IntSignal           DMA_COM_INTSTATUS_SLVIF_WrOnHold_ERR_IntSignal_Msk                     /*!< Slave Interface Common Register Write On Hold Error Interrupt Signal Bit. */
#define DMA_COM_INTSTATUS_SLVIF_UndefinedReg_DEC_IntSignal_Pos   (8U)
#define DMA_COM_INTSTATUS_SLVIF_UndefinedReg_DEC_IntSignal_Msk   (0x01UL << DMA_COM_INTSTATUS_SLVIF_UndefinedReg_DEC_IntSignal_Pos)     /*!< 0x00000000 00000100 */
#define DMA_COM_INTSTATUS_SLVIF_UndefinedReg_DEC_IntSignal       DMA_COM_INTSTATUS_SLVIF_UndefinedReg_DEC_IntSignal_Msk                 /*!< Slave Interface Undefined register Decode Error Interrupt Signal Enable Bit. */

/******************  Bit definition for DMAC_RESETREG register  ******************/
#define DMA_RESET_DMAC_RST_Pos                        (0U)
#define DMA_RESET_DMAC_RST_Msk                        (0x01UL << DMA_RESET_DMAC_RST_Pos)                    /*!< 0x00000000 00000001 */
#define DMA_RESET_DMAC_RST                            DMA_RESET_DMAC_RST_Msk                                /*!< DMAC Reset Request bit. */

/******************  Bit definition for DMAC_LOWPOWER_CFGREG register  ******************/
#define DMA_LOWPOWER_CFG_GBL_CSLP_EN_Pos                       (0U)
#define DMA_LOWPOWER_CFG_GBL_CSLP_EN_Msk                       (0x01UL << DMA_LOWPOWER_CFG_GBL_CSLP_EN_Pos)                    /*!< 0x00000000 00000001 */
#define DMA_LOWPOWER_CFG_GBL_CSLP_EN                           DMA_LOWPOWER_CFG_GBL_CSLP_EN_Msk                                /*!< Global Context Sensitive Low Power feature enable. */
#define DMA_LOWPOWER_CFG_CHNL_CSLP_EN_Pos                      (1U)
#define DMA_LOWPOWER_CFG_CHNL_CSLP_EN_Msk                      (0x01UL << DMA_LOWPOWER_CFG_CHNL_CSLP_EN_Pos)                    /*!< 0x00000000 00000002 */
#define DMA_LOWPOWER_CFG_CHNL_CSLP_EN                          DMA_LOWPOWER_CFG_CHNL_CSLP_EN_Msk                                /*!< DMA Channel Context Sensitive Low Power feature enable. */
#define DMA_LOWPOWER_CFG_SBIU_CSLP_EN_Pos                      (2U)
#define DMA_LOWPOWER_CFG_SBIU_CSLP_EN_Msk                      (0x01UL << DMA_LOWPOWER_CFG_SBIU_CSLP_EN_Pos)                    /*!< 0x00000000 00000004 */
#define DMA_LOWPOWER_CFG_SBIU_CSLP_EN                          DMA_LOWPOWER_CFG_SBIU_CSLP_EN_Msk                                /*!< SBIU Context Sensitive Low Power feature enable. */
#define DMA_LOWPOWER_CFG_MXIF_CSLP_EN_Pos                      (3U)
#define DMA_LOWPOWER_CFG_MXIF_CSLP_EN_Msk                      (0x01UL << DMA_LOWPOWER_CFG_MXIF_CSLP_EN_Pos)                    /*!< 0x00000000 00000004 */
#define DMA_LOWPOWER_CFG_MXIF_CSLP_EN                          DMA_LOWPOWER_CFG_MXIF_CSLP_EN_Msk                                /*!< AXI Master Interface Context Sensitive Low Power feature enable. */
#define DMA_LOWPOWER_CFG_GLCH_LPDLY_Pos                        (32U)
#define DMA_LOWPOWER_CFG_GLCH_LPDLY_Msk                        (0xFFUL << DMA_LOWPOWER_CFG_GLCH_LPDLY_Pos)                    /*!< 0x000000FF 00000000 */
#define DMA_LOWPOWER_CFG_GLCH_LPDLY                            DMA_LOWPOWER_CFG_GLCH_LPDLY_Msk                                /*!< Defines the load value to be programmed into the Global and DMA Channel low power delay counter.  */
#define DMA_LOWPOWER_CFG_SBIU_LPDLY_Pos                        (40U)
#define DMA_LOWPOWER_CFG_SBIU_LPDLY_Msk                        (0xFFUL << DMA_LOWPOWER_CFG_SBIU_LPDLY_Pos)                    /*!< 0x0000FF00 00000000 */
#define DMA_LOWPOWER_CFG_SBIU_LPDLY                            DMA_LOWPOWER_CFG_SBIU_LPDLY_Msk                                /*!< Defines the load value to be programmed into the SBIU low power delay counter.  */
#define DMA_LOWPOWER_CFG_MXIF_LPDLY_Pos                        (48U)
#define DMA_LOWPOWER_CFG_MXIF_LPDLY_Msk                        (0xFFUL << DMA_LOWPOWER_CFG_MXIF_LPDLY_Pos)                    /*!< 0x00FF0000 00000000 */
#define DMA_LOWPOWER_CFG_MXIF_LPDLY                            DMA_LOWPOWER_CFG_MXIF_LPDLY_Msk                                /*!< Defines the load value to be programmed into the AXI Master Interface low power delay counter. */



/******************  Bit definition for CHx_BLOCK_TS register  ******************/
#define DMA_CHx_BLOCK_TS_Pos                        (0U)
#define DMA_CHx_BLOCK_TS_Msk                        (0x3FFFFFUL << DMA_CHx_BLOCK_TS_Pos)                    /*!< 0x00000000 003FFFFF */
#define DMA_CHx_BLOCK_TS                            DMA_CHx_BLOCK_TS_Msk                                    /*!< Block Transfer Size. */
#define DMA_CHx_CTL_SMS_Pos                         (0U)
#define DMA_CHx_CTL_SMS_Msk                         (0x01UL << DMA_CHx_CTL_SMS_Pos)                         /*!< 0x00000000 00000001 */
#define DMA_CHx_CTL_SMS                             DMA_CHx_CTL_SMS_Msk                                     /*!< Source Master Select. */
#define DMA_CHx_CTL_DMS_Pos                         (2U)
#define DMA_CHx_CTL_DMS_Msk                         (0x01UL << DMA_CHx_CTL_DMS_Pos)                         /*!< 0x00000000 00000004 */
#define DMA_CHx_CTL_DMS                             DMA_CHx_CTL_DMS_Msk                                     /*!< Destination  Master Select. */
#define DMA_CHx_CTL_SINC_Pos                        (4U)
#define DMA_CHx_CTL_SINC_Msk                        (0x01UL << DMA_CHx_CTL_SINC_Pos)                        /*!< 0x00000000 00000010 */
#define DMA_CHx_CTL_SINC                            DMA_CHx_CTL_SINC_Msk                                    /*!< Source Address Increment. */
#define DMA_CHx_CTL_DINC_Pos                        (6U)
#define DMA_CHx_CTL_DINC_Msk                        (0x01UL << DMA_CHx_CTL_DINC_Pos)                        /*!< 0x00000000 00000040 */
#define DMA_CHx_CTL_DINC                            DMA_CHx_CTL_DINC_Msk                                    /*!< Destination Address Increment. */
#define DMA_CHx_CTL_SRC_TR_WIDTH_Pos                (8U)
#define DMA_CHx_CTL_SRC_TR_WIDTH_Msk                (0x07UL << DMA_CHx_CTL_SRC_TR_WIDTH_Pos)                /*!< 0x00000000 00000700 */
#define DMA_CHx_CTL_SRC_TR_WIDTH                    DMA_CHx_CTL_SRC_TR_WIDTH_Msk                            /*!< Source Transfer Width. */
#define DMA_CHx_CTL_DST_TR_WIDTH_Pos                (11U)
#define DMA_CHx_CTL_DST_TR_WIDTH_Msk                (0x07UL << DMA_CHx_CTL_DST_TR_WIDTH_Pos)                /*!< 0x00000000 00003800 */
#define DMA_CHx_CTL_DST_TR_WIDTH                    DMA_CHx_CTL_DST_TR_WIDTH_Msk                            /*!< Destination Transfer Width. */
#define DMA_CHx_CTL_SRC_MSIZE_Pos                   (14U)
#define DMA_CHx_CTL_SRC_MSIZE_Msk                   (0x0FUL << DMA_CHx_CTL_SRC_MSIZE_Pos)                   /*!< 0x00000000 0003C000 */
#define DMA_CHx_CTL_SRC_MSIZE                       DMA_CHx_CTL_SRC_MSIZE_Msk                               /*!< Source Burst Transaction Length. */
#define DMA_CHx_CTL_DST_MSIZE_Pos                   (18U)
#define DMA_CHx_CTL_DST_MSIZE_Msk                   (0x0FUL << DMA_CHx_CTL_DST_MSIZE_Pos)                   /*!< 0x00000000 003C0000 */
#define DMA_CHx_CTL_DST_MSIZE                       DMA_CHx_CTL_DST_MSIZE_Msk                               /*!< Destination Burst Transaction Length. */
#define DMA_CHx_CTL_AR_CACHE_Pos                    (22U)
#define DMA_CHx_CTL_AR_CACHE_Msk                    (0x0FUL << DMA_CHx_CTL_AR_CACHE_Pos)                    /*!< 0x00000000 03C00000 */
#define DMA_CHx_CTL_AR_CACHE                        DMA_CHx_CTL_AR_CACHE_Msk                                /*!< AXI 'ar_cache' signal. */
#define DMA_CHx_CTL_AW_CACHE_Pos                    (26U)
#define DMA_CHx_CTL_AW_CACHE_Msk                    (0x0FUL << DMA_CHx_CTL_AW_CACHE_Pos)                    /*!< 0x00000000 3C000000 */
#define DMA_CHx_CTL_AW_CACHE                        DMA_CHx_CTL_AW_CACHE_Msk                                /*!< AXI 'aw_cache' signal. */
#define DMA_CHx_CTL_NonPosted_LastWrite_En_Pos      (30U)
#define DMA_CHx_CTL_NonPosted_LastWrite_En_Msk      (0x01UL << DMA_CHx_CTL_NonPosted_LastWrite_En_Pos)      /*!< 0x00000000 40000000 */
#define DMA_CHx_CTL_NonPosted_LastWrite_En          DMA_CHx_CTL_NonPosted_LastWrite_En_Msk                  /*!< Non Posted Last Write Enable. */
#define DMA_CHx_CTL_AR_PROT_Pos                     (32U)
#define DMA_CHx_CTL_AR_PROT_Msk                     (0x07UL << DMA_CHx_CTL_AR_PROT_Pos)                     /*!< 0x00000007 00000000 */
#define DMA_CHx_CTL_AR_PROT                         DMA_CHx_CTL_AR_PROT_Msk                                 /*!< AXI 'ar_prot' signal. */
#define DMA_CHx_CTL_AW_PROT_Pos                     (35U)
#define DMA_CHx_CTL_AW_PROT_Msk                     (0x07UL << DMA_CHx_CTL_AW_PROT_Pos)                     /*!< 0x00000038 00000000 */
#define DMA_CHx_CTL_AW_PROT                         DMA_CHx_CTL_AW_PROT_Msk                                 /*!< AXI 'aw_prot' signal. */
#define DMA_CHx_CTL_ARLEN_EN_Pos                    (38U)
#define DMA_CHx_CTL_ARLEN_EN_Msk                    (0x01UL << DMA_CHx_CTL_ARLEN_EN_Pos)                    /*!< 0x00000040 00000000 */
#define DMA_CHx_CTL_ARLEN_EN                        DMA_CHx_CTL_ARLEN_EN_Msk                                /*!< Source Burst Length Enable. */
#define DMA_CHx_CTL_ARLEN_Pos                       (39U)
#define DMA_CHx_CTL_ARLEN_Msk                       (0xFFUL << DMA_CHx_CTL_ARLEN_Pos)                       /*!< 0x00003FC0 00000000 */
#define DMA_CHx_CTL_ARLEN                           DMA_CHx_CTL_ARLEN_Msk                                   /*!< Source Burst Length. */
#define DMA_CHx_CTL_AWLEN_EN_Pos                    (47U)
#define DMA_CHx_CTL_AWLEN_EN_Msk                    (0x01UL << DMA_CHx_CTL_AWLEN_EN_Pos)                    /*!< 0x00004000 00000000 */
#define DMA_CHx_CTL_AWLEN_EN                        DMA_CHx_CTL_AWLEN_EN_Msk                                /*!< Destination Burst Length Enable. */
#define DMA_CHx_CTL_AWLEN_Pos                       (48U)
#define DMA_CHx_CTL_AWLEN_Msk                       (0xFFUL << DMA_CHx_CTL_AWLEN_Pos)                       /*!< 0x007F8000 00000000 */
#define DMA_CHx_CTL_AWLEN                           DMA_CHx_CTL_AWLEN_Msk                                   /*!< Destination Burst Length. */
#define DMA_CHx_CTL_SRC_STAT_EN_Pos                 (56U)
#define DMA_CHx_CTL_SRC_STAT_EN_Msk                 (0x01UL << DMA_CHx_CTL_SRC_STAT_EN_Pos)                 /*!< 0x00800000 00000000 */
#define DMA_CHx_CTL_SRC_STAT_EN                     DMA_CHx_CTL_SRC_STAT_EN_Msk                             /*!< Source Status Enable. */
#define DMA_CHx_CTL_DST_STAT_EN_Pos                 (57U)
#define DMA_CHx_CTL_DST_STAT_EN_Msk                 (0x01UL << DMA_CHx_CTL_DST_STAT_EN_Pos)                 /*!< 0x01000000 00000000 */
#define DMA_CHx_CTL_DST_STAT_EN                     DMA_CHx_CTL_DST_STAT_EN_Msk                             /*!< Destination Status Enable. */
#define DMA_CHx_CTL_IOC_BlkTfr_Pos                  (58U)
#define DMA_CHx_CTL_IOC_BlkTfr_Msk                  (0x01UL << DMA_CHx_CTL_IOC_BlkTfr_Pos)                  /*!< 0x02000000 00000000 */
#define DMA_CHx_CTL_IOC_BlkTfr                      DMA_CHx_CTL_IOC_BlkTfr_Msk                              /*!< Interrupt On completion of Block Transfer. */
#define DMA_CHx_CTL_SHADOWREG_OR_LLI_LAST_Pos       (62U)
#define DMA_CHx_CTL_SHADOWREG_OR_LLI_LAST_Msk       (0x01UL << DMA_CHx_CTL_SHADOWREG_OR_LLI_LAST_Pos)       /*!< 0x40000000 00000000 */
#define DMA_CHx_CTL_SHADOWREG_OR_LLI_LAST           DMA_CHx_CTL_SHADOWREG_OR_LLI_LAST_Msk                   /*!< Last Shadow Register/Linked List Item. */
#define DMA_CHx_CTL_SHADOWREG_OR_LLI_VALID_Pos      (63U)
#define DMA_CHx_CTL_SHADOWREG_OR_LLI_VALID_Msk      (0x01UL << DMA_CHx_CTL_SHADOWREG_OR_LLI_VALID_Pos)      /*!< 0x80000000 00000000 */
#define DMA_CHx_CTL_SHADOWREG_OR_LLI_VALID          DMA_CHx_CTL_SHADOWREG_OR_LLI_VALID_Msk                  /*!< Shadow Register content/Linked List Item valid. */

/******************  Bit definition for CHx_CFG register  ******************/
#define DMA_CHx_CFG_SRC_MULTBLK_TYPE_Pos            (0)
#define DMA_CHx_CFG_SRC_MULTBLK_TYPE_Msk            (0x03UL << DMA_CHx_CFG_SRC_MULTBLK_TYPE_Pos)            /*!< 0x00000000 00000003 */
#define DMA_CHx_CFG_SRC_MULTBLK_TYPE                DMA_CHx_CFG_SRC_MULTBLK_TYPE_Msk                        /*!< Source Multi Block Transfer Type. */
#define DMA_CHx_CFG_DST_MULTBLK_TYPE_Pos            (2)
#define DMA_CHx_CFG_DST_MULTBLK_TYPE_Msk            (0x03UL << DMA_CHx_CFG_DST_MULTBLK_TYPE_Pos)            /*!< 0x00000000 0000000C */
#define DMA_CHx_CFG_DST_MULTBLK_TYPE                DMA_CHx_CFG_DST_MULTBLK_TYPE_Msk                        /*!< Destination Multi Block Transfer Type. */
#define DMA_CHx_CFG_TT_FC_Pos                       (32)
#define DMA_CHx_CFG_TT_FC_Msk                       (0x07UL << DMA_CHx_CFG_TT_FC_Pos)                       /*!< 0x00000007 00000000 */
#define DMA_CHx_CFG_TT_FC                           DMA_CHx_CFG_TT_FC_Msk                                   /*!< Transfer Type and Flow Control. */
#define DMA_CHx_CFG_HS_SEL_SRC_Pos                  (35)
#define DMA_CHx_CFG_HS_SEL_SRC_Msk                  (0x01UL << DMA_CHx_CFG_HS_SEL_SRC_Pos)                  /*!< 0x00000008 00000000 */
#define DMA_CHx_CFG_HS_SEL_SRC                      DMA_CHx_CFG_HS_SEL_SRC_Msk                              /*!< Source Software or Hardware Handshaking Select. */
#define DMA_CHx_CFG_HS_SEL_DST_Pos                  (36)
#define DMA_CHx_CFG_HS_SEL_DST_Msk                  (0x01UL << DMA_CHx_CFG_HS_SEL_DST_Pos)                  /*!< 0x00000010 00000000 */
#define DMA_CHx_CFG_HS_SEL_DST                      DMA_CHx_CFG_HS_SEL_DST_Msk                              /*!< Destination Software or Hardware Handshaking Select. */
#define DMA_CHx_CFG_SRC_HWHS_POL_Pos                (37)
#define DMA_CHx_CFG_SRC_HWHS_POL_Msk                (0x01UL << DMA_CHx_CFG_SRC_HWHS_POL_Pos)                /*!< 0x00000020 00000000 */
#define DMA_CHx_CFG_SRC_HWHS_POL                    DMA_CHx_CFG_SRC_HWHS_POL_Msk                            /*!< Source Hardware Handshaking Interface Polarity. */
#define DMA_CHx_CFG_DST_HWHS_POL_Pos                (38)
#define DMA_CHx_CFG_DST_HWHS_POL_Msk                (0x01UL << DMA_CHx_CFG_DST_HWHS_POL_Pos)                /*!< 0x00000040 00000000 */
#define DMA_CHx_CFG_DST_HWHS_POL                    DMA_CHx_CFG_DST_HWHS_POL_Msk                            /*!< Destination Hardware Handshaking Interface Polarity. */
#define DMA_CHx_CFG_CH_PRIOR_Pos                    (49)
#define DMA_CHx_CFG_CH_PRIOR_Msk                    (0x07UL << DMA_CHx_CFG_CH_PRIOR_Pos)                    /*!< 0x000E0000 00000000 */
#define DMA_CHx_CFG_CH_PRIOR                        DMA_CHx_CFG_CH_PRIOR_Msk                                /*!< Channel Priority. */
#define DMA_CHx_CFG_LOCK_CH_Pos                     (52)
#define DMA_CHx_CFG_LOCK_CH_Msk                     (0x01UL << DMA_CHx_CFG_LOCK_CH_Pos)                     /*!< 0x00100000 00000000 */
#define DMA_CHx_CFG_LOCK_CH                         DMA_CHx_CFG_LOCK_CH_Msk                                 /*!< Channel Lock bit. */
#define DMA_CHx_CFG_LOCK_CH_L_Pos                   (53)
#define DMA_CHx_CFG_LOCK_CH_L_Msk                   (0x03UL << DMA_CHx_CFG_LOCK_CH_L_Pos)                   /*!< 0x00600000 00000000 */
#define DMA_CHx_CFG_LOCK_CH_L                       DMA_CHx_CFG_LOCK_CH_L_Msk                               /*!< Channel Lock Level. */
#define DMA_CHx_CFG_SRC_OSR_LMT_Pos                 (55)
#define DMA_CHx_CFG_SRC_OSR_LMT_Msk                 (0x0FUL << DMA_CHx_CFG_SRC_OSR_LMT_Pos)                 /*!< 0x07800000 00000000 */
#define DMA_CHx_CFG_SRC_OSR_LMT                     DMA_CHx_CFG_SRC_OSR_LMT_Msk                             /*!< Source Outstanding Request Limit. */
#define DMA_CHx_CFG_DST_OSR_LMT_Pos                 (59)
#define DMA_CHx_CFG_DST_OSR_LMT_Msk                 (0x0FUL << DMA_CHx_CFG_DST_OSR_LMT_Pos)                 /*!< 0x78000000 00000000 */
#define DMA_CHx_CFG_DST_OSR_LMT                     DMA_CHx_CFG_DST_OSR_LMT_Msk                             /*!< Destination Outstanding Request Limit. */

/******************  Bit definition for CHx_CFG2 register  ******************/
#define DMA_CHx_CFG2_SRC_MULTBLK_TYPE_Pos           (0)
#define DMA_CHx_CFG2_SRC_MULTBLK_TYPE_Msk           (0x03UL << DMA_CHx_CFG2_SRC_MULTBLK_TYPE_Pos)           /*!< 0x00000000 00000003 */
#define DMA_CHx_CFG2_SRC_MULTBLK_TYPE               DMA_CHx_CFG2_SRC_MULTBLK_TYPE_Msk                       /*!< Source Multi Block Transfer Type. */
#define DMA_CHx_CFG2_DST_MULTBLK_TYPE_Pos           (2)
#define DMA_CHx_CFG2_DST_MULTBLK_TYPE_Msk           (0x03UL << DMA_CHx_CFG2_DST_MULTBLK_TYPE_Pos)           /*!< 0x00000000 0000000C */
#define DMA_CHx_CFG2_DST_MULTBLK_TYPE               DMA_CHx_CFG2_DST_MULTBLK_TYPE_Msk                       /*!< Destination Multi Block Transfer Type. */

#define DMA_CHx_CFG2_SRC_PER_Pos                    (4)
#define DMA_CHx_CFG2_DST_PER_Pos                    (11)

#define DMA_CHx_CFG2_TT_FC_Pos                      (32)
#define DMA_CHx_CFG2_TT_FC_Msk                      (0x07UL << DMA_CHx_CFG2_TT_FC_Pos)                      /*!< 0x00000007 00000000 */
#define DMA_CHx_CFG2_TT_FC                          DMA_CHx_CFG2_TT_FC_Msk                                  /*!< Destination Multi Block Transfer Type. */
#define DMA_CHx_CFG2_HS_SEL_SRC_Pos                 (35)
#define DMA_CHx_CFG2_HS_SEL_SRC_Msk                 (0x01UL << DMA_CHx_CFG2_HS_SEL_SRC_Pos)                 /*!< 0x00000008 00000000 */
#define DMA_CHx_CFG2_HS_SEL_SRC                     DMA_CHx_CFG2_HS_SEL_SRC_Msk                             /*!< Source Software or Hardware Handshaking Select. */
#define DMA_CHx_CFG2_HS_SEL_DST_Pos                 (36)
#define DMA_CHx_CFG2_HS_SEL_DST_Msk                 (0x01UL << DMA_CHx_CFG2_HS_SEL_DST_Pos)                 /*!< 0x00000010 00000000 */
#define DMA_CHx_CFG2_HS_SEL_DST                     DMA_CHx_CFG2_HS_SEL_DST_Msk                             /*!< Destination Software or Hardware Handshaking Select. */
#define DMA_CHx_CFG2_SRC_HWHS_POL_Pos               (37)
#define DMA_CHx_CFG2_SRC_HWHS_POL_Msk               (0x01UL << DMA_CHx_CFG2_SRC_HWHS_POL_Pos)               /*!< 0x00000020 00000000 */
#define DMA_CHx_CFG2_SRC_HWHS_POL                   DMA_CHx_CFG2_SRC_HWHS_POL_Msk                           /*!< Source Hardware Handshaking Interface Polarity. */
#define DMA_CHx_CFG2_DST_HWHS_POL_Pos               (38)
#define DMA_CHx_CFG2_DST_HWHS_POL_Msk               (0x01UL << DMA_CHx_CFG2_DST_HWHS_POL_Pos)               /*!< 0x00000040 00000000 */
#define DMA_CHx_CFG2_DST_HWHS_POL                   DMA_CHx_CFG2_DST_HWHS_POL_Msk                           /*!< Destination Hardware Handshaking Interface Polarity. */
#define DMA_CHx_CFG2_CH_PRIOR_Pos                   (47)
#define DMA_CHx_CFG2_CH_PRIOR_Msk                   (0x1FUL << DMA_CHx_CFG2_CH_PRIOR_Pos)                   /*!< 0x000F8000 00000000 */
#define DMA_CHx_CFG2_CH_PRIOR                       DMA_CHx_CFG2_CH_PRIOR_Msk                               /*!< Channel Priority. */
#define DMA_CHx_CFG2_LOCK_CH_Pos                    (52)
#define DMA_CHx_CFG2_LOCK_CH_Msk                    (0x01UL << DMA_CHx_CFG2_LOCK_CH_Pos)                    /*!< 0x00100000 00000000 */
#define DMA_CHx_CFG2_LOCK_CH                        DMA_CHx_CFG2_LOCK_CH_Msk                                /*!< Channel Lock bit. */
#define DMA_CHx_CFG2_LOCK_CH_L_Pos                  (53)
#define DMA_CHx_CFG2_LOCK_CH_L_Msk                  (0x03UL << DMA_CHx_CFG2_LOCK_CH_L_Pos)                  /*!< 0x00600000 00000000 */
#define DMA_CHx_CFG2_LOCK_CH_L                      DMA_CHx_CFG2_LOCK_CH_L_Msk                              /*!< Channel Lock Level. */
#define DMA_CHx_CFG2_SRC_OSR_LMT_Pos                (55)
#define DMA_CHx_CFG2_SRC_OSR_LMT_Msk                (0x0FUL << DMA_CHx_CFG2_SRC_OSR_LMT_Pos)                /*!< 0x07800000 00000000 */
#define DMA_CHx_CFG2_SRC_OSR_LMT                    DMA_CHx_CFG2_SRC_OSR_LMT_Msk                            /*!< Source Outstanding Request Limit. */
#define DMA_CHx_CFG2_DST_OSR_LMT_Pos                (59)
#define DMA_CHx_CFG2_DST_OSR_LMT_Msk                (0x0FUL << DMA_CHx_CFG2_DST_OSR_LMT_Pos)                /*!< 0x78000000 00000000 */
#define DMA_CHx_CFG2_DST_OSR_LMT                    DMA_CHx_CFG2_DST_OSR_LMT_Msk                            /*!< Destination Outstanding Request Limit. */

/******************  Bit definition for CHx_LLP register  ******************/
#define DMA_CHx_LLP_LMS_Pos                         (0)
#define DMA_CHx_LLP_LMS_Msk                         (0x01UL << DMA_CHx_LLP_LMS_Pos)                         /*!< 0x00000000 00000001 */
#define DMA_CHx_LLP_LMS                             DMA_CHx_LLP_LMS_Msk                                     /*!< LLI master Select. */
#define DMA_CHx_LLP_LOC_Pos                         (6)
#define DMA_CHx_LLP_LOC_Msk                         (0x01FFFFFFFFFFFFFFUL << DMA_CHx_LLP_LOC_Pos)           /*!< 0xFFFFFFFF FFFFFF80 */
#define DMA_CHx_LLP_LOC                             DMA_CHx_LLP_LOC_Msk                                     /*!< Starting Address Memory of LLI block. */

/******************  Bit definition for CHx_STATUSREG register  ******************/
#define DMA_CHx_STATUSREG_CMPLTD_BLK_TFR_SIZE_Pos   (0)
#define DMA_CHx_STATUSREG_CMPLTD_BLK_TFR_SIZE_Msk   (0x003FFFFFUL << DMA_CHx_STATUSREG_CMPLTD_BLK_TFR_SIZE_Pos)   /*!< 0x00000000 003FFFFF */
#define DMA_CHx_STATUSREG_CMPLTD_BLK_TFR_SIZE       DMA_CHx_STATUSREG_CMPLTD_BLK_TFR_SIZE_Msk                     /*!< Completed Block Transfer Size. */
#define DMA_CHx_STATUSREG_DATA_LEFT_IN_FIFO_Pos     (32)
#define DMA_CHx_STATUSREG_DATA_LEFT_IN_FIFO_Msk     (0x7FFFUL << DMA_CHx_STATUSREG_DATA_LEFT_IN_FIFO_Pos)         /*!< 0x00007FFF 00000000 */
#define DMA_CHx_STATUSREG_DATA_LEFT_IN_FIFO         DMA_CHx_STATUSREG_DATA_LEFT_IN_FIFO_Msk                       /*!< Data Left in FIFO. */

/******************  Bit definition for CHx_SWHSSRCREG register  ******************/
#define DMA_CHx_SWHSSRCREG_SWHS_REQ_SRC_Pos         (0)
#define DMA_CHx_SWHSSRCREG_SWHS_REQ_SRC_Msk         (0x01UL << DMA_CHx_SWHSSRCREG_SWHS_REQ_SRC_Pos)               /*!< 0x00000000 00000001 */
#define DMA_CHx_SWHSSRCREG_SWHS_REQ_SRC             DMA_CHx_SWHSSRCREG_SWHS_REQ_SRC_Msk                           /*!< Software Handshake Request for Channel Source. */
#define DMA_CHx_SWHSSRCREG_SWHS_REQ_SRC_WE_Pos      (1)
#define DMA_CHx_SWHSSRCREG_SWHS_REQ_SRC_WE_Msk      (0x01UL << DMA_CHx_SWHSSRCREG_SWHS_REQ_SRC_WE_Pos)            /*!< 0x00000000 00000002 */
#define DMA_CHx_SWHSSRCREG_SWHS_REQ_SRC_WE          DMA_CHx_SWHSSRCREG_SWHS_REQ_SRC_WE_Msk                        /*!< Write Enable bit for Software Handshake Request for Channel Source. */
#define DMA_CHx_SWHSSRCREG_SWHS_SGLREQ_SRC_Pos      (2)
#define DMA_CHx_SWHSSRCREG_SWHS_SGLREQ_SRC_Msk      (0x01UL << DMA_CHx_SWHSSRCREG_SWHS_SGLREQ_SRC_Pos)            /*!< 0x00000000 00000004 */
#define DMA_CHx_SWHSSRCREG_SWHS_SGLREQ_SRC          DMA_CHx_SWHSSRCREG_SWHS_SGLREQ_SRC_Msk                        /*!< Software Handshake Single Request for Channel Source. */
#define DMA_CHx_SWHSSRCREG_SWHS_SGLREQ_SRC_WE_Pos   (3)
#define DMA_CHx_SWHSSRCREG_SWHS_SGLREQ_SRC_WE_Msk   (0x01UL << DMA_CHx_SWHSSRCREG_SWHS_SGLREQ_SRC_WE_Pos)         /*!< 0x00000000 00000008 */
#define DMA_CHx_SWHSSRCREG_SWHS_SGLREQ_SRC_WE       DMA_CHx_SWHSSRCREG_SWHS_SGLREQ_SRC_WE_Msk                     /*!< Write Enable bit for Software Handshake Single Request for Channel Source. */
#define DMA_CHx_SWHSSRCREG_SWHS_LST_SRC_Pos         (4)
#define DMA_CHx_SWHSSRCREG_SWHS_LST_SRC_Msk         (0x01UL << DMA_CHx_SWHSSRCREG_SWHS_LST_SRC_Pos)               /*!< 0x00000000 00000010 */
#define DMA_CHx_SWHSSRCREG_SWHS_LST_SRC             DMA_CHx_SWHSSRCREG_SWHS_LST_SRC_Msk                           /*!< Software Handshake Last Request for Channel Source. */
#define DMA_CHx_SWHSSRCREG_SWHS_LST_SRC_WE_Pos      (5)
#define DMA_CHx_SWHSSRCREG_SWHS_LST_SRC_WE_Msk      (0x01UL << DMA_CHx_SWHSSRCREG_SWHS_LST_SRC_WE_Pos)            /*!< 0x00000000 00000020 */
#define DMA_CHx_SWHSSRCREG_SWHS_LST_SRC_WE          DMA_CHx_SWHSSRCREG_SWHS_LST_SRC_WE_Msk                        /*!< Write Enable bit for Software Handshake Last Request for Channel Source. */

/******************  Bit definition for CHx_SWHSDSTREG register  ******************/
#define DMA_CHx_SWHSDSTREG_SWHS_REQ_DST_Pos         (0)
#define DMA_CHx_SWHSDSTREG_SWHS_REQ_DST_Msk         (0x01UL << DMA_CHx_SWHSDSTREG_SWHS_REQ_DST_Pos)               /*!< 0x00000000 00000001 */
#define DMA_CHx_SWHSDSTREG_SWHS_REQ_DST             DMA_CHx_SWHSDSTREG_SWHS_REQ_DST_Msk                           /*!< Software Handshake Request for Channel Destination. */
#define DMA_CHx_SWHSDSTREG_SWHS_REQ_DST_WE_Pos      (1)
#define DMA_CHx_SWHSDSTREG_SWHS_REQ_DST_WE_Msk      (0x01UL << DMA_CHx_SWHSDSTREG_SWHS_REQ_DST_WE_Pos)            /*!< 0x00000000 00000002 */
#define DMA_CHx_SWHSDSTREG_SWHS_REQ_DST_WE          DMA_CHx_SWHSDSTREG_SWHS_REQ_DST_WE_Msk                        /*!< Write Enable bit for Software Handshake Request for Channel Destination. */
#define DMA_CHx_SWHSDSTREG_SWHS_SGLREQ_DST_Pos      (2)
#define DMA_CHx_SWHSDSTREG_SWHS_SGLREQ_DST_Msk      (0x01UL << DMA_CHx_SWHSDSTREG_SWHS_SGLREQ_DST_Pos)            /*!< 0x00000000 00000004 */
#define DMA_CHx_SWHSDSTREG_SWHS_SGLREQ_DST          DMA_CHx_SWHSDSTREG_SWHS_SGLREQ_DST_Msk                        /*!< Software Handshake Single Request for Channel Destination. */
#define DMA_CHx_SWHSDSTREG_SWHS_SGLREQ_DST_WE_Pos   (3)
#define DMA_CHx_SWHSDSTREG_SWHS_SGLREQ_DST_WE_Msk   (0x01UL << DMA_CHx_SWHSDSTREG_SWHS_SGLREQ_DST_WE_Pos)         /*!< 0x00000000 00000008 */
#define DMA_CHx_SWHSDSTREG_SWHS_SGLREQ_DST_WE       DMA_CHx_SWHSDSTREG_SWHS_SGLREQ_DST_WE_Msk                     /*!< Write Enable bit for Software Handshake Single Request for Channel Destination. */
#define DMA_CHx_SWHSDSTREG_SWHS_LST_DST_Pos         (4)
#define DMA_CHx_SWHSDSTREG_SWHS_LST_DST_Msk         (0x01UL << DMA_CHx_SWHSDSTREG_SWHS_LST_DST_Pos)               /*!< 0x00000000 00000010 */
#define DMA_CHx_SWHSDSTREG_SWHS_LST_DST             DMA_CHx_SWHSDSTREG_SWHS_LST_DST_Msk                           /*!< Software Handshake Last Request for Channel Destination. */
#define DMA_CHx_SWHSDSTREG_SWHS_LST_DST_WE_Pos      (5)
#define DMA_CHx_SWHSDSTREG_SWHS_LST_DST_WE_Msk      (0x01UL << DMA_CHx_SWHSDSTREG_SWHS_LST_DST_WE_Pos)            /*!< 0x00000000 00000020 */
#define DMA_CHx_SWHSDSTREG_SWHS_LST_DST_WE          DMA_CHx_SWHSDSTREG_SWHS_LST_DST_WE_Msk                        /*!< Write Enable bit for Software Handshake Last Request for Channel Destination. */

/******************  Bit definition for CHx_BLK_TFR_RESUMEREQREG register  ******************/
#define DMA_CHx_BLK_TFR_RESUMEREQREG_BLK_TFR_RESUMEREQ_Pos         (0)
#define DMA_CHx_BLK_TFR_RESUMEREQREG_BLK_TFR_RESUMEREQ_Msk         (0x01UL << DMA_CHx_BLK_TFR_RESUMEREQREG_BLK_TFR_RESUMEREQ_Pos)               /*!< 0x00000000 00000001 */
#define DMA_CHx_BLK_TFR_RESUMEREQREG_BLK_TFR_RESUMEREQ             DMA_CHx_BLK_TFR_RESUMEREQREG_BLK_TFR_RESUMEREQ_Msk                           /*!< Block Transfer Resume Request during Linked-List or Shadow-Register-based multi-block transfer. */
/******************  Bit definition for CHx_AXI_QOSREG register  ******************/
#define DMA_CHx_AXI_QOSREG_AXI_AWQOS_Pos            (0)
#define DMA_CHx_AXI_QOSREG_AXI_AWQOS_Msk            (0x0FUL << DMA_CHx_AXI_QOSREG_AXI_AWQOS_Pos)                 /*!< 0x00000000 0000000F */
#define DMA_CHx_AXI_QOSREG_AXI_AWQOS                DMA_CHx_AXI_QOSREG_AXI_AWQOS_Msk                             /*!< AXI AWQOS. */
#define DMA_CHx_AXI_QOSREG_AXI_ARQOS_Pos            (4)
#define DMA_CHx_AXI_QOSREG_AXI_ARQOS_Msk            (0x0FUL << DMA_CHx_AXI_QOSREG_AXI_ARQOS_Pos)                 /*!< 0x00000000 000000F0 */
#define DMA_CHx_AXI_QOSREG_AXI_ARQOS                DMA_CHx_AXI_QOSREG_AXI_ARQOS_Msk                             /*!< AXI AWQOS. */
/******************  Bit definition for CHx_SSTAT register  ******************/
#define DMA_CHx_SSTAT_SSTAT_Pos                     (0)
#define DMA_CHx_SSTAT_SSTAT_Msk                     (0xFFFFFFFFUL << DMA_CHx_SSTAT_SSTAT_Pos)                    /*!< 0x00000000 FFFFFFFF */
#define DMA_CHx_SSTAT_SSTAT                         DMA_CHx_SSTAT_SSTAT_Msk                                      /*!< Source Status. */
/******************  Bit definition for CHx_DSTAT register  ******************/
#define DMA_CHx_DSTAT_DSTAT_Pos                     (0)
#define DMA_CHx_DSTAT_DSTAT_Msk                     (0xFFFFFFFFUL << DMA_CHx_DSTAT_DSTAT_Pos)                    /*!< 0x00000000 FFFFFFFF */
#define DMA_CHx_DSTAT_DSTAT                         DMA_CHx_DSTAT_DSTAT_Msk                                      /*!< Destination Status. */

/******************  Bit definition for CHx_INTSTATUS_ENABLEREG register  ******************/
#define DMA_CHx_INTSTATUS_EN_BLOCK_TFR_DONE_IntStat_Pos       (0)
#define DMA_CHx_INTSTATUS_EN_BLOCK_TFR_DONE_IntStat_Msk       (0x01UL << DMA_CHx_INTSTATUS_EN_BLOCK_TFR_DONE_IntStat_Pos)     /*!< 0x00000000 00000001 */
#define DMA_CHx_INTSTATUS_EN_BLOCK_TFR_DONE_IntStat           DMA_CHx_INTSTATUS_EN_BLOCK_TFR_DONE_IntStat_Msk                 /*!< Block Transfer Done Interrupt Status Enable. */
#define DMA_CHx_INTSTATUS_EN_DMA_TFR_DONE_IntStat_Pos         (1)
#define DMA_CHx_INTSTATUS_EN_DMA_TFR_DONE_IntStat_Msk         (0x01UL << DMA_CHx_INTSTATUS_EN_DMA_TFR_DONE_IntStat_Pos)       /*!< 0x00000000 00000002 */
#define DMA_CHx_INTSTATUS_EN_DMA_TFR_DONE_IntStat             DMA_CHx_INTSTATUS_EN_DMA_TFR_DONE_IntStat_Msk                   /*!< DMA Transfer Done Interrupt Status Enable. */
#define DMA_CHx_INTSTATUS_EN_SRC_TRANSCOMP_IntStat_Pos        (3)
#define DMA_CHx_INTSTATUS_EN_SRC_TRANSCOMP_IntStat_Msk        (0x01UL << DMA_CHx_INTSTATUS_EN_SRC_TRANSCOMP_IntStat_Pos)      /*!< 0x00000000 00000008 */
#define DMA_CHx_INTSTATUS_EN_SRC_TRANSCOMP_IntStat            DMA_CHx_INTSTATUS_EN_SRC_TRANSCOMP_IntStat_Msk                  /*!< Source Transaction Completed Status Enable. */
#define DMA_CHx_INTSTATUS_EN_DST_TRANSCOMP_IntStat_Pos        (4)
#define DMA_CHx_INTSTATUS_EN_DST_TRANSCOMP_IntStat_Msk        (0x01UL << DMA_CHx_INTSTATUS_EN_DST_TRANSCOMP_IntStat_Pos)      /*!< 0x00000000 00000010 */
#define DMA_CHx_INTSTATUS_EN_DST_TRANSCOMP_IntStat            DMA_CHx_INTSTATUS_EN_DST_TRANSCOMP_IntStat_Msk                  /*!< Destination Transaction Completed Status Enable. */
#define DMA_CHx_INTSTATUS_EN_SRC_DEC_ERR_IntStat_Pos          (5)
#define DMA_CHx_INTSTATUS_EN_SRC_DEC_ERR_IntStat_Msk          (0x01UL << DMA_CHx_INTSTATUS_EN_SRC_DEC_ERR_IntStat_Pos)        /*!< 0x00000000 00000020 */
#define DMA_CHx_INTSTATUS_EN_SRC_DEC_ERR_IntStat              DMA_CHx_INTSTATUS_EN_SRC_DEC_ERR_IntStat_Msk                    /*!< Source Decode Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_DST_DEC_ERR_IntStat_Pos          (6)
#define DMA_CHx_INTSTATUS_EN_DST_DEC_ERR_IntStat_Msk          (0x01UL << DMA_CHx_INTSTATUS_EN_DST_DEC_ERR_IntStat_Pos)        /*!< 0x00000000 00000040 */
#define DMA_CHx_INTSTATUS_EN_DST_DEC_ERR_IntStat              DMA_CHx_INTSTATUS_EN_DST_DEC_ERR_IntStat_Msk                    /*!< Destination Decode Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_SRC_SLV_ERR_IntStat_Pos          (7)
#define DMA_CHx_INTSTATUS_EN_SRC_SLV_ERR_IntStat_Msk          (0x01UL << DMA_CHx_INTSTATUS_EN_SRC_SLV_ERR_IntStat_Pos)        /*!< 0x00000000 00000080 */
#define DMA_CHx_INTSTATUS_EN_SRC_SLV_ERR_IntStat              DMA_CHx_INTSTATUS_EN_SRC_SLV_ERR_IntStat_Msk                    /*!< Source Slave Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_DST_SLV_ERR_IntStat_Pos          (8)
#define DMA_CHx_INTSTATUS_EN_DST_SLV_ERR_IntStat_Msk          (0x01UL << DMA_CHx_INTSTATUS_EN_DST_SLV_ERR_IntStat_Pos)        /*!< 0x00000000 00000100 */
#define DMA_CHx_INTSTATUS_EN_DST_SLV_ERR_IntStat              DMA_CHx_INTSTATUS_EN_DST_SLV_ERR_IntStat_Msk                    /*!< Destination Slave Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_LLI_RD_DEC_ERR_IntStat_Pos       (9)
#define DMA_CHx_INTSTATUS_EN_LLI_RD_DEC_ERR_IntStat_Msk       (0x01UL << DMA_CHx_INTSTATUS_EN_LLI_RD_DEC_ERR_IntStat_Pos)     /*!< 0x00000000 00000200 */
#define DMA_CHx_INTSTATUS_EN_LLI_RD_DEC_ERR_IntStat           DMA_CHx_INTSTATUS_EN_LLI_RD_DEC_ERR_IntStat_Msk                 /*!< LLI Read Decode Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_LLI_WR_DEC_ERR_IntStat_Pos       (10)
#define DMA_CHx_INTSTATUS_EN_LLI_WR_DEC_ERR_IntStat_Msk       (0x01UL << DMA_CHx_INTSTATUS_EN_LLI_WR_DEC_ERR_IntStat_Pos)     /*!< 0x00000000 00000400 */
#define DMA_CHx_INTSTATUS_EN_LLI_WR_DEC_ERR_IntStat           DMA_CHx_INTSTATUS_EN_LLI_WR_DEC_ERR_IntStat_Msk                 /*!< LLI WRITE Decode Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_LLI_RD_SLV_ERR_IntStat_Pos       (11)
#define DMA_CHx_INTSTATUS_EN_LLI_RD_SLV_ERR_IntStat_Msk       (0x01UL << DMA_CHx_INTSTATUS_EN_LLI_RD_SLV_ERR_IntStat_Pos)     /*!< 0x00000000 00000800 */
#define DMA_CHx_INTSTATUS_EN_LLI_RD_SLV_ERR_IntStat           DMA_CHx_INTSTATUS_EN_LLI_RD_SLV_ERR_IntStat_Msk                 /*!< LLI Read Slave Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_LLI_WR_SLV_ERR_IntStat_Pos       (12)
#define DMA_CHx_INTSTATUS_EN_LLI_WR_SLV_ERR_IntStat_Msk       (0x01UL << DMA_CHx_INTSTATUS_EN_LLI_WR_SLV_ERR_IntStat_Pos)     /*!< 0x00000000 00001000 */
#define DMA_CHx_INTSTATUS_EN_LLI_WR_SLV_ERR_IntStat           DMA_CHx_INTSTATUS_EN_LLI_WR_SLV_ERR_IntStat_Msk                 /*!< LLI WRITE Slave Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_SHADOWREG_OR_LLI_INVALID_ERR_IntStat_Pos  (13)
#define DMA_CHx_INTSTATUS_EN_SHADOWREG_OR_LLI_INVALID_ERR_IntStat_Msk  (0x01UL << DMA_CHx_INTSTATUS_EN_SHADOWREG_OR_LLI_INVALID_ERR_IntStat_Pos)  /*!< 0x00000000 00002000 */
#define DMA_CHx_INTSTATUS_EN_SHADOWREG_OR_LLI_INVALID_ERR_IntStat      DMA_CHx_INTSTATUS_EN_SHADOWREG_OR_LLI_INVALID_ERR_IntStat_Msk              /*!< Shadow register or LLI Invalid Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_SLVIF_MULTIBLKTYPE_ERR_IntStat_Pos        (14)
#define DMA_CHx_INTSTATUS_EN_SLVIF_MULTIBLKTYPE_ERR_IntStat_Msk        (0x01UL << DMA_CHx_INTSTATUS_EN_SLVIF_MULTIBLKTYPE_ERR_IntStat_Pos)        /*!< 0x00000000 00004000 */
#define DMA_CHx_INTSTATUS_EN_SLVIF_MULTIBLKTYPE_ERR_IntStat            DMA_CHx_INTSTATUS_EN_SLVIF_MULTIBLKTYPE_ERR_IntStat_Msk                    /*!< Slave Interface Multi Block type Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_SLVIF_DEC_ERR_IntStat_Pos        (16)
#define DMA_CHx_INTSTATUS_EN_SLVIF_DEC_ERR_IntStat_Msk        (0x01UL << DMA_CHx_INTSTATUS_EN_SLVIF_DEC_ERR_IntStat_Pos)        /*!< 0x00000000 00010000 */
#define DMA_CHx_INTSTATUS_EN_SLVIF_DEC_ERR_IntStat            DMA_CHx_INTSTATUS_EN_SLVIF_DEC_ERR_IntStat_Msk                    /*!< Slave Interface Decode Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_SLVIF_WR2RO_ERR_IntStat_Pos      (17)
#define DMA_CHx_INTSTATUS_EN_SLVIF_WR2RO_ERR_IntStat_Msk      (0x01UL << DMA_CHx_INTSTATUS_EN_SLVIF_WR2RO_ERR_IntStat_Pos)      /*!< 0x00000000 00020000 */
#define DMA_CHx_INTSTATUS_EN_SLVIF_WR2RO_ERR_IntStat          DMA_CHx_INTSTATUS_EN_SLVIF_WR2RO_ERR_IntStat_Msk                  /*!< Slave Interface Write to Read Only Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_SLVIF_RD2RWO_ERR_IntStat_Pos     (18)
#define DMA_CHx_INTSTATUS_EN_SLVIF_RD2RWO_ERR_IntStat_Msk     (0x01UL << DMA_CHx_INTSTATUS_EN_SLVIF_RD2RWO_ERR_IntStat_Pos)     /*!< 0x00000000 00040000 */
#define DMA_CHx_INTSTATUS_EN_SLVIF_RD2RWO_ERR_IntStat         DMA_CHx_INTSTATUS_EN_SLVIF_RD2RWO_ERR_IntStat_Msk                 /*!< Slave Interface Read to write Only Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_SLVIF_WRONCHEN_ERR_IntStat_Pos   (19)
#define DMA_CHx_INTSTATUS_EN_SLVIF_WRONCHEN_ERR_IntStat_Msk   (0x01UL << DMA_CHx_INTSTATUS_EN_SLVIF_WRONCHEN_ERR_IntStat_Pos)   /*!< 0x00000000 00080000 */
#define DMA_CHx_INTSTATUS_EN_SLVIF_WRONCHEN_ERR_IntStat       DMA_CHx_INTSTATUS_EN_SLVIF_WRONCHEN_ERR_IntStat_Msk               /*!< Slave Interface Write On Channel Enabled Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat_Pos   (20)
#define DMA_CHx_INTSTATUS_EN_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat_Msk   (0x01UL << DMA_CHx_INTSTATUS_EN_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat_Pos)   /*!< 0x00000000 00100000 */
#define DMA_CHx_INTSTATUS_EN_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat       DMA_CHx_INTSTATUS_EN_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat_Msk               /*!< Shadow Register Write On Valid Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_SLVIF_WRONHOLD_ERR_IntStat_Pos   (21)
#define DMA_CHx_INTSTATUS_EN_SLVIF_WRONHOLD_ERR_IntStat_Msk   (0x01UL << DMA_CHx_INTSTATUS_EN_SLVIF_WRONHOLD_ERR_IntStat_Pos)   /*!< 0x00000000 00200000 */
#define DMA_CHx_INTSTATUS_EN_SLVIF_WRONHOLD_ERR_IntStat       DMA_CHx_INTSTATUS_EN_SLVIF_WRONHOLD_ERR_IntStat_Msk               /*!< Slave Interface Write On Hold Error Status Enable. */
#define DMA_CHx_INTSTATUS_EN_CH_LOCK_CLEARED_IntStat_Pos      (27)
#define DMA_CHx_INTSTATUS_EN_CH_LOCK_CLEARED_IntStat_Msk      (0x01UL << DMA_CHx_INTSTATUS_EN_CH_LOCK_CLEARED_IntStat_Pos)      /*!< 0x00000000 08000000 */
#define DMA_CHx_INTSTATUS_EN_CH_LOCK_CLEARED_IntStat          DMA_CHx_INTSTATUS_EN_CH_LOCK_CLEARED_IntStat_Msk                  /*!< Channel Lock Cleared Status Enable. */
#define DMA_CHx_INTSTATUS_EN_CH_SRC_SUSPENDED_IntStat_Pos     (28)
#define DMA_CHx_INTSTATUS_EN_CH_SRC_SUSPENDED_IntStat_Msk     (0x01UL << DMA_CHx_INTSTATUS_EN_CH_SRC_SUSPENDED_IntStat_Pos)     /*!< 0x00000000 10000000 */
#define DMA_CHx_INTSTATUS_EN_CH_SRC_SUSPENDED_IntStat         DMA_CHx_INTSTATUS_EN_CH_SRC_SUSPENDED_IntStat_Msk                 /*!< Channel Source Suspended Status Enable. */
#define DMA_CHx_INTSTATUS_EN_CH_SUSPENDED_IntStat_Pos         (29)
#define DMA_CHx_INTSTATUS_EN_CH_SUSPENDED_IntStat_Msk         (0x01UL << DMA_CHx_INTSTATUS_EN_CH_SUSPENDED_IntStat_Pos)         /*!< 0x00000000 20000000 */
#define DMA_CHx_INTSTATUS_EN_CH_SUSPENDED_IntStat             DMA_CHx_INTSTATUS_EN_CH_SUSPENDED_IntStat_Msk                     /*!< Channel Suspended Status Enable. */
#define DMA_CHx_INTSTATUS_EN_CH_DISABLED_IntStat_Pos          (30)
#define DMA_CHx_INTSTATUS_EN_CH_DISABLED_IntStat_Msk          (0x01UL << DMA_CHx_INTSTATUS_EN_CH_DISABLED_IntStat_Pos)          /*!< 0x00000000 40000000 */
#define DMA_CHx_INTSTATUS_EN_CH_DISABLED_IntStat              DMA_CHx_INTSTATUS_EN_CH_DISABLED_IntStat_Msk                      /*!< Channel Disabled Status Enable. */
#define DMA_CHx_INTSTATUS_EN_CH_ABORTED_IntStat_Pos           (31)
#define DMA_CHx_INTSTATUS_EN_CH_ABORTED_IntStat_Msk           (0x01UL << DMA_CHx_INTSTATUS_EN_CH_ABORTED_IntStat_Pos)           /*!< 0x00000000 80000000 */
#define DMA_CHx_INTSTATUS_EN_CH_ABORTED_IntStat               DMA_CHx_INTSTATUS_EN_CH_ABORTED_IntStat_Msk                       /*!< Channel Aborted Status Enable. */

/******************  Bit definition for CHx_INTSTATUS register  ******************/
#define DMA_CHx_INT_BLOCK_TFR_DONE_IntStat_Pos       (0)
#define DMA_CHx_INT_BLOCK_TFR_DONE_IntStat_Msk       (0x01UL << DMA_CHx_INT_BLOCK_TFR_DONE_IntStat_Pos)     /*!< 0x00000000 00000001 */
#define DMA_CHx_INT_BLOCK_TFR_DONE_IntStat           DMA_CHx_INT_BLOCK_TFR_DONE_IntStat_Msk                 /*!< Block Transfer Done Interrupt. */
#define DMA_CHx_INT_DMA_TFR_DONE_IntStat_Pos         (1)
#define DMA_CHx_INT_DMA_TFR_DONE_IntStat_Msk         (0x01UL << DMA_CHx_INT_DMA_TFR_DONE_IntStat_Pos)       /*!< 0x00000000 00000002 */
#define DMA_CHx_INT_DMA_TFR_DONE_IntStat             DMA_CHx_INT_DMA_TFR_DONE_IntStat_Msk                   /*!< DMA Transfer Done Interrupt. */
#define DMA_CHx_INT_SRC_TRANSCOMP_IntStat_Pos        (3)
#define DMA_CHx_INT_SRC_TRANSCOMP_IntStat_Msk        (0x01UL << DMA_CHx_INT_SRC_TRANSCOMP_IntStat_Pos)      /*!< 0x00000000 00000008 */
#define DMA_CHx_INT_SRC_TRANSCOMP_IntStat            DMA_CHx_INT_SRC_TRANSCOMP_IntStat_Msk                  /*!< Source Transaction Completed. */
#define DMA_CHx_INT_DST_TRANSCOMP_IntStat_Pos        (4)
#define DMA_CHx_INT_DST_TRANSCOMP_IntStat_Msk        (0x01UL << DMA_CHx_INT_DST_TRANSCOMP_IntStat_Pos)      /*!< 0x00000000 00000010 */
#define DMA_CHx_INT_DST_TRANSCOMP_IntStat            DMA_CHx_INT_DST_TRANSCOMP_IntStat_Msk                  /*!< Destination Transaction Completed. */
#define DMA_CHx_INT_SRC_DEC_ERR_IntStat_Pos          (5)
#define DMA_CHx_INT_SRC_DEC_ERR_IntStat_Msk          (0x01UL << DMA_CHx_INT_SRC_DEC_ERR_IntStat_Pos)        /*!< 0x00000000 00000020 */
#define DMA_CHx_INT_SRC_DEC_ERR_IntStat              DMA_CHx_INT_SRC_DEC_ERR_IntStat_Msk                    /*!< Source Decode Error. */
#define DMA_CHx_INT_DST_DEC_ERR_IntStat_Pos          (6)
#define DMA_CHx_INT_DST_DEC_ERR_IntStat_Msk          (0x01UL << DMA_CHx_INT_DST_DEC_ERR_IntStat_Pos)        /*!< 0x00000000 00000040 */
#define DMA_CHx_INT_DST_DEC_ERR_IntStat              DMA_CHx_INT_DST_DEC_ERR_IntStat_Msk                    /*!< Destination Decode Error. */
#define DMA_CHx_INT_SRC_SLV_ERR_IntStat_Pos          (7)
#define DMA_CHx_INT_SRC_SLV_ERR_IntStat_Msk          (0x01UL << DMA_CHx_INT_SRC_SLV_ERR_IntStat_Pos)        /*!< 0x00000000 00000080 */
#define DMA_CHx_INT_SRC_SLV_ERR_IntStat              DMA_CHx_INT_SRC_SLV_ERR_IntStat_Msk                    /*!< Source Slave Error. */
#define DMA_CHx_INT_DST_SLV_ERR_IntStat_Pos          (8)
#define DMA_CHx_INT_DST_SLV_ERR_IntStat_Msk          (0x01UL << DMA_CHx_INT_DST_SLV_ERR_IntStat_Pos)        /*!< 0x00000000 00000100 */
#define DMA_CHx_INT_DST_SLV_ERR_IntStat              DMA_CHx_INT_DST_SLV_ERR_IntStat_Msk                    /*!< Destination Slave Error. */
#define DMA_CHx_INT_LLI_RD_DEC_ERR_IntStat_Pos       (9)
#define DMA_CHx_INT_LLI_RD_DEC_ERR_IntStat_Msk       (0x01UL << DMA_CHx_INT_LLI_RD_DEC_ERR_IntStat_Pos)     /*!< 0x00000000 00000200 */
#define DMA_CHx_INT_LLI_RD_DEC_ERR_IntStat           DMA_CHx_INT_LLI_RD_DEC_ERR_IntStat_Msk                 /*!< LLI Read Decode Error. */
#define DMA_CHx_INT_LLI_WR_DEC_ERR_IntStat_Pos       (10)
#define DMA_CHx_INT_LLI_WR_DEC_ERR_IntStat_Msk       (0x01UL << DMA_CHx_INT_LLI_WR_DEC_ERR_IntStat_Pos)     /*!< 0x00000000 00000400 */
#define DMA_CHx_INT_LLI_WR_DEC_ERR_IntStat           DMA_CHx_INT_LLI_WR_DEC_ERR_IntStat_Msk                 /*!< LLI WRITE Decode Error. */
#define DMA_CHx_INT_LLI_RD_SLV_ERR_IntStat_Pos       (11)
#define DMA_CHx_INT_LLI_RD_SLV_ERR_IntStat_Msk       (0x01UL << DMA_CHx_INT_LLI_RD_SLV_ERR_IntStat_Pos)     /*!< 0x00000000 00000800 */
#define DMA_CHx_INT_LLI_RD_SLV_ERR_IntStat           DMA_CHx_INT_LLI_RD_SLV_ERR_IntStat_Msk                 /*!< LLI Read Slave Error. */
#define DMA_CHx_INT_LLI_WR_SLV_ERR_IntStat_Pos       (12)
#define DMA_CHx_INT_LLI_WR_SLV_ERR_IntStat_Msk       (0x01UL << DMA_CHx_INT_LLI_WR_SLV_ERR_IntStat_Pos)     /*!< 0x00000000 00001000 */
#define DMA_CHx_INT_LLI_WR_SLV_ERR_IntStat           DMA_CHx_INT_LLI_WR_SLV_ERR_IntStat_Msk                 /*!< LLI WRITE Slave Error. */
#define DMA_CHx_INT_SHADOWREG_OR_LLI_INVALID_ERR_IntStat_Pos  (13)
#define DMA_CHx_INT_SHADOWREG_OR_LLI_INVALID_ERR_IntStat_Msk  (0x01UL << DMA_CHx_INT_SHADOWREG_OR_LLI_INVALID_ERR_IntStat_Pos)  /*!< 0x00000000 00002000 */
#define DMA_CHx_INT_SHADOWREG_OR_LLI_INVALID_ERR_IntStat      DMA_CHx_INT_SHADOWREG_OR_LLI_INVALID_ERR_IntStat_Msk              /*!< Shadow register or LLI Invalid Error. */
#define DMA_CHx_INT_SLVIF_MULTIBLKTYPE_ERR_IntStat_Pos        (14)
#define DMA_CHx_INT_SLVIF_MULTIBLKTYPE_ERR_IntStat_Msk        (0x01UL << DMA_CHx_INT_SLVIF_MULTIBLKTYPE_ERR_IntStat_Pos)        /*!< 0x00000000 00004000 */
#define DMA_CHx_INT_SLVIF_MULTIBLKTYPE_ERR_IntStat            DMA_CHx_INT_SLVIF_MULTIBLKTYPE_ERR_IntStat_Msk                    /*!< Slave Interface Multi Block type Error. */
#define DMA_CHx_INT_SLVIF_DEC_ERR_IntStat_Pos        (16)
#define DMA_CHx_INT_SLVIF_DEC_ERR_IntStat_Msk        (0x01UL << DMA_CHx_INT_SLVIF_DEC_ERR_IntStat_Pos)        /*!< 0x00000000 00010000 */
#define DMA_CHx_INT_SLVIF_DEC_ERR_IntStat            DMA_CHx_INT_SLVIF_DEC_ERR_IntStat_Msk                    /*!< Slave Interface Decode Error. */
#define DMA_CHx_INT_SLVIF_WR2RO_ERR_IntStat_Pos      (17)
#define DMA_CHx_INT_SLVIF_WR2RO_ERR_IntStat_Msk      (0x01UL << DMA_CHx_INT_SLVIF_WR2RO_ERR_IntStat_Pos)      /*!< 0x00000000 00020000 */
#define DMA_CHx_INT_SLVIF_WR2RO_ERR_IntStat          DMA_CHx_INT_SLVIF_WR2RO_ERR_IntStat_Msk                  /*!< Slave Interface Write to Read Only Error. */
#define DMA_CHx_INT_SLVIF_RD2RWO_ERR_IntStat_Pos     (18)
#define DMA_CHx_INT_SLVIF_RD2RWO_ERR_IntStat_Msk     (0x01UL << DMA_CHx_INT_SLVIF_RD2RWO_ERR_IntStat_Pos)     /*!< 0x00000000 00040000 */
#define DMA_CHx_INT_SLVIF_RD2RWO_ERR_IntStat         DMA_CHx_INT_SLVIF_RD2RWO_ERR_IntStat_Msk                 /*!< Slave Interface Read to write Only Error. */
#define DMA_CHx_INT_SLVIF_WRONCHEN_ERR_IntStat_Pos   (19)
#define DMA_CHx_INT_SLVIF_WRONCHEN_ERR_IntStat_Msk   (0x01UL << DMA_CHx_INT_SLVIF_WRONCHEN_ERR_IntStat_Pos)   /*!< 0x00000000 00080000 */
#define DMA_CHx_INT_SLVIF_WRONCHEN_ERR_IntStat       DMA_CHx_INT_SLVIF_WRONCHEN_ERR_IntStat_Msk               /*!< Slave Interface Write On Channel Enabled Error. */
#define DMA_CHx_INT_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat_Pos   (20)
#define DMA_CHx_INT_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat_Msk   (0x01UL << DMA_CHx_INT_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat_Pos)   /*!< 0x00000000 00100000 */
#define DMA_CHx_INT_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat       DMA_CHx_INT_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat_Msk               /*!< Shadow Register Write On Valid Error. */
#define DMA_CHx_INT_SLVIF_WRONHOLD_ERR_IntStat_Pos   (21)
#define DMA_CHx_INT_SLVIF_WRONHOLD_ERR_IntStat_Msk   (0x01UL << DMA_CHx_INT_SLVIF_WRONHOLD_ERR_IntStat_Pos)   /*!< 0x00000000 00200000 */
#define DMA_CHx_INT_SLVIF_WRONHOLD_ERR_IntStat       DMA_CHx_INT_SLVIF_WRONHOLD_ERR_IntStat_Msk               /*!< Slave Interface Write On Hold Error. */
#define DMA_CHx_INT_CH_LOCK_CLEARED_IntStat_Pos      (27)
#define DMA_CHx_INT_CH_LOCK_CLEARED_IntStat_Msk      (0x01UL << DMA_CHx_INT_CH_LOCK_CLEARED_IntStat_Pos)      /*!< 0x00000000 08000000 */
#define DMA_CHx_INT_CH_LOCK_CLEARED_IntStat          DMA_CHx_INT_CH_LOCK_CLEARED_IntStat_Msk                  /*!< Channel Lock Cleared. */
#define DMA_CHx_INT_CH_SRC_SUSPENDED_IntStat_Pos     (28)
#define DMA_CHx_INT_CH_SRC_SUSPENDED_IntStat_Msk     (0x01UL << DMA_CHx_INT_CH_SRC_SUSPENDED_IntStat_Pos)     /*!< 0x00000000 10000000 */
#define DMA_CHx_INT_CH_SRC_SUSPENDED_IntStat         DMA_CHx_INT_CH_SRC_SUSPENDED_IntStat_Msk                 /*!< Channel Source Suspended. */
#define DMA_CHx_INT_CH_SUSPENDED_IntStat_Pos         (29)
#define DMA_CHx_INT_CH_SUSPENDED_IntStat_Msk         (0x01UL << DMA_CHx_INT_CH_SUSPENDED_IntStat_Pos)         /*!< 0x00000000 20000000 */
#define DMA_CHx_INT_CH_SUSPENDED_IntStat             DMA_CHx_INT_CH_SUSPENDED_IntStat_Msk                     /*!< Channel Suspended. */
#define DMA_CHx_INT_CH_DISABLED_IntStat_Pos          (30)
#define DMA_CHx_INT_CH_DISABLED_IntStat_Msk          (0x01UL << DMA_CHx_INT_CH_DISABLED_IntStat_Pos)          /*!< 0x00000000 40000000 */
#define DMA_CHx_INT_CH_DISABLED_IntStat              DMA_CHx_INT_CH_DISABLED_IntStat_Msk                      /*!< Channel Disabled. */
#define DMA_CHx_INT_CH_ABORTED_IntStat_Pos           (31)
#define DMA_CHx_INT_CH_ABORTED_IntStat_Msk           (0x01UL << DMA_CHx_INT_CH_ABORTED_IntStat_Pos)           /*!< 0x00000000 80000000 */
#define DMA_CHx_INT_CH_ABORTED_IntStat               DMA_CHx_INT_CH_ABORTED_IntStat_Msk                       /*!< Channel Aborted . */

/******************  Bit definition for CHx_INTSIGNAL_ENABLEREG register  ******************/
#define DMA_CHx_INTSIGNAL_EN_BLOCK_TFR_DONE_IntSignal_Pos       (0)
#define DMA_CHx_INTSIGNAL_EN_BLOCK_TFR_DONE_IntSignal_Msk       (0x01UL << DMA_CHx_INTSIGNAL_EN_BLOCK_TFR_DONE_IntSignal_Pos)     /*!< 0x00000000 00000001 */
#define DMA_CHx_INTSIGNAL_EN_BLOCK_TFR_DONE_IntSignal           DMA_CHx_INTSIGNAL_EN_BLOCK_TFR_DONE_IntSignal_Msk                 /*!< Block Transfer Done Interrupt Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_DMA_TFR_DONE_IntSignal_Pos         (1)
#define DMA_CHx_INTSIGNAL_EN_DMA_TFR_DONE_IntSignal_Msk         (0x01UL << DMA_CHx_INTSIGNAL_EN_DMA_TFR_DONE_IntSignal_Pos)       /*!< 0x00000000 00000002 */
#define DMA_CHx_INTSIGNAL_EN_DMA_TFR_DONE_IntSignal             DMA_CHx_INTSIGNAL_EN_DMA_TFR_DONE_IntSignal_Msk                   /*!< DMA Transfer Done Interrupt Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_SRC_TRANSCOMP_IntSignal_Pos        (3)
#define DMA_CHx_INTSIGNAL_EN_SRC_TRANSCOMP_IntSignal_Msk        (0x01UL << DMA_CHx_INTSIGNAL_EN_SRC_TRANSCOMP_IntSignal_Pos)      /*!< 0x00000000 00000008 */
#define DMA_CHx_INTSIGNAL_EN_SRC_TRANSCOMP_IntSignal            DMA_CHx_INTSIGNAL_EN_SRC_TRANSCOMP_IntSignal_Msk                  /*!< Source Transaction Completed Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_DST_TRANSCOMP_IntSignal_Pos        (4)
#define DMA_CHx_INTSIGNAL_EN_DST_TRANSCOMP_IntSignal_Msk        (0x01UL << DMA_CHx_INTSIGNAL_EN_DST_TRANSCOMP_IntSignal_Pos)      /*!< 0x00000000 00000010 */
#define DMA_CHx_INTSIGNAL_EN_DST_TRANSCOMP_IntSignal            DMA_CHx_INTSIGNAL_EN_DST_TRANSCOMP_IntSignal_Msk                  /*!< Destination Transaction Completed Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_SRC_DEC_ERR_IntSignal_Pos          (5)
#define DMA_CHx_INTSIGNAL_EN_SRC_DEC_ERR_IntSignal_Msk          (0x01UL << DMA_CHx_INTSIGNAL_EN_SRC_DEC_ERR_IntSignal_Pos)        /*!< 0x00000000 00000020 */
#define DMA_CHx_INTSIGNAL_EN_SRC_DEC_ERR_IntSignal              DMA_CHx_INTSIGNAL_EN_SRC_DEC_ERR_IntSignal_Msk                    /*!< Source Decode Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_DST_DEC_ERR_IntSignal_Pos          (6)
#define DMA_CHx_INTSIGNAL_EN_DST_DEC_ERR_IntSignal_Msk          (0x01UL << DMA_CHx_INTSIGNAL_EN_DST_DEC_ERR_IntSignal_Pos)        /*!< 0x00000000 00000040 */
#define DMA_CHx_INTSIGNAL_EN_DST_DEC_ERR_IntSignal              DMA_CHx_INTSIGNAL_EN_DST_DEC_ERR_IntSignal_Msk                    /*!< Destination Decode Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_SRC_SLV_ERR_IntSignal_Pos          (7)
#define DMA_CHx_INTSIGNAL_EN_SRC_SLV_ERR_IntSignal_Msk          (0x01UL << DMA_CHx_INTSIGNAL_EN_SRC_SLV_ERR_IntSignal_Pos)        /*!< 0x00000000 00000080 */
#define DMA_CHx_INTSIGNAL_EN_SRC_SLV_ERR_IntSignal              DMA_CHx_INTSIGNAL_EN_SRC_SLV_ERR_IntSignal_Msk                    /*!< Source Slave Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_DST_SLV_ERR_IntSignal_Pos          (8)
#define DMA_CHx_INTSIGNAL_EN_DST_SLV_ERR_IntSignal_Msk          (0x01UL << DMA_CHx_INTSIGNAL_EN_DST_SLV_ERR_IntSignal_Pos)        /*!< 0x00000000 00000100 */
#define DMA_CHx_INTSIGNAL_EN_DST_SLV_ERR_IntSignal              DMA_CHx_INTSIGNAL_EN_DST_SLV_ERR_IntSignal_Msk                    /*!< Destination Slave Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_LLI_RD_DEC_ERR_IntSignal_Pos       (9)
#define DMA_CHx_INTSIGNAL_EN_LLI_RD_DEC_ERR_IntSignal_Msk       (0x01UL << DMA_CHx_INTSIGNAL_EN_LLI_RD_DEC_ERR_IntSignal_Pos)     /*!< 0x00000000 00000200 */
#define DMA_CHx_INTSIGNAL_EN_LLI_RD_DEC_ERR_IntSignal           DMA_CHx_INTSIGNAL_EN_LLI_RD_DEC_ERR_IntSignal_Msk                 /*!< LLI Read Decode Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_LLI_WR_DEC_ERR_IntSignal_Pos       (10)
#define DMA_CHx_INTSIGNAL_EN_LLI_WR_DEC_ERR_IntSignal_Msk       (0x01UL << DMA_CHx_INTSIGNAL_EN_LLI_WR_DEC_ERR_IntSignal_Pos)     /*!< 0x00000000 00000400 */
#define DMA_CHx_INTSIGNAL_EN_LLI_WR_DEC_ERR_IntSignal           DMA_CHx_INTSIGNAL_EN_LLI_WR_DEC_ERR_IntSignal_Msk                 /*!< LLI WRITE Decode Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_LLI_RD_SLV_ERR_IntSignal_Pos       (11)
#define DMA_CHx_INTSIGNAL_EN_LLI_RD_SLV_ERR_IntSignal_Msk       (0x01UL << DMA_CHx_INTSIGNAL_EN_LLI_RD_SLV_ERR_IntSignal_Pos)     /*!< 0x00000000 00000800 */
#define DMA_CHx_INTSIGNAL_EN_LLI_RD_SLV_ERR_IntSignal           DMA_CHx_INTSIGNAL_EN_LLI_RD_SLV_ERR_IntSignal_Msk                 /*!< LLI Read Slave Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_LLI_WR_SLV_ERR_IntSignal_Pos       (12)
#define DMA_CHx_INTSIGNAL_EN_LLI_WR_SLV_ERR_IntSignal_Msk       (0x01UL << DMA_CHx_INTSIGNAL_EN_LLI_WR_SLV_ERR_IntSignal_Pos)     /*!< 0x00000000 00001000 */
#define DMA_CHx_INTSIGNAL_EN_LLI_WR_SLV_ERR_IntSignal           DMA_CHx_INTSIGNAL_EN_LLI_WR_SLV_ERR_IntSignal_Msk                 /*!< LLI WRITE Slave Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_SHADOWREG_OR_LLI_INVALID_ERR_IntSignal_Pos  (13)
#define DMA_CHx_INTSIGNAL_EN_SHADOWREG_OR_LLI_INVALID_ERR_IntSignal_Msk  (0x01UL << DMA_CHx_INTSIGNAL_EN_SHADOWREG_OR_LLI_INVALID_ERR_IntSignal_Pos)  /*!< 0x00000000 00002000 */
#define DMA_CHx_INTSIGNAL_EN_SHADOWREG_OR_LLI_INVALID_ERR_IntSignal      DMA_CHx_INTSIGNAL_EN_SHADOWREG_OR_LLI_INVALID_ERR_IntSignal_Msk              /*!< Shadow register or LLI Invalid Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_MULTIBLKTYPE_ERR_IntSignal_Pos        (14)
#define DMA_CHx_INTSIGNAL_EN_SLVIF_MULTIBLKTYPE_ERR_IntSignal_Msk        (0x01UL << DMA_CHx_INTSIGNAL_EN_SLVIF_MULTIBLKTYPE_ERR_IntSignal_Pos)        /*!< 0x00000000 00004000 */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_MULTIBLKTYPE_ERR_IntSignal            DMA_CHx_INTSIGNAL_EN_SLVIF_MULTIBLKTYPE_ERR_IntSignal_Msk                    /*!< Slave Interface Multi Block type Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_DEC_ERR_IntSignal_Pos        (16)
#define DMA_CHx_INTSIGNAL_EN_SLVIF_DEC_ERR_IntSignal_Msk        (0x01UL << DMA_CHx_INTSIGNAL_EN_SLVIF_DEC_ERR_IntSignal_Pos)        /*!< 0x00000000 00010000 */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_DEC_ERR_IntSignal            DMA_CHx_INTSIGNAL_EN_SLVIF_DEC_ERR_IntSignal_Msk                    /*!< Slave Interface Decode Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_WR2RO_ERR_IntSignal_Pos      (17)
#define DMA_CHx_INTSIGNAL_EN_SLVIF_WR2RO_ERR_IntSignal_Msk      (0x01UL << DMA_CHx_INTSIGNAL_EN_SLVIF_WR2RO_ERR_IntSignal_Pos)      /*!< 0x00000000 00020000 */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_WR2RO_ERR_IntSignal          DMA_CHx_INTSIGNAL_EN_SLVIF_WR2RO_ERR_IntSignal_Msk                  /*!< Slave Interface Write to Read Only Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_RD2RWO_ERR_IntSignal_Pos     (18)
#define DMA_CHx_INTSIGNAL_EN_SLVIF_RD2RWO_ERR_IntSignal_Msk     (0x01UL << DMA_CHx_INTSIGNAL_EN_SLVIF_RD2RWO_ERR_IntSignal_Pos)     /*!< 0x00000000 00040000 */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_RD2RWO_ERR_IntSignal         DMA_CHx_INTSIGNAL_EN_SLVIF_RD2RWO_ERR_IntSignal_Msk                 /*!< Slave Interface Read to write Only Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_WRONCHEN_ERR_IntSignal_Pos   (19)
#define DMA_CHx_INTSIGNAL_EN_SLVIF_WRONCHEN_ERR_IntSignal_Msk   (0x01UL << DMA_CHx_INTSIGNAL_EN_SLVIF_WRONCHEN_ERR_IntSignal_Pos)   /*!< 0x00000000 00080000 */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_WRONCHEN_ERR_IntSignal       DMA_CHx_INTSIGNAL_EN_SLVIF_WRONCHEN_ERR_IntSignal_Msk               /*!< Slave Interface Write On Channel Enabled Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_SHADOWREG_WRON_VALID_ERR_IntSignal_Pos   (20)
#define DMA_CHx_INTSIGNAL_EN_SLVIF_SHADOWREG_WRON_VALID_ERR_IntSignal_Msk   (0x01UL << DMA_CHx_INTSIGNAL_EN_SLVIF_SHADOWREG_WRON_VALID_ERR_IntSignal_Pos)   /*!< 0x00000000 00100000 */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_SHADOWREG_WRON_VALID_ERR_IntSignal       DMA_CHx_INTSIGNAL_EN_SLVIF_SHADOWREG_WRON_VALID_ERR_IntSignal_Msk               /*!< Shadow Register Write On Valid Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_WRONHOLD_ERR_IntSignal_Pos   (21)
#define DMA_CHx_INTSIGNAL_EN_SLVIF_WRONHOLD_ERR_IntSignal_Msk   (0x01UL << DMA_CHx_INTSIGNAL_EN_SLVIF_WRONHOLD_ERR_IntSignal_Pos)   /*!< 0x00000000 00200000 */
#define DMA_CHx_INTSIGNAL_EN_SLVIF_WRONHOLD_ERR_IntSignal       DMA_CHx_INTSIGNAL_EN_SLVIF_WRONHOLD_ERR_IntSignal_Msk               /*!< Slave Interface Write On Hold Error Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_CH_LOCK_CLEARED_IntSignal_Pos      (27)
#define DMA_CHx_INTSIGNAL_EN_CH_LOCK_CLEARED_IntSignal_Msk      (0x01UL << DMA_CHx_INTSIGNAL_EN_CH_LOCK_CLEARED_IntSignal_Pos)      /*!< 0x00000000 08000000 */
#define DMA_CHx_INTSIGNAL_EN_CH_LOCK_CLEARED_IntSignal          DMA_CHx_INTSIGNAL_EN_CH_LOCK_CLEARED_IntSignal_Msk                  /*!< Channel Lock Cleared Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_CH_SRC_SUSPENDED_IntSignal_Pos     (28)
#define DMA_CHx_INTSIGNAL_EN_CH_SRC_SUSPENDED_IntSignal_Msk     (0x01UL << DMA_CHx_INTSIGNAL_EN_CH_SRC_SUSPENDED_IntSignal_Pos)     /*!< 0x00000000 10000000 */
#define DMA_CHx_INTSIGNAL_EN_CH_SRC_SUSPENDED_IntSignal         DMA_CHx_INTSIGNAL_EN_CH_SRC_SUSPENDED_IntSignal_Msk                 /*!< Channel Source Suspended Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_CH_SUSPENDED_IntSignal_Pos         (29)
#define DMA_CHx_INTSIGNAL_EN_CH_SUSPENDED_IntSignal_Msk         (0x01UL << DMA_CHx_INTSIGNAL_EN_CH_SUSPENDED_IntSignal_Pos)         /*!< 0x00000000 20000000 */
#define DMA_CHx_INTSIGNAL_EN_CH_SUSPENDED_IntSignal             DMA_CHx_INTSIGNAL_EN_CH_SUSPENDED_IntSignal_Msk                     /*!< Channel Suspended Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_CH_DISABLED_IntSignal_Pos          (30)
#define DMA_CHx_INTSIGNAL_EN_CH_DISABLED_IntSignal_Msk          (0x01UL << DMA_CHx_INTSIGNAL_EN_CH_DISABLED_IntSignal_Pos)          /*!< 0x00000000 40000000 */
#define DMA_CHx_INTSIGNAL_EN_CH_DISABLED_IntSignal              DMA_CHx_INTSIGNAL_EN_CH_DISABLED_IntSignal_Msk                      /*!< Channel Disabled Signal  Enable. */
#define DMA_CHx_INTSIGNAL_EN_CH_ABORTED_IntSignal_Pos           (31)
#define DMA_CHx_INTSIGNAL_EN_CH_ABORTED_IntSignal_Msk           (0x01UL << DMA_CHx_INTSIGNAL_EN_CH_ABORTED_IntSignal_Pos)           /*!< 0x00000000 80000000 */
#define DMA_CHx_INTSIGNAL_EN_CH_ABORTED_IntSignal               DMA_CHx_INTSIGNAL_EN_CH_ABORTED_IntSignal_Msk                       /*!< Channel Aborted Signal  Enable. */

/******************  Bit definition for CHx_INTCLEARREG register  ******************/
#define DMA_CHx_INTCLEAR_BLOCK_TFR_DONE_IntStat_Pos       (0)
#define DMA_CHx_INTCLEAR_BLOCK_TFR_DONE_IntStat_Msk       (0x01UL << DMA_CHx_INTCLEAR_BLOCK_TFR_DONE_IntStat_Pos)     /*!< 0x00000000 00000001 */
#define DMA_CHx_INTCLEAR_BLOCK_TFR_DONE_IntStat           DMA_CHx_INTCLEAR_BLOCK_TFR_DONE_IntStat_Msk                 /*!< Block Transfer Done Interrupt Clear Bit. */
#define DMA_CHx_INTCLEAR_DMA_TFR_DONE_IntStat_Pos         (1)
#define DMA_CHx_INTCLEAR_DMA_TFR_DONE_IntStat_Msk         (0x01UL << DMA_CHx_INTCLEAR_DMA_TFR_DONE_IntStat_Pos)       /*!< 0x00000000 00000002 */
#define DMA_CHx_INTCLEAR_DMA_TFR_DONE_IntStat             DMA_CHx_INTCLEAR_DMA_TFR_DONE_IntStat_Msk                   /*!< DMA Transfer Done Interrupt Clear Bit. */
#define DMA_CHx_INTCLEAR_SRC_TRANSCOMP_IntStat_Pos        (3)
#define DMA_CHx_INTCLEAR_SRC_TRANSCOMP_IntStat_Msk        (0x01UL << DMA_CHx_INTCLEAR_SRC_TRANSCOMP_IntStat_Pos)      /*!< 0x00000000 00000008 */
#define DMA_CHx_INTCLEAR_SRC_TRANSCOMP_IntStat            DMA_CHx_INTCLEAR_SRC_TRANSCOMP_IntStat_Msk                  /*!< Source Transaction Completed Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_DST_TRANSCOMP_IntStat_Pos        (4)
#define DMA_CHx_INTCLEAR_DST_TRANSCOMP_IntStat_Msk        (0x01UL << DMA_CHx_INTCLEAR_DST_TRANSCOMP_IntStat_Pos)      /*!< 0x00000000 00000010 */
#define DMA_CHx_INTCLEAR_DST_TRANSCOMP_IntStat            DMA_CHx_INTCLEAR_DST_TRANSCOMP_IntStat_Msk                  /*!< Destination Transaction Completed Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_SRC_DEC_ERR_IntStat_Pos          (5)
#define DMA_CHx_INTCLEAR_SRC_DEC_ERR_IntStat_Msk          (0x01UL << DMA_CHx_INTCLEAR_SRC_DEC_ERR_IntStat_Pos)        /*!< 0x00000000 00000020 */
#define DMA_CHx_INTCLEAR_SRC_DEC_ERR_IntStat              DMA_CHx_INTCLEAR_SRC_DEC_ERR_IntStat_Msk                    /*!< Source Decode Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_DST_DEC_ERR_IntStat_Pos          (6)
#define DMA_CHx_INTCLEAR_DST_DEC_ERR_IntStat_Msk          (0x01UL << DMA_CHx_INTCLEAR_DST_DEC_ERR_IntStat_Pos)        /*!< 0x00000000 00000040 */
#define DMA_CHx_INTCLEAR_DST_DEC_ERR_IntStat              DMA_CHx_INTCLEAR_DST_DEC_ERR_IntStat_Msk                    /*!< Destination Decode Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_SRC_SLV_ERR_IntStat_Pos          (7)
#define DMA_CHx_INTCLEAR_SRC_SLV_ERR_IntStat_Msk          (0x01UL << DMA_CHx_INTCLEAR_SRC_SLV_ERR_IntStat_Pos)        /*!< 0x00000000 00000080 */
#define DMA_CHx_INTCLEAR_SRC_SLV_ERR_IntStat              DMA_CHx_INTCLEAR_SRC_SLV_ERR_IntStat_Msk                    /*!< Source Slave Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_DST_SLV_ERR_IntStat_Pos          (8)
#define DMA_CHx_INTCLEAR_DST_SLV_ERR_IntStat_Msk          (0x01UL << DMA_CHx_INTCLEAR_DST_SLV_ERR_IntStat_Pos)        /*!< 0x00000000 00000100 */
#define DMA_CHx_INTCLEAR_DST_SLV_ERR_IntStat              DMA_CHx_INTCLEAR_DST_SLV_ERR_IntStat_Msk                    /*!< Destination Slave Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_LLI_RD_DEC_ERR_IntStat_Pos       (9)
#define DMA_CHx_INTCLEAR_LLI_RD_DEC_ERR_IntStat_Msk       (0x01UL << DMA_CHx_INTCLEAR_LLI_RD_DEC_ERR_IntStat_Pos)     /*!< 0x00000000 00000200 */
#define DMA_CHx_INTCLEAR_LLI_RD_DEC_ERR_IntStat           DMA_CHx_INTCLEAR_LLI_RD_DEC_ERR_IntStat_Msk                 /*!< LLI Read Decode Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_LLI_WR_DEC_ERR_IntStat_Pos       (10)
#define DMA_CHx_INTCLEAR_LLI_WR_DEC_ERR_IntStat_Msk       (0x01UL << DMA_CHx_INTCLEAR_LLI_WR_DEC_ERR_IntStat_Pos)     /*!< 0x00000000 00000400 */
#define DMA_CHx_INTCLEAR_LLI_WR_DEC_ERR_IntStat           DMA_CHx_INTCLEAR_LLI_WR_DEC_ERR_IntStat_Msk                 /*!< LLI WRITE Decode Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_LLI_RD_SLV_ERR_IntStat_Pos       (11)
#define DMA_CHx_INTCLEAR_LLI_RD_SLV_ERR_IntStat_Msk       (0x01UL << DMA_CHx_INTCLEAR_LLI_RD_SLV_ERR_IntStat_Pos)     /*!< 0x00000000 00000800 */
#define DMA_CHx_INTCLEAR_LLI_RD_SLV_ERR_IntStat           DMA_CHx_INTCLEAR_LLI_RD_SLV_ERR_IntStat_Msk                 /*!< LLI Read Slave Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_LLI_WR_SLV_ERR_IntStat_Pos       (12)
#define DMA_CHx_INTCLEAR_LLI_WR_SLV_ERR_IntStat_Msk       (0x01UL << DMA_CHx_INTCLEAR_LLI_WR_SLV_ERR_IntStat_Pos)     /*!< 0x00000000 00001000 */
#define DMA_CHx_INTCLEAR_LLI_WR_SLV_ERR_IntStat           DMA_CHx_INTCLEAR_LLI_WR_SLV_ERR_IntStat_Msk                 /*!< LLI WRITE Slave Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_SHADOWREG_OR_LLI_INVALID_ERR_IntStat_Pos  (13)
#define DMA_CHx_INTCLEAR_SHADOWREG_OR_LLI_INVALID_ERR_IntStat_Msk  (0x01UL << DMA_CHx_INTCLEAR_SHADOWREG_OR_LLI_INVALID_ERR_IntStat_Pos)  /*!< 0x00000000 00002000 */
#define DMA_CHx_INTCLEAR_SHADOWREG_OR_LLI_INVALID_ERR_IntStat      DMA_CHx_INTCLEAR_SHADOWREG_OR_LLI_INVALID_ERR_IntStat_Msk              /*!< Shadow register or LLI Invalid Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_SLVIF_MULTIBLKTYPE_ERR_IntStat_Pos        (14)
#define DMA_CHx_INTCLEAR_SLVIF_MULTIBLKTYPE_ERR_IntStat_Msk        (0x01UL << DMA_CHx_INTCLEAR_SLVIF_MULTIBLKTYPE_ERR_IntStat_Pos)        /*!< 0x00000000 00004000 */
#define DMA_CHx_INTCLEAR_SLVIF_MULTIBLKTYPE_ERR_IntStat            DMA_CHx_INTCLEAR_SLVIF_MULTIBLKTYPE_ERR_IntStat_Msk                    /*!< Slave Interface Multi Block type Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_SLVIF_DEC_ERR_IntStat_Pos        (16)
#define DMA_CHx_INTCLEAR_SLVIF_DEC_ERR_IntStat_Msk        (0x01UL << DMA_CHx_INTCLEAR_SLVIF_DEC_ERR_IntStat_Pos)        /*!< 0x00000000 00010000 */
#define DMA_CHx_INTCLEAR_SLVIF_DEC_ERR_IntStat            DMA_CHx_INTCLEAR_SLVIF_DEC_ERR_IntStat_Msk                    /*!< Slave Interface Decode Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_SLVIF_WR2RO_ERR_IntStat_Pos      (17)
#define DMA_CHx_INTCLEAR_SLVIF_WR2RO_ERR_IntStat_Msk      (0x01UL << DMA_CHx_INTCLEAR_SLVIF_WR2RO_ERR_IntStat_Pos)      /*!< 0x00000000 00020000 */
#define DMA_CHx_INTCLEAR_SLVIF_WR2RO_ERR_IntStat          DMA_CHx_INTCLEAR_SLVIF_WR2RO_ERR_IntStat_Msk                  /*!< Slave Interface Write to Read Only Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_SLVIF_RD2RWO_ERR_IntStat_Pos     (18)
#define DMA_CHx_INTCLEAR_SLVIF_RD2RWO_ERR_IntStat_Msk     (0x01UL << DMA_CHx_INTCLEAR_SLVIF_RD2RWO_ERR_IntStat_Pos)     /*!< 0x00000000 00040000 */
#define DMA_CHx_INTCLEAR_SLVIF_RD2RWO_ERR_IntStat         DMA_CHx_INTCLEAR_SLVIF_RD2RWO_ERR_IntStat_Msk                 /*!< Slave Interface Read to write Only Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_SLVIF_WRONCHEN_ERR_IntStat_Pos   (19)
#define DMA_CHx_INTCLEAR_SLVIF_WRONCHEN_ERR_IntStat_Msk   (0x01UL << DMA_CHx_INTCLEAR_SLVIF_WRONCHEN_ERR_IntStat_Pos)   /*!< 0x00000000 00080000 */
#define DMA_CHx_INTCLEAR_SLVIF_WRONCHEN_ERR_IntStat       DMA_CHx_INTCLEAR_SLVIF_WRONCHEN_ERR_IntStat_Msk               /*!< Slave Interface Write On Channel Enabled Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat_Pos   (20)
#define DMA_CHx_INTCLEAR_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat_Msk   (0x01UL << DMA_CHx_INTCLEAR_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat_Pos)   /*!< 0x00000000 00100000 */
#define DMA_CHx_INTCLEAR_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat       DMA_CHx_INTCLEAR_SLVIF_SHADOWREG_WRON_VALID_ERR_IntStat_Msk               /*!< Shadow Register Write On Valid Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_SLVIF_WRONHOLD_ERR_IntStat_Pos   (21)
#define DMA_CHx_INTCLEAR_SLVIF_WRONHOLD_ERR_IntStat_Msk   (0x01UL << DMA_CHx_INTCLEAR_SLVIF_WRONHOLD_ERR_IntStat_Pos)   /*!< 0x00000000 00200000 */
#define DMA_CHx_INTCLEAR_SLVIF_WRONHOLD_ERR_IntStat       DMA_CHx_INTCLEAR_SLVIF_WRONHOLD_ERR_IntStat_Msk               /*!< Slave Interface Write On Hold Error Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_CH_LOCK_CLEARED_IntStat_Pos      (27)
#define DMA_CHx_INTCLEAR_CH_LOCK_CLEARED_IntStat_Msk      (0x01UL << DMA_CHx_INTCLEAR_CH_LOCK_CLEARED_IntStat_Pos)      /*!< 0x00000000 08000000 */
#define DMA_CHx_INTCLEAR_CH_LOCK_CLEARED_IntStat          DMA_CHx_INTCLEAR_CH_LOCK_CLEARED_IntStat_Msk                  /*!< Channel Lock Cleared Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_CH_SRC_SUSPENDED_IntStat_Pos     (28)
#define DMA_CHx_INTCLEAR_CH_SRC_SUSPENDED_IntStat_Msk     (0x01UL << DMA_CHx_INTCLEAR_CH_SRC_SUSPENDED_IntStat_Pos)     /*!< 0x00000000 10000000 */
#define DMA_CHx_INTCLEAR_CH_SRC_SUSPENDED_IntStat         DMA_CHx_INTCLEAR_CH_SRC_SUSPENDED_IntStat_Msk                 /*!< Channel Source Suspended Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_CH_SUSPENDED_IntStat_Pos         (29)
#define DMA_CHx_INTCLEAR_CH_SUSPENDED_IntStat_Msk         (0x01UL << DMA_CHx_INTCLEAR_CH_SUSPENDED_IntStat_Pos)         /*!< 0x00000000 20000000 */
#define DMA_CHx_INTCLEAR_CH_SUSPENDED_IntStat             DMA_CHx_INTCLEAR_CH_SUSPENDED_IntStat_Msk                     /*!< Channel Suspended Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_CH_DISABLED_IntStat_Pos          (30)
#define DMA_CHx_INTCLEAR_CH_DISABLED_IntStat_Msk          (0x01UL << DMA_CHx_INTCLEAR_CH_DISABLED_IntStat_Pos)          /*!< 0x00000000 40000000 */
#define DMA_CHx_INTCLEAR_CH_DISABLED_IntStat              DMA_CHx_INTCLEAR_CH_DISABLED_IntStat_Msk                      /*!< Channel Disabled Interrupt Clear Bit */
#define DMA_CHx_INTCLEAR_CH_ABORTED_IntStat_Pos           (31)
#define DMA_CHx_INTCLEAR_CH_ABORTED_IntStat_Msk           (0x01UL << DMA_CHx_INTCLEAR_CH_ABORTED_IntStat_Pos)           /*!< 0x00000000 80000000 */
#define DMA_CHx_INTCLEAR_CH_ABORTED_IntStat               DMA_CHx_INTCLEAR_CH_ABORTED_IntStat_Msk                       /*!< Channel Aborted  Interrupt Clear Bit */

/******************************************************************************/
/*                                                                            */
/*                Integrated Interchip Sound                                  */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for IER register  ******************/
#define I2S_IER_IEN_Pos                 (0)
#define I2S_IER_IEN_Msk                 (0x1UL << I2S_IER_IEN_Pos)                  /*!< 0x00000001 */
#define I2S_IER_IEN                     I2S_IER_IEN_Msk                             /*!< DW_apb_i2s enable. */
/******************  Bit definition for IRER register  ******************/
#define I2S_IRER_RXEN_Pos               (0)
#define I2S_IRER_RXEN_Msk               (0x1UL << I2S_IRER_RXEN_Pos)                /*!< 0x00000001 */
#define I2S_IRER_RXEN                   I2S_IRER_RXEN_Msk                           /*!< Receiver block enable. */
/******************  Bit definition for ITER register  ******************/
#define I2S_ITER_TXEN_Pos               (0)
#define I2S_ITER_TXEN_Msk               (0x1UL << I2S_ITER_TXEN_Pos)                /*!< 0x00000001 */
#define I2S_ITER_TXEN                   I2S_ITER_TXEN_Msk                           /*!< Transmitter block enable. */
/******************  Bit definition for CER register  ******************/
#define I2S_CER_CLKEN_Pos               (0)
#define I2S_CER_CLKEN_Msk               (0x1UL << I2S_CER_CLKEN_Pos)                /*!< 0x00000001 */
#define I2S_CER_CLKEN                   I2S_CER_CLKEN_Msk                           /*!< Clock generation enable/disable. */
/******************  Bit definition for CCR register  ******************/
#define I2S_CCR_SCLKG_Pos               (0)
#define I2S_CCR_SCLKG_Msk               (0x7UL << I2S_CCR_SCLKG_Pos)                /*!< 0x00000007 */
#define I2S_CCR_SCLKG                   I2S_CCR_SCLKG_Msk                           /*!< the gating of sclk. */
#define I2S_CCR_WSS_Pos                 (3)
#define I2S_CCR_WSS_Msk                 (0x3UL << I2S_CCR_WSS_Pos)                  /*!< 0x00000018 */
#define I2S_CCR_WSS                     I2S_CCR_WSS_Msk                             /*!< sclk cycles.  */
/******************  Bit definition for RXFFR register  ******************/
#define I2S_RXFFR_RXFFR_Pos             (0)
#define I2S_RXFFR_RXFFR_Msk             (0x1UL << I2S_RXFFR_RXFFR_Pos)              /*!< 0x00000001 */
#define I2S_RXFFR_RXFFR                 I2S_RXFFR_RXFFR_Msk                         /*!< Receiver FIFO Reset. */
/******************  Bit definition for TXFFR register  ******************/
#define I2S_TXFFR_TXFFR_Pos             (0)
#define I2S_TXFFR_TXFFR_Msk             (0x1UL << I2S_TXFFR_TXFFR_Pos)              /*!< 0x00000001 */
#define I2S_TXFFR_TXFFR                 I2S_TXFFR_TXFFR_Msk                         /*!< Transmitter FIFO Reset. */
/******************  Bit definition for RERx register  ******************/
#define I2S_RER_RXCHEN_Pos              (0)
#define I2S_RER_RXCHEN_Msk              (0x1UL << I2S_RER_RXCHEN_Pos)               /*!< 0x00000001 */
#define I2S_RER_RXCHEN                  I2S_RER_RXCHEN_Msk                          /*!< Receive channel enable. */
/******************  Bit definition for TERx register  ******************/
#define I2S_TER_TXCHEN_Pos              (0)
#define I2S_TER_TXCHEN_Msk              (0x1UL << I2S_TER_TXCHEN_Pos)               /*!< 0x00000001 */
#define I2S_TER_TXCHEN                  I2S_TER_TXCHEN_Msk                          /*!< Transmitter channel enable. */
/******************  Bit definition for RCRx register  ******************/
#define I2S_RCR_WLEN_Pos                (0)
#define I2S_RCR_WLEN_Msk                (0x7UL << I2S_RCR_WLEN_Pos)                 /*!< 0x00000007 */
#define I2S_RCR_WLEN                    I2S_RCR_WLEN_Msk                            /*!< Programmed data resolution. */
/******************  Bit definition for TCRx register  ******************/
#define I2S_TCR_WLEN_Pos                (0)
#define I2S_TCR_WLEN_Msk                (0x7UL << I2S_TCR_WLEN_Pos)                 /*!< 0x00000007 */
#define I2S_TCR_WLEN                    I2S_TCR_WLEN_Msk                            /*!< Programmed data resolution. */
/******************  Bit definition for ISRx register  ******************/
#define I2S_ISR_RXDA_Pos                (0)
#define I2S_ISR_RXDA_Msk                (0x1UL << I2S_ISR_RXDA_Pos)                 /*!< 0x00000001 */
#define I2S_ISR_RXDA                    I2S_ISR_RXDA_Msk                            /*!< Status of Receive Data Available interrupt. */
#define I2S_ISR_RXFO_Pos                (1)
#define I2S_ISR_RXFO_Msk                (0x1UL << I2S_ISR_RXFO_Pos)                 /*!< 0x00000002 */
#define I2S_ISR_RXFO                    I2S_ISR_RXFO_Msk                            /*!< Status of Data Overrun interrupt for the RX channel. */
#define I2S_ISR_TXFE_Pos                (4)
#define I2S_ISR_TXFE_Msk                (0x1UL << I2S_ISR_TXFE_Pos)                 /*!< 0x00000010 */
#define I2S_ISR_TXFE                    I2S_ISR_TXFE_Msk                            /*!< Status of Transmit Empty Trigger interrupt. */
#define I2S_ISR_TXFO_Pos                (5)
#define I2S_ISR_TXFO_Msk                (0x1UL << I2S_ISR_TXFO_Pos)                 /*!< 0x00000010 */
#define I2S_ISR_TXFO                    I2S_ISR_TXFO_Msk                            /*!< Status of Data Overrun interrupt for the TX channel. */
/******************  Bit definition for IMRx register  ******************/
#define I2S_IMR_RXDAM_Pos               (0)
#define I2S_IMR_RXDAM_Msk               (0x1UL << I2S_IMR_RXDAM_Pos)                /*!< 0x00000001 */
#define I2S_IMR_RXDAM                   I2S_IMR_RXDAM_Msk                           /*!< Mask RX FIFO Data Available interrupt. */
#define I2S_IMR_RXFOM_Pos               (1)
#define I2S_IMR_RXFOM_Msk               (0x1UL << I2S_IMR_RXFOM_Pos)                /*!< 0x00000002 */
#define I2S_IMR_RXFOM                   I2S_IMR_RXFOM_Msk                           /*!< Mask RX FIFO Overrun interrupt. */
#define I2S_IMR_TXFEM_Pos               (4)
#define I2S_IMR_TXFEM_Msk               (0x1UL << I2S_IMR_TXFEM_Pos)                /*!< 0x00000010 */
#define I2S_IMR_TXFEM                   I2S_IMR_TXFEM_Msk                           /*!< Mask TX FIFO Empty interrupt. */
#define I2S_IMR_TXFOM_Pos               (5)
#define I2S_IMR_TXFOM_Msk               (0x1UL << I2S_IMR_TXFOM_Pos)                /*!< 0x00000010 */
#define I2S_IMR_TXFOM                   I2S_IMR_TXFOM_Msk                           /*!< Mask TX FIFO Overrun interrupt. */
/******************  Bit definition for RORx register  ******************/
#define I2S_ROR_RXCHO_Pos               (0)
#define I2S_ROR_RXCHO_Msk               (0x1UL << I2S_ROR_RXCHO_Pos)                /*!< 0x00000001 */
#define I2S_ROR_RXCHO                   I2S_ROR_RXCHO_Msk                           /*!< Read this bit to clear the RX FIFO Data Overrun interrupt. */
/******************  Bit definition for TORx register  ******************/
#define I2S_TOR_TXCHO_Pos               (0)
#define I2S_TOR_TXCHO_Msk               (0x1UL << I2S_TOR_TXCHO_Pos)                /*!< 0x00000001 */
#define I2S_TOR_TXCHO                   I2S_TOR_TXCHO_Msk                           /*!< Read this bit to clear the TX FIFO Data Overrun interrupt. */
/******************  Bit definition for RFCRx register  ******************/
#define I2S_RFCR_RXCHDT_Pos             (0)
#define I2S_RFCR_RXCHDT_Msk             (0xFUL << I2S_RFCR_RXCHDT_Pos)              /*!< 0x0000000F */
#define I2S_RFCR_RXCHDT                 I2S_RFCR_RXCHDT_Msk                         /*!< Received Data Available interrupt trigger level. */
/******************  Bit definition for TFCRx register  ******************/
#define I2S_TFCR_TXCHET_Pos             (0)
#define I2S_TFCR_TXCHET_Msk             (0xFUL << I2S_TFCR_TXCHET_Pos)              /*!< 0x0000000F */
#define I2S_TFCR_TXCHET                 I2S_TFCR_TXCHET_Msk                         /*!< Empty Threshold Reached Interrupt trigger level. */
/******************  Bit definition for RFFx register  ******************/
#define I2S_RFF_RXCHFR_Pos              (0)
#define I2S_RFF_RXCHFR_Msk              (0x1UL << I2S_RFF_RXCHFR_Pos)               /*!< 0x00000001 */
#define I2S_RFF_RXCHFR                  I2S_RFF_RXCHFR_Msk                          /*!< Receive Channel FIFO Reset. */
/******************  Bit definition for TFFx register  ******************/
#define I2S_TFF_TXCHFR_Pos              (0)
#define I2S_TFF_TXCHFR_Msk              (0x1UL << I2S_TFF_TXCHFR_Pos)               /*!< 0x00000001 */
#define I2S_TFF_TXCHFR                  I2S_TFF_TXCHFR_Msk                          /*!< Transmit Channel FIFO Reset. */
/******************  Bit definition for RRXDMA register  ******************/
#define I2S_RRXDMA_RRXDMA_Pos           (0)
#define I2S_RRXDMA_RRXDMA_Msk           (0x1UL << I2S_RRXDMA_RRXDMA_Pos)            /*!< 0x00000001 */
#define I2S_RRXDMA_RRXDMA               I2S_RRXDMA_RRXDMA_Msk                       /*!< Reset Receiver Block DMA Register. */
/******************  Bit definition for RTXDMA register  ******************/
#define I2S_RTXDMA_RTXDMA_Pos           (0)
#define I2S_RTXDMA_RTXDMA_Msk           (0x1UL << I2S_RTXDMA_RTXDMA_Pos)            /*!< 0x00000001 */
#define I2S_RTXDMA_RTXDMA               I2S_RTXDMA_RTXDMA_Msk                       /*!< Reset Transmitter Block DMA Register. */
/******************  Bit definition for I2S_COMP_PARAM_2 register  ******************/
#define I2S_COMP_PARAM_2_RX_WORDSIZE_0_Pos      (0)
#define I2S_COMP_PARAM_2_RX_WORDSIZE_0_Msk      (0x7UL << I2S_COMP_PARAM_2_RX_WORDSIZE_0_Pos)   /*!< 0x00000007 */
#define I2S_COMP_PARAM_2_RX_WORDSIZE_0          I2S_COMP_PARAM_2_RX_WORDSIZE_0_Msk              /*!< These bits specify the RX resolution for WORDSIZE_0. */
#define I2S_COMP_PARAM_2_RX_WORDSIZE_1_Pos      (3)
#define I2S_COMP_PARAM_2_RX_WORDSIZE_1_Msk      (0x7UL << I2S_COMP_PARAM_2_RX_WORDSIZE_1_Pos)   /*!< 0x00000038 */
#define I2S_COMP_PARAM_2_RX_WORDSIZE_1          I2S_COMP_PARAM_2_RX_WORDSIZE_1_Msk              /*!< These bits specify the RX resolution for WORDSIZE_1. */
#define I2S_COMP_PARAM_2_RX_WORDSIZE_2_Pos      (7)
#define I2S_COMP_PARAM_2_RX_WORDSIZE_2_Msk      (0x7UL << I2S_COMP_PARAM_2_RX_WORDSIZE_2_Pos)   /*!< 0x00000380 */
#define I2S_COMP_PARAM_2_RX_WORDSIZE_2          I2S_COMP_PARAM_2_RX_WORDSIZE_2_Msk              /*!< These bits specify the RX resolution for WORDSIZE_2. */
#define I2S_COMP_PARAM_2_RX_WORDSIZE_3_Pos      (10)
#define I2S_COMP_PARAM_2_RX_WORDSIZE_3_Msk      (0x7UL << I2S_COMP_PARAM_2_RX_WORDSIZE_3_Pos)   /*!< 0x00001C00 */
#define I2S_COMP_PARAM_2_RX_WORDSIZE_3          I2S_COMP_PARAM_2_RX_WORDSIZE_3_Msk              /*!< These bits specify the RX resolution for WORDSIZE_3. */
/******************  Bit definition for I2S_COMP_PARAM_1 register  ******************/
#define I2S_COMP_PARAM_1_APB_DATA_WIDTH_Pos     (0)
#define I2S_COMP_PARAM_1_APB_DATA_WIDTH_Msk     (0x3UL << I2S_COMP_PARAM_1_APB_DATA_WIDTH_Pos)      /*!< 0x00000003 */
#define I2S_COMP_PARAM_1_APB_DATA_WIDTH         I2S_COMP_PARAM_1_APB_DATA_WIDTH_Msk                 /*!< These bits specify the APB data width. */
#define I2S_COMP_PARAM_1_FIFO_DEPTH_GLOBAL_Pos  (2)
#define I2S_COMP_PARAM_1_FIFO_DEPTH_GLOBAL_Msk  (0x3UL << I2S_COMP_PARAM_1_FIFO_DEPTH_GLOBAL_Pos)   /*!< 0x0000000C */
#define I2S_COMP_PARAM_1_FIFO_DEPTH_GLOBAL      I2S_COMP_PARAM_1_FIFO_DEPTH_GLOBAL_Msk              /*!< These bits specify the FIFO depth for TX and RX channels. */
#define I2S_COMP_PARAM_1_MODE_EN_Pos            (4)
#define I2S_COMP_PARAM_1_MODE_EN_Msk            (0x1UL << I2S_COMP_PARAM_1_MODE_EN_Pos)             /*!< 0x00000010 */
#define I2S_COMP_PARAM_1_MODE_EN                I2S_COMP_PARAM_1_MODE_EN_Msk                        /*!< This bit specifies whether the master mode is enabled or not. */
#define I2S_COMP_PARAM_1_TRANSMITTER_BLOCK_Pos  (5)
#define I2S_COMP_PARAM_1_TRANSMITTER_BLOCK_Msk  (0x1UL << I2S_COMP_PARAM_1_TRANSMITTER_BLOCK_Pos)   /*!< 0x00000020 */
#define I2S_COMP_PARAM_1_TRANSMITTER_BLOCK      I2S_COMP_PARAM_1_TRANSMITTER_BLOCK_Msk              /*!< This bit specifies whether the transmitter block is enabled or not. */
#define I2S_COMP_PARAM_1_RECEIVER_BLOCK_Pos     (6)
#define I2S_COMP_PARAM_1_RECEIVER_BLOCK_Msk     (0x1UL << I2S_COMP_PARAM_1_RECEIVER_BLOCK_Pos)      /*!< 0x00000040 */
#define I2S_COMP_PARAM_1_RECEIVER_BLOCK         I2S_COMP_PARAM_1_RECEIVER_BLOCK_Msk                 /*!< This bit specifies whether the receiver block is enabled or not. */
#define I2S_COMP_PARAM_1_RX_CHANNELS_Pos        (7)
#define I2S_COMP_PARAM_1_RX_CHANNELS_Msk        (0x3UL << I2S_COMP_PARAM_1_RX_CHANNELS_Pos)         /*!< 0x00000180 */
#define I2S_COMP_PARAM_1_RX_CHANNELS            I2S_COMP_PARAM_1_RX_CHANNELS_Msk                    /*!< These bits specify the number of RX channels. */
#define I2S_COMP_PARAM_1_TX_CHANNELS_Pos        (9)
#define I2S_COMP_PARAM_1_TX_CHANNELS_Msk        (0x3UL << I2S_COMP_PARAM_1_TX_CHANNELS_Pos)         /*!< 0x00000600 */
#define I2S_COMP_PARAM_1_TX_CHANNELS            I2S_COMP_PARAM_1_TX_CHANNELS_Msk                    /*!< These bits specify the number of TX channels. */
#define I2S_COMP_PARAM_1_TX_WORDSIZE_0_Pos      (16)
#define I2S_COMP_PARAM_1_TX_WORDSIZE_0_Msk      (0x7UL << I2S_COMP_PARAM_1_TX_WORDSIZE_0_Pos)       /*!< 0x00070000 */
#define I2S_COMP_PARAM_1_TX_WORDSIZE_0          I2S_COMP_PARAM_1_TX_WORDSIZE_0_Msk                  /*!< These bits specify the TX resolution for WORDSIZE_0. */
#define I2S_COMP_PARAM_1_TX_WORDSIZE_1_Pos      (19)
#define I2S_COMP_PARAM_1_TX_WORDSIZE_1_Msk      (0x7UL << I2S_COMP_PARAM_1_TX_WORDSIZE_1_Pos)       /*!< 0x00380000 */
#define I2S_COMP_PARAM_1_TX_WORDSIZE_1          I2S_COMP_PARAM_1_TX_WORDSIZE_1_Msk                  /*!< These bits specify the TX resolution for WORDSIZE_1. */
#define I2S_COMP_PARAM_1_TX_WORDSIZE_2_Pos      (22)
#define I2S_COMP_PARAM_1_TX_WORDSIZE_2_Msk      (0x7UL << I2S_COMP_PARAM_1_TX_WORDSIZE_2_Pos)       /*!< 0x01C00000 */
#define I2S_COMP_PARAM_1_TX_WORDSIZE_2          I2S_COMP_PARAM_1_TX_WORDSIZE_2_Msk                  /*!< These bits specify the TX resolution for WORDSIZE_2. */
#define I2S_COMP_PARAM_1_TX_WORDSIZE_3_Pos      (25)
#define I2S_COMP_PARAM_1_TX_WORDSIZE_3_Msk      (0x7UL << I2S_COMP_PARAM_1_TX_WORDSIZE_3_Pos)       /*!< 0x07000000 */
#define I2S_COMP_PARAM_1_TX_WORDSIZE_3          I2S_COMP_PARAM_1_TX_WORDSIZE_3_Msk                  /*!< These bits specify the TX resolution for WORDSIZE_3. */
/******************  Bit definition for DMACR register  ******************/
#define I2S_DMACR_DMAEN_RXCH_0_Pos      (0)
#define I2S_DMACR_DMAEN_RXCH_0_Msk      (0x1UL << I2S_DMACR_DMAEN_RXCH_0_Pos)       /*!< 0x00000001 */
#define I2S_DMACR_DMAEN_RXCH_0          I2S_DMACR_DMAEN_RXCH_0_Msk                  /*!< DMA Enable for receive channel 0. */
#define I2S_DMACR_DMAEN_RXCH_1_Pos      (1)
#define I2S_DMACR_DMAEN_RXCH_1_Msk      (0x1UL << I2S_DMACR_DMAEN_RXCH_1_Pos)       /*!< 0x00000002 */
#define I2S_DMACR_DMAEN_RXCH_1          I2S_DMACR_DMAEN_RXCH_1_Msk                  /*!< DMA Enable for receive channel 1. */
#define I2S_DMACR_DMAEN_RXCH_2_Pos      (2)
#define I2S_DMACR_DMAEN_RXCH_2_Msk      (0x1UL << I2S_DMACR_DMAEN_RXCH_2_Pos)       /*!< 0x00000004 */
#define I2S_DMACR_DMAEN_RXCH_2          I2S_DMACR_DMAEN_RXCH_2_Msk                  /*!< DMA Enable for receive channel 2. */
#define I2S_DMACR_DMAEN_RXCH_3_Pos      (3)
#define I2S_DMACR_DMAEN_RXCH_3_Msk      (0x1UL << I2S_DMACR_DMAEN_RXCH_3_Pos)       /*!< 0x00000008 */
#define I2S_DMACR_DMAEN_RXCH_3          I2S_DMACR_DMAEN_RXCH_3_Msk                  /*!< DMA Enable for receive channel 3. */
#define I2S_DMACR_DMAEN_TXCH_0_Pos      (8)
#define I2S_DMACR_DMAEN_TXCH_0_Msk      (0x1UL << I2S_DMACR_DMAEN_TXCH_0_Pos)       /*!< 0x00000100 */
#define I2S_DMACR_DMAEN_TXCH_0          I2S_DMACR_DMAEN_TXCH_0_Msk                  /*!< DMA Enable for transmit channel 0. */
#define I2S_DMACR_DMAEN_TXCH_1_Pos      (9)
#define I2S_DMACR_DMAEN_TXCH_1_Msk      (0x1UL << I2S_DMACR_DMAEN_TXCH_1_Pos)       /*!< 0x00000200 */
#define I2S_DMACR_DMAEN_TXCH_1          I2S_DMACR_DMAEN_TXCH_1_Msk                  /*!< DMA Enable for transmit channel 1. */
#define I2S_DMACR_DMAEN_TXCH_2_Pos      (10)
#define I2S_DMACR_DMAEN_TXCH_2_Msk      (0x1UL << I2S_DMACR_DMAEN_TXCH_2_Pos)       /*!< 0x00000400 */
#define I2S_DMACR_DMAEN_TXCH_2          I2S_DMACR_DMAEN_TXCH_2_Msk                  /*!< DMA Enable for transmit channel 2. */
#define I2S_DMACR_DMAEN_TXCH_3_Pos      (11)
#define I2S_DMACR_DMAEN_TXCH_3_Msk      (0x1UL << I2S_DMACR_DMAEN_TXCH_3_Pos)       /*!< 0x00000800 */
#define I2S_DMACR_DMAEN_TXCH_3          I2S_DMACR_DMAEN_TXCH_3_Msk                  /*!< DMA Enable for transmit channel 3. */
#define I2S_DMACR_DMAEN_RXBLOCK_Pos     (16)
#define I2S_DMACR_DMAEN_RXBLOCK_Msk     (0x1UL << I2S_DMACR_DMAEN_RXBLOCK_Pos)      /*!< 0x00010000 */
#define I2S_DMACR_DMAEN_RXBLOCK         I2S_DMACR_DMAEN_RXBLOCK_Msk                 /*!< DMA Enable for receive block. */
#define I2S_DMACR_DMAEN_TXBLOCK_Pos     (17)
#define I2S_DMACR_DMAEN_TXBLOCK_Msk     (0x1UL << I2S_DMACR_DMAEN_TXBLOCK_Pos)      /*!< 0x00020000 */
#define I2S_DMACR_DMAEN_TXBLOCK         I2S_DMACR_DMAEN_TXBLOCK_Msk                 /*!< DMA Enable for transmit block. */
/******************************************************************************/
/*                                                                            */
/*                Inter-Integrated Circuit                                    */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for CR register  ******************/
#define I2C_CR_MASTER_MODE_Pos              (0)
#define I2C_CR_MASTER_MODE_Msk              (0x1UL << I2C_CR_MASTER_MODE_Pos)           /*!< 0x00000001 */
#define I2C_CR_MASTER_MODE                  I2C_CR_MASTER_MODE_Msk                      /*!< Enable Master mode. */
#define I2C_CR_SPEED_Pos                    (1)
#define I2C_CR_SPEED_Msk                    (0x3UL << I2C_CR_SPEED_Pos)                 /*!< 0x00000006 */
#define I2C_CR_SPEED                        I2C_CR_SPEED_Msk                            /*!< speed of the DW_apb_i2c operates. */
#define I2C_CR_10BITADDR_SLAVE_Pos          (3)
#define I2C_CR_10BITADDR_SLAVE_Msk          (0x1UL << I2C_CR_10BITADDR_SLAVE_Pos)       /*!< 0x00000008 */
#define I2C_CR_10BITADDR_SLAVE              I2C_CR_10BITADDR_SLAVE_Msk                  /*!< Enable 10-bit addresses on slave. */
#define I2C_CR_10BITADDR_MASTER_Pos         (4)
#define I2C_CR_10BITADDR_MASTER_Msk         (0x1UL << I2C_CR_10BITADDR_MASTER_Pos)      /*!< 0x00000010 */
#define I2C_CR_10BITADDR_MASTER             I2C_CR_10BITADDR_MASTER_Msk                 /*!< Enable 10-bit addresses on master. */
#define I2C_CR_RESTART_EN_Pos               (5)
#define I2C_CR_RESTART_EN_Msk               (0x1UL << I2C_CR_RESTART_EN_Pos)            /*!< 0x00000020 */
#define I2C_CR_RESTART_EN                   I2C_CR_RESTART_EN_Msk                       /*!< Determines whether RESTART conditions may be sent when acting as a master. */
#define I2C_CR_SLAVE_DISABLE_Pos            (6)
#define I2C_CR_SLAVE_DISABLE_Msk            (0x1UL << I2C_CR_SLAVE_DISABLE_Pos)         /*!< 0x00000040 */
#define I2C_CR_SLAVE_DISABLE                I2C_CR_SLAVE_DISABLE_Msk                    /*!< Disable Slave mode. */
#define I2C_CR_STOP_DET_IFADDRESSED_Pos     (7)
#define I2C_CR_STOP_DET_IFADDRESSED_Msk     (0x1UL << I2C_CR_STOP_DET_IFADDRESSED_Pos)  /*!< 0x00000080 */
#define I2C_CR_STOP_DET_IFADDRESSED         I2C_CR_STOP_DET_IFADDRESSED_Msk             /*!< Issues the STOP_DET interrrupt select. */
#define I2C_CR_TX_EMPTY_CTRL_Pos            (8)
#define I2C_CR_TX_EMPTY_CTRL_Msk            (0x1UL << I2C_CR_TX_EMPTY_CTRL_Pos)         /*!< 0x00000100 */
#define I2C_CR_TX_EMPTY_CTRL                I2C_CR_TX_EMPTY_CTRL_Msk                    /*!< Controlled generation of TX_EMPTY interrupt. */
#define I2C_CR_RX_FIFO_FULL_HLD_CTRL_Pos    (9)
#define I2C_CR_RX_FIFO_FULL_HLD_CTRL_Msk    (0x1UL << I2C_CR_RX_FIFO_FULL_HLD_CTRL_Pos) /*!< 0x00000200 */
#define I2C_CR_RX_FIFO_FULL_HLD_CTRL        I2C_CR_RX_FIFO_FULL_HLD_CTRL_Msk            /*!< Hold bus or Overflow when RX_FIFO is full. */
#define I2C_CR_STOP_DET_IFMASTER_ACTIVE_Pos (10)
#define I2C_CR_STOP_DET_IFMASTER_ACTIVE_Msk (0x1UL << I2C_CR_STOP_DET_IFMASTER_ACTIVE_Pos) /*!< 0x00000400 */
#define I2C_CR_STOP_DET_IFMASTER_ACTIVE     I2C_CR_STOP_DET_IFMASTER_ACTIVE_Msk            /*!< Issues the STOP_DET interrrupt select whether master is active or not. */
#define I2C_CR_BUS_CLEAR_FEATURE_CTRL_Pos   (11)
#define I2C_CR_BUS_CLEAR_FEATURE_CTRL_Msk   (0x1UL << I2C_CR_BUS_CLEAR_FEATURE_CTRL_Pos) /*!< 0x00000800 */
#define I2C_CR_BUS_CLEAR_FEATURE_CTRL       I2C_CR_BUS_CLEAR_FEATURE_CTRL_Msk            /*!< Enable Bus Clear Feature. */
#define I2C_CR_OPTIONAL_SAR_CTRL_Pos        (16)
#define I2C_CR_OPTIONAL_SAR_CTRL_Msk        (0x1UL << I2C_CR_OPTIONAL_SAR_CTRL_Pos)     /*!< 0x00010000 */
#define I2C_CR_OPTIONAL_SAR_CTRL            I2C_CR_OPTIONAL_SAR_CTRL_Msk                /*!< Enable Optional SAR Address Register. */
#define I2C_CR_SMBUS_SLAVE_QUICK_EN_Pos     (17)
#define I2C_CR_SMBUS_SLAVE_QUICK_EN_Msk     (0x1UL << I2C_CR_SMBUS_SLAVE_QUICK_EN_Pos)  /*!< 0x00020000 */
#define I2C_CR_SMBUS_SLAVE_QUICK_EN         I2C_CR_SMBUS_SLAVE_QUICK_EN_Msk             /*!< Enable SMBus SLave in receive Quick command. */
#define I2C_CR_SMBUS_ARP_EN_Pos             (18)
#define I2C_CR_SMBUS_ARP_EN_Msk             (0x1UL << I2C_CR_SMBUS_ARP_EN_Pos)          /*!< 0x00040000 */
#define I2C_CR_SMBUS_ARP_EN                 I2C_CR_SMBUS_ARP_EN_Msk                     /*!< Enable SMBus ARP control . */
#define I2C_CR_SMBUS_PSADDR_EN_Pos          (19)
#define I2C_CR_SMBUS_PSADDR_EN_Msk          (0x1UL << I2C_CR_SMBUS_PSADDR_EN_Pos)       /*!< 0x00080000 */
#define I2C_CR_SMBUS_PSADDR_EN              I2C_CR_SMBUS_PSADDR_EN_Msk                  /*!< Enable SMBus Persistent Slave address control. */
/******************  Bit definition for TAR register  ******************/
#define I2C_TAR_Pos                         (0)
#define I2C_TAR_Msk                         (0x3FFUL << I2C_TAR_Pos)                /*!< 0x000003FF */
#define I2C_TAR                             I2C_TAR_Msk                             /*!< This is the target address for any master transaction. */
#define I2C_TAR_GC_OR_START_Pos             (10)
#define I2C_TAR_GC_OR_START_Msk             (0x1UL << I2C_TAR_GC_OR_START_Pos)          /*!< 0x00000400 */
#define I2C_TAR_GC_OR_START                 I2C_TAR_GC_OR_START_Msk                     /*!< General Call Address or START BYTE select. */
#define I2C_TAR_SPECIAL_Pos                 (11)
#define I2C_TAR_SPECIAL_Msk                 (0x1UL << I2C_TAR_SPECIAL_Pos)              /*!< 0x00000800 */
#define I2C_TAR_SPECIAL                     I2C_TAR_SPECIAL_Msk                         /*!< Enables programming of GENERAL_CALL or START_BYTE transmission. */
#define I2C_TAR_10BITADDR_MASTER_Pos        (12)
#define I2C_TAR_10BITADDR_MASTER_Msk        (0x1UL << I2C_TAR_10BITADDR_MASTER_Pos)     /*!< 0x00001000 */
#define I2C_TAR_10BITADDR_MASTER            I2C_TAR_10BITADDR_MASTER_Msk                /*!< Address 10Bit transmission format select. */
#define I2C_TAR_DEVICE_ID_Pos               (13)
#define I2C_TAR_DEVICE_ID_Msk               (0x1UL << I2C_TAR_DEVICE_ID_Pos)            /*!< 0x00002000 */
#define I2C_TAR_DEVICE_ID                   I2C_TAR_DEVICE_ID_Msk                       /*!< Enables programming of DEVICE-ID transmission. */
#define I2C_TAR_SMBUS_QUICK_CMD_Pos         (16)
#define I2C_TAR_SMBUS_QUICK_CMD_Msk         (0x1UL << I2C_TAR_SMBUS_QUICK_CMD_Pos)      /*!< 0x00004000 */
#define I2C_TAR_SMBUS_QUICK_CMD             I2C_TAR_SMBUS_QUICK_CMD_Msk                 /*!< Enables programming of QUICK-CMD transmission. */
/******************  Bit definition for SAR register  ******************/
#define I2C_SAR_Pos                         (0)
#define I2C_SAR_Msk                         (0x3FFUL << I2C_SAR_Pos)                /*!< 0x000003FF */
#define I2C_SAR                             I2C_SAR_Msk                             /*!< The IC_SAR holds the slave address when the I2C is operating as a slave. */
/******************  Bit definition for HS_MADDR register  ******************/
#define I2C_HS_MADDR_HS_MAR_Pos             (0)
#define I2C_HS_MADDR_HS_MAR_Msk             (0x7UL << I2C_HS_MADDR_HS_MAR_Pos)          /*!< 0x00000007 */
#define I2C_HS_MADDR_HS_MAR                 I2C_HS_MADDR_HS_MAR_Msk                     /*!< This bit field holds the value of the I2C HS mode master code. */
/******************  Bit definition for DATA_CMD register  ******************/
#define I2C_DATA_CMD_DAT_Pos                (0)
#define I2C_DATA_CMD_DAT_Msk                (0xFFUL << I2C_DATA_CMD_DAT_Pos)            /*!< 0x000000FF */
#define I2C_DATA_CMD_DAT                    I2C_DATA_CMD_DAT_Msk                        /*!< This register contains the data to be transmitted or received on the I2C bus. */
#define I2C_DATA_CMD_CMD_Pos                (8)
#define I2C_DATA_CMD_CMD_Msk                (0x1UL << I2C_DATA_CMD_CMD_Pos)             /*!< 0x00000100 */
#define I2C_DATA_CMD_CMD                    I2C_DATA_CMD_CMD_Msk                        /*!< This bit controls whether a read or a write is performed. */
#define I2C_DATA_CMD_STOP_Pos               (9)
#define I2C_DATA_CMD_STOP_Msk               (0x1UL << I2C_DATA_CMD_STOP_Pos)            /*!< 0x00000200 */
#define I2C_DATA_CMD_STOP                   I2C_DATA_CMD_STOP_Msk                       /*!< This bit controls whether a STOP is issued after the byte is sent or received. */
#define I2C_DATA_CMD_RESTART_Pos            (10)
#define I2C_DATA_CMD_RESTART_Msk            (0x1UL << I2C_DATA_CMD_RESTART_Pos)         /*!< 0x00000400 */
#define I2C_DATA_CMD_RESTART                I2C_DATA_CMD_RESTART_Msk                    /*!< This bit controls whether a RESTART is issued before the byte is sent or received. */
#define I2C_DATA_CMD_FIRST_DATA_BYTE_Pos    (11)
#define I2C_DATA_CMD_FIRST_DATA_BYTE_Msk    (0x1UL << I2C_DATA_CMD_FIRST_DATA_BYTE_Pos) /*!< 0x00000800 */
#define I2C_DATA_CMD_FIRST_DATA_BYTE        I2C_DATA_CMD_FIRST_DATA_BYTE_Msk            /*!< Indicates the first data byte received after the address phase for receive transfer in Master receiver or Slave receiver mode. */
/******************  Bit definition for SS_SCL_HCNT register  ******************/
#define I2C_SS_SCL_HCNT_Pos                 (0)
#define I2C_SS_SCL_HCNT_Msk                 (0xFFFFUL << I2C_SS_SCL_HCNT_Pos)           /*!< 0x0000FFFF */
#define I2C_SS_SCL_HCNT                     I2C_SS_SCL_HCNT_Msk                         /*!< This register sets the SCL clock high-period count for standard speed. */
/******************  Bit definition for UFM_SCL_HCNT register  ******************/
#define I2C_UFM_SCL_HCNT_Pos                (0)
#define I2C_UFM_SCL_HCNT_Msk                (0xFFFFUL << I2C_UFM_SCL_HCNT_Pos)          /*!< 0x0000FFFF */
#define I2C_UFM_SCL_HCNT                    I2C_UFM_SCL_HCNT_Msk                        /*!< This register sets the SCL clock high-period count for Ultra-Fast speed. */
/******************  Bit definition for SS_SCL_LCNT register  ******************/
#define I2C_SS_SCL_LCNT_Pos                 (0)
#define I2C_SS_SCL_LCNT_Msk                 (0xFFFFUL << I2C_SS_SCL_LCNT_Pos)           /*!< 0x0000FFFF */
#define I2C_SS_SCL_LCNT                     I2C_SS_SCL_LCNT_Msk                         /*!< This register sets the SCL clock low period count for standard speed. */
/******************  Bit definition for UFM_SCL_LCNT register  ******************/
#define I2C_UFM_SCL_LCNT_Pos                (0)
#define I2C_UFM_SCL_LCNT_Msk                (0xFFFFUL << I2C_UFM_SCL_LCNT_Pos)          /*!< 0x0000FFFF */
#define I2C_UFM_SCL_LCNT                    I2C_UFM_SCL_LCNT_Msk                        /*!< This register sets the SCL clock low period count for Ultra-Fast speed. */
/******************  Bit definition for FS_SCL_HCNT register  ******************/
#define I2C_FS_SCL_HCNT_Pos                 (0)
#define I2C_FS_SCL_HCNT_Msk                 (0xFFFFUL << I2C_FS_SCL_HCNT_Pos)           /*!< 0x0000FFFF */
#define I2C_FS_SCL_HCNT                     I2C_FS_SCL_HCNT_Msk                         /*!< This register sets the SCL clock high-period count for fast mode or fast mode plus. */
/******************  Bit definition for UFM_TBUF_CNT register  ******************/
#define I2C_UFM_TBUF_CNT_Pos                (0)
#define I2C_UFM_TBUF_CNT_Msk                (0xFFFFUL << I2C_UFM_TBUF_CNT_Pos)          /*!< 0x0000FFFF */
#define I2C_UFM_TBUF_CNT                    I2C_UFM_TBUF_CNT_Msk                        /*!< This register sets the Bus-Free time between a STOP and STOP condition count for Ultra-Fast speed. */
/******************  Bit definition for FS_SCL_LCNT register  ******************/
#define I2C_FS_SCL_LCNT_Pos                 (0)
#define I2C_FS_SCL_LCNT_Msk                 (0xFFFFUL << I2C_FS_SCL_LCNT_Pos)           /*!< 0x0000FFFF */
#define I2C_FS_SCL_LCNT                     I2C_FS_SCL_LCNT_Msk                         /*!< This register sets the SCL clock low period count for fast speed. */
/******************  Bit definition for HS_SCL_HCNT register  ******************/
#define I2C_HS_SCL_HCNT_Pos                 (0)
#define I2C_HS_SCL_HCNT_Msk                 (0xFFFFUL << I2C_HS_SCL_HCNT_Pos)           /*!< 0x0000FFFF */
#define I2C_HS_SCL_HCNT                     I2C_HS_SCL_HCNT_Msk                         /*!< This register sets the SCL clock high period count for high speed. */
/******************  Bit definition for HS_SCL_LCNT register  ******************/
#define I2C_HS_SCL_LCNT_Pos                 (0)
#define I2C_HS_SCL_LCNT_Msk                 (0xFFFFUL << I2C_HS_SCL_LCNT_Pos)           /*!< 0x0000FFFF */
#define I2C_HS_SCL_LCNT                     I2C_HS_SCL_LCNT_Msk                         /*!< This register sets the SCL clock low period count for high speed. */
/******************  Bit definition for ISR register  ******************/
#define I2C_ISR_RX_UNDER_Pos                (0)
#define I2C_ISR_RX_UNDER_Msk                (0x1UL << I2C_ISR_RX_UNDER_Pos)             /*!< 0x00000001 */
#define I2C_ISR_RX_UNDER                    I2C_ISR_RX_UNDER_Msk                        /*!< Receive buffer underflow interrupt. */
#define I2C_ISR_RX_OVER_Pos                 (1)
#define I2C_ISR_RX_OVER_Msk                 (0x1UL << I2C_ISR_RX_OVER_Pos)              /*!< 0x00000002 */
#define I2C_ISR_RX_OVER                     I2C_ISR_RX_OVER_Msk                         /*!< Receive buffer overflow interrupt state. */
#define I2C_ISR_RX_FULL_Pos                 (2)
#define I2C_ISR_RX_FULL_Msk                 (0x1UL << I2C_ISR_RX_FULL_Pos)              /*!< 0x00000004 */
#define I2C_ISR_RX_FULL                     I2C_ISR_RX_FULL_Msk                         /*!< Receive buffer full interrupt state. */
#define I2C_ISR_TX_OVER_Pos                 (3)
#define I2C_ISR_TX_OVER_Msk                 (0x1UL << I2C_ISR_TX_OVER_Pos)              /*!< 0x00000008 */
#define I2C_ISR_TX_OVER                     I2C_ISR_TX_OVER_Msk                         /*!< Transmit buffer overflow interrupt state. */
#define I2C_ISR_TX_EMPTY_Pos                (4)
#define I2C_ISR_TX_EMPTY_Msk                (0x1UL << I2C_ISR_TX_EMPTY_Pos)             /*!< 0x00000010 */
#define I2C_ISR_TX_EMPTY                    I2C_ISR_TX_EMPTY_Msk                        /*!< Transmit buffer empty interrupt state. */
#define I2C_ISR_RD_REQ_Pos                  (5)
#define I2C_ISR_RD_REQ_Msk                  (0x1UL << I2C_ISR_RD_REQ_Pos)               /*!< 0x00000020 */
#define I2C_ISR_RD_REQ                      I2C_ISR_RD_REQ_Msk                          /*!< Slave read request interrupt state. */
#define I2C_ISR_TX_ABRT_Pos                 (6)
#define I2C_ISR_TX_ABRT_Msk                 (0x1UL << I2C_ISR_TX_ABRT_Pos)              /*!< 0x00000040 */
#define I2C_ISR_TX_ABRT                     I2C_ISR_TX_ABRT_Msk                         /*!< Transmit abort interrupt. state. */
#define I2C_ISR_RX_DONE_Pos                 (7)
#define I2C_ISR_RX_DONE_Msk                 (0x1UL << I2C_ISR_RX_DONE_Pos)              /*!< 0x00000080 */
#define I2C_ISR_RX_DONE                     I2C_ISR_RX_DONE_Msk                         /*!< Receive done interrupt state. */
#define I2C_ISR_ACTIVITY_Pos                (8)
#define I2C_ISR_ACTIVITY_Msk                (0x1UL << I2C_ISR_ACTIVITY_Pos)             /*!< 0x00000100 */
#define I2C_ISR_ACTIVITY                    I2C_ISR_ACTIVITY_Msk                        /*!< I2C activity interrupt state. */
#define I2C_ISR_STOP_DET_Pos                (9)
#define I2C_ISR_STOP_DET_Msk                (0x1UL << I2C_ISR_STOP_DET_Pos)             /*!< 0x00000200 */
#define I2C_ISR_STOP_DET                    I2C_ISR_STOP_DET_Msk                        /*!< Stop condition detect on I2C interrupt state. */
#define I2C_ISR_START_DET_Pos               (10)
#define I2C_ISR_START_DET_Msk               (0x1UL << I2C_ISR_START_DET_Pos)            /*!< 0x00000400 */
#define I2C_ISR_START_DET                   I2C_ISR_START_DET_Msk                       /*!< Start condition detect on I2C interrupt state. */
#define I2C_ISR_GEN_CALL_Pos                (11)
#define I2C_ISR_GEN_CALL_Msk                (0x1UL << I2C_ISR_GEN_CALL_Pos)             /*!< 0x00000800 */
#define I2C_ISR_GEN_CALL                    I2C_ISR_GEN_CALL_Msk                        /*!< General Call received interrupt state. */
#define I2C_ISR_RESTART_DET_Pos             (12)
#define I2C_ISR_RESTART_DET_Msk             (0x1UL << I2C_ISR_RESTART_DET_Pos)          /*!< 0x00001000 */
#define I2C_ISR_RESTART_DET                 I2C_ISR_RESTART_DET_Msk                     /*!< Restart condition detect on I2C interrupt state. */
#define I2C_ISR_MASTER_ON_HOLD_Pos          (13)
#define I2C_ISR_MASTER_ON_HOLD_Msk          (0x1UL << I2C_ISR_MASTER_ON_HOLD_Pos)       /*!< 0x00002000 */
#define I2C_ISR_MASTER_ON_HOLD              I2C_ISR_MASTER_ON_HOLD_Msk                  /*!< Master on hold I2C interrupt state. */
#define I2C_ISR_SCL_STUCK_AT_LOW_Pos        (14)
#define I2C_ISR_SCL_STUCK_AT_LOW_Msk        (0x1UL << I2C_ISR_SCL_STUCK_AT_LOW_Pos)     /*!< 0x00004000 */
#define I2C_ISR_SCL_STUCK_AT_LOW            I2C_ISR_SCL_STUCK_AT_LOW_Msk                /*!< SCL Stuck condition detect on I2C interrupt state. */
/******************  Bit definition for IMR register  ******************/
#define I2C_IMR_RX_UNDER_Pos                (0)
#define I2C_IMR_RX_UNDER_Msk                (0x1UL << I2C_IMR_RX_UNDER_Pos)             /*!< 0x00000001 */
#define I2C_IMR_RX_UNDER                    I2C_IMR_RX_UNDER_Msk                        /*!< Receive buffer underflow interrupt state mask. */
#define I2C_IMR_RX_OVER_Pos                 (1)
#define I2C_IMR_RX_OVER_Msk                 (0x1UL << I2C_IMR_RX_OVER_Pos)              /*!< 0x00000002 */
#define I2C_IMR_RX_OVER                     I2C_IMR_RX_OVER_Msk                         /*!< Receive buffer overflow interrupt state mask. */
#define I2C_IMR_RX_FULL_Pos                 (2)
#define I2C_IMR_RX_FULL_Msk                 (0x1UL << I2C_IMR_RX_FULL_Pos)              /*!< 0x00000004 */
#define I2C_IMR_RX_FULL                     I2C_IMR_RX_FULL_Msk                         /*!< Receive buffer full interrupt state mask. */
#define I2C_IMR_TX_OVER_Pos                 (3)
#define I2C_IMR_TX_OVER_Msk                 (0x1UL << I2C_IMR_TX_OVER_Pos)              /*!< 0x00000008 */
#define I2C_IMR_TX_OVER                     I2C_IMR_TX_OVER_Msk                         /*!< Transmit buffer overflow interrupt state mask. */
#define I2C_IMR_TX_EMPTY_Pos                (4)
#define I2C_IMR_TX_EMPTY_Msk                (0x1UL << I2C_IMR_TX_EMPTY_Pos)             /*!< 0x00000010 */
#define I2C_IMR_TX_EMPTY                    I2C_IMR_TX_EMPTY_Msk                        /*!< Transmit buffer empty interrupt state mask. */
#define I2C_IMR_RD_REQ_Pos                  (5)
#define I2C_IMR_RD_REQ_Msk                  (0x1UL << I2C_IMR_RD_REQ_Pos)               /*!< 0x00000020 */
#define I2C_IMR_RD_REQ                      I2C_IMR_RD_REQ_Msk                          /*!< Slave read request interrupt state mask. */
#define I2C_IMR_TX_ABRT_Pos                 (6)
#define I2C_IMR_TX_ABRT_Msk                 (0x1UL << I2C_IMR_TX_ABRT_Pos)              /*!< 0x00000040 */
#define I2C_IMR_TX_ABRT                     I2C_IMR_TX_ABRT_Msk                         /*!< Transmit abort interrupt. state mask. */
#define I2C_IMR_RX_DONE_Pos                 (7)
#define I2C_IMR_RX_DONE_Msk                 (0x1UL << I2C_IMR_RX_DONE_Pos)              /*!< 0x00000080 */
#define I2C_IMR_RX_DONE                     I2C_IMR_RX_DONE_Msk                         /*!< Receive done interrupt state mask. */
#define I2C_IMR_ACTIVITY_Pos                (8)
#define I2C_IMR_ACTIVITY_Msk                (0x1UL << I2C_IMR_ACTIVITY_Pos)             /*!< 0x00000100 */
#define I2C_IMR_ACTIVITY                    I2C_IMR_ACTIVITY_Msk                        /*!< I2C activity interrupt state mask. */
#define I2C_IMR_STOP_DET_Pos                (9)
#define I2C_IMR_STOP_DET_Msk                (0x1UL << I2C_IMR_STOP_DET_Pos)             /*!< 0x00000200 */
#define I2C_IMR_STOP_DET                    I2C_IMR_STOP_DET_Msk                        /*!< Stop condition detect on I2C interrupt state mask. */
#define I2C_IMR_START_DET_Pos               (10)
#define I2C_IMR_START_DET_Msk               (0x1UL << I2C_IMR_START_DET_Pos)            /*!< 0x00000400 */
#define I2C_IMR_START_DET                   I2C_IMR_START_DET_Msk                       /*!< Start condition detect on I2C interrupt state mask. */
#define I2C_IMR_GEN_CALL_Pos                (11)
#define I2C_IMR_GEN_CALL_Msk                (0x1UL << I2C_IMR_GEN_CALL_Pos)             /*!< 0x00000800 */
#define I2C_IMR_GEN_CALL                    I2C_IMR_GEN_CALL_Msk                        /*!< General Call received interrupt state mask. */
#define I2C_IMR_RESTART_DET_Pos             (12)
#define I2C_IMR_RESTART_DET_Msk             (0x1UL << I2C_IMR_RESTART_DET_Pos)          /*!< 0x00001000 */
#define I2C_IMR_RESTART_DET                 I2C_IMR_RESTART_DET_Msk                     /*!< Restart condition detect on I2C interrupt state mask. */
#define I2C_IMR_MASTER_ON_HOLD_Pos          (13)
#define I2C_IMR_MASTER_ON_HOLD_Msk          (0x1UL << I2C_IMR_MASTER_ON_HOLD_Pos)       /*!< 0x00002000 */
#define I2C_IMR_MASTER_ON_HOLD              I2C_IMR_MASTER_ON_HOLD_Msk                  /*!< Master on hold I2C interrupt state mask. */
#define I2C_IMR_SCL_STUCK_AT_LOW_Pos        (14)
#define I2C_IMR_SCL_STUCK_AT_LOW_Msk        (0x1UL << I2C_IMR_SCL_STUCK_AT_LOW_Pos)     /*!< 0x00004000 */
#define I2C_IMR_SCL_STUCK_AT_LOW            I2C_IMR_SCL_STUCK_AT_LOW_Msk                /*!< SCL Stuck condition detect on I2C interrupt state mask. */
/******************  Bit definition for RISR register  ******************/
#define I2C_RISR_RX_UNDER_Pos                (0)
#define I2C_RISR_RX_UNDER_Msk                (0x1UL << I2C_RISR_RX_UNDER_Pos)             /*!< 0x00000001 */
#define I2C_RISR_RX_UNDER                    I2C_RISR_RX_UNDER_Msk                        /*!< Receive buffer underflow raw interrupt state. */
#define I2C_RISR_RX_OVER_Pos                 (1)
#define I2C_RISR_RX_OVER_Msk                 (0x1UL << I2C_RISR_RX_OVER_Pos)              /*!< 0x00000002 */
#define I2C_RISR_RX_OVER                     I2C_RISR_RX_OVER_Msk                         /*!< Receive buffer overflow raw interrupt state. */
#define I2C_RISR_RX_FULL_Pos                 (2)
#define I2C_RISR_RX_FULL_Msk                 (0x1UL << I2C_RISR_RX_FULL_Pos)              /*!< 0x00000004 */
#define I2C_RISR_RX_FULL                     I2C_RISR_RX_FULL_Msk                         /*!< Receive buffer full raw interrupt state. */
#define I2C_RISR_TX_OVER_Pos                 (3)
#define I2C_RISR_TX_OVER_Msk                 (0x1UL << I2C_RISR_TX_OVER_Pos)              /*!< 0x00000008 */
#define I2C_RISR_TX_OVER                     I2C_RISR_TX_OVER_Msk                         /*!< Transmit buffer overflow raw interrupt state. */
#define I2C_RISR_TX_EMPTY_Pos                (4)
#define I2C_RISR_TX_EMPTY_Msk                (0x1UL << I2C_RISR_TX_EMPTY_Pos)             /*!< 0x00000010 */
#define I2C_RISR_TX_EMPTY                    I2C_RISR_TX_EMPTY_Msk                        /*!< Transmit buffer empty raw interrupt state. */
#define I2C_RISR_RD_REQ_Pos                  (5)
#define I2C_RISR_RD_REQ_Msk                  (0x1UL << I2C_RISR_RD_REQ_Pos)               /*!< 0x00000020 */
#define I2C_RISR_RD_REQ                      I2C_RISR_RD_REQ_Msk                          /*!< Slave read request raw interrupt state. */
#define I2C_RISR_TX_ABRT_Pos                 (6)
#define I2C_RISR_TX_ABRT_Msk                 (0x1UL << I2C_RISR_TX_ABRT_Pos)              /*!< 0x00000040 */
#define I2C_RISR_TX_ABRT                     I2C_RISR_TX_ABRT_Msk                         /*!< Transmit abort raw interrupt. state. */
#define I2C_RISR_RX_DONE_Pos                 (7)
#define I2C_RISR_RX_DONE_Msk                 (0x1UL << I2C_RISR_RX_DONE_Pos)              /*!< 0x00000080 */
#define I2C_RISR_RX_DONE                     I2C_RISR_RX_DONE_Msk                         /*!< Receive done raw interrupt state. */
#define I2C_RISR_ACTIVITY_Pos                (8)
#define I2C_RISR_ACTIVITY_Msk                (0x1UL << I2C_RISR_ACTIVITY_Pos)             /*!< 0x00000100 */
#define I2C_RISR_ACTIVITY                    I2C_RISR_ACTIVITY_Msk                        /*!< I2C activity raw interrupt state. */
#define I2C_RISR_STOP_DET_Pos                (9)
#define I2C_RISR_STOP_DET_Msk                (0x1UL << I2C_RISR_STOP_DET_Pos)             /*!< 0x00000200 */
#define I2C_RISR_STOP_DET                    I2C_RISR_STOP_DET_Msk                        /*!< Stop condition detect on I2C raw interrupt state. */
#define I2C_RISR_START_DET_Pos               (10)
#define I2C_RISR_START_DET_Msk               (0x1UL << I2C_RISR_START_DET_Pos)            /*!< 0x00000400 */
#define I2C_RISR_START_DET                   I2C_RISR_START_DET_Msk                       /*!< Start condition detect on I2C raw interrupt state. */
#define I2C_RISR_GEN_CALL_Pos                (11)
#define I2C_RISR_GEN_CALL_Msk                (0x1UL << I2C_RISR_GEN_CALL_Pos)             /*!< 0x00000800 */
#define I2C_RISR_GEN_CALL                    I2C_RISR_GEN_CALL_Msk                        /*!< General Call received raw interrupt state. */
#define I2C_RISR_RESTART_DET_Pos             (12)
#define I2C_RISR_RESTART_DET_Msk             (0x1UL << I2C_RISR_RESTART_DET_Pos)          /*!< 0x00001000 */
#define I2C_RISR_RESTART_DET                 I2C_RISR_RESTART_DET_Msk                     /*!< Restart condition detect on I2C raw interrupt state. */
#define I2C_RISR_MASTER_ON_HOLD_Pos          (13)
#define I2C_RISR_MASTER_ON_HOLD_Msk          (0x1UL << I2C_RISR_MASTER_ON_HOLD_Pos)       /*!< 0x00002000 */
#define I2C_RISR_MASTER_ON_HOLD              I2C_RISR_MASTER_ON_HOLD_Msk                  /*!< Master on hold I2C raw interrupt state. */
#define I2C_RISR_SCL_STUCK_AT_LOW_Pos        (14)
#define I2C_RISR_SCL_STUCK_AT_LOW_Msk        (0x1UL << I2C_RISR_SCL_STUCK_AT_LOW_Pos)     /*!< 0x00004000 */
#define I2C_RISR_SCL_STUCK_AT_LOW            I2C_RISR_SCL_STUCK_AT_LOW_Msk                /*!< SCL Stuck condition detect on I2C raw interrupt state. */
/******************  Bit definition for RX_TL register  ******************/
#define I2C_RX_TL_Pos                       (0)
#define I2C_RX_TL_Msk                       (0xFFUL << I2C_RX_TL_Pos)                   /*!< 0x000000FF */
#define I2C_RX_TL                           I2C_RX_TL_Msk                               /*!< Receive FIFO Threshold Level. */
/******************  Bit definition for TX_TL register  ******************/
#define I2C_TX_TL_Pos                       (0)
#define I2C_TX_TL_Msk                       (0xFFUL << I2C_TX_TL_Pos)                   /*!< 0x000000FF */
#define I2C_TX_TL                           I2C_TX_TL_Msk                               /*!< Transmit FIFO Threshold Level. */
/******************  Bit definition for CLR_INTR register  ******************/
#define I2C_CLR_INTR_Pos                    (0)
#define I2C_CLR_INTR_Msk                    (0x1UL << I2C_CLR_INTR_Pos)                 /*!< 0x00000001 */
#define I2C_CLR_INTR                        I2C_CLR_INTR_Msk                            /*!< Read this register to clear the combined interrupt */
/******************  Bit definition for CLR_RX_UNDER register  ******************/
#define I2C_CLR_RX_UNDER_Pos                (0)
#define I2C_CLR_RX_UNDER_Msk                (0x1UL << I2C_CLR_RX_UNDER_Pos)             /*!< 0x00000001 */
#define I2C_CLR_RX_UNDER                    I2C_CLR_RX_UNDER_Msk                        /*!< Read this register to clear the RX_UNDER interrupt */
/******************  Bit definition for CLR_RX_OVER register  ******************/
#define I2C_CLR_RX_OVER_Pos                 (0)
#define I2C_CLR_RX_OVER_Msk                 (0x1UL << I2C_CLR_RX_OVER_Pos)              /*!< 0x00000001 */
#define I2C_CLR_RX_OVER                     I2C_CLR_RX_OVER_Msk                         /*!< Read this register to clear the RX_OVER interrupt */
/******************  Bit definition for CLR_TX_OVER register  ******************/
#define I2C_CLR_TX_OVER_Pos                 (0)
#define I2C_CLR_TX_OVER_Msk                 (0x1UL << I2C_CLR_TX_OVER_Pos)              /*!< 0x00000001 */
#define I2C_CLR_TX_OVER                     I2C_CLR_TX_OVER_Msk                         /*!< Read this register to clear the TX_OVER interrupt */
/******************  Bit definition for CLR_RD_REQ register  ******************/
#define I2C_CLR_RD_REQ_Pos                  (0)
#define I2C_CLR_RD_REQ_Msk                  (0x1UL << I2C_CLR_RD_REQ_Pos)               /*!< 0x00000001 */
#define I2C_CLR_RD_REQ                      I2C_CLR_RD_REQ_Msk                          /*!< Read this register to clear the RD_REQ interrupt */
/******************  Bit definition for CLR_TX_ABRT register  ******************/
#define I2C_CLR_TX_ABRT_Pos                 (0)
#define I2C_CLR_TX_ABRT_Msk                 (0x1UL << I2C_CLR_TX_ABRT_Pos)              /*!< 0x00000001 */
#define I2C_CLR_TX_ABRT                     I2C_CLR_TX_ABRT_Msk                         /*!< Read this register to clear the TX_ABRT interrupt */
/******************  Bit definition for CLR_RX_DONE register  ******************/
#define I2C_CLR_RX_DONE_Pos                 (0)
#define I2C_CLR_RX_DONE_Msk                 (0x1UL << I2C_CLR_RX_DONE_Pos)              /*!< 0x00000001 */
#define I2C_CLR_RX_DONE                     I2C_CLR_RX_DONE_Msk                         /*!< Read this register to clear the RX_DONE interrupt */
/******************  Bit definition for CLR_ACTIVITY register  ******************/
#define I2C_CLR_ACTIVITY_Pos                (0)
#define I2C_CLR_ACTIVITY_Msk                (0x1UL << I2C_CLR_ACTIVITY_Pos)             /*!< 0x00000001 */
#define I2C_CLR_ACTIVITY                    I2C_CLR_ACTIVITY_Msk                        /*!< Read this register to clear the ACTIVITY interrupt */
/******************  Bit definition for CLR_STOP_DET register  ******************/
#define I2C_CLR_STOP_DET_Pos                (0)
#define I2C_CLR_STOP_DET_Msk                (0x1UL << I2C_CLR_STOP_DET_Pos)             /*!< 0x00000001 */
#define I2C_CLR_STOP_DET                    I2C_CLR_STOP_DET_Msk                        /*!< Read this register to clear the STOP_DET interrupt */
/******************  Bit definition for CLR_START_DET register  ******************/
#define I2C_CLR_START_DET_Pos               (0)
#define I2C_CLR_START_DET_Msk               (0x1UL << I2C_CLR_START_DET_Pos)            /*!< 0x00000001 */
#define I2C_CLR_START_DET                   I2C_CLR_START_DET_Msk                       /*!< Read this register to clear the START_DET interrupt */
/******************  Bit definition for CLR_GEN_CALL register  ******************/
#define I2C_CLR_GEN_CALL_Pos                (0)
#define I2C_CLR_GEN_CALL_Msk                (0x1UL << I2C_CLR_GEN_CALL_Pos)             /*!< 0x00000001 */
#define I2C_CLR_GEN_CALL                    I2C_CLR_GEN_CALL_Msk                        /*!< Read this register to clear the GEN_CALL interrupt */
/******************  Bit definition for ENR register  ******************/
#define I2C_ENR_ENABLE_Pos                 (0)
#define I2C_ENR_ENABLE_Msk                 (0x1UL << I2C_ENR_ENABLE_Pos)              /*!< 0x00000001 */
#define I2C_ENR_ENABLE                     I2C_ENR_ENABLE_Msk                         /*!< Controls whether the DW_apb_i2c is enabled. */
#define I2C_ENR_ABORT_Pos                  (1)
#define I2C_ENR_ABORT_Msk                  (0x1UL << I2C_ENR_ABORT_Pos)               /*!< 0x00000002 */
#define I2C_ENR_ABORT                      I2C_ENR_ABORT_Msk                          /*!< the controller initiates the transfer abort. */
#define I2C_ENR_TX_CMD_BLOCK_Pos           (2)
#define I2C_ENR_TX_CMD_BLOCK_Msk           (0x1UL << I2C_ENR_TX_CMD_BLOCK_Pos)        /*!< 0x00000004 */
#define I2C_ENR_TX_CMD_BLOCK               I2C_ENR_TX_CMD_BLOCK_Msk                   /*!< Blocks the transmission of data on I2C bus even if Tx FIFO has data to transmit. */
#define I2C_ENR_SDA_STUCK_RECOVERY_Pos     (3)
#define I2C_ENR_SDA_STUCK_RECOVERY_Msk     (0x1UL << I2C_ENR_SDA_STUCK_RECOVERY_Pos)  /*!< 0x00000008 */
#define I2C_ENR_SDA_STUCK_RECOVERY         I2C_ENR_SDA_STUCK_RECOVERY_Msk             /*!< Enable SDA stuck recovery. */
#define I2C_ENR_SMBUS_CLK_RESET_Pos        (16)
#define I2C_ENR_SMBUS_CLK_RESET_Msk        (0x1UL << I2C_ENR_SMBUS_CLK_RESET_Pos)     /*!< 0x00010000 */
#define I2C_ENR_SMBUS_CLK_RESET            I2C_ENR_SMBUS_CLK_RESET_Msk                /*!< SMBus Master Clock Reset. */
#define I2C_ENR_SMBUS_SUSPEND_EN_Pos       (17)
#define I2C_ENR_SMBUS_SUSPEND_EN_Msk       (0x1UL << I2C_ENR_SMBUS_SUSPEND_EN_Pos)    /*!< 0x00020000 */
#define I2C_ENR_SMBUS_SUSPEND_EN           I2C_ENR_SMBUS_SUSPEND_EN_Msk               /*!< Enable assertion of SMBSUS signal. */
#define I2C_ENR_SMBUS_ALERT_EN_Pos         (18)
#define I2C_ENR_SMBUS_ALERT_EN_Msk         (0x1UL << I2C_ENR_SMBUS_ALERT_EN_Pos)      /*!< 0x00040000 */
#define I2C_ENR_SMBUS_ALERT_EN             I2C_ENR_SMBUS_ALERT_EN_Msk                 /*!< Enable assertion of SMBSUS ALERT signal. */
/******************  Bit definition for SR register  ******************/
#define I2C_SR_ACTIVITY_Pos                 (0)
#define I2C_SR_ACTIVITY_Msk                 (0x1UL << I2C_SR_ACTIVITY_Pos)              /*!< 0x00000001 */
#define I2C_SR_ACTIVITY                     I2C_SR_ACTIVITY_Msk                         /*!< I2C Activity Status. */
#define I2C_SR_TFNF_Pos                     (1)
#define I2C_SR_TFNF_Msk                     (0x1UL << I2C_SR_TFNF_Pos)                  /*!< 0x00000002 */
#define I2C_SR_TFNF                         I2C_SR_TFNF_Msk                             /*!< Transmit FIFO Not Full. */
#define I2C_SR_TFE_Pos                      (2)
#define I2C_SR_TFE_Msk                      (0x1UL << I2C_SR_TFE_Pos)                   /*!< 0x00000004 */
#define I2C_SR_TFE                          I2C_SR_TFE_Msk                              /*!< Transmit FIFO Completely Empty. */
#define I2C_SR_RFNE_Pos                     (3)
#define I2C_SR_RFNE_Msk                     (0x1UL << I2C_SR_RFNE_Pos)                  /*!< 0x00000008 */
#define I2C_SR_RFNE                         I2C_SR_RFNE_Msk                             /*!< Receive FIFO Not Empty. */
#define I2C_SR_RFF_Pos                      (4)
#define I2C_SR_RFF_Msk                      (0x1UL << I2C_SR_RFF_Pos)                   /*!< 0x00000010 */
#define I2C_SR_RFF                          I2C_SR_RFF_Msk                              /*!< Receive FIFO Completely Full. */
#define I2C_SR_MST_ACTIVITY_Pos             (5)
#define I2C_SR_MST_ACTIVITY_Msk             (0x1UL << I2C_SR_MST_ACTIVITY_Pos)          /*!< 0x00000020 */
#define I2C_SR_MST_ACTIVITY                 I2C_SR_MST_ACTIVITY_Msk                     /*!< Master FSM Activity Status. */
#define I2C_SR_SLV_ACTIVITY_Pos             (6)
#define I2C_SR_SLV_ACTIVITY_Msk             (0x1UL << I2C_SR_SLV_ACTIVITY_Pos)          /*!< 0x00000040 */
#define I2C_SR_SLV_ACTIVITY                 I2C_SR_SLV_ACTIVITY_Msk                     /*!< Slave FSM Activity Status. */
#define I2C_SR_MST_HOLD_TX_FIFO_EMPTY_Pos   (7)
#define I2C_SR_MST_HOLD_TX_FIFO_EMPTY_Msk   (0x1UL << I2C_SR_MST_HOLD_TX_FIFO_EMPTY_Pos)/*!< 0x00000080 */
#define I2C_SR_MST_HOLD_TX_FIFO_EMPTY       I2C_SR_MST_HOLD_TX_FIFO_EMPTY_Msk           /*!< Master holds the bus due to Tx FIFO is empty. */
#define I2C_SR_MST_HOLD_RX_FIFO_FULL_Pos    (8)
#define I2C_SR_MST_HOLD_RX_FIFO_FULL_Msk    (0x1UL << I2C_SR_MST_HOLD_RX_FIFO_FULL_Pos) /*!< 0x00000100 */
#define I2C_SR_MST_HOLD_RX_FIFO_FULL        I2C_SR_MST_HOLD_RX_FIFO_FULL_Msk            /*!< Master holds the bus due to Rx FIFO is full. */
#define I2C_SR_SLV_HOLD_TX_FIFO_EMPTY_Pos   (9)
#define I2C_SR_SLV_HOLD_TX_FIFO_EMPTY_Msk   (0x1UL << I2C_SR_SLV_HOLD_TX_FIFO_EMPTY_Pos)/*!< 0x00000200 */
#define I2C_SR_SLV_HOLD_TX_FIFO_EMPTY       I2C_SR_SLV_HOLD_TX_FIFO_EMPTY_Msk           /*!< Slave holds the bus due to Tx FIFO is empty. */
#define I2C_SR_SLV_HOLD_RX_FIFO_FULL_Pos    (10)
#define I2C_SR_SLV_HOLD_RX_FIFO_FULL_Msk    (0x1UL << I2C_SR_SLV_HOLD_RX_FIFO_FULL_Pos) /*!< 0x00000400 */
#define I2C_SR_SLV_HOLD_RX_FIFO_FULL        I2C_SR_SLV_HOLD_RX_FIFO_FULL_Msk            /*!< Slave holds the bus due to Rx FIFO is full. */
#define I2C_SR_SDA_STUCK_NOT_RECOVERED_Pos  (11)
#define I2C_SR_SDA_STUCK_NOT_RECOVERED_Msk  (0x1UL << I2C_SR_SDA_STUCK_NOT_RECOVERED_Pos)/*!< 0x00000800 */
#define I2C_SR_SDA_STUCK_NOT_RECOVERED      I2C_SR_SDA_STUCK_NOT_RECOVERED_Msk           /*!< SDA Stuck at low is recovered after recovery mechanism. */
#define I2C_SR_SMBUS_QUICK_CMD_BIT_Pos          (16)
#define I2C_SR_SMBUS_QUICK_CMD_BIT_Msk          (0x1UL << I2C_SR_SMBUS_QUICK_CMD_BIT_Pos)       /*!< 0x00010000 */
#define I2C_SR_SMBUS_QUICK_CMD_BIT              I2C_SR_SMBUS_QUICK_CMD_BIT_Msk                  /*!< This bit indicates the R/W bit of the Quick command received. */
#define I2C_SR_SMBUS_SLAVE_ADDR_VALID_Pos       (17)
#define I2C_SR_SMBUS_SLAVE_ADDR_VALID_Msk       (0x1UL << I2C_SR_SMBUS_SLAVE_ADDR_VALID_Pos)    /*!< 0x00020000 */
#define I2C_SR_SMBUS_SLAVE_ADDR_VALID           I2C_SR_SMBUS_SLAVE_ADDR_VALID_Msk               /*!< This bit indicates whether the slave address (ic_sar) is valid or not. */
#define I2C_SR_SMBUS_SLAVE_ADDR_RESOLVED_Pos    (18)
#define I2C_SR_SMBUS_SLAVE_ADDR_RESOLVED_Msk    (0x1UL << I2C_SR_SMBUS_SLAVE_ADDR_RESOLVED_Pos) /*!< 0x00040000 */
#define I2C_SR_SMBUS_SLAVE_ADDR_RESOLVED        I2C_SR_SMBUS_SLAVE_ADDR_RESOLVED_Msk            /*!< This bit indicates whether the slave address (ic_sar) is resolved by the ARP Master. */
#define I2C_SR_SMBUS_SUSPEND_STATUS_Pos         (19)
#define I2C_SR_SMBUS_SUSPEND_STATUS_Msk         (0x1UL << I2C_SR_SMBUS_SUSPEND_STATUS_Pos)      /*!< 0x00080000 */
#define I2C_SR_SMBUS_SUSPEND_STATUS             I2C_SR_SMBUS_SUSPEND_STATUS_Msk                 /*!< This bit indicates the status of the SMBus Suspend signal. */
#define I2C_SR_SMBUS_ALERT_STATUS_Pos           (20)
#define I2C_SR_SMBUS_ALERT_STATUS_Msk           (0x1UL << I2C_SR_SMBUS_ALERT_STATUS_Pos)        /*!< 0x00100000 */
#define I2C_SR_SMBUS_ALERT_STATUS               I2C_SR_SMBUS_ALERT_STATUS_Msk                   /*!< This bit indicates the status of the SMBus Alert signal. */
/******************  Bit definition for SDA_HOLD register  ******************/
#define I2C_SDA_HOLD_TX_Pos                 (0)
#define I2C_SDA_HOLD_TX_Msk                 (0xFFFFUL << I2C_SDA_HOLD_TX_Pos)           /*!< 0x0000FFFF */
#define I2C_SDA_HOLD_TX                     I2C_SDA_HOLD_TX_Msk                         /*!< Sets the required SDA hold time in units of ic_clk period, when DW_apb_i2c acts as a transmitter. */
#define I2C_SDA_HOLD_RX_Pos                 (16)
#define I2C_SDA_HOLD_RX_Msk                 (0xFFUL << I2C_SDA_HOLD_RX_Pos)             /*!< 0x00FF0000 */
#define I2C_SDA_HOLD_RX                     I2C_SDA_HOLD_RX_Msk                         /*!< Sets the required SDA hold time in units of ic_clk period, when DW_apb_i2c acts as a receiver. */
/******************  Bit definition for TX_ABRT_SOURCE register  ******************/
#define I2C_TX_ABRT_SOURCE_7B_ADDR_NOACK_Pos        (0)
#define I2C_TX_ABRT_SOURCE_7B_ADDR_NOACK_Msk        (0x1UL << I2C_TX_ABRT_SOURCE_7B_ADDR_NOACK_Pos)     /*!< 0x00000001 */
#define I2C_TX_ABRT_SOURCE_7B_ADDR_NOACK            I2C_TX_ABRT_SOURCE_7B_ADDR_NOACK_Msk                /*!< This abort is generated because of NOACK for 7-bit address. */
#define I2C_TX_ABRT_SOURCE_10ADDR1_NOACK_Pos        (1)
#define I2C_TX_ABRT_SOURCE_10ADDR1_NOACK_Msk        (0x1UL << I2C_TX_ABRT_SOURCE_10ADDR1_NOACK_Pos)     /*!< 0x00000002 */
#define I2C_TX_ABRT_SOURCE_10ADDR1_NOACK            I2C_TX_ABRT_SOURCE_10ADDR1_NOACK_Msk                /*!< Byte 1 of 10Bit Address not ACKed by any slave. */
#define I2C_TX_ABRT_SOURCE_10ADDR2_NOACK_Pos        (2)
#define I2C_TX_ABRT_SOURCE_10ADDR2_NOACK_Msk        (0x1UL << I2C_TX_ABRT_SOURCE_10ADDR2_NOACK_Pos)     /*!< 0x00000004 */
#define I2C_TX_ABRT_SOURCE_10ADDR2_NOACK            I2C_TX_ABRT_SOURCE_10ADDR2_NOACK_Msk                /*!< Byte 2 of 10Bit Address not ACKed by any slave. */
#define I2C_TX_ABRT_SOURCE_TXDATA_NOACK_Pos         (3)
#define I2C_TX_ABRT_SOURCE_TXDATA_NOACK_Msk         (0x1UL << I2C_TX_ABRT_SOURCE_TXDATA_NOACK_Pos)      /*!< 0x00000008 */
#define I2C_TX_ABRT_SOURCE_TXDATA_NOACK             I2C_TX_ABRT_SOURCE_TXDATA_NOACK_Msk                 /*!< Transmitted data not ACKed by addressed slave. */
#define I2C_TX_ABRT_SOURCE_GCALL_NOACK_Pos          (4)
#define I2C_TX_ABRT_SOURCE_GCALL_NOACK_Msk          (0x1UL << I2C_TX_ABRT_SOURCE_GCALL_NOACK_Pos)       /*!< 0x00000010 */
#define I2C_TX_ABRT_SOURCE_GCALL_NOACK              I2C_TX_ABRT_SOURCE_GCALL_NOACK_Msk                  /*!< GCALL not ACKed by any slave. */
#define I2C_TX_ABRT_SOURCE_GCALL_READ_Pos           (5)
#define I2C_TX_ABRT_SOURCE_GCALL_READ_Msk           (0x1UL << I2C_TX_ABRT_SOURCE_GCALL_READ_Pos)        /*!< 0x00000020 */
#define I2C_TX_ABRT_SOURCE_GCALL_READ               I2C_TX_ABRT_SOURCE_GCALL_READ_Msk                   /*!< GCALL is followed by read from bus. */
#define I2C_TX_ABRT_SOURCE_HS_ACKDET_Pos            (6)
#define I2C_TX_ABRT_SOURCE_HS_ACKDET_Msk            (0x1UL << I2C_TX_ABRT_SOURCE_HS_ACKDET_Pos)         /*!< 0x00000040 */
#define I2C_TX_ABRT_SOURCE_HS_ACKDET                I2C_TX_ABRT_SOURCE_HS_ACKDET_Msk                    /*!< HS Master code ACKed in HS Mode. */
#define I2C_TX_ABRT_SOURCE_SBYTE_ACKDET_Pos         (7)
#define I2C_TX_ABRT_SOURCE_SBYTE_ACKDET_Msk         (0x1UL << I2C_TX_ABRT_SOURCE_SBYTE_ACKDET_Pos)      /*!< 0x00000080 */
#define I2C_TX_ABRT_SOURCE_SBYTE_ACKDET             I2C_TX_ABRT_SOURCE_SBYTE_ACKDET_Msk                 /*!< ACK detected for START byte. */
#define I2C_TX_ABRT_SOURCE_HS_NORSTRT_Pos           (8)
#define I2C_TX_ABRT_SOURCE_HS_NORSTRT_Msk           (0x1UL << I2C_TX_ABRT_SOURCE_HS_NORSTRT_Pos)        /*!< 0x00000100 */
#define I2C_TX_ABRT_SOURCE_HS_NORSTRT               I2C_TX_ABRT_SOURCE_HS_NORSTRT_Msk                   /*!< User trying to switch Master to HS mode when RESTART disabled. */
#define I2C_TX_ABRT_SOURCE_SBYTE_NORSTRT_Pos        (9)
#define I2C_TX_ABRT_SOURCE_SBYTE_NORSTRT_Msk        (0x1UL << I2C_TX_ABRT_SOURCE_SBYTE_NORSTRT_Pos)     /*!< 0x00000200 */
#define I2C_TX_ABRT_SOURCE_SBYTE_NORSTRT            I2C_TX_ABRT_SOURCE_SBYTE_NORSTRT_Msk                /*!< User trying to send START byte when RESTART disabled. */
#define I2C_TX_ABRT_SOURCE_10B_RD_NORSTRT_Pos       (10)
#define I2C_TX_ABRT_SOURCE_10B_RD_NORSTRT_Msk       (0x1UL << I2C_TX_ABRT_SOURCE_10B_RD_NORSTRT_Pos)    /*!< 0x00000400 */
#define I2C_TX_ABRT_SOURCE_10B_RD_NORSTRT           I2C_TX_ABRT_SOURCE_10B_RD_NORSTRT_Msk               /*!< Master trying to read in 10Bit addressing mode when RESTART disabled. */
#define I2C_TX_ABRT_SOURCE_MASTER_DIS_Pos           (11)
#define I2C_TX_ABRT_SOURCE_MASTER_DIS_Msk           (0x1UL << I2C_TX_ABRT_SOURCE_MASTER_DIS_Pos)        /*!< 0x00000800 */
#define I2C_TX_ABRT_SOURCE_MASTER_DIS               I2C_TX_ABRT_SOURCE_MASTER_DIS_Msk                   /*!< User intitating master operation when MASTER disabled. */
#define I2C_TX_ABRT_SOURCE_LOST_Pos                 (12)
#define I2C_TX_ABRT_SOURCE_LOST_Msk                 (0x1UL << I2C_TX_ABRT_SOURCE_LOST_Pos)              /*!< 0x00001000 */
#define I2C_TX_ABRT_SOURCE_LOST                     I2C_TX_ABRT_SOURCE_LOST_Msk                         /*!< Master or SlaveTransmitter lost arbitration. */
#define I2C_TX_ABRT_SOURCE_SLVFLUSH_TXFIFO_Pos      (13)
#define I2C_TX_ABRT_SOURCE_SLVFLUSH_TXFIFO_Msk      (0x1UL << I2C_TX_ABRT_SOURCE_SLVFLUSH_TXFIFO_Pos)   /*!< 0x00002000 */
#define I2C_TX_ABRT_SOURCE_SLVFLUSH_TXFIFO          I2C_TX_ABRT_SOURCE_SLVFLUSH_TXFIFO_Msk              /*!< Slave flushes existing data in TX-FIFO upon getting read command. */
#define I2C_TX_ABRT_SOURCE_SLV_ARBLOST_Pos          (14)
#define I2C_TX_ABRT_SOURCE_SLV_ARBLOST_Msk          (0x1UL << I2C_TX_ABRT_SOURCE_SLV_ARBLOST_Pos)       /*!< 0x00004000 */
#define I2C_TX_ABRT_SOURCE_SLV_ARBLOST              I2C_TX_ABRT_SOURCE_SLV_ARBLOST_Msk                  /*!< Slave lost arbitration to remote master. */
#define I2C_TX_ABRT_SOURCE_SLVRD_INTX_Pos           (15)
#define I2C_TX_ABRT_SOURCE_SLVRD_INTX_Msk           (0x1UL << I2C_TX_ABRT_SOURCE_SLVRD_INTX_Pos)        /*!< 0x00008000 */
#define I2C_TX_ABRT_SOURCE_SLVRD_INTX               I2C_TX_ABRT_SOURCE_SLVRD_INTX_Msk                   /*!< Slave trying to transmit to remote master in read mode. */
#define I2C_TX_ABRT_SOURCE_USER_ABRT_Pos            (16)
#define I2C_TX_ABRT_SOURCE_USER_ABRT_Msk            (0x1UL << I2C_TX_ABRT_SOURCE_USER_ABRT_Pos)         /*!< 0x00010000 */
#define I2C_TX_ABRT_SOURCE_USER_ABRT                I2C_TX_ABRT_SOURCE_USER_ABRT_Msk                    /*!< Transfer abort detected by master. */
#define I2C_TX_ABRT_SOURCE_SDA_STUCK_AT_LOW_Pos     (17)
#define I2C_TX_ABRT_SOURCE_SDA_STUCK_AT_LOW_Msk     (0x1UL << I2C_TX_ABRT_SOURCE_SDA_STUCK_AT_LOW_Pos)  /*!< 0x00020000 */
#define I2C_TX_ABRT_SOURCE_SDA_STUCK_AT_LOW         I2C_TX_ABRT_SOURCE_SDA_STUCK_AT_LOW_Msk             /*!< This abort is generated because of Sda stuck at low for IC_SDA_STUCK_AT_LOW_TIMEOUT value of ic_clks. */
#define I2C_TX_ABRT_SOURCE_DEVICE_NOACK_Pos         (18)
#define I2C_TX_ABRT_SOURCE_DEVICE_NOACK_Msk         (0x1UL << I2C_TX_ABRT_SOURCE_DEVICE_NOACK_Pos)      /*!< 0x00040000 */
#define I2C_TX_ABRT_SOURCE_DEVICE_NOACK             I2C_TX_ABRT_SOURCE_DEVICE_NOACK_Msk                 /*!< This abort is generated because of NOACK for DEVICE-ID. */
#define I2C_TX_ABRT_SOURCE_DEVICE_SLVADDR_NOACK_Pos (19)
#define I2C_TX_ABRT_SOURCE_DEVICE_SLVADDR_NOACK_Msk (0x1UL << I2C_TX_ABRT_SOURCE_DEVICE_SLVADDR_NOACK_Pos)  /*!< 0x00080000 */
#define I2C_TX_ABRT_SOURCE_DEVICE_SLVADDR_NOACK     I2C_TX_ABRT_SOURCE_DEVICE_SLVADDR_NOACK_Msk             /*!< This abort is generated because of NOACK for Slave address. */
#define I2C_TX_ABRT_SOURCE_DEVICE_WRITE_Pos         (20)
#define I2C_TX_ABRT_SOURCE_DEVICE_WRITE_Msk         (0x1UL << I2C_TX_ABRT_SOURCE_DEVICE_WRITE_Pos)      /*!< 0x00100000 */
#define I2C_TX_ABRT_SOURCE_DEVICE_WRITE             I2C_TX_ABRT_SOURCE_DEVICE_WRITE_Msk                 /*!< This abort is generated because of NOACK for Slave address. */
#define I2C_TX_ABRT_SOURCE_TX_FLUSH_CNT_Pos         (23)
#define I2C_TX_ABRT_SOURCE_TX_FLUSH_CNT_Msk         (0x1FFUL << I2C_TX_ABRT_SOURCE_TX_FLUSH_CNT_Pos)    /*!< 0xFF800000 */
#define I2C_TX_ABRT_SOURCE_TX_FLUSH_CNT             I2C_TX_ABRT_SOURCE_TX_FLUSH_CNT_Msk                 /*!< This field indicates the number of Tx FIFO Data Commands which are flushed due to TX_ABRT interrupt. */
/******************  Bit definition for DMA_CR register  ******************/
#define I2C_DMA_CR_RDMAE_Pos        (0)
#define I2C_DMA_CR_RDMAE_Msk        (0x1UL << I2C_DMA_CR_RDMAE_Pos)     /*!< 0x00000001 */
#define I2C_DMA_CR_RDMAE            I2C_DMA_CR_RDMAE_Msk                /*!< Receive DMA Enable. */
#define I2C_DMA_CR_TDMAE_Pos        (1)
#define I2C_DMA_CR_TDMAE_Msk        (0x1UL << I2C_DMA_CR_TDMAE_Pos)     /*!< 0x00000002 */
#define I2C_DMA_CR_TDMAE            I2C_DMA_CR_TDMAE_Msk                /*!< Transmit DMA Enable. */
/******************  Bit definition for SDA_SETUP register  ******************/
#define I2C_SDA_SETUP_Pos           (0)
#define I2C_SDA_SETUP_Msk           (0xFFUL << I2C_SDA_SETUP_Pos)       /*!< 0x000000FF */
#define I2C_SDA_SETUP               I2C_SDA_SETUP_Msk                   /*!< SDA Setup. */
/******************  Bit definition for ACK_GENERAL_CALL register  ******************/
#define I2C_ACK_GENERAL_CALL_Pos    (0)
#define I2C_ACK_GENERAL_CALL_Msk    (0x1UL << I2C_ACK_GENERAL_CALL_Pos) /*!< 0x00000001 */
#define I2C_ACK_GENERAL_CALL        I2C_ACK_GENERAL_CALL_Msk            /*!< ACK General Call. */
/******************  Bit definition for ENSR register  ******************/
#define I2C_ENSR_IC_EN_Pos                      (0)
#define I2C_ENSR_IC_EN_Msk                      (0x1UL << I2C_ENSR_IC_EN_Pos)                   /*!< 0x00000001 */
#define I2C_ENSR_IC_EN                          I2C_ENSR_IC_EN_Msk                              /*!< DW_apb_i2c is deemed to be in an enabled state. */
#define I2C_ENSR_SLV_DISABLED_WHILE_BUSY_Pos    (1)
#define I2C_ENSR_SLV_DISABLED_WHILE_BUSY_Msk    (0x1UL << I2C_ENSR_SLV_DISABLED_WHILE_BUSY_Pos) /*!< 0x00000002 */
#define I2C_ENSR_SLV_DISABLED_WHILE_BUSY        I2C_ENSR_SLV_DISABLED_WHILE_BUSY_Msk            /*!< Slave Disabled While Busy(Transmit, Receive). */
#define I2C_ENSR_SLV_RX_DATA_LOST_Pos           (2)
#define I2C_ENSR_SLV_RX_DATA_LOST_Msk           (0x1UL << I2C_ENSR_SLV_RX_DATA_LOST_Pos)        /*!< 0x00000004 */
#define I2C_ENSR_SLV_RX_DATA_LOST               I2C_ENSR_SLV_RX_DATA_LOST_Msk                   /*!< Slave Received Data Lost. */
/******************  Bit definition for FS_SPKLEN register  ******************/
#define I2C_FS_SPKLEN_Pos                       (0)
#define I2C_FS_SPKLEN_Msk                       (0xFFUL << I2C_FS_SPKLEN_Pos)                   /*!< 0x000000FF */
#define I2C_FS_SPKLEN                           I2C_FS_SPKLEN_Msk                               /*!< This register sets the duration, measured in ic_clk cycles. */
/******************  Bit definition for UFM_SPKLEN register  ******************/
#define I2C_UFM_SPKLEN_Pos                      (0)
#define I2C_UFM_SPKLEN_Msk                      (0xFFUL << I2C_UFM_SPKLEN_Pos)                  /*!< 0x000000FF */
#define I2C_UFM_SPKLEN                          I2C_UFM_SPKLEN_Msk                              /*!< This register sets the duration, measured in ic_clk cycles. */
/******************  Bit definition for HS_SPKLEN register  ******************/
#define I2C_HS_SPKLEN_Pos                       (0)
#define I2C_HS_SPKLEN_Msk                       (0xFFUL << I2C_HS_SPKLEN_Pos)                   /*!< 0x000000FF */
#define I2C_HS_SPKLEN                           I2C_HS_SPKLEN_Msk                               /*!< This register sets the duration, measured in ic_clk cycles. */
/******************  Bit definition for CLR_RESTART_DET register  ******************/
#define I2C_CLR_RESTART_DET_Pos                 (0)
#define I2C_CLR_RESTART_DET_Msk                 (0x1UL << I2C_CLR_RESTART_DET_Pos)              /*!< 0x00000001 */
#define I2C_CLR_RESTART_DET                     I2C_CLR_RESTART_DET_Msk                         /*!< Read this register to clear the RESTART_DET interrupt. */
/******************  Bit definition for CLR_SCL_STUCK_DET register  ******************/
#define I2C_CLR_SCL_STUCK_DET_Pos               (0)
#define I2C_CLR_SCL_STUCK_DET_Msk               (0x1UL << I2C_CLR_SCL_STUCK_DET_Pos)            /*!< 0x00000001 */
#define I2C_CLR_SCL_STUCK_DET                   I2C_CLR_SCL_STUCK_DET_Msk                       /*!< Read this register to clear the SCL_STUCT_AT_LOW interrupt. */
/******************  Bit definition for DEVICE_ID register  ******************/
#define I2C_DEVICE_ID_Pos                       (0)
#define I2C_DEVICE_ID_Msk                       (0xFFFFFFUL << I2C_DEVICE_ID_Pos)               /*!< 0x00FFFFFF */
#define I2C_DEVICE_ID                           I2C_DEVICE_ID_Msk                               /*!< Device-ID. */
/******************  Bit definition for OPTIONAL_SAR register  ******************/
#define I2C_OPTIONAL_SAR_Pos                    (0)
#define I2C_OPTIONAL_SAR_Msk                    (0x7FUL << I2C_OPTIONAL_SAR_Pos)                /*!< 0x0000007F */
#define I2C_OPTIONAL_SAR                        I2C_OPTIONAL_SAR_Msk                            /*!< Optional Slave address  when operating as a slave in SMBus Mode. */
/******************  Bit definition for COMP_PARAM_1 register  ******************/
#define I2C_COMP_PARAM_APB_DATA_WIDTH_Pos       (0)
#define I2C_COMP_PARAM_APB_DATA_WIDTH_Msk       (0x3UL << I2C_COMP_PARAM_APB_DATA_WIDTH_Pos)    /*!< 0x00000003 */
#define I2C_COMP_PARAM_APB_DATA_WIDTH           I2C_COMP_PARAM_APB_DATA_WIDTH_Msk               /*!< The value of this register is derived from the APB_DATA_WIDTH coreConsultant parameter. */
#define I2C_COMP_PARAM_MAX_SPEED_MODE_Pos       (2)
#define I2C_COMP_PARAM_MAX_SPEED_MODE_Msk       (0x3UL << I2C_COMP_PARAM_MAX_SPEED_MODE_Pos)    /*!< 0x0000000C */
#define I2C_COMP_PARAM_MAX_SPEED_MODE           I2C_COMP_PARAM_MAX_SPEED_MODE_Msk               /*!< The value of this register is derived from the MAX_SPEED_MODE coreConsultant parameter. */
#define I2C_COMP_PARAM_HC_COUNT_VALUES_Pos      (4)
#define I2C_COMP_PARAM_HC_COUNT_VALUES_Msk      (0x1UL << I2C_COMP_PARAM_HC_COUNT_VALUES_Pos)   /*!< 0x00000010 */
#define I2C_COMP_PARAM_HC_COUNT_VALUES          I2C_COMP_PARAM_HC_COUNT_VALUES_Msk              /*!< The value of this register is derived from the HC_COUNT_VALUES coreConsultant parameter. */
#define I2C_COMP_PARAM_INTR_IO_Pos              (5)
#define I2C_COMP_PARAM_INTR_IO_Msk              (0x1UL << I2C_COMP_PARAM_INTR_IO_Pos)           /*!< 0x00000020 */
#define I2C_COMP_PARAM_INTR_IO                  I2C_COMP_PARAM_INTR_IO_Msk                      /*!< The value of this register is derived from the INTR_IO coreConsultant parameter. */
#define I2C_COMP_PARAM_HAS_DMA_Pos              (6)
#define I2C_COMP_PARAM_HAS_DMA_Msk              (0x1UL << I2C_COMP_PARAM_HAS_DMA_Pos)           /*!< 0x00000040 */
#define I2C_COMP_PARAM_HAS_DMA                  I2C_COMP_PARAM_HAS_DMA_Msk                      /*!< The value of this register is derived from the HAS_DMA coreConsultant parameter. */
#define I2C_COMP_PARAM_ADD_ENCODED_PARAMS_Pos   (7)
#define I2C_COMP_PARAM_ADD_ENCODED_PARAMS_Msk   (0x1UL << I2C_COMP_PARAM_ADD_ENCODED_PARAMS_Pos)/*!< 0x00000080 */
#define I2C_COMP_PARAM_ADD_ENCODED_PARAMS       I2C_COMP_PARAM_ADD_ENCODED_PARAMS_Msk           /*!< The value of this register is derived from the ADD_ENCODED_PARAMS coreConsultant parameter. */
#define I2C_COMP_PARAM_RX_BUFFER_DEPTH_Pos      (8)
#define I2C_COMP_PARAM_RX_BUFFER_DEPTH_Msk      (0xFFUL << I2C_COMP_PARAM_RX_BUFFER_DEPTH_Pos)  /*!< 0x0000FF00 */
#define I2C_COMP_PARAM_RX_BUFFER_DEPTH          I2C_COMP_PARAM_RX_BUFFER_DEPTH_Msk              /*!< The value of this register is derived from the IC_RX_BUFFER_DEPTH coreConsultant parameter. */
#define I2C_COMP_PARAM_TX_BUFFER_DEPTH_Pos      (16)
#define I2C_COMP_PARAM_TX_BUFFER_DEPTH_Msk      (0xFFUL << I2C_COMP_PARAM_TX_BUFFER_DEPTH_Pos)  /*!< 0x00FF0000 */
#define I2C_COMP_PARAM_TX_BUFFER_DEPTH          I2C_COMP_PARAM_TX_BUFFER_DEPTH_Msk              /*!< The value of this register is derived from the IC_TX_BUFFER_DEPTH coreConsultant parameter. */

/******************************************************************************/
/*                                                                            */
/*                    Controller Area Network                                 */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for MOD register  ******************/
#define CAN_MOD_RESET_Pos                   (0)
#define CAN_MOD_RESET_Msk                   (0x1UL << CAN_MOD_RESET_Pos)        /*!< 0x00000001 */
#define CAN_MOD_RESET                       CAN_MOD_RESET_Msk                   /*!< Reset Mode. */
#define CAN_MOD_LOM_Pos                     (1)
#define CAN_MOD_LOM_Msk                     (0x1UL << CAN_MOD_LOM_Pos)          /*!< 0x00000002 */
#define CAN_MOD_LOM                         CAN_MOD_LOM_Msk                     /*!< Listen Only Mode. */
#define CAN_MOD_STM_Pos                     (2)
#define CAN_MOD_STM_Msk                     (0x1UL << CAN_MOD_STM_Pos)          /*!< 0x00000004 */
#define CAN_MOD_STM                         CAN_MOD_STM_Msk                     /*!< Self Test Mode. */
#define CAN_MOD_AFM_Pos                     (3)
#define CAN_MOD_AFM_Msk                     (0x1UL << CAN_MOD_AFM_Pos)          /*!< 0x00000008 */
#define CAN_MOD_AFM                         CAN_MOD_AFM_Msk                     /*!< Acceptance Filter Mode. */
#define CAN_MOD_SM_Pos                      (4)
#define CAN_MOD_SM_Msk                      (0x1UL << CAN_MOD_SM_Pos)          /*!< 0x00000010 */
#define CAN_MOD_SM                          CAN_MOD_SM_Msk                     /*!< Sleep Mode. */
/******************  Bit definition for IR register  ******************/
#define CAN_IR_RI_Pos                       (0)
#define CAN_IR_RI_Msk                       (0x1UL << CAN_IR_RI_Pos)        /*!< 0x00000001 */
#define CAN_IR_RI                           CAN_IR_RI_Msk                   /*!< Receive Interrupt. */
#define CAN_IR_TI_Pos                       (1)
#define CAN_IR_TI_Msk                       (0x1UL << CAN_IR_TI_Pos)        /*!< 0x00000002 */
#define CAN_IR_TI                           CAN_IR_TI_Msk                   /*!< Transmit Interrupt. */
#define CAN_IR_EI_Pos                       (2)
#define CAN_IR_EI_Msk                       (0x1UL << CAN_IR_EI_Pos)        /*!< 0x00000004 */
#define CAN_IR_EI                           CAN_IR_EI_Msk                   /*!< Error Warning Interrupt. */
#define CAN_IR_DOI_Pos                      (3)
#define CAN_IR_DOI_Msk                      (0x1UL << CAN_IR_DOI_Pos)       /*!< 0x00000008 */
#define CAN_IR_DOI                          CAN_IR_DOI_Msk                  /*!< Data Overrun Interrupt. */
#define CAN_IR_WUI_Pos                      (4)
#define CAN_IR_WUI_Msk                      (0x1UL << CAN_IR_WUI_Pos)       /*!< 0x00000010 */
#define CAN_IR_WUI                          CAN_IR_WUI_Msk                  /*!< Wake-Up Interrupt. */
#define CAN_IR_EPI_Pos                      (5)
#define CAN_IR_EPI_Msk                      (0x1UL << CAN_IR_EPI_Pos)       /*!< 0x00000020 */
#define CAN_IR_EPI                          CAN_IR_EPI_Msk                  /*!< Error Passive Interrupt. */
#define CAN_IR_ALI_Pos                      (6)
#define CAN_IR_ALI_Msk                      (0x1UL << CAN_IR_ALI_Pos)        /*!< 0x00000040 */
#define CAN_IR_ALI                          CAN_IR_ALI_Msk                   /*!< Arbitration Lost Interrupt. */
#define CAN_IR_BEI_Pos                      (7)
#define CAN_IR_BEI_Msk                      (0x1UL << CAN_IR_BEI_Pos)        /*!< 0x00000080 */
#define CAN_IR_BEI                          CAN_IR_BEI_Msk                   /*!< Bus Error Interrupt. */
/******************  Bit definition for SR register  ******************/
#define CAN_SR_RBS_Pos                     (0)
#define CAN_SR_RBS_Msk                     (0x1UL << CAN_SR_RBS_Pos)                      /*!< 0x00000001 */
#define CAN_SR_RBS                         CAN_SR_RBS_Msk                                 /*!< Receive Buffer Full Status. */
#define CAN_SR_DOS_Pos                     (1)
#define CAN_SR_DOS_Msk                     (0x1UL << CAN_SR_DOS_Pos)                      /*!< 0x00000002 */
#define CAN_SR_DOS                         CAN_SR_DOS_Msk                                 /*!< Data Overrun Status. */
#define CAN_SR_TBS_Pos                     (2)
#define CAN_SR_TBS_Msk                     (0x1UL << CAN_SR_TBS_Pos)                      /*!< 0x00000004 */
#define CAN_SR_TBS                         CAN_SR_TBS_Msk                                 /*!< Transmit Buffer Released Status. */
#define CAN_SR_TCS_Pos                     (3)
#define CAN_SR_TCS_Msk                     (0x1UL << CAN_SR_TCS_Pos)                      /*!< 0x00000008 */
#define CAN_SR_TCS                         CAN_SR_TCS_Msk                                 /*!< Transmit Complete Status. */
#define CAN_SR_RS_Pos                      (4)
#define CAN_SR_RS_Msk                      (0x1UL << CAN_SR_RS_Pos)                       /*!< 0x00000010 */
#define CAN_SR_RS                          CAN_SR_RS_Msk                                  /*!< The MCAN2 is in the process of receiving a message. */
#define CAN_SR_TS_Pos                      (5)
#define CAN_SR_TS_Msk                      (0x1UL << CAN_SR_TS_Pos)                       /*!< 0x00000020 */
#define CAN_SR_TS                          CAN_SR_TS_Msk                                  /*!< The MCAN2 is in the process of transmitting a message. */
#define CAN_SR_ES_Pos                      (6)
#define CAN_SR_ES_Msk                      (0x1UL << CAN_SR_ES_Pos)                       /*!< 0x00000040 */
#define CAN_SR_ES                          CAN_SR_ES_Msk                                  /*!< Error Status. */
#define CAN_SR_BS_Pos                      (7)
#define CAN_SR_BS_Msk                      (0x1UL << CAN_SR_BS_Pos)                       /*!< 0x00000080 */
#define CAN_SR_BS                          CAN_SR_BS_Msk                                  /*!< Bus Off Status. */
/******************  Bit definition for CMR register  ******************/
#define CAN_CMR_TR_Pos                      (0)
#define CAN_CMR_TR_Msk                      (0x1UL << CAN_CMR_TR_Pos)           /*!< 0x00000001 */
#define CAN_CMR_TR                          CAN_CMR_TR_Msk                      /*!< Transmission Request. */
#define CAN_CMR_AT_Pos                      (1)
#define CAN_CMR_AT_Msk                      (0x1UL << CAN_CMR_AT_Pos)           /*!< 0x00000002 */
#define CAN_CMR_AT                          CAN_CMR_AT_Msk                      /*!< Abort Transmission. */
#define CAN_CMR_RRB_Pos                     (2)
#define CAN_CMR_RRB_Msk                     (0x1UL << CAN_CMR_RRB_Pos)          /*!< 0x00000004 */
#define CAN_CMR_RRB                         CAN_CMR_RRB_Msk                     /*!< Release Receive Buffer. */
#define CAN_CMR_CDO_Pos                     (3)
#define CAN_CMR_CDO_Msk                     (0x1UL << CAN_CMR_CDO_Pos)          /*!< 0x00000008 */
#define CAN_CMR_CDO                         CAN_CMR_CDO_Msk                     /*!< Clear Data Overrun. */
#define CAN_CMR_SRR_Pos                     (4)
#define CAN_CMR_SRR_Msk                     (0x1UL << CAN_CMR_SRR_Pos)          /*!< 0x00000010 */
#define CAN_CMR_SRR                         CAN_CMR_SRR_Msk                     /*!< Self Reception Request. */

/******************************************************************************/
/*                                                                            */
/*                    Controller Area Network and Flexible Data-Rate          */
/*                                                                            */
/******************************************************************************/
typedef struct
{
  __IO uint32_t CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
  __IO uint32_t CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
  __IO uint32_t SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
  __IO uint32_t DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  __IO uint32_t SR;          /*!< TIM status register,                 Address offset: 0x10 */
  __IO uint32_t EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
  __IO uint32_t CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  __IO uint32_t CCMR2;       /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  __IO uint32_t CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
  __IO uint32_t CNT;         /*!< TIM counter register,                Address offset: 0x24 */
  __IO uint32_t PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
  __IO uint32_t ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
  __IO uint32_t RCR;         /*!< TIM repetition counter register,     Address offset: 0x30 */
  __IO uint32_t CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  __IO uint32_t CCR2;        /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  __IO uint32_t CCR3;        /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  __IO uint32_t CCR4;        /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  __IO uint32_t BDTR;        /*!< TIM break and dead-time register,    Address offset: 0x44 */
  __IO uint32_t DCR;         /*!< TIM DMA control register,            Address offset: 0x48 */
  __IO uint32_t DMAR;        /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  __IO uint32_t OR;          /*!< TIM option register,                 Address offset: 0x50 */
} TIM_TypeDef;

#define TIM0_BASE           (0x40003000UL)
#define TIM1_BASE           (0x40004000UL)

#define TIM0  ((TIM_TypeDef*)TIM0_BASE)
#define TIM1  ((TIM_TypeDef*)TIM1_BASE)

/****************** TIM Instances : All supported instances *******************/
#define IS_TIM_INSTANCE(INSTANCE)  (((INSTANCE) == TIM0) || \
                                    ((INSTANCE) == TIM1))

/****************** TIM Instances : supporting counting mode selection ********/
#define IS_TIM_COUNTER_MODE_SELECT_INSTANCE(INSTANCE)  (((INSTANCE) == TIM0) || \
                                                        ((INSTANCE) == TIM1))

/****************** TIM Instances : supporting clock division *****************/
#define IS_TIM_CLOCK_DIVISION_INSTANCE(INSTANCE) (((INSTANCE) == TIM0) || \
                                                  ((INSTANCE) == TIM1))

/****************** TIM Instances : supporting repetition counter *************/
#define IS_TIM_REPETITION_COUNTER_INSTANCE(INSTANCE)    0
//#define IS_TIM_REPETITION_COUNTER_INSTANCE(INSTANCE)  (((INSTANCE) == TIM0) || \
//                                                       ((INSTANCE) == TIM1))

/*********** TIM Instances : Slave mode available (TIMx_SMCR available )*******/
#define IS_TIM_SLAVE_INSTANCE(INSTANCE) (((INSTANCE) == TIM0) || \
                                         ((INSTANCE) == TIM1))

/****************** TIM Instances : supporting the break function *************/
#define IS_TIM_BREAK_INSTANCE(INSTANCE)  0
//#define IS_TIM_BREAK_INSTANCE(INSTANCE)  (((INSTANCE) == TIM0) || \
//                                          ((INSTANCE) == TIM1))

/************ TIM Instances : complementary output(s) available ***************/
#define IS_TIM_CCXN_INSTANCE(INSTANCE, CHANNEL) \
   ((((INSTANCE) == TIM0) &&                    \
     (((CHANNEL) == TIM_CHANNEL_1) ||           \
      ((CHANNEL) == TIM_CHANNEL_2) ||           \
      ((CHANNEL) == TIM_CHANNEL_3)))            \
    ||                                          \
    (((INSTANCE) == TIM1) &&                    \
     (((CHANNEL) == TIM_CHANNEL_1) ||           \
      ((CHANNEL) == TIM_CHANNEL_2) ||           \
      ((CHANNEL) == TIM_CHANNEL_3))))

/************* TIM Instances : at least 1 capture/compare channel *************/
#define IS_TIM_CC1_INSTANCE(INSTANCE)   (((INSTANCE) == TIM0)  || \
                                         ((INSTANCE) == TIM1))

/************ TIM Instances : at least 2 capture/compare channels *************/
#define IS_TIM_CC2_INSTANCE(INSTANCE) (((INSTANCE) == TIM0) || \
                                       ((INSTANCE) == TIM1))

/************ TIM Instances : at least 3 capture/compare channels *************/
#define IS_TIM_CC3_INSTANCE(INSTANCE)   (((INSTANCE) == TIM0) || \
                                         ((INSTANCE) == TIM1))

/************ TIM Instances : at least 4 capture/compare channels *************/
#define IS_TIM_CC4_INSTANCE(INSTANCE) (((INSTANCE) == TIM0) || \
                                       ((INSTANCE) == TIM1))
/******************************************************************************/
/*                                                                            */
/*                                    TIM                                     */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for TIM_CR1 register  ********************/
#define TIM_CR1_CEN_Pos           (0U)
#define TIM_CR1_CEN_Msk           (0x1UL << TIM_CR1_CEN_Pos)                    /*!< 0x00000001 */
#define TIM_CR1_CEN               TIM_CR1_CEN_Msk                              /*!<Counter enable        */
#define TIM_CR1_UDIS_Pos          (1U)
#define TIM_CR1_UDIS_Msk          (0x1UL << TIM_CR1_UDIS_Pos)                   /*!< 0x00000002 */
#define TIM_CR1_UDIS              TIM_CR1_UDIS_Msk                             /*!<Update disable        */
#define TIM_CR1_URS_Pos           (2U)
#define TIM_CR1_URS_Msk           (0x1UL << TIM_CR1_URS_Pos)                    /*!< 0x00000004 */
#define TIM_CR1_URS               TIM_CR1_URS_Msk                              /*!<Update request source */
#define TIM_CR1_OPM_Pos           (3U)
#define TIM_CR1_OPM_Msk           (0x1UL << TIM_CR1_OPM_Pos)                    /*!< 0x00000008 */
#define TIM_CR1_OPM               TIM_CR1_OPM_Msk                              /*!<One pulse mode        */
#define TIM_CR1_DIR_Pos           (4U)
#define TIM_CR1_DIR_Msk           (0x1UL << TIM_CR1_DIR_Pos)                    /*!< 0x00000010 */
#define TIM_CR1_DIR               TIM_CR1_DIR_Msk                              /*!<Direction             */

#define TIM_CR1_CMS_Pos           (5U)
#define TIM_CR1_CMS_Msk           (0x3UL << TIM_CR1_CMS_Pos)                    /*!< 0x00000060 */
#define TIM_CR1_CMS               TIM_CR1_CMS_Msk                              /*!<CMS[1:0] bits (Center-aligned mode selection) */
#define TIM_CR1_CMS_0             (0x1UL << TIM_CR1_CMS_Pos)                    /*!< 0x0020 */
#define TIM_CR1_CMS_1             (0x2UL << TIM_CR1_CMS_Pos)                    /*!< 0x0040 */

#define TIM_CR1_ARPE_Pos          (7U)
#define TIM_CR1_ARPE_Msk          (0x1UL << TIM_CR1_ARPE_Pos)                   /*!< 0x00000080 */
#define TIM_CR1_ARPE              TIM_CR1_ARPE_Msk                             /*!<Auto-reload preload enable     */

#define TIM_CR1_CKD_Pos           (8U)
#define TIM_CR1_CKD_Msk           (0x3UL << TIM_CR1_CKD_Pos)                    /*!< 0x00000300 */
#define TIM_CR1_CKD               TIM_CR1_CKD_Msk                              /*!<CKD[1:0] bits (clock division) */
#define TIM_CR1_CKD_0             (0x1UL << TIM_CR1_CKD_Pos)                    /*!< 0x0100 */
#define TIM_CR1_CKD_1             (0x2UL << TIM_CR1_CKD_Pos)                    /*!< 0x0200 */

/*******************  Bit definition for TIM_CR2 register  ********************/
#define TIM_CR2_CCPC_Pos          (0U)
#define TIM_CR2_CCPC_Msk          (0x1UL << TIM_CR2_CCPC_Pos)                   /*!< 0x00000001 */
#define TIM_CR2_CCPC              TIM_CR2_CCPC_Msk                             /*!<Capture/Compare Preloaded Control        */
#define TIM_CR2_CCUS_Pos          (2U)
#define TIM_CR2_CCUS_Msk          (0x1UL << TIM_CR2_CCUS_Pos)                   /*!< 0x00000004 */
#define TIM_CR2_CCUS              TIM_CR2_CCUS_Msk                             /*!<Capture/Compare Control Update Selection */
#define TIM_CR2_CCDS_Pos          (3U)
#define TIM_CR2_CCDS_Msk          (0x1UL << TIM_CR2_CCDS_Pos)                   /*!< 0x00000008 */
#define TIM_CR2_CCDS              TIM_CR2_CCDS_Msk                             /*!<Capture/Compare DMA Selection            */

#define TIM_CR2_MMS_Pos           (4U)
#define TIM_CR2_MMS_Msk           (0x7UL << TIM_CR2_MMS_Pos)                    /*!< 0x00000070 */
#define TIM_CR2_MMS               TIM_CR2_MMS_Msk                              /*!<MMS[2:0] bits (Master Mode Selection) */
#define TIM_CR2_MMS_0             (0x1UL << TIM_CR2_MMS_Pos)                    /*!< 0x0010 */
#define TIM_CR2_MMS_1             (0x2UL << TIM_CR2_MMS_Pos)                    /*!< 0x0020 */
#define TIM_CR2_MMS_2             (0x4UL << TIM_CR2_MMS_Pos)                    /*!< 0x0040 */

#define TIM_CR2_TI1S_Pos          (7U)
#define TIM_CR2_TI1S_Msk          (0x1UL << TIM_CR2_TI1S_Pos)                   /*!< 0x00000080 */
#define TIM_CR2_TI1S              TIM_CR2_TI1S_Msk                             /*!<TI1 Selection */
#define TIM_CR2_OIS1_Pos          (8U)
#define TIM_CR2_OIS1_Msk          (0x1UL << TIM_CR2_OIS1_Pos)                   /*!< 0x00000100 */
#define TIM_CR2_OIS1              TIM_CR2_OIS1_Msk                             /*!<Output Idle state 1 (OC1 output)  */
#define TIM_CR2_OIS1N_Pos         (9U)
#define TIM_CR2_OIS1N_Msk         (0x1UL << TIM_CR2_OIS1N_Pos)                  /*!< 0x00000200 */
#define TIM_CR2_OIS1N             TIM_CR2_OIS1N_Msk                            /*!<Output Idle state 1 (OC1N output) */
#define TIM_CR2_OIS2_Pos          (10U)
#define TIM_CR2_OIS2_Msk          (0x1UL << TIM_CR2_OIS2_Pos)                   /*!< 0x00000400 */
#define TIM_CR2_OIS2              TIM_CR2_OIS2_Msk                             /*!<Output Idle state 2 (OC2 output)  */
#define TIM_CR2_OIS2N_Pos         (11U)
#define TIM_CR2_OIS2N_Msk         (0x1UL << TIM_CR2_OIS2N_Pos)                  /*!< 0x00000800 */
#define TIM_CR2_OIS2N             TIM_CR2_OIS2N_Msk                            /*!<Output Idle state 2 (OC2N output) */
#define TIM_CR2_OIS3_Pos          (12U)
#define TIM_CR2_OIS3_Msk          (0x1UL << TIM_CR2_OIS3_Pos)                   /*!< 0x00001000 */
#define TIM_CR2_OIS3              TIM_CR2_OIS3_Msk                             /*!<Output Idle state 3 (OC3 output)  */
#define TIM_CR2_OIS3N_Pos         (13U)
#define TIM_CR2_OIS3N_Msk         (0x1UL << TIM_CR2_OIS3N_Pos)                  /*!< 0x00002000 */
#define TIM_CR2_OIS3N             TIM_CR2_OIS3N_Msk                            /*!<Output Idle state 3 (OC3N output) */
#define TIM_CR2_OIS4_Pos          (14U)
#define TIM_CR2_OIS4_Msk          (0x1UL << TIM_CR2_OIS4_Pos)                   /*!< 0x00004000 */
#define TIM_CR2_OIS4              TIM_CR2_OIS4_Msk                             /*!<Output Idle state 4 (OC4 output)  */

/*******************  Bit definition for TIM_SMCR register  *******************/
#define TIM_SMCR_SMS_Pos          (0U)
#define TIM_SMCR_SMS_Msk          (0x7UL << TIM_SMCR_SMS_Pos)                   /*!< 0x00000007 */
#define TIM_SMCR_SMS              TIM_SMCR_SMS_Msk                             /*!<SMS[2:0] bits (Slave mode selection)    */
#define TIM_SMCR_SMS_0            (0x1UL << TIM_SMCR_SMS_Pos)                   /*!< 0x0001 */
#define TIM_SMCR_SMS_1            (0x2UL << TIM_SMCR_SMS_Pos)                   /*!< 0x0002 */
#define TIM_SMCR_SMS_2            (0x4UL << TIM_SMCR_SMS_Pos)                   /*!< 0x0004 */

#define TIM_SMCR_TS_Pos           (4U)
#define TIM_SMCR_TS_Msk           (0x7UL << TIM_SMCR_TS_Pos)                    /*!< 0x00000070 */
#define TIM_SMCR_TS               TIM_SMCR_TS_Msk                              /*!<TS[2:0] bits (Trigger selection)        */
#define TIM_SMCR_TS_0             (0x1UL << TIM_SMCR_TS_Pos)                    /*!< 0x0010 */
#define TIM_SMCR_TS_1             (0x2UL << TIM_SMCR_TS_Pos)                    /*!< 0x0020 */
#define TIM_SMCR_TS_2             (0x4UL << TIM_SMCR_TS_Pos)                    /*!< 0x0040 */

#define TIM_SMCR_MSM_Pos          (7U)
#define TIM_SMCR_MSM_Msk          (0x1UL << TIM_SMCR_MSM_Pos)                   /*!< 0x00000080 */
#define TIM_SMCR_MSM              TIM_SMCR_MSM_Msk                             /*!<Master/slave mode                       */

#define TIM_SMCR_ETF_Pos          (8U)
#define TIM_SMCR_ETF_Msk          (0xFUL << TIM_SMCR_ETF_Pos)                   /*!< 0x00000F00 */
#define TIM_SMCR_ETF              TIM_SMCR_ETF_Msk                             /*!<ETF[3:0] bits (External trigger filter) */
#define TIM_SMCR_ETF_0            (0x1UL << TIM_SMCR_ETF_Pos)                   /*!< 0x0100 */
#define TIM_SMCR_ETF_1            (0x2UL << TIM_SMCR_ETF_Pos)                   /*!< 0x0200 */
#define TIM_SMCR_ETF_2            (0x4UL << TIM_SMCR_ETF_Pos)                   /*!< 0x0400 */
#define TIM_SMCR_ETF_3            (0x8UL << TIM_SMCR_ETF_Pos)                   /*!< 0x0800 */

#define TIM_SMCR_ETPS_Pos         (12U)
#define TIM_SMCR_ETPS_Msk         (0x3UL << TIM_SMCR_ETPS_Pos)                  /*!< 0x00003000 */
#define TIM_SMCR_ETPS             TIM_SMCR_ETPS_Msk                            /*!<ETPS[1:0] bits (External trigger prescaler) */
#define TIM_SMCR_ETPS_0           (0x1UL << TIM_SMCR_ETPS_Pos)                  /*!< 0x1000 */
#define TIM_SMCR_ETPS_1           (0x2UL << TIM_SMCR_ETPS_Pos)                  /*!< 0x2000 */

#define TIM_SMCR_ECE_Pos          (14U)
#define TIM_SMCR_ECE_Msk          (0x1UL << TIM_SMCR_ECE_Pos)                   /*!< 0x00004000 */
#define TIM_SMCR_ECE              TIM_SMCR_ECE_Msk                             /*!<External clock enable     */
#define TIM_SMCR_ETP_Pos          (15U)
#define TIM_SMCR_ETP_Msk          (0x1UL << TIM_SMCR_ETP_Pos)                   /*!< 0x00008000 */
#define TIM_SMCR_ETP              TIM_SMCR_ETP_Msk                             /*!<External trigger polarity */

/*******************  Bit definition for TIM_DIER register  *******************/
#define TIM_DIER_UIE_Pos          (0U)
#define TIM_DIER_UIE_Msk          (0x1UL << TIM_DIER_UIE_Pos)                   /*!< 0x00000001 */
#define TIM_DIER_UIE              TIM_DIER_UIE_Msk                             /*!<Update interrupt enable */
#define TIM_DIER_CC1IE_Pos        (1U)
#define TIM_DIER_CC1IE_Msk        (0x1UL << TIM_DIER_CC1IE_Pos)                 /*!< 0x00000002 */
#define TIM_DIER_CC1IE            TIM_DIER_CC1IE_Msk                           /*!<Capture/Compare 1 interrupt enable   */
#define TIM_DIER_CC2IE_Pos        (2U)
#define TIM_DIER_CC2IE_Msk        (0x1UL << TIM_DIER_CC2IE_Pos)                 /*!< 0x00000004 */
#define TIM_DIER_CC2IE            TIM_DIER_CC2IE_Msk                           /*!<Capture/Compare 2 interrupt enable   */
#define TIM_DIER_CC3IE_Pos        (3U)
#define TIM_DIER_CC3IE_Msk        (0x1UL << TIM_DIER_CC3IE_Pos)                 /*!< 0x00000008 */
#define TIM_DIER_CC3IE            TIM_DIER_CC3IE_Msk                           /*!<Capture/Compare 3 interrupt enable   */
#define TIM_DIER_CC4IE_Pos        (4U)
#define TIM_DIER_CC4IE_Msk        (0x1UL << TIM_DIER_CC4IE_Pos)                 /*!< 0x00000010 */
#define TIM_DIER_CC4IE            TIM_DIER_CC4IE_Msk                           /*!<Capture/Compare 4 interrupt enable   */
#define TIM_DIER_COMIE_Pos        (5U)
#define TIM_DIER_COMIE_Msk        (0x1UL << TIM_DIER_COMIE_Pos)                 /*!< 0x00000020 */
#define TIM_DIER_COMIE            TIM_DIER_COMIE_Msk                           /*!<COM interrupt enable                 */
#define TIM_DIER_TIE_Pos          (6U)
#define TIM_DIER_TIE_Msk          (0x1UL << TIM_DIER_TIE_Pos)                   /*!< 0x00000040 */
#define TIM_DIER_TIE              TIM_DIER_TIE_Msk                             /*!<Trigger interrupt enable             */
#define TIM_DIER_BIE_Pos          (7U)
#define TIM_DIER_BIE_Msk          (0x1UL << TIM_DIER_BIE_Pos)                   /*!< 0x00000080 */
#define TIM_DIER_BIE              TIM_DIER_BIE_Msk                             /*!<Break interrupt enable               */
#define TIM_DIER_UDE_Pos          (8U)
#define TIM_DIER_UDE_Msk          (0x1UL << TIM_DIER_UDE_Pos)                   /*!< 0x00000100 */
#define TIM_DIER_UDE              TIM_DIER_UDE_Msk                             /*!<Update DMA request enable            */
#define TIM_DIER_CC1DE_Pos        (9U)
#define TIM_DIER_CC1DE_Msk        (0x1UL << TIM_DIER_CC1DE_Pos)                 /*!< 0x00000200 */
#define TIM_DIER_CC1DE            TIM_DIER_CC1DE_Msk                           /*!<Capture/Compare 1 DMA request enable */
#define TIM_DIER_CC2DE_Pos        (10U)
#define TIM_DIER_CC2DE_Msk        (0x1UL << TIM_DIER_CC2DE_Pos)                 /*!< 0x00000400 */
#define TIM_DIER_CC2DE            TIM_DIER_CC2DE_Msk                           /*!<Capture/Compare 2 DMA request enable */
#define TIM_DIER_CC3DE_Pos        (11U)
#define TIM_DIER_CC3DE_Msk        (0x1UL << TIM_DIER_CC3DE_Pos)                 /*!< 0x00000800 */
#define TIM_DIER_CC3DE            TIM_DIER_CC3DE_Msk                           /*!<Capture/Compare 3 DMA request enable */
#define TIM_DIER_CC4DE_Pos        (12U)
#define TIM_DIER_CC4DE_Msk        (0x1UL << TIM_DIER_CC4DE_Pos)                 /*!< 0x00001000 */
#define TIM_DIER_CC4DE            TIM_DIER_CC4DE_Msk                           /*!<Capture/Compare 4 DMA request enable */
#define TIM_DIER_COMDE_Pos        (13U)
#define TIM_DIER_COMDE_Msk        (0x1UL << TIM_DIER_COMDE_Pos)                 /*!< 0x00002000 */
#define TIM_DIER_COMDE            TIM_DIER_COMDE_Msk                           /*!<COM DMA request enable               */
#define TIM_DIER_TDE_Pos          (14U)
#define TIM_DIER_TDE_Msk          (0x1UL << TIM_DIER_TDE_Pos)                   /*!< 0x00004000 */
#define TIM_DIER_TDE              TIM_DIER_TDE_Msk                             /*!<Trigger DMA request enable           */

/********************  Bit definition for TIM_SR register  ********************/
#define TIM_SR_UIF_Pos            (0U)
#define TIM_SR_UIF_Msk            (0x1UL << TIM_SR_UIF_Pos)                     /*!< 0x00000001 */
#define TIM_SR_UIF                TIM_SR_UIF_Msk                               /*!<Update interrupt Flag              */
#define TIM_SR_CC1IF_Pos          (1U)
#define TIM_SR_CC1IF_Msk          (0x1UL << TIM_SR_CC1IF_Pos)                   /*!< 0x00000002 */
#define TIM_SR_CC1IF              TIM_SR_CC1IF_Msk                             /*!<Capture/Compare 1 interrupt Flag   */
#define TIM_SR_CC2IF_Pos          (2U)
#define TIM_SR_CC2IF_Msk          (0x1UL << TIM_SR_CC2IF_Pos)                   /*!< 0x00000004 */
#define TIM_SR_CC2IF              TIM_SR_CC2IF_Msk                             /*!<Capture/Compare 2 interrupt Flag   */
#define TIM_SR_CC3IF_Pos          (3U)
#define TIM_SR_CC3IF_Msk          (0x1UL << TIM_SR_CC3IF_Pos)                   /*!< 0x00000008 */
#define TIM_SR_CC3IF              TIM_SR_CC3IF_Msk                             /*!<Capture/Compare 3 interrupt Flag   */
#define TIM_SR_CC4IF_Pos          (4U)
#define TIM_SR_CC4IF_Msk          (0x1UL << TIM_SR_CC4IF_Pos)                   /*!< 0x00000010 */
#define TIM_SR_CC4IF              TIM_SR_CC4IF_Msk                             /*!<Capture/Compare 4 interrupt Flag   */
#define TIM_SR_COMIF_Pos          (5U)
#define TIM_SR_COMIF_Msk          (0x1UL << TIM_SR_COMIF_Pos)                   /*!< 0x00000020 */
#define TIM_SR_COMIF              TIM_SR_COMIF_Msk                             /*!<COM interrupt Flag                 */
#define TIM_SR_TIF_Pos            (6U)
#define TIM_SR_TIF_Msk            (0x1UL << TIM_SR_TIF_Pos)                     /*!< 0x00000040 */
#define TIM_SR_TIF                TIM_SR_TIF_Msk                               /*!<Trigger interrupt Flag             */
#define TIM_SR_BIF_Pos            (7U)
#define TIM_SR_BIF_Msk            (0x1UL << TIM_SR_BIF_Pos)                     /*!< 0x00000080 */
#define TIM_SR_BIF                TIM_SR_BIF_Msk                               /*!<Break interrupt Flag               */
#define TIM_SR_CC1OF_Pos          (9U)
#define TIM_SR_CC1OF_Msk          (0x1UL << TIM_SR_CC1OF_Pos)                   /*!< 0x00000200 */
#define TIM_SR_CC1OF              TIM_SR_CC1OF_Msk                             /*!<Capture/Compare 1 Overcapture Flag */
#define TIM_SR_CC2OF_Pos          (10U)
#define TIM_SR_CC2OF_Msk          (0x1UL << TIM_SR_CC2OF_Pos)                   /*!< 0x00000400 */
#define TIM_SR_CC2OF              TIM_SR_CC2OF_Msk                             /*!<Capture/Compare 2 Overcapture Flag */
#define TIM_SR_CC3OF_Pos          (11U)
#define TIM_SR_CC3OF_Msk          (0x1UL << TIM_SR_CC3OF_Pos)                   /*!< 0x00000800 */
#define TIM_SR_CC3OF              TIM_SR_CC3OF_Msk                             /*!<Capture/Compare 3 Overcapture Flag */
#define TIM_SR_CC4OF_Pos          (12U)
#define TIM_SR_CC4OF_Msk          (0x1UL << TIM_SR_CC4OF_Pos)                   /*!< 0x00001000 */
#define TIM_SR_CC4OF              TIM_SR_CC4OF_Msk                             /*!<Capture/Compare 4 Overcapture Flag */

/*******************  Bit definition for TIM_EGR register  ********************/
#define TIM_EGR_UG_Pos            (0U)
#define TIM_EGR_UG_Msk            (0x1UL << TIM_EGR_UG_Pos)                     /*!< 0x00000001 */
#define TIM_EGR_UG                TIM_EGR_UG_Msk                               /*!<Update Generation                         */
#define TIM_EGR_CC1G_Pos          (1U)
#define TIM_EGR_CC1G_Msk          (0x1UL << TIM_EGR_CC1G_Pos)                   /*!< 0x00000002 */
#define TIM_EGR_CC1G              TIM_EGR_CC1G_Msk                             /*!<Capture/Compare 1 Generation              */
#define TIM_EGR_CC2G_Pos          (2U)
#define TIM_EGR_CC2G_Msk          (0x1UL << TIM_EGR_CC2G_Pos)                   /*!< 0x00000004 */
#define TIM_EGR_CC2G              TIM_EGR_CC2G_Msk                             /*!<Capture/Compare 2 Generation              */
#define TIM_EGR_CC3G_Pos          (3U)
#define TIM_EGR_CC3G_Msk          (0x1UL << TIM_EGR_CC3G_Pos)                   /*!< 0x00000008 */
#define TIM_EGR_CC3G              TIM_EGR_CC3G_Msk                             /*!<Capture/Compare 3 Generation              */
#define TIM_EGR_CC4G_Pos          (4U)
#define TIM_EGR_CC4G_Msk          (0x1UL << TIM_EGR_CC4G_Pos)                   /*!< 0x00000010 */
#define TIM_EGR_CC4G              TIM_EGR_CC4G_Msk                             /*!<Capture/Compare 4 Generation              */
#define TIM_EGR_COMG_Pos          (5U)
#define TIM_EGR_COMG_Msk          (0x1UL << TIM_EGR_COMG_Pos)                   /*!< 0x00000020 */
#define TIM_EGR_COMG              TIM_EGR_COMG_Msk                             /*!<Capture/Compare Control Update Generation */
#define TIM_EGR_TG_Pos            (6U)
#define TIM_EGR_TG_Msk            (0x1UL << TIM_EGR_TG_Pos)                     /*!< 0x00000040 */
#define TIM_EGR_TG                TIM_EGR_TG_Msk                               /*!<Trigger Generation                        */
#define TIM_EGR_BG_Pos            (7U)
#define TIM_EGR_BG_Msk            (0x1UL << TIM_EGR_BG_Pos)                     /*!< 0x00000080 */
#define TIM_EGR_BG                TIM_EGR_BG_Msk                               /*!<Break Generation                          */

/******************  Bit definition for TIM_CCMR1 register  *******************/
#define TIM_CCMR1_CC1S_Pos        (0U)
#define TIM_CCMR1_CC1S_Msk        (0x3UL << TIM_CCMR1_CC1S_Pos)                 /*!< 0x00000003 */
#define TIM_CCMR1_CC1S            TIM_CCMR1_CC1S_Msk                           /*!<CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define TIM_CCMR1_CC1S_0          (0x1UL << TIM_CCMR1_CC1S_Pos)                 /*!< 0x0001 */
#define TIM_CCMR1_CC1S_1          (0x2UL << TIM_CCMR1_CC1S_Pos)                 /*!< 0x0002 */

#define TIM_CCMR1_OC1FE_Pos       (2U)
#define TIM_CCMR1_OC1FE_Msk       (0x1UL << TIM_CCMR1_OC1FE_Pos)                /*!< 0x00000004 */
#define TIM_CCMR1_OC1FE           TIM_CCMR1_OC1FE_Msk                          /*!<Output Compare 1 Fast enable                 */
#define TIM_CCMR1_OC1PE_Pos       (3U)
#define TIM_CCMR1_OC1PE_Msk       (0x1UL << TIM_CCMR1_OC1PE_Pos)                /*!< 0x00000008 */
#define TIM_CCMR1_OC1PE           TIM_CCMR1_OC1PE_Msk                          /*!<Output Compare 1 Preload enable              */

#define TIM_CCMR1_OC1M_Pos        (4U)
#define TIM_CCMR1_OC1M_Msk        (0x7UL << TIM_CCMR1_OC1M_Pos)                 /*!< 0x00000070 */
#define TIM_CCMR1_OC1M            TIM_CCMR1_OC1M_Msk                           /*!<OC1M[2:0] bits (Output Compare 1 Mode)       */
#define TIM_CCMR1_OC1M_0          (0x1UL << TIM_CCMR1_OC1M_Pos)                 /*!< 0x0010 */
#define TIM_CCMR1_OC1M_1          (0x2UL << TIM_CCMR1_OC1M_Pos)                 /*!< 0x0020 */
#define TIM_CCMR1_OC1M_2          (0x4UL << TIM_CCMR1_OC1M_Pos)                 /*!< 0x0040 */

#define TIM_CCMR1_OC1CE_Pos       (7U)
#define TIM_CCMR1_OC1CE_Msk       (0x1UL << TIM_CCMR1_OC1CE_Pos)                /*!< 0x00000080 */
#define TIM_CCMR1_OC1CE           TIM_CCMR1_OC1CE_Msk                          /*!<Output Compare 1Clear Enable                 */

#define TIM_CCMR1_CC2S_Pos        (8U)
#define TIM_CCMR1_CC2S_Msk        (0x3UL << TIM_CCMR1_CC2S_Pos)                 /*!< 0x00000300 */
#define TIM_CCMR1_CC2S            TIM_CCMR1_CC2S_Msk                           /*!<CC2S[1:0] bits (Capture/Compare 2 Selection) */
#define TIM_CCMR1_CC2S_0          (0x1UL << TIM_CCMR1_CC2S_Pos)                 /*!< 0x0100 */
#define TIM_CCMR1_CC2S_1          (0x2UL << TIM_CCMR1_CC2S_Pos)                 /*!< 0x0200 */

#define TIM_CCMR1_OC2FE_Pos       (10U)
#define TIM_CCMR1_OC2FE_Msk       (0x1UL << TIM_CCMR1_OC2FE_Pos)                /*!< 0x00000400 */
#define TIM_CCMR1_OC2FE           TIM_CCMR1_OC2FE_Msk                          /*!<Output Compare 2 Fast enable                 */
#define TIM_CCMR1_OC2PE_Pos       (11U)
#define TIM_CCMR1_OC2PE_Msk       (0x1UL << TIM_CCMR1_OC2PE_Pos)                /*!< 0x00000800 */
#define TIM_CCMR1_OC2PE           TIM_CCMR1_OC2PE_Msk                          /*!<Output Compare 2 Preload enable              */

#define TIM_CCMR1_OC2M_Pos        (12U)
#define TIM_CCMR1_OC2M_Msk        (0x7UL << TIM_CCMR1_OC2M_Pos)                 /*!< 0x00007000 */
#define TIM_CCMR1_OC2M            TIM_CCMR1_OC2M_Msk                           /*!<OC2M[2:0] bits (Output Compare 2 Mode)       */
#define TIM_CCMR1_OC2M_0          (0x1UL << TIM_CCMR1_OC2M_Pos)                 /*!< 0x1000 */
#define TIM_CCMR1_OC2M_1          (0x2UL << TIM_CCMR1_OC2M_Pos)                 /*!< 0x2000 */
#define TIM_CCMR1_OC2M_2          (0x4UL << TIM_CCMR1_OC2M_Pos)                 /*!< 0x4000 */

#define TIM_CCMR1_OC2CE_Pos       (15U)
#define TIM_CCMR1_OC2CE_Msk       (0x1UL << TIM_CCMR1_OC2CE_Pos)                /*!< 0x00008000 */
#define TIM_CCMR1_OC2CE           TIM_CCMR1_OC2CE_Msk                          /*!<Output Compare 2 Clear Enable */

/*----------------------------------------------------------------------------*/

#define TIM_CCMR1_IC1PSC_Pos      (2U)
#define TIM_CCMR1_IC1PSC_Msk      (0x3UL << TIM_CCMR1_IC1PSC_Pos)               /*!< 0x0000000C */
#define TIM_CCMR1_IC1PSC          TIM_CCMR1_IC1PSC_Msk                         /*!<IC1PSC[1:0] bits (Input Capture 1 Prescaler) */
#define TIM_CCMR1_IC1PSC_0        (0x1UL << TIM_CCMR1_IC1PSC_Pos)               /*!< 0x0004 */
#define TIM_CCMR1_IC1PSC_1        (0x2UL << TIM_CCMR1_IC1PSC_Pos)               /*!< 0x0008 */

#define TIM_CCMR1_IC1F_Pos        (4U)
#define TIM_CCMR1_IC1F_Msk        (0xFUL << TIM_CCMR1_IC1F_Pos)                 /*!< 0x000000F0 */
#define TIM_CCMR1_IC1F            TIM_CCMR1_IC1F_Msk                           /*!<IC1F[3:0] bits (Input Capture 1 Filter)      */
#define TIM_CCMR1_IC1F_0          (0x1UL << TIM_CCMR1_IC1F_Pos)                 /*!< 0x0010 */
#define TIM_CCMR1_IC1F_1          (0x2UL << TIM_CCMR1_IC1F_Pos)                 /*!< 0x0020 */
#define TIM_CCMR1_IC1F_2          (0x4UL << TIM_CCMR1_IC1F_Pos)                 /*!< 0x0040 */
#define TIM_CCMR1_IC1F_3          (0x8UL << TIM_CCMR1_IC1F_Pos)                 /*!< 0x0080 */

#define TIM_CCMR1_IC2PSC_Pos      (10U)
#define TIM_CCMR1_IC2PSC_Msk      (0x3UL << TIM_CCMR1_IC2PSC_Pos)               /*!< 0x00000C00 */
#define TIM_CCMR1_IC2PSC          TIM_CCMR1_IC2PSC_Msk                         /*!<IC2PSC[1:0] bits (Input Capture 2 Prescaler)  */
#define TIM_CCMR1_IC2PSC_0        (0x1UL << TIM_CCMR1_IC2PSC_Pos)               /*!< 0x0400 */
#define TIM_CCMR1_IC2PSC_1        (0x2UL << TIM_CCMR1_IC2PSC_Pos)               /*!< 0x0800 */

#define TIM_CCMR1_IC2F_Pos        (12U)
#define TIM_CCMR1_IC2F_Msk        (0xFUL << TIM_CCMR1_IC2F_Pos)                 /*!< 0x0000F000 */
#define TIM_CCMR1_IC2F            TIM_CCMR1_IC2F_Msk                           /*!<IC2F[3:0] bits (Input Capture 2 Filter)       */
#define TIM_CCMR1_IC2F_0          (0x1UL << TIM_CCMR1_IC2F_Pos)                 /*!< 0x1000 */
#define TIM_CCMR1_IC2F_1          (0x2UL << TIM_CCMR1_IC2F_Pos)                 /*!< 0x2000 */
#define TIM_CCMR1_IC2F_2          (0x4UL << TIM_CCMR1_IC2F_Pos)                 /*!< 0x4000 */
#define TIM_CCMR1_IC2F_3          (0x8UL << TIM_CCMR1_IC2F_Pos)                 /*!< 0x8000 */

/******************  Bit definition for TIM_CCMR2 register  *******************/
#define TIM_CCMR2_CC3S_Pos        (0U)
#define TIM_CCMR2_CC3S_Msk        (0x3UL << TIM_CCMR2_CC3S_Pos)                 /*!< 0x00000003 */
#define TIM_CCMR2_CC3S            TIM_CCMR2_CC3S_Msk                           /*!<CC3S[1:0] bits (Capture/Compare 3 Selection)  */
#define TIM_CCMR2_CC3S_0          (0x1UL << TIM_CCMR2_CC3S_Pos)                 /*!< 0x0001 */
#define TIM_CCMR2_CC3S_1          (0x2UL << TIM_CCMR2_CC3S_Pos)                 /*!< 0x0002 */

#define TIM_CCMR2_OC3FE_Pos       (2U)
#define TIM_CCMR2_OC3FE_Msk       (0x1UL << TIM_CCMR2_OC3FE_Pos)                /*!< 0x00000004 */
#define TIM_CCMR2_OC3FE           TIM_CCMR2_OC3FE_Msk                          /*!<Output Compare 3 Fast enable           */
#define TIM_CCMR2_OC3PE_Pos       (3U)
#define TIM_CCMR2_OC3PE_Msk       (0x1UL << TIM_CCMR2_OC3PE_Pos)                /*!< 0x00000008 */
#define TIM_CCMR2_OC3PE           TIM_CCMR2_OC3PE_Msk                          /*!<Output Compare 3 Preload enable        */

#define TIM_CCMR2_OC3M_Pos        (4U)
#define TIM_CCMR2_OC3M_Msk        (0x7UL << TIM_CCMR2_OC3M_Pos)                 /*!< 0x00000070 */
#define TIM_CCMR2_OC3M            TIM_CCMR2_OC3M_Msk                           /*!<OC3M[2:0] bits (Output Compare 3 Mode) */
#define TIM_CCMR2_OC3M_0          (0x1UL << TIM_CCMR2_OC3M_Pos)                 /*!< 0x0010 */
#define TIM_CCMR2_OC3M_1          (0x2UL << TIM_CCMR2_OC3M_Pos)                 /*!< 0x0020 */
#define TIM_CCMR2_OC3M_2          (0x4UL << TIM_CCMR2_OC3M_Pos)                 /*!< 0x0040 */

#define TIM_CCMR2_OC3CE_Pos       (7U)
#define TIM_CCMR2_OC3CE_Msk       (0x1UL << TIM_CCMR2_OC3CE_Pos)                /*!< 0x00000080 */
#define TIM_CCMR2_OC3CE           TIM_CCMR2_OC3CE_Msk                          /*!<Output Compare 3 Clear Enable */

#define TIM_CCMR2_CC4S_Pos        (8U)
#define TIM_CCMR2_CC4S_Msk        (0x3UL << TIM_CCMR2_CC4S_Pos)                 /*!< 0x00000300 */
#define TIM_CCMR2_CC4S            TIM_CCMR2_CC4S_Msk                           /*!<CC4S[1:0] bits (Capture/Compare 4 Selection) */
#define TIM_CCMR2_CC4S_0          (0x1UL << TIM_CCMR2_CC4S_Pos)                 /*!< 0x0100 */
#define TIM_CCMR2_CC4S_1          (0x2UL << TIM_CCMR2_CC4S_Pos)                 /*!< 0x0200 */

#define TIM_CCMR2_OC4FE_Pos       (10U)
#define TIM_CCMR2_OC4FE_Msk       (0x1UL << TIM_CCMR2_OC4FE_Pos)                /*!< 0x00000400 */
#define TIM_CCMR2_OC4FE           TIM_CCMR2_OC4FE_Msk                          /*!<Output Compare 4 Fast enable    */
#define TIM_CCMR2_OC4PE_Pos       (11U)
#define TIM_CCMR2_OC4PE_Msk       (0x1UL << TIM_CCMR2_OC4PE_Pos)                /*!< 0x00000800 */
#define TIM_CCMR2_OC4PE           TIM_CCMR2_OC4PE_Msk                          /*!<Output Compare 4 Preload enable */

#define TIM_CCMR2_OC4M_Pos        (12U)
#define TIM_CCMR2_OC4M_Msk        (0x7UL << TIM_CCMR2_OC4M_Pos)                 /*!< 0x00007000 */
#define TIM_CCMR2_OC4M            TIM_CCMR2_OC4M_Msk                           /*!<OC4M[2:0] bits (Output Compare 4 Mode) */
#define TIM_CCMR2_OC4M_0          (0x1UL << TIM_CCMR2_OC4M_Pos)                 /*!< 0x1000 */
#define TIM_CCMR2_OC4M_1          (0x2UL << TIM_CCMR2_OC4M_Pos)                 /*!< 0x2000 */
#define TIM_CCMR2_OC4M_2          (0x4UL << TIM_CCMR2_OC4M_Pos)                 /*!< 0x4000 */

#define TIM_CCMR2_OC4CE_Pos       (15U)
#define TIM_CCMR2_OC4CE_Msk       (0x1UL << TIM_CCMR2_OC4CE_Pos)                /*!< 0x00008000 */
#define TIM_CCMR2_OC4CE           TIM_CCMR2_OC4CE_Msk                          /*!<Output Compare 4 Clear Enable */

/*----------------------------------------------------------------------------*/

#define TIM_CCMR2_IC3PSC_Pos      (2U)
#define TIM_CCMR2_IC3PSC_Msk      (0x3UL << TIM_CCMR2_IC3PSC_Pos)               /*!< 0x0000000C */
#define TIM_CCMR2_IC3PSC          TIM_CCMR2_IC3PSC_Msk                         /*!<IC3PSC[1:0] bits (Input Capture 3 Prescaler) */
#define TIM_CCMR2_IC3PSC_0        (0x1UL << TIM_CCMR2_IC3PSC_Pos)               /*!< 0x0004 */
#define TIM_CCMR2_IC3PSC_1        (0x2UL << TIM_CCMR2_IC3PSC_Pos)               /*!< 0x0008 */

#define TIM_CCMR2_IC3F_Pos        (4U)
#define TIM_CCMR2_IC3F_Msk        (0xFUL << TIM_CCMR2_IC3F_Pos)                 /*!< 0x000000F0 */
#define TIM_CCMR2_IC3F            TIM_CCMR2_IC3F_Msk                           /*!<IC3F[3:0] bits (Input Capture 3 Filter) */
#define TIM_CCMR2_IC3F_0          (0x1UL << TIM_CCMR2_IC3F_Pos)                 /*!< 0x0010 */
#define TIM_CCMR2_IC3F_1          (0x2UL << TIM_CCMR2_IC3F_Pos)                 /*!< 0x0020 */
#define TIM_CCMR2_IC3F_2          (0x4UL << TIM_CCMR2_IC3F_Pos)                 /*!< 0x0040 */
#define TIM_CCMR2_IC3F_3          (0x8UL << TIM_CCMR2_IC3F_Pos)                 /*!< 0x0080 */

#define TIM_CCMR2_IC4PSC_Pos      (10U)
#define TIM_CCMR2_IC4PSC_Msk      (0x3UL << TIM_CCMR2_IC4PSC_Pos)               /*!< 0x00000C00 */
#define TIM_CCMR2_IC4PSC          TIM_CCMR2_IC4PSC_Msk                         /*!<IC4PSC[1:0] bits (Input Capture 4 Prescaler) */
#define TIM_CCMR2_IC4PSC_0        (0x1UL << TIM_CCMR2_IC4PSC_Pos)               /*!< 0x0400 */
#define TIM_CCMR2_IC4PSC_1        (0x2UL << TIM_CCMR2_IC4PSC_Pos)               /*!< 0x0800 */

#define TIM_CCMR2_IC4F_Pos        (12U)
#define TIM_CCMR2_IC4F_Msk        (0xFUL << TIM_CCMR2_IC4F_Pos)                 /*!< 0x0000F000 */
#define TIM_CCMR2_IC4F            TIM_CCMR2_IC4F_Msk                           /*!<IC4F[3:0] bits (Input Capture 4 Filter) */
#define TIM_CCMR2_IC4F_0          (0x1UL << TIM_CCMR2_IC4F_Pos)                 /*!< 0x1000 */
#define TIM_CCMR2_IC4F_1          (0x2UL << TIM_CCMR2_IC4F_Pos)                 /*!< 0x2000 */
#define TIM_CCMR2_IC4F_2          (0x4UL << TIM_CCMR2_IC4F_Pos)                 /*!< 0x4000 */
#define TIM_CCMR2_IC4F_3          (0x8UL << TIM_CCMR2_IC4F_Pos)                 /*!< 0x8000 */

/*******************  Bit definition for TIM_CCER register  *******************/
#define TIM_CCER_CC1E_Pos         (0U)
#define TIM_CCER_CC1E_Msk         (0x1UL << TIM_CCER_CC1E_Pos)                  /*!< 0x00000001 */
#define TIM_CCER_CC1E             TIM_CCER_CC1E_Msk                            /*!<Capture/Compare 1 output enable                 */
#define TIM_CCER_CC1P_Pos         (1U)
#define TIM_CCER_CC1P_Msk         (0x1UL << TIM_CCER_CC1P_Pos)                  /*!< 0x00000002 */
#define TIM_CCER_CC1P             TIM_CCER_CC1P_Msk                            /*!<Capture/Compare 1 output Polarity               */
#define TIM_CCER_CC1NE_Pos        (2U)
#define TIM_CCER_CC1NE_Msk        (0x1UL << TIM_CCER_CC1NE_Pos)                 /*!< 0x00000004 */
#define TIM_CCER_CC1NE            TIM_CCER_CC1NE_Msk                           /*!<Capture/Compare 1 Complementary output enable   */
#define TIM_CCER_CC1NP_Pos        (3U)
#define TIM_CCER_CC1NP_Msk        (0x1UL << TIM_CCER_CC1NP_Pos)                 /*!< 0x00000008 */
#define TIM_CCER_CC1NP            TIM_CCER_CC1NP_Msk                           /*!<Capture/Compare 1 Complementary output Polarity */
#define TIM_CCER_CC2E_Pos         (4U)
#define TIM_CCER_CC2E_Msk         (0x1UL << TIM_CCER_CC2E_Pos)                  /*!< 0x00000010 */
#define TIM_CCER_CC2E             TIM_CCER_CC2E_Msk                            /*!<Capture/Compare 2 output enable                 */
#define TIM_CCER_CC2P_Pos         (5U)
#define TIM_CCER_CC2P_Msk         (0x1UL << TIM_CCER_CC2P_Pos)                  /*!< 0x00000020 */
#define TIM_CCER_CC2P             TIM_CCER_CC2P_Msk                            /*!<Capture/Compare 2 output Polarity               */
#define TIM_CCER_CC2NE_Pos        (6U)
#define TIM_CCER_CC2NE_Msk        (0x1UL << TIM_CCER_CC2NE_Pos)                 /*!< 0x00000040 */
#define TIM_CCER_CC2NE            TIM_CCER_CC2NE_Msk                           /*!<Capture/Compare 2 Complementary output enable   */
#define TIM_CCER_CC2NP_Pos        (7U)
#define TIM_CCER_CC2NP_Msk        (0x1UL << TIM_CCER_CC2NP_Pos)                 /*!< 0x00000080 */
#define TIM_CCER_CC2NP            TIM_CCER_CC2NP_Msk                           /*!<Capture/Compare 2 Complementary output Polarity */
#define TIM_CCER_CC3E_Pos         (8U)
#define TIM_CCER_CC3E_Msk         (0x1UL << TIM_CCER_CC3E_Pos)                  /*!< 0x00000100 */
#define TIM_CCER_CC3E             TIM_CCER_CC3E_Msk                            /*!<Capture/Compare 3 output enable                 */
#define TIM_CCER_CC3P_Pos         (9U)
#define TIM_CCER_CC3P_Msk         (0x1UL << TIM_CCER_CC3P_Pos)                  /*!< 0x00000200 */
#define TIM_CCER_CC3P             TIM_CCER_CC3P_Msk                            /*!<Capture/Compare 3 output Polarity               */
#define TIM_CCER_CC3NE_Pos        (10U)
#define TIM_CCER_CC3NE_Msk        (0x1UL << TIM_CCER_CC3NE_Pos)                 /*!< 0x00000400 */
#define TIM_CCER_CC3NE            TIM_CCER_CC3NE_Msk                           /*!<Capture/Compare 3 Complementary output enable   */
#define TIM_CCER_CC3NP_Pos        (11U)
#define TIM_CCER_CC3NP_Msk        (0x1UL << TIM_CCER_CC3NP_Pos)                 /*!< 0x00000800 */
#define TIM_CCER_CC3NP            TIM_CCER_CC3NP_Msk                           /*!<Capture/Compare 3 Complementary output Polarity */
#define TIM_CCER_CC4E_Pos         (12U)
#define TIM_CCER_CC4E_Msk         (0x1UL << TIM_CCER_CC4E_Pos)                  /*!< 0x00001000 */
#define TIM_CCER_CC4E             TIM_CCER_CC4E_Msk                            /*!<Capture/Compare 4 output enable                 */
#define TIM_CCER_CC4P_Pos         (13U)
#define TIM_CCER_CC4P_Msk         (0x1UL << TIM_CCER_CC4P_Pos)                  /*!< 0x00002000 */
#define TIM_CCER_CC4P             TIM_CCER_CC4P_Msk                            /*!<Capture/Compare 4 output Polarity               */
#define TIM_CCER_CC4NP_Pos        (15U)
#define TIM_CCER_CC4NP_Msk        (0x1UL << TIM_CCER_CC4NP_Pos)                 /*!< 0x00008000 */
#define TIM_CCER_CC4NP            TIM_CCER_CC4NP_Msk                           /*!<Capture/Compare 4 Complementary output Polarity */

/*******************  Bit definition for TIM_CNT register  ********************/
#define TIM_CNT_CNT_Pos           (0U)
#define TIM_CNT_CNT_Msk           (0xFFFFFFFFUL << TIM_CNT_CNT_Pos)                 /*!< 0xFFFFFFFF */
#define TIM_CNT_CNT               TIM_CNT_CNT_Msk                                  /*!<Counter Value            */

/*******************  Bit definition for TIM_PSC register  ********************/
#define TIM_PSC_PSC_Pos           (0U)
#define TIM_PSC_PSC_Msk           (0xFFFFUL << TIM_PSC_PSC_Pos)                 /*!< 0x0000FFFF */
#define TIM_PSC_PSC               TIM_PSC_PSC_Msk                              /*!<Prescaler Value          */

/*******************  Bit definition for TIM_ARR register  ********************/
#define TIM_ARR_ARR_Pos           (0U)
#define TIM_ARR_ARR_Msk           (0xFFFFFFFFUL << TIM_ARR_ARR_Pos)             /*!< 0xFFFFFFFF */
#define TIM_ARR_ARR               TIM_ARR_ARR_Msk                              /*!<actual auto-reload Value */

/*******************  Bit definition for TIM_RCR register  ********************/
#define TIM_RCR_REP_Pos           (0U)
#define TIM_RCR_REP_Msk           (0xFFUL << TIM_RCR_REP_Pos)                   /*!< 0x000000FF */
#define TIM_RCR_REP               TIM_RCR_REP_Msk                              /*!<Repetition Counter Value */

/*******************  Bit definition for TIM_CCR1 register  *******************/
#define TIM_CCR1_CCR1_Pos         (0U)
#define TIM_CCR1_CCR1_Msk         (0xFFFFUL << TIM_CCR1_CCR1_Pos)               /*!< 0x0000FFFF */
#define TIM_CCR1_CCR1             TIM_CCR1_CCR1_Msk                            /*!<Capture/Compare 1 Value  */

/*******************  Bit definition for TIM_CCR2 register  *******************/
#define TIM_CCR2_CCR2_Pos         (0U)
#define TIM_CCR2_CCR2_Msk         (0xFFFFUL << TIM_CCR2_CCR2_Pos)               /*!< 0x0000FFFF */
#define TIM_CCR2_CCR2             TIM_CCR2_CCR2_Msk                            /*!<Capture/Compare 2 Value  */

/*******************  Bit definition for TIM_CCR3 register  *******************/
#define TIM_CCR3_CCR3_Pos         (0U)
#define TIM_CCR3_CCR3_Msk         (0xFFFFUL << TIM_CCR3_CCR3_Pos)               /*!< 0x0000FFFF */
#define TIM_CCR3_CCR3             TIM_CCR3_CCR3_Msk                            /*!<Capture/Compare 3 Value  */

/*******************  Bit definition for TIM_CCR4 register  *******************/
#define TIM_CCR4_CCR4_Pos         (0U)
#define TIM_CCR4_CCR4_Msk         (0xFFFFUL << TIM_CCR4_CCR4_Pos)               /*!< 0x0000FFFF */
#define TIM_CCR4_CCR4             TIM_CCR4_CCR4_Msk                            /*!<Capture/Compare 4 Value  */

/*******************  Bit definition for TIM_BDTR register  *******************/
#define TIM_BDTR_DTG_Pos          (0U)
#define TIM_BDTR_DTG_Msk          (0xFFUL << TIM_BDTR_DTG_Pos)                  /*!< 0x000000FF */
#define TIM_BDTR_DTG              TIM_BDTR_DTG_Msk                             /*!<DTG[0:7] bits (Dead-Time Generator set-up) */
#define TIM_BDTR_DTG_0            (0x01UL << TIM_BDTR_DTG_Pos)                  /*!< 0x0001 */
#define TIM_BDTR_DTG_1            (0x02UL << TIM_BDTR_DTG_Pos)                  /*!< 0x0002 */
#define TIM_BDTR_DTG_2            (0x04UL << TIM_BDTR_DTG_Pos)                  /*!< 0x0004 */
#define TIM_BDTR_DTG_3            (0x08UL << TIM_BDTR_DTG_Pos)                  /*!< 0x0008 */
#define TIM_BDTR_DTG_4            (0x10UL << TIM_BDTR_DTG_Pos)                  /*!< 0x0010 */
#define TIM_BDTR_DTG_5            (0x20UL << TIM_BDTR_DTG_Pos)                  /*!< 0x0020 */
#define TIM_BDTR_DTG_6            (0x40UL << TIM_BDTR_DTG_Pos)                  /*!< 0x0040 */
#define TIM_BDTR_DTG_7            (0x80UL << TIM_BDTR_DTG_Pos)                  /*!< 0x0080 */

#define TIM_BDTR_LOCK_Pos         (8U)
#define TIM_BDTR_LOCK_Msk         (0x3UL << TIM_BDTR_LOCK_Pos)                  /*!< 0x00000300 */
#define TIM_BDTR_LOCK             TIM_BDTR_LOCK_Msk                            /*!<LOCK[1:0] bits (Lock Configuration) */
#define TIM_BDTR_LOCK_0           (0x1UL << TIM_BDTR_LOCK_Pos)                  /*!< 0x0100 */
#define TIM_BDTR_LOCK_1           (0x2UL << TIM_BDTR_LOCK_Pos)                  /*!< 0x0200 */

#define TIM_BDTR_OSSI_Pos         (10U)
#define TIM_BDTR_OSSI_Msk         (0x1UL << TIM_BDTR_OSSI_Pos)                  /*!< 0x00000400 */
#define TIM_BDTR_OSSI             TIM_BDTR_OSSI_Msk                            /*!<Off-State Selection for Idle mode */
#define TIM_BDTR_OSSR_Pos         (11U)
#define TIM_BDTR_OSSR_Msk         (0x1UL << TIM_BDTR_OSSR_Pos)                  /*!< 0x00000800 */
#define TIM_BDTR_OSSR             TIM_BDTR_OSSR_Msk                            /*!<Off-State Selection for Run mode  */
#define TIM_BDTR_BKE_Pos          (12U)
#define TIM_BDTR_BKE_Msk          (0x1UL << TIM_BDTR_BKE_Pos)                   /*!< 0x00001000 */
#define TIM_BDTR_BKE              TIM_BDTR_BKE_Msk                             /*!<Break enable                      */
#define TIM_BDTR_BKP_Pos          (13U)
#define TIM_BDTR_BKP_Msk          (0x1UL << TIM_BDTR_BKP_Pos)                   /*!< 0x00002000 */
#define TIM_BDTR_BKP              TIM_BDTR_BKP_Msk                             /*!<Break Polarity                    */
#define TIM_BDTR_AOE_Pos          (14U)
#define TIM_BDTR_AOE_Msk          (0x1UL << TIM_BDTR_AOE_Pos)                   /*!< 0x00004000 */
#define TIM_BDTR_AOE              TIM_BDTR_AOE_Msk                             /*!<Automatic Output enable           */
#define TIM_BDTR_MOE_Pos          (15U)
#define TIM_BDTR_MOE_Msk          (0x1UL << TIM_BDTR_MOE_Pos)                   /*!< 0x00008000 */
#define TIM_BDTR_MOE              TIM_BDTR_MOE_Msk                             /*!<Main Output enable                */

/*******************  Bit definition for TIM_DCR register  ********************/
#define TIM_DCR_DBA_Pos           (0U)
#define TIM_DCR_DBA_Msk           (0x1FUL << TIM_DCR_DBA_Pos)                   /*!< 0x0000001F */
#define TIM_DCR_DBA               TIM_DCR_DBA_Msk                              /*!<DBA[4:0] bits (DMA Base Address) */
#define TIM_DCR_DBA_0             (0x01UL << TIM_DCR_DBA_Pos)                   /*!< 0x0001 */
#define TIM_DCR_DBA_1             (0x02UL << TIM_DCR_DBA_Pos)                   /*!< 0x0002 */
#define TIM_DCR_DBA_2             (0x04UL << TIM_DCR_DBA_Pos)                   /*!< 0x0004 */
#define TIM_DCR_DBA_3             (0x08UL << TIM_DCR_DBA_Pos)                   /*!< 0x0008 */
#define TIM_DCR_DBA_4             (0x10UL << TIM_DCR_DBA_Pos)                   /*!< 0x0010 */

#define TIM_DCR_DBL_Pos           (8U)
#define TIM_DCR_DBL_Msk           (0x1FUL << TIM_DCR_DBL_Pos)                   /*!< 0x00001F00 */
#define TIM_DCR_DBL               TIM_DCR_DBL_Msk                              /*!<DBL[4:0] bits (DMA Burst Length) */
#define TIM_DCR_DBL_0             (0x01UL << TIM_DCR_DBL_Pos)                   /*!< 0x0100 */
#define TIM_DCR_DBL_1             (0x02UL << TIM_DCR_DBL_Pos)                   /*!< 0x0200 */
#define TIM_DCR_DBL_2             (0x04UL << TIM_DCR_DBL_Pos)                   /*!< 0x0400 */
#define TIM_DCR_DBL_3             (0x08UL << TIM_DCR_DBL_Pos)                   /*!< 0x0800 */
#define TIM_DCR_DBL_4             (0x10UL << TIM_DCR_DBL_Pos)                   /*!< 0x1000 */

/******************************************************************************/
/*                                                                            */
/*                        Serial Peripheral Interface                         */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for SPI_CR0 register  ********************/
#define SPI_CR0_DSS_Pos         (0U)
#define SPI_CR0_DSS_Msk         (0xFUL << SPI_CR0_DSS_Pos)                       /*!< 0x00000030 */
#define SPI_CR0_DSS             SPI_CR0_DSS_Msk                                  /*!<DSS[3:0] bits (Data Frame Size)     */
#define SPI_CR0_DSS_0           (0x1UL << SPI_CR0_DSS_Pos)                       /*!< 0x00000001 */
#define SPI_CR0_DSS_1           (0x2UL << SPI_CR0_DSS_Pos)                       /*!< 0x00000002 */
#define SPI_CR0_DSS_2           (0x4UL << SPI_CR0_DSS_Pos)                       /*!< 0x00000004 */
#define SPI_CR0_DSS_3           (0x8UL << SPI_CR0_DSS_Pos)                       /*!< 0x00000008 */
#define SPI_CR0_FRF_Pos         (4U)
#define SPI_CR0_FRF_Msk         (0x3UL << SPI_CR0_FRF_Pos)                       /*!< 0x00000030 */
#define SPI_CR0_FRF             SPI_CR0_FRF_Msk                                  /*!<FRF[1:0] bits (Frame Format)        */
#define SPI_CR0_FRF_0           (0x1UL << SPI_CR0_FRF_Pos)                       /*!< 0x00000010 */
#define SPI_CR0_FRF_1           (0x2UL << SPI_CR0_FRF_Pos)                       /*!< 0x00000020 */
#define SPI_CR0_SPO_Pos         (6U)
#define SPI_CR0_SPO_Msk         (0x1UL << SPI_CR0_SPO_Pos)                       /*!< 0x00000040 */
#define SPI_CR0_SPO             SPI_CR0_SPO_Msk                                  /*!<SPICLKOUT Polarity                  */
#define SPI_CR0_SPH_Pos         (7U)
#define SPI_CR0_SPH_Msk         (0x1UL << SPI_CR0_SPH_Pos)                       /*!< 0x00000080 */
#define SPI_CR0_SPH             SPI_CR0_SPH_Msk                                  /*!<SPICLKOUT Phase                */
#define SPI_CR0_SCR_Pos         (8U)
#define SPI_CR0_SCR_Msk         (0xFFUL << SPI_CR0_SCR_Pos)                      /*!< 0x0000FF00 */
#define SPI_CR0_SCR             SPI_CR0_SCR_Msk                                  /*!<Serial Clock Rate                    */
/*******************  Bit definition for SPI_CR1 register  ********************/
#define SPI_CR1_LOOPBACK_Pos         (0U)
#define SPI_CR1_LOOPBACK_Msk         (0x1UL << SPI_CR1_LOOPBACK_Pos)             /*!< 0x00000001 */
#define SPI_CR1_LOOPBACK             SPI_CR1_LOOPBACK_Msk                        /*!<SPI Loop Back Mode                  */
#define SPI_CR1_SPE_Pos              (1U)
#define SPI_CR1_SPE_Msk              (0x1UL << SPI_CR1_SPE_Pos)                  /*!< 0x00000002 */
#define SPI_CR1_SPE                  SPI_CR1_SPE_Msk                             /*!<SPI Enable                          */
#define SPI_CR1_SLAVE_Pos            (2U)
#define SPI_CR1_SLAVE_Msk            (0x1UL << SPI_CR1_SLAVE_Pos)                /*!< 0x00000004 */
#define SPI_CR1_SLAVE                SPI_CR1_SLAVE_Msk                           /*!<SPI Slave Mode                      */
#define SPI_CR1_SLAVEMODE_TXDIS_Pos  (3U)
#define SPI_CR1_SLAVEMODE_TXDIS_Msk  (0x1UL << SPI_CR1_SLAVEMODE_TXDIS_Pos)      /*!< 0x00000008 */
#define SPI_CR1_SLAVEMODE_TXDIS      SPI_CR1_SLAVEMODE_TXDIS_Msk                 /*!<SPI Slave Mode Ouput Disable        */

/********************  Bit definition for SPI_SR register  ********************/
#define SPI_SR_TXFE_Pos             (0U)
#define SPI_SR_TXFE_Msk             (0x1UL << SPI_SR_TXFE_Pos)                   /*!< 0x00000001 */
#define SPI_SR_TXFE                 SPI_SR_TXFE_Msk                              /*!<Transmit FIFO Empty                 */
#define SPI_SR_TXNF_Pos             (1U)
#define SPI_SR_TXNF_Msk             (0x1UL << SPI_SR_TXNF_Pos)                   /*!< 0x00000002 */
#define SPI_SR_TXNF                 SPI_SR_TXNF_Msk                              /*!<Transmit FIFO Not Full              */
#define SPI_SR_RXNE_Pos             (2U)
#define SPI_SR_RXNE_Msk             (0x1UL << SPI_SR_RXNE_Pos)                   /*!< 0x00000004 */
#define SPI_SR_RXNE                 SPI_SR_RXNE_Msk                              /*!<Receive FIFO Not Empty              */
#define SPI_SR_RXFF_Pos             (3U)
#define SPI_SR_RXFF_Msk             (0x1UL << SPI_SR_RXFF_Pos)                   /*!< 0x00000008 */
#define SPI_SR_RXFF                 SPI_SR_RXFF_Msk                              /*!<Receive FIFO Full                   */
#define SPI_SR_BSY_Pos              (4U)
#define SPI_SR_BSY_Msk              (0x1UL << SPI_SR_BSY_Pos)                    /*!< 0x00000010 */
#define SPI_SR_BSY                  SPI_SR_BSY_Msk                               /*!<Busy flag                           */

/*******************  Bit definition for SPI_IMSC register  ******************/
#define SPI_IMSC_RORIM_Pos          (0U)
#define SPI_IMSC_RORIM_Msk          (0x1UL << SPI_IMSC_RORIM_Pos)                /*!< 0x00000001 */
#define SPI_IMSC_RORIM              SPI_IMSC_RORIM_Msk                           /*!<Receive Overrun Interrutp register  */
#define SPI_IMSC_RTIM_Pos           (1U)
#define SPI_IMSC_RTIM_Msk           (0x1UL << SPI_IMSC_RTIM_Pos)                 /*!< 0x00000002 */
#define SPI_IMSC_RTIM               SPI_IMSC_RTIM_Msk                            /*!<Receive Timeout Interrutp register  */
#define SPI_IMSC_RXIM_Pos           (2U)
#define SPI_IMSC_RXIM_Msk           (0x1UL << SPI_IMSC_RXIM_Pos)                 /*!< 0x00000004 */
#define SPI_IMSC_RXIM               SPI_IMSC_RXIM_Msk                            /*!<Receive FIFO Interrutp register     */
#define SPI_IMSC_TXIM_Pos           (3U)
#define SPI_IMSC_TXIM_Msk           (0x1UL << SPI_IMSC_TXIM_Pos)                 /*!< 0x00000008 */
#define SPI_IMSC_TXIM               SPI_IMSC_TXIM_Msk                            /*!<Transmit FIFO Interrutp register    */

/*******************  Bit definition for SPI_RIS register  ******************/
#define SPI_RIS_RORRIS_Pos          (0U)
#define SPI_RIS_RORRIS_Msk          (0x1UL << SPI_RIS_RORRIS_Pos)                /*!< 0x00000001 */
#define SPI_RIS_RORRIS              SPI_RIS_RORRIS_Msk                           /*!<Receive Overrun Interrutp flag      */
#define SPI_RIS_RTRIS_Pos           (1U)
#define SPI_RIS_RTRIS_Msk           (0x1UL << SPI_RIS_RTRIS_Pos)                 /*!< 0x00000002 */
#define SPI_RIS_RTRIS               SPI_RIS_RTRIS_Msk                            /*!<Receive Timeout Interrutp flag      */
#define SPI_RIS_RXRIS_Pos           (2U)
#define SPI_RIS_RXRIS_Msk           (0x1UL << SPI_RIS_RXRIS_Pos)                 /*!< 0x00000004 */
#define SPI_RIS_RXRIS               SPI_RIS_RXRIS_Msk                            /*!<Receive FIFO Interrutp flag         */
#define SPI_RIS_TXRIS_Pos           (3U)
#define SPI_RIS_TXRIS_Msk           (0x1UL << SPI_RIS_TXRIS_Pos)                 /*!< 0x00000008 */
#define SPI_RIS_TXRIS               SPI_RIS_TXRIS_Msk                            /*!<Transmit FIFO Interrutp flag        */

/*******************  Bit definition for SPI_MIS register  ******************/
#define SPI_MIS_RORMIS_Pos          (0U)
#define SPI_MIS_RORMIS_Msk          (0x1UL << SPI_MIS_RORMIS_Pos)                /*!< 0x00000001 */
#define SPI_MIS_RORMIS              SPI_MIS_RORMIS_Msk                           /*!<Receive Overrun Interrutp flag      */
#define SPI_MIS_RTMIS_Pos           (1U)
#define SPI_MIS_RTMIS_Msk           (0x1UL << SPI_MIS_RTMIS_Pos)                 /*!< 0x00000002 */
#define SPI_MIS_RTMIS               SPI_MIS_RTMIS_Msk                            /*!<Receive Timeout Interrutp flag      */
#define SPI_MIS_RXMIS_Pos           (2U)
#define SPI_MIS_RXMIS_Msk           (0x1UL << SPI_MIS_RXMIS_Pos)                 /*!< 0x00000004 */
#define SPI_MIS_RXMIS               SPI_MIS_RXMIS_Msk                            /*!<Receive FIFO Interrutp flag         */
#define SPI_MIS_TXMIS_Pos           (3U)
#define SPI_MIS_TXMIS_Msk           (0x1UL << SPI_MIS_TXMIS_Pos)                 /*!< 0x00000008 */
#define SPI_MIS_TXMIS               SPI_MIS_TXMIS_Msk                            /*!<Transmit FIFO Interrutp flag        */

/*******************  Bit definition for SPI_ICR register  ******************/
#define SPI_ICR_RORICR_Pos             (0U)
#define SPI_ICR_RORICR_Msk             (0x1UL << SPI_ICR_RORICR_Pos)           /*!< 0x00000001 */
#define SPI_ICR_RORICR                 SPI_ICR_RORICR_Msk                      /*!<Receive Overrun Interrutp clear flag*/
#define SPI_ICR_RTICR_Pos              (1U)
#define SPI_ICR_RTICR_Msk              (0x1UL << SPI_ICR_RTICR_Pos)            /*!< 0x00000002 */
#define SPI_ICR_RTICR                  SPI_ICR_RTICR_Msk                       /*!<Receive Timeout Interrutp clear flag*/

/*******************  Bit definition for SPI_DMACR register  ******************/
#define SPI_DMACR_RXEDMACR_Pos         (0U)
#define SPI_DMACR_RXEDMACR_Msk         (0x1UL << SPI_DMACR_RXEDMACR_Pos)       /*!< 0x00000001 */
#define SPI_DMACR_RXEDMACR             SPI_DMACR_RXEDMACR_Msk                  /*!<Receive DMA Enable                  */
#define SPI_DMACR_TXEDMACR_Pos         (1U)
#define SPI_DMACR_TXEDMACR_Msk         (0x1UL << SPI_DMACR_TXEDMACR_Pos)       /*!< 0x00000002 */
#define SPI_DMACR_TXEDMACR             SPI_DMACR_TXEDMACR_Msk                  /*!<Transmit DMA Enable                 */

/******************************************************************************/
/*                                                                            */
/*                        Analog convert digital Interface                    */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for ADC_START register  ********************/
#define ADC_START_Pos            (0U)
#define ADC_START_Msk            (0x1UL << ADC_START_Pos)                         /*!< 0x00000001 */
#define ADC_START                ADC_START_Msk                                    /*!<START[0] bit (ADC convert start)   */
/*******************  Bit definition for ADC_CLKDIV register  ********************/
#define ADC_CLKDIV_Pos           (0U)
#define ADC_CLKDIV_Msk           (0xFFUL << ADC_CLKDIV_Pos)                       /*!< 0x000000FF */
#define ADC_CLKDIV               ADC_CLKDIV_Msk                                   /*!<CLKDIV[7:0] bits (ADC clk divide)  */
#define ADC_CLKDIV_ADCON_Pos     (8U)
#define ADC_CLKDIV_ADCON_Msk     (0x1UL << ADC_CLKDIV_ADCON_Pos)                  /*!< 0x00000100 */
#define ADC_CLKDIV_ADCON         ADC_CLKDIV_ADCON_Msk                             /*!<CLKDIV[8] bit (ADC on)             */
/*******************  Bit definition for ADC_ADCCON register  ********************/
#define ADC_CON_CH_Pos           (0U)
#define ADC_CON_CH_Msk           (0xFUL << ADC_CON_CH_Pos)                        /*!< 0x0000000F */
#define ADC_CON_CH               ADC_CON_CH_Msk                                   /*!<ADCCON[3:0] bits (ADC channel)     */
#define ADC_CON_CONT_Pos         (4U)
#define ADC_CON_CONT_Msk         (0x1UL << ADC_CON_CONT_Pos)                      /*!< 0x00000010 */
#define ADC_CON_CONT             ADC_CON_CONT_Msk                                 /*!<ADCCON[4] bit (ADC continue mode)  */
#define ADC_CON_DMA_EN_Pos       (5U)
#define ADC_CON_DMA_EN_Msk       (0x1UL << ADC_CON_DMA_EN_Pos)                    /*!< 0x00000020 */
#define ADC_CON_DMA_EN           ADC_CON_DMA_EN_Msk                               /*!<ADCCON[5] bit (ADC dma enable)     */
#define ADC_CON_VTEMP_Pos        (7U)
#define ADC_CON_VTEMP_Msk        (0x1UL << ADC_CON_VTEMP_Pos)                     /*!< 0x00000080 */
#define ADC_CON_VTEMP            ADC_CON_VTEMP_Msk                                /*!<ADCCON[7] bit (ADC VTEMP)          */
#define ADC_CON_DIFF_Pos         (8U)
#define ADC_CON_DIFF_Msk         (0x1UL << ADC_CON_DIFF_Pos)                      /*!< 0x00000100 */
#define ADC_CON_DIFF             ADC_CON_DIFF_Msk                                 /*!<ADCCON[8] bit (ADC DIFF)           */
#define ADC_CON_SIGN_Pos         (9U)
#define ADC_CON_SIGN_Msk         (0x1UL << ADC_CON_SIGN_Pos)                      /*!< 0x00000200 */
#define ADC_CON_SIGN             ADC_CON_SIGN_Msk                                 /*!<ADCCON[9] bit (ADC SIGN)           */
#define ADC_CON_V18_SEL_Pos      (10U)
#define ADC_CON_V18_SEL_Msk      (0x1UL << ADC_CON_V18_SEL_Pos)                   /*!< 0x00000400 */
#define ADC_CON_V18_SEL          ADC_CON_V18_SEL_Msk                              /*!<ADCCON[10] bit (ADC V18_SEL)       */
#define ADC_CON_VDD_SEL_Pos      (11U)
#define ADC_CON_VDD_SEL_Msk      (0x1UL << ADC_CON_VDD_SEL_Pos)                   /*!< 0x00000800 */
#define ADC_CON_VDD_SEL          ADC_CON_VDD_SEL_Msk                              /*!<ADCCON[11] bit (ADC VDD_SEL)       */
/*******************  Bit definition for ADC_TEMPCON register  ********************/
#define ADC_TEMPCON_EN_TS_Pos    (0U)
#define ADC_TEMPCON_EN_TS_Msk    (0x1UL << ADC_TEMPCON_EN_TS_Pos)                 /*!< 0x00000001 */
#define ADC_TEMPCON_EN_TS        ADC_TEMPCON_EN_TS_Msk                            /*!<TEMPCON[0] bit (ADC EN_TS)         */
#define ADC_TEMPCON_ALG_MEAN_Pos (2U)
#define ADC_TEMPCON_ALG_MEAN_Msk (0x3UL << ADC_TEMPCON_ALG_MEAN_Pos)              /*!< 0x0000000C */
#define ADC_TEMPCON_ALG_MEAN     ADC_TEMPCON_ALG_MEAN_Msk                         /*!<TEMPCON[3:2] bits (ADC ALG_MEAN)   */
#define ADC_TEMPCON_ST_SEL_Pos   (4U)
#define ADC_TEMPCON_ST_SEL_Msk   (0xFUL << ADC_TEMPCON_ST_SEL_Pos)                /*!< 0x000000F0 */
#define ADC_TEMPCON_ST_SEL       ADC_TEMPCON_ST_SEL_Msk                           /*!<TEMPCON[7:4] bits (ADC ST_SEL)     */
#define ADC_TEMPCON_TIRIM_Pos    (8U)
#define ADC_TEMPCON_TIRIM_Msk    (0xFUL << ADC_TEMPCON_TIRIM_Pos)                 /*!< 0x00000F00 */
#define ADC_TEMPCON_TIRIM        ADC_TEMPCON_TIRIM_Msk                            /*!<TEMPCON[11:8] bits (ADC TIRIM)     */
#define ADC_TEMPCON_TDOS_Pos     (12U)
#define ADC_TEMPCON_TDOS_Msk     (0xFUL << ADC_TEMPCON_TDOS_Pos)                  /*!< 0x0000F000 */
#define ADC_TEMPCON_TDOS         ADC_TEMPCON_TDOS_Msk                             /*!<TEMPCON[15:12] bits (ADC TDOS)     */
#define ADC_TEMPCON_TDGA_Pos     (16U)
#define ADC_TEMPCON_TDGA_Msk     (0xFUL << ADC_TEMPCON_TDGA_Pos)                  /*!< 0x000F0000 */
#define ADC_TEMPCON_TDGA         ADC_TEMPCON_TDGA_Msk                             /*!<TEMPCON[19:16] bits (ADC TDGA)     */
/*******************  Bit definition for ADC_INTREN register  ********************/
#define ADC_INTR_EN_Pos          (0U)
#define ADC_INTR_EN_Msk          (0x1UL << ADC_INTR_EN_Pos)                       /*!< 0x00000001 */
#define ADC_INTR_EN              ADC_INTR_EN_Msk                                  /*!<INTR_EN[0] bit (ADC interrupt enable)*/
/*******************  Bit definition for ADC_INTR register  ********************/
#define ADC_INTR_Pos             (0U)
#define ADC_INTR_Msk             (0x1UL << ADC_INTR_Pos)                          /*!< 0x00000001 */
#define ADC_INTR                 ADC_INTR_Msk                                     /*!<INTR[0] bit (ADC interrupt status) */
/*******************  Bit definition for ADC_FULLSIZE register  ********************/
#define ADC_FULL_SIZE_Pos        (0U)
#define ADC_FULL_SIZE_Msk        (0xFUL << ADC_FULL_SIZE_Pos)                     /*!< 0x0000000F */
#define ADC_FULL_SIZE            ADC_FULL_SIZE_Msk                                /*!<FULLSIZE[3:0] bits (ADC full size) */
/*******************  Bit definition for ADC_CONTSTOP register  ********************/
#define ADC_STOP_Pos             (0U)
#define ADC_STOP_Msk             (0x1UL << ADC_STOP_Pos)                          /*!< 0x00000001 */
#define ADC_STOP                 ADC_STOP_Msk                                     /*!<STOP[0] bit (ADC convert stop)     */
/*******************  Bit definition for ADC_DONE register  ********************/
#define ADC_DONE_Pos             (0U)
#define ADC_DONE_Msk             (0x1UL << ADC_DONE_Pos)                          /*!< 0x00000001 */
#define ADC_DONE                 ADC_DONE_Msk                                     /*!<DONE[0] bit (ADC convert done)     */
/*******************  Bit definition for ADC_DOUT register  ********************/
#define ADC_DOUT_Pos             (0U)
#define ADC_DOUT_Msk             (0x1UL << ADC_DOUT_Pos)                          /*!< 0x00000FFF */
#define ADC_DOUT                 ADC_DOUT_Msk                                     /*!<DOUT[11:0] bits (ADC convert result)*/
/*******************  Bit definition for ADC_FIFOSTATUS register  ********************/
#define ADC_FIFOSTA_DATA_COUNT_Pos (0U)
#define ADC_FIFOSTA_DATA_COUNT_Msk (0x1FUL << ADC_FIFOSTA_DATA_COUNT_Pos)         /*!< 0x0000001F */
#define ADC_FIFOSTA_DATA_COUNT     ADC_FIFOSTA_DATA_COUNT_Msk                     /*!<FIFOSTATUS[4:0] bits (ADC data count)*/
#define ADC_FIFOSTA_FULL_Pos     (8U)
#define ADC_FIFOSTA_FULL_Msk     (0x1UL << ADC_FIFOSTA_FULL_Pos)                  /*!< 0x00000100 */
#define ADC_FIFOSTA_FULL         ADC_FIFOSTA_FULL_Msk                             /*!<FIFOSTATUS[8] bit (ADC fifo full)  */
#define ADC_FIFOSTA_AFULL_Pos    (16U)
#define ADC_FIFOSTA_AFULL_Msk    (0x1UL << ADC_FIFOSTA_AFULL_Pos)                 /*!< 0x00010000 */
#define ADC_FIFOSTA_AFULL        ADC_FIFOSTA_AFULL_Msk                            /*!<FIFOSTATUS[16] bit (ADC afull)     */
#define ADC_FIFOSTA_EMPTY_Pos    (24U)
#define ADC_FIFOSTA_EMPTY_Msk    (0x1UL << ADC_FIFOSTA_EMPTY_Pos)                 /*!< 0x01000000 */
#define ADC_FIFOSTA_EMPTY        ADC_FIFOSTA_EMPTY_Msk                            /*!<FIFOSTATUS[24] bit (ADC empty)     */


/******************************************************************************/
/*                                                                            */
/*                Ethernet MAC Registers bits definitions                     */
/*                                                                            */
/******************************************************************************/
/* Bit definition for Ethernet MAC Control Register register */
#define ETH_MACCR_WD_Pos                              (23U)
#define ETH_MACCR_WD_Msk                              (0x1UL << ETH_MACCR_WD_Pos) /*!< 0x00800000 */
#define ETH_MACCR_WD                                  ETH_MACCR_WD_Msk         /* Watchdog disable */
#define ETH_MACCR_JD_Pos                              (22U)
#define ETH_MACCR_JD_Msk                              (0x1UL << ETH_MACCR_JD_Pos) /*!< 0x00400000 */
#define ETH_MACCR_JD                                  ETH_MACCR_JD_Msk         /* Jabber disable */
#define ETH_MACCR_IFG_Pos                             (17U)
#define ETH_MACCR_IFG_Msk                             (0x7UL << ETH_MACCR_IFG_Pos) /*!< 0x000E0000 */
#define ETH_MACCR_IFG                                 ETH_MACCR_IFG_Msk        /* Inter-frame gap */
#define ETH_MACCR_IFG_96Bit                           0x00000000U              /* Minimum IFG between frames during transmission is 96Bit */
#define ETH_MACCR_IFG_88Bit                           0x00020000U              /* Minimum IFG between frames during transmission is 88Bit */
#define ETH_MACCR_IFG_80Bit                           0x00040000U              /* Minimum IFG between frames during transmission is 80Bit */
#define ETH_MACCR_IFG_72Bit                           0x00060000U              /* Minimum IFG between frames during transmission is 72Bit */
#define ETH_MACCR_IFG_64Bit                           0x00080000U              /* Minimum IFG between frames during transmission is 64Bit */
#define ETH_MACCR_IFG_56Bit                           0x000A0000U              /* Minimum IFG between frames during transmission is 56Bit */
#define ETH_MACCR_IFG_48Bit                           0x000C0000U              /* Minimum IFG between frames during transmission is 48Bit */
#define ETH_MACCR_IFG_40Bit                           0x000E0000U              /* Minimum IFG between frames during transmission is 40Bit */
#define ETH_MACCR_CSD_Pos                             (16U)
#define ETH_MACCR_CSD_Msk                             (0x1UL << ETH_MACCR_CSD_Pos) /*!< 0x00010000 */
#define ETH_MACCR_CSD                                 ETH_MACCR_CSD_Msk        /* Carrier sense disable (during transmission) */
#define ETH_MACCR_FES_Pos                             (14U)
#define ETH_MACCR_FES_Msk                             (0x1UL << ETH_MACCR_FES_Pos) /*!< 0x00004000 */
#define ETH_MACCR_FES                                 ETH_MACCR_FES_Msk        /* Fast ethernet speed */
#define ETH_MACCR_ROD_Pos                             (13U)
#define ETH_MACCR_ROD_Msk                             (0x1UL << ETH_MACCR_ROD_Pos) /*!< 0x00002000 */
#define ETH_MACCR_ROD                                 ETH_MACCR_ROD_Msk        /* Receive own disable */
#define ETH_MACCR_LM_Pos                              (12U)
#define ETH_MACCR_LM_Msk                              (0x1UL << ETH_MACCR_LM_Pos) /*!< 0x00001000 */
#define ETH_MACCR_LM                                  ETH_MACCR_LM_Msk         /* loopback mode */
#define ETH_MACCR_DM_Pos                              (11U)
#define ETH_MACCR_DM_Msk                              (0x1UL << ETH_MACCR_DM_Pos) /*!< 0x00000800 */
#define ETH_MACCR_DM                                  ETH_MACCR_DM_Msk         /* Duplex mode */
#define ETH_MACCR_IPCO_Pos                            (10U)
#define ETH_MACCR_IPCO_Msk                            (0x1UL << ETH_MACCR_IPCO_Pos) /*!< 0x00000400 */
#define ETH_MACCR_IPCO                                ETH_MACCR_IPCO_Msk       /* IP Checksum offload */
#define ETH_MACCR_RD_Pos                              (9U)
#define ETH_MACCR_RD_Msk                              (0x1UL << ETH_MACCR_RD_Pos) /*!< 0x00000200 */
#define ETH_MACCR_RD                                  ETH_MACCR_RD_Msk         /* Retry disable */
#define ETH_MACCR_APCS_Pos                            (7U)
#define ETH_MACCR_APCS_Msk                            (0x1UL << ETH_MACCR_APCS_Pos) /*!< 0x00000080 */
#define ETH_MACCR_APCS                                ETH_MACCR_APCS_Msk       /* Automatic Pad/CRC stripping */
#define ETH_MACCR_BL_Pos                              (5U)
#define ETH_MACCR_BL_Msk                              (0x3UL << ETH_MACCR_BL_Pos) /*!< 0x00000060 */
#define ETH_MACCR_BL                                  ETH_MACCR_BL_Msk         /* Back-off limit: random integer number (r) of slot time delays before rescheduling
                                                       a transmission attempt during retries after a collision: 0 =< r <2^k */
#define ETH_MACCR_BL_10                               0x00000000U              /* k = min (n, 10) */
#define ETH_MACCR_BL_8                                0x00000020U              /* k = min (n, 8) */
#define ETH_MACCR_BL_4                                0x00000040U              /* k = min (n, 4) */
#define ETH_MACCR_BL_1                                0x00000060U              /* k = min (n, 1) */
#define ETH_MACCR_DC_Pos                              (4U)
#define ETH_MACCR_DC_Msk                              (0x1UL << ETH_MACCR_DC_Pos) /*!< 0x00000010 */
#define ETH_MACCR_DC                                  ETH_MACCR_DC_Msk         /* Defferal check */
#define ETH_MACCR_TE_Pos                              (3U)
#define ETH_MACCR_TE_Msk                              (0x1UL << ETH_MACCR_TE_Pos) /*!< 0x00000008 */
#define ETH_MACCR_TE                                  ETH_MACCR_TE_Msk         /* Transmitter enable */
#define ETH_MACCR_RE_Pos                              (2U)
#define ETH_MACCR_RE_Msk                              (0x1UL << ETH_MACCR_RE_Pos) /*!< 0x00000004 */
#define ETH_MACCR_RE                                  ETH_MACCR_RE_Msk         /* Receiver enable */

/* Bit definition for Ethernet MAC Frame Filter Register */
#define ETH_MACFFR_RA_Pos                             (31U)
#define ETH_MACFFR_RA_Msk                             (0x1UL << ETH_MACFFR_RA_Pos) /*!< 0x80000000 */
#define ETH_MACFFR_RA                                 ETH_MACFFR_RA_Msk        /* Receive all */
#define ETH_MACFFR_HPF_Pos                            (10U)
#define ETH_MACFFR_HPF_Msk                            (0x1UL << ETH_MACFFR_HPF_Pos) /*!< 0x00000400 */
#define ETH_MACFFR_HPF                                ETH_MACFFR_HPF_Msk       /* Hash or perfect filter */
#define ETH_MACFFR_SAF_Pos                            (9U)
#define ETH_MACFFR_SAF_Msk                            (0x1UL << ETH_MACFFR_SAF_Pos) /*!< 0x00000200 */
#define ETH_MACFFR_SAF                                ETH_MACFFR_SAF_Msk       /* Source address filter enable */
#define ETH_MACFFR_SAIF_Pos                           (8U)
#define ETH_MACFFR_SAIF_Msk                           (0x1UL << ETH_MACFFR_SAIF_Pos) /*!< 0x00000100 */
#define ETH_MACFFR_SAIF                               ETH_MACFFR_SAIF_Msk      /* SA inverse filtering */
#define ETH_MACFFR_PCF_Pos                            (6U)
#define ETH_MACFFR_PCF_Msk                            (0x3UL << ETH_MACFFR_PCF_Pos) /*!< 0x000000C0 */
#define ETH_MACFFR_PCF                                ETH_MACFFR_PCF_Msk       /* Pass control frames: 3 cases */
#define ETH_MACFFR_PCF_BlockAll_Pos                   (6U)
#define ETH_MACFFR_PCF_BlockAll_Msk                   (0x1UL << ETH_MACFFR_PCF_BlockAll_Pos) /*!< 0x00000040 */
#define ETH_MACFFR_PCF_BlockAll                       ETH_MACFFR_PCF_BlockAll_Msk /* MAC filters all control frames from reaching the application */
#define ETH_MACFFR_PCF_ForwardAll_Pos                 (7U)
#define ETH_MACFFR_PCF_ForwardAll_Msk                 (0x1UL << ETH_MACFFR_PCF_ForwardAll_Pos) /*!< 0x00000080 */
#define ETH_MACFFR_PCF_ForwardAll                     ETH_MACFFR_PCF_ForwardAll_Msk /* MAC forwards all control frames to application even if they fail the Address Filter */
#define ETH_MACFFR_PCF_ForwardPassedAddrFilter_Pos    (6U)
#define ETH_MACFFR_PCF_ForwardPassedAddrFilter_Msk    (0x3UL << ETH_MACFFR_PCF_ForwardPassedAddrFilter_Pos) /*!< 0x000000C0 */
#define ETH_MACFFR_PCF_ForwardPassedAddrFilter        ETH_MACFFR_PCF_ForwardPassedAddrFilter_Msk /* MAC forwards control frames that pass the Address Filter. */
#define ETH_MACFFR_BFD_Pos                            (5U)
#define ETH_MACFFR_BFD_Msk                            (0x1UL << ETH_MACFFR_BFD_Pos) /*!< 0x00000020 */
#define ETH_MACFFR_BFD                                ETH_MACFFR_BFD_Msk       /* Broadcast frame disable */
#define ETH_MACFFR_PAM_Pos                            (4U)
#define ETH_MACFFR_PAM_Msk                            (0x1UL << ETH_MACFFR_PAM_Pos) /*!< 0x00000010 */
#define ETH_MACFFR_PAM                                ETH_MACFFR_PAM_Msk       /* Pass all mutlicast */
#define ETH_MACFFR_DAIF_Pos                           (3U)
#define ETH_MACFFR_DAIF_Msk                           (0x1UL << ETH_MACFFR_DAIF_Pos) /*!< 0x00000008 */
#define ETH_MACFFR_DAIF                               ETH_MACFFR_DAIF_Msk      /* DA Inverse filtering */
#define ETH_MACFFR_HM_Pos                             (2U)
#define ETH_MACFFR_HM_Msk                             (0x1UL << ETH_MACFFR_HM_Pos) /*!< 0x00000004 */
#define ETH_MACFFR_HM                                 ETH_MACFFR_HM_Msk        /* Hash multicast */
#define ETH_MACFFR_HU_Pos                             (1U)
#define ETH_MACFFR_HU_Msk                             (0x1UL << ETH_MACFFR_HU_Pos) /*!< 0x00000002 */
#define ETH_MACFFR_HU                                 ETH_MACFFR_HU_Msk        /* Hash unicast */
#define ETH_MACFFR_PM_Pos                             (0U)
#define ETH_MACFFR_PM_Msk                             (0x1UL << ETH_MACFFR_PM_Pos) /*!< 0x00000001 */
#define ETH_MACFFR_PM                                 ETH_MACFFR_PM_Msk        /* Promiscuous mode */

/* Bit definition for Ethernet MAC Hash Table High Register */
#define ETH_MACHTHR_HTH_Pos                           (0U)
#define ETH_MACHTHR_HTH_Msk                           (0xFFFFFFFFUL << ETH_MACHTHR_HTH_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACHTHR_HTH                               ETH_MACHTHR_HTH_Msk      /* Hash table high */

/* Bit definition for Ethernet MAC Hash Table Low Register */
#define ETH_MACHTLR_HTL_Pos                           (0U)
#define ETH_MACHTLR_HTL_Msk                           (0xFFFFFFFFUL << ETH_MACHTLR_HTL_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACHTLR_HTL                               ETH_MACHTLR_HTL_Msk      /* Hash table low */

/* Bit definition for Ethernet MAC MII Address Register */
#define ETH_MACMIIAR_PA_Pos                           (11U)
#define ETH_MACMIIAR_PA_Msk                           (0x1FUL << ETH_MACMIIAR_PA_Pos) /*!< 0x0000F800 */
#define ETH_MACMIIAR_PA                               ETH_MACMIIAR_PA_Msk      /* Physical layer address */
#define ETH_MACMIIAR_MR_Pos                           (6U)
#define ETH_MACMIIAR_MR_Msk                           (0x1FUL << ETH_MACMIIAR_MR_Pos) /*!< 0x000007C0 */
#define ETH_MACMIIAR_MR                               ETH_MACMIIAR_MR_Msk      /* MII register in the selected PHY */
#define ETH_MACMIIAR_CR_Pos                           (2U)
#define ETH_MACMIIAR_CR_Msk                           (0x7UL << ETH_MACMIIAR_CR_Pos) /*!< 0x0000001C */
#define ETH_MACMIIAR_CR                               ETH_MACMIIAR_CR_Msk      /* CR clock range: 6 cases */
#define ETH_MACMIIAR_CR_Div42                         0x00000000U              /* HCLK:60-100 MHz; MDC clock= HCLK/42   */
#define ETH_MACMIIAR_CR_Div62_Pos                     (2U)
#define ETH_MACMIIAR_CR_Div62_Msk                     (0x1UL << ETH_MACMIIAR_CR_Div62_Pos) /*!< 0x00000004 */
#define ETH_MACMIIAR_CR_Div62                         ETH_MACMIIAR_CR_Div62_Msk /* HCLK:100-150 MHz; MDC clock= HCLK/62  */
#define ETH_MACMIIAR_CR_Div16_Pos                     (3U)
#define ETH_MACMIIAR_CR_Div16_Msk                     (0x1UL << ETH_MACMIIAR_CR_Div16_Pos) /*!< 0x00000008 */
#define ETH_MACMIIAR_CR_Div16                         ETH_MACMIIAR_CR_Div16_Msk /* HCLK:20-35 MHz; MDC clock= HCLK/16    */
#define ETH_MACMIIAR_CR_Div26_Pos                     (2U)
#define ETH_MACMIIAR_CR_Div26_Msk                     (0x3UL << ETH_MACMIIAR_CR_Div26_Pos) /*!< 0x0000000C */
#define ETH_MACMIIAR_CR_Div26                         ETH_MACMIIAR_CR_Div26_Msk /* HCLK:35-60 MHz; MDC clock= HCLK/26    */
#define ETH_MACMIIAR_CR_Div102_Pos                    (4U)
#define ETH_MACMIIAR_CR_Div102_Msk                    (0x1UL << ETH_MACMIIAR_CR_Div102_Pos) /*!< 0x00000010 */
#define ETH_MACMIIAR_CR_Div102                        ETH_MACMIIAR_CR_Div102_Msk /* HCLK:150-168 MHz; MDC clock= HCLK/102 */
#define ETH_MACMIIAR_MW_Pos                           (1U)
#define ETH_MACMIIAR_MW_Msk                           (0x1UL << ETH_MACMIIAR_MW_Pos) /*!< 0x00000002 */
#define ETH_MACMIIAR_MW                               ETH_MACMIIAR_MW_Msk      /* MII write */
#define ETH_MACMIIAR_MB_Pos                           (0U)
#define ETH_MACMIIAR_MB_Msk                           (0x1UL << ETH_MACMIIAR_MB_Pos) /*!< 0x00000001 */
#define ETH_MACMIIAR_MB                               ETH_MACMIIAR_MB_Msk      /* MII busy  */

/* Bit definition for Ethernet MAC MII Data Register */
#define ETH_MACMIIDR_MD_Pos                           (0U)
#define ETH_MACMIIDR_MD_Msk                           (0xFFFFUL << ETH_MACMIIDR_MD_Pos) /*!< 0x0000FFFF */
#define ETH_MACMIIDR_MD                               ETH_MACMIIDR_MD_Msk      /* MII data: read/write data from/to PHY */

/* Bit definition for Ethernet MAC Flow Control Register */
#define ETH_MACFCR_PT_Pos                             (16U)
#define ETH_MACFCR_PT_Msk                             (0xFFFFUL << ETH_MACFCR_PT_Pos) /*!< 0xFFFF0000 */
#define ETH_MACFCR_PT                                 ETH_MACFCR_PT_Msk        /* Pause time */
#define ETH_MACFCR_ZQPD_Pos                           (7U)
#define ETH_MACFCR_ZQPD_Msk                           (0x1UL << ETH_MACFCR_ZQPD_Pos) /*!< 0x00000080 */
#define ETH_MACFCR_ZQPD                               ETH_MACFCR_ZQPD_Msk      /* Zero-quanta pause disable */
#define ETH_MACFCR_PLT_Pos                            (4U)
#define ETH_MACFCR_PLT_Msk                            (0x3UL << ETH_MACFCR_PLT_Pos) /*!< 0x00000030 */
#define ETH_MACFCR_PLT                                ETH_MACFCR_PLT_Msk       /* Pause low threshold: 4 cases */
#define ETH_MACFCR_PLT_Minus4                         0x00000000U              /* Pause time minus 4 slot times   */
#define ETH_MACFCR_PLT_Minus28_Pos                    (4U)
#define ETH_MACFCR_PLT_Minus28_Msk                    (0x1UL << ETH_MACFCR_PLT_Minus28_Pos) /*!< 0x00000010 */
#define ETH_MACFCR_PLT_Minus28                        ETH_MACFCR_PLT_Minus28_Msk /* Pause time minus 28 slot times  */
#define ETH_MACFCR_PLT_Minus144_Pos                   (5U)
#define ETH_MACFCR_PLT_Minus144_Msk                   (0x1UL << ETH_MACFCR_PLT_Minus144_Pos) /*!< 0x00000020 */
#define ETH_MACFCR_PLT_Minus144                       ETH_MACFCR_PLT_Minus144_Msk /* Pause time minus 144 slot times */
#define ETH_MACFCR_PLT_Minus256_Pos                   (4U)
#define ETH_MACFCR_PLT_Minus256_Msk                   (0x3UL << ETH_MACFCR_PLT_Minus256_Pos) /*!< 0x00000030 */
#define ETH_MACFCR_PLT_Minus256                       ETH_MACFCR_PLT_Minus256_Msk /* Pause time minus 256 slot times */
#define ETH_MACFCR_UPFD_Pos                           (3U)
#define ETH_MACFCR_UPFD_Msk                           (0x1UL << ETH_MACFCR_UPFD_Pos) /*!< 0x00000008 */
#define ETH_MACFCR_UPFD                               ETH_MACFCR_UPFD_Msk      /* Unicast pause frame detect */
#define ETH_MACFCR_RFCE_Pos                           (2U)
#define ETH_MACFCR_RFCE_Msk                           (0x1UL << ETH_MACFCR_RFCE_Pos) /*!< 0x00000004 */
#define ETH_MACFCR_RFCE                               ETH_MACFCR_RFCE_Msk      /* Receive flow control enable */
#define ETH_MACFCR_TFCE_Pos                           (1U)
#define ETH_MACFCR_TFCE_Msk                           (0x1UL << ETH_MACFCR_TFCE_Pos) /*!< 0x00000002 */
#define ETH_MACFCR_TFCE                               ETH_MACFCR_TFCE_Msk      /* Transmit flow control enable */
#define ETH_MACFCR_FCBBPA_Pos                         (0U)
#define ETH_MACFCR_FCBBPA_Msk                         (0x1UL << ETH_MACFCR_FCBBPA_Pos) /*!< 0x00000001 */
#define ETH_MACFCR_FCBBPA                             ETH_MACFCR_FCBBPA_Msk    /* Flow control busy/backpressure activate */

/* Bit definition for Ethernet MAC VLAN Tag Register */
#define ETH_MACVLANTR_VLANTC_Pos                      (16U)
#define ETH_MACVLANTR_VLANTC_Msk                      (0x1UL << ETH_MACVLANTR_VLANTC_Pos) /*!< 0x00010000 */
#define ETH_MACVLANTR_VLANTC                          ETH_MACVLANTR_VLANTC_Msk /* 12-bit VLAN tag comparison */
#define ETH_MACVLANTR_VLANTI_Pos                      (0U)
#define ETH_MACVLANTR_VLANTI_Msk                      (0xFFFFUL << ETH_MACVLANTR_VLANTI_Pos) /*!< 0x0000FFFF */
#define ETH_MACVLANTR_VLANTI                          ETH_MACVLANTR_VLANTI_Msk /* VLAN tag identifier (for receive frames) */

/* Bit definition for Ethernet MAC Remote Wake-UpFrame Filter Register */
#define ETH_MACRWUFFR_D_Pos                           (0U)
#define ETH_MACRWUFFR_D_Msk                           (0xFFFFFFFFUL << ETH_MACRWUFFR_D_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACRWUFFR_D                               ETH_MACRWUFFR_D_Msk      /* Wake-up frame filter register data */
/* Eight sequential Writes to this address (offset 0x28) will write all Wake-UpFrame Filter Registers.
   Eight sequential Reads from this address (offset 0x28) will read all Wake-UpFrame Filter Registers. */
/* Wake-UpFrame Filter Reg0 : Filter 0 Byte Mask
   Wake-UpFrame Filter Reg1 : Filter 1 Byte Mask
   Wake-UpFrame Filter Reg2 : Filter 2 Byte Mask
   Wake-UpFrame Filter Reg3 : Filter 3 Byte Mask
   Wake-UpFrame Filter Reg4 : RSVD - Filter3 Command - RSVD - Filter2 Command -
                              RSVD - Filter1 Command - RSVD - Filter0 Command
   Wake-UpFrame Filter Re5 : Filter3 Offset - Filter2 Offset - Filter1 Offset - Filter0 Offset
   Wake-UpFrame Filter Re6 : Filter1 CRC16 - Filter0 CRC16
   Wake-UpFrame Filter Re7 : Filter3 CRC16 - Filter2 CRC16 */

/* Bit definition for Ethernet MAC PMT Control and Status Register */
#define ETH_MACPMTCSR_WFFRPR_Pos                      (31U)
#define ETH_MACPMTCSR_WFFRPR_Msk                      (0x1UL << ETH_MACPMTCSR_WFFRPR_Pos) /*!< 0x80000000 */
#define ETH_MACPMTCSR_WFFRPR                          ETH_MACPMTCSR_WFFRPR_Msk /* Wake-Up Frame Filter Register Pointer Reset */
#define ETH_MACPMTCSR_GU_Pos                          (9U)
#define ETH_MACPMTCSR_GU_Msk                          (0x1UL << ETH_MACPMTCSR_GU_Pos) /*!< 0x00000200 */
#define ETH_MACPMTCSR_GU                              ETH_MACPMTCSR_GU_Msk     /* Global Unicast                              */
#define ETH_MACPMTCSR_WFR_Pos                         (6U)
#define ETH_MACPMTCSR_WFR_Msk                         (0x1UL << ETH_MACPMTCSR_WFR_Pos) /*!< 0x00000040 */
#define ETH_MACPMTCSR_WFR                             ETH_MACPMTCSR_WFR_Msk    /* Wake-Up Frame Received                      */
#define ETH_MACPMTCSR_MPR_Pos                         (5U)
#define ETH_MACPMTCSR_MPR_Msk                         (0x1UL << ETH_MACPMTCSR_MPR_Pos) /*!< 0x00000020 */
#define ETH_MACPMTCSR_MPR                             ETH_MACPMTCSR_MPR_Msk    /* Magic Packet Received                       */
#define ETH_MACPMTCSR_WFE_Pos                         (2U)
#define ETH_MACPMTCSR_WFE_Msk                         (0x1UL << ETH_MACPMTCSR_WFE_Pos) /*!< 0x00000004 */
#define ETH_MACPMTCSR_WFE                             ETH_MACPMTCSR_WFE_Msk    /* Wake-Up Frame Enable                        */
#define ETH_MACPMTCSR_MPE_Pos                         (1U)
#define ETH_MACPMTCSR_MPE_Msk                         (0x1UL << ETH_MACPMTCSR_MPE_Pos) /*!< 0x00000002 */
#define ETH_MACPMTCSR_MPE                             ETH_MACPMTCSR_MPE_Msk    /* Magic Packet Enable                         */
#define ETH_MACPMTCSR_PD_Pos                          (0U)
#define ETH_MACPMTCSR_PD_Msk                          (0x1UL << ETH_MACPMTCSR_PD_Pos) /*!< 0x00000001 */
#define ETH_MACPMTCSR_PD                              ETH_MACPMTCSR_PD_Msk     /* Power Down                                  */

/* Bit definition for Ethernet MAC debug Register */
#define ETH_MACDBGR_TFF_Pos                           (25U)
#define ETH_MACDBGR_TFF_Msk                           (0x1UL << ETH_MACDBGR_TFF_Pos) /*!< 0x02000000 */
#define ETH_MACDBGR_TFF                               ETH_MACDBGR_TFF_Msk      /* Tx FIFO full                                                            */
#define ETH_MACDBGR_TFNE_Pos                          (24U)
#define ETH_MACDBGR_TFNE_Msk                          (0x1UL << ETH_MACDBGR_TFNE_Pos) /*!< 0x01000000 */
#define ETH_MACDBGR_TFNE                              ETH_MACDBGR_TFNE_Msk     /* Tx FIFO not empty                                                       */
#define ETH_MACDBGR_TFWA_Pos                          (22U)
#define ETH_MACDBGR_TFWA_Msk                          (0x1UL << ETH_MACDBGR_TFWA_Pos) /*!< 0x00400000 */
#define ETH_MACDBGR_TFWA                              ETH_MACDBGR_TFWA_Msk     /* Tx FIFO write active                                                    */
#define ETH_MACDBGR_TFRS_Pos                          (20U)
#define ETH_MACDBGR_TFRS_Msk                          (0x3UL << ETH_MACDBGR_TFRS_Pos) /*!< 0x00300000 */
#define ETH_MACDBGR_TFRS                              ETH_MACDBGR_TFRS_Msk     /* Tx FIFO read status mask                                                */
#define ETH_MACDBGR_TFRS_WRITING_Pos                  (20U)
#define ETH_MACDBGR_TFRS_WRITING_Msk                  (0x3UL << ETH_MACDBGR_TFRS_WRITING_Pos) /*!< 0x00300000 */
#define ETH_MACDBGR_TFRS_WRITING                      ETH_MACDBGR_TFRS_WRITING_Msk /* Writing the received TxStatus or flushing the TxFIFO                    */
#define ETH_MACDBGR_TFRS_WAITING_Pos                  (21U)
#define ETH_MACDBGR_TFRS_WAITING_Msk                  (0x1UL << ETH_MACDBGR_TFRS_WAITING_Pos) /*!< 0x00200000 */
#define ETH_MACDBGR_TFRS_WAITING                      ETH_MACDBGR_TFRS_WAITING_Msk /* Waiting for TxStatus from MAC transmitter                               */
#define ETH_MACDBGR_TFRS_READ_Pos                     (20U)
#define ETH_MACDBGR_TFRS_READ_Msk                     (0x1UL << ETH_MACDBGR_TFRS_READ_Pos) /*!< 0x00100000 */
#define ETH_MACDBGR_TFRS_READ                         ETH_MACDBGR_TFRS_READ_Msk /* Read state (transferring data to the MAC transmitter)                   */
#define ETH_MACDBGR_TFRS_IDLE                         0x00000000U              /* Idle state                                                              */
#define ETH_MACDBGR_MTP_Pos                           (19U)
#define ETH_MACDBGR_MTP_Msk                           (0x1UL << ETH_MACDBGR_MTP_Pos) /*!< 0x00080000 */
#define ETH_MACDBGR_MTP                               ETH_MACDBGR_MTP_Msk      /* MAC transmitter in pause                                                */
#define ETH_MACDBGR_MTFCS_Pos                         (17U)
#define ETH_MACDBGR_MTFCS_Msk                         (0x3UL << ETH_MACDBGR_MTFCS_Pos) /*!< 0x00060000 */
#define ETH_MACDBGR_MTFCS                             ETH_MACDBGR_MTFCS_Msk    /* MAC transmit frame controller status mask                               */
#define ETH_MACDBGR_MTFCS_TRANSFERRING_Pos            (17U)
#define ETH_MACDBGR_MTFCS_TRANSFERRING_Msk            (0x3UL << ETH_MACDBGR_MTFCS_TRANSFERRING_Pos) /*!< 0x00060000 */
#define ETH_MACDBGR_MTFCS_TRANSFERRING                ETH_MACDBGR_MTFCS_TRANSFERRING_Msk /* Transferring input frame for transmission                               */
#define ETH_MACDBGR_MTFCS_GENERATINGPCF_Pos           (18U)
#define ETH_MACDBGR_MTFCS_GENERATINGPCF_Msk           (0x1UL << ETH_MACDBGR_MTFCS_GENERATINGPCF_Pos) /*!< 0x00040000 */
#define ETH_MACDBGR_MTFCS_GENERATINGPCF               ETH_MACDBGR_MTFCS_GENERATINGPCF_Msk /* Generating and transmitting a Pause control frame (in full duplex mode) */
#define ETH_MACDBGR_MTFCS_WAITING_Pos                 (17U)
#define ETH_MACDBGR_MTFCS_WAITING_Msk                 (0x1UL << ETH_MACDBGR_MTFCS_WAITING_Pos) /*!< 0x00020000 */
#define ETH_MACDBGR_MTFCS_WAITING                     ETH_MACDBGR_MTFCS_WAITING_Msk /* Waiting for Status of previous frame or IFG/backoff period to be over   */
#define ETH_MACDBGR_MTFCS_IDLE                        0x00000000U              /* Idle                                                                    */
#define ETH_MACDBGR_MMTEA_Pos                         (16U)
#define ETH_MACDBGR_MMTEA_Msk                         (0x1UL << ETH_MACDBGR_MMTEA_Pos) /*!< 0x00010000 */
#define ETH_MACDBGR_MMTEA                             ETH_MACDBGR_MMTEA_Msk    /* MAC MII transmit engine active                                          */
#define ETH_MACDBGR_RFFL_Pos                          (8U)
#define ETH_MACDBGR_RFFL_Msk                          (0x3UL << ETH_MACDBGR_RFFL_Pos) /*!< 0x00000300 */
#define ETH_MACDBGR_RFFL                              ETH_MACDBGR_RFFL_Msk     /* Rx FIFO fill level mask                                                 */
#define ETH_MACDBGR_RFFL_FULL_Pos                     (8U)
#define ETH_MACDBGR_RFFL_FULL_Msk                     (0x3UL << ETH_MACDBGR_RFFL_FULL_Pos) /*!< 0x00000300 */
#define ETH_MACDBGR_RFFL_FULL                         ETH_MACDBGR_RFFL_FULL_Msk /* RxFIFO full                                                             */
#define ETH_MACDBGR_RFFL_ABOVEFCT_Pos                 (9U)
#define ETH_MACDBGR_RFFL_ABOVEFCT_Msk                 (0x1UL << ETH_MACDBGR_RFFL_ABOVEFCT_Pos) /*!< 0x00000200 */
#define ETH_MACDBGR_RFFL_ABOVEFCT                     ETH_MACDBGR_RFFL_ABOVEFCT_Msk /* RxFIFO fill-level above flow-control activate threshold                 */
#define ETH_MACDBGR_RFFL_BELOWFCT_Pos                 (8U)
#define ETH_MACDBGR_RFFL_BELOWFCT_Msk                 (0x1UL << ETH_MACDBGR_RFFL_BELOWFCT_Pos) /*!< 0x00000100 */
#define ETH_MACDBGR_RFFL_BELOWFCT                     ETH_MACDBGR_RFFL_BELOWFCT_Msk /* RxFIFO fill-level below flow-control de-activate threshold              */
#define ETH_MACDBGR_RFFL_EMPTY                        0x00000000U              /* RxFIFO empty                                                            */
#define ETH_MACDBGR_RFRCS_Pos                         (5U)
#define ETH_MACDBGR_RFRCS_Msk                         (0x3UL << ETH_MACDBGR_RFRCS_Pos) /*!< 0x00000060 */
#define ETH_MACDBGR_RFRCS                             ETH_MACDBGR_RFRCS_Msk    /* Rx FIFO read controller status mask                                     */
#define ETH_MACDBGR_RFRCS_FLUSHING_Pos                (5U)
#define ETH_MACDBGR_RFRCS_FLUSHING_Msk                (0x3UL << ETH_MACDBGR_RFRCS_FLUSHING_Pos) /*!< 0x00000060 */
#define ETH_MACDBGR_RFRCS_FLUSHING                    ETH_MACDBGR_RFRCS_FLUSHING_Msk /* Flushing the frame data and status                                      */
#define ETH_MACDBGR_RFRCS_STATUSREADING_Pos           (6U)
#define ETH_MACDBGR_RFRCS_STATUSREADING_Msk           (0x1UL << ETH_MACDBGR_RFRCS_STATUSREADING_Pos) /*!< 0x00000040 */
#define ETH_MACDBGR_RFRCS_STATUSREADING               ETH_MACDBGR_RFRCS_STATUSREADING_Msk /* Reading frame status (or time-stamp)                                    */
#define ETH_MACDBGR_RFRCS_DATAREADING_Pos             (5U)
#define ETH_MACDBGR_RFRCS_DATAREADING_Msk             (0x1UL << ETH_MACDBGR_RFRCS_DATAREADING_Pos) /*!< 0x00000020 */
#define ETH_MACDBGR_RFRCS_DATAREADING                 ETH_MACDBGR_RFRCS_DATAREADING_Msk /* Reading frame data                                                      */
#define ETH_MACDBGR_RFRCS_IDLE                        0x00000000U              /* IDLE state                                                              */
#define ETH_MACDBGR_RFWRA_Pos                         (4U)
#define ETH_MACDBGR_RFWRA_Msk                         (0x1UL << ETH_MACDBGR_RFWRA_Pos) /*!< 0x00000010 */
#define ETH_MACDBGR_RFWRA                             ETH_MACDBGR_RFWRA_Msk    /* Rx FIFO write controller active                                         */
#define ETH_MACDBGR_MSFRWCS_Pos                       (1U)
#define ETH_MACDBGR_MSFRWCS_Msk                       (0x3UL << ETH_MACDBGR_MSFRWCS_Pos) /*!< 0x00000006 */
#define ETH_MACDBGR_MSFRWCS                           ETH_MACDBGR_MSFRWCS_Msk  /* MAC small FIFO read / write controllers status  mask                    */
#define ETH_MACDBGR_MSFRWCS_1                         (0x2UL << ETH_MACDBGR_MSFRWCS_Pos) /*!< 0x00000004 */
#define ETH_MACDBGR_MSFRWCS_0                         (0x1UL << ETH_MACDBGR_MSFRWCS_Pos) /*!< 0x00000002 */
#define ETH_MACDBGR_MMRPEA_Pos                        (0U)
#define ETH_MACDBGR_MMRPEA_Msk                        (0x1UL << ETH_MACDBGR_MMRPEA_Pos) /*!< 0x00000001 */
#define ETH_MACDBGR_MMRPEA                            ETH_MACDBGR_MMRPEA_Msk   /* MAC MII receive protocol engine active                                  */

/* Bit definition for Ethernet MAC Status Register */
#define ETH_MACSR_TSTS_Pos                            (9U)
#define ETH_MACSR_TSTS_Msk                            (0x1UL << ETH_MACSR_TSTS_Pos) /*!< 0x00000200 */
#define ETH_MACSR_TSTS                                ETH_MACSR_TSTS_Msk       /* Time stamp trigger status */
#define ETH_MACSR_MMCTS_Pos                           (6U)
#define ETH_MACSR_MMCTS_Msk                           (0x1UL << ETH_MACSR_MMCTS_Pos) /*!< 0x00000040 */
#define ETH_MACSR_MMCTS                               ETH_MACSR_MMCTS_Msk      /* MMC transmit status       */
#define ETH_MACSR_MMMCRS_Pos                          (5U)
#define ETH_MACSR_MMMCRS_Msk                          (0x1UL << ETH_MACSR_MMMCRS_Pos) /*!< 0x00000020 */
#define ETH_MACSR_MMMCRS                              ETH_MACSR_MMMCRS_Msk     /* MMC receive status        */
#define ETH_MACSR_MMCS_Pos                            (4U)
#define ETH_MACSR_MMCS_Msk                            (0x1UL << ETH_MACSR_MMCS_Pos) /*!< 0x00000010 */
#define ETH_MACSR_MMCS                                ETH_MACSR_MMCS_Msk       /* MMC status                */
#define ETH_MACSR_PMTS_Pos                            (3U)
#define ETH_MACSR_PMTS_Msk                            (0x1UL << ETH_MACSR_PMTS_Pos) /*!< 0x00000008 */
#define ETH_MACSR_PMTS                                ETH_MACSR_PMTS_Msk       /* PMT status                */

/* Bit definition for Ethernet MAC Interrupt Mask Register */
#define ETH_MACIMR_TSTIM_Pos                          (9U)
#define ETH_MACIMR_TSTIM_Msk                          (0x1UL << ETH_MACIMR_TSTIM_Pos) /*!< 0x00000200 */
#define ETH_MACIMR_TSTIM                              ETH_MACIMR_TSTIM_Msk     /* Time stamp trigger interrupt mask */
#define ETH_MACIMR_PMTIM_Pos                          (3U)
#define ETH_MACIMR_PMTIM_Msk                          (0x1UL << ETH_MACIMR_PMTIM_Pos) /*!< 0x00000008 */
#define ETH_MACIMR_PMTIM                              ETH_MACIMR_PMTIM_Msk     /* PMT interrupt mask                */

/* Bit definition for Ethernet MAC Address0 High Register */
#define ETH_MACA0HR_MACA0H_Pos                        (0U)
#define ETH_MACA0HR_MACA0H_Msk                        (0xFFFFUL << ETH_MACA0HR_MACA0H_Pos) /*!< 0x0000FFFF */
#define ETH_MACA0HR_MACA0H                            ETH_MACA0HR_MACA0H_Msk   /* MAC address0 high */

/* Bit definition for Ethernet MAC Address0 Low Register */
#define ETH_MACA0LR_MACA0L_Pos                        (0U)
#define ETH_MACA0LR_MACA0L_Msk                        (0xFFFFFFFFUL << ETH_MACA0LR_MACA0L_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACA0LR_MACA0L                            ETH_MACA0LR_MACA0L_Msk   /* MAC address0 low */

/* Bit definition for Ethernet MAC Address1 High Register */
#define ETH_MACA1HR_AE_Pos                            (31U)
#define ETH_MACA1HR_AE_Msk                            (0x1UL << ETH_MACA1HR_AE_Pos) /*!< 0x80000000 */
#define ETH_MACA1HR_AE                                ETH_MACA1HR_AE_Msk       /* Address enable */
#define ETH_MACA1HR_SA_Pos                            (30U)
#define ETH_MACA1HR_SA_Msk                            (0x1UL << ETH_MACA1HR_SA_Pos) /*!< 0x40000000 */
#define ETH_MACA1HR_SA                                ETH_MACA1HR_SA_Msk       /* Source address */
#define ETH_MACA1HR_MBC_Pos                           (24U)
#define ETH_MACA1HR_MBC_Msk                           (0x3FUL << ETH_MACA1HR_MBC_Pos) /*!< 0x3F000000 */
#define ETH_MACA1HR_MBC                               ETH_MACA1HR_MBC_Msk      /* Mask byte control: bits to mask for comparison of the MAC Address bytes */
#define ETH_MACA1HR_MBC_HBits15_8                     0x20000000U              /* Mask MAC Address high reg bits [15:8] */
#define ETH_MACA1HR_MBC_HBits7_0                      0x10000000U              /* Mask MAC Address high reg bits [7:0]  */
#define ETH_MACA1HR_MBC_LBits31_24                    0x08000000U              /* Mask MAC Address low reg bits [31:24] */
#define ETH_MACA1HR_MBC_LBits23_16                    0x04000000U              /* Mask MAC Address low reg bits [23:16] */
#define ETH_MACA1HR_MBC_LBits15_8                     0x02000000U              /* Mask MAC Address low reg bits [15:8]  */
#define ETH_MACA1HR_MBC_LBits7_0                      0x01000000U              /* Mask MAC Address low reg bits [7:0]   */
#define ETH_MACA1HR_MACA1H_Pos                        (0U)
#define ETH_MACA1HR_MACA1H_Msk                        (0xFFFFUL << ETH_MACA1HR_MACA1H_Pos) /*!< 0x0000FFFF */
#define ETH_MACA1HR_MACA1H                            ETH_MACA1HR_MACA1H_Msk   /* MAC address1 high */

/* Bit definition for Ethernet MAC Address1 Low Register */
#define ETH_MACA1LR_MACA1L_Pos                        (0U)
#define ETH_MACA1LR_MACA1L_Msk                        (0xFFFFFFFFUL << ETH_MACA1LR_MACA1L_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACA1LR_MACA1L                            ETH_MACA1LR_MACA1L_Msk   /* MAC address1 low */

/* Bit definition for Ethernet MAC Address2 High Register */
#define ETH_MACA2HR_AE_Pos                            (31U)
#define ETH_MACA2HR_AE_Msk                            (0x1UL << ETH_MACA2HR_AE_Pos) /*!< 0x80000000 */
#define ETH_MACA2HR_AE                                ETH_MACA2HR_AE_Msk       /* Address enable */
#define ETH_MACA2HR_SA_Pos                            (30U)
#define ETH_MACA2HR_SA_Msk                            (0x1UL << ETH_MACA2HR_SA_Pos) /*!< 0x40000000 */
#define ETH_MACA2HR_SA                                ETH_MACA2HR_SA_Msk       /* Source address */
#define ETH_MACA2HR_MBC_Pos                           (24U)
#define ETH_MACA2HR_MBC_Msk                           (0x3FUL << ETH_MACA2HR_MBC_Pos) /*!< 0x3F000000 */
#define ETH_MACA2HR_MBC                               ETH_MACA2HR_MBC_Msk      /* Mask byte control */
#define ETH_MACA2HR_MBC_HBits15_8                     0x20000000U              /* Mask MAC Address high reg bits [15:8] */
#define ETH_MACA2HR_MBC_HBits7_0                      0x10000000U              /* Mask MAC Address high reg bits [7:0]  */
#define ETH_MACA2HR_MBC_LBits31_24                    0x08000000U              /* Mask MAC Address low reg bits [31:24] */
#define ETH_MACA2HR_MBC_LBits23_16                    0x04000000U              /* Mask MAC Address low reg bits [23:16] */
#define ETH_MACA2HR_MBC_LBits15_8                     0x02000000U              /* Mask MAC Address low reg bits [15:8]  */
#define ETH_MACA2HR_MBC_LBits7_0                      0x01000000U              /* Mask MAC Address low reg bits [70]    */
#define ETH_MACA2HR_MACA2H_Pos                        (0U)
#define ETH_MACA2HR_MACA2H_Msk                        (0xFFFFUL << ETH_MACA2HR_MACA2H_Pos) /*!< 0x0000FFFF */
#define ETH_MACA2HR_MACA2H                            ETH_MACA2HR_MACA2H_Msk   /* MAC address1 high */

/* Bit definition for Ethernet MAC Address2 Low Register */
#define ETH_MACA2LR_MACA2L_Pos                        (0U)
#define ETH_MACA2LR_MACA2L_Msk                        (0xFFFFFFFFUL << ETH_MACA2LR_MACA2L_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACA2LR_MACA2L                            ETH_MACA2LR_MACA2L_Msk   /* MAC address2 low */

/* Bit definition for Ethernet MAC Address3 High Register */
#define ETH_MACA3HR_AE_Pos                            (31U)
#define ETH_MACA3HR_AE_Msk                            (0x1UL << ETH_MACA3HR_AE_Pos) /*!< 0x80000000 */
#define ETH_MACA3HR_AE                                ETH_MACA3HR_AE_Msk       /* Address enable */
#define ETH_MACA3HR_SA_Pos                            (30U)
#define ETH_MACA3HR_SA_Msk                            (0x1UL << ETH_MACA3HR_SA_Pos) /*!< 0x40000000 */
#define ETH_MACA3HR_SA                                ETH_MACA3HR_SA_Msk       /* Source address */
#define ETH_MACA3HR_MBC_Pos                           (24U)
#define ETH_MACA3HR_MBC_Msk                           (0x3FUL << ETH_MACA3HR_MBC_Pos) /*!< 0x3F000000 */
#define ETH_MACA3HR_MBC                               ETH_MACA3HR_MBC_Msk      /* Mask byte control */
#define ETH_MACA3HR_MBC_HBits15_8                     0x20000000U              /* Mask MAC Address high reg bits [15:8] */
#define ETH_MACA3HR_MBC_HBits7_0                      0x10000000U              /* Mask MAC Address high reg bits [7:0]  */
#define ETH_MACA3HR_MBC_LBits31_24                    0x08000000U              /* Mask MAC Address low reg bits [31:24] */
#define ETH_MACA3HR_MBC_LBits23_16                    0x04000000U              /* Mask MAC Address low reg bits [23:16] */
#define ETH_MACA3HR_MBC_LBits15_8                     0x02000000U              /* Mask MAC Address low reg bits [15:8]  */
#define ETH_MACA3HR_MBC_LBits7_0                      0x01000000U              /* Mask MAC Address low reg bits [70]    */
#define ETH_MACA3HR_MACA3H_Pos                        (0U)
#define ETH_MACA3HR_MACA3H_Msk                        (0xFFFFUL << ETH_MACA3HR_MACA3H_Pos) /*!< 0x0000FFFF */
#define ETH_MACA3HR_MACA3H                            ETH_MACA3HR_MACA3H_Msk   /* MAC address3 high */

/* Bit definition for Ethernet MAC Address3 Low Register */
#define ETH_MACA3LR_MACA3L_Pos                        (0U)
#define ETH_MACA3LR_MACA3L_Msk                        (0xFFFFFFFFUL << ETH_MACA3LR_MACA3L_Pos) /*!< 0xFFFFFFFF */
#define ETH_MACA3LR_MACA3L                            ETH_MACA3LR_MACA3L_Msk   /* MAC address3 low */

/******************************************************************************/
/*                Ethernet MMC Registers bits definition                      */
/******************************************************************************/

/* Bit definition for Ethernet MMC Contol Register */
#define ETH_MMCCR_MCFHP_Pos                           (5U)
#define ETH_MMCCR_MCFHP_Msk                           (0x1UL << ETH_MMCCR_MCFHP_Pos) /*!< 0x00000020 */
#define ETH_MMCCR_MCFHP                               ETH_MMCCR_MCFHP_Msk      /* MMC counter Full-Half preset */
#define ETH_MMCCR_MCP_Pos                             (4U)
#define ETH_MMCCR_MCP_Msk                             (0x1UL << ETH_MMCCR_MCP_Pos) /*!< 0x00000010 */
#define ETH_MMCCR_MCP                                 ETH_MMCCR_MCP_Msk        /* MMC counter preset           */
#define ETH_MMCCR_MCF_Pos                             (3U)
#define ETH_MMCCR_MCF_Msk                             (0x1UL << ETH_MMCCR_MCF_Pos) /*!< 0x00000008 */
#define ETH_MMCCR_MCF                                 ETH_MMCCR_MCF_Msk        /* MMC Counter Freeze           */
#define ETH_MMCCR_ROR_Pos                             (2U)
#define ETH_MMCCR_ROR_Msk                             (0x1UL << ETH_MMCCR_ROR_Pos) /*!< 0x00000004 */
#define ETH_MMCCR_ROR                                 ETH_MMCCR_ROR_Msk        /* Reset on Read                */
#define ETH_MMCCR_CSR_Pos                             (1U)
#define ETH_MMCCR_CSR_Msk                             (0x1UL << ETH_MMCCR_CSR_Pos) /*!< 0x00000002 */
#define ETH_MMCCR_CSR                                 ETH_MMCCR_CSR_Msk        /* Counter Stop Rollover        */
#define ETH_MMCCR_CR_Pos                              (0U)
#define ETH_MMCCR_CR_Msk                              (0x1UL << ETH_MMCCR_CR_Pos) /*!< 0x00000001 */
#define ETH_MMCCR_CR                                  ETH_MMCCR_CR_Msk         /* Counters Reset               */

/* Bit definition for Ethernet MMC Receive Interrupt Register */
#define ETH_MMCRIR_RGUFS_Pos                          (17U)
#define ETH_MMCRIR_RGUFS_Msk                          (0x1UL << ETH_MMCRIR_RGUFS_Pos) /*!< 0x00020000 */
#define ETH_MMCRIR_RGUFS                              ETH_MMCRIR_RGUFS_Msk     /* Set when Rx good unicast frames counter reaches half the maximum value */
#define ETH_MMCRIR_RFAES_Pos                          (6U)
#define ETH_MMCRIR_RFAES_Msk                          (0x1UL << ETH_MMCRIR_RFAES_Pos) /*!< 0x00000040 */
#define ETH_MMCRIR_RFAES                              ETH_MMCRIR_RFAES_Msk     /* Set when Rx alignment error counter reaches half the maximum value */
#define ETH_MMCRIR_RFCES_Pos                          (5U)
#define ETH_MMCRIR_RFCES_Msk                          (0x1UL << ETH_MMCRIR_RFCES_Pos) /*!< 0x00000020 */
#define ETH_MMCRIR_RFCES                              ETH_MMCRIR_RFCES_Msk     /* Set when Rx crc error counter reaches half the maximum value */

/* Bit definition for Ethernet MMC Transmit Interrupt Register */
#define ETH_MMCTIR_TGFS_Pos                           (21U)
#define ETH_MMCTIR_TGFS_Msk                           (0x1UL << ETH_MMCTIR_TGFS_Pos) /*!< 0x00200000 */
#define ETH_MMCTIR_TGFS                               ETH_MMCTIR_TGFS_Msk      /* Set when Tx good frame count counter reaches half the maximum value */
#define ETH_MMCTIR_TGFMSCS_Pos                        (15U)
#define ETH_MMCTIR_TGFMSCS_Msk                        (0x1UL << ETH_MMCTIR_TGFMSCS_Pos) /*!< 0x00008000 */
#define ETH_MMCTIR_TGFMSCS                            ETH_MMCTIR_TGFMSCS_Msk   /* Set when Tx good multi col counter reaches half the maximum value */
#define ETH_MMCTIR_TGFSCS_Pos                         (14U)
#define ETH_MMCTIR_TGFSCS_Msk                         (0x1UL << ETH_MMCTIR_TGFSCS_Pos) /*!< 0x00004000 */
#define ETH_MMCTIR_TGFSCS                             ETH_MMCTIR_TGFSCS_Msk    /* Set when Tx good single col counter reaches half the maximum value */

/* Bit definition for Ethernet MMC Receive Interrupt Mask Register */
#define ETH_MMCRIMR_RGUFM_Pos                         (17U)
#define ETH_MMCRIMR_RGUFM_Msk                         (0x1UL << ETH_MMCRIMR_RGUFM_Pos) /*!< 0x00020000 */
#define ETH_MMCRIMR_RGUFM                             ETH_MMCRIMR_RGUFM_Msk    /* Mask the interrupt when Rx good unicast frames counter reaches half the maximum value */
#define ETH_MMCRIMR_RFAEM_Pos                         (6U)
#define ETH_MMCRIMR_RFAEM_Msk                         (0x1UL << ETH_MMCRIMR_RFAEM_Pos) /*!< 0x00000040 */
#define ETH_MMCRIMR_RFAEM                             ETH_MMCRIMR_RFAEM_Msk    /* Mask the interrupt when when Rx alignment error counter reaches half the maximum value */
#define ETH_MMCRIMR_RFCEM_Pos                         (5U)
#define ETH_MMCRIMR_RFCEM_Msk                         (0x1UL << ETH_MMCRIMR_RFCEM_Pos) /*!< 0x00000020 */
#define ETH_MMCRIMR_RFCEM                             ETH_MMCRIMR_RFCEM_Msk    /* Mask the interrupt when Rx crc error counter reaches half the maximum value */

/* Bit definition for Ethernet MMC Transmit Interrupt Mask Register */
#define ETH_MMCTIMR_TGFM_Pos                          (21U)
#define ETH_MMCTIMR_TGFM_Msk                          (0x1UL << ETH_MMCTIMR_TGFM_Pos) /*!< 0x00200000 */
#define ETH_MMCTIMR_TGFM                              ETH_MMCTIMR_TGFM_Msk     /* Mask the interrupt when Tx good frame count counter reaches half the maximum value */
#define ETH_MMCTIMR_TGFMSCM_Pos                       (15U)
#define ETH_MMCTIMR_TGFMSCM_Msk                       (0x1UL << ETH_MMCTIMR_TGFMSCM_Pos) /*!< 0x00008000 */
#define ETH_MMCTIMR_TGFMSCM                           ETH_MMCTIMR_TGFMSCM_Msk  /* Mask the interrupt when Tx good multi col counter reaches half the maximum value */
#define ETH_MMCTIMR_TGFSCM_Pos                        (14U)
#define ETH_MMCTIMR_TGFSCM_Msk                        (0x1UL << ETH_MMCTIMR_TGFSCM_Pos) /*!< 0x00004000 */
#define ETH_MMCTIMR_TGFSCM                            ETH_MMCTIMR_TGFSCM_Msk   /* Mask the interrupt when Tx good single col counter reaches half the maximum value */

/* Bit definition for Ethernet MMC Transmitted Good Frames after Single Collision Counter Register */
#define ETH_MMCTGFSCCR_TGFSCC_Pos                     (0U)
#define ETH_MMCTGFSCCR_TGFSCC_Msk                     (0xFFFFFFFFUL << ETH_MMCTGFSCCR_TGFSCC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCTGFSCCR_TGFSCC                         ETH_MMCTGFSCCR_TGFSCC_Msk /* Number of successfully transmitted frames after a single collision in Half-duplex mode. */

/* Bit definition for Ethernet MMC Transmitted Good Frames after More than a Single Collision Counter Register */
#define ETH_MMCTGFMSCCR_TGFMSCC_Pos                   (0U)
#define ETH_MMCTGFMSCCR_TGFMSCC_Msk                   (0xFFFFFFFFUL << ETH_MMCTGFMSCCR_TGFMSCC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCTGFMSCCR_TGFMSCC                       ETH_MMCTGFMSCCR_TGFMSCC_Msk /* Number of successfully transmitted frames after more than a single collision in Half-duplex mode. */

/* Bit definition for Ethernet MMC Transmitted Good Frames Counter Register */
#define ETH_MMCTGFCR_TGFC_Pos                         (0U)
#define ETH_MMCTGFCR_TGFC_Msk                         (0xFFFFFFFFUL << ETH_MMCTGFCR_TGFC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCTGFCR_TGFC                             ETH_MMCTGFCR_TGFC_Msk    /* Number of good frames transmitted. */

/* Bit definition for Ethernet MMC Received Frames with CRC Error Counter Register */
#define ETH_MMCRFCECR_RFCEC_Pos                       (0U)
#define ETH_MMCRFCECR_RFCEC_Msk                       (0xFFFFFFFFUL << ETH_MMCRFCECR_RFCEC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCRFCECR_RFCEC                           ETH_MMCRFCECR_RFCEC_Msk  /* Number of frames received with CRC error. */

/* Bit definition for Ethernet MMC Received Frames with Alignement Error Counter Register */
#define ETH_MMCRFAECR_RFAEC_Pos                       (0U)
#define ETH_MMCRFAECR_RFAEC_Msk                       (0xFFFFFFFFUL << ETH_MMCRFAECR_RFAEC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCRFAECR_RFAEC                           ETH_MMCRFAECR_RFAEC_Msk  /* Number of frames received with alignment (dribble) error */

/* Bit definition for Ethernet MMC Received Good Unicast Frames Counter Register */
#define ETH_MMCRGUFCR_RGUFC_Pos                       (0U)
#define ETH_MMCRGUFCR_RGUFC_Msk                       (0xFFFFFFFFUL << ETH_MMCRGUFCR_RGUFC_Pos) /*!< 0xFFFFFFFF */
#define ETH_MMCRGUFCR_RGUFC                           ETH_MMCRGUFCR_RGUFC_Msk  /* Number of good unicast frames received. */

/******************************************************************************/
/*               Ethernet PTP Registers bits definition                       */
/******************************************************************************/

/* Bit definition for Ethernet PTP Time Stamp Contol Register */
#define ETH_PTPTSCR_TSCNT_Pos                         (16U)
#define ETH_PTPTSCR_TSCNT_Msk                         (0x3UL << ETH_PTPTSCR_TSCNT_Pos) /*!< 0x00030000 */
#define ETH_PTPTSCR_TSCNT                             ETH_PTPTSCR_TSCNT_Msk    /* Time stamp clock node type */
#define ETH_PTPTSSR_TSSMRME_Pos                       (15U)
#define ETH_PTPTSSR_TSSMRME_Msk                       (0x1UL << ETH_PTPTSSR_TSSMRME_Pos) /*!< 0x00008000 */
#define ETH_PTPTSSR_TSSMRME                           ETH_PTPTSSR_TSSMRME_Msk  /* Time stamp snapshot for message relevant to master enable */
#define ETH_PTPTSSR_TSSEME_Pos                        (14U)
#define ETH_PTPTSSR_TSSEME_Msk                        (0x1UL << ETH_PTPTSSR_TSSEME_Pos) /*!< 0x00004000 */
#define ETH_PTPTSSR_TSSEME                            ETH_PTPTSSR_TSSEME_Msk   /* Time stamp snapshot for event message enable */
#define ETH_PTPTSSR_TSSIPV4FE_Pos                     (13U)
#define ETH_PTPTSSR_TSSIPV4FE_Msk                     (0x1UL << ETH_PTPTSSR_TSSIPV4FE_Pos) /*!< 0x00002000 */
#define ETH_PTPTSSR_TSSIPV4FE                         ETH_PTPTSSR_TSSIPV4FE_Msk /* Time stamp snapshot for IPv4 frames enable */
#define ETH_PTPTSSR_TSSIPV6FE_Pos                     (12U)
#define ETH_PTPTSSR_TSSIPV6FE_Msk                     (0x1UL << ETH_PTPTSSR_TSSIPV6FE_Pos) /*!< 0x00001000 */
#define ETH_PTPTSSR_TSSIPV6FE                         ETH_PTPTSSR_TSSIPV6FE_Msk /* Time stamp snapshot for IPv6 frames enable */
#define ETH_PTPTSSR_TSSPTPOEFE_Pos                    (11U)
#define ETH_PTPTSSR_TSSPTPOEFE_Msk                    (0x1UL << ETH_PTPTSSR_TSSPTPOEFE_Pos) /*!< 0x00000800 */
#define ETH_PTPTSSR_TSSPTPOEFE                        ETH_PTPTSSR_TSSPTPOEFE_Msk /* Time stamp snapshot for PTP over ethernet frames enable */
#define ETH_PTPTSSR_TSPTPPSV2E_Pos                    (10U)
#define ETH_PTPTSSR_TSPTPPSV2E_Msk                    (0x1UL << ETH_PTPTSSR_TSPTPPSV2E_Pos) /*!< 0x00000400 */
#define ETH_PTPTSSR_TSPTPPSV2E                        ETH_PTPTSSR_TSPTPPSV2E_Msk /* Time stamp PTP packet snooping for version2 format enable */
#define ETH_PTPTSSR_TSSSR_Pos                         (9U)
#define ETH_PTPTSSR_TSSSR_Msk                         (0x1UL << ETH_PTPTSSR_TSSSR_Pos) /*!< 0x00000200 */
#define ETH_PTPTSSR_TSSSR                             ETH_PTPTSSR_TSSSR_Msk    /* Time stamp Sub-seconds rollover */
#define ETH_PTPTSSR_TSSARFE_Pos                       (8U)
#define ETH_PTPTSSR_TSSARFE_Msk                       (0x1UL << ETH_PTPTSSR_TSSARFE_Pos) /*!< 0x00000100 */
#define ETH_PTPTSSR_TSSARFE                           ETH_PTPTSSR_TSSARFE_Msk  /* Time stamp snapshot for all received frames enable */

#define ETH_PTPTSCR_TSARU_Pos                         (5U)
#define ETH_PTPTSCR_TSARU_Msk                         (0x1UL << ETH_PTPTSCR_TSARU_Pos) /*!< 0x00000020 */
#define ETH_PTPTSCR_TSARU                             ETH_PTPTSCR_TSARU_Msk    /* Addend register update */
#define ETH_PTPTSCR_TSITE_Pos                         (4U)
#define ETH_PTPTSCR_TSITE_Msk                         (0x1UL << ETH_PTPTSCR_TSITE_Pos) /*!< 0x00000010 */
#define ETH_PTPTSCR_TSITE                             ETH_PTPTSCR_TSITE_Msk    /* Time stamp interrupt trigger enable */
#define ETH_PTPTSCR_TSSTU_Pos                         (3U)
#define ETH_PTPTSCR_TSSTU_Msk                         (0x1UL << ETH_PTPTSCR_TSSTU_Pos) /*!< 0x00000008 */
#define ETH_PTPTSCR_TSSTU                             ETH_PTPTSCR_TSSTU_Msk    /* Time stamp update */
#define ETH_PTPTSCR_TSSTI_Pos                         (2U)
#define ETH_PTPTSCR_TSSTI_Msk                         (0x1UL << ETH_PTPTSCR_TSSTI_Pos) /*!< 0x00000004 */
#define ETH_PTPTSCR_TSSTI                             ETH_PTPTSCR_TSSTI_Msk    /* Time stamp initialize */
#define ETH_PTPTSCR_TSFCU_Pos                         (1U)
#define ETH_PTPTSCR_TSFCU_Msk                         (0x1UL << ETH_PTPTSCR_TSFCU_Pos) /*!< 0x00000002 */
#define ETH_PTPTSCR_TSFCU                             ETH_PTPTSCR_TSFCU_Msk    /* Time stamp fine or coarse update */
#define ETH_PTPTSCR_TSE_Pos                           (0U)
#define ETH_PTPTSCR_TSE_Msk                           (0x1UL << ETH_PTPTSCR_TSE_Pos) /*!< 0x00000001 */
#define ETH_PTPTSCR_TSE                               ETH_PTPTSCR_TSE_Msk      /* Time stamp enable */

/* Bit definition for Ethernet PTP Sub-Second Increment Register */
#define ETH_PTPSSIR_STSSI_Pos                         (0U)
#define ETH_PTPSSIR_STSSI_Msk                         (0xFFUL << ETH_PTPSSIR_STSSI_Pos) /*!< 0x000000FF */
#define ETH_PTPSSIR_STSSI                             ETH_PTPSSIR_STSSI_Msk    /* System time Sub-second increment value */

/* Bit definition for Ethernet PTP Time Stamp High Register */
#define ETH_PTPTSHR_STS_Pos                           (0U)
#define ETH_PTPTSHR_STS_Msk                           (0xFFFFFFFFUL << ETH_PTPTSHR_STS_Pos) /*!< 0xFFFFFFFF */
#define ETH_PTPTSHR_STS                               ETH_PTPTSHR_STS_Msk      /* System Time second */

/* Bit definition for Ethernet PTP Time Stamp Low Register */
#define ETH_PTPTSLR_STPNS_Pos                         (31U)
#define ETH_PTPTSLR_STPNS_Msk                         (0x1UL << ETH_PTPTSLR_STPNS_Pos) /*!< 0x80000000 */
#define ETH_PTPTSLR_STPNS                             ETH_PTPTSLR_STPNS_Msk    /* System Time Positive or negative time */
#define ETH_PTPTSLR_STSS_Pos                          (0U)
#define ETH_PTPTSLR_STSS_Msk                          (0x7FFFFFFFUL << ETH_PTPTSLR_STSS_Pos) /*!< 0x7FFFFFFF */
#define ETH_PTPTSLR_STSS                              ETH_PTPTSLR_STSS_Msk     /* System Time sub-seconds */

/* Bit definition for Ethernet PTP Time Stamp High Update Register */
#define ETH_PTPTSHUR_TSUS_Pos                         (0U)
#define ETH_PTPTSHUR_TSUS_Msk                         (0xFFFFFFFFUL << ETH_PTPTSHUR_TSUS_Pos) /*!< 0xFFFFFFFF */
#define ETH_PTPTSHUR_TSUS                             ETH_PTPTSHUR_TSUS_Msk    /* Time stamp update seconds */

/* Bit definition for Ethernet PTP Time Stamp Low Update Register */
#define ETH_PTPTSLUR_TSUPNS_Pos                       (31U)
#define ETH_PTPTSLUR_TSUPNS_Msk                       (0x1UL << ETH_PTPTSLUR_TSUPNS_Pos) /*!< 0x80000000 */
#define ETH_PTPTSLUR_TSUPNS                           ETH_PTPTSLUR_TSUPNS_Msk  /* Time stamp update Positive or negative time */
#define ETH_PTPTSLUR_TSUSS_Pos                        (0U)
#define ETH_PTPTSLUR_TSUSS_Msk                        (0x7FFFFFFFUL << ETH_PTPTSLUR_TSUSS_Pos) /*!< 0x7FFFFFFF */
#define ETH_PTPTSLUR_TSUSS                            ETH_PTPTSLUR_TSUSS_Msk   /* Time stamp update sub-seconds */

/* Bit definition for Ethernet PTP Time Stamp Addend Register */
#define ETH_PTPTSAR_TSA_Pos                           (0U)
#define ETH_PTPTSAR_TSA_Msk                           (0xFFFFFFFFUL << ETH_PTPTSAR_TSA_Pos) /*!< 0xFFFFFFFF */
#define ETH_PTPTSAR_TSA                               ETH_PTPTSAR_TSA_Msk      /* Time stamp addend */

/* Bit definition for Ethernet PTP Target Time High Register */
#define ETH_PTPTTHR_TTSH_Pos                          (0U)
#define ETH_PTPTTHR_TTSH_Msk                          (0xFFFFFFFFUL << ETH_PTPTTHR_TTSH_Pos) /*!< 0xFFFFFFFF */
#define ETH_PTPTTHR_TTSH                              ETH_PTPTTHR_TTSH_Msk     /* Target time stamp high */

/* Bit definition for Ethernet PTP Target Time Low Register */
#define ETH_PTPTTLR_TTSL_Pos                          (0U)
#define ETH_PTPTTLR_TTSL_Msk                          (0xFFFFFFFFUL << ETH_PTPTTLR_TTSL_Pos) /*!< 0xFFFFFFFF */
#define ETH_PTPTTLR_TTSL                              ETH_PTPTTLR_TTSL_Msk     /* Target time stamp low */

/* Bit definition for Ethernet PTP Time Stamp Status Register */
#define ETH_PTPTSSR_TSTTR_Pos                         (5U)
#define ETH_PTPTSSR_TSTTR_Msk                         (0x1UL << ETH_PTPTSSR_TSTTR_Pos) /*!< 0x00000020 */
#define ETH_PTPTSSR_TSTTR                             ETH_PTPTSSR_TSTTR_Msk    /* Time stamp target time reached */
#define ETH_PTPTSSR_TSSO_Pos                          (4U)
#define ETH_PTPTSSR_TSSO_Msk                          (0x1UL << ETH_PTPTSSR_TSSO_Pos) /*!< 0x00000010 */
#define ETH_PTPTSSR_TSSO                              ETH_PTPTSSR_TSSO_Msk     /* Time stamp seconds overflow */

/******************************************************************************/
/*                 Ethernet DMA Registers bits definition                     */
/******************************************************************************/

/* Bit definition for Ethernet DMA Bus Mode Register */
#define ETH_DMABMR_AAB_Pos                            (25U)
#define ETH_DMABMR_AAB_Msk                            (0x1UL << ETH_DMABMR_AAB_Pos) /*!< 0x02000000 */
#define ETH_DMABMR_AAB                                ETH_DMABMR_AAB_Msk       /* Address-Aligned beats */
#define ETH_DMABMR_FPM_Pos                            (24U)
#define ETH_DMABMR_FPM_Msk                            (0x1UL << ETH_DMABMR_FPM_Pos) /*!< 0x01000000 */
#define ETH_DMABMR_FPM                                ETH_DMABMR_FPM_Msk       /* 4xPBL mode */
#define ETH_DMABMR_USP_Pos                            (23U)
#define ETH_DMABMR_USP_Msk                            (0x1UL << ETH_DMABMR_USP_Pos) /*!< 0x00800000 */
#define ETH_DMABMR_USP                                ETH_DMABMR_USP_Msk       /* Use separate PBL */
#define ETH_DMABMR_RDP_Pos                            (17U)
#define ETH_DMABMR_RDP_Msk                            (0x3FUL << ETH_DMABMR_RDP_Pos) /*!< 0x007E0000 */
#define ETH_DMABMR_RDP                                ETH_DMABMR_RDP_Msk       /* RxDMA PBL */
#define ETH_DMABMR_RDP_1Beat                          0x00020000U              /* maximum number of beats to be transferred in one RxDMA transaction is 1 */
#define ETH_DMABMR_RDP_2Beat                          0x00040000U              /* maximum number of beats to be transferred in one RxDMA transaction is 2 */
#define ETH_DMABMR_RDP_4Beat                          0x00080000U              /* maximum number of beats to be transferred in one RxDMA transaction is 4 */
#define ETH_DMABMR_RDP_8Beat                          0x00100000U              /* maximum number of beats to be transferred in one RxDMA transaction is 8 */
#define ETH_DMABMR_RDP_16Beat                         0x00200000U              /* maximum number of beats to be transferred in one RxDMA transaction is 16 */
#define ETH_DMABMR_RDP_32Beat                         0x00400000U              /* maximum number of beats to be transferred in one RxDMA transaction is 32 */
#define ETH_DMABMR_RDP_4xPBL_4Beat                    0x01020000U              /* maximum number of beats to be transferred in one RxDMA transaction is 4 */
#define ETH_DMABMR_RDP_4xPBL_8Beat                    0x01040000U              /* maximum number of beats to be transferred in one RxDMA transaction is 8 */
#define ETH_DMABMR_RDP_4xPBL_16Beat                   0x01080000U              /* maximum number of beats to be transferred in one RxDMA transaction is 16 */
#define ETH_DMABMR_RDP_4xPBL_32Beat                   0x01100000U              /* maximum number of beats to be transferred in one RxDMA transaction is 32 */
#define ETH_DMABMR_RDP_4xPBL_64Beat                   0x01200000U              /* maximum number of beats to be transferred in one RxDMA transaction is 64 */
#define ETH_DMABMR_RDP_4xPBL_128Beat                  0x01400000U              /* maximum number of beats to be transferred in one RxDMA transaction is 128 */
#define ETH_DMABMR_FB_Pos                             (16U)
#define ETH_DMABMR_FB_Msk                             (0x1UL << ETH_DMABMR_FB_Pos) /*!< 0x00010000 */
#define ETH_DMABMR_FB                                 ETH_DMABMR_FB_Msk        /* Fixed Burst */
#define ETH_DMABMR_RTPR_Pos                           (14U)
#define ETH_DMABMR_RTPR_Msk                           (0x3UL << ETH_DMABMR_RTPR_Pos) /*!< 0x0000C000 */
#define ETH_DMABMR_RTPR                               ETH_DMABMR_RTPR_Msk      /* Rx Tx priority ratio */
#define ETH_DMABMR_RTPR_1_1                           0x00000000U              /* Rx Tx priority ratio */
#define ETH_DMABMR_RTPR_2_1                           0x00004000U              /* Rx Tx priority ratio */
#define ETH_DMABMR_RTPR_3_1                           0x00008000U              /* Rx Tx priority ratio */
#define ETH_DMABMR_RTPR_4_1                           0x0000C000U              /* Rx Tx priority ratio */
#define ETH_DMABMR_PBL_Pos                            (8U)
#define ETH_DMABMR_PBL_Msk                            (0x3FUL << ETH_DMABMR_PBL_Pos) /*!< 0x00003F00 */
#define ETH_DMABMR_PBL                                ETH_DMABMR_PBL_Msk       /* Programmable burst length */
#define ETH_DMABMR_PBL_1Beat                          0x00000100U              /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 1 */
#define ETH_DMABMR_PBL_2Beat                          0x00000200U              /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 2 */
#define ETH_DMABMR_PBL_4Beat                          0x00000400U              /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 4 */
#define ETH_DMABMR_PBL_8Beat                          0x00000800U              /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 8 */
#define ETH_DMABMR_PBL_16Beat                         0x00001000U              /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 16 */
#define ETH_DMABMR_PBL_32Beat                         0x00002000U              /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 32 */
#define ETH_DMABMR_PBL_4xPBL_4Beat                    0x01000100U              /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 4 */
#define ETH_DMABMR_PBL_4xPBL_8Beat                    0x01000200U              /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 8 */
#define ETH_DMABMR_PBL_4xPBL_16Beat                   0x01000400U              /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 16 */
#define ETH_DMABMR_PBL_4xPBL_32Beat                   0x01000800U              /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 32 */
#define ETH_DMABMR_PBL_4xPBL_64Beat                   0x01001000U              /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 64 */
#define ETH_DMABMR_PBL_4xPBL_128Beat                  0x01002000U              /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 128 */
#define ETH_DMABMR_EDE_Pos                            (7U)
#define ETH_DMABMR_EDE_Msk                            (0x1UL << ETH_DMABMR_EDE_Pos) /*!< 0x00000080 */
#define ETH_DMABMR_EDE                                ETH_DMABMR_EDE_Msk       /* Enhanced Descriptor Enable */
#define ETH_DMABMR_DSL_Pos                            (2U)
#define ETH_DMABMR_DSL_Msk                            (0x1FUL << ETH_DMABMR_DSL_Pos) /*!< 0x0000007C */
#define ETH_DMABMR_DSL                                ETH_DMABMR_DSL_Msk       /* Descriptor Skip Length */
#define ETH_DMABMR_DA_Pos                             (1U)
#define ETH_DMABMR_DA_Msk                             (0x1UL << ETH_DMABMR_DA_Pos) /*!< 0x00000002 */
#define ETH_DMABMR_DA                                 ETH_DMABMR_DA_Msk        /* DMA arbitration scheme */
#define ETH_DMABMR_SR_Pos                             (0U)
#define ETH_DMABMR_SR_Msk                             (0x1UL << ETH_DMABMR_SR_Pos) /*!< 0x00000001 */
#define ETH_DMABMR_SR                                 ETH_DMABMR_SR_Msk        /* Software reset */

/* Bit definition for Ethernet DMA Transmit Poll Demand Register */
#define ETH_DMATPDR_TPD_Pos                           (0U)
#define ETH_DMATPDR_TPD_Msk                           (0xFFFFFFFFUL << ETH_DMATPDR_TPD_Pos) /*!< 0xFFFFFFFF */
#define ETH_DMATPDR_TPD                               ETH_DMATPDR_TPD_Msk      /* Transmit poll demand */

/* Bit definition for Ethernet DMA Receive Poll Demand Register */
#define ETH_DMARPDR_RPD_Pos                           (0U)
#define ETH_DMARPDR_RPD_Msk                           (0xFFFFFFFFUL << ETH_DMARPDR_RPD_Pos) /*!< 0xFFFFFFFF */
#define ETH_DMARPDR_RPD                               ETH_DMARPDR_RPD_Msk      /* Receive poll demand  */

/* Bit definition for Ethernet DMA Receive Descriptor List Address Register */
#define ETH_DMARDLAR_SRL_Pos                          (0U)
#define ETH_DMARDLAR_SRL_Msk                          (0xFFFFFFFFUL << ETH_DMARDLAR_SRL_Pos) /*!< 0xFFFFFFFF */
#define ETH_DMARDLAR_SRL                              ETH_DMARDLAR_SRL_Msk     /* Start of receive list */

/* Bit definition for Ethernet DMA Transmit Descriptor List Address Register */
#define ETH_DMATDLAR_STL_Pos                          (0U)
#define ETH_DMATDLAR_STL_Msk                          (0xFFFFFFFFUL << ETH_DMATDLAR_STL_Pos) /*!< 0xFFFFFFFF */
#define ETH_DMATDLAR_STL                              ETH_DMATDLAR_STL_Msk     /* Start of transmit list */

/* Bit definition for Ethernet DMA Status Register */
#define ETH_DMASR_TSTS_Pos                            (29U)
#define ETH_DMASR_TSTS_Msk                            (0x1UL << ETH_DMASR_TSTS_Pos) /*!< 0x20000000 */
#define ETH_DMASR_TSTS                                ETH_DMASR_TSTS_Msk       /* Time-stamp trigger status */
#define ETH_DMASR_PMTS_Pos                            (28U)
#define ETH_DMASR_PMTS_Msk                            (0x1UL << ETH_DMASR_PMTS_Pos) /*!< 0x10000000 */
#define ETH_DMASR_PMTS                                ETH_DMASR_PMTS_Msk       /* PMT status */
#define ETH_DMASR_MMCS_Pos                            (27U)
#define ETH_DMASR_MMCS_Msk                            (0x1UL << ETH_DMASR_MMCS_Pos) /*!< 0x08000000 */
#define ETH_DMASR_MMCS                                ETH_DMASR_MMCS_Msk       /* MMC status */
#define ETH_DMASR_EBS_Pos                             (23U)
#define ETH_DMASR_EBS_Msk                             (0x7UL << ETH_DMASR_EBS_Pos) /*!< 0x03800000 */
#define ETH_DMASR_EBS                                 ETH_DMASR_EBS_Msk        /* Error bits status */
  /* combination with EBS[2:0] for GetFlagStatus function */
#define ETH_DMASR_EBS_DescAccess_Pos                  (25U)
#define ETH_DMASR_EBS_DescAccess_Msk                  (0x1UL << ETH_DMASR_EBS_DescAccess_Pos) /*!< 0x02000000 */
#define ETH_DMASR_EBS_DescAccess                      ETH_DMASR_EBS_DescAccess_Msk /* Error bits 0-data buffer, 1-desc. access */
#define ETH_DMASR_EBS_ReadTransf_Pos                  (24U)
#define ETH_DMASR_EBS_ReadTransf_Msk                  (0x1UL << ETH_DMASR_EBS_ReadTransf_Pos) /*!< 0x01000000 */
#define ETH_DMASR_EBS_ReadTransf                      ETH_DMASR_EBS_ReadTransf_Msk /* Error bits 0-write trnsf, 1-read transfr */
#define ETH_DMASR_EBS_DataTransfTx_Pos                (23U)
#define ETH_DMASR_EBS_DataTransfTx_Msk                (0x1UL << ETH_DMASR_EBS_DataTransfTx_Pos) /*!< 0x00800000 */
#define ETH_DMASR_EBS_DataTransfTx                    ETH_DMASR_EBS_DataTransfTx_Msk /* Error bits 0-Rx DMA, 1-Tx DMA */
#define ETH_DMASR_TPS_Pos                             (20U)
#define ETH_DMASR_TPS_Msk                             (0x7UL << ETH_DMASR_TPS_Pos) /*!< 0x00700000 */
#define ETH_DMASR_TPS                                 ETH_DMASR_TPS_Msk        /* Transmit process state */
#define ETH_DMASR_TPS_Stopped                         0x00000000U              /* Stopped - Reset or Stop Tx Command issued  */
#define ETH_DMASR_TPS_Fetching_Pos                    (20U)
#define ETH_DMASR_TPS_Fetching_Msk                    (0x1UL << ETH_DMASR_TPS_Fetching_Pos) /*!< 0x00100000 */
#define ETH_DMASR_TPS_Fetching                        ETH_DMASR_TPS_Fetching_Msk /* Running - fetching the Tx descriptor */
#define ETH_DMASR_TPS_Waiting_Pos                     (21U)
#define ETH_DMASR_TPS_Waiting_Msk                     (0x1UL << ETH_DMASR_TPS_Waiting_Pos) /*!< 0x00200000 */
#define ETH_DMASR_TPS_Waiting                         ETH_DMASR_TPS_Waiting_Msk /* Running - waiting for status */
#define ETH_DMASR_TPS_Reading_Pos                     (20U)
#define ETH_DMASR_TPS_Reading_Msk                     (0x3UL << ETH_DMASR_TPS_Reading_Pos) /*!< 0x00300000 */
#define ETH_DMASR_TPS_Reading                         ETH_DMASR_TPS_Reading_Msk /* Running - reading the data from host memory */
#define ETH_DMASR_TPS_Suspended_Pos                   (21U)
#define ETH_DMASR_TPS_Suspended_Msk                   (0x3UL << ETH_DMASR_TPS_Suspended_Pos) /*!< 0x00600000 */
#define ETH_DMASR_TPS_Suspended                       ETH_DMASR_TPS_Suspended_Msk /* Suspended - Tx Descriptor unavailabe */
#define ETH_DMASR_TPS_Closing_Pos                     (20U)
#define ETH_DMASR_TPS_Closing_Msk                     (0x7UL << ETH_DMASR_TPS_Closing_Pos) /*!< 0x00700000 */
#define ETH_DMASR_TPS_Closing                         ETH_DMASR_TPS_Closing_Msk /* Running - closing Rx descriptor */
#define ETH_DMASR_RPS_Pos                             (17U)
#define ETH_DMASR_RPS_Msk                             (0x7UL << ETH_DMASR_RPS_Pos) /*!< 0x000E0000 */
#define ETH_DMASR_RPS                                 ETH_DMASR_RPS_Msk        /* Receive process state */
#define ETH_DMASR_RPS_Stopped                         0x00000000U              /* Stopped - Reset or Stop Rx Command issued */
#define ETH_DMASR_RPS_Fetching_Pos                    (17U)
#define ETH_DMASR_RPS_Fetching_Msk                    (0x1UL << ETH_DMASR_RPS_Fetching_Pos) /*!< 0x00020000 */
#define ETH_DMASR_RPS_Fetching                        ETH_DMASR_RPS_Fetching_Msk /* Running - fetching the Rx descriptor */
#define ETH_DMASR_RPS_Waiting_Pos                     (17U)
#define ETH_DMASR_RPS_Waiting_Msk                     (0x3UL << ETH_DMASR_RPS_Waiting_Pos) /*!< 0x00060000 */
#define ETH_DMASR_RPS_Waiting                         ETH_DMASR_RPS_Waiting_Msk /* Running - waiting for packet */
#define ETH_DMASR_RPS_Suspended_Pos                   (19U)
#define ETH_DMASR_RPS_Suspended_Msk                   (0x1UL << ETH_DMASR_RPS_Suspended_Pos) /*!< 0x00080000 */
#define ETH_DMASR_RPS_Suspended                       ETH_DMASR_RPS_Suspended_Msk /* Suspended - Rx Descriptor unavailable */
#define ETH_DMASR_RPS_Closing_Pos                     (17U)
#define ETH_DMASR_RPS_Closing_Msk                     (0x5UL << ETH_DMASR_RPS_Closing_Pos) /*!< 0x000A0000 */
#define ETH_DMASR_RPS_Closing                         ETH_DMASR_RPS_Closing_Msk /* Running - closing descriptor */
#define ETH_DMASR_RPS_Queuing_Pos                     (17U)
#define ETH_DMASR_RPS_Queuing_Msk                     (0x7UL << ETH_DMASR_RPS_Queuing_Pos) /*!< 0x000E0000 */
#define ETH_DMASR_RPS_Queuing                         ETH_DMASR_RPS_Queuing_Msk /* Running - queuing the recieve frame into host memory */
#define ETH_DMASR_NIS_Pos                             (16U)
#define ETH_DMASR_NIS_Msk                             (0x1UL << ETH_DMASR_NIS_Pos) /*!< 0x00010000 */
#define ETH_DMASR_NIS                                 ETH_DMASR_NIS_Msk        /* Normal interrupt summary */
#define ETH_DMASR_AIS_Pos                             (15U)
#define ETH_DMASR_AIS_Msk                             (0x1UL << ETH_DMASR_AIS_Pos) /*!< 0x00008000 */
#define ETH_DMASR_AIS                                 ETH_DMASR_AIS_Msk        /* Abnormal interrupt summary */
#define ETH_DMASR_ERS_Pos                             (14U)
#define ETH_DMASR_ERS_Msk                             (0x1UL << ETH_DMASR_ERS_Pos) /*!< 0x00004000 */
#define ETH_DMASR_ERS                                 ETH_DMASR_ERS_Msk        /* Early receive status */
#define ETH_DMASR_FBES_Pos                            (13U)
#define ETH_DMASR_FBES_Msk                            (0x1UL << ETH_DMASR_FBES_Pos) /*!< 0x00002000 */
#define ETH_DMASR_FBES                                ETH_DMASR_FBES_Msk       /* Fatal bus error status */
#define ETH_DMASR_ETS_Pos                             (10U)
#define ETH_DMASR_ETS_Msk                             (0x1UL << ETH_DMASR_ETS_Pos) /*!< 0x00000400 */
#define ETH_DMASR_ETS                                 ETH_DMASR_ETS_Msk        /* Early transmit status */
#define ETH_DMASR_RWTS_Pos                            (9U)
#define ETH_DMASR_RWTS_Msk                            (0x1UL << ETH_DMASR_RWTS_Pos) /*!< 0x00000200 */
#define ETH_DMASR_RWTS                                ETH_DMASR_RWTS_Msk       /* Receive watchdog timeout status */
#define ETH_DMASR_RPSS_Pos                            (8U)
#define ETH_DMASR_RPSS_Msk                            (0x1UL << ETH_DMASR_RPSS_Pos) /*!< 0x00000100 */
#define ETH_DMASR_RPSS                                ETH_DMASR_RPSS_Msk       /* Receive process stopped status */
#define ETH_DMASR_RBUS_Pos                            (7U)
#define ETH_DMASR_RBUS_Msk                            (0x1UL << ETH_DMASR_RBUS_Pos) /*!< 0x00000080 */
#define ETH_DMASR_RBUS                                ETH_DMASR_RBUS_Msk       /* Receive buffer unavailable status */
#define ETH_DMASR_RS_Pos                              (6U)
#define ETH_DMASR_RS_Msk                              (0x1UL << ETH_DMASR_RS_Pos) /*!< 0x00000040 */
#define ETH_DMASR_RS                                  ETH_DMASR_RS_Msk         /* Receive status */
#define ETH_DMASR_TUS_Pos                             (5U)
#define ETH_DMASR_TUS_Msk                             (0x1UL << ETH_DMASR_TUS_Pos) /*!< 0x00000020 */
#define ETH_DMASR_TUS                                 ETH_DMASR_TUS_Msk        /* Transmit underflow status */
#define ETH_DMASR_ROS_Pos                             (4U)
#define ETH_DMASR_ROS_Msk                             (0x1UL << ETH_DMASR_ROS_Pos) /*!< 0x00000010 */
#define ETH_DMASR_ROS                                 ETH_DMASR_ROS_Msk        /* Receive overflow status */
#define ETH_DMASR_TJTS_Pos                            (3U)
#define ETH_DMASR_TJTS_Msk                            (0x1UL << ETH_DMASR_TJTS_Pos) /*!< 0x00000008 */
#define ETH_DMASR_TJTS                                ETH_DMASR_TJTS_Msk       /* Transmit jabber timeout status */
#define ETH_DMASR_TBUS_Pos                            (2U)
#define ETH_DMASR_TBUS_Msk                            (0x1UL << ETH_DMASR_TBUS_Pos) /*!< 0x00000004 */
#define ETH_DMASR_TBUS                                ETH_DMASR_TBUS_Msk       /* Transmit buffer unavailable status */
#define ETH_DMASR_TPSS_Pos                            (1U)
#define ETH_DMASR_TPSS_Msk                            (0x1UL << ETH_DMASR_TPSS_Pos) /*!< 0x00000002 */
#define ETH_DMASR_TPSS                                ETH_DMASR_TPSS_Msk       /* Transmit process stopped status */
#define ETH_DMASR_TS_Pos                              (0U)
#define ETH_DMASR_TS_Msk                              (0x1UL << ETH_DMASR_TS_Pos) /*!< 0x00000001 */
#define ETH_DMASR_TS                                  ETH_DMASR_TS_Msk         /* Transmit status */

/* Bit definition for Ethernet DMA Operation Mode Register */
#define ETH_DMAOMR_DTCEFD_Pos                         (26U)
#define ETH_DMAOMR_DTCEFD_Msk                         (0x1UL << ETH_DMAOMR_DTCEFD_Pos) /*!< 0x04000000 */
#define ETH_DMAOMR_DTCEFD                             ETH_DMAOMR_DTCEFD_Msk    /* Disable Dropping of TCP/IP checksum error frames */
#define ETH_DMAOMR_RSF_Pos                            (25U)
#define ETH_DMAOMR_RSF_Msk                            (0x1UL << ETH_DMAOMR_RSF_Pos) /*!< 0x02000000 */
#define ETH_DMAOMR_RSF                                ETH_DMAOMR_RSF_Msk       /* Receive store and forward */
#define ETH_DMAOMR_DFRF_Pos                           (24U)
#define ETH_DMAOMR_DFRF_Msk                           (0x1UL << ETH_DMAOMR_DFRF_Pos) /*!< 0x01000000 */
#define ETH_DMAOMR_DFRF                               ETH_DMAOMR_DFRF_Msk      /* Disable flushing of received frames */
#define ETH_DMAOMR_TSF_Pos                            (21U)
#define ETH_DMAOMR_TSF_Msk                            (0x1UL << ETH_DMAOMR_TSF_Pos) /*!< 0x00200000 */
#define ETH_DMAOMR_TSF                                ETH_DMAOMR_TSF_Msk       /* Transmit store and forward */
#define ETH_DMAOMR_FTF_Pos                            (20U)
#define ETH_DMAOMR_FTF_Msk                            (0x1UL << ETH_DMAOMR_FTF_Pos) /*!< 0x00100000 */
#define ETH_DMAOMR_FTF                                ETH_DMAOMR_FTF_Msk       /* Flush transmit FIFO */
#define ETH_DMAOMR_TTC_Pos                            (14U)
#define ETH_DMAOMR_TTC_Msk                            (0x7UL << ETH_DMAOMR_TTC_Pos) /*!< 0x0001C000 */
#define ETH_DMAOMR_TTC                                ETH_DMAOMR_TTC_Msk       /* Transmit threshold control */
#define ETH_DMAOMR_TTC_64Bytes                        0x00000000U              /* threshold level of the MTL Transmit FIFO is 64 Bytes */
#define ETH_DMAOMR_TTC_128Bytes                       0x00004000U              /* threshold level of the MTL Transmit FIFO is 128 Bytes */
#define ETH_DMAOMR_TTC_192Bytes                       0x00008000U              /* threshold level of the MTL Transmit FIFO is 192 Bytes */
#define ETH_DMAOMR_TTC_256Bytes                       0x0000C000U              /* threshold level of the MTL Transmit FIFO is 256 Bytes */
#define ETH_DMAOMR_TTC_40Bytes                        0x00010000U              /* threshold level of the MTL Transmit FIFO is 40 Bytes */
#define ETH_DMAOMR_TTC_32Bytes                        0x00014000U              /* threshold level of the MTL Transmit FIFO is 32 Bytes */
#define ETH_DMAOMR_TTC_24Bytes                        0x00018000U              /* threshold level of the MTL Transmit FIFO is 24 Bytes */
#define ETH_DMAOMR_TTC_16Bytes                        0x0001C000U              /* threshold level of the MTL Transmit FIFO is 16 Bytes */
#define ETH_DMAOMR_ST_Pos                             (13U)
#define ETH_DMAOMR_ST_Msk                             (0x1UL << ETH_DMAOMR_ST_Pos) /*!< 0x00002000 */
#define ETH_DMAOMR_ST                                 ETH_DMAOMR_ST_Msk        /* Start/stop transmission command */
#define ETH_DMAOMR_FEF_Pos                            (7U)
#define ETH_DMAOMR_FEF_Msk                            (0x1UL << ETH_DMAOMR_FEF_Pos) /*!< 0x00000080 */
#define ETH_DMAOMR_FEF                                ETH_DMAOMR_FEF_Msk       /* Forward error frames */
#define ETH_DMAOMR_FUGF_Pos                           (6U)
#define ETH_DMAOMR_FUGF_Msk                           (0x1UL << ETH_DMAOMR_FUGF_Pos) /*!< 0x00000040 */
#define ETH_DMAOMR_FUGF                               ETH_DMAOMR_FUGF_Msk      /* Forward undersized good frames */
#define ETH_DMAOMR_RTC_Pos                            (3U)
#define ETH_DMAOMR_RTC_Msk                            (0x3UL << ETH_DMAOMR_RTC_Pos) /*!< 0x00000018 */
#define ETH_DMAOMR_RTC                                ETH_DMAOMR_RTC_Msk       /* receive threshold control */
#define ETH_DMAOMR_RTC_64Bytes                        0x00000000U              /* threshold level of the MTL Receive FIFO is 64 Bytes */
#define ETH_DMAOMR_RTC_32Bytes                        0x00000008U              /* threshold level of the MTL Receive FIFO is 32 Bytes */
#define ETH_DMAOMR_RTC_96Bytes                        0x00000010U              /* threshold level of the MTL Receive FIFO is 96 Bytes */
#define ETH_DMAOMR_RTC_128Bytes                       0x00000018U              /* threshold level of the MTL Receive FIFO is 128 Bytes */
#define ETH_DMAOMR_OSF_Pos                            (2U)
#define ETH_DMAOMR_OSF_Msk                            (0x1UL << ETH_DMAOMR_OSF_Pos) /*!< 0x00000004 */
#define ETH_DMAOMR_OSF                                ETH_DMAOMR_OSF_Msk       /* operate on second frame */
#define ETH_DMAOMR_SR_Pos                             (1U)
#define ETH_DMAOMR_SR_Msk                             (0x1UL << ETH_DMAOMR_SR_Pos) /*!< 0x00000002 */
#define ETH_DMAOMR_SR                                 ETH_DMAOMR_SR_Msk        /* Start/stop receive */

/* Bit definition for Ethernet DMA Interrupt Enable Register */
#define ETH_DMAIER_NISE_Pos                           (16U)
#define ETH_DMAIER_NISE_Msk                           (0x1UL << ETH_DMAIER_NISE_Pos) /*!< 0x00010000 */
#define ETH_DMAIER_NISE                               ETH_DMAIER_NISE_Msk      /* Normal interrupt summary enable */
#define ETH_DMAIER_AISE_Pos                           (15U)
#define ETH_DMAIER_AISE_Msk                           (0x1UL << ETH_DMAIER_AISE_Pos) /*!< 0x00008000 */
#define ETH_DMAIER_AISE                               ETH_DMAIER_AISE_Msk      /* Abnormal interrupt summary enable */
#define ETH_DMAIER_ERIE_Pos                           (14U)
#define ETH_DMAIER_ERIE_Msk                           (0x1UL << ETH_DMAIER_ERIE_Pos) /*!< 0x00004000 */
#define ETH_DMAIER_ERIE                               ETH_DMAIER_ERIE_Msk      /* Early receive interrupt enable */
#define ETH_DMAIER_FBEIE_Pos                          (13U)
#define ETH_DMAIER_FBEIE_Msk                          (0x1UL << ETH_DMAIER_FBEIE_Pos) /*!< 0x00002000 */
#define ETH_DMAIER_FBEIE                              ETH_DMAIER_FBEIE_Msk     /* Fatal bus error interrupt enable */
#define ETH_DMAIER_ETIE_Pos                           (10U)
#define ETH_DMAIER_ETIE_Msk                           (0x1UL << ETH_DMAIER_ETIE_Pos) /*!< 0x00000400 */
#define ETH_DMAIER_ETIE                               ETH_DMAIER_ETIE_Msk      /* Early transmit interrupt enable */
#define ETH_DMAIER_RWTIE_Pos                          (9U)
#define ETH_DMAIER_RWTIE_Msk                          (0x1UL << ETH_DMAIER_RWTIE_Pos) /*!< 0x00000200 */
#define ETH_DMAIER_RWTIE                              ETH_DMAIER_RWTIE_Msk     /* Receive watchdog timeout interrupt enable */
#define ETH_DMAIER_RPSIE_Pos                          (8U)
#define ETH_DMAIER_RPSIE_Msk                          (0x1UL << ETH_DMAIER_RPSIE_Pos) /*!< 0x00000100 */
#define ETH_DMAIER_RPSIE                              ETH_DMAIER_RPSIE_Msk     /* Receive process stopped interrupt enable */
#define ETH_DMAIER_RBUIE_Pos                          (7U)
#define ETH_DMAIER_RBUIE_Msk                          (0x1UL << ETH_DMAIER_RBUIE_Pos) /*!< 0x00000080 */
#define ETH_DMAIER_RBUIE                              ETH_DMAIER_RBUIE_Msk     /* Receive buffer unavailable interrupt enable */
#define ETH_DMAIER_RIE_Pos                            (6U)
#define ETH_DMAIER_RIE_Msk                            (0x1UL << ETH_DMAIER_RIE_Pos) /*!< 0x00000040 */
#define ETH_DMAIER_RIE                                ETH_DMAIER_RIE_Msk       /* Receive interrupt enable */
#define ETH_DMAIER_TUIE_Pos                           (5U)
#define ETH_DMAIER_TUIE_Msk                           (0x1UL << ETH_DMAIER_TUIE_Pos) /*!< 0x00000020 */
#define ETH_DMAIER_TUIE                               ETH_DMAIER_TUIE_Msk      /* Transmit Underflow interrupt enable */
#define ETH_DMAIER_ROIE_Pos                           (4U)
#define ETH_DMAIER_ROIE_Msk                           (0x1UL << ETH_DMAIER_ROIE_Pos) /*!< 0x00000010 */
#define ETH_DMAIER_ROIE                               ETH_DMAIER_ROIE_Msk      /* Receive Overflow interrupt enable */
#define ETH_DMAIER_TJTIE_Pos                          (3U)
#define ETH_DMAIER_TJTIE_Msk                          (0x1UL << ETH_DMAIER_TJTIE_Pos) /*!< 0x00000008 */
#define ETH_DMAIER_TJTIE                              ETH_DMAIER_TJTIE_Msk     /* Transmit jabber timeout interrupt enable */
#define ETH_DMAIER_TBUIE_Pos                          (2U)
#define ETH_DMAIER_TBUIE_Msk                          (0x1UL << ETH_DMAIER_TBUIE_Pos) /*!< 0x00000004 */
#define ETH_DMAIER_TBUIE                              ETH_DMAIER_TBUIE_Msk     /* Transmit buffer unavailable interrupt enable */
#define ETH_DMAIER_TPSIE_Pos                          (1U)
#define ETH_DMAIER_TPSIE_Msk                          (0x1UL << ETH_DMAIER_TPSIE_Pos) /*!< 0x00000002 */
#define ETH_DMAIER_TPSIE                              ETH_DMAIER_TPSIE_Msk     /* Transmit process stopped interrupt enable */
#define ETH_DMAIER_TIE_Pos                            (0U)
#define ETH_DMAIER_TIE_Msk                            (0x1UL << ETH_DMAIER_TIE_Pos) /*!< 0x00000001 */
#define ETH_DMAIER_TIE                                ETH_DMAIER_TIE_Msk       /* Transmit interrupt enable */

/* Bit definition for Ethernet DMA Missed Frame and Buffer Overflow Counter Register */
#define ETH_DMAMFBOCR_OFOC_Pos                        (28U)
#define ETH_DMAMFBOCR_OFOC_Msk                        (0x1UL << ETH_DMAMFBOCR_OFOC_Pos) /*!< 0x10000000 */
#define ETH_DMAMFBOCR_OFOC                            ETH_DMAMFBOCR_OFOC_Msk   /* Overflow bit for FIFO overflow counter */
#define ETH_DMAMFBOCR_MFA_Pos                         (17U)
#define ETH_DMAMFBOCR_MFA_Msk                         (0x7FFUL << ETH_DMAMFBOCR_MFA_Pos) /*!< 0x0FFE0000 */
#define ETH_DMAMFBOCR_MFA                             ETH_DMAMFBOCR_MFA_Msk    /* Number of frames missed by the application */
#define ETH_DMAMFBOCR_OMFC_Pos                        (16U)
#define ETH_DMAMFBOCR_OMFC_Msk                        (0x1UL << ETH_DMAMFBOCR_OMFC_Pos) /*!< 0x00010000 */
#define ETH_DMAMFBOCR_OMFC                            ETH_DMAMFBOCR_OMFC_Msk   /* Overflow bit for missed frame counter */
#define ETH_DMAMFBOCR_MFC_Pos                         (0U)
#define ETH_DMAMFBOCR_MFC_Msk                         (0xFFFFUL << ETH_DMAMFBOCR_MFC_Pos) /*!< 0x0000FFFF */
#define ETH_DMAMFBOCR_MFC                             ETH_DMAMFBOCR_MFC_Msk    /* Number of frames missed by the controller */

/* Bit definition for Ethernet DMA Current Host Transmit Descriptor Register */
#define ETH_DMACHTDR_HTDAP_Pos                        (0U)
#define ETH_DMACHTDR_HTDAP_Msk                        (0xFFFFFFFFUL << ETH_DMACHTDR_HTDAP_Pos) /*!< 0xFFFFFFFF */
#define ETH_DMACHTDR_HTDAP                            ETH_DMACHTDR_HTDAP_Msk   /* Host transmit descriptor address pointer */

/* Bit definition for Ethernet DMA Current Host Receive Descriptor Register */
#define ETH_DMACHRDR_HRDAP_Pos                        (0U)
#define ETH_DMACHRDR_HRDAP_Msk                        (0xFFFFFFFFUL << ETH_DMACHRDR_HRDAP_Pos) /*!< 0xFFFFFFFF */
#define ETH_DMACHRDR_HRDAP                            ETH_DMACHRDR_HRDAP_Msk   /* Host receive descriptor address pointer */

/* Bit definition for Ethernet DMA Current Host Transmit Buffer Address Register */
#define ETH_DMACHTBAR_HTBAP_Pos                       (0U)
#define ETH_DMACHTBAR_HTBAP_Msk                       (0xFFFFFFFFUL << ETH_DMACHTBAR_HTBAP_Pos) /*!< 0xFFFFFFFF */
#define ETH_DMACHTBAR_HTBAP                           ETH_DMACHTBAR_HTBAP_Msk  /* Host transmit buffer address pointer */

/* Bit definition for Ethernet DMA Current Host Receive Buffer Address Register */
#define ETH_DMACHRBAR_HRBAP_Pos                       (0U)
#define ETH_DMACHRBAR_HRBAP_Msk                       (0xFFFFFFFFUL << ETH_DMACHRBAR_HRBAP_Pos) /*!< 0xFFFFFFFF */
#define ETH_DMACHRBAR_HRBAP                           ETH_DMACHRBAR_HRBAP_Msk  /* Host receive buffer address pointer */

#ifdef __cplusplus
}
#endif

#endif  /* __GC9002_H__ */
