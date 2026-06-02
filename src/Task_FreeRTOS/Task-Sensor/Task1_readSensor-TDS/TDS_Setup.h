#ifndef FREERTOS_TDS_SENSOR_H
#define FREERTOS_TDS_SENSOR_H

#include "TDS_Lib.h"


typedef struct {
    uint8_t data1;
    uint8_t data2;
    uint8_t data3;

    int16_t data1_raw;
    int16_t data2_raw;
    int16_t data3_raw;
} tds_queue_struct;

extern TDS_Sensor tds;
extern TaskHandle_t TDS_task_FreeRTOS;
extern tds_queue_struct tds_data;
extern QueueHandle_t TDS_task_Queue;

#define TDS_QUEUE_LENGTH 6

class TDS_FreeRTOS {
    public:
        TDS_FreeRTOS();

        void beginTask();   // Begin Task
        
    private:
        
};

#endif