#include "pH_Setup.h"

typedef struct {
    uint8_t pH_data;
    int16_t pH_temp;
    int16_t pH_raw;
} pH_struct_t;

pH_struct_t data_ph;
pH_Sensor ph;
TaskHandle_t pH_Task_handle;
QueueHandle_t pH_queue;

void runTask_pH(void *param);

pH_FreeRTOS::pH_FreeRTOS() {
    // No Once Func at Here
};

void pH_FreeRTOS::beginTask() {
    xTaskCreatePinnedToCore(
        runTask_pH,
        "pH_Task_FreeRTOS",
        1023,
        this,
        1,
        &pH_Task_handle,
        1
    );

    pH_queue = xQueueCreate(PH_QUEUE_LENGTH, sizeof(data_ph));
}

void runTask_pH(void *param) {
    TickType_t saveTick = xTaskGetTickCount();
    ph.calibrate(0, 0);

    while (1) {
        {
            static uint32_t lastTime = 0;
            uint16_t interval = 1000;

            // Timer
            if(millis() - lastTime >= interval) {
                lastTime = millis();

                // Get pH Data & Save to Struct
                data_ph.pH_data = ph.read_pH();
                data_ph.pH_raw = ph.read_raw_pH();
                data_ph.pH_temp = ph.read_pH_temp();

                // Send Data to Queue
                xQueueSend(
                    pH_queue,
                    &data_ph,
                    pdMS_TO_TICKS(50)
                );
            }
        }
        xTaskDelayUntil(&saveTick, pdMS_TO_TICKS(50));
    }
    
}