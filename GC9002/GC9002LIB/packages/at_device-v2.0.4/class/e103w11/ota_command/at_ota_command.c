#include "e103w11/ota_command/at_ota_command.h"
#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)

/* OTA update [AT+CIUPDATE=...] */
static int e103w11_ota_CIUPDATE(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CIUPDATE=..." commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+CIUPDATE=%s", arg);//AT+CIUPDATE="http://10.15.12.226/ota.bin"
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
/* Switch system [AT+CICHANGE=1] */
static int e103w11_ota_CICHANGE(struct at_device* device, char *buff, int size, char *arg)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(64, 0, AT_TIMEOUT_MAX);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send "AT+CICHANGE=1" commonds to e103w11 device */
    result = at_obj_exec_cmd(client, resp, "AT+CICHANGE=%s", arg);
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


#if(defined AT_DEVICE_INTERFACE_OTA && AT_DEVICE_INTERFACE_OTA == 1U)
extern int e103w11_device_cmd_no_result(struct at_device *device, const char *cmd, int timeout);
extern int e103w11_device_cmd_ret_string(struct at_device *device, const char *cmd, int timeout, 
                                        unsigned int resp_line, char *buff_str, unsigned int size);
                                        
int at_ota_CIUPDATE(struct at_device *device, const char *url)
{
    char cmd[64];
    sprintf(cmd, "AT+CIUPDATE=\"%s\"", url);
    return e103w11_device_cmd_no_result(device, cmd, AT_TIMEOUT_UPDATE);
}
int at_ota_CICHANGE(struct at_device *device, unsigned int check)
{
    return e103w11_device_cmd_no_result(device, ((check == 1) ? "AT+CICHANGE=1" : "AT+CICHANGE=0"), AT_TIMEOUT_SYS_SW);
}
#endif /* AT_DEVICE_INTERFACE_OTA */

static const command_desc_t ota_commands[] = {
    {"OTA_CIUPDATE",    &e103w11_ota_CIUPDATE,  AT_DEVICE_CTRL_OTA_CIUPDATE},
    {"OTA_CICHANGE",    &e103w11_ota_CICHANGE,  AT_DEVICE_CTRL_OTA_CICHANGE},
};

void at_register_ota_commands(struct at_device *device)
{
    at_register_command_group(device, "OTA", ota_commands, sizeof(ota_commands) / sizeof(command_desc_t), 0);
}
#endif /* AT_DEVICE_USING_E103W11 */