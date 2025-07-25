#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "display_conf.h"
#if(defined DRV_DISPLAY_RGB565_ENABLED && DRV_DISPLAY_RGB565_ENABLED == 1U)
#include "display_rgb565.h"
#endif
#if(defined DRV_DISPLAY_888_ENABLED && DRV_DISPLAY_888_ENABLED == 1U)
#include "display_888.h"
#endif
#if(defined DRV_DISPLAY_MRB3205_ENABLED && DRV_DISPLAY_MRB3205_ENABLED == 1U)
#include "display_mrb3205.h"
#endif
#if(defined DRV_DISPLAY_SJXD_ENABLED && DRV_DISPLAY_SJXD_ENABLED == 1U)
#include "display_sjxd.h"
#endif
#if(defined DRV_DISPLAY_ST7512_ENABLED && DRV_DISPLAY_ST7512_ENABLED == 1U)
#include "display_st7512.h"
#endif
#if(defined DRV_DISPLAY_ST7735S_ENABLED && DRV_DISPLAY_ST7735S_ENABLED == 1U)
#include "display_st7735s.h"
#endif
#if(defined DRV_DISPLAY_ST77903_ENABLED && DRV_DISPLAY_ST77903_ENABLED == 1U)
#include "display_st77903.h"
#endif
#if(defined DRV_DISPLAY_LEIKE_ENABLED && DRV_DISPLAY_LEIKE_ENABLED == 1U)
#include "display_leike.h"
#endif
#if(defined DRV_DISPLAY_TFT128QM071_ENABLED && DRV_DISPLAY_TFT128QM071_ENABLED == 1U)
#include "display_tft128qm071.h"
#endif
#if(defined DRV_DISPLAY_GC9307N_ENABLED && DRV_DISPLAY_GC9307N_ENABLED == 1U)
#include "display_gc9307n.h"
#endif

#ifdef CONFIG_DRV_DISPLAY_ENABLED

#define DISPLAY_STREAM_LENGTH       3
#define DISPLAY_BUFFER_SIZE         (DISPLAY_STREAM_LENGTH+1)
#define DISPLAY_BRIGHT_DEFAULT      100
#define DISPLAY_BRIGHT_MAX          200
#define DISPLAY_CONTRAST_DEFAULT    100
#define DISPLAY_CONTRAST_MAX        200
#define DISPLAY_SATURATION_DEFAULT  100
#define DISPLAY_SATURATION_MAX      200
#define DISPLAY_COLOR_MEDIAN        0x7F
#define DISPLAY_HUE_ANGLE_DEFAULT   (0)

typedef enum
{
    DisplayWait,
    DisplayDone,
}DisplayState;
struct DisplayStreamCtrl
{
    DISPLAY_MasterParamType framebuffer[DISPLAY_BUFFER_SIZE];
    uint8_t state[DISPLAY_BUFFER_SIZE];
    uint8_t m_PushIdx;  //入队列索引
    uint8_t m_PopIdx;   //出队列索引
};
typedef struct
{
    union 
    {
        struct
        {
            unsigned char m[2][2];
        };
        DISPLAY_YCT_Type yct;
    };
}mat2_t;
typedef struct
{
    union 
    {
        struct
        {
            unsigned char m[4][4];
        };
        struct
        {
            DISPLAY_CCM_Type ccm;
            unsigned char t[4];
        };
    };
}mat4_t;
class Display
{
private:
    static struct DisplayStreamCtrl m_field;
    static DISPLAY_HandleTypeDef m_hdisplay;
    static uint32_t m_bright;       /* 记录当前亮度[0~100],默认值 @ref DISPLAY_BRIGHT_DEFAULT       */
    static uint32_t m_contrast;     /* 记录当前对比度[0~100],默认值 @ref DISPLAY_CONTRAST_DEFAULT   */
    static uint32_t m_saturation;   /* 记录当前饱和度[0~100],默认值 @ref DISPLAY_SATURATION_DEFAULT */
    static int64_t m_hue_angle;     /* 记录当前色相 Fixed-point integer: 1.39.24,默认值 0           */
    static mat4_t m_mat4_bright;
    static mat4_t m_mat4_contrast;
    static mat4_t m_mat4_ccm;
    static mat2_t m_mat2_yct;
    static void matrix_multiply_fixed_unsigned(const mat4_t *mat_a, const mat4_t *mat_b, mat4_t *mat_r, unsigned char q);
    static void set_mat4_bright(void);
    static void set_mat4_contrast(void);
    static void set_mat2_hue_saturation(void);
public:
    Display() 
    {
		Display::m_hdisplay.Instance = DRV_DISPLAY_INSTANCE;
		
        Display::m_bright = DISPLAY_BRIGHT_DEFAULT;
        Display::m_contrast = DISPLAY_CONTRAST_DEFAULT;
//        Display::m_saturation = DISPLAY_SATURATION_MAX;
//        Display::m_hue_angle = 0;

//        Display::m_mat4_bright.t[0] = 0;
//        Display::m_mat4_bright.t[1] = 0;
//        Display::m_mat4_bright.t[2] = 0;
//        Display::m_mat4_bright.t[3] = 1;
//
//        Display::m_mat4_contrast.t[0] = 0;
//        Display::m_mat4_contrast.t[1] = 0;
//        Display::m_mat4_contrast.t[2] = 0;
//        Display::m_mat4_contrast.t[3] = 1;
    };
    /**
     * @brief  Display initial configuration
     * @param  config Configuration parameter of display LCD
     * @retval true-success  false-failed
     */
    static bool Init(struct display_conf *config);
    /**
     * @brief  Display vs_int irq handler
     * @param  hdisplay pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
     * @retval void
     */
    static void VSIntCallback(DISPLAY_HandleTypeDef *hdisplay);
    /**
     * @brief  检查Display FIFO是否满
     * @retval true-Fifo is empty  false-Fifo is not empty
     */
    static bool IsEmpty(void);
    /**
     * @brief  检查Display FIFO是否满
     * @retval true-Fifo is full  false-Fifo is not full
     */
    static bool IsFull(void);
    /**
     * @brief  入队方式插入已准备好的FrameBuffer参数结构体-由AHMI 中断callback调用.
     * @param  para_ptr pointer of para framebuffer
     * @retval true-success  false-Fifo is full
     */
    static bool DisplayPush(DISPLAY_MasterParamType *para_ptr);
    /**
     * @brief  出队方式获取已准备好的FrameBuffer参数结构体-由Display 中断callback调用.
     * @param  p_para_ptr pointer of para framebuffer to get
     * @retval true-success  false-Fifo is empty,no new framebuffer
     */
    static bool DisplayPop(DISPLAY_MasterParamType **p_para_ptr);
    /**
     * @brief  查询FrameBuffer是否发送完成.
     * @param  p_para para of framebuffer
     * @retval true-display done  false-waitting
     */
    static bool GetDisplayDone(DISPLAY_MasterParamType *p_para);
    /**
     * @brief  设置亮度.
     * @param  percentage 亮度调整比例[0,100]
     * @retval true-success  false-failed
     */
    static bool SetBright(uint32_t percentage);
    /**
     * @brief  设置对比度.
     * @param  percentage 对比度调整比例[0,100]
     * @retval true-success  false-failed
     */
    static bool SetContrast(uint32_t percentage);
    /**
     * @brief  设置色相和饱和度.
     * @param  percentage 饱和度调整比例[0,100]
     * @param  angle 色相旋转角度 Fixed-point integer: 1.39.24
     * @retval true-success  false-failed
     */
    static bool SetHueSaturation(uint32_t percentage, int64_t angle);
    /**
     * @brief  伽玛校正.
     * @param  gamma Fixed-point integer: 1.15.16
     * @retval true-success  false-failed
     */
    static bool GammaCorrection(int gamma);
};

#endif /* CONFIG_DRV_DISPLAY_ENABLED */
#endif /* __DISPLAY_H__ */