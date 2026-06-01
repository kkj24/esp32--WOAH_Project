#ifndef TDS_SENSOR_LIBRARIES
#define TDS_SENSOR_LIBRARIES

#include "Sensor_Setup.h"

#define TDS1 0
#define TDS2 1
#define TDS3 2

class TDS_Sensor {
    public:
        TDS_Sensor();

        // Auto Calibrate (Auto range), must be looping
        void auto_calibrate(bool state_auto);

        // Manually Calibrate
        void calibrate(
            int16_t min1, int16_t max1,
            int16_t min2, int16_t max2,
            int16_t min3, int16_t max3
        );

        // read raw data TDS Sensor
        int16_t raw_TDS(uint8_t ch);

        // Must be calibrate
        // read data TDS Sensor (range 0 - 100 "%")
        uint8_t read_TDS(uint8_t ch);

    private:
    // Min TDS Config Value 
        int16_t _min_TDS1 = 0, 
                _min_TDS2 = 0, 
                _min_TDS3 = 0;

    // Max TDS Config Value
        int16_t _max_TDS1 = 0, 
                _max_TDS2 = 0, 
                _max_TDS3 = 0;
};

#endif