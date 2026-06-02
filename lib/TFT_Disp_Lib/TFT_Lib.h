#ifndef TFT_DISPLAY_LIBARIES_H
#define TFT_DISPLAY_LIBARIES_H

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>

#define TFT_SCREEN_WIDTH  320
#define TFT_SCREEN_HEIGHT 480
#define TFT_BUF_SIZE      (TFT_SCREEN_WIDTH * 40)

enum dashboard_tab_t {
    TAB_SENSOR = 0,
    TAB_INFO,
    TAB_WIFI,
    TAB_MQTT
};

struct sensor_data_t {
    float turbidity1;
    float turbidity2;
    float turbidity3;
    float turbidity_avg;

    float tds1;
    float tds2;
    float tds3;
    float tds_avg;

    float ph_water;
    float temperature;
};

struct info_data_t {
    float cpu_temp;
    uint32_t heap;
    uint32_t memory;
    uint32_t ram;
    float usage;
    uint8_t task_count;
    uint16_t fps;
    String infos;
};

struct wifi_data_t {
    String status;
    int8_t rssi;
    String ip;
    String mac;
    String ssid;
    String pass;
    uint32_t ping;
};

struct mqtt_data_t {
    String status;
    String connection;
    String topic;
    String send;
    String host;
    String name;
    uint32_t ping;
};

class myTFT {
    public:
        myTFT();

        void begin();
        void dashboard();
        void lv_timer();

        void switchTab(dashboard_tab_t tab);

        void updateSensor(sensor_data_t data);
        void updateInfo(info_data_t data);
        void updateWiFi(wifi_data_t data);
        void updateMQTT(mqtt_data_t data);

    private:
        void _createBackground(lv_obj_t* parent);
        void _createTabBar(lv_obj_t* parent);
        void _createSensorTab(lv_obj_t* parent);
        void _createInfoTab(lv_obj_t* parent);
        void _createWiFiTab(lv_obj_t* parent);
        void _createMQTTTab(lv_obj_t* parent);

        lv_obj_t* _createCard(lv_obj_t* parent, lv_color_t border_color);
        lv_obj_t* _createBarRow(lv_obj_t* parent, const char* label_text, lv_color_t bar_color, lv_obj_t** label_out);

        lv_obj_t* _screen;
        lv_obj_t* _tabBtns[4];
        lv_obj_t* _tabPages[4];

        dashboard_tab_t _activeTab;

        // Sensor Tab
        lv_obj_t* _lbl_turb1;
        lv_obj_t* _lbl_turb2;
        lv_obj_t* _lbl_turb3;
        lv_obj_t* _lbl_turb_avg;
        lv_obj_t* _bar_turb1;
        lv_obj_t* _bar_turb2;
        lv_obj_t* _bar_turb3;
        lv_obj_t* _bar_turb_avg;

        lv_obj_t* _lbl_tds1;
        lv_obj_t* _lbl_tds2;
        lv_obj_t* _lbl_tds3;
        lv_obj_t* _lbl_tds_avg;
        lv_obj_t* _bar_tds1;
        lv_obj_t* _bar_tds2;
        lv_obj_t* _bar_tds3;
        lv_obj_t* _bar_tds_avg;

        lv_obj_t* _lbl_ph;
        lv_obj_t* _lbl_temp;
        lv_obj_t* _bar_ph;
        lv_obj_t* _bar_temp;

        // Info Tab
        lv_obj_t* _lbl_cpu_temp;
        lv_obj_t* _lbl_heap;
        lv_obj_t* _lbl_memory;
        lv_obj_t* _lbl_ram;
        lv_obj_t* _lbl_usage;
        lv_obj_t* _lbl_task;
        lv_obj_t* _lbl_fps;
        lv_obj_t* _lbl_infos;

        // WiFi Tab
        lv_obj_t* _lbl_wifi_status;
        lv_obj_t* _lbl_rssi;
        lv_obj_t* _lbl_ip;
        lv_obj_t* _lbl_mac;
        lv_obj_t* _lbl_ssid;
        lv_obj_t* _lbl_pass;
        lv_obj_t* _lbl_wifi_ping;

        // MQTT Tab
        lv_obj_t* _lbl_mqtt_status;
        lv_obj_t* _lbl_mqtt_conn;
        lv_obj_t* _lbl_mqtt_topic;
        lv_obj_t* _lbl_mqtt_send;
        lv_obj_t* _lbl_mqtt_host;
        lv_obj_t* _lbl_mqtt_name;
        lv_obj_t* _lbl_mqtt_ping;
};

#endif