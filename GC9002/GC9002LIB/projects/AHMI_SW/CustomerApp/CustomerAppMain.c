/* Customer Application Enterance File */

#include "CustomerAppMain.h"

extern void default_attr_init(void);

extern void read_page_init(void);

extern void cmd_send_init(void);

void CustApp_vInitEnterance(void)
{
    default_attr_init();
    cmd_send_init();
    read_page_init();

}




