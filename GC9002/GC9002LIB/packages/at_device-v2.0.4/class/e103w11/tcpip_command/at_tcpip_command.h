/*
 * File      : at_tcpip_command.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-31     huyt         first version
 */

#ifndef __AT_TCPIP_COMMAND_H__
#define __AT_TCPIP_COMMAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "at_device.h"
#include "e103w11/at_config_e103w11.h"
#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)


#define E103W11_MODULE_SEND_MAX_SIZE   2048
/* set real event by current socket and current state */
#define SET_EVENT(socket, event)       (((socket + 1) << 16) | (event))

/* AT socket event type */
#define AT_EVENT_TCPIP_CONN_OK            (1L << 0)
#define AT_EVENT_TCPIP_SEND_OK            (1L << 1)
#define AT_EVENT_TCPIP_RECV_OK            (1L << 2)
#define AT_EVNET_TCPIP_CLOSE_OK           (1L << 3)
#define AT_EVENT_TCPIP_CONN_FAIL          (1L << 4)
#define AT_EVENT_TCPIP_SEND_FAIL          (1L << 5)
#define AT_EVENT_WIFI_CONN_OK             (1L << 6)

void at_register_tcpip_commands(struct at_device *device);
int at_tcpip_event_send(struct at_device *device, uint32_t event);
int at_tcpip_event_recv(struct at_device *device, uint32_t event, uint32_t timeout, rt_uint8_t option);



#if(defined AT_DEVICE_INTERFACE_TCPIP && AT_DEVICE_INTERFACE_TCPIP == 1U)
struct s_tcpip_status
{
    /* 2： Station 已连接 AP，获得 IP 地址
     * 3： Station 已建立 TCP/UDP/SSL 连接
     * 5： Station 未连接 AP
     */
    unsigned int stat;
    unsigned int link_id;       //⽹络连接 ID (0~4)， ⽤于多连接的情况
    char type[4];               //"TCP" or "UDP"
    char remote_ip[64];         //远端 IP 地址 "192.168.101.101"
    unsigned int remote_port;   //远端端⼝值
    unsigned int local_port;    //本地端⼝值
    unsigned int tetype;        //0： 作为客户端  1：作为服务器
};
struct s_tcpip_net_info
{
    char ip[16];
    char gateway[16];
    char mask[16];
};

int at_tcpip_CIPSTATUS(struct at_device *device, struct s_tcpip_status *status);
int at_tcpip_CIPMUX_get(struct at_device *device, unsigned int *link_mux);
int at_tcpip_CIPMUX_set(struct at_device *device, unsigned int link_mux);
int at_tcpip_CIPSTART_tcp(struct at_device *device, unsigned int link_id, 
                        const char *remote_ip, unsigned int remote_port, 
                        unsigned int keep_alive_time);
int at_tcpip_CIPSTART_udp(struct at_device *device, unsigned int link_id, 
                        const char *remote_ip, unsigned int remote_port, 
                        unsigned int local_port, unsigned int mode);
int at_tcpip_CIPSTART_ssl(struct at_device *device, unsigned int link_id,
                        const char *remote_ip, unsigned int remote_port, 
                        unsigned int keep_alive_time);
int at_tcpip_CIPSEND(struct at_device *device, unsigned int link_id, 
                        const char *buff, unsigned int size, unsigned int type,
                        const char *remote_ip, unsigned int remote_port);
int at_tcpip_CIPCLOSE(struct at_device *device, unsigned int link_id);
int at_tcpip_CIPSERVER(struct at_device *device, unsigned int mode, unsigned int port, unsigned int type);
int at_tcpip_CIPMODE_get(struct at_device *device, unsigned int *mode);
int at_tcpip_CIPMODE_set(struct at_device *device, unsigned int mode);
int at_tcpip_SAVETRANSLINK_tcp(struct at_device *device, unsigned int mode, const char *remote_ip, 
                               unsigned int remote_port, unsigned int keep_alive_time);
int at_tcpip_SAVETRANSLINK_udp(struct at_device *device, unsigned int mode, const char *remote_ip, 
                               unsigned int remote_port, unsigned int local_port);
int at_tcpip_CIPRECVMODE_get(struct at_device *device, unsigned int *mode);
int at_tcpip_CIPRECVMODE_set(struct at_device *device, unsigned int mode);
int at_tcpip_CIPRECVDATA(struct at_device *device, unsigned int link_id, unsigned int len, char data[]);
int at_tcpip_PING(struct at_device *device, const char *ip);
int at_tcpip_CIPSTAMAC_get(struct at_device *device, 
                            char *mac_cur, unsigned int mac_cur_len, 
                            char *mac_def, unsigned int mac_def_len);
int at_tcpip_CIPSTAMAC_set(struct at_device *device, const char *mac, bool save_flash);
int at_tcpip_CIPSTA_get(struct at_device *device, struct s_tcpip_net_info *cur, struct s_tcpip_net_info *def);
int at_tcpip_CIPSTA_set(struct at_device *device, const struct s_tcpip_net_info *cur, bool save_flash);
int at_tcpip_CIPAP_get(struct at_device *device, struct s_tcpip_net_info *cur, struct s_tcpip_net_info *def);
int at_tcpip_CIPAP_set(struct at_device *device, const struct s_tcpip_net_info cur, bool save_flash);
#endif /* AT_DEVICE_INTERFACE_TCPIP */

#endif /* AT_DEVICE_USING_E103W11 */

#ifdef __cplusplus
}
#endif

#endif /* __AT_TCPIP_COMMAND_H__ */