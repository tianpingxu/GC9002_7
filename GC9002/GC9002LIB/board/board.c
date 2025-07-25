#include <stdio.h>
#include <stdint.h>
#include "soc.h"
#include <csi_config.h>
#include <csi_core.h>
#include "gc90xx_hal_conf.h"
#include "csi_irq.h"
#include "board.h"

int __main(void)
{
    extern int main(void);
	return main();
}

/**
 * @brief Setup hardware board for rt-thread
 *
 */
#if(defined CONFIG_CONSOLE_DEBUG_UART_EN && CONFIG_CONSOLE_DEBUG_UART_EN == 1U)
static void hw_uart_init(void);
#endif
void hw_board_init(void)
{
    csi_common_mem_init();
#if(defined CONFIG_CONSOLE_DEBUG_UART_EN && CONFIG_CONSOLE_DEBUG_UART_EN == 1U)
	// debug huart_debug configuration
	hw_uart_init();
#endif
    #if(0)
    HAL_StatusTypeDef ret = HAL_PMU_VTR12_Calibrate(100);
    if(ret != HAL_OK)
    {
        printf("PMU VTR12 Calibrate fail.\n");
        assert_param(0);
    }
    else
    {
//        printf("PMU VTR12 Calibrate success.\n");
    }
	ret = HAL_PMU_VTR3318_Calibrate(100);
    if(ret != HAL_OK)
    {
        printf("PMU VTR3318 Calibrate fail.\n");
        assert_param(0);
    }
    else
    {
//        printf("PMU VTR3318 Calibrate success.\n");
    }
    #endif
}

#if(defined CONFIG_CONSOLE_DEBUG_UART_EN && CONFIG_CONSOLE_DEBUG_UART_EN == 1U)
UART_HandleTypeDef huart_debug;

static void hw_uart_init(void)
{
    /* config the UART */
    huart_debug.Instance = CONFIG_CONSOLE_DEBUG_UART;
    HAL_UART_StructInit(&(huart_debug.Init));
    HAL_UART_Init(&huart_debug);
}
#endif

/**
  * @brief  VIDEOCAP MSP Init.
  * @param  hvideocap  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified VIDEOCAP module.
  * @retval None
  */
void HAL_VIDEOCAP_MspInit(VIDEOCAP_HandleTypeDef *hvideocap)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    /**VIDEO GPIO Configuration
     * PB0      ------> VI_CK
     * PB1      ------> VI_HS
     * PB2      ------> VI_VS
     * PB3      ------> VI_D0
     * PB4      ------> VI_D1
     * PB5      ------> VI_D2
     * PB6      ------> VI_D3
     * PB7      ------> VI_D4
     * PB8      ------> VI_D5
     * PB9      ------> VI_D6
     * PB10     ------> VI_D7
     * */
    GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF6;
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | 
                          GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief  UART MSP Init.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	if(huart->Instance == UART0)
	{
        /**UART0 GPIO Configuration
         * PB2      ------> UART0_RX    AB23    U19 VSYNC
         * PB3      ------> UART0_TX    AE25    U19 VIDEO_D0
         * */
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF5;
		GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
    else if(huart->Instance == UART1)
    {
        /**UART0 GPIO Configuration
         * PB6      ------> UART1_RX    J21     EX2_IO0
         * PB7      ------> UART1_TX    M22     EX2_IO1
         * */
//		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF1;
//		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
//		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF4;
		GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
    else if(huart->Instance == UART2)
    {
        /**UART0 GPIO Configuration
         * PB8      ------> UART2_RX    K24     EX2_IO2
         * PB9      ------> UART2_TX    K25     EX2_IO3
         * */
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF1;
		GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}
/**
  * @brief  UART MSP DeInit.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	if(huart->Instance == UART0)
	{
        /**UART0 GPIO Configuration
         * PB2      ------> UART0_RX    AB23    U19 VSYNC
         * PB3      ------> UART0_TX    AE25    U19 VIDEO_D0
         * */
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
		GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
    else if(huart->Instance == UART1)
    {
        /**UART0 GPIO Configuration
         * PB6      ------> UART1_RX    J21     EX2_IO0
         * PB7      ------> UART1_TX    M22     EX2_IO1
         * */
//		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
//		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
//		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
		GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
    else if(huart->Instance == UART2)
    {
        /**UART0 GPIO Configuration
         * PB8      ------> UART2_RX    K24     EX2_IO2
         * PB9      ------> UART2_TX    K25     EX2_IO3
         * */
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
		GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}

void HAL_CANFD_MspInit(CANFD_HandleTypeDef *hcanfd)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    if(hcanfd->Instance == CANFD)
    {
        #if(0)
        /**CANFD GPIO Configuration
         * PA6      ------> CAN_TX
         * PA7      ------> CAN_RX
         * */
        GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF5;
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        #else
        /**CANFD GPIO Configuration
         * PB18     ------> CAN_TX
         * PB19     ------> CAN_RX
         * */
        GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF6;
        GPIO_InitStruct.Pin = GPIO_PIN_18 | GPIO_PIN_19;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        #endif
    }
    /* Peripheral clock enable */
    HAL_RCC_CAN_PresSet(hcanfd->Init.Prescaler, ENABLE);
}
void HAL_CANFD_MspDeInit(CANFD_HandleTypeDef *hcanfd)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    if(hcanfd->Instance == CANFD)
    {
        #if(0)
        /**CANFD GPIO Configuration
         * PA6      ------> CAN_TX
         * PA7      ------> CAN_RX
         * */
        GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        #else
        /**CANFD GPIO Configuration
         * PB18     ------> CAN_TX
         * PB19     ------> CAN_RX
         * */
        GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
        GPIO_InitStruct.Pin = GPIO_PIN_18 | GPIO_PIN_19;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        #endif
    }
    /* Peripheral clock enable */
    HAL_RCC_CAN_PresSet(hcanfd->Init.Prescaler, DISABLE);
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    if(hi2c->Instance == I2C0)
    {
        /**I2C GPIO Configuration
         * PB14      ------> SDA0   AD24    VIDEO_D4
         * PB15      ------> SCL0   AE24    VIDEO_D5
         * */
        GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF1;
        GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
    else if(hi2c->Instance == I2C1)
    {
        /**I2C GPIO Configuration
         * PA6       ------> SDA1
         * PA7       ------> SCL1
         * */
        GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF4;
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    /* Peripheral clock enable */
    __HAL_RCC_I2C_CLK_ENABLE();
}
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    if(hi2c->Instance == I2C0)
    {
        /**I2C GPIO Configuration
         * PB14      ------> SDA0   AD24    VIDEO_D4
         * PB15      ------> SCL0   AE24    VIDEO_D5
         * */
        GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
        GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
    else if(hi2c->Instance == I2C1)
    {
        /**I2C GPIO Configuration
         * PA6       ------> SDA1
         * PA7       ------> SCL1
         * */
        GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    /* Peripheral clock disable */
    __HAL_RCC_I2C_CLK_DISABLE();
}

void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s)
{
    uint32_t byte_cycles = 0;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* configuration of prescaler */
    if(hi2s->Init.DataFormat == I2S_WSS_CLOCK_CYCLES_16)
    {
        byte_cycles = 16;
    }
    else if(hi2s->Init.DataFormat == I2S_WSS_CLOCK_CYCLES_24)
    {
        byte_cycles = 24;
    }
    else if(hi2s->Init.DataFormat == I2S_WSS_CLOCK_CYCLES_32)
    {
        byte_cycles = 32;
    }
    HAL_RCC_ConfigI2SCLK(hi2s->Init.AudioFreq, byte_cycles);
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    /**I2S GPIO Configuration
     * PB17     ------> I2S_CLK	-AK24       VIDEO_D7
     * PB18     ------> I2S_WS  -L28        EX2_IO7
     * PB19     ------> I2S_DO  M24         EX2_IO8
     * */
    GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF1;
    GPIO_InitStruct.Pin = GPIO_PIN_17 | GPIO_PIN_18 | GPIO_PIN_19;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    /* Peripheral clock enable */
}
void HAL_I2S_MspDeInit(I2S_HandleTypeDef *hi2s)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    /**I2S GPIO Configuration
     * PB17     ------> I2S_CLK	-AK24       VIDEO_D7
     * PB18     ------> I2S_WS  -L28        EX2_IO7
     * PB19     ------> I2S_DO  M24         EX2_IO8
     * */
    GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
    GPIO_InitStruct.Pin = GPIO_PIN_17 | GPIO_PIN_18 | GPIO_PIN_19;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    /* Peripheral clock disable */
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	if(hspi->Instance == SPI0)
	{
		/* SPI0 GPIO Configuration
         * SPI0_SCK 		PB13	-AG25	    VIDEO_D3
		 * SPI0_NSS 		PB12	-K30		EX2_IO6
		 * SPI0_MOSI		PB11	-L27		EX2_IO5
		 * SPI0_MISO		PB10	-L23 		EX2_IO4
		 * */
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF1;
		GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_13;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        
        /* CS must initialized as OUTPUT */
        HAL_GPIO_StructInit(&GPIO_InitStruct);
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	    // GPIO_InitStruct.Alternate = (uint32_t)0;
        // GPIO_InitStruct.Pin = GPIO_PIN_12;
        // HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else if(hspi->Instance == SPI1)
	{
		/* SPI1 GPIO Configuration
         * SPI1_SCK 		PB19	-L28		
		 * SPI1_NSS 		PB18	-M24		
		 * SPI1_MOSI		PB17	-AK24       VIDEO_D7		
		 * SPI1_MISO		PB16	-AC24 		
		 * */
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF4;
		GPIO_InitStruct.Pin = GPIO_PIN_16 | GPIO_PIN_17 | GPIO_PIN_18 | GPIO_PIN_19;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else if(hspi->Instance == SPI2)
	{
		/* SPI2 GPIO Configuration
         * SPI2_SCK 		PA18	-		
		 * SPI2_NSS 		PA17	-		
		 * SPI2_MOSI		PA14	-		
		 * SPI2_MISO		PA13	- 		
		 * */
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF2;
		GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_17 | GPIO_PIN_18;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
    /* Peripheral clock enable */
	__HAL_RCC_SPI_CLK_ENABLE();
}
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	if(hspi->Instance == SPI0)
	{
		/* SPI0 GPIO Configuration
         * SPI0_SCK 		PB13	-AG25	    VIDEO_D3
		 * SPI0_NSS 		PB12	-K30		EX2_IO6
		 * SPI0_MOSI		PB11	-L27		EX2_IO5
		 * SPI0_MISO		PB10	-L23 		EX2_IO4
		 * */
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
		GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_12;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	    // GPIO_InitStruct.Alternate = (uint32_t)0;
        // GPIO_InitStruct.Pin = GPIO_PIN_12;
        // HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else if(hspi->Instance == SPI1)
	{
		/* SPI1 GPIO Configuration
         * SPI1_SCK 		PB19	-L28		
		 * SPI1_NSS 		PB18	-M24		
		 * SPI1_MOSI		PB17	-AK24       VIDEO_D7		
		 * SPI1_MISO		PB16	-AC24 		
		 * */
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
		GPIO_InitStruct.Pin = GPIO_PIN_16 | GPIO_PIN_17 | GPIO_PIN_18 | GPIO_PIN_19;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else if(hspi->Instance == SPI2)
	{
		/* SPI2 GPIO Configuration
         * SPI2_SCK 		PA18	-		
		 * SPI2_NSS 		PA17	-		
		 * SPI2_MOSI		PA14	-		
		 * SPI2_MISO		PA13	- 		
		 * */
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
		GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_17 | GPIO_PIN_18;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
    /* Peripheral clock disable */
	__HAL_RCC_SPI_CLK_DISABLE();
}

/**
* @brief ETH MSP Initialization
* This function configures the hardware resources used in this example
* @param heth: ETH handle pointer
* @retval None
*/
void HAL_ETH_MspInit(ETH_HandleTypeDef* heth)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(heth->Instance == ETH)
    {
        /* USER CODE BEGIN ETH_MspInit 0 */

        /* USER CODE END ETH_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ETH_CLK_ENABLE();

        // __HAL_RCC_GPIOB_CLK_ENABLE();
        /**ETH GPIO Configuration
        PB0     ------> ETH_MDC
        PB1     ------> ETH_MDIO
        PB2     ------> ETH_CLK
        PB3     ------> ETH_TX_EN
        PB4     ------> ETH_TXD0
        PB5     ------> ETH_TXD1
        PB6     ------> ETH_RX_DV
        PB7     ------> ETH_RXD0
        PB8     ------> ETH_RXD1
        */
        HAL_GPIO_StructInit(&GPIO_InitStruct);
        GPIO_InitStruct.Alternate = GPIO_AF1;
        GPIO_InitStruct.Pin = 0x003F;  //PB0-PB5
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Alternate = GPIO_AF5;
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        /* USER CODE BEGIN ETH_MspInit 1 */

        /* USER CODE END ETH_MspInit 1 */
    }
}

/**
* @brief ETH MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param heth: ETH handle pointer
* @retval None
*/
void HAL_ETH_MspDeInit(ETH_HandleTypeDef* heth)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(heth->Instance == ETH)
    {
        /* USER CODE BEGIN ETH_MspDeInit 0 */

        /* USER CODE END ETH_MspDeInit 0 */
        /* Peripheral clock disable */
        // __HAL_RCC_ETH_CLK_DISABLE();

        /**ETH GPIO Configuration
        /**ETH GPIO Configuration
        PB0     ------> ETH_MDC
        PB1     ------> ETH_MDIO
        PB2     ------> ETH_CLK
        PB3     ------> ETH_TX_EN
        PB4     ------> ETH_TXD0
        PB5     ------> ETH_TXD1
        PB6     ------> ETH_RX_DV
        PB7     ------> ETH_RXD0
        PB8     ------> ETH_RXD1
        */
        HAL_GPIO_StructInit(&GPIO_InitStruct);
        GPIO_InitStruct.Alternate = GPIO_AF6;
        GPIO_InitStruct.Pin = 0xFFFE;  //PB1-PB15
        HAL_GPIO_DeInit(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_22;
        HAL_GPIO_DeInit(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN ETH_MspDeInit 1 */

        /* USER CODE END ETH_MspDeInit 1 */
    }
}

