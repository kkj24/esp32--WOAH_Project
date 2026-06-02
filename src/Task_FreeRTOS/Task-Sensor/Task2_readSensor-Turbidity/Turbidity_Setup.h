#ifndef TURBIDITY_SENSOR_FREERTOS_H
#define TURBIDITY_SENSOR_FREERTOS_H

#include "Turbidity_Lib.h"


typedef struct {
    uint8_t data1;
    uint8_t data2;
    uint8_t data3;

    int16_t data_raw1;
    int16_t data_raw2;
    int16_t data_raw3;
} turbidity_struct_t;

extern turbidity_struct_t turbidity_data;

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