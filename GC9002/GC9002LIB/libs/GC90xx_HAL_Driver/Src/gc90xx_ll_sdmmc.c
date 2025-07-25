/**
  ******************************************************************************
  * @file    gc90xx_ll_sdmmc.c
  * @author  MCD Application Team
  * @brief   SDMMC Low Layer HAL module driver.
  *    
  *          This file provides firmware functions to manage the following 
  *          functionalities of the SDMMC peripheral:
  *           + Initialization/de-initialization functions
  *           + I/O operation functions
  *           + Peripheral Control functions 
  *           + Peripheral State functions
  *         
  @verbatim
  ==============================================================================
                       ##### SDMMC peripheral features #####
  ==============================================================================        
    [..] The SD/SDMMC MMC card host interface (SDMMC) provides an interface between the APB2
         peripheral bus and MultiMedia cards (MMCs), SD memory cards, SDMMC cards and CE-ATA
         devices.
    
    [..] The SDMMC features include the following:
         (+) Full compliance with MultiMedia Card System Specification Version 4.2. Card support
             for three different databus modes: 1-bit (default), 4-bit and 8-bit
         (+) Full compatibility with previous versions of MultiMedia Cards (forward compatibility)
         (+) Full compliance with SD Memory Card Specifications Version 2.0
         (+) Full compliance with SD I/O Card Specification Version 2.0: card support for two
             different data bus modes: 1-bit (default) and 4-bit
         (+) Full support of the CE-ATA features (full compliance with CE-ATA digital protocol
             Rev1.1)
         (+) Data transfer up to 48 MHz for the 8 bit mode
         (+) Data and command output enable signals to control external bidirectional drivers.
                 
   
                           ##### How to use this driver #####
  ==============================================================================
    [..]
      This driver is a considered as a driver of service for external devices drivers 
      that interfaces with the SDMMC peripheral.
      According to the device used (SD card/ MMC card / SDMMC card ...), a set of APIs 
      is used in the device's driver to perform SDMMC operations and functionalities.
   
      This driver is almost transparent for the final user, it is only used to implement other
      functionalities of the external device.
   
    [..]
      (+) The SDMMC clock (SDMMCCLK = 48 MHz) is coming from a specific output of PLL 
          (PLL48CLK). Before start working with SDMMC peripheral make sure that the
          PLL is well configured.
          The SDMMC peripheral uses two clock signals:
          (++) SDMMC adapter clock (SDMMCCLK = 48 MHz)
          (++) APB2 bus clock (PCLK2)
       
          -@@- PCLK2 and SDMMC_CK clock frequencies must respect the following condition:
               Frequency(PCLK2) >= (3 / 8 x Frequency(SDMMC_CK))
  
      (+) Enable/Disable peripheral clock using RCC peripheral macros related to SDMMC
          peripheral.

      (+) Enable the Power ON State using the SDIO_PowerState_ON(SDIOx) 
          function and disable it using the function SDIO_PowerState_ON(SDIOx).
                
      (+) Enable/Disable the clock using the __SDIO_ENABLE()/__SDIO_DISABLE() macros.
  
      (+) Enable/Disable the peripheral interrupts using the macros __SDIO_ENABLE_IT(hSDIO, IT) 
          and __SDIO_DISABLE_IT(hSDIO, IT) if you need to use interrupt mode. 
  
      (+) When using the DMA mode 
          (++) Configure the DMA in the MSP layer of the external device
          (++) Active the needed channel Request 
          (++) Enable the DMA using __SDIO_DMA_ENABLE() macro or Disable it using the macro
               __SDIO_DMA_DISABLE().
  
      (+) To control the CPSM (Command Path State Machine) and send 
          commands to the card use the SDIO_SendCommand(), 
          SDIO_GetCommandResponse() and SDIO_GetResponse() functions. First, user has
          to fill the command structure (pointer to SDIO_CmdInitTypeDef) according 
          to the selected command to be sent.
          The parameters that should be filled are:
           (++) Command Argument
           (++) Command Index
           (++) Command Response type
           (++) Command Wait
           (++) CPSM Status (Enable or Disable).
  
          -@@- To check if the command is well received, read the SDIO_CMDRESP
              register using the SDIO_GetCommandResponse().
              The SDMMC responses registers (SDIO_RESP0 to SDIO_RESP1), use the
              SDIO_GetResponse() function.
  
      (+) To control the DPSM (Data Path State Machine) and send/receive 
           data to/from the card use the SDIO_ConfigData(), SDIO_GetDataCounter(), 
          SDIO_ReadFIFO() and SDIO_WriteFIFO() functions.
  
    *** Read Operations ***
    =======================
    [..]
      (#) First, user has to fill the data structure (pointer to
          SDIO_DataInitTypeDef) according to the selected data type to be received.
          The parameters that should be filled are:
           (++) Data TimeOut
           (++) Data Length
           (++) Data Block size
           (++) Data Transfer direction: should be from card (To SDMMC)
           (++) Data Transfer mode
           (++) DPSM Status (Enable or Disable)
                                     
      (#) Configure the SDMMC resources to receive the data from the card
          according to selected transfer mode (Refer to Step 8, 9 and 10).
  
      (#) Send the selected Read command (refer to step 11).
                    
      (#) Use the SDIO flags/interrupts to check the transfer status.
  
    *** Write Operations ***
    ========================
    [..]
     (#) First, user has to fill the data structure (pointer to
         SDIO_DataInitTypeDef) according to the selected data type to be received.
         The parameters that should be filled are:
          (++) Data TimeOut
          (++) Data Length
          (++) Data Block size
          (++) Data Transfer direction:  should be to card (To CARD)
          (++) Data Transfer mode
          (++) DPSM Status (Enable or Disable)
  
     (#) Configure the SDMMC resources to send the data to the card according to 
         selected transfer mode.
                     
     (#) Send the selected Write command.
                    
     (#) Use the SDIO flags/interrupts to check the transfer status.
       
    *** Command management operations ***
    =====================================
    [..]
     (#) The commands used for Read/Write/Erase operations are managed in 
         separate functions. 
         Each function allows to send the needed command with the related argument,
         then check the response.
         By the same approach, you could implement a command and check the response.
  
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal.h>
#ifdef HAL_SDMMC_MODULE_ENABLED

/** @addtogroup GC90xx_HAL_Driver
  * @{
  */

/** @defgroup SDMMC_LL SDMMC Low Layer
  * @brief Low layer module for SD
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint32_t SDMMC_GetCmdError(SDIO_TypeDef *SDIOx);
static uint32_t SDMMC_GetCmdResp1(SDIO_TypeDef *SDIOx, uint8_t SD_CMD, uint32_t Timeout);
static uint32_t SDMMC_GetCmdResp2(SDIO_TypeDef *SDIOx);
static uint32_t SDMMC_GetCmdResp3(SDIO_TypeDef *SDIOx);
static uint32_t SDMMC_GetCmdResp7(SDIO_TypeDef *SDIOx);
static uint32_t SDMMC_GetCmdResp6(SDIO_TypeDef *SDIOx, uint8_t SD_CMD, uint16_t *pRCA);

/* Exported functions --------------------------------------------------------*/

/** @defgroup SDMMC_LL_Exported_Functions SDMMC Low Layer Exported Functions
  * @{
  */

/** @defgroup HAL_SDMMC_LL_Group1 Initialization de-initialization functions 
 *  @brief    Initialization and Configuration functions 
 *
@verbatim    
 ===============================================================================
              ##### Initialization/de-initialization functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
 */
/**
  * @brief SDIO MSP Initialization
  * This function configures the hardware resources used in this example
  * @param  SDIOx Pointer to SDMMC register base
  * @retval None
*/
__WEAK void SDIO_MspInit(SDIO_TypeDef *SDIOx)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(SDIOx);

    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SD_MspInit could be implemented in the user file
    */
}
/**
  * @brief  Initializes the SDMMC Host according to the specified
  *         parameters in the SDMMC_InitTypeDef and create the associated handle.
  * @param  SDIOx Pointer to SDMMC register base
  * @retval HAL status
  */
HAL_StatusTypeDef SDIO_HostReset(SDIO_HostTypeDef *Host)
{
    Host->GIR |= SD_GIR_SWR;
    while(Host->GIR & SD_GIR_SWR);
    Host->DSR = HAL_RCC_GetAPBClock()/50000;
    return HAL_OK;
}
static HAL_StatusTypeDef SDIO_HostConfig(SDIO_TypeDef *SDIOx, SDIO_HwInitType config)
{
    volatile uint32_t* tmpCR = NULL;
    if(SDIOx->Slot == SDIO_Slot0)
    {
        tmpCR = &(SDIOx->Host->HWInit_CR1_0);
    }
    else if(SDIOx->Slot == SDIO_Slot1)
    {
        tmpCR = &(SDIOx->Host->HWInit_CR1_1);
    }
    else if(SDIOx->Slot == SDIO_Slot2)
    {
        tmpCR = &(SDIOx->Host->HWInit_CR1_2);
    }
    else if(SDIOx->Slot == SDIO_Slot3)
    {
        tmpCR = &(SDIOx->Host->HWInit_CR1_3);
    }
    else
    {
        return HAL_ERROR;
    }
    *(tmpCR++) = config.mCr1.value;
    *(tmpCR++) = config.mCr2.value;
    *(tmpCR++) = config.mCr3.value;
    return HAL_OK;
}
void SDIO_Host_Struct_Init(SDIO_HostInitTypeDef *init)
{
    if(init != NULL)
    {
        init->DDRModeEn[0] = 0;
        init->DDRModeEn[1] = 0;
        init->DDRModeEn[2] = 0;
        init->DDRModeEn[3] = 0;
        init->MMC8ModeEn[0] = 0;
        init->MMC8ModeEn[1] = 0;
        init->MMC8ModeEn[2] = 0;
        init->MMC8ModeEn[3] = 0;
        init->DebouncePeriod = 0;
        init->BurstLength = 0;
        init->CPRMClkEn = 0;
        init->CPRMEn = 0;
    }
}
HAL_StatusTypeDef SDIO_HostInit(SDIO_HostTypeDef *Host, SDIO_HostInitTypeDef init)
{
    __HAL_RCC_SDIO_CLK_ENABLE();
    SDIO_HwInitType config;
    config.mCr1.TCF       = 24;//Timeout Clock Frequency
    config.mCr1.TCU       = 1;//Timeout Clock Unit,0 – kHz, 1 – MHz
    config.mCr1.BCSDCLK   = 48;//Base Clock Frequency For SD Clock, 10MHz to 255MHz
    config.mCr1.MBL       = 0;//Max Block Length, 0-512 Bytes; 0x1-1024 Bytes; 0x2-2048 bytes
    config.mCr1._8EDS     = 1;//8-bit Embedded Device Support;
    config.mCr1.ADMA2S    = 1;//ADMA2 Support
    config.mCr1.ADMA1S    = 1;//ADMA1 Support
    config.mCr1.HSS       = 1;//High Speed Support
    config.mCr1.DMAS      = 1;//SDMA Support
    config.mCr1.SRS       = 1;//Suspend/Resume Support
    config.mCr1.VS33      = 1;//Voltage Support 3.3V
    config.mCr1.VS30      = 0;//Voltage Support 3.0V
    config.mCr1.VS18      = 0;//Voltage Support 1.8V
    config.mCr1._64BS     = 0;//64-bit System Bus Support
    config.mCr1.AIS       = 1;//Asynchronous Interrupt Support
    config.mCr1.SLT       = 0;//Slot Type, 0x0-Removable Card Slot; 0x1-Embedded Slot for One Device; 0x2-Shared Bus Slot

    config.mCr2.SDR104    = 0;//SDR104 Supported
    config.mCr2.DDR50     = 0;//DDR50 Supported
    config.mCr2.DRVA      = 0;//1.8V Line Driver Type A Supported
    config.mCr2.DRVC      = 0;//1.8V Line Driver Type C Supported
    config.mCr2.DRVD      = 0;//1.8V Line Driver Type D Supported
    config.mCr2.RTNGCNT   = 0;//Timer Count for Re-Tuning,2^(n-1) second,if set 0,disable
    config.mCr2.SDR50     = 0;//SDR50 Supported
    config.mCr2.UTSM50    = 0;//Use Tuning for SDR50
    config.mCr2.RTNGM     = 0;//Re-Tuning Modes
    config.mCr2.CLKMPR    = 0;//Clock Multiplier

    config.mCr3.MC33      = 10;//Maximum Current for 3.3V
    config.mCr3.MC30      = 0;//Maximum Current for 3.0V
    config.mCr3.MC18      = 0;//Maximum Current for 1.8V

//    SDIO_HostConfig(SDIOx, config);

    Host->GIR |= (init.DDRModeEn[0] ? 1 : 0) << SD_GIR_S0DDR_Pos;
    Host->GIR |= (init.DDRModeEn[1] ? 1 : 0) << SD_GIR_S1DDR_Pos;
    Host->GIR |= (init.DDRModeEn[2] ? 1 : 0) << SD_GIR_S2DDR_Pos;
    Host->GIR |= (init.DDRModeEn[3] ? 1 : 0) << SD_GIR_S3DDR_Pos;

    Host->GIR |= (init.MMC8ModeEn[0] ? 1 : 0) << SD_GIR_S0MMC8_Pos;
    Host->GIR |= (init.MMC8ModeEn[1] ? 1 : 0) << SD_GIR_S1MMC8_Pos;
    Host->GIR |= (init.MMC8ModeEn[2] ? 1 : 0) << SD_GIR_S2MMC8_Pos;
    Host->GIR |= (init.MMC8ModeEn[3] ? 1 : 0) << SD_GIR_S3MMC8_Pos;

    MODIFY_REG(Host->DSR, SD_DSR_DP_Msk, init.DebouncePeriod);
    MODIFY_REG(Host->BSR, SD_BSR_PBL_Msk, init.BurstLength);

    if(init.CPRMClkEn)
    {
        Host->CPRM_SR |= 1 << SD_CPRM_SR_CPRMCE_Pos;
        while((Host->CPRM_SR & SD_CPRM_SR_CPRMCS) == RESET);
    }
    Host->CPRM_SR |= (init.CPRMEn ? 1 : 0) << SD_CPRM_SR_CPRM_EN_Pos;
}

/**
  * @brief  Initializes the SDMMC Slot according to the specified
  *         parameters in the SDMMC_InitTypeDef and create the associated handle.
  * @param  SDIOx Pointer to SDMMC register base
  * @retval HAL status
  */
HAL_StatusTypeDef SDIO_SlotReset(SDIO_TypeDef *SDIOx)
{
    SDIOx->Slot->HCSR1 |= SD_HCSR1_SRFA;
    while(SDIOx->Slot->HCSR1 & SD_HCSR1_SRFA);
    return HAL_OK;
}
static void SDIO_HCSR0_Struct_Init(SDIO_HCSR0Type *hcsr0)
{
    hcsr0->LEDC   = 0; //LED Control.
    hcsr0->DTW    = 0; //Data Transfer Width.
    hcsr0->HSE    = 0; //High Speed Enable.
    hcsr0->DMASEL = 0; //DMA Select.
    hcsr0->EDTW   = 0; //Extended Data Transfer Width.
    hcsr0->CDTL   = 0; //Card Detect Test Level.
    hcsr0->CDSS   = 0; //Card Detect Signal Selection.
    hcsr0->BP     = 0; //SD Bus Power.
    hcsr0->BVS    = 0; //SD Bus Voltage Select.
    hcsr0->SBGR   = 0; //Stop At Block Gap Request.
    hcsr0->CR     = 0; //Continue Request.
    hcsr0->RWC    = 0; //Read Wait Control.
    hcsr0->IBG    = 0; //Interrupt At Block Gap.
    hcsr0->WCINT  = 0; //Wakeup Event Enable On Card Interrupt.
    hcsr0->WCI    = 0; //Wakeup Event Enable On Card Inserted.
    hcsr0->WCR    = 0; //Wakeup Event Enable On SD Card Removal.
}
static void SDIO_HCSR1_Struct_Init(SDIO_HCSR1Type *hcsr1)
{
    hcsr1->ICE      = 0; //Internal Clock Enable.
    hcsr1->ICS      = 0; //<!- Read Only -!>Internal Clock Stable.
    hcsr1->SDCE     = 0; //SD Clock Enable.
    hcsr1->CGS      = 0; //Clock Generator Select.
    hcsr1->USDCLKFS = 0; //SDCLK Frequency Select (upper).
    hcsr1->SDCLKFS  = 0; //SDCLK Frequency Select (lower).
    hcsr1->DTCV     = 0; //Data Timeout Counter Value.
    hcsr1->SRFA     = 0; //Software Reset For All.
    hcsr1->SRCMD    = 0; //Software Reset For CMD Line.
    hcsr1->SRDAT    = 0; //Software Reset For DAT Line.
}
static void SDIO_HCSR2_Struct_Init(SDIO_HCSR2Type *hcsr2)
{
    hcsr2->ACNE   = 0; //<!- Read Only -!>Auto CMD12 Not Executed.
    hcsr2->ACTE   = 0; //<!- Read Only -!>Auto CMD Timeout Error.
    hcsr2->ACCE   = 0; //<!- Read Only -!>Auto CMD CRC Error.
    hcsr2->ACEBE  = 0; //<!- Read Only -!>Auto CMD End Bit Error.
    hcsr2->ACIE   = 0; //<!- Read Only -!>Auto CMD Index Error.
    hcsr2->CNIACE = 0; //<!- Read Only -!>Command Not Issued By Auto CMD12 Error.
    hcsr2->UHSMS  = 0; //UHS Mode Select.
    hcsr2->_1V8SE = 0; //1.8V Signaling Enable.
    hcsr2->DSS    = 0; //Driver Strength Select.
    hcsr2->EXTNG  = 0; //Execute Tuning.
    hcsr2->SCS    = 0; //Sampling Clock Select.
    hcsr2->AIE    = 0; //Asynchronous Interrupt Enable.
    hcsr2->PVE    = 0; //Preset Value Enable.
}
void SDIO_ResetDATLine(SDIO_TypeDef *SDIOx)
{
    SDIO_HCSR1Type hcsr1;
    hcsr1.value = SDIOx->Slot->HCSR1;
    hcsr1.SRDAT = 1; //Software Reset For DAT Line.
    SDIOx->Slot->HCSR1 = hcsr1.value;
    do
    {
        hcsr1.value = SDIOx->Slot->HCSR1;
    }while(hcsr1.SRDAT);
}
void SDIO_ResetCMDLine(SDIO_TypeDef *SDIOx)
{
    SDIO_HCSR1Type hcsr1;
    hcsr1.value = SDIOx->Slot->HCSR1;
    hcsr1.SRCMD = 1; //Software Reset For CMD Line.
    SDIOx->Slot->HCSR1 = hcsr1.value;
    do
    {
        hcsr1.value = SDIOx->Slot->HCSR1;
    }while(hcsr1.SRCMD);
}
void SDIO_ResetALL(SDIO_TypeDef *SDIOx)
{
    SDIO_HCSR1Type hcsr1;
    hcsr1.value = SDIOx->Slot->HCSR1;
    hcsr1.SRFA = 1; //Software Reset For All.
    SDIOx->Slot->HCSR1 = hcsr1.value;
    do
    {
        hcsr1.value = SDIOx->Slot->HCSR1;
    }while(hcsr1.SRFA);
}
/**
  * @brief  Initializes the SDMMC according to the specified
  *         parameters in the SDMMC_InitTypeDef and create the associated handle.
  * @param  SDIOx Pointer to SDMMC register base
  * @param  Init SDMMC initialization structure   
  * @retval HAL status
  */
HAL_StatusTypeDef SDIO_Init(SDIO_TypeDef *SDIOx, SDIO_InitTypeDef Init)
{
    uint32_t tmpreg = 0U;
    SDIO_HCSR0Type hcsr0;
    SDIO_HCSR1Type hcsr1;
    SDIO_HCSR2Type hcsr2;
    SDIO_HCSR0_Struct_Init(&hcsr0);
    SDIO_HCSR1_Struct_Init(&hcsr1);
    SDIO_HCSR2_Struct_Init(&hcsr2);
    /* Check the parameters */
    assert_param(IS_SDIO_ALL_INSTANCE(SDIOx));
    assert_param(IS_SDIO_SPEED_MODE(Init.SpeedMode));
    assert_param(IS_SDIO_CLOCK_MODE(Init.ClockMode));
    assert_param(IS_SDIO_BUS_WIDE(Init.BusWide));
    assert_param(IS_SDIO_CLKDIV(Init.ClockDiv));

    SDIO_MspInit(SDIOx);

    hcsr0.value  = SDIOx->Slot->HCSR0;
//    hcsr0.WCR    = 0; //Wakeup Event Enable On SD Card Removal.
//    hcsr0.WCI    = 0; //Wakeup Event Enable On Card Inserted.
    hcsr0.WCINT  = 1; //Wakeup Event Enable On Card Interrupt.
//    hcsr0.IBG    = 0; //Interrupt At Block Gap.
    hcsr0.RWC    = 0; //Read Wait Control.
//    hcsr0.CR     = 0; //Continue Request.
    hcsr0.SBGR   = 0; //Stop At Block Gap Request.
//    hcsr0.BVS    = 0; //SD Bus Voltage Select.
//    hcsr0.BP     = 0; //SD Bus Power.
//    hcsr0.CDSS   = 0; //Card Detect Signal Selection.
//    hcsr0.CDTL   = 0; //Card Detect Test Level.
    hcsr0.EDTW   = (Init.BusWide == SDIO_BUS_WIDE_8B ? 1 : 0); //Extended Data Transfer Width.
    hcsr0.DMASEL = SDIO_SDMA; //DMA Select.
    hcsr0.HSE    = Init.SpeedMode; //High Speed Enable. SDIO_Speed_Default
    hcsr0.DTW    = (Init.BusWide == SDIO_BUS_WIDE_4B ? 1 : 0); //Data Transfer Width.
//    hcsr0.LEDC   = 0; //LED Control.

    hcsr1.value  = SDIOx->Slot->HCSR1;
//    hcsr1.SRDAT    = 0; //Software Reset For DAT Line.
//    hcsr1.SRCMD    = 0; //Software Reset For CMD Line.
//    hcsr1.SRFA     = 0; //Software Reset For All.
    hcsr1.DTCV     = 0xC; //Data Timeout Counter Value.
    hcsr1.SDCLKFS  = Init.ClockDiv & 0xFF; //SDCLK Frequency Select (lower).
    hcsr1.USDCLKFS = Init.ClockDiv & 300; //SDCLK Frequency Select (upper).
    hcsr1.CGS      = Init.ClockMode; //Clock Generator Select. select 0 - 10-bit Divider Clock Mode
//    hcsr1.SDCE     = SDIO_DISABLE; //SD Clock Enable.
//    hcsr1.ICS      = 0; //<!- Read Only -!>Internal Clock Stable.
    hcsr1.ICE      = SDIO_ENABLE; //Internal Clock Enable.

    hcsr2.value  = SDIOx->Slot->HCSR2;
//    hcsr2.PVE    = 0; //Preset Value Enable.
//    hcsr2.AIE    = 0; //Asynchronous Interrupt Enable.  This bit is used in 4-bit SD mode only
//    hcsr2.SCS    = 0; //Sampling Clock Select.
//    hcsr2.EXTNG  = 0; //Execute Tuning.
    hcsr2.DSS    = SDIO_Driver_Type_Default; //Driver Strength Select.
//    hcsr2._1V8SE = 0; //1.8V Signaling Enable.
//    hcsr2.UHSMS  = 0; //UHS Mode Select.
//    hcsr2.CNIACE = 0; //<!- Read Only -!>Command Not Issued By Auto CMD12 Error.
//    hcsr2.ACIE   = 0; //<!- Read Only -!>Auto CMD Index Error.
//    hcsr2.ACEBE  = 0; //<!- Read Only -!>Auto CMD End Bit Error.
//    hcsr2.ACCE   = 0; //<!- Read Only -!>Auto CMD CRC Error.
//    hcsr2.ACTE   = 0; //<!- Read Only -!>Auto CMD Timeout Error.
//    hcsr2.ACNE   = 0; //<!- Read Only -!>Auto CMD12 Not Executed.

    //reset AND Wait reset complete
    __SDIO_SOFT_RESET_ALL(SDIOx);
    __SDIO_CLEAR_IT(SDIOx, SDIO_IT_CIN | SDIO_IT_CR);
    __SDIO_ENABLE_IT(SDIOx, SDIO_IT_CIN | SDIO_IT_CR);

    /* Write to SDMMC HCSR0 */
    MODIFY_REG(SDIOx->Slot->HCSR0, SD_HCSR0_WRITE_MASK, hcsr0.value);
    /* Write to SDMMC HCSR1 */
    MODIFY_REG(SDIOx->Slot->HCSR1, SD_HCSR1_WRITE_MASK, hcsr1.value);
    __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECT | SDIO_FLAG_EDT);
    /* Write to SDMMC HCSR2 */
    MODIFY_REG(SDIOx->Slot->HCSR2, SD_HCSR2_WRITE_MASK, hcsr2.value);
    // wait internal clock stable
    while((SDIOx->Slot->HCSR1 & SD_HCSR1_ICS) == RESET);
    //enable sdclk
    SDIOx->Slot->HCSR1 |= SD_HCSR1_SDCE;
    return HAL_OK;
}

/**
  * @}
  */

/** @defgroup HAL_SDMMC_LL_Group2 IO operation functions 
 *  @brief   Data transfers functions 
 *
@verbatim   
 ===============================================================================
                      ##### I/O operation functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to manage the SDMMC data 
    transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Read data (word) from Rx FIFO in blocking mode (polling) 
  * @param  SDIOx Pointer to SDMMC register base
  * @retval HAL status
  */
HAL_StatusTypeDef SDIO_ReadFIFO(SDIO_TypeDef *SDIOx, uint32_t *pReadData)
{
    /* Read data from Rx FIFO */
    *pReadData = SDIOx->Slot->BDPR;
    return HAL_OK;
}
/**
  * @brief  Write data (word) to Tx FIFO in blocking mode (polling) 
  * @param  SDIOx Pointer to SDMMC register base
  * @param  pWriteData pointer to data to write
  * @retval HAL status
  */
HAL_StatusTypeDef SDIO_WriteFIFO(SDIO_TypeDef *SDIOx, uint32_t *pWriteData)
{ 
    /* Write data to FIFO */
    SDIOx->Slot->BDPR = *pWriteData;
    return HAL_OK;
}

/**
  * @}
  */

/** @defgroup HAL_SDMMC_LL_Group3 Peripheral Control functions 
 *  @brief   management functions 
 *
@verbatim   
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to control the SDMMC data 
    transfers.

@endverbatim
  * @{
  */

/**
  * @brief  Set SDMMC Power state to UP.
  * @param  SDIOx Pointer to SDMMC register base
  * @retval HAL status
  */
//HAL_StatusTypeDef SDIO_PowerState_UP(SDIO_TypeDef *SDIOx)
//{
//    uint32_t voltage = SDIO_Bus_Vol_3_3V;
//    uint32_t tmpreg = SDIOx->Slot->Capabilities0 & (SD_Capabilities0_VS33 | SD_Capabilities0_VS30 | SD_Capabilities0_VS18);
//    if((tmpreg & SD_Capabilities0_VS33) == SD_Capabilities0_VS33)
//    {
//        voltage = SDIO_Bus_Vol_3_3V;
//    }
//    else if((tmpreg & SD_Capabilities0_VS30) == SD_Capabilities0_VS30)
//    {
//        voltage = SDIO_Bus_Vol_3_0V;
//    }
//    else if((tmpreg & SD_Capabilities0_VS18) == SD_Capabilities0_VS18)
//    {
//        voltage = SDIO_Bus_Vol_1_8V;
//    }
//    MODIFY_REG(SDIOx->Slot->HCSR0, SD_HCSR0_BVS_Msk, (voltage << SD_HCSR0_BVS_Pos));
//    /* Set power state to ON */
//    SDIOx->Slot->HCSR0 |= SD_HCSR0_BP;
//    __SDIO_ENABLE(SDIOx);
//
//    return HAL_OK;
//}
/**
  * @brief  Set SDMMC Power state to ON. Provides a clock for the card
  * @param  SDIOx Pointer to SDMMC register base
  * @retval HAL status
  */
HAL_StatusTypeDef SDIO_PowerState_ON(SDIO_TypeDef *SDIOx)
{
    uint32_t voltage = SDIO_Bus_Vol_3_3V;
    uint32_t tmpreg = SDIOx->Slot->Capabilities0 & (SD_Capabilities0_VS33 | SD_Capabilities0_VS30 | SD_Capabilities0_VS18);
    if((tmpreg & SD_Capabilities0_VS33) == SD_Capabilities0_VS33)
    {
        voltage = SDIO_Bus_Vol_3_3V;
    }
    else if((tmpreg & SD_Capabilities0_VS30) == SD_Capabilities0_VS30)
    {
        voltage = SDIO_Bus_Vol_3_0V;
    }
    else if((tmpreg & SD_Capabilities0_VS18) == SD_Capabilities0_VS18)
    {
        voltage = SDIO_Bus_Vol_1_8V;
    }
    MODIFY_REG(SDIOx->Slot->HCSR0, SD_HCSR0_BVS_Msk, (voltage << SD_HCSR0_BVS_Pos));
    /* Set power state to ON */
    SDIOx->Slot->HCSR0 |= SD_HCSR0_BP;
    __SDIO_ENABLE(SDIOx);

    return HAL_OK;
}

/**
  * @brief  Set SDMMC Power state to OFF. 
  * @param  SDIOx Pointer to SDMMC register base
  * @retval HAL status
  */
HAL_StatusTypeDef SDIO_PowerState_OFF(SDIO_TypeDef *SDIOx)
{
    /* Set power state to OFF */
    SDIOx->Slot->HCSR0 &= ~SD_HCSR0_BP;
  
    return HAL_OK;
}

/**
  * @brief  Get SDMMC Power state. 
  * @param  SDIOx Pointer to SDMMC register base
  * @retval Power status of the controller. The returned value can be one of the 
  *         following values:
  *            - 0x00: Power OFF
  *            - 0x00000100: Power ON
  */
uint32_t SDIO_GetPowerState(SDIO_TypeDef *SDIOx)
{
    return (SDIOx->Slot->HCSR0 & SD_HCSR0_BP);
}

/**
  * @brief  Configure the SDMMC command path according to the specified parameters in
  *         SDIO_CmdInitTypeDef structure and send the command 
  * @param  SDIOx Pointer to SDMMC register base
  * @param  Command pointer to a SDIO_CmdInitTypeDef structure that contains 
  *         the configuration information for the SDMMC command
  * @retval HAL status
  */
HAL_StatusTypeDef SDIO_SendCommand(SDIO_TypeDef *SDIOx, SDIO_CmdInitTypeDef *Command)
{
    uint32_t tmpreg = 0U;
    /* Check the parameters */
    assert_param(IS_SDIO_CMD_INDEX(Command->CmdIndex));
    assert_param(IS_SDIO_CMD_TYPE(Command->CmdType));
    assert_param(IS_SDIO_EN(Command->CmdIndexCheckEn));
    assert_param(IS_SDIO_EN(Command->CmdCRCCheckEn));
    assert_param(IS_SDIO_EN(Command->DataEn));
    assert_param(IS_SDIO_TRANSFER_DIR(Command->TransferDir));
    assert_param(IS_SDIO_RESPONSE(Command->Response));
    assert_param(IS_SDIO_DATA_BLK_SEL(Command->DataBlockSelect));
    assert_param(IS_SDIO_AUTO_CMD_SEL(Command->AutoCmdSelect));
    assert_param(IS_SDIO_EN(Command->BlockCountEn));
    assert_param(IS_SDIO_EN(Command->DMAEn));

    /* verify that no other command/response cycle is currently in progress by checking SRS9.CICMD bit (Command Inhibit CMD)
     * When SRS9.CICMD = 0, then the CMD line is idle and the software is free to send another command
     */
    while((SDIOx->Slot->PSR & SD_PSR_CICMD) != RESET);

    /*First, the software writes an appropriate command argument, 32-bit field in SRS2. */
    /* Set the SDMMC Argument value */
    SDIOx->Slot->ARG1 = Command->Argument;
    __SDIO_IT_STATE_ENABLE(SDIOx, SDIO_IT_STATE_ALL);
    __SDIO_CLEAR_FLAG(SDIOx, SDIO_IT_STATE_ALL);

    if(Command->CmdIndex == SDMMC_CMD_SEND_OP_COND ||
       Command->CmdIndex == SDMMC_CMD_ALL_SEND_CID ||
       Command->CmdIndex == SDMMC_CMD_SET_REL_ADDR ||
       Command->CmdIndex == SDMMC_CMD_HS_SEND_EXT_CSD ||
       Command->CmdIndex == SDMMC_CMD_SEND_CSD ||
       Command->CmdIndex == SDMMC_CMD_SD_APP_OP_COND ||
       Command->CmdIndex == SDMMC_CMD_SDMMC_SEN_OP_COND)
    {
        Command->CmdIndexCheckEn = SDIO_DISABLE;
        __SDIO_IT_STATE_DISABLE(SDIOx, SDIO_IT_STATE_ECI);
    }
    else
    {
        Command->CmdIndexCheckEn = SDIO_ENABLE;
    }
    if(Command->CmdIndex == SDMMC_CMD_SEND_OP_COND ||
       Command->CmdIndex == SDMMC_CMD_SD_APP_OP_COND ||
       Command->CmdIndex == SDMMC_CMD_SDMMC_SEN_OP_COND)
    {
        Command->CmdCRCCheckEn = SDIO_DISABLE;
        __SDIO_IT_STATE_DISABLE(SDIOx, SDIO_IT_STATE_ECCRC);
    }

#if(!(defined USE_AUTO_CMD12 && USE_AUTO_CMD12))
    Command->AutoCmdSelect = SDIO_Auto_CMD_No;
#endif
    /* Set SDMMC command parameters */
    tmpreg |= (uint32_t)((Command->CmdIndex       << SD_TMR_CIR_CI_Pos)      |\
                         (Command->CmdType        << SD_TMR_CIR_CT_Pos)      |\
                         (Command->CmdIndexCheckEn<< SD_TMR_CIR_CICE_Pos)    |\
                         (Command->CmdCRCCheckEn  << SD_TMR_CIR_CRCCE_Pos)   |\
                         (Command->DataEn         << SD_TMR_CIR_DPS_Pos)     |\
                         (Command->TransferDir    << SD_TMR_CIR_DTDS_Pos)    |\
                         (Command->DMAEn          << SD_TMR_CIR_DMAE_Pos)    |\
                         (Command->Response       << SD_TMR_CIR_RTS_Pos)     |\
                         (Command->DataBlockSelect<< SD_TMR_CIR_MSBS_Pos)    |\
                         (Command->AutoCmdSelect  << SD_TMR_CIR_ACE_Pos)     |\
                         (Command->BlockCountEn   << SD_TMR_CIR_BCE_Pos));
    if(Command->DataEn)
    {
        while((SDIOx->Slot->PSR & SD_PSR_CIDAT) != RESET);
    }
    while((SDIOx->Slot->PSR & SD_PSR_CICMD) != RESET);
    /*Then it writes SRS3 with an appropriate command configuration.*/
    /* Write to SDMMC CMD register */
    SDIOx->Slot->CIR = tmpreg;
    return HAL_OK;
}
void SDIO_GetResponse136(SDIO_TypeDef *SDIOx, uint32_t csd[])
{
    uint32_t tempreg0 = 0;
    uint32_t tempreg1 = 0;
    /* Get Card Specific Data */
    tempreg0 = SDIO_GetResponse(SDIOx, SDIO_RESP0);
    csd[3] = (tempreg0 << 8);

    tempreg1 = SDIO_GetResponse(SDIOx, SDIO_RESP1);
    csd[2] = ((tempreg1 << 8) | (tempreg0 >> 24));

    tempreg0 = SDIO_GetResponse(SDIOx, SDIO_RESP2);
    csd[1] = ((tempreg0 << 8) | (tempreg1 >> 24));

    tempreg1 = SDIO_GetResponse(SDIOx, SDIO_RESP3);
    csd[0] = ((tempreg1 << 8) | (tempreg0 >> 24));
}

/**
  * @brief  Configure the SDMMC data path according to the specified 
  *         parameters in the SDIO_DataInitTypeDef.
  * @param  SDIOx Pointer to SDMMC register base  
  * @param  Data  pointer to a SDIO_DataInitTypeDef structure 
  *         that contains the configuration information for the SDMMC data.
  * @retval HAL status
  */
HAL_StatusTypeDef SDIO_ConfigData(SDIO_TypeDef *SDIOx, SDIO_DataInitTypeDef* Data)
{
    /* Check the parameters */
    assert_param(IS_SDIO_DATA_LENGTH(Data->DataLength));
    assert_param(IS_SDIO_BLOCK_SIZE(Data->DataBlockSize));

    /* Set the SDMMC Data TimeOut value */
//    MODIFY_REG(SDIOx->Slot->HCSR1, SD_HCSR1_DTCV_Msk, Data->DataTimeOut << SD_HCSR1_DTCV_Pos);
    /* Set the SDMMC DataLength value */
    /* Set the SDMMC Transfer Block Size */
    MODIFY_REG(SDIOx->Slot->BLOCK_CSR, SD_BLOCK_CSR_BCCT_Msk | SD_BLOCK_CSR_TBS_Msk, \
            ((Data->DataLength / Data->DataBlockSize) << SD_BLOCK_CSR_BCCT_Pos) | \
            (Data->DataBlockSize << SD_BLOCK_CSR_TBS_Pos));
    return HAL_OK;
}

/**
  * @brief  Returns number of remaining data bytes to be transferred.
  * @param  SDIOx Pointer to SDMMC register base
  * @retval Count of remaining Blocks to be transferred, During the data transfer,
  *         read operations on this register may return an invalid value
  */
uint32_t SDIO_GetDataCounter(SDIO_TypeDef *SDIOx)
{
    return (SDIOx->Slot->BLOCK_CSR >> SD_BLOCK_CSR_BCCT_Pos);
}

/**
  * @brief  Sets one of the two options of inserting read wait interval.
  * @param  SDIOx Pointer to SDMMC register base   
  * @param  SDIO_ReadWaitMode SDMMC Read Wait operation mode.
  *          This parameter can be:
  *            @arg SDIO_READ_WAIT_MODE_CLK: Read Wait control by stopping SDMMCCLK
  *            @arg SDIO_READ_WAIT_MODE_DATA2: Read Wait control using SDMMC_DATA2
  * @retval None
  */
HAL_StatusTypeDef SDIO_SetSDMMCReadWaitMode(SDIO_TypeDef *SDIOx, uint32_t SDIO_ReadWaitMode)
{
    UNUSED(SDIOx);
    UNUSED(SDIO_ReadWaitMode);
    return HAL_OK;
}

/**
  * @}
  */


/** @defgroup HAL_SDMMC_LL_Group4 Command management functions 
 *  @brief   Data transfers functions 
 *
@verbatim   
 ===============================================================================
                   ##### Commands management functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to manage the needed commands.

@endverbatim
  * @{
  */

/**
  * @brief  Send the Data Block Count command and check the response
  * @param  SDIOx Pointer to SDMMC register base
  * @retval HAL status
  */
uint32_t SDMMC_CmdBlockCount(SDIO_TypeDef *SDIOx, uint32_t BlockCount)
{
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument          = (uint32_t)BlockCount;
    sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_SET_BLOCK_COUNT;
    sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
    sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
    sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
    sdmmc_cmdinit.DataEn            = 0;
    sdmmc_cmdinit.TransferDir       = 0;
    sdmmc_cmdinit.DMAEn             = 0;
    sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
    sdmmc_cmdinit.DataBlockSelect   = 0;
    sdmmc_cmdinit.AutoCmdSelect     = 0;
    sdmmc_cmdinit.BlockCountEn      = 0;
    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SET_BLOCK_COUNT, SDIO_CMDTIMEOUT);

    return errorstate;
}
/**
  * @brief  Send the Data Block Lenght command and check the response
  * @param  SDIOx Pointer to SDMMC register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdBlockLength(SDIO_TypeDef *SDIOx, uint32_t BlockSize)
{
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument          = (uint32_t)BlockSize;
    sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_SET_BLOCKLEN;
    sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
    sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
    sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
    sdmmc_cmdinit.DataEn            = 0;
    sdmmc_cmdinit.TransferDir       = 0;
    sdmmc_cmdinit.DMAEn             = 0;
    sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
    sdmmc_cmdinit.DataBlockSelect   = 0;
    sdmmc_cmdinit.AutoCmdSelect     = 0;
    sdmmc_cmdinit.BlockCountEn      = 0;
    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SET_BLOCKLEN, SDIO_CMDTIMEOUT);

    return errorstate;
}

/**
  * @brief  Send the Read Single Block command and check the response
  * @param  SDIOx Pointer to SDMMC register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdReadSingleBlock(SDIO_TypeDef *SDIOx, uint32_t ReadAdd)
{
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument          = (uint32_t)ReadAdd;
    sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_READ_SINGLE_BLOCK;
    sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
    sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
    sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
    sdmmc_cmdinit.DataEn            = SDIO_ENABLE;
    sdmmc_cmdinit.TransferDir       = SDIO_Data_TFR_Dir_Read;
    sdmmc_cmdinit.DMAEn             = 0;
    sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
    sdmmc_cmdinit.DataBlockSelect   = SDIO_Block_Single;
    sdmmc_cmdinit.AutoCmdSelect     = SDIO_Auto_CMD_No;
    sdmmc_cmdinit.BlockCountEn      = SDIO_ENABLE;
    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_READ_SINGLE_BLOCK, SDIO_CMDTIMEOUT);

    return errorstate;
}

/**
  * @brief  Send the Read Single Block command and check the response
  * @param  SDIOx Pointer to SDMMC register base
  * @retval HAL status
  */
uint32_t SDMMC_CmdReadSingleBlockDMA(SDIO_TypeDef *SDIOx, uint32_t ReadAdd)
{
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument          = (uint32_t)ReadAdd;
    sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_READ_SINGLE_BLOCK;
    sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
    sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
    sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
    sdmmc_cmdinit.DataEn            = SDIO_ENABLE;
    sdmmc_cmdinit.TransferDir       = SDIO_Data_TFR_Dir_Read;
    sdmmc_cmdinit.DMAEn             = SDIO_ENABLE;
    sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
    sdmmc_cmdinit.DataBlockSelect   = SDIO_Block_Single;
    sdmmc_cmdinit.AutoCmdSelect     = SDIO_Auto_CMD_No;
    sdmmc_cmdinit.BlockCountEn      = SDIO_ENABLE;
    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_READ_SINGLE_BLOCK, SDIO_CMDTIMEOUT);

    return errorstate;
}
/**
  * @brief  Send the Read Multi Block command and check the response
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdReadMultiBlock(SDIO_TypeDef *SDIOx, uint32_t ReadAdd)
{
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument          = (uint32_t)ReadAdd;
    sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_READ_MULT_BLOCK;
    sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
    sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
    sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
    sdmmc_cmdinit.DataEn            = SDIO_ENABLE;
    sdmmc_cmdinit.TransferDir       = SDIO_Data_TFR_Dir_Read;
    sdmmc_cmdinit.DMAEn             = SDIO_DISABLE;
    sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
    sdmmc_cmdinit.DataBlockSelect   = SDIO_Block_Multi;
    sdmmc_cmdinit.AutoCmdSelect     = SDIO_Auto_CMD12;
    sdmmc_cmdinit.BlockCountEn      = SDIO_ENABLE;

    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_READ_MULT_BLOCK, SDIO_CMDTIMEOUT);
    return errorstate;
}
/**
  * @brief  Send the Read Multi Block command and check the response
  * @param  SDIOx Pointer to SDIO register base
  * @retval HAL status
  */
uint32_t SDMMC_CmdReadMultiBlockDMA(SDIO_TypeDef *SDIOx, uint32_t ReadAdd)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;

  /* Set Block Size for Card */
  sdmmc_cmdinit.Argument          = (uint32_t)ReadAdd;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_READ_MULT_BLOCK;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = SDIO_ENABLE;
  sdmmc_cmdinit.TransferDir       = SDIO_Data_TFR_Dir_Read;
  sdmmc_cmdinit.DMAEn             = SDIO_ENABLE;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = SDIO_Block_Multi;
  sdmmc_cmdinit.AutoCmdSelect     = SDIO_Auto_CMD12;
  sdmmc_cmdinit.BlockCountEn      = SDIO_ENABLE;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_READ_MULT_BLOCK, SDIO_CMDTIMEOUT);

  return errorstate;
}

/**
  * @brief  Send the Write Single Block command and check the response
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdWriteSingleBlock(SDIO_TypeDef *SDIOx, uint32_t WriteAdd)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  /* Set Block Size for Card */ 
  sdmmc_cmdinit.Argument          = (uint32_t)WriteAdd;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_WRITE_SINGLE_BLOCK;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = SDIO_ENABLE;
  sdmmc_cmdinit.TransferDir       = SDIO_Data_TFR_Dir_Write;
  sdmmc_cmdinit.DMAEn             = SDIO_DISABLE;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = SDIO_Block_Single;
  sdmmc_cmdinit.AutoCmdSelect     = SDIO_Auto_CMD_No;
  sdmmc_cmdinit.BlockCountEn      = SDIO_ENABLE;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_WRITE_SINGLE_BLOCK, SDIO_CMDTIMEOUT);

  return errorstate;
}

/**
  * @brief  Send the Write Single Block command and check the response
  * @param  SDIOx Pointer to SDIO register base
  * @retval HAL status
  */
uint32_t SDMMC_CmdWriteSingleBlockDMA(SDIO_TypeDef *SDIOx, uint32_t WriteAdd)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;

  /* Set Block Size for Card */
  sdmmc_cmdinit.Argument          = (uint32_t)WriteAdd;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_WRITE_SINGLE_BLOCK;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = SDIO_ENABLE;
  sdmmc_cmdinit.TransferDir       = SDIO_Data_TFR_Dir_Write;
  sdmmc_cmdinit.DMAEn             = SDIO_ENABLE;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = SDIO_Block_Single;
  sdmmc_cmdinit.AutoCmdSelect     = SDIO_Auto_CMD_No;
  sdmmc_cmdinit.BlockCountEn      = SDIO_ENABLE;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_WRITE_SINGLE_BLOCK, SDIO_CMDTIMEOUT);

  return errorstate;
}

/**
  * @brief  Send the Write Multi Block command and check the response
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdWriteMultiBlock(SDIO_TypeDef *SDIOx, uint32_t WriteAdd)
{
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument          = (uint32_t)WriteAdd;
    sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_WRITE_MULT_BLOCK;
    sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
    sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
    sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
    sdmmc_cmdinit.DataEn            = SDIO_ENABLE;
    sdmmc_cmdinit.TransferDir       = SDIO_Data_TFR_Dir_Write;
    sdmmc_cmdinit.DMAEn             = SDIO_DISABLE;
    sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
    sdmmc_cmdinit.DataBlockSelect   = SDIO_Block_Multi;
    sdmmc_cmdinit.AutoCmdSelect     = SDIO_Auto_CMD12;
    sdmmc_cmdinit.BlockCountEn      = SDIO_ENABLE;
    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_WRITE_MULT_BLOCK, SDIO_CMDTIMEOUT);

    return errorstate;
}
/**
  * @brief  Send the Write Multi Block command and check the response
  * @param  SDIOx Pointer to SDIO register base
  * @retval HAL status
  */
uint32_t SDMMC_CmdWriteMultiBlockDMA(SDIO_TypeDef *SDIOx, uint32_t WriteAdd)
{
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    /* Set Block Size for Card */
    sdmmc_cmdinit.Argument          = (uint32_t)WriteAdd;
    sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_WRITE_MULT_BLOCK;
    sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
    sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
    sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
    sdmmc_cmdinit.DataEn            = SDIO_ENABLE;
    sdmmc_cmdinit.TransferDir       = SDIO_Data_TFR_Dir_Write;
    sdmmc_cmdinit.DMAEn             = SDIO_ENABLE;
    sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
    sdmmc_cmdinit.DataBlockSelect   = SDIO_Block_Multi;
    sdmmc_cmdinit.AutoCmdSelect     = SDIO_Auto_CMD12;
    sdmmc_cmdinit.BlockCountEn      = SDIO_ENABLE;
    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_WRITE_MULT_BLOCK, SDIO_CMDTIMEOUT);

    return errorstate;
}

/**
  * @brief  Send the Start Address Erase command for SD and check the response
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdSDEraseStartAdd(SDIO_TypeDef *SDIOx, uint32_t StartAdd)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  /* Set Block Size for Card */ 
  sdmmc_cmdinit.Argument          = (uint32_t)StartAdd;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_SD_ERASE_GRP_START;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SD_ERASE_GRP_START, SDIO_CMDTIMEOUT);

  return errorstate;
}

/**
  * @brief  Send the End Address Erase command for SD and check the response
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdSDEraseEndAdd(SDIO_TypeDef *SDIOx, uint32_t EndAdd)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  /* Set Block Size for Card */ 
  sdmmc_cmdinit.Argument          = (uint32_t)EndAdd;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_SD_ERASE_GRP_END;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SD_ERASE_GRP_END, SDIO_CMDTIMEOUT);

  return errorstate;
}

/**
  * @brief  Send the Start Address Erase command and check the response
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdEraseStartAdd(SDIO_TypeDef *SDIOx, uint32_t StartAdd)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  /* Set Block Size for Card */ 
  sdmmc_cmdinit.Argument          = (uint32_t)StartAdd;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_ERASE_GRP_START;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_ERASE_GRP_START, SDIO_CMDTIMEOUT);

  return errorstate;
}

/**
  * @brief  Send the End Address Erase command and check the response
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdEraseEndAdd(SDIO_TypeDef *SDIOx, uint32_t EndAdd)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  /* Set Block Size for Card */ 
  sdmmc_cmdinit.Argument          = (uint32_t)EndAdd;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_ERASE_GRP_END;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_ERASE_GRP_END, SDIO_CMDTIMEOUT);

  return errorstate;
}

/**
  * @brief  Send the Erase command and check the response
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdErase(SDIO_TypeDef *SDIOx)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  /* Set Block Size for Card */ 
  sdmmc_cmdinit.Argument          = 0U;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_ERASE;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_ERASE, SDIO_MAXERASETIMEOUT);

  return errorstate;
}

/**
  * @brief  Send the Stop Transfer command and check the response.
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdStopTransfer(SDIO_TypeDef *SDIOx)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  /* Send CMD12 STOP_TRANSMISSION  */
  sdmmc_cmdinit.Argument          = 0U;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_STOP_TRANSMISSION;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Abort;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48_Busy;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_STOP_TRANSMISSION, 100000000U);

  return errorstate;
}

/**
  * @brief  Send the Select Deselect command and check the response.
  * @param  SDIOx Pointer to SDIO register base 
  * @param  addr Address of the card to be selected  
  * @retval HAL status
  */
uint32_t SDMMC_CmdSelDesel(SDIO_TypeDef *SDIOx, uint64_t Addr)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  /* Send CMD7 SDMMC_SEL_DESEL_CARD */
  sdmmc_cmdinit.Argument          = (uint32_t)Addr;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_SEL_DESEL_CARD;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48_Busy;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SEL_DESEL_CARD, SDIO_CMDTIMEOUT);

  return errorstate;
}

/**
  * @brief  Send the Go Idle State command and check the response.
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdGoIdleState(SDIO_TypeDef *SDIOx)
{
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    sdmmc_cmdinit.Argument          = 0U;
    sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_GO_IDLE_STATE;
    sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
    sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
    sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
    sdmmc_cmdinit.DataEn            = 0;
    sdmmc_cmdinit.TransferDir       = 0;
    sdmmc_cmdinit.DMAEn             = 0;
    sdmmc_cmdinit.Response          = SDIO_Response_No;
    sdmmc_cmdinit.DataBlockSelect   = 0;
    sdmmc_cmdinit.AutoCmdSelect     = 0;
    sdmmc_cmdinit.BlockCountEn      = 0;
    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdError(SDIOx);

    return errorstate;
}

/**
  * @brief  Send the Operating Condition command and check the response.
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdOperCond(SDIO_TypeDef *SDIOx)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  /* Send CMD8 to verify SD card interface operating condition */
  /* Argument: - [31:12]: Reserved (shall be set to '0')
  - [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
  - [7:0]: Check Pattern (recommended 0xAA) */
  /* CMD Response: R7 */
  sdmmc_cmdinit.Argument          = SDMMC_CHECK_PATTERN;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_HS_SEND_EXT_CSD;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp7(SDIOx);

  return errorstate;
}

/**
  * @brief  Send the Application command to verify that that the next command 
  *         is an application specific com-mand rather than a standard command
  *         and check the response.
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdAppCommand(SDIO_TypeDef *SDIOx, uint32_t Argument)
{
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    sdmmc_cmdinit.Argument          = (uint32_t)Argument;
    sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_APP_CMD;
    sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
    sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
    sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
    sdmmc_cmdinit.DataEn            = 0;
    sdmmc_cmdinit.TransferDir       = 0;
    sdmmc_cmdinit.DMAEn             = 0;
    sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
    sdmmc_cmdinit.DataBlockSelect   = 0;
    sdmmc_cmdinit.AutoCmdSelect     = 0;
    sdmmc_cmdinit.BlockCountEn      = 0;
    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

    /* Check for error conditions */
    /* If there is a HAL_ERROR, it is a MMC card, else
    it is a SD card: SD card 2.0 (voltage range mismatch)
     or SD card 1.x */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_APP_CMD, SDIO_CMDTIMEOUT);
    return errorstate;
}

/**
  * @brief  Send the command asking the accessed card to send its operating 
  *         condition register (OCR)
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdAppOperCommand(SDIO_TypeDef *SDIOx, uint32_t SdType)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  sdmmc_cmdinit.Argument          = SDMMC_VOLTAGE_WINDOW_SD | SdType;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_SD_APP_OP_COND;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp3(SDIOx);

  return errorstate;
}

/**
  * @brief  Send the Bus Width command and check the response.
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdBusWidth(SDIO_TypeDef *SDIOx, uint32_t BusWidth)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  sdmmc_cmdinit.Argument         = (uint32_t)BusWidth;
  sdmmc_cmdinit.CmdIndex         = SDMMC_CMD_APP_SD_SET_BUSWIDTH;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_APP_SD_SET_BUSWIDTH, SDIO_CMDTIMEOUT);

  return errorstate;
}

/**
  * @brief  Send the Send SCR command and check the response.
  * @param  SDIOx Pointer to SDMMC register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdSendSCR(SDIO_TypeDef *SDIOx)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  /* Send CMD51 SD_APP_SEND_SCR */
  sdmmc_cmdinit.Argument         = 0U;
  sdmmc_cmdinit.CmdIndex         = SDMMC_CMD_SD_APP_SEND_SCR;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = SDIO_ENABLE;
  sdmmc_cmdinit.TransferDir       = SDIO_Data_TFR_Dir_Read;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = SDIO_Block_Single;
  sdmmc_cmdinit.AutoCmdSelect     = SDIO_DISABLE;
  sdmmc_cmdinit.BlockCountEn      = SDIO_ENABLE;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SD_APP_SEND_SCR, SDIO_CMDTIMEOUT);

  return errorstate;
}

/**
  * @brief  Send the Send CID command and check the response.
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdSendCID(SDIO_TypeDef *SDIOx)
{
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    /* Send CMD2 ALL_SEND_CID */
    sdmmc_cmdinit.Argument          = 0U;
    sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_ALL_SEND_CID;
    sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
    sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
    sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
    sdmmc_cmdinit.DataEn            = 0;
    sdmmc_cmdinit.TransferDir       = 0;
    sdmmc_cmdinit.DMAEn             = 0;
    sdmmc_cmdinit.Response          = SDIO_Response_Bit136;
    sdmmc_cmdinit.DataBlockSelect   = 0;
    sdmmc_cmdinit.AutoCmdSelect     = 0;
    sdmmc_cmdinit.BlockCountEn      = 0;
    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp2(SDIOx);

    return errorstate;
}

/**
  * @brief  Send the Send CSD command and check the response.
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdSendCSD(SDIO_TypeDef *SDIOx, uint32_t Argument)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  /* Send CMD9 SEND_CSD */
  sdmmc_cmdinit.Argument          = (uint32_t)Argument;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_SEND_CSD;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit136;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp2(SDIOx);

  return errorstate;
}

/**
  * @brief  Send the Send CSD command and check the response.
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdSetRelAdd(SDIO_TypeDef *SDIOx, uint16_t *pRCA)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  /* Send CMD3 SD_CMD_SET_REL_ADDR */
  sdmmc_cmdinit.Argument          = 0U;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_SET_REL_ADDR;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp6(SDIOx, SDMMC_CMD_SET_REL_ADDR, pRCA);

  return errorstate;
}

/**
  * @brief  Send the Status command and check the response.
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdSendStatus(SDIO_TypeDef *SDIOx, uint32_t Argument)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  sdmmc_cmdinit.Argument          = (uint32_t)Argument;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_SEND_STATUS;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SEND_STATUS, SDIO_CMDTIMEOUT);

  return errorstate;
}

/**
  * @brief  Send the Status register command and check the response.
  * @param  SDIOx Pointer to SDIO register base 
  * @retval HAL status
  */
uint32_t SDMMC_CmdStatusRegister(SDIO_TypeDef *SDIOx)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  sdmmc_cmdinit.Argument          = 0U;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_SD_APP_STATUS;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = SDIO_ENABLE;
  sdmmc_cmdinit.TransferDir       = SDIO_Data_TFR_Dir_Read;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = SDIO_Block_Single;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = SDIO_DISABLE;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_SD_APP_STATUS, SDIO_CMDTIMEOUT);

  return errorstate;
}

/**
  * @brief  Sends host capacity support information and activates the card's 
  *         initialization process. Send SDMMC_CMD_SEND_OP_COND command
  * @param  SDIOx Pointer to SDIO register base 
  * @parame Argument Argument used for the command
  * @retval HAL status
  */
uint32_t SDMMC_CmdOpCondition(SDIO_TypeDef *SDIOx, uint32_t Argument)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;
  
  sdmmc_cmdinit.Argument          = Argument;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_SEND_OP_COND;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);
  
  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp3(SDIOx);

  return errorstate;
}

/**
  * @brief  Checks switchable function and switch card function. SDMMC_CMD_HS_SWITCH comand
  * @param  SDIOx Pointer to SDIO register base 
  * @parame Argument Argument used for the command
  * @retval HAL status
  */
uint32_t SDMMC_CmdSwitch(SDIO_TypeDef *SDIOx, uint32_t Argument)
{
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    sdmmc_cmdinit.Argument          = Argument;
    sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_HS_SWITCH;
    sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
    sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
    sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
    sdmmc_cmdinit.DataEn            = 0;
    sdmmc_cmdinit.TransferDir       = 0;
    sdmmc_cmdinit.DMAEn             = 0;
    sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
    sdmmc_cmdinit.DataBlockSelect   = 0;
    sdmmc_cmdinit.AutoCmdSelect     = 0;
    sdmmc_cmdinit.BlockCountEn      = 0;
    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

    /* Check for error conditions */
    errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_HS_SWITCH, SDIO_CMDTIMEOUT);

    return errorstate;
}
//#define ADD_FUNC
#ifdef ADD_FUNC

/**
  * @brief  Send the Operating Condition command and check the response.
  * @param  SDIOx Pointer to SDIO register base
  * @retval HAL status
  */
uint32_t SDMMC_CmdOperCond(SDIO_TypeDef *SDIOx)
{
  SDIO_CmdInitTypeDef  sdmmc_cmdinit;
  uint32_t errorstate = SDMMC_ERROR_NONE;

  /* Send CMD8 to verify SD card interface operating condition */
  /* Argument: - [31:12]: Reserved (shall be set to '0')
  - [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
  - [7:0]: Check Pattern (recommended 0xAA) */
  /* CMD Response: R7 */
  sdmmc_cmdinit.Argument          = SDMMC_CHECK_PATTERN;
  sdmmc_cmdinit.CmdIndex          = SDMMC_CMD_HS_SEND_EXT_CSD;
  sdmmc_cmdinit.CmdType           = SDIO_CMD_Type_Normal;
  sdmmc_cmdinit.CmdIndexCheckEn   = SDIO_ENABLE;
  sdmmc_cmdinit.CmdCRCCheckEn     = SDIO_ENABLE;
  sdmmc_cmdinit.DataEn            = 0;
  sdmmc_cmdinit.TransferDir       = 0;
  sdmmc_cmdinit.DMAEn             = 0;
  sdmmc_cmdinit.Response          = SDIO_Response_Bit48;
  sdmmc_cmdinit.DataBlockSelect   = 0;
  sdmmc_cmdinit.AutoCmdSelect     = 0;
  sdmmc_cmdinit.BlockCountEn      = 0;
  SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

  /* Check for error conditions */
  errorstate = SDMMC_GetCmdResp7(SDIOx);

  return errorstate;
}
#endif

/**
  * @}
  */

/* Private function ----------------------------------------------------------*/  
/** @addtogroup SD_Private_Functions
  * @{
  */
static uint32_t SDMMC_WaitCmdEnd(SDIO_TypeDef *SDIOx, uint32_t timeout)
{
    register uint32_t count = timeout;
    if(__SDIO_GET_IT_STATE_EN(SDIOx, SDIO_IT_CC) && !__SDIO_GET_IT_EN(SDIOx, SDIO_IT_CC))
    {
        do
        {
            if (count-- == 0U)
            {
                return SDMMC_ERROR_TIMEOUT;
            }
        }while(!__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECI | SDIO_FLAG_ECEB | SDIO_FLAG_ECCRC | SDIO_FLAG_ECT | SDIO_FLAG_CC));
    }
    else
    {
        while(__SDIO_GET_STATE(SDIOx, SDIO_STATE_CICMD))
        {
            if (count-- == 0U)
            {
                return SDMMC_ERROR_TIMEOUT;
            }
        }
    }
    return SDMMC_ERROR_NONE;
}
/**
  * @brief  Checks for error conditions for CMD0.
  * @param  hsd SD handle
  * @retval SD Card error state
  */
static uint32_t SDMMC_GetCmdError(SDIO_TypeDef *SDIOx)
{
    /* 8 is the number of required instructions cycles for the below loop statement.
        The SDMMC_CMDTIMEOUT is expressed in ms */
    uint32_t count = SDIO_CMDTIMEOUT * (GetSystemCoreClock() / 8U /1000U);
    uint32_t ret = SDMMC_WaitCmdEnd(SDIOx, count);
    if(ret != SDMMC_ERROR_NONE)
    {
        return ret;
    }
    /* Clear all the static flags */
    __SDIO_CLEAR_FLAG(SDIOx, SDIO_STATIC_FLAGS);

    return SDMMC_ERROR_NONE;
}

/**
  * @brief  Checks for error conditions for R1 response.
  * @param  hsd SD handle
  * @param  SD_CMD The sent command index  
  * @retval SD Card error state
  */
static uint32_t SDMMC_GetCmdResp1(SDIO_TypeDef *SDIOx, uint8_t SD_CMD, uint32_t Timeout)
{
    uint32_t response_r0;

    /* 8 is the number of required instructions cycles for the below loop statement.
        The Timeout is expressed in ms */
    uint32_t count = Timeout * (GetSystemCoreClock() / 8U /1000U);
    uint32_t ret = SDMMC_WaitCmdEnd(SDIOx, count);
    if(ret != SDMMC_ERROR_NONE)
    {
        return ret;
    }
    /* Check response received is of desired command */
    if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_EINT))
    {
        if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECT))
        {
            __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECT);
            return SDMMC_ERROR_HW_CMD_TIMEOUT;
        }
        else if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECCRC))
        {
            __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECCRC);
            return SDMMC_ERROR_HW_CMD_CRC;
        }
        else if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECEB))
        {
            __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECEB);
            return SDMMC_ERROR_HW_DATA_END_BIT;
        }
        else if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECI))
        {
            __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECI);
            return SDMMC_ERROR_HW_CMD_Index;
        }
    }
    /* Clear all the static flags */
    __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_CC);
    /* We have received response, retrieve it for analysis  */
    response_r0 = SDIO_GetResponse(SDIOx, SDIO_RESP0);
  
    if((response_r0 & SDMMC_OCR_ERRORBITS) == SDMMC_ALLZERO)
    {
        return SDMMC_ERROR_NONE;
    }
    else if((response_r0 & SDMMC_OCR_ADDR_OUT_OF_RANGE) == SDMMC_OCR_ADDR_OUT_OF_RANGE)
    {
        return SDMMC_ERROR_ADDR_OUT_OF_RANGE;
    }
    else if((response_r0 & SDMMC_OCR_ADDR_MISALIGNED) == SDMMC_OCR_ADDR_MISALIGNED)
    {
        return SDMMC_ERROR_ADDR_MISALIGNED;
    }
    else if((response_r0 & SDMMC_OCR_BLOCK_LEN_ERR) == SDMMC_OCR_BLOCK_LEN_ERR)
    {
        return SDMMC_ERROR_BLOCK_LEN_ERR;
    }
    else if((response_r0 & SDMMC_OCR_ERASE_SEQ_ERR) == SDMMC_OCR_ERASE_SEQ_ERR)
    {
        return SDMMC_ERROR_ERASE_SEQ_ERR;
    }
    else if((response_r0 & SDMMC_OCR_BAD_ERASE_PARAM) == SDMMC_OCR_BAD_ERASE_PARAM)
    {
        return SDMMC_ERROR_BAD_ERASE_PARAM;
    }
    else if((response_r0 & SDMMC_OCR_WRITE_PROT_VIOLATION) == SDMMC_OCR_WRITE_PROT_VIOLATION)
    {
        return SDMMC_ERROR_WRITE_PROT_VIOLATION;
    }
    else if((response_r0 & SDMMC_OCR_LOCK_UNLOCK_FAILED) == SDMMC_OCR_LOCK_UNLOCK_FAILED)
    {
        return SDMMC_ERROR_LOCK_UNLOCK_FAILED;
    }
    else if((response_r0 & SDMMC_OCR_COM_CRC_FAILED) == SDMMC_OCR_COM_CRC_FAILED)
    {
        return SDMMC_ERROR_COM_CRC_FAILED;
    }
    else if((response_r0 & SDMMC_OCR_ILLEGAL_CMD) == SDMMC_OCR_ILLEGAL_CMD)
    {
        return SDMMC_ERROR_ILLEGAL_CMD;
    }
    else if((response_r0 & SDMMC_OCR_CARD_ECC_FAILED) == SDMMC_OCR_CARD_ECC_FAILED)
    {
        return SDMMC_ERROR_CARD_ECC_FAILED;
    }
    else if((response_r0 & SDMMC_OCR_CC_ERROR) == SDMMC_OCR_CC_ERROR)
    {
        return SDMMC_ERROR_CC_ERR;
    }
    else if((response_r0 & SDMMC_OCR_STREAM_READ_UNDERRUN) == SDMMC_OCR_STREAM_READ_UNDERRUN)
    {
        return SDMMC_ERROR_STREAM_READ_UNDERRUN;
    }
    else if((response_r0 & SDMMC_OCR_STREAM_WRITE_OVERRUN) == SDMMC_OCR_STREAM_WRITE_OVERRUN)
    {
        return SDMMC_ERROR_STREAM_WRITE_OVERRUN;
    }
    else if((response_r0 & SDMMC_OCR_CID_CSD_OVERWRITE) == SDMMC_OCR_CID_CSD_OVERWRITE)
    {
        return SDMMC_ERROR_CID_CSD_OVERWRITE;
    }
    else if((response_r0 & SDMMC_OCR_WP_ERASE_SKIP) == SDMMC_OCR_WP_ERASE_SKIP)
    {
        return SDMMC_ERROR_WP_ERASE_SKIP;
    }
    else if((response_r0 & SDMMC_OCR_CARD_ECC_DISABLED) == SDMMC_OCR_CARD_ECC_DISABLED)
    {
        return SDMMC_ERROR_CARD_ECC_DISABLED;
    }
    else if((response_r0 & SDMMC_OCR_ERASE_RESET) == SDMMC_OCR_ERASE_RESET)
    {
        return SDMMC_ERROR_ERASE_RESET;
    }
    else if((response_r0 & SDMMC_OCR_AKE_SEQ_ERROR) == SDMMC_OCR_AKE_SEQ_ERROR)
    {
        return SDMMC_ERROR_AKE_SEQ_ERR;
    }
    else
    {
        return SDMMC_ERROR_GENERAL_UNKNOWN_ERR;
    }
}

/**
  * @brief  Checks for error conditions for R2 (CID or CSD) response.
  * @param  hsd SD handle
  * @retval SD Card error state
  */
static uint32_t SDMMC_GetCmdResp2(SDIO_TypeDef *SDIOx)
{
    /* 8 is the number of required instructions cycles for the below loop statement.
        The SDMMC_CMDTIMEOUT is expressed in ms */
    uint32_t count = SDIO_CMDTIMEOUT * (GetSystemCoreClock() / 8U /1000U);
    uint32_t ret = SDMMC_WaitCmdEnd(SDIOx, count);
    if(ret != SDMMC_ERROR_NONE)
    {
        return ret;
    }
    if (__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECT))
    {
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECT);
        return SDMMC_ERROR_HW_CMD_TIMEOUT;
    }
    else if (__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECCRC))
    {
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECCRC);
        return SDMMC_ERROR_HW_CMD_CRC;
    }
    else if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECEB))
    {
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECEB);
        return SDMMC_ERROR_HW_CMD_END_BIT;
    }
    else
    {
        /* No error flag set */
        /* Clear all the static flags */
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_STATIC_FLAGS);
    }
    
    return SDMMC_ERROR_NONE;
}

/**
  * @brief  Checks for error conditions for R3 (OCR) response.
  * @param  hsd SD handle
  * @retval SD Card error state
  */
static uint32_t SDMMC_GetCmdResp3(SDIO_TypeDef *SDIOx)
{
    /* 8 is the number of required instructions cycles for the below loop statement.
        The SDMMC_CMDTIMEOUT is expressed in ms */
    uint32_t count = SDIO_CMDTIMEOUT * (GetSystemCoreClock() / 8U /1000U);
    uint32_t ret = SDMMC_WaitCmdEnd(SDIOx, count);
    if(ret != SDMMC_ERROR_NONE)
    {
        return ret;
    }
    if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECT))
    {
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECT);
        return SDMMC_ERROR_HW_CMD_TIMEOUT;
    }
    else if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECEB))
    {
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECEB);
        return SDMMC_ERROR_HW_CMD_END_BIT;
    }
    else
    {
        /* Clear all the static flags */
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_STATIC_FLAGS);
    }
  
    return SDMMC_ERROR_NONE;
}

/**
  * @brief  Checks for error conditions for R6 (RCA) response.
  * @param  hsd SD handle
  * @param  SD_CMD The sent command index
  * @param  pRCA Pointer to the variable that will contain the SD card relative 
  *         address RCA   
  * @retval SD Card error state
  */
static uint32_t SDMMC_GetCmdResp6(SDIO_TypeDef *SDIOx, uint8_t SD_CMD, uint16_t *pRCA)
{
    uint32_t response_r0;

    /* 8 is the number of required instructions cycles for the below loop statement.
        The SDMMC_CMDTIMEOUT is expressed in ms */
    uint32_t count = SDIO_CMDTIMEOUT * (GetSystemCoreClock() / 8U /1000U);
    uint32_t ret = SDMMC_WaitCmdEnd(SDIOx, count);
    if(ret != SDMMC_ERROR_NONE)
    {
        return ret;
    }
    if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECT))
    {
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECT);
        return SDMMC_ERROR_HW_CMD_TIMEOUT;
    }
    else if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECCRC))
    {
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECCRC);
        return SDMMC_ERROR_HW_CMD_CRC;
    }
    else if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECEB))
    {
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECEB);
        return SDMMC_ERROR_HW_CMD_END_BIT;
    }
    else if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECI))
    {
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECI);
        return SDMMC_ERROR_HW_CMD_Index;
    }
    /* Clear all the static flags */
    __SDIO_CLEAR_FLAG(SDIOx, SDIO_STATIC_FLAGS);

    /* We have received response, retrieve it.  */
    response_r0 = SDIO_GetResponse(SDIOx, SDIO_RESP0);
  
    if((response_r0 & (SDMMC_R6_GENERAL_UNKNOWN_ERROR | SDMMC_R6_ILLEGAL_CMD | SDMMC_R6_COM_CRC_FAILED)) == SDMMC_ALLZERO)
    {
        *pRCA = (uint16_t) (response_r0 >> 16);
        return SDMMC_ERROR_NONE;
    }
    else if((response_r0 & SDMMC_R6_ILLEGAL_CMD) == SDMMC_R6_ILLEGAL_CMD)
    {
        return SDMMC_ERROR_ILLEGAL_CMD;
    }
    else if((response_r0 & SDMMC_R6_COM_CRC_FAILED) == SDMMC_R6_COM_CRC_FAILED)
    {
        return SDMMC_ERROR_COM_CRC_FAILED;
    }
    else
    {
        return SDMMC_ERROR_GENERAL_UNKNOWN_ERR;
    }
}

/**
  * @brief  Checks for error conditions for R7 response.
  * @param  hsd SD handle
  * @retval SD Card error state
  */
static uint32_t SDMMC_GetCmdResp7(SDIO_TypeDef *SDIOx)
{
    /* 8 is the number of required instructions cycles for the below loop statement.
        The SDIO_CMDTIMEOUT is expressed in ms */
    uint32_t count = SDIO_CMDTIMEOUT * (GetSystemCoreClock() / 8U /1000U);
    uint32_t ret = SDMMC_WaitCmdEnd(SDIOx, count);
    if(ret != SDMMC_ERROR_NONE)
    {
        return ret;
    }
    if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_ECT))
    {
        /* Card is SD V2.0 compliant */
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_ECT);
        return SDMMC_ERROR_CMD_RSP_TIMEOUT;
    }
    if(__SDIO_GET_FLAG(SDIOx, SDIO_FLAG_CC))
    {
        /* Card is SD V2.0 compliant */
        __SDIO_CLEAR_FLAG(SDIOx, SDIO_FLAG_CC);
    }
    return SDMMC_ERROR_NONE;
}

#endif /* HAL_SDMMC_MODULE_ENABLED */
