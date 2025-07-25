/**
* Copyright (c), 2015-2025
* @file fifo.h
* @brief fifo header file
* @author
* @verbatim 
* Change Logs:
* Date           Author       Notes
* @endverbatim
*/
 
#ifndef COM_FIFO_H
#define COM_FIFO_H
 
#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"
#include "stdbool.h"
#include "string.h"
#include "math.h"

/**
* @brief fifo结构
*/
typedef struct
{
    uint32_t idx_write;
    uint32_t idx_read;
 
    // fifo中存储的元素数,不是字节大小
    uint32_t item_sum;
    // 元素大小.单位: 字节
    uint32_t item_size;
    void *fifo_ptr;
} Fifo_t;

 
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
 
int8_t fifo_init(Fifo_t* fifo, void* fifo_ptr, int item_sum, int item_size);
 
/**
* @brief fifo检查是否可以写入
* @param fifo: 待操作的fifo
* @retval -1: 输入参数错误
*          0: 不可写入
*          1: 可写
*/
 
int8_t fifo_writeable(Fifo_t* fifo);
 
/**
* @brief fifo写入
* @param fifo: 待操作的fifo
* @param data: 待写入的数据地址
* @return -1: 输入参数错误
*          0: 写入失败
*          1: 写入成功
*/
 
int8_t fifo_write(Fifo_t *fifo, void *data);
 
/**
* @brief fifo批量写入
* @param fifo: 待操作的fifo
* @param data: 写入元素指针
* @param item_num：写入的元素数目
* @return -1: 输入参数错误
*          0: 写入失败
*          1: 写入成功
*/
 
int8_t fifo_write_batch(Fifo_t *fifo, void *data, int item_num);
 
/**
* @brief fifo检查是否可以读取
* @param fifo: 待操作的fifo
* @return -1: 输入参数错误
*          0: 不可读
*          1: 可读
*/
 
int8_t fifo_readable(Fifo_t *fifo);
 
/**
* @brief fifo读取
* @param fifo: 待操作的fifo
* @param data: 读取的数据存储地址
* @return -1: 输入参数错误
*          0: 读取失败
*          1: 读取成功
*/
 
int8_t fifo_read(Fifo_t *fifo, void *data);
 
/**
* @brief fifo批量读取
* @param fifo: 待操作的fifo
* @param data: 读取的数据
* @param item_num：读取的元素数
* @return -1: 输入参数错误
*          0: 读取失败
*          1: 读取成功
*/
 
int8_t fifo_read_batch(Fifo_t *fifo, void *data, int item_num);
 
/**
* @brief fifo可读的元素数
* @param fifo: 待操作的fifo
* @return 元素数
*/
 
uint32_t fifo_readable_item_count(Fifo_t *fifo);
 
/**
* @brief fifo可写的元素数
* @param fifo: 待操作的fifo
* @return 元素数
*/
 
uint32_t fifo_writeable_item_count(Fifo_t *fifo);
 
/**
* @brief fifo可写的元素数
* @param fifo: 待操作的fifo
* @return 元素数
*/
 
uint32_t fifo_writeable_item_count(Fifo_t *fifo);
 
#endif

