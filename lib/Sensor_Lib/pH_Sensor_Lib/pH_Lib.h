#ifndef PH_SENSOR_LIBRARIES
#define PH_SENSOR_LIBRARIES

#include "Sensor_Setup.h"

class pH_Sensor {
    public:
        pH_Sensor();

        // Manually Calibrate
        void calibrate(int16_t min, int16_t max);

        // read raw pH Sensor
        int16_t read_raw_pH(uint8_t ch = ADS_CH4);
        
        // read raw pH temperature Sensor (Not Calibrated)
        int16_t read_pH_temp(uint8_t ch = ADS_CH4);

        // Read pH (range 0 - 100 "%")
        uint8_t read_pH(uint8_t ch = ADS_CH4);

    private:
        // Minimum pH Value
        int16_t _pH_min = 0;

        // Maximum pH Value
        int16_t _pH_max = 0;
};

#endif