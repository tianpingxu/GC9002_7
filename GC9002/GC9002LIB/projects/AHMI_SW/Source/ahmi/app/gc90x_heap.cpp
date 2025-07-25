/*
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. If you wish to use our Amazon
 * FreeRTOS name, please do so in a fair use way that does not cause confusion.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 * 1 tab == 4 spaces!
 */

/*
 * A sample implementation of pvPortMalloc() and vPortFree() that combines
 * (coalescences) adjacent memory blocks as they are freed, and in so doing
 * limits memory fragmentation.
 *
 * See heap_1.c, heap_2.c and heap_3.c for alternative implementations, and the
 * memory management pages of http://www.FreeRTOS.org for more information.
 */


/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
all the API functions to use the MPU wrappers.  That should only be done when
task.h is included from an application file. */

#include<string.h>
#include "gc90x_heap.h"


#ifndef CHIP_GC9002_NoHyper
/* Allocate the memory for the heap. */
uint8_t gcHeap[ gcConfigTOTAL_HEAP_SIZE ]__attribute__((section(".gcheap")));

/*-----------------------------------------------------------*/

/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void gcInsertBlockIntoFreeList(GCHeap* pHeapInfo, BlockLink_t *pxBlockToInsert );

/*
 * Called automatically to setup the required heap structures the first time
 * pvPortMalloc() is called.
 */

static void gcHeapInit( GCHeap* pHeapInfo );

/*-----------------------------------------------------------*/

/* The size of the structure placed at the beginning of each allocated memory
block must by correctly byte aligned. */
static const size_t gcHeapStructSize	= ( sizeof( BlockLink_t ) + ( ( size_t ) ( gcBYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) gcBYTE_ALIGNMENT_MASK );

///* Create a couple of list links to mark the start and end of the list. */
//static BlockLink_t xStart, *pxEnd = NULL;

///* Keeps track of the number of free bytes remaining, but says nothing about
//fragmentation. */
//static size_t gcFreeBytesRemaining = 0U;
//static size_t gcMinimumEverFreeBytesRemaining = 0U;

///* Gets set to the top bit of an size_t type.  When this bit in the xBlockSize
//member of an BlockLink_t structure is set then the block belongs to the
//application.  When the bit is free the block is still part of the free heap
//space. */
//static size_t gcBlockAllocatedBit = 0;



GCHeap::GCHeap()
{
    
}
GCHeap::GCHeap(uint8_t* heapaddr, size_t size)
{
    memset((void*)this, 0, sizeof(GCHeap));
    
    this->pHeap = heapaddr;
    this->gcHeapSize = size;
    this->xStart.pxNextFreeBlock = NULL;
    this->pxEnd = NULL; 
}

GCHeap::~GCHeap()
{
    
}

GCHeap gcheap(gcHeap, gcConfigTOTAL_HEAP_SIZE);
//= {
//    gcHeap,
//    gcConfigTOTAL_HEAP_ADDR,
//};

/*-----------------------------------------------------------*/

void GCHeap::init( uint8_t* heapaddr, size_t size)
{
    memset((void*)this, 0, sizeof(GCHeap));
    
    this->pHeap = heapaddr;
    this->gcHeapSize = size;
    this->xStart.pxNextFreeBlock = NULL;
    this->pxEnd = NULL;      
	return;
}

/*-----------------------------------------------------------*/

void* GCHeap::malloc(size_t gcWantedSize )
{
BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
void *pvReturn = NULL;

	gcTaskSuspendAll();
	{
		/* If this is the first call to malloc then the heap will require
		initialisation to setup the list of free blocks. */
		if( this->pxEnd == NULL )
		{
			gcHeapInit(this);
		}
		else
		{
			gcCOVERAGE_TEST_MARKER();
		}

		/* Check the requested block size is not so large that the top bit is
		set.  The top bit of the block size member of the BlockLink_t structure
		is used to determine who owns the block - the application or the
		kernel, so it must be free. */
		if( ( gcWantedSize & this->gcBlockAllocatedBit ) == 0 )
		{
			/* The wanted size is increased so it can contain a BlockLink_t
			structure in addition to the requested amount of bytes. */
			if( gcWantedSize > 0 )
			{
				gcWantedSize += gcHeapStructSize;

				/* Ensure that blocks are always aligned to the required number
				of bytes. */
				if( ( gcWantedSize & gcBYTE_ALIGNMENT_MASK ) != 0x00 )
				{
					/* Byte alignment required. */
					gcWantedSize += ( gcBYTE_ALIGNMENT - ( gcWantedSize & gcBYTE_ALIGNMENT_MASK ) );
					gcConfigASSERT( ( gcWantedSize & gcBYTE_ALIGNMENT_MASK ) == 0 );
				}
				else
				{
					gcCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				gcCOVERAGE_TEST_MARKER();
			}

			if( ( gcWantedSize > 0 ) && ( gcWantedSize <= this->gcFreeBytesRemaining ) )
			{
				/* Traverse the list from the start	(lowest address) block until
				one	of adequate size is found. */
				pxPreviousBlock = &(this->xStart);
				pxBlock = this->xStart.pxNextFreeBlock;
				while( ( pxBlock->xBlockSize < gcWantedSize ) && ( pxBlock->pxNextFreeBlock != NULL ) )
				{
					pxPreviousBlock = pxBlock;
					pxBlock = pxBlock->pxNextFreeBlock;
				}

				/* If the end marker was reached then a block of adequate size
				was	not found. */
				if( pxBlock != this->pxEnd )
				{
					/* Return the memory space pointed to - jumping over the
					BlockLink_t structure at its start. */
					pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->pxNextFreeBlock ) + gcHeapStructSize );

					/* This block is being returned for use so must be taken out
					of the list of free blocks. */
					pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

					/* If the block is larger than required it can be split into
					two. */
					if( ( pxBlock->xBlockSize - gcWantedSize ) > gcHeapMINIMUM_BLOCK_SIZE )
					{
						/* This block is to be split into two.  Create a new
						block following the number of bytes requested. The void
						cast is used to prevent byte alignment warnings from the
						compiler. */
						pxNewBlockLink = ( BlockLink_t * ) ( ( ( uint8_t * ) pxBlock ) + gcWantedSize );
						gcConfigASSERT( ( ( ( size_t ) pxNewBlockLink ) & gcBYTE_ALIGNMENT_MASK ) == 0 );

						/* Calculate the sizes of two blocks split from the
						single block. */
						pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - gcWantedSize;
						pxBlock->xBlockSize = gcWantedSize;

						/* Insert the new block into the list of free blocks. */
						gcInsertBlockIntoFreeList( this, pxNewBlockLink );
					}
					else
					{
						gcCOVERAGE_TEST_MARKER();
					}

					this->gcFreeBytesRemaining -= pxBlock->xBlockSize;

					if( this->gcFreeBytesRemaining < this->gcMinimumEverFreeBytesRemaining )
					{
						this->gcMinimumEverFreeBytesRemaining = this->gcFreeBytesRemaining;
					}
					else
					{
						gcCOVERAGE_TEST_MARKER();
					}

					/* The block is being returned - it is allocated and owned
					by the application and has no "next" block. */
					pxBlock->xBlockSize |= this->gcBlockAllocatedBit;
					pxBlock->pxNextFreeBlock = NULL;
				}
				else
				{
					gcCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				gcCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			gcCOVERAGE_TEST_MARKER();
		}

		gcTraceMALLOC( pvReturn, gcWantedSize );
	}
	gcTaskResumeAll();

	gcConfigASSERT( ( ( ( size_t ) pvReturn ) & ( size_t ) gcBYTE_ALIGNMENT_MASK ) == 0 );
	return pvReturn;
}

/*-----------------------------------------------------------*/

void* GCHeap::calloc(size_t n, size_t size)
{
	return malloc(n * size );
}

/*-----------------------------------------------------------*/

void* GCHeap::realloc(void *pv, size_t newsize)
{
	uint8_t *puc = ( uint8_t * ) pv;
	BlockLink_t *pxLink;
	if( pv != NULL )
	{
		/* The memory being freed will have an BlockLink_t structure immediately
		before it. */
		puc -= gcHeapStructSize;

		/* This casting is to keep the compiler from issuing warnings. */
		pxLink = ( BlockLink_t * ) puc;
		
		/* If the block have enough space. */
		if(pxLink->xBlockSize >= newsize)
		{
			/* Return the pointer. */
			return pv;
		}
		else
		{
			/* Get a new memory block. */
			puc = (uint8_t *)malloc( newsize );
			
			/* Copy date to the new memory block. */
			memcpy(( void * )puc, pv, pxLink->xBlockSize);
			
			/* Free the old memory block. */
			free( pv);
			
			/* Return the new pointer. */
			return puc;
		}
	}
	
	return pv;
}

/*-----------------------------------------------------------*/

void GCHeap::free(void *pv )
{
uint8_t *puc = ( uint8_t * ) pv;
BlockLink_t *pxLink;

	if( pv != NULL )
	{
		/* The memory being freed will have an BlockLink_t structure immediately
		before it. */
		puc -= gcHeapStructSize;

		/* This casting is to keep the compiler from issuing warnings. */
		pxLink = ( BlockLink_t * ) puc;

		/* Check the block is actually allocated. */
		gcConfigASSERT( ( pxLink->xBlockSize & this->gcBlockAllocatedBit ) != 0 );
		gcConfigASSERT( pxLink->pxNextFreeBlock == NULL );

		if( ( pxLink->xBlockSize & this->gcBlockAllocatedBit ) != 0 )
		{
			if( pxLink->pxNextFreeBlock == NULL )
			{
				/* The block is being returned to the heap - it is no longer
				allocated. */
				pxLink->xBlockSize &= ~(this->gcBlockAllocatedBit);

				gcTaskSuspendAll();
				{
					/* Add this block to the list of free blocks. */
					this->gcFreeBytesRemaining += pxLink->xBlockSize;
					gcTraceFREE( pv, pxLink->xBlockSize );
					gcInsertBlockIntoFreeList( this, ( ( BlockLink_t * ) pxLink ) );
				}
				gcTaskResumeAll();
			}
			else
			{
				gcCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			gcCOVERAGE_TEST_MARKER();
		}
	}
}

/*-----------------------------------------------------------*/

size_t GCHeap::getFreeHeapSize()
{
	return this->gcFreeBytesRemaining;
}
/*-----------------------------------------------------------*/

size_t GCHeap::getMinimumEverFreeHeapSize( )
{
	return this->gcMinimumEverFreeBytesRemaining;
}
/*-----------------------------------------------------------*/

void gcPortInitialiseBlocks( void )
{
	/* This just exists to keep the linker quiet. */
}
/*-----------------------------------------------------------*/

static void gcHeapInit( GCHeap* pHeapInfo )
{
	BlockLink_t *pxFirstFreeBlock;
	uint8_t *pucAlignedHeap;
	size_t uxAddress;
	size_t xTotalHeapSize = pHeapInfo->gcHeapSize;
	
	/* Ensure the heap starts on a correctly aligned boundary. */
	uxAddress = ( size_t ) pHeapInfo->pHeap;

	if( ( uxAddress & gcBYTE_ALIGNMENT_MASK ) != 0 )
	{
		uxAddress += ( gcBYTE_ALIGNMENT - 1 );
		uxAddress &= ~( ( size_t ) gcBYTE_ALIGNMENT_MASK );
		xTotalHeapSize -= uxAddress - ( size_t ) pHeapInfo->pHeap;
	}

	pucAlignedHeap = ( uint8_t * ) uxAddress;

	/* xStart is used to hold a pointer to the first item in the list of free
	blocks.  The void cast is used to prevent compiler warnings. */
	pHeapInfo->xStart.pxNextFreeBlock = ( BlockLink_t * ) pucAlignedHeap;
	pHeapInfo->xStart.xBlockSize = ( size_t ) 0;

	/* pxEnd is used to mark the end of the list of free blocks and is inserted
	at the end of the heap space. */
	uxAddress = ( ( size_t ) pucAlignedHeap ) + xTotalHeapSize;
	uxAddress -= gcHeapStructSize;
	uxAddress &= ~( ( size_t ) gcBYTE_ALIGNMENT_MASK );
	pHeapInfo->pxEnd = ( BlockLink_t * ) uxAddress;
	pHeapInfo->pxEnd->xBlockSize = 0;
	pHeapInfo->pxEnd->pxNextFreeBlock = NULL;

	/* To start with there is a single free block that is sized to take up the
	entire heap space, minus the space taken by pxEnd. */
	pxFirstFreeBlock = ( BlockLink_t * ) pucAlignedHeap;
	pxFirstFreeBlock->xBlockSize = uxAddress - ( size_t ) pxFirstFreeBlock;
	pxFirstFreeBlock->pxNextFreeBlock = pHeapInfo->pxEnd;

	/* Only one block exists - and it covers the entire usable heap space. */
	pHeapInfo->gcMinimumEverFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
	pHeapInfo->gcFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;

	/* Work out the position of the top bit in a size_t variable. */
	pHeapInfo->gcBlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * gcHeapBITS_PER_BYTE ) - 1 );
}
/*-----------------------------------------------------------*/

static void gcInsertBlockIntoFreeList( GCHeap* pHeapInfo, BlockLink_t *pxBlockToInsert )
{
BlockLink_t *pxIterator;
uint8_t *puc;

	/* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */
	for( pxIterator = &(pHeapInfo->xStart); pxIterator->pxNextFreeBlock < pxBlockToInsert; pxIterator = pxIterator->pxNextFreeBlock )
	{
		/* Nothing to do here, just iterate to the right position. */
	}

	/* Do the block being inserted, and the block it is being inserted after
	make a contiguous block of memory? */
	puc = ( uint8_t * ) pxIterator;
	if( ( puc + pxIterator->xBlockSize ) == ( uint8_t * ) pxBlockToInsert )
	{
		pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
		pxBlockToInsert = pxIterator;
	}
	else
	{
		gcCOVERAGE_TEST_MARKER();
	}

	/* Do the block being inserted, and the block it is being inserted before
	make a contiguous block of memory? */
	puc = ( uint8_t * ) pxBlockToInsert;
	if( ( puc + pxBlockToInsert->xBlockSize ) == ( uint8_t * ) pxIterator->pxNextFreeBlock )
	{
		if( pxIterator->pxNextFreeBlock != pHeapInfo->pxEnd )
		{
			/* Form one big block from the two blocks. */
			pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
			pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
		}
		else
		{
			pxBlockToInsert->pxNextFreeBlock = pHeapInfo->pxEnd;
		}
	}
	else
	{
		pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
	}

	/* If the block being inserted plugged a gab, so was merged with the block
	before and the block after, then it's pxNextFreeBlock pointer will have
	already been set, and should not be set here as that would make it point
	to itself. */
	if( pxIterator != pxBlockToInsert )
	{
		pxIterator->pxNextFreeBlock = pxBlockToInsert;
	}
	else
	{
		gcCOVERAGE_TEST_MARKER();
	}
}

#endif /* CHIP_GC9002_NoHyper */