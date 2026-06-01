#ifndef MQTT_LIBRARIES_H
#define MQTT_LIBRARIES_H

#include <Arduino.h>
#include <ArduinoJSON.h>
#include <PubSubClient.h>

#include "WiFi_Lib.h"

// Password
#ifndef MQTT_PASS
#define MQTT_PASS ""
#endif

// User name
#ifndef MQTT_NAME
#define MQTT_NAME ""
#endif

// Port
#ifndef MQTT_PORT
#define MQTT_PORT 1883
#endif

// Broker
#ifndef MQTT_HOST
#define MQTT_HOST "broker.emqx.io"
#endif

// Topic
#ifndef MQTT_TOPIC
#define MQTT_TOPIC "ESP32/Project/Location/WOAH/Sensor"
#endif

extern WiFiClient my_client;
extern PubSubClient my_mqtt;

class myMQTT {
    public:
        myMQTT();
    
        void begin();
    
        template <typename value>
        void sendData(String topic, String data_name, value data_value);     // Send Data JSON to MQTT
    
        void reconnect();    // Reconnect MQTT
    
        bool mqttStatus();   // MQTT Connection Status

    private:

};

#endif