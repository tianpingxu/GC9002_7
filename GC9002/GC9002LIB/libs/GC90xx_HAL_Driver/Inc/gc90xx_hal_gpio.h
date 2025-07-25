/**
  ******************************************************************************
  * @file    gc90xx_hal_gpio.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2022-10-08
  * @brief   This file contains all the functions prototypes for the GPIO
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_HAL_GPIO_H__

#define __GC90XX_HAL_GPIO_H__

#ifdef __cplusplus
extern "C"{
#endif

/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal_def.h>
#ifdef HAL_GPIO_MODULE_ENABLED

/* constants definitions */
typedef FlagStatus bit_status;

/* interrupt trigger mode */
typedef enum {
    EXTI_TRIG_RISING = 0,                                     /*!< EXTI rising edge trigger */
    EXTI_TRIG_FALLING,                                        /*!< EXTI falling edge trigger */
    EXTI_TRIG_BOTH,                                           /*!< EXTI rising edge and falling edge trigger */
    EXTI_TRIG_NONE                                            /*!< without rising edge or falling edge trigger */
} exti_trig_type_enum;


#define GPIO_PIN_All                GPIO_PIN_MASK  /* All pins selected */
#define GPIO_PIN_MASK               0xFFFFFFFFU /* PIN mask for assert test */
#define IS_GPIO_PIN(PIN)            ((((PIN) & GPIO_PIN_MASK ) != 0x00U) && (((PIN) & ~GPIO_PIN_MASK) == 0x00U))

#define IS_GPIO_INISTRUCT(STRUCT)   (STRUCT != NULL)
#define IS_GPIO_BIT_ACTION(ACTION)  (((ACTION) == GPIO_PIN_RESET) || ((ACTION) == GPIO_PIN_SET))
#define IS_GPIO_FUNCSTATE(STATE)    (((STATE) == DISABLE) || ((STATE) == ENABLE))


#define GPIO_PIN_0                 ((uint32_t)0x00000001) /*!< Pin 0 selected */
#define GPIO_PIN_1                 ((uint32_t)0x00000002) /*!< Pin 1 selected */
#define GPIO_PIN_2                 ((uint32_t)0x00000004) /*!< Pin 2 selected */
#define GPIO_PIN_3                 ((uint32_t)0x00000008) /*!< Pin 3 selected */
#define GPIO_PIN_4                 ((uint32_t)0x00000010) /*!< Pin 4 selected */
#define GPIO_PIN_5                 ((uint32_t)0x00000020) /*!< Pin 5 selected */
#define GPIO_PIN_6                 ((uint32_t)0x00000040) /*!< Pin 6 selected */
#define GPIO_PIN_7                 ((uint32_t)0x00000080) /*!< Pin 7 selected */
#define GPIO_PIN_8                 ((uint32_t)0x00000100) /*!< Pin 8 selected */
#define GPIO_PIN_9                 ((uint32_t)0x00000200) /*!< Pin 9 selected */
#define GPIO_PIN_10                ((uint32_t)0x00000400) /*!< Pin 10 selected */
#define GPIO_PIN_11                ((uint32_t)0x00000800) /*!< Pin 11 selected */
#define GPIO_PIN_12                ((uint32_t)0x00001000) /*!< Pin 12 selected */
#define GPIO_PIN_13                ((uint32_t)0x00002000) /*!< Pin 13 selected */
#define GPIO_PIN_14                ((uint32_t)0x00004000) /*!< Pin 14 selected */
#define GPIO_PIN_15                ((uint32_t)0x00008000) /*!< Pin 15 selected */
#define GPIO_PIN_16                ((uint32_t)0x00010000) /*!< Pin 16 selected */
#define GPIO_PIN_17                ((uint32_t)0x00020000) /*!< Pin 17 selected */
#define GPIO_PIN_18                ((uint32_t)0x00040000) /*!< Pin 18 selected */
#define GPIO_PIN_19                ((uint32_t)0x00080000) /*!< Pin 19 selected */
#define GPIO_PIN_20                ((uint32_t)0x00100000) /*!< Pin 20 selected */
#define GPIO_PIN_21                ((uint32_t)0x00200000) /*!< Pin 21 selected */
#define GPIO_PIN_22                ((uint32_t)0x00400000) /*!< Pin 22 selected */
#define GPIO_PIN_23                ((uint32_t)0x00800000) /*!< Pin 23 selected */
#define GPIO_PIN_24                ((uint32_t)0x01000000) /*!< Pin 24 selected */
#define GPIO_PIN_25                ((uint32_t)0x02000000) /*!< Pin 25 selected */
#define GPIO_PIN_26                ((uint32_t)0x04000000) /*!< Pin 26 selected */
#define GPIO_PIN_27                ((uint32_t)0x08000000) /*!< Pin 27 selected */
#define GPIO_PIN_28                ((uint32_t)0x10000000) /*!< Pin 28 selected */
#define GPIO_PIN_29                ((uint32_t)0x20000000) /*!< Pin 29 selected */
#define GPIO_PIN_30                ((uint32_t)0x40000000) /*!< Pin 30 selected */
#define GPIO_PIN_31                ((uint32_t)0x80000000) /*!< Pin 31 selected */
#define GPIO_PIN_ALL               ((uint32_t)0xFFFFFFFF) /*!< Pin ALL selected */
/**
  * @brief  GPIO Bit SET and Bit RESET enumeration
  */
typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
}GPIO_PinState;

typedef struct
{
	uint32_t Pin;
    uint32_t Mode;      /*!< Specifies the operating mode for the selected pins.
                           This parameter can be a value of @ref GPIO_mode_define */
    uint32_t Pull;      /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.
                             This parameter can be a value of @ref GPIO_pull_define */
    uint32_t Speed;     /*!< Specifies the speed for the selected pins.
                             This parameter can be a value of @ref GPIO_speed_define */
    uint32_t SchmidtTrigger;     /*!< Specifies the SchmidtTrigger for the selected pins.
                             This parameter can be a value of @ref GPIO_schmidt_trigger_define */
    uint32_t DriverStrength;     /*!< Specifies the DriverStrength for the selected pins.
                          This parameter can be a value of @ref GPIO_driver_strength_define */
    uint32_t Alternate;  /*!< Peripheral to be connected to the selected pins.
                            This parameter can be a value of @ref GPIO_Alternate_function_selection */
#if(defined BSP_GPIO_USE_AFMODE && BSP_GPIO_USE_AFMODE == 1U)
    uint32_t AltfMode;   /*!< Specifies the Altf mode for the selected pins.
                            This parameter can be a value of @ref GPIO_Altf_Mode_selection */
#endif /* BSP_GPIO_USE_AFMODE */
}GPIO_InitTypeDef;

typedef enum
{
    GPIO_DIR_INPUT,   //输入模式
    GPIO_DIR_OUTPUT,  //输出模式
}GPIO_Dir;
#define IS_GPIO_DIR(DIR)        ((DIR) == GPIO_DIR_INPUT || (DIR) == GPIO_DIR_OUTPUT)
/** @defgroup GPIO_mode_define GPIO mode define
  * @brief GPIO Configuration Mode
  *        Elements values convention: 0xX0yz00YZ
  *           - X  : GPIO mode or EXTI Mode
  *           - y  : External IT or Event trigger detection
  *           - z  : IO configuration on External IT or Event
  *           - Y  : Output type (Push Pull or Open Drain)
  *           - Z  : IO Direction mode (Input, Output, Alternate or Analog)
  * @{
  */
#define  GPIO_MODE_INPUT                        0x00010000U   /*!< Input Floating Mode                   */
#define  GPIO_MODE_OUTPUT_PP                    0x00000000U   /*!< Output Push Pull Mode                 */
#define  GPIO_MODE_OUTPUT_OD                    0x00020000U   /*!< Output Open Drain Mode                */
#define  GPIO_MODE_AF_PP                        0x07000000U   /*!< Alternate Function Push Pull Mode     */
#define  GPIO_MODE_AF_OD                        0x07020000U   /*!< Alternate Function Open Drain Mode    */
/*GC9005 Analog Mode
 * @remark set Input mode for specific IO
 * */
#define  GPIO_MODE_ANALOG                       0xFFFFFFFFU   /*!< Analog Mode  */

#define  GPIO_MODE_IT_RISING                    0x00000800U   /*!< External Interrupt Mode with Rising edge trigger detection          */
#define  GPIO_MODE_IT_FALLING                   0x00000C00U   /*!< External Interrupt Mode with Falling edge trigger detection         */
#define  GPIO_MODE_IT_RISING_FALLING            0x00000A00U   /*!< External Interrupt Mode with Rising/Falling edge trigger detection  */

#define  GPIO_MODE_IT_LOW                       0x00000D00U   /*!< External Interrupt Mode with Low Level trigger detection          */
#define  GPIO_MODE_IT_HIGH                      0x00000900U   /*!< External Interrupt Mode with High Level trigger detection         */
#define  IS_GPIO_MODE(MODE)      (((MODE) & ~(GPIO_MODE_INPUT | \
                                              GPIO_MODE_OUTPUT_PP | \
                                              GPIO_MODE_OUTPUT_OD | \
                                              GPIO_MODE_AF_PP | \
                                              GPIO_MODE_AF_OD | \
                                              GPIO_MODE_IT_RISING | \
                                              GPIO_MODE_IT_FALLING | \
                                              GPIO_MODE_IT_RISING_FALLING | \
                                              GPIO_MODE_IT_LOW | \
                                              GPIO_MODE_IT_HIGH)) == 0x00000000U \
                                              || (MODE) == GPIO_MODE_ANALOG)
                                              
#define  IS_GPIO_IT_MODE(MODE)   ((MODE) & GPIO_MODE_IT_RISING)
/** @defgroup GPIO_pull_define GPIO pull define
  * @brief GPIO Pull-Up or Pull-Down Activation
  * @{
  */
#define  GPIO_NOPULL        0x00000000U   /*!< No Pull-up or Pull-down activation  */
#define  GPIO_PULLUP        0x00080000U   /*!< Pull-up activation                  */
#define  GPIO_PULLDOWN      0x00040000U   /*!< Pull-down activation                */
#define IS_GPIO_PULL(PULL) (((PULL) == GPIO_NOPULL) || ((PULL) == GPIO_PULLUP) || \
                            ((PULL) == GPIO_PULLDOWN))

/** @defgroup GPIO_schmidt_trigger_define GPIO schmidt trigger define
  * @brief GPIO Schmidt Trigger Enable or Disable
  * @{
  */
#define GPIO_SCHMIDT_TRIGGER_ENABLE     GPIO_Px_ST
#define GPIO_SCHMIDT_TRIGGER_DISABLE    0x00000000U
#define IS_GPIO_SCHMIDT_TRIGGER(STATE) (((STATE) == GPIO_SCHMIDT_TRIGGER_ENABLE) || ((STATE) == GPIO_SCHMIDT_TRIGGER_DISABLE))

/** @defgroup GPIO_driver_strength_define GPIO driver strength define
  * @brief GPIO driver strength 0/1/2/3
  * @{
  */
#define GPIO_DRIVER_STRENGTH_0  (0 << GPIO_Px_DS_Pos)
#define GPIO_DRIVER_STRENGTH_1  (1 << GPIO_Px_DS_Pos)
#define GPIO_DRIVER_STRENGTH_2  (2 << GPIO_Px_DS_Pos)
#define GPIO_DRIVER_STRENGTH_3  (3 << GPIO_Px_DS_Pos)
#define IS_GPIO_DRIVER_STRENGTH(DS) (((DS) == GPIO_DRIVER_STRENGTH_0) || ((DS) == GPIO_DRIVER_STRENGTH_1) || \
                                     ((DS) == GPIO_DRIVER_STRENGTH_2) || ((DS) == GPIO_DRIVER_STRENGTH_3))
/** @defgroup GPIO_speed_define  GPIO speed define
  * @brief GPIO Output Maximum frequency
  * @{
  */
#define GPIO_SPEED_FREQ_LOW         0x00000000U  /*!< IO works at x MHz, please refer to the product datasheet */
#define GPIO_SPEED_FREQ_HIGH        GPIO_Px_SL  /*!< range xx MHz to xx MHz, please refer to the product datasheet  */
#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_SPEED_FREQ_LOW) || ((SPEED) == GPIO_SPEED_FREQ_HIGH))
/**
  * @ref GPIO_Alternate_function_selection
  */
#define GPIO_AF0                (0UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF1                (1UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF2                (2UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF3                (3UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF4                (4UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF5                (5UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF6                (6UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF7                (7UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF8                (8UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF9                (9UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF10               (10UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF11               (11UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF12               (12UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF13               (13UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF14               (14UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF15               (15UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF16               (16UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF17               (17UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF18               (18UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF19               (19UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF20               (20UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF21               (21UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF22               (22UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF23               (23UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF24               (24UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF25               (25UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF26               (26UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF27               (27UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF28               (28UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF29               (29UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF30               (30UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF31               (31UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF32               (32UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF33               (33UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF34               (34UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF35               (35UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF36               (36UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF37               (37UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF38               (38UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF39               (39UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF40               (40UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF41               (41UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF42               (42UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF43               (43UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF44               (44UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF45               (45UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF46               (46UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF47               (47UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF48               (48UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF49               (49UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF50               (50UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF51               (51UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF52               (52UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF53               (53UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF54               (54UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF55               (55UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF56               (56UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF57               (57UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF58               (58UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF59               (59UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF60               (60UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF61               (61UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF62               (62UL << GPIO_Px_ALTF_Pos)
#define GPIO_AF63               (63UL << GPIO_Px_ALTF_Pos)
#define IS_GPIO_ALTERNATE(ALTF) ((((ALTF) & ~GPIO_Px_ALTF_Msk) == 0))

#if(defined BSP_GPIO_USE_AFMODE && BSP_GPIO_USE_AFMODE == 1U)
/**
  * @ref GPIO_Altf_Mode_selection
  */
#define GPIO_AFM0                (0x00UL << GPIO_Px_ALTF_MODE_Pos)
#define GPIO_AFM1                (0x01UL << GPIO_Px_ALTF_MODE_Pos)
#define GPIO_AFM2                (0x02UL << GPIO_Px_ALTF_MODE_Pos)
#define GPIO_AFM3                (0x03UL << GPIO_Px_ALTF_MODE_Pos)
#define GPIO_AFM4                (0x04UL << GPIO_Px_ALTF_MODE_Pos)
#define GPIO_AFM5                (0x05UL << GPIO_Px_ALTF_MODE_Pos)
#define GPIO_AFM6                (0x06UL << GPIO_Px_ALTF_MODE_Pos)
#define GPIO_AFM7                (0x07UL << GPIO_Px_ALTF_MODE_Pos)
#define IS_GPIO_AFMMODE(AFM)     (((AFM) == GPIO_AFM0) || \
                                  ((AFM) == GPIO_AFM1) || \
                                  ((AFM) == GPIO_AFM2) || \
                                  ((AFM) == GPIO_AFM3) || \
                                  ((AFM) == GPIO_AFM4) || \
                                  ((AFM) == GPIO_AFM5) || \
                                  ((AFM) == GPIO_AFM6) || \
                                  ((AFM) == GPIO_AFM7))

#endif /* BSP_GPIO_USE_AFMODE */
#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
/* if BSP_GPIO_USE_PHY defined, and equal 1U
 * phy_pin_mode_init must be create a new entity function in file gc90xx_hal.c
 */
void phy_pin_mode_init(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
/* if BSP_GPIO_USE_PHY defined, and equal 1U
 * phy_pin_mode_ctrl must be create a new entity function in file gc90xx_hal.c
 */
void phy_pin_mode_ctrl(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
void phy_pin_ch1_all_line_disable(void);
#endif /* BSP_GPIO_USE_PHY */

void HAL_GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
void HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void HAL_GPIO_SetInterrupt(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef* GPIO_InitStruct, FunctionalState NewState);
void HAL_GPIO_ClearInterruptFlag(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
uint32_t HAL_GPIO_GetInterruptStatus(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);

#if(defined BSP_GPIO_USE_AFMODE && BSP_GPIO_USE_AFMODE == 1U)
void     HAL_GPIO_SetALTFMode(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t GPIO_AFM);
#endif /* BSP_GPIO_USE_AFMODE */
void     HAL_GPIO_SetALTF(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t GPIO_AFx);
uint32_t HAL_GPIO_GetALTF(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
void     HAL_GPIO_SetMode(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t Mode, uint32_t Pull);
void     HAL_GPIO_SetDir(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, GPIO_Dir Dir);
void     HAL_GPIO_SetSpeed(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t Speed);
void     HAL_GPIO_SetDrive(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t Drive);

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, GPIO_PinState PinState);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);

void LL_GPIO_SetMode(GPIO_TypeDef* GPIOx, uint32_t mode);

#endif/* HAL_GPIO_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif/* __GC90XX_HAL_GPIO_H__ */
