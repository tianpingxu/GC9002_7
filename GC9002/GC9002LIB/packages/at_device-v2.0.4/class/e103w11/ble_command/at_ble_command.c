#include "e103w11/ble_command/at_ble_command.h"
#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)

/* Get Bluetooth LE device address [AT+BLEADDR] */
static int e103w11_ble_BLEADDR(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+BLEADDR" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+BLEADDR?");
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
/* Start Bluetooth LE broadcast [AT+BLEADVSTART] */
static int e103w11_ble_BLEADVSTART(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+BLEADVSTART" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+BLEADVSTART");
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
/* Stop Bluetooth LE broadcast [AT+BLEADVSTOP] */
static int e103w11_ble_BLEADVSTOP(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+BLEADVSTOP" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+BLEADVSTOP");
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
/* Bluetooth LE Disconnect [AT+BLEDISCONN] */
static int e103w11_ble_BLEDISCONN(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+BLEDISCONN" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+BLEDISCONN");
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
/* Set/Get Bluetooth LE device name [AT+BLENAME] */
static int e103w11_ble_BLENAME(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+BLENAME" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+BLENAME?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+BLENAME=%s", arg);
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
/* Enable Bluetooth LE device scan [AT+BLESCAN] */
static int e103w11_ble_BLESCAN(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+BLESCAN" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+BLESCAN");
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


#if(defined AT_DEVICE_INTERFACE_BLE && AT_DEVICE_INTERFACE_BLE == 1U)

extern int e103w11_device_cmd_no_result(struct at_device *device, const char *cmd, int timeout);
extern int e103w11_device_cmd_ret_string(struct at_device *device, const char *cmd, int timeout, 
                                        unsigned int resp_line, char *buff_str, unsigned int size);

/**
  * @brief  Bluetooth LE 设备地址查询
  * @param  device at 设备句柄
  * @param  addr 输出：设备地址  eg:<aa:bb:c2:d3:dd:2a>
  * @param  size addr输出缓存区大小
  * @retval 0-success others-error
  */
int at_ble_BLEADDR(struct at_device *device, char *addr, unsigned int size)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+BLEADDR?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(addr != RT_NULL && size > 0)
    {
        char format[24];
        rt_sprintf(format, "+BLEADDR:\"%c%d[^\"]\"", '%', size);
        if (at_resp_parse_line_args_by_kw(resp, "+BLEADDR:", format, addr) < 0)
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
int at_ble_BLEADVSTART(struct at_device *device)
{
    return e103w11_device_cmd_no_result(device, "AT+BLEADVSTART", AT_TIMEOUT_NORMAL);
}
int at_ble_BLEADVSTOP(struct at_device *device)
{
    return e103w11_device_cmd_no_result(device, "AT+BLEADVSTOP", AT_TIMEOUT_NORMAL);
}
int at_ble_BLEDISCONN(struct at_device *device)
{
    return e103w11_device_cmd_no_result(device, "AT+BLEDISCONN", AT_TIMEOUT_NORMAL);
}
/**
  * @brief  Bluetooth LE 设备地址查询
  * @param  device at 设备句柄
  * @param  addr 输出：设备地址  eg:<aa:bb:c2:d3:dd:2a>
  * @param  size addr输出缓存区大小
  * @retval 0-success others-error
  */
int at_ble_BLENAME_get(struct at_device *device, char *name, unsigned int size)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+BLENAME?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(name != RT_NULL && size > 0)
    {
        char format[24];
        rt_sprintf(format, "+BLENAME:\"%c%d[^\"]\"", '%', size);
        if (at_resp_parse_line_args_by_kw(resp, "+BLENAME:", format, name) < 0)
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
  * @brief  Bluetooth LE 设备地址查询
  * @param  device at 设备句柄
  * @param  addr 设备地址  eg:<aa:bb:c2:d3:dd:2a>
  * @param  size addr输出缓存区大小
  * @retval 0-success others-error
  */
int at_ble_BLENAME_set(struct at_device *device, const char *name)
{
    if(device == RT_NULL || name == RT_NULL)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+BLENAME=\"%s\"", name);
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
  * @brief  使能 Bluetooth LE 扫描
  * @param  device at 设备句柄
  * @param  rssi 输出：信号强度
  * @param  addr 输出：设备地址
  * @param  addr_size 输出缓存区大小
  * @param  data 扫描响应数据
  * @param  data_size 输出缓存区大小
  * @retval 0-success others-error
  */
int at_ble_BLESCAN(struct at_device *device, 
				   unsigned int *rssi, 
				   char *addr, 
				   unsigned int addr_size,
				   unsigned char *data,
				   unsigned int data_size)
{
    if(device == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    char *ptr = RT_NULL;
    unsigned int length = 0;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(256, 2, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+BLESCAN");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    ptr = (char *)at_resp_get_line(resp, 1);
    if(rt_memcmp(ptr, "OK", 2) == 0)
    {
        ptr = (char *)at_resp_get_line(resp, 2);
    }
    if(ptr != RT_NULL && rssi != RT_NULL && addr != RT_NULL && data != RT_NULL)
    {
        *rssi = atoi(ptr);
        ptr = rt_strstr(ptr, ",");
        if(ptr != RT_NULL)
        {
            ptr++;
            length = atoi(ptr);
            ptr = rt_strstr(ptr, ",");
            if(ptr != RT_NULL)
            {
                ptr++;
                for(int i = 0; i < addr_size; i++)
                {
                    if(*ptr == ',')
                    {
                        addr[i] = '\0';
                        break;
                    }
                    addr[i] = *ptr;
                    ptr++;
                }
                ptr++;
                for(int i = 0; i < data_size; i++)
                {
                    if(*ptr == '\r' || *ptr == 0)
                    {
                        data[i] = '\0';
                        break;
                    }
                    data[i] = *ptr;
                    ptr++;
                }
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
#endif /* AT_DEVICE_INTERFACE_BLE */

static const command_desc_t ble_commands[] = {
    {"BLEADDR",     &e103w11_ble_BLEADDR,       AT_DEVICE_CTRL_BLE_BLEADDR},
    {"BLEADVSTART", &e103w11_ble_BLEADVSTART,   AT_DEVICE_CTRL_BLE_BLEADVSTART},
    {"BLEADVSTOP",  &e103w11_ble_BLEADVSTOP,    AT_DEVICE_CTRL_BLE_BLEADVSTOP},
    {"BLEDISCONN",  &e103w11_ble_BLEDISCONN,    AT_DEVICE_CTRL_BLE_BLEDISCONN},
    {"BLENAME",     &e103w11_ble_BLENAME,       AT_DEVICE_CTRL_BLE_BLENAME},
    {"BLESCAN",     &e103w11_ble_BLESCAN,       AT_DEVICE_CTRL_BLE_BLESCAN},
};

void at_register_ble_commands(struct at_device *device)
{
    at_register_command_group(device, "BLE", ble_commands, sizeof(ble_commands) / sizeof(command_desc_t), 0);
}
#endif /* AT_DEVICE_USING_E103W11 */