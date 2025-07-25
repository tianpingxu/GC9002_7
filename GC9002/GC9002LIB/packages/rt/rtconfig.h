
#ifndef __RTCONFIG_H__
#define __RTCONFIG_H__
#include "csi_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Network interface device */

#define RT_USING_FINSH

#if(defined CONFIG_NETDEV_ENABLE && CONFIG_NETDEV_ENABLE == 1U)
#define RT_USING_NETDEV
#define NETDEV_USING_IFCONFIG
#define NETDEV_USING_PING
#define NETDEV_USING_NETSTAT
#define NETDEV_USING_AUTO_DEFAULT
#define NETDEV_IPV4 1
#define NETDEV_IPV6 0

#endif /* CONFIG_NETDEV_ENABLE */

#if(defined CONFIG_SAL_ENABLE && CONFIG_NETDEV_ENABLE == 1U)
#define RT_USING_SAL
// #define SAL_USING_AT
#endif /* CONFIG_SAL_ENABLE */


#ifdef __cplusplus
}
#endif

#endif /* __RTCONFIG_H__ */