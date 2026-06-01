#ifndef WIFI_LIBRARIES_H
#define WIFI_LIBRARIES_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <esp_wifi.h>

// Soft AP SSID
#ifndef WIFIAP_SSID
#define WIFIAP_SSID "WOAH--ESP32"
#endif

// Soft AP Password
#ifndef WIFIAP_PASS
#define WIFIAP_PASS "WOAH--Project-1021"
#endif

#ifndef WIFI_HOSTNAME
#define WIFI_HOSTNAME "WOAH_ESP32"
#endif

class myWiFi_Lib {  
    public:
        myWiFi_Lib();

        void begin(String PASS = "Test12345", String SSID = "ESP32--WOAH");   // Begin WiFi
        void init_web();                        // Initialize Web
        void AutoReconnect(bool enable);        // Auto Reconnecting WiFi Func
        void run();                             // Looping Func

        bool statusWiFi();                      // Connection Status WiFi           
        int8_t WiFi_RSSI();                     // RSSI WiFi (Signal Strength)

    private:
        WebServer server;
        String wifiStatus;
};

#endif