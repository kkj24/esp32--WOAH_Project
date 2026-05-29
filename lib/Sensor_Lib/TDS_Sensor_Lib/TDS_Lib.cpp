#include "TDS_Lib.h"

TDS::TDS() {
    // No Once Func in Here
}

void TDS::calibrate(
    int16_t min1, int16_t max1,
    int16_t min2, int16_t max2,
    int16_t min3, int16_t max3
) {
    // Minimum TDS Value
    _min_TDS1 = min1;
    _min_TDS2 = min2;
    _min_TDS3 = min3;

    // Maximum TDS Value
    _max_TDS1 = max1;
    _max_TDS2 = max2;
    _max_TDS3 = max3;
}

void TDS::auto_calibrate(bool state_auto) {
    if(state_auto) {
        // While Sample Data = false
        if(!sample_data) {
            int16_t value1 = ads2.readADC_SingleEnded(ADS_CH1),
                    value2 = ads2.readADC_SingleEnded(ADS_CH2),
                    value3 = ads2.readADC_SingleEnded(ADS_CH3);
    
            // Maximum Value
            if(value1 > _max_TDS1) {
                _max_TDS1 = value1;
            }
    
            if(value2 > _max_TDS2) {
                _max_TDS2 = value2;
            }
    
            if(value3 > _max_TDS3) {
                _max_TDS3 = value3;
            }
    
            // Minimum Value
            if(value1 < _min_TDS1) {
                _min_TDS1 = value1;
            }
    
            if(value2 < _min_TDS2) {
                _min_TDS2 = value2;
            }
    
            if(value3 < _min_TDS3) {
                _min_TDS3 = value3;
            }
        // While Sample Data = true
        } else {
            int16_t value1 = ads2.readADC_SingleEnded(ADS_CH1),
                    value2 = ads2.readADC_SingleEnded(ADS_CH2),
                    value3 = ads2.readADC_SingleEnded(ADS_CH3);
    
            // Maximum Value
            if(value1 > _max_TDS1) {
                _max_TDS1 = value1;
            }
    
            if(value2 > _max_TDS2) {
                _max_TDS2 = value2;
            }
    
            if(value3 > _max_TDS3) {
                _max_TDS3 = value3;
            }
    
            // Minimum Value
            if(value1 < _min_TDS1) {
                _min_TDS1 = value1;
            }
    
            if(value2 < _min_TDS2) {
                _min_TDS2 = value2;
            }
    
            if(value3 < _min_TDS3) { 
                _min_TDS3 = value3;
            }
        }
    }
}

int16_t TDS::raw_TDS(uint8_t ch) {
    // While Data Sample = false
    if(!sample_data) {
        int16_t raw_data = ads2.readADC_SingleEnded(ch);
        return raw_data;
    // While Data Sample = true
    } else {
        int16_t raw_data = random(-4096, 4096);
        return raw_data;
    }
}

uint8_t TDS::read_TDS(uint8_t ch) {
    switch(ch) {
        
        int16_t min, 
                max;

        // CH_0
        case ADS_CH1:
            min = _min_TDS1;
            max = _max_TDS1;
        break;

        // CH_1
        case ADS_CH2:
            min = _min_TDS2;
            max = _max_TDS2;
        break;
        // CH_2
        case ADS_CH3:
            min = _min_TDS3;
            max = _max_TDS3;
        break;

        // While Sample Data = false
        if(!sample_data) {
            uint8_t TDS_data = constrain(map(ads2.readADC_SingleEnded(ch), min, max, 0, 100), 0, 100);
            return TDS_data;
        // While Sample Data = true
        } else {
            uint8_t TDS_data = constrain(map(random(-4096, 4096), min, max, 0, 100), 0, 100);
            return TDS_data;
        }
    }
}