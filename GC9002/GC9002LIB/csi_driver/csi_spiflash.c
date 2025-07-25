/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Analyze the SFDP (Serial Flash Discoverable Parameters) which from JESD216/A/B (V1.X) standard.
 *           JESD216  (V1.0) document: http://www.jedec.org/sites/default/files/docs/JESD216.pdf
 *           JESD216A (V1.5) document: http://www.jedec.org/sites/default/files/docs/JESD216A.pdf
 *           JESD216B (V1.6) document: http://www.jedec.org/sites/default/files/docs/JESD216B.pdf
 *
 * Created on: 2016-05-26
 */
#include <csi_spiflash.h>
#ifdef CONFIG_CSI_DRV_SPIFLASH_ENABLED

__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_set_write_protect(csi_spiflash_t *spiflash);
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_clear_write_protect(csi_spiflash_t *spiflash);
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_send_opcode(csi_spiflash_t *spiflash, uint8_t opcode);
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_send_opcode_with_addr(csi_spiflash_t *spiflash, uint8_t opcode, uint32_t addr, uint8_t addr_size);

__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_get_flash_info(csi_spiflash_t *spiflash, csi_spiflash_info_t *flash_info);
__CSI_FLASH_DRIVER_ATTRIBUTE static int32_t     qspiflash_read(csi_spiflash_t *spiflash, uint32_t offset, void *data, uint32_t size);
__CSI_FLASH_DRIVER_ATTRIBUTE static int32_t     qspiflash_program(csi_spiflash_t *spiflash, uint32_t offset, const void *data, uint32_t size);
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_erase(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size, csi_spiflash_erase_type_t erase_type);

__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_read_sr(csi_spiflash_t *spiflash, csi_spiflash_info_t *flash_info);
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t SFDP_flash_read_parameter(csi_spiflash_t *spiflash, csi_spiflash_info_t *flash_info);
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t SFDP_flash_qspi_config(csi_spiflash_t *spiflash);
__CSI_FLASH_DRIVER_ATTRIBUTE static int32_t     SFDP_flash_read(csi_spiflash_t *spiflash, uint32_t offset, void *data, uint32_t size);
__CSI_FLASH_DRIVER_ATTRIBUTE static int32_t     SFDP_flash_program(csi_spiflash_t *spiflash, uint32_t offset, const void *data, uint32_t size);
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t SFDP_flash_erase(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size, csi_spiflash_erase_type_t erase_type);

#if(!(defined CSI_FLASH_SFDP_CMD_ENABLE) || CSI_FLASH_SFDP_CMD_ENABLE == 0U)
static csi_error_t nor_flash_GD25Q257D_qspi_config(csi_spiflash_t *spiflash);
static int32_t     nor_flash_GD25Q257D_read(csi_spiflash_t *spiflash, uint32_t offset, void *data, uint32_t size);
static int32_t     nor_flash_GD25Q257D_program(csi_spiflash_t *spiflash, uint32_t offset, const void *data, uint32_t size);
static csi_error_t nor_flash_GD25Q257D_erase(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size);
#endif /* !(defined CSI_FLASH_SFDP_CMD_ENABLE) || CSI_FLASH_SFDP_CMD_ENABLE == 0U */

#if(defined CSI_QSPI_FLASH_CNT || CSI_QSPI_FLASH_CNT > 0)
    #define CSI_CONFIG_CNT  CSI_QSPI_FLASH_CNT
#else
    #define CSI_CONFIG_CNT  1
#endif
static csi_qspi_direct_config_t qspi_direct_config[CSI_CONFIG_CNT];



csi_error_t csi_spiflash_spi_init(csi_spiflash_t *spiflash, uint32_t spi_idx, void(*spi_cs_callback)(csi_gpio_pin_state_t value))
{
    if(spiflash == NULL)
    {
        return CSI_ERROR;
    }
    spiflash->spi_cs_callback = spi_cs_callback;
    csi_spi_init(&(spiflash->spi_qspi.spi), spi_idx);
    spiflash->priv = (void *)&(spiflash->spi_qspi.spi);
    spiflash->spi_qspi.type = SPIFLASH_BUS_SPI;
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE csi_error_t csi_spiflash_qspi_init(csi_spiflash_t *spiflash, uint32_t qspi_idx, void (*spi_cs_callback)(csi_gpio_pin_state_t value))
{
    if(spiflash == NULL)
    {
        return CSI_ERROR;
    }
    csi_error_t ret;
    spiflash->spi_cs_callback = spi_cs_callback;
    ret = csi_qspi_init(&(spiflash->spi_qspi.qspi), qspi_idx);
    if(ret != CSI_OK)
    {
        return ret;
    }
    spiflash->spi_qspi.qspi.direct_config = &qspi_direct_config[qspi_idx];
    spiflash->priv = (void *)&(spiflash->spi_qspi.qspi);
    spiflash->spi_qspi.type = SPIFLASH_BUS_QSPI;
    ret = qspiflash_get_flash_info(spiflash, &spiflash->flash_prv_info);
    if(ret != CSI_OK)
    {
        return ret;
    }
    uint32_t data[2];
    if(csi_spiflash_read(spiflash, 0, data, 8) != 8)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
void csi_spiflash_spi_uninit(csi_spiflash_t *spiflash)
{
    if(spiflash == NULL)
    {
        return;
    }
    spiflash->spi_cs_callback = NULL;
    spiflash->priv = NULL;
    csi_spi_uninit(&(spiflash->spi_qspi.spi));
}
void csi_spiflash_qspi_uninit(csi_spiflash_t *spiflash)
{
    if(spiflash == NULL)
    {
        return;
    }
    spiflash->spi_cs_callback = NULL;
    spiflash->priv = NULL;
    csi_qspi_uninit(&(spiflash->spi_qspi.qspi));
}
csi_error_t csi_spiflash_get_flash_info(csi_spiflash_t *spiflash, csi_spiflash_info_t *flash_info)
{
    if(spiflash == NULL || flash_info == NULL)
    {
        return CSI_ERROR;
    }
    memcpy(flash_info, (void *)&spiflash->flash_prv_info, sizeof(csi_spiflash_info_t));
    return CSI_OK;
}

__CSI_FLASH_DRIVER_ATTRIBUTE int32_t csi_spiflash_read(csi_spiflash_t *spiflash, uint32_t offset, void *data, uint32_t size)
{
    if(spiflash == NULL || data == NULL)
    {
        return CSI_ERROR;
    }
    if(spiflash->spi_qspi.type == SPIFLASH_BUS_SPI)
    {
        return CSI_UNSUPPORTED;
    }
    else if(spiflash->spi_qspi.type == SPIFLASH_BUS_QSPI)
    {
        return qspiflash_read(spiflash, offset, data, size);
    }
    return size;
}
__CSI_FLASH_DRIVER_ATTRIBUTE int32_t csi_spiflash_program(csi_spiflash_t *spiflash, uint32_t offset, const void *data, uint32_t size)
{
    if(spiflash == NULL || data == NULL)
    {
        return CSI_ERROR;
    }
    if(spiflash->spi_qspi.type == SPIFLASH_BUS_SPI)
    {
        return CSI_UNSUPPORTED;
    }
    else if(spiflash->spi_qspi.type == SPIFLASH_BUS_QSPI)
    {
        return qspiflash_program(spiflash, offset, data, size);
    }
    return size;
}
__CSI_FLASH_DRIVER_ATTRIBUTE csi_error_t csi_spiflash_erase(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size)
{
    if(spiflash == NULL)
    {
        return CSI_ERROR;
    }
    if(spiflash->spi_qspi.type == SPIFLASH_BUS_SPI)
    {
        return CSI_UNSUPPORTED;
    }
    else if(spiflash->spi_qspi.type == SPIFLASH_BUS_QSPI)
    {
        return qspiflash_erase(spiflash, offset, size, SECTOR_ERASE);
    }
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE csi_error_t csi_spiflash_block_erase(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size)
{
    if(spiflash == NULL)
    {
        return CSI_ERROR;
    }
    if(spiflash->spi_qspi.type == SPIFLASH_BUS_SPI)
    {
        return CSI_UNSUPPORTED;
    }
    else if(spiflash->spi_qspi.type == SPIFLASH_BUS_QSPI)
    {
        return qspiflash_erase(spiflash, offset, size, BLOCK_ERASE);
    }
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE csi_error_t csi_spiflash_chip_erase(csi_spiflash_t *spiflash)
{
    if(spiflash == NULL)
    {
        return CSI_ERROR;
    }
    if(spiflash->spi_qspi.type == SPIFLASH_BUS_SPI)
    {
        return CSI_UNSUPPORTED;
    }
    else if(spiflash->spi_qspi.type == SPIFLASH_BUS_QSPI)
    {
        return qspiflash_erase(spiflash, 0, 0, CHIP_ERASE);
    }
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE csi_error_t csi_spiflash_read_reg(csi_spiflash_t *spiflash, uint8_t cmd_code, uint8_t *data, uint32_t size)
{
    if(spiflash == NULL)
    {
        return CSI_ERROR;
    }
    csi_qspi_command_t command;    
    /* Read device id operations */
    command.instruction.value     = cmd_code;
    command.instruction.bus_width = QSPI_CFG_BUS_SINGLE;
    command.instruction.disabled  = false;
    command.alt.disabled          = true;
    command.address.disabled      = true;
    command.address.value         = 0;
    command.address.size          = 0;
    command.address.bus_width     = QSPI_CFG_BUS_SINGLE;
    command.data.bus_width        = QSPI_CFG_BUS_SINGLE;
    command.dummy_count           = 0;
    
    if(csi_qspi_receive(&spiflash->spi_qspi.qspi, &command, data, size, CSI_TIMEOUT_CLOCKS) != size)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE csi_error_t csi_spiflash_write_reg(csi_spiflash_t *spiflash, uint8_t cmd_code, uint8_t *data, uint32_t size)
{
    if(spiflash == NULL)
    {
        return CSI_ERROR;
    }
    csi_qspi_command_t command;    
    /* Read device id operations */
    command.instruction.value     = cmd_code;
    command.instruction.bus_width = QSPI_CFG_BUS_SINGLE;
    command.instruction.disabled  = false;
    command.alt.disabled          = true;
    command.address.disabled      = true;
    command.address.value         = 0;
    command.address.size          = 0;
    command.address.bus_width     = QSPI_CFG_BUS_SINGLE;
    command.data.bus_width        = QSPI_CFG_BUS_SINGLE;
    command.dummy_count           = 0;
    
    if(csi_qspi_send(&spiflash->spi_qspi.qspi, &command, data, size, CSI_TIMEOUT_CLOCKS) != size)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
csi_error_t csi_spiflash_lock(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size)
{
    return CSI_OK;
}
csi_error_t csi_spiflash_unlock(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size)
{
    return CSI_OK;
}
int csi_spiflash_is_locked(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size)
{
    return size;
}
csi_error_t csi_spiflash_config_data_line(csi_spiflash_t *spiflash, csi_spiflash_data_line_t line)
{
    return CSI_OK;
}
uint32_t csi_spiflash_frequence(csi_spiflash_t *spiflash, uint32_t hz)
{
    return hz;
}
csi_error_t csi_spiflash_release_power_down(csi_spiflash_t *spiflash)
{
    return CSI_OK;
}
void csi_spiflash_power_down(csi_spiflash_t *spiflash)
{
    
}










__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_set_write_protect(csi_spiflash_t *spiflash)
{
#if(defined CSI_QSPI_WRITE_PROTECTION_ENABLE && CSI_QSPI_WRITE_PROTECTION_ENABLE == 1U)
    csi_qspi_write_protection_ctrl(&spiflash->spi_qspi.qspi, true);
#else
    switch(spiflash->flash_prv_info.flash_id)
    {
        case CSI_GD25Q257D_ID:
        {
            uint32_t write_data = 0;
            if(qspiflash_send_opcode(spiflash, 0x50) != 0)//opcode 0x50 to set the WELe to config reg, 1 data bytes to write,
            {
                return CSI_ERROR;
            }
            write_data = 0x7C;
            if(csi_spiflash_write_reg(spiflash, 0x01, (const void *)&write_data, 1) != 1)
            {
                return CSI_ERROR;
            }
            
        }break;
        default: return CSI_ERROR;
    }
#endif
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_clear_write_protect(csi_spiflash_t *spiflash)
{
#if(1)
    csi_qspi_write_protection_ctrl(&spiflash->spi_qspi.qspi, false);
#else
    switch(spiflash->flash_prv_info.flash_id)
    {
        case CSI_GD25Q257D_ID:
        {
            uint32_t write_data = 0;
            if(qspiflash_send_opcode(spiflash, 0x50) != 0)//opcode 0x50 to set the WELe to config reg, 1 data bytes to write,
            {
                return CSI_ERROR;
            }
            write_data = 0x00;
            if(csi_spiflash_write_reg(spiflash, 0x01, (const void *)&write_data, 1) != 1)
            {
                return CSI_ERROR;
            }
            
        }break;
        default: return CSI_ERROR;
    }
#endif
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_get_SFDP_by_addr(csi_spiflash_t *spiflash, uint32_t addr, uint32_t data[], uint32_t size)
{
    csi_qspi_command_t command;
    /* Read SFDP operations */
    command.instruction.value     = CSI_COM_OP_READ_SFDP;
    command.instruction.bus_width = QSPI_CFG_BUS_SINGLE;
    command.instruction.disabled  = false;
    command.alt.disabled          = true;
    command.address.disabled      = false;
    command.address.value         = addr;
    command.address.size          = 3;// default
    command.address.bus_width     = QSPI_CFG_BUS_SINGLE;
    command.data.bus_width        = QSPI_CFG_BUS_SINGLE;
    command.dummy_count           = 1;
    if(csi_qspi_receive(&spiflash->spi_qspi.qspi, &command, data, size, CSI_TIMEOUT_CLOCKS) != size)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_send_opcode(csi_spiflash_t *spiflash, uint8_t opcode)
{
    csi_qspi_command_t command;
    /* Read SFDP operations */
    command.instruction.value     = opcode;
    command.instruction.bus_width = QSPI_CFG_BUS_SINGLE;
    command.instruction.disabled  = false;
    command.alt.disabled          = true;
    command.address.disabled      = true;
    command.address.value         = 0;
    command.address.size          = 0;
    command.address.bus_width     = QSPI_CFG_BUS_SINGLE;
    command.data.bus_width        = QSPI_CFG_BUS_SINGLE;
    command.dummy_count           = 0;
    if(csi_qspi_send(&spiflash->spi_qspi.qspi, &command, NULL, 0, CSI_TIMEOUT_CLOCKS) != 0)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_send_opcode_with_addr(csi_spiflash_t *spiflash, uint8_t opcode, uint32_t addr, uint8_t addr_size)
{
    csi_qspi_command_t command;
    /* Read SFDP operations */
    command.instruction.value     = opcode;
    command.instruction.bus_width = QSPI_CFG_BUS_SINGLE;
    command.instruction.disabled  = false;
    command.alt.disabled          = true;
    command.address.disabled      = false;
    command.address.value         = addr;
    command.address.size          = addr_size;
    command.address.bus_width     = QSPI_CFG_BUS_SINGLE;
    command.data.bus_width        = QSPI_CFG_BUS_SINGLE;
    command.dummy_count           = 0;
    if(csi_qspi_send(&spiflash->spi_qspi.qspi, &command, NULL, 0, CSI_TIMEOUT_CLOCKS) != 0)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_read_sr(csi_spiflash_t *spiflash, csi_spiflash_info_t *flash_info)
{
    if(spiflash->opcode.read_sr1 == 0 || csi_spiflash_read_reg(spiflash, spiflash->opcode.read_sr1, (void *)&flash_info->SR.SR1.u8_value, 1) != CSI_OK)
    {
        return CSI_ERROR;
    }
    if(spiflash->opcode.read_sr2 == 0 || csi_spiflash_read_reg(spiflash, spiflash->opcode.read_sr2, (void *)&flash_info->SR.SR2.u8_value, 1) != CSI_OK)
    {
        return CSI_ERROR;
    }
    if(spiflash->opcode.read_sr3 == 0 || csi_spiflash_read_reg(spiflash, spiflash->opcode.read_sr3, (void *)&flash_info->SR.SR3.u8_value, 1) != CSI_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t SFDP_flash_read_parameter(csi_spiflash_t *spiflash, csi_spiflash_info_t *flash_info)
{
    uint32_t u32_value[2];
#if(defined CSI_QSPI_FLASH_DEBUG && CSI_QSPI_FLASH_DEBUG == 1U)
    uint32_t data[64];
    for (int i = 0; i < 64; i += 2) {
        if(qspiflash_get_SFDP_by_addr(spiflash, 0x00+0x08*i, &data[i], 0x08) != CSI_OK)
        {
            return CSI_ERROR;
        }
    }
#endif
    // 1. Get SFDP Signature, and get parameter table pointer
    if(qspiflash_get_SFDP_by_addr(spiflash, 0x00, u32_value, 4) != CSI_OK)
    {
        SFUD_INFO("Error: Can't read SFDP header.");
        return CSI_ERROR;
    }
    if(((union SFDP_00H *)&u32_value[0])->SFDP_Signature != CSI_SFDP_SIGNATURE)
    {
        return CSI_ERROR;
    }
    // 2. Get SFDP Minor Revision Number, SFDP Major Revision Number, Number of Parameters Headers
    if(qspiflash_get_SFDP_by_addr(spiflash, 0x04, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    if(((union SFDP_04H *)&u32_value[0])->SFDP_Major_Revision > CSI_SUPPORT_MAX_SFDP_MAJOR_REV)
    {
        SFUD_INFO("Error: This reversion(V%d.%d) of SFDP is not supported.",
            ((union SFDP_04H *)&u32_value[0])->SFDP_Major_Revision, ((union SFDP_04H *)&u32_value[0])->SFDP_Minor_Revision);
        return CSI_ERROR;
    }
    SFUD_DEBUG("Check SFDP header is OK. The reversion is V%d.%d, NPN is %d.",
            ((union SFDP_04H *)&u32_value[0])->SFDP_Major_Revision,
            ((union SFDP_04H *)&u32_value[0])->SFDP_Minor_Revision,
            ((union SFDP_04H *)&u32_value[0])->Para_Headers_Num);
    // 3. Get First address of JEDEC Flash Parameter table
    if(qspiflash_get_SFDP_by_addr(spiflash, 0x0C, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    flash_info->JEDEC_PTP_base_addr = ((union SFDP_0CH *)&u32_value[0])->Parameter_Table_Pointer;
    flash_info->JEDEC_PTP_EN = CSI_IS_SFDP_PTP_BASE_ADDR(flash_info->JEDEC_PTP_base_addr);
    if(flash_info->JEDEC_PTP_EN == 0)
    {
        return CSI_ERROR;
    }
    // 4. Get First address of GigaDevice Flash Parameter table
    if(qspiflash_get_SFDP_by_addr(spiflash, 0x14, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    flash_info->MAKER_PTP_base_addr = ((union SFDP_14H *)&u32_value[0])->Parameter_Table_Pointer;
    flash_info->MAKER_PTP_EN = CSI_IS_SFDP_PTP_BASE_ADDR(flash_info->MAKER_PTP_base_addr);
    // 5. First address of 4-Byte Address Flash Parameter table
    if(qspiflash_get_SFDP_by_addr(spiflash, 0x1C, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    flash_info->BYTE4_PTP_base_addr = ((union SFDP_1CH *)&u32_value[0])->Parameter_Table_Pointer;
    flash_info->BYTE4_PTP_EN = CSI_IS_SFDP_PTP_BASE_ADDR(flash_info->BYTE4_PTP_base_addr);
    
    // 6. Get flash Manufacturer ID
    if(qspiflash_get_SFDP_by_addr(spiflash, 0x10, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    flash_info->flash_name = (char *)"SFDP_FLASH";
    flash_info->flash_id = ((union SFDP_10H *)&u32_value[0])->ID_Num_Manufacturer;
    
    // 7. Get sector size
    if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x1C, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    flash_info->sector_size = 1;     //4K Bytes     4096
    if(((union SFDP_JEDEC_PT_1CH *)&u32_value[0])->Sector_Type_1_Size < 31)
    {
        flash_info->sector_size <<= ((union SFDP_JEDEC_PT_1CH *)&u32_value[0])->Sector_Type_1_Size;
    }
    
    // 8. Get block size
    if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x20, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    flash_info->block_size = 1;     //64K Bytes   65536
    if(((union SFDP_JEDEC_PT_20H *)&u32_value[0])->Sector_Type_3_Size < 31)
    {
        flash_info->block_size <<= ((union SFDP_JEDEC_PT_20H *)&u32_value[0])->Sector_Type_3_Size;
    }
    
    // 9. Get page size
    if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x28, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    flash_info->page_size = 1;      //256 Bytes
    if(((union SFDP_JEDEC_PT_28H *)&u32_value[0])->Page_Size < 31)
    {
        flash_info->page_size <<= ((union SFDP_JEDEC_PT_28H *)&u32_value[0])->Page_Size;
    }

    // 10. Get flash capacity
    if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x04, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    switch ((((union SFDP_JEDEC_PT_04H *)&u32_value[0])->Flash_Memory_Density >> 31) & 0x1) {
    case 0:
        flash_info->capacity = 1 + (((union SFDP_JEDEC_PT_04H *)&u32_value[0])->Flash_Memory_Density >> 3);//eg: 32M Bytes [0x0000 0000 ~ 0x01FF FFFF]
        break;
    case 1:
        ((union SFDP_JEDEC_PT_04H *)&u32_value[0])->Flash_Memory_Density &= 0x7FFFFFFF;
        if (((union SFDP_JEDEC_PT_04H *)&u32_value[0])->Flash_Memory_Density > sizeof(flash_info->capacity) * 8 + 3) {
            flash_info->capacity = 0;
            SFUD_INFO("Error: The flash capacity is grater than 32 Gb/ 4 GB! Not Supported.");
            return CSI_ERROR;
        }
        flash_info->capacity = 1L << (((union SFDP_JEDEC_PT_04H *)&u32_value[0])->Flash_Memory_Density - 3);
        break;
    }
    SFUD_DEBUG("Capacity is %d Bytes.", flash_info->capacity);
    flash_info->xip_addr = 0;
    flash_info->protect_blk_size = 0;
    flash_info->protect_type = 0;

    // 11. Get address mode
    if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x00, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    if(((union SFDP_JEDEC_PT_00H *)&u32_value[0])->Address_Bytes_Num == 0)
    {
        flash_info->addr_3_byte = 1;
        flash_info->addr_4_byte = 0;
    }
    else if(((union SFDP_JEDEC_PT_00H *)&u32_value[0])->Address_Bytes_Num == 1)
    {
        flash_info->addr_3_byte = 1;
        flash_info->addr_4_byte = 1;
    }
    else if(((union SFDP_JEDEC_PT_00H *)&u32_value[0])->Address_Bytes_Num == 2)
    {
        flash_info->addr_3_byte = 0;
        flash_info->addr_4_byte = 1;
    }
    // 12. Get read config
    if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x10, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
#if(defined CSI_QSPI_FLASH_READ_444_DISABLE && CSI_QSPI_FLASH_READ_444_DISABLE == 1U)
    ((union SFDP_JEDEC_PT_10H *)&u32_value[0])->Fast_Read_4_4_4 = 0;
#endif
    if(((union SFDP_JEDEC_PT_10H *)&u32_value[0])->Fast_Read_4_4_4 == 1)
    {
        if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x18, u32_value, 4) != CSI_OK)
        {
            return CSI_ERROR;
        }
        spiflash->spi_qspi.qspi.direct_config->instruction.read_bus_width = QSPI_CFG_BUS_QUAD;
        spiflash->spi_qspi.qspi.direct_config->address.read_bus_width = QSPI_CFG_BUS_QUAD;
        spiflash->spi_qspi.qspi.direct_config->read_data.bus_width = QSPI_CFG_BUS_QUAD;
        spiflash->spi_qspi.qspi.direct_config->read_data.opcode = ((union SFDP_JEDEC_PT_18H *)&u32_value[0])->Fast_Read_4_4_4_Opcode;
        spiflash->spi_qspi.qspi.direct_config->read_data.dummy_count = ((union SFDP_JEDEC_PT_18H *)&u32_value[0])->Fast_Read_Num_Wait_4_4_4;
        spiflash->spi_qspi.qspi.direct_config->mode_bit.value = ((union SFDP_JEDEC_PT_18H *)&u32_value[0])->Fast_Read_Num_Mode_Bits_4_4_4;
        spiflash->spi_qspi.qspi.direct_config->mode_bit.enabled = ((union SFDP_JEDEC_PT_18H *)&u32_value[0])->Fast_Read_Num_Mode_Bits_4_4_4 != 0;
    }
    else
    {
        if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x00, u32_value, 4) != CSI_OK)
        {
            return CSI_ERROR;
        }
#if(defined CSI_QSPI_FLASH_READ_144_DISABLE && CSI_QSPI_FLASH_READ_144_DISABLE == 1U)
        ((union SFDP_JEDEC_PT_00H *)&u32_value[0])->Fast_Read_1_4_4 = 0;
#endif
        if(((union SFDP_JEDEC_PT_00H *)&u32_value[0])->Fast_Read_1_4_4)
        {
            if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x08, u32_value, 4) != CSI_OK)
            {
                return CSI_ERROR;
            }
            spiflash->spi_qspi.qspi.direct_config->instruction.read_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->address.read_bus_width = QSPI_CFG_BUS_QUAD;
            spiflash->spi_qspi.qspi.direct_config->read_data.bus_width = QSPI_CFG_BUS_QUAD;
            spiflash->spi_qspi.qspi.direct_config->read_data.opcode = ((union SFDP_JEDEC_PT_08H *)&u32_value[0])->Fast_Read_1_4_4_Opcode;
            spiflash->spi_qspi.qspi.direct_config->read_data.dummy_count = ((union SFDP_JEDEC_PT_08H *)&u32_value[0])->Fast_Read_Num_Wait_1_4_4;
            spiflash->spi_qspi.qspi.direct_config->mode_bit.value = ((union SFDP_JEDEC_PT_08H *)&u32_value[0])->Fast_Read_Num_Mode_Bits_1_4_4;
            spiflash->spi_qspi.qspi.direct_config->mode_bit.enabled = ((union SFDP_JEDEC_PT_08H *)&u32_value[0])->Fast_Read_Num_Mode_Bits_1_4_4 != 0;
        }
        else if(((union SFDP_JEDEC_PT_00H *)&u32_value[0])->Fast_Read_1_1_4 != 0)
        {
            if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x08, u32_value, 4) != CSI_OK)
            {
                return CSI_ERROR;
            }
            spiflash->spi_qspi.qspi.direct_config->instruction.read_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->address.read_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->read_data.bus_width = QSPI_CFG_BUS_QUAD;
            spiflash->spi_qspi.qspi.direct_config->read_data.opcode = ((union SFDP_JEDEC_PT_08H *)&u32_value[0])->Fast_Read_1_1_4_Opcode;
            spiflash->spi_qspi.qspi.direct_config->read_data.dummy_count = ((union SFDP_JEDEC_PT_08H *)&u32_value[0])->Fast_Read_Num_Wait_1_1_4;
            spiflash->spi_qspi.qspi.direct_config->mode_bit.value = ((union SFDP_JEDEC_PT_08H *)&u32_value[0])->Fast_Read_Num_Mode_Bits_1_1_4;
            spiflash->spi_qspi.qspi.direct_config->mode_bit.enabled = ((union SFDP_JEDEC_PT_08H *)&u32_value[0])->Fast_Read_Num_Mode_Bits_1_1_4 != 0;
        }
        else if(((union SFDP_JEDEC_PT_00H *)&u32_value[0])->Fast_Read_1_2_2 != 0)
        {
            if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x0C, u32_value, 4) != CSI_OK)
            {
                return CSI_ERROR;
            }
            spiflash->spi_qspi.qspi.direct_config->instruction.read_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->address.read_bus_width = QSPI_CFG_BUS_DUAL;
            spiflash->spi_qspi.qspi.direct_config->read_data.bus_width = QSPI_CFG_BUS_DUAL;
            spiflash->spi_qspi.qspi.direct_config->read_data.opcode = ((union SFDP_JEDEC_PT_0CH *)&u32_value[0])->Fast_Read_1_2_2_Opcode;
            spiflash->spi_qspi.qspi.direct_config->read_data.dummy_count = ((union SFDP_JEDEC_PT_0CH *)&u32_value[0])->Fast_Read_Num_Wait_1_2_2;
            spiflash->spi_qspi.qspi.direct_config->mode_bit.value = ((union SFDP_JEDEC_PT_0CH *)&u32_value[0])->Fast_Read_Num_Mode_Bits_1_2_2;
            spiflash->spi_qspi.qspi.direct_config->mode_bit.enabled = ((union SFDP_JEDEC_PT_0CH *)&u32_value[0])->Fast_Read_Num_Mode_Bits_1_2_2 != 0;
        }
        else if(((union SFDP_JEDEC_PT_00H *)&u32_value[0])->Fast_Read_1_1_2 != 0)
        {
            if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x0C, u32_value, 4) != CSI_OK)
            {
                return CSI_ERROR;
            }
            spiflash->spi_qspi.qspi.direct_config->instruction.read_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->address.read_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->read_data.bus_width = QSPI_CFG_BUS_DUAL;
            spiflash->spi_qspi.qspi.direct_config->read_data.opcode = ((union SFDP_JEDEC_PT_0CH *)&u32_value[0])->Fast_Read_1_1_2_Opcode;
            spiflash->spi_qspi.qspi.direct_config->read_data.dummy_count = ((union SFDP_JEDEC_PT_0CH *)&u32_value[0])->Fast_Read_Num_Wait_1_1_2;
            spiflash->spi_qspi.qspi.direct_config->mode_bit.value = ((union SFDP_JEDEC_PT_0CH *)&u32_value[0])->Fast_Read_Num_Mode_Bits_1_1_2;
            spiflash->spi_qspi.qspi.direct_config->mode_bit.enabled = ((union SFDP_JEDEC_PT_0CH *)&u32_value[0])->Fast_Read_Num_Mode_Bits_1_1_2 != 0;
        }
        else//使用 1-1-1 模式  需要修改opcode
        {
            spiflash->spi_qspi.qspi.direct_config->instruction.read_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->address.read_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->read_data.bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->read_data.opcode = CSI_COM_OP_111_READ;
            spiflash->spi_qspi.qspi.direct_config->read_data.dummy_count = 0;
            spiflash->spi_qspi.qspi.direct_config->mode_bit.value = 0;
            spiflash->spi_qspi.qspi.direct_config->mode_bit.enabled = 0;
        }
    }
    
    // 13. Get program config
    if(flash_info->BYTE4_PTP_EN)
    {
        if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->BYTE4_PTP_base_addr, u32_value, 4) != CSI_OK)
        {
            return CSI_ERROR;
        }
        if(((union SFDP_BYTE4_PT_00H *)&u32_value[0])->Page_Program_cmd_1_4_4_Sup == 1)
        {
            spiflash->spi_qspi.qspi.direct_config->instruction.write_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->address.write_bus_width = QSPI_CFG_BUS_QUAD;
            spiflash->spi_qspi.qspi.direct_config->write_data.bus_width = QSPI_CFG_BUS_QUAD;
            spiflash->spi_qspi.qspi.direct_config->write_data.opcode = CSI_COM_OP_144_PAGE_PROGRAM;
            spiflash->spi_qspi.qspi.direct_config->write_data.dummy_count = CSI_SFDP_DUMMY_WRITE;
        }
        else if(((union SFDP_BYTE4_PT_00H *)&u32_value[0])->Page_Program_cmd_1_1_4_Sup == 1)
        {
            spiflash->spi_qspi.qspi.direct_config->instruction.write_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->address.write_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->write_data.bus_width = QSPI_CFG_BUS_QUAD;
            spiflash->spi_qspi.qspi.direct_config->write_data.opcode = CSI_COM_OP_114_PAGE_PROGRAM;
            spiflash->spi_qspi.qspi.direct_config->write_data.dummy_count = CSI_SFDP_DUMMY_WRITE;
        }
        else
        {
            spiflash->spi_qspi.qspi.direct_config->instruction.write_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->address.write_bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->write_data.bus_width = QSPI_CFG_BUS_SINGLE;
            spiflash->spi_qspi.qspi.direct_config->write_data.opcode = CSI_COM_OP_111_PAGE_PROGRAM;
            spiflash->spi_qspi.qspi.direct_config->write_data.dummy_count = CSI_SFDP_DUMMY_WRITE;
        }
    }
    else
    {
        spiflash->spi_qspi.qspi.direct_config->instruction.write_bus_width = QSPI_CFG_BUS_SINGLE;
        spiflash->spi_qspi.qspi.direct_config->address.write_bus_width = QSPI_CFG_BUS_SINGLE;
        spiflash->spi_qspi.qspi.direct_config->write_data.bus_width = QSPI_CFG_BUS_SINGLE;
        spiflash->spi_qspi.qspi.direct_config->write_data.opcode = CSI_COM_OP_111_PAGE_PROGRAM;
        spiflash->spi_qspi.qspi.direct_config->write_data.dummy_count = CSI_SFDP_DUMMY_WRITE;
    }
    spiflash->spi_qspi.qspi.direct_config->address.size = CSI_SFDP_ADDR_BITS;
    spiflash->spi_qspi.qspi.direct_config->flash.page_size = flash_info->page_size;
    spiflash->spi_qspi.qspi.direct_config->flash.block_size = flash_info->block_size;
    
    // 14. Get other opcodes
    spiflash->opcode.read_sr1 = CSI_COM_OP_READ_SR1;
    spiflash->opcode.read_sr2 = CSI_COM_OP_READ_SR2;
    spiflash->opcode.read_sr3 = CSI_COM_OP_READ_SR3;
    spiflash->opcode.write_sr1 = CSI_COM_OP_WRITE_SR1;
    spiflash->opcode.write_sr2 = CSI_COM_OP_WRITE_SR2;
    spiflash->opcode.write_sr3 = CSI_COM_OP_WRITE_SR3;
    if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x00, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    spiflash->opcode.write_enable = CSI_COM_OP_WRITE_ENABLE;
    spiflash->opcode.write_enable_vsr = (((union SFDP_JEDEC_PT_00H *)&u32_value[0])->Write_Enable_Opcode ? CSI_COM_OP_WRITE_ENABLE : CSI_COM_OP_WRITE_ENABLE_VSR);
    spiflash->opcode.erase_4kb = 0;
    if(((union SFDP_JEDEC_PT_00H *)&u32_value[0])->Block_Sector_Erase_Size == 0x1)
    {
        spiflash->opcode.erase_4kb = ((union SFDP_JEDEC_PT_00H *)&u32_value[0])->Erase_4KB_Opcode;
    }
    if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x1C, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    spiflash->opcode.erase_sector = ((union SFDP_JEDEC_PT_1CH *)&u32_value[0])->Sector_Type_1_erase_Opcode;
    spiflash->opcode.erase_half_block = ((union SFDP_JEDEC_PT_1CH *)&u32_value[0])->Sector_Type_2_erase_Opcode;
    if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x20, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    spiflash->opcode.erase_full_block = ((union SFDP_JEDEC_PT_20H *)&u32_value[0])->Sector_Type_3_erase_Opcode;
    spiflash->opcode.erase_chip = CSI_COM_OP_CHIP_ERASE1;
    if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x3C, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    if((((union SFDP_JEDEC_PT_3CH *)&u32_value[0])->Enter_4Byte_Addressing & 0x01) != 0 && flash_info->addr_4_byte)
    {
        spiflash->opcode.enter_4bits_addr = CSI_COM_OP_ENTER_4BADDR_MODE;
    }
    else
    {
        spiflash->spi_qspi.qspi.direct_config->address.size = QSPI_ADDRESS_24_BITS;
    }
    if(qspiflash_get_SFDP_by_addr(spiflash, flash_info->JEDEC_PTP_base_addr+0x38, u32_value, 4) != CSI_OK)
    {
        return CSI_ERROR;
    }
    if((((union SFDP_JEDEC_PT_38H *)&u32_value[0])->mode_4_4_4_disable_sequences & 0x01) != 0)
    {
        spiflash->opcode.Mode_444_Disable = 0xFF;
    }
    else if((((union SFDP_JEDEC_PT_38H *)&u32_value[0])->mode_4_4_4_disable_sequences & 0x02) != 0)
    {
        spiflash->opcode.Mode_444_Disable = 0xF5;
    }
    if((((union SFDP_JEDEC_PT_38H *)&u32_value[0])->mode_4_4_4_enable_sequences & 0x01) != 0)
    {
        spiflash->opcode.Mode_444_Enable = 0x38;
    }
    else if((((union SFDP_JEDEC_PT_38H *)&u32_value[0])->mode_4_4_4_enable_sequences & 0x02) != 0)
    {
        spiflash->opcode.Mode_444_Enable = 0x38;
    }
    else if((((union SFDP_JEDEC_PT_38H *)&u32_value[0])->mode_4_4_4_enable_sequences & 0x04) != 0)
    {
        spiflash->opcode.Mode_444_Enable = 0x35;
    }
//    spiflash->opcode.enter_qpi_mode = CSI_COM_OP_ENTER_QPI_MODE;
//    spiflash->opcode.exit_qpi_mode = CSI_COM_OP_EXIT_QPI_MODE;
    spiflash->opcode.enter_qpi_mode = 0;
    spiflash->opcode.exit_qpi_mode = CSI_COM_OP_EXIT_QPI_MODE;
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t SFDP_flash_qspi_config(csi_spiflash_t *spiflash)
{
    csi_error_t ret;
    if(csi_qspi_frequence(&spiflash->spi_qspi.qspi, CSI_SFDP_FREQ) != CSI_SFDP_FREQ)
    {
        return CSI_ERROR;
    }
    ret = csi_qspi_adapted_capture(&spiflash->spi_qspi.qspi);
    if(ret != CSI_OK)
    {
        return ret;
    }
    if(spiflash->opcode.enter_4bits_addr != 0)
    {
        if(qspiflash_send_opcode(spiflash, spiflash->opcode.enter_4bits_addr) != CSI_OK)
        {
            return CSI_ERROR;
        } 
    }
    if(qspiflash_send_opcode(spiflash, spiflash->opcode.write_enable) != CSI_OK)
    {
        return CSI_ERROR;
    }
    if(qspiflash_send_opcode(spiflash, spiflash->opcode.write_enable_vsr) != CSI_OK)
    {
        return CSI_ERROR;
    }
    uint32_t data = 0x2;            //Quad Enable
    if(csi_spiflash_write_reg(spiflash, spiflash->opcode.write_sr2, (uint8_t *)&data, 1) != CSI_OK)
    {
        return CSI_ERROR;
    }
    if(spiflash->opcode.Mode_444_Enable != 0)
    {
        if(qspiflash_send_opcode(spiflash, spiflash->opcode.Mode_444_Enable) != CSI_OK)
        {
            return CSI_ERROR;
        }
    }
    if(spiflash->opcode.enter_qpi_mode != 0)
    {
        if(qspiflash_send_opcode(spiflash, spiflash->opcode.enter_qpi_mode) != CSI_OK)
        {
            return CSI_ERROR;
        }
    }
    csi_qspi_write_protection_config(&spiflash->spi_qspi.qspi, 0x00000000, 0xFFFFFFFF, true);
    return CSI_OK;
}
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_get_flash_info(csi_spiflash_t *spiflash, csi_spiflash_info_t *flash_info)
{
    if(csi_qspi_frequence(&spiflash->spi_qspi.qspi, CSI_QSPI_FREQ_MIN) != CSI_QSPI_FREQ_MIN)
    {
        return CSI_ERROR;
    }
#if(defined CSI_QSPI_FLASH_DEBUG && CSI_QSPI_FLASH_DEBUG == 1U)
    if(qspiflash_send_opcode(spiflash, CSI_COM_OP_RESET_ENABLE) != CSI_OK)
    {
        return CSI_ERROR;
    }
    if(qspiflash_send_opcode(spiflash, CSI_COM_OP_RESET) != CSI_OK)
    {
        return CSI_ERROR;
    }
    // mdelay(10);
    for (int i = 0; i < 1000000; i++) {
        __ASM volatile("nop");
        __ASM volatile("nop");
        __ASM volatile("nop");
        __ASM volatile("nop");
        __ASM volatile("nop");
    }
    
//    if(qspiflash_send_opcode(spiflash, CSI_COM_OP_CLEAR_SR) != CSI_OK)
//    {
//        return CSI_ERROR;
//    }
#endif
#if(defined CSI_FLASH_SFDP_CMD_ENABLE && CSI_FLASH_SFDP_CMD_ENABLE == 1)
    if(SFDP_flash_read_parameter(spiflash, flash_info) != CSI_OK)
    {
        return CSI_ERROR;
    }
    if(qspiflash_read_sr(spiflash, flash_info) != CSI_OK)
    {
        return CSI_ERROR;
    }
    if(SFDP_flash_qspi_config(spiflash) != CSI_OK)
    {
        return CSI_ERROR;
    }
#else
    uint32_t device_id = 0;
    if(csi_spiflash_read_reg(spiflash, CSI_COM_OP_READ_DEVICE_ID, (uint8_t *)&device_id, 3) != CSI_OK)
    {
        return CSI_ERROR;
    }
    if((device_id & 0xFF) == CSI_GD25Q257D_ID)
    {
        flash_info->flash_name = (char *)"GD25Q257D";
        flash_info->flash_id = CSI_GD25Q257D_ID;
        flash_info->capacity = 32*1024*1024;//32M Bytes [0x0000 0000 ~ 0x01FF FFFF]
        flash_info->xip_addr = 0;
        flash_info->block_size = 64*1024;     //64K Bytes
        flash_info->sector_size = 4*1024;     //4K Bytes
        flash_info->page_size = 256;          //256 Bytes
        flash_info->protect_blk_size = 0;
        flash_info->protect_type = 0;
        if(nor_flash_GD25Q257D_qspi_config(spiflash) != CSI_OK)
        {
            return CSI_ERROR;
        }
    }
    else
    {
        return CSI_ERROR;
    }
#endif
    return qspiflash_set_write_protect(spiflash);
}
__CSI_FLASH_DRIVER_ATTRIBUTE static int32_t SFDP_flash_read(csi_spiflash_t *spiflash, uint32_t offset, void *data, uint32_t size)
{
    uint32_t remap_addr = 0;
    uint32_t offset_addr = 0;

    if(size > (CSI_AHB_MAP_ADDR_LENGTH/2))
    {
        return 0;
    }
    remap_addr  = offset & CSI_AHB_MAP_ADDR_MASK;
    offset_addr = offset & CSI_AHB_OFF_ADDR_MASK;
    if(csi_qspi_direct_receive(&spiflash->spi_qspi.qspi, spiflash->spi_qspi.qspi.direct_config, remap_addr, offset_addr, data, size) != CSI_OK)
    {
        return 0;
    }
    return size;
}
__CSI_FLASH_DRIVER_ATTRIBUTE static int32_t qspiflash_read(csi_spiflash_t *spiflash, uint32_t offset, void *data, uint32_t size)
{
#if(defined CSI_FLASH_SFDP_CMD_ENABLE && CSI_FLASH_SFDP_CMD_ENABLE == 1)
    return SFDP_flash_read(spiflash, offset, data, size);
#else
    if(CSI_IS_GD25Q257D_ID(spiflash->flash_prv_info.flash_id))
    {
        return nor_flash_GD25Q257D_read(spiflash, offset, data, size);
    }
#endif
    return 0;
}

__CSI_FLASH_DRIVER_ATTRIBUTE static int32_t SFDP_flash_program(csi_spiflash_t *spiflash, uint32_t offset, const void *data, uint32_t size)
{
    csi_error_t ret;
    uint32_t remap_addr = 0;
    uint32_t offset_addr = 0;
    
    if(size > (CSI_AHB_MAP_ADDR_LENGTH/2))
    {
        return 0;
    }
    remap_addr  = offset & CSI_AHB_MAP_ADDR_MASK;
    offset_addr = offset & CSI_AHB_OFF_ADDR_MASK;
    ret = csi_qspi_direct_send(&spiflash->spi_qspi.qspi, spiflash->spi_qspi.qspi.direct_config, remap_addr, offset_addr, data, size);
    if(ret != CSI_OK)
    {
        return 0;
    }
    do
    {
        if(csi_spiflash_read_reg(spiflash, spiflash->opcode.read_sr1, (void *)&spiflash->flash_prv_info.SR.SR1.u8_value, 1) != CSI_OK)
        {
            return CSI_ERROR;
        }
    }while(spiflash->flash_prv_info.SR.SR1.bits.BUSY);
    return size;
}
__CSI_FLASH_DRIVER_ATTRIBUTE static int32_t qspiflash_program(csi_spiflash_t *spiflash, uint32_t offset, const void *data, uint32_t size)
{
    int32_t ret_len = 0;
    csi_error_t ret;
    ret = qspiflash_clear_write_protect(spiflash);
    if(ret != CSI_OK)
    {
        return 0;
    }
#if(defined CSI_FLASH_SFDP_CMD_ENABLE && CSI_FLASH_SFDP_CMD_ENABLE == 1)
    ret_len = SFDP_flash_program(spiflash, offset, data, size);
#else
    if(CSI_IS_GD25Q257D_ID(spiflash->flash_prv_info.flash_id))
    {
        ret_len = nor_flash_GD25Q257D_program(spiflash, offset, data, size);
    }
#endif
    ret = qspiflash_set_write_protect(spiflash);
    csi_qspi_direct_mode_config(&spiflash->spi_qspi.qspi, spiflash->spi_qspi.qspi.direct_config, QSPI_READ);
    if(ret != CSI_OK)
    {
        return 0;
    }
    return ret_len;
}

__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t qspiflash_erase(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size, csi_spiflash_erase_type_t erase_type)
{
    csi_error_t ret;
    ret = qspiflash_clear_write_protect(spiflash);
    if(ret != CSI_OK)
    {
        return ret;
    }
#if(defined CSI_FLASH_SFDP_CMD_ENABLE && CSI_FLASH_SFDP_CMD_ENABLE == 1)
    ret = SFDP_flash_erase(spiflash, offset, size, erase_type);
#else
    switch(spiflash->flash_prv_info.flash_id)
    {
        case CSI_GD25Q257D_ID:
            ret = nor_flash_GD25Q257D_erase(spiflash, offset, size);
        break;
        default: ret = CSI_ERROR;
    }
#endif
    if(ret != CSI_OK)
    {
        qspiflash_set_write_protect(spiflash);
        csi_qspi_direct_mode_config(&spiflash->spi_qspi.qspi, spiflash->spi_qspi.qspi.direct_config, QSPI_READ);
        return ret;
    }
    else
    {
        ret = qspiflash_set_write_protect(spiflash);
    }
    csi_qspi_direct_mode_config(&spiflash->spi_qspi.qspi, spiflash->spi_qspi.qspi.direct_config, QSPI_READ);
    return ret;
}
__CSI_FLASH_DRIVER_ATTRIBUTE static csi_error_t SFDP_flash_erase(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size, csi_spiflash_erase_type_t erase_type)
{
    uint32_t erase_addr = 0;
    uint32_t erase_end_addr = offset + size;
    uint32_t erase_sectors = 0;
    uint32_t erase_size = 0;
    uint8_t erase_opcode = 0;
    
    switch(erase_type)
    {
        case SECTOR_ERASE:
            erase_size = spiflash->flash_prv_info.sector_size;
            erase_opcode = spiflash->opcode.erase_sector;
        break;
        case HALF_BLOCK_ERASE:
            erase_size = spiflash->flash_prv_info.block_size/2;
            erase_opcode = spiflash->opcode.erase_half_block;
            if(erase_opcode == 0)//当不支持half block erase时，使用block erase
            {
                erase_size = spiflash->flash_prv_info.block_size;
                erase_opcode = spiflash->opcode.erase_full_block;
            }
        break;
        case BLOCK_ERASE:
            erase_size = spiflash->flash_prv_info.block_size;
            erase_opcode = spiflash->opcode.erase_full_block;
        break;
        case CHIP_ERASE:
            erase_size = spiflash->flash_prv_info.capacity;
            erase_opcode = spiflash->opcode.erase_chip;
        break;
        default: return CSI_ERROR;
    }
    if(erase_opcode == 0 || erase_size == 0)
    {
        return CSI_ERROR;
    }
    if(erase_type != CHIP_ERASE)
    {
        int count = ((size%erase_size) > 0 ? (size/erase_size + 1) : size/erase_size);
        for(int i = 0; i < count; i++)
        {
            // addr=0xff  size=0x2000  erase_end_addr=0x20FF  erase 3 sectors
            erase_addr = offset & 0xFFFFF000;
            if(erase_addr > erase_end_addr)
            {
                break;
            }
            if(qspiflash_send_opcode(spiflash, spiflash->opcode.write_enable) != CSI_OK)//0x06;//write enable
            {
                return CSI_ERROR;
            }
            //opcode 0x20 for sector（4KB - 1024*4 Bytes） erase
            if(qspiflash_send_opcode_with_addr(spiflash, erase_opcode, erase_addr, 
                        (spiflash->opcode.enter_4bits_addr != 0 ? 4 : 3)) != CSI_OK)
            {
                return CSI_ERROR;
            }
            do
            {
                if(csi_spiflash_read_reg(spiflash, spiflash->opcode.read_sr1, (void *)&spiflash->flash_prv_info.SR.SR1.u8_value, 1) != CSI_OK)
                {
                    return CSI_ERROR;
                }
            }while(spiflash->flash_prv_info.SR.SR1.bits.BUSY);
            offset += erase_size;
            erase_sectors++;
        }
    }
    else if(erase_type == CHIP_ERASE)
    {
        if(qspiflash_send_opcode(spiflash, spiflash->opcode.write_enable) != CSI_OK)//write enable
        {
            return CSI_ERROR;
        }
        if(qspiflash_send_opcode(spiflash, erase_opcode) != CSI_OK)//opcode 0x20 chip erase
        {
            return CSI_ERROR;
        }
        do
        {
            if(csi_spiflash_read_reg(spiflash, spiflash->opcode.read_sr1, (void *)&spiflash->flash_prv_info.SR.SR1.u8_value, 1) != CSI_OK)
            {
                return CSI_ERROR;
            }
        }while(spiflash->flash_prv_info.SR.SR1.bits.BUSY);
    }
    return CSI_OK;
}

#if(!(defined CSI_FLASH_SFDP_CMD_ENABLE) || CSI_FLASH_SFDP_CMD_ENABLE == 0U)
static csi_error_t nor_flash_GD25Q257D_qspi_config(csi_spiflash_t *spiflash)
{
    csi_error_t ret;
    csi_qspi_command_t command;
    if(csi_qspi_frequence(&spiflash->spi_qspi.qspi, CSI_GD25Q257D_FREQ) != CSI_GD25Q257D_FREQ)
    {
        return CSI_ERROR;
    }
    ret = csi_qspi_adapted_capture(&spiflash->spi_qspi.qspi);
    if(ret != CSI_OK)
    {
        return ret;
    }
    spiflash->spi_qspi.qspi.direct_config->instruction.read_bus_width     = CSI_GD25Q257D_INDIRECT_INST_BUS_WIDTH;
    spiflash->spi_qspi.qspi.direct_config->instruction.write_bus_width    = CSI_GD25Q257D_INDIRECT_INST_BUS_WIDTH;
    spiflash->spi_qspi.qspi.direct_config->address.read_bus_width  		= CSI_GD25Q257D_INDIRECT_ADDR_READ_BUS_WIDTH;
    spiflash->spi_qspi.qspi.direct_config->address.write_bus_width 		= CSI_GD25Q257D_INDIRECT_ADDR_WRITE_BUS_WIDTH;
    spiflash->spi_qspi.qspi.direct_config->address.size            		= CSI_GD25Q257D_ADDR_BITS;
    spiflash->spi_qspi.qspi.direct_config->read_data.bus_width     		= CSI_GD25Q257D_INDIRECT_DATA_READ_BUS_WIDTH;
    spiflash->spi_qspi.qspi.direct_config->read_data.opcode        		= CSI_GD25Q257D_OPCODE_READ;
    spiflash->spi_qspi.qspi.direct_config->read_data.dummy_count   		= CSI_GD25Q257D_DUMMY_READ;
    spiflash->spi_qspi.qspi.direct_config->write_data.bus_width    		= CSI_GD25Q257D_INDIRECT_DATA_WRITE_BUS_WIDTH;
    spiflash->spi_qspi.qspi.direct_config->write_data.opcode       		= CSI_GD25Q257D_OPCODE_WRITE;
    spiflash->spi_qspi.qspi.direct_config->write_data.dummy_count  		= CSI_GD25Q257D_DUMMY_WRITE;
    spiflash->spi_qspi.qspi.direct_config->mode_bit.value          		= CSI_GD25Q257D_MODE_BIT;
    spiflash->spi_qspi.qspi.direct_config->mode_bit.enabled        		= CSI_GD25Q257D_MODE_BIT_EN;
    spiflash->spi_qspi.qspi.direct_config->flash.page_size         		= CSI_GD25Q257D_PAGE_SIZE;
    spiflash->spi_qspi.qspi.direct_config->flash.block_size        		= CSI_GD25Q257D_BLOCK_SIZE;

    /* Read device id operations */
    command.instruction.bus_width = QSPI_CFG_BUS_SINGLE;
    command.instruction.disabled  = false;
    command.alt.disabled          = true;
    command.address.disabled      = true;
    command.address.value         = 0;
    command.address.size          = 0;
    command.address.bus_width     = QSPI_CFG_BUS_SINGLE;
    command.data.bus_width        = QSPI_CFG_BUS_SINGLE;
    command.dummy_count           = 0;
    
    command.instruction.value     = CSI_COM_OP_ENTER_4BADDR_MODE;
    csi_qspi_send(&spiflash->spi_qspi.qspi, &command, NULL, 0, CSI_TIMEOUT_CLOCKS);
    command.instruction.value     = CSI_COM_OP_WRITE_ENABLE_VSR;
    csi_qspi_send(&spiflash->spi_qspi.qspi, &command, NULL, 0, CSI_TIMEOUT_CLOCKS);
    
    uint32_t data = 0x2;            //Quad Enable
    command.instruction.value     = CSI_COM_OP_WRITE_SR2;
    csi_qspi_send(&spiflash->spi_qspi.qspi, &command, &data, 1, CSI_TIMEOUT_CLOCKS);
    csi_qspi_write_protection_config(&spiflash->spi_qspi.qspi, 0x00000000, 0xFFFFFFFF, true);
    return CSI_OK;
}
static int32_t nor_flash_GD25Q257D_read(csi_spiflash_t *spiflash, uint32_t offset, void *data, uint32_t size)
{
    csi_error_t ret;
    uint32_t remap_addr = 0;
    uint32_t offset_addr = 0;

    if(size > (CSI_AHB_MAP_ADDR_LENGTH/2))
    {
        return 0;
    }
    remap_addr  = offset & CSI_AHB_MAP_ADDR_MASK;
    offset_addr = offset & CSI_AHB_OFF_ADDR_MASK;
    ret = csi_qspi_direct_receive(&spiflash->spi_qspi.qspi, spiflash->spi_qspi.qspi.direct_config, remap_addr, offset_addr, data, size);
    if(ret != CSI_OK)
    {
        return 0;
    }
    return size;
}
static int32_t nor_flash_GD25Q257D_program(csi_spiflash_t *spiflash, uint32_t offset, const void *data, uint32_t size)
{
    csi_error_t ret;
    uint32_t read_data = 0;
    uint32_t remap_addr = 0;
    uint32_t offset_addr = 0;

    if(size > (CSI_AHB_MAP_ADDR_LENGTH/2))
    {
        return 0;
    }
    remap_addr  = offset & CSI_AHB_MAP_ADDR_MASK;
    offset_addr = offset & CSI_AHB_OFF_ADDR_MASK;
    ret = csi_qspi_direct_send(&spiflash->spi_qspi.qspi, spiflash->spi_qspi.qspi.direct_config, remap_addr, offset_addr, data, size);
    if(ret != CSI_OK)
    {
        return 0;
    }
    do
    {
        if(csi_spiflash_read_reg(spiflash, CSI_COM_OP_READ_SR1, (void *)&read_data, 1) != CSI_OK)
        {
            return CSI_ERROR;
        }
    }while(read_data & 0x01);
    return size;
}
static csi_error_t nor_flash_GD25Q257D_erase(csi_spiflash_t *spiflash, uint32_t offset, uint32_t size)
{
    if(spiflash->flash_prv_info.sector_size == 0)
    {
        return CSI_ERROR;
    }
    
    uint32_t read_data = 0;
    uint32_t erase_addr = 0;
    uint32_t erase_end_addr = offset + size;
    uint32_t erase_sectors = 0;
    
    csi_qspi_command_t command;
    /* operations */
    command.instruction.bus_width = QSPI_CFG_BUS_SINGLE;
    command.instruction.disabled  = false;
    command.alt.disabled          = true;
    command.address.disabled      = true;
    command.address.value         = 0;
    command.address.size          = 0;
    command.address.bus_width     = QSPI_CFG_BUS_SINGLE;
    command.data.bus_width        = QSPI_CFG_BUS_SINGLE;
    command.dummy_count           = 0;
    
    for(int i = 0; i < (size/spiflash->flash_prv_info.sector_size + 1); i++)
    {
        // addr=0xff  size=0x2000  erase_end_addr=0x20FF  erase 3 sectors
        erase_addr = offset & 0xFFFFF000;
        if(erase_addr > erase_end_addr)
        {
            break;
        }
        command.instruction.value     = 0x06;//write enable
        if(csi_qspi_send(&spiflash->spi_qspi.qspi, &command, NULL, 0, CSI_TIMEOUT_CLOCKS) != 0)
        {
            return CSI_ERROR;
        }
        command.instruction.value     = 0x20;//opcode 0x20 for sector（4KB - 1024*4 Bytes） erase
        command.address.value         = erase_addr;
        command.address.size          = 4;
        if(csi_qspi_send(&spiflash->spi_qspi.qspi, &command, NULL, 0, CSI_TIMEOUT_CLOCKS) != 0)
        {
            return CSI_ERROR;
        }
        do
        {
            command.instruction.value     = CSI_COM_OP_READ_SR1;
            command.address.value         = 0;
            command.address.size          = 0;
            if(csi_qspi_receive(&spiflash->spi_qspi.qspi, &command, (void *)&read_data, 1, CSI_TIMEOUT_CLOCKS) != 1)
            {
                return CSI_ERROR;
            }
        }while(read_data & 0x01);
        offset += spiflash->flash_prv_info.sector_size;
        erase_sectors++;
    }
    return CSI_OK;
}
#endif /* !(defined CSI_FLASH_SFDP_CMD_ENABLE) || CSI_FLASH_SFDP_CMD_ENABLE == 0U */
#endif /* CONFIG_CSI_DRV_SPIFLASH_ENABLED */
