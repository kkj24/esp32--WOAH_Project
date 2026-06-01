#ifndef WIFI_LIBRARIES_H
#define WIFI_LIBRARIES_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <esp_wifi.h>

class myWiFi {  
    public:
        myWiFi();

        void begin(String PASS, String SSID);   // Begin WiFi
        void init_web();                        // Initialize Web
        void AutoReconnect();                   // Auto Reconnecting WiFi Func
        void run();                             // Looping Func

    private:

};

#endif