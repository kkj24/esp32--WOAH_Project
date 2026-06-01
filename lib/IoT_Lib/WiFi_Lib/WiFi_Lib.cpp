#include "WiFi_Lib.h"
#include "Web_Page_Login/Web_Page.h"

myWiFi_Lib::myWiFi_Lib() {
    // No Func in Here
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
    // Web Page Func
}

bool myWiFi_Lib::statusWiFi() {
    bool status;
    if(WiFi.status() == WL_CONNECTED)
        status = true;
    else
        status = false;

    return status;
}

int8_t myWiFi_Lib::WiFi_RSSI() {
    int8_t RSSI = WiFi.RSSI();
    return RSSI;
}