/*
 * crc16.c
 *
 *  Created on: 2024年9月15日
 *      Author: TianPing Xu
 */

#include "crc16.h"

/**
 * @brief get string len
 * @param str higher bits data
 * @return string len
 */
unsigned long xtp_strlen(unsigned char *str) {
    unsigned long len = 0;
    if (str == NULL) {
        return 0;
    }

    for (len = 0; *str++ != '\0';) {
        len++;
    }

    return len;
}

/**
 * @brief  assign ch to the first count bytes of the memory address src
 * @param src srouce address
 * @param ch set value
 * @param count length of set address
 * @return void
 */
void *xtp_memset(void *src, unsigned char ch, unsigned short count) {
    unsigned char *tmp = (unsigned char *) src;
    if (src == NULL) {
        return NULL;
    }

    while (count--) {
        *tmp++ = ch;
    }
    return src;
}

/**
 * @brief copy count bytes data from src to dest
 * @param dest destination address
 * @param src srouce address
 * @param count length of copy data
 * @return void
 */
void *xtp_memcpy(void *dest, const void *src, unsigned short count) {
    unsigned char *pdest = (unsigned char *) dest;
    const unsigned char *psrc = (const unsigned char *) src;
    unsigned short i;

    if (dest == NULL || src == NULL) {
        return NULL;
    }

    if ((pdest <= psrc) || (pdest > psrc + count)) {
        for (i = 0; i < count; i++) {
            pdest[i] = psrc[i];
        }
    } else {
        for (i = count; i > 0; i--) {
            pdest[i - 1] = psrc[i - 1];
        }
    }

    return dest;
}

/**
 * @brief copy string src to string dest
 * @param dest destination address
 * @param src srouce address
 * @return the tail of destination
 */
char *xtp_strcpy(char *dest, const char *src) {
    char *p = dest;
    while (*src != '\0') {
        *dest++ = *src++;
    }
    *dest = '\0';
    return p;
}

/**
 * @brief compare string s1 with string s2
 * @param str1 srouce address
 * @param str2 destination address
 * @return compare result
 */
int xtp_strcmp(char *str1, const char *str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}


//HRS_crc Header至DATA参与CRC16校验
static uint16_t _gen_ccitt_crc16(uint8_t byte, uint16_t result) {
	result = ((uint16_t) (result >> 8)) | ((uint16_t) (result << 8));
	result ^= byte;
	result ^= (result & 0xff) >> 4;
	result ^= (uint16_t) (((uint16_t) (result << 8)) << 4);
	result ^= ((uint8_t) (((uint8_t) (result & 0xff)) << 5)) |
	((uint16_t) ((uint16_t) ((uint8_t) (((uint8_t) (result & 
	0xff)) >> 3)) << 8));
	return result;
}

uint16_t gen_crc16_ccitt_stream(const uint8_t *buffer, uint16_t length) {
	uint16_t position;
    uint16_t crc = 0x0000;
	for (position = 0; position < length; position++) {
	    crc = _gen_ccitt_crc16(buffer[position], crc);
	}
	return crc;
}