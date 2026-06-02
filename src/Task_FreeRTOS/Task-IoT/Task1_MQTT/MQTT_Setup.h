#ifndef MQTT_FREERTOS_TASK_H
#define MQTT_FREERTOS_TASK_H

#include "TDS_Setup.h"
#include "Turbidity_Setup.h"
#include "pH_Setup.h"

#include "MQTT_Lib.h"

extern myMQTT mqtt_lib;
extern TaskHandle_t mqtt_task_handle;

class MQTT_FreeRTOS {
    public:
        MQTT_FreeRTOS();

        void beginTask(); // Begin Task

    private:
};

#endif