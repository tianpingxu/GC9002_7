/*
 * crc16.h
 *
 *  Created on: 2024年9月15日
 *      Author: TianPing Xu
 */

#ifndef CRC16_H
#define CRC16_H

#include "board.h"

unsigned long xtp_strlen(unsigned char *str);
void *xtp_memset(void *src, unsigned char ch, unsigned short count);
void *xtp_memcpy(void *dest, const void *src, unsigned short count);
char *xtp_strcpy(char *dest, const char *src);
int xtp_strcmp(char *s1, const char *s2);

static uint16_t _gen_ccitt_crc16(uint8_t byte, uint16_t result);

uint16_t gen_crc16_ccitt_stream(const uint8_t *buffer, uint16_t length);


#endif //PROTOCOL_H