#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MAIN            0
#define TEST_TEMPLE_BSP 1
#define TEST_SAMPLE_CSI 2
#define TEST_AT         3
#define TEST_OTHER_APP  0xFF

#define BSP_DEBUG       TEST_TEMPLE_BSP

int main(void);
void kernel_start_in_debug(void);

#ifdef __cplusplus
}
#endif

#endif