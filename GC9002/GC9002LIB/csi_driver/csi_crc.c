#include <csi_crc.h>
#ifdef CONFIG_CSI_DRV_CRC_ENABLED

uint8_t csi_crc7_be(uint8_t crc, uint8_t* data, uint32_t size)
{
    return crc7_mmc(data, size, crc);
}
uint8_t csi_crc8(uint8_t crc, uint8_t* data, uint32_t size)
{
    return crc8(data, size, crc);
}
uint8_t csi_crc8_maxim(uint8_t crc, uint8_t* data, uint32_t size)
{
    return crc8_maxim(data, size, crc);
}
uint16_t csi_crc16(uint16_t crc, uint8_t* data, uint32_t size)
{
    return crc16_ibm(data, size, crc);
}
uint16_t csi_crc16_ccitt(uint16_t crc, uint8_t* data, uint32_t size)
{
    return crc16_ccitt(data, size, crc);
}
uint16_t csi_crc16_dnp(uint16_t crc, uint8_t* data, uint32_t size)
{
    return crc16_dnp(data, size, crc);
}
uint16_t csi_crc16_itu(uint16_t crc, uint8_t* data, uint32_t size)
{
    return crc16_x25(data, size, crc);
}
uint32_t csi_crc32_le(uint32_t crc, uint8_t* data, uint32_t size)
{
    uint32_t crc32 = ~crc;
    uint32_t index = 0;
    uint32_t size_cal = size;//
    if(crc32 == 0x00000000)
    {
#ifdef HAL_CRC32_MODULE_ENABLED
        if(IS_ADDR_ALIGNED_64B(data) && (size & 0xFFFFFFFC) > 0)//Address misalignment
        {
            HAL_CRC32_Begin((uint32_t *)data, (size & 0xFFFFFFFC));
            HAL_CRC32_GetResult(&crc32, HAL_MAX_DELAY);
            index += (size & 0xFFFFFFFC);
            size_cal = size & 0x3;
        }
#endif
    }
    if(size_cal > 0)
    {
        crc32 = crc32_le(&data[index], size_cal, ~crc32);
    }
    return crc32;
}
uint32_t csi_crc32_be(uint32_t crc, uint8_t* data, uint32_t size)
{
    return crc32_mpeg_2(data, size, crc);
}



#endif /* CONFIG_CSI_DRV_CRC_ENABLED */