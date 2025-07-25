#ifndef __CSI_CONFIG_H__
#define __CSI_CONFIG_H__

#define __SYSTEM_INIT                   1U
#define __BOARD_INIT                    1U
#define CONFIG_CPU_GC9002               1U
#define CONFIG_SYSTEM_SECURE            1U
/* @note If you use RTOS, mask this macro definition
 */
//#define CONFIG_KERNEL_NONE              1U

#ifndef CONFIG_KERNEL_NONE
#define CONFIG_KERNEL_FREERTOS 1U
#endif

#define CONFIG_PLL_FREQ                 RCC_PLL_480M
#define CONFIG_AHB_DIV                  RCC_AHB_Pres_4
#define CONFIG_USE_HYPER                1U
#define CONFIG_USE_ICACHE               1U
#define CONFIG_USE_DCACHE               0U
#define CONFIG_HYPER_DIV                HYPER_Pres_0
/* @note Please set it to 0
 * @mark E902M core not support none vector interrupt
 */
#define CONFIG_CSI_NON_VECTOR           0U
#define CONFIG_VECTOR_TABLE_SIZE        48
#define CONFIG_ARCH_INTERRUPTSTACK      1024
#define CONFIG_ATTRIBUTE_FLASH_DRIVER   __attribute__((used, section(".flashdriver")))

#ifndef CONFIG_SYSTICK_HZ
#define CONFIG_SYSTICK_HZ               1000   //1/CONFIG_SYSTICK_HZ s
#endif
#define CONFIG_QSPI_FLASH_FREQ          CSI_QSPI_FREQ_MAX

#define CONFIG_PRINTF_ENABLE            1U
#define CONFIG_CONSOLE_DEBUG_UART_EN    1U
#define CONFIG_CONSOLE_DEBUG_UART       UART0
#define CONFIG_FPGA_SOC                 USE_FPGA_SOC
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
#define CONFIG_FPGA_SOC_FREQ            USE_FPGA_SOC_FREQ
#endif


/* @note csi driver switch define
 * @mark none
 */
#define CONFIG_CSI_DRV_TICK_ENABLED
#define CONFIG_CSI_DRV_GPIO_ENABLED
#define CONFIG_CSI_DRV_GPIO_PIN_ENABLED
#define CONFIG_CSI_DRV_PIN_ENABLED
#define CONFIG_CSI_DRV_DMA_ENABLED
#define CONFIG_CSI_DRV_UART_ENABLED
#define CONFIG_CSI_DRV_TIMER_ENABLED
#define CONFIG_CSI_DRV_WDT_ENABLED
#define CONFIG_CSI_DRV_CRC_ENABLED
#define CONFIG_CSI_DRV_RTC_ENABLED
#define CONFIG_CSI_DRV_SPI_ENABLED
#define CONFIG_CSI_DRV_IIC_ENABLED
#define CONFIG_CSI_DRV_PWM_ENABLED
#define CONFIG_CSI_DRV_PORTING_ENABLED
#define CONFIG_CSI_DRV_QSPI_ENABLED
#define CONFIG_CSI_DRV_SPIFLASH_ENABLED
#define CONFIG_CSI_DRV_I2S_ENABLED
#define CONFIG_CSI_DRV_TRIANGLE_ENABLED
#define CONFIG_CSI_DRV_ADC_ENABLED
#define CONFIG_DRV_DISPLAY_ENABLED

#ifdef CONFIG_CSI_DRV_PWM_ENABLED
/* @note If you want use motor pwm channel, must set it to 1
 */
#define CONFIG_CSI_DRV_MOTOR_PWM        1U
#endif /* CONFIG_CSI_DRV_PWM_ENABLED */

/* @note AT client driver frame switch define
 * @mark none
 */
#define CONFIG_AT_CLIENT_ENABLE     0U
#define CONFIG_NETDEV_ENABLE        0U
#define CONFIG_SAL_ENABLE           0U


#endif /* __CSI_CONFIG_H__ */
