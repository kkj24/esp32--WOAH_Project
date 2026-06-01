#include "MQTT_Lib.h"

WiFiClient my_client;
PubSubClient my_mqtt(my_client);

// Get Unique ID
String getID_unique() {
    String ID;

    if(WiFi.status() == WL_CONNECTED) {
        ID = "ESP32/WOAH--" + WiFi.macAddress();
    }

    return ID;
}

myMQTT::myMQTT() {
    /// No Once Func at Here
};

void myMQTT::begin() {
    my_mqtt.setServer(MQTT_HOST, MQTT_PORT);

    reconnect();
}

template <typename value>
void myMQTT::sendData(String topic, String data_name, value data_value) {
    JsonDocument my_doc;                       // JSON Object
    char data_buff[512];                       // Data Buffer
    my_doc[data_name] = data_value;            // Convert Data to JSON
    serializeJson(my_doc, data_buff);          // Serialize JSON data to Buffer
    my_mqtt.publish(topic.c_str(), data_buff); // Send Data Buffer to MQTT
}

void myMQTT::reconnect() {
    static bool last_status;

    // Connecting
    while(!mqttStatus()) {
        my_mqtt.connect(getID_unique().c_str());
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    if(!last_status && mqttStatus()) {
        Serial.printf("Success to Connect MQTT with ID: %s", getID_unique());
    }

    last_status = mqttStatus();
}

bool myMQTT::mqttStatus() {
    bool status = (my_mqtt.connected() ? true : false);
    return status;
}