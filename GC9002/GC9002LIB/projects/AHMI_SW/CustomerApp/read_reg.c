/*
 * read_reg.c
 *
 *  Created on: 2025年4月8日
 *      Author: TianPing Xu
 */

#include "read_reg.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "csi_ahmi.h"
#include "protocol.h"
#include "AHMITouch.h"
#include "csi_rtc.h"

#include "core_rv32.h"
#include "csi_irq.h"
#include "AHMIInterface.h"
#include "gc90xx_hal_rtc.h"
#include "gc90xx_hal.h"

extern TouchFSM g_TM_stTouchFsm;

extern uint8_t cmd_len;

uint8_t password[4];

uint8_t password_len = 0;

char password_null[] = "\0";
char password_deft[] = "2013";
char password_read[] = "\0";

uint8_t current_page = 0;

uint8_t user_cnt_10ms = 0;

uint8_t countdown_cnt_160ms = 0;

k_task_handle_t ReadregTaskHandle;

static void Read_Reg_vTask(void *pvParameters);

static void READ_Reg_TaskInit(void)
{
    int32_t xReturn;

    xReturn = csi_kernel_task_new(	Read_Reg_vTask,        //任务的入口地址
									"ahmi read reg task",  //任务名
									NULL,                  //任务传入参数
									2,           		   //任务优先级
									0,                     //即每个任务在不被抢占的情况下，每个可以运行的时间片时间。
									NULL,                  //堆栈的开始地址，如果传入NULL，则系统自动分配
									256 * 2, 			   //堆栈大小，以字为单位
									&ReadregTaskHandle);   //任务句柄
    if (xReturn != 0) {
        printf("Fail to create CmdSendTask Task.\r\n");
    }
}


RTC_TimeTypeDef user_time, count_time, user_time_base, count_time_base;

static RTC_HandleTypeDef handle_rtc = {0};
static RTC_DateTypeDef RTC_DateStructure = {0};
static RTC_TimeTypeDef RTC_TimeStructure = {0};

static void RTC_Init(void)
{
	handle_rtc.Instance = RTC;
	HAL_RTC_Struct_Init(&handle_rtc);
	
	RTC_DateStructure.Year = 1999;  
	RTC_DateStructure.Month = 9;
	RTC_DateStructure.Day = 8;

  	RTC_TimeStructure.Hour = 6;
	RTC_TimeStructure.Minute = 30;
  	RTC_TimeStructure.Second = 00;
	
	HAL_RTC_Init(&handle_rtc);
	
	// 将日期和时间设置到RTC模块
	HAL_RTC_SetDate(&handle_rtc, &RTC_DateStructure, RTC_FORMAT_BIN);
	HAL_RTC_SetTime(&handle_rtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
}

static void rtc_poll(void)
{
	 // 获取RTC的日期和时间
    HAL_RTC_GetDate(&handle_rtc, &RTC_DateStructure, RTC_FORMAT_BCD);	//获取格式按BCD码
    HAL_RTC_GetTime(&handle_rtc, &RTC_TimeStructure, RTC_FORMAT_BCD);
	
	  // 打印RTC的日期和时间
	printf("RTC Date: %02d/%02d/%02d\n", bcd_to_decimal_code(RTC_DateStructure.Year),
											bcd_to_decimal_code(RTC_DateStructure.Month), 
												bcd_to_decimal_code(RTC_DateStructure.Day));
	printf("RTC Time: %02d:%02d:%02d\n", bcd_to_decimal_code(RTC_TimeStructure.Hour),
											bcd_to_decimal_code(RTC_TimeStructure.Minute),
												bcd_to_decimal_code(RTC_TimeStructure.Second));
}

uint8_t first_in = 0;
uint8_t blink_flag = 0;
uint8_t mode_onoff = 0;
uint8_t param_change = 0;
uint8_t cnt_parame = 0;
uint8_t info_change = 0;

uint8_t first_flag = 0;
uint8_t def_change = 0;
uint8_t cnt_first = 0;

uint8_t next_page = 0;
uint8_t def_info_change_flag = 0;
uint8_t language_change_flag = 0;
uint8_t rodar_change_flag = 0;
uint8_t auto_change_flag = 0;

void read_page_init(void) {
	RTC_Init();
	READ_Reg_TaskInit();
}

REG_ATTR attr_value;
extern void deft_info_reset(void);
extern void deft_parame_info_read(void);
extern void deft_parame_info_save(void);

//------start---------读取寄存器值的任务体--------------------------
static void Read_Reg_vTask(void *pvParameters) 
{
	while(1) {
		//读页面
		current_page = csi_ahmi_GetRegValue(CURRENT_PAGE_REG);
		
		if(!first_in) {
			first_in++;
			first_flag = 1;
			mode_onoff = 1;
			deft_parame_info_read();
			if(attr_value.first_power != 18) {
				deft_info_reset();
			}
			csi_ahmi_SetRegValue(DEF_FAN_SPEED_REG, dafault_set_attr_on.fan_speed_now);
			csi_ahmi_SetRegValue(DEF_M_LUM_REG, dafault_set_attr_on.m_light_lum_now);
			csi_ahmi_SetRegValue(DEF_S_LUM_REG, dafault_set_attr_on.s_light_lum_now);
			csi_ahmi_SetRegValue(DEF_M_COLOR_REG, dafault_set_attr_on.m_light_color_now);
			csi_ahmi_SetRegValue(DEF_S_COLOR_REG, dafault_set_attr_on.s_light_color_now);
			if(attr_value.language_type_now == LANGUAGE_ENGLISH) {
				csi_ahmi_SetRegValue(LANGUAGE_ENGLISH_REG, 1);
				csi_ahmi_SetRegValue(LANGUAGE_CHINESE_REG, 0);
			}else if(attr_value.language_type_now == LANGUAGE_CHINESE) {
				csi_ahmi_SetRegValue(LANGUAGE_ENGLISH_REG, 0);
				csi_ahmi_SetRegValue(LANGUAGE_CHINESE_REG, 1);
			}
			csi_ahmi_SetRegValue(RODAR_TIME_REG, attr_value.dev_rodar_time_now);
			csi_ahmi_SetRegValue(AUTO_VENT_ONOFF_REG, attr_value.auto_onoff_now);
			csi_ahmi_SetRegValue(INTERVAL_TIME_REG, attr_value.Interval_time_now);
			csi_ahmi_SetRegValue(DURATION_TIME_REG, attr_value.Duration_time_now);

			csi_ahmi_SetRegValue(LANGUAGE_TYPE_REG, attr_value.language_type_now);
			csi_ahmi_SetRegValue(FAN_SPEED_REG, dafault_set_attr_on.fan_speed_now);
			csi_ahmi_SetRegValue(M_LIGHT_LUM_REG, dafault_set_attr_on.m_light_lum_now);
			csi_ahmi_SetRegValue(S_LIGHT_LUM_REG, dafault_set_attr_on.s_light_lum_now);
			csi_ahmi_SetRegValue(M_LIGHT_COLOR_REG, dafault_set_attr_on.m_light_color_now);
			csi_ahmi_SetRegValue(S_LIGHT_COLOR_REG, dafault_set_attr_on.s_light_color_now);

			csi_ahmi_SetRegValue(DOCK_HOME_REG, 1);
			csi_ahmi_SetRegValue(COUNTDOWN_PERCENT_REG, 1000);

			if(attr_value.dev_rodar_time_now == 0) {
				cmd_generate_rodar(30);
			}else {
				cmd_generate_rodar(attr_value.dev_rodar_time_now * 60);
			}
			if(attr_value.auto_onoff_now) {
				cmd_generate_auto_vent(attr_value.Interval_time_now + 1, attr_value.Duration_time_now + 1);
			}else {
				cmd_generate_auto_vent(0, 0);
			}
			
			dafault_set_attr_on.fan_speed_set = dafault_set_attr_on.fan_speed_now;
			dafault_set_attr_on.m_light_lum_set = dafault_set_attr_on.m_light_lum_now;
			dafault_set_attr_on.s_light_lum_set = dafault_set_attr_on.s_light_lum_now;
			dafault_set_attr_on.m_light_color_set = dafault_set_attr_on.m_light_color_now;
			dafault_set_attr_on.s_light_color_set = dafault_set_attr_on.s_light_color_now;

			attr_value.fan_speed_now = dafault_set_attr_on.fan_speed_now;
			attr_value.m_light_lum_now = dafault_set_attr_on.m_light_lum_now;
			attr_value.s_light_lum_now = dafault_set_attr_on.s_light_lum_now;
			attr_value.m_light_color_now = dafault_set_attr_on.m_light_color_now;
			attr_value.s_light_color_now = dafault_set_attr_on.s_light_color_now;
			
			attr_value.language_type_set = attr_value.language_type_now;
			attr_value.dev_rodar_time_set = attr_value.dev_rodar_time_now;
			attr_value.auto_onoff_set = attr_value.auto_onoff_now;
			attr_value.Interval_time_set = attr_value.Interval_time_now;
			attr_value.Duration_time_set = attr_value.Duration_time_now;
		}

		if(first_flag){
			cnt_first++;
			if(cnt_first >= 20) {
				cnt_first = 0;
				first_flag = 0;
				def_change = 1;
			}
		}

		if(mode_onoff) {
			cnt_parame++;
			if(cnt_parame >= 10) {
				cnt_parame = 0;
				mode_onoff = 0;
				param_change = 1;
			}
		}

		if(csi_ahmi_GetRegValue(FAN_SPEED_REG)) {
			csi_ahmi_SetRegValue(FAN_LOGO_REG, 1);
		}else {
			csi_ahmi_SetRegValue(FAN_LOGO_REG, 0);
		}

		if(g_TM_stTouchFsm.mPosi.event == NOTOUCH) {
			//读主灯色温
			if(!mode_onoff) {
				attr_value.m_light_color_read = csi_ahmi_GetRegValue(M_LIGHT_COLOR_REG);
				if (attr_value.m_light_color_now != attr_value.m_light_color_read) {
					attr_value.m_light_color_now = attr_value.m_light_color_read;
					cmd_generate(M_LIGHT_COLOR, attr_value.m_light_color_now);

					//模式参数修改
					if(param_change) 
						info_change = 1;
				}
				//读主灯亮度
				attr_value.m_light_lum_read = csi_ahmi_GetRegValue(M_LIGHT_LUM_REG);
				if (attr_value.m_light_lum_now != attr_value.m_light_lum_read) {
					attr_value.m_light_lum_now = attr_value.m_light_lum_read;
					cmd_generate(M_LIGHT_LUM, attr_value.m_light_lum_now);

					//模式参数修改
					if(param_change) 
						info_change = 1;
				}
				//读侧灯色温
				attr_value.s_light_color_read = csi_ahmi_GetRegValue(S_LIGHT_COLOR_REG);
				if (attr_value.s_light_color_now != attr_value.s_light_color_read) {
					attr_value.s_light_color_now = attr_value.s_light_color_read;
					cmd_generate(S_LIGHT_COLOR, attr_value.s_light_color_now);

					//模式参数修改
					if(param_change) 
						info_change = 1;
				}
				//读侧灯亮度
				attr_value.s_light_lum_read = csi_ahmi_GetRegValue(S_LIGHT_LUM_REG);
				if (attr_value.s_light_lum_now != attr_value.s_light_lum_read) {
					attr_value.s_light_lum_now = attr_value.s_light_lum_read;
					cmd_generate(S_LIGHT_LUM, attr_value.s_light_lum_now);

					//模式参数修改
					if(param_change) 
						info_change = 1;
				}
				// //读氛围灯色温
				// attr_value.f_light_color_read = csi_ahmi_GetRegValue(F_LIGHT_COLOR_REG);
				// if (attr_value.f_light_color_now != attr_value.f_light_color_read) {
				// 	attr_value.f_light_color_now = attr_value.f_light_color_read;
				// 	cmd_generate(ATMOSPHERE_COLOR, attr_value.f_light_color_now);

				// 	//模式参数修改
				// 	if(param_change) 
				// 		info_change = 1;
				// }
				// //读氛围灯亮度
				// attr_value.f_light_lum_read = csi_ahmi_GetRegValue(F_LIGHT_LUM_REG);
				// if (attr_value.f_light_lum_now != attr_value.f_light_lum_read) {
				// 	attr_value.f_light_lum_now = attr_value.f_light_lum_read;
				// 	cmd_generate(ATMOSPHERE_LUM, attr_value.f_light_lum_now);

				// 	//模式参数修改
				// 	if(param_change) 
				// 		info_change = 1;
				// }
				//读风扇风速
				attr_value.fan_speed_read = csi_ahmi_GetRegValue(FAN_SPEED_REG);
				if (attr_value.fan_speed_now != attr_value.fan_speed_read) {
					attr_value.fan_speed_now = attr_value.fan_speed_read;
					cmd_generate(FAN_SPEED, attr_value.fan_speed_now);

					//模式参数修改
					if(param_change) 
						info_change = 1;
				}
			}

			if(info_change) {
				info_change = 0;
				param_change = 0;

				attr_value.sence_mode_meet_onoff = 0;
				attr_value.sence_mode_work_onoff = 0;
				attr_value.sence_mode_phone_onoff = 0;
				attr_value.sence_mode_relax_onoff = 0;
				
				csi_ahmi_SetRegValue(SENCE_MEET_MODE_REG, attr_value.sence_mode_meet_onoff);
				csi_ahmi_SetRegValue(SENCE_WORK_MODE_REG, attr_value.sence_mode_work_onoff);
				csi_ahmi_SetRegValue(SENCE_PHONE_MODE_REG, attr_value.sence_mode_phone_onoff);
				csi_ahmi_SetRegValue(SENCE_RELAX_MODE_REG, attr_value.sence_mode_relax_onoff);
			}

			if(def_change) {
				//读主灯默认色温
				dafault_set_attr_on.m_light_color_read = csi_ahmi_GetRegValue(DEF_M_COLOR_REG);
				if (dafault_set_attr_on.m_light_color_set != dafault_set_attr_on.m_light_color_read) {
					dafault_set_attr_on.m_light_color_set = dafault_set_attr_on.m_light_color_read;
					def_info_change_flag = 1;
				}
				//读主灯默认亮度
				dafault_set_attr_on.m_light_lum_read = csi_ahmi_GetRegValue(DEF_M_LUM_REG);
				if (dafault_set_attr_on.m_light_lum_set != dafault_set_attr_on.m_light_lum_read) {
					dafault_set_attr_on.m_light_lum_set = dafault_set_attr_on.m_light_lum_read;
					def_info_change_flag = 1;
				}
				//读侧灯默认色温
				dafault_set_attr_on.s_light_color_read = csi_ahmi_GetRegValue(DEF_S_COLOR_REG);
				if (dafault_set_attr_on.s_light_color_set != dafault_set_attr_on.s_light_color_read) {
					dafault_set_attr_on.s_light_color_set = dafault_set_attr_on.s_light_color_read;
					def_info_change_flag = 1;
				}
				//读侧灯默认亮度
				dafault_set_attr_on.s_light_lum_read = csi_ahmi_GetRegValue(DEF_S_LUM_REG);
				if (dafault_set_attr_on.s_light_lum_set != dafault_set_attr_on.s_light_lum_read) {
					dafault_set_attr_on.s_light_lum_set = dafault_set_attr_on.s_light_lum_read;
					def_info_change_flag = 1;
				}
				// //读氛围灯默认色温
				// dafault_set_attr_on.f_light_color_read = csi_ahmi_GetRegValue(DEF_F_COLOR_REG);
				// if (dafault_set_attr_on.f_light_color_set != dafault_set_attr_on.f_light_color_read) {
				// 	dafault_set_attr_on.f_light_color_set = dafault_set_attr_on.f_light_color_read;
				// 	def_info_change_flag = 1;
				// }
				// //读氛围灯默认亮度
				// dafault_set_attr_on.f_light_lum_read = csi_ahmi_GetRegValue(DEF_F_LUM_REG);
				// if (dafault_set_attr_on.f_light_lum_set != dafault_set_attr_on.f_light_lum_read) {
				// 	dafault_set_attr_on.f_light_lum_set = dafault_set_attr_on.f_light_lum_read;
				// 	def_info_change_flag = 1;
				// }
				//读风扇默认风速
				dafault_set_attr_on.fan_speed_read = csi_ahmi_GetRegValue(DEF_FAN_SPEED_REG);
				if (dafault_set_attr_on.fan_speed_set != dafault_set_attr_on.fan_speed_read) {
					dafault_set_attr_on.fan_speed_set = dafault_set_attr_on.fan_speed_read;
					def_info_change_flag = 1;
				}
				//读人感检测延迟时间
				attr_value.dev_rodar_time_read = csi_ahmi_GetRegValue(RODAR_TIME_REG);
				if (attr_value.dev_rodar_time_set != attr_value.dev_rodar_time_read) {
					attr_value.dev_rodar_time_set = attr_value.dev_rodar_time_read;
					rodar_change_flag = 1;
				}
				//读自动通风时间
				if(attr_value.auto_onoff_now || attr_value.auto_onoff_set) {
					attr_value.Interval_time_read = csi_ahmi_GetRegValue(INTERVAL_TIME_REG);
					attr_value.Duration_time_read = csi_ahmi_GetRegValue(DURATION_TIME_REG);
					if ((attr_value.Interval_time_set != attr_value.Interval_time_read) || (attr_value.Duration_time_set != attr_value.Duration_time_read)) {
						attr_value.Interval_time_set = attr_value.Interval_time_read;
						attr_value.Duration_time_set = attr_value.Duration_time_read;
						auto_change_flag = 1;
					}
				}
			}

			if((def_info_change_flag) || (language_change_flag) ||(rodar_change_flag) || (auto_change_flag)) {
				csi_ahmi_SetRegValue(PARAME_SET_SAVE_YES_REG, 1);
			}
		}

		// 有人 --- 使用时间开始计时
		if(attr_value.human_sense_status) {
			HAL_RTC_GetTime(&handle_rtc, &user_time, RTC_FORMAT_BCD);
			if(user_time.Second != user_time_base.Second) {
				user_time_base.Second = user_time.Second;
				attr_value.count_second_user++;
				if(attr_value.count_second_user >= 60) {
					attr_value.count_second_user = 0;
					attr_value.count_minute_user++;
					if(attr_value.count_minute_user >= 60) {
						attr_value.count_minute_user = 0;
						attr_value.count_hour_user++;
					}
				}
				// 使用时间同步
				if(!attr_value.countdown_start) {
					csi_ahmi_SetRegValue(COUNTUP_TIME_REG, (((attr_value.count_hour_user / 10) * 0x10 + attr_value.count_hour_user % 10) * 0x10000 +
															   ((attr_value.count_minute_user / 10) * 0x10 + attr_value.count_minute_user % 10) * 0x100 +
															   ((attr_value.count_second_user / 10) * 0x10 + attr_value.count_second_user % 10)));
				}
			}
		}else {
			attr_value.count_hour_user = 0;
			attr_value.count_minute_user = 0;
			attr_value.count_second_user = 0;
			HAL_RTC_GetTime(&handle_rtc, &user_time_base, RTC_FORMAT_BCD);
		}

		if(current_page == PAGE_CLOC_SET) {
			// 读倒计时
			if(!attr_value.countdown_start) {
				attr_value.countdown_hour = csi_ahmi_GetRegValue(COUNTDOWN_HOUR_REG);
				attr_value.countdown_minute = csi_ahmi_GetRegValue(COUNTDOWN_MINUTE_REG);
				attr_value.countdown_second = csi_ahmi_GetRegValue(COUNTDOWN_SECOND_REG);
				if(attr_value.countdown_hour || attr_value.countdown_minute || attr_value.countdown_second) {
					attr_value.countdown_time_read = 1;
					csi_ahmi_SetRegValue(COUNTDOWN_START_REG, 1);
				}else {
					attr_value.countdown_time_read = 0;
					csi_ahmi_SetRegValue(COUNTDOWN_START_REG, 0);
				}
			}
		}
		// countdown run
		if(attr_value.countdown_start) {
			if(attr_value.countdown_stop) {
				HAL_RTC_GetTime(&handle_rtc, &count_time, RTC_FORMAT_BCD);
				count_time_base.Second = count_time.Second;
			}else {
				HAL_RTC_GetTime(&handle_rtc, &count_time, RTC_FORMAT_BCD);
				if(count_time.Second != count_time_base.Second) {
					count_time_base.Second = count_time.Second;
					if((attr_value.countdown_hour > 0) || (attr_value.countdown_minute > 0) || (attr_value.countdown_second > 0)) {
						if(attr_value.countdown_second == 0) {
							if(attr_value.countdown_minute > 0) {
								attr_value.countdown_minute--;
								attr_value.countdown_second = 59;
							}else {
								if(attr_value.countdown_hour > 0) {
									attr_value.countdown_hour--;
									attr_value.countdown_minute = 59;
									attr_value.countdown_second = 59;
								}else {
									attr_value.countdown_start = 0;
								}
							}
						}else {
							attr_value.countdown_second--;
						}

						// 倒计时剩余时长
						attr_value.countdown_time_now = attr_value.countdown_hour * 3600 + attr_value.countdown_minute * 60 + attr_value.countdown_second;
						// 时间同步
						csi_ahmi_SetRegValue(COUNTDOWN_TIME_REG, (((attr_value.countdown_hour / 10) * 0x10 + attr_value.countdown_hour % 10) * 0x10000 +
																	((attr_value.countdown_minute / 10) * 0x10 + attr_value.countdown_minute % 10) * 0x100 +
																	((attr_value.countdown_second / 10) * 0x10 + attr_value.countdown_second % 10)));
						
						// 倒计时剩余时间百分比
						attr_value.countdown_percent_now = (attr_value.countdown_time_now * 1000) / attr_value.countdown_time_all;
						if(attr_value.countdown_percent_now != attr_value.countdown_percent_old) {
							attr_value.countdown_percent_old = attr_value.countdown_percent_now;
							// 百分比同步
							csi_ahmi_SetRegValue(COUNTDOWN_PERCENT_REG, attr_value.countdown_percent_now);
						}
						printf("percent = %d\r\n", attr_value.countdown_percent_now);
					}else {
						attr_value.countdown_start = 0;

						csi_ahmi_SetRegValue(COUNTDOWN_HOUR_REG, 0);
						csi_ahmi_SetRegValue(COUNTDOWN_MINUTE_REG, 0);
						csi_ahmi_SetRegValue(COUNTDOWN_SECOND_REG, 0);
						csi_ahmi_SetRegValue(COUNTDOWN_PERCENT_REG, 1000);

						if(current_page == PAGE_COUNTDOWN) 
							csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_CLOC_SET);

						if(attr_value.human_sense_status) {
							csi_ahmi_SetRegValue(COUNTDOWN_BLINK_REG, 1);
							csi_ahmi_SetRegValue(COUNTUP_TIME_REG, (((attr_value.count_hour_user / 10) * 0x10 + attr_value.count_hour_user % 10) * 0x10000 +
																	((attr_value.count_minute_user / 10) * 0x10 + attr_value.count_minute_user % 10) * 0x100 +
																	((attr_value.count_second_user / 10) * 0x10 + attr_value.count_second_user % 10)));
						}else {
							csi_ahmi_SetRegValue(COUNTDOWN_BLINK_REG, 0);
						}
					}
				}
				// 闪烁
				if((attr_value.countdown_hour == 0) && (attr_value.countdown_minute == 0) && attr_value.countdown_second) {
					if(attr_value.countdown_second <= 5) {
						blink_flag = !blink_flag;
						if(blink_flag) {
							csi_ahmi_SetRegValue(COUNTDOWN_BLINK_REG, 2);
						}else {
							csi_ahmi_SetRegValue(COUNTDOWN_BLINK_REG, 1);
						}
					}
				}else {
					if(attr_value.human_sense_status) {
						csi_ahmi_SetRegValue(COUNTDOWN_BLINK_REG, 1);
					}else {
						csi_ahmi_SetRegValue(COUNTDOWN_BLINK_REG, 0);
					}
					csi_ahmi_SetRegValue(COUNTDOWN_PERCENT_REG, 1000);
				}
			}
		}else {
		}
		//延时10ms
        csi_kernel_delay(pdMS_TO_TICKS(150)); 
    }
}
/*******************************************************DOCK栏处理*****************************************************/
void dock_home_handle(void) {
	if((def_info_change_flag) || (language_change_flag) ||(rodar_change_flag) || (auto_change_flag)) {
		csi_ahmi_SetRegValue(WINDOW_SET_SAVE, 1);
		next_page = PAGE_HOME;
	}else {
		csi_ahmi_SetRegValue(DOCK_HOME_REG, 1);
		csi_ahmi_SetRegValue(DOCK_CLOCK_REG, 0);
		csi_ahmi_SetRegValue(DOCK_LIGHT_REG, 0);
		csi_ahmi_SetRegValue(DOCK_FAN_REG, 0);
		csi_ahmi_SetRegValue(DOCK_SET_REG, 0);
		if(current_page != PAGE_HOME)
			csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_HOME);
	}
}
void dock_clock_handle(void) {
	csi_ahmi_SetRegValue(UP_WINDOW_REG, WINDOW_OFF);
	if((def_info_change_flag) || (language_change_flag) ||(rodar_change_flag) || (auto_change_flag)) {
		csi_ahmi_SetRegValue(WINDOW_SET_SAVE, 1);
		if(!attr_value.countdown_start) {
			next_page = PAGE_CLOC_SET;
		}else {
			next_page = PAGE_COUNTDOWN;
		}
	}else {
		csi_ahmi_SetRegValue(DOCK_HOME_REG, 0);
		csi_ahmi_SetRegValue(DOCK_CLOCK_REG, 1);
		csi_ahmi_SetRegValue(DOCK_LIGHT_REG, 0);
		csi_ahmi_SetRegValue(DOCK_FAN_REG, 0);
		csi_ahmi_SetRegValue(DOCK_SET_REG, 0);
		if(!attr_value.countdown_start) {
			csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_CLOC_SET);
		}else {
			csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_COUNTDOWN);
		}
	}
}
uint8_t dock_light_flag = 0;
void dock_light_handle(void) {
	if((def_info_change_flag) || (language_change_flag) ||(rodar_change_flag) || (auto_change_flag)) {
		csi_ahmi_SetRegValue(WINDOW_SET_SAVE, 1);
		next_page = PAGE_LIGHT;
		dock_light_flag = 1;
	}else {
		if(current_page != PAGE_LIGHT)
			csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_LIGHT);

		csi_ahmi_SetRegValue(DOCK_HOME_REG, 0);
		csi_ahmi_SetRegValue(DOCK_CLOCK_REG, 0);
		csi_ahmi_SetRegValue(DOCK_LIGHT_REG, 1);
		csi_ahmi_SetRegValue(DOCK_FAN_REG, 0);
		csi_ahmi_SetRegValue(DOCK_SET_REG, 0);
	}
}
uint8_t dock_fan_flag = 0;
void dock_fan_handle(void) {
	if((def_info_change_flag) || (language_change_flag) ||(rodar_change_flag) || (auto_change_flag)) {
		csi_ahmi_SetRegValue(WINDOW_SET_SAVE, 1);
		next_page = PAGE_HOME;
		dock_fan_flag = 1;
	}else {
		if(current_page != PAGE_FAN)
			csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_FAN);

		csi_ahmi_SetRegValue(DOCK_HOME_REG, 0);
		csi_ahmi_SetRegValue(DOCK_CLOCK_REG, 0);
		csi_ahmi_SetRegValue(DOCK_LIGHT_REG, 0);
		csi_ahmi_SetRegValue(DOCK_FAN_REG, 1);
		csi_ahmi_SetRegValue(DOCK_SET_REG, 0);
	}
}
void dock_sanitation_handle(void) {

}
void dock_ion_handle(void) {

}
void dock_curtain_handle(void) {

}
void dock_set_handle(void) {
	csi_ahmi_SetRegValue(UP_WINDOW_REG, WINDOW_OFF);
	
	if(current_page == PAGE_HOME || current_page == PAGE_CLOC_SET || current_page == PAGE_COUNTDOWN) {
		csi_ahmi_SetRegValue(DOCK_HOME_REG, 0);
		csi_ahmi_SetRegValue(DOCK_CLOCK_REG, 0);
		csi_ahmi_SetRegValue(DOCK_LIGHT_REG, 0);
		csi_ahmi_SetRegValue(DOCK_FAN_REG, 0);
		csi_ahmi_SetRegValue(DOCK_SET_REG, 1);
		csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_PASSWORD);
		if(attr_value.language_type_now == LANGUAGE_ENGLISH) {
			csi_ahmi_SetRegValue(PASSWORD_VERIFY_REG, 0);
		}else if(attr_value.language_type_now == LANGUAGE_CHINESE) {
			csi_ahmi_SetRegValue(PASSWORD_VERIFY_REG, 2);
		}else{
		}
	}else {
		// csi_ahmi_SetRegValue(DOCK_SET_REG, 0);
		// csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_HOME);
		// csi_ahmi_SetRegValue(DOCK_HOME_REG, 1);
	}
	// else if((current_page == PAGE_SET_DEF) || (current_page == PAGE_SET_LANGUAGE) || (current_page == PAGE_SET_DEV_INFO) || 
 	// 		 (current_page == PAGE_SET_DEV_RODAR) || (current_page == PAGE_SET_VENTI_OFF) || (current_page == PAGE_SET_VENTI_ON)){
	// 	csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_HOME);
	// }
}

void up_window_handle(void) {
	csi_ahmi_SetRegValue(DOCK_LIGHT_REG, 0);
	csi_ahmi_SetRegValue(DOCK_FAN_REG, 0);
	csi_ahmi_SetRegValue(UP_WINDOW_REG, WINDOW_OFF);

	csi_ahmi_SetRegValue(DOCK_HOME_REG, 1);
}
/*******************************************************定时处理*****************************************************/
// 倒计时开始
void countdown_start(void) {
	if(attr_value.countdown_time_read) {
		attr_value.countdown_start  = 1;
		HAL_RTC_GetTime(&handle_rtc, &count_time_base, RTC_FORMAT_BCD);

		csi_ahmi_SetRegValue(COUNTDOWN_TIME_REG, (((attr_value.countdown_hour / 10) * 0x10 + attr_value.countdown_hour % 10) * 0x10000 +
												((attr_value.countdown_minute / 10) * 0x10 + attr_value.countdown_minute % 10) * 0x100 +
												((attr_value.countdown_second / 10) * 0x10 + attr_value.countdown_second % 10)));

		attr_value.countdown_hour_set = attr_value.countdown_hour;
		attr_value.countdown_minute_set = attr_value.countdown_minute;
		attr_value.countdown_second_set = attr_value.countdown_second;
		
		attr_value.countdown_time_all = attr_value.countdown_hour * 3600 + attr_value.countdown_minute * 60 + attr_value.countdown_second;		
		attr_value.countdown_time_now = attr_value.countdown_time_all;

		attr_value.countdown_percent_now = 1000;
		attr_value.countdown_percent_old = 1000;

		csi_ahmi_SetRegValue(COUNTDOWN_BLINK_REG, 1);

		csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_HOME);
		csi_ahmi_SetRegValue(DOCK_CLOCK_REG, 0);
		csi_ahmi_SetRegValue(DOCK_HOME_REG, 1);
	}
}
// 倒计时暂停
void countdown_stop(void) {
	attr_value.countdown_stop  = !attr_value.countdown_stop;
	csi_ahmi_SetRegValue(COUNTDOWN_STOP_REG, attr_value.countdown_stop);

	csi_ahmi_SetRegValue(COUNTDOWN_BLINK_REG, 0);
}
// 倒计时停止
void countdown_end(void) {
	attr_value.countdown_start  = 0;

	attr_value.countdown_stop = 0;
	csi_ahmi_SetRegValue(COUNTDOWN_STOP_REG, attr_value.countdown_stop);

	attr_value.countdown_time_now = 0;
	attr_value.countdown_time_all = 0;
	attr_value.countdown_percent_now = 0;
	attr_value.countdown_percent_old = 0;

	csi_ahmi_SetRegValue(COUNTDOWN_TIME_REG, 0);
	csi_ahmi_SetRegValue(COUNTDOWN_PERCENT_REG, 1000);

	csi_ahmi_SetRegValue(COUNTDOWN_HOUR_REG, attr_value.countdown_hour_set);
	csi_ahmi_SetRegValue(COUNTDOWN_MINUTE_REG, attr_value.countdown_minute_set);
	csi_ahmi_SetRegValue(COUNTDOWN_SECOND_REG, attr_value.countdown_second_set);

	csi_ahmi_SetRegValue(COUNTDOWN_BLINK_REG, 0);
	csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_CLOC_SET);
}
/*******************************************************模式选择*****************************************************/
// 场景模式-会议
void sence_mode_meet(void) {
	if(attr_value.sence_mode_meet_onoff) return;

	attr_value.m_light_color_now = 75;
	attr_value.m_light_lum_now = 100;
	attr_value.s_light_color_now = 75;
	attr_value.s_light_lum_now = 80;
	attr_value.fan_speed_now = 100;

	cmd_generate(M_LIGHT_COLOR, attr_value.m_light_color_now);
	cmd_generate(M_LIGHT_LUM, attr_value.m_light_lum_now);
	cmd_generate(S_LIGHT_COLOR, attr_value.s_light_color_now);
	cmd_generate(S_LIGHT_LUM, attr_value.s_light_lum_now);
	cmd_generate(FAN_SPEED, attr_value.fan_speed_now);
	
	csi_ahmi_SetRegValue(M_LIGHT_COLOR_REG, attr_value.m_light_color_now);
	csi_ahmi_SetRegValue(M_LIGHT_LUM_REG, attr_value.m_light_lum_now);
	csi_ahmi_SetRegValue(S_LIGHT_COLOR_REG, attr_value.s_light_color_now);
	csi_ahmi_SetRegValue(S_LIGHT_LUM_REG, attr_value.s_light_lum_now);
	csi_ahmi_SetRegValue(FAN_SPEED_REG, attr_value.fan_speed_now);

	//开启会议模式
	mode_onoff = 1;
	attr_value.sence_mode_meet_onoff = 1;
	csi_ahmi_SetRegValue(SENCE_MEET_MODE_REG, attr_value.sence_mode_meet_onoff);
	printf("sence_mode_meet\r\n");
	//关闭其他模式
	attr_value.sence_mode_work_onoff = 0;
	attr_value.sence_mode_phone_onoff = 0;
	attr_value.sence_mode_relax_onoff = 0;
	csi_ahmi_SetRegValue(SENCE_WORK_MODE_REG, attr_value.sence_mode_work_onoff);
	csi_ahmi_SetRegValue(SENCE_PHONE_MODE_REG, attr_value.sence_mode_phone_onoff);
	csi_ahmi_SetRegValue(SENCE_RELAX_MODE_REG, attr_value.sence_mode_relax_onoff);

}
// 场景模式-电话
void sence_mode_phone(void) {
	if(attr_value.sence_mode_phone_onoff) return;

	attr_value.m_light_color_now = 50;
	attr_value.m_light_lum_now = 30;
	attr_value.s_light_color_now = 75;
	attr_value.s_light_lum_now = 30;
	attr_value.fan_speed_now = 75;
	
	csi_ahmi_SetRegValue(M_LIGHT_COLOR_REG, attr_value.m_light_color_now);
	csi_ahmi_SetRegValue(M_LIGHT_LUM_REG, attr_value.m_light_lum_now);
	csi_ahmi_SetRegValue(S_LIGHT_COLOR_REG, attr_value.s_light_color_now);
	csi_ahmi_SetRegValue(S_LIGHT_LUM_REG, attr_value.s_light_lum_now);
	csi_ahmi_SetRegValue(FAN_SPEED_REG, attr_value.fan_speed_now);

	cmd_generate(M_LIGHT_COLOR, attr_value.m_light_color_now);
	cmd_generate(M_LIGHT_LUM, attr_value.m_light_lum_now);
	cmd_generate(S_LIGHT_COLOR, attr_value.s_light_color_now);
	cmd_generate(S_LIGHT_LUM, attr_value.s_light_lum_now);
	cmd_generate(FAN_SPEED, attr_value.fan_speed_now);

	//开启电话模式
	mode_onoff = 1;
	attr_value.sence_mode_phone_onoff = 1;
	csi_ahmi_SetRegValue(SENCE_PHONE_MODE_REG, attr_value.sence_mode_phone_onoff);
	printf("sence_mode_phone\r\n");
	//关闭其他模式
	attr_value.sence_mode_meet_onoff = 0;
	attr_value.sence_mode_work_onoff = 0;
	attr_value.sence_mode_relax_onoff = 0;
	csi_ahmi_SetRegValue(SENCE_MEET_MODE_REG, attr_value.sence_mode_meet_onoff);
	csi_ahmi_SetRegValue(SENCE_WORK_MODE_REG, attr_value.sence_mode_work_onoff);
	csi_ahmi_SetRegValue(SENCE_RELAX_MODE_REG, attr_value.sence_mode_relax_onoff);

}
// 场景模式-工作
void sence_mode_work(void) {
	if(attr_value.sence_mode_work_onoff) return;

	attr_value.m_light_color_now = 75;
	attr_value.m_light_lum_now = 70;
	attr_value.s_light_color_now = 75;
	attr_value.s_light_lum_now = 60;
	attr_value.fan_speed_now = 75;
	
	csi_ahmi_SetRegValue(M_LIGHT_COLOR_REG, attr_value.m_light_color_now);
	csi_ahmi_SetRegValue(M_LIGHT_LUM_REG, attr_value.m_light_lum_now);
	csi_ahmi_SetRegValue(S_LIGHT_COLOR_REG, attr_value.s_light_color_now);
	csi_ahmi_SetRegValue(S_LIGHT_LUM_REG, attr_value.s_light_lum_now);
	csi_ahmi_SetRegValue(FAN_SPEED_REG, attr_value.fan_speed_now);

	cmd_generate(M_LIGHT_COLOR, attr_value.m_light_color_now);
	cmd_generate(M_LIGHT_LUM, attr_value.m_light_lum_now);
	cmd_generate(S_LIGHT_COLOR, attr_value.s_light_color_now);
	cmd_generate(S_LIGHT_LUM, attr_value.s_light_lum_now);
	cmd_generate(FAN_SPEED, attr_value.fan_speed_now);

	//开启工作模式
	mode_onoff = 1;
	attr_value.sence_mode_work_onoff = 1;
	csi_ahmi_SetRegValue(SENCE_WORK_MODE_REG, attr_value.sence_mode_work_onoff);
	printf("sence_mode_work\r\n");
	//关闭其他模式
	attr_value.sence_mode_meet_onoff = 0;
	attr_value.sence_mode_phone_onoff = 0;
	attr_value.sence_mode_relax_onoff = 0;
	csi_ahmi_SetRegValue(SENCE_MEET_MODE_REG, attr_value.sence_mode_meet_onoff);
	csi_ahmi_SetRegValue(SENCE_PHONE_MODE_REG, attr_value.sence_mode_phone_onoff);
	csi_ahmi_SetRegValue(SENCE_RELAX_MODE_REG, attr_value.sence_mode_relax_onoff);
}
// 场景模式-放松
void sence_mode_relax(void) {
	if(attr_value.sence_mode_relax_onoff) return;

	attr_value.m_light_color_now = 25;
	attr_value.m_light_lum_now = 50;
	attr_value.s_light_lum_now = 0;
	attr_value.fan_speed_now = 60;
	
	csi_ahmi_SetRegValue(M_LIGHT_COLOR_REG, attr_value.m_light_color_now);
	csi_ahmi_SetRegValue(M_LIGHT_LUM_REG, attr_value.m_light_lum_now);
	csi_ahmi_SetRegValue(S_LIGHT_LUM_REG, attr_value.s_light_lum_now);
	csi_ahmi_SetRegValue(FAN_SPEED_REG, attr_value.fan_speed_now);

	cmd_generate(M_LIGHT_COLOR, attr_value.m_light_color_now);
	cmd_generate(M_LIGHT_LUM, attr_value.m_light_lum_now);
	cmd_generate(S_LIGHT_LUM, attr_value.s_light_lum_now);
	cmd_generate(FAN_SPEED, attr_value.fan_speed_now);

	//开启放松模式
	mode_onoff = 1;
	attr_value.sence_mode_relax_onoff = 1;
	csi_ahmi_SetRegValue(SENCE_RELAX_MODE_REG, attr_value.sence_mode_relax_onoff);
	printf("sence_mode_relax\r\n");
	//关闭其他模式
	attr_value.sence_mode_meet_onoff = 0;
	attr_value.sence_mode_work_onoff = 0;
	attr_value.sence_mode_phone_onoff = 0;
	csi_ahmi_SetRegValue(SENCE_MEET_MODE_REG, attr_value.sence_mode_meet_onoff);
	csi_ahmi_SetRegValue(SENCE_WORK_MODE_REG, attr_value.sence_mode_work_onoff);
	csi_ahmi_SetRegValue(SENCE_PHONE_MODE_REG, attr_value.sence_mode_phone_onoff);
}
/*******************************************************密码处理*****************************************************/
// 密码验证
void password_handle(uint8_t no) {
	if(no != 10) {
		password_len++;
		password[password_len - 1] = no;
	}else{
		if(password_len > 0) {
			password_len--;
		}
	}
	if(password_len == 1) {
		if(attr_value.language_type_now == LANGUAGE_ENGLISH) {
			csi_ahmi_SetRegValue(PASSWORD_VERIFY_REG, 0);
		}else if(attr_value.language_type_now == LANGUAGE_CHINESE) {
			csi_ahmi_SetRegValue(PASSWORD_VERIFY_REG, 2);
		}
	}
	
	if(password_len == 4) {
		if(password[0] == 2 && password[1] == 0 && password[2] == 1 && password[3] == 3) {
			csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_SET_DEF);
			if(attr_value.language_type_now == LANGUAGE_ENGLISH) {
				csi_ahmi_SetRegValue(PASSWORD_VERIFY_REG, 0);
			}else if(attr_value.language_type_now == LANGUAGE_CHINESE) {
				csi_ahmi_SetRegValue(PASSWORD_VERIFY_REG, 2);
			}
		}else {
			if(attr_value.language_type_now == LANGUAGE_ENGLISH) {
				csi_ahmi_SetRegValue(PASSWORD_VERIFY_REG, 1);
			}else if(attr_value.language_type_now == LANGUAGE_CHINESE) {
				csi_ahmi_SetRegValue(PASSWORD_VERIFY_REG, 3);
			}
		}
		password_len = 0;
		memset(password, 0, sizeof(password));
	}
	if(password_len > 4) {
		password_len = 0;
		memset(password, 0, sizeof(password));
		if(attr_value.language_type_now == LANGUAGE_ENGLISH) {
			csi_ahmi_SetRegValue(PASSWORD_VERIFY_REG, 1);
		}else if(attr_value.language_type_now == LANGUAGE_CHINESE) {
			csi_ahmi_SetRegValue(PASSWORD_VERIFY_REG, 3);
		}
	}
	csi_ahmi_SetRegValue(KEYBOARD_IN_REG, password_len);
}
// 退出密码界面
void password_back_handle(void) {
	password_len = 0;
	memset(password, 0, sizeof(password));
	csi_ahmi_SetRegValue(KEYBOARD_IN_REG, 0);
	csi_ahmi_SetRegValue(PASSWORD_VERIFY_REG, 0);
}
/*******************************************************设置操作*****************************************************/
// 设置返回
void set_back_handle(void) {
	password_len = 0;
	memset(password, 0, sizeof(password));
	csi_ahmi_SetRegValue(KEYBOARD_IN_REG, 0);
	csi_ahmi_SetRegValue(PASSWORD_VERIFY_REG, 0);

	csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_HOME);

	csi_ahmi_SetRegValue(DOCK_HOME_REG, 1);
	csi_ahmi_SetRegValue(DOCK_CLOCK_REG, 0);
	csi_ahmi_SetRegValue(DOCK_LIGHT_REG, 0);
	csi_ahmi_SetRegValue(DOCK_FAN_REG, 0);
	csi_ahmi_SetRegValue(DOCK_SET_REG, 0);

	csi_ahmi_SetRegValue(COUNTDOWN_HOUR_REG, 0);
	csi_ahmi_SetRegValue(COUNTDOWN_MINUTE_REG, 0);
	csi_ahmi_SetRegValue(COUNTDOWN_SECOND_REG, 0);
}
// 设置默认参数
void set_default_handle(void) {
	if((language_change_flag) || (rodar_change_flag) || (auto_change_flag)) {
		csi_ahmi_SetRegValue(WINDOW_SET_SAVE, 1);
		next_page = PAGE_SET_DEF;
	}else {
		if(current_page != PAGE_SET_DEF) {
			csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_SET_DEF);
		}
	}
}
// 设置语言
void set_language_handle(void) {
	if((def_info_change_flag) ||(rodar_change_flag) || (auto_change_flag)) {
		csi_ahmi_SetRegValue(WINDOW_SET_SAVE, 1);
		next_page = PAGE_SET_LANGUAGE;
	}else {
		if(current_page != PAGE_SET_LANGUAGE) {
			csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_SET_LANGUAGE);
		}
	}
}
// 语言选择
void language_choose_handle(uint8_t type) {
	if(attr_value.language_type_set == type) return;
	language_change_flag = 1;
	if(type == LANGUAGE_ENGLISH) {
		attr_value.language_type_set = LANGUAGE_ENGLISH;
		csi_ahmi_SetRegValue(LANGUAGE_ENGLISH_REG, 1);
		csi_ahmi_SetRegValue(LANGUAGE_CHINESE_REG, 0);
	}else if(type == LANGUAGE_CHINESE) {
		attr_value.language_type_set = LANGUAGE_CHINESE;
		csi_ahmi_SetRegValue(LANGUAGE_ENGLISH_REG, 0);
		csi_ahmi_SetRegValue(LANGUAGE_CHINESE_REG, 1);
	}
	csi_ahmi_SetRegValue(LANGUAGE_TYPE_REG, attr_value.language_type_set);
}
// 设置设备信息
void set_dev_info_handle(void) {
	if((def_info_change_flag) || (language_change_flag) ||(rodar_change_flag) || (auto_change_flag)) {
		csi_ahmi_SetRegValue(WINDOW_SET_SAVE, 1);
		next_page = PAGE_SET_DEV_INFO;
	}else {
		if(current_page != PAGE_SET_DEV_INFO) {
			csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_SET_DEV_INFO);
		}
	}
}
// 设置人感检测
void set_dev_rodar_handle(void) {
	if((def_info_change_flag) || (language_change_flag) || (auto_change_flag)) {
		csi_ahmi_SetRegValue(WINDOW_SET_SAVE, 1);
		next_page = PAGE_SET_DEV_RODAR;
	}else {
		if(current_page != PAGE_SET_DEV_RODAR) {
			csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_SET_DEV_RODAR);
		}
	}
}
// 设置自动通风
void set_auto_vent_handle(void) {
	if((def_info_change_flag) || (language_change_flag) ||(rodar_change_flag)) {
		csi_ahmi_SetRegValue(WINDOW_SET_SAVE, 1);
		next_page = PAGE_SET_VENTI_ON;
	}else {
		if(current_page != PAGE_SET_VENTI_ON && current_page != PAGE_SET_VENTI_OFF) {
			printf("auto_in_onoff_now = %d\r\n",attr_value.auto_onoff_now);
			if(attr_value.auto_onoff_now) {
				csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_SET_VENTI_ON);
			}else {
				csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_SET_VENTI_OFF);
			}
		}
	}
}
// 是否保存
void parame_change_save(uint8_t or) {
	if(or) {
		if(def_info_change_flag) {
			printf("def_info_change_flag\r\n");
			dafault_set_attr_on.m_light_color_now = dafault_set_attr_on.m_light_color_set;
			dafault_set_attr_on.m_light_lum_now = dafault_set_attr_on.m_light_lum_set;
			dafault_set_attr_on.s_light_color_now = dafault_set_attr_on.s_light_color_set;
			dafault_set_attr_on.s_light_lum_now = dafault_set_attr_on.s_light_lum_set;
			dafault_set_attr_on.fan_speed_now = dafault_set_attr_on.fan_speed_set;
			// dafault_set_attr_on.screen_off_now = !dafault_set_attr_on.screen_off_set;
		}else if(language_change_flag) {
			printf("language_change_flag\r\n");
			attr_value.language_type_now = attr_value.language_type_set;
			csi_ahmi_SetRegValue(LANGUAGE_TYPE_REG, attr_value.language_type_now);
		}else if(rodar_change_flag) {
			printf("rodar_change_flag\r\n");
			attr_value.dev_rodar_time_now = attr_value.dev_rodar_time_set;

			if(attr_value.dev_rodar_time_now == 0) {
				cmd_generate_rodar(30);
			}else {
				cmd_generate_rodar(attr_value.dev_rodar_time_now * 60);
			}
		}else if(auto_change_flag) {
			printf("auto_change_flag\r\n");
			attr_value.auto_onoff_now = attr_value.auto_onoff_set;
			attr_value.Interval_time_now = attr_value.Interval_time_set;
			attr_value.Duration_time_now = attr_value.Duration_time_set;
			if(attr_value.auto_onoff_now)
				cmd_generate_auto_vent(attr_value.Interval_time_now + 1, attr_value.Duration_time_now + 1);
			else
				cmd_generate_auto_vent(0, 0);
		}
		deft_parame_info_save();
	}else {
		if(def_info_change_flag) {
			csi_ahmi_SetRegValue(DEF_M_LUM_REG, dafault_set_attr_on.m_light_lum_now);
			csi_ahmi_SetRegValue(DEF_S_LUM_REG, dafault_set_attr_on.s_light_lum_now);
			csi_ahmi_SetRegValue(DEF_M_COLOR_REG, dafault_set_attr_on.m_light_color_now);
			csi_ahmi_SetRegValue(DEF_S_COLOR_REG, dafault_set_attr_on.s_light_color_now);
			csi_ahmi_SetRegValue(DEF_FAN_SPEED_REG, dafault_set_attr_on.fan_speed_now);
			// csi_ahmi_SetRegValue(SET_SCREEN_OFF_REG, dafault_set_attr_on.screen_off_now);

			dafault_set_attr_on.fan_speed_set = dafault_set_attr_on.fan_speed_now;
			dafault_set_attr_on.m_light_lum_set = dafault_set_attr_on.m_light_lum_now;
			dafault_set_attr_on.s_light_lum_set = dafault_set_attr_on.s_light_lum_now;
			dafault_set_attr_on.m_light_color_set = dafault_set_attr_on.m_light_color_now;
			dafault_set_attr_on.s_light_color_set = dafault_set_attr_on.s_light_color_now;
			// dafault_set_attr_on.screen_off_set = dafault_set_attr_on.screen_off_now;
		}else if(language_change_flag) {
			csi_ahmi_SetRegValue(LANGUAGE_TYPE_REG, attr_value.language_type_now);
			attr_value.language_type_set = attr_value.language_type_now;
		}else if(rodar_change_flag) {
			csi_ahmi_SetRegValue(RODAR_TIME_REG, attr_value.dev_rodar_time_now);
			attr_value.dev_rodar_time_set = attr_value.dev_rodar_time_now;
		}else if(auto_change_flag) {
			csi_ahmi_SetRegValue(INTERVAL_TIME_REG, attr_value.Interval_time_now);
			csi_ahmi_SetRegValue(DURATION_TIME_REG, attr_value.Duration_time_now);
			attr_value.auto_onoff_set = attr_value.auto_onoff_now;
			attr_value.Interval_time_set = attr_value.Interval_time_now;
			attr_value.Duration_time_set = attr_value.Duration_time_now;
		}
	}
	def_info_change_flag = 0;
	language_change_flag = 0;
	rodar_change_flag = 0;
	auto_change_flag = 0;

	csi_ahmi_SetRegValue(WINDOW_SET_SAVE, 0);
	csi_ahmi_SetRegValue(PARAME_SET_SAVE_YES_REG, 0);

	if(next_page)
		csi_ahmi_SetRegValue(CURRENT_PAGE_REG, next_page);

	if(next_page == PAGE_CLOC_SET || next_page == PAGE_COUNTDOWN) {
		csi_ahmi_SetRegValue(DOCK_CLOCK_REG, 1);
		csi_ahmi_SetRegValue(DOCK_LIGHT_REG, 0);
		csi_ahmi_SetRegValue(DOCK_FAN_REG, 0);
		csi_ahmi_SetRegValue(DOCK_SET_REG, 0);
	}else if(next_page == PAGE_HOME) {
		if(dock_light_flag) {
			dock_light_flag = 0;
			csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_LIGHT);
			csi_ahmi_SetRegValue(DOCK_HOME_REG, 0);
			csi_ahmi_SetRegValue(DOCK_CLOCK_REG, 0);
			csi_ahmi_SetRegValue(DOCK_LIGHT_REG, 1);
			csi_ahmi_SetRegValue(DOCK_FAN_REG, 0);
			csi_ahmi_SetRegValue(DOCK_SET_REG, 0);
		}else if(dock_fan_flag){
			dock_fan_flag = 0;
			csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_FAN);
			csi_ahmi_SetRegValue(DOCK_HOME_REG, 0);
			csi_ahmi_SetRegValue(DOCK_CLOCK_REG, 0);
			csi_ahmi_SetRegValue(DOCK_LIGHT_REG, 0);
			csi_ahmi_SetRegValue(DOCK_FAN_REG, 1);
			csi_ahmi_SetRegValue(DOCK_SET_REG, 0);
		}else {
		}
	}
	next_page = 0;
}
// AUTO VENT ONOFF
void auto_onoff_handle(void) {
	attr_value.auto_onoff_set = !attr_value.auto_onoff_set;
	auto_change_flag = 1;
	if(attr_value.auto_onoff_set) {
		csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_SET_VENTI_ON);
		attr_value.Interval_time_set = csi_ahmi_GetRegValue(INTERVAL_TIME_REG);
		attr_value.Duration_time_set = csi_ahmi_GetRegValue(DURATION_TIME_REG);
	}else {
		csi_ahmi_SetRegValue(CURRENT_PAGE_REG, PAGE_SET_VENTI_OFF);
		attr_value.Interval_time_set = 0;
		attr_value.Duration_time_set = 0;
	}
}
// SCREEN ONOFF
void screen_onoff_handle(void) {
	// dafault_set_attr_on.screen_off_set = !dafault_set_attr_on.screen_off_set;

	// def_info_change_flag = 1;
	// csi_ahmi_SetRegValue(SET_SCREEN_OFF_REG, dafault_set_attr_on.screen_off_set);
}

//指令生成
void cmd_generate(uint8_t fun, uint8_t data) {
	//指令数组
	cmd_len = 7;
    uint16_t crc_data = 0x00;
	uint8_t frame1[7] = {HEADER_1, HEADER_2, T_TO_M, fun, data};
    crc_data = gen_crc16_ccitt_stream(frame1, 5);
    frame1[5] = crc_data & 0xFF;
	frame1[6] = crc_data >> 8;

	for(uint8_t i = 0; i < 7; i++) {
		cmd_queue_input(frame1[i]);
	}
}

void cmd_generate_rodar(uint16_t time) {
	cmd_len = 8;
    uint16_t crc_data = 0x00;
	uint8_t data_low = 0x00, data_high = 0x00;

	data_low = time & 0x00FF;
	data_high = (time >> 8) & 0x00FF;

	uint8_t frame1[8] = {HEADER_1, HEADER_2, T_TO_M, HUMAN_SENSE_DELAY, data_low, data_high};
    crc_data = gen_crc16_ccitt_stream(frame1, 6);
    frame1[6] = crc_data & 0xFF;
	frame1[7] = crc_data >> 8;

	for(uint8_t i = 0; i < 8; i++) {
		cmd_queue_input(frame1[i]);
	}
}

void cmd_generate_auto_vent(uint8_t time1, uint16_t time2) {
	cmd_len = 8;
    uint16_t crc_data = 0x00;

	uint8_t frame1[8] = {HEADER_1, HEADER_2, T_TO_M, FAN_COUNTDOWN_SET, time1, time2};
    crc_data = gen_crc16_ccitt_stream(frame1, 6);
    frame1[6] = crc_data & 0xFF;
	frame1[7] = crc_data >> 8;

	for(uint8_t i = 0; i < 8; i++) {
		cmd_queue_input(frame1[i]);
	}
}

