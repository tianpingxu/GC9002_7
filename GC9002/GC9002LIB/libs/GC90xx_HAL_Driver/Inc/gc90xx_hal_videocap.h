/**
  ******************************************************************************
  * @file    gc90xx_hal_videocap.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2022-10-08
  * @brief   This file contains all the functions prototypes for the GPIO
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_HAL_VIDEOCAP_H__

#define __GC90XX_HAL_VIDEOCAP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal_def.h>
#ifdef HAL_VIDEOCAP_MODULE_ENABLED

//control bt656 color depth
#define BT656_COLOR_16Bit                   (0x00000000U)
#define BT656_COLOR_32Bit                   (0x00000002U)

//control bt656 mode
#define BT656_MODE_NORMAL                   (0x00000000U)
#define BT656_MODE_DVP                      (0x00000004U)
#define BT656_MODE_24BitRGB                 (0x00000008U)
#define BT656_MODE_ADC                      (0x0000000CU)

//control bt656 dvp mode
#define BT656_DVP_YUV422                    (0x00000000U)
#define BT656_DVP_RGB565                    (0x00000010U)
#define BT656_DVP_RGB888                    (0x00000020U)

//control bt656 YUV quantization
#define BT656_YUV_QUANTIZATION_FULLRANGE    (0x00000000U)
#define BT656_YUV_QUANTIZATION_LIMITRANGE   (0x00000040U)

//control bt656 sample mode
#define BT656_SAMPLEMODE_MASK               (0x00000300U)
#define BT656_SAMPLEMODE_ODD                (0x00000000U)
#define BT656_SAMPLEMODE_EVEN               (0x00000100U)
#define BT656_SAMPLEMODE_BOTH               (0x00000200U)

//control bt656 de_pol(only used in dvp & 16BitRGB)
#define BT656_DE_POL_HIGH                   (0x00000000U)
#define BT656_DE_POL_LOW                    (0x00001000U)

//control bt656 hs_pol(only used in dvp & 16BitRGB)
#define BT656_HS_POL_HIGH                   (0x00000000U)
#define BT656_HS_POL_LOW                    (0x00002000U)

//control bt656 vs_pol(only used in dvp & 16BitRGB)
#define BT656_VS_POL_HIGH                   (0x00000000U)
#define BT656_VS_POL_LOW                    (0x00004000U)

//control bt656 down sample status
#define BT656_DOWNSAMPLE_DISABLE            (0x00000000U)
#define BT656_DOWNSAMPLE_ENABLE             (0x40000000U)

//control bt656 reset mask
#define BT656_RESET_MASK                    (0x80000000U)

//control bt656 enable
#define BT656_DISABLE                       (0x00000000U)
#define BT656_ENABLE                        (0x00000001U)

#define BT656_INT_DRAWDONE                  (0x00000001U)
#define BT656_INT_ADC                       (0x00000080U)
#define BT656_INT_FULLFIFO                  (0x00000100U)
#define BT656_INT_FRAMEOVERSIZE             (0x00000200U)


typedef enum
{
    VIDEOCAP_DownSample_Disable = BT656_DOWNSAMPLE_DISABLE,
    VIDEOCAP_DownSample_Enable = BT656_DOWNSAMPLE_ENABLE,
}VIDEOCAP_DownSample_Enum;
typedef enum
{
    VIDEOCAP_DE_Pol_High = BT656_DE_POL_HIGH,
    VIDEOCAP_DE_Pol_Low = BT656_DE_POL_LOW,
}VIDEOCAP_DEPol_Enum;
typedef enum
{
    VIDEOCAP_HS_Pol_High = BT656_HS_POL_HIGH,
    VIDEOCAP_HS_Pol_Low = BT656_HS_POL_LOW,
}VIDEOCAP_HSPol_Enum;
typedef enum
{
    VIDEOCAP_VS_Pol_High = BT656_VS_POL_HIGH,
    VIDEOCAP_VS_Pol_Low = BT656_VS_POL_LOW,
}VIDEOCAP_VSPol_Enum;
typedef enum
{
    VIDEOCAP_YUV_FullRange = BT656_YUV_QUANTIZATION_FULLRANGE,
    VIDEOCAP_YUV_LimitedRange = BT656_YUV_QUANTIZATION_LIMITRANGE,
}VIDEOCAP_YUV_Enum;
typedef enum
{
    VIDEOCAP_DVP_YUV422 = BT656_DVP_YUV422,
    VIDEOCAP_DVP_RGB565 = BT656_DVP_RGB565,
    VIDEOCAP_DVP_RGB888 = BT656_DVP_RGB888,
}VIDEOCAP_DVP_Enum;
typedef enum
{
    VIDEOCAP_Mode_Normal = BT656_MODE_NORMAL,
    VIDEOCAP_Mode_DVP = BT656_MODE_DVP,
    VIDEOCAP_Mode_24BitRGB = BT656_MODE_24BitRGB,
    VIDEOCAP_Mode_ADC = BT656_MODE_ADC,
}VIDEOCAP_Mode_Enum;
typedef enum
{
    VIDEOCAP_COLOR_16BIT = BT656_COLOR_16Bit,
    VIDEOCAP_COLOR_32BIT = BT656_COLOR_32Bit,
}VIDEOCAP_ColorDepth_Enum;

#define VIDEOCAP_SAMPLE_FIELD_MASK      BT656_SAMPLEMODE_MASK
typedef enum
{
    VIDEOCAP_Sample_ODD_Filed = BT656_SAMPLEMODE_ODD,   //only sample odd field
    VIDEOCAP_Sample_EVEN_Filed = BT656_SAMPLEMODE_EVEN, //only sample even field
    VIDEOCAP_Sample_BOTH_Filed = BT656_SAMPLEMODE_BOTH, //sample both field
}VIDEOCAP_SampleMode_Enum;
typedef enum
{
    VIDEOCAP_CB_Y0_CR_Y1 = 0,
    VIDEOCAP_Y1_CB_Y0_CR = (0x1 << 15),
    VIDEOCAP_Y0_CR_Y1_CB = (0x2 << 15),
    VIDEOCAP_CR_Y0_CB_Y1 = (0x3 << 15),
}VIDEOCAP_YCBCR_POS_Enum;

typedef struct
{
    uint32_t ColorBit;          /*!< This parameter can be a value of @ref VIDEOCAP_ColorDepth_Enum */
    uint32_t CaptureMode;       /*!< This parameter can be a value of @ref VIDEOCAP_Mode_Enum */
    uint32_t DvpMode;           /*!< This parameter can be a value of @ref VIDEOCAP_DVP_Enum */
    uint32_t YUVQuantization;   /*!< This parameter can be a value of @ref VIDEOCAP_YUV_Enum */
    uint32_t FieldSampleMode;   /*!< This parameter can be a value of @ref VIDEOCAP_SampleMode_Enum */
    uint32_t DE_Pol;            /*!< This parameter can be a value of @ref VIDEOCAP_DEPol_Enum */
    uint32_t HS_Pol;            /*!< This parameter can be a value of @ref VIDEOCAP_HSPol_Enum */
    uint32_t VS_Pol;            /*!< This parameter can be a value of @ref VIDEOCAP_VSPol_Enum */
    uint32_t YCBCR_POS;         /*!< This parameter can be a value of @ref VIDEOCAP_YCBCR_POS_Enum */
    uint32_t DownscaleSample;   /*!< This parameter can be a value of @ref VIDEOCAP_DownSample_Enum */
    uint32_t MaxAddrSize;
    uint32_t CH0FieldBaseAddr;
    uint32_t FrameNumCnt;
    uint32_t MaxADCFrameNum;//only used in adc mode
}VIDEOCAP_InitTypeDef;
/**
  * @brief  HAL State structures definition
  */
typedef enum
{
    HAL_VIDEOCAP_STATE_RESET             = 0x00U,  /*!< VIDEOCAP not yet initialized or disabled */
    HAL_VIDEOCAP_STATE_READY             = 0x01U,  /*!< VIDEOCAP initialized and ready for use   */
    HAL_VIDEOCAP_STATE_BUSY              = 0x02U,  /*!< VIDEOCAP process is ongoing              */
    HAL_VIDEOCAP_STATE_TIMEOUT           = 0x06U,  /*!< Timeout state                            */
    HAL_VIDEOCAP_STATE_ERROR             = 0x0AU   /*!< VIDEOCAP error state                     */
}HAL_VIDEOCAP_StateTypeDef;
/**
 * @brief  VIDEOCAP handle Structure definition
 */
typedef struct __VIDEOCAP_HandleTypeDef
{
    VIDEOCAP_TypeDef                *Instance;                  /*!< Register base address */
    VIDEOCAP_InitTypeDef            Init;                       /*!< VIDEOCAP required parameters */
    uint32_t                        int_flag;
    HAL_LockTypeDef                 Lock;                       /*!< VIDEOCAP locking object */
    __IO HAL_VIDEOCAP_StateTypeDef  State;                      /*!< VIDEOCAP communication state */
    __IO uint32_t                   ErrorCode;                  /*!< VIDEOCAP Error code.
                                                                This parameter can be a value of @ref VIDEOCAP_Error_Code */
} VIDEOCAP_HandleTypeDef;

/** @defgroup Vedio Capture Interrupt definition
  * @{
  */
#define VIDEOCAP_IT_DRAWDONE                            BT656_INT_DRAWDONE
#define VIDEOCAP_IT_ADC                                 BT656_INT_ADC
#define VIDEOCAP_IT_FULLFIFO                            BT656_INT_FULLFIFO
#define VIDEOCAP_IT_FRAMEOVERSIZE                       BT656_INT_FRAMEOVERSIZE
/** @brief  Vedio Capture Interrupt Enable/Disable
  * @param  __HANDLE__ specifies the Vedio Capture Handle.
  * @param  __FLAG__    specifies the interrupt source.
  *         This parameter can be one of the following values:
  *            @arg VIDEOCAP_IT_DRAWDONE              drawing done.
  *            @arg VIDEOCAP_IT_ADC                   ADC.
  *            @arg VIDEOCAP_IT_FULLFIFO              Write full data fifo.
  *            @arg VIDEOCAP_IT_FRAMEOVERSIZE         frame oversize error.
  * @retval None
  */
#define __HAL_VIDEOCAP_IT_ENABLE(__HANDLE__, __IT__)        SET_BIT((__HANDLE__)->Instance->IER, (__IT__))
#define __HAL_VIDEOCAP_IT_DISABLE(__HANDLE__, __IT__)       CLEAR_BIT((__HANDLE__)->Instance->IER, (__IT__))

/** @brief  Get/Clear Vedio Capture Interrupt Flag
  * @param  __HANDLE__ specifies the Vedio Capture Handle.
  * @param  __FLAG__    specifies the interrupt source.
  *         This parameter can be one of the following values:
  *            @arg VIDEOCAP_IT_DRAWDONE              drawing done.
  *            @arg VIDEOCAP_IT_ADC                   ADC.
  *            @arg VIDEOCAP_IT_FULLFIFO              Write full data fifo.
  *            @arg VIDEOCAP_IT_FRAMEOVERSIZE         frame oversize error.
  * @retval None
  */
#define __HAL_VIDEOCAP_GET_IT_FLAG(__HANDLE__, __FLAG__)      (((__HANDLE__)->Instance->ISR & (__FLAG__)) != RESET)
#define __HAL_VIDEOCAP_CLEAR_IT_FLAG(__HANDLE__, __FLAG__)    CLEAR_BIT((__HANDLE__)->Instance->ISR, (__FLAG__))

/*-----------------drv cmd define ------------------*/
#define RT_VIDEOCAP_CMD_SET_BASEADDR            0x11
#define RT_VIDEOCAP_CMD_SET_SAMPLEMODE          0x12
#define RT_VIDEOCAP_CMD_GET_PARAM               0x13

HAL_StatusTypeDef HAL_VIDEOCAP_StructInit(VIDEOCAP_HandleTypeDef *hvideocap);
HAL_StatusTypeDef HAL_VIDEOCAP_Init(VIDEOCAP_HandleTypeDef *hvideocap);
HAL_StatusTypeDef HAL_VIDEOCAP_SetBaseAddress(VIDEOCAP_HandleTypeDef *hvideocap, uint32_t address);
HAL_StatusTypeDef HAL_VIDEOCAP_Disable(VIDEOCAP_HandleTypeDef *hvideocap);
HAL_StatusTypeDef HAL_VIDEOCAP_Start(VIDEOCAP_HandleTypeDef *hvideocap);
HAL_StatusTypeDef HAL_VIDEOCAP_SetSampleMode(VIDEOCAP_HandleTypeDef *hvideocap, VIDEOCAP_SampleMode_Enum mode);
HAL_StatusTypeDef HAL_VIDEOCAP_GetParam(VIDEOCAP_HandleTypeDef *hvideocap, uint16_t* Width, uint16_t* Height);

void HAL_VIDEOCAP_IRQHandler(VIDEOCAP_HandleTypeDef *hvideocap);
void HAL_VIDEOCAP_MspInit(VIDEOCAP_HandleTypeDef *hvideocap);
#endif/* HAL_VIDEOCAP_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif/* __GC90XX_HAL_VIDEOCAP_H__ */
