#include "WiFi_Setup.h"

void runTask_WiFi(void *param);

typedef struct {
    int8_t RSSI_wifi;
    bool status;
} wifi_struct_t;

wifi_struct_t wifi_data;
myWiFi_Lib wifi_lib;
TaskHandle_t wifi_task_handle;
QueueHandle_t WiFi_queue;

WiFi_FreeRTOS::WiFi_FreeRTOS() {
    // No Once Func at Here
}

void WiFi_FreeRTOS::beginTask() {
    wifi_lib.AutoReconnect(true);
    wifi_lib.begin();
    wifi_lib.init_web();

    xTaskCreatePinnedToCore(
        runTask_WiFi,
        "WiFi_FreeRTOS_Task",
        1023,
        this,
        1,
        &wifi_task_handle,
        0
    );

    WiFi_queue = xQueueCreate(WIFI_QUEUE_LENGTH, sizeof(wifi_data));
}

void runTask_WiFi(void *param) {
    TickType_t saveTick = xTaskGetTickCount();

    while(1) {
        {
            wifi_lib.run();

            wifi_data.RSSI_wifi = wifi_lib.WiFi_RSSI();
            wifi_data.status = wifi_lib.statusWiFi();

            xQueueSend(
                WiFi_queue,
                &wifi_data,
                pdMS_TO_TICKS(50)
            );
        }
        xTaskDelayUntil(&saveTick, pdMS_TO_TICKS(50));
    }
}