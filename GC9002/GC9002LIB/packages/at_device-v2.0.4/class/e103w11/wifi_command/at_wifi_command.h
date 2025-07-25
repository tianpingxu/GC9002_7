/*
 * File      : at_wifi_command.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-31     huyt         first version
 */

#ifndef __AT_WIFI_COMMAND_H__
#define __AT_WIFI_COMMAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "at_device.h"
#include "e103w11/at_config_e103w11.h"
#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)


void at_register_wifi_commands(struct at_device *device);

#if(defined AT_DEVICE_INTERFACE_WIFI && AT_DEVICE_INTERFACE_WIFI == 1U)
struct at_wifi_CWDHCPS
{
    unsigned int time;  //SoftAP 的 DHCP 服务器是否开启， 0 关闭， 1 开启（当前配置）
    char start_ip[AT_WIFI_IP_LENGTH];  //起始 IP 地址（当前配置）
    char end_ip[AT_WIFI_IP_LENGTH];    //结束 IP 地址（当前配置）
};
struct at_wifi_CWJAP
{
    char ssid[AT_WIFI_SSID_LENGTH];		//SSID-无线网络名称
    char mac_addr[AT_WIFI_MAC_ADDR_LENGTH];	//AP 的 MAC 地址
    unsigned int ch;	//信道
    int sig_inte;		//信号强度
};
struct at_wifi_CWSAP
{
    char ssid[AT_WIFI_SSID_LENGTH];  //无线网络名称
    char passwd[AT_WIFI_PASSWORD_LENGTH];//无线网络密码
    unsigned int ch;//信道
    /* AUTH_OPEN=0
     * AUTH_WEP=1
     * AUTH_WPA_PSK=2
     * AUTH_WPA2_PSK=3
     * AUTH_WPA_WPA2_PSK=4
     */
    unsigned int auth_mode;//加密模式
    unsigned int max_con;//最大连接数
    unsigned int hidden_ssid;//隐藏 ssid
};
struct at_wifi_CWLAP
{
    /* AUTH_OPEN=0
     * AUTH_WEP=1
     * AUTH_WPA_PSK=2
     * AUTH_WPA2_PSK=3
     * AUTH_WPA_WPA2_PSK=4
     */
    unsigned int auth_mode;//加密模式
    char ssid[AT_WIFI_SSID_LENGTH];  //无线网络名称
    int signal_strength;//信号强度
    char mac_addr[AT_WIFI_MAC_ADDR_LENGTH];	//AP 的 MAC 地址
    unsigned int ch;//信道
    int freq_offset;//频偏值
    /* CIPHER_NONE = 0
     * CIPHER_WEP40 = 1
     * CIPHER_WEP104 = 2
     * CIPHER_TKIP = 3
     * CIPHER_CCMP = 4
     */
    unsigned int encry_algo;//加密算法
};
int at_wifi_CWMODE_get(struct at_device *device, unsigned int *mode_cur, unsigned int *mode_def);
int at_wifi_CWMODE_set(struct at_device *device, unsigned int mode, bool save_flash);
int at_wifi_CWJAP_get(struct at_device *device, struct at_wifi_CWJAP *cur, struct at_wifi_CWJAP *def);
int at_wifi_CWJAP_set(struct at_device *device, const char *ssid, const char *passwd, bool save_flash);
int at_wifi_CWLAP_get(struct at_device *device, struct at_wifi_CWLAP cwlap[], unsigned int max_num);
int at_wifi_CWLAP_set(struct at_device *device, const char *ssid, const char *bssid);
int at_wifi_CWQAP(struct at_device *device);

int at_wifi_CWSAP_get(struct at_device *device, struct at_wifi_CWSAP *s_cwsap);
int at_wifi_CWSAP_set(struct at_device *device, const struct at_wifi_CWSAP *s_cwsap, bool save_flash);
int at_wifi_CWDHCP_get(struct at_device *device, 
                  		unsigned int *SoftAP_status_cur, unsigned int *Station_status_cur, 
                        unsigned int *SoftAP_status_def, unsigned int *Station_status_def);
int at_wifi_CWDHCP_set(struct at_device *device, unsigned int interface, unsigned int flag, bool save_flash);
int at_wifi_CWDHCPS_get(struct at_device *device, struct at_wifi_CWDHCPS *cur, struct at_wifi_CWDHCPS *def);
int at_wifi_CWDHCPS_set(struct at_device *device, 
						unsigned int enable, 
						unsigned int lease_time, 
						const char *start_ip, 
						const char *end_ip, 
						bool save_flash);
int at_wifi_CWAUTOCONN(struct at_device *device, bool enable);
#endif /* AT_DEVICE_INTERFACE_WIFI */

#endif /* AT_DEVICE_USING_E103W11 */

#ifdef __cplusplus
}
#endif

#endif /* __AT_WIFI_COMMAND_H__ */