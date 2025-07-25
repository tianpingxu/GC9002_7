//#include "CPU1_CMSDK_CM0.h"
#include "gc90x_ahmi.h"

typedef struct
{
    //__IO uint32_t NA0;
    __IO uint32_t REG00;
    __IO uint32_t REG04;
    __IO uint32_t REG08;
    __IO uint32_t REG0C;
    __IO uint32_t REG10;
    __IO uint32_t REG14;
    __IO uint32_t REG18;
    __IO uint32_t REG1C;
    __IO uint32_t REG20;
    __IO uint32_t REG24;
    __IO uint32_t REG28;
    __IO uint32_t REG2C;
    __IO uint32_t REG30;
    __IO uint32_t REG34;
    __IO uint32_t REG38;
    __IO uint32_t REG3C;
    __IO uint32_t REG40;
    __IO uint32_t REG44;
    __IO uint32_t REG48;
    __IO uint32_t REG4C;
    __IO uint32_t REG50;
    __IO uint32_t REG54;
    __IO uint32_t REG58;
    __IO uint32_t REG5C;
    __IO uint32_t REG60;
    __IO uint32_t REG64;
    __IO uint32_t REG68;
    __IO uint32_t REG6C;
    __IO uint32_t REG70;
    __IO uint32_t REG74;
    __IO uint32_t REG78;
    __IO uint32_t REG7C;
    __IO uint32_t REG80;
    __IO uint32_t REG84;
    __IO uint32_t REG88;
    __IO uint32_t REG8C;
    __IO uint32_t REG90;
    __IO uint32_t REG94;
    __IO uint32_t REG98;
    __IO uint32_t REG9C;
    __IO uint32_t REGA0;
    __IO uint32_t REGA4;
    __IO uint32_t REGA8;
    __IO uint32_t REGAC;
    __IO uint32_t REGB0;
    __IO uint32_t REGB4;
    __IO uint32_t REGB8;
    __IO uint32_t REGBC;
    __IO uint32_t REGC0;
    __IO uint32_t REGC4;
	__IO uint32_t PageInfo;
	__IO uint32_t BaseAddr;
    __IO uint32_t FlashRdDelay;
	__IO uint32_t DramCacheFlush;
	__IO uint32_t Gzip_EN;
	__IO uint32_t RequestNUM;
	__IO uint32_t LastRequestAddr;
	__IO uint32_t SingleWay_EN;
}DRAM_CACHE_Type;

#define DRAM_CACHE ((DRAM_CACHE_Type*) 0x40100000)


//#define DRAM_CACHE_ROM_BASE 0x50002080
//#define DRAM_CACHE_BUFFER_ADDR 0x42000000
#define NAND_FLASH_START_ADDR 0xA0000000
#define UI_SIZE_Mb	1024
#define UI_SIZE_Mb_Nor	256

#define COLUMN_ADDR_LEN 11

#define  PAGE_SIZE_1KB      0
#define  PAGE_SIZE_2KB      1
#define  PAGE_SIZE_4KB      2
#define  PAGE_SIZE_8KB      3
#define  USING_NOR_FLASH    (1<<2)
#define  USING_NAND_FLASH   (0<<2)


#define QSPI_NAND_OK 0
#define QSPI_NAND_ERROR 1

#define QSPI_NOR_OK 0
#define QSPI_NOR_ERROR 1

#define QSPI_NAND_BUFFER_LEN 2048
#define QSPI2_NOR_BUFFER_LEN 2048

enum FlashType
{
	NOR=0,
	WB_NAND,
	GD_NAND_Q4,
	GD_NAND_Q5,
	DS_NAND,
	NOR256,
	S25FL127S
};

enum FlashState
{
	OK=0,
	InvalidFlashType,
	DelayCalibError,
	NoACFinFlash,
	OtherError
};

#ifdef __cplusplus
 extern "C" {
#endif

enum FlashState init_texture_flash(void);
enum FlashState ext_flash_setWriteProtect(void);
enum FlashState ext_flash_clrWriteProtect(void);
enum FlashState ext_flash_erase(unsigned long addr);
enum FlashState ext_flash_write(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState ext_flash_read(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState ext_flash_gzip_self_adaption(void);

int set_device_size (int mb);
enum FlashState qspi2_CheckTextureFlashType(void);

enum FlashState qspi_gd_nand_Q4_init(void);
enum FlashState qspi_gd_nand_Q5_init(void);
enum FlashState qspi_ds_nand_init(void);
enum FlashState qspi_nand_init(void);
enum FlashState qspi2_S25FL127S_init(void);
enum FlashState qspi2_nor_init(void);
enum FlashState qspi2_nor256_init(void);
enum FlashState qspi_nand_erase(unsigned long addr);
enum FlashState qspi_gd_nand_erase(unsigned long addr);
enum FlashState qspi2_nor_erase(unsigned long addr);
enum FlashState qspi2_nor256_erase(unsigned long addr);
enum FlashState qspi2_nor_write(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState qspi2_nor256_write(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState qspi_nand_write(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState qspi_gd_nand_write(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState qspi_nand_page_write(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState qspi_gd_nand_page_write(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState qspi_nand_read(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState qspi_gd_nand_read(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState qspi2_nor_read(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState qspi2_nor256_read(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState qspi_gd_nand_page_read(unsigned long addr, unsigned long size, unsigned char *buffer);
enum FlashState qspi_nand_page_read(unsigned long addr, unsigned long size, unsigned char *buffer);

#ifdef __cplusplus
}
#endif
