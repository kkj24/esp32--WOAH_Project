#include "pH_Lib.h"

pH_Sensor::pH_Sensor() {
    // No Once Function in Here
};

void pH_Sensor::calibrate(int16_t min, int16_t max) {
    _pH_min = min;
    _pH_max = max;
}

int16_t pH_Sensor::read_raw_pH(uint8_t ch) {
    // While Sample Data = false
    if(!sample_data) {
        int16_t pH_data = ads2.readADC_SingleEnded(ch);
        return pH_data;
    // While Sample Data = true
    } else {
        int16_t pH_data = random(-4096, 4096);
        return pH_data;
    }
}

int16_t pH_Sensor::read_pH_temp(uint8_t ch) {
    // While Sample Data = false
    if(!sample_data) {
        int16_t temp_data = ads1.readADC_SingleEnded(ch);
        return temp_data;
    // While Sample Data = true
    } else {
        int16_t temp_data = random(-4096, 4096);
        return temp_data;
    }
}

uint8_t pH_Sensor::read_pH(uint8_t ch) {
    // While Sample Data = false
    if(!sample_data) {
        uint8_t pH_data = constrain(map(ads2.readADC_SingleEnded(ch), _pH_min, _pH_max, 0, 100), 0, 100);
        return pH_data;
    // While Sample Data = true
    } else {
        uint8_t pH_data = constrain(map(random(-4096, 4096), _pH_min, _pH_max, 0, 100), 0, 100);
        return pH_data;
    }
}