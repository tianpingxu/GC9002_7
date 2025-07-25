##
## Auto Generated makefile by CDK
## Do not modify this file, and any manual changes will be erased!!!   
##
## BuildSet
ProjectName            :=Bootloader
ConfigurationName      :=BuildSet
WorkspacePath          :=./
ProjectPath            :=./
IntermediateDirectory  :=Obj
OutDir                 :=$(IntermediateDirectory)
User                   :=HP
Date                   :=27/03/2025
CDKPath                :=../../../../../../../C-Sky/CDK
ToolchainPath          :=E:/C-Sky/CDKRepo/Toolchain/XTGccElfNewlib/V2.6.1/R/
LinkerName             :=riscv64-unknown-elf-gcc
LinkerNameoption       :=
SIZE                   :=riscv64-unknown-elf-size
READELF                :=riscv64-unknown-elf-readelf
CHECKSUM               :=crc32
SharedObjectLinkerName :=
ObjectSuffix           :=.o
DependSuffix           :=.d
PreprocessSuffix       :=.i
DisassemSuffix         :=.asm
IHexSuffix             :=.ihex
BinSuffix              :=.bin
ExeSuffix              :=.elf
LibSuffix              :=.a
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
ElfInfoSwitch          :=-hlS
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
UnPreprocessorSwitch   :=-U
SourceSwitch           :=-c 
ObjdumpSwitch          :=-S
ObjcopySwitch          :=-O ihex
ObjcopyBinSwitch       :=-O binary
OutputFile             :=$(ProjectName)
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=Bootloader.txt
MakeDirCommand         :=mkdir
LinkOptions            := -mabi=ilp32e -mtune=e902 -march=rv32emcxtheadse  -nostartfiles -Wl,--gc-sections -T"$(ProjectPath)/../../linkscripts/gcc_flash_boot.ld"
LinkOtherFlagsOption   :=-Wl,-zmax-page-size=1024 
IncludePackagePath     :=
IncludeCPath           := $(IncludeSwitch). $(IncludeSwitch)../../../application $(IncludeSwitch)../../../board $(IncludeSwitch)../../../csi_core/include $(IncludeSwitch)../../../csi_driver/include $(IncludeSwitch)../../../csi_driver/smartl_rv32/include $(IncludeSwitch)../../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/include $(IncludeSwitch)../../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv $(IncludeSwitch)../../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/chip_specific_extensions/thead_rv32 $(IncludeSwitch)../../../csi_kernel/freertosv10.3.1/include $(IncludeSwitch)../../../csi_kernel/include $(IncludeSwitch)../../../csi_kernel/include/ $(IncludeSwitch)../../../libs/CMSIS/Device/GC9002/Include $(IncludeSwitch)../../../libs/GC9002_HAL_Driver/Inc $(IncludeSwitch)../../../libs/include $(IncludeSwitch)../../../libs/include/ringbuffer $(IncludeSwitch)../../../libs/include/sys $(IncludeSwitch)../../board $(IncludeSwitch)../../csi_core/include $(IncludeSwitch)../../csi_driver/include $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/include $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/chip_specific_extensions/thead_rv32 $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/include $(IncludeSwitch)../../csi_kernel/include $(IncludeSwitch)../../libs/CMSIS/Device/GC9002/Include $(IncludeSwitch)../../libs/GC9002_HAL_Driver/Inc $(IncludeSwitch)../../libs/GC90xx_HAL_Driver/Inc $(IncludeSwitch)../../libs/display $(IncludeSwitch)../../libs/include $(IncludeSwitch)../../libs/include/ringbuffer $(IncludeSwitch)../../libs/include/sys $(IncludeSwitch)../../libs/soft_libc $(IncludeSwitch)../../packages/CherryUSB $(IncludeSwitch)../../packages/CherryUSB/class/cdc $(IncludeSwitch)../../packages/CherryUSB/class/hub $(IncludeSwitch)../../packages/CherryUSB/class/msc $(IncludeSwitch)../../packages/CherryUSB/common $(IncludeSwitch)../../packages/CherryUSB/core $(IncludeSwitch)../../packages/CherryUSB/osal $(IncludeSwitch)../../packages/CherryUSB/port/musb $(IncludeSwitch)../../packages/FF13 $(IncludeSwitch)../../packages/at/at_socket $(IncludeSwitch)../../packages/at/config $(IncludeSwitch)../../packages/at/hal $(IncludeSwitch)../../packages/at/include $(IncludeSwitch)../../packages/at_device-v2.0.4/at_main $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11 $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/basic_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/ble_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/mqtt_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/ota_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/tcpip_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/wifi_command $(IncludeSwitch)../../packages/at_device-v2.0.4/inc $(IncludeSwitch)../../packages/netdev/include $(IncludeSwitch)../../packages/netdev/include/arpa $(IncludeSwitch)../../packages/rt $(IncludeSwitch)../../packages/sal_socket/impl $(IncludeSwitch)../../packages/sal_socket/include $(IncludeSwitch)../../packages/sal_socket/include/dfs_net $(IncludeSwitch)../../packages/sal_socket/include/dfs_net/sys_select/sys $(IncludeSwitch)../../packages/sal_socket/include/socket $(IncludeSwitch)../../packages/sal_socket/include/socket/netinet $(IncludeSwitch)../../packages/sal_socket/include/socket/sys_socket/sys $(IncludeSwitch)../../packages/uip/port $(IncludeSwitch)../../packages/uip/src $(IncludeSwitch)FreeRTOSConfig $(IncludeSwitch)SDCard $(IncludeSwitch)csi_hal_driver $(IncludeSwitch)main $(IncludeSwitch)Crc/core $(IncludeSwitch)EM/adapt $(IncludeSwitch)EM/core $(IncludeSwitch)Source/CAN_Diag $(IncludeSwitch)Source/CAN_Diag/NWL $(IncludeSwitch)Source/CAN_Diag/SRV $(IncludeSwitch)Source/CAN_Diag/SRV/Repro $(IncludeSwitch)Source/CAN_Diag/SRV/SIDInclude $(IncludeSwitch)Source/CAN_Diag/SRV/SIDSource/SID_XTD $(IncludeSwitch)Source/Cfg/adapt $(IncludeSwitch)Source/Cfg/core $(IncludeSwitch)Source/Crc/core $(IncludeSwitch)Source/EM/adapt $(IncludeSwitch)Source/EM/core $(IncludeSwitch)Source/FreeRTOSConfig $(IncludeSwitch)Source/SDCard $(IncludeSwitch)Source/Srv_Timer/adapt $(IncludeSwitch)Source/Srv_Timer/core $(IncludeSwitch)Source/Upgrade/adapt $(IncludeSwitch)Source/Upgrade/core $(IncludeSwitch)Source/ahmi/DrawProgramBar $(IncludeSwitch)Source/ahmi/GClib/inc $(IncludeSwitch)Source/ahmi/ParseParameter $(IncludeSwitch)Source/csi_config $(IncludeSwitch)Source/csi_hal_driver $(IncludeSwitch)Source/lcd_init $(IncludeSwitch)Source/main $(IncludeSwitch)Srv_Timer/adapt $(IncludeSwitch)Srv_Timer/core $(IncludeSwitch)Upgrade/adapt $(IncludeSwitch)Upgrade/core $(IncludeSwitch)ahmi/DrawProgramBar $(IncludeSwitch)ahmi/GClib/inc $(IncludeSwitch)ahmi/ParseParameter  
IncludeAPath           := $(IncludeSwitch). $(IncludeSwitch)../../../application $(IncludeSwitch)../../../application/template $(IncludeSwitch)../../../board $(IncludeSwitch)../../../csi_core/include $(IncludeSwitch)../../../csi_driver/include $(IncludeSwitch)../../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/include $(IncludeSwitch)../../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv $(IncludeSwitch)../../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/chip_specific_extensions/thead_rv32 $(IncludeSwitch)../../../csi_kernel/freertosv10.3.1/include $(IncludeSwitch)../../../csi_kernel/include $(IncludeSwitch)../../../libs/CMSIS/Device/GC9002/Include $(IncludeSwitch)../../../libs/GC9002_HAL_Driver/Inc $(IncludeSwitch)../../../libs/include $(IncludeSwitch)../../../libs/include/ringbuffer $(IncludeSwitch)../../../libs/include/sys $(IncludeSwitch)../../board $(IncludeSwitch)../../csi_core/include $(IncludeSwitch)../../csi_driver/include $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/include $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/chip_specific_extensions/thead_rv32 $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/include $(IncludeSwitch)../../csi_kernel/include $(IncludeSwitch)../../libs/CMSIS/Device/GC9002/Include $(IncludeSwitch)../../libs/GC9002_HAL_Driver/Inc $(IncludeSwitch)../../libs/GC90xx_HAL_Driver/Inc $(IncludeSwitch)../../libs/display $(IncludeSwitch)../../libs/include $(IncludeSwitch)../../libs/include/ringbuffer $(IncludeSwitch)../../libs/include/sys $(IncludeSwitch)../../libs/soft_libc $(IncludeSwitch)../../packages/CherryUSB $(IncludeSwitch)../../packages/CherryUSB/class/cdc $(IncludeSwitch)../../packages/CherryUSB/class/hub $(IncludeSwitch)../../packages/CherryUSB/class/msc $(IncludeSwitch)../../packages/CherryUSB/common $(IncludeSwitch)../../packages/CherryUSB/core $(IncludeSwitch)../../packages/CherryUSB/osal $(IncludeSwitch)../../packages/CherryUSB/port/musb $(IncludeSwitch)../../packages/FF13 $(IncludeSwitch)../../packages/at/at_socket $(IncludeSwitch)../../packages/at/config $(IncludeSwitch)../../packages/at/hal $(IncludeSwitch)../../packages/at/include $(IncludeSwitch)../../packages/at_device-v2.0.4/at_main $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11 $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/basic_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/ble_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/mqtt_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/ota_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/tcpip_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/wifi_command $(IncludeSwitch)../../packages/at_device-v2.0.4/inc $(IncludeSwitch)../../packages/netdev/include $(IncludeSwitch)../../packages/netdev/include/arpa $(IncludeSwitch)../../packages/rt $(IncludeSwitch)../../packages/sal_socket/impl $(IncludeSwitch)../../packages/sal_socket/include $(IncludeSwitch)../../packages/sal_socket/include/dfs_net $(IncludeSwitch)../../packages/sal_socket/include/dfs_net/sys_select/sys $(IncludeSwitch)../../packages/sal_socket/include/socket $(IncludeSwitch)../../packages/sal_socket/include/socket/netinet $(IncludeSwitch)../../packages/sal_socket/include/socket/sys_socket/sys $(IncludeSwitch)../../packages/uip/port $(IncludeSwitch)../../packages/uip/src $(IncludeSwitch)FreeRTOSConfig $(IncludeSwitch)SDCard $(IncludeSwitch)csi_hal_driver $(IncludeSwitch)main $(IncludeSwitch)Crc/core $(IncludeSwitch)EM/adapt $(IncludeSwitch)EM/core $(IncludeSwitch)Source/CAN_Diag $(IncludeSwitch)Source/CAN_Diag/NWL $(IncludeSwitch)Source/CAN_Diag/SRV $(IncludeSwitch)Source/CAN_Diag/SRV/Repro $(IncludeSwitch)Source/CAN_Diag/SRV/SIDInclude $(IncludeSwitch)Source/CAN_Diag/SRV/SIDSource/SID_XTD $(IncludeSwitch)Source/Cfg/adapt $(IncludeSwitch)Source/Crc/core $(IncludeSwitch)Source/EM/adapt $(IncludeSwitch)Source/EM/core $(IncludeSwitch)Source/FreeRTOSConfig $(IncludeSwitch)Source/SDCard $(IncludeSwitch)Source/Srv_Timer/adapt $(IncludeSwitch)Source/Srv_Timer/core $(IncludeSwitch)Source/Upgrade/adapt $(IncludeSwitch)Source/Upgrade/core $(IncludeSwitch)Source/ahmi/DrawProgramBar $(IncludeSwitch)Source/ahmi/GClib/inc $(IncludeSwitch)Source/ahmi/ParseParameter $(IncludeSwitch)Source/csi_config $(IncludeSwitch)Source/csi_hal_driver $(IncludeSwitch)Source/lcd_init $(IncludeSwitch)Source/main $(IncludeSwitch)Srv_Timer/adapt $(IncludeSwitch)Srv_Timer/core $(IncludeSwitch)Upgrade/adapt $(IncludeSwitch)Upgrade/core $(IncludeSwitch)ahmi/DrawProgramBar $(IncludeSwitch)ahmi/GClib/inc $(IncludeSwitch)ahmi/ParseParameter  
Libs                   := -Wl,--start-group  -Wl,--end-group $(LibrarySwitch)m  
ArLibs                 := "m" 
PackagesLibPath        :=
LibPath                := $(PackagesLibPath) 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       :=riscv64-unknown-elf-ar rcu
CXX      :=riscv64-unknown-elf-g++
CC       :=riscv64-unknown-elf-gcc
AS       :=riscv64-unknown-elf-gcc
OBJDUMP  :=riscv64-unknown-elf-objdump
OBJCOPY  :=riscv64-unknown-elf-objcopy
CXXFLAGS := -mabi=ilp32e -mtune=e902 -march=rv32emcxtheadse   $(PreprocessorSwitch)USE_HAL_DRIVER  $(PreprocessorSwitch)CHIP_GC9002   -O2  -g3  -Wall -ffunction-sections -fdata-sections -Wall -Wno-unused-function -Wno-unused-but-set-variable -Wno-comment -Wno-unused-variable -Wno-maybe-uninitialized 
CFLAGS   := -mabi=ilp32e -mtune=e902 -march=rv32emcxtheadse   $(PreprocessorSwitch)USE_HAL_DRIVER  $(PreprocessorSwitch)CHIP_GC9002   -O2  -g3  -Wall -ffunction-sections -fdata-sections -Wall -Wno-unused-function -Wno-unused-but-set-variable -Wno-comment -Wno-unused-variable -Wno-maybe-uninitialized 
ASFLAGS  := -mabi=ilp32e -mtune=e902 -march=rv32emcxtheadse   $(PreprocessorSwitch)CHIP_GC9002   -Wa,--gdwarf2    


Objects0=$(IntermediateDirectory)/board_board$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_adc$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_crc$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_dma$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_gpio$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_gpio_pin$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_i2s$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_iic$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_irq$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_irq_vectors$(ObjectSuffix) \
	$(IntermediateDirectory)/csi_driver_csi_motor$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_pin$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_porting$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_pwm$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_qspi$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_rtc$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_spi$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_spiflash$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_tick$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_timer$(ObjectSuffix) \
	$(IntermediateDirectory)/csi_driver_csi_triangle$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_uart$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_wdt$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_drv_eth$(ObjectSuffix) $(IntermediateDirectory)/csi_hal_driver_csi_hal_driver_flash$(ObjectSuffix) $(IntermediateDirectory)/csi_hal_driver_gc9002_power$(ObjectSuffix) $(IntermediateDirectory)/main_main$(ObjectSuffix) $(IntermediateDirectory)/main_Bootloader_Version$(ObjectSuffix) $(IntermediateDirectory)/CAN_Diag_DiagPlatform$(ObjectSuffix) $(IntermediateDirectory)/CAN_Diag_DiagTimer$(ObjectSuffix) \
	$(IntermediateDirectory)/SDCard_SPI_SDCard$(ObjectSuffix) $(IntermediateDirectory)/lcd_init_lcd_init$(ObjectSuffix) $(IntermediateDirectory)/lcd_init_spi_3line$(ObjectSuffix) $(IntermediateDirectory)/display_display$(ObjectSuffix) $(IntermediateDirectory)/display_display_888$(ObjectSuffix) $(IntermediateDirectory)/display_display_gc9307n$(ObjectSuffix) $(IntermediateDirectory)/display_display_leike$(ObjectSuffix) $(IntermediateDirectory)/display_display_mrb3205$(ObjectSuffix) $(IntermediateDirectory)/display_display_rgb565$(ObjectSuffix) $(IntermediateDirectory)/display_display_sjxd$(ObjectSuffix) \
	$(IntermediateDirectory)/display_display_st7512$(ObjectSuffix) $(IntermediateDirectory)/display_display_st7735s$(ObjectSuffix) $(IntermediateDirectory)/display_display_st77903$(ObjectSuffix) $(IntermediateDirectory)/display_display_tft128qm071$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/libc_malloc$(ObjectSuffix) $(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix) $(IntermediateDirectory)/libc_printf$(ObjectSuffix) $(IntermediateDirectory)/libc__init$(ObjectSuffix) $(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix) $(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix) \
	$(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_free$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix) $(IntermediateDirectory)/ringbuffer_ringbuffer$(ObjectSuffix) $(IntermediateDirectory)/soft_libc_soft_libc$(ObjectSuffix) \
	$(IntermediateDirectory)/syslog_syslog$(ObjectSuffix) $(IntermediateDirectory)/FF13_diskio$(ObjectSuffix) $(IntermediateDirectory)/FF13_ff$(ObjectSuffix) $(IntermediateDirectory)/FF13_ffsystem$(ObjectSuffix) $(IntermediateDirectory)/FF13_ffunicode$(ObjectSuffix) $(IntermediateDirectory)/DrawProgramBar_DrawProgramBar$(ObjectSuffix) $(IntermediateDirectory)/DrawProgramBar_short_sourcebuffer$(ObjectSuffix) $(IntermediateDirectory)/ParseParameter_ParsePara$(ObjectSuffix) $(IntermediateDirectory)/core_crc_c1$(ObjectSuffix) $(IntermediateDirectory)/core_em_c1$(ObjectSuffix) \
	$(IntermediateDirectory)/core_srv_timer_c1$(ObjectSuffix) $(IntermediateDirectory)/core_upgrade_c1$(ObjectSuffix) $(IntermediateDirectory)/core_upgrade_sm$(ObjectSuffix) $(IntermediateDirectory)/core_upgrade_subsm$(ObjectSuffix) $(IntermediateDirectory)/core_upgrade_passive$(ObjectSuffix) $(IntermediateDirectory)/core_upgrade_subsm_passive$(ObjectSuffix) $(IntermediateDirectory)/core_upgrade_uart_platform$(ObjectSuffix) $(IntermediateDirectory)/core_upgrade_btld$(ObjectSuffix) $(IntermediateDirectory)/NWL_DiagNWL$(ObjectSuffix) $(IntermediateDirectory)/SRV_DiagMgr$(ObjectSuffix) \
	$(IntermediateDirectory)/SRV_DiagTbl$(ObjectSuffix) $(IntermediateDirectory)/SRV_DID$(ObjectSuffix) $(IntermediateDirectory)/SRV_Security$(ObjectSuffix) $(IntermediateDirectory)/SRV_Session$(ObjectSuffix) $(IntermediateDirectory)/adapt_cfg_autogen_ca$(ObjectSuffix) $(IntermediateDirectory)/adapt_cfg_ca$(ObjectSuffix) $(IntermediateDirectory)/core_cfg_c1$(ObjectSuffix) $(IntermediateDirectory)/adapter_csi_freertos$(ObjectSuffix) 

Objects2=$(IntermediateDirectory)/Src_gc9002_hal_adc$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_ahmi$(ObjectSuffix) \
	$(IntermediateDirectory)/Src_gc9002_hal_ccm$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_crc32$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_display$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_dma$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_eth$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_extension$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_hyper$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_pmu$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_rcc$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_usb$(ObjectSuffix) \
	$(IntermediateDirectory)/Src_gc9002_hal_usbd$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_wdt$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_ll_ahmi$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_ll_display$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal$(ObjectSuffix) $(IntermediateDirectory)/Src_mmalloc$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_can$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_canfd$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_gpio$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_i2c$(ObjectSuffix) \
	$(IntermediateDirectory)/Src_gc90xx_hal_i2s$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_mmc$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_rtc$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_sd$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_spi$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_tim$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_tim_ex$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_uart$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_videocap$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_ll_qspi$(ObjectSuffix) \
	$(IntermediateDirectory)/Src_gc90xx_ll_sdmmc$(ObjectSuffix) $(IntermediateDirectory)/core_usbd_core$(ObjectSuffix) $(IntermediateDirectory)/core_usbh_core$(ObjectSuffix) $(IntermediateDirectory)/osal_usb_osal_freertos$(ObjectSuffix) $(IntermediateDirectory)/src_gc90x_ahmi$(ObjectSuffix) $(IntermediateDirectory)/src_gc90x_cache$(ObjectSuffix) $(IntermediateDirectory)/src_gc90x_display$(ObjectSuffix) $(IntermediateDirectory)/Repro_Repro$(ObjectSuffix) 

Objects3=$(IntermediateDirectory)/SIDSource_SID_10$(ObjectSuffix) $(IntermediateDirectory)/SIDSource_SID_11$(ObjectSuffix) \
	$(IntermediateDirectory)/SIDSource_SID_28$(ObjectSuffix) $(IntermediateDirectory)/Source_croutine$(ObjectSuffix) $(IntermediateDirectory)/Source_event_groups$(ObjectSuffix) $(IntermediateDirectory)/Source_list$(ObjectSuffix) $(IntermediateDirectory)/Source_queue$(ObjectSuffix) $(IntermediateDirectory)/Source_stream_buffer$(ObjectSuffix) $(IntermediateDirectory)/Source_tasks$(ObjectSuffix) $(IntermediateDirectory)/Source_timers$(ObjectSuffix) $(IntermediateDirectory)/cdc_usbd_cdc$(ObjectSuffix) $(IntermediateDirectory)/cdc_usbh_cdc_acm$(ObjectSuffix) \
	$(IntermediateDirectory)/hub_usbh_hub$(ObjectSuffix) $(IntermediateDirectory)/msc_usbh_msc$(ObjectSuffix) $(IntermediateDirectory)/musb_usb_dc_musb$(ObjectSuffix) $(IntermediateDirectory)/musb_usb_hc_musb$(ObjectSuffix) $(IntermediateDirectory)/SID_XTD_SID_64$(ObjectSuffix) $(IntermediateDirectory)/SID_XTD_SID_66$(ObjectSuffix) $(IntermediateDirectory)/SID_XTD_SID_67$(ObjectSuffix) $(IntermediateDirectory)/Source_startup$(ObjectSuffix) $(IntermediateDirectory)/Source_system$(ObjectSuffix) $(IntermediateDirectory)/Source_trap_c$(ObjectSuffix) \
	$(IntermediateDirectory)/Source_vectors$(ObjectSuffix) $(IntermediateDirectory)/MemMang_heap_4$(ObjectSuffix) $(IntermediateDirectory)/riscv_port$(ObjectSuffix) $(IntermediateDirectory)/riscv_portASM$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) $(Objects2) $(Objects3) 

##
## Main Build Targets 
##
.PHONY: all
all: $(IntermediateDirectory)/$(OutputFile)

$(IntermediateDirectory)/$(OutputFile):  $(Objects) Always_Link 
	$(LinkerName) $(OutputSwitch) $(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) $(LinkerNameoption) -Wl,-Map=$(ProjectPath)/Lst/$(OutputFile).map  @$(ObjectsFileList)  $(LinkOptions) $(LibPath) $(Libs) $(LinkOtherFlagsOption)
	-@mv $(ProjectPath)/Lst/$(OutputFile).map $(ProjectPath)/Lst/$(OutputFile).temp && $(READELF) $(ElfInfoSwitch) $(ProjectPath)/Obj/$(OutputFile)$(ExeSuffix) > $(ProjectPath)/Lst/$(OutputFile).map && echo ====================================================================== >> $(ProjectPath)/Lst/$(OutputFile).map && cat $(ProjectPath)/Lst/$(OutputFile).temp >> $(ProjectPath)/Lst/$(OutputFile).map && rm -rf $(ProjectPath)/Lst/$(OutputFile).temp
	$(OBJDUMP) $(ObjdumpSwitch) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix)  > $(ProjectPath)/Lst/$(OutputFile)$(DisassemSuffix) 
	@echo size of target:
	@$(SIZE) $(ProjectPath)$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) 
	@echo -n checksum value of target:  
	@$(CHECKSUM) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) 
	@$(ProjectName).modify.bat $(IntermediateDirectory) $(OutputFile)$(ExeSuffix) 

Always_Link:


##
## Objects
##
$(IntermediateDirectory)/board_board$(ObjectSuffix): ../../board/board.c  
	$(CC) $(SourceSwitch) ../../board/board.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/board_board$(ObjectSuffix) -MF$(IntermediateDirectory)/board_board$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/board_board$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/board_board$(PreprocessSuffix): ../../board/board.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/board_board$(PreprocessSuffix) ../../board/board.c

$(IntermediateDirectory)/csi_driver_csi_adc$(ObjectSuffix): ../../csi_driver/csi_adc.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_adc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_adc$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_adc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_adc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_adc$(PreprocessSuffix): ../../csi_driver/csi_adc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_adc$(PreprocessSuffix) ../../csi_driver/csi_adc.c

$(IntermediateDirectory)/csi_driver_csi_crc$(ObjectSuffix): ../../csi_driver/csi_crc.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_crc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_crc$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_crc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_crc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_crc$(PreprocessSuffix): ../../csi_driver/csi_crc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_crc$(PreprocessSuffix) ../../csi_driver/csi_crc.c

$(IntermediateDirectory)/csi_driver_csi_dma$(ObjectSuffix): ../../csi_driver/csi_dma.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_dma.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_dma$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_dma$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_dma$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_dma$(PreprocessSuffix): ../../csi_driver/csi_dma.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_dma$(PreprocessSuffix) ../../csi_driver/csi_dma.c

$(IntermediateDirectory)/csi_driver_csi_gpio$(ObjectSuffix): ../../csi_driver/csi_gpio.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_gpio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_gpio$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_gpio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_gpio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_gpio$(PreprocessSuffix): ../../csi_driver/csi_gpio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_gpio$(PreprocessSuffix) ../../csi_driver/csi_gpio.c

$(IntermediateDirectory)/csi_driver_csi_gpio_pin$(ObjectSuffix): ../../csi_driver/csi_gpio_pin.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_gpio_pin.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_gpio_pin$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_gpio_pin$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_gpio_pin$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_gpio_pin$(PreprocessSuffix): ../../csi_driver/csi_gpio_pin.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_gpio_pin$(PreprocessSuffix) ../../csi_driver/csi_gpio_pin.c

$(IntermediateDirectory)/csi_driver_csi_i2s$(ObjectSuffix): ../../csi_driver/csi_i2s.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_i2s.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_i2s$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_i2s$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_i2s$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_i2s$(PreprocessSuffix): ../../csi_driver/csi_i2s.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_i2s$(PreprocessSuffix) ../../csi_driver/csi_i2s.c

$(IntermediateDirectory)/csi_driver_csi_iic$(ObjectSuffix): ../../csi_driver/csi_iic.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_iic.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_iic$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_iic$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_iic$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_iic$(PreprocessSuffix): ../../csi_driver/csi_iic.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_iic$(PreprocessSuffix) ../../csi_driver/csi_iic.c

$(IntermediateDirectory)/csi_driver_csi_irq$(ObjectSuffix): ../../csi_driver/csi_irq.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_irq.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_irq$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_irq$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_irq$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_irq$(PreprocessSuffix): ../../csi_driver/csi_irq.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_irq$(PreprocessSuffix) ../../csi_driver/csi_irq.c

$(IntermediateDirectory)/csi_driver_csi_irq_vectors$(ObjectSuffix): ../../csi_driver/csi_irq_vectors.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_irq_vectors.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_irq_vectors$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_irq_vectors$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_irq_vectors$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_irq_vectors$(PreprocessSuffix): ../../csi_driver/csi_irq_vectors.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_irq_vectors$(PreprocessSuffix) ../../csi_driver/csi_irq_vectors.c

$(IntermediateDirectory)/csi_driver_csi_motor$(ObjectSuffix): ../../csi_driver/csi_motor.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_motor.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_motor$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_motor$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_motor$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_motor$(PreprocessSuffix): ../../csi_driver/csi_motor.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_motor$(PreprocessSuffix) ../../csi_driver/csi_motor.c

$(IntermediateDirectory)/csi_driver_csi_pin$(ObjectSuffix): ../../csi_driver/csi_pin.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_pin.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_pin$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_pin$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_pin$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_pin$(PreprocessSuffix): ../../csi_driver/csi_pin.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_pin$(PreprocessSuffix) ../../csi_driver/csi_pin.c

$(IntermediateDirectory)/csi_driver_csi_porting$(ObjectSuffix): ../../csi_driver/csi_porting.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_porting.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_porting$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_porting$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_porting$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_porting$(PreprocessSuffix): ../../csi_driver/csi_porting.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_porting$(PreprocessSuffix) ../../csi_driver/csi_porting.c

$(IntermediateDirectory)/csi_driver_csi_pwm$(ObjectSuffix): ../../csi_driver/csi_pwm.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_pwm.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_pwm$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_pwm$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_pwm$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_pwm$(PreprocessSuffix): ../../csi_driver/csi_pwm.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_pwm$(PreprocessSuffix) ../../csi_driver/csi_pwm.c

$(IntermediateDirectory)/csi_driver_csi_qspi$(ObjectSuffix): ../../csi_driver/csi_qspi.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_qspi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_qspi$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_qspi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_qspi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_qspi$(PreprocessSuffix): ../../csi_driver/csi_qspi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_qspi$(PreprocessSuffix) ../../csi_driver/csi_qspi.c

$(IntermediateDirectory)/csi_driver_csi_rtc$(ObjectSuffix): ../../csi_driver/csi_rtc.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_rtc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_rtc$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_rtc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_rtc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_rtc$(PreprocessSuffix): ../../csi_driver/csi_rtc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_rtc$(PreprocessSuffix) ../../csi_driver/csi_rtc.c

$(IntermediateDirectory)/csi_driver_csi_spi$(ObjectSuffix): ../../csi_driver/csi_spi.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_spi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_spi$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_spi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_spi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_spi$(PreprocessSuffix): ../../csi_driver/csi_spi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_spi$(PreprocessSuffix) ../../csi_driver/csi_spi.c

$(IntermediateDirectory)/csi_driver_csi_spiflash$(ObjectSuffix): ../../csi_driver/csi_spiflash.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_spiflash.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_spiflash$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_spiflash$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_spiflash$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_spiflash$(PreprocessSuffix): ../../csi_driver/csi_spiflash.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_spiflash$(PreprocessSuffix) ../../csi_driver/csi_spiflash.c

$(IntermediateDirectory)/csi_driver_csi_tick$(ObjectSuffix): ../../csi_driver/csi_tick.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_tick.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_tick$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_tick$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_tick$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_tick$(PreprocessSuffix): ../../csi_driver/csi_tick.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_tick$(PreprocessSuffix) ../../csi_driver/csi_tick.c

$(IntermediateDirectory)/csi_driver_csi_timer$(ObjectSuffix): ../../csi_driver/csi_timer.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_timer.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_timer$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_timer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_timer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_timer$(PreprocessSuffix): ../../csi_driver/csi_timer.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_timer$(PreprocessSuffix) ../../csi_driver/csi_timer.c

$(IntermediateDirectory)/csi_driver_csi_triangle$(ObjectSuffix): ../../csi_driver/csi_triangle.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_triangle.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_triangle$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_triangle$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_triangle$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_triangle$(PreprocessSuffix): ../../csi_driver/csi_triangle.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_triangle$(PreprocessSuffix) ../../csi_driver/csi_triangle.c

$(IntermediateDirectory)/csi_driver_csi_uart$(ObjectSuffix): ../../csi_driver/csi_uart.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_uart.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_uart$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_uart$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_uart$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_uart$(PreprocessSuffix): ../../csi_driver/csi_uart.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_uart$(PreprocessSuffix) ../../csi_driver/csi_uart.c

$(IntermediateDirectory)/csi_driver_csi_wdt$(ObjectSuffix): ../../csi_driver/csi_wdt.c  
	$(CC) $(SourceSwitch) ../../csi_driver/csi_wdt.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_csi_wdt$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_csi_wdt$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_csi_wdt$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_csi_wdt$(PreprocessSuffix): ../../csi_driver/csi_wdt.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_csi_wdt$(PreprocessSuffix) ../../csi_driver/csi_wdt.c

$(IntermediateDirectory)/csi_driver_drv_eth$(ObjectSuffix): ../../csi_driver/drv_eth.c  
	$(CC) $(SourceSwitch) ../../csi_driver/drv_eth.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_driver_drv_eth$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_driver_drv_eth$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_driver_drv_eth$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_driver_drv_eth$(PreprocessSuffix): ../../csi_driver/drv_eth.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_driver_drv_eth$(PreprocessSuffix) ../../csi_driver/drv_eth.c

$(IntermediateDirectory)/csi_hal_driver_csi_hal_driver_flash$(ObjectSuffix): Source/csi_hal_driver/csi_hal_driver_flash.c  
	$(CC) $(SourceSwitch) Source/csi_hal_driver/csi_hal_driver_flash.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_hal_driver_csi_hal_driver_flash$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_hal_driver_csi_hal_driver_flash$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_hal_driver_csi_hal_driver_flash$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_hal_driver_csi_hal_driver_flash$(PreprocessSuffix): Source/csi_hal_driver/csi_hal_driver_flash.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_hal_driver_csi_hal_driver_flash$(PreprocessSuffix) Source/csi_hal_driver/csi_hal_driver_flash.c

$(IntermediateDirectory)/csi_hal_driver_gc9002_power$(ObjectSuffix): Source/csi_hal_driver/gc9002_power.c  
	$(CC) $(SourceSwitch) Source/csi_hal_driver/gc9002_power.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_hal_driver_gc9002_power$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_hal_driver_gc9002_power$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_hal_driver_gc9002_power$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_hal_driver_gc9002_power$(PreprocessSuffix): Source/csi_hal_driver/gc9002_power.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_hal_driver_gc9002_power$(PreprocessSuffix) Source/csi_hal_driver/gc9002_power.c

$(IntermediateDirectory)/main_main$(ObjectSuffix): Source/main/main.c  
	$(CC) $(SourceSwitch) Source/main/main.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/main_main$(ObjectSuffix) -MF$(IntermediateDirectory)/main_main$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/main_main$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/main_main$(PreprocessSuffix): Source/main/main.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/main_main$(PreprocessSuffix) Source/main/main.c

$(IntermediateDirectory)/main_Bootloader_Version$(ObjectSuffix): Source/main/Bootloader_Version.c  
	$(CC) $(SourceSwitch) Source/main/Bootloader_Version.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/main_Bootloader_Version$(ObjectSuffix) -MF$(IntermediateDirectory)/main_Bootloader_Version$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/main_Bootloader_Version$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/main_Bootloader_Version$(PreprocessSuffix): Source/main/Bootloader_Version.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/main_Bootloader_Version$(PreprocessSuffix) Source/main/Bootloader_Version.c

$(IntermediateDirectory)/CAN_Diag_DiagPlatform$(ObjectSuffix): Source/CAN_Diag/DiagPlatform.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/DiagPlatform.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/CAN_Diag_DiagPlatform$(ObjectSuffix) -MF$(IntermediateDirectory)/CAN_Diag_DiagPlatform$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/CAN_Diag_DiagPlatform$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/CAN_Diag_DiagPlatform$(PreprocessSuffix): Source/CAN_Diag/DiagPlatform.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/CAN_Diag_DiagPlatform$(PreprocessSuffix) Source/CAN_Diag/DiagPlatform.c

$(IntermediateDirectory)/CAN_Diag_DiagTimer$(ObjectSuffix): Source/CAN_Diag/DiagTimer.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/DiagTimer.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/CAN_Diag_DiagTimer$(ObjectSuffix) -MF$(IntermediateDirectory)/CAN_Diag_DiagTimer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/CAN_Diag_DiagTimer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/CAN_Diag_DiagTimer$(PreprocessSuffix): Source/CAN_Diag/DiagTimer.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/CAN_Diag_DiagTimer$(PreprocessSuffix) Source/CAN_Diag/DiagTimer.c

$(IntermediateDirectory)/SDCard_SPI_SDCard$(ObjectSuffix): Source/SDCard/SPI_SDCard.c  
	$(CC) $(SourceSwitch) Source/SDCard/SPI_SDCard.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SDCard_SPI_SDCard$(ObjectSuffix) -MF$(IntermediateDirectory)/SDCard_SPI_SDCard$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SDCard_SPI_SDCard$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SDCard_SPI_SDCard$(PreprocessSuffix): Source/SDCard/SPI_SDCard.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SDCard_SPI_SDCard$(PreprocessSuffix) Source/SDCard/SPI_SDCard.c

$(IntermediateDirectory)/lcd_init_lcd_init$(ObjectSuffix): Source/lcd_init/lcd_init.c  
	$(CC) $(SourceSwitch) Source/lcd_init/lcd_init.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/lcd_init_lcd_init$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_init_lcd_init$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/lcd_init_lcd_init$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/lcd_init_lcd_init$(PreprocessSuffix): Source/lcd_init/lcd_init.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/lcd_init_lcd_init$(PreprocessSuffix) Source/lcd_init/lcd_init.c

$(IntermediateDirectory)/lcd_init_spi_3line$(ObjectSuffix): Source/lcd_init/spi_3line.c  
	$(CC) $(SourceSwitch) Source/lcd_init/spi_3line.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/lcd_init_spi_3line$(ObjectSuffix) -MF$(IntermediateDirectory)/lcd_init_spi_3line$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/lcd_init_spi_3line$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/lcd_init_spi_3line$(PreprocessSuffix): Source/lcd_init/spi_3line.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/lcd_init_spi_3line$(PreprocessSuffix) Source/lcd_init/spi_3line.c

$(IntermediateDirectory)/display_display$(ObjectSuffix): ../../libs/display/display.cpp  
	$(CXX) $(SourceSwitch) ../../libs/display/display.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/display_display$(ObjectSuffix) -MF$(IntermediateDirectory)/display_display$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/display_display$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/display_display$(PreprocessSuffix): ../../libs/display/display.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/display_display$(PreprocessSuffix) ../../libs/display/display.cpp

$(IntermediateDirectory)/display_display_888$(ObjectSuffix): ../../libs/display/display_888.c  
	$(CC) $(SourceSwitch) ../../libs/display/display_888.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/display_display_888$(ObjectSuffix) -MF$(IntermediateDirectory)/display_display_888$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/display_display_888$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/display_display_888$(PreprocessSuffix): ../../libs/display/display_888.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/display_display_888$(PreprocessSuffix) ../../libs/display/display_888.c

$(IntermediateDirectory)/display_display_gc9307n$(ObjectSuffix): ../../libs/display/display_gc9307n.c  
	$(CC) $(SourceSwitch) ../../libs/display/display_gc9307n.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/display_display_gc9307n$(ObjectSuffix) -MF$(IntermediateDirectory)/display_display_gc9307n$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/display_display_gc9307n$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/display_display_gc9307n$(PreprocessSuffix): ../../libs/display/display_gc9307n.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/display_display_gc9307n$(PreprocessSuffix) ../../libs/display/display_gc9307n.c

$(IntermediateDirectory)/display_display_leike$(ObjectSuffix): ../../libs/display/display_leike.c  
	$(CC) $(SourceSwitch) ../../libs/display/display_leike.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/display_display_leike$(ObjectSuffix) -MF$(IntermediateDirectory)/display_display_leike$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/display_display_leike$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/display_display_leike$(PreprocessSuffix): ../../libs/display/display_leike.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/display_display_leike$(PreprocessSuffix) ../../libs/display/display_leike.c

$(IntermediateDirectory)/display_display_mrb3205$(ObjectSuffix): ../../libs/display/display_mrb3205.c  
	$(CC) $(SourceSwitch) ../../libs/display/display_mrb3205.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/display_display_mrb3205$(ObjectSuffix) -MF$(IntermediateDirectory)/display_display_mrb3205$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/display_display_mrb3205$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/display_display_mrb3205$(PreprocessSuffix): ../../libs/display/display_mrb3205.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/display_display_mrb3205$(PreprocessSuffix) ../../libs/display/display_mrb3205.c

$(IntermediateDirectory)/display_display_rgb565$(ObjectSuffix): ../../libs/display/display_rgb565.c  
	$(CC) $(SourceSwitch) ../../libs/display/display_rgb565.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/display_display_rgb565$(ObjectSuffix) -MF$(IntermediateDirectory)/display_display_rgb565$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/display_display_rgb565$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/display_display_rgb565$(PreprocessSuffix): ../../libs/display/display_rgb565.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/display_display_rgb565$(PreprocessSuffix) ../../libs/display/display_rgb565.c

$(IntermediateDirectory)/display_display_sjxd$(ObjectSuffix): ../../libs/display/display_sjxd.c  
	$(CC) $(SourceSwitch) ../../libs/display/display_sjxd.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/display_display_sjxd$(ObjectSuffix) -MF$(IntermediateDirectory)/display_display_sjxd$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/display_display_sjxd$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/display_display_sjxd$(PreprocessSuffix): ../../libs/display/display_sjxd.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/display_display_sjxd$(PreprocessSuffix) ../../libs/display/display_sjxd.c

$(IntermediateDirectory)/display_display_st7512$(ObjectSuffix): ../../libs/display/display_st7512.c  
	$(CC) $(SourceSwitch) ../../libs/display/display_st7512.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/display_display_st7512$(ObjectSuffix) -MF$(IntermediateDirectory)/display_display_st7512$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/display_display_st7512$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/display_display_st7512$(PreprocessSuffix): ../../libs/display/display_st7512.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/display_display_st7512$(PreprocessSuffix) ../../libs/display/display_st7512.c

$(IntermediateDirectory)/display_display_st7735s$(ObjectSuffix): ../../libs/display/display_st7735s.c  
	$(CC) $(SourceSwitch) ../../libs/display/display_st7735s.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/display_display_st7735s$(ObjectSuffix) -MF$(IntermediateDirectory)/display_display_st7735s$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/display_display_st7735s$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/display_display_st7735s$(PreprocessSuffix): ../../libs/display/display_st7735s.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/display_display_st7735s$(PreprocessSuffix) ../../libs/display/display_st7735s.c

$(IntermediateDirectory)/display_display_st77903$(ObjectSuffix): ../../libs/display/display_st77903.c  
	$(CC) $(SourceSwitch) ../../libs/display/display_st77903.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/display_display_st77903$(ObjectSuffix) -MF$(IntermediateDirectory)/display_display_st77903$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/display_display_st77903$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/display_display_st77903$(PreprocessSuffix): ../../libs/display/display_st77903.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/display_display_st77903$(PreprocessSuffix) ../../libs/display/display_st77903.c

$(IntermediateDirectory)/display_display_tft128qm071$(ObjectSuffix): ../../libs/display/display_tft128qm071.c  
	$(CC) $(SourceSwitch) ../../libs/display/display_tft128qm071.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/display_display_tft128qm071$(ObjectSuffix) -MF$(IntermediateDirectory)/display_display_tft128qm071$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/display_display_tft128qm071$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/display_display_tft128qm071$(PreprocessSuffix): ../../libs/display/display_tft128qm071.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/display_display_tft128qm071$(PreprocessSuffix) ../../libs/display/display_tft128qm071.c

$(IntermediateDirectory)/libc_malloc$(ObjectSuffix): ../../libs/libc/malloc.c  
	$(CC) $(SourceSwitch) ../../libs/libc/malloc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/libc_malloc$(ObjectSuffix) -MF$(IntermediateDirectory)/libc_malloc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/libc_malloc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/libc_malloc$(PreprocessSuffix): ../../libs/libc/malloc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/libc_malloc$(PreprocessSuffix) ../../libs/libc/malloc.c

$(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix): ../../libs/libc/minilibc_port.c  
	$(CC) $(SourceSwitch) ../../libs/libc/minilibc_port.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix) -MF$(IntermediateDirectory)/libc_minilibc_port$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/libc_minilibc_port$(PreprocessSuffix): ../../libs/libc/minilibc_port.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/libc_minilibc_port$(PreprocessSuffix) ../../libs/libc/minilibc_port.c

$(IntermediateDirectory)/libc_printf$(ObjectSuffix): ../../libs/libc/printf.c  
	$(CC) $(SourceSwitch) ../../libs/libc/printf.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/libc_printf$(ObjectSuffix) -MF$(IntermediateDirectory)/libc_printf$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/libc_printf$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/libc_printf$(PreprocessSuffix): ../../libs/libc/printf.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/libc_printf$(PreprocessSuffix) ../../libs/libc/printf.c

$(IntermediateDirectory)/libc__init$(ObjectSuffix): ../../libs/libc/_init.c  
	$(CC) $(SourceSwitch) ../../libs/libc/_init.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/libc__init$(ObjectSuffix) -MF$(IntermediateDirectory)/libc__init$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/libc__init$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/libc__init$(PreprocessSuffix): ../../libs/libc/_init.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/libc__init$(PreprocessSuffix) ../../libs/libc/_init.c

$(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix): ../../libs/mm/dq_addlast.c  
	$(CC) $(SourceSwitch) ../../libs/mm/dq_addlast.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_dq_addlast$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_dq_addlast$(PreprocessSuffix): ../../libs/mm/dq_addlast.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_dq_addlast$(PreprocessSuffix) ../../libs/mm/dq_addlast.c

$(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix): ../../libs/mm/dq_rem.c  
	$(CC) $(SourceSwitch) ../../libs/mm/dq_rem.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_dq_rem$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_dq_rem$(PreprocessSuffix): ../../libs/mm/dq_rem.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_dq_rem$(PreprocessSuffix) ../../libs/mm/dq_rem.c

$(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix): ../../libs/mm/lib_mallinfo.c  
	$(CC) $(SourceSwitch) ../../libs/mm/lib_mallinfo.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_lib_mallinfo$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_lib_mallinfo$(PreprocessSuffix): ../../libs/mm/lib_mallinfo.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_lib_mallinfo$(PreprocessSuffix) ../../libs/mm/lib_mallinfo.c

$(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix): ../../libs/mm/mm_addfreechunk.c  
	$(CC) $(SourceSwitch) ../../libs/mm/mm_addfreechunk.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_addfreechunk$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_addfreechunk$(PreprocessSuffix): ../../libs/mm/mm_addfreechunk.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_addfreechunk$(PreprocessSuffix) ../../libs/mm/mm_addfreechunk.c

$(IntermediateDirectory)/mm_mm_free$(ObjectSuffix): ../../libs/mm/mm_free.c  
	$(CC) $(SourceSwitch) ../../libs/mm/mm_free.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_free$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_free$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_free$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_free$(PreprocessSuffix): ../../libs/mm/mm_free.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_free$(PreprocessSuffix) ../../libs/mm/mm_free.c

$(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix): ../../libs/mm/mm_initialize.c  
	$(CC) $(SourceSwitch) ../../libs/mm/mm_initialize.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_initialize$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_initialize$(PreprocessSuffix): ../../libs/mm/mm_initialize.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_initialize$(PreprocessSuffix) ../../libs/mm/mm_initialize.c

$(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix): ../../libs/mm/mm_leak.c  
	$(CC) $(SourceSwitch) ../../libs/mm/mm_leak.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_leak$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_leak$(PreprocessSuffix): ../../libs/mm/mm_leak.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_leak$(PreprocessSuffix) ../../libs/mm/mm_leak.c

$(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix): ../../libs/mm/mm_mallinfo.c  
	$(CC) $(SourceSwitch) ../../libs/mm/mm_mallinfo.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_mallinfo$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_mallinfo$(PreprocessSuffix): ../../libs/mm/mm_mallinfo.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_mallinfo$(PreprocessSuffix) ../../libs/mm/mm_mallinfo.c

$(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix): ../../libs/mm/mm_malloc.c  
	$(CC) $(SourceSwitch) ../../libs/mm/mm_malloc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_malloc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_malloc$(PreprocessSuffix): ../../libs/mm/mm_malloc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_malloc$(PreprocessSuffix) ../../libs/mm/mm_malloc.c

$(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix): ../../libs/mm/mm_size2ndx.c  
	$(CC) $(SourceSwitch) ../../libs/mm/mm_size2ndx.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_size2ndx$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_size2ndx$(PreprocessSuffix): ../../libs/mm/mm_size2ndx.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_size2ndx$(PreprocessSuffix) ../../libs/mm/mm_size2ndx.c

$(IntermediateDirectory)/ringbuffer_ringbuffer$(ObjectSuffix): ../../libs/ringbuffer/ringbuffer.c  
	$(CC) $(SourceSwitch) ../../libs/ringbuffer/ringbuffer.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ringbuffer_ringbuffer$(ObjectSuffix) -MF$(IntermediateDirectory)/ringbuffer_ringbuffer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ringbuffer_ringbuffer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ringbuffer_ringbuffer$(PreprocessSuffix): ../../libs/ringbuffer/ringbuffer.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ringbuffer_ringbuffer$(PreprocessSuffix) ../../libs/ringbuffer/ringbuffer.c

$(IntermediateDirectory)/soft_libc_soft_libc$(ObjectSuffix): ../../libs/soft_libc/soft_libc.c  
	$(CC) $(SourceSwitch) ../../libs/soft_libc/soft_libc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/soft_libc_soft_libc$(ObjectSuffix) -MF$(IntermediateDirectory)/soft_libc_soft_libc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/soft_libc_soft_libc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/soft_libc_soft_libc$(PreprocessSuffix): ../../libs/soft_libc/soft_libc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/soft_libc_soft_libc$(PreprocessSuffix) ../../libs/soft_libc/soft_libc.c

$(IntermediateDirectory)/syslog_syslog$(ObjectSuffix): ../../libs/syslog/syslog.c  
	$(CC) $(SourceSwitch) ../../libs/syslog/syslog.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/syslog_syslog$(ObjectSuffix) -MF$(IntermediateDirectory)/syslog_syslog$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/syslog_syslog$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/syslog_syslog$(PreprocessSuffix): ../../libs/syslog/syslog.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/syslog_syslog$(PreprocessSuffix) ../../libs/syslog/syslog.c

$(IntermediateDirectory)/FF13_diskio$(ObjectSuffix): ../../packages/FF13/diskio.c  
	$(CC) $(SourceSwitch) ../../packages/FF13/diskio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/FF13_diskio$(ObjectSuffix) -MF$(IntermediateDirectory)/FF13_diskio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/FF13_diskio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/FF13_diskio$(PreprocessSuffix): ../../packages/FF13/diskio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/FF13_diskio$(PreprocessSuffix) ../../packages/FF13/diskio.c

$(IntermediateDirectory)/FF13_ff$(ObjectSuffix): ../../packages/FF13/ff.c  
	$(CC) $(SourceSwitch) ../../packages/FF13/ff.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/FF13_ff$(ObjectSuffix) -MF$(IntermediateDirectory)/FF13_ff$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/FF13_ff$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/FF13_ff$(PreprocessSuffix): ../../packages/FF13/ff.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/FF13_ff$(PreprocessSuffix) ../../packages/FF13/ff.c

$(IntermediateDirectory)/FF13_ffsystem$(ObjectSuffix): ../../packages/FF13/ffsystem.c  
	$(CC) $(SourceSwitch) ../../packages/FF13/ffsystem.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/FF13_ffsystem$(ObjectSuffix) -MF$(IntermediateDirectory)/FF13_ffsystem$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/FF13_ffsystem$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/FF13_ffsystem$(PreprocessSuffix): ../../packages/FF13/ffsystem.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/FF13_ffsystem$(PreprocessSuffix) ../../packages/FF13/ffsystem.c

$(IntermediateDirectory)/FF13_ffunicode$(ObjectSuffix): ../../packages/FF13/ffunicode.c  
	$(CC) $(SourceSwitch) ../../packages/FF13/ffunicode.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/FF13_ffunicode$(ObjectSuffix) -MF$(IntermediateDirectory)/FF13_ffunicode$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/FF13_ffunicode$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/FF13_ffunicode$(PreprocessSuffix): ../../packages/FF13/ffunicode.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/FF13_ffunicode$(PreprocessSuffix) ../../packages/FF13/ffunicode.c

$(IntermediateDirectory)/DrawProgramBar_DrawProgramBar$(ObjectSuffix): Source/ahmi/DrawProgramBar/DrawProgramBar.c  
	$(CC) $(SourceSwitch) Source/ahmi/DrawProgramBar/DrawProgramBar.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/DrawProgramBar_DrawProgramBar$(ObjectSuffix) -MF$(IntermediateDirectory)/DrawProgramBar_DrawProgramBar$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/DrawProgramBar_DrawProgramBar$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/DrawProgramBar_DrawProgramBar$(PreprocessSuffix): Source/ahmi/DrawProgramBar/DrawProgramBar.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/DrawProgramBar_DrawProgramBar$(PreprocessSuffix) Source/ahmi/DrawProgramBar/DrawProgramBar.c

$(IntermediateDirectory)/DrawProgramBar_short_sourcebuffer$(ObjectSuffix): Source/ahmi/DrawProgramBar/short_sourcebuffer.c  
	$(CC) $(SourceSwitch) Source/ahmi/DrawProgramBar/short_sourcebuffer.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/DrawProgramBar_short_sourcebuffer$(ObjectSuffix) -MF$(IntermediateDirectory)/DrawProgramBar_short_sourcebuffer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/DrawProgramBar_short_sourcebuffer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/DrawProgramBar_short_sourcebuffer$(PreprocessSuffix): Source/ahmi/DrawProgramBar/short_sourcebuffer.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/DrawProgramBar_short_sourcebuffer$(PreprocessSuffix) Source/ahmi/DrawProgramBar/short_sourcebuffer.c

$(IntermediateDirectory)/ParseParameter_ParsePara$(ObjectSuffix): Source/ahmi/ParseParameter/ParsePara.c  
	$(CC) $(SourceSwitch) Source/ahmi/ParseParameter/ParsePara.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ParseParameter_ParsePara$(ObjectSuffix) -MF$(IntermediateDirectory)/ParseParameter_ParsePara$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ParseParameter_ParsePara$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ParseParameter_ParsePara$(PreprocessSuffix): Source/ahmi/ParseParameter/ParsePara.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ParseParameter_ParsePara$(PreprocessSuffix) Source/ahmi/ParseParameter/ParsePara.c

$(IntermediateDirectory)/core_crc_c1$(ObjectSuffix): Source/Crc/core/crc_c1.c  
	$(CC) $(SourceSwitch) Source/Crc/core/crc_c1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_crc_c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_crc_c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_crc_c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_crc_c1$(PreprocessSuffix): Source/Crc/core/crc_c1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_crc_c1$(PreprocessSuffix) Source/Crc/core/crc_c1.c

$(IntermediateDirectory)/core_em_c1$(ObjectSuffix): Source/EM/core/em_c1.c  
	$(CC) $(SourceSwitch) Source/EM/core/em_c1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_em_c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_em_c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_em_c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_em_c1$(PreprocessSuffix): Source/EM/core/em_c1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_em_c1$(PreprocessSuffix) Source/EM/core/em_c1.c

$(IntermediateDirectory)/core_srv_timer_c1$(ObjectSuffix): Source/Srv_Timer/core/srv_timer_c1.c  
	$(CC) $(SourceSwitch) Source/Srv_Timer/core/srv_timer_c1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_srv_timer_c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_srv_timer_c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_srv_timer_c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_srv_timer_c1$(PreprocessSuffix): Source/Srv_Timer/core/srv_timer_c1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_srv_timer_c1$(PreprocessSuffix) Source/Srv_Timer/core/srv_timer_c1.c

$(IntermediateDirectory)/core_upgrade_c1$(ObjectSuffix): Source/Upgrade/core/upgrade_c1.c  
	$(CC) $(SourceSwitch) Source/Upgrade/core/upgrade_c1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_upgrade_c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_upgrade_c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_upgrade_c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_upgrade_c1$(PreprocessSuffix): Source/Upgrade/core/upgrade_c1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_upgrade_c1$(PreprocessSuffix) Source/Upgrade/core/upgrade_c1.c

$(IntermediateDirectory)/core_upgrade_sm$(ObjectSuffix): Source/Upgrade/core/upgrade_sm.c  
	$(CC) $(SourceSwitch) Source/Upgrade/core/upgrade_sm.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_upgrade_sm$(ObjectSuffix) -MF$(IntermediateDirectory)/core_upgrade_sm$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_upgrade_sm$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_upgrade_sm$(PreprocessSuffix): Source/Upgrade/core/upgrade_sm.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_upgrade_sm$(PreprocessSuffix) Source/Upgrade/core/upgrade_sm.c

$(IntermediateDirectory)/core_upgrade_subsm$(ObjectSuffix): Source/Upgrade/core/upgrade_subsm.c  
	$(CC) $(SourceSwitch) Source/Upgrade/core/upgrade_subsm.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_upgrade_subsm$(ObjectSuffix) -MF$(IntermediateDirectory)/core_upgrade_subsm$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_upgrade_subsm$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_upgrade_subsm$(PreprocessSuffix): Source/Upgrade/core/upgrade_subsm.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_upgrade_subsm$(PreprocessSuffix) Source/Upgrade/core/upgrade_subsm.c

$(IntermediateDirectory)/core_upgrade_passive$(ObjectSuffix): Source/Upgrade/core/upgrade_passive.c  
	$(CC) $(SourceSwitch) Source/Upgrade/core/upgrade_passive.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_upgrade_passive$(ObjectSuffix) -MF$(IntermediateDirectory)/core_upgrade_passive$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_upgrade_passive$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_upgrade_passive$(PreprocessSuffix): Source/Upgrade/core/upgrade_passive.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_upgrade_passive$(PreprocessSuffix) Source/Upgrade/core/upgrade_passive.c

$(IntermediateDirectory)/core_upgrade_subsm_passive$(ObjectSuffix): Source/Upgrade/core/upgrade_subsm_passive.c  
	$(CC) $(SourceSwitch) Source/Upgrade/core/upgrade_subsm_passive.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_upgrade_subsm_passive$(ObjectSuffix) -MF$(IntermediateDirectory)/core_upgrade_subsm_passive$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_upgrade_subsm_passive$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_upgrade_subsm_passive$(PreprocessSuffix): Source/Upgrade/core/upgrade_subsm_passive.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_upgrade_subsm_passive$(PreprocessSuffix) Source/Upgrade/core/upgrade_subsm_passive.c

$(IntermediateDirectory)/core_upgrade_uart_platform$(ObjectSuffix): Source/Upgrade/core/upgrade_uart_platform.c  
	$(CC) $(SourceSwitch) Source/Upgrade/core/upgrade_uart_platform.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_upgrade_uart_platform$(ObjectSuffix) -MF$(IntermediateDirectory)/core_upgrade_uart_platform$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_upgrade_uart_platform$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_upgrade_uart_platform$(PreprocessSuffix): Source/Upgrade/core/upgrade_uart_platform.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_upgrade_uart_platform$(PreprocessSuffix) Source/Upgrade/core/upgrade_uart_platform.c

$(IntermediateDirectory)/core_upgrade_btld$(ObjectSuffix): Source/Upgrade/core/upgrade_btld.c  
	$(CC) $(SourceSwitch) Source/Upgrade/core/upgrade_btld.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_upgrade_btld$(ObjectSuffix) -MF$(IntermediateDirectory)/core_upgrade_btld$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_upgrade_btld$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_upgrade_btld$(PreprocessSuffix): Source/Upgrade/core/upgrade_btld.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_upgrade_btld$(PreprocessSuffix) Source/Upgrade/core/upgrade_btld.c

$(IntermediateDirectory)/NWL_DiagNWL$(ObjectSuffix): Source/CAN_Diag/NWL/DiagNWL.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/NWL/DiagNWL.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/NWL_DiagNWL$(ObjectSuffix) -MF$(IntermediateDirectory)/NWL_DiagNWL$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/NWL_DiagNWL$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/NWL_DiagNWL$(PreprocessSuffix): Source/CAN_Diag/NWL/DiagNWL.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/NWL_DiagNWL$(PreprocessSuffix) Source/CAN_Diag/NWL/DiagNWL.c

$(IntermediateDirectory)/SRV_DiagMgr$(ObjectSuffix): Source/CAN_Diag/SRV/DiagMgr.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/SRV/DiagMgr.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SRV_DiagMgr$(ObjectSuffix) -MF$(IntermediateDirectory)/SRV_DiagMgr$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SRV_DiagMgr$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SRV_DiagMgr$(PreprocessSuffix): Source/CAN_Diag/SRV/DiagMgr.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SRV_DiagMgr$(PreprocessSuffix) Source/CAN_Diag/SRV/DiagMgr.c

$(IntermediateDirectory)/SRV_DiagTbl$(ObjectSuffix): Source/CAN_Diag/SRV/DiagTbl.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/SRV/DiagTbl.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SRV_DiagTbl$(ObjectSuffix) -MF$(IntermediateDirectory)/SRV_DiagTbl$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SRV_DiagTbl$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SRV_DiagTbl$(PreprocessSuffix): Source/CAN_Diag/SRV/DiagTbl.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SRV_DiagTbl$(PreprocessSuffix) Source/CAN_Diag/SRV/DiagTbl.c

$(IntermediateDirectory)/SRV_DID$(ObjectSuffix): Source/CAN_Diag/SRV/DID.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/SRV/DID.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SRV_DID$(ObjectSuffix) -MF$(IntermediateDirectory)/SRV_DID$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SRV_DID$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SRV_DID$(PreprocessSuffix): Source/CAN_Diag/SRV/DID.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SRV_DID$(PreprocessSuffix) Source/CAN_Diag/SRV/DID.c

$(IntermediateDirectory)/SRV_Security$(ObjectSuffix): Source/CAN_Diag/SRV/Security.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/SRV/Security.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SRV_Security$(ObjectSuffix) -MF$(IntermediateDirectory)/SRV_Security$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SRV_Security$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SRV_Security$(PreprocessSuffix): Source/CAN_Diag/SRV/Security.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SRV_Security$(PreprocessSuffix) Source/CAN_Diag/SRV/Security.c

$(IntermediateDirectory)/SRV_Session$(ObjectSuffix): Source/CAN_Diag/SRV/Session.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/SRV/Session.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SRV_Session$(ObjectSuffix) -MF$(IntermediateDirectory)/SRV_Session$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SRV_Session$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SRV_Session$(PreprocessSuffix): Source/CAN_Diag/SRV/Session.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SRV_Session$(PreprocessSuffix) Source/CAN_Diag/SRV/Session.c

$(IntermediateDirectory)/adapt_cfg_autogen_ca$(ObjectSuffix): Source/Cfg/adapt/cfg_autogen_ca.c  
	$(CC) $(SourceSwitch) Source/Cfg/adapt/cfg_autogen_ca.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/adapt_cfg_autogen_ca$(ObjectSuffix) -MF$(IntermediateDirectory)/adapt_cfg_autogen_ca$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/adapt_cfg_autogen_ca$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/adapt_cfg_autogen_ca$(PreprocessSuffix): Source/Cfg/adapt/cfg_autogen_ca.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/adapt_cfg_autogen_ca$(PreprocessSuffix) Source/Cfg/adapt/cfg_autogen_ca.c

$(IntermediateDirectory)/adapt_cfg_ca$(ObjectSuffix): Source/Cfg/adapt/cfg_ca.c  
	$(CC) $(SourceSwitch) Source/Cfg/adapt/cfg_ca.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/adapt_cfg_ca$(ObjectSuffix) -MF$(IntermediateDirectory)/adapt_cfg_ca$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/adapt_cfg_ca$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/adapt_cfg_ca$(PreprocessSuffix): Source/Cfg/adapt/cfg_ca.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/adapt_cfg_ca$(PreprocessSuffix) Source/Cfg/adapt/cfg_ca.c

$(IntermediateDirectory)/core_cfg_c1$(ObjectSuffix): Source/Cfg/core/cfg_c1.c  
	$(CC) $(SourceSwitch) Source/Cfg/core/cfg_c1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_cfg_c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_cfg_c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_cfg_c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_cfg_c1$(PreprocessSuffix): Source/Cfg/core/cfg_c1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_cfg_c1$(PreprocessSuffix) Source/Cfg/core/cfg_c1.c

$(IntermediateDirectory)/adapter_csi_freertos$(ObjectSuffix): ../../csi_kernel/freertosv10.3.1/adapter/csi_freertos.c  
	$(CC) $(SourceSwitch) ../../csi_kernel/freertosv10.3.1/adapter/csi_freertos.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/adapter_csi_freertos$(ObjectSuffix) -MF$(IntermediateDirectory)/adapter_csi_freertos$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/adapter_csi_freertos$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/adapter_csi_freertos$(PreprocessSuffix): ../../csi_kernel/freertosv10.3.1/adapter/csi_freertos.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/adapter_csi_freertos$(PreprocessSuffix) ../../csi_kernel/freertosv10.3.1/adapter/csi_freertos.c

$(IntermediateDirectory)/Src_gc9002_hal_adc$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_adc.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_adc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_adc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_adc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_adc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_adc$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_adc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_adc$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_adc.c

$(IntermediateDirectory)/Src_gc9002_hal_ahmi$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_ahmi.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_ahmi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_ahmi$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_ahmi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_ahmi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_ahmi$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_ahmi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_ahmi$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_ahmi.c

$(IntermediateDirectory)/Src_gc9002_hal_ccm$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_ccm.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_ccm.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_ccm$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_ccm$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_ccm$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_ccm$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_ccm.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_ccm$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_ccm.c

$(IntermediateDirectory)/Src_gc9002_hal_crc32$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_crc32.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_crc32.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_crc32$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_crc32$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_crc32$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_crc32$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_crc32.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_crc32$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_crc32.c

$(IntermediateDirectory)/Src_gc9002_hal_display$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_display.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_display.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_display$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_display$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_display$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_display$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_display.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_display$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_display.c

$(IntermediateDirectory)/Src_gc9002_hal_dma$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_dma.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_dma.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_dma$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_dma$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_dma$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_dma$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_dma.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_dma$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_dma.c

$(IntermediateDirectory)/Src_gc9002_hal_eth$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_eth.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_eth.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_eth$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_eth$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_eth$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_eth$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_eth.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_eth$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_eth.c

$(IntermediateDirectory)/Src_gc9002_hal_extension$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_extension.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_extension.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_extension$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_extension$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_extension$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_extension$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_extension.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_extension$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_extension.c

$(IntermediateDirectory)/Src_gc9002_hal_hyper$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_hyper.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_hyper.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_hyper$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_hyper$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_hyper$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_hyper$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_hyper.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_hyper$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_hyper.c

$(IntermediateDirectory)/Src_gc9002_hal_pmu$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_pmu.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_pmu.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_pmu$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_pmu$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_pmu$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_pmu$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_pmu.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_pmu$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_pmu.c

$(IntermediateDirectory)/Src_gc9002_hal_rcc$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_rcc.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_rcc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_rcc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_rcc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_rcc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_rcc$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_rcc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_rcc$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_rcc.c

$(IntermediateDirectory)/Src_gc9002_hal_usb$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_usb.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_usb.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_usb$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_usb$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_usb$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_usb$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_usb.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_usb$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_usb.c

$(IntermediateDirectory)/Src_gc9002_hal_usbd$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_usbd.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_usbd.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_usbd$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_usbd$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_usbd$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_usbd$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_usbd.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_usbd$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_usbd.c

$(IntermediateDirectory)/Src_gc9002_hal_wdt$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_wdt.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_wdt.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_hal_wdt$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_hal_wdt$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_hal_wdt$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_hal_wdt$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_wdt.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_hal_wdt$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_hal_wdt.c

$(IntermediateDirectory)/Src_gc9002_ll_ahmi$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_ll_ahmi.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_ll_ahmi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_ll_ahmi$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_ll_ahmi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_ll_ahmi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_ll_ahmi$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_ll_ahmi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_ll_ahmi$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_ll_ahmi.c

$(IntermediateDirectory)/Src_gc9002_ll_display$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_ll_display.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc9002_ll_display.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc9002_ll_display$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc9002_ll_display$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc9002_ll_display$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc9002_ll_display$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc9002_ll_display.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc9002_ll_display$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc9002_ll_display.c

$(IntermediateDirectory)/Src_gc90xx_hal$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/gc90xx_hal.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/gc90xx_hal.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/gc90xx_hal.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/gc90xx_hal.c

$(IntermediateDirectory)/Src_mmalloc$(ObjectSuffix): ../../libs/GC9002_HAL_Driver/Src/mmalloc.c  
	$(CC) $(SourceSwitch) ../../libs/GC9002_HAL_Driver/Src/mmalloc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_mmalloc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_mmalloc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_mmalloc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_mmalloc$(PreprocessSuffix): ../../libs/GC9002_HAL_Driver/Src/mmalloc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_mmalloc$(PreprocessSuffix) ../../libs/GC9002_HAL_Driver/Src/mmalloc.c

$(IntermediateDirectory)/Src_gc90xx_hal_can$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_can.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_can.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_can$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_can$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_can$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_can$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_can.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_can$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_can.c

$(IntermediateDirectory)/Src_gc90xx_hal_canfd$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_canfd.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_canfd.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_canfd$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_canfd$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_canfd$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_canfd$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_canfd.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_canfd$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_canfd.c

$(IntermediateDirectory)/Src_gc90xx_hal_gpio$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_gpio.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_gpio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_gpio$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_gpio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_gpio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_gpio$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_gpio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_gpio$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_gpio.c

$(IntermediateDirectory)/Src_gc90xx_hal_i2c$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_i2c.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_i2c.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_i2c$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_i2c$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_i2c$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_i2c$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_i2c.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_i2c$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_i2c.c

$(IntermediateDirectory)/Src_gc90xx_hal_i2s$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_i2s.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_i2s.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_i2s$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_i2s$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_i2s$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_i2s$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_i2s.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_i2s$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_i2s.c

$(IntermediateDirectory)/Src_gc90xx_hal_mmc$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_mmc.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_mmc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_mmc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_mmc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_mmc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_mmc$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_mmc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_mmc$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_mmc.c

$(IntermediateDirectory)/Src_gc90xx_hal_rtc$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_rtc.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_rtc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_rtc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_rtc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_rtc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_rtc$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_rtc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_rtc$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_rtc.c

$(IntermediateDirectory)/Src_gc90xx_hal_sd$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_sd.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_sd.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_sd$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_sd$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_sd$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_sd$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_sd.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_sd$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_sd.c

$(IntermediateDirectory)/Src_gc90xx_hal_spi$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_spi.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_spi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_spi$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_spi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_spi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_spi$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_spi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_spi$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_spi.c

$(IntermediateDirectory)/Src_gc90xx_hal_tim$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_tim.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_tim.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_tim$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_tim$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_tim$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_tim$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_tim.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_tim$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_tim.c

$(IntermediateDirectory)/Src_gc90xx_hal_tim_ex$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_tim_ex.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_tim_ex.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_tim_ex$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_tim_ex$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_tim_ex$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_tim_ex$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_tim_ex.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_tim_ex$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_tim_ex.c

$(IntermediateDirectory)/Src_gc90xx_hal_uart$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_uart.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_uart.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_uart$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_uart$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_uart$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_uart$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_uart.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_uart$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_uart.c

$(IntermediateDirectory)/Src_gc90xx_hal_videocap$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_videocap.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_videocap.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_hal_videocap$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_hal_videocap$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_hal_videocap$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_hal_videocap$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_videocap.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_hal_videocap$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_hal_videocap.c

$(IntermediateDirectory)/Src_gc90xx_ll_qspi$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_ll_qspi.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_ll_qspi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_ll_qspi$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_ll_qspi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_ll_qspi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_ll_qspi$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_ll_qspi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_ll_qspi$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_ll_qspi.c

$(IntermediateDirectory)/Src_gc90xx_ll_sdmmc$(ObjectSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_ll_sdmmc.c  
	$(CC) $(SourceSwitch) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_ll_sdmmc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Src_gc90xx_ll_sdmmc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_gc90xx_ll_sdmmc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Src_gc90xx_ll_sdmmc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Src_gc90xx_ll_sdmmc$(PreprocessSuffix): ../../libs/GC90xx_HAL_Driver/Src/gc90xx_ll_sdmmc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Src_gc90xx_ll_sdmmc$(PreprocessSuffix) ../../libs/GC90xx_HAL_Driver/Src/gc90xx_ll_sdmmc.c

$(IntermediateDirectory)/core_usbd_core$(ObjectSuffix): ../../packages/CherryUSB/core/usbd_core.c  
	$(CC) $(SourceSwitch) ../../packages/CherryUSB/core/usbd_core.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_usbd_core$(ObjectSuffix) -MF$(IntermediateDirectory)/core_usbd_core$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_usbd_core$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_usbd_core$(PreprocessSuffix): ../../packages/CherryUSB/core/usbd_core.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_usbd_core$(PreprocessSuffix) ../../packages/CherryUSB/core/usbd_core.c

$(IntermediateDirectory)/core_usbh_core$(ObjectSuffix): ../../packages/CherryUSB/core/usbh_core.c  
	$(CC) $(SourceSwitch) ../../packages/CherryUSB/core/usbh_core.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_usbh_core$(ObjectSuffix) -MF$(IntermediateDirectory)/core_usbh_core$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_usbh_core$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_usbh_core$(PreprocessSuffix): ../../packages/CherryUSB/core/usbh_core.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_usbh_core$(PreprocessSuffix) ../../packages/CherryUSB/core/usbh_core.c

$(IntermediateDirectory)/osal_usb_osal_freertos$(ObjectSuffix): ../../packages/CherryUSB/osal/usb_osal_freertos.c  
	$(CC) $(SourceSwitch) ../../packages/CherryUSB/osal/usb_osal_freertos.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/osal_usb_osal_freertos$(ObjectSuffix) -MF$(IntermediateDirectory)/osal_usb_osal_freertos$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/osal_usb_osal_freertos$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/osal_usb_osal_freertos$(PreprocessSuffix): ../../packages/CherryUSB/osal/usb_osal_freertos.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/osal_usb_osal_freertos$(PreprocessSuffix) ../../packages/CherryUSB/osal/usb_osal_freertos.c

$(IntermediateDirectory)/src_gc90x_ahmi$(ObjectSuffix): Source/ahmi/GClib/src/gc90x_ahmi.c  
	$(CC) $(SourceSwitch) Source/ahmi/GClib/src/gc90x_ahmi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/src_gc90x_ahmi$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gc90x_ahmi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/src_gc90x_ahmi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/src_gc90x_ahmi$(PreprocessSuffix): Source/ahmi/GClib/src/gc90x_ahmi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/src_gc90x_ahmi$(PreprocessSuffix) Source/ahmi/GClib/src/gc90x_ahmi.c

$(IntermediateDirectory)/src_gc90x_cache$(ObjectSuffix): Source/ahmi/GClib/src/gc90x_cache.c  
	$(CC) $(SourceSwitch) Source/ahmi/GClib/src/gc90x_cache.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/src_gc90x_cache$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gc90x_cache$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/src_gc90x_cache$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/src_gc90x_cache$(PreprocessSuffix): Source/ahmi/GClib/src/gc90x_cache.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/src_gc90x_cache$(PreprocessSuffix) Source/ahmi/GClib/src/gc90x_cache.c

$(IntermediateDirectory)/src_gc90x_display$(ObjectSuffix): Source/ahmi/GClib/src/gc90x_display.cpp  
	$(CXX) $(SourceSwitch) Source/ahmi/GClib/src/gc90x_display.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/src_gc90x_display$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gc90x_display$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/src_gc90x_display$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/src_gc90x_display$(PreprocessSuffix): Source/ahmi/GClib/src/gc90x_display.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/src_gc90x_display$(PreprocessSuffix) Source/ahmi/GClib/src/gc90x_display.cpp

$(IntermediateDirectory)/Repro_Repro$(ObjectSuffix): Source/CAN_Diag/SRV/Repro/Repro.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/SRV/Repro/Repro.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Repro_Repro$(ObjectSuffix) -MF$(IntermediateDirectory)/Repro_Repro$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Repro_Repro$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Repro_Repro$(PreprocessSuffix): Source/CAN_Diag/SRV/Repro/Repro.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Repro_Repro$(PreprocessSuffix) Source/CAN_Diag/SRV/Repro/Repro.c

$(IntermediateDirectory)/SIDSource_SID_10$(ObjectSuffix): Source/CAN_Diag/SRV/SIDSource/SID_10.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/SRV/SIDSource/SID_10.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SIDSource_SID_10$(ObjectSuffix) -MF$(IntermediateDirectory)/SIDSource_SID_10$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SIDSource_SID_10$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SIDSource_SID_10$(PreprocessSuffix): Source/CAN_Diag/SRV/SIDSource/SID_10.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SIDSource_SID_10$(PreprocessSuffix) Source/CAN_Diag/SRV/SIDSource/SID_10.c

$(IntermediateDirectory)/SIDSource_SID_11$(ObjectSuffix): Source/CAN_Diag/SRV/SIDSource/SID_11.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/SRV/SIDSource/SID_11.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SIDSource_SID_11$(ObjectSuffix) -MF$(IntermediateDirectory)/SIDSource_SID_11$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SIDSource_SID_11$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SIDSource_SID_11$(PreprocessSuffix): Source/CAN_Diag/SRV/SIDSource/SID_11.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SIDSource_SID_11$(PreprocessSuffix) Source/CAN_Diag/SRV/SIDSource/SID_11.c

$(IntermediateDirectory)/SIDSource_SID_28$(ObjectSuffix): Source/CAN_Diag/SRV/SIDSource/SID_28.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/SRV/SIDSource/SID_28.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SIDSource_SID_28$(ObjectSuffix) -MF$(IntermediateDirectory)/SIDSource_SID_28$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SIDSource_SID_28$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SIDSource_SID_28$(PreprocessSuffix): Source/CAN_Diag/SRV/SIDSource/SID_28.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SIDSource_SID_28$(PreprocessSuffix) Source/CAN_Diag/SRV/SIDSource/SID_28.c

$(IntermediateDirectory)/Source_croutine$(ObjectSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/croutine.c  
	$(CC) $(SourceSwitch) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/croutine.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Source_croutine$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_croutine$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Source_croutine$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Source_croutine$(PreprocessSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/croutine.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Source_croutine$(PreprocessSuffix) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/croutine.c

$(IntermediateDirectory)/Source_event_groups$(ObjectSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/event_groups.c  
	$(CC) $(SourceSwitch) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/event_groups.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Source_event_groups$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_event_groups$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Source_event_groups$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Source_event_groups$(PreprocessSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/event_groups.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Source_event_groups$(PreprocessSuffix) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/event_groups.c

$(IntermediateDirectory)/Source_list$(ObjectSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/list.c  
	$(CC) $(SourceSwitch) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/list.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Source_list$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_list$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Source_list$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Source_list$(PreprocessSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/list.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Source_list$(PreprocessSuffix) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/list.c

$(IntermediateDirectory)/Source_queue$(ObjectSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/queue.c  
	$(CC) $(SourceSwitch) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/queue.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Source_queue$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_queue$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Source_queue$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Source_queue$(PreprocessSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/queue.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Source_queue$(PreprocessSuffix) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/queue.c

$(IntermediateDirectory)/Source_stream_buffer$(ObjectSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/stream_buffer.c  
	$(CC) $(SourceSwitch) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/stream_buffer.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Source_stream_buffer$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_stream_buffer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Source_stream_buffer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Source_stream_buffer$(PreprocessSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/stream_buffer.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Source_stream_buffer$(PreprocessSuffix) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/stream_buffer.c

$(IntermediateDirectory)/Source_tasks$(ObjectSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/tasks.c  
	$(CC) $(SourceSwitch) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/tasks.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Source_tasks$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_tasks$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Source_tasks$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Source_tasks$(PreprocessSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/tasks.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Source_tasks$(PreprocessSuffix) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/tasks.c

$(IntermediateDirectory)/Source_timers$(ObjectSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/timers.c  
	$(CC) $(SourceSwitch) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/timers.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Source_timers$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_timers$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Source_timers$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Source_timers$(PreprocessSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/timers.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Source_timers$(PreprocessSuffix) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/timers.c

$(IntermediateDirectory)/cdc_usbd_cdc$(ObjectSuffix): ../../packages/CherryUSB/class/cdc/usbd_cdc.c  
	$(CC) $(SourceSwitch) ../../packages/CherryUSB/class/cdc/usbd_cdc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/cdc_usbd_cdc$(ObjectSuffix) -MF$(IntermediateDirectory)/cdc_usbd_cdc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/cdc_usbd_cdc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/cdc_usbd_cdc$(PreprocessSuffix): ../../packages/CherryUSB/class/cdc/usbd_cdc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/cdc_usbd_cdc$(PreprocessSuffix) ../../packages/CherryUSB/class/cdc/usbd_cdc.c

$(IntermediateDirectory)/cdc_usbh_cdc_acm$(ObjectSuffix): ../../packages/CherryUSB/class/cdc/usbh_cdc_acm.c  
	$(CC) $(SourceSwitch) ../../packages/CherryUSB/class/cdc/usbh_cdc_acm.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/cdc_usbh_cdc_acm$(ObjectSuffix) -MF$(IntermediateDirectory)/cdc_usbh_cdc_acm$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/cdc_usbh_cdc_acm$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/cdc_usbh_cdc_acm$(PreprocessSuffix): ../../packages/CherryUSB/class/cdc/usbh_cdc_acm.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/cdc_usbh_cdc_acm$(PreprocessSuffix) ../../packages/CherryUSB/class/cdc/usbh_cdc_acm.c

$(IntermediateDirectory)/hub_usbh_hub$(ObjectSuffix): ../../packages/CherryUSB/class/hub/usbh_hub.c  
	$(CC) $(SourceSwitch) ../../packages/CherryUSB/class/hub/usbh_hub.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/hub_usbh_hub$(ObjectSuffix) -MF$(IntermediateDirectory)/hub_usbh_hub$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/hub_usbh_hub$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/hub_usbh_hub$(PreprocessSuffix): ../../packages/CherryUSB/class/hub/usbh_hub.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/hub_usbh_hub$(PreprocessSuffix) ../../packages/CherryUSB/class/hub/usbh_hub.c

$(IntermediateDirectory)/msc_usbh_msc$(ObjectSuffix): ../../packages/CherryUSB/class/msc/usbh_msc.c  
	$(CC) $(SourceSwitch) ../../packages/CherryUSB/class/msc/usbh_msc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/msc_usbh_msc$(ObjectSuffix) -MF$(IntermediateDirectory)/msc_usbh_msc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/msc_usbh_msc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/msc_usbh_msc$(PreprocessSuffix): ../../packages/CherryUSB/class/msc/usbh_msc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/msc_usbh_msc$(PreprocessSuffix) ../../packages/CherryUSB/class/msc/usbh_msc.c

$(IntermediateDirectory)/musb_usb_dc_musb$(ObjectSuffix): ../../packages/CherryUSB/port/musb/usb_dc_musb.c  
	$(CC) $(SourceSwitch) ../../packages/CherryUSB/port/musb/usb_dc_musb.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/musb_usb_dc_musb$(ObjectSuffix) -MF$(IntermediateDirectory)/musb_usb_dc_musb$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/musb_usb_dc_musb$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/musb_usb_dc_musb$(PreprocessSuffix): ../../packages/CherryUSB/port/musb/usb_dc_musb.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/musb_usb_dc_musb$(PreprocessSuffix) ../../packages/CherryUSB/port/musb/usb_dc_musb.c

$(IntermediateDirectory)/musb_usb_hc_musb$(ObjectSuffix): ../../packages/CherryUSB/port/musb/usb_hc_musb.c  
	$(CC) $(SourceSwitch) ../../packages/CherryUSB/port/musb/usb_hc_musb.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/musb_usb_hc_musb$(ObjectSuffix) -MF$(IntermediateDirectory)/musb_usb_hc_musb$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/musb_usb_hc_musb$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/musb_usb_hc_musb$(PreprocessSuffix): ../../packages/CherryUSB/port/musb/usb_hc_musb.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/musb_usb_hc_musb$(PreprocessSuffix) ../../packages/CherryUSB/port/musb/usb_hc_musb.c

$(IntermediateDirectory)/SID_XTD_SID_64$(ObjectSuffix): Source/CAN_Diag/SRV/SIDSource/SID_XTD/SID_64.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/SRV/SIDSource/SID_XTD/SID_64.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SID_XTD_SID_64$(ObjectSuffix) -MF$(IntermediateDirectory)/SID_XTD_SID_64$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SID_XTD_SID_64$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SID_XTD_SID_64$(PreprocessSuffix): Source/CAN_Diag/SRV/SIDSource/SID_XTD/SID_64.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SID_XTD_SID_64$(PreprocessSuffix) Source/CAN_Diag/SRV/SIDSource/SID_XTD/SID_64.c

$(IntermediateDirectory)/SID_XTD_SID_66$(ObjectSuffix): Source/CAN_Diag/SRV/SIDSource/SID_XTD/SID_66.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/SRV/SIDSource/SID_XTD/SID_66.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SID_XTD_SID_66$(ObjectSuffix) -MF$(IntermediateDirectory)/SID_XTD_SID_66$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SID_XTD_SID_66$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SID_XTD_SID_66$(PreprocessSuffix): Source/CAN_Diag/SRV/SIDSource/SID_XTD/SID_66.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SID_XTD_SID_66$(PreprocessSuffix) Source/CAN_Diag/SRV/SIDSource/SID_XTD/SID_66.c

$(IntermediateDirectory)/SID_XTD_SID_67$(ObjectSuffix): Source/CAN_Diag/SRV/SIDSource/SID_XTD/SID_67.c  
	$(CC) $(SourceSwitch) Source/CAN_Diag/SRV/SIDSource/SID_XTD/SID_67.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SID_XTD_SID_67$(ObjectSuffix) -MF$(IntermediateDirectory)/SID_XTD_SID_67$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SID_XTD_SID_67$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SID_XTD_SID_67$(PreprocessSuffix): Source/CAN_Diag/SRV/SIDSource/SID_XTD/SID_67.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SID_XTD_SID_67$(PreprocessSuffix) Source/CAN_Diag/SRV/SIDSource/SID_XTD/SID_67.c

$(IntermediateDirectory)/Source_startup$(ObjectSuffix): ../../libs/CMSIS/Device/GC9002/Source/startup.S  
	$(AS) $(SourceSwitch) ../../libs/CMSIS/Device/GC9002/Source/startup.S $(ASFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Source_startup$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_startup$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Source_startup$(ObjectSuffix) $(IncludeAPath) $(IncludePackagePath)
Lst/Source_startup$(PreprocessSuffix): ../../libs/CMSIS/Device/GC9002/Source/startup.S
	$(CC) $(CFLAGS)$(IncludeAPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Source_startup$(PreprocessSuffix) ../../libs/CMSIS/Device/GC9002/Source/startup.S

$(IntermediateDirectory)/Source_system$(ObjectSuffix): ../../libs/CMSIS/Device/GC9002/Source/system.c  
	$(CC) $(SourceSwitch) ../../libs/CMSIS/Device/GC9002/Source/system.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Source_system$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_system$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Source_system$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Source_system$(PreprocessSuffix): ../../libs/CMSIS/Device/GC9002/Source/system.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Source_system$(PreprocessSuffix) ../../libs/CMSIS/Device/GC9002/Source/system.c

$(IntermediateDirectory)/Source_trap_c$(ObjectSuffix): ../../libs/CMSIS/Device/GC9002/Source/trap_c.c  
	$(CC) $(SourceSwitch) ../../libs/CMSIS/Device/GC9002/Source/trap_c.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Source_trap_c$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_trap_c$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Source_trap_c$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Source_trap_c$(PreprocessSuffix): ../../libs/CMSIS/Device/GC9002/Source/trap_c.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Source_trap_c$(PreprocessSuffix) ../../libs/CMSIS/Device/GC9002/Source/trap_c.c

$(IntermediateDirectory)/Source_vectors$(ObjectSuffix): ../../libs/CMSIS/Device/GC9002/Source/vectors.S  
	$(AS) $(SourceSwitch) ../../libs/CMSIS/Device/GC9002/Source/vectors.S $(ASFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Source_vectors$(ObjectSuffix) -MF$(IntermediateDirectory)/Source_vectors$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Source_vectors$(ObjectSuffix) $(IncludeAPath) $(IncludePackagePath)
Lst/Source_vectors$(PreprocessSuffix): ../../libs/CMSIS/Device/GC9002/Source/vectors.S
	$(CC) $(CFLAGS)$(IncludeAPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Source_vectors$(PreprocessSuffix) ../../libs/CMSIS/Device/GC9002/Source/vectors.S

$(IntermediateDirectory)/MemMang_heap_4$(ObjectSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/MemMang/heap_4.c  
	$(CC) $(SourceSwitch) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/MemMang/heap_4.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/MemMang_heap_4$(ObjectSuffix) -MF$(IntermediateDirectory)/MemMang_heap_4$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/MemMang_heap_4$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/MemMang_heap_4$(PreprocessSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/MemMang/heap_4.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/MemMang_heap_4$(PreprocessSuffix) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/MemMang/heap_4.c

$(IntermediateDirectory)/riscv_port$(ObjectSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/port.c  
	$(CC) $(SourceSwitch) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/port.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/riscv_port$(ObjectSuffix) -MF$(IntermediateDirectory)/riscv_port$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/riscv_port$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/riscv_port$(PreprocessSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/port.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/riscv_port$(PreprocessSuffix) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/port.c

$(IntermediateDirectory)/riscv_portASM$(ObjectSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/portASM.S  
	$(AS) $(SourceSwitch) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/portASM.S $(ASFLAGS) -MMD -MP -MT$(IntermediateDirectory)/riscv_portASM$(ObjectSuffix) -MF$(IntermediateDirectory)/riscv_portASM$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/riscv_portASM$(ObjectSuffix) $(IncludeAPath) $(IncludePackagePath)
Lst/riscv_portASM$(PreprocessSuffix): ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/portASM.S
	$(CC) $(CFLAGS)$(IncludeAPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/riscv_portASM$(PreprocessSuffix) ../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/portASM.S


-include $(IntermediateDirectory)/*$(DependSuffix)
