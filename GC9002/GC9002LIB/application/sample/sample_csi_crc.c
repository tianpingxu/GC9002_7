#include "sample_def.h"
#include "csi_crc.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_CRC_

#define TICK_CHECK_RETURN(ret)                      \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);
        
#define CRC32_TEST_BUFF_LEN	16//2048
int main(void)
{
    uint32_t val[3];
    __attribute__ ((aligned(64))) unsigned int data[4];
    __attribute__ ((aligned(64))) unsigned char buff[16];
//    unsigned int data[4];
//    unsigned int buff[4];
    unsigned int crc;
    data[0] = 0x11111111;
    data[1] = 0x22222222;
    data[2] = 0x33333333;
    data[3] = 0x44444444;
    memcpy(buff, data, 16);
    // uint8_t *buff = (uint8_t *)(0x02000000);//FPGA工程定义的变量在ROM空间，CRC无法访问ROM，所以这里直接使用RAM空间
    // for(int i = 0; i < CRC32_TEST_BUFF_LEN; i++)
    // {
    //     buff[i] = i;
    // }
    crc = csi_crc32_le(0xFFFFFFFF, (unsigned char*)buff, CRC32_TEST_BUFF_LEN-2);
    printf("crc0:%08X\n", crc);
    crc = csi_crc32_le(~crc, (unsigned char*)&buff[CRC32_TEST_BUFF_LEN-2], 2);
    printf("crc0:%08X\n", crc);
    
    crc = csi_crc32_le(0xFFFFFFFF, (unsigned char*)buff, CRC32_TEST_BUFF_LEN-2);
    printf("crc1:%08X\n", crc);
    crc = crc32_le((unsigned char*)&buff[CRC32_TEST_BUFF_LEN-2], 2, ~crc);
    printf("crc1:%08X\n", crc);
    
    crc = crc32_le((unsigned char*)buff, CRC32_TEST_BUFF_LEN-2, ~0);
    printf("crc2:%08X\n", crc);
    crc = crc32_le((unsigned char*)&buff[CRC32_TEST_BUFF_LEN-2], 2, ~crc);
    printf("crc2:%08X\n", crc);

    crc = csi_crc32_le(0xFFFFFFFF, (unsigned char*)buff, CRC32_TEST_BUFF_LEN);
    printf("crc3:%08X\n", crc);
    val[1] = csi_crc32_be(0xFFFFFFFF, (unsigned char*)buff, CRC32_TEST_BUFF_LEN);
    val[2] = crc32_le((unsigned char*)buff, CRC32_TEST_BUFF_LEN, 0xFFFFFFFF);
    
//    printf("value:%08X\n", crc);
//    printf("value:%08X\n", val[1]);
    printf("value:%08X\n", val[2]);
    while(1)
    {
		mdelay(100);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_CRC_ */
#endif /* BSP_DEBUG */