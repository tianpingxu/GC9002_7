/*
 * File      : at_device_e103w11.c
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
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <e103w11/at_device_e103w11.h>

#define LOG_TAG                        "at.dev.e103w11"

#include <at_log.h>

#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)


/*
 * 将字符转换为数值
 * */
int c2i(char ch)
{
    // 如果是数字，则用数字的ASCII码减去48, 如果ch = '2' ,则 '2' - 48 = 2  
    if (isdigit(ch))
        return ch - 48;

    // 如果是字母，但不是A~F,a~f则返回  
    if (ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z')
        return -1;

    // 如果是大写字母，则用数字的ASCII码减去55, 如果ch = 'A' ,则 'A' - 55 = 10  
    // 如果是小写字母，则用数字的ASCII码减去87, 如果ch = 'a' ,则 'a' - 87 = 10  
    if (isalpha(ch))
        return isupper(ch) ? ch - 55 : ch - 87;

    return -1;
}

/*
 * 功能：将十六进制字符串转换为整型(int)数值
 * */
int hex2dec(const char* hex, size_t len)
{
    size_t num = 0;
    size_t temp;
    size_t bits;
    size_t i;

    for (i = 0, temp = 0; i < len; i++, temp = 0)
    {
        // 第一次：i=0, *(hex + i) = *(hex + 0) = '1', 即temp = 1  
        // 第二次：i=1, *(hex + i) = *(hex + 1) = 'd', 即temp = 13  
        // 第三次：i=2, *(hex + i) = *(hex + 2) = 'd', 即temp = 14  
        temp = c2i(*(hex + i));
        // 总共3位，一个16进制位用 4 bit保存  
        // 第一次：'1'为最高位，所以temp左移 (len - i -1) * 4 = 2 * 4 = 8 位  
        // 第二次：'d'为次高位，所以temp左移 (len - i -1) * 4 = 1 * 4 = 4 位  
        // 第三次：'e'为最低位，所以temp左移 (len - i -1) * 4 = 0 * 4 = 0 位  
        bits = (len - i - 1) * 4;
        temp = temp << bits;

        // 此处也可以用 num += temp;进行累加  
        num = num | temp;
    }

    // 返回结果  
    return (int)num;
}
/* =============================  e103w11 network interface operations ============================= */

static int e103w11_netdev_set_dns_server(struct netdev *netdev, uint8_t dns_num, ip_addr_t *dns_server);

// static void e103w11_get_netdev_info(struct rt_work *work, void *work_data)
// {
// #define AT_ADDR_LEN          32
// #define AT_ERR_DNS_SERVER    "255.255.255.255"
// #define AT_DEF_DNS_SERVER    "114.114.114.114"

//     at_response_t resp = RT_NULL;
//     char ip[AT_ADDR_LEN] = {0}, mac[AT_ADDR_LEN] = {0};
//     char gateway[AT_ADDR_LEN] = {0}, netmask[AT_ADDR_LEN] = {0};
//     char dns_server1[AT_ADDR_LEN] = {0}, dns_server2[AT_ADDR_LEN] = {0};
//     const char *resp_expr = "%*[^\"]\"%[^\"]\"";
//     const char *resp_dns = "+CIPDNS_CUR:%s";
//     ip_addr_t ip_addr;
//     rt_uint32_t mac_addr[6] = {0};
//     rt_uint32_t num = 0;
//     rt_uint32_t dhcp_stat = 0;
//     struct rt_delayed_work *delay_work = (struct rt_delayed_work *)work;
//     struct at_device *device = (struct at_device *)work_data;
//     struct netdev *netdev = device->netdev;
//     struct at_client *client = device->client;

//     if (delay_work)
//     {
//         rt_free(delay_work);
//     }

//     resp = at_create_resp(512, 0, rt_tick_from_millisecond(300));
//     if (resp == RT_NULL)
//     {
//         LOG_E("no memory for resp create.");
//         return;
//     }

//     /* send mac addr query commond "AT+CIFSR" and wait response */
//     if (at_obj_exec_cmd(client, resp, "AT+CIFSR") < 0)
//     {
//         goto __exit;
//     }

//     if (at_resp_parse_line_args_by_kw(resp, "STAMAC", resp_expr, mac) <= 0)
//     {
//         LOG_E("%s device parse \"AT+CIFSR\" cmd error.", device->name);
//         goto __exit;
//     }

//     /* send addr info query commond "AT+CIPSTA?" and wait response */
//     if (at_obj_exec_cmd(client, resp, "AT+CIPSTA?") < 0)
//     {
//         LOG_E("%s device send \"AT+CIPSTA?\" cmd error.", device->name);
//         goto __exit;
//     }

//     if (at_resp_parse_line_args_by_kw(resp, "ip", resp_expr, ip) <= 0 ||
//             at_resp_parse_line_args_by_kw(resp, "gateway", resp_expr, gateway) <= 0 ||
//             at_resp_parse_line_args_by_kw(resp, "netmask", resp_expr, netmask) <= 0)
//     {
//         LOG_E("%s device prase \"AT+CIPSTA?\" cmd error.", device->name);
//         goto __exit;
//     }

//     /* set netdev info */
//     inet_aton(gateway, &ip_addr);
//     netdev_low_level_set_gw(netdev, &ip_addr);
//     inet_aton(netmask, &ip_addr);
//     netdev_low_level_set_netmask(netdev, &ip_addr);
//     inet_aton(ip, &ip_addr);
//     netdev_low_level_set_ipaddr(netdev, &ip_addr);
//     sscanf(mac, "%x:%x:%x:%x:%x:%x",
//             &mac_addr[0], &mac_addr[1], &mac_addr[2], &mac_addr[3], &mac_addr[4], &mac_addr[5]);
//     for (num = 0; num < netdev->hwaddr_len; num++)
//     {
//         netdev->hwaddr[num] = mac_addr[num];
//     }

//     /* send dns server query commond "AT+CIPDNS_CUR?" and wait response */
//     if (at_obj_exec_cmd(device->client, resp, "AT+CIPDNS_CUR?") < 0)
//     {
//         LOG_W("please check and update %s device firmware to support the \"AT+CIPDNS_CUR?\" cmd.", device->name);
//         goto __exit;
//     }

//     if (at_resp_parse_line_args(resp, 1, resp_dns, dns_server1) <= 0 &&
//             at_resp_parse_line_args(resp, 2, resp_dns, dns_server2) <= 0)
//     {
//         LOG_E("%s device prase \"AT+CIPDNS_CUR?\" cmd error.", device->name);
//         goto __exit;
//     }

//     /* set primary DNS server address */
//     if (rt_strlen(dns_server1) > 0 &&
//             rt_strncmp(dns_server1, AT_ERR_DNS_SERVER, rt_strlen(AT_ERR_DNS_SERVER)) != 0)
//     {
//         inet_aton(dns_server1, &ip_addr);
//         netdev_low_level_set_dns_server(netdev, 0, &ip_addr);
//     }
//     else
//     {
//         inet_aton(AT_DEF_DNS_SERVER, &ip_addr);
//         e103w11_netdev_set_dns_server(netdev, 0, &ip_addr);
//     }

//     /* set secondary DNS server address */
//     if (rt_strlen(dns_server2) > 0 )
//     {
//         inet_aton(dns_server2, &ip_addr);
//         netdev_low_level_set_dns_server(netdev, 1, &ip_addr);
//     }

//     /* send DHCP query commond " AT+CWDHCP_CUR?" and wait response */
//     if (at_obj_exec_cmd(client, resp, "AT+CWDHCP_CUR?") < 0)
//     {
//         goto __exit;
//     }

//     /* parse response data, get the DHCP status */
//     if (at_resp_parse_line_args_by_kw(resp, "+CWDHCP_CUR:", "+CWDHCP_CUR:%d", &dhcp_stat) < 0)
//     {
//         LOG_E("%s device prase DHCP status error.", device->name);
//         goto __exit;
//     }

//     /* Bit0 - SoftAP DHCP status, Bit1 - Station DHCP status */
//     netdev_low_level_set_dhcp_status(netdev, dhcp_stat & 0x02 ? RT_TRUE : RT_FALSE);

// __exit:
//     if (resp)
//     {
//         at_delete_resp(resp);
//     }
// }

static int e103w11_net_init(struct at_device *device);

static int e103w11_netdev_set_up(struct netdev *netdev)
{
    struct at_device *device = RT_NULL;

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return RT_ERROR;
    }

    if (device->is_init == RT_FALSE)
    {
        e103w11_net_init(device);
        netdev_low_level_set_status(netdev, RT_TRUE);
        LOG_D("network interface device(%s) set up status", netdev->name);
    }

    return RT_EOK;
}

static int e103w11_netdev_set_down(struct netdev *netdev)
{
    struct at_device *device = RT_NULL;

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device by netdev(%s) failed.", netdev->name);
        return RT_ERROR;
    }

    if (device->is_init == RT_TRUE)
    {
        device->is_init = RT_FALSE;
        netdev_low_level_set_status(netdev, RT_FALSE);
        LOG_D("network interface device(%s) set down status", netdev->name);
    }

    return RT_EOK;
}

static int e103w11_netdev_set_addr_info(struct netdev *netdev, ip_addr_t *ip_addr, ip_addr_t *netmask, ip_addr_t *gw)
{
#define IPADDR_RESP_SIZE       128
#define IPADDR_SIZE            16

    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;
    char ip_str[IPADDR_SIZE] = {0};
    char gw_str[IPADDR_SIZE] = {0};
    char netmask_str[IPADDR_SIZE] = {0};

    RT_ASSERT(netdev);
    RT_ASSERT(ip_addr || netmask || gw);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return RT_ERROR;
    }

    resp = at_create_resp(IPADDR_RESP_SIZE, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        result = RT_ENOMEM;
        goto __exit;
    }

    /* Convert numeric IP address into decimal dotted ASCII representation. */
    if (ip_addr)
        rt_memcpy(ip_str, inet_ntoa(*ip_addr), IPADDR_SIZE);
    else
        rt_memcpy(ip_str, inet_ntoa(netdev->ip_addr), IPADDR_SIZE);

    if (gw)
        rt_memcpy(gw_str, inet_ntoa(*gw), IPADDR_SIZE);
    else
        rt_memcpy(gw_str, inet_ntoa(netdev->gw), IPADDR_SIZE);

    if (netmask)
        rt_memcpy(netmask_str, inet_ntoa(*netmask), IPADDR_SIZE);
    else
        rt_memcpy(netmask_str, inet_ntoa(netdev->netmask), IPADDR_SIZE);

    /* send addr info set commond "AT+CIPSTA_CUR=<ip>[,<gateway>,<netmask>]" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+CIPSTA_CUR=\"%s\",\"%s\",\"%s\"",
                        ip_str, gw_str, netmask_str) < 0)
    {
        LOG_E("%s device set address failed.", device->name);
        result = RT_ERROR;
    }
    else
    {
        /* Update netdev information */
        if (ip_addr)
            netdev_low_level_set_ipaddr(netdev, ip_addr);

        if (gw)
            netdev_low_level_set_gw(netdev, gw);

        if (netmask)
            netdev_low_level_set_netmask(netdev, netmask);

        LOG_D("%s device set address success.", device->name);
    }

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

static int e103w11_netdev_set_dns_server(struct netdev *netdev, uint8_t dns_num, ip_addr_t *dns_server)
{
#define DNS_RESP_SIZE           128

    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(netdev);
    RT_ASSERT(dns_server);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device by netdev(%s) failed.", netdev->name);
        return RT_ERROR;
    }

    resp = at_create_resp(DNS_RESP_SIZE, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }

    /* send dns server set commond "AT+CIPDNS_CUR=<enable>[,<DNS    server0>,<DNS   server1>]" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+CIPDNS_CUR=1,\"%s\"", inet_ntoa(*dns_server)) < 0)
    {
        LOG_E("%s device set DNS failed.", device->name);
        result = RT_ERROR;
    }
    else
    {
        netdev_low_level_set_dns_server(netdev, dns_num, dns_server);
        LOG_D("%s device set DNS(%s) success.", device->name, inet_ntoa(*dns_server));
    }

    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

static int e103w11_netdev_set_dhcp(struct netdev *netdev, rt_bool_t is_enabled)
{
#define E103W11_STATION     1
#define RESP_SIZE           128

    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;

    RT_ASSERT(netdev);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device by netdev(%s) failed.", netdev->name);
        return RT_ERROR;
    }

    resp = at_create_resp(RESP_SIZE, 0, rt_tick_from_millisecond(300));
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp struct.");
        return RT_ENOMEM;
    }

    /* send dhcp set commond "AT+CWDHCP_CUR=<mode>,<en>" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+CWDHCP_CUR=%d,%d", E103W11_STATION, is_enabled) < 0)
    {
        LOG_E("%s device set DHCP status(%d) failed.", device->name, is_enabled);
        result = RT_ERROR;
        goto __exit;
    }
    else
    {
        netdev_low_level_set_dhcp_status(netdev, is_enabled);
        LOG_D("%s device set DHCP status(%d) ok.", device->name, is_enabled);
    }

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}

#ifdef NETDEV_USING_PING
static int e103w11_netdev_ping(struct netdev *netdev, const char *host,
                size_t data_len, uint32_t timeout, struct netdev_ping_resp *ping_resp)
{
#define E103W11_PING_IP_SIZE         16

    rt_err_t result = RT_EOK;
    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;
    char ip_addr[E103W11_PING_IP_SIZE] = {0};
    int req_time;

    RT_ASSERT(netdev);
    RT_ASSERT(host);
    RT_ASSERT(ping_resp);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return RT_ERROR;
    }

    resp = at_create_resp(64, 0, timeout);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }

    /* send domain commond "AT+CIPDOMAIN=<domain name>" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+CIPDOMAIN=\"%s\"", host) < 0)
    {
        result = RT_ERROR;
        goto __exit;
    }

    /* parse the third line of response data, get the IP address */
    char format[24];
    rt_sprintf(format, "+CIPDOMAIN:\"%c%d[^\"]\"", '%', E103W11_PING_IP_SIZE);
    if (at_resp_parse_line_args_by_kw(resp, "+CIPDOMAIN:", format, ip_addr) < 0)
    {
        result = RT_ERROR;
        goto __exit;
    }

    /* send ping commond "AT+PING=<IP>" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+PING=\"%s\"", host) < 0)
    {
        result = RT_ERROR;
        goto __exit;
    }

    if (at_resp_parse_line_args_by_kw(resp, "+", "+:%d", &req_time) < 0)
    {
        result = RT_ERROR;
        goto __exit;
    }

    if (req_time)
    {
        inet_aton(ip_addr, &(ping_resp->ip_addr));
        ping_resp->data_len = data_len;
        ping_resp->ttl = 0;
        ping_resp->ticks = req_time;
    }

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    return result;
}
#endif /* NETDEV_USING_PING */

#ifdef NETDEV_USING_NETSTAT
void e103w11_netdev_netstat(struct netdev *netdev)
{
#define E103W11_NETSTAT_RESP_SIZE         320
#define E103W11_NETSTAT_TYPE_SIZE         4
#define E103W11_NETSTAT_IPADDR_SIZE       17
#define E103W11_NETSTAT_EXPRESSION        "+CIPSTATUS:%*d,\"%[^\"]\",\"%[^\"]\",%d,%d,%*d"

    at_response_t resp = RT_NULL;
    struct at_device *device = RT_NULL;
    int remote_port, local_port, i;
    char *type = RT_NULL;
    char *ipaddr = RT_NULL;

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_NETDEV, netdev->name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", netdev->name);
        return;
    }

    type = (char *) rt_calloc(1, E103W11_NETSTAT_TYPE_SIZE);
    ipaddr = (char *) rt_calloc(1, E103W11_NETSTAT_IPADDR_SIZE);
    if ((void *)(type && ipaddr) == RT_NULL)
    {
        LOG_E("no memory for ipaddr create.");
        goto __exit;
    }

    resp = at_create_resp(E103W11_NETSTAT_RESP_SIZE, 0, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("%s no memory for resp create.", device->name);
        goto __exit;
    }

    /* send network connection information commond "AT+CIPSTATUS" and wait response */
    if (at_obj_exec_cmd(device->client, resp, "AT+CIPSTATUS") < 0)
    {
        goto __exit;
    }

    for (i = 1; i <= resp->line_counts; i++)
    {
        if (strstr(at_resp_get_line(resp, i), "+CIPSTATUS"))
        {
            /* parse the third line of response data, get the network connection information */
            if (at_resp_parse_line_args(resp, i, E103W11_NETSTAT_EXPRESSION, type, ipaddr, &remote_port, &local_port) < 0)
            {
                goto __exit;
            }
            else
            {
                LOG_RAW("%s: %s:%d ==> %s:%d\n", type, inet_ntoa(netdev->ip_addr), local_port, ipaddr, remote_port);
            }
        }
    }

__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }

    if (type)
    {
        rt_free(type);
    }

    if (ipaddr)
    {
        rt_free(ipaddr);
    }
}
#endif /* NETDEV_USING_NETSTAT */

static const struct netdev_ops e103w11_netdev_ops =
{
    e103w11_netdev_set_up,
    e103w11_netdev_set_down,

    e103w11_netdev_set_addr_info,
    e103w11_netdev_set_dns_server,
    e103w11_netdev_set_dhcp,

#ifdef NETDEV_USING_PING
    e103w11_netdev_ping,
#endif
#ifdef NETDEV_USING_NETSTAT
    e103w11_netdev_netstat,
#endif
};

static struct netdev *e103w11_netdev_add(const char *netdev_name)
{
#define ETHERNET_MTU        1500
#define HWADDR_LEN          6
    struct netdev *netdev = RT_NULL;

    RT_ASSERT(netdev_name);

    netdev = netdev_get_by_name(netdev_name);
    if (netdev != RT_NULL)
    {
        return (netdev);
    }

    netdev = (struct netdev *) rt_calloc(1, sizeof(struct netdev));
    if (netdev == RT_NULL)
    {
        LOG_E("no memory for netdev create.");
        return RT_NULL;
    }

    netdev->mtu = ETHERNET_MTU;
    netdev->ops = &e103w11_netdev_ops;
    netdev->hwaddr_len = HWADDR_LEN;

#ifdef SAL_USING_AT
    extern int sal_at_netdev_set_pf_info(struct netdev *netdev);
    /* set the network interface socket/netdb operations */
    sal_at_netdev_set_pf_info(netdev);
#endif

    netdev_register(netdev, netdev_name, RT_NULL);

    return netdev;
}

/* =============================  e103w11 device operations ============================= */

#define AT_SEND_CMD(client, resp, cmd)                                     \
    do {                                                                   \
        (resp) = at_resp_set_info((resp), 256, 0, 5 * RT_TICK_PER_SECOND); \
        if (at_obj_exec_cmd((client), (resp), (cmd)) < 0)                  \
        {                                                                  \
            result = RT_ERROR;                                            \
            goto __exit;                                                   \
        }                                                                  \
    } while(0)                                                             \

static void e103w11_netdev_start_delay_work(struct at_device *device)
{
    // struct rt_delayed_work *net_work = RT_NULL;
    // net_work = (struct rt_delayed_work *)rt_calloc(1, sizeof(struct rt_delayed_work));
    // if (net_work == RT_NULL)
    // {
    //     return;
    // }

    // rt_delayed_work_init(net_work, e103w11_get_netdev_info, (void *)device);
    // rt_work_submit(&(net_work->work), RT_TICK_PER_SECOND);
}

static void e103w11_init_thread_entry(void *parameter)
{
#define INIT_RETRY    5

    struct at_device *device = (struct at_device *) parameter;
    struct at_device_e103w11 *e103w11 = (struct at_device_e103w11 *) device->user_data;
    struct at_client *client = device->client;
    at_response_t resp = RT_NULL;
    rt_err_t result = RT_EOK;
    rt_size_t i = 0, retry_num = INIT_RETRY;
    rt_bool_t wifi_is_conn = RT_FALSE;

    LOG_D("%s device initialize start.", device->name);

    /* wait e103w11 device startup finish */
    if (at_client_obj_wait_connect(client, E103W11_WAIT_CONNECT_TIME))
    {
        return;
    }

    resp = at_create_resp(128, 0, 5 * RT_TICK_PER_SECOND);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return;
    }

    while (retry_num--)
    {
        /* reset module */
        AT_SEND_CMD(client, resp, "AT+RST");
        /* reset waiting delay */
        rt_thread_mdelay(1000);
        /* disable echo */
        AT_SEND_CMD(client, resp, "ATE0");
        /* set current mode to Wi-Fi station */
        AT_SEND_CMD(client, resp, "AT+CWMODE=1,\"s.n\"");
        /* get module version */
        AT_SEND_CMD(client, resp, "AT+GMR");
        /* show module version */
        for (i = 0; resp->line_counts > 0 && i < resp->line_counts - 1; i++)
        {
            LOG_D("%s", at_resp_get_line(resp, i + 1));
        }

        AT_SEND_CMD(client, resp, "AT+CIPMUX=1");
        at_client_set_flag(device->client, AT_CLIENT_LINK_MUX_POS, true);
        /* initialize successfully  */
        result = RT_EOK;
        break;

    __exit:
        if (result != RT_EOK)
        {
            rt_thread_mdelay(1000);
            LOG_I("%s device initialize retry...", device->name);
        }
    }

    /* connect to WiFi AP */
    if (at_obj_exec_cmd(client, at_resp_set_info(resp, 128, 0, 20 * RT_TICK_PER_SECOND),
                        "AT+CWJAP=\"%s\",\"%s\",\"s.n\"", e103w11->wifi_ssid, e103w11->wifi_password) != RT_EOK)
    {
        LOG_W("%s device wifi connect failed, check ssid(%s) and password(%s).",
              device->name, e103w11->wifi_ssid, e103w11->wifi_password);
    }
    else
    {
        wifi_is_conn = RT_TRUE;
    }
    if (resp)
    {
        at_delete_resp(resp);
    }

    if (result != RT_EOK)
    {
        netdev_low_level_set_status(device->netdev, RT_FALSE);
        LOG_E("%s device network initialize failed(%d).", device->name, result);
    }
    else
    {
        device->is_init = RT_TRUE;
        netdev_low_level_set_status(device->netdev, RT_TRUE);
        if (wifi_is_conn)
        {
            netdev_low_level_set_link_status(device->netdev, RT_TRUE);
        }
        e103w11_netdev_start_delay_work(device);
        LOG_I("%s device network initialize successfully.", device->name);
    }
}

static int e103w11_net_init(struct at_device *device)
{
#ifdef AT_DEVICE_E103W11_INIT_ASYN
    rt_thread_t tid;

    tid = rt_thread_create("esp_net", e103w11_init_thread_entry, (void *) device,
            E103W11_THREAD_STACK_SIZE, E103W11_THREAD_PRIORITY, 20);
    if (tid)
    {
        rt_thread_startup(tid);
    }
    else
    {
        LOG_E("create %s device init thread failed.", device->name);
        return RT_ERROR;
    }
#else
    e103w11_init_thread_entry(device);
#endif /* AT_DEVICE_E103W11_INIT_ASYN */

    return RT_EOK;
}

static void urc_busy_p_func(struct at_client *client, const char *data, rt_size_t size)
{
    LOG_D("system is processing a commands...");
}

static void urc_busy_s_func(struct at_client *client, const char *data, rt_size_t size)
{
    LOG_D("system is sending data...");
}

static void urc_func(struct at_client *client, const char *data, rt_size_t size)
{
    struct at_device *device = RT_NULL;
    char *client_name = client->device->name;

    RT_ASSERT(client && data && size);

    device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, client_name);
    if (device == RT_NULL)
    {
        LOG_E("get device(%s) failed.", client_name);
        return;
    }

    if (rt_strstr(data, "WIFI CONNECTED"))
    {
        LOG_I("%s device wifi is connected.", device->name);

        if (device->is_init)
        {
            netdev_low_level_set_link_status(device->netdev, RT_TRUE);

            e103w11_netdev_start_delay_work(device);
        }
    }
    else if (rt_strstr(data, "WIFI DISCONNECT"))
    {
        LOG_I("%s device wifi is disconnect.", device->name);

        if (device->is_init)
        {
            netdev_low_level_set_link_status(device->netdev, RT_FALSE);
        }
    }
}

static const struct at_urc urc_table[] =
{
    {"busy p", "\r\n", urc_busy_p_func},
    {"busy s", "\r\n", urc_busy_s_func},
    //"SYSTEM_EVENT_STA_CONNECTED\nWIFI CONNECTED\r\n"
    {"SYSTEM_EVENT_STA_CONNECTED", "WIFI CONNECTED\r\n", urc_func},
    //"SYSTEM_EVENT_STA_DISCONNECTED\nWIFI DISCONNECT\r\n"
    {"SYSTEM_EVENT_STA_DISCONNECTED", "WIFI DISCONNECT\r\n", urc_func},
};

static int e103w11_init(struct at_device *device)
{
    struct at_device_e103w11 *e103w11 = (struct at_device_e103w11 *) device->user_data;

    /* initialize AT client */
    at_client_init(e103w11->client_name, e103w11->recv_line_num);

    device->client = at_client_get(e103w11->client_name);
    if (device->client == RT_NULL)
    {
        LOG_E("get AT client(%s) failed.", e103w11->client_name);
        return RT_ERROR;
    }

    /* register URC data execution function  */
    at_obj_set_urc_table(device->client, urc_table, sizeof(urc_table) / sizeof(urc_table[0]));

#if(defined AT_USING_SOCKET && AT_USING_SOCKET == 1U)
    e103w11_socket_init(device);
#endif

    /* add e103w11 device to the netdev list */
    device->netdev = e103w11_netdev_add(e103w11->device_name);
    if (device->netdev == RT_NULL)
    {
        LOG_E("add netdev(%s) failed.", e103w11->device_name);
        return RT_ERROR;
    }

    /* initialize e103w11 device network */
    return e103w11_netdev_set_up(device->netdev);
    return 0;
}

static int e103w11_deinit(struct at_device *device)
{
    return e103w11_netdev_set_down(device->netdev);
}


static int e103w11_control(struct at_device *device, int cmd, char *arg, char *buff, int size)
{
    int result = RT_ERROR;
    RT_ASSERT(device);

    int igrp;
    int pos;
    int icmd;
    for (igrp = 0; igrp < device->command_groups_count; ++igrp)
    {
        const command_group_t* group = device->command_groups + igrp;
        for (icmd = 0; icmd < group->commands_count; ++icmd)
        {
            const command_desc_t* command = group->commands + icmd;
            if(command->id == cmd)
            {
                return command->fn(device, buff, size, arg);
            }
        }
    }
    return result;
}


static const struct at_device_ops e103w11_device_ops =
{
    e103w11_init,
    e103w11_deinit,
    e103w11_control,
};

int e103w11_exec_cmd(struct at_device* device, char *cmd, char *buff, int size, char *arg, unsigned int res_line_num, int timeout)
{
    int result = RT_EOK;
    struct at_client *client = device->client;

    at_response_t resp = at_create_resp(size, res_line_num, timeout);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send commonds to e103w11 device */
    if(arg == RT_NULL)
    {
        result = at_obj_exec_cmd(client, resp, cmd);
    }
    else
    {
        result = at_obj_exec_cmd(client, resp, "%s%s", cmd, arg);//cmd=AT+SYSFLASH=   arg=1,"nv_amt",0,10
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
int e103w11_device_cmd_no_result(struct at_device *device, const char *cmd, int timeout)
{
    if(device == RT_NULL || cmd == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(64, 0, timeout);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, cmd);
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
int e103w11_device_cmd_ret_string(struct at_device *device, const char *cmd, int timeout, unsigned int resp_line, char *buff_str, unsigned int size)
{
    if(device == RT_NULL || cmd == RT_NULL || buff_str == RT_NULL)
    {
        return RT_EINVAL;
    }
    int result = RT_EOK;
    at_response_t resp = RT_NULL;
    resp = at_create_resp(size, 0, timeout);
    if (resp == RT_NULL)
    {
        LOG_E("no memory for resp create.");
        return RT_ENOMEM;
    }
    /* send domain commond and wait response */
    result = at_obj_exec_cmd(device->client, resp, cmd);
    if (result != RT_EOK)
    {
        goto __exit;
    }
    if(buff_str != NULL)
    {
        rt_memcpy(buff_str, resp->buf, (resp->buf_len < size ? resp->buf_len : size));
    }
__exit:
    if (resp)
    {
        at_delete_resp(resp);
    }
    return result;
}
int e103w11_device_class_register(void)
{
    struct at_device_class *class = RT_NULL;

    class = (struct at_device_class *) rt_calloc(1, sizeof(struct at_device_class));
    if (class == RT_NULL)
    {
        LOG_E("no memory for class create.");
        return RT_ENOMEM;
    }

    /* fill E103W11 device class object */
#if(defined AT_USING_SOCKET && AT_USING_SOCKET == 1U)
    e103w11_socket_class_register(class);
#endif
    class->device_ops = &e103w11_device_ops;

    return at_device_class_register(class, AT_DEVICE_CLASS_E103W11);
}

#endif /* AT_DEVICE_USING_E103W11 */
