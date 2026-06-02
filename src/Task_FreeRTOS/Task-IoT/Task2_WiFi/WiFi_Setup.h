#ifndef WIFI_FREERTOS_H
#define WIFI_FREERTOS_H

#include "WiFi_Lib.h"

extern myWiFi_Lib wifi_lib;
extern TaskHandle_t wifi_task_handle;
extern QueueHandle_t WiFi_queue;

#define WIFI_QUEUE_LENGTH 2

class WiFi_FreeRTOS {
    public:
        WiFi_FreeRTOS();

        void beginTask();   // Begin Task

    private:

};

#endif