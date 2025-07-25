#include "e103w11/mqtt_command/at_mqtt_command.h"
#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)

/* Set mqtt user attribute [AT+MQTTUSERCFG=...] */
static int e103w11_mqtt_MQTTUSERCFG(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+MQTTUSERCFG=..." commonds to e103w11 device */
    //AT+MQTTUSERCFG=0,1,"E103W11_AT_TEST1","","",0,0,"mqtt://192.168.1.103"
    result = at_obj_exec_cmd(client, resp, "AT+MQTTUSERCFG=%s", arg);
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    return result;
}
/* Set mqtt user attribute [AT+MQTTCLIENTID=...] */
static int e103w11_mqtt_MQTTCLIENTID(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+MQTTCLIENTID=..." commonds to e103w11 device */ 
    result = at_obj_exec_cmd(client, resp, "AT+MQTTCLIENTID=%s", arg);//AT+MQTTCLIENTID=0,"E103W11_AT_TEST3"
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    return result;
}
/* Set mqtt user name [AT+MQTTUSERNAME=...] */
static int e103w11_mqtt_MQTTUSERNAME(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+MQTTUSERNAME=..." commonds to e103w11 device */ 
    result = at_obj_exec_cmd(client, resp, "AT+MQTTUSERNAME=%s", arg);//AT+MQTTUSERNAME=0,"AAAA"
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    return result;
}
/* Set mqtt user password [AT+MQTTPASSWORD=...] */
static int e103w11_mqtt_MQTTPASSWORD(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+MQTTPASSWORD=..." commonds to e103w11 device */ 
    result = at_obj_exec_cmd(client, resp, "AT+MQTTPASSWORD=%s", arg);//AT+MQTTPASSWORD=0,"12345678"
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    return result;
}
/* Set mqtt connect attribute [AT+MQTTCONNCFG=...] */
static int e103w11_mqtt_MQTTCONNCFG(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+MQTTCONNCFG=..." commonds to e103w11 device */ 
    result = at_obj_exec_cmd(client, resp, "AT+MQTTCONNCFG=%s", arg);//AT+MQTTCONNCFG=0,0,0,"lwt_topic","lwt_msg",2,1
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    return result;
}
/* connect mqtt Broker [AT+MQTTCONN=...] */
static int e103w11_mqtt_MQTTCONN(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+MQTTCONN=..." commonds to e103w11 device */ 
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+MQTTCONN?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+MQTTCONN=%s", arg);//AT+MQTTCONN=<LinkID>,<"host">,<port>,<reconnect>
    }
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    return result;
}
/* Publish an MQTT string message [AT+MQTTPUB=...] */
static int e103w11_mqtt_MQTTPUB(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+MQTTPUB=..." commonds to e103w11 device */ 
    result = at_obj_exec_cmd(client, resp, "AT+MQTTPUB=%s", arg);//AT+MQTTPUB=0,"topic0","data_test",0,0
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    return result;
}
/* Publish MQTT binary messages [AT+MQTTPUBRAW=...] */
static int e103w11_mqtt_MQTTPUBRAW(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+MQTTPUBRAW=..." commonds to e103w11 device */ 
    result = at_obj_exec_cmd(client, resp, "AT+MQTTPUBRAW=%s", arg);//AT+MQTTPUBRAW=0,"topic0",10,0,0
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    return result;
}
/* Subscribe MQTT Topic [AT+MQTTSUB=...] */
static int e103w11_mqtt_MQTTSUB(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+MQTTSUB=..." commonds to e103w11 device */ 
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+MQTTSUB?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+MQTTSUB=%s", arg);//AT+MQTTSUB=<LinkID>,<"topic">,<qos>
    }
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    return result;
}
/* Unsubscribe MQTT Topic [AT+MQTTUNSUB=...] */
static int e103w11_mqtt_MQTTUNSUB(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+MQTTUNSUB=..." commonds to e103w11 device */ 
    result = at_obj_exec_cmd(client, resp, "AT+MQTTUNSUB=%s", arg);//AT+MQTTUNSUB=0,"topic2"
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    return result;
}
/* disconnect MQTT [AT+MQTTCLEAN=...] */
static int e103w11_mqtt_MQTTCLEAN(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+MQTTCLEAN=..." commonds to e103w11 device */ 
    result = at_obj_exec_cmd(client, resp, "AT+MQTTCLEAN=%s", arg);//AT+MQTTCLEAN=0
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    return result;
}


#if(defined AT_DEVICE_INTERFACE_MQTT && AT_DEVICE_INTERFACE_MQTT == 1U)
extern int e103w11_device_cmd_no_result(struct at_device *device, const char *cmd, int timeout);
extern int e103w11_device_cmd_ret_string(struct at_device *device, const char *cmd, int timeout, 
                                        unsigned int resp_line, char *buff_str, unsigned int size);

/**
  * @brief  设置 MQTT 用户属性
  * @param  device at 设备句柄
  * @param  cfg 属性信息
  * @retval 0-success others-error
  * @note <scheme>当前仅支持 1: MQTT over TCP。
  */
int at_mqtt_MQTTUSERCFG(struct at_device *device, const struct s_mqtt_MQTTUSERCFG *cfg)
{
    if(device == RT_NULL || cfg == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTUSERCFG=%d,%d\"%s\",\"%s\",\"%s\",%d,%d\"%s\"", 
                            cfg->link_id, cfg->scheme, cfg->client_id, cfg->username, 
                            cfg->password, cfg->cert_key_id, cfg->ca_id, cfg->path);
    if (result != RT_EOK)
    {
        goto __exit;
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  设置 MQTT 客户端 ID
  * @param  device at 设备句柄
  * @param  link_id 当前仅支持 link ID 0
  * @param  client_id MQTT 客户端 ID， 最大长度： 256 字节
  * @retval 0-success others-error
  * @note 
  */
int at_mqtt_MQTTCLENTID(struct at_device *device, unsigned int link_id, const char *client_id)
{
    if(device == RT_NULL || client_id == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTCLENTID=%d\"%s\"", link_id, client_id);
    if (result != RT_EOK)
    {
        goto __exit;
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  设置 MQTT 登录用户名
  * @param  device at 设备句柄
  * @param  link_id 当前仅支持 link ID 0
  * @param  username 用户名， 用于登陆 MQTT broker， 最大长度： 64 字节
  * @retval 0-success others-error
  * @note 
  */
int at_mqtt_MQTTUSERNAME(struct at_device *device, unsigned int link_id, const char *username)
{
    if(device == RT_NULL || username == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTUSERNAME=%d\"%s\"", link_id, username);
    if (result != RT_EOK)
    {
        goto __exit;
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  设置 MQTT 登录密码
  * @param  device at 设备句柄
  * @param  link_id 当前仅支持 link ID 0
  * @param  password 密码， 用于登陆 MQTT broker， 最大长度： 64 字节
  * @retval 0-success others-error
  * @note 
  */
int at_mqtt_MQTTPASSWORD(struct at_device *device, unsigned int link_id, const char *password)
{
    if(device == RT_NULL || password == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTPASSWORD=%d\"%s\"", link_id, password);
    if (result != RT_EOK)
    {
        goto __exit;
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  设置 MQTT 连接属性
  * @param  device at 设备句柄
  * @param  cfg 连接属性
  * @retval 0-success others-error
  * @note 
  */
int at_mqtt_MQTTCONNCFG(struct at_device *device, const struct s_mqtt_MQTTCONNCFG *cfg)
{
    if(device == RT_NULL || cfg == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTCONNCFG=%d,%d,%d,\"%s\",\"%s\",%d,%d", 
                            cfg->link_id, cfg->keepalive, cfg->disable_clean_session, 
                            cfg->lwt_topic, cfg->lwt_msg, cfg->lwt_qos, cfg->lwt_retain);
    if (result != RT_EOK)
    {
        goto __exit;
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  连接 MQTT Broker
  * @param  device at 设备句柄
  * @param  conn 连接信息
  * @retval 0-success others-error
  * @note 
  */
int at_mqtt_MQTTCONN_get(struct at_device *device, struct s_mqtt_MQTTCONN *conn)
{
    if(device == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTCONN?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(conn != RT_NULL)
    {
        char format[48];
        //"+MQTTCONN:%d,%d,%d,\"%128[^\"]\",%d,\"%32[^\"]\",%d"
        rt_sprintf(format, "+MQTTCONN:%cd,%cd,%cd,\"%c%d[^\"]\",%cd,\"%c%d[^\"]\",%cd", 
                           '%', '%', '%', '%', sizeof(conn->host), '%', '%', sizeof(conn->path), '%');
        if (at_resp_parse_line_args_by_kw(resp, "+MQTTCONN:", format, 
                                    &conn->link_id, &conn->state, 
                                    &conn->scheme, conn->host, 
                                    &conn->port, conn->path, 
                                    &conn->reconnect) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  连接 MQTT Broker
  * @param  device at 设备句柄
  * @param  link_id 当前仅支持 link ID 0
  * @param  host MQTT broker 域名， 最大长度： 128 字节
  * @param  port MQTT broker 端口， 最大端口： 65535
  *              注： 如果该参数值为 0， 系统实际使用的端口为 1883
  * @param  path 资源路径， 最大长度： 32 字节
  * @param  reconnect 0: MQTT 不自动重连
  *                   1: MQTT 自动重连， 会消耗较多的内存资源
  * @retval 0-success others-error
  * @note 
  */
int at_mqtt_MQTTCONN_set(struct at_device *device, 
                        unsigned int link_id, const char *host, 
                        unsigned int port, const char *path, unsigned int reconnect)
{
    if(device == RT_NULL || host == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTCONN=%d,\"%s\",%d,\"%s\",%d", link_id, host, port, path, reconnect);
    if (result != RT_EOK)
    {
        goto __exit;
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  发布 MQTT 字符串消息
  * @param  device at 设备句柄
  * @param  link_id 当前仅支持 link ID 0
  * @param  topic MQTT 字符串消息发送到的 MQTT topic， 最大长度： 128 字节
  * @param  data 发布的 MQTT 字符串消息
  * @param  qos 发布消息的 QoS， 参数可选 0、 1、 或 2， 默认值：0
  *             0： QoS0 等级， Broker 最多能收到一次 Publisher 发布的消息
  *             1： QoS1 等级， Broker 至少能收到一次 Publisher 发布的消息
  *             2： QoS2 等级， Broker 只能收到一次 publisher 发布的消息
  * @param  retain 保留消息标志位
  *             0： Broker 不保留该 topic 中的消息
  *             1： Broker 保留该 topic 中最后一条消息
  * @retval 0-success others-error
  * @note 
  */
int at_mqtt_MQTTPUB(struct at_device *device, 
                        unsigned int link_id, const char *topic, 
                        const char *data, unsigned int qos, unsigned int retain)
{
    if(device == RT_NULL || topic == RT_NULL || data == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTPUB=%d,\"%s\",\"%s\",%d,%d", link_id, topic, data, qos, retain);
    if (result != RT_EOK)
    {
        goto __exit;
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  发布 MQTT 二进制消息
  * @param  device at 设备句柄
  * @param  link_id 当前仅支持 link ID 0
  * @param  topic MQTT 字符串消息发送到的 MQTT topic， 最大长度： 128 字节
  * @param  data 发布的 MQTT 二进制消息
  * @param  length 发布的 MQTT 二进制消息长度
  * @param  qos 发布消息的 QoS， 参数可选 0、 1、 或 2， 默认值：0
  *             0： QoS0 等级， Broker 最多能收到一次 Publisher 发布的消息
  *             1： QoS1 等级， Broker 至少能收到一次 Publisher 发布的消息
  *             2： QoS2 等级， Broker 只能收到一次 publisher 发布的消息
  * @param  retain 保留消息标志位
  *             0： Broker 不保留该 topic 中的消息
  *             1： Broker 保留该 topic 中最后一条消息
  * @retval 0-success others-error
  * @note 
  */
int at_mqtt_MQTTPUBRAW(struct at_device *device, 
                        unsigned int link_id, const char *topic, 
                        const char data[], unsigned int length, unsigned int qos, unsigned int retain)
{
    if(device == RT_NULL || topic == RT_NULL || data == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* set AT client end sign to deal with '>' sign */
    at_obj_set_end_sign(device->client, '>');

    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTPUBRAW=%d,\"%s\",%d,%d,%d", link_id, topic, length, qos, retain);
    if (result != RT_EOK)
    {
        goto __exit;
    }
    /* send the real data to server or client */
    result = (int) at_client_obj_send(device->client, data, length);
    if (result == 0)
    {
        result = RT_ERROR;
        goto __exit;
    }
__exit:
    /* reset the end sign for data */
    at_obj_set_end_sign(device->client, 0);
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  订阅 MQTT Topic
  * @param  device at 设备句柄
  * @param  topic 输出：存储所有topic信息缓冲区
  * @param  size topic缓冲区个数
  * @retval 0-success others-error
  * @note 
  */
int at_mqtt_MQTTSUB_get(struct at_device *device, struct s_mqtt_MQTTSUB topic[], unsigned int size)
{
    if(device == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(1024, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTSUB?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(topic != RT_NULL)
    {
        char format[48];
        //"+MQTTCONN:%d,%d,%d,\"%128[^\"]\",%d,\"%32[^\"]\",%d"
        rt_sprintf(format, "+MQTTSUB:%cd,%cd,\"%c%d[^\"]\",%cd", 
                           '%', '%', '%', sizeof(topic[0].topic), '%');
        for(int i = 0; i < size && i < resp->line_counts-1; i++)
        {
            if (at_resp_parse_line_args(resp, i+1, format, 
                                            &topic[i].link_id, &topic[i].state, topic[i].topic, &topic[i].qos) < 0)
            {
                // result = RT_ERROR;
                goto __exit;
            }
        }
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  订阅 MQTT Topic
  * @param  device at 设备句柄
  * @param  link_id 当前仅支持 link ID 0
  * @param  topic MQTT 字符串消息发送到的 MQTT topic， 最大长度： 128 字节
  * @param  qos 订阅的 QoS
  *             0： QoS0 等级， Broker 最多能收到一次 Publisher 发布的消息
  *             1： QoS1 等级， Broker 至少能收到一次 Publisher 发布的消息
  *             2： QoS2 等级， Broker 只能收到一次 publisher 发布的消息
  * @retval 0-success others-error
  * @note 
  */
int at_mqtt_MQTTSUB_set(struct at_device *device, unsigned int link_id, const char *topic, unsigned int qos)
{
    if(device == RT_NULL || topic == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTSUB=%d,\"%s\",%d", link_id, topic, qos);
    if (result != RT_EOK)
    {
        goto __exit;
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  取消订阅 MQTT Topic
  * @param  device at 设备句柄
  * @param  link_id 当前仅支持 link ID 0
  * @param  topic MQTT 字符串消息发送到的 MQTT topic， 最大长度： 128 字节
  * @param  qos 订阅的 QoS
  *             0： QoS0 等级， Broker 最多能收到一次 Publisher 发布的消息
  *             1： QoS1 等级， Broker 至少能收到一次 Publisher 发布的消息
  *             2： QoS2 等级， Broker 只能收到一次 publisher 发布的消息
  * @retval 0-success others-error
  * @note 
  */
int at_mqtt_MQTTUNSUB(struct at_device *device, unsigned int link_id, const char *topic, unsigned int qos)
{
    if(device == RT_NULL || topic == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTUNSUB=%d,\"%s\",%d", link_id, topic, qos);
    if (result != RT_EOK)
    {
        goto __exit;
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief   断开 MQTT 连接
  * @param  device at 设备句柄
  * @param  link_id 当前仅支持 link ID 0
  * @retval 0-success others-error
  * @note 
  */
int at_mqtt_MQTTCLEAN(struct at_device *device, unsigned int link_id)
{
    if(device == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+MQTTCLEAN=%d", link_id);
    if (result != RT_EOK)
    {
        goto __exit;
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
#endif /* AT_DEVICE_INTERFACE_MQTT */
static const command_desc_t mqtt_commands[] = {
    {"MQTTUSERCFG",     &e103w11_mqtt_MQTTUSERCFG,  AT_DEVICE_CTRL_MQTT_MQTTUSERCFG},
    {"MQTTCLIENTID",    &e103w11_mqtt_MQTTCLIENTID, AT_DEVICE_CTRL_MQTT_MQTTCLIENTID},
    {"MQTTUSERNAME",    &e103w11_mqtt_MQTTUSERNAME, AT_DEVICE_CTRL_MQTT_MQTTUSERNAME},
    {"MQTTPASSWORD",    &e103w11_mqtt_MQTTPASSWORD, AT_DEVICE_CTRL_MQTT_MQTTPASSWORD},
    {"MQTTCONNCFG",     &e103w11_mqtt_MQTTCONNCFG,  AT_DEVICE_CTRL_MQTT_MQTTCONNCFG},
    {"MQTTCONN",        &e103w11_mqtt_MQTTCONN,     AT_DEVICE_CTRL_MQTT_MQTTCONN},
    {"MQTTPUB",         &e103w11_mqtt_MQTTPUB,      AT_DEVICE_CTRL_MQTT_MQTTPUB},
    {"MQTTPUBRAW",      &e103w11_mqtt_MQTTPUBRAW,   AT_DEVICE_CTRL_MQTT_MQTTPUBRAW},
    {"MQTTSUB",         &e103w11_mqtt_MQTTSUB,      AT_DEVICE_CTRL_MQTT_MQTTSUB},
    {"MQTTUNSUB",       &e103w11_mqtt_MQTTUNSUB,    AT_DEVICE_CTRL_MQTT_MQTTUNSUB},
    {"MQTTCLEAN",       &e103w11_mqtt_MQTTCLEAN,    AT_DEVICE_CTRL_MQTT_MQTTCLEAN},
    {"MQTTCLEAN",       &e103w11_mqtt_MQTTCLEAN,    AT_DEVICE_CTRL_MQTT_MQTTCLEAN},
};

void at_register_mqtt_commands(struct at_device *device)
{
    at_register_command_group(device, "MQTT", mqtt_commands, sizeof(mqtt_commands) / sizeof(command_desc_t), 0);
}
#endif /* AT_DEVICE_USING_E103W11 */