#include "TDS_Setup.h"

typedef struct {
    uint8_t data1;
    uint8_t data2;
    uint8_t data3;

    int16_t data1_raw;
    int16_t data2_raw;
    int16_t data3_raw;
} tds_queue_struct;

TDS_Sensor tds;
TaskHandle_t TDS_task_FreeRTOS;
tds_queue_struct tds_data;

void runTask_TDS(void *param);

TDS_FreeRTOS::TDS_FreeRTOS() {
    // No Once Func at Here
};

void TDS_FreeRTOS::beginTask() {
    xTaskCreatePinnedToCore(
        runTask_TDS,                // Task Func
        "TDS_Sensor_FreeRTOS",  // Name
        1023,                   // Stack depth
        this,                   // Param
        1,                      // Priority
        &TDS_task_FreeRTOS,     // Handler
        1                       // Core
    );

    // Create Queue Task
    TDS_task_Queue = xQueueCreate(TDS_QUEUE_LENGTH, sizeof(tds_data));
}

// Run Task
void runTask_TDS(void *param) {
    TickType_t saveTick = xTaskGetTickCount();
    
    while(1) {
        {
            static uint32_t lastTime = 0;
            uint16_t interval = 1000;

            // Timer
            if(millis() - lastTime >= interval) {
                lastTime = millis();

                // Run TDS Read & save to struct
                tds.auto_calibrate(true);
                tds_data.data1 = tds.read_TDS(TDS1);
                tds_data.data2 = tds.read_TDS(TDS2);
                tds_data.data3 = tds.read_TDS(TDS3);

                // Read & Save Raw TDS Data
                tds_data.data1_raw = tds.raw_TDS(TDS1);
                tds_data.data2_raw = tds.raw_TDS(TDS2);
                tds_data.data3_raw = tds.raw_TDS(TDS3);

                // Send Queue
                xQueueSend(
                    TDS_task_Queue,
                    &tds_data,
                    pdMS_TO_TICKS(50)
                );
            }
        }

        xTaskDelayUntil(&saveTick, pdMS_TO_TICKS(50));
    }
}