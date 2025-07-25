#include "e103w11/tcpip_command/at_tcpip_command.h"
#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)

/* Get the network connection status [AT+CIPSTATUS] */
static int e103w11_tcpip_CIPSTATUS(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIPSTATUS" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+CIPSTATUS");
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
/* Set connection type [AT+CIPMUX] */
static int e103w11_tcpip_CIPMUX(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIPMUX" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+CIPMUX?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+CIPMUX=%s", arg);//AT+CIPMUX=1 //设置为多连接。
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
/* Creat TCP/UDP/SSL client [AT+CIPSTART] */
static int e103w11_tcpip_CIPSTART(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIPSTART" commonds to e103w11 device */
    //AT+CIPSTART="TCP","192.168.101.110",1000
    //AT+CIPSTART="UDP","192.168.101.110",1000,1002
    //AT+CIPSTART="SSL","iot.eswin.cn",8443
    result = at_obj_exec_cmd(client, resp, "AT+CIPSTART=%s", arg);
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
/* Send data [AT+CIPSEND] */
static int e103w11_tcpip_CIPSEND(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIPSEND" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+CIPSEND=%s", arg);
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
/* Close TCP/UDP/SSL connection [AT+CIPCLOSE] */
static int e103w11_tcpip_CIPCLOSE(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIPCLOSE" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+CIPCLOSE=%s", arg);
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
/* Creat TCP/UDP server [AT+CIPSERVER] */
static int e103w11_tcpip_CIPSERVER(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIPSERVER" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+CIPSERVER=%s", arg);//AT+CIPSERSER=1,8000,"TCP"
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
/* Set transmission mode [AT+CIPMODE] */
static int e103w11_tcpip_CIPMODE(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIPMODE" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+CIPMODE?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+CIPMODE=%s", arg);//AT+CIPSERSER=1,8000,"TCP"
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
/* Save the transport link and enable automatic linking [AT+SAVETRANSLINK] */
static int e103w11_tcpip_SAVETRANSLINK(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+SAVETRANSLINK" commonds to e103w11 device */
    //AT+SAVETRANSLINK=1,"192.168.6.110",1002,"TCP"
    //AT+SAVETRANSLINK=1,"192.168.6.110",1002,"UDP",1005
    result = at_obj_exec_cmd(client, resp, "AT+SAVETRANSLINK=%s", arg);//AT+CIPSERSER=1,8000,"TCP"
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
/* Set the automatic data receiving mode [AT+CIPRECVMODE] */
static int e103w11_tcpip_CIPRECVMODE(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIPRECVMODE" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+CIPRECVMODE?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+CIPRECVMODE=%s", arg);//AT+CIPSERSER=1,8000,"TCP"
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
/* Receive data of socket [AT+CIPRECVDATA] */
static int e103w11_tcpip_CIPRECVDATA(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIPRECVDATA" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+CIPRECVDATA=%s", arg);
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
/* PING [AT+CIPRECVDATA] */
static int e103w11_tcpip_PING(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+PING" commonds to e103w11 device */
    //AT+PING="192.168.1.1"
    //AT+PING="www.baidu.com"
    result = at_obj_exec_cmd(client, resp, "AT+PING=%s", arg);
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
/* Set station mac address [AT+CIPSTAMAC] */
static int e103w11_tcpip_CIPSTAMAC(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIPSTAMAC" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+CIPSTAMAC?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+CIPSTAMAC=%s", arg);//AT+CIPSTAMAC="aa:bb:c2:d3:dd:2a","s.y"
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
/* Set station static ip [AT+CIPSTA] */
static int e103w11_tcpip_CIPSTA(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIPSTA" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+CIPSTA?");
    }
    else
    {
        /*AT+CIPSTA="192.168.3.150","192.168.3.1","255.255.255.0","s.y"
            设置 Station 的 IP 为 192.168.3.150，网关为：192.168.3.1，子网掩码为： 255.255.255.0，并保存 flash。*/
        result = at_obj_exec_cmd(client, resp, "AT+CIPSTA=%s", arg);
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
/* Set softap IP [AT+CIPAP] */
static int e103w11_tcpip_CIPAP(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIPAP" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+CIPAP?");
    }
    else
    {
        /*AT+CIPAP="192.168.10.1","192.168.10.1","255.255.255.0","s.y"
            设置 SoftAP 的 IP 为 192.168.10.1，网关为： 192.168.10.1，子网掩码为： 255.255.255.0，并保存 Flash*/
        result = at_obj_exec_cmd(client, resp, "AT+CIPAP=%s", arg);
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







#if(defined AT_DEVICE_INTERFACE_TCPIP && AT_DEVICE_INTERFACE_TCPIP == 1U)
extern int e103w11_device_cmd_no_result(struct at_device *device, const char *cmd, int timeout);
extern int e103w11_device_cmd_ret_string(struct at_device *device, const char *cmd, int timeout, 
                                        unsigned int resp_line, char *buff_str, unsigned int size);
/**
  * @brief  查询网络连接状态
  * @param  device at 设备句柄
  * @param  status 输出：网络连接状态
  * @retval 0-success others-error
  */
int at_tcpip_CIPSTATUS(struct at_device *device, struct s_tcpip_status *status)
{
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(128, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTATUS");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(status != RT_NULL)
    {
        char format[48];
        //"+CIPSTATUS:%d,%d\"%3[^\"]\",\"%64[^\"]\",%d,%d,%d"
        rt_sprintf(format, "+CIPSTATUS:%cd,\"%c3[^\"]\",\"%c%d[^\"]\",%cd,%cd,%cd", 
                    '%', '%', '%', sizeof(status->remote_ip), '%', '%', '%');
        if (at_resp_parse_line_args_by_kw(resp, "+CIPSTATUS:", format, 
                                        &status->link_id, &status->type, &status->remote_ip, 
                                        &status->remote_port, &status->local_port, &status->tetype) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
        if (at_resp_parse_line_args_by_kw(resp, "STATUS:", "STATUS:%d", &status->stat) < 0)
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
  * @brief  查询设置连接类型
  * @param  device at 设备句柄
  * @param  link_mux 输出：连接类型
  *         0：单连接
  *         1：多连接
  * @retval 0-success others-error
  */
int at_tcpip_CIPMUX_get(struct at_device *device, unsigned int *link_mux)
{
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPMUX?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if (at_resp_parse_line_args_by_kw(resp, "+", "+CIPMUX:%d", link_mux) < 0)
    {
        result = RT_ERROR;
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
  * @brief  设置连接类型
  * @param  device at 设备句柄
  * @param  link_mux 连接类型
  *         0：单连接
  *         1：多连接
  * @retval 0-success others-error
  */
int at_tcpip_CIPMUX_set(struct at_device *device, unsigned int link_mux)
{
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPMUX=%d", link_mux);
    if (result != RT_EOK)
    {
        goto __exit;
    }
    at_client_set_flag(device->client, AT_CLIENT_LINK_MUX_POS, (link_mux == 0 ? false : true));
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  创建 TCP 客户端
  * @param  device at 设备句柄
  * @param  link_id ⽹络连接 ID (0~4)， ⽤于多连接的情况
  * @param  remote_ip 字符串参数，远端 IP 地址
  * @param  remote_port 远端端⼝号
  * @param  keep_alive_time 可选参数
  *         TCP keep-alive 侦测时间，默认关闭此功能
  *         0：关闭 TCP keep-alive 功能
  *         1~7200：侦测时间，单位为 1s
  * @retval 0-success others-error
  */
int at_tcpip_CIPSTART_tcp(struct at_device *device, unsigned int link_id, 
                        const char *remote_ip, unsigned int remote_port, 
                        unsigned int keep_alive_time)
{
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    //单连接
    if(at_client_get_flag(device->client, AT_CLIENT_LINK_MUX_POS) == false)
    {
        if(keep_alive_time == 0)
        {
            result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=\"TCP\",\"%s\",%d", remote_ip, remote_port);
        }
        else
        {
            result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=\"TCP\",\"%s\",%d,%d", remote_ip, remote_port, keep_alive_time);
        }
    }
    else
    {
        if(keep_alive_time == 0)
        {
            result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=,%d,\"TCP\",\"%s\",%d", link_id, remote_ip, remote_port);
        }
        else
        {
            result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=,%d,\"TCP\",\"%s\",%d,%d", link_id, remote_ip, remote_port, keep_alive_time);
        }
    }
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
  * @brief  创建 UDP 客户端
  * @param  device at 设备句柄
  * @param  link_id ⽹络连接 ID (0~4)， ⽤于多连接的情况
  * @param  remote_ip 字符串参数，远端 IP 地址
  * @param  remote_port 远端端⼝号
  * @param  local_port 可选参数，非0值有效
  *         UDP 传输时，设置本地端⼝
  * @param  mode 可选参数，0xFF是无效值，当mode有效时，local_port必须是有效参数
  *         UDP 传输的属性，若透传，则必须为 0
  *         0：收到数据后，不更改远端⽬标，默认值为 0
  *         1：收到数据后，改变⼀次远端⽬标
  *         2：收到数据后，改变远端⽬标
  * @retval 0-success others-error
  */
int at_tcpip_CIPSTART_udp(struct at_device *device, unsigned int link_id, 
                        const char *remote_ip, unsigned int remote_port, 
                        unsigned int local_port, unsigned int mode)
{
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    if(at_client_get_flag(device->client, AT_CLIENT_LINK_MUX_POS) == false)
    {
        if(local_port == 0)
        {
            result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=\"UDP\",\"%s\",%d", remote_ip, remote_port);
        }
        else
        {
            if(mode != 0xFF)
            {
                result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=\"UDP\",\"%s\",%d,%d,%d", remote_ip, remote_port, local_port, mode);
            }
            else
            {
                result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=\"UDP\",\"%s\",%d,%d", remote_ip, remote_port, local_port);
            }
        }
    }
    else
    {
        if(local_port == 0)
        {
            result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=,%d,\"UDP\",\"%s\",%d", link_id, remote_ip, remote_port);
        }
        else
        {
            if(mode != 0xFF)
            {
                result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=,%d,\"UDP\",\"%s\",%d,%d,%d", link_id, remote_ip, remote_port, local_port, mode);
            }
            else
            {
                result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=,%d,\"UDP\",\"%s\",%d,%d", link_id, remote_ip, remote_port, local_port);
            }
        }
    }
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
  * @brief  创建 SSL 客户端
  * @param  device at 设备句柄
  * @param  link_id ⽹络连接 ID (0~4)， ⽤于多连接的情况
  * @param  remote_ip 字符串参数，远端 IP 地址
  * @param  remote_port 远端端⼝号
  * @param  keep_alive_time 可选参数
  *         TCP keep-alive 侦测时间，默认关闭此功能
  *         0：关闭 TCP keep-alive 功能
  *         1~7200：侦测时间，单位为 1s
  * @retval 0-success others-error
  */
int at_tcpip_CIPSTART_ssl(struct at_device *device, unsigned int link_id,
                        const char *remote_ip, unsigned int remote_port, 
                        unsigned int keep_alive_time)
{
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    if(at_client_get_flag(device->client, AT_CLIENT_LINK_MUX_POS) == false)
    {
        if(keep_alive_time == 0)
        {
            result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=\"SSL\",\"%s\",%d", remote_ip, remote_port);
        }
        else
        {
            result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=\"SSL\",\"%s\",%d,%d", remote_ip, remote_port, keep_alive_time);
        }
    }
    else
    {
        if(keep_alive_time == 0)
        {
            result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=,%d,\"SSL\",\"%s\",%d", link_id, remote_ip, remote_port);
        }
        else
        {
            result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTART=,%d,\"SSL\",\"%s\",%d,%d", link_id, remote_ip, remote_port, keep_alive_time);
        }
    }
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


int at_tcpip_event_send(struct at_device *device, uint32_t event)
{
    return (int) at_osal_event_send(device->socket_event, event);
}

int at_tcpip_event_recv(struct at_device *device, uint32_t event, uint32_t timeout, rt_uint8_t option)
{
    int result = 0;
    rt_uint32_t recved;
    result = at_osal_event_recv(device->socket_event, event, option | RT_EVENT_FLAG_CLEAR, timeout, &recved);
    if (result != RT_EOK)
    {
        return -RT_ETIMEOUT;
    }
    return recved;
}
/**
  * @brief  发送数据
  * @param  device at 设备句柄
  * @param  link_id ⽹络连接 ID (0~4)， ⽤于多连接的情况
  * @param  buff 数据缓冲区
  * @param  size 数据缓冲区大小
  * @param  type 0-tcp  1-udp
  * @param  remote_ip 字符串参数，远端 IP 地址
  * @param  remote_port 远端端⼝号
  * @retval 0-success others-error
  */
int at_tcpip_CIPSEND(struct at_device *device, unsigned int link_id, 
                        const char *buff, unsigned int size, unsigned int type,
                        const char *remote_ip, unsigned int remote_port)
{
    int result = RT_EOK;
    int event_result = 0;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    unsigned int cur_pkt_size = 0, sent_size = 0;
    int device_socket = link_id;
    resp = at_create_resp(128, 2, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* set AT client end sign to deal with '>' sign */
    at_obj_set_end_sign(device->client, '>');
    while (sent_size < size)
    {
        if (size - sent_size < E103W11_MODULE_SEND_MAX_SIZE)
        {
            cur_pkt_size = size - sent_size;
        }
        else
        {
            cur_pkt_size = E103W11_MODULE_SEND_MAX_SIZE;
        }
        if(at_client_get_flag(device->client, AT_CLIENT_LINK_MUX_POS) == false)
        {
            if(type == 0 && remote_ip != RT_NULL && remote_port != 0)
            {
                if (at_obj_exec_cmd(device->client, resp, "AT+CIPSEND=%d,\"%s\",%d", cur_pkt_size, remote_ip, remote_port) < 0)
                {
                    result = RT_ERROR;
                    goto __exit;
                }
            }
            else
            {
                if (at_obj_exec_cmd(device->client, resp, "AT+CIPSEND=%d", cur_pkt_size) < 0)
                {
                    result = RT_ERROR;
                    goto __exit;
                }
            }
        }
        else
        {
            if(type == 0 && remote_ip != RT_NULL && remote_port != 0)
            {
                if (at_obj_exec_cmd(device->client, resp, "AT+CIPSEND=%d,%d,\"%s\",%d", link_id, cur_pkt_size, remote_ip, remote_port) < 0)
                {
                    result = RT_ERROR;
                    goto __exit;
                }
            }
            else
            {
                if (at_obj_exec_cmd(device->client, resp, "AT+CIPSEND=%d,%d", link_id, cur_pkt_size) < 0)
                {
                    result = RT_ERROR;
                    goto __exit;
                }
            }
        }
        if (result != RT_EOK)
        {
            goto __exit;
        }
        /* send the real data to server or client */
        result = (int) at_client_obj_send(device->client, buff + sent_size, cur_pkt_size);
        if (result == 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
        /* waiting result event from AT URC */
        if (at_tcpip_event_recv(device, SET_EVENT(device_socket, 0), 10 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR) < 0)
        {
            LOG_E("%s device socket(%d) wait connect result timeout.", device->name, device_socket);
            result = RT_ETIMEOUT;
            goto __exit;
        }
        /* waiting OK or failed result */
        event_result = at_tcpip_event_recv(device, AT_EVENT_TCPIP_SEND_OK | AT_EVENT_TCPIP_SEND_FAIL, 5 * RT_TICK_PER_SECOND, RT_EVENT_FLAG_OR);
        if (event_result < 0)
        {
            LOG_E("%s device socket(%d) wait connect OK|FAIL timeout.", device->name, device_socket);
            result = RT_ETIMEOUT;
            goto __exit;
        }
        /* check result */
        if (event_result & AT_EVENT_TCPIP_SEND_FAIL)
        {
            LOG_E("%s device socket(%d) send failed.", device->name, device_socket);
            result = RT_ERROR;
            goto __exit;
        }
        sent_size += cur_pkt_size;
    }
__exit:
    at_obj_set_end_sign(device->client, 0);
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  关闭 TCP/UDP/SSL 连接
  * @param  device at 设备句柄
  * @param  link_id ⽹络连接 ID (0~4)， ⽤于多连接的情况，需要关闭的连接 ID 号。当 ID 为 5 时，关闭所有连接
  * @retval 0-success others-error
  */
int at_tcpip_CIPCLOSE(struct at_device *device, unsigned int link_id)
{
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    if(at_client_get_flag(device->client, AT_CLIENT_LINK_MUX_POS) == false)
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+CIPCLOSE");
    }
    else
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+CIPCLOSE=,%d", link_id);
    }
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
  * @brief  创建 TCP/UDP 服务器
  * @param  device at 设备句柄
  * @param  mode 
  *         0：关闭服务器
  *         1：建⽴服务器
  * @param  port 端⼝号，默认为 333
  * @param  type 0-TCP 服务器  1-UDP 服务器
  * @retval 0-success others-error
  * @note 此命令要求：
  *         1. STA 正常获取地址
  *         2. 多连接情况下(AT+CIPMUX=1)，才能开启 TCP 服务器。
  *         3. 创建 TCP 服务器后， ⾃动建⽴TCP 服务器监听。
  *         4. 当有 TCP 客户端接⼊，会⾃动占⽤⼀个连接 ID。
  */
int at_tcpip_CIPSERVER(struct at_device *device, unsigned int mode, unsigned int port, unsigned int type)
{
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPSERVER=,%d,%d,%s", mode, port, (type == 0 ? "\"TCP\"" : "\"UDP\""));
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
  * @brief  获取传输模式(正常模式或透传模式）
  * @param  device at 设备句柄
  * @param  mode 输出：传输模式
  *         0：普通传输模式
  *         1：透传模式，仅⽀持 TCP 单连接和 UDP 固定通信对端的情况
  * @retval 0-success others-error
  */
int at_tcpip_CIPMODE_get(struct at_device *device, unsigned int *mode)
{
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPMODE?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(mode != RT_NULL)
    {
        if (at_resp_parse_line_args_by_kw(resp, "+CIPMODE:", "+CIPMODE:%d", mode) < 0)
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
  * @brief  设置传输模式(正常模式或透传模式）
  * @param  device at 设备句柄
  * @param  mode 传输模式
  *         0：普通传输模式
  *         1：透传模式，仅⽀持 TCP 单连接和 UDP 固定通信对端的情况
  * @retval 0-success others-error
  * @note   1. 本设置保存到 Flash。
  *         2. 透传模式传输时，如果连接断开，会不停尝试重连，此时单独输⼊+++退出透传，则停⽌重连；
  *         普通传输模式则不会重连，提示连接断开
  */
int at_tcpip_CIPMODE_set(struct at_device *device, unsigned int mode)
{
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPMODE=%d", mode);
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
  * @brief  保存传输链接并启用自动链接
  * @param  device at 设备句柄
  * @param  mode 传输模式
  *         0：取消开机透传
  *         1：保存开机进⼊透传模式
  * @param  remote_ip 远端 IP 或者域名
  * @param  remote_port 远端端⼝
  * @param  keep_alive_time TCP keep-alive 侦测,缺省默认关闭此功能
  *         0：关闭 TCP keep-alive 功能
  *         1~7200：侦测时间，单位为秒
  * @retval 0-success others-error
  * @note   1. STA 正常获取地址
  *         2. 本设置将透传模式及建⽴的 TCP 连接均保存在 Flash system parameter 区域，下次上电⾃动建⽴TCP 连接并进⼊透传
  *         3. 只要远端 IP，端⼝的值符合规范，本设置就会被保存到 Flash
  */
int at_tcpip_SAVETRANSLINK_tcp(struct at_device *device, unsigned int mode, const char *remote_ip, 
                               unsigned int remote_port, unsigned int keep_alive_time)
{
    if(device == RT_NULL || remote_ip == RT_NULL || keep_alive_time > 7200)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPMODE=%d,%s,%d,\"TCP\",%d", mode, remote_ip, remote_port, keep_alive_time);
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
  * @brief  保存传输链接并启用自动链接
  * @param  device at 设备句柄
  * @param  mode 传输模式
  *         0：取消开机透传
  *         1：保存开机进⼊透传模式
  * @param  remote_ip 远端 IP 或者域名
  * @param  remote_port 远端端⼝
  * @param  local_port 开机进⼊UDP 传输时，使⽤的本地端⼝
  * @retval 0-success others-error
  */
int at_tcpip_SAVETRANSLINK_udp(struct at_device *device, unsigned int mode, const char *remote_ip, 
                               unsigned int remote_port, unsigned int local_port)
{
    if(device == RT_NULL || remote_ip == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPMODE=%d,%s,%d,\"UDP\",%d", mode, remote_ip, remote_port, local_port);
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
  * @brief  获取自动接收数据模式
  * @param  device at 设备句柄
  * @param  mode 输出：TCP 连接的数据接收⽅式
  *         0：主动模式；收到 TCP 数据时，通过+IPD 主动将数据通过串⼝发给 MCU
  *         1：被动模式；默认使⽤2920 bytes 的 buffer⽤于缓存接收到的 TCP 数据，被动等待 MCU 读取；
  *         接收buffer 满后，将阻塞对端 TCP 发送数据
  * @retval 0-success others-error
  */
int at_tcpip_CIPRECVMODE_get(struct at_device *device, unsigned int *mode)
{
    if(device == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPRECVMODE?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(mode != RT_NULL)
    {
        if (at_resp_parse_line_args_by_kw(resp, "+CIPRECVMODE:", "+CIPRECVMODE:%d", mode) < 0)
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
  * @brief  设置自动接收数据模式
  * @param  device at 设备句柄
  * @param  mode TCP 连接的数据接收⽅式
  *         0：主动模式；收到 TCP 数据时，通过+IPD 主动将数据通过串⼝发给 MCU
  *         1：被动模式；默认使⽤2920 bytes 的 buffer⽤于缓存接收到的 TCP 数据，被动等待 MCU 读取；
  *         接收buffer 满后，将阻塞对端 TCP 发送数据
  * @retval 0-success others-error
  */
int at_tcpip_CIPRECVMODE_set(struct at_device *device, unsigned int mode)
{
    if(device == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPRECVMODE=%d", mode);
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
  * @brief  获得 socket 数据(被动模式)
  * @param  device at 设备句柄
  * @param  link_id 多连接的连接 ID 号
  * @param  len 读取的数据⻓度，每次最多可读取 2048 字节
  * @param  data 输出：存储读取的数据
  * @retval 0-success others-error
  */
int at_tcpip_CIPRECVDATA(struct at_device *device, unsigned int link_id, unsigned int len, char data[])
{
    if(device == RT_NULL || len == 0 || len > 2048 || data == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64+len, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    if(at_client_get_flag(device->client, AT_CLIENT_LINK_MUX_POS) == false)
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+CIPRECVDATA=%d", len);
    }
    else
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+CIPRECVDATA=%d,%d", link_id, len);
    }
    if (result != RT_EOK)
    {
        goto __exit;
    }
    unsigned int len_temp = 0;
    if (at_resp_parse_line_args_by_kw(resp, "+CIPRECVDATA", "+CIPRECVDATA,%d", &len_temp) < 0)
    {
        result = RT_ERROR;
        goto __exit;
    }
    ptr = (char *)at_resp_get_line_by_kw(resp, "+CIPRECVDATA");
    if(ptr != RT_NULL)
    {
        ptr = rt_strstr(resp->buf, ":");
        if(ptr != RT_NULL)
        {
            ptr++;
            for (unsigned int i = 0; i < len_temp && i < len; i++)
            {
                data[i] = *(ptr+i);
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
  * @brief  PING 命令
  * @param  device at 设备句柄
  * @param  ip IP 地址
  * @retval 0-success others-error
  */
int at_tcpip_PING(struct at_device *device, const char *ip)
{
    if(device == RT_NULL || ip == RT_NULL)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+PING=\"%s\"", ip);
    if (result != RT_EOK)
    {
        goto __exit;
    }
    int req_time;
    if (at_resp_parse_line_args_by_kw(resp, "+", "+%d", &req_time) < 0)
    {
        result = RT_ERROR;
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
  * @brief  查询 station mac 地址
  * @param  device at 设备句柄
  * @param  mac_cur 输出：Station 的 MAC 地址(当前使用的配置)
  * @param  mac_cur_len mac_cur缓冲区的大小
  * @param  mac_def 输出：Station 的 MAC 地址(FLASH 上保存的配置)
  * @param  mac_def_len mac_def缓冲区的大小
  * @retval 0-success others-error
  */
int at_tcpip_CIPSTAMAC_get(struct at_device *device, 
                            char *mac_cur, unsigned int mac_cur_len, 
                            char *mac_def, unsigned int mac_def_len)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTAMAC?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(mac_cur != RT_NULL && mac_cur_len > 0)
    {
        char format[48];
        //"+CIPSTAMAC_CUR:\"%16[^\"]\""
        rt_sprintf(format, "+CIPSTAMAC_CUR:\"%c%d[^\"]\"", '%', mac_cur_len);
        if (at_resp_parse_line_args_by_kw(resp, "+CIPSTAMAC_CUR:", format, mac_cur) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
    }
    if(mac_def != RT_NULL && mac_def_len > 0)
    {
        char format[48];
        //"+CIPSTAMAC_CUR:\"%16[^\"]\""
        rt_sprintf(format, "+CIPSTAMAC_DEF:\"%c%d[^\"]\"", '%', mac_def_len);
        if (at_resp_parse_line_args_by_kw(resp, "+CIPSTAMAC_DEF:", format, mac_def) < 0)
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
  * @brief  设置 station mac 地址
  * @param  device at 设备句柄
  * @param  mac Station 的 MAC 地址
  * @param  save_flash 是否保存到Flash
  * @retval 0-success others-error
  */
int at_tcpip_CIPSTAMAC_set(struct at_device *device, const char *mac, bool save_flash)
{
    if(device == RT_NULL || mac == RT_NULL)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTAMAC=\"%s\",\"%s\"", mac, (save_flash ? "\"s.y\"" : "\"s.n\""));
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
  * @brief  查询 station mac 地址
  * @param  device at 设备句柄
  * @param  cur 输出：ip 网关 掩码结构体(当前使用的配置)
  * @param  def 输出：ip 网关 掩码结构体(Flash保存的配置)
  * @retval 0-success others-error
  */
int at_tcpip_CIPSTA_get(struct at_device *device, struct s_tcpip_net_info *cur, struct s_tcpip_net_info *def)
{
    if(device == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(256, 6, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTA?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(cur != RT_NULL)
    {
        char format[48];
        //"+CIPSTA_CUR:\"%16[^\"]\""
        rt_sprintf(format, "+CIPSTA_CUR:\"%c%d[^\"]\"", '%', sizeof(cur->ip));
        if (at_resp_parse_line_args(resp, 1, format, cur->ip) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
        if (at_resp_parse_line_args(resp, 2, format, cur->gateway) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
        if (at_resp_parse_line_args(resp, 3, format, cur->mask) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
    }
    if(def != RT_NULL)
    {
        char format[48];
        //"+CIPSTA_DEF:\"%16[^\"]\""
        rt_sprintf(format, "+CIPSTA_DEF:\"%c%d[^\"]\"", '%', sizeof(def->ip));
        if (at_resp_parse_line_args(resp, 1, format, def->ip) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
        if (at_resp_parse_line_args(resp, 2, format, def->gateway) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
        if (at_resp_parse_line_args(resp, 3, format, def->mask) < 0)
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
  * @brief  设置 station mac 地址
  * @param  device at 设备句柄
  * @param  cur ip 网关 掩码结构体(当前使用的配置)
  * @param  save_flash 是否保存到Flash
  * @retval 0-success others-error
  * @note 执行此命令会关闭 DHCP 客户端。
  */
int at_tcpip_CIPSTA_set(struct at_device *device, const struct s_tcpip_net_info *cur, bool save_flash)
{
    if(device == RT_NULL || cur == RT_NULL)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPSTA=\"%s\",\"%s\",\"%s\",\"%s\"", 
                            cur->ip, cur->gateway, cur->mask, (save_flash ? "\"s.y\"" : "\"s.n\""));
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
  * @brief  配置 softap IP 地址
  * @param  device at 设备句柄
  * @param  cur 输出：ip 网关 掩码结构体(当前使用的配置)
  * @param  def 输出：ip 网关 掩码结构体(Flash保存的配置)
  * @retval 0-success others-error
  */
int at_tcpip_CIPAP_get(struct at_device *device, struct s_tcpip_net_info *cur, struct s_tcpip_net_info *def)
{
    if(device == RT_NULL)
    {
        return RT_ERROR;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(256, 6, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPAP?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(cur != RT_NULL)
    {
        char format[48];
        //"+CIPAP_CUR:\"%16[^\"]\""
        rt_sprintf(format, "+CIPAP_CUR:\"%c%d[^\"]\"", '%', sizeof(cur->ip));
        if (at_resp_parse_line_args(resp, 1, format, cur->ip) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
        if (at_resp_parse_line_args(resp, 2, format, cur->gateway) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
        if (at_resp_parse_line_args(resp, 3, format, cur->mask) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
    }
    if(def != RT_NULL)
    {
        char format[48];
        //"+CIPAP_DEF:\"%16[^\"]\""
        rt_sprintf(format, "+CIPAP_DEF:\"%c%d[^\"]\"", '%', sizeof(def->ip));
        if (at_resp_parse_line_args(resp, 1, format, def->ip) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
        if (at_resp_parse_line_args(resp, 2, format, def->gateway) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
        if (at_resp_parse_line_args(resp, 3, format, def->mask) < 0)
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
  * @brief  配置 softap IP 地址
  * @param  device at 设备句柄
  * @param  cur ip 网关 掩码结构体(当前使用的配置)
  * @param  save_flash 是否保存到Flash
  * @retval 0-success others-error
  * @note 执行此命令会关闭 DHCP 客户端。
  */
int at_tcpip_CIPAP_set(struct at_device *device, const struct s_tcpip_net_info cur, bool save_flash)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+CIPAP=\"%s\",\"%s\",\"%s\",\"%s\"", 
                            cur.ip, cur.gateway, cur.mask, (save_flash ? "\"s.y\"" : "\"s.n\""));
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
#endif /* AT_DEVICE_INTERFACE_TCPIP */
static const command_desc_t ble_commands[] = {
    {"TCPIP_CIPSTATUS",     &e103w11_tcpip_CIPSTATUS,       AT_DEVICE_CTRL_TCPIP_CIPSTATUS},
    {"TCPIP_CIPMUX",        &e103w11_tcpip_CIPMUX,          AT_DEVICE_CTRL_TCPIP_CIPMUX},
    {"TCPIP_CIPSTART",      &e103w11_tcpip_CIPSTART,        AT_DEVICE_CTRL_TCPIP_CIPSTART},
    {"TCPIP_CIPSEND",       &e103w11_tcpip_CIPSEND,         AT_DEVICE_CTRL_TCPIP_CIPSEND},
    {"TCPIP_CIPCLOSE",      &e103w11_tcpip_CIPCLOSE,        AT_DEVICE_CTRL_TCPIP_CIPCLOSE},
    {"TCPIP_CIPSERVER",     &e103w11_tcpip_CIPSERVER,       AT_DEVICE_CTRL_TCPIP_CIPSERVER},
    {"TCPIP_CIPMODE",       &e103w11_tcpip_CIPMODE,         AT_DEVICE_CTRL_TCPIP_CIPMODE},
    {"TCPIP_SAVETRANSLINK", &e103w11_tcpip_SAVETRANSLINK,   AT_DEVICE_CTRL_TCPIP_SAVETRANSLINK},
    {"TCPIP_CIPRECVMODE",   &e103w11_tcpip_CIPRECVMODE,     AT_DEVICE_CTRL_TCPIP_CIPRECVMODE},
    {"TCPIP_CIPRECVDATA",   &e103w11_tcpip_CIPRECVDATA,     AT_DEVICE_CTRL_TCPIP_CIPRECVDATA},
    {"TCPIP_PING",          &e103w11_tcpip_PING,            AT_DEVICE_CTRL_TCPIP_PING},
    {"TCPIP_CIPSTAMAC",     &e103w11_tcpip_CIPSTAMAC,       AT_DEVICE_CTRL_TCPIP_CIPSTAMAC},
    {"TCPIP_CIPSTA",        &e103w11_tcpip_CIPSTA,          AT_DEVICE_CTRL_TCPIP_CIPSTA},
    {"TCPIP_CIPAP",         &e103w11_tcpip_CIPAP,           AT_DEVICE_CTRL_TCPIP_CIPAP},
};

void at_register_tcpip_commands(struct at_device *device)
{
    at_register_command_group(device, "BLE", ble_commands, sizeof(ble_commands) / sizeof(command_desc_t), 0);
}
#endif /* AT_DEVICE_USING_E103W11 */