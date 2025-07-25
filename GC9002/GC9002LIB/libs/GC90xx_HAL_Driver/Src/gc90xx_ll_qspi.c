/*
 * Copyright (c) 2018-2019 Arm Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <gc90xx_hal.h>

//#include <stdint.h>
//#include <stdlib.h>
//#include <stdbool.h>
/* Use memcpy */
//#include <string.h>

#ifdef HAL_QSPI_MODULE_ENABLED

/** Setter bit manipulation macro */
#define SET_BIT_POS(WORD, BIT_INDEX) ((WORD) |= (1U << (BIT_INDEX)))
/** Clearing bit manipulation macro */
#define CLR_BIT_POS(WORD, BIT_INDEX) ((WORD) &= ~(1U << (BIT_INDEX)))
/** Getter bit manipulation macro */
#define GET_BIT_POS(WORD, BIT_INDEX) (((WORD) & (1U << (BIT_INDEX))) != 0)

#ifdef WORD_ALIGN_4B_MASK
#undef WORD_ALIGN_4B_MASK
#endif
#define WORD_ALIGN_4B_MASK    0x3U /* Mask the first 2 bits */

#ifdef IS_ADDR_ALIGNED
#undef IS_ADDR_ALIGNED
#endif
#define IS_ADDR_ALIGNED(ADDR) (((uint32_t)(ADDR) & (WORD_ALIGN_4B_MASK)) == 0U)

#define BITS_PER_BYTE               8U
#define BITS_PER_WORD               32U

#define CFG_READS  true
#define CFG_WRITES false

#define DATA_REG_NUMBER 2U
#define DATA_REG_LOWER  0U
#define DATA_REG_UPPER  1U

#define ERROR_VALUE 0xFFFFFFFFU

/**
 * \brief QSPI IP6514E register map structure
 */
struct _qspi_reg_map_t
{
    volatile uint32_t CR;           //0x00
    volatile uint32_t DRIR;         //0x04
    volatile uint32_t DWICR;        //0x08
    volatile uint32_t DDR;          //0x0C
    volatile uint32_t RDCAPR;       //0x10
    volatile uint32_t DSCR;         //0x14
    volatile uint32_t SRAMPSR;      //0x18
    volatile uint32_t IAHBATR;      //0x1C
    volatile uint32_t DMAPCR;       //0x20
    volatile uint32_t RAR;          //0x24
    volatile uint32_t MCR;          //0x28
    volatile uint32_t SRAMFLR;      //0x2C
    volatile uint32_t TXTR;         //0x30
    volatile uint32_t RXTR;         //0x34
    volatile uint32_t WCCR;         //0x38
    volatile uint32_t PER;          //0x3C
    volatile uint32_t ISR;          //0x40
    volatile uint32_t IMR;          //0x44
    volatile uint32_t Reserved0;    //0x48
    volatile uint32_t Reserved1;    //0x4C
    volatile uint32_t LWPR;         //0x50
    volatile uint32_t UWPR;         //0x54
    volatile uint32_t WPR;          //0x58
    volatile uint32_t Reserved2;    //0x5C
    volatile uint32_t IRTCR;        //0x60
    volatile uint32_t IRTWR;        //0x64
    volatile uint32_t IRTSAR;       //0x68
    volatile uint32_t IRTNR;        //0x6C
    volatile uint32_t IWTCR;        //0x70
    volatile uint32_t IWTWR;        //0x74
    volatile uint32_t IWTSAR;       //0x78
    volatile uint32_t IWTNR;        //0x7C
    volatile uint32_t ITARR;        //0x80
    volatile uint32_t Reserved3;    //0x84
    volatile uint32_t Reserved4;    //0x88
    volatile uint32_t Reserved5;    //0x8C
    volatile uint32_t FCCR;         //0x90
    volatile uint32_t FCAR;         //0x94
    volatile uint32_t Reserved6;    //0x98
    volatile uint32_t Reserved7;    //0x9C
    volatile uint32_t FCRLDR;       //0xA0
    volatile uint32_t FCRUDR;       //0xA4
    volatile uint32_t FCWLDR;       //0xA8
    volatile uint32_t FCWUDR;       //0xAC
    volatile uint32_t PFSR;         //0xB0
    volatile uint32_t Reserved8[14];//0xB4~0xF8
    volatile uint32_t MIDR;         //0xFC
};

/** QSPI Configuration register description (offset 0x00) */
#define QSPI_CFG_ENABLE_POS            0U
#define QSPI_CFG_ENABLE_ADDR_REMAP_POS 16U
#define QSPI_CFG_BAUD_DIV_POS          19U
    #define QSPI_CFG_BAUD_DIV_MIN          2U
    #define QSPI_CFG_BAUD_DIV_MAX          32U
    #define QSPI_CFG_BAUD_DIV_BITS         4U
#define QSPI_CFG_IDLE_POS              31U

/**
 * Device Read/Write Instruction registers description (offset 0x04 and 0x08).
 * These values are the same for the Device Read Instruction register at offset
 * 0x04 and the Device Write Instruction register at offset 0x08.
 */
#define DEVICE_READ_WRITE_INST_OPCODE_POS        0U
#define DEVICE_READ_INST_INST_TYPE_POS           8U /* Only applies to the Read
                                                     * register. */
#define DEVICE_READ_WRITE_INST_ADDR_TYPE_POS     12U
#define DEVICE_READ_WRITE_INST_DATA_TYPE_POS     16U
    #define DEVICE_READ_WRITE_INST_MODE_QSPI         2U
    #define DEVICE_READ_WRITE_INST_MODE_DSPI         1U
    #define DEVICE_READ_WRITE_INST_MODE_SPI          0U
    #define DEVICE_READ_WRITE_INST_MODE_BITS         2U
#define DEVICE_READ_WRITE_INST_DUMMY_CYCLES_POS  24U
    #define DEVICE_READ_WRITE_INST_DUMMY_CYCLES_BITS 5U
    #define DEVICE_READ_WRITE_INST_DUMMY_CYCLES_MAX  31U

/** Device Size Configuration register description (offset 0x14) */
#define DEVICE_SIZE_ADDR_BYTES_POS  0U
    #define DEVICE_SIZE_ADDR_BYTES_MIN  1U
    #define DEVICE_SIZE_ADDR_BYTES_MAX  16U
    #define DEVICE_SIZE_ADDR_BYTES_BITS 4U
#define DEVICE_SIZE_PAGE_BYTES_POS  4U
    #define DEVICE_SIZE_PAGE_BYTES_MAX  4095U
    #define DEVICE_SIZE_PAGE_BYTES_BITS 12U
#define DEVICE_SIZE_BLOCK_BYTES_POS  16U
    #define DEVICE_SIZE_BLOCK_BYTES_MAX  31U
    #define DEVICE_SIZE_BLOCK_BYTES_BITS 5U
#define DEVICE_SIZE_FLASH_CS0_POS  21U
    #define DEVICE_SIZE_FLASH_CS0_MAX  3U
    #define DEVICE_SIZE_FLASH_CS0_BITS 2U
#define DEVICE_SIZE_FLASH_CS1_POS  23U
    #define DEVICE_SIZE_FLASH_CS1_MAX  3U
    #define DEVICE_SIZE_FLASH_CS1_BITS 2U
#define DEVICE_SIZE_FLASH_CS2_POS  25U
    #define DEVICE_SIZE_FLASH_CS2_MAX  3U
    #define DEVICE_SIZE_FLASH_CS2_BITS 2U
#define DEVICE_SIZE_FLASH_CS3_POS  27U
    #define DEVICE_SIZE_FLASH_CS3_MAX  3U
    #define DEVICE_SIZE_FLASH_CS3_BITS 2U

/** Flash Command Control register description (offset 0x90) */
#define FLASH_CMD_CTRL_EXECUTE_POS        0U
#define FLASH_CMD_CTRL_BUSY_POS           1U
#define FLASH_CMD_CTRL_DUMMY_CYCLES_POS   7U
    #define FLASH_CMD_CTRL_DUMMY_CYCLES_MAX   31U
    #define FLASH_CMD_CTRL_DUMMY_CYCLES_BITS  5U
#define FLASH_CMD_CTRL_WRITE_BYTES_POS    12U
    #define FLASH_CMD_CTRL_WRITE_BYTES_MAX    8U
    #define FLASH_CMD_CTRL_WRITE_BYTES_BITS   3U
#define FLASH_CMD_CTRL_WRITE_ENABLE_POS   15U
#define FLASH_CMD_CTRL_ADDR_BYTES_POS     16U
    #define FLASH_CMD_CTRL_ADDR_BYTES_MAX     4U
    #define FLASH_CMD_CTRL_ADDR_BYTES_BITS    2U
#define FLASH_CMD_CTRL_ADDR_ENABLE_POS    19U
#define FLASH_CMD_CTRL_READ_BYTES_POS     20U
    #define FLASH_CMD_CTRL_READ_BYTES_MAX     8U
    #define FLASH_CMD_CTRL_READ_BYTES_BITS    3U
#define FLASH_CMD_CTRL_READ_ENABLE_POS    23U
#define FLASH_CMD_CTRL_OPCODE_POS         24U

/** Default register values of the QSPI Flash controller */
#define QSPI_CFG_REG_RESET_VALUE              (0x80080080U)
#define DEVICE_READ_INSTR_REG_RESET_VALUE     (0x080220EBU)
#define DEVICE_WRITE_INSTR_REG_RESET_VALUE    (0x00000002U)
#define DEVICE_SIZE_CFG_REG_RESET_VALUE       (0x00101002U)
#define REMAP_ADDR_REG_RESET_VALUE            (0x00000000U)
#define FLASH_CMD_CONTROL_REG_RESET_VALUE     (0x00000000U)
#define FLASH_CMD_ADDRESS_REG_RESET_VALUE     (0x00000000U)
#define FLASH_CMD_WRITE_DATA_REG_RESET_VALUE  (0x00000000U)

__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_memcpy(void *destin, const void *source, size_t size)
{
    for(uint32_t i = 0; i < size; i++)
    {
        *(((uint8_t *)destin) + i) = *(((uint8_t *)source) + i);
    }
}

/**
 * \brief Change specific bits in a 32 bits word.
 *
 * \param[in,out] word         Pointer of the word to change
 * \param[in]     bits         bits_length bits to put at bits_pos in the word
 *                             pointed
 * \param[in]     bits_length  Number of bits to change
 * \param[in]     bits_pos     Position of the bits to change
 *
 * \note This function will do nothing if the parameters given are incorret:
 *         * word is NULL
 *         * bits_length + bits_pos > 32
 *         * bits_length is 0
 */
__HAL_QSPI_DRIVER_ATTRIBUTE static void change_bits_in_word(volatile uint32_t *word,
                                uint32_t bits,
                                uint32_t bits_length,
                                uint32_t bits_pos)
{
    uint32_t mask;
    if ((word == NULL) ||
        ((bits_length + bits_pos) > BITS_PER_WORD) ||
        (bits_length == 0U)) {
        /* Silently fail */
        return;
    }
    /* Change all the bits */
    if (bits_length == BITS_PER_WORD) {
        *word = bits;
        return;
    }
    mask = ((1U << bits_length) - 1);
    /*
     * We change the bits in three steps:
     *   - clear bits_length bits with zeroes at bits_pos in the word
     *   - mask bits in case it contains more than bits_length bits
     *   - set the new bits in the cleared word
     * Because the data pointed by word is only read once, the data will still
     * be coherent after an interruption that changes it.
     */
    *word = ((*word & ~(mask << bits_pos)) | ((bits & mask) << bits_pos));
}

/**
 * \brief Configure reads or writes commands for direct operations.
 *
 * \param[in] dev             QSPI IP6514E device struct \ref qspi_dev_t
 * \param[in] opcode          Read/write opcode that will be used for every
 *                            direct read/write
 * \param[in] dummy_cycles    Number of dummy cycles to wait before triggering
 *                            the command, this value must be between 0 and 31
 *                            (both included)
 * \param[in] is_reads_cfg    true to configure direct reads, false to configure
 *                            direct writes
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE static enum qspi_error_t qspi_cfg_reads_writes(  struct qspi_dev_t* dev,
                                                 uint8_t opcode,
                                                 uint32_t dummy_cycles,
                                                 bool is_reads_cfg)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    /*
     * Select the good register address if we want to configure reads or writes.
     */
    volatile uint32_t *device_read_write_inst_reg = is_reads_cfg ? &(reg_map->DRIR) : &(reg_map->DWICR);
    uint32_t device_read_write_inst_reg_copy = *device_read_write_inst_reg;
    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_is_idle(dev));
    if (dummy_cycles > DEVICE_READ_WRITE_INST_DUMMY_CYCLES_MAX) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    change_bits_in_word(&device_read_write_inst_reg_copy,
                        (uint32_t)opcode,
                        BITS_PER_BYTE,
                        DEVICE_READ_WRITE_INST_OPCODE_POS);
    change_bits_in_word(&device_read_write_inst_reg_copy,
                        dummy_cycles,
                        DEVICE_READ_WRITE_INST_DUMMY_CYCLES_BITS,
                        DEVICE_READ_WRITE_INST_DUMMY_CYCLES_POS);

    *device_read_write_inst_reg = device_read_write_inst_reg_copy;
    return QSPI_ERR_NONE;
}

/**
 * \brief Given the public SPI mode enumeration, returns the private value it
 *        maps to in the register field.
 *
 * \param[in] spi_mode Read/write opcode that will be used for every direct
 *            read/write
 *
 * \return Return the correct DEVICE_READ_WRITE_INST_MODE value.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE static uint32_t spi_mode_field_value(enum qspi_spi_mode_t spi_mode)
{
    switch (spi_mode) {
        case QSPI_SPI_MODE:
            return DEVICE_READ_WRITE_INST_MODE_SPI;
        case QSPI_DSPI_MODE:
            return DEVICE_READ_WRITE_INST_MODE_DSPI;
        case QSPI_QSPI_MODE:
            return DEVICE_READ_WRITE_INST_MODE_QSPI;
        default:
            return ERROR_VALUE;
    }
}

__HAL_QSPI_DRIVER_ATTRIBUTE bool qspi_is_idle(struct qspi_dev_t* dev)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    return GET_BIT_POS(reg_map->CR, QSPI_CFG_IDLE_POS);
}

__HAL_QSPI_DRIVER_ATTRIBUTE bool qspi_is_enabled(struct qspi_dev_t* dev)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    return GET_BIT_POS(reg_map->CR, QSPI_CFG_ENABLE_POS);
}

__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_disable(struct qspi_dev_t* dev)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    CLR_BIT_POS(reg_map->CR, QSPI_CFG_ENABLE_POS);
}
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_callback_register(struct qspi_dev_t* dev, unsigned int id, void(*callback)(unsigned int base_addr))
{
    if(id == 0)
    {
        dev->QSPI_MspInitCallback = callback;
    }
}
__HAL_QSPI_DRIVER_ATTRIBUTE __WEAK void HAL_QSPI_MspInit(unsigned int base_addr)
{
    (void)base_addr;
}
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_enable(struct qspi_dev_t* dev)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    if(dev->QSPI_MspInitCallback != NULL)
    {
        dev->QSPI_MspInitCallback(dev->cfg->base);
    }
    else
    {
        HAL_QSPI_MspInit(dev->cfg->base);
    }
    SET_BIT_POS(reg_map->CR, QSPI_CFG_ENABLE_POS);
}
__HAL_QSPI_DRIVER_ATTRIBUTE uint32_t qspi_get_baud_rate_div(struct qspi_dev_t* dev)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    return ((((reg_map->CR >> QSPI_CFG_BAUD_DIV_POS) & 0xF) + 1) * 2);
}
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_set_baud_rate_div(struct qspi_dev_t* dev, uint32_t div)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_is_idle(dev));
    /* div should be an even number. */
    if (((div & 1U) == 1) ||
        (div < QSPI_CFG_BAUD_DIV_MIN) ||
        (div > QSPI_CFG_BAUD_DIV_MAX)) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    /*
     * The div value (between 2 and 32) needs to be stored in the register on a
     * 4 bits field.
     */
    bool is_enabled = qspi_is_enabled(dev);
    if (is_enabled)
    {
        qspi_disable(dev);
    }
    change_bits_in_word(&(reg_map->CR),
                        (div / 2) - 1,
                        QSPI_CFG_BAUD_DIV_BITS,
                        QSPI_CFG_BAUD_DIV_POS);
    if (is_enabled)
    {
        qspi_enable(dev);
    }
    return QSPI_ERR_NONE;
}

__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_set_mode_bit(     struct qspi_dev_t* dev,
                                         uint8_t mode_bit_enable,
                                         uint8_t mode_bit)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_is_idle(dev));
    if (mode_bit_enable > 1) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    change_bits_in_word(&(reg_map->DRIR), mode_bit_enable, 1, 20);
    change_bits_in_word(&(reg_map->MCR), mode_bit, 8, 0);
    return QSPI_ERR_NONE;
}
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_set_spi_mode(     struct qspi_dev_t* dev,
                                         enum qspi_spi_mode_t inst_type,
                                         enum qspi_spi_mode_t addr_type,
                                         enum qspi_spi_mode_t data_type)
{
    return qspi_set_spi_mode_base(dev, inst_type, addr_type, data_type, addr_type, data_type);
}
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_set_spi_mode_base(struct qspi_dev_t* dev,
                                         enum qspi_spi_mode_t inst_type,
                                         enum qspi_spi_mode_t read_addr_type,
                                         enum qspi_spi_mode_t read_data_type,
                                         enum qspi_spi_mode_t write_addr_type,
                                         enum qspi_spi_mode_t write_data_type)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    uint32_t inst_spi_mode, read_addr_spi_mode, read_data_spi_mode, write_addr_spi_mode, write_data_spi_mode;
    /*
     * A local copy of the Device Read Instruction and Device Write Instruction
     * registers is used to limit APB accesses.
     */
    uint32_t device_read_inst_cpy = reg_map->DRIR;
    uint32_t device_write_inst_cpy = reg_map->DWICR;
    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_is_idle(dev));
    /*
     * First check that the instruction mode is not SPI. If that is the case,
     * the address and data mode register fields become DO NOT CARE.
     */
    inst_spi_mode = spi_mode_field_value(inst_type);
    if (inst_spi_mode == ERROR_VALUE) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    if (inst_type == QSPI_SPI_MODE) {
        change_bits_in_word(&(reg_map->DRIR),
                            inst_spi_mode,
                            DEVICE_READ_WRITE_INST_MODE_BITS,
                            DEVICE_READ_INST_INST_TYPE_POS);
//        return QSPI_ERR_NONE;
    }
    /* Now check and set address and data modes. */
    read_addr_spi_mode = spi_mode_field_value(read_addr_type);
    read_data_spi_mode = spi_mode_field_value(read_data_type);
    write_addr_spi_mode = spi_mode_field_value(write_addr_type);
    write_data_spi_mode = spi_mode_field_value(write_data_type);
    if ((read_addr_spi_mode == ERROR_VALUE) || 
        (read_data_spi_mode == ERROR_VALUE) || 
        (write_addr_spi_mode == ERROR_VALUE) || 
        (write_data_spi_mode == ERROR_VALUE)) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    /* Change the Device Read Instruction register. */
    change_bits_in_word(&device_read_inst_cpy,
                        inst_spi_mode,
                        DEVICE_READ_WRITE_INST_MODE_BITS,
                        DEVICE_READ_INST_INST_TYPE_POS);
    change_bits_in_word(&device_read_inst_cpy,
                        read_addr_spi_mode,
                        DEVICE_READ_WRITE_INST_MODE_BITS,
                        DEVICE_READ_WRITE_INST_ADDR_TYPE_POS);
    change_bits_in_word(&device_read_inst_cpy,
                        read_data_spi_mode,
                        DEVICE_READ_WRITE_INST_MODE_BITS,
                        DEVICE_READ_WRITE_INST_DATA_TYPE_POS);

    /* Change the Device Write Instruction register. */
    change_bits_in_word(&device_write_inst_cpy,
                        write_addr_spi_mode,
                        DEVICE_READ_WRITE_INST_MODE_BITS,
                        DEVICE_READ_WRITE_INST_ADDR_TYPE_POS);
    change_bits_in_word(&device_write_inst_cpy,
                        write_data_spi_mode,
                        DEVICE_READ_WRITE_INST_MODE_BITS,
                        DEVICE_READ_WRITE_INST_DATA_TYPE_POS);
    /* Save the changes. */
    reg_map->DRIR = device_read_inst_cpy;
    reg_map->DWICR = device_write_inst_cpy;
    return QSPI_ERR_NONE;
}

__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_reads(struct qspi_dev_t* dev, uint8_t opcode, uint32_t dummy_cycles)
{
    return qspi_cfg_reads_writes(dev, opcode, dummy_cycles, CFG_READS);
}

__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_writes(struct qspi_dev_t* dev, uint8_t opcode, uint32_t dummy_cycles)
{
    return qspi_cfg_reads_writes(dev, opcode, dummy_cycles, CFG_WRITES);
}
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_polling_disable(struct qspi_dev_t* dev)
{
    return qspi_cfg_polling(dev, 0, 0, 0, 0, 0, 0);
}
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_polling(  struct qspi_dev_t* dev,
                                     uint8_t polling_enable,
                                     uint8_t polling_polarity,
                                     uint8_t polling_bit_index,
                                     uint8_t polling_repetition_delay,
                                     uint8_t polling_count,
                                     uint8_t opcode)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_is_idle(dev));
    if (polling_enable > 1 ||
        polling_polarity > 1 ||
        polling_bit_index > 7) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    if(polling_enable)
    {
        change_bits_in_word(&(reg_map->WCCR), 0, 1, 14);
        change_bits_in_word(&(reg_map->WCCR), polling_polarity, 1, 13);
        change_bits_in_word(&(reg_map->WCCR), polling_bit_index, 3, 8);
        change_bits_in_word(&(reg_map->WCCR), polling_repetition_delay, 8, 24);
        change_bits_in_word(&(reg_map->WCCR), polling_count, 8, 16);
        change_bits_in_word(&(reg_map->WCCR), opcode, 8, 0);
    }
    else
    {
        change_bits_in_word(&(reg_map->WCCR), 1, 1, 14);
    }
    return QSPI_ERR_NONE;
}
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_flash_size(   struct qspi_dev_t* dev,
                                         enum qspi_flash_size_t flash_cs0_size,
                                         enum qspi_flash_size_t flash_cs1_size,
                                         enum qspi_flash_size_t flash_cs2_size,
                                         enum qspi_flash_size_t flash_cs3_size)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_is_idle(dev));
    change_bits_in_word(&(reg_map->DSCR),
                        flash_cs0_size,
                        DEVICE_SIZE_FLASH_CS0_BITS,
                        DEVICE_SIZE_FLASH_CS0_POS);
    change_bits_in_word(&(reg_map->DSCR),
                        flash_cs1_size,
                        DEVICE_SIZE_FLASH_CS1_BITS,
                        DEVICE_SIZE_FLASH_CS1_POS);
    change_bits_in_word(&(reg_map->DSCR),
                        flash_cs2_size,
                        DEVICE_SIZE_FLASH_CS2_BITS,
                        DEVICE_SIZE_FLASH_CS2_POS);
    change_bits_in_word(&(reg_map->DSCR),
                        flash_cs3_size,
                        DEVICE_SIZE_FLASH_CS3_BITS,
                        DEVICE_SIZE_FLASH_CS3_POS);
    return QSPI_ERR_NONE;
}
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_block_bytes(struct qspi_dev_t* dev, uint32_t block_bytes)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_is_idle(dev));
    if (block_bytes > DEVICE_SIZE_BLOCK_BYTES_MAX) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    change_bits_in_word(&(reg_map->DSCR),
                        block_bytes,
                        DEVICE_SIZE_BLOCK_BYTES_BITS,
                        DEVICE_SIZE_BLOCK_BYTES_POS);
    return QSPI_ERR_NONE;
}
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_page_size(struct qspi_dev_t* dev, uint32_t page_size)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_is_idle(dev));
    if (page_size > DEVICE_SIZE_PAGE_BYTES_MAX) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    change_bits_in_word(&(reg_map->DSCR),
                        page_size,
                        DEVICE_SIZE_PAGE_BYTES_BITS,
                        DEVICE_SIZE_PAGE_BYTES_POS);
    return QSPI_ERR_NONE;
}

__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_addr_bytes(struct qspi_dev_t* dev, uint32_t bytes_number)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    /*
     * Wait for the Serial Interface and QSPI pipeline to be IDLE when
     * all low level synchronization has been done.
     */
    while(!qspi_is_idle(dev));
    if (bytes_number < DEVICE_SIZE_ADDR_BYTES_MIN ||
        bytes_number > DEVICE_SIZE_ADDR_BYTES_MAX) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    change_bits_in_word(&(reg_map->DSCR),
                        bytes_number - 1,
                        DEVICE_SIZE_ADDR_BYTES_BITS,
                        DEVICE_SIZE_ADDR_BYTES_POS);
    return QSPI_ERR_NONE;
}

__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_remap_addr(struct qspi_dev_t* dev, uint32_t offset)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    /* Save the enable state to restore it after. */
    bool is_enabled = qspi_is_enabled(dev);
    if (is_enabled)
    {
        qspi_disable(dev);
    }
    reg_map->RAR = offset;
    SET_BIT_POS(reg_map->CR, QSPI_CFG_ENABLE_ADDR_REMAP_POS);
    if (is_enabled)
    {
        qspi_enable(dev);
    }
}

__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_disable_remap(struct qspi_dev_t* dev)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    /* Save the enable state to restore it after. */
    bool is_enabled = qspi_is_enabled(dev);
    if (is_enabled)
    {
        qspi_disable(dev);
    }
    CLR_BIT_POS(reg_map->CR, QSPI_CFG_ENABLE_ADDR_REMAP_POS);
    if (is_enabled)
    {
        qspi_enable(dev);
    }
}
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_cfg_read_capture( struct qspi_dev_t* dev,
                            uint8_t delay_cycles_tx,
                            uint8_t sample_edge_sel,
                            uint8_t delay_cycles_rx,
                            uint8_t loopback_clk_circuit_en)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    uint32_t tempreg = 0;
    if(loopback_clk_circuit_en == 0)
    {
        reg_map->RDCAPR = 1;
    }
    else
    {
        change_bits_in_word(&tempreg, delay_cycles_tx, 4, 16);
        change_bits_in_word(&tempreg, sample_edge_sel, 1, 5);
        change_bits_in_word(&tempreg, delay_cycles_rx, 4, 1);
        tempreg |= 1;
        reg_map->RDCAPR = tempreg;
    }
}
/**
 * \brief set clock mode(Clock phase, Clock polarity outside SPI word)
 *
 * \param[in] dev           QSPI device struct \ref qspi_dev_t
 * \param[in] CPOL          \ref 1/0
 * \param[in] CPHA          \ref 1/0
 */
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_set_clock_mode(struct qspi_dev_t* dev, uint8_t CPOL, uint8_t CPHA)
{
    if(CPOL > 1 || CPHA > 1)
    {
        return;
    }
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    reg_map->CR &= (0x3 << 1);
    reg_map->CR |= (CPOL << 1);
    reg_map->CR |= (CPHA << 2);
}
#define WRITE_PROTECTION_ENABLE         0x2
#define WRITE_PROTECTION_INVERSION      0x1
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_write_protection_config(struct qspi_dev_t* dev, uint32_t lower_addr, uint32_t upper_addr, bool enable)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    reg_map->LWPR = lower_addr;
    reg_map->UWPR = upper_addr;
    if(enable) {
        reg_map->WPR |= WRITE_PROTECTION_ENABLE;
    }
    else {
        reg_map->WPR &= ~WRITE_PROTECTION_ENABLE;
    }
}
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_write_protection_ctrl(struct qspi_dev_t* dev, bool enable)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    if(enable) {
        reg_map->WPR |= WRITE_PROTECTION_ENABLE;
    }
    else {
        reg_map->WPR &= ~WRITE_PROTECTION_ENABLE;
    }
}

__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_reset_regs(struct qspi_dev_t* dev)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;

    /* Restore the default value of the QSPI Configuration register. */
    reg_map->CR = QSPI_CFG_REG_RESET_VALUE;

    /* Restore the default value of the Device R/W Instruction registers. */
    reg_map->DRIR = DEVICE_READ_INSTR_REG_RESET_VALUE;
    reg_map->DWICR = DEVICE_WRITE_INSTR_REG_RESET_VALUE;

    /* Restore the default value of the Device Size Configuration register. */
    reg_map->DSCR = DEVICE_SIZE_CFG_REG_RESET_VALUE;

    /* Restore the default value of the Remap Address register. */
    reg_map->RAR = REMAP_ADDR_REG_RESET_VALUE;

    /* Restore the default value of the Flash Command Control register. */
    reg_map->FCCR = FLASH_CMD_CONTROL_REG_RESET_VALUE;
    /* Restore the default value of the Flash Command Address register. */
    reg_map->FCAR = FLASH_CMD_ADDRESS_REG_RESET_VALUE;

    /* Restore the default value of the Flash Command Write Data registers. */
    reg_map->FCWLDR = FLASH_CMD_WRITE_DATA_REG_RESET_VALUE;
    reg_map->FCWUDR = FLASH_CMD_WRITE_DATA_REG_RESET_VALUE;
    /*
     * This function does not affect the Flash Command Read Data registers
     * which are completely Read-Only.
     */
}

__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_send_cmd(struct qspi_dev_t* dev,
                                uint8_t opcode,
                                void *read_data,
                                uint32_t read_len,
                                const void *write_data,
                                uint32_t write_len,
                                uint32_t addr,
                                uint32_t addr_bytes_number,
                                uint32_t dummy_cycles,
                                uint32_t timeout)
{
    struct _qspi_reg_map_t *reg_map = (struct _qspi_reg_map_t *)dev->cfg->base;
    /* To limit APB accesses, we set this reg up locally before */
    uint32_t flash_cmd_ctrl = 0U;
    bool read_requested = ((read_data != NULL) && (read_len != 0));
    bool write_requested = ((write_data != NULL) && (write_len != 0));
    bool addr_requested = (addr_bytes_number != 0);
    uint64_t tickstart = 0U;
    
    /*
     * To prevent unaligned and byte or halfbyte accesses to the APB registers,
     * a word aligned buffer is used to temporary transfer the data before doing
     * word accesses on these registers from that buffer.
     */
    uint32_t data_regs[DATA_REG_NUMBER] = {0};
    if (read_len > FLASH_CMD_CTRL_READ_BYTES_MAX) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    if (write_len > FLASH_CMD_CTRL_WRITE_BYTES_MAX) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    if (addr_bytes_number > FLASH_CMD_CTRL_ADDR_BYTES_MAX) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    if (dummy_cycles > FLASH_CMD_CTRL_DUMMY_CYCLES_MAX) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    if (read_requested && write_requested) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    change_bits_in_word(&flash_cmd_ctrl,
                        (uint32_t)opcode,
                        BITS_PER_BYTE,
                        FLASH_CMD_CTRL_OPCODE_POS);
    /* Enable read if requested */
    if (read_requested) {
        SET_BIT_POS(flash_cmd_ctrl, FLASH_CMD_CTRL_READ_ENABLE_POS);
        change_bits_in_word(&flash_cmd_ctrl,
                            read_len - 1,
                            FLASH_CMD_CTRL_READ_BYTES_BITS,
                            FLASH_CMD_CTRL_READ_BYTES_POS);
    }
    /* Enable write if requested */
    if (write_requested) {
        SET_BIT_POS(flash_cmd_ctrl, FLASH_CMD_CTRL_WRITE_ENABLE_POS);
        change_bits_in_word(&flash_cmd_ctrl,
                            write_len - 1,
                            FLASH_CMD_CTRL_WRITE_BYTES_BITS,
                            FLASH_CMD_CTRL_WRITE_BYTES_POS);
        if (IS_ADDR_ALIGNED(write_data) && IS_ADDR_ALIGNED(write_len)) {
            /*
             * Optimised case when write_data is word aligned and write_len is
             * 4 or 8.
             */
            reg_map->FCWLDR = *(uint32_t *)write_data;
            if (write_len == FLASH_CMD_CTRL_WRITE_BYTES_MAX) {
                reg_map->FCWUDR = *((uint32_t *)write_data + 1);
            }
        } else {
            /*
             * data_regs is used as a buffer to only do unaligned access on the
             * AHB bus and word aligned accesses to the APB registers.
             */
            qspi_memcpy((void *)data_regs, write_data, write_len);
            /*
             * Only write_len bytes will be written even if both data registers
             * are written.
             */
            reg_map->FCWLDR = data_regs[DATA_REG_LOWER];
            reg_map->FCWUDR = data_regs[DATA_REG_UPPER];
        }
    }

    /* Enable the address if requested */
    if (addr_requested) {
        SET_BIT_POS(flash_cmd_ctrl, FLASH_CMD_CTRL_ADDR_ENABLE_POS);
        reg_map->FCAR = addr;
        change_bits_in_word(&flash_cmd_ctrl,
                            addr_bytes_number - 1,
                            FLASH_CMD_CTRL_ADDR_BYTES_BITS,
                            FLASH_CMD_CTRL_ADDR_BYTES_POS);
    }

    /* Put dummy cycles number */
    change_bits_in_word(&flash_cmd_ctrl,
                        dummy_cycles,
                        FLASH_CMD_CTRL_DUMMY_CYCLES_BITS,
                        FLASH_CMD_CTRL_DUMMY_CYCLES_POS);

    /* Copy the Flash Command Control register and execute the command */
    reg_map->FCCR = flash_cmd_ctrl;
    SET_BIT_POS(reg_map->FCCR, FLASH_CMD_CTRL_EXECUTE_POS);
    
    /* Init tickstart for timeout managment */
    tickstart = 0;
    /* Wait for termination */
    while (GET_BIT_POS(reg_map->FCCR, FLASH_CMD_CTRL_BUSY_POS))
    {
        tickstart++;
        /* Check for the Timeout */
        if (timeout != HAL_MAX_DELAY)
        {
            if ((timeout == 0U) || (tickstart > timeout))
            {
                return QSPI_ERR_TIMEOUT;
            }
        }
    }
    /*
     * Recolt the read data if it was requested. read_len validity has already
     * been verified at this point.
     */
    if (read_requested) {
        if (IS_ADDR_ALIGNED(read_data) && IS_ADDR_ALIGNED(read_len)) {
            /*
             * Optimised case when read_data is word aligned and read_len is
             * 4 or 8.
             */
            *(uint32_t *)read_data = reg_map->FCRLDR;
            if (read_len == FLASH_CMD_CTRL_READ_BYTES_MAX) {
                *((uint32_t *)read_data + 1) = reg_map->FCRUDR;
            }
        } else {
            /*
             * Only read_len bytes have been written even if both data registers
             * are written.
             */
            data_regs[DATA_REG_LOWER] = reg_map->FCRLDR;
            data_regs[DATA_REG_UPPER] = reg_map->FCRUDR;
            /*
             * data_regs is used as a buffer to only do unaligned access on the
             * AHB bus and word aligned accesses to the APB registers.
             */
            qspi_memcpy(read_data, (void *)data_regs, read_len);
        }
    }
    return QSPI_ERR_NONE;
}

__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_send_simple_cmd(struct qspi_dev_t* dev, uint8_t opcode)
{
    /*
     * No read/write data, no address, no dummy cycles.
     * Given the arguments, this function can not fail.
     */
    (void)qspi_send_cmd(dev,
                        opcode,
                        QSPI_ARG_PTR_NOT_USED,
                        QSPI_ARG_NOT_USED,
                        QSPI_ARG_PTR_NOT_USED,
                        QSPI_ARG_NOT_USED,
                        QSPI_ARG_NOT_USED,
                        QSPI_ARG_NOT_USED,
                        0,
                        HAL_MAX_DELAY);
}
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_send_addr_cmd(   struct qspi_dev_t* dev,
                                         uint8_t opcode,
                                         uint32_t addr,
                                         uint32_t addr_bytes_number,
                                         uint32_t dummy_cycles)
{
    /* Write arguments are expected */
    if (addr_bytes_number == QSPI_ARG_NOT_USED) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    /*
     * No read/write data, no dummy cycles.
     * Given the arguments, this function can not fail.
     */
    return qspi_send_cmd(dev,
                         opcode,
                         QSPI_ARG_PTR_NOT_USED,
                         QSPI_ARG_NOT_USED,
                         QSPI_ARG_PTR_NOT_USED,
                         QSPI_ARG_NOT_USED,
                         addr,
                         addr_bytes_number,
                         dummy_cycles,
                         HAL_MAX_DELAY);
}

__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_send_read_cmd(struct qspi_dev_t* dev,
                                     uint8_t opcode,
                                     void *read_data,
                                     uint32_t read_len,
                                     uint32_t addr,
                                     uint32_t addr_bytes_number,
                                     uint32_t dummy_cycles)
{
    /* Read arguments are expected */
    if (read_data == QSPI_ARG_PTR_NOT_USED || read_len == QSPI_ARG_NOT_USED) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    /* No write data */
    return qspi_send_cmd(dev,
                         opcode,
                         read_data,
                         read_len,
                         QSPI_ARG_PTR_NOT_USED,
                         QSPI_ARG_NOT_USED,
                         addr,
                         addr_bytes_number,
                         dummy_cycles,
                         HAL_MAX_DELAY);
}

__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_send_write_cmd(   struct qspi_dev_t* dev,
                                         uint8_t opcode,
                                         const void *write_data,
                                         uint32_t write_len,
                                         uint32_t addr,
                                         uint32_t addr_bytes_number,
                                         uint32_t dummy_cycles)
{
    /* Write arguments are expected */
    if (write_data == QSPI_ARG_PTR_NOT_USED || write_len == QSPI_ARG_NOT_USED) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
    /* No read data, no dummy cycles */
    return qspi_send_cmd(dev,
                         opcode,
                         QSPI_ARG_PTR_NOT_USED,
                         QSPI_ARG_NOT_USED,
                         write_data,
                         write_len,
                         addr,
                         addr_bytes_number,
                         dummy_cycles,
                         HAL_MAX_DELAY);
}
/**
 * \brief Read in Direct Access Mode.
 *
 * \param[in]  dev                      QSPI device struct
 *                                      \ref qspi_dev_t
 * \param[in]  remap_addr               a different address used by the FLASH device
 *                                      The lower 20 bits are invalid(& 0xFFF00000)
 * \param[in]  offset_addr              a offset address used by the FLASH device
 *                                      The lower 20 bits are valid(& 0x000FFFFF)
 * \param[in]  read_data                Pointer to a memory zone where are
 *                                      located the write_len bytes to read for
 *                                      this command.
 * \param[in]  read_len                 Number of bytes to read for the command.
 *                                      Between 1 and 8 bytes (both included) can be
 *                                      readed.
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note Check the flash memory device specifications for the possible opcodes
 *       that can be used and the other informations needed for this function.
 * \note The SPI mode used for this command is the one set with the
 *       \ref qspi_activate_qspi_mode function or the default one.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_direct_access_read(  struct qspi_dev_t* dev,
                                            uint32_t remap_addr,
                                            uint32_t offset_addr,
                                            void *read_data,
                                            uint32_t read_len)
{
    uint32_t index = 0;
    uint32_t remainder = 0;
    /* Write arguments are expected */
    if (read_data == QSPI_ARG_PTR_NOT_USED || read_len == QSPI_ARG_NOT_USED) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }    
//    remap_addr += (offset_addr % 4);
//    offset_addr -= (offset_addr % 4);
    index = read_len / 4;
    remainder = read_len % 4;
    qspi_remap_addr(dev, remap_addr);
    if(IS_ADDR_ALIGNED_4B(read_data))//4 bytes align
    {
        for(int i = 0; i < index; i++)
        {
            *(((uint32_t *)read_data)+i) = *(uint32_t *)(dev->cfg->remap_bus_base + offset_addr);
            offset_addr += 4;
        }
        qspi_memcpy((void *)(((uint32_t *)read_data)+index), (const void *)(dev->cfg->remap_bus_base + offset_addr), remainder);
    }
    else
    {
        uint32_t map_read_data = ((uint32_t)read_data & 0xFFFFFFFC) + 4;
//        uint32_t off = 4 - ((uint32_t)read_data % 4);
        uint32_t temp_data[2];
        for(int i = 0; i < index-1; i++)
        {
            *(((uint32_t *)map_read_data)+i) = *(uint32_t *)(dev->cfg->remap_bus_base + offset_addr);
            offset_addr += 4;
        }
        temp_data[0] = *(uint32_t *)(dev->cfg->remap_bus_base + offset_addr);
        offset_addr += 4;
        temp_data[1] = *(uint32_t *)(dev->cfg->remap_bus_base + offset_addr);
        qspi_memcpy(read_data, (const void *)map_read_data, (index-1)*4);
        qspi_memcpy((void *)((uint32_t)read_data+(index-1)*4), (const void *)temp_data, read_len-(index-1)*4);
    }
    return QSPI_ERR_NONE;
}
/**
 * \brief Write in Direct Access Mode.
 *
 * \param[in]  dev                      QSPI device struct
 *                                      \ref qspi_dev_t
 * \param[in]  remap_addr               a different address used by the FLASH device
 *                                      The lower 20 bits are invalid(& 0xFFF00000)
 * \param[in]  offset_addr              a offset address used by the FLASH device
 *                                      The lower 20 bits are valid(& 0x000FFFFF)
 * \param[in]  write_data               Pointer to a memory zone where are
 *                                      located the write_len bytes to write for
 *                                      this command.
 * \param[in]  write_len                Number of bytes to write for the command.
 *                                      Between 1 and 8 bytes (both included) can be
 *                                      written.
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note Check the flash memory device specifications for the possible opcodes
 *       that can be used and the other informations needed for this function.
 * \note The SPI mode used for this command is the one set with the
 *       \ref qspi_activate_qspi_mode function or the default one.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_direct_access_write( struct qspi_dev_t* dev,
                                            uint32_t remap_addr,
                                            uint32_t offset_addr,
                                            const void *write_data,
                                            uint32_t write_len)
{
    uint32_t index = 0;
    uint32_t remainder = 0;
    /* Write arguments are expected */
    if (write_data == QSPI_ARG_PTR_NOT_USED || write_len == QSPI_ARG_NOT_USED) {
        return QSPI_ERR_WRONG_ARGUMENT;
    }
#if(defined USE_HAL_QSPI_WRITE_LOW_DIV && \
            USE_HAL_QSPI_WRITE_LOW_DIV >= QSPI_CFG_BAUD_DIV_MIN && \
            USE_HAL_QSPI_WRITE_LOW_DIV <= QSPI_CFG_BAUD_DIV_MAX)
    uint32_t baud_rate_div = qspi_get_baud_rate_div(dev);
    qspi_set_baud_rate_div(dev, USE_HAL_QSPI_WRITE_LOW_DIV);
#endif
//    remap_addr += (offset_addr % 4);
//    offset_addr -= (offset_addr % 4);
    index = write_len / 4;
    remainder = write_len % 4;
    qspi_remap_addr(dev, remap_addr);
    if(IS_ADDR_ALIGNED_4B(write_data))//4 bytes alignment
    {
        for(int i = 0; i < index; i++)
        {
            *(volatile uint32_t *)(dev->cfg->remap_bus_base + offset_addr) = *(((uint32_t *)write_data)+i);
            offset_addr += 4;
        }
    }
    else
    {
        for(int i = 0; i < index; i++)
        {
            *(volatile uint32_t *)(dev->cfg->remap_bus_base + offset_addr) = ReadUint32_LittleEndian(((uint8_t *)write_data)+i*4);
            offset_addr += 4;
        }
    }
    qspi_memcpy((void *)(dev->cfg->remap_bus_base + offset_addr), (const void *)((uint32_t)write_data+index*4), remainder);
#if(defined USE_HAL_QSPI_WRITE_LOW_DIV && \
            USE_HAL_QSPI_WRITE_LOW_DIV >= QSPI_CFG_BAUD_DIV_MIN && \
            USE_HAL_QSPI_WRITE_LOW_DIV <= QSPI_CFG_BAUD_DIV_MAX)
    qspi_set_baud_rate_div(dev, baud_rate_div);
#endif
    return QSPI_ERR_NONE;
}

/**
  * @brief  Calibration Flash Read Delay
  * @retval 0: success   -1: failed
  */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_adapted_capture(struct qspi_dev_t* dev)
{
#ifndef OPCODE_READ_DEVICE_ID
#define OPCODE_READ_DEVICE_ID            0x9F
#endif

#define QSPI_CAP_EDGE_RISING        1
#define QSPI_CAP_EDGE_FALLING       0
    uint32_t testID;
    uint32_t FlashID = 0;
	uint32_t num = 0;
    uint8_t pass[32];

    uint32_t baud_rate_div = qspi_get_baud_rate_div(dev);
    qspi_set_baud_rate_div(dev, 32);
    qspi_cfg_read_capture(dev, 0, 0, 0, 0);
    qspi_send_read_cmd(dev, OPCODE_READ_DEVICE_ID, (void *)&FlashID, 4, 0, 0, 0);
    qspi_set_baud_rate_div(dev, baud_rate_div);
    for(int i = 0; i <= 0xF; i++)
    {
        // rising edge
        qspi_cfg_read_capture(dev, 0, QSPI_CAP_EDGE_RISING, i, 1);
        qspi_send_read_cmd(dev, OPCODE_READ_DEVICE_ID, (void *)&testID, 4, 0, 0, 0);
        if(testID == FlashID)
        {
			num++;
			pass[i*2] = 1;
        }
        else
        {
			pass[i*2] = 0;
        }
        // falling edge
        qspi_cfg_read_capture(dev, 0, QSPI_CAP_EDGE_FALLING, i, 1);
        qspi_send_read_cmd(dev, OPCODE_READ_DEVICE_ID, (void *)&testID, 4, 0, 0, 0);
        if(testID == FlashID)
        {
			num++;
			pass[i*2+1] = 1;
        }
        else
        {
			pass[i*2+1] = 0;
        }
    }

	int temp = 0;
	for(int i = 0; i < sizeof(pass); i++)
	{
		if(pass[i] == 1)
		{
            // ++temp --> 1/2/3/4 select 2
            // temp++ --> 1/2/3/4 select 3
			if(++temp == (num/2))
			{
                qspi_cfg_read_capture(dev, 0, (i%2) == 0 ? QSPI_CAP_EDGE_RISING : QSPI_CAP_EDGE_FALLING, i/2, 1);
				return QSPI_ERR_NONE;
			}
		}
	}
    return QSPI_ERR_ADAPTED_CAPTURE;
}
#endif /* HAL_QSPI_MODULE_ENABLED */
