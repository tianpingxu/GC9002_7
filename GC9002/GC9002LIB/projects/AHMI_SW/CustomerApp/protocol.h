/*
 * protocol.h
 *
 *  Created on: 2025年4月7日
 *      Author: TianPing Xu
 */

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "board.h"
#include "crc16.h"

//包头
#define HEADER_1           0x55
#define HEADER_2           0xAA

//数据收发
#define T_TO_M             0x25
#define M_TO_T             0x26

//语言
#define LANGUAGE_ENGLISH   0x00
#define LANGUAGE_CHINESE   0x01

//功能
#define M_LIGHT_ONOFF      0x01
#define M_LIGHT_LUM        0x02
#define M_LIGHT_COLOR      0x03

#define FAN_ONOFF          0x04
#define FAN_SPEED          0x05

#define S_LIGHT_ONOFF      0x06
#define S_LIGHT_LUM        0x07
#define S_LIGHT_COLOR      0x08

#define ATMOSPHERE_ONOFF   0x09
#define ATMOSPHERE_LUM     0x10
#define ATMOSPHERE_COLOR   0x11

#define HUMAN_SENSE_STATE  0x20

#define FAN_COUNTDOWN_SET  0x21

#define HUMAN_SENSE_DELAY  0x22


//人感状态
typedef enum {
	HUMAN_SENSE_ON  = 1,		//有人
	HUMAN_SENSE_OFF = 0,       	//没人
} HUMAN_SENSE_STATSUS;		

//场景模式
typedef enum {
	SCENE_MODE_1,
	SCENE_MODE_2,
	SCENE_MODE_3,
	SCENE_MODE_4
} SCENE_MODE;

//设置默认值
typedef struct {
	uint8_t fan_speed_now;
	uint8_t m_light_lum_now;
	uint8_t m_light_color_now;
	uint8_t s_light_lum_now;
	uint8_t s_light_color_now;
	
	uint8_t fan_speed_set;
    uint8_t m_light_lum_set;
	uint8_t s_light_lum_set;
	uint8_t m_light_color_set;
	uint8_t s_light_color_set;

	uint8_t fan_speed_read;
    uint8_t m_light_lum_read;
	uint8_t s_light_lum_read;
	uint8_t m_light_color_read;
	uint8_t s_light_color_read;

	uint8_t screen_off_now;
	uint8_t screen_off_set;
	uint8_t screen_off_read;

} DEFAULT_ATTR;

extern DEFAULT_ATTR dafault_set_attr_on;

void default_attr_init(void);

void rx_data_hadle(uint8_t* buf, uint16_t len);

void cmd_queue_input(uint8_t value);

#endif //PROTOCOL_H
