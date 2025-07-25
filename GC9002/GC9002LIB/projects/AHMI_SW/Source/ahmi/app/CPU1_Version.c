//const char CPU1Version[32] __attribute__((at(0x08040790))) = "1.10.18_build_01.31.16.45"; 
//static const char CPU1_app_compile_date[32] __attribute__((at(0x080407B0))) = __DATE__; 
//static const char CPU1_app_compile_time[32] __attribute__((at(0x080407D0))) = __TIME__;
//static const char CPU1_code_1k_align[16] __attribute__((at(0x080407F0))) = "RAM";

const char CPU1Version[32] __attribute__((section(".version"))) = "1.10.19_build_2024.10.29.17.00";
const char CPU1_app_compile_date[16] __attribute__((section(".compiledate"))) = __DATE__; 
const char CPU1_app_compile_time[16] __attribute__((section(".compiletime"))) = __TIME__;

#ifdef CHIP_GC9002_NoHyper
const char CPU1_app_compile_Addr[32] __attribute__((section(".compileAddr"))) = "NoHyperForGC9002";
#elif defined (CHIP_GC9002_CodeHyper)
const char CPU1_app_compile_Addr[32] __attribute__((section(".compileAddr"))) = "CodeHyperForGC9002";
#else
const char CPU1_app_compile_Addr[32] __attribute__((section(".compileAddr"))) = "GC9002";
#endif
