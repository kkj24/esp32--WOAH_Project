#include "Turbidity_Lib.h"

Turbidity_Sensor::Turbidity_Sensor() {
    // No Func at Here
};

void Turbidity_Sensor::calibrate(
    int16_t min1, int16_t max1,
    int16_t min2, int16_t max2,
    int16_t min3, int16_t max3
) {
    // minimum raw data
    _min_turbidity1 = min1;
    _min_turbidity2 = min2;
    _min_turbidity3 = min3;

    // maximum raw data
    _max_turbidity1 = max1;
    _max_turbidity2 = max2;
    _max_turbidity3 = max3;
}

void Turbidity_Sensor::auto_calibrate(bool auto_state) {
    if(auto_state) {
        // Read Turbidity1 - Turbidity3
        int16_t Turbidity1 = (sample_data ? random(-4096, 4096) : ads1.readADC_SingleEnded(TURBIDITY1)),
                Turbidity2 = (sample_data ? random(-4096, 4096) : ads1.readADC_SingleEnded(TURBIDITY2)),
                Turbidity3 = (sample_data ? random(-4096, 4096) : ads1.readADC_SingleEnded(TURBIDITY3));

        // Configure Turbidity1
        if(Turbidity1 < _min_turbidity1)        // Min
            _min_turbidity1 = Turbidity1;
        else if(Turbidity1 > _max_turbidity1)   // Max

        // Configure Turbidity2
        if(Turbidity2 < _min_turbidity2)        // Min
            _min_turbidity2 = Turbidity2;
        else if(Turbidity2 > _max_turbidity2)   // Max

        // Configure Turbidity3
        if(Turbidity3 < _min_turbidity3)        // Min
            _min_turbidity3 = Turbidity3;
        else if(Turbidity3 > _max_turbidity3)   // Max
            _max_turbidity3 = Turbidity3;
    }
}

uint8_t Turbidity_Sensor::getTurbidity_read(uint8_t ch) {
    int16_t min, max;

    switch(ch) {
        case TURBIDITY1:
            min = _min_turbidity1;
            max = _max_turbidity1;
        break;

        case TURBIDITY2:
            min = _min_turbidity2;
            max = _max_turbidity2;
        break;


        case TURBIDITY3:
            min = _min_turbidity3;
            max = _max_turbidity3;
        break;
    }

    uint8_t Turbidity_data = constrain(map((sample_data ? random(-4096, 4096) : ads1.readADC_SingleEnded(ch)), min, max, 0, 100), 0, 100);
    return Turbidity_data;
}

int16_t Turbidity_Sensor::readTurbidity_raw(uint8_t ch) {
    int16_t Turbidity_raw = (sample_data ? random(-4096, 4096) : ads1.readADC_SingleEnded(ch));
    return Turbidity_raw;
}