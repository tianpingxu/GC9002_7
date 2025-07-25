////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:     
//
// Create Date:   2015/11/17
// File Name:     modbus_task.c
// Project Name:  AHMISimulator
//
// Revision:
// Revision 2.00 - File Created 2018/08/22 by Yu Chunying
// Revision 2.00 - add the modbus tasks function
//
////////////////////////////////////////////////////////////////////////////////

#include "modbus_task.h"
#include "csi_kernel.h"


extern QueueHandle_t ModbusMasterRWTaskQueue;
extern k_task_handle_t ModBusMasterRWTaskHandle;
extern k_task_handle_t ModBusSlaveTaskHandle;
extern k_task_handle_t ModBusMasterTaskHandle;

void ModBusTaskCreate(void)
{
#ifdef USE_MODBUS_MASTER  
    
    portBASE_TYPE xReturn;
    
    ModbusMasterRWTaskQueue = xQueueCreate(50, sizeof(ModBusMasterRWMsg));

    xReturn = csi_kernel_task_new(ModBusMasterRWTask, "ModBusMasterRWTask", NULL, KPRIO_LOW1, 0, NULL, configMINIMAL_STACK_SIZE, &ModBusMasterRWTaskHandle);
    //xReturn = xTaskCreate(ModBusMasterRWTask, "ModBusMasterRWTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &ModBusMasterRWTaskHandle);
    if (xReturn != 0)
    {
        ERROR_PRINT("Fail to create ModBus Task.\r\n");
        return;
    }

    xReturn = csi_kernel_task_new(ModBusMasterTask, "ModBusMasterTask", NULL, KPRIO_LOW3, 0, NULL, configMINIMAL_STACK_SIZE, &ModBusMasterTaskHandle);
    //xReturn = xTaskCreate(ModBusMasterTask, "ModBusMasterTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, &ModBusMasterTaskHandle);
    if (xReturn != 0)
    {
        ERROR_PRINT("Fail to create ModBus Task.\r\n");
        return;
    }
#endif

#ifdef USE_MODBUS_SLAVE
    
    portBASE_TYPE xReturn;

    xReturn = csi_kernel_task_new(ModBusSlaveTask, "ModBusSlaveTask", NULL, KPRIO_LOW0, 0, NULL, configMINIMAL_STACK_SIZE, &ModBusSlaveTaskHandle);
    //xReturn = xTaskCreate(ModBusSlaveTask, "ModBusSlaveTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &ModBusSlaveTaskHandle);
    if (xReturn != 0)
    {
        ERROR_PRINT("Fail to create ModBus Task.\r\n");
        return;
    }
#endif
}

