#ifndef FREERTOS_TDS_SENSOR_H
#define FREERTOS_TDS_SENSOR_H

#include "TDS_Lib.h"

extern TDS_Sensor tds;
extern TaskHandle_t TDS_task_FreeRTOS;
extern QueueHandle_t TDS_task_Queue;

#define TDS_QUEUE_LENGTH 6

class TDS_FreeRTOS {
    public:
        TDS_FreeRTOS();

        void beginTask();   // Begin Task
        
    private:
        
};

#endif