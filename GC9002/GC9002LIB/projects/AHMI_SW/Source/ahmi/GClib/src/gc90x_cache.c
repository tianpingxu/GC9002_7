
#include "gc90x_cache.h"

void DCache_Flush(void)
{
	if(*(uint32_t *)(0x40200030) == 1)
	{
		ccm_FlushDCache();
		//ccm_FlushDCache_by_Addr(0x0200c000, 0x4000);
	}
}

void DCache_Invalid(void)
{
	if(*(uint32_t *)(0x40200030) == 1)
	{
		ccm_InvalidateDCache();
	}
}