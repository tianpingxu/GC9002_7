


#ifndef __QRMEMORY_H__
#define __QRMEMORY_H__
#include "platform.h"
#if defined(EMBEDDED) && defined(VERSION_2)
#include "gc90x_heap.h"
#endif

#if defined(__cplusplus)
extern "C" {
#endif
//extern GCHeapInfo* qrheapInfo;
//GCHeapInfo* qrheapInfo;
//GCHeap* qrheap;

#if defined(EMBEDDED) && defined(VERSION_2)
extern   GCHeap* qrheap;

#define QRMalloc        qrheap->malloc
#define QRCalloc        qrheap->calloc
#define QRRealloc       qrheap->realloc
#define QRFree          qrheap->free

#define qr_malloc       QRMalloc
#define qr_calloc       QRCalloc
#define qr_realloc      QRRealloc
#define qr_free         QRFree
#endif

#if (defined(PC_SIM) && defined(VERSION_2))
#define qr_malloc       malloc
#define qr_calloc       calloc
#define qr_realloc      realloc
#define qr_free         free
#endif

#if defined(__cplusplus)
}
#endif
    
#endif
