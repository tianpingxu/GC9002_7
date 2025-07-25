#ifndef GC90X_HAL_USBD_H
#define GC90X_HAL_USBD_H

#include <stdint.h>

void cdc_acm_init(void);

bool cdc_acm_data_send_with_dtr_test(void);

#endif