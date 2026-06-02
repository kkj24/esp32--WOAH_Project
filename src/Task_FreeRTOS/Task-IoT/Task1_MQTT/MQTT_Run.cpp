#include "MQTT_Setup.h"

void runTask_mqtt(void *param);

myMQTT mqtt_lib;
TaskHandle_t mqtt_task_handle;

MQTT_FreeRTOS::MQTT_FreeRTOS() {
    // No Once Func at Here
};

void MQTT_FreeRTOS::beginTask() {
    mqtt_lib.begin();

    xTaskCreatePinnedToCore(
        runTask_mqtt,
        "MQTT_FreeRTOS_Task",
        4096,
        this,
        1,
        &mqtt_task_handle,
        0
    );
}

void runTask_mqtt(void *param) {
    TickType_t saveTick = xTaskGetTickCount();

    while(1) {
        {
            static uint32_t lastTime = 0;
            uint16_t interval = 1000;

            // Timer
            if(millis() - lastTime >= interval) {
                lastTime = millis();

                // Receive TDS
                xQueueReceive(
                    TDS_task_Queue,
                    &tds_data,
                    pdMS_TO_TICKS(50)
                );

                // Receive Turbidity
                xQueueReceive(
                    turbidity_queue,
                    &turbidity_data,
                    pdMS_TO_TICKS(50)
                );

                // Receive pH
                xQueueReceive(
                    pH_queue,
                    &data_ph,
                    pdMS_TO_TICKS(50)
                );

                mqtt_lib.run();

                // Send TDS
                mqtt_lib.sendData(MQTT_TOPIC, "TDS1", tds_data.data1);
                mqtt_lib.sendData(MQTT_TOPIC, "TDS2", tds_data.data2);
                mqtt_lib.sendData(MQTT_TOPIC, "TDS3", tds_data.data3);

                // Send Turbidity
                mqtt_lib.sendData(MQTT_TOPIC, "Turbidity1", turbidity_data.data1);
                mqtt_lib.sendData(MQTT_TOPIC, "Turbidity2", turbidity_data.data2);
                mqtt_lib.sendData(MQTT_TOPIC, "Turbidity3", turbidity_data.data3);

                // Send pH
                mqtt_lib.sendData(MQTT_TOPIC, "pH_Water", data_ph.pH_data);
            }
        }
        xTaskDelayUntil(&saveTick, pdMS_TO_TICKS(50));
    }
}