#include "WiFi_Lib.h"
#include "Web_Page_Login/Web_Page.h"
#include <ArduinoJson.h>

myWiFi_Lib::myWiFi_Lib() : server(80) {
}

void myWiFi_Lib::begin(String PASS, String SSID) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(WIFIAP_SSID, WIFIAP_PASS);
    WiFi.setHostname(WIFI_HOSTNAME);
    WiFi.begin(SSID.c_str(), PASS.c_str());
    
    while(!statusWiFi()) {
        WiFi.begin(SSID.c_str(), PASS.c_str());
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    Serial.println(F("WiFi Connected!"));
}

void myWiFi_Lib::AutoReconnect(bool enable) {
    WiFi.setAutoConnect(enable);
    WiFi.setAutoReconnect(enable);
}

void myWiFi_Lib::init_web() {
    server.on("/", [this]() {
        server.send(200, "text/html", HTML_Page);
    });

    server.on("/wifi", HTTP_POST, [this]() {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, server.arg("plain")); // Argument

        if(error) {
            server.send(400, "text/plain", "Invalid JSON"); // Send error info "Invalid JSON"
            return;
        }

        String ssid = doc["ssid"].as<String>();         // SSID as JSON
        String password = doc["password"].as<String>(); // PASS as JSON 

        // Connect
        WiFi.begin(ssid.c_str(), password.c_str());
        wifiStatus = "CONNECTING";

        // TimeOut Connect
        unsigned long startTime = millis();
        while(WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        // While Connected
        if(WiFi.status() == WL_CONNECTED) {
            wifiStatus = "CONNECTED";
            server.send(200, "text/plain", "WiFi Connected!");
        // While Failed to Connect
        } else {
            wifiStatus = "FAILED";
            server.send(200, "text/plain", "WiFi Failed to Connect!");
        }
    });

    // Send WiFi Status
    server.on("/status", HTTP_GET, [this]() {
        server.send(200, "text/plain", wifiStatus);
    });

    server.begin();
}

bool myWiFi_Lib::statusWiFi() {
    bool status;
    if(WiFi.status() == WL_CONNECTED) {
        status = true;

        bool lastStatus;
        if(!lastStatus && status)
            Serial.println(F("WiFi Connnected!"));

        lastStatus = status;
    }
    else {
        status = false;

        bool lastStatus;
        if(lastStatus && !status)
            Serial.println(F("WiFi Disconnected!"));

        lastStatus = status;
    }

    return status;
}

int8_t myWiFi_Lib::WiFi_RSSI() {
    int8_t RSSI = WiFi.RSSI();
    return RSSI;
}

void myWiFi_Lib::run() {
    server.handleClient();
}