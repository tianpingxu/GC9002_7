/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     gcc_csky.ld
 * @brief    csky linker file
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
MEMORY
{
	I-SRAM : ORIGIN = 0x20000000 , LENGTH = 0xB0000   /* I-SRAM  704KB */
	D-SRAM : ORIGIN = 0x200B0000 , LENGTH = 0x50000   /* D-SRAM  320KB */
	O-SRAM : ORIGIN = 0x02000000 , LENGTH = 0x8000    /* on-chip SRAM 32KB */
	GC-SRAM : ORIGIN = 0x20100000 , LENGTH = 0x300000   /* GC-SRAM  3MB */
}

__min_heap_size = 0x000;
/*PROVIDE (__ram_end  = 0x02000400);
PROVIDE (__heap_end = __ram_end);*/

REGION_ALIAS("REGION_TEXT",    I-SRAM);
REGION_ALIAS("REGION_RODATA",  I-SRAM);
REGION_ALIAS("REGION_DATA",    D-SRAM);
REGION_ALIAS("REGION_BSS",     D-SRAM);
REGION_ALIAS("REGION_SRAM",    D-SRAM);
REGION_ALIAS("REGION_FLASH",   O-SRAM);
REGION_ALIAS("REGION_GC",      GC-SRAM);

ENTRY(Reset_Handler)
SECTIONS
{
 .text : {
  . = ALIGN(0x4) ;
  KEEP (*(SORT(.version)))
  KEEP (*(SORT(.compiledate)))
  KEEP (*(SORT(.compiletime)))
  KEEP (*(SORT(.compileAddr)))
  . = ALIGN(0x4) ;
  __stext = . ;
  KEEP(*startup.o(*.text*))
  *(.text)
  *(.text*)
  *(.text.*)
  *(.gnu.warning)
  *(.stub)
  *(.gnu.linkonce.t*)
  *(.glue_7t)
  *(.glue_7)
  *(.jcr)
  KEEP (*(.init))
  KEEP (*(.fini))
  . = ALIGN (4) ;
  PROVIDE(__ctbp = .);
  *(.call_table_data)
  *(.call_table_text)
  . = ALIGN(0x10) ;
   __usbh_class_info_start__ = .;
  KEEP(*(.usbh_class_info))
  __usbh_class_info_end__ = .;
  __etext = . ;
 } > REGION_TEXT
 .eh_frame_hdr : {
  *(.eh_frame_hdr)
 } > REGION_TEXT
 .eh_frame : ONLY_IF_RO {
  KEEP (*(.eh_frame))
 } > REGION_TEXT
 .gcc_except_table : ONLY_IF_RO {
  *(.gcc_except_table .gcc_except_table.*)
 } > REGION_TEXT
 
 .rodata : {
  . = ALIGN(0x4) ;
  __srodata = .;
  *(.rdata)
  *(.rdata*)
  *(.rdata1)
  *(.rdata.*)
  *(.rodata)
  *(.rodata1)
  *(.rodata*)
  *(.rodata.*)
  *(.rodata.str1.4)
  *(.srodata*)
  . = ALIGN(0x4) ;
  __ctor_start__ = .;
  KEEP (*(SORT(.ctors.*)))
  KEEP (*(.ctors))
  __ctor_end__ = .;
  KEEP (*(SORT(.dtors.*)))
  KEEP (*(.dtors))
  __dtor_end__ = .;
  . = ALIGN(0x4) ;
  __erodata_in = .;
  __rodata_end__ = .;
 } > REGION_RODATA
 
 .data_in : {
  . = ALIGN(0x4) ;
  __sdata = . ;
  __data_in_start__ = . ;
  data_in_start = . ;
  KEEP(*startup.o(*.vectors*))
  *(.got.plt)
  *(.got)
  *(.gnu.linkonce.r*)
  *lib_a*.o(.data .data* .data1 .data.*)
  *startup.o(.data .data* .data1 .data.*)
  *system.o(.data .data* .data1 .data.*)
  *trap_c.o(.data .data* .data1 .data.*)
  *vectors.o(.data .data* .data1 .data.*)
  *gc9002_power.o(.data .data* .data1 .data.*)
  *gc9002_hal_hyper.o(.data .data* .data1 .data.*)
  *(.gnu.linkonce.d*)
  *(.gcc_except_table)
  *(.gcc_except_table*)
  __start_init_call = .;
  *(.initcall.init)
  __stop_init_call = .;
  __start_cmd = .;
  *(.bootloaddata.cmd)
  . = ALIGN(4) ;
  __stop_cmd = .;
  __global_pointer$ = .;
  *lib_a*.o(.sdata .sdata.* .sdata2.*)
  *startup.o(.sdata .sdata.* .sdata2.*)
  *system.o(.sdata .sdata.* .sdata2.*)
  *trap_c.o(.sdata .sdata.* .sdata2.*)
  *vectors.o(.sdata .sdata.* .sdata2.*)
  *gc9002_power.o(.sdata .sdata.* .sdata2.*)
  *gc9002_hal_hyper.o(.sdata .sdata.* .sdata2.*)
  *(.gnu.linkonce.s.*)
  *(__libc_atexit)
  *(__libc_subinit)
  *(__libc_subfreeres)
  *(.note.ABI-tag)
  __edata = .;
  __data_in_end__ = .;
  . = ALIGN(0x4) ;
 } > REGION_DATA AT > REGION_RODATA
 
 .eh_frame : ONLY_IF_RW {
  KEEP (*(.eh_frame))
 } > REGION_DATA AT > REGION_RODATA
 
 .gcc_except_table : ONLY_IF_RW {
  *(.gcc_except_table .gcc_except_table.*)
  __edata = .;
  __data_end__ = .;
 } > REGION_DATA AT > REGION_RODATA
 
 .bss_in : {
  . = ALIGN(0x4) ;
  __sbss = ALIGN(0x4) ;
  __bss_in_start__ = . ;
  *(.dynsbss)
  *(.scommon)
  *(.dynbss)
  *lib_a*.o(.bss .bss.* .sbss .sbss.*)
  . = ALIGN(0x4) ;
  *startup.o(.bss .bss.* .sbss .sbss.*)
  . = ALIGN(0x4) ;
  *system.o(.bss .bss.* .sbss .sbss.*)
  . = ALIGN(0x4) ;
  *trap_c.o(.bss .bss.* .sbss .sbss.*)
  . = ALIGN(0x4) ;
  *vectors.o(.bss .bss.* .sbss .sbss.*)
  . = ALIGN(0x4) ;
  *gc9002_power.o(.bss .bss.* .sbss .sbss.*)
  . = ALIGN(0x4) ;
  *gc9002_hal_hyper.o(.bss .bss.* .sbss .sbss.*)
  . = ALIGN(0x4) ;
  *(COMMON)
  . = ALIGN(0x4) ;
  __ebss = . ;
  __bss_in_end__ = .;
  __end = . ;
  end = . ;
 } > REGION_BSS AT > REGION_RODATA
 
 ._user_heap : {
  . = ALIGN(0x4) ;
  __heap_start = .;
  . += __min_heap_size;
  . = ALIGN(0x4) ;
  __heap_end = .;
 } > REGION_BSS AT > REGION_RODATA
 
 
  .exalign         :
  {
    . = ALIGN(4);
    __erodata_ex = .;;
  } >REGION_TEXT
 
 .data_ex : {
  . = ALIGN(0x4) ;
  __sdata = . ;
  __data_ex_start__ = . ;
  data_ex_start = . ;
  *(.data)
  *(.data*)
  *(.data1)
  *(.data.*)
  *(.data1)
  . = ALIGN(4) ;
  *(.sdata)
  *(.sdata.*)
  *(.sdata2.*)
  __edata = .;
  __data_ex_end__ = .;
  . = ALIGN(0x4) ;
 } > REGION_SRAM AT > REGION_RODATA
 
  .bss_ex : {
  . = ALIGN(0x4) ;
  __sbss = ALIGN(0x4) ;
  __bss_ex_start__ = . ;
  *(.sbss)
  *(.sbss.*)
  *(.bss)
  *(.bss.*)
  *(COMMON)
  . = ALIGN(0x4) ;
  __ebss = . ;
  __bss_ex_end__ = .;
  __end = . ;
  end = . ;
 } > REGION_SRAM AT > REGION_RODATA
 
  .ilalign         :
  {
    . = ALIGN(4);
    PROVIDE( __e_f_text = . );
  } >REGION_TEXT 

 .flashdriver      :
  {
    __f_code_start = .;
    KEEP (*(SORT_NONE(.flashdriver)))
    . = ALIGN(4);
/*    KEEP (*(SORT_NONE(.dramcachecode)))
    . = ALIGN(4); */
    __f_code_end = .;
/*  } >REGION_DATA AT>REGION_TEXT */
  } >REGION_FLASH AT>REGION_TEXT 
  
   .bss_gc   (NOLOAD)         :
  {
    *(.gcheap)
  } >REGION_GC AT>REGION_GC
}
