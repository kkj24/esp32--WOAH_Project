#include "pH_Lib.h"

pH_Sensor::pH_Sensor() {
    // No Once Func at Here
}

void pH_Sensor::calibrate(int16_t min, int16_t max) {
    _pH_min = min;  // Minimum value
    _pH_max = max;  // Maximum value
}

uint8_t pH_Sensor::read_pH(uint8_t ch = ADS_CH4) {
    uint8_t pH_value = constrain(map((sample_data ? random(_pH_min, _pH_max) : ads2.readADC_SingleEnded(ch)), _pH_min, _pH_max, 0, 14), 0, 14);    // Convert raw analog read to 0 - 14 arrange
    return pH_value;    // return pH value
}

int16_t pH_Sensor::read_pH_temp(uint8_t ch = ADS_CH4) {
    int16_t pH_temp = (sample_data ? random(_pH_min, _pH_max) : ads1.readADC_SingleEnded(ch));
    return pH_temp;
}

int16_t pH_Sensor::read_raw_pH(uint8_t ch = ADS_CH4) {
    int16_t pH_raw = (sample_data ? random(_pH_min, _pH_max) : ads2.readADC_SingleEnded(ch));
    return pH_raw;
}