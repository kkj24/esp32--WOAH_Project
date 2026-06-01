#include <Arduino.h>

#include "Sensor_Setup.h"
#include "TDS_Sensor_Lib/TDS_Lib.h"
#include "Turbidity_Sensor_Lib/Turbidity_Lib.h"
#include "pH_Sensor_Lib/pH_Lib.h"

#include "WiFi_Lib.h"

myWiFi wifi;

void setup() {
    Serial.begin(115200);
    delay(200);

    wifi.begin("", "");
}

void loop() {
    wifi.handlePortal();
}