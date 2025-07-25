/**
* Copyright (c), 2015-2025
* @file fifo.c
* @brief fifo
* @author 
* @verbatim
* Change Logs:
* Date           Author       Notes
* @endverbatim
*/
 
 
#include "com_c1.h"

#if COM_COMPILE_SWITCH
/**
* @brief 初始化fifo
* @param fifo: 待操作的fifo
* @param item_sum：fifo中元素数.注意不是字节数
* @param fifo_ptr: 缓存指针
* @param item_size: 元素大小.单位: 字节
* @return -1: 输入参数错误
*          0: 内存申请失败
*          1: 初始化成功
*/
int8_t fifo_init(Fifo_t* fifo, void* fifo_ptr, int item_sum, int item_size)
{
	int8_t ret = -1;

	if ((fifo == NULL) || (item_sum == 0) || (item_size == 0) || (fifo_ptr == NULL))
	{
		return ret;
	}
	
    fifo->item_sum = item_sum;
    fifo->item_size = item_size;
    fifo->idx_write = 0;
    fifo->idx_read = 0;
    fifo->fifo_ptr = fifo_ptr;
	
    return ret;
}


/**
* @brief fifo检查是否可以写入
* @param fifo: 待操作的fifo
* @retval -1: 输入参数错误
*          0: 不可写入
*          1: 可写
*/
int8_t fifo_writeable(Fifo_t* fifo)
{
    int8_t ret = -1;

    if (fifo == NULL)
    {
    	return ret;
    }

    if ((fifo->idx_write == fifo->idx_read - 1)
     || (fifo->idx_write == fifo->idx_read + fifo->item_sum - 1))
    {
        return 0;
    }
    else
    {
        return 1;
    }

    return ret;
}
 
/**
* @brief fifo写入
* @param fifo: 待操作的fifo
* @param data: 待写入的数据地址
* @return -1: 输入参数错误
*          0: 写入失败
*          1: 写入成功
*/
 
int8_t fifo_write(Fifo_t *fifo, void *data)
{
	int8_t ret = -1;
	
	if (fifo == NULL)
	{
		return ret;
	}
	
    if ((fifo->idx_write == fifo->idx_read - 1)
        || (fifo->idx_write == fifo->idx_read + fifo->item_sum - 1))
    {
        return 0;
    }
    
    memcpy((char *)(fifo->fifo_ptr) + fifo->idx_write * fifo->item_size, (char*)data, fifo->item_size);
    fifo->idx_write ++;
    if (fifo->idx_write >= fifo->item_sum)
    {
        fifo->idx_write = 0;
    }
	ret = 1;
	
	return ret;
}


/**
* @brief fifo批量写入
* @param fifo: 待操作的fifo
* @param data: 写入元素指针
* @param item_num：写入的元素数目
* @return -1: 输入参数错误
*          0: 写入失败
*          1: 写入成功
*/
int8_t fifo_write_batch(Fifo_t *fifo, void *data, int item_num)
{
	int8_t ret = -1;
	int i;
    int copy1st;
    int copy2nd;
	
	if (fifo == NULL)
	{
		return ret;
	}
	
    if (fifo_writeable_item_count(fifo) < item_num)
    {
		ret = 0;
        return ret;
    }

    /* Specially deal with item_size is 1 */
    if(fifo->item_size == 1)
    {
        if(fifo->item_sum - fifo->idx_write >= item_num)
        {
            memcpy((char *)(fifo->fifo_ptr) + fifo->idx_write, (char*)data, item_num);
            fifo->idx_write += item_num;

            if(fifo->idx_write >= fifo->item_sum)
            {
                fifo->idx_write = 0;
            }
        }
        else
        {
            /* Calculate the 1st copy count */
            copy1st = fifo->item_sum - fifo->idx_write;
            /* Calculate the 2nd copy count */
            copy2nd = item_num - copy1st;
            memcpy((char *)(fifo->fifo_ptr) + fifo->idx_write, (char*)data, copy1st);
            memcpy((char *)(fifo->fifo_ptr), (char*)data + copy1st, copy2nd);
            fifo->idx_write = copy2nd;
        }
        
    }
    else
    {
        for (i = 0; i < item_num; i++)
        {
            if ((fifo->idx_write == fifo->idx_read - 1)
                || (fifo->idx_write == fifo->idx_read + fifo->item_sum - 1))
            {
                break;
            }

            memcpy((char *)(fifo->fifo_ptr) + fifo->idx_write * fifo->item_size, (char*)data + i*fifo->item_size, fifo->item_size);
            fifo->idx_write ++;
            if (fifo->idx_write >= fifo->item_sum)
            {
                fifo->idx_write = 0;
            }
        }
    }
	ret = 1;
 
    return ret;
}


/**
* @brief fifo检查是否可以读取
* @param fifo: 待操作的fifo
* @return -1: 输入参数错误
*          0: 不可读
*          1: 可读
*/
int8_t fifo_readable(Fifo_t *fifo)
{
    int8_t ret = -1;

    if (fifo == NULL) 
    {
        return ret;
    }

    if (fifo->idx_write == fifo->idx_read)
    {
        ret = 0;
    }
    else
    {
        ret = 1;
    }

    return ret;
}


/**
* @brief fifo读取
* @param fifo: 待操作的fifo
* @param data: 读取的数据存储地址
* @return -1: 输入参数错误
*          0: 读取失败
*          1: 读取成功
*/
int8_t fifo_read(Fifo_t *fifo, void *data)
{
    int8_t ret = -1;

    if (fifo == NULL)
    {
        return ret;
    }
    
    if (fifo->idx_write == fifo->idx_read)
    {
        ret = 0;
        return ret;
    }

    memcpy(data, (char *)(fifo->fifo_ptr) + fifo->idx_read * fifo->item_size, fifo->item_size);
    fifo->idx_read++;
    if (fifo->idx_read >= fifo->item_sum)
    {
        fifo->idx_read = 0;
    }

    ret = 1;
    return ret;
}


/**
* @brief fifo批量读取
* @param fifo: 待操作的fifo
* @param data: 读取的数据
* @param item_num：读取的元素数
* @return -1: 输入参数错误
*          0: 读取失败
*          1: 读取成功
*/
int8_t fifo_read_batch(Fifo_t *fifo, void *data, int item_num)
{
    int8_t ret = -1;
    int i;
    int copy1st;
    int copy2nd;

    if ((fifo == NULL) || (item_num== 0))
    {
        return ret;
    }

    if (fifo_readable_item_count(fifo) < item_num)
    {
        ret = 0;
        return ret;
    }

    /* Specially deal with item_size is 1 */
    if(fifo->item_size == 1)
    {
        if(fifo->item_sum - fifo->idx_read >= item_num)
        {
            memcpy((char*)data, (char *)(fifo->fifo_ptr) + fifo->idx_read, item_num);
            fifo->idx_read += item_num;
            if(fifo->idx_read >= fifo->item_sum)
            {
                fifo->idx_read = 0;
            }
        }
        else
        {
            /* Calculate the 1st copy count */
            copy1st = fifo->item_sum - fifo->idx_read;
            /* Calculate the 2nd copy count */
            copy2nd = item_num - copy1st;
            memcpy((char*)data, (char *)(fifo->fifo_ptr) + fifo->idx_read, copy1st);
            memcpy((char*)data + copy1st, (char *)(fifo->fifo_ptr), copy2nd);
            fifo->idx_read = copy2nd;
        }
    }
    else
    {

        for (i = 0; i < item_num; i++)
        {   
            if (fifo->idx_read == fifo->idx_write)
            {
                break;
            }
            
            memcpy((char*)data + i*fifo->item_size, (char *)(fifo->fifo_ptr) + fifo->idx_read * fifo->item_size, fifo->item_size);
            fifo->idx_read ++;
            if (fifo->idx_read >= fifo->item_sum)
            {
                fifo->idx_read = 0;
            }
        }
    }

    ret = 1;
    return ret;
}
 
/**
* @brief fifo可读的元素数
* @param fifo: 待操作的fifo
* @return 元素数
*/


uint32_t fifo_readable_item_count(Fifo_t *fifo)
{
    if (fifo == NULL)
    {
        return 0;
    }

    if (fifo->idx_write >= fifo->idx_read)
    {
        return fifo->idx_write - fifo->idx_read;
    }
    else
    {
        return fifo->item_sum + fifo->idx_write - fifo->idx_read;
    }
}


/**
* @brief fifo可写的元素数
* @param fifo: 待操作的fifo
* @return 元素数
*/
uint32_t fifo_writeable_item_count(Fifo_t *fifo)
{
    if (fifo == NULL)
    {
        return 0;
    }

    if (fifo->idx_write >= fifo->idx_read)
    {
        return fifo->item_sum - fifo->idx_write + fifo->idx_read;
    }
    else
    {
        return fifo->idx_read - fifo->idx_write;
    }

	return 0;
}

#endif
