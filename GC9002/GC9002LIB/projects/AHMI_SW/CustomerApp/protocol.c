/*
 * protocol.c
 *
 *  Created on: 2024年4月7日
 *      Author: TianPing Xu
 */

#include "protocol.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"

#include "com_c1.h"
#include "csi_ahmi.h"

#include "read_reg.h"

extern uint8_t info_change;

extern REG_ATTR attr_value;

DEFAULT_ATTR dafault_set_attr_on;
DEFAULT_ATTR dafault_set_attr_off;

void default_attr_init(void) {
    //默认语言英文
    attr_value.language_type_now = LANGUAGE_ENGLISH;

    dafault_set_attr_on.fan_speed_now = 60;
	dafault_set_attr_on.m_light_lum_now = 60;
    dafault_set_attr_on.s_light_lum_now = 60;
	dafault_set_attr_on.m_light_color_now = 50;
	dafault_set_attr_on.s_light_color_now = 50;
    dafault_set_attr_on.screen_off_now = 0;
}
extern uint8_t mode_onoff;
//接收数据处理
void rx_data_hadle(uint8_t* buf, uint16_t len)
{
    uint16_t crc_data = 0;
    uint8_t crc_high, crc_low, rx_offset = 2;
    //55 AA
    if((buf[0] == 0x55) && (buf[1] == 0xAA)) {
        if(len == 7) {
            crc_data = gen_crc16_ccitt_stream(buf, 5);
            crc_low = crc_data >> 8;
            crc_high = crc_data & 0xFF;
            if((buf[5] == crc_high)&&(buf[6] == crc_low)) {
                //数据处理
                if(buf[rx_offset + 0] == M_TO_T) {
                    switch (buf[rx_offset + 1]) {
                        //人感状态
                        case HUMAN_SENSE_STATE :
                            mode_onoff = 1;
                            info_change = 1;
                            attr_value.human_sense_status = buf[rx_offset + 2];
                            cmd_generate(HUMAN_SENSE_STATE, attr_value.human_sense_status);
                            if (attr_value.human_sense_status == HUMAN_SENSE_ON) {
                                /***********************************有人*************************************/
                                csi_ahmi_SetRegValue(COUNTDOWN_BLINK_REG, 1);
                                csi_ahmi_SetRegValue(COUNTDOWN_PERCENT_REG, 1000);

                                attr_value.m_light_color_now = dafault_set_attr_on.m_light_color_now;
                                attr_value.s_light_color_now = dafault_set_attr_on.s_light_color_now;
                                attr_value.m_light_lum_now = dafault_set_attr_on.m_light_lum_now;
                                attr_value.s_light_lum_now = dafault_set_attr_on.s_light_lum_now;
                                attr_value.fan_speed_now = dafault_set_attr_on.fan_speed_now;

                                csi_ahmi_SetRegValue(M_LIGHT_COLOR_REG, attr_value.m_light_color_now);
                                csi_ahmi_SetRegValue(S_LIGHT_COLOR_REG, attr_value.s_light_color_now);
                                csi_ahmi_SetRegValue(M_LIGHT_LUM_REG, attr_value.m_light_lum_now);
                                csi_ahmi_SetRegValue(S_LIGHT_LUM_REG, attr_value.s_light_lum_now);
                                csi_ahmi_SetRegValue(FAN_SPEED_REG, attr_value.fan_speed_now);

                                // csi_ahmi_SetRegValue(F_LIGHT_COLOR_REG, attr_value.f_light_color_now);
                                // csi_ahmi_SetRegValue(F_LIGHT_LUM_REG, attr_value.f_light_lum_now);

                                //主灯色温
                                cmd_generate(M_LIGHT_COLOR, attr_value.m_light_color_now);
                                //主灯亮度
                                cmd_generate(M_LIGHT_LUM, attr_value.m_light_lum_now);
                                //侧灯色温
                                cmd_generate(S_LIGHT_COLOR, attr_value.s_light_color_now);
                                //侧灯亮度
                                cmd_generate(S_LIGHT_LUM, attr_value.s_light_lum_now);
                                //风扇风速
                                cmd_generate(FAN_SPEED, attr_value.fan_speed_now);

                                // //氛围灯色温
                                // attr_value.f_light_color_now = dafault_set_attr_on.f_light_color_now;
                                // cmd_generate(ATMOSPHERE_COLOR, attr_value.f_light_color_now);
                                // //氛围灯亮度
                                // attr_value.f_light_lum_now = dafault_set_attr_on.f_light_lum_now;
                                // cmd_generate(ATMOSPHERE_LUM, attr_value.f_light_lum_now);
                            }else if (attr_value.human_sense_status == HUMAN_SENSE_OFF) {
                                /***********************************无人*************************************/
                                csi_ahmi_SetRegValue(COUNTDOWN_BLINK_REG, 0);
                                csi_ahmi_SetRegValue(COUNTDOWN_PERCENT_REG, 1000);

                                csi_ahmi_SetRegValue(M_LIGHT_COLOR_REG, 0);
                                csi_ahmi_SetRegValue(S_LIGHT_COLOR_REG, 0);
                                csi_ahmi_SetRegValue(M_LIGHT_LUM_REG, 0);
                                csi_ahmi_SetRegValue(S_LIGHT_LUM_REG, 0);
                                csi_ahmi_SetRegValue(FAN_SPEED_REG, 0);
                                // csi_ahmi_SetRegValue(F_LIGHT_COLOR_REG, attr_value.m_light_color_now);
                                // csi_ahmi_SetRegValue(F_LIGHT_LUM_REG, attr_value.m_light_lum_now);

                                //主灯色温
                                attr_value.m_light_color_now = 0;
                                cmd_generate(M_LIGHT_COLOR, attr_value.m_light_color_now);
                                //主灯亮度
                                attr_value.m_light_lum_now = 0;
                                cmd_generate(M_LIGHT_LUM, attr_value.m_light_lum_now);
                                //侧灯色温
                                attr_value.s_light_color_now = 0;
                                cmd_generate(S_LIGHT_COLOR, attr_value.s_light_color_now);
                                //侧灯亮度
                                attr_value.s_light_lum_now = 0;
                                cmd_generate(S_LIGHT_LUM, attr_value.s_light_lum_now);
                                //风扇风速
                                attr_value.fan_speed_now = 0;
                                cmd_generate(FAN_SPEED, attr_value.fan_speed_now);

                                // //氛围灯色温
                                // attr_value.m_light_color_now = 0;
                                // cmd_generate(ATMOSPHERE_COLOR, attr_value.m_light_color_now);
                                // //氛围灯亮度
                                // attr_value.m_light_lum_now = 0;
                                // cmd_generate(ATMOSPHERE_LUM, attr_value.m_light_lum_now);

                                attr_value.countdown_start  = 0;
                                attr_value.countdown_stop = 0;
                                csi_ahmi_SetRegValue(COUNTUP_TIME_REG, 0);
                                csi_ahmi_SetRegValue(COUNTDOWN_HOUR_REG, 0);
                                csi_ahmi_SetRegValue(COUNTDOWN_MINUTE_REG, 0);
                                csi_ahmi_SetRegValue(COUNTDOWN_SECOND_REG, 0);
                            }else {
                            }
                            break;
                        default :
                            break;
                    }
                }
            }
        }else if(len == 8) {
            crc_data = gen_crc16_ccitt_stream(buf, 6);
            crc_low = crc_data >> 8;
            crc_high = crc_data & 0xFF;
            if((buf[6] == crc_high)&&(buf[7] == crc_low)) {
                //数据处理
               
                if(buf[rx_offset + 0] == M_TO_T) {
                    if(buf[rx_offset + 1] == FAN_COUNTDOWN_SET) {

                    }
                } 
            }               
        }else{
        }
    }else {
    }
}

//------start---------指令发送的任务体--------------------------

k_task_handle_t CmdSendTaskHandle;

static void CMD_Send_vTask(void* pvParameters);

static void CMD_Send_TaskInit(void)
{
    int32_t xReturn;
    
    xReturn = csi_kernel_task_new(  CMD_Send_vTask,         //任务的入口地址
                                    "ahmi cmd send task",   //任务名
                                    NULL,                   //任务传入参数
                                    3,                      //任务优先级
                                    0,                      //即每个任务在不被抢占的情况下，每个可以运行的时间片时间。
                                    NULL,                   //堆栈的开始地址，如果传入NULL，则系统自动分配
                                    256,                    //堆栈大小，以字为单位
                                    &CmdSendTaskHandle);    //任务句柄
    if (xReturn != 0) {
        printf("Fail to create CmdSendTask Task.\r\n");
    }
}


#define CMD_QUEUE_LEN                               (128)
#define CMD_PROTOCOL_LEN                            (0x07)

static uint8_t *cmd_queue_in;
static uint8_t *cmd_queue_out;

static uint8_t cmd_sQueue[CMD_QUEUE_LEN];

void cmd_queue_input(uint8_t value) {
    if (1 == cmd_queue_out - cmd_queue_in || (cmd_queue_in > cmd_queue_out && (cmd_queue_in - cmd_queue_out) >= CMD_QUEUE_LEN)) {
        // queue full
    } else {
        // queue not full
        if(cmd_queue_in >= cmd_sQueue + CMD_QUEUE_LEN){
            cmd_queue_in = cmd_sQueue;
        }
        *cmd_queue_in ++ = value;
    }
}

static uint8_t cmd_queue_output(void) {
    uint8_t value = 0;
    if (cmd_queue_out != cmd_queue_in) {
        // have data in queue
        if(cmd_queue_out >= cmd_sQueue + CMD_QUEUE_LEN) {
            cmd_queue_out = cmd_sQueue;
        }
        value = *cmd_queue_out ++;
    }
    return value;
}

static uint8_t cmd_is_empty(void) {
    return cmd_queue_out != cmd_queue_in;
}

static uint8_t cmd_buff[CMD_QUEUE_LEN];

uint8_t cmd_len = 7;

static void CMD_Send_vTask(void* pvParameters)
{
	while(1) {
		static uint16_t rx_in = 0;
		uint16_t offset = 0;
		uint16_t crc_data = 0;
        while(rx_in < CMD_QUEUE_LEN && cmd_is_empty() > 0){
            cmd_buff[rx_in++] = cmd_queue_output();
        }
        while((rx_in - offset) >= CMD_PROTOCOL_LEN) {
            //帧头校验
            if((cmd_buff[offset] != HEADER_1) && (cmd_buff[offset + 1] != HEADER_2)){
                offset++;
                continue;
            }
            if(cmd_len == 7) {
                //CRC校验
                crc_data = cmd_buff[offset + 5] | cmd_buff[offset + 6] << 8;
                if(gen_crc16_ccitt_stream(cmd_buff + offset, 5) != crc_data) {
                    offset++;
                    crc_data = cmd_buff[offset + 5] | cmd_buff[offset + 6] << 8;
                    continue;
                }
                //数据处理
                COM_iUartAsynSend(cmd_buff + offset, cmd_len);
                offset += cmd_len;
            }else if(cmd_len == 8) {
                //CRC校验
                crc_data = cmd_buff[offset + 6] | cmd_buff[offset + 7] << 8;
                if(gen_crc16_ccitt_stream(cmd_buff + offset, 6) != crc_data) {
                    offset++;
                    crc_data = cmd_buff[offset + 6] | cmd_buff[offset + 7] << 8;
                    continue;
                }
                //数据处理
                COM_iUartAsynSend(cmd_buff + offset, cmd_len);
                offset += cmd_len;
            }else {
            }
            break;
        }
        rx_in -= offset;
        if(rx_in > 0) {
            xtp_memcpy(cmd_buff, cmd_buff + offset, rx_in);
        }
        //延时300ms
        csi_kernel_delay(pdMS_TO_TICKS(200)); 
    }
}

void cmd_send_init(void) {
    cmd_queue_in = cmd_sQueue;
    cmd_queue_out = cmd_sQueue;
    //任务创建
	CMD_Send_TaskInit();
}














