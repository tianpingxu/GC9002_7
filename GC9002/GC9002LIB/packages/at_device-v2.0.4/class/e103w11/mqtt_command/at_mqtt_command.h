/*
 * File      : at_mqtt_command.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-31     huyt         first version
 */

#ifndef __AT_MQTT_COMMAND_H__
#define __AT_MQTT_COMMAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "at_device.h"
#include "e103w11/at_config_e103w11.h"
#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)


void at_register_mqtt_commands(struct at_device *device);

#if(defined AT_DEVICE_INTERFACE_MQTT && AT_DEVICE_INTERFACE_MQTT == 1U)
struct s_mqtt_MQTTUSERCFG
{
    unsigned int link_id;//当前仅支持 link ID 0
    /* 1: MQTT over TCP
     * 2:MQTT over TLS（不校验证书）
     * 3: MQTT over TLS（校验 server 证书） 
     * 4: MQTT over TLS（提供 client 证书）
     * 5: MQTT over TLS（校验 server 证书并且提供 client 证书）
     * 6: MQTT over WebSocket（基于 TCP）
     * 7: MQTT over WebSocket Secure（基于 TLS， 不校验证书）
     * 8: MQTT over WebSocket Secure（基于 TLS， 校验 server 证书）
     * 9: MQTT over WebSocket Secure（基于 TLS， 提供 client 证书）
     * 10: MQTT over WebSocket Secure（ 基于 TLS， 校验 server 证书并且提供client 证书）
     */
    unsigned int scheme;
    char client_id[256];//MQTT 客户端 ID， 最大长度： 256 字节
    char username[64];//用户名， 用于登陆 MQTT broker， 最大长度： 64 字节
    char password[64];//密码， 用于登陆 MQTT broker， 最大长度： 64 字节
    unsigned int cert_key_id;//证书 ID， 目前仅支持 MQTT over TCP， 此参数为 0
    unsigned int ca_id;//CA ID， 目前仅支持 MQTT over TCP， 此参数为 0
    char path[32];//资源路径， 最大长度： 32 字节
};
struct s_mqtt_MQTTCONNCFG
{
    unsigned int link_id;//当前仅支持 link ID 0
    /* MQTT ping 超时时间， 单位： 秒。 范围： [0,7200]
     * 默认值： 0
     * 注： 当该参数值为 0 时， 系统自动将其修改为 120 秒
     */
    unsigned int keepalive;
    /* 设置 MQTT 清理会话标志
     * 0: 使能清理会话
     * 1: 禁用清理会话
     */
    unsigned int disable_clean_session;
    char lwt_topic[128];//遗嘱 topic， 最大长度： 128 字节
    char lwt_msg[64];//遗嘱 message， 最大长度： 64 字节
    unsigned int lwt_qos;//遗嘱 QoS， 参数可选 0、 1、 2， 默认值： 0
    unsigned int lwt_retain;//遗嘱 retain， 参数可选 0 或 1， 默认值： 0
};
struct s_mqtt_MQTTCONN
{
    unsigned int link_id;//当前仅支持 link ID 0
    /* MQTT 状态
     * 0: MQTT 未初始化
     * 1: 已设置 AT+MQTTUSERCFG
     * 2: 已设置 AT+MQTTCONNCFG
     * 3: 连接已断开
     * 4: 已建立连接
     * 5: 已连接， 但未订阅 topic
     * 6: 已连接， 已订阅过 topic
     */
    unsigned int state;
    /* 1: MQTT over TCP
     * 2:MQTT over TLS（不校验证书）
     * 3: MQTT over TLS（校验 server 证书） 
     * 4: MQTT over TLS（提供 client 证书）
     * 5: MQTT over TLS（校验 server 证书并且提供 client 证书）
     * 6: MQTT over WebSocket（基于 TCP）
     * 7: MQTT over WebSocket Secure（基于 TLS， 不校验证书）
     * 8: MQTT over WebSocket Secure（基于 TLS， 校验 server 证书）
     * 9: MQTT over WebSocket Secure（基于 TLS， 提供 client 证书）
     * 10: MQTT over WebSocket Secure（ 基于 TLS， 校验 server 证书并且提供client 证书）
     */
    unsigned int scheme;
    char host[128];//MQTT broker 域名， 最大长度： 128 字节
    unsigned int port;//MQTT broker 端口， 最大端口： 65535。 注： 如果该参数值为 0， 系统实际使用的端口为 1883
    char path[32];//资源路径， 最大长度： 32 字节
    /* 0: MQTT 不自动重连
     * 1: MQTT 自动重连， 会消耗较多的内存资源
     **/
    unsigned int reconnect;
};
struct s_mqtt_MQTTSUB
{
    unsigned int link_id;//当前仅支持 link ID 0
    /* MQTT 状态
     * 0: MQTT 未初始化
     * 1: 已设置 AT+MQTTUSERCFG
     * 2: 已设置 AT+MQTTCONNCFG
     * 3: 连接已断开
     * 4: 已建立连接
     * 5: 已连接， 但未订阅 topic
     * 6: 已连接， 已订阅过 topic
     */
    unsigned int state;
    char topic[128];//MQTT 字符串消息发送到的 MQTT topic， 最大长度： 128 字节
    /* 订阅的 QoS
     * 0： QoS0 等级， Broker 最多能收到一次 Publisher 发布的消息
     * 1： QoS1 等级， Broker 至少能收到一次 Publisher 发布的消息
     * 2： QoS2 等级， Broker 只能收到一次 publisher 发布的消息
     */
    unsigned int qos;
};
int at_mqtt_MQTTUSERCFG(struct at_device *device, const struct s_mqtt_MQTTUSERCFG *cfg);
int at_mqtt_MQTTCLENTID(struct at_device *device, unsigned int link_id, const char *client_id);
int at_mqtt_MQTTUSERNAME(struct at_device *device, unsigned int link_id, const char *username);
int at_mqtt_MQTTPASSWORD(struct at_device *device, unsigned int link_id, const char *password);
int at_mqtt_MQTTCONNCFG(struct at_device *device, const struct s_mqtt_MQTTCONNCFG *cfg);
int at_mqtt_MQTTCONN_get(struct at_device *device, struct s_mqtt_MQTTCONN *conn);
int at_mqtt_MQTTCONN_set(struct at_device *device, 
                        unsigned int link_id, const char *host, 
                        unsigned int port, const char *path, unsigned int reconnect);
int at_mqtt_MQTTPUB(struct at_device *device, 
                        unsigned int link_id, const char *topic, 
                        const char *data, unsigned int qos, unsigned int retain);
int at_mqtt_MQTTPUBRAW(struct at_device *device, 
                        unsigned int link_id, const char *topic, 
                        const char data[], unsigned int length, unsigned int qos, unsigned int retain);
int at_mqtt_MQTTSUB_get(struct at_device *device, struct s_mqtt_MQTTSUB topic[], unsigned int size);
int at_mqtt_MQTTSUB_set(struct at_device *device, unsigned int link_id, const char *topic, unsigned int qos);
int at_mqtt_MQTTUNSUB(struct at_device *device, unsigned int link_id, const char *topic, unsigned int qos);
int at_mqtt_MQTTCLEAN(struct at_device *device, unsigned int link_id);

#endif /* AT_DEVICE_INTERFACE_MQTT */

#endif /* AT_DEVICE_USING_E103W11 */

#ifdef __cplusplus
}
#endif

#endif /* __AT_MQTT_COMMAND_H__ */