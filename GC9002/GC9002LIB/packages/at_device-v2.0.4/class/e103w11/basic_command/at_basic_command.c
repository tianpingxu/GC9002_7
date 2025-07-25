#include "e103w11/basic_command/at_basic_command.h"
#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)

/* Test AT start [AT] */
static int e103w11_basic_AT(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT");
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
/* reset e103w11 device [AT+RST] */
static int e103w11_basic_RST(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+RST" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+RST");
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    rt_thread_mdelay(1000);
    /* waiting 10 seconds for e103w11 device reset */
    device->is_init = RT_FALSE;
    if (at_client_obj_wait_connect(client, E103W11_WAIT_CONNECT_TIME))
    {
        return -RT_ETIMEOUT;
    }
    device->is_init = RT_TRUE;
    return result;
}
/* Get version Information [AT+GMR] */
static int e103w11_basic_GMR(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(128, 3, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+GMR" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+GMR");
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
/* Get all cmd [AT+CMD] */
static int e103w11_basic_CMD(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(size, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CMD" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+CMD");
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
/* Get all cmd [AT+RESTORE] */
static int e103w11_basic_RESTORE(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+RESTORE" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+RESTORE");
    if (resp)
    {
        if(buff != RT_NULL)
        {
            rt_memcpy(buff, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
        }
        at_delete_resp(resp);
    }
    rt_thread_mdelay(1000);
    /* waiting 10 seconds for e103w11 device reset */
    device->is_init = RT_FALSE;
    if (at_client_obj_wait_connect(client, E103W11_WAIT_CONNECT_TIME))
    {
        return -RT_ETIMEOUT;
    }
    device->is_init = RT_TRUE;
    return result;
}
/* Get UART CFG [AT+UART?] */
static int e103w11_basic_UART(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(128, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+UART?" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+UART?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+UART=%s", arg);//AT+UART=115200,8,1,0,0,"s.y"
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
/* Set GPIO CFG [AT+SYSIOSETCFG=...] */
static int e103w11_basic_SYSIOSETCFG(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+SYSIOSETCFG=..." commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+SYSIOSETCFG=%s", arg);//AT+SYSIOSETCFG=13,1,1 //设置 GPIO13 工作在 GPIO 模式
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
/* Get GPIO CFG [AT+SYSIOGETCFG=...] */
static int e103w11_basic_SYSIOGETCFG(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+SYSIOGETCFG=..." commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+SYSIOGETCFG=%s", arg);//AT+SYSIOGETCFG=13 //读取 GPIO13 工作在 GPIO 模式
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
/* Set GPIO DIR [AT+SYSGPIODIR=...] */
static int e103w11_basic_SYSGPIODIR(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+SYSGPIODIR=..." commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+SYSGPIODIR=%s", arg);//AT+SYSGPIODIR=13,1 //设置 GPIO13 为输出
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
/* Set GPIO LEVEL [AT+SYSGPIOWRITE=...] */
static int e103w11_basic_SYSGPIOWRITE(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+SYSGPIOWRITE=..." commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+SYSGPIOWRITE=%s", arg);//AT+SYSGPIOWRITE=13,1 //设置 GPIO13 输出为高电平
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
/* Get GPIO LEVEL [AT+SYSGPIOREAD=...] */
static int e103w11_basic_SYSGPIOREAD(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+SYSGPIOREAD=..." commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+SYSGPIOREAD=%s", arg);//AT+SYSGPIOREAD=13 //读取 GPIO13 状态
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
/* echo close [ATE0] */
static int e103w11_basic_ATE(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "ATE0" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "ATE%s", arg);
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
/* Get ram [AT+SYSRAM?] */
static int e103w11_basic_SYSRAM(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+SYSRAM?" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+SYSRAM?");
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
/* Get temperature [AT+SYSTEMP?] */
static int e103w11_basic_SYSTEMP(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 1, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+SYSTEMP?" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+SYSTEMP?");
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
/* Get flash [AT+SYSFLASH?] */
static int e103w11_basic_SYSFLASH(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(size, 10, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+SYSFLASH?" commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, "AT+SYSFLASH?");
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "AT+SYSFLASH=%s", arg);//AT+SYSFLASH=1,"nv_amt",0,10
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



#if(defined AT_DEVICE_INTERFACE_BASIC && AT_DEVICE_INTERFACE_BASIC == 1U)

extern int e103w11_device_cmd_no_result(struct at_device *device, const char *cmd, int timeout);
extern int e103w11_device_cmd_ret_string(struct at_device *device, const char *cmd, int timeout, 
                                        unsigned int resp_line, char *buff_str, unsigned int size);
extern int hex2dec(const char* hex, size_t len);

int at_basic_AT(struct at_device *device)
{
    return e103w11_device_cmd_no_result(device, "AT", AT_TIMEOUT_NORMAL);
}
int at_basic_RST(struct at_device *device)
{
    return e103w11_device_cmd_no_result(device, "AT+RST", AT_TIMEOUT_NORMAL);
}
int at_basic_GMR(struct at_device *device, char *buff, unsigned int size)
{
    return e103w11_device_cmd_ret_string(device, "AT+GMR", AT_TIMEOUT_NORMAL, 0, buff, size);
}
int at_basic_CMD(struct at_device *device, char *buff, unsigned int size)
{
    return e103w11_device_cmd_ret_string(device, "AT+CMD", AT_TIMEOUT_NORMAL, 0, buff, size);
}
int at_basic_RESTORE(struct at_device *device)
{
    return e103w11_device_cmd_no_result(device, "AT+RESTORE", AT_TIMEOUT_NORMAL);
}
int at_basic_UART_get(struct at_device *device, 
				  struct at_uart_config *cur, 
				  struct at_uart_config *def)
{
    if(device == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(128, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+UART?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(cur != RT_NULL)
    {
        if (at_resp_parse_line_args_by_kw(resp, "+UART_CUR", "+UART_CUR:%d,%d,%d,%d,%d", 
                                        &cur->baudrate, &cur->databits, &cur->stopbits, &cur->parity, &cur->control) < 0)
        {
            result = RT_ERROR;
            goto __exit;
        }
    }
    if(def != RT_NULL)
    {
        if (at_resp_parse_line_args_by_kw(resp, "+UART_DEF", "+UART_DEF:%d,%d,%d,%d,%d", 
                                        &def->baudrate, &def->databits, &def->stopbits, &def->parity, &def->control) < 0)
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
  * @brief  更改 UART 设置
  * @param  device at 设备句柄
  * @param  baudrate 波特率
  * @param  databits 数据位
  * @param  stopbits 停止位
  * @param  parity 奇偶校验位
  * @param  flow_control 流控设置
  * @param  save_flash 是否保存到Flash
  * @retval 0-success others-error
  */
int at_basic_UART_set(struct at_device *device, 
				  const struct at_uart_config cur, 
				  bool save_flash)
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
        result = at_obj_exec_cmd(device->client, resp, "AT+UART=%d,%d,%d,%d,%d,\"s.y\"", cur.baudrate, cur.databits, cur.stopbits, cur.parity, cur.control);
    }
    else
    {
        result = at_obj_exec_cmd(device->client, resp, "AT+UART=%d,%d,%d,%d,%d,\"s.n\"", cur.baudrate, cur.databits, cur.stopbits, cur.parity, cur.control);
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
  * @brief  设置 GPIO 口工作模式
  * @param  device at 设备句柄
  * @param  pin 引脚号
  * @param  mode 引脚工作模式，为 1 时，则为普通 IO 模式，其余功能暂不开放。
  *         当此参数为 1 时， 即为普通 IO 口
  * @param  pull 0-拉低  1-拉高
  * @retval 0-success others-error
  */
int at_basic_SYSIOSETCFG(struct at_device *device, unsigned int pin, unsigned int mode, unsigned int pull)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+SYSIOSETCFG=%d,%d,%d", pin, mode, pull);
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
  * @brief  查询 GPIO 口工作模式
  * @param  device at 设备句柄
  * @param  pin 引脚号
  * @param  mode 输出：引脚工作模式
  * @param  pull 输出：0-拉低  1-拉高
  * @retval 0-success others-error
  */
int at_basic_SYSIOGETCFG(struct at_device *device, unsigned int pin, unsigned int *mode, unsigned int *pull)
{
    if(device == RT_NULL || mode == RT_NULL || pull == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    unsigned int pin_temp = 0;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+SYSIOGETCFG=%d", pin);
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if (at_resp_parse_line_args_by_kw(resp, "+SYSIOGETCFG", "+SYSIOGETCFG:%d,%d,%d", &pin_temp, mode, pull) < 0)
    {
        result = RT_ERROR;
        goto __exit;
    }
    if(pin_temp != pin)
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
  * @brief  设置 GPIO 口方向
  * @param  device at 设备句柄
  * @param  pin 引脚号
  * @param  dir 0-设置 GPIO 作为输入   1-设置 GPIO 作为输出
  * @retval 0-success others-error
  */
int at_basic_SYSGPIODIR(struct at_device *device, unsigned int pin, unsigned int dir)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+SYSGPIODIR=%d,%d", pin, dir);
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
  * @brief  设置 GPIO 输出电平
  * @param  device at 设备句柄
  * @param  pin 引脚号
  * @param  level 0-低电平   1-高电平
  * @retval 0-success others-error
  */
int at_basic_SYSGPIOWRITE(struct at_device *device, unsigned int pin, unsigned int level)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+SYSGPIOWRITE=%d,%d", pin, level);
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
  * @brief  读取 GPIO 状态
  * @param  device at 设备句柄
  * @param  pin 引脚号
  * @param  dir 输出：0-GPIO 作为输入   1-GPIO 作为输出
  * @param  level 输出：0-低电平   1-高电平
  * @retval 0-success others-error
  */
int at_basic_SYSGPIOREAD(struct at_device *device, unsigned int pin, unsigned int *dir, unsigned int *level)
{
    if(device == RT_NULL || dir == RT_NULL || level == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    unsigned int pin_temp = 0;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, AT_TIMEOUT_NORMAL);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+SYSGPIOREAD=%d", pin);
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if (at_resp_parse_line_args_by_kw(resp, "+SYSGPIOREAD", "+SYSGPIOREAD:%d,%d,%d", &pin_temp, dir, level) < 0)
    {
        result = RT_ERROR;
        goto __exit;
    }
    if(pin_temp != pin)
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
int at_basic_ATE(struct at_device *device, bool enable)
{
    return e103w11_device_cmd_no_result(device, (enable ? "ATE1" : "ATE0"), AT_TIMEOUT_NORMAL);
}
/**
  * @brief  查询系统堆大小
  * @param  device at 设备句柄
  * @param  release_heap 输出：系统释放堆大小
  * @param  release_heap_min 输出：系统曾释放的最小堆大小
  * @retval 0-success others-error
  */
int at_basic_SYSRAM(struct at_device *device, unsigned int *release_heap, unsigned int *release_heap_min)
{
    if(device == RT_NULL || release_heap == RT_NULL || release_heap_min == RT_NULL)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+SYSRAM?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if (at_resp_parse_line_args_by_kw(resp, "+SYSRAM", "+SYSRAM:%d,%d", release_heap, release_heap_min) < 0)
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
  * @brief  查询芯片内部温度
  * @param  device at 设备句柄
  * @param  temperature 输出：芯片内部温度值
  * @retval 0-success others-error
  */
int at_basic_SYSTEMP(struct at_device *device, unsigned int *temperature)
{
    if(device == RT_NULL || temperature == RT_NULL)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+SYSTEMP?");
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if (at_resp_parse_line_args_by_kw(resp, "+SYSTEMP", "+SYSTEMP:%d", temperature) < 0)
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
  * @brief  查询Flash
  * @param  device at 设备句柄
  * @param  buff 输出：Flash信息输出缓冲区
  * @param  size buff大小
  * @retval 0-success others-error
  */
int at_basic_SYSFLASH_get(struct at_device *device, char *buff, unsigned int size)
{
    return e103w11_device_cmd_ret_string(device, "AT+SYSFLASH?", AT_TIMEOUT_NORMAL, 0, buff, size);
}
/**
  * @brief  擦除Flash
  * @param  device at 设备句柄
  * @param  partition 分区名
  * @param  offset 相对于分区起始地址的偏移地址
  * @param  length 擦除 FLASH 的长度
  * @retval 0-success others-error
  */
int at_basic_SYSFLASH_erase(struct at_device *device, const char *partition, unsigned int offset, unsigned int length)
{
    if(device == RT_NULL || partition == RT_NULL)
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
    unsigned int temp = length%4096;
    length = length/4096;
    if(temp > 0)
    {
        length = 4096*(length+1);
    }
    else
    {
        length = 4096*length;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+SYSFLASH=0,\"%s\",%d,%d", partition, offset, length);
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
int at_basic_SYSFLASH_write(struct at_device *device, 
                            char *partition, unsigned int offset, 
                            const unsigned char *data, unsigned int size)
{
    if(device == RT_NULL || partition == RT_NULL || data == RT_NULL)
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
    /* set AT client end sign to deal with '>' sign */
    at_obj_set_end_sign(device->client, '>');
    unsigned int length = size;
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, "AT+SYSFLASH=1,\"%s\",%d,%d", partition, offset, length);
    if (result != RT_EOK)
    {
        goto __exit;
    }
    ptr = rt_malloc(size*3);
    if(ptr == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        result = RT_ENOMEM;
        goto __exit;
    }
    for(int i = 0; i < size; i++)
    {
        sprintf(&ptr[i*3], "%02x ", data[i]);
    }
    /* send the real data to server or client */
    result = (int) at_client_obj_send(device->client, ptr, size*3-1);
    if (result == 0)
    {
        result = RT_ERROR;
        goto __exit;
    }
__exit:
    /* reset the end sign for data */
    at_obj_set_end_sign(device->client, 0);
    if(ptr != RT_NULL)
    {
        rt_free(ptr);
    }
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
int at_basic_SYSFLASH_read(struct at_device *device, 
                            char *partition, unsigned int offset, 
                            unsigned char *data, unsigned int size)
{
    if(device == RT_NULL || partition == RT_NULL || data == RT_NULL)
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
    result = at_obj_exec_cmd(device->client, resp, "AT+SYSFLASH=2,\"%s\",%d,%d", partition, offset, size);
    if (result != RT_EOK)
    {
        goto __exit;
    }
    ptr = resp->buf;
    int len = resp->buf_len/3 - 1;
    if(len > size)
    {
        len = size;
    }
    for(int i = 0; i < len; i++)
    {
        data[i] = (unsigned char)hex2dec(&resp->buf[i*3], 2);
    }
__exit:
    if(ptr != RT_NULL)
    {
        rt_free(ptr);
    }
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
#endif /* AT_DEVICE_INTERFACE_BASIC */

static const command_desc_t basic_commands[] = {
    {"BASIC_AT",            &e103w11_basic_AT,          AT_DEVICE_CTRL_BASIC_AT},
    {"BASIC_RST",           &e103w11_basic_RST,         AT_DEVICE_CTRL_BASIC_RST},
    {"BASIC_GMR",           &e103w11_basic_GMR,         AT_DEVICE_CTRL_BASIC_GMR},
    {"BASIC_CMD",           &e103w11_basic_CMD,         AT_DEVICE_CTRL_BASIC_CMD},
    {"BASIC_RESTORE",       &e103w11_basic_RESTORE,     AT_DEVICE_CTRL_BASIC_RESTORE},
    {"BASIC_UART",          &e103w11_basic_UART,        AT_DEVICE_CTRL_BASIC_UART},
    {"BASIC_SYSIOSETCFG",   &e103w11_basic_SYSIOSETCFG, AT_DEVICE_CTRL_BASIC_SYSIOSETCFG},
    {"BASIC_SYSIOGETCFG",   &e103w11_basic_SYSIOGETCFG, AT_DEVICE_CTRL_BASIC_SYSIOGETCFG},
    {"BASIC_SYSGPIODIR",    &e103w11_basic_SYSGPIODIR,  AT_DEVICE_CTRL_BASIC_SYSGPIODIR},
    {"BASIC_SYSGPIOWRITE",  &e103w11_basic_SYSGPIOWRITE,AT_DEVICE_CTRL_BASIC_SYSGPIOWRITE},
    {"BASIC_SYSGPIOREAD",   &e103w11_basic_SYSGPIOREAD, AT_DEVICE_CTRL_BASIC_SYSGPIOREAD},
    {"BASIC_ATE",           &e103w11_basic_ATE,         AT_DEVICE_CTRL_BASIC_ATE},
    {"BASIC_SYSRAM",        &e103w11_basic_SYSRAM,      AT_DEVICE_CTRL_BASIC_SYSRAM},
    {"BASIC_SYSTEMP",       &e103w11_basic_SYSTEMP,     AT_DEVICE_CTRL_BASIC_SYSTEMP},
    {"BASIC_SYSFLASH",      &e103w11_basic_SYSFLASH,    AT_DEVICE_CTRL_BASIC_SYSFLASH},
};

void at_register_basic_commands(struct at_device *device)
{
    at_register_command_group(device, "BASIC", basic_commands, sizeof(basic_commands) / sizeof(command_desc_t), 0);
}
#endif /* AT_DEVICE_USING_E103W11 */