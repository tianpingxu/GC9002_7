/*
 * read_reg.h
 *
 *  Created on: 2025年4月8日
 *      Author: TianPing Xu
 */
 
 #ifndef __READ_REG_H
 #define __READ_REG_H
 
 #include "board.h" 
 
//寄存器地址
#define CURRENT_PAGE_REG       	2

#define SENCE_MEET_MODE_REG    	1001

#define SENCE_PHONE_MODE_REG   	1002

#define SENCE_WORK_MODE_REG    	1003

#define SENCE_RELAX_MODE_REG   	1004

#define DOCK_HOME_REG        	1005

#define DOCK_CLOCK_REG     		1006

#define DOCK_LIGHT_REG        	1007

#define DOCK_FAN_REG        	1008

#define DOCK_SANITATION_REG     1009

#define DOCK_ION_REG         	1010

#define DOCK_CURTAIN_REG      	1011

#define DOCK_SET_REG      		1012

#define M_LIGHT_LUM_REG        	1013

#define M_LIGHT_COLOR_REG      	1014

#define S_LIGHT_LUM_REG        	1015

#define S_LIGHT_COLOR_REG      	1016

#define FAN_SPEED_REG          	1017

#define UP_WINDOW_REG          	1018

#define LANGUAGE_TYPE_REG      	1019

#define F_LIGHT_LUM_REG        	1020

#define F_LIGHT_COLOR_REG      	1021

#define COUNTUP_TIME_REG    	5

#define COUNTDOWN_TIME_REG    	5
#define COUNTDOWN_BLINK_REG    	1056
#define COUNTDOWN_PERCENT_REG  	1000

#define COUNTDOWN_HOUR_REG     	1023
#define COUNTDOWN_MINUTE_REG   	1024
#define COUNTDOWN_SECOND_REG   	1025

#define COUNTDOWN_START_REG    	1026
#define COUNTDOWN_STOP_REG     	1027
#define COUNTDOWN_END_REG      	1028

#define BTN_BACK_REG      		1029

#define KEYBOARD_IN_REG         1022
#define KEYBOARD_O_REG         	1034
#define KEYBOARD_1_REG         	1035
#define KEYBOARD_2_REG         	1036
#define KEYBOARD_3_REG         	1037
#define KEYBOARD_4_REG         	1038
#define KEYBOARD_5_REG         	1039
#define KEYBOARD_6_REG         	1040
#define KEYBOARD_7_REG         	1041
#define KEYBOARD_8_REG         	1042
#define KEYBOARD_9_REG         	1043
#define KEYBOARD_DEL_REG       	1044
#define KEYBOARD_BACK_REG       1059
#define PASSWORD_VERIFY_REG    	1057

#define SET_DEFAULT_REG      	1045
#define DEF_M_LUM_REG      		1058
#define DEF_M_COLOR_REG    		1059
#define DEF_S_LUM_REG      		1060
#define DEF_S_COLOR_REG    		1061
#define DEF_F_LUM_REG      		1061
#define DEF_F_COLOR_REG    		1062
#define DEF_FAN_SPEED_REG      	1062

#define SET_LANGUAGE_REG        1046
#define LANGUAGE_ENGLISH_REG   	1050
#define LANGUAGE_CHINESE_REG   	1051

#define SET_DEV_INFO_REG        1047

#define SET_DEV_RODAR_REG       1048
#define RODAR_TIME_REG       	1064

#define SET_AUTO_VENT_REG       1049
#define AUTO_VENT_ONOFF_REG   	1055
#define INTERVAL_TIME_REG      	1065
#define DURATION_TIME_REG      	1066

#define WINDOW_SET_SAVE		 	1054
#define PARAME_SET_SAVE_YES_REG	1052
#define PARAME_SET_SAVE_NO_REG	1053

#define SET_SCREEN_OFF_REG     	1063

#define FAN_LOGO_REG     		1067


//灯亮度  四档
typedef struct {
	//当前值
	uint8_t fan_speed_now; 
    uint8_t m_light_lum_now;
	uint8_t m_light_color_now;
	uint8_t s_light_lum_now;
	uint8_t s_light_color_now;
	uint8_t f_light_lum_now;
	uint8_t f_light_color_now;

	uint8_t language_type_now;
	uint8_t dev_rodar_time_now;
	uint8_t auto_onoff_now;
	uint8_t Interval_time_now;
	uint8_t Duration_time_now;

	uint8_t language_type_set;
	uint8_t dev_rodar_time_set;
	uint8_t auto_onoff_set;
	uint8_t Interval_time_set;
	uint8_t Duration_time_set;

	uint8_t screen_onoff;

	uint8_t first_power;

	//读取的寄存器保存值
	uint8_t fan_speed_read;
    uint8_t m_light_lum_read;
	uint8_t m_light_color_read;
    uint8_t s_light_lum_read;
	uint8_t s_light_color_read;   
	uint8_t f_light_lum_read;
	uint8_t f_light_color_read;

	uint8_t dev_fan_speed_read;
    uint8_t dev_m_light_lum_read;
	uint8_t dev_m_light_color_read;

	uint8_t dev_rodar_time_read;
	uint8_t Interval_time_read;
	uint8_t Duration_time_read;
		
	//人感
	bool human_sense_status;

	bool sence_mode_meet_onoff;
	bool sence_mode_phone_onoff;
	bool sence_mode_work_onoff;
	bool sence_mode_relax_onoff;

	//风扇定时倒计时
	uint16_t fan_countdown_hour;
	uint16_t fan_countdown_minute;

	//使用时间
	uint16_t count_hour_user;
	uint16_t count_minute_user;
	uint16_t count_second_user;

	//倒计时
	uint16_t countdown_hour;
	uint16_t countdown_minute;
	uint16_t countdown_second;

	uint16_t countdown_hour_set;
	uint16_t countdown_minute_set;
	uint16_t countdown_second_set;

	uint8_t countdown_start;

	uint8_t countdown_stop;

	uint16_t countdown_percent_now;
	uint16_t countdown_percent_old;

	uint32_t countdown_time_now;
	uint32_t countdown_time_all;

	uint32_t countdown_time;

	uint32_t countdown_time_read;

} REG_ATTR;

extern REG_ATTR attr_value;

//当前页面
typedef enum {
	WINDOW_OFF,
	WINDOW_LIGHT,
 	WINDOW_FAN,

} WINDOW;
 
//当前页面
typedef enum {
    PAGE_HOME = 1,
	PAGE_LIGHT,
	PAGE_FAN,
	PAGE_CLOC_SET,
	PAGE_COUNTDOWN,
	PAGE_PASSWORD,
	PAGE_SET_DEF,
	PAGE_SET_LANGUAGE,
	PAGE_SET_DEV_INFO,
	PAGE_SET_DEV_RODAR,
	PAGE_SET_VENTI_OFF,
	PAGE_SET_VENTI_ON,
} CURRENT_PAGE;

void dock_home_handle(void);
void dock_clock_handle(void);
void dock_light_handle(void);
void dock_fan_handle(void);
void dock_sanitation_handle(void);
void dock_ion_handle(void);
void dock_curtain_handle(void);
void dock_set_handle(void);
void up_window_handle(void);

void countdown_start(void);

void countdown_stop(void);

void countdown_end(void);

void sence_mode_meet(void);

void sence_mode_phone(void);

void sence_mode_work(void);

void sence_mode_relax(void);

void set_back_handle(void);

void password_back_handle(void);

void password_handle(uint8_t no);

void set_default_handle(void);
void set_language_handle(void);
void set_dev_info_handle(void);
void set_dev_rodar_handle(void);
void set_auto_vent_handle(void);

void parame_change_save(uint8_t or);

void language_choose_handle(uint8_t type);

void auto_onoff_handle(void);

void screen_onoff_handle(void);
 
void cmd_generate(uint8_t fun, uint8_t data);

void cmd_generate_rodar(uint16_t time);

void cmd_generate_auto_vent(uint8_t time1, uint16_t time2);

 #endif