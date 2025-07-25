#include "texture_flash.h"


const u32 gd_nand_con_rom[]={
0x00C05700,
0x480088B1,
0x59000F88,
0x37940400,
0xDA331131,
0x5A003AA8,
0x4A00A001,
0x3A901300,
0x1128E081,
0x4A008AB1,
0xA5B03100,
0xAC501005,
0x3F004A00,
0xE0813A90,
0x8A903268,
0x8A10A5B0,
0xC22A3A6C,
0x8AB1CDDA,
0x2A603A60,
0x10FD7A05,
0x10FB6A02,
0x1008AD10,
0x10EEB240,
0x8CC18331,
0x760082B0,
0x10D510DE,
0x00015A00,
0xFF004A00,
0xF0003A90,
0xE0813890,
0x6A002AA0,
0x12FA10FB
};

//const u32 wb_nand_com_rom[]={
//0x00C05600,
//0x470087A1,
//0x8BA10F88,
//0x36943BFF,
//0x29903790,
//0x10FD6901,
//0x690029A0,
//0x200010F9,
//0x32683000,
//0x33A8316C,
//0xA0015800,
//0x13004800,
//0x29903890,
//0x10FD6901,
//0x29903790,
//0x10FD6901,
//0x690029A0,
//0x3B6010F9,
//0x79052960,
//0x690210FD,
//0xF00010FB
//};
const u32 nand_flash_rom[]={
0x00C05700,
0x480088B1,
0x59000F88,
0x8CB10400,
0x30002000,
0x38903794,
0x2AA0E081,
0x10FB6A00,
0x3AA8DA33,
0xA0015A00,
0x13004A00,
0xE0813A90,
0xE0813890,
0x6A002AA0,
0x326810FB,
0xA5B08A90,
0x3A6C8A10,
0xCEEAC22A,
0x2A603C60,
0x10FD7A05,
0x10FB6A02,
0x1007AE10,
0x10EFB240,
0x8DD18331,
0xBD5082B0,
0xF00010DD

};

const u32 nor_flash_rom[]={
0x30002000,
0x316C3268,
0x00015700,
0x28603760,
0x10FD7805,
0x10FB6802,
0x0000F000
};

enum FlashType TextureFlashType = NOR;
uint32_t TextureFlashSize = 0;
u32 SDRAM_IS_32MB = 1;
uint32_t FlashID = 0;

/**
  * @brief 1. check flash type
  *        2. initialize QSPI controller and DramCache
  *        3. Calibration Flash Read DelayCalibError
  *        4. Check Acf File in Flash compressed or not
  * @retval FlashState
  */
enum FlashState init_texture_flash(void)
{
	enum FlashState state;
    //check flash type and init flash
    state = qspi2_CheckTextureFlashType();
	
	if(state == InvalidFlashType) return InvalidFlashType;

	DramCache_Reset();
	
    switch(TextureFlashType)
    {
		case(S25FL127S):	state = qspi2_S25FL127S_init();break;
        case(NOR):      state = qspi2_nor_init();break;
        case(WB_NAND):  state = qspi_nand_init();break;
        case(GD_NAND_Q4):  state = qspi_gd_nand_Q4_init();break;
		case(GD_NAND_Q5):  state = qspi_gd_nand_Q5_init();break;
		case(DS_NAND):  state = qspi_ds_nand_init();break;
        case(NOR256):   state = qspi2_nor256_init();break;
    }
    
    if(state!=QSPI_NAND_OK) return OtherError;
	
	//state = ext_flash_gzip_self_adaption();

	//state = ext_flash_setWriteProtect();

	return state;
}

/**
  * @brief setWriteProtect
  *        when Ext flash is Nor Flash and its capacity less than 32MB
  * @retval FlashState
  */
enum FlashState ext_flash_setWriteProtect(void)
{
//	int tmp = 0;
	
	switch(TextureFlashType)
    {
		case(S25FL127S):
			DRAM_CACHE->REG90 = 0x06000001;	//opcode 0x06 to set the WELe to config reg, 1 data bytes to write,
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
				
			//config BPNV & QUAD bit
			DRAM_CACHE->REGA8 = 0x3E1C;
			DRAM_CACHE->REG90 = 0x01009001;
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
				
			DRAM_CACHE->REG90 = 0x05800001;
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
				
			DRAM_CACHE->REG90 = 0x35800001;
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));	
			break;
        case(NOR):
			DRAM_CACHE->REG90 = 0x50000001;	//opcode 0x50 to set the WELe to config reg, 1 data bytes to write,
			while((DRAM_CACHE->REG90)==0){};
				
			//config CMP bit
			DRAM_CACHE->REGA8 = 0x42;
			DRAM_CACHE->REG90 = 0x31008001;
			while((DRAM_CACHE->REG90 &0x02) != 0);  
			break;
        case(WB_NAND):
			DRAM_CACHE->REG90 = 0x06000001;     //write enable
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
		
			//config write protect mode
			DRAM_CACHE->REG94 = 0xA0;
			DRAM_CACHE->REGA8 = 0x7C;
			DRAM_CACHE->REG90 = 0x1f088001;
			while((DRAM_CACHE->REG90 &0x02) != 0) ; 
			break;
        case(GD_NAND_Q4):
			DRAM_CACHE->REG90 = 0x06000001;     //write enable
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
		
			//config write protect mode
			DRAM_CACHE->REG94 = 0xA0;
			DRAM_CACHE->REGA8 = 0x3E;
			DRAM_CACHE->REG90 = 0x1f088001;
			while((DRAM_CACHE->REG90 &0x02) != 0) ;  
		
			do
			{
				DRAM_CACHE->REG94 = 0xc0;
				DRAM_CACHE->REG90 = 0x0F880001;
				while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
			}while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
			break;
		case(GD_NAND_Q5):
			DRAM_CACHE->REG90 = 0x06000001;     //write enable
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
		
			//config write protect mode
			DRAM_CACHE->REG94 = 0xA0;
			DRAM_CACHE->REGA8 = 0x3E;
			DRAM_CACHE->REG90 = 0x1f088001;
			while((DRAM_CACHE->REG90 &0x02) != 0) ;  
		
			do
			{
				DRAM_CACHE->REG94 = 0xc0;
				DRAM_CACHE->REG90 = 0x0F880001;
				while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
			}while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
			break;
		case(DS_NAND):
			DRAM_CACHE->REG90 = 0x06000001;     //write enable
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
		
			//config write protect mode
			DRAM_CACHE->REG94 = 0xA0;
			DRAM_CACHE->REGA8 = 0x3E;
			DRAM_CACHE->REG90 = 0x1f088001;
			while((DRAM_CACHE->REG90 &0x02) != 0) ;  
		
			do
			{
				DRAM_CACHE->REG94 = 0xc0;
				DRAM_CACHE->REG90 = 0x0F880001;
				while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
			}while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
			break;
        case(NOR256):
			DRAM_CACHE->REG90 = 0x50000001;	//opcode 0x50 to set the WELe to config reg, 1 data bytes to write,
			while((DRAM_CACHE->REG90)==0){};     
			
			//config write protect mode
			DRAM_CACHE->REGA8 = 0x7C;
			DRAM_CACHE->REG90 = 0x01008001;
			while((DRAM_CACHE->REG90 &0x02) != 0);
			break;
    }
	return OK;
}

/**
  * @brief clrWriteProtect
  *        when Ext flash is Nor Flash and its capacity less than 32MB
  * @retval FlashState
  */
enum FlashState ext_flash_clrWriteProtect(void)
{
	switch(TextureFlashType)
    {
		case(S25FL127S):
			DRAM_CACHE->REG90 = 0x06000001;	//opcode 0x06 to set the WELe to config reg, 1 data bytes to write,
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
				
			//config BPNV & QUAD bit
			DRAM_CACHE->REGA8 = 0x3e00;
			DRAM_CACHE->REG90 = 0x01009001;
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));

			DRAM_CACHE->REG90 = 0x05800001;
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
				
			DRAM_CACHE->REG90 = 0x35800001;
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));	
				
			break;
        case(NOR):
			DRAM_CACHE->REG90 = 0x50000001;	//opcode 0x50 to set the WELe to config reg, 1 data bytes to write,
			while((DRAM_CACHE->REG90)==0){};
				
			//clear CMP bit
			DRAM_CACHE->REGA8 = 0x02;
			DRAM_CACHE->REG90 = 0x31008001;
			while((DRAM_CACHE->REG90 &0x02) != 0);  
			break;
        case(WB_NAND):
			DRAM_CACHE->REG90 = 0x06000001;     //write enable
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
		
			//clear write protect mode
			DRAM_CACHE->REG94 = 0xA0;
			DRAM_CACHE->REGA8 = 0x00;
			DRAM_CACHE->REG90 = 0x1f088001;
			while((DRAM_CACHE->REG90 &0x02) != 0) ;  
			break;
         case(GD_NAND_Q4):
			DRAM_CACHE->REG90 = 0x06000001;     //write enable
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
		
			//clear write protect mode
			DRAM_CACHE->REG94 = 0xA0;
			DRAM_CACHE->REGA8 = 0x00;
			DRAM_CACHE->REG90 = 0x1f088001;
			while((DRAM_CACHE->REG90 &0x02) != 0) ;  
		
			do
			{
				DRAM_CACHE->REG94 = 0xc0;
				DRAM_CACHE->REG90 = 0x0F880001;
				while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
			}while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
			break;
		case(GD_NAND_Q5):
			DRAM_CACHE->REG90 = 0x06000001;     //write enable
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
		
			//clear write protect mode
			DRAM_CACHE->REG94 = 0xA0;
			DRAM_CACHE->REGA8 = 0x00;
			DRAM_CACHE->REG90 = 0x1f088001;
			while((DRAM_CACHE->REG90 &0x02) != 0) ;  
		
			do
			{
				DRAM_CACHE->REG94 = 0xc0;
				DRAM_CACHE->REG90 = 0x0F880001;
				while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
			}while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
			break;
		case(DS_NAND):
			DRAM_CACHE->REG90 = 0x06000001;     //write enable
			while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
		
			//clear write protect mode
			DRAM_CACHE->REG94 = 0xA0;
			DRAM_CACHE->REGA8 = 0x00;
			DRAM_CACHE->REG90 = 0x1f088001;
			while((DRAM_CACHE->REG90 &0x02) != 0) ;  
		
			do
			{
				DRAM_CACHE->REG94 = 0xc0;
				DRAM_CACHE->REG90 = 0x0F880001;
				while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
			}while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
			break;
        case(NOR256):
			DRAM_CACHE->REG90 = 0x50000001;	//opcode 0x50 to set the WELe to config reg, 1 data bytes to write,
			while((DRAM_CACHE->REG90)==0){};     
			
			//config QE mode
			DRAM_CACHE->REGA8 = 0x00;
			DRAM_CACHE->REG90 = 0x01008001;
			while((DRAM_CACHE->REG90 &0x02) != 0);
			break;
    }
	return OK;
}


/**
  * @brief  Check Acf File in Flash compressed or not
  * @retval FlashState
  */
enum FlashState ext_flash_gzip_self_adaption(void)
{
    uint8_t temp[4];
    uint32_t flag = 0;
    enum FlashState ret;
	
	
	//check uncompress file
    ret = ext_flash_read(0x20, 4, temp);
	
	if(ret!=OK) return OtherError;
	
    flag = (uint32_t)temp[0] | ((uint32_t)temp[1] << 8) |  ((uint32_t)temp[2] << 16) | ((uint32_t)temp[3] << 24);
	
	if(flag == 0x494d4841)
	{
		DRAM_CACHE->Gzip_EN = 0x0;
		return OK;
	}
    
	
	//check compress file
    ret = ext_flash_read(0x40000, 4, temp);
	
	if(ret!=OK) return OtherError;
	
    flag = (uint32_t)temp[0] | ((uint32_t)temp[1] << 8) |  ((uint32_t)temp[2] << 16) | ((uint32_t)temp[3] << 24);
	
	if(flag == 0x494d4841)
	{
		DRAM_CACHE->Gzip_EN = 0x1;
		return OK;
	}	
	
	DRAM_CACHE->Gzip_EN = 0x0;
	return NoACFinFlash;	
	

}

/**
  * @brief  Erase the block of the Flash which addr belongs to
  *         According to FlashType, choose the corresponding funciton.
  * @param  addr: the address in the block being erased
  * @retval FlashState
  */
enum FlashState ext_flash_erase(unsigned long addr)
{
	ext_flash_clrWriteProtect();
	
    switch(TextureFlashType)
    {
		case(S25FL127S):
        case(NOR):      return qspi2_nor_erase(addr);
        case(WB_NAND):  return qspi_nand_erase(addr);
        case(GD_NAND_Q4):  return qspi_gd_nand_erase(addr);
		case(GD_NAND_Q5):  return qspi_gd_nand_erase(addr);
		case(DS_NAND):  return qspi_gd_nand_erase(addr);
        case(NOR256):   return qspi2_nor256_erase(addr);
    }
		return OtherError;
}

/**
  * @brief  Write Flash from addr 
  *         According to FlashType, choose the corresponding funciton.
  * @param  addr:   start address
  * @param  size:   write size in byte
  * @param  buffer: the data stored in
  * @retval FlashState
  */
enum FlashState ext_flash_write(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    switch(TextureFlashType)
    {
		case(S25FL127S):
        case(NOR):      return qspi2_nor_write(addr,size,buffer);
        case(WB_NAND):  return qspi_nand_write(addr,size,buffer);
        case(GD_NAND_Q4):  return qspi_gd_nand_write(addr,size,buffer);
		case(GD_NAND_Q5):  return qspi_gd_nand_write(addr,size,buffer);
		case(DS_NAND):  return qspi_gd_nand_write(addr,size,buffer);
        case(NOR256):   return qspi2_nor256_write(addr,size,buffer);
    }
		return OtherError;
}


/**
  * @brief  Read Flash to addr 
  *         According to FlashType, choose the corresponding funciton.
  * @param  addr:   start address
  * @param  size:   Read size in byte
  * @param  buffer: the data load to
  * @retval FlashState
  */
enum FlashState ext_flash_read(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    switch(TextureFlashType)
    {
		case(S25FL127S):
        case(NOR):      return qspi2_nor_read(addr,size,buffer);
        case(WB_NAND):  return qspi_nand_read(addr,size,buffer);
        case(GD_NAND_Q4):  return qspi_gd_nand_read(addr,size,buffer);
		case(GD_NAND_Q5):  return qspi_gd_nand_read(addr,size,buffer);
		case(DS_NAND):  return qspi_gd_nand_read(addr,size,buffer);
        case(NOR256):   return qspi2_nor256_read(addr,size,buffer);
    }
		
		return OtherError;
}


/**
  * @brief  check flash type
  * @retval FlashState
  */
enum FlashState qspi2_CheckTextureFlashType(void)
{
    // config flash clk divider to 32x
    DRAM_CACHE->REG00 = DRAM_CACHE->REG00 | 0x780000;
    
    // config flash read delay to 0
    DRAM_CACHE->REG10 = 1;
    
    
    DRAM_CACHE->REG90 = 0x9FB00001;
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    
    FlashID = DRAM_CACHE->REGA0;
    
    if((FlashID == 0x001940EF) || (FlashID == 0xC81940C8))
    {
        TextureFlashType = NOR256;// 32MB nor flash
        TextureFlashSize = 32 * 1024 *1024;//W25Q256 & GD25Q257
		return OK;

    }
	else if(FlashID == 0x4D182001)
    {
        TextureFlashType = S25FL127S;// 16MB nor flash
        TextureFlashSize = 16 * 1024 *1024;
		return OK;

    }
    else if((FlashID == 0x001840EF) || (FlashID == 0x001870EF) || (FlashID == 0xC81840C8) || (FlashID == 0x68184068) || (FlashID == 0x20187020) || (FlashID == 0x5E18405E))
    {
        TextureFlashType = NOR;// 16MB nor flash
        TextureFlashSize = 16 * 1024 *1024;//W25Q128 & GD25Q127 & BY25Q128 & XM25QH128 & ZB25VQ128
		return OK;

    }
    else if((FlashID == 0x001740EF) || (FlashID == 0x001770EF) || (FlashID == 0xC81740C8) || (FlashID == 0x68174068) || (FlashID == 0x20177020) || (FlashID == 0x5E17405E))
    {
        TextureFlashType = NOR;// 8MB nor flash
        TextureFlashSize = 8 * 1024 * 1024;//W25Q64 & GD25Q64 & BY25Q64 & XM25QH64 & ZB25VQ64
		return OK;

    }
	else if((FlashID == 0x20164020) || (FlashID == 0x5E16405E))
    {
        TextureFlashType = NOR;// 4MB nor flash
        TextureFlashSize = 4 * 1024 * 1024;//XM25QH32 & ZB25VQ32
		return OK;

    }
    else if(FlashID == 0x21AAEFFF)
    {
        TextureFlashType = WB_NAND;// WB nand flash
        TextureFlashSize = 125 * 1024 * 1024;//125MB WB Nand Flash
		return OK;

    }
    else if(FlashID == 0xC848B1C8)
    {
        TextureFlashType = GD_NAND_Q4;// GD nand flash
        TextureFlashSize = 125 * 1024 * 1024;//125MB GD Nand Flash
		return OK;

    }
	else if(FlashID == 0xC851C8FF)
    {
        TextureFlashType = GD_NAND_Q5;// GD nand flash
        TextureFlashSize = 125 * 1024 * 1024;//125MB GD Nand Flash
		return OK;

    }
	else if(FlashID == 0xE571E5FF)
    {
        TextureFlashType = DS_NAND;// DS nand flash
        TextureFlashSize = 125 * 1024 * 1024;//125MB GD Nand Flash
		return OK;

    }
		
	return InvalidFlashType;
}


int set_device_size (int mB)
{
    int size = 0;
    
    mB = mB>>3;
 
    while(mB!=1)
    {
        mB = mB >> 1;
        size = size + 1;
    }
    return size<<4;
}

/**
  * @brief  Calibration Flash Read Delay
  * @retval FlashState
  */
enum FlashState Flash_Delay_calib()
{
    u32 i;
    u32 tmp;
    u32 testID;
    u32 old_reg00;
    int max=-1,min=-1;
	uint8_t pass[32];
    
    old_reg00 = DRAM_CACHE->REG00;
    
    // config flash clk divider to 32x
    DRAM_CACHE->REG00 = DRAM_CACHE->REG00 | 0x780000;
    
    // config flash read delay to 0
    DRAM_CACHE->REG10 = 1;  
    
    DRAM_CACHE->REG90 = 0x9FB00001;
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    
	FlashID = DRAM_CACHE->REGA0;  
    
    //restore reg00
    DRAM_CACHE->REG00 = old_reg00;
    
    for(i=0;i<32;i++)
    {
        tmp = ((i>>1)<<1) + (((u32)i<<31)>>26) + 1;
        
        DRAM_CACHE->REG10 = tmp;
        
        DRAM_CACHE->REG90 = 0x9FB00001;
        while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
        
        testID = DRAM_CACHE->REGA0;     

        if(testID == FlashID) pass[i] = 1;
        else pass[i]=0; 
        
        if((i==0) && (pass[0]==1)) min = 0;
        else if((pass[i-1]==0) &&(pass[i]==1)) min = i;
        
        if((i==31) && (pass[31]==1)) max = 31;
        else if((pass[i-1]==1) &&(pass[i]==0)) max = i;
        
    }
    
    if((max == -1) || (max == -1)) return DelayCalibError;
    
    i = (max+min)/2;
    
    
    tmp = ((i>>1)<<1) + (((u32)i<<31)>>26) + 1;
    
    DRAM_CACHE->REG10 = tmp;
    
    return OK;
    
}

/**
  * @brief initialize QSPI controller and DramCache 
  *        when Ext flash is GD NAND Flash
  * @retval FlashState
  */
enum FlashState qspi_gd_nand_Q4_init(void)
{
    u32* ptr;
    u32  tmp;
    enum FlashState ret;
    
    ptr = (u32*)DRAM_CACHE_ROM_BASE;
    for(tmp=0;tmp<sizeof(gd_nand_con_rom)/4;tmp++)
       *ptr++ = gd_nand_con_rom[tmp];
    
    
    DRAM_CACHE->REG00 = 0x8B881;// enable QSPI, enable Direct Access, choose Device 0, change flash freq to div2
    DRAM_CACHE->REG04 = 0x20220eb; 
    DRAM_CACHE->REG38 = 0x4000;// disable auto write enable
    DRAM_CACHE->REG14 = 0xAB18001; //4x 1Gb, 128K per block, 2k per page, 2 byte addr
    DRAM_CACHE->PageInfo = set_device_size(1024) + USING_NAND_FLASH + PAGE_SIZE_2KB ;
    DRAM_CACHE->REG20 = 0x600;
    DRAM_CACHE->REG64 = 0x40;
    
    if(SDRAM_IS_32MB) 
    {
        DRAM_CACHE->BaseAddr = 0x61000000;
        DRAM_CACHE->SingleWay_EN = 0;
    }
    else 
    {
        DRAM_CACHE->BaseAddr = 0x60800000;
        DRAM_CACHE->SingleWay_EN = 1;
    }

    ret = Flash_Delay_calib();
    
    if(ret != OK) return ret;
    
    DRAM_CACHE->REG90 = 0xff000001; //reset device
    while((DRAM_CACHE->REG90)==0){};
    while((DRAM_CACHE->REG90)==0){};
    while((DRAM_CACHE->REG90)==0){};
    while((DRAM_CACHE->REG90)==0){};
    
        //set QE and ECC
    DRAM_CACHE->REG94 = 0xB0;
    DRAM_CACHE->REGA8 = 0x11;
    DRAM_CACHE->REG90 = 0x1f088001;
    while((DRAM_CACHE->REG90 &0x02) != 0) ;         
        

    
    return OK;
}

/**
  * @brief initialize QSPI controller and DramCache 
  *        when Ext flash is GD NAND Flash
  * @retval FlashState
  */
enum FlashState qspi_gd_nand_Q5_init(void)
{
    u32* ptr;
    u32  tmp;
    enum FlashState ret;
    
    ptr = (u32*)DRAM_CACHE_ROM_BASE;
    for(tmp=0;tmp<sizeof(nand_flash_rom)/4;tmp++)
       *ptr++ = nand_flash_rom[tmp];
    
    
    DRAM_CACHE->REG00 = 0x8B881;// enable QSPI, enable Direct Access, choose Device 0, change flash freq to div2
    DRAM_CACHE->REG04 = 0x40220EB; 
    DRAM_CACHE->REG38 = 0x4000;// disable auto write enable
    DRAM_CACHE->REG14 = 0xAB18001; //4x 1Gb, 128K per block, 2k per page, 2 byte addr
    DRAM_CACHE->PageInfo = set_device_size(1024) + USING_NAND_FLASH + PAGE_SIZE_2KB ;
    DRAM_CACHE->REG20 = 0x600;
    DRAM_CACHE->REG64 = 0x40;
    
    if(SDRAM_IS_32MB) 
    {
        DRAM_CACHE->BaseAddr = 0x61000000;
        DRAM_CACHE->SingleWay_EN = 0;
    }
    else 
    {
        DRAM_CACHE->BaseAddr = 0x60800000;
        DRAM_CACHE->SingleWay_EN = 1;
    }

    ret = Flash_Delay_calib();
    
    if(ret != OK) return ret;
    
    DRAM_CACHE->REG90 = 0xff000001; //reset device
    while((DRAM_CACHE->REG90)==0){};
    while((DRAM_CACHE->REG90)==0){};
    while((DRAM_CACHE->REG90)==0){};
    while((DRAM_CACHE->REG90)==0){};
    
        //set QE and ECC
    DRAM_CACHE->REG94 = 0xB0;
    DRAM_CACHE->REGA8 = 0x11;
    DRAM_CACHE->REG90 = 0x1f088001;
    while((DRAM_CACHE->REG90 &0x02) != 0) ;         
        

    
    return OK;
}

/**
  * @brief initialize QSPI controller and DramCache 
  *        when Ext flash is DS NAND Flash
  * @retval FlashState
  */
enum FlashState qspi_ds_nand_init(void)
{
    u32* ptr;
    u32  tmp;
    enum FlashState ret;
    
    ptr = (u32*)DRAM_CACHE_ROM_BASE;
    for(tmp=0;tmp<sizeof(nand_flash_rom)/4;tmp++)
       *ptr++ = nand_flash_rom[tmp];
    
    
    DRAM_CACHE->REG00 = 0xB881;// enable QSPI, enable Direct Access, choose Device 0, change flash freq to div2
    DRAM_CACHE->REG04 = 0x802006B; 
    DRAM_CACHE->REG38 = 0x4000;// disable auto write enable
    DRAM_CACHE->REG14 = 0xAB18001; //4x 1Gb, 128K per block, 2k per page, 2 byte addr
    DRAM_CACHE->PageInfo = set_device_size(1024) + USING_NAND_FLASH + PAGE_SIZE_2KB ;
    DRAM_CACHE->REG20 = 0x600;
    DRAM_CACHE->REG64 = 0x40;
    
    if(SDRAM_IS_32MB) 
    {
        DRAM_CACHE->BaseAddr = 0x61000000;
        DRAM_CACHE->SingleWay_EN = 0;
    }
    else 
    {
        DRAM_CACHE->BaseAddr = 0x60800000;
        DRAM_CACHE->SingleWay_EN = 1;
    }

    ret = Flash_Delay_calib();
    
    if(ret != OK) return ret;
    
    DRAM_CACHE->REG90 = 0xff000001; //reset device
    while((DRAM_CACHE->REG90)==0){};
    while((DRAM_CACHE->REG90)==0){};
    while((DRAM_CACHE->REG90)==0){};
    while((DRAM_CACHE->REG90)==0){};
    
        //set QE and ECC
    DRAM_CACHE->REG94 = 0xB0;
    DRAM_CACHE->REGA8 = 0x11;
    DRAM_CACHE->REG90 = 0x1f088001;
    while((DRAM_CACHE->REG90 &0x02) != 0) ;         
        

    
    return OK;
}

/**
  * @brief initialize QSPI controller and DramCache 
  *        when Ext flash is WinBond NAND Flash
  * @retval FlashState
  */
enum FlashState qspi_nand_init(void)
{
    u32* ptr;
    u32  tmp;
    enum FlashState ret;
	
    ptr = (u32*)DRAM_CACHE_ROM_BASE;
    for(tmp=0;tmp<sizeof(nand_flash_rom)/4;tmp++)
       *ptr++ = nand_flash_rom[tmp];
    


    DRAM_CACHE->REG00 = 0xB881;// enable QSPI, enable Direct Access, choose Device 0, change flash freq to div2
    DRAM_CACHE->REG04 = 0x40220eb; 
    DRAM_CACHE->REG38 = 0x4000;// disable auto write enable
    DRAM_CACHE->REG14 = 0xAB18001; //4x 1Gb, 128K per block, 2k per page, 2 byte addr
    DRAM_CACHE->PageInfo = set_device_size(1024) + USING_NAND_FLASH + PAGE_SIZE_2KB;
    DRAM_CACHE->REG20 = 0x600;
    DRAM_CACHE->REG64 = 0x40;
    
    if(SDRAM_IS_32MB) 
    {
        DRAM_CACHE->BaseAddr = 0x61000000;
        DRAM_CACHE->SingleWay_EN = 0;
    }
    else 
    {
        DRAM_CACHE->BaseAddr = 0x60800000;
        DRAM_CACHE->SingleWay_EN = 1;
    }

    ret = Flash_Delay_calib();
    if(ret != OK) return ret;
    
    DRAM_CACHE->REG90 = 0xff000001; //reset device
    while((DRAM_CACHE->REG90)==0){};
    while((DRAM_CACHE->REG90)==0){};
    while((DRAM_CACHE->REG90)==0){};
    while((DRAM_CACHE->REG90)==0){};

    //config burst mode
    DRAM_CACHE->REG94 = 0xB0;
    DRAM_CACHE->REGA8 = 0x08;
    DRAM_CACHE->REG90 = 0x1f088001;
    while((DRAM_CACHE->REG90 &0x02) != 0) ;   
    
    return OK;
}

/**
  * @brief initialize QSPI controller and DramCache 
  *        when Ext flash is Nor Flash and its capacity less than 32MB
  * @retval FlashState
  */
enum FlashState qspi2_S25FL127S_init(void)
{
    u32* ptr;
    u32  tmp;
	enum FlashState ret;
    
    ptr = (u32*)DRAM_CACHE_ROM_BASE;
    for(tmp=0;tmp<sizeof(nor_flash_rom)/4;tmp++)
        *ptr++ = nor_flash_rom[tmp];
    
    DRAM_CACHE->REG00 = 0x1B881;// enable QSPI, enable Direct Access, choose Device 0, change flash freq to div2
    DRAM_CACHE->REG04 = 0x41220eb; 
    DRAM_CACHE->REG28 = 0xff;// set mode bit 
    DRAM_CACHE->REG14 = 0x101002; //64K per block, 2k per page, 2 byte addr
    DRAM_CACHE->PageInfo = set_device_size(128) + USING_NOR_FLASH;
    DRAM_CACHE->REG20 = 0x600;
    DRAM_CACHE->REG64 = 0x40;
    
    if(SDRAM_IS_32MB) 
    {
        DRAM_CACHE->BaseAddr = 0x61000000;
        DRAM_CACHE->SingleWay_EN = 0;
    }
    else 
    {
        DRAM_CACHE->BaseAddr = 0x60800000;
        DRAM_CACHE->SingleWay_EN = 1;
    }

    ret = Flash_Delay_calib();
    if(ret != OK) return ret; 
		
    return OK;
}

/**
  * @brief initialize QSPI controller and DramCache 
  *        when Ext flash is Nor Flash and its capacity less than 32MB
  * @retval FlashState
  */
enum FlashState qspi2_nor_init(void)
{
    u32* ptr;
    u32  tmp;
	enum FlashState ret;
    
    ptr = (u32*)DRAM_CACHE_ROM_BASE;
    for(tmp=0;tmp<sizeof(nor_flash_rom)/4;tmp++)
        *ptr++ = nor_flash_rom[tmp];
    
    DRAM_CACHE->REG00 = 0x1B881;// enable QSPI, enable Direct Access, choose Device 0, change flash freq to div2
    DRAM_CACHE->REG04 = 0x41220eb; 
    DRAM_CACHE->REG28 = 0xff;// set mode bit 
    DRAM_CACHE->REG14 = 0x101002; //64K per block, 2k per page, 2 byte addr
    DRAM_CACHE->PageInfo = set_device_size(128) + USING_NOR_FLASH;
    DRAM_CACHE->REG20 = 0x600;
    DRAM_CACHE->REG64 = 0x40;
    
    if(SDRAM_IS_32MB) 
    {
        DRAM_CACHE->BaseAddr = 0x61000000;
        DRAM_CACHE->SingleWay_EN = 0;
    }
    else 
    {
        DRAM_CACHE->BaseAddr = 0x60800000;
        DRAM_CACHE->SingleWay_EN = 1;
    }

    ret = Flash_Delay_calib();
    if(ret != OK) return ret;
    
    DRAM_CACHE->REG90 = 0x50000001;	//opcode 0x50 to set the WELe to config reg, 1 data bytes to write,
	while((DRAM_CACHE->REG90)==0){};    
		
    //config QE mode
    DRAM_CACHE->REGA8 = 0x02;
    DRAM_CACHE->REG90 = 0x31008001;
    while((DRAM_CACHE->REG90 &0x02) != 0);   
		
    return OK;
}


/**
  * @brief initialize QSPI controller and DramCache 
  *        when Ext flash is Nor Flash and its capacity equals to 32MB
  * @retval FlashState
  */
enum FlashState qspi2_nor256_init(void)
{
    u32* ptr;
    u32  tmp;
	enum FlashState ret;
    
    ptr = (u32*)DRAM_CACHE_ROM_BASE;
    for(tmp=0;tmp<sizeof(nor_flash_rom)/4;tmp++)
        *ptr++ = nor_flash_rom[tmp];
    
    //DRAM_CACHE->REG00 = 0x1B881;// enable QSPI, enable Direct Access, choose Device 0, change flash freq to div2
	DRAM_CACHE->REG00 = 0x80080081;
    DRAM_CACHE->REG04 = 0x41220ec; 
    DRAM_CACHE->REG28 = 0xff;// set mode bit
    //DRAM_CACHE->REG08 = 0x13;// 4bit write
    DRAM_CACHE->REG14 = 0x101003; //64K per block, 2k per page, 4 byte addr
    DRAM_CACHE->PageInfo = set_device_size(256) + USING_NOR_FLASH;
    DRAM_CACHE->REG20 = 0x600;
    DRAM_CACHE->REG64 = 0x40;
    
    if(SDRAM_IS_32MB) 
    {
        DRAM_CACHE->BaseAddr = 0x61000000;
        DRAM_CACHE->SingleWay_EN = 0;
    }
    else 
    {
        DRAM_CACHE->BaseAddr = 0x60800000;
        DRAM_CACHE->SingleWay_EN = 1;
    }

    ret = Flash_Delay_calib();
    if(ret != OK) return ret;
    
	DRAM_CACHE->REG90 = 0xb7000001;	//opcode 0xb7 to enter 4 byte address mode.
	while((DRAM_CACHE->REG90)==0){};   
	
    DRAM_CACHE->REG90 = 0x50000001;	//opcode 0x50 to set the WELe to config reg, 1 data bytes to write,
	while((DRAM_CACHE->REG90)==0){};     
	
    //config QE mode
    DRAM_CACHE->REGA8 = 0x02;
    DRAM_CACHE->REG90 = 0x31008001;
    while((DRAM_CACHE->REG90 &0x02) != 0);
		
    return OK;
}

/**
  * @brief  Erase the block of the Flash which addr belongs to
  *         the ext flash is GD Nand Flash
  * @param  addr: the address in the block being erased
  * @retval FlashState
  */
enum FlashState qspi_gd_nand_erase(unsigned long addr)
{
    uint32_t PageAddr = (addr >> COLUMN_ADDR_LEN);

    DRAM_CACHE->REG90 = 0x06000001; //opcode 0x06 to set the WELe to config reg, 1 data bytes to write,
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)){};

	//clear write protect mode
	DRAM_CACHE->REG94 = 0xA0;
	DRAM_CACHE->REGA8 = 0x00;
	DRAM_CACHE->REG90 = 0x1f088001;
	while((DRAM_CACHE->REG90 &0x02) != 0) ;  
	
	do
	{
		DRAM_CACHE->REG94 = 0xc0;
		DRAM_CACHE->REG90 = 0x0F880001;
		while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
	}while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
		
    DRAM_CACHE->REG94 = PageAddr;       //set erase sector/block address
    DRAM_CACHE->REG90 = 0xD80A0001; //opcode 0xd8 for block erase
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)){};

    do
    {
        DRAM_CACHE->REG94 = 0xc0;
        DRAM_CACHE->REG90 = 0x0F880001;
        while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    }while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
    return OK;
}

/**
  * @brief  Erase the block of the Flash which addr belongs to
  *         the ext flash is Winbond Nand Flash
  * @param  addr: the address in the block being erased
  * @retval FlashState
  */
enum FlashState qspi_nand_erase(unsigned long addr)
{
    uint32_t PageAddr = (addr >> COLUMN_ADDR_LEN);
    
    DRAM_CACHE->REG90 = 0x06000001; //opcode 0x06 to set the WELe to config reg, 1 data bytes to write,
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)){};

    DRAM_CACHE->REG94 = PageAddr;       //set erase sector/block address
    DRAM_CACHE->REG90 = 0xD80A0001; //opcode 0xd8 for block erase
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)){};

    do
    {
        DRAM_CACHE->REG94 = 0xc0;
        DRAM_CACHE->REG90 = 0x05880001;
        while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    }while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
    return OK;
}


/**
  * @brief  Erase the block of the Flash which addr belongs to
  *         the ext flash is Nor Flash and its capacity less than 32MB
  * @param  addr: the address in the block being erased
  * @retval FlashState
  */
enum FlashState qspi2_nor_erase(unsigned long addr)
{
    DRAM_CACHE->REG90 = 0x06000001; //opcode 0x06 to set the WELe to config reg, 1 data bytes to write,
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)){};
        
    DRAM_CACHE->REG94 = addr & 0x00FFFFFF;  //set erase sector/block address
    DRAM_CACHE->REG90 = 0xD80A0001; 		//opcode 0xD8 for block erase
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)){};

    do
    {
        DRAM_CACHE->REG90 = 0x05800001;
        while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    }while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
    return OK;
}


/**
  * @brief  Erase the block of the Flash which addr belongs to
  *         the ext flash is Nor Flash and its capacity equals to 32MB
  * @param  addr: the address in the block being erased
  * @retval FlashState
  */
enum FlashState qspi2_nor256_erase(unsigned long addr)
{
    DRAM_CACHE->REG90 = 0x06000001; //opcode 0x06 to set the WELe to config reg, 1 data bytes to write,
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)){};
        
    DRAM_CACHE->REG94 = addr;       //set erase sector/block address
    DRAM_CACHE->REG90 = 0xD80B0001; //opcode 0xD8 for block erase
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)){};

    do
    {
        DRAM_CACHE->REG90 = 0x05800001;
        while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    }while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
    return OK;
}


/**
  * @brief  Write Flash from addr 
  *         the ext flash is GD NAND Flash
  * @param  addr:   start address
  * @param  size:   write size in byte
  * @param  buffer: the data stored in
  * @retval FlashState
  */
enum FlashState qspi_gd_nand_write(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    int base;
    int offset;
    int re;
    int sz;
	enum FlashState ret;
    
    base = addr>>COLUMN_ADDR_LEN;
    offset = addr % QSPI_NAND_BUFFER_LEN;
    re = size;
    
    while(re!=0)
    {
        if(re+offset > QSPI_NAND_BUFFER_LEN)
		{
			if(offset != 0)
			{
				sz = QSPI_NAND_BUFFER_LEN-offset;
			}
			else if(re / QSPI_NAND_BUFFER_LEN != 0)
			{
				sz = QSPI_NAND_BUFFER_LEN;
			}
		}
        else
		{
			sz = re;
		}
		
        ret = qspi_gd_nand_page_write((base<<COLUMN_ADDR_LEN)+offset,sz,buffer);
		
        if(ret!=OK) return ret;
		
		offset = 0;
        re -= sz;
        buffer += sz;
        base++;
    }
	return OK;
}


/**
  * @brief  Write Flash from addr, addr and size cannot cross page boundary
  *         the ext flash is GD NAND Flash
  * @param  addr:   start address
  * @param  size:   write size in byte
  * @param  buffer: the data stored in
  * @retval FlashState
  */
enum FlashState qspi_gd_nand_page_write(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    unsigned long ad = DRAM_CACHE_BUFFER_ADDR;
    unsigned long sz = size;
    unsigned long *bu = (unsigned long *)buffer;
    uint16_t addr1, addr2;
    
    addr1 = addr % QSPI_NAND_BUFFER_LEN;                //column address
    addr2 = addr >> COLUMN_ADDR_LEN;    //page address

    ad |= addr1;
    sz = (sz+3)/4*4;
    
    DRAM_CACHE->REG90 = 0x06000001;     //write enable
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)||((DRAM_CACHE->REG00 & 0x80000000)==0));
    
    //disable write protect 
    DRAM_CACHE->REG94 = 0xa0;
    DRAM_CACHE->REGA8 = 0x00;
    DRAM_CACHE->REG90 = 0x1F088001;
	while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)||((DRAM_CACHE->REG00 & 0x80000000)==0));
        
    do
    {
        DRAM_CACHE->REG94 = 0xc0;
        DRAM_CACHE->REG90 = 0x0F880001;
        while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)||((DRAM_CACHE->REG00 & 0x80000000)==0));
    }while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
    

    
    while(sz)
    {
        *(unsigned long *)(ad) = *bu++;
        ad +=4;
        sz -=4;
    }
    
    DRAM_CACHE->REG90 = 0x06000001;     //write enable
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)||((DRAM_CACHE->REG00 & 0x80000000)==0));

    // need addtional delay
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)||((DRAM_CACHE->REG00 & 0x80000000)==0));
    
    
    DRAM_CACHE->REG94 = addr2;      //set page address
    DRAM_CACHE->REG90 = 0x100A0001;     //write page address
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)||((DRAM_CACHE->REG00 & 0x80000000)==0));
    do
    {
        DRAM_CACHE->REG94 = 0xc0;
        DRAM_CACHE->REG90 = 0x0F880001;
        while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)||((DRAM_CACHE->REG00 & 0x80000000)==0));
    }while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
    
    return OK;
}


/**
  * @brief  Write Flash from addr
  *         the ext flash is WinBond NAND Flash
  * @param  addr:   start address
  * @param  size:   write size in byte
  * @param  buffer: the data stored in
  * @retval FlashState
  */
enum FlashState qspi_nand_write(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    int base;
    int offset;
    int re;
    int sz;
	enum FlashState ret;
    
    base = addr>>COLUMN_ADDR_LEN;
    offset = addr % QSPI_NAND_BUFFER_LEN;
    re = size;
    
    while(re!=0)
    {
        if(re+offset > QSPI_NAND_BUFFER_LEN)
		{
			if(offset != 0)
			{
				sz = QSPI_NAND_BUFFER_LEN-offset;
			}
			else if(re / QSPI_NAND_BUFFER_LEN != 0)
			{
				sz = QSPI_NAND_BUFFER_LEN;
			}
		}
        else
		{
			sz = re;
		}
		
        ret = qspi_nand_page_write((base<<COLUMN_ADDR_LEN)+offset,sz,buffer);
		
		if(ret!=OK) return ret;
        
		offset = 0;
        re -= sz;
        buffer += sz;
        base++;
    }
	
	return OK;
}

/**
  * @brief  Write Flash from addr, addr and size cannot cross page boundary
  *         the ext flash is WinBond NAND Flash
  * @param  addr:   start address
  * @param  size:   write size in byte
  * @param  buffer: the data stored in
  * @retval FlashState
  */
enum FlashState qspi_nand_page_write(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    unsigned long ad = DRAM_CACHE_BUFFER_ADDR;
    unsigned long sz = size;
    unsigned long *bu = (unsigned long *)buffer;
    uint16_t addr1, addr2;
    addr1 = addr % QSPI_NAND_BUFFER_LEN;                //column address
    addr2 = addr >> COLUMN_ADDR_LEN;    //page address
    
    ad |= addr1;
    sz = (sz+3)/4*4;
    
    if(size > QSPI_NAND_BUFFER_LEN)
    {
        return OtherError;
    }
    
    while(sz)
    {
        *(unsigned long *)(ad) = *bu;
        
        ad += 4;
        bu += 1;
        sz -= 4;
    }
    
    DRAM_CACHE->REG90 = 0x06000001;     //write enable
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
//  DRAM_CACHE->REG94 = addr1;          //set column address
//  DRAM_CACHE->REG90 = 0x02090001;     //write column address
//  while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    DRAM_CACHE->REG94 = addr2;          //set page address
    DRAM_CACHE->REG90 = 0x100A0001;     //write page address
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    do
    {
        DRAM_CACHE->REG94 = 0xc0;
        DRAM_CACHE->REG90 = 0x05880001;
        while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    }while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
    return OK;
}



/**
  * @brief  Write Flash from addr
  *         the ext flash is Nor Flash and its capacity less than 32MB
  * @param  addr:   start address
  * @param  size:   write size in byte
  * @param  buffer: the data stored in
  * @retval FlashState
  */
enum FlashState qspi2_nor_write(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    unsigned long ad1 = 0, ad2 = 0;
    unsigned long sz = size;
    unsigned long *bu = (unsigned long *)buffer;

    sz = (sz+3)/4*4;
    
    ad1 = addr & 0x00F00000;
    ad2 = addr & 0x000FFFFF;
    DRAM_CACHE->REG24 = ad1;
    
    while(sz)
    {
        *(unsigned long *)(DRAM_CACHE_BUFFER_ADDR + ad2) = *bu;
        
        ad2 += 4;
        bu += 1;
        sz -= 4;
		
		if(ad2 == 0x100000 && sz != 0)
		{
			ad2 = 0;
			DRAM_CACHE->REG24 = ad1 + 0x100000;
		}
    }
    
    return OK;
}

/**
  * @brief  Write Flash from addr
  *         the ext flash is Nor Flash and its capacity equals to 32MB
  * @param  addr:   start address
  * @param  size:   write size in byte
  * @param  buffer: the data stored in
  * @retval FlashState
  */
enum FlashState qspi2_nor256_write(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    unsigned long ad1 = 0, ad2 = 0;
    unsigned long sz = size;
    unsigned long *bu = (unsigned long *)buffer;

    sz = (sz+3)/4*4;
    
    ad1 = addr & 0xFFF00000;
    ad2 = addr & 0x000FFFFF;
    DRAM_CACHE->REG24 = ad1;
    
    while(sz)
    {
        *(unsigned long *)(DRAM_CACHE_BUFFER_ADDR + ad2) = *bu;
        
        ad2 += 4;
        bu += 1;
        sz -= 4;
		
		if(ad2 == 0x100000 && sz != 0)
		{
			ad2 = 0;
			DRAM_CACHE->REG24 = ad1 + 0x100000;
		}
    }
    
    return OK;
}


/**
  * @brief  Read Flash to addr 
  *         the ext flash is GD NAND Flash
  * @param  addr:   start address
  * @param  size:   read size in byte
  * @param  buffer: the data load to
  * @retval FlashState
  */
enum FlashState qspi_gd_nand_read(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    int base;
    int offset;
    int re;
    int sz;
	enum FlashState ret;
    
    base = addr>>COLUMN_ADDR_LEN;
    offset = addr % QSPI_NAND_BUFFER_LEN;
    re = size;
    
    while(re!=0)
    {
        if(re+offset > QSPI_NAND_BUFFER_LEN)
		{
			if(offset != 0)
			{
				sz = QSPI_NAND_BUFFER_LEN-offset;
			}
			else if(re / QSPI_NAND_BUFFER_LEN != 0)
			{
				sz = QSPI_NAND_BUFFER_LEN;
			}
		}
        else
		{
			sz = re;
		}
		
        ret = qspi_gd_nand_page_read((base<<COLUMN_ADDR_LEN)+offset,sz,buffer);
		
		if(ret!=OK) return OtherError;
        
		offset = 0;
        re -= sz;
        buffer += sz;
        base++;
    }   
	
	return OK;
}

/**
  * @brief  Read Flash to addr, addr and size cannot cross page boundary 
  *         the ext flash is GD NAND Flash
  * @param  addr:   start address
  * @param  size:   read size in byte
  * @param  buffer: the data load to
  * @retval FlashState
  */
enum FlashState qspi_gd_nand_page_read(unsigned long addr, unsigned long size, unsigned char *buffer)
{
//    uint32_t ad = 0;
    uint32_t ad1 = 0, ad2 = 0;
    uint32_t sz = size;
    uint32_t *bu = (uint32_t *)buffer;


    sz = (sz+3)/4*4;
    ad1 = addr % QSPI_NAND_BUFFER_LEN;      //column address
    ad2 = addr >> COLUMN_ADDR_LEN;          //page address

    
    DRAM_CACHE->REG94 = ad2;
    DRAM_CACHE->REG90 = 0x130A0001;
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)||((DRAM_CACHE->REG00 & 0x80000000)==0));
    do
    {
        DRAM_CACHE->REG94 = 0xc0;
        DRAM_CACHE->REG90 = 0x0f880001;
        while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0)||((DRAM_CACHE->REG00 & 0x80000000)==0));
    }while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
    

    
    
    while(sz)
    {
        *bu = *(uint32_t *)(DRAM_CACHE_BUFFER_ADDR + ad1);
        bu+=1;
        ad1+=4;
        sz-=4;
    }
    
    return OK;
}

/**
  * @brief  Read Flash to addr 
  *         the ext flash is WinBond NAND Flash
  * @param  addr:   start address
  * @param  size:   read size in byte
  * @param  buffer: the data load to
  * @retval FlashState
  */
enum FlashState qspi_nand_read(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    int base;
    int offset;
    int re;
    int sz;
	enum FlashState ret;
    
    base = addr>>COLUMN_ADDR_LEN;
    offset = addr % QSPI_NAND_BUFFER_LEN;
    re = size;
    
    while(re!=0)
    {
        if(re+offset > QSPI_NAND_BUFFER_LEN)
		{
			if(offset != 0)
			{
				sz = QSPI_NAND_BUFFER_LEN-offset;
			}
			else if(re / QSPI_NAND_BUFFER_LEN != 0)
			{
				sz = QSPI_NAND_BUFFER_LEN;
			}
		}
        else
		{
			sz = re;
		}
		
        ret = qspi_gd_nand_page_read((base<<COLUMN_ADDR_LEN)+offset,sz,buffer);
		
		if(ret!=OK) return ret;
        
		offset = 0;
        re -= sz;
        buffer += sz;
        base++;
    }   
	
	return OK;
}


/**
  * @brief  Read Flash to addr , addr and size cannot cross page boundary
  *         the ext flash is WinBond NAND Flash
  * @param  addr:   start address
  * @param  size:   write size in byte
  * @param  buffer: the data load to
  * @retval FlashState
  */
enum FlashState qspi_nand_page_read(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    uint32_t ad = 0;
    uint32_t /*ad1 = 0,*/ ad2 = 0;
    uint32_t sz = size;
    uint32_t *bu = (uint32_t *)buffer;

    if(size > QSPI_NAND_BUFFER_LEN)
    {
        return OtherError;
    }
    
    DRAM_CACHE->DramCacheFlush = 1;

    sz = (sz+3)/4*4;
    //ad1 = addr & 0xFFF00000;
    ad2 = addr & 0x000FFFFF;
    //DRAM_CACHE->REG24 = ad1;
    
    DRAM_CACHE->REG94 = addr/QSPI_NAND_BUFFER_LEN;
    DRAM_CACHE->REG90 = 0x130A0001;
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    do
    {
        DRAM_CACHE->REG94 = 0xc0;
        DRAM_CACHE->REG90 = 0x05880001;
        while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    }while(((DRAM_CACHE->REGA0)&0x01)||((DRAM_CACHE->REGA0)==0));
    
    ad = addr % QSPI_NAND_BUFFER_LEN;
    DRAM_CACHE->REG94 = ad;
    DRAM_CACHE->REG90 = 0x03B90081;
    while(((DRAM_CACHE->REG90)&0x2)||((DRAM_CACHE->REG90)==0));
    
    ad2 = 0;
    
    while(sz)
    {
        *bu = *(uint32_t *)(DRAM_CACHE_BUFFER_ADDR + ad2);
        bu+=1;
//      ad+=4;
        ad2+=4;
        sz-=4;
    }
    
    return OK;
}


/**
  * @brief  read Flash from addr
  *         the ext flash is Nor Flash and its capacity less than 32MB
  * @param  addr:   start address
  * @param  size:   read size in byte
  * @param  buffer: the data load to
  * @retval FlashState
  */
enum FlashState qspi2_nor_read(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    uint32_t ad1 = 0, ad2 = 0;
    uint32_t sz = size;
    uint32_t *bu = (uint32_t *)buffer;
	
	DRAM_CACHE->DramCacheFlush = 1;
    
    sz = (sz+3)/4*4;
    ad1 = addr & 0x00F00000;
    ad2 = addr & 0x000FFFFF;
    
    DRAM_CACHE->REG24 = ad1;
   
    while(sz)
    {
        *bu = *(uint32_t *)(DRAM_CACHE_BUFFER_ADDR + ad2);
        bu+=1;
        ad2+=4;
        sz-=4;
		
		if(ad2 == 0x100000 && sz != 0)
		{
			ad2 = 0;
			DRAM_CACHE->REG24 = ad1 + 0x100000;
		}
    }
    
	DRAM_CACHE->DramCacheFlush = 1;
	
    return OK;
}



/**
  * @brief  Read Flash from addr
  *         the ext flash is Nor Flash and its capacity equals to 32MB
  * @param  addr:   start address
  * @param  size:   read size in byte
  * @param  buffer: the data load to
  * @retval FlashState
  */
enum FlashState qspi2_nor256_read(unsigned long addr, unsigned long size, unsigned char *buffer)
{
    uint32_t ad1 = 0, ad2 = 0;
    uint32_t sz = size;
    uint32_t *bu = (uint32_t *)buffer;

    DRAM_CACHE->DramCacheFlush = 1;
	
    sz = (sz+3)/4*4;
    ad1 = addr & 0xFFF00000;
    ad2 = addr & 0x000FFFFF;
    
    DRAM_CACHE->REG24 = ad1;
   
    while(sz)
    {
        *bu = *(uint32_t *)(DRAM_CACHE_BUFFER_ADDR + ad2);
        bu+=1;
        ad2+=4;
        sz-=4;
		
		if(ad2 == 0x100000 && sz != 0)
		{
			ad2 = 0;
			DRAM_CACHE->REG24 = ad1 + 0x100000;
		}
    }
    
	DRAM_CACHE->DramCacheFlush = 1;
	
    return OK;
}





