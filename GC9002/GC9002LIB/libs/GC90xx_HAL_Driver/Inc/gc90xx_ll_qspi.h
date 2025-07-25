/**
 * \file gc90xx_ll_qspi.h
 * \brief Driver for Cadence QSPI Flash Controller IP.
 *        There are two ways to communicate with the flash memory device:
 *         - issue AHB requests for direct read and writes in the Flash memory
 *           mapped address zone. The commands used for those can be configured
 *           by the driver
 *         - send a command to the device to access his internal registers and
 *           do other operations like erasing a sector
 *        At reset, the QSPI controller will work in a default mode which will
 *        allow to do basic commands. It should be configured with the
 *        flash memory device specifications for optimal use for commands and
 *        direct reads/writes. Here is an example of configuration:
 *         - send command to activate QSPI mode on the flash memory device
 *         - send command to change dummy cycles on the flash memory device
 *         - check if any operation is ungoing
 *         - disable the QSPI controller
 *         - change the baud rate divisor
 *         - activate the QSPI mode on the controller
 *         - change the dummy cycles number and opcode for reads/writes
 *         - change the number of bytes per page
 *         - change the number of address bytes
 *         - activate the QSPI controller
 *
 *        Warning: none of the functions declared here check if the dev
 *        argument points to NULL.
 */

#ifndef __GC90XX_LL_QSPI_H__

#define __GC90XX_LL_QSPI_H__

//#include <stdint.h>
//#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal_def.h>
#ifdef HAL_QSPI_MODULE_ENABLED

#if(defined BSP_QSPI_ATTRIBUTE_SECTION_RAM_EN)
#define __HAL_QSPI_DRIVER_ATTRIBUTE 		        HAL_ATTRIBUTE_SECTION_RAM
#else
#define __HAL_QSPI_DRIVER_ATTRIBUTE
#endif

#define QSPI_ARG_NOT_USED        0
#define QSPI_ARG_PTR_NOT_USED    NULL
/**
 * \brief Cadence QSPI error enumeration types
 */
enum qspi_error_t {
    QSPI_ERR_NONE,
    QSPI_ERR_WRONG_ARGUMENT,
    QSPI_ERR_CONTROLLER_NOT_DISABLED,
    QSPI_ERR_READ_IN_PROGRESS,
    QSPI_ERR_WRITE_IN_PROGRESS,
    QSPI_ERR_TIMEOUT,
    QSPI_ERR_ADAPTED_CAPTURE,
    /* Any new error should be added to the enumeration type error of
     * the corresponding Flash device library as well.
     */
};

/**
 * \brief Cadence QSPI SPI modes
 */
enum qspi_spi_mode_t {
    QSPI_SPI_MODE,
        /*!< Use 1 line for Instruction, Address and Data */
    QSPI_DSPI_MODE,
        /*!< Use 2 lines for Instruction, Address and Data */
    QSPI_QSPI_MODE,
        /*!< Use 4 lines for Instruction, Address and Data */
};

/**
 * \brief Cadence QSPI device configuration structure
 */
struct qspi_dev_cfg_t {
    uint32_t base; /*!< QSPI base address */
    /*
     * If not all the AHB wires are connected to the QSPI Flash Controller the
     * driver can still access all of the Flash memory. The bits of this value
     * should be put to 1 for every wire that is connected. Set it to
     * 0xFFFFFFFFU if all AHB address wires are connected to the
     * QSPI Flash Controller.
     */
    uint32_t remap_bus_base;
};

/**
 * \brief Cadence QSPI device structure
 */
struct qspi_dev_t
{
    struct qspi_dev_cfg_t* cfg; /*!< QSPI configuration */
    void(*QSPI_MspInitCallback)(unsigned int base_addr);
};

/**
 * \brief register callback.
 *
 * \param[in] dev QSPI device struct \ref qspi_dev_t
 * \param[in] id        0
 * \param[in] callback  
 */
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_callback_register(struct qspi_dev_t* dev, unsigned int id, void(*callback)(unsigned int base_addr));
/**
 * \brief Check if the controller is idle.
 *
 * \param[in] dev QSPI device struct \ref qspi_dev_t
 *
 * \return true if the controller is idle, false otherwise.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE bool qspi_is_idle(struct qspi_dev_t* dev);

/**
 * \brief Check if the controller is enabled.
 *
 * \param[in] dev QSPI device struct \ref qspi_dev_t
 *
 * \return true if the controller is enabled, false otherwise.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE bool qspi_is_enabled(struct qspi_dev_t* dev);

/**
 * \brief Disable the QSPI controller.
 *
 * \param[in] dev QSPI device struct \ref qspi_dev_t
 */
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_disable(struct qspi_dev_t* dev);

/**
 * \brief Enable the QSPI controller.
 *
 * \param[in] dev QSPI device struct \ref qspi_dev_t
 */
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_enable(struct qspi_dev_t* dev);
__HAL_QSPI_DRIVER_ATTRIBUTE void HAL_QSPI_MspInit(unsigned int base_addr);
/**
 * \brief Get the baud rate divisor.
 *
 * \param[in] dev QSPI device struct \ref qspi_dev_t
 *
 * \return Returns the baud rate divisor.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE uint32_t qspi_get_baud_rate_div(struct qspi_dev_t* dev);
/**
 * \brief Change the baud rate divisor.
 *
 * \param[in] dev QSPI device struct \ref qspi_dev_t
 * \param[in] div Baud rate divisor value. It can only be an even number
 *                    between 2 and 32 (both included).
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI frequency is calculated dividing the QSPI controller clock by
 *       this divisor. Please check Flash memory device specifications to know
 *       the maximal frequency that can be used.
 * \note The QSPI controller should be disabled before calling this function.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_set_baud_rate_div( struct qspi_dev_t* dev, uint32_t div);
/**
 * \brief Set SPI mode for instruction, address and data.
 *
 * \param[in] dev               QSPI device struct \ref qspi_dev_t
 * \param[in] mode_bit_enable   Mode Bit Enable, must be 0/1.
 * \param[in] mode_bit          Mode bits. These are the 8 bits that are sent to the
 *                              device following the address bytes
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 * \note Changing this setting will affect commands and direct operations.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_set_mode_bit( struct qspi_dev_t* dev,
                                     uint8_t mode_bit_enable,
                                     uint8_t mode_bit);
/**
 * \brief Set SPI mode for instruction, address and data.
 *
 * \param[in] dev               QSPI device struct \ref qspi_dev_t
 * \param[in] inst_type SPI mode to use for the instruction part of the command
 * \param[in] addr_type SPI mode to use for the address part of the command
 * \param[in] data_type SPI mode to use for the data part of the command
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 * \note Changing this setting will affect commands and direct operations.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_set_spi_mode( struct qspi_dev_t* dev,
                                    enum qspi_spi_mode_t inst_type,
                                    enum qspi_spi_mode_t addr_type,
                                    enum qspi_spi_mode_t data_type);

/**
 * \brief Set SPI mode for instruction, address and data.
 *
 * \param[in] dev               QSPI device struct \ref qspi_dev_t
 * \param[in] inst_type SPI mode to use for the instruction part of the command
 * \param[in] read_addr_type SPI mode to use for the address part of the read command
 * \param[in] read_data_type SPI mode to use for the data part of the read command
 * \param[in] write_addr_type SPI mode to use for the address part of the write command
 * \param[in] write_data_type SPI mode to use for the data part of the write command
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 * \note Changing this setting will affect commands and direct operations.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_set_spi_mode_base(struct qspi_dev_t* dev,
                                         enum qspi_spi_mode_t inst_type,
                                         enum qspi_spi_mode_t read_addr_type,
                                         enum qspi_spi_mode_t read_data_type,
                                         enum qspi_spi_mode_t write_addr_type,
                                         enum qspi_spi_mode_t write_data_type);
/**
 * \brief Configure read commands for direct reads.
 *
 * \param[in] dev          QSPI device struct \ref qspi_dev_t
 * \param[in] opcode       Read opcode that will be used for every direct read
 * \param[in] dummy_cycles Number of dummy cycles to wait before triggering the
 *                         command, this value must be between 0 and 31
 *                         (both included)
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_reads(struct qspi_dev_t* dev,
                                 uint8_t opcode,
                                 uint32_t dummy_cycles);

/**
 * \brief Configure write commands for direct writes.
 *
 * \param[in] dev          QSPI device struct \ref qspi_dev_t
 * \param[in] opcode       Write opcode that will be used for every direct write
 * \param[in] dummy_cycles Number of dummy cycles to wait before triggering the
 *                         command, this value must be between 0 and 31
 *                         (both included)
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_writes(   struct qspi_dev_t* dev,
                                     uint8_t opcode,
                                     uint32_t dummy_cycles);
/**
 * \brief switches off the automatic polling function.
 *
 * \param[in] dev          QSPI device struct \ref qspi_dev_t
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_polling_disable(struct qspi_dev_t* dev);
/**
 * \brief Configure polling for Write Completion.
 *
 * \param[in] dev                       QSPI device struct \ref qspi_dev_t
 * \param[in] polling_enable            polling is enable, must be 0/1.
 * \param[in] polling_polarity          Defines the polling polarity, must be 0/1.
 * \param[in] polling_bit_index         Defines the bit index that should be polled,
 *                                      must be 0~7.
 * \param[in] polling_repetition_delay  Defines additional delay for maintain Chip Select
 *                                      de-asserted during auto-polling phase,
 *                                      must be 0~255.
 * \param[in] polling_count             Defines the number of times the controller should
 *                                      expect to see a true result from the polling in
 *                                      successive reads of the device register,
 *                                      must be 0~255.
 * \param[in] opcode       Write opcode Defines the opcode that should be issued by the
 *                         controller when it is automatically polling for
 *                         device program completion.
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_polling(  struct qspi_dev_t* dev,
                                     uint8_t polling_enable,
                                     uint8_t polling_polarity,
                                     uint8_t polling_bit_index,
                                     uint8_t polling_repetition_delay,
                                     uint8_t polling_count,
                                     uint8_t opcode);
enum qspi_flash_size_t
{
    QSPI_FLASH_SIZE_512Mb = 0x0U,
    QSPI_FLASH_SIZE_1Gb,
    QSPI_FLASH_SIZE_2Gb,
    QSPI_FLASH_SIZE_4Gb,
};
/**
 * \brief Change the size of Flash Device connected to CS[0/1/2/3] pin.
 *
 * \param[in] dev             QSPI device struct \ref qspi_dev_t
 * \param[in] flash_cs0_size  size of Flash Device connected to CS[0] pin, must be between 0
 *                         and 3 (both included)
 * \param[in] flash_cs0_size  size of Flash Device connected to CS[1] pin, must be between 0
 *                         and 3 (both included)
 * \param[in] flash_cs0_size  size of Flash Device connected to CS[2] pin, must be between 0
 *                         and 3 (both included)
 * \param[in] flash_cs0_size  size of Flash Device connected to CS[3] pin, must be between 0
 *                         and 3 (both included)
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 * \note This function will affect direct reads/writes.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_flash_size(   struct qspi_dev_t* dev,
                                         enum qspi_flash_size_t flash_cs0_size,
                                         enum qspi_flash_size_t flash_cs1_size,
                                         enum qspi_flash_size_t flash_cs2_size,
                                         enum qspi_flash_size_t flash_cs3_size);
/**
 * \brief Change the number of bytes per device block.
 *
 * \param[in] dev          QSPI device struct \ref qspi_dev_t
 * \param[in] block_bytes  Number of bytes per device block, must be a power of 2 number
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 * \note This function will affect direct reads/writes.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_block_bytes(struct qspi_dev_t* dev, uint32_t block_bytes);
/**
 * \brief Change the number of bytes per device page.
 *
 * \param[in] dev          QSPI device struct \ref qspi_dev_t
 * \param[in] page_size    Number of bytes per device page, must be between 0
 *                         and 4095 (both included)
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 * \note This function will affect direct reads/writes.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_page_size(struct qspi_dev_t* dev, uint32_t page_size);

/**
 * \brief Change the number of device address bytes.
 *
 * \param[in] dev          QSPI device struct \ref qspi_dev_t
 * \param[in] bytes_number Number of device address bytes, must be between 1
 *                         and 16 (both included)
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note The QSPI controller should be idle before calling this function.
 * \note This function will affect direct reads/writes.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_cfg_addr_bytes(struct qspi_dev_t* dev, uint32_t bytes_number);

/**
 * \brief Remap the incoming AHB address with an offset for direct accesses.
 *
 * \param[in] dev    QSPI device struct \ref qspi_dev_t
 * \param[in] offset Offset that will be added to the incoming AHB address to
 *                   access the Flash memory
 *
 * \note This function will only affect direct reads/writes.
 * \note This function does not check if the resulting address is out of memory
 *       bounds.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_remap_addr(struct qspi_dev_t* dev, uint32_t offset);

/**
 * \brief Disable AHB address remapping for direct accesses.
 *
 * \param[in] dev    QSPI device struct \ref qspi_dev_t
 *
 * \note This function will disable the controller if it is not already
 *       disabled and enable it again (if it was).
 * \note This function will only affect direct reads/writes.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_disable_remap(struct qspi_dev_t* dev);

/**
 * \brief Write protection config
 *
 * \param[in] dev           QSPI device struct \ref qspi_dev_t
 * \param[in] lower_addr    The block number that defines the lower block in the range of blocks that is to be locked from writing
 * \param[in] upper_addr    The block number that defines the upper block in the range of blocks that is to be locked from writing.
 * \param[in] enable        \ref true/false
 */
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_write_protection_config(struct qspi_dev_t* dev, uint32_t lower_addr, uint32_t upper_addr, bool enable);
/**
 * \brief Write protection control
 *
 * \param[in] dev           QSPI device struct \ref qspi_dev_t
 * \param[in] enable        \ref true/false
 */
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_write_protection_ctrl(struct qspi_dev_t* dev, bool enable);
/**
 * \brief Disable AHB address remapping for direct accesses.
 *
 * \param[in] dev    QSPI device struct \ref qspi_dev_t
 * \param[in] delay_cycles_tx           Delay the transmitted data by the programmed
 *                                      number of ref_clk cycles
 * \param[in] sample_edge_sel           Sample edge selection (of the flash memory data outputs)
 * \param[in] delay_cycles_rx           Delay the read data capturing logic by the programmed number of ref_clk cycles
 * \param[in] loopback_clk_circuit_en   loopback clock circuit enable
 *
 * \note This function will disable the controller if it is not already
 *       disabled and enable it again (if it was).
 * \note This function will only affect direct reads/writes.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_cfg_read_capture( struct qspi_dev_t* dev,
                            uint8_t delay_cycles_tx,
                            uint8_t sample_edge_sel,
                            uint8_t delay_cycles_rx,
                            uint8_t loopback_clk_circuit_en);
/**
 * \brief set clock mode(Clock phase, Clock polarity outside SPI word)
 *
 * \param[in] dev           QSPI device struct \ref qspi_dev_t
 * \param[in] CPOL          \ref 1/0
 * \param[in] CPHA          \ref 1/0
 */
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_set_clock_mode(struct qspi_dev_t* dev, uint8_t CPOL, uint8_t CPHA);

/**
 * \brief Restore the default value of the QSPI controller registers.
 *
 * \param[in] dev    QSPI device struct \ref qspi_dev_t
 *
 * \note The QSPI controller should be disabled before calling this function.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_reset_regs(struct qspi_dev_t* dev);

/**
 * \brief Send a command to the flash memory device using the Software Triggered
 *        Instruction Generator (STIG).
 *
 * \param[in]  dev               QSPI device struct
 *                               \ref qspi_dev_t
 * \param[in]  opcode            Opcode for the command.
 * \param[out] read_data         Pointer to a memory zone where the read_len
 *                               bytes read will be written to. If no data is to
 *                               be read for the command,
 *                               this argument should be NULL.
 * \param[in]  read_len          Number of bytes to read for the command. If
 *                               no bytes are to be read, use 0 for argument
 *                               otherwise between 1 and 8 bytes (both
 *                               included) can be read.
 * \param[in]  write_data        Pointer to a memory zone where are
 *                               located the write_len bytes to write for
 *                               this command. If no bytes are to be written,
 *                               use NULL as argument.
 * \param[in]  write_len         Number of bytes to write for the command. If
 *                               no bytes are to be written, use 0 for
 *                               argument otherwise between 1 and 8 bytes
 *                               (both included) can be written.
 * \param[in]  addr              Address used for the command
 * \param[in]  addr_bytes_number Number of address bytes for this command.
 *                               If an address is not needed for the command,
 *                               use 0 for argument, otherwise between 1 and
 *                               4 bytes (both included) can be used.
 * \param[in]  dummy_cycles      Number of dummy cycles required for the
 *                               command, between 0 and 31 (both included).
 * 
 * \param[in]  timeout           wait for ticks of timeout).
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note Check the flash memory device specifications for the possible opcodes
 *       that can be used and the other informations needed for this function.
 * \note The SPI mode used for this command is the one set with the
 *       \ref qspi_activate_qspi_mode function or the default one.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_send_cmd(struct qspi_dev_t* dev,
                                uint8_t opcode,
                                void *read_data,
                                uint32_t read_len,
                                const void *write_data,
                                uint32_t write_len,
                                uint32_t addr,
                                uint32_t addr_bytes_number,
                                uint32_t dummy_cycles,
                                uint32_t timeout);

/**
 * \brief Send a simple command to the flash memory device using the Software
 *        Triggered Instruction Generator (STIG) with no data arguments.
 *        This command can be used for example to send the WRITE ENABLE command.
 *
 * \param[in]  dev    QSPI device struct \ref qspi_dev_t
 * \param[in]  opcode Opcode for the command.
 *
 * \note Check the flash memory device specifications for the possible opcodes
 *       that can be used and the other informations needed for this function.
 * \note The SPI mode used for this command is the one set with the
 *       \ref qspi_activate_qspi_mode function or the default one.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE void qspi_send_simple_cmd(struct qspi_dev_t* dev, uint8_t opcode);

/**
 * \brief Send a command to the flash memory device using the Software
 *        Triggered Instruction Generator (STIG). This command can be used to
 *        operat Flash.
 *
 * \param[in]  dev               QSPI device struct
 *                               \ref qspi_dev_t
 * \param[in]  opcode            Opcode for the command.
 * \param[in]  addr              Address used for the command
 * \param[in]  addr_bytes_number Number of address bytes for this command.
 *                               If an address is not needed for the command,
 *                               use 0 for argument, otherwise between 1 and
 *                               4 bytes (both included) can be used.
 * \param[in]  dummy_cycles      Number of dummy cycles required for the
 *                               command, between 0 and 31 (both included).
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note Check the flash memory device specifications for the possible opcodes
 *       that can be used and the other informations needed for this function.
 * \note The SPI mode used for this command is the one set with the
 *       \ref qspi_activate_qspi_mode function or the default one.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_send_addr_cmd(   struct qspi_dev_t* dev,
                                         uint8_t opcode,
                                         uint32_t addr,
                                         uint32_t addr_bytes_number,
                                         uint32_t dummy_cycles);

/**
 * \brief Send a read command to the flash memory device using the Software
 *        Triggered Instruction Generator (STIG). This command can be used to
 *        read Flash memory data or registers.
 *
 * \param[in]  dev               QSPI device struct
 *                               \ref qspi_dev_t
 * \param[in]  opcode            Opcode for the command.
 * \param[out] read_data         Pointer to a memory zone where the
 *                               read_len bytes read will be written to.
 * \param[in]  read_len          Number of bytes to read for the command.
 *                               Between 1 and 8 bytes (both included) can be
 *                               read.
 * \param[in]  addr              Address used for the command
 * \param[in]  addr_bytes_number Number of address bytes for this command.
 *                               If an address is not needed for the command,
 *                               use 0 for argument, otherwise between 1 and
 *                               4 bytes (both included) can be used.
 * \param[in]  dummy_cycles      Number of dummy cycles required for the
 *                               command, between 0 and 31 (both included).
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note Check the flash memory device specifications for the possible opcodes
 *       that can be used and the other informations needed for this function.
 * \note The SPI mode used for this command is the one set with the
 *       \ref qspi_activate_qspi_mode function or the default one.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_send_read_cmd(struct qspi_dev_t* dev,
                                     uint8_t opcode,
                                     void *read_data,
                                     uint32_t read_len,
                                     uint32_t addr,
                                     uint32_t addr_bytes_number,
                                     uint32_t dummy_cycles);

/**
 * \brief Send a write command to the flash memory device using the Software
 *        Triggered Instruction Generator (STIG). This command can be used to
 *        write Flash memory or registers.
 *
 * \param[in]  dev               QSPI device struct
 *                               \ref qspi_dev_t
 * \param[in]  opcode            Opcode for the command.
 * \param[in]  write_data        Pointer to a memory zone where are
 *                               located the write_len bytes to write for
 *                               this command.
 * \param[in]  write_len         Number of bytes to write for the command.
 *                               Between 1 and 8 bytes (both included) can be
 *                               written.
 * \param[in]  addr              Address used for the command
 * \param[in]  addr_bytes_number Number of address bytes for this command.
 *                               If an address is not needed for the command,
 *                               use 0 for argument, otherwise between 1 and
 *                               4 bytes (both included) can be used.
 * \param[in]  dummy_cycles      Number of dummy cycles required for the
 *                               command, between 0 and 31 (both included).
 *
 * \return Returns error code as specified in \ref qspi_error_t
 *
 * \note Check the flash memory device specifications for the possible opcodes
 *       that can be used and the other informations needed for this function.
 * \note The SPI mode used for this command is the one set with the
 *       \ref qspi_activate_qspi_mode function or the default one.
 */
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_send_write_cmd(   struct qspi_dev_t* dev,
                                         uint8_t opcode,
                                         const void *write_data,
                                         uint32_t write_len,
                                         uint32_t addr,
                                         uint32_t addr_bytes_number,
                                         uint32_t dummy_cycles);
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_direct_access_read(  struct qspi_dev_t* dev,
                                            uint32_t remap_addr,
                                            uint32_t offset_addr,
                                            void *read_data,
                                            uint32_t read_len);
__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_direct_access_write( struct qspi_dev_t* dev,
                                            uint32_t remap_addr,
                                            uint32_t offset_addr,
                                            const void *write_data,
                                            uint32_t write_len);

__HAL_QSPI_DRIVER_ATTRIBUTE enum qspi_error_t qspi_adapted_capture(struct qspi_dev_t* dev);
#endif /* HAL_QSPI_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif

#endif /* __GC90XX_LL_QSPI_H__ */
