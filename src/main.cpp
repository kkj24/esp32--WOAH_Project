#include <Arduino.h>

#include "Sensor_Setup.h"
#include "TDS_Sensor_Lib/TDS_Lib.h"
#include "Turbidity_Sensor_Lib/Turbidity_Lib.h"
#include "pH_Sensor_Lib/pH_Lib.h"

#include "WiFi_Lib.h"
#include "MQTT_Lib.h"

#include "Task_FreeRTOS/Task-Sensor/Task1(readSensor-TDS)/TDS_Setup.h"
#include "Task_FreeRTOS/Task-Sensor/Task2(readSensor-Turbidity)/Turbidity_Setup.h"
#include "Task_FreeRTOS/Task-Sensor/Task3(readSensor-pH)/pH_Setup.h"

void setup() {

}

void loop() {

}