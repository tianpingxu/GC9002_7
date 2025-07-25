#include "e103w11/wifi_command/at_wifi_command.h"
#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)

/* [AT+CWMODE] */
static int e103w11_wifi_CWMODE(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CWMODE?" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+CWMODE?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+CWMODE=%s", arg);//AT+CWMode=1,"s.y" 设置为 SoftAP 模式并保存 Flash。
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
/* [AT+CWJAP] */
static int e103w11_wifi_CWJAP(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CWJAP?" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+CWJAP?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+CWJAP=%s", arg);
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
/* [AT+CWLAP] */
static int e103w11_wifi_CWLAP(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CWLAP?" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+CWLAP?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+CWLAP=%s", arg);
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
/* [AT+CWQAP] */
static int e103w11_wifi_CWQAP(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CWQAP?" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+CWQAP?");
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
/* [AT+CWSAP] */
static int e103w11_wifi_CWSAP(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CWSAP?" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+CWSAP?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+CWSAP=%s", arg);
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
/* [AT+CWDHCP] */
static int e103w11_wifi_CWDHCP(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CWDHCP?" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+CWDHCP?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+CWDHCP=%s", arg);//AT+CWDHCP=1,1,"s.y" 关闭 SoftAP 的 DHCP 服务器并保存 Flash。
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
/* [AT+CWDHCPS] */
static int e103w11_wifi_CWDHCPS(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CWDHCPS?" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+CWDHCPS?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+CWDHCPS=%s", arg);//AT+CWDHCPS=0,"s.n" 关闭 DHCP 服务器，不保存 FLASH。
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
/* [AT+CWAUTOCONN] */
static int e103w11_wifi_CWAUTOCONN(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CWAUTOCONN?" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+CWAUTOCONN=%s", arg);
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



#if(defined AT_DEVICE_INTERFACE_WIFI && AT_DEVICE_INTERFACE_WIFI == 1U)

/**
  * @brief  查询Wi-Fi 工作模式
  * @param  device at 设备句柄
  * @param  mode_cur 输出：当前使用的配置
  * @param  mode_def 输出：Flash 上保存的配置
  * @retval 0-success others-error
  */
int at_wifi_CWMODE_get(struct at_device *device, unsigned int *mode_cur, unsigned int *mode_def)
{
    if(device == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 2, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+CWMODE?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(mode_cur != RT_NULL)
    {
        if (at_resp_parse_line_args_by_kw(resp, "+CWMODE_CUR", "+CWMODE_CUR:%d", mode_cur) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
    }
    if(mode_def != RT_NULL)
    {
        if (at_resp_parse_line_args_by_kw(resp, "+CWMODE_DEF", "+CWMODE_DEF:%d", mode_def) < 0)
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
  * @brief  查询Wi-Fi 工作模式
  * @param  device at 设备句柄
  * @param  mode 工作模式
  * @param  save_flash false-不保存到Flash  true-保存到Flash
  * @retval 0-success others-error
  */
int at_wifi_CWMODE_set(struct at_device *device, unsigned int mode, bool save_flash)
{
    if(device == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 2, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    if(save_flash)
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+CWMODE=%d,\"s.y\"", mode);
    }
    else
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+CWMODE=%d,\"s.n\"", mode);
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
  * @brief  查找 AP
  * @param  device at 设备句柄
  * @param  cur 输出：当前工作模式结构体指针
  * @param  def 输出：存储在Flash工作模式结构体指针
  * @retval 0-success others-error
  */
int at_wifi_CWJAP_get(struct at_device *device, struct at_wifi_CWJAP *cur, struct at_wifi_CWJAP *def)
{
    if(device == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(256, 2, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+CWJAP?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(def != RT_NULL)
    {
        char format[48];
        //"+CWJAP_DEF:\"%24[^\"]\",\"%24[^\"]\",%d,%d"
        rt_sprintf(format, "+CWJAP_DEF:\"%c%d[^\"]\",\"%c%d[^\"]\",%cd,%cd", 
                    '%', AT_WIFI_SSID_LENGTH, '%', AT_WIFI_MAC_ADDR_LENGTH, '%', '%');
        if (at_resp_parse_line_args_by_kw(resp, "+CWJAP_DEF:", format, def->ssid, def->mac_addr, &def->ch, &def->sig_inte) < 0)
        {
            result = RT_ERROR;
        }
    }
    if(cur != RT_NULL)
    {
        char format[48];
        //"+CWJAP_DEF:\"%64[^\"]\",\"%24[^\"]\",%d,%d"
        rt_sprintf(format, "+CWJAP_CUR:\"%c%d[^\"]\",\"%c%d[^\"]\",%cd,%cd", 
                    '%', AT_WIFI_SSID_LENGTH, '%', AT_WIFI_MAC_ADDR_LENGTH, '%', '%');
        if (at_resp_parse_line_args_by_kw(resp, "+CWJAP_CUR:", format, cur->ssid, cur->mac_addr, &cur->ch, &cur->sig_inte) < 0)
        {
            result = RT_ERROR;
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
  * @brief  连接 AP
  * @param  device at 设备句柄
  * @param  ssid SSID-无线网络名称
  * @param  passwd 输出：无线网络密码
  * @param  save_flash false-不保存到Flash  true-保存到Flash
  * @retval 0-success others-error
  */
int at_wifi_CWJAP_set(struct at_device *device, const char *ssid, const char *passwd, bool save_flash)
{
    if(device == RT_NULL || ssid == RT_NULL || passwd == RT_NULL || 
        rt_strlen(ssid) > AT_WIFI_SSID_LENGTH || 
        rt_strlen(passwd) > AT_WIFI_PASSWORD_LENGTH)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    if(save_flash)
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+CWJAP=%s,%s,\"s.y\"", ssid, passwd);
    }
    else
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+CWJAP=%s,%s,\"s.n\"", ssid, passwd);
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
  * @brief  扫描 AP
  * @param  device at 设备句柄
  * @param  cwlap 输出： +CWLAP:(3,"MI_6",-59,"20:47:da:49:1e:c6",13,0,4)
  *                     +CWLAP:(3,"personal",-95,"c2:68:e6:76:55:bb",11,0,4)
  * @param  max_num 输出缓冲区个数
  * @retval 0-success others-error
  */
int at_wifi_CWLAP_get(struct at_device *device, struct at_wifi_CWLAP cwlap[], unsigned int max_num)
{
    if(device == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(1280, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+CWLAP?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(cwlap != RT_NULL && max_num > 0)
    {
        char format[48];
        //"+CWJAP_DEF:\"%64[^\"]\",\"%24[^\"]\",%d,%d"
        rt_sprintf(format, "+CWLAP:(%cd,\"%c%d[^\"]\",%cd,\"%c%d[^\"]\",%cd,%cd,%cd)", 
                    '%', '%', AT_WIFI_SSID_LENGTH, '%', '%', AT_WIFI_MAC_ADDR_LENGTH, '%', '%', '%');
        for(int i = 0; i < max_num && i < resp->line_counts; i++)
        {
            if (at_resp_parse_line_args(resp, i+1, format, 
                                            &cwlap[i].auth_mode, 
                                            cwlap[i].ssid, 
                                            &cwlap[i].signal_strength, 
                                            cwlap[i].mac_addr, 
                                            &cwlap[i].ch, 
                                            &cwlap[i].freq_offset, 
                                            &cwlap[i].encry_algo) < 0)
            {
                result = RT_ERROR;
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
  * @brief  扫描 AP
  * @param  device at 设备句柄
  * @param  ssid 无线网络名称, NULL-扫描所有的 AP
  * @param  bssid 无线网络 MAC 地址, NULL-扫描所有的 AP
  * @retval 0-success others-error
  */
int at_wifi_CWLAP_set(struct at_device *device, const char *ssid, const char *bssid)
{
    if(device == RT_NULL || ssid == RT_NULL || bssid == RT_NULL || 
        rt_strlen(ssid) > AT_WIFI_SSID_LENGTH || 
        rt_strlen(bssid) > AT_WIFI_SSID_LENGTH)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+CWLAP=\"%s\",\"%s\"", ssid, bssid);
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
  * @brief  与 AP 断开连接
  * @param  device at 设备句柄
  * @retval 0-success others-error
  */
int at_wifi_CWQAP(struct at_device *device)
{
    if(device == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+CWQAP?");
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
  * @brief  查询设置 AP 模式
  * @param  device at 设备句柄
  * @param  s_cwsap 
  * @retval 0-success others-error
  */
int at_wifi_CWSAP_get(struct at_device *device, struct at_wifi_CWSAP *s_cwsap)
{
    if(device == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(192, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+CWSAP?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(s_cwsap != RT_NULL)
    {
        char format[48];
        //"+CWSAP_CUR:\"%64[^\"]\",\"%24[^\"]\",%d,%d,%d,%d"
        rt_sprintf(format, "+CWSAP_CUR:\"%c%d[^\"]\",\"%c%d[^\"]\",%cd,%cd,%cd,%cd", 
                    '%', AT_WIFI_SSID_LENGTH, '%', AT_WIFI_PASSWORD_LENGTH, '%', '%', '%', '%');
        if (at_resp_parse_line_args_by_kw(resp, "+CWSAP_CUR:", format, 
                                        s_cwsap->ssid, s_cwsap->passwd, &s_cwsap->ch, 
                                        &s_cwsap->auth_mode, &s_cwsap->max_con, &s_cwsap->hidden_ssid) < 0)
        {
            result = RT_ERROR;
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
  * @brief  设置 AP 模式
  * @param  device at 设备句柄
  * @param  s_cwsap 
  * @param  save_flash 是否保存到Flash
  * @retval 0-success others-error
  */
int at_wifi_CWSAP_set(struct at_device *device, const struct at_wifi_CWSAP *s_cwsap, bool save_flash)
{
    if(device == RT_NULL || s_cwsap == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    if(save_flash)
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+CWSAP=\"%s\",\"%s\",%d,%d,%d,%d,\"s.y\"", 
                                s_cwsap->ssid, s_cwsap->passwd, s_cwsap->ch, s_cwsap->auth_mode, s_cwsap->max_con, s_cwsap->hidden_ssid);
    }
    else
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+CWSAP=\"%s\",\"%s\",%d,%d,%d,%d,\"s.n\"", 
                                s_cwsap->ssid, s_cwsap->passwd, s_cwsap->ch, s_cwsap->auth_mode, s_cwsap->max_con, s_cwsap->hidden_ssid);
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
  * @brief  查询DHCP 模式
  * @param  device at 设备句柄
  * @param  SoftAP_status_cur 输出：SoftAP 的 DHCP 服务器是否开启， 0 关闭， 1 开启（当前配置）
  * @param  Station_status_cur 输出：Station 的 DHCP 客户端是否开启， 0 关闭， 1开启（当前配置）
  * @param  SoftAP_status_def 输出：SoftAP 的 DHCP 服务器是否开启， 0 关闭， 1 开启（Flash保存的配置）
  * @param  Station_status_def 输出：Station 的 DHCP 客户端是否开启， 0 关闭， 1开启（Flash保存的配置）
  * @retval 0-success others-error
  */
int at_wifi_CWDHCP_get(struct at_device *device, 
                        unsigned int *SoftAP_status_cur, unsigned int *Station_status_cur, 
                        unsigned int *SoftAP_status_def, unsigned int *Station_status_def)
{
    if(device == RT_NULL)
    {
        return RT_EINVAL;
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
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+CWDHCP?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    int val = 0;
    if (at_resp_parse_line_args_by_kw(resp, "+CWDHCP_CUR", "+CWDHCP_CUR:%d", &val) < 0)
    {
        result = RT_ERROR;
        goto __exit;
    }
    if(SoftAP_status_cur != RT_NULL)
    {
        *SoftAP_status_cur = (val & 0x1);
    }
    if(Station_status_cur != RT_NULL)
    {
        *Station_status_cur = (val & 0x2);
    }
    val = 0;
    if (at_resp_parse_line_args_by_kw(resp, "+CWDHCP_DEF", "+CWDHCP_DEF:%d", &val) < 0)
    {
        result = RT_ERROR;
        goto __exit;
    }
    if(SoftAP_status_def != RT_NULL)
    {
        *SoftAP_status_def = (val & 0x1);
    }
    if(Station_status_def != RT_NULL)
    {
        *Station_status_def = (val & 0x2);
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
/**
  * @brief  设置 DHCP 模式
  * @param  device at 设备句柄
  * @param  interface 
  *         0： SoftAP，控制 SoftAP 的 DHCP 服务器
  *         1： Station，控制 Station 的 DHCP 客户端
  * @param  flag 0：关闭  1：开启
  * @param  save_flash 是否保存到Flash
  * @retval 0-success others-error
  */
int at_wifi_CWDHCP_set(struct at_device *device, unsigned int interface, unsigned int flag, bool save_flash)
{
    if(device == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    if(save_flash)
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+CWDHCP=%d,%d,\"s.y\"", interface, flag);
    }
    else
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+CWDHCP=%d,%d,\"s.n\"", interface, flag);
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
  * @brief  查询设置 DHCP 规则和网关
  * @param  device at 设备句柄
  * @param  s_cwdhcps 输出
  * @retval 0-success others-error
  */
int at_wifi_CWDHCPS_get(struct at_device *device, struct at_wifi_CWDHCPS *cur, struct at_wifi_CWDHCPS *def)
{
    if(device == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    char *ptr = RT_NULL;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 2, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+CWDHCPS?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(cur != RT_NULL)
    {
        char format[48];
        //"+CWDHCPS_CUR:\"%16[^\"]\",\"%16[^\"]\",%d,%d,%d,%d"
        rt_sprintf(format, "+CWDHCPS_CUR:%cd,\"%c%d[^\"]\",\"%c%d[^\"]\"", 
                    '%', '%', AT_WIFI_IP_LENGTH, '%', AT_WIFI_IP_LENGTH);
        if (at_resp_parse_line_args_by_kw(resp, "+CWDHCPS_CUR:", format, &cur->time, cur->start_ip, cur->end_ip) < 0)
        {
            result = RT_ERROR;
        }
    }
    if(def != RT_NULL)
    {
        char format[48];
        //"+CWDHCPS_DEF:\"%16[^\"]\",\"%16[^\"]\",%d,%d,%d,%d"
        rt_sprintf(format, "+CWDHCPS_DEF:%cd,\"%c%d[^\"]\",\"%c%d[^\"]\"", 
                    '%', '%', AT_WIFI_IP_LENGTH, '%', AT_WIFI_IP_LENGTH);
        if (at_resp_parse_line_args_by_kw(resp, "+CWDHCPS_DEF:", format, &def->time, def->start_ip, def->end_ip) < 0)
        {
            result = RT_ERROR;
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
  * @brief  设置 DHCP 模式
  * @param  device at 设备句柄
  * @param  enable 
  *         0：清除 DHCP 服务器配置到默认值。此时不需要<leasetime>,<start IP>,<end IP>配置
  *         1：配置 DHCP 服务器
  * @param  lease_time 租期，单位秒
  * @param  start_ip 地址池的起始地址
  * @param  end_ip 地址池的结束地址
  * @param  save_flash 是否保存到Flash
  * @retval 0-success others-error
  */
int at_wifi_CWDHCPS_set(struct at_device *device, 
				 	unsigned int enable, 
					unsigned int lease_time, 
					const char *start_ip, 
					const char *end_ip, 
					bool save_flash)
{
    if(device == RT_NULL || start_ip == RT_NULL || end_ip == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    if(enable)
    {
        if(save_flash)
        {
            result = at_obj_exec_cmd(device->client, resp, "AT+CWDHCP=1,%d,%s,%s,\"s.y\"", lease_time, start_ip, end_ip);
        }
        else
        {
            result = at_obj_exec_cmd(device->client, resp, "AT+CWDHCP=1,%d,%s,%s,\"s.n\"", lease_time, start_ip, end_ip);
        }
    }
    else
    {
        result = at_obj_exec_cmd(device->client, resp, (save_flash ? "AT+CWDHCPS=0,\"s.y\"" : "AT+CWDHCPS=0,\"s.n\""));
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
  * @brief  设置自动连接
  * @param  device at 设备句柄
  * @param  enable 
  *         false：上电后不自动关联 AP
  *         true：上电后自动关联 AP
  * @retval 0-success others-error
  */
int at_wifi_CWAUTOCONN(struct at_device *device, bool enable)
{
    if(device == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, (enable ? "AT+CWAUTOCONN=1" : "AT+CWAUTOCONN=0"));
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
#endif /* AT_DEVICE_INTERFACE_WIFI */

static const command_desc_t wifi_commands[] = {
    {"WIFI_CWMODE",     &e103w11_wifi_CWMODE,       AT_DEVICE_CTRL_WIFI_CWMODE},
    {"WIFI_CWJAP",      &e103w11_wifi_CWJAP,        AT_DEVICE_CTRL_WIFI_CWJAP},
    {"WIFI_CWLAP",      &e103w11_wifi_CWLAP,        AT_DEVICE_CTRL_WIFI_CWLAP},
    {"WIFI_CWQAP",      &e103w11_wifi_CWQAP,        AT_DEVICE_CTRL_WIFI_CWQAP},
    {"WIFI_CWSAP",      &e103w11_wifi_CWSAP,        AT_DEVICE_CTRL_WIFI_CWSAP},
    {"WIFI_CWDHCP",     &e103w11_wifi_CWDHCP,       AT_DEVICE_CTRL_WIFI_CWDHCP},
    {"WIFI_CWDHCPS",    &e103w11_wifi_CWDHCPS,      AT_DEVICE_CTRL_WIFI_CWDHCPS},
    {"WIFI_CWAUTOCONN", &e103w11_wifi_CWAUTOCONN,   AT_DEVICE_CTRL_WIFI_CWAUTOCONN},
};

void at_register_wifi_commands(struct at_device *device)
{
    at_register_command_group(device, "BLE", wifi_commands, sizeof(wifi_commands) / sizeof(command_desc_t), 0);
}
#endif /* AT_DEVICE_USING_E103W11 */