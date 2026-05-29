#include "Turbidity_Lib.h"

Turbidity::Turbidity() {
    // No Once Function in Here
};

int16_t Turbidity::readTurbidity(uint8_t ch) {
    if(!sample_data) 
        return ads1.readADC_SingleEnded(ch);
    else
        return ads1.readADC_SingleEnded(ch);
}

uint8_t Turbidity::getTurbidity_read(uint8_t ch) {
    switch(ch) {

        int16_t min,
                max;

        // ch 0
        case ADS_CH1:
            min = _min_turbidity1;
            max = _max_turbidity1;
        break;

        // ch 1
        case ADS_CH2:
            min = _min_turbidity2;
            max = _max_turbidity2;
        break;

        // ch 2
        case ADS_CH3:
            min = _min_turbidity3;
            max = _max_turbidity3;
        break;
    
    // While Sample Data = false
    if(!sample_data) {
        uint8_t turbidity_data = constrain(map(ads1.readADC_SingleEnded(ch), min, max, 0, 100), 0, 100);
        return turbidity_data;
    // While Sample Data = true
    } else {
        uint8_t turbidity_data = constrain(map(random(-4096, 4096), min, max, 0, 100), 0, 100);
        return turbidity_data;
    }

    }
}

void Turbidity::calibrate(
    int16_t min1, int16_t max1,
    int16_t min2, int16_t max2,
    int16_t min3, int16_t max3
) {
    // Set Minimum
    _min_turbidity1 = min1;
    _min_turbidity2 = min2;
    _min_turbidity3 = min3;

    // Set Maximum
    _max_turbidity1 = max1;
    _max_turbidity2 = max2;
    _max_turbidity3 = max3;
}

void Turbidity::auto_calibrate(bool auto_state) {
    if(auto_state) {
        // While Sample Data = false
        if(!sample_data) {
            int16_t value1 = ads1.readADC_SingleEnded(ADS_CH1),
                    value2 = ads1.readADC_SingleEnded(ADS_CH2),
                    value3 = ads1.readADC_SingleEnded(ADS_CH3);
        
            // Set Maximum
            if(value1 > _max_turbidity1) {
                _max_turbidity1 = value1;
            }
        
            if(value2 > _max_turbidity2) {
                _max_turbidity2 = value2;
            }
        
            if(value3 > _max_turbidity3) {
                _max_turbidity3 = value3;
            }
        
            // Set Minimum
            if(value1 < _min_turbidity1) {
                _min_turbidity1 = value1;
            }
        
            if(value2 < _min_turbidity2) {
                _min_turbidity2 = value2;
            }
        
            if(value1 < _min_turbidity1) {
                _min_turbidity3 = value3;
        }
        
        // While Sample Data = True
        } else {
            int16_t value1 = random(-4096, 4096),
                    value2 = random(-4096, 4096),
                    value3 = random(-4096, 4096);
        
            // Set Maximum
            if(value1 > _max_turbidity1) {
                _max_turbidity1 = value1;
            }
        
            if(value2 > _max_turbidity2) {
                _max_turbidity2 = value2;
            }
        
            if(value3 > _max_turbidity3) {
                _max_turbidity3 = value3;
            }
        
            // Set Minimum
            if(value1 < _min_turbidity1) {
                _min_turbidity1 = value1;
            }
        
            if(value2 < _min_turbidity2) {
                _min_turbidity2 = value2;
            }
        
            if(value3 < _min_turbidity3) {
                _min_turbidity3 = value3;
            }
        }
    }
}