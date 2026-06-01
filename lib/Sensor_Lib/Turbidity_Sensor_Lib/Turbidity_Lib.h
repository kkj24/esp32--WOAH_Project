#ifndef TURBIDITY_LIBRARIES_SENSOR
#define TURBIDITY_LIBRARIES_SENSOR

#include "Sensor_Setup.h"

#define TURBIDITY1 0
#define TURBIDITY2 1
#define TURBIDITY3 2

class Turbidity_Sensor {
    public:
        Turbidity_Sensor();

        // Manually Calibrate
        void calibrate(
            int16_t min1, int16_t max1,
            int16_t min2, int16_t max2,
            int16_t min3, int16_t max3
        );

        // Auto Calibrate, must be looping
        void auto_calibrate(bool auto_state);

        // Must be calibrate
        // get turbidity read (range 0 - 100 "%")
        uint8_t getTurbidity_read(uint8_t ch);

        // get turbidity read raw with channel reference
        int16_t readTurbidity_raw(uint8_t ch);

    private:
    // minimum raw data
        int16_t _min_turbidity1 = 0, 
                _min_turbidity2 = 0, 
                _min_turbidity3 = 0;

    // maximum raw data
        int16_t _max_turbidity1 = 0,
                _max_turbidity2 = 0, 
                _max_turbidity3 = 0;
};

#endif