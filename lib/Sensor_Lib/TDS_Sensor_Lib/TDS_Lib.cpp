#include "TDS_Lib.h"

TDS_Sensor::TDS_Sensor() {
    // No Once func at here
};

void TDS_Sensor::calibrate(
    int16_t min1, int16_t max1,
    int16_t min2, int16_t max2,
    int16_t min3, int16_t max3
) {
    // Min TDS Config Value
    _min_TDS1 = min1;
    _min_TDS2 = min2;
    _min_TDS3 = min3;

    // Max TDS Config Value
    _max_TDS1 = max1;
    _max_TDS2 = max2;
    _max_TDS3 = max3;
}

void TDS_Sensor::auto_calibrate(bool state_auto) {
    if(state_auto) {
        // Read TDS1 - TDS3
        int16_t raw_TDS1 = (sample_data ? random(-4096, 4096) : raw_TDS(TDS1)),
                raw_TDS2 = (sample_data ? random(-4096, 4096) : raw_TDS(TDS2)),
                raw_TDS3 = (sample_data ? random(-4096, 4096) : raw_TDS(TDS3));

        // Configure TDS1
        if(raw_TDS1 < _min_TDS1)        // Min
            _min_TDS1 = raw_TDS1;
        else if(raw_TDS1 > _max_TDS1)   // Max
            _max_TDS1 = raw_TDS1;
            
        // Configure TDS2 
        if(raw_TDS2 < _min_TDS2)        // Min
            _min_TDS2 = raw_TDS2;
        else if(raw_TDS2 > _max_TDS2)   // Max
            _max_TDS2 = raw_TDS2;
                
        // Configure TDS3
        if(raw_TDS3 < _min_TDS3)        // Min
            _min_TDS3 = raw_TDS3;
        else if(raw_TDS3 > _max_TDS3)   // Max
            _max_TDS3 = raw_TDS3;
    }
}

int16_t TDS_Sensor::raw_TDS(uint8_t ch) {
    int16_t TDS_raw = (sample_data ? random(-4096, 4096) : ads2.readADC_SingleEnded(ch));
    return TDS_raw;
}

uint8_t TDS_Sensor::read_TDS(uint8_t ch) {
    int16_t min, max;

    switch(ch) {
        case TDS1:
            min = _min_TDS1;
            max = _max_TDS1;
        break;

        case TDS2:
            min = _min_TDS2;
            max = _max_TDS2;
        break;

        case TDS3:
            min = _min_TDS3;
            max = _max_TDS3;
        break;
    }

    uint8_t TDS_data = constrain(map((sample_data ? random(-4096, 4096) :ads2.readADC_SingleEnded(ch)), min, max, 0, 100), 0, 100);
    return TDS_data;
}