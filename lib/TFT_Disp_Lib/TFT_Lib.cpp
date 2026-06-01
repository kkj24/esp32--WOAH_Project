#include "TFT_Lib.h"

/*
Specification: https://github.com/ianlunam/esp32-mpi3501

DRIVER:         ILI9486
TOUCH:          XPT2046
MODEL:          SKU:MPI3501

Power Input 5v	                    2	 -   1	    Power Input 3.3v
Power Input 5v	                    4	 -   3	    SDA
GND	                                6	 -   5	    SCL
TX	                                8	 -   7	    P7
RX	                                10	 -   9	    GND
P1	                                12	 -   11	    P0
GND	                                14	 -   13	    P2
P4	                                16	 -   15	    P3
LCD Reset LCD_RST	                18	 -   17	    Power Input 3.3v
GND	                                20	 -   19	    Touch Panel Input MOSI
Touch Panel Reset TP_RST	        22	 -   21	    Touch Panel Output MISO
LCD Chip Select LCD_CS	            24	 -   23	    Touch Panel Clock SCLK
Touch Panel Chip Select TP_CS	    26	 -   25	    GND

esp32::

MISO:           19
MOSI:           23
CLK:            18
DC(LCD_RST):    32
TP_RST:         33
CS_TP:          4
CS_LCD:         5
*/

class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_ILI9486 _panel;
    lgfx::Bus_SPI       _bus;
    lgfx::Touch_XPT2046 _touch;

public:
    LGFX(void) {
        {
            auto cfg = _bus.config();
            cfg.spi_host   = VSPI_HOST;
            cfg.spi_mode   = 0;
            cfg.freq_write = 39000000;
            cfg.freq_read  = 16000000;
            cfg.pin_sclk   = 18;
            cfg.pin_mosi   = 23;
            cfg.pin_miso   = 19;
            cfg.pin_dc     = 32;
            _bus.config(cfg);
            _panel.setBus(&_bus);
        }
        {
            auto cfg = _panel.config();
            cfg.pin_cs   = 5;
            cfg.pin_rst  = 33;
            cfg.pin_busy = -1;
            cfg.memory_width  = TFT_SCREEN_WIDTH;
            cfg.memory_height = TFT_SCREEN_HEIGHT;
            cfg.panel_width   = TFT_SCREEN_WIDTH;
            cfg.panel_height  = TFT_SCREEN_HEIGHT;
            cfg.offset_x      = 0;
            cfg.offset_y      = 0;
            cfg.offset_rotation = 0;
            cfg.readable   = true;
            cfg.invert     = false;
            cfg.rgb_order  = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = true;
            _panel.config(cfg);
        }

        {
            auto cfg = _touch.config();
            cfg.x_min      = 300;
            cfg.x_max      = 3900;
            cfg.y_min      = 400;
            cfg.y_max      = 3900;
            cfg.pin_int    = -1;
            cfg.bus_shared = true;
            cfg.offset_rotation = 4;
            cfg.spi_host   = VSPI_HOST;
            cfg.freq       = 1000000;
            cfg.pin_sclk   = 18;
            cfg.pin_mosi   = 23;
            cfg.pin_miso   = 19;
            cfg.pin_cs     = 4;
            _touch.config(cfg);
            _panel.setTouch(&_touch);
        }
        setPanel(&_panel);
    }
};

static LGFX tft;
static lv_color_t buf1[TFT_BUF_SIZE];
static lv_color_t buf2[TFT_BUF_SIZE];

static void my_disp_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.writePixels((lgfx::rgb565_t*)px_map, w * h);
    tft.endWrite();
    lv_display_flush_ready(disp);
}

static void my_touchpad_read(lv_indev_t* indev, lv_indev_data_t* data) {
    uint16_t tx, ty;
    if(tft.getTouch(&tx, &ty)) {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = tx;
        data->point.y = ty;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static const char* tabNames[] = {"Sensor", "Info", "WiFi", "MQTT"};

myTFT::myTFT() {
    _activeTab = TAB_SENSOR;
    _screen = nullptr;

    for(int i = 0; i < 4; i++) {
        _tabBtns[i] = nullptr;
        _tabPages[i] = nullptr;
    }
}

void myTFT::begin() {
    tft.begin();
    tft.setRotation(0);

    lv_init();

    lv_display_t* disp = lv_display_create(TFT_SCREEN_WIDTH, TFT_SCREEN_HEIGHT);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_indev_t* indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);
}

void myTFT::lv_timer() {
    lv_timer_handler();
}

void myTFT::dashboard() {
    _screen = lv_scr_act();
    lv_obj_set_style_bg_color(_screen, lv_color_hex(0x000000), 0);

    _createBackground(_screen);
    _createTabBar(_screen);

    for(int i = 0; i < 4; i++) {
        _tabPages[i] = lv_obj_create(_screen);
        lv_obj_set_size(_tabPages[i], TFT_SCREEN_WIDTH, TFT_SCREEN_HEIGHT - 90);
        lv_obj_set_pos(_tabPages[i], 0, 90);
        lv_obj_set_style_bg_opa(_tabPages[i], LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(_tabPages[i], 0, 0);
        lv_obj_set_style_pad_all(_tabPages[i], 8, 0);
        lv_obj_set_flex_flow(_tabPages[i], LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(_tabPages[i], LV_FLEX_ALIGN_START, 0);
        lv_obj_set_flex_align(_tabPages[i], LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_scrollbar_mode(_tabPages[i], LV_SCROLLBAR_MODE_OFF);
    }

    _createSensorTab(_tabPages[TAB_SENSOR]);
    _createInfoTab(_tabPages[TAB_INFO]);
    _createWiFiTab(_tabPages[TAB_WIFI]);
    _createMQTTTab(_tabPages[TAB_MQTT]);

    switchTab(TAB_SENSOR);
}

void myTFT::_createBackground(lv_obj_t* parent) {
    lv_obj_set_style_bg_grad_color(parent, lv_color_hex(0xE53935), 0);
    lv_obj_set_style_bg_color(parent, lv_color_hex(0x7B1FA2), 0);
    lv_obj_set_style_bg_grad_dir(parent, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_bg_opa(parent, LV_OPA_COVER, 0);
}

void myTFT::_createTabBar(lv_obj_t* parent) {
    lv_obj_t* title = lv_label_create(parent);
    lv_label_set_text(title, "WOAH Dashboard");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 8);

    lv_obj_t* tabContainer = lv_obj_create(parent);
    lv_obj_set_size(tabContainer, 280, 38);
    lv_obj_align(tabContainer, LV_ALIGN_TOP_MID, 0, 42);
    lv_obj_set_style_bg_color(tabContainer, lv_color_hex(0xF57C00), 0);
    lv_obj_set_style_bg_opa(tabContainer, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(tabContainer, 19, 0);
    lv_obj_set_style_border_color(tabContainer, lv_color_hex(0xFFAB40), 0);
    lv_obj_set_style_border_width(tabContainer, 2, 0);
    lv_obj_set_style_pad_all(tabContainer, 4, 0);
    lv_obj_set_flex_flow(tabContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(tabContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(tabContainer, LV_SCROLLBAR_MODE_OFF);

    for(int i = 0; i < 4; i++) {
        _tabBtns[i] = lv_btn_create(tabContainer);
        lv_obj_set_size(_tabBtns[i], 62, 28);
        lv_obj_set_style_radius(_tabBtns[i], 14, 0);
        lv_obj_set_style_bg_opa(_tabBtns[i], LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(_tabBtns[i], 0, 0);
        lv_obj_set_style_shadow_width(_tabBtns[i], 0, 0);
        lv_obj_set_style_pad_all(_tabBtns[i], 0, 0);

        lv_obj_t* lbl = lv_label_create(_tabBtns[i]);
        lv_label_set_text(lbl, tabNames[i]);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), 0);
        lv_obj_center(lbl);

        lv_obj_add_event_cb(_tabBtns[i], [](lv_event_t* e) {
            myTFT* self = (myTFT*)lv_event_get_user_data(e);
            lv_obj_t* btn = (lv_obj_t*)lv_event_get_target(e);
            for(int j = 0; j < 4; j++) {
                if(self->_tabBtns[j] == btn) {
                    self->switchTab((dashboard_tab_t)j);
                    break;
                }
            }
        }, LV_EVENT_CLICKED, this);
    }
}

void myTFT::switchTab(dashboard_tab_t tab) {
    _activeTab = tab;
    for(int i = 0; i < 4; i++) {
        if(i == tab) {
            lv_obj_clear_flag(_tabPages[i], LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(_tabBtns[i], lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_bg_opa(_tabBtns[i], LV_OPA_30, 0);
            lv_obj_t* lbl = lv_obj_get_child(_tabBtns[i], 0);
            lv_obj_set_style_text_color(lbl, lv_color_hex(0xFF6D00), 0);
        } else {
            lv_obj_add_flag(_tabPages[i], LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_opa(_tabBtns[i], LV_OPA_TRANSP, 0);
            lv_obj_t* lbl = lv_obj_get_child(_tabBtns[i], 0);
            lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), 0);
        }
    }
}

lv_obj_t* myTFT::_createCard(lv_obj_t* parent, lv_color_t border_color) {
    lv_obj_t* card = lv_obj_create(parent);
    lv_obj_set_width(card, lv_pct(100));
    lv_obj_set_height(card, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(card, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(card, LV_OPA_20, 0);
    lv_obj_set_style_radius(card, 16, 0);
    lv_obj_set_style_border_color(card, border_color, 0);
    lv_obj_set_style_border_width(card, 2, 0);
    lv_obj_set_style_border_opa(card, LV_OPA_40, 0);
    lv_obj_set_style_pad_all(card, 10, 0);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_flex_main_place(card, LV_FLEX_ALIGN_START, 0);
    lv_obj_set_scrollbar_mode(card, LV_SCROLLBAR_MODE_OFF);
    return card;
}

lv_obj_t* myTFT::_createBarRow(lv_obj_t* parent, const char* label_text, lv_color_t bar_color, lv_obj_t** label_out) {
    lv_obj_t* row = lv_obj_create(parent);
    lv_obj_set_width(row, lv_pct(100));
    lv_obj_set_height(row, 22);
    lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(row, 0, 0);
    lv_obj_set_style_pad_all(row, 0, 0);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(row, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t* lbl = lv_label_create(row);
    lv_label_set_text(lbl, label_text);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_12, 0);
    lv_obj_set_width(lbl, 110);
    *label_out = lbl;

    lv_obj_t* bar = lv_bar_create(row);
    lv_obj_set_flex_grow(bar, 1);
    lv_obj_set_height(bar, 12);
    lv_bar_set_range(bar, 0, 100);
    lv_bar_set_value(bar, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(bar, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(bar, LV_OPA_30, 0);
    lv_obj_set_style_radius(bar, 6, 0);
    lv_obj_set_style_bg_color(bar, bar_color, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, LV_PART_INDICATOR);
    lv_obj_set_style_radius(bar, 6, LV_PART_INDICATOR);

    return bar;
}

void myTFT::_createSensorTab(lv_obj_t* parent) {
    lv_obj_t* card_turb = _createCard(parent, lv_color_hex(0xE040FB));

    _bar_turb1   = _createBarRow(card_turb, "Turbidity1: -%", lv_color_hex(0xE040FB), &_lbl_turb1);
    _bar_turb2   = _createBarRow(card_turb, "Turbidity2: -%", lv_color_hex(0xE040FB), &_lbl_turb2);
    _bar_turb3   = _createBarRow(card_turb, "Turbidity3: -%", lv_color_hex(0xE040FB), &_lbl_turb3);
    _bar_turb_avg = _createBarRow(card_turb, "Average: -%", lv_color_hex(0xE040FB), &_lbl_turb_avg);

    lv_obj_t* card_tds = _createCard(parent, lv_color_hex(0x90CAF9));

    _bar_tds1   = _createBarRow(card_tds, "TDS1: -%", lv_color_hex(0x90CAF9), &_lbl_tds1);
    _bar_tds2   = _createBarRow(card_tds, "TDS2: -%", lv_color_hex(0x90CAF9), &_lbl_tds2);
    _bar_tds3   = _createBarRow(card_tds, "TDS3: -%", lv_color_hex(0x90CAF9), &_lbl_tds3);
    _bar_tds_avg = _createBarRow(card_tds, "Average: -%", lv_color_hex(0x90CAF9), &_lbl_tds_avg);

    lv_obj_t* card_ph = _createCard(parent, lv_color_hex(0xFFCC80));

    _bar_ph   = _createBarRow(card_ph, "pH Water:-", lv_color_hex(0xFFCC80), &_lbl_ph);
    _bar_temp = _createBarRow(card_ph, "Temp: -'C", lv_color_hex(0xFFCC80), &_lbl_temp);
}

void myTFT::_createInfoTab(lv_obj_t* parent) {
    lv_obj_t* card = _createCard(parent, lv_color_hex(0xCE93D8));

    const char* labels[] = {
        "CPU Temp: -'C", "Heap:", "Memory:", "RAM:",
        "Usage:", "Task:", "FPS:", "Info's:"
    };
    lv_obj_t** ptrs[] = {
        &_lbl_cpu_temp, &_lbl_heap, &_lbl_memory, &_lbl_ram,
        &_lbl_usage, &_lbl_task, &_lbl_fps, &_lbl_infos
    };

    for(int i = 0; i < 8; i++) {
        lv_obj_t* lbl = lv_label_create(card);
        lv_label_set_text(lbl, labels[i]);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0x000000), 0);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
        lv_obj_set_style_pad_ver(lbl, 6, 0);
        *ptrs[i] = lbl;
    }
}

void myTFT::_createWiFiTab(lv_obj_t* parent) {
    lv_obj_t* card = _createCard(parent, lv_color_hex(0xF48FB1));

    const char* labels[] = {
        "WiFi Status:", "RSSI:", "IP:", "Mac:",
        "SSID:", "PASS:", "Ping:"
    };
    lv_obj_t** ptrs[] = {
        &_lbl_wifi_status, &_lbl_rssi, &_lbl_ip, &_lbl_mac,
        &_lbl_ssid, &_lbl_pass, &_lbl_wifi_ping
    };

    for(int i = 0; i < 7; i++) {
        lv_obj_t* lbl = lv_label_create(card);
        lv_label_set_text(lbl, labels[i]);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0x000000), 0);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
        lv_obj_set_style_pad_ver(lbl, 6, 0);
        *ptrs[i] = lbl;
    }
}

void myTFT::_createMQTTTab(lv_obj_t* parent) {
    lv_obj_t* card = _createCard(parent, lv_color_hex(0xCE93D8));

    const char* labels[] = {
        "MQTT Status:", "Connection:", "Topic:", "Send:",
        "Host:", "Name:", "Ping:"
    };
    lv_obj_t** ptrs[] = {
        &_lbl_mqtt_status, &_lbl_mqtt_conn, &_lbl_mqtt_topic, &_lbl_mqtt_send,
        &_lbl_mqtt_host, &_lbl_mqtt_name, &_lbl_mqtt_ping
    };

    for(int i = 0; i < 7; i++) {
        lv_obj_t* lbl = lv_label_create(card);
        lv_label_set_text(lbl, labels[i]);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0x000000), 0);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
        lv_obj_set_style_pad_ver(lbl, 6, 0);
        *ptrs[i] = lbl;
    }
}

void myTFT::updateSensor(sensor_data_t data) {
    char buf[32];

    snprintf(buf, sizeof(buf), "Turbidity1: %.0f%%", data.turbidity1);
    lv_label_set_text(_lbl_turb1, buf);
    lv_bar_set_value(_bar_turb1, (int32_t)data.turbidity1, LV_ANIM_ON);

    snprintf(buf, sizeof(buf), "Turbidity2: %.0f%%", data.turbidity2);
    lv_label_set_text(_lbl_turb2, buf);
    lv_bar_set_value(_bar_turb2, (int32_t)data.turbidity2, LV_ANIM_ON);

    snprintf(buf, sizeof(buf), "Turbidity3: %.0f%%", data.turbidity3);
    lv_label_set_text(_lbl_turb3, buf);
    lv_bar_set_value(_bar_turb3, (int32_t)data.turbidity3, LV_ANIM_ON);

    snprintf(buf, sizeof(buf), "Average: %.0f%%", data.turbidity_avg);
    lv_label_set_text(_lbl_turb_avg, buf);
    lv_bar_set_value(_bar_turb_avg, (int32_t)data.turbidity_avg, LV_ANIM_ON);

    snprintf(buf, sizeof(buf), "TDS1: %.0f%%", data.tds1);
    lv_label_set_text(_lbl_tds1, buf);
    lv_bar_set_value(_bar_tds1, (int32_t)data.tds1, LV_ANIM_ON);

    snprintf(buf, sizeof(buf), "TDS2: %.0f%%", data.tds2);
    lv_label_set_text(_lbl_tds2, buf);
    lv_bar_set_value(_bar_tds2, (int32_t)data.tds2, LV_ANIM_ON);

    snprintf(buf, sizeof(buf), "TDS3: %.0f%%", data.tds3);
    lv_label_set_text(_lbl_tds3, buf);
    lv_bar_set_value(_bar_tds3, (int32_t)data.tds3, LV_ANIM_ON);

    snprintf(buf, sizeof(buf), "Average: %.0f%%", data.tds_avg);
    lv_label_set_text(_lbl_tds_avg, buf);
    lv_bar_set_value(_bar_tds_avg, (int32_t)data.tds_avg, LV_ANIM_ON);

    snprintf(buf, sizeof(buf), "pH Water:%.1f", data.ph_water);
    lv_label_set_text(_lbl_ph, buf);
    lv_bar_set_value(_bar_ph, (int32_t)data.ph_water, LV_ANIM_ON);

    snprintf(buf, sizeof(buf), "Temp: %.1f'C", data.temperature);
    lv_label_set_text(_lbl_temp, buf);
    lv_bar_set_value(_bar_temp, (int32_t)data.temperature, LV_ANIM_ON);
}

void myTFT::updateInfo(info_data_t data) {
    char buf[48];

    snprintf(buf, sizeof(buf), "CPU Temp: %.1f'C", data.cpu_temp);
    lv_label_set_text(_lbl_cpu_temp, buf);

    snprintf(buf, sizeof(buf), "Heap: %u", data.heap);
    lv_label_set_text(_lbl_heap, buf);

    snprintf(buf, sizeof(buf), "Memory: %u", data.memory);
    lv_label_set_text(_lbl_memory, buf);

    snprintf(buf, sizeof(buf), "RAM: %u", data.ram);
    lv_label_set_text(_lbl_ram, buf);

    snprintf(buf, sizeof(buf), "Usage: %.1f%%", data.usage);
    lv_label_set_text(_lbl_usage, buf);

    snprintf(buf, sizeof(buf), "Task: %u", data.task_count);
    lv_label_set_text(_lbl_task, buf);

    snprintf(buf, sizeof(buf), "FPS: %u", data.fps);
    lv_label_set_text(_lbl_fps, buf);

    snprintf(buf, sizeof(buf), "Info's: %s", data.infos.c_str());
    lv_label_set_text(_lbl_infos, buf);
}

void myTFT::updateWiFi(wifi_data_t data) {
    char buf[48];

    snprintf(buf, sizeof(buf), "WiFi Status: %s", data.status.c_str());
    lv_label_set_text(_lbl_wifi_status, buf);

    snprintf(buf, sizeof(buf), "RSSI: %d dBm", data.rssi);
    lv_label_set_text(_lbl_rssi, buf);

    snprintf(buf, sizeof(buf), "IP: %s", data.ip.c_str());
    lv_label_set_text(_lbl_ip, buf);

    snprintf(buf, sizeof(buf), "Mac: %s", data.mac.c_str());
    lv_label_set_text(_lbl_mac, buf);

    snprintf(buf, sizeof(buf), "SSID: %s", data.ssid.c_str());
    lv_label_set_text(_lbl_ssid, buf);

    snprintf(buf, sizeof(buf), "PASS: %s", data.pass.c_str());
    lv_label_set_text(_lbl_pass, buf);

    snprintf(buf, sizeof(buf), "Ping: %u ms", data.ping);
    lv_label_set_text(_lbl_wifi_ping, buf);
}

void myTFT::updateMQTT(mqtt_data_t data) {
    char buf[48];

    snprintf(buf, sizeof(buf), "MQTT Status: %s", data.status.c_str());
    lv_label_set_text(_lbl_mqtt_status, buf);

    snprintf(buf, sizeof(buf), "Connection: %s", data.connection.c_str());
    lv_label_set_text(_lbl_mqtt_conn, buf);

    snprintf(buf, sizeof(buf), "Topic: %s", data.topic.c_str());
    lv_label_set_text(_lbl_mqtt_topic, buf);

    snprintf(buf, sizeof(buf), "Send: %s", data.send.c_str());
    lv_label_set_text(_lbl_mqtt_send, buf);

    snprintf(buf, sizeof(buf), "Host: %s", data.host.c_str());
    lv_label_set_text(_lbl_mqtt_host, buf);

    snprintf(buf, sizeof(buf), "Name: %s", data.name.c_str());
    lv_label_set_text(_lbl_mqtt_name, buf);

    snprintf(buf, sizeof(buf), "Ping: %u ms", data.ping);
    lv_label_set_text(_lbl_mqtt_ping, buf);
}
