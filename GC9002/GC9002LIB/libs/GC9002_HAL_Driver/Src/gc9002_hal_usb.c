#include "gc90xx_hal_def.h"
#include "core_rv32.h"
#include "csi_irq.h"

#include "FreeRTOS.h"
#include "task.h"
#include "ff.h"
#include "diskio.h"
#include "usbh_core.h"

#if(defined CONFIG_RUN_BOOTLOADER && CONFIG_RUN_BOOTLOADER == 1U)
#include "cfg_c1.h"

#define SYN_GET_USB_IF_USE_ENABLE               (CFG_Get_usb_if_use_enable())
#define SYN_GET_USB_CTRL_ENABLE_PORT            (CFG_pCfg2Port(CFG_Get_usb_ctrl_enable_port()))
#define SYN_GET_USB_CTRL_ENABLE_PIN             (CFG_u32Cfg2Pin(CFG_Get_usb_ctrl_enable_pin()))

#define SYN_GET_USB_IF_USE_INSERT_CHECK         (CFG_Get_usb_if_use_insert_check())
#define SYN_GET_USB_CTRL_INSERT_CHECK_PORT      (CFG_pCfg2Port(CFG_Get_usb_ctrl_insert_check_port()))
#define SYN_GET_USB_CTRL_INSERT_CHECK_PIN       (CFG_u32Cfg2Pin(CFG_Get_usb_ctrl_insert_check_pin()))

#endif

#define USE_USB_TEST_TASK     0
//#define HARDWARE_VERSION_V1_1
#define HARDWARE_VERSION_V2_0

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
void OTG_IRQHandler(void);
#else
__attribute__ ((interrupt ("machine"))) void OTG_IRQHandler(void);
#endif

volatile uint32_t g_u32USBConnFlag = 0;

uint32_t checkMSDDevice(void)
{
#if(defined CONFIG_RUN_BOOTLOADER && CONFIG_RUN_BOOTLOADER == 1U)
    uint32_t insert_check_pin;
    GPIO_TypeDef* insert_check_port;
    uint32_t res = 0;
    
    insert_check_pin = SYN_GET_USB_CTRL_INSERT_CHECK_PIN;
    insert_check_port = SYN_GET_USB_CTRL_INSERT_CHECK_PORT;

    if((SYN_GET_USB_IF_USE_INSERT_CHECK))
    {
        if(HAL_GPIO_ReadPin(insert_check_port, insert_check_pin) == 1)
        {
            /* Configed USB Insert Detecting and No USB insert */
        }
        else
        {
            res = 1;
        }
    }
    else /* No config USB Insert Detecting */
    {
        for(int i = 0; i < 40; i++)
        {
            vTaskDelay(20 / portTICK_RATE_MS);
            
            if(g_u32USBConnFlag > 2)
            { 
                res = 1;
                break;
            }
        }
    }

    return res;
#else
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
#endif
}

void usb_hc_low_level_init(void)
{
#if defined (HARDWARE_VERSION_V1_1)
	uint32_t vbus_en_pin = GPIO_PIN_12;
#else
	uint32_t vbus_en_pin = GPIO_PIN_17;
#endif
#if(defined CONFIG_RUN_BOOTLOADER && CONFIG_RUN_BOOTLOADER == 1U)
    uint32_t insert_check_pin;
    GPIO_TypeDef* vbus_en_port;
    GPIO_TypeDef* insert_check_port;
#endif 

	//vbus_en must be set high level
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    
#if(defined CONFIG_RUN_BOOTLOADER && CONFIG_RUN_BOOTLOADER == 1U)
    if(SYN_GET_USB_IF_USE_ENABLE)
    {
        vbus_en_pin = SYN_GET_USB_CTRL_ENABLE_PIN;
        vbus_en_port = SYN_GET_USB_CTRL_ENABLE_PORT;

        GPIO_InitStruct.Pin = vbus_en_pin;
    	HAL_GPIO_Init(vbus_en_port, &GPIO_InitStruct);
    	HAL_GPIO_WritePin(vbus_en_port, vbus_en_pin, GPIO_PIN_SET);
    }
    

    if(SYN_GET_USB_IF_USE_INSERT_CHECK)
    {
        insert_check_pin = SYN_GET_USB_CTRL_INSERT_CHECK_PIN;
        insert_check_port = SYN_GET_USB_CTRL_INSERT_CHECK_PORT;
        GPIO_InitStruct.Pin = insert_check_pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(insert_check_port, &GPIO_InitStruct);
    }
    
#else
	GPIO_InitStruct.Pin = vbus_en_pin;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOB, vbus_en_pin, GPIO_PIN_SET);
#endif  
	
    __HAL_RCC_USB_CLK_ENABLE();

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(OTG_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, OTG_IRQHandler);
#else
    csi_clic_register_irq(OTG_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, OTG_IRQHandler);
#endif
}

struct usbh_msc *msc_dev_probe(uint32_t timeOutCnt)
{
    struct usbh_msc *msc_class = NULL;
    char devName[9] = "/dev/sda";
    uint32_t count = 0;
    
    while(1)
    {
        msc_class = (struct usbh_msc *)usbh_find_class_instance(devName);
        if (msc_class != NULL)
        {
            return msc_class;
        }
        
        devName[7] += 1;
        if(devName[7] > 'z')  //registered msc device is /dev/sda.../dev/sdz
        {
            devName[7] = 'a';
            count += 1;
            vTaskDelay(20 / portTICK_RATE_MS);
        }
        
        if((timeOutCnt > 0) && (count >= timeOutCnt))
        {
            return NULL;
        }
    }
}

#if USE_USB_TEST_TASK
void usb_test(void* pvParameters)
{
    FATFS fs;
    FIL fp;
    UINT bw;
    FRESULT ret = FR_OK;
    const char *testStr = "This is testing string...\n";
    uint8_t buf[100];
    uint32_t sizeOfFile = 0;
	
	vTaskDelay((1*1000) / portTICK_RATE_MS);  //this delay for debug, in release code, please delete this row 
	
	usbh_initialize();
    
    //find available msc device
    struct usbh_msc *udisk = msc_dev_probe(0);
	if(NULL == udisk)
	{
		printf("can't probe u disk.\n");
		while(1)
		{
			vTaskDelay(pdMS_TO_TICKS(1000));
		}
	}
	else
	{
		printf("probe u disk!!!!.\n");
	}
    
    //file open, read and write test
    memset(buf, 0, 100);
    ret = f_mount(&fs, "2:", 1);  //in diskio.c file, DEV_USB is defined 2
    if (ret == FR_OK)
    {
		//注：test_read.txt测试文件中的字符串长度不应超过100字节
        printf("Filesystem mount success!\n");        
        ret = f_open(&fp, "2:tmp/test_read.txt", FA_READ);
        if(ret == FR_OK)
        {
            sizeOfFile = f_size(&fp);
            f_read(&fp, buf, sizeOfFile, &bw);
			printf("test_read.txt %s\n", buf);
        }
		else
		{
			printf("open file test_read.txt failed.\n");
		}
        f_close(&fp);
      
        vTaskDelay((2*1000) / portTICK_RATE_MS);
        
        ret = f_open(&fp, "2:tmp/test_write01.txt", FA_WRITE | FA_CREATE_NEW | FA_OPEN_APPEND);
        if(ret == FR_OK)
        {
            f_write(&fp, buf, strlen((char *)buf), &bw);
        }
        f_close(&fp);
        
        vTaskDelay((2*1000) / portTICK_RATE_MS);
        
        memset(buf, 0, 100);
        strcpy((char *)buf, "This is a test string.");
        ret = f_open(&fp, "2:tmp/test_write02.txt", FA_WRITE | FA_CREATE_NEW | FA_OPEN_APPEND);
        if(ret == FR_OK)
        {
            f_write(&fp, buf, strlen((char *)buf), &bw);
        }
        f_close(&fp);
    }
	
	printf("read write test success.\n");
	
	while(1)
	{
		vTaskDelay(5000 / portTICK_RATE_MS);
    }
}
#endif