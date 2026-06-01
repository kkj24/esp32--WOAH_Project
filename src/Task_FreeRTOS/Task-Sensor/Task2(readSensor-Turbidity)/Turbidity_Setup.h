#ifndef TURBIDITY_SENSOR_FREERTOS_H
#define TURBIDITY_SENSOR_FREERTOS_H

#include "Turbidity_Lib.h"

extern Turbidity_Sensor turbidity;
extern TaskHandle_t turbidity_task_handle;
extern QueueHandle_t turbidity_queue;

#define TURBIDITY_QUEUE_LENGTH 6

class Turbidity_FreeRTOS {
    public:
        Turbidity_FreeRTOS();

        void beginTask();       // Begin Task

    private:

};

#endif