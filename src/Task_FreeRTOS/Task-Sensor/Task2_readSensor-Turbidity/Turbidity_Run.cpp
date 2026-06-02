#include "Turbidity_Setup.h"

turbidity_struct_t turbidity_data;
Turbidity_Sensor turbidity;
TaskHandle_t turbidity_task_handle;
QueueHandle_t turbidity_queue;

void runTask_Turbidity(void *param);

Turbidity_FreeRTOS::Turbidity_FreeRTOS() {
    // No Once Func at Here
};

void Turbidity_FreeRTOS::beginTask() {
    xTaskCreatePinnedToCore(
        runTask_Turbidity,
        "Turbidity_FreeRTOS_Task",
        1023,
        this,
        1,
        &turbidity_task_handle,
        1
    );

    turbidity_queue = xQueueCreate(TURBIDITY_QUEUE_LENGTH, sizeof(turbidity_data));
}

void runTask_Turbidity(void *param) {
    TickType_t saveTick = xTaskGetTickCount();

    while(1) {
        {
            static uint32_t lastTime = 0;
            uint16_t interval = 1000;

            if(millis() - lastTime >= interval) {
                lastTime = millis();

                // Get Turbidity Data & Save to Struct
                turbidity.auto_calibrate(true);
                turbidity_data.data1 = turbidity.getTurbidity_read(TURBIDITY1);
                turbidity_data.data2 = turbidity.getTurbidity_read(TURBIDITY2);
                turbidity_data.data3 = turbidity.getTurbidity_read(TURBIDITY3);

                // Get Raw Turbidity Data & Save to Struct
                turbidity_data.data_raw1 = turbidity.readTurbidity_raw(TURBIDITY1);
                turbidity_data.data_raw2 = turbidity.readTurbidity_raw(TURBIDITY2);
                turbidity_data.data_raw3 = turbidity.readTurbidity_raw(TURBIDITY3);

                // Send Data to Queue
                xQueueSend(
                    turbidity_queue,
                    &turbidity_data,
                    pdMS_TO_TICKS(50)
                );
            }
        }

        xTaskDelayUntil(&saveTick, pdMS_TO_TICKS(50));
    }
}