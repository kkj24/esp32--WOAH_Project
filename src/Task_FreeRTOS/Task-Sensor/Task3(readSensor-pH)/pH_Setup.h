#ifndef PH_SENSOR_FREERTOS_H
#define PH_SENSOR_FREERTOS_H

#include "pH_Lib.h"

extern pH_Sensor ph;
extern TaskHandle_t pH_Task_handle;
extern QueueHandle_t pH_queue;

#define PH_QUEUE_LENGTH 3

class pH_FreeRTOS {
    public:
        pH_FreeRTOS();

        void beginTask();

    private:

};

#endif