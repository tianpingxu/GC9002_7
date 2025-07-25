#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "csi_irq.h"
#include "csi_hal_driver_flash.h"


#include "protocol.h"
#include "read_reg.h"

/*创建任务宏定义参数*/
k_task_handle_t g_uwTask1;	
#define TASK_STK_SIZE 1024
#define TASK_PRIOR_TASK1 3
#define TIME_QUANTA 100

#define FLASH_ADDR_FOR_USER			0x080F9000
#define FLASH_ADDR_FOR_USER_SIZE	(4096)

static uint8_t flash_erase_for_user(uint32_t addr, size_t size)
{
	//判断传参（自行补充）
	
	csi_hal_internal_flash_take(0);
	
	csi_hal_internal_flash_ClearWriteProtect();
	for(int i = 0; i < size/4096; i++)
	{
		csi_hal_interal_flash_sector_erase(addr + i*4096);
	}
	csi_hal_internal_flash_SetWriteProtect();
	
	csi_hal_internal_flash_give(0);
	
	return 0;
}

static uint8_t flash_write_for_user(uint32_t addr, uint8_t *buf, size_t size)
{
	//判断传参（自行补充）
	
	csi_hal_internal_flash_take(0);
	
	csi_hal_internal_flash_ClearWriteProtect();
	csi_hal_internal_flash_write(addr,size, buf);
	csi_hal_internal_flash_SetWriteProtect();
	
	csi_hal_internal_flash_give(0);
	
	return 0;
}

static uint8_t flash_read_for_user(uint32_t addr, uint8_t *buf, size_t size)
{
	//判断传参（自行补充）
	
	csi_hal_internal_flash_take(0);
	
	csi_hal_internal_flash_read(addr,size, buf);

	csi_hal_internal_flash_give(0);
	
	return 0;
}

void deft_info_reset(void);
void deft_parame_info_save(void);
void deft_parame_info_read(void);

void deft_info_reset(void) {
    dafault_set_attr_on.fan_speed_now = 60;
	dafault_set_attr_on.m_light_lum_now = 60;
    dafault_set_attr_on.s_light_lum_now = 60;
	dafault_set_attr_on.m_light_color_now = 50;
	dafault_set_attr_on.s_light_color_now = 50;
    dafault_set_attr_on.screen_off_now = 0;

	attr_value.language_type_now = 0;
	attr_value.dev_rodar_time_now = 8;
	attr_value.auto_onoff_now = 1;
	attr_value.Interval_time_now = 0;
	attr_value.Duration_time_now = 2;
	attr_value.first_power = 18;

	flash_erase_for_user(FLASH_ADDR_FOR_USER, FLASH_ADDR_FOR_USER_SIZE);
	deft_parame_info_save();
}

void deft_parame_info_save(void) {
	uint8_t arr_flash_save[1024] = {0};
	arr_flash_save[0] = dafault_set_attr_on.fan_speed_now;
	arr_flash_save[1] = dafault_set_attr_on.m_light_lum_now;
	arr_flash_save[2] = dafault_set_attr_on.s_light_lum_now;
	arr_flash_save[3] = dafault_set_attr_on.m_light_color_now;
	arr_flash_save[4] = dafault_set_attr_on.s_light_color_now;
	arr_flash_save[5] = dafault_set_attr_on.screen_off_now;

	arr_flash_save[6] = attr_value.language_type_now;
	arr_flash_save[7] = attr_value.dev_rodar_time_now;
	arr_flash_save[8] = attr_value.auto_onoff_now;
	arr_flash_save[9] = attr_value.Interval_time_now;
	arr_flash_save[10] = attr_value.Duration_time_now;

	attr_value.first_power = 18;
	arr_flash_save[11] = attr_value.first_power;

	flash_erase_for_user(FLASH_ADDR_FOR_USER, FLASH_ADDR_FOR_USER_SIZE);
	flash_write_for_user(FLASH_ADDR_FOR_USER, arr_flash_save, sizeof(arr_flash_save));
}

void deft_parame_info_read(void) {
	uint8_t arr_flash_read[1024] = {0};
	flash_read_for_user(FLASH_ADDR_FOR_USER,  arr_flash_read, sizeof(arr_flash_read));

	dafault_set_attr_on.fan_speed_now = arr_flash_read[0];
	dafault_set_attr_on.m_light_lum_now = arr_flash_read[1];
	dafault_set_attr_on.s_light_lum_now = arr_flash_read[2];
	dafault_set_attr_on.m_light_color_now = arr_flash_read[3];
	dafault_set_attr_on.s_light_color_now = arr_flash_read[4];
	dafault_set_attr_on.screen_off_now = arr_flash_read[5];

	attr_value.language_type_now = arr_flash_read[6];
	attr_value.dev_rodar_time_now = arr_flash_read[7];
	attr_value.auto_onoff_now = arr_flash_read[8];
	attr_value.Interval_time_now = arr_flash_read[9];
	attr_value.Duration_time_now = arr_flash_read[10];
	attr_value.first_power = arr_flash_read[11];

	printf("fan_speed_now = %d\r\n", dafault_set_attr_on.fan_speed_now);
	printf("m_light_lum_now = %d\r\n", dafault_set_attr_on.m_light_lum_now);
	printf("s_light_lum_now = %d\r\n", dafault_set_attr_on.s_light_lum_now);
	printf("m_light_color_now = %d\r\n", dafault_set_attr_on.m_light_color_now);
	printf("s_light_color_now = %d\r\n", dafault_set_attr_on.s_light_color_now);
	printf("screen_off_now = %d\r\n", dafault_set_attr_on.screen_off_now);
	printf("language_type_now = %d\r\n", attr_value.language_type_now);
	printf("dev_rodar_time_now = %d\r\n", attr_value.dev_rodar_time_now);
	printf("auto_onoff_now = %d\r\n", attr_value.auto_onoff_now);
	printf("Interval_time_now = %d\r\n", attr_value.Interval_time_now);
	printf("Duration_time_now = %d\r\n", attr_value.Duration_time_now);
	printf("dev_rodar_time_now = %d\r\n", attr_value.dev_rodar_time_now);

	// printf("first_power = %d\r\n", attr_value.first_power);
}

static void task1(void *p)
{
	uint8_t arr_flash[1024] = {0};

	memset(arr_flash, 20, sizeof(arr_flash));
	
	while(1)
	{	
		flash_erase_for_user(FLASH_ADDR_FOR_USER, FLASH_ADDR_FOR_USER_SIZE);
		flash_write_for_user(FLASH_ADDR_FOR_USER, arr_flash, sizeof(arr_flash));
		
		csi_kernel_delay(pdMS_TO_TICKS(1000));
	}	
}

int test_flash(void) /*函数名可更改，不与 main 重名即可*/
{
	csi_kernel_task_new((k_task_entry_t)task1, "task1", NULL, TASK_PRIOR_TASK1, TIME_QUANTA, NULL, TASK_STK_SIZE, &g_uwTask1);
	if (g_uwTask1 == NULL) 
	{
		csi_kernel_sched_resume(0);
		printf("Fail to create task1! \r\n");
	}
	
	return 0;
}





