
#ifndef __GCHEAP_H__
#define __GCHEAP_H__
#include <stdint.h>
#include <stdio.h>
#include "FreeRTOSConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined (CHIP_GC9002_NoHyper)
#define gcConfigTOTAL_HEAP_SIZE     ( ( size_t ) ( 0x0000 ) )//( 0 * 1024 )
#define gcConfigTOTAL_HEAP_ADDR     ( 0x0200B000 )
#elif defined (CHIP_GC9002_CodeHyper)
#define gcConfigTOTAL_HEAP_SIZE     ( ( size_t ) ( 0x300000 ) )//( 3 * 1024 * 1024 )
#define gcConfigTOTAL_HEAP_ADDR     ( 0x20100000 )
#else
#define gcConfigTOTAL_HEAP_SIZE     ( ( size_t ) ( 0x3B0000 ) )//( 3 * 1024 * 1024 + 704 * 1024 )
#define gcConfigTOTAL_HEAP_ADDR     ( 0x20050000 )
#endif

#define gcBYTE_ALIGNMENT			8
#define gcBYTE_ALIGNMENT_MASK       ( 0x0007 )

#define gcTraceMALLOC( pvAddress, uiSize )
#define gcCOVERAGE_TEST_MARKER()
#define gcTraceFREE( pvAddress, uiSize )


#define gcTaskSuspendAll()
#define gcTaskResumeAll()
#define gcDISABLE_INTERRUPTS()
#define gcConfigASSERT( x ) if( ( x ) == 0 ) { gcDISABLE_INTERRUPTS(); for( ;; ); }

/* Block sizes must not get too small. */
#define gcHeapMINIMUM_BLOCK_SIZE	( ( size_t ) ( gcHeapStructSize << 1 ) )

/* Assumes 8bit bytes! */
#define gcHeapBITS_PER_BYTE		( ( size_t ) 8 ) 

/* Define the linked list structure.  This is used to link free blocks in order
of their memory address. */
typedef struct gc_BLOCK_LINK
{
	struct gc_BLOCK_LINK *pxNextFreeBlock;	/*<< The next free block in the list. */
	size_t xBlockSize;						/*<< The size of the free block. */
} BlockLink_t;

class GCHeap
{
public:
    uint8_t*        pHeap;
    size_t          gcHeapSize;
    BlockLink_t     xStart;
    BlockLink_t*    pxEnd;
    size_t          gcFreeBytesRemaining;
    size_t          gcMinimumEverFreeBytesRemaining;
    size_t          gcBlockAllocatedBit;

    GCHeap();
    GCHeap(uint8_t* heapaddr, size_t size);
    ~GCHeap();
    void init(uint8_t* heapaddr, size_t size);
    void *malloc(size_t gcWantedSize );
    void *calloc(size_t n, size_t size);
    void *realloc(void *pv, size_t newsize);
    void free(void *pv );
    size_t getFreeHeapSize();
    size_t getMinimumEverFreeHeapSize();
};

//void gcHeapInfoInit(GCHeapInfo* pHeapInfo, uint8_t* heapaddr, size_t size);
//void *gcPortMalloc(GCHeapInfo* pHeapInfo, size_t gcWantedSize );
//void *gcPortCalloc(GCHeapInfo* pHeapInfo, size_t n, size_t size);
//void *gcPortRealloc(GCHeapInfo* pHeapInfo, void *pv, size_t newsize);
//void gcPortFree(GCHeapInfo* pHeapInfo, void *pv );

extern GCHeap gcheap;
#define gcInit        	gcheap.init
#define gcMalloc        gcheap.malloc
#define gcCalloc        gcheap.calloc
#define gcRealloc       gcheap.realloc
#define gcFree          gcheap.free

//extern GCHeapInfo gcheapInfo;
//#define gcMalloc(x)     gcPortMalloc(&gcheapInfo, x )
//#define gcCalloc(x, y)  gcPortCalloc(&gcheapInfo, x , y)
//#define gcRealloc(x, y) gcPortRealloc(&gcheapInfo, x , y)
//#define gcFree(x)       gcPortFree(&gcheapInfo, x )


//extern GCHeapInfo* ttheapInfo;
//#define FTMalloc(x)     gcPortMalloc(ttheapInfo, x )
//#define FTCalloc(x, y)  gcPortCalloc(ttheapInfo, x , y)
//#define FTRealloc(x, y) gcPortRealloc(ttheapInfo, x , y)
//#define FTFree(x)       gcPortFree(ttheapInfo, x )
    
#ifdef __cplusplus
}
#endif

#endif
