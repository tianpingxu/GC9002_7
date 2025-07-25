/*
 * Copyright (c) 2001, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: tapdev.c,v 1.8 2006/06/07 08:39:58 adam Exp $
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "gc90xx_hal_conf.h"

#include "uip.h"
#include "uip_arp.h"


struct uip_eth_addr uip_mac;
#define DEFAULT_MAC_ADDRESS {MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5}

uint8_t rxBuf[1500];
int low_level_output(uint8_t *buf, uint32_t bufLen);
int low_level_input(uint8_t *buf);

/*---------------------------------------------------------------------------*/

void
tapdev_init(void)
{
	uint8_t macaddr[6] = DEFAULT_MAC_ADDRESS;
	
	for (int i = 0; i < 6; i++)
	{
		uip_mac.addr[i] = macaddr[i];
	}
	uip_setethaddr(uip_mac);
}

/*---------------------------------------------------------------------------*/
unsigned int
tapdev_read(void)
{
	int ret = low_level_input(uip_buf);
	if(ret < 0) 
	{
		return 0;
	}
	else
	{
		return ret;
	}
}
/*---------------------------------------------------------------------------*/
void
tapdev_send(void)
{
	low_level_output(uip_buf, uip_len);
}
/*---------------------------------------------------------------------------*/
