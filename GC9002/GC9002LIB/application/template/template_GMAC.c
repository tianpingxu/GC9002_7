/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     template_GMAC.c
 * @brief    GMAC test code
 * @version  V1.0
 * @date     8. Feb 2023
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx_hal.h"
#include "csi_motor.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_kernel.h"

#include "FreeRTOS.h" 
#include "task.h"
#include "timers.h"
#include "semphr.h"

#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "timer.h"



#if((defined BSP_DEBUG) && (BSP_DEBUG == TEST_TEMPLE_BSP))
#ifdef _TEMPLATE_GMAC_

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
#define TASK_STK_SIZE      512
#define TASK_PRIOR_TASK10  10
#define TASK_PRIOR_TASK20  11
#define TIME_QUANTA        100

HAL_StatusTypeDef Bsp_Eth_Init(void);
void tcp_client_connect();
void udp_send_connect();
void udp_recv_connect();

k_task_handle_t g_uwTask10;
k_task_handle_t g_uwTask20;
extern xSemaphoreHandle s_xSemaphore;


void task10(void *p)
{
    for(;;) {
        printf("task10 ........ \n");
		csi_kernel_delay(pdMS_TO_TICKS(1000));
    }
}

void task20(void *p)
{
    int i;
    uip_ipaddr_t ipaddr;
    struct timer periodic_timer, arp_timer;
    
    timer_set(&periodic_timer, 5000);
    timer_set(&arp_timer, 10000);
    
    Bsp_Eth_Init();
    tapdev_init();
    uip_init();
 
    uip_ipaddr(ipaddr, 192, 168, 3, 15);     //配置Ip
    uip_sethostaddr(ipaddr);
    uip_ipaddr(ipaddr, 192, 168, 3, 1);     //配置网关
    uip_setdraddr(ipaddr);
    uip_ipaddr(ipaddr, 255, 255, 255, 0);   //配置子网掩码
    uip_setnetmask(ipaddr);
    
    s_xSemaphore = xSemaphoreCreateCounting(10, 0);
    
    // printf("task2 init ok. %04X\n", HTONS(1200));
    // uip_listen(HTONS(1200));
    // udp_send_connect();
    // udp_recv_connect();
    
    while(1) {
        xSemaphoreTake(s_xSemaphore, 1000 / portTICK_PERIOD_MS);

        uip_len = tapdev_read();
        if(uip_len > 0) {
            printf("uip_len is %d \n", uip_len);
            if(BUF->type == htons(UIP_ETHTYPE_IP)) { //IP包，缓冲地址为[12][13]=0x0800
                printf("IP packet. \n");
                uip_arp_ipin();
                uip_input();
                /* If the above function invocation resulted in data that
                should be sent out on the network, the global variable
                uip_len is set to a value > 0. */
                if(uip_len > 0) {
                    uip_arp_out();
                    tapdev_send();
                }
            } else if(BUF->type == htons(UIP_ETHTYPE_ARP)) {//ARP包
                printf("ARP packet. \n");
                uip_arp_arpin();
                /* If the above function invocation resulted in data that
                should be sent out on the network, the global variable
                uip_len is set to a value > 0. */
                if(uip_len > 0) {
                    printf("arp send packet. \n");
                    tapdev_send();
                }
            }
        } else if(timer_expired(&periodic_timer)) {//5s
            printf("time expired 5s \n");
            timer_reset(&periodic_timer);
            for(i = 0; i < UIP_CONNS; i++) {
                uip_periodic(i);
                /* If the above function invocation resulted in data that
                should be sent out on the network, the global variable
                uip_len is set to a value > 0. */
                if(uip_len > 0) {
                    printf("time expired 5s arp out \n");
                    uip_arp_out();
                    tapdev_send();
                }
            }
#if UIP_UDP
            for(i = 0; i < UIP_UDP_CONNS; i++) {
                uip_udp_periodic(i);
                /* If the above function invocation resulted in data that
                should be sent out on the network, the global variable
                uip_len is set to a value > 0. */
                if(uip_len > 0) {
                    uip_arp_out();
                    tapdev_send();
                }
            }
#endif /* UIP_UDP */
            /* Call the ARP timer function every 10 seconds. */
            if(timer_expired(&arp_timer)) {
                printf("time expired 10s \n");
                timer_reset(&arp_timer);
                uip_arp_timer();
            }
        }
    }
} 

u8 tcp_client_databuf[200];       //发送数据缓存

u8 tcp_client_sta;                //客户端状态
//[7]:0,无连接;1,已经连接;
//[6]:0,无数据;1,收到客户端数据
//[5]:0,无数据;1,有数据需要发送

//这里我们假定Server端的IP地址为:192.168.3.2
//这个IP必须根据Server端的IP修改.
//尝试重新连接
void tcp_client_connect()
{
    uip_ipaddr_t ipaddr;
    uip_ipaddr(&ipaddr, 192, 168, 3, 2);    //设置IP为192.168.3.2
    struct uip_conn *conn = uip_connect(&ipaddr, htons(1400));      //端口为1400
    if(NULL != conn)
    {
        printf("uip_connect create success...\n");
    }
}

//终止连接,回调函数
void tcp_client_aborted(void)
{
    tcp_client_sta&=~(1<<7);    //标志没有连接
    tcp_client_connect();        //尝试重新连接
    printf("tcp_client aborted!\r\n");//打印log
}

//连接超时,回调函数
void tcp_client_timedout(void)
{
    tcp_client_sta&=~(1<<7);    //标志没有连接
    printf("tcp_client timeout!\r\n");//打印log
}


//连接关闭,回调函数
void tcp_client_closed(void)
{
    tcp_client_sta&=~(1<<7);    //标志没有连接
    tcp_client_connect();        //尝试重新连接
    printf("tcp_client closed!\r\n");//打印log
}

//连接建立,回调函数
void tcp_client_connected(void)
{ 
    tcp_client_sta|=1<<7;        //标志连接成功
    printf("tcp_client connected!\r\n");//打印log
}

//发送的数据成功送达
void tcp_client_acked(void)
{
    struct tcp_appstate *s=(struct tcp_appstate *)&uip_conn->appstate;
    s->textlen=0;//发送清零
    printf("tcp_client acked!\r\n");//表示成功发送
}

//发送数据给服务端
void tcp_client_senddata(void)
{
    struct tcp_appstate *s = (struct tcp_appstate *)&uip_conn->appstate;
    //s->textptr:发送的数据包缓冲区指针
    //s->textlen:数据包的大小（单位字节）
    if(s->textlen>0)uip_send(s->textptr, s->textlen);//发送TCP数据包
}

//这是一个 TCP 客户端应用回调函数。
//该函数通过UIP_APPCALL(tcp_demo_appcall)调用,实现Web Client的功能.
//当uip事件发生时，UIP_APPCALL函数会被调用,根据所属端口(1400),确定是否执行该函数。
//例如 : 当一个TCP连接被创建时、有新的数据到达、数据已经被应答、数据需要重发等事件
void tcp_client_appcall(void)
{
    printf("tcp_client_appcall......\n");
    struct tcp_appstate *s = (struct tcp_appstate *)&uip_conn->appstate;
    if(uip_aborted())    tcp_client_aborted();       //连接终止
    if(uip_timedout())   tcp_client_timedout();      //连接超时
    if(uip_closed())     tcp_client_closed();        //连接关闭
    if(uip_connected())  tcp_client_connected();     //连接成功
    if(uip_acked())      tcp_client_acked();         //发送的数据成功送达
     //接收到一个新的TCP数据包 
    if (uip_newdata())
    {
        if((tcp_client_sta&(1<<6))==0)//还未收到数据
        {
            if(uip_len>199)
            {
                ((u8*)uip_appdata)[199]=0;
            }
            strcpy((char*)tcp_client_databuf,uip_appdata);
            tcp_client_sta|=1<<6;//表示收到客户端数据
        }
    } else if(tcp_client_sta&(1<<5))//有数据需要发送
    {
        s->textptr=tcp_client_databuf;
        s->textlen=strlen((const char*)tcp_client_databuf);
        tcp_client_sta&=~(1<<5);//清除标记
    }  
    //当需要重发、新数据到达、数据包送达、连接建立时，通知uip发送数据 
    if(uip_rexmit()||uip_newdata()||uip_acked()||uip_connected()||uip_poll())
    {
        tcp_client_senddata();
    }
}

u8 tcp_server_databuf[200];       //发送数据缓存
u8 tcp_server_sta;                //服务端状态
//[7]:0,无连接;1,已经连接;
//[6]:0,无数据;1,收到客户端数据
//[5]:0,无数据;1,有数据需要发送

//终止连接
void tcp_server_aborted(void)
{
    tcp_server_sta&=~(1<<7);    //标志没有连接
    printf("tcp_server aborted!\r\n");//打印log
}

//连接超时
void tcp_server_timedout(void)
{
    tcp_server_sta&=~(1<<7);    //标志没有连接
    printf("tcp_server timeout!\r\n");//打印log
}

//连接关闭
void tcp_server_closed(void)
{
    tcp_server_sta&=~(1<<7);    //标志没有连接
    printf("tcp_server closed!\r\n");//打印log
}

//连接建立
void tcp_server_connected(void)
{
//    struct tcp_appstate *s = (struct tcp_appstate *)&uip_conn->appstate;
    //uip_conn结构体有一个"appstate"字段指向应用程序自定义的结构体。
    //声明一个s指针，是为了便于使用。
     //不需要再单独为每个uip_conn分配内存，这个已经在uip中分配好了。
    //在uip.c 中 的相关代码如下：
    //        struct uip_conn *uip_conn;
    //        struct uip_conn uip_conns[UIP_CONNS]; //UIP_CONNS缺省=10
    //定义了1个连接的数组，支持同时创建几个连接。
    //uip_conn是一个全局的指针，指向当前的tcp或udp连接。
    tcp_server_sta|=1<<7;        //标志连接成功
    printf("tcp_server connected!\r\n");//打印log
} 

//发送的数据成功送达
void tcp_server_acked(void)
{                                 
    struct tcp_appstate *s=(struct tcp_appstate *)&uip_conn->appstate;
    s->textlen=0;//发送清零
    printf("tcp_server acked!\r\n");//表示成功发送
}

//发送数据给客户端
void tcp_server_senddata(void)
{
    struct tcp_appstate *s = (struct tcp_appstate *)&uip_conn->appstate;
    //s->textptr : 发送的数据包缓冲区指针
    //s->textlen ：数据包的大小（单位字节）
    if(s->textlen>0)uip_send(s->textptr, s->textlen);//发送TCP数据包
}
        
//这是一个TCP 服务器应用回调函数。
//该函数通过UIP_APPCALL(tcp_demo_appcall)调用,实现Web Server的功能.
//当uip事件发生时，UIP_APPCALL函数会被调用,根据所属端口(1200),确定是否执行该函数。
//例如 : 当一个TCP连接被创建时、有新的数据到达、数据已经被应答、数据需要重发等事件
void tcp_server_appcall(void)
{
    struct tcp_appstate *s = (struct tcp_appstate *)&uip_conn->appstate;
    if(uip_aborted())   tcp_server_aborted();      //连接终止
    if(uip_timedout())  tcp_server_timedout();     //连接超时
    if(uip_closed())    tcp_server_closed();       //连接关闭
    if(uip_connected()) tcp_server_connected();    //连接成功
    if(uip_acked())     tcp_server_acked();        //发送的数据成功送达
    //接收到一个新的TCP数据包 
    if (uip_newdata())//收到客户端发过来的数据
    {
        if((tcp_server_sta&(1<<6))==0)//还未收到数据
        {
            if(uip_len>199)
            {           
                ((u8*)uip_appdata)[199]=0;
            }            
            strcpy((char*)tcp_server_databuf,uip_appdata);
            tcp_server_sta|=1<<6;//表示收到客户端数据
        }
    }else if(tcp_server_sta&(1<<5))//有数据需要发送
    {
        s->textptr=tcp_server_databuf;
        s->textlen=strlen((const char*)tcp_server_databuf);
        tcp_server_sta&=~(1<<5);//清除标记
    }
    //当需要重发、新数据到达、数据包送达、连接建立时，通知uip发送数据 
    if(uip_rexmit()||uip_newdata()||uip_acked()||uip_connected()||uip_poll())
    {
        tcp_server_senddata();
    }
}

void tcp_Appcall(void)
{
    switch(uip_conn->lport)//本地监听端口对应的事件处理程序 
    {
        case HTONS(1200):
            tcp_server_appcall();
            break;
        default:
            break;
    }
    switch(uip_conn->rport)    //远程连接1400端口
    {
        case HTONS(1400):    //远程连接端口号
            tcp_client_appcall();
           break;
        default:
           break;
    }
}

u8 udp_send_databuf[200];       //发送数据缓存
u8 udp_send_sta;                //发送端状态

//这是一个udp 发送端应用回调函数。
//该函数通过UIP_APPCALL(udp_demo_appcall)调用,实现Web Client的功能.
//当uip事件发生时，UIP_APPCALL函数会被调用,根据所属端口(1500),确定是否执行该函数。
//例如 : 当一个udp连接被创建时、有新的数据到达、数据已经被应答、数据需要重发等事件
void udp_send_appcall(void)
{
    struct uip_appstate *s = (struct uip_appstate *)&uip_udp_conn->appstate;
    
    if(uip_poll())//当前连接空闲轮训
    {
        printf("udp_send uip_poll!\r\n");//打印log
        if(udp_send_sta&(1<<5))//需要发送数据
        { 
            s->textptr=udp_send_databuf;
            s->textlen=strlen((const char*)udp_send_databuf);
            udp_send_sta&=~(1<<5);//清除标记
            uip_send(s->textptr, s->textlen);//发送udp数据包
            uip_udp_send(s->textlen);
        }
    }
}

//建立一个udp_client的连接
void udp_send_connect()
{
    uip_ipaddr_t ipaddr;
    static struct uip_udp_conn *c=0;    
    uip_ipaddr(&ipaddr, 192, 168, 3, 2);    //设置IP为192.168.3.2
    if(c!=0)
    {
        uip_udp_remove(c);                  //已经建立连接则删除连接
    }
    c = uip_udp_new(&ipaddr, htons(1500));     //端口为1500
    //发送端发送的数据端口为1500
}

u8 udp_recv_databuf[200];       //发送数据缓存
u8 udp_recv_sta;                //客户端状态

void udp_recv_appcall(void)
{
//    struct uip_appstate *s = (struct uip_appstate *)&uip_udp_conn->appstate;
    
    //接收到一个新的udp数据包 
    if (uip_newdata())//收到客户端发过来的数据
    {
        if((udp_recv_sta & (1 << 6)) == 0)//还未收到数据
        {
            if(uip_len > 199)
            {
                ((u8*)uip_appdata)[199] = 0;
            }
            strcpy((char*)udp_recv_databuf, uip_appdata);
            udp_recv_sta |= 1 << 6;//表示收到客户端数据
            printf("udp recv data %s. \n", (char *)uip_appdata);
        }
    }
    if(uip_poll())//udp空转
    {
        printf("udp_server uip_poll!\r\n");//打印log  
    }
}


//建立UDP接收链接
//建立UDP服务器需要将目标IP设置为全1 并对应端口为0,绑定相应的数据端口
void udp_recv_connect(void)
{
    uip_ipaddr_t ipaddr;
    static struct uip_udp_conn *c = 0;
    uip_ipaddr(&ipaddr, 0xff, 0xff, 0xff, 0xff);    //将远程IP设置为 255.255.255.255 具体原理见uip.c的源码
    if(c!=0)    //已经建立连接则删除连接
    {
        uip_udp_remove(c);
    }
    c = uip_udp_new(&ipaddr,0);     //远程端口为0
    if(c)
    {
        uip_udp_bind(c, HTONS(1600));
    }
}

void Udp_Appcall(void)
{
    printf("Udp_Appcall....\n");
    switch(uip_udp_conn->lport)//本地监听端口1600 
    {
        case HTONS(1600):
            printf("Udp_Appcall receive....\n");
            udp_recv_appcall(); 
            break;
        default:
            break;
    }
    switch(uip_udp_conn->rport)    //远程连接1500端口,也就是数据发送端
    {
        case HTONS(1500):
            printf("Udp_Appcall send....\n");
            udp_send_appcall();
           break;
        default: 
           break;
    }
}

int main(void)
{
    printf("start to execute main fun. \n");
    csi_kernel_init();
    
	csi_kernel_task_new((k_task_entry_t)task10, "task10", NULL, TASK_PRIOR_TASK10, TIME_QUANTA, NULL, TASK_STK_SIZE, &g_uwTask10);
    if (g_uwTask10 == NULL) {
        csi_kernel_sched_resume(0);
        printf("Fail to create task10! \r\n");
    }
    
	csi_kernel_task_new((k_task_entry_t)task20, "task20", NULL, TASK_PRIOR_TASK20, TIME_QUANTA, NULL, 2*TASK_STK_SIZE, &g_uwTask20);
    if (g_uwTask20 == NULL) {
        csi_kernel_sched_resume(0);
        printf("Fail to create task20! \r\n");
    }
    
    csi_kernel_start();

    
    while(1)
    {
        printf("Hello FreeRTOS.\n");
        HAL_Delay(100*1000);
    }
    return 0;
}

#endif 
#endif