/*
 * File      : at_device.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-05-08     chenyong     first version
 */

#ifndef __AT_DEVICE_H__
#define __AT_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <at.h>
#if(defined CONFIG_AT_CLIENT_ENABLE && CONFIG_AT_CLIENT_ENABLE == 1U)

#if(defined AT_USING_SOCKET && AT_USING_SOCKET == 1U)
#include <at_socket.h>
#endif

#if !defined(RT_USING_NETDEV) || (!defined(AT_SW_VERSION_NUM) || AT_SW_VERSION_NUM < 0x10300)
#error "This RT-Thread version is older, please check and updata laster RT-Thread!"
#else

#include <arpa/inet.h>
#include <netdev.h>
#endif /* RT_USING_NETDEV */


#define AT_DEVICE_SW_VERSION           "2.0.4"
#define AT_DEVICE_SW_VERSION_NUM       0x20004

#define AT_MAX_COMMAND_GROUPS			16

/* AT device class ID */
#define AT_DEVICE_CLASS_ESP8266        0x01U
#define AT_DEVICE_CLASS_M26_MC20       0x02U
#define AT_DEVICE_CLASS_EC20           0x03U
#define AT_DEVICE_CLASS_SIM800C        0x04U
#define AT_DEVICE_CLASS_SIM76XX        0x05U
#define AT_DEVICE_CLASS_RW007          0x06U
#define AT_DEVICE_CLASS_MW31           0x07U
#define AT_DEVICE_CLASS_ESP32          0x08U
#define AT_DEVICE_CLASS_W60X           0x09U
#define AT_DEVICE_CLASS_A9G            0x0AU
#define AT_DEVICE_CLASS_BC26           0x0BU
#define AT_DEVICE_CLASS_AIR720         0x0CU
#define AT_DEVICE_CLASS_ME3616         0x0DU
#define AT_DEVICE_CLASS_M6315          0x0EU
#define AT_DEVICE_CLASS_BC28           0x0FU
#define AT_DEVICE_CLASS_EC200X         0x10U
#define AT_DEVICE_CLASS_N21            0x11U
#define AT_DEVICE_CLASS_N58            0x12U
#define AT_DEVICE_CLASS_M5311          0X13U
#define AT_DEVICE_CLASS_N720           0X14U
#define AT_DEVICE_CLASS_L610           0X15U
#define AT_DEVICE_CLASS_ECR6600        0X16U
#define AT_DEVICE_CLASS_E103W11        0X17U

/* Options and Commands for AT device control opreations */
enum AT_CMD
{
    //基础命令
	AT_DEVICE_CTRL_POWER_ON = 0x01L,
	AT_DEVICE_CTRL_POWER_OFF,
	AT_DEVICE_CTRL_BASIC_AT,			//测试 AT 启动          ["AT"]
	AT_DEVICE_CTRL_BASIC_RST,			//重启模块              ["AT+RST"]
	AT_DEVICE_CTRL_BASIC_GMR,			//查询版本信息          ["AT+GMR"]
	AT_DEVICE_CTRL_BASIC_CMD,			//列出所有 AT 命令      ["AT+CMD"]
	AT_DEVICE_CTRL_BASIC_RESTORE,		//恢复出厂设置功能      ["AT+RESTORE"]
	AT_DEVICE_CTRL_BASIC_UART,			//查询 UART 配置        ["AT+UART?"]
	AT_DEVICE_CTRL_BASIC_SYSIOSETCFG,	//设置 GPIO 口工作模式  ["AT+SYSIOSETCFG"]
	AT_DEVICE_CTRL_BASIC_SYSIOGETCFG,	//查询 GPIO 口工作模式  ["AT+SYSIOGETCFG"]
	AT_DEVICE_CTRL_BASIC_SYSGPIODIR,	//设置 GPIO 口方向      ["AT+SYSGPIODIR"]
	AT_DEVICE_CTRL_BASIC_SYSGPIOWRITE,	//设置 GPIO 输出电平    ["AT+SYSGPIOWRITE"]
	AT_DEVICE_CTRL_BASIC_SYSGPIOREAD,	//读取 GPIO 状态        ["AT+SYSGPIOREAD"]
	AT_DEVICE_CTRL_BASIC_ATE,			//关闭/打开回显功能		["ATE"]
	AT_DEVICE_CTRL_BASIC_SYSRAM,		//查询系统堆大小        ["AT+SYSRAM"]
	AT_DEVICE_CTRL_BASIC_SYSTEMP,		//查询芯片内部温度      ["AT+SYSTEMP"]
	AT_DEVICE_CTRL_BASIC_SYSFLASH,		//读/写/擦 Flash 分区   ["AT+SYSFLASH"]
    //OTA
	AT_DEVICE_CTRL_OTA_CIUPDATE,  	//OTA 升级              ["AT+CIUPDATE"]
	AT_DEVICE_CTRL_OTA_CICHANGE,	//系统切换				["AT+CICHANGE"]
    //wifi 命令
	AT_DEVICE_CTRL_WIFI_CWMODE,   	//设置 Wi-Fi 工作模式   ["AT+CWMODE"]
	AT_DEVICE_CTRL_WIFI_CWJAP,		//连接到 AP				["AT+CWJAP"]
	AT_DEVICE_CTRL_WIFI_CWLAP,    	//扫描 AP               ["AT+CWLAP"]
	AT_DEVICE_CTRL_WIFI_CWQAP,    	//与 AP 断开连接        ["AT+CWQAP"]
	AT_DEVICE_CTRL_WIFI_CWSAP,		//设置 AP 模式          ["AT+CWSAP"]
	AT_DEVICE_CTRL_WIFI_CWDHCP,		//设置 DHCP 模式      	["AT+CWDHCP"]
	AT_DEVICE_CTRL_WIFI_CWDHCPS,	//设置 DHCP 规则和网关	["AT+CWDHCPS"]
	AT_DEVICE_CTRL_WIFI_CWAUTOCONN,	//设置自动连接        	["AT+CWAUTOCONN"]
    //TCP/IP 命令
	AT_DEVICE_CTRL_TCPIP_CIPSTATUS,		//查询网络连接状态      ["AT+CIPSTATUS"]
	AT_DEVICE_CTRL_TCPIP_CIPMUX,     	//设置连接类型          ["AT+CIPMUX"]
	AT_DEVICE_CTRL_TCPIP_CIPSTART,		//创建TCP/UDP/SSL 客户端["AT+CIPSTART"]
	AT_DEVICE_CTRL_TCPIP_CIPSEND,		//发送数据              ["AT+CIPSEND"]
	AT_DEVICE_CTRL_TCPIP_CIPCLOSE,   	//关闭 TCP/UDP/SSL 连接 ["AT+CIPCLOSE"]
	AT_DEVICE_CTRL_TCPIP_CIPSERVER,		//创建 TCP/UDP 服务器   ["AT+CIPSERVER"]
	AT_DEVICE_CTRL_TCPIP_CIPMODE,    	//设置传输模式(正常模式或透传模式）["AT+CIPMODE"]
	AT_DEVICE_CTRL_TCPIP_SAVETRANSLINK,	//保存传输链接并启用自动链接["AT+SAVETRANSLINK"]
	AT_DEVICE_CTRL_TCPIP_CIPRECVMODE,	//设置自动接收数据模式  ["AT+CIPRECVMODE"]
	AT_DEVICE_CTRL_TCPIP_CIPRECVDATA,	//获得 socket 数据(被动模式)  ["AT+CIPRECVDATA"]
	AT_DEVICE_CTRL_TCPIP_PING,        	//获得 socket 数据(被动模式)  ["AT+PING"]
	AT_DEVICE_CTRL_TCPIP_CIPSTAMAC,		//设置 station mac 地址 ["AT+CIPSTAMAC"]
	AT_DEVICE_CTRL_TCPIP_CIPSTA,		//设置 station 静态 IP  ["AT+CIPSTA"]
	AT_DEVICE_CTRL_TCPIP_CIPAP,			//配置 softap IP 地址   ["AT+CIPAP"]
	//BLE 命令
	AT_DEVICE_CTRL_BLE_BLEADDR,			//Bluetooth LE 设备地址查询 ["AT+BLEADDR"]
	AT_DEVICE_CTRL_BLE_BLEADVSTART,		//开始 Bluetooth LE 广播 ["AT+BLEADVSTART"]
	AT_DEVICE_CTRL_BLE_BLEADVSTOP,		//停止 Bluetooth LE 广播 ["AT+BLEADVSTOP"]
	AT_DEVICE_CTRL_BLE_BLEDISCONN,		//断开 Bluetooth LE 连接 ["AT+BLEDISCONN"]
	AT_DEVICE_CTRL_BLE_BLENAME,			//查询/设置 Bluetooth LE 设备名称 ["AT+BLENAME"]
	AT_DEVICE_CTRL_BLE_BLESCAN,			//使能 Bluetooth LE 扫描(AT+BLESCAN) ["AT+BLESCAN"]
	//MQTT 命令
	AT_DEVICE_CTRL_MQTT_MQTTUSERCFG,	//设置 MQTT 用户属性 ["AT+MQTTUSERCFG"]
	AT_DEVICE_CTRL_MQTT_MQTTCLIENTID,	//设置 MQTT 客户端 ID ["AT+MQTTCLIENTID"]
	AT_DEVICE_CTRL_MQTT_MQTTUSERNAME,	//设置 MQTT 登录用户名 ["AT+MQTTUSERNAME"]
	AT_DEVICE_CTRL_MQTT_MQTTPASSWORD,	//设置 MQTT 登录密码 ["AT+MQTTPASSWORD"]
	AT_DEVICE_CTRL_MQTT_MQTTCONNCFG,	//设置 MQTT 连接属性 ["AT+MQTTCONNCFG"]
	AT_DEVICE_CTRL_MQTT_MQTTCONN,		//连接 MQTT Broker ["AT+MQTTCONN"]
	AT_DEVICE_CTRL_MQTT_MQTTPUB,		//发布 MQTT 字符串消息 ["AT+MQTTPUB"]
	AT_DEVICE_CTRL_MQTT_MQTTPUBRAW,		//发布 MQTT 二进制消息 ["AT+MQTTPUBRAW"]
	AT_DEVICE_CTRL_MQTT_MQTTSUB,		//订阅 MQTT Topic ["AT+MQTTSUB"]
	AT_DEVICE_CTRL_MQTT_MQTTUNSUB,		//取消订阅 MQTT Topic ["AT+MQTTUNSUB"]
	AT_DEVICE_CTRL_MQTT_MQTTCLEAN,		//断开 MQTT 连接 ["AT+MQTTCLEAN"]
};
/* Name type */
#define AT_DEVICE_NAMETYPE_DEVICE      0x01
#define AT_DEVICE_NAMETYPE_NETDEV      0x02
#define AT_DEVICE_NAMETYPE_CLIENT      0x03

struct at_device;


/**********************add********************/
typedef int (*cmdhandler_t)(struct at_device* device, char *buff, int size, char *arg);
typedef struct
{
    const char* name;
    cmdhandler_t fn;
    enum AT_CMD id;
} command_desc_t;

typedef struct {
	unsigned int group_id;
    const char* groupname;
    const command_desc_t* commands;
    int commands_count;
    void* context;
} command_group_t;
/**********************end********************/





/* AT device wifi ssid and password information */
struct at_device_ssid_pwd
{
    char *ssid;
    char *password;
};

/* AT device operations */
struct at_device_ops
{
    int (*init)(struct at_device *device);
    int (*deinit)(struct at_device *device);
    int (*control)(struct at_device *device, int cmd, char *arg, char *buff, int size);
};

struct at_device_class
{
    uint16_t class_id;                           /* AT device class ID */
    const struct at_device_ops *device_ops;      /* AT device operaiotns */
#if(defined AT_USING_SOCKET && AT_USING_SOCKET == 1U)
    uint32_t socket_num;                         /* The maximum number of sockets support */
    const struct at_socket_ops *socket_ops;      /* AT device socket operations */
#endif
    rt_slist_t list;                             /* AT device class list */
};


struct at_device
{
    char name[RT_DEVICE_NAME_MAX];                      /* AT device name */
    rt_bool_t is_init;                           /* AT device initialization completed */
    struct at_device_class *class;               /* AT device class object */
    struct at_client *client;                    /* AT Client object for AT device */
    struct netdev *netdev;                       /* Network interface device for AT device */
#if(defined AT_USING_SOCKET && AT_USING_SOCKET == 1U)
    rt_event_t socket_event;                 	 /* AT device socket event */
    struct at_socket *sockets;                   /* AT device sockets list */
#endif
    rt_slist_t list;                             /* AT device list */
    void *user_data;                             /* User-specific data */

    command_group_t command_groups[AT_MAX_COMMAND_GROUPS];
    unsigned int command_groups_count;
};

/* Get AT device object */
struct at_device *at_device_get_first_initialized(void);
struct at_device *at_device_get_by_name(int type, const char *name);
#if(defined AT_USING_SOCKET && AT_USING_SOCKET == 1U)
// struct at_device *at_device_get_by_socket(int at_socket);
#endif

/* AT device control operaions */
int at_device_control(struct at_device *device, int cmd, char *arg, char *buff, int size);
/* Register AT device class object */
int at_device_class_register(struct at_device_class *class, uint16_t class_id);
/* Register AT device object */
int at_device_register(struct at_device *device, const char *device_name,
                        const char *at_client_name, uint16_t class_id, void *user_data);

typedef enum
{
	AT_GROUP_BASIC,
	AT_GROUP_OTA,
	AT_GROUP_WIFI,
	AT_GROUP_TCPIP,
	AT_GROUP_BLE,
	AT_GROUP_MQTT,
}at_group_id;
void at_register_command_group(struct at_device* device, const char* groupname, const command_desc_t* desc, int ndesc, void* group_ctx);


#endif /* CONFIG_AT_CLIENT_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* __AT_DEVICE_H__ */
