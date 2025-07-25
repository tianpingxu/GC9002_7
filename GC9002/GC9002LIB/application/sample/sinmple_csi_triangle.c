#include "sample_def.h"
#include "csi_triangle.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_TRIANGLE_

        
int main(void)
{
    int value_int;
    float value_float;
    while(1)
    {
        value_int = csi_sin_fixed(((int64_t)135 << 24));
        value_int = csi_sin_fixed((30 << 24));
        value_int = csi_sin_fixed((30 << 24) + 0x800000);

        value_int = csi_cos_fixed((60 << 24));
        value_int = csi_cos_fixed((59 << 24) + 0x800000);
        
        value_int = csi_arctan_fixed((1 << 16));
        value_int = csi_arctan_fixed((1 << 16) + 0x8000);

        value_float = csi_sin(-120);
        value_float = csi_sin(-135);
        value_float = csi_sin(-150);
        value_float = csi_sin(210);

        value_float = csi_sin(30.0);
        value_float = csi_sin(30.5);
        
        value_float = csi_cos(60.0);
        value_float = csi_cos(59.5);
        
        value_float = csi_arctan(1.0);
        value_float = csi_arctan(1.5);
		mdelay(100);
    }
    while(1)
    {
		mdelay(100);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_TRIANGLE_ */
#endif /* BSP_DEBUG */