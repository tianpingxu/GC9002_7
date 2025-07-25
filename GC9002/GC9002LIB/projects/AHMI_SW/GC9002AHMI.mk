##
## Auto Generated makefile by CDK
## Do not modify this file, and any manual changes will be erased!!!   
##
## BuildSet
ProjectName            :=GC9002AHMI
ConfigurationName      :=BuildSet
WorkspacePath          :=./
ProjectPath            :=./
IntermediateDirectory  :=Obj
OutDir                 :=$(IntermediateDirectory)
User                   :=xuyi
Date                   :=02/07/2025
CDKPath                :=../../../../../C-Sky/CDK
ToolchainPath          :=D:/C-Sky/CDKRepo/Toolchain/XTGccElfNewlib/V2.6.1/R/
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
PreprocessOnlyDisableLineSwitch   :=-P
ObjectsFileList        :=GC9002AHMI.txt
MakeDirCommand         :=mkdir
LinkOptions            := -mcpu=e902m  --specs=nosys.specs  -nostartfiles -Wl,--gc-sections -T"$(ProjectPath)/../../linkscripts/gcc_ahmi_CodeFlash_RamHyper.ld"
LinkOtherFlagsOption   :=-Wl,-zmax-page-size=1024 -Wl,-Map=$(ProjectPath)/Lst/$(OutputFile).map 
IncludePackagePath     :=
IncludeCPath           := $(IncludeSwitch). $(IncludeSwitch)../../../../CommonSource/AHMI $(IncludeSwitch)../../../../CommonSource/AHMI/Config $(IncludeSwitch)../../../../CommonSource/AHMI/Driver $(IncludeSwitch)../../../../CommonSource/AHMI/Player/adapt $(IncludeSwitch)../../../../CommonSource/AHMI/Player/core $(IncludeSwitch)../../../../CommonSource/AHMI/Simulator $(IncludeSwitch)../../../../CommonSource/AHMI/Touch $(IncludeSwitch)../../../../CommonSource/AHMI/Widget $(IncludeSwitch)../../../../CommonSource/AHMI/csi_ahmi $(IncludeSwitch)../../../../CommonSource/AHMI/sxgl/include $(IncludeSwitch)../../../../CommonSource/AHMI/testFunc $(IncludeSwitch)../../../../CommonSource/EM/adapt $(IncludeSwitch)../../../../CommonSource/EM/core $(IncludeSwitch)../../../../CommonSource/Main $(IncludeSwitch)../../../../CommonSource/SM/core $(IncludeSwitch)../../../../CommonSource/csi_hal/csi_hal_core $(IncludeSwitch)../../../../CommonSource/csi_hal/csi_hal_driver $(IncludeSwitch)../../../../CommonSource/csi_hal/csi_hal_isr $(IncludeSwitch)../../../../CommonSource/qrencode-3.4.4 $(IncludeSwitch)../../ahmi/GClib/inc $(IncludeSwitch)../../ahmi/MiddleWare $(IncludeSwitch)../../ahmi/ParseParameter $(IncludeSwitch)../../ahmi/Touch $(IncludeSwitch)../../ahmi/app $(IncludeSwitch)../../ahmi/itm/adapt $(IncludeSwitch)../../ahmi/itm/core $(IncludeSwitch)../../ahmi/sbm/adapt $(IncludeSwitch)../../ahmi/sbm/core $(IncludeSwitch)../../application $(IncludeSwitch)../../application/sample $(IncludeSwitch)../../application/template $(IncludeSwitch)../../board $(IncludeSwitch)../../csi_core/include $(IncludeSwitch)../../csi_driver/include $(IncludeSwitch)../../csi_driver/smartl_rv32/include $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/include $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/chip_specific_extensions/thead_rv32 $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/include $(IncludeSwitch)../../csi_kernel/include $(IncludeSwitch)../../csi_kernel/include/ $(IncludeSwitch)../../libs/CMSIS/Device/GC9002/Include $(IncludeSwitch)../../libs/GC9002_HAL_Driver/Inc $(IncludeSwitch)../../libs/GC90xx_HAL_Driver/Inc $(IncludeSwitch)../../libs/display $(IncludeSwitch)../../libs/include $(IncludeSwitch)../../libs/include/ringbuffer $(IncludeSwitch)../../libs/include/sys $(IncludeSwitch)../../libs/soft_libc $(IncludeSwitch)../../packages/CherryUSB $(IncludeSwitch)../../packages/CherryUSB/class/cdc $(IncludeSwitch)../../packages/CherryUSB/class/hub $(IncludeSwitch)../../packages/CherryUSB/class/msc $(IncludeSwitch)../../packages/CherryUSB/common $(IncludeSwitch)../../packages/CherryUSB/core $(IncludeSwitch)../../packages/CherryUSB/osal $(IncludeSwitch)../../packages/CherryUSB/port/musb $(IncludeSwitch)../../packages/FF13 $(IncludeSwitch)../../packages/at/at_socket $(IncludeSwitch)../../packages/at/config $(IncludeSwitch)../../packages/at/hal $(IncludeSwitch)../../packages/at/include $(IncludeSwitch)../../packages/at_device-v2.0.4/at_main $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11 $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/basic_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/ble_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/mqtt_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/ota_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/tcpip_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/wifi_command $(IncludeSwitch)../../packages/at_device-v2.0.4/inc $(IncludeSwitch)../../packages/netdev/include $(IncludeSwitch)../../packages/netdev/include/arpa $(IncludeSwitch)../../packages/rt $(IncludeSwitch)../../packages/sal_socket/impl $(IncludeSwitch)../../packages/sal_socket/include $(IncludeSwitch)../../packages/sal_socket/include/dfs_net $(IncludeSwitch)../../packages/sal_socket/include/dfs_net/sys_select/sys $(IncludeSwitch)../../packages/sal_socket/include/socket $(IncludeSwitch)../../packages/sal_socket/include/socket/netinet $(IncludeSwitch)../../packages/sal_socket/include/socket/sys_socket/sys $(IncludeSwitch)../../packages/uip/port $(IncludeSwitch)../../packages/uip/src $(IncludeSwitch)FreeRTOSConfig $(IncludeSwitch)CustomerApp $(IncludeSwitch)Source/FreeRTOSConfig $(IncludeSwitch)Source/ahmi/GClib/inc $(IncludeSwitch)Source/ahmi/MiddleWare $(IncludeSwitch)Source/ahmi/ParseParameter $(IncludeSwitch)Source/ahmi/SBBatchDraw $(IncludeSwitch)Source/ahmi/Touch $(IncludeSwitch)Source/ahmi/adcshare $(IncludeSwitch)Source/ahmi/adcshare/adapt $(IncludeSwitch)Source/ahmi/adcshare/core $(IncludeSwitch)Source/ahmi/app $(IncludeSwitch)Source/ahmi/com/adapt $(IncludeSwitch)Source/ahmi/com/core $(IncludeSwitch)Source/ahmi/itm/adapt $(IncludeSwitch)Source/ahmi/itm/core $(IncludeSwitch)Source/ahmi/sbm/adapt $(IncludeSwitch)Source/ahmi/sbm/core $(IncludeSwitch)Source/csi_config $(IncludeSwitch)Source/hal_config $(IncludeSwitch)../../../../CommonSource/AHMI/sxgl/include/ $(IncludeSwitch)../Bootloader/Source/Cfg/adapt/ $(IncludeSwitch)../Bootloader/Source/Cfg/core/  
IncludeAPath           := $(IncludeSwitch). $(IncludeSwitch)../../../../CommonSource/AHMI $(IncludeSwitch)../../../../CommonSource/AHMI/Config $(IncludeSwitch)../../../../CommonSource/AHMI/Driver $(IncludeSwitch)../../../../CommonSource/AHMI/Player/adapt $(IncludeSwitch)../../../../CommonSource/AHMI/Player/core $(IncludeSwitch)../../../../CommonSource/AHMI/Simulator $(IncludeSwitch)../../../../CommonSource/AHMI/Touch $(IncludeSwitch)../../../../CommonSource/AHMI/Widget $(IncludeSwitch)../../../../CommonSource/AHMI/csi_ahmi $(IncludeSwitch)../../../../CommonSource/AHMI/sxgl/include $(IncludeSwitch)../../../../CommonSource/AHMI/testFunc $(IncludeSwitch)../../../../CommonSource/EM/adapt $(IncludeSwitch)../../../../CommonSource/EM/core $(IncludeSwitch)../../../../CommonSource/Main $(IncludeSwitch)../../../../CommonSource/SM/core $(IncludeSwitch)../../../../CommonSource/csi_hal/csi_hal_core $(IncludeSwitch)../../../../CommonSource/csi_hal/csi_hal_driver $(IncludeSwitch)../../../../CommonSource/csi_hal/csi_hal_isr $(IncludeSwitch)../../../../CommonSource/qrencode-3.4.4 $(IncludeSwitch)../../ahmi/GClib/inc $(IncludeSwitch)../../ahmi/MiddleWare $(IncludeSwitch)../../ahmi/ParseParameter $(IncludeSwitch)../../ahmi/Touch $(IncludeSwitch)../../ahmi/app $(IncludeSwitch)../../ahmi/itm/adapt $(IncludeSwitch)../../ahmi/itm/core $(IncludeSwitch)../../ahmi/sbm/adapt $(IncludeSwitch)../../ahmi/sbm/core $(IncludeSwitch)../../application $(IncludeSwitch)../../application/sample $(IncludeSwitch)../../application/template $(IncludeSwitch)../../board $(IncludeSwitch)../../csi_core/include $(IncludeSwitch)../../csi_driver/include $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/include $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/FreeRTOS/Source/portable/GCC/riscv/chip_specific_extensions/thead_rv32 $(IncludeSwitch)../../csi_kernel/freertosv10.3.1/include $(IncludeSwitch)../../csi_kernel/include $(IncludeSwitch)../../libs/CMSIS/Device/GC9002/Include $(IncludeSwitch)../../libs/GC9002_HAL_Driver/Inc $(IncludeSwitch)../../libs/GC90xx_HAL_Driver/Inc $(IncludeSwitch)../../libs/display $(IncludeSwitch)../../libs/include $(IncludeSwitch)../../libs/include/ringbuffer $(IncludeSwitch)../../libs/include/sys $(IncludeSwitch)../../libs/soft_libc $(IncludeSwitch)../../packages/CherryUSB $(IncludeSwitch)../../packages/CherryUSB/class/cdc $(IncludeSwitch)../../packages/CherryUSB/class/hub $(IncludeSwitch)../../packages/CherryUSB/class/msc $(IncludeSwitch)../../packages/CherryUSB/common $(IncludeSwitch)../../packages/CherryUSB/core $(IncludeSwitch)../../packages/CherryUSB/osal $(IncludeSwitch)../../packages/CherryUSB/port/musb $(IncludeSwitch)../../packages/FF13 $(IncludeSwitch)../../packages/at/at_socket $(IncludeSwitch)../../packages/at/config $(IncludeSwitch)../../packages/at/hal $(IncludeSwitch)../../packages/at/include $(IncludeSwitch)../../packages/at_device-v2.0.4/at_main $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11 $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/basic_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/ble_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/mqtt_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/ota_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/tcpip_command $(IncludeSwitch)../../packages/at_device-v2.0.4/class/e103w11/wifi_command $(IncludeSwitch)../../packages/at_device-v2.0.4/inc $(IncludeSwitch)../../packages/netdev/include $(IncludeSwitch)../../packages/netdev/include/arpa $(IncludeSwitch)../../packages/rt $(IncludeSwitch)../../packages/sal_socket/impl $(IncludeSwitch)../../packages/sal_socket/include $(IncludeSwitch)../../packages/sal_socket/include/dfs_net $(IncludeSwitch)../../packages/sal_socket/include/dfs_net/sys_select/sys $(IncludeSwitch)../../packages/sal_socket/include/socket $(IncludeSwitch)../../packages/sal_socket/include/socket/netinet $(IncludeSwitch)../../packages/sal_socket/include/socket/sys_socket/sys $(IncludeSwitch)../../packages/uip/port $(IncludeSwitch)../../packages/uip/src $(IncludeSwitch)FreeRTOSConfig $(IncludeSwitch)CustomerApp $(IncludeSwitch)Source/FreeRTOSConfig $(IncludeSwitch)Source/ahmi/GClib/inc $(IncludeSwitch)Source/ahmi/MiddleWare $(IncludeSwitch)Source/ahmi/ParseParameter $(IncludeSwitch)Source/ahmi/SBBatchDraw $(IncludeSwitch)Source/ahmi/Touch $(IncludeSwitch)Source/ahmi/adcshare $(IncludeSwitch)Source/ahmi/adcshare/adapt $(IncludeSwitch)Source/ahmi/adcshare/core $(IncludeSwitch)Source/ahmi/app $(IncludeSwitch)Source/ahmi/com/adapt $(IncludeSwitch)Source/ahmi/com/core $(IncludeSwitch)Source/ahmi/itm/adapt $(IncludeSwitch)Source/ahmi/itm/core $(IncludeSwitch)Source/ahmi/sbm/adapt $(IncludeSwitch)Source/ahmi/sbm/core $(IncludeSwitch)Source/csi_config $(IncludeSwitch)Source/hal_config  
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
CXXFLAGS := -mcpu=e902m   $(PreprocessorSwitch)USE_HAL_DRIVER  $(PreprocessorSwitch)CHIP_GC9002   -O2  -g3  -Wall -ffunction-sections -fdata-sections -Wall -Wno-unused-function -Wno-unused-but-set-variable -Wno-comment -Wno-unused-variable -Wno-maybe-uninitialized -fno-exceptions 
CFLAGS   := -mcpu=e902m   $(PreprocessorSwitch)USE_HAL_DRIVER  $(PreprocessorSwitch)CHIP_GC9002   -O2  -g3  -Wall -ffunction-sections -fdata-sections -Wall -Wno-unused-function -Wno-unused-but-set-variable -Wno-comment -Wno-unused-variable -Wno-maybe-uninitialized -fno-exceptions 
ASFLAGS  := -mcpu=e902m     -Wa,-gdwarf-2    
PreprocessFlags  := -mcpu=e902m   $(PreprocessorSwitch)USE_HAL_DRIVER  $(PreprocessorSwitch)CHIP_GC9002   -O2     -Wall -ffunction-sections -fdata-sections -Wall -Wno-unused-function -Wno-unused-but-set-variable -Wno-comment -Wno-unused-variable -Wno-maybe-uninitialized -fno-exceptions 


Objects0=$(IntermediateDirectory)/CustomerApp_CustomerAppMain$(ObjectSuffix) $(IntermediateDirectory)/CustomerApp_crc16$(ObjectSuffix) $(IntermediateDirectory)/CustomerApp_protocol$(ObjectSuffix) $(IntermediateDirectory)/CustomerApp_read_reg$(ObjectSuffix) $(IntermediateDirectory)/CustomerApp_template_flash$(ObjectSuffix) $(IntermediateDirectory)/board_board$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_crc$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_dma$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_gpio$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_gpio_pin$(ObjectSuffix) \
	$(IntermediateDirectory)/csi_driver_csi_i2s$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_iic$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_irq$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_irq_vectors$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_motor$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_pin$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_porting$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_pwm$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_qspi$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_rtc$(ObjectSuffix) \
	$(IntermediateDirectory)/csi_driver_csi_spi$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_spiflash$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_tick$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_timer$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_triangle$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_uart$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_csi_wdt$(ObjectSuffix) $(IntermediateDirectory)/csi_driver_drv_eth$(ObjectSuffix) $(IntermediateDirectory)/display_display$(ObjectSuffix) $(IntermediateDirectory)/display_display_888$(ObjectSuffix) \
	$(IntermediateDirectory)/display_display_gc9307n$(ObjectSuffix) $(IntermediateDirectory)/display_display_leike$(ObjectSuffix) $(IntermediateDirectory)/display_display_mrb3205$(ObjectSuffix) $(IntermediateDirectory)/display_display_rgb565$(ObjectSuffix) $(IntermediateDirectory)/display_display_sjxd$(ObjectSuffix) $(IntermediateDirectory)/display_display_st7512$(ObjectSuffix) $(IntermediateDirectory)/display_display_st7735s$(ObjectSuffix) $(IntermediateDirectory)/display_display_st77903$(ObjectSuffix) $(IntermediateDirectory)/display_display_tft128qm071$(ObjectSuffix) $(IntermediateDirectory)/libc_malloc$(ObjectSuffix) \
	

Objects1=$(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix) $(IntermediateDirectory)/libc_printf$(ObjectSuffix) $(IntermediateDirectory)/libc__init$(ObjectSuffix) $(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix) $(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix) $(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_free$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix) \
	$(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix) $(IntermediateDirectory)/ringbuffer_ringbuffer$(ObjectSuffix) $(IntermediateDirectory)/soft_libc_soft_libc$(ObjectSuffix) $(IntermediateDirectory)/syslog_syslog$(ObjectSuffix) $(IntermediateDirectory)/app_ahmiv4$(ObjectSuffix) $(IntermediateDirectory)/app_CPU1_Version$(ObjectSuffix) $(IntermediateDirectory)/app_gc90x_heap$(ObjectSuffix) $(IntermediateDirectory)/app_gc90x_initial$(ObjectSuffix) \
	$(IntermediateDirectory)/MiddleWare_MidTimer$(ObjectSuffix) $(IntermediateDirectory)/ParseParameter_ParsePara$(ObjectSuffix) $(IntermediateDirectory)/Touch_AHMITouch$(ObjectSuffix) $(IntermediateDirectory)/Touch_bsp_i2c_touch$(ObjectSuffix) $(IntermediateDirectory)/Touch_gc90x_touch$(ObjectSuffix) $(IntermediateDirectory)/Touch_gt9xx$(ObjectSuffix) $(IntermediateDirectory)/Touch_i2c_aw2083$(ObjectSuffix) $(IntermediateDirectory)/Touch_capacitiveTouch_Interface$(ObjectSuffix) $(IntermediateDirectory)/Touch_cst820$(ObjectSuffix) $(IntermediateDirectory)/Touch_cst836u$(ObjectSuffix) \
	$(IntermediateDirectory)/Touch_STTouch$(ObjectSuffix) $(IntermediateDirectory)/SBBatchDraw_SBBatchDraw$(ObjectSuffix) $(IntermediateDirectory)/AHMI_ActionProcessor$(ObjectSuffix) $(IntermediateDirectory)/AHMI_ActionTriggerClass$(ObjectSuffix) $(IntermediateDirectory)/AHMI_animation$(ObjectSuffix) $(IntermediateDirectory)/AHMI_animationAction$(ObjectSuffix) $(IntermediateDirectory)/AHMI_animationQueueHandler$(ObjectSuffix) $(IntermediateDirectory)/AHMI_CanvasClass$(ObjectSuffix) $(IntermediateDirectory)/AHMI_DynamicPageCfgClass$(ObjectSuffix) $(IntermediateDirectory)/AHMI_DynamicPageClass$(ObjectSuffix) \
	$(IntermediateDirectory)/AHMI_Easing$(ObjectSuffix) $(IntermediateDirectory)/AHMI_encoding$(ObjectSuffix) $(IntermediateDirectory)/AHMI_FileLoad$(ObjectSuffix) $(IntermediateDirectory)/AHMI_FontlayoutEngine$(ObjectSuffix) $(IntermediateDirectory)/AHMI_Hinter$(ObjectSuffix) $(IntermediateDirectory)/AHMI_InterruptTask$(ObjectSuffix) $(IntermediateDirectory)/AHMI_IntervalClass$(ObjectSuffix) $(IntermediateDirectory)/AHMI_keyboard$(ObjectSuffix) $(IntermediateDirectory)/AHMI_matrixClass$(ObjectSuffix) $(IntermediateDirectory)/AHMI_myMathClass$(ObjectSuffix) \
	$(IntermediateDirectory)/AHMI_publicVariables$(ObjectSuffix) 

Objects2=$(IntermediateDirectory)/AHMI_refreshQueueHandler$(ObjectSuffix) $(IntermediateDirectory)/AHMI_SdlPlayer$(ObjectSuffix) $(IntermediateDirectory)/AHMI_StringClass$(ObjectSuffix) $(IntermediateDirectory)/AHMI_subCanvasClass$(ObjectSuffix) $(IntermediateDirectory)/AHMI_TagClass$(ObjectSuffix) $(IntermediateDirectory)/AHMI_tagUpdate$(ObjectSuffix) $(IntermediateDirectory)/AHMI_TextureClass$(ObjectSuffix) $(IntermediateDirectory)/AHMI_tileBoxClass$(ObjectSuffix) $(IntermediateDirectory)/AHMI_TimerClass$(ObjectSuffix) \
	$(IntermediateDirectory)/AHMI_validation$(ObjectSuffix) $(IntermediateDirectory)/AHMI_WaveFilter$(ObjectSuffix) $(IntermediateDirectory)/AHMI_widgetClass$(ObjectSuffix) $(IntermediateDirectory)/AHMI_WidgetClassInterface$(ObjectSuffix) $(IntermediateDirectory)/AHMI_normalizer$(ObjectSuffix) $(IntermediateDirectory)/Main_AHMIInterface$(ObjectSuffix) $(IntermediateDirectory)/Main_main$(ObjectSuffix) $(IntermediateDirectory)/qrencode-3.4.4_bitstream$(ObjectSuffix) $(IntermediateDirectory)/qrencode-3.4.4_mask$(ObjectSuffix) $(IntermediateDirectory)/qrencode-3.4.4_mmask$(ObjectSuffix) \
	$(IntermediateDirectory)/qrencode-3.4.4_mqrspec$(ObjectSuffix) $(IntermediateDirectory)/qrencode-3.4.4_qrencode$(ObjectSuffix) $(IntermediateDirectory)/qrencode-3.4.4_qrinput$(ObjectSuffix) $(IntermediateDirectory)/qrencode-3.4.4_qrspec$(ObjectSuffix) $(IntermediateDirectory)/qrencode-3.4.4_rscode$(ObjectSuffix) $(IntermediateDirectory)/qrencode-3.4.4_split$(ObjectSuffix) $(IntermediateDirectory)/adapt_cfg_autogen_ca$(ObjectSuffix) $(IntermediateDirectory)/adapt_cfg_ca$(ObjectSuffix) $(IntermediateDirectory)/core_cfg_c1$(ObjectSuffix) $(IntermediateDirectory)/adapter_csi_freertos$(ObjectSuffix) \
	$(IntermediateDirectory)/Src_gc9002_hal_adc$(ObjectSuffix) 

Objects3=$(IntermediateDirectory)/Src_gc9002_hal_ahmi$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_ccm$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_crc32$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_display$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_dma$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_eth$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_extension$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_hyper$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_pmu$(ObjectSuffix) \
	$(IntermediateDirectory)/Src_gc9002_hal_rcc$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_usb$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_usbd$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_hal_wdt$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_ll_ahmi$(ObjectSuffix) $(IntermediateDirectory)/Src_gc9002_ll_display$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal$(ObjectSuffix) $(IntermediateDirectory)/Src_mmalloc$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_can$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_canfd$(ObjectSuffix) \
	$(IntermediateDirectory)/Src_gc90xx_hal_gpio$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_i2c$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_i2s$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_mmc$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_rtc$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_sd$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_spi$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_tim$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_tim_ex$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_hal_uart$(ObjectSuffix) \
	$(IntermediateDirectory)/Src_gc90xx_hal_videocap$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_ll_qspi$(ObjectSuffix) $(IntermediateDirectory)/Src_gc90xx_ll_sdmmc$(ObjectSuffix) $(IntermediateDirectory)/src_gc90x_ahmi$(ObjectSuffix) $(IntermediateDirectory)/src_gc90x_bt656$(ObjectSuffix) $(IntermediateDirectory)/src_gc90x_cache$(ObjectSuffix) $(IntermediateDirectory)/src_gc90x_fix$(ObjectSuffix) $(IntermediateDirectory)/src_gc90x_shmemmanage$(ObjectSuffix) $(IntermediateDirectory)/src_gc90x_display$(ObjectSuffix) $(IntermediateDirectory)/core_ITM1c1$(ObjectSuffix) \
	$(IntermediateDirectory)/core_SBM1c1$(ObjectSuffix) $(IntermediateDirectory)/core_com_c1$(ObjectSuffix) $(IntermediateDirectory)/core_com_fifo$(ObjectSuffix) $(IntermediateDirectory)/csi_ahmi_csi_ahmi$(ObjectSuffix) $(IntermediateDirectory)/csi_ahmi_csi_ahmi_bt656$(ObjectSuffix) $(IntermediateDirectory)/csi_ahmi_csi_ahmi_fix$(ObjectSuffix) $(IntermediateDirectory)/csi_ahmi_csi_ahmi_hw$(ObjectSuffix) $(IntermediateDirectory)/csi_ahmi_csi_ahmi_i2s$(ObjectSuffix) 

Objects4=$(IntermediateDirectory)/Touch_TouchTask$(ObjectSuffix) $(IntermediateDirectory)/Widget_AlphaImg$(ObjectSuffix) \
	$(IntermediateDirectory)/Widget_AlphaSlide$(ObjectSuffix) $(IntermediateDirectory)/Widget_ButtonClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_ButtonSwitch$(ObjectSuffix) $(IntermediateDirectory)/Widget_Chart$(ObjectSuffix) $(IntermediateDirectory)/Widget_CircleSliderClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_ClockBoardClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_ClockClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_CombinationSwitch$(ObjectSuffix) $(IntermediateDirectory)/Widget_DataImage$(ObjectSuffix) $(IntermediateDirectory)/Widget_DynamicTexClass$(ObjectSuffix) \
	$(IntermediateDirectory)/Widget_GalleryClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_IOClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_KeyboardContainer$(ObjectSuffix) $(IntermediateDirectory)/Widget_KnobClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_MenuContainer$(ObjectSuffix) $(IntermediateDirectory)/Widget_MeterClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_movingTexture$(ObjectSuffix) $(IntermediateDirectory)/Widget_NumberClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_OnOffTexture$(ObjectSuffix) $(IntermediateDirectory)/Widget_ProgBarClass$(ObjectSuffix) \
	$(IntermediateDirectory)/Widget_QRcodeClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_SelectorClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_SliderClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_SwipeResponderClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_TextClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_TextInput$(ObjectSuffix) $(IntermediateDirectory)/Widget_TextureNumberClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_TextureTimeClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_TouchTrack$(ObjectSuffix) $(IntermediateDirectory)/Widget_VideoClass$(ObjectSuffix) \
	$(IntermediateDirectory)/Widget_WriteTexture$(ObjectSuffix) $(IntermediateDirectory)/Widget_BarChart$(ObjectSuffix) $(IntermediateDirectory)/Widget_Gif$(ObjectSuffix) $(IntermediateDirectory)/Widget_PieChart$(ObjectSuffix) $(IntermediateDirectory)/Widget_Custom$(ObjectSuffix) $(IntermediateDirectory)/Widget_ListClass$(ObjectSuffix) $(IntermediateDirectory)/Widget_ThreeDimensionalClass$(ObjectSuffix) $(IntermediateDirectory)/csi_hal_core_csi_hal_core$(ObjectSuffix) $(IntermediateDirectory)/csi_hal_driver_csi_hal_driver_flash$(ObjectSuffix) $(IntermediateDirectory)/csi_hal_driver_ahmi_flash_driver_interface$(ObjectSuffix) \
	$(IntermediateDirectory)/csi_hal_isr_csi_hal_isr$(ObjectSuffix) $(IntermediateDirectory)/core_em_c1$(ObjectSuffix) $(IntermediateDirectory)/core_sm_c1$(ObjectSuffix) $(IntermediateDirectory)/Source_croutine$(ObjectSuffix) $(IntermediateDirectory)/Source_event_groups$(ObjectSuffix) $(IntermediateDirectory)/Source_list$(ObjectSuffix) $(IntermediateDirectory)/Source_queue$(ObjectSuffix) $(IntermediateDirectory)/Source_stream_buffer$(ObjectSuffix) $(IntermediateDirectory)/Source_tasks$(ObjectSuffix) $(IntermediateDirectory)/Source_timers$(ObjectSuffix) \
	

Objects5=$(IntermediateDirectory)/core_adcshare_c1$(ObjectSuffix) $(IntermediateDirectory)/core_player1c1$(ObjectSuffix) $(IntermediateDirectory)/source_sxClip$(ObjectSuffix) $(IntermediateDirectory)/source_sxCull$(ObjectSuffix) $(IntermediateDirectory)/source_sxGLApplication$(ObjectSuffix) $(IntermediateDirectory)/source_sxGLBuffer$(ObjectSuffix) $(IntermediateDirectory)/source_sxGLMath$(ObjectSuffix) $(IntermediateDirectory)/source_sxGLObject$(ObjectSuffix) $(IntermediateDirectory)/source_sxGLObjIn$(ObjectSuffix) $(IntermediateDirectory)/source_sxGLOperator$(ObjectSuffix) \
	$(IntermediateDirectory)/source_sxGLPipeline$(ObjectSuffix) $(IntermediateDirectory)/source_sxGLSourceBuffer$(ObjectSuffix) $(IntermediateDirectory)/source_sxGLState$(ObjectSuffix) $(IntermediateDirectory)/source_sxGL_draw$(ObjectSuffix) $(IntermediateDirectory)/source_sxLight$(ObjectSuffix) $(IntermediateDirectory)/source_sxMatrix$(ObjectSuffix) $(IntermediateDirectory)/source_sxTexture$(ObjectSuffix) $(IntermediateDirectory)/Source_startup$(ObjectSuffix) $(IntermediateDirectory)/Source_system$(ObjectSuffix) $(IntermediateDirectory)/Source_trap_c$(ObjectSuffix) \
	$(IntermediateDirectory)/Source_vectors$(ObjectSuffix) $(IntermediateDirectory)/MemMang_heap_4$(ObjectSuffix) $(IntermediateDirectory)/riscv_port$(ObjectSuffix) $(IntermediateDirectory)/riscv_portASM$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) $(Objects2) $(Objects3) $(Objects4) $(Objects5) 

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
	@GC9002AHMI.modify.bat $(IntermediateDirectory) $(OutputFile)$(ExeSuffix) 

Always_Link:


##
## Objects
##
$(IntermediateDirectory)/CustomerApp_CustomerAppMain$(ObjectSuffix): CustomerApp/CustomerAppMain.c  
	$(CC) $(SourceSwitch) CustomerApp/CustomerAppMain.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/CustomerApp_CustomerAppMain$(ObjectSuffix) -MF$(IntermediateDirectory)/CustomerApp_CustomerAppMain$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/CustomerApp_CustomerAppMain$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/CustomerApp_CustomerAppMain$(PreprocessSuffix): CustomerApp/CustomerAppMain.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/CustomerApp_CustomerAppMain$(PreprocessSuffix) CustomerApp/CustomerAppMain.c

$(IntermediateDirectory)/CustomerApp_crc16$(ObjectSuffix): CustomerApp/crc16.c  
	$(CC) $(SourceSwitch) CustomerApp/crc16.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/CustomerApp_crc16$(ObjectSuffix) -MF$(IntermediateDirectory)/CustomerApp_crc16$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/CustomerApp_crc16$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/CustomerApp_crc16$(PreprocessSuffix): CustomerApp/crc16.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/CustomerApp_crc16$(PreprocessSuffix) CustomerApp/crc16.c

$(IntermediateDirectory)/CustomerApp_protocol$(ObjectSuffix): CustomerApp/protocol.c  
	$(CC) $(SourceSwitch) CustomerApp/protocol.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/CustomerApp_protocol$(ObjectSuffix) -MF$(IntermediateDirectory)/CustomerApp_protocol$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/CustomerApp_protocol$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/CustomerApp_protocol$(PreprocessSuffix): CustomerApp/protocol.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/CustomerApp_protocol$(PreprocessSuffix) CustomerApp/protocol.c

$(IntermediateDirectory)/CustomerApp_read_reg$(ObjectSuffix): CustomerApp/read_reg.c  
	$(CC) $(SourceSwitch) CustomerApp/read_reg.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/CustomerApp_read_reg$(ObjectSuffix) -MF$(IntermediateDirectory)/CustomerApp_read_reg$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/CustomerApp_read_reg$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/CustomerApp_read_reg$(PreprocessSuffix): CustomerApp/read_reg.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/CustomerApp_read_reg$(PreprocessSuffix) CustomerApp/read_reg.c

$(IntermediateDirectory)/CustomerApp_template_flash$(ObjectSuffix): CustomerApp/template_flash.c  
	$(CC) $(SourceSwitch) CustomerApp/template_flash.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/CustomerApp_template_flash$(ObjectSuffix) -MF$(IntermediateDirectory)/CustomerApp_template_flash$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/CustomerApp_template_flash$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/CustomerApp_template_flash$(PreprocessSuffix): CustomerApp/template_flash.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/CustomerApp_template_flash$(PreprocessSuffix) CustomerApp/template_flash.c

$(IntermediateDirectory)/board_board$(ObjectSuffix): ../../board/board.c  
	$(CC) $(SourceSwitch) ../../board/board.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/board_board$(ObjectSuffix) -MF$(IntermediateDirectory)/board_board$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/board_board$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/board_board$(PreprocessSuffix): ../../board/board.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/board_board$(PreprocessSuffix) ../../board/board.c

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

$(IntermediateDirectory)/app_ahmiv4$(ObjectSuffix): Source/ahmi/app/ahmiv4.c  
	$(CC) $(SourceSwitch) Source/ahmi/app/ahmiv4.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/app_ahmiv4$(ObjectSuffix) -MF$(IntermediateDirectory)/app_ahmiv4$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/app_ahmiv4$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/app_ahmiv4$(PreprocessSuffix): Source/ahmi/app/ahmiv4.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/app_ahmiv4$(PreprocessSuffix) Source/ahmi/app/ahmiv4.c

$(IntermediateDirectory)/app_CPU1_Version$(ObjectSuffix): Source/ahmi/app/CPU1_Version.c  
	$(CC) $(SourceSwitch) Source/ahmi/app/CPU1_Version.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/app_CPU1_Version$(ObjectSuffix) -MF$(IntermediateDirectory)/app_CPU1_Version$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/app_CPU1_Version$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/app_CPU1_Version$(PreprocessSuffix): Source/ahmi/app/CPU1_Version.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/app_CPU1_Version$(PreprocessSuffix) Source/ahmi/app/CPU1_Version.c

$(IntermediateDirectory)/app_gc90x_heap$(ObjectSuffix): Source/ahmi/app/gc90x_heap.cpp  
	$(CXX) $(SourceSwitch) Source/ahmi/app/gc90x_heap.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/app_gc90x_heap$(ObjectSuffix) -MF$(IntermediateDirectory)/app_gc90x_heap$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/app_gc90x_heap$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/app_gc90x_heap$(PreprocessSuffix): Source/ahmi/app/gc90x_heap.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/app_gc90x_heap$(PreprocessSuffix) Source/ahmi/app/gc90x_heap.cpp

$(IntermediateDirectory)/app_gc90x_initial$(ObjectSuffix): Source/ahmi/app/gc90x_initial.cpp  
	$(CXX) $(SourceSwitch) Source/ahmi/app/gc90x_initial.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/app_gc90x_initial$(ObjectSuffix) -MF$(IntermediateDirectory)/app_gc90x_initial$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/app_gc90x_initial$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/app_gc90x_initial$(PreprocessSuffix): Source/ahmi/app/gc90x_initial.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/app_gc90x_initial$(PreprocessSuffix) Source/ahmi/app/gc90x_initial.cpp

$(IntermediateDirectory)/MiddleWare_MidTimer$(ObjectSuffix): Source/ahmi/MiddleWare/MidTimer.c  
	$(CC) $(SourceSwitch) Source/ahmi/MiddleWare/MidTimer.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/MiddleWare_MidTimer$(ObjectSuffix) -MF$(IntermediateDirectory)/MiddleWare_MidTimer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/MiddleWare_MidTimer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/MiddleWare_MidTimer$(PreprocessSuffix): Source/ahmi/MiddleWare/MidTimer.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/MiddleWare_MidTimer$(PreprocessSuffix) Source/ahmi/MiddleWare/MidTimer.c

$(IntermediateDirectory)/ParseParameter_ParsePara$(ObjectSuffix): Source/ahmi/ParseParameter/ParsePara.c  
	$(CC) $(SourceSwitch) Source/ahmi/ParseParameter/ParsePara.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ParseParameter_ParsePara$(ObjectSuffix) -MF$(IntermediateDirectory)/ParseParameter_ParsePara$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ParseParameter_ParsePara$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ParseParameter_ParsePara$(PreprocessSuffix): Source/ahmi/ParseParameter/ParsePara.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ParseParameter_ParsePara$(PreprocessSuffix) Source/ahmi/ParseParameter/ParsePara.c

$(IntermediateDirectory)/Touch_AHMITouch$(ObjectSuffix): Source/ahmi/Touch/AHMITouch.cpp  
	$(CXX) $(SourceSwitch) Source/ahmi/Touch/AHMITouch.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Touch_AHMITouch$(ObjectSuffix) -MF$(IntermediateDirectory)/Touch_AHMITouch$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Touch_AHMITouch$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Touch_AHMITouch$(PreprocessSuffix): Source/ahmi/Touch/AHMITouch.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Touch_AHMITouch$(PreprocessSuffix) Source/ahmi/Touch/AHMITouch.cpp

$(IntermediateDirectory)/Touch_bsp_i2c_touch$(ObjectSuffix): Source/ahmi/Touch/bsp_i2c_touch.cpp  
	$(CXX) $(SourceSwitch) Source/ahmi/Touch/bsp_i2c_touch.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Touch_bsp_i2c_touch$(ObjectSuffix) -MF$(IntermediateDirectory)/Touch_bsp_i2c_touch$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Touch_bsp_i2c_touch$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Touch_bsp_i2c_touch$(PreprocessSuffix): Source/ahmi/Touch/bsp_i2c_touch.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Touch_bsp_i2c_touch$(PreprocessSuffix) Source/ahmi/Touch/bsp_i2c_touch.cpp

$(IntermediateDirectory)/Touch_gc90x_touch$(ObjectSuffix): Source/ahmi/Touch/gc90x_touch.cpp  
	$(CXX) $(SourceSwitch) Source/ahmi/Touch/gc90x_touch.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Touch_gc90x_touch$(ObjectSuffix) -MF$(IntermediateDirectory)/Touch_gc90x_touch$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Touch_gc90x_touch$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Touch_gc90x_touch$(PreprocessSuffix): Source/ahmi/Touch/gc90x_touch.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Touch_gc90x_touch$(PreprocessSuffix) Source/ahmi/Touch/gc90x_touch.cpp

$(IntermediateDirectory)/Touch_gt9xx$(ObjectSuffix): Source/ahmi/Touch/gt9xx.cpp  
	$(CXX) $(SourceSwitch) Source/ahmi/Touch/gt9xx.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Touch_gt9xx$(ObjectSuffix) -MF$(IntermediateDirectory)/Touch_gt9xx$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Touch_gt9xx$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Touch_gt9xx$(PreprocessSuffix): Source/ahmi/Touch/gt9xx.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Touch_gt9xx$(PreprocessSuffix) Source/ahmi/Touch/gt9xx.cpp

$(IntermediateDirectory)/Touch_i2c_aw2083$(ObjectSuffix): Source/ahmi/Touch/i2c_aw2083.cpp  
	$(CXX) $(SourceSwitch) Source/ahmi/Touch/i2c_aw2083.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Touch_i2c_aw2083$(ObjectSuffix) -MF$(IntermediateDirectory)/Touch_i2c_aw2083$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Touch_i2c_aw2083$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Touch_i2c_aw2083$(PreprocessSuffix): Source/ahmi/Touch/i2c_aw2083.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Touch_i2c_aw2083$(PreprocessSuffix) Source/ahmi/Touch/i2c_aw2083.cpp

$(IntermediateDirectory)/Touch_capacitiveTouch_Interface$(ObjectSuffix): Source/ahmi/Touch/capacitiveTouch_Interface.c  
	$(CC) $(SourceSwitch) Source/ahmi/Touch/capacitiveTouch_Interface.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Touch_capacitiveTouch_Interface$(ObjectSuffix) -MF$(IntermediateDirectory)/Touch_capacitiveTouch_Interface$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Touch_capacitiveTouch_Interface$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Touch_capacitiveTouch_Interface$(PreprocessSuffix): Source/ahmi/Touch/capacitiveTouch_Interface.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Touch_capacitiveTouch_Interface$(PreprocessSuffix) Source/ahmi/Touch/capacitiveTouch_Interface.c

$(IntermediateDirectory)/Touch_cst820$(ObjectSuffix): Source/ahmi/Touch/cst820.cpp  
	$(CXX) $(SourceSwitch) Source/ahmi/Touch/cst820.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Touch_cst820$(ObjectSuffix) -MF$(IntermediateDirectory)/Touch_cst820$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Touch_cst820$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Touch_cst820$(PreprocessSuffix): Source/ahmi/Touch/cst820.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Touch_cst820$(PreprocessSuffix) Source/ahmi/Touch/cst820.cpp

$(IntermediateDirectory)/Touch_cst836u$(ObjectSuffix): Source/ahmi/Touch/cst836u.cpp  
	$(CXX) $(SourceSwitch) Source/ahmi/Touch/cst836u.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Touch_cst836u$(ObjectSuffix) -MF$(IntermediateDirectory)/Touch_cst836u$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Touch_cst836u$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Touch_cst836u$(PreprocessSuffix): Source/ahmi/Touch/cst836u.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Touch_cst836u$(PreprocessSuffix) Source/ahmi/Touch/cst836u.cpp

$(IntermediateDirectory)/Touch_STTouch$(ObjectSuffix): Source/ahmi/Touch/STTouch.cpp  
	$(CXX) $(SourceSwitch) Source/ahmi/Touch/STTouch.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Touch_STTouch$(ObjectSuffix) -MF$(IntermediateDirectory)/Touch_STTouch$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Touch_STTouch$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Touch_STTouch$(PreprocessSuffix): Source/ahmi/Touch/STTouch.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Touch_STTouch$(PreprocessSuffix) Source/ahmi/Touch/STTouch.cpp

$(IntermediateDirectory)/SBBatchDraw_SBBatchDraw$(ObjectSuffix): Source/ahmi/SBBatchDraw/SBBatchDraw.c  
	$(CC) $(SourceSwitch) Source/ahmi/SBBatchDraw/SBBatchDraw.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/SBBatchDraw_SBBatchDraw$(ObjectSuffix) -MF$(IntermediateDirectory)/SBBatchDraw_SBBatchDraw$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/SBBatchDraw_SBBatchDraw$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/SBBatchDraw_SBBatchDraw$(PreprocessSuffix): Source/ahmi/SBBatchDraw/SBBatchDraw.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/SBBatchDraw_SBBatchDraw$(PreprocessSuffix) Source/ahmi/SBBatchDraw/SBBatchDraw.c

$(IntermediateDirectory)/AHMI_ActionProcessor$(ObjectSuffix): ../../../../CommonSource/AHMI/ActionProcessor.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/ActionProcessor.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_ActionProcessor$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_ActionProcessor$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_ActionProcessor$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_ActionProcessor$(PreprocessSuffix): ../../../../CommonSource/AHMI/ActionProcessor.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_ActionProcessor$(PreprocessSuffix) ../../../../CommonSource/AHMI/ActionProcessor.cpp

$(IntermediateDirectory)/AHMI_ActionTriggerClass$(ObjectSuffix): ../../../../CommonSource/AHMI/ActionTriggerClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/ActionTriggerClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_ActionTriggerClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_ActionTriggerClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_ActionTriggerClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_ActionTriggerClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/ActionTriggerClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_ActionTriggerClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/ActionTriggerClass.cpp

$(IntermediateDirectory)/AHMI_animation$(ObjectSuffix): ../../../../CommonSource/AHMI/animation.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/animation.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_animation$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_animation$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_animation$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_animation$(PreprocessSuffix): ../../../../CommonSource/AHMI/animation.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_animation$(PreprocessSuffix) ../../../../CommonSource/AHMI/animation.cpp

$(IntermediateDirectory)/AHMI_animationAction$(ObjectSuffix): ../../../../CommonSource/AHMI/animationAction.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/animationAction.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_animationAction$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_animationAction$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_animationAction$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_animationAction$(PreprocessSuffix): ../../../../CommonSource/AHMI/animationAction.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_animationAction$(PreprocessSuffix) ../../../../CommonSource/AHMI/animationAction.cpp

$(IntermediateDirectory)/AHMI_animationQueueHandler$(ObjectSuffix): ../../../../CommonSource/AHMI/animationQueueHandler.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/animationQueueHandler.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_animationQueueHandler$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_animationQueueHandler$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_animationQueueHandler$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_animationQueueHandler$(PreprocessSuffix): ../../../../CommonSource/AHMI/animationQueueHandler.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_animationQueueHandler$(PreprocessSuffix) ../../../../CommonSource/AHMI/animationQueueHandler.cpp

$(IntermediateDirectory)/AHMI_CanvasClass$(ObjectSuffix): ../../../../CommonSource/AHMI/CanvasClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/CanvasClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_CanvasClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_CanvasClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_CanvasClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_CanvasClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/CanvasClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_CanvasClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/CanvasClass.cpp

$(IntermediateDirectory)/AHMI_DynamicPageCfgClass$(ObjectSuffix): ../../../../CommonSource/AHMI/DynamicPageCfgClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/DynamicPageCfgClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_DynamicPageCfgClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_DynamicPageCfgClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_DynamicPageCfgClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_DynamicPageCfgClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/DynamicPageCfgClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_DynamicPageCfgClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/DynamicPageCfgClass.cpp

$(IntermediateDirectory)/AHMI_DynamicPageClass$(ObjectSuffix): ../../../../CommonSource/AHMI/DynamicPageClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/DynamicPageClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_DynamicPageClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_DynamicPageClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_DynamicPageClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_DynamicPageClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/DynamicPageClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_DynamicPageClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/DynamicPageClass.cpp

$(IntermediateDirectory)/AHMI_Easing$(ObjectSuffix): ../../../../CommonSource/AHMI/Easing.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Easing.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_Easing$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_Easing$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_Easing$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_Easing$(PreprocessSuffix): ../../../../CommonSource/AHMI/Easing.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_Easing$(PreprocessSuffix) ../../../../CommonSource/AHMI/Easing.cpp

$(IntermediateDirectory)/AHMI_encoding$(ObjectSuffix): ../../../../CommonSource/AHMI/encoding.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/encoding.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_encoding$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_encoding$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_encoding$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_encoding$(PreprocessSuffix): ../../../../CommonSource/AHMI/encoding.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_encoding$(PreprocessSuffix) ../../../../CommonSource/AHMI/encoding.cpp

$(IntermediateDirectory)/AHMI_FileLoad$(ObjectSuffix): ../../../../CommonSource/AHMI/FileLoad.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/FileLoad.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_FileLoad$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_FileLoad$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_FileLoad$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_FileLoad$(PreprocessSuffix): ../../../../CommonSource/AHMI/FileLoad.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_FileLoad$(PreprocessSuffix) ../../../../CommonSource/AHMI/FileLoad.cpp

$(IntermediateDirectory)/AHMI_FontlayoutEngine$(ObjectSuffix): ../../../../CommonSource/AHMI/FontlayoutEngine.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/FontlayoutEngine.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_FontlayoutEngine$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_FontlayoutEngine$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_FontlayoutEngine$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_FontlayoutEngine$(PreprocessSuffix): ../../../../CommonSource/AHMI/FontlayoutEngine.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_FontlayoutEngine$(PreprocessSuffix) ../../../../CommonSource/AHMI/FontlayoutEngine.cpp

$(IntermediateDirectory)/AHMI_Hinter$(ObjectSuffix): ../../../../CommonSource/AHMI/Hinter.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Hinter.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_Hinter$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_Hinter$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_Hinter$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_Hinter$(PreprocessSuffix): ../../../../CommonSource/AHMI/Hinter.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_Hinter$(PreprocessSuffix) ../../../../CommonSource/AHMI/Hinter.cpp

$(IntermediateDirectory)/AHMI_InterruptTask$(ObjectSuffix): ../../../../CommonSource/AHMI/InterruptTask.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/InterruptTask.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_InterruptTask$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_InterruptTask$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_InterruptTask$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_InterruptTask$(PreprocessSuffix): ../../../../CommonSource/AHMI/InterruptTask.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_InterruptTask$(PreprocessSuffix) ../../../../CommonSource/AHMI/InterruptTask.cpp

$(IntermediateDirectory)/AHMI_IntervalClass$(ObjectSuffix): ../../../../CommonSource/AHMI/IntervalClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/IntervalClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_IntervalClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_IntervalClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_IntervalClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_IntervalClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/IntervalClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_IntervalClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/IntervalClass.cpp

$(IntermediateDirectory)/AHMI_keyboard$(ObjectSuffix): ../../../../CommonSource/AHMI/keyboard.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/keyboard.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_keyboard$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_keyboard$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_keyboard$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_keyboard$(PreprocessSuffix): ../../../../CommonSource/AHMI/keyboard.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_keyboard$(PreprocessSuffix) ../../../../CommonSource/AHMI/keyboard.cpp

$(IntermediateDirectory)/AHMI_matrixClass$(ObjectSuffix): ../../../../CommonSource/AHMI/matrixClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/matrixClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_matrixClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_matrixClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_matrixClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_matrixClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/matrixClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_matrixClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/matrixClass.cpp

$(IntermediateDirectory)/AHMI_myMathClass$(ObjectSuffix): ../../../../CommonSource/AHMI/myMathClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/myMathClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_myMathClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_myMathClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_myMathClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_myMathClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/myMathClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_myMathClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/myMathClass.cpp

$(IntermediateDirectory)/AHMI_publicVariables$(ObjectSuffix): ../../../../CommonSource/AHMI/publicVariables.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/publicVariables.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_publicVariables$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_publicVariables$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_publicVariables$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_publicVariables$(PreprocessSuffix): ../../../../CommonSource/AHMI/publicVariables.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_publicVariables$(PreprocessSuffix) ../../../../CommonSource/AHMI/publicVariables.cpp

$(IntermediateDirectory)/AHMI_refreshQueueHandler$(ObjectSuffix): ../../../../CommonSource/AHMI/refreshQueueHandler.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/refreshQueueHandler.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_refreshQueueHandler$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_refreshQueueHandler$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_refreshQueueHandler$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_refreshQueueHandler$(PreprocessSuffix): ../../../../CommonSource/AHMI/refreshQueueHandler.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_refreshQueueHandler$(PreprocessSuffix) ../../../../CommonSource/AHMI/refreshQueueHandler.cpp

$(IntermediateDirectory)/AHMI_SdlPlayer$(ObjectSuffix): ../../../../CommonSource/AHMI/SdlPlayer.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/SdlPlayer.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_SdlPlayer$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_SdlPlayer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_SdlPlayer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_SdlPlayer$(PreprocessSuffix): ../../../../CommonSource/AHMI/SdlPlayer.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_SdlPlayer$(PreprocessSuffix) ../../../../CommonSource/AHMI/SdlPlayer.cpp

$(IntermediateDirectory)/AHMI_StringClass$(ObjectSuffix): ../../../../CommonSource/AHMI/StringClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/StringClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_StringClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_StringClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_StringClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_StringClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/StringClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_StringClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/StringClass.cpp

$(IntermediateDirectory)/AHMI_subCanvasClass$(ObjectSuffix): ../../../../CommonSource/AHMI/subCanvasClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/subCanvasClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_subCanvasClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_subCanvasClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_subCanvasClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_subCanvasClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/subCanvasClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_subCanvasClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/subCanvasClass.cpp

$(IntermediateDirectory)/AHMI_TagClass$(ObjectSuffix): ../../../../CommonSource/AHMI/TagClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/TagClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_TagClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_TagClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_TagClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_TagClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/TagClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_TagClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/TagClass.cpp

$(IntermediateDirectory)/AHMI_tagUpdate$(ObjectSuffix): ../../../../CommonSource/AHMI/tagUpdate.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/tagUpdate.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_tagUpdate$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_tagUpdate$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_tagUpdate$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_tagUpdate$(PreprocessSuffix): ../../../../CommonSource/AHMI/tagUpdate.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_tagUpdate$(PreprocessSuffix) ../../../../CommonSource/AHMI/tagUpdate.cpp

$(IntermediateDirectory)/AHMI_TextureClass$(ObjectSuffix): ../../../../CommonSource/AHMI/TextureClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/TextureClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_TextureClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_TextureClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_TextureClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_TextureClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/TextureClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_TextureClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/TextureClass.cpp

$(IntermediateDirectory)/AHMI_tileBoxClass$(ObjectSuffix): ../../../../CommonSource/AHMI/tileBoxClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/tileBoxClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_tileBoxClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_tileBoxClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_tileBoxClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_tileBoxClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/tileBoxClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_tileBoxClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/tileBoxClass.cpp

$(IntermediateDirectory)/AHMI_TimerClass$(ObjectSuffix): ../../../../CommonSource/AHMI/TimerClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/TimerClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_TimerClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_TimerClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_TimerClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_TimerClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/TimerClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_TimerClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/TimerClass.cpp

$(IntermediateDirectory)/AHMI_validation$(ObjectSuffix): ../../../../CommonSource/AHMI/validation.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/validation.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_validation$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_validation$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_validation$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_validation$(PreprocessSuffix): ../../../../CommonSource/AHMI/validation.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_validation$(PreprocessSuffix) ../../../../CommonSource/AHMI/validation.cpp

$(IntermediateDirectory)/AHMI_WaveFilter$(ObjectSuffix): ../../../../CommonSource/AHMI/WaveFilter.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/WaveFilter.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_WaveFilter$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_WaveFilter$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_WaveFilter$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_WaveFilter$(PreprocessSuffix): ../../../../CommonSource/AHMI/WaveFilter.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_WaveFilter$(PreprocessSuffix) ../../../../CommonSource/AHMI/WaveFilter.cpp

$(IntermediateDirectory)/AHMI_widgetClass$(ObjectSuffix): ../../../../CommonSource/AHMI/widgetClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/widgetClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_widgetClass$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_widgetClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_widgetClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_widgetClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/widgetClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_widgetClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/widgetClass.cpp

$(IntermediateDirectory)/AHMI_WidgetClassInterface$(ObjectSuffix): ../../../../CommonSource/AHMI/WidgetClassInterface.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/WidgetClassInterface.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_WidgetClassInterface$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_WidgetClassInterface$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_WidgetClassInterface$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_WidgetClassInterface$(PreprocessSuffix): ../../../../CommonSource/AHMI/WidgetClassInterface.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_WidgetClassInterface$(PreprocessSuffix) ../../../../CommonSource/AHMI/WidgetClassInterface.cpp

$(IntermediateDirectory)/AHMI_normalizer$(ObjectSuffix): ../../../../CommonSource/AHMI/normalizer.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/normalizer.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/AHMI_normalizer$(ObjectSuffix) -MF$(IntermediateDirectory)/AHMI_normalizer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/AHMI_normalizer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/AHMI_normalizer$(PreprocessSuffix): ../../../../CommonSource/AHMI/normalizer.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/AHMI_normalizer$(PreprocessSuffix) ../../../../CommonSource/AHMI/normalizer.cpp

$(IntermediateDirectory)/Main_AHMIInterface$(ObjectSuffix): ../../../../CommonSource/Main/AHMIInterface.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/Main/AHMIInterface.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Main_AHMIInterface$(ObjectSuffix) -MF$(IntermediateDirectory)/Main_AHMIInterface$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Main_AHMIInterface$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Main_AHMIInterface$(PreprocessSuffix): ../../../../CommonSource/Main/AHMIInterface.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Main_AHMIInterface$(PreprocessSuffix) ../../../../CommonSource/Main/AHMIInterface.cpp

$(IntermediateDirectory)/Main_main$(ObjectSuffix): ../../../../CommonSource/Main/main.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/Main/main.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Main_main$(ObjectSuffix) -MF$(IntermediateDirectory)/Main_main$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Main_main$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Main_main$(PreprocessSuffix): ../../../../CommonSource/Main/main.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Main_main$(PreprocessSuffix) ../../../../CommonSource/Main/main.cpp

$(IntermediateDirectory)/qrencode-3.4.4_bitstream$(ObjectSuffix): ../../../../CommonSource/qrencode-3.4.4/bitstream.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/qrencode-3.4.4/bitstream.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/qrencode-3.4.4_bitstream$(ObjectSuffix) -MF$(IntermediateDirectory)/qrencode-3.4.4_bitstream$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/qrencode-3.4.4_bitstream$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/qrencode-3.4.4_bitstream$(PreprocessSuffix): ../../../../CommonSource/qrencode-3.4.4/bitstream.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/qrencode-3.4.4_bitstream$(PreprocessSuffix) ../../../../CommonSource/qrencode-3.4.4/bitstream.cpp

$(IntermediateDirectory)/qrencode-3.4.4_mask$(ObjectSuffix): ../../../../CommonSource/qrencode-3.4.4/mask.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/qrencode-3.4.4/mask.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/qrencode-3.4.4_mask$(ObjectSuffix) -MF$(IntermediateDirectory)/qrencode-3.4.4_mask$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/qrencode-3.4.4_mask$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/qrencode-3.4.4_mask$(PreprocessSuffix): ../../../../CommonSource/qrencode-3.4.4/mask.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/qrencode-3.4.4_mask$(PreprocessSuffix) ../../../../CommonSource/qrencode-3.4.4/mask.cpp

$(IntermediateDirectory)/qrencode-3.4.4_mmask$(ObjectSuffix): ../../../../CommonSource/qrencode-3.4.4/mmask.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/qrencode-3.4.4/mmask.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/qrencode-3.4.4_mmask$(ObjectSuffix) -MF$(IntermediateDirectory)/qrencode-3.4.4_mmask$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/qrencode-3.4.4_mmask$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/qrencode-3.4.4_mmask$(PreprocessSuffix): ../../../../CommonSource/qrencode-3.4.4/mmask.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/qrencode-3.4.4_mmask$(PreprocessSuffix) ../../../../CommonSource/qrencode-3.4.4/mmask.cpp

$(IntermediateDirectory)/qrencode-3.4.4_mqrspec$(ObjectSuffix): ../../../../CommonSource/qrencode-3.4.4/mqrspec.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/qrencode-3.4.4/mqrspec.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/qrencode-3.4.4_mqrspec$(ObjectSuffix) -MF$(IntermediateDirectory)/qrencode-3.4.4_mqrspec$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/qrencode-3.4.4_mqrspec$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/qrencode-3.4.4_mqrspec$(PreprocessSuffix): ../../../../CommonSource/qrencode-3.4.4/mqrspec.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/qrencode-3.4.4_mqrspec$(PreprocessSuffix) ../../../../CommonSource/qrencode-3.4.4/mqrspec.cpp

$(IntermediateDirectory)/qrencode-3.4.4_qrencode$(ObjectSuffix): ../../../../CommonSource/qrencode-3.4.4/qrencode.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/qrencode-3.4.4/qrencode.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/qrencode-3.4.4_qrencode$(ObjectSuffix) -MF$(IntermediateDirectory)/qrencode-3.4.4_qrencode$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/qrencode-3.4.4_qrencode$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/qrencode-3.4.4_qrencode$(PreprocessSuffix): ../../../../CommonSource/qrencode-3.4.4/qrencode.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/qrencode-3.4.4_qrencode$(PreprocessSuffix) ../../../../CommonSource/qrencode-3.4.4/qrencode.cpp

$(IntermediateDirectory)/qrencode-3.4.4_qrinput$(ObjectSuffix): ../../../../CommonSource/qrencode-3.4.4/qrinput.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/qrencode-3.4.4/qrinput.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/qrencode-3.4.4_qrinput$(ObjectSuffix) -MF$(IntermediateDirectory)/qrencode-3.4.4_qrinput$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/qrencode-3.4.4_qrinput$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/qrencode-3.4.4_qrinput$(PreprocessSuffix): ../../../../CommonSource/qrencode-3.4.4/qrinput.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/qrencode-3.4.4_qrinput$(PreprocessSuffix) ../../../../CommonSource/qrencode-3.4.4/qrinput.cpp

$(IntermediateDirectory)/qrencode-3.4.4_qrspec$(ObjectSuffix): ../../../../CommonSource/qrencode-3.4.4/qrspec.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/qrencode-3.4.4/qrspec.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/qrencode-3.4.4_qrspec$(ObjectSuffix) -MF$(IntermediateDirectory)/qrencode-3.4.4_qrspec$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/qrencode-3.4.4_qrspec$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/qrencode-3.4.4_qrspec$(PreprocessSuffix): ../../../../CommonSource/qrencode-3.4.4/qrspec.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/qrencode-3.4.4_qrspec$(PreprocessSuffix) ../../../../CommonSource/qrencode-3.4.4/qrspec.cpp

$(IntermediateDirectory)/qrencode-3.4.4_rscode$(ObjectSuffix): ../../../../CommonSource/qrencode-3.4.4/rscode.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/qrencode-3.4.4/rscode.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/qrencode-3.4.4_rscode$(ObjectSuffix) -MF$(IntermediateDirectory)/qrencode-3.4.4_rscode$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/qrencode-3.4.4_rscode$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/qrencode-3.4.4_rscode$(PreprocessSuffix): ../../../../CommonSource/qrencode-3.4.4/rscode.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/qrencode-3.4.4_rscode$(PreprocessSuffix) ../../../../CommonSource/qrencode-3.4.4/rscode.cpp

$(IntermediateDirectory)/qrencode-3.4.4_split$(ObjectSuffix): ../../../../CommonSource/qrencode-3.4.4/split.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/qrencode-3.4.4/split.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/qrencode-3.4.4_split$(ObjectSuffix) -MF$(IntermediateDirectory)/qrencode-3.4.4_split$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/qrencode-3.4.4_split$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/qrencode-3.4.4_split$(PreprocessSuffix): ../../../../CommonSource/qrencode-3.4.4/split.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/qrencode-3.4.4_split$(PreprocessSuffix) ../../../../CommonSource/qrencode-3.4.4/split.cpp

$(IntermediateDirectory)/adapt_cfg_autogen_ca$(ObjectSuffix): ../Bootloader/Source/Cfg/adapt/cfg_autogen_ca.c  
	$(CC) $(SourceSwitch) ../Bootloader/Source/Cfg/adapt/cfg_autogen_ca.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/adapt_cfg_autogen_ca$(ObjectSuffix) -MF$(IntermediateDirectory)/adapt_cfg_autogen_ca$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/adapt_cfg_autogen_ca$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/adapt_cfg_autogen_ca$(PreprocessSuffix): ../Bootloader/Source/Cfg/adapt/cfg_autogen_ca.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/adapt_cfg_autogen_ca$(PreprocessSuffix) ../Bootloader/Source/Cfg/adapt/cfg_autogen_ca.c

$(IntermediateDirectory)/adapt_cfg_ca$(ObjectSuffix): ../Bootloader/Source/Cfg/adapt/cfg_ca.c  
	$(CC) $(SourceSwitch) ../Bootloader/Source/Cfg/adapt/cfg_ca.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/adapt_cfg_ca$(ObjectSuffix) -MF$(IntermediateDirectory)/adapt_cfg_ca$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/adapt_cfg_ca$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/adapt_cfg_ca$(PreprocessSuffix): ../Bootloader/Source/Cfg/adapt/cfg_ca.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/adapt_cfg_ca$(PreprocessSuffix) ../Bootloader/Source/Cfg/adapt/cfg_ca.c

$(IntermediateDirectory)/core_cfg_c1$(ObjectSuffix): ../Bootloader/Source/Cfg/core/cfg_c1.c  
	$(CC) $(SourceSwitch) ../Bootloader/Source/Cfg/core/cfg_c1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_cfg_c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_cfg_c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_cfg_c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_cfg_c1$(PreprocessSuffix): ../Bootloader/Source/Cfg/core/cfg_c1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_cfg_c1$(PreprocessSuffix) ../Bootloader/Source/Cfg/core/cfg_c1.c

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

$(IntermediateDirectory)/src_gc90x_ahmi$(ObjectSuffix): Source/ahmi/GClib/src/gc90x_ahmi.c  
	$(CC) $(SourceSwitch) Source/ahmi/GClib/src/gc90x_ahmi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/src_gc90x_ahmi$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gc90x_ahmi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/src_gc90x_ahmi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/src_gc90x_ahmi$(PreprocessSuffix): Source/ahmi/GClib/src/gc90x_ahmi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/src_gc90x_ahmi$(PreprocessSuffix) Source/ahmi/GClib/src/gc90x_ahmi.c

$(IntermediateDirectory)/src_gc90x_bt656$(ObjectSuffix): Source/ahmi/GClib/src/gc90x_bt656.c  
	$(CC) $(SourceSwitch) Source/ahmi/GClib/src/gc90x_bt656.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/src_gc90x_bt656$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gc90x_bt656$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/src_gc90x_bt656$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/src_gc90x_bt656$(PreprocessSuffix): Source/ahmi/GClib/src/gc90x_bt656.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/src_gc90x_bt656$(PreprocessSuffix) Source/ahmi/GClib/src/gc90x_bt656.c

$(IntermediateDirectory)/src_gc90x_cache$(ObjectSuffix): Source/ahmi/GClib/src/gc90x_cache.c  
	$(CC) $(SourceSwitch) Source/ahmi/GClib/src/gc90x_cache.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/src_gc90x_cache$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gc90x_cache$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/src_gc90x_cache$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/src_gc90x_cache$(PreprocessSuffix): Source/ahmi/GClib/src/gc90x_cache.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/src_gc90x_cache$(PreprocessSuffix) Source/ahmi/GClib/src/gc90x_cache.c

$(IntermediateDirectory)/src_gc90x_fix$(ObjectSuffix): Source/ahmi/GClib/src/gc90x_fix.c  
	$(CC) $(SourceSwitch) Source/ahmi/GClib/src/gc90x_fix.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/src_gc90x_fix$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gc90x_fix$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/src_gc90x_fix$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/src_gc90x_fix$(PreprocessSuffix): Source/ahmi/GClib/src/gc90x_fix.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/src_gc90x_fix$(PreprocessSuffix) Source/ahmi/GClib/src/gc90x_fix.c

$(IntermediateDirectory)/src_gc90x_shmemmanage$(ObjectSuffix): Source/ahmi/GClib/src/gc90x_shmemmanage.c  
	$(CC) $(SourceSwitch) Source/ahmi/GClib/src/gc90x_shmemmanage.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/src_gc90x_shmemmanage$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gc90x_shmemmanage$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/src_gc90x_shmemmanage$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/src_gc90x_shmemmanage$(PreprocessSuffix): Source/ahmi/GClib/src/gc90x_shmemmanage.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/src_gc90x_shmemmanage$(PreprocessSuffix) Source/ahmi/GClib/src/gc90x_shmemmanage.c

$(IntermediateDirectory)/src_gc90x_display$(ObjectSuffix): Source/ahmi/GClib/src/gc90x_display.cpp  
	$(CXX) $(SourceSwitch) Source/ahmi/GClib/src/gc90x_display.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/src_gc90x_display$(ObjectSuffix) -MF$(IntermediateDirectory)/src_gc90x_display$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/src_gc90x_display$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/src_gc90x_display$(PreprocessSuffix): Source/ahmi/GClib/src/gc90x_display.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/src_gc90x_display$(PreprocessSuffix) Source/ahmi/GClib/src/gc90x_display.cpp

$(IntermediateDirectory)/core_ITM1c1$(ObjectSuffix): Source/ahmi/itm/core/ITM1c1.c  
	$(CC) $(SourceSwitch) Source/ahmi/itm/core/ITM1c1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_ITM1c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_ITM1c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_ITM1c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_ITM1c1$(PreprocessSuffix): Source/ahmi/itm/core/ITM1c1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_ITM1c1$(PreprocessSuffix) Source/ahmi/itm/core/ITM1c1.c

$(IntermediateDirectory)/core_SBM1c1$(ObjectSuffix): Source/ahmi/sbm/core/SBM1c1.c  
	$(CC) $(SourceSwitch) Source/ahmi/sbm/core/SBM1c1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_SBM1c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_SBM1c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_SBM1c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_SBM1c1$(PreprocessSuffix): Source/ahmi/sbm/core/SBM1c1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_SBM1c1$(PreprocessSuffix) Source/ahmi/sbm/core/SBM1c1.c

$(IntermediateDirectory)/core_com_c1$(ObjectSuffix): Source/ahmi/com/core/com_c1.c  
	$(CC) $(SourceSwitch) Source/ahmi/com/core/com_c1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_com_c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_com_c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_com_c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_com_c1$(PreprocessSuffix): Source/ahmi/com/core/com_c1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_com_c1$(PreprocessSuffix) Source/ahmi/com/core/com_c1.c

$(IntermediateDirectory)/core_com_fifo$(ObjectSuffix): Source/ahmi/com/core/com_fifo.c  
	$(CC) $(SourceSwitch) Source/ahmi/com/core/com_fifo.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_com_fifo$(ObjectSuffix) -MF$(IntermediateDirectory)/core_com_fifo$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_com_fifo$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_com_fifo$(PreprocessSuffix): Source/ahmi/com/core/com_fifo.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_com_fifo$(PreprocessSuffix) Source/ahmi/com/core/com_fifo.c

$(IntermediateDirectory)/csi_ahmi_csi_ahmi$(ObjectSuffix): ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi.c  
	$(CC) $(SourceSwitch) ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_ahmi_csi_ahmi$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_ahmi_csi_ahmi$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_ahmi_csi_ahmi$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_ahmi_csi_ahmi$(PreprocessSuffix): ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_ahmi_csi_ahmi$(PreprocessSuffix) ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi.c

$(IntermediateDirectory)/csi_ahmi_csi_ahmi_bt656$(ObjectSuffix): ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_bt656.c  
	$(CC) $(SourceSwitch) ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_bt656.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_ahmi_csi_ahmi_bt656$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_ahmi_csi_ahmi_bt656$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_ahmi_csi_ahmi_bt656$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_ahmi_csi_ahmi_bt656$(PreprocessSuffix): ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_bt656.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_ahmi_csi_ahmi_bt656$(PreprocessSuffix) ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_bt656.c

$(IntermediateDirectory)/csi_ahmi_csi_ahmi_fix$(ObjectSuffix): ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_fix.c  
	$(CC) $(SourceSwitch) ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_fix.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_ahmi_csi_ahmi_fix$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_ahmi_csi_ahmi_fix$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_ahmi_csi_ahmi_fix$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_ahmi_csi_ahmi_fix$(PreprocessSuffix): ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_fix.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_ahmi_csi_ahmi_fix$(PreprocessSuffix) ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_fix.c

$(IntermediateDirectory)/csi_ahmi_csi_ahmi_hw$(ObjectSuffix): ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_hw.c  
	$(CC) $(SourceSwitch) ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_hw.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_ahmi_csi_ahmi_hw$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_ahmi_csi_ahmi_hw$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_ahmi_csi_ahmi_hw$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_ahmi_csi_ahmi_hw$(PreprocessSuffix): ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_hw.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_ahmi_csi_ahmi_hw$(PreprocessSuffix) ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_hw.c

$(IntermediateDirectory)/csi_ahmi_csi_ahmi_i2s$(ObjectSuffix): ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_i2s.c  
	$(CC) $(SourceSwitch) ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_i2s.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_ahmi_csi_ahmi_i2s$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_ahmi_csi_ahmi_i2s$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_ahmi_csi_ahmi_i2s$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_ahmi_csi_ahmi_i2s$(PreprocessSuffix): ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_i2s.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_ahmi_csi_ahmi_i2s$(PreprocessSuffix) ../../../../CommonSource/AHMI/csi_ahmi/csi_ahmi_i2s.c

$(IntermediateDirectory)/Touch_TouchTask$(ObjectSuffix): ../../../../CommonSource/AHMI/Touch/TouchTask.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Touch/TouchTask.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Touch_TouchTask$(ObjectSuffix) -MF$(IntermediateDirectory)/Touch_TouchTask$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Touch_TouchTask$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Touch_TouchTask$(PreprocessSuffix): ../../../../CommonSource/AHMI/Touch/TouchTask.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Touch_TouchTask$(PreprocessSuffix) ../../../../CommonSource/AHMI/Touch/TouchTask.cpp

$(IntermediateDirectory)/Widget_AlphaImg$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/AlphaImg.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/AlphaImg.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_AlphaImg$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_AlphaImg$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_AlphaImg$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_AlphaImg$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/AlphaImg.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_AlphaImg$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/AlphaImg.cpp

$(IntermediateDirectory)/Widget_AlphaSlide$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/AlphaSlide.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/AlphaSlide.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_AlphaSlide$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_AlphaSlide$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_AlphaSlide$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_AlphaSlide$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/AlphaSlide.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_AlphaSlide$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/AlphaSlide.cpp

$(IntermediateDirectory)/Widget_ButtonClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/ButtonClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/ButtonClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_ButtonClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_ButtonClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_ButtonClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_ButtonClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/ButtonClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_ButtonClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/ButtonClass.cpp

$(IntermediateDirectory)/Widget_ButtonSwitch$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/ButtonSwitch.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/ButtonSwitch.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_ButtonSwitch$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_ButtonSwitch$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_ButtonSwitch$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_ButtonSwitch$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/ButtonSwitch.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_ButtonSwitch$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/ButtonSwitch.cpp

$(IntermediateDirectory)/Widget_Chart$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/Chart.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/Chart.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_Chart$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_Chart$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_Chart$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_Chart$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/Chart.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_Chart$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/Chart.cpp

$(IntermediateDirectory)/Widget_CircleSliderClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/CircleSliderClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/CircleSliderClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_CircleSliderClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_CircleSliderClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_CircleSliderClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_CircleSliderClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/CircleSliderClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_CircleSliderClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/CircleSliderClass.cpp

$(IntermediateDirectory)/Widget_ClockBoardClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/ClockBoardClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/ClockBoardClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_ClockBoardClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_ClockBoardClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_ClockBoardClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_ClockBoardClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/ClockBoardClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_ClockBoardClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/ClockBoardClass.cpp

$(IntermediateDirectory)/Widget_ClockClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/ClockClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/ClockClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_ClockClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_ClockClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_ClockClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_ClockClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/ClockClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_ClockClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/ClockClass.cpp

$(IntermediateDirectory)/Widget_CombinationSwitch$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/CombinationSwitch.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/CombinationSwitch.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_CombinationSwitch$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_CombinationSwitch$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_CombinationSwitch$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_CombinationSwitch$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/CombinationSwitch.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_CombinationSwitch$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/CombinationSwitch.cpp

$(IntermediateDirectory)/Widget_DataImage$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/DataImage.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/DataImage.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_DataImage$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_DataImage$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_DataImage$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_DataImage$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/DataImage.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_DataImage$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/DataImage.cpp

$(IntermediateDirectory)/Widget_DynamicTexClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/DynamicTexClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/DynamicTexClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_DynamicTexClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_DynamicTexClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_DynamicTexClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_DynamicTexClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/DynamicTexClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_DynamicTexClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/DynamicTexClass.cpp

$(IntermediateDirectory)/Widget_GalleryClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/GalleryClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/GalleryClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_GalleryClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_GalleryClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_GalleryClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_GalleryClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/GalleryClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_GalleryClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/GalleryClass.cpp

$(IntermediateDirectory)/Widget_IOClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/IOClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/IOClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_IOClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_IOClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_IOClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_IOClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/IOClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_IOClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/IOClass.cpp

$(IntermediateDirectory)/Widget_KeyboardContainer$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/KeyboardContainer.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/KeyboardContainer.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_KeyboardContainer$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_KeyboardContainer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_KeyboardContainer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_KeyboardContainer$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/KeyboardContainer.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_KeyboardContainer$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/KeyboardContainer.cpp

$(IntermediateDirectory)/Widget_KnobClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/KnobClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/KnobClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_KnobClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_KnobClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_KnobClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_KnobClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/KnobClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_KnobClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/KnobClass.cpp

$(IntermediateDirectory)/Widget_MenuContainer$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/MenuContainer.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/MenuContainer.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_MenuContainer$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_MenuContainer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_MenuContainer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_MenuContainer$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/MenuContainer.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_MenuContainer$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/MenuContainer.cpp

$(IntermediateDirectory)/Widget_MeterClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/MeterClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/MeterClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_MeterClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_MeterClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_MeterClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_MeterClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/MeterClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_MeterClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/MeterClass.cpp

$(IntermediateDirectory)/Widget_movingTexture$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/movingTexture.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/movingTexture.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_movingTexture$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_movingTexture$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_movingTexture$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_movingTexture$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/movingTexture.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_movingTexture$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/movingTexture.cpp

$(IntermediateDirectory)/Widget_NumberClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/NumberClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/NumberClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_NumberClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_NumberClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_NumberClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_NumberClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/NumberClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_NumberClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/NumberClass.cpp

$(IntermediateDirectory)/Widget_OnOffTexture$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/OnOffTexture.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/OnOffTexture.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_OnOffTexture$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_OnOffTexture$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_OnOffTexture$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_OnOffTexture$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/OnOffTexture.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_OnOffTexture$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/OnOffTexture.cpp

$(IntermediateDirectory)/Widget_ProgBarClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/ProgBarClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/ProgBarClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_ProgBarClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_ProgBarClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_ProgBarClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_ProgBarClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/ProgBarClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_ProgBarClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/ProgBarClass.cpp

$(IntermediateDirectory)/Widget_QRcodeClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/QRcodeClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/QRcodeClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_QRcodeClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_QRcodeClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_QRcodeClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_QRcodeClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/QRcodeClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_QRcodeClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/QRcodeClass.cpp

$(IntermediateDirectory)/Widget_SelectorClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/SelectorClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/SelectorClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_SelectorClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_SelectorClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_SelectorClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_SelectorClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/SelectorClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_SelectorClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/SelectorClass.cpp

$(IntermediateDirectory)/Widget_SliderClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/SliderClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/SliderClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_SliderClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_SliderClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_SliderClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_SliderClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/SliderClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_SliderClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/SliderClass.cpp

$(IntermediateDirectory)/Widget_SwipeResponderClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/SwipeResponderClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/SwipeResponderClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_SwipeResponderClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_SwipeResponderClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_SwipeResponderClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_SwipeResponderClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/SwipeResponderClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_SwipeResponderClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/SwipeResponderClass.cpp

$(IntermediateDirectory)/Widget_TextClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/TextClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/TextClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_TextClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_TextClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_TextClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_TextClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/TextClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_TextClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/TextClass.cpp

$(IntermediateDirectory)/Widget_TextInput$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/TextInput.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/TextInput.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_TextInput$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_TextInput$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_TextInput$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_TextInput$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/TextInput.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_TextInput$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/TextInput.cpp

$(IntermediateDirectory)/Widget_TextureNumberClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/TextureNumberClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/TextureNumberClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_TextureNumberClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_TextureNumberClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_TextureNumberClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_TextureNumberClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/TextureNumberClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_TextureNumberClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/TextureNumberClass.cpp

$(IntermediateDirectory)/Widget_TextureTimeClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/TextureTimeClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/TextureTimeClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_TextureTimeClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_TextureTimeClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_TextureTimeClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_TextureTimeClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/TextureTimeClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_TextureTimeClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/TextureTimeClass.cpp

$(IntermediateDirectory)/Widget_TouchTrack$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/TouchTrack.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/TouchTrack.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_TouchTrack$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_TouchTrack$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_TouchTrack$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_TouchTrack$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/TouchTrack.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_TouchTrack$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/TouchTrack.cpp

$(IntermediateDirectory)/Widget_VideoClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/VideoClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/VideoClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_VideoClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_VideoClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_VideoClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_VideoClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/VideoClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_VideoClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/VideoClass.cpp

$(IntermediateDirectory)/Widget_WriteTexture$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/WriteTexture.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/WriteTexture.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_WriteTexture$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_WriteTexture$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_WriteTexture$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_WriteTexture$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/WriteTexture.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_WriteTexture$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/WriteTexture.cpp

$(IntermediateDirectory)/Widget_BarChart$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/BarChart.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/BarChart.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_BarChart$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_BarChart$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_BarChart$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_BarChart$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/BarChart.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_BarChart$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/BarChart.cpp

$(IntermediateDirectory)/Widget_Gif$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/Gif.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/Gif.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_Gif$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_Gif$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_Gif$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_Gif$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/Gif.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_Gif$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/Gif.cpp

$(IntermediateDirectory)/Widget_PieChart$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/PieChart.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/PieChart.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_PieChart$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_PieChart$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_PieChart$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_PieChart$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/PieChart.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_PieChart$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/PieChart.cpp

$(IntermediateDirectory)/Widget_Custom$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/Custom.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/Custom.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_Custom$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_Custom$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_Custom$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_Custom$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/Custom.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_Custom$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/Custom.cpp

$(IntermediateDirectory)/Widget_ListClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/ListClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/ListClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_ListClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_ListClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_ListClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_ListClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/ListClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_ListClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/ListClass.cpp

$(IntermediateDirectory)/Widget_ThreeDimensionalClass$(ObjectSuffix): ../../../../CommonSource/AHMI/Widget/ThreeDimensionalClass.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Widget/ThreeDimensionalClass.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/Widget_ThreeDimensionalClass$(ObjectSuffix) -MF$(IntermediateDirectory)/Widget_ThreeDimensionalClass$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/Widget_ThreeDimensionalClass$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/Widget_ThreeDimensionalClass$(PreprocessSuffix): ../../../../CommonSource/AHMI/Widget/ThreeDimensionalClass.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/Widget_ThreeDimensionalClass$(PreprocessSuffix) ../../../../CommonSource/AHMI/Widget/ThreeDimensionalClass.cpp

$(IntermediateDirectory)/csi_hal_core_csi_hal_core$(ObjectSuffix): ../../../../CommonSource/csi_hal/csi_hal_core/csi_hal_core.c  
	$(CC) $(SourceSwitch) ../../../../CommonSource/csi_hal/csi_hal_core/csi_hal_core.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_hal_core_csi_hal_core$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_hal_core_csi_hal_core$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_hal_core_csi_hal_core$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_hal_core_csi_hal_core$(PreprocessSuffix): ../../../../CommonSource/csi_hal/csi_hal_core/csi_hal_core.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_hal_core_csi_hal_core$(PreprocessSuffix) ../../../../CommonSource/csi_hal/csi_hal_core/csi_hal_core.c

$(IntermediateDirectory)/csi_hal_driver_csi_hal_driver_flash$(ObjectSuffix): ../../../../CommonSource/csi_hal/csi_hal_driver/csi_hal_driver_flash.c  
	$(CC) $(SourceSwitch) ../../../../CommonSource/csi_hal/csi_hal_driver/csi_hal_driver_flash.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_hal_driver_csi_hal_driver_flash$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_hal_driver_csi_hal_driver_flash$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_hal_driver_csi_hal_driver_flash$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_hal_driver_csi_hal_driver_flash$(PreprocessSuffix): ../../../../CommonSource/csi_hal/csi_hal_driver/csi_hal_driver_flash.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_hal_driver_csi_hal_driver_flash$(PreprocessSuffix) ../../../../CommonSource/csi_hal/csi_hal_driver/csi_hal_driver_flash.c

$(IntermediateDirectory)/csi_hal_driver_ahmi_flash_driver_interface$(ObjectSuffix): ../../../../CommonSource/csi_hal/csi_hal_driver/ahmi_flash_driver_interface.c  
	$(CC) $(SourceSwitch) ../../../../CommonSource/csi_hal/csi_hal_driver/ahmi_flash_driver_interface.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_hal_driver_ahmi_flash_driver_interface$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_hal_driver_ahmi_flash_driver_interface$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_hal_driver_ahmi_flash_driver_interface$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_hal_driver_ahmi_flash_driver_interface$(PreprocessSuffix): ../../../../CommonSource/csi_hal/csi_hal_driver/ahmi_flash_driver_interface.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_hal_driver_ahmi_flash_driver_interface$(PreprocessSuffix) ../../../../CommonSource/csi_hal/csi_hal_driver/ahmi_flash_driver_interface.c

$(IntermediateDirectory)/csi_hal_isr_csi_hal_isr$(ObjectSuffix): ../../../../CommonSource/csi_hal/csi_hal_isr/csi_hal_isr.c  
	$(CC) $(SourceSwitch) ../../../../CommonSource/csi_hal/csi_hal_isr/csi_hal_isr.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/csi_hal_isr_csi_hal_isr$(ObjectSuffix) -MF$(IntermediateDirectory)/csi_hal_isr_csi_hal_isr$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/csi_hal_isr_csi_hal_isr$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/csi_hal_isr_csi_hal_isr$(PreprocessSuffix): ../../../../CommonSource/csi_hal/csi_hal_isr/csi_hal_isr.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/csi_hal_isr_csi_hal_isr$(PreprocessSuffix) ../../../../CommonSource/csi_hal/csi_hal_isr/csi_hal_isr.c

$(IntermediateDirectory)/core_em_c1$(ObjectSuffix): ../../../../CommonSource/EM/core/em_c1.c  
	$(CC) $(SourceSwitch) ../../../../CommonSource/EM/core/em_c1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_em_c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_em_c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_em_c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_em_c1$(PreprocessSuffix): ../../../../CommonSource/EM/core/em_c1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_em_c1$(PreprocessSuffix) ../../../../CommonSource/EM/core/em_c1.c

$(IntermediateDirectory)/core_sm_c1$(ObjectSuffix): ../../../../CommonSource/SM/core/sm_c1.c  
	$(CC) $(SourceSwitch) ../../../../CommonSource/SM/core/sm_c1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_sm_c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_sm_c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_sm_c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_sm_c1$(PreprocessSuffix): ../../../../CommonSource/SM/core/sm_c1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_sm_c1$(PreprocessSuffix) ../../../../CommonSource/SM/core/sm_c1.c

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

$(IntermediateDirectory)/core_adcshare_c1$(ObjectSuffix): Source/ahmi/adcshare/core/adcshare_c1.c  
	$(CC) $(SourceSwitch) Source/ahmi/adcshare/core/adcshare_c1.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_adcshare_c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_adcshare_c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_adcshare_c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_adcshare_c1$(PreprocessSuffix): Source/ahmi/adcshare/core/adcshare_c1.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_adcshare_c1$(PreprocessSuffix) Source/ahmi/adcshare/core/adcshare_c1.c

$(IntermediateDirectory)/core_player1c1$(ObjectSuffix): ../../../../CommonSource/AHMI/Player/core/player1c1.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/Player/core/player1c1.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/core_player1c1$(ObjectSuffix) -MF$(IntermediateDirectory)/core_player1c1$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/core_player1c1$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/core_player1c1$(PreprocessSuffix): ../../../../CommonSource/AHMI/Player/core/player1c1.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/core_player1c1$(PreprocessSuffix) ../../../../CommonSource/AHMI/Player/core/player1c1.cpp

$(IntermediateDirectory)/source_sxClip$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxClip.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxClip.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxClip$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxClip$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxClip$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxClip$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxClip.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxClip$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxClip.cpp

$(IntermediateDirectory)/source_sxCull$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxCull.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxCull.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxCull$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxCull$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxCull$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxCull$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxCull.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxCull$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxCull.cpp

$(IntermediateDirectory)/source_sxGLApplication$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLApplication.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxGLApplication.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxGLApplication$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxGLApplication$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxGLApplication$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxGLApplication$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLApplication.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxGLApplication$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxGLApplication.cpp

$(IntermediateDirectory)/source_sxGLBuffer$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLBuffer.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxGLBuffer.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxGLBuffer$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxGLBuffer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxGLBuffer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxGLBuffer$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLBuffer.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxGLBuffer$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxGLBuffer.cpp

$(IntermediateDirectory)/source_sxGLMath$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLMath.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxGLMath.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxGLMath$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxGLMath$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxGLMath$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxGLMath$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLMath.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxGLMath$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxGLMath.cpp

$(IntermediateDirectory)/source_sxGLObject$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLObject.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxGLObject.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxGLObject$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxGLObject$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxGLObject$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxGLObject$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLObject.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxGLObject$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxGLObject.cpp

$(IntermediateDirectory)/source_sxGLObjIn$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLObjIn.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxGLObjIn.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxGLObjIn$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxGLObjIn$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxGLObjIn$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxGLObjIn$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLObjIn.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxGLObjIn$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxGLObjIn.cpp

$(IntermediateDirectory)/source_sxGLOperator$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLOperator.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxGLOperator.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxGLOperator$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxGLOperator$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxGLOperator$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxGLOperator$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLOperator.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxGLOperator$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxGLOperator.cpp

$(IntermediateDirectory)/source_sxGLPipeline$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLPipeline.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxGLPipeline.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxGLPipeline$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxGLPipeline$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxGLPipeline$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxGLPipeline$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLPipeline.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxGLPipeline$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxGLPipeline.cpp

$(IntermediateDirectory)/source_sxGLSourceBuffer$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLSourceBuffer.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxGLSourceBuffer.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxGLSourceBuffer$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxGLSourceBuffer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxGLSourceBuffer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxGLSourceBuffer$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLSourceBuffer.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxGLSourceBuffer$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxGLSourceBuffer.cpp

$(IntermediateDirectory)/source_sxGLState$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLState.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxGLState.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxGLState$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxGLState$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxGLState$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxGLState$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGLState.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxGLState$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxGLState.cpp

$(IntermediateDirectory)/source_sxGL_draw$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGL_draw.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxGL_draw.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxGL_draw$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxGL_draw$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxGL_draw$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxGL_draw$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxGL_draw.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxGL_draw$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxGL_draw.cpp

$(IntermediateDirectory)/source_sxLight$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxLight.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxLight.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxLight$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxLight$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxLight$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxLight$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxLight.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxLight$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxLight.cpp

$(IntermediateDirectory)/source_sxMatrix$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxMatrix.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxMatrix.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxMatrix$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxMatrix$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxMatrix$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxMatrix$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxMatrix.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxMatrix$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxMatrix.cpp

$(IntermediateDirectory)/source_sxTexture$(ObjectSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxTexture.cpp  
	$(CXX) $(SourceSwitch) ../../../../CommonSource/AHMI/sxgl/source/sxTexture.cpp $(CXXFLAGS) -MMD -MP -MT$(IntermediateDirectory)/source_sxTexture$(ObjectSuffix) -MF$(IntermediateDirectory)/source_sxTexture$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/source_sxTexture$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/source_sxTexture$(PreprocessSuffix): ../../../../CommonSource/AHMI/sxgl/source/sxTexture.cpp
	$(CXX) $(CXXFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/source_sxTexture$(PreprocessSuffix) ../../../../CommonSource/AHMI/sxgl/source/sxTexture.cpp

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
