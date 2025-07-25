/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-06-07     huyt       the first version
 */
#ifndef __TEST_DEF_H__
#define __TEST_DEF_H__

#include <main.h>

#if(defined BSP_DEBUG && BSP_DEBUG == TEST_AT)
#if(defined CONFIG_AT_CLIENT_ENABLE && CONFIG_AT_CLIENT_ENABLE == 1U)

// #define _TEST_AT_CLENT_
// #define _TEST_AT_BASIC_
// #define _TEST_AT_BLE_
// #define _TEST_AT_OTA_
// #define _TEST_AT_WIFI_
// #define _TEST_AT_TCPIP_
// #define _TEST_AT_SOCKET_

// #define _TEST_AT_TCP_SEND_
#define _TEST_AT_UDP_SEND_

#endif /* CONFIG_AT_CLIENT_ENABLE */
#endif /* (defined BSP_DEBUG && BSP_DEBUG) */
#endif /* __TEST_DEF_H__ */
