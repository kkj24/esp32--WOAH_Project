#ifndef ADS_SENSOR_SETUP
#define ADS_SENSOR_SETUP


#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Channel
constexpr uint8_t ADS_CH1 = 0;
constexpr uint8_t ADS_CH2 = 1;
constexpr uint8_t ADS_CH3 = 2;
constexpr uint8_t ADS_CH4 = 3;

extern Adafruit_ADS1115 ads1;
extern Adafruit_ADS1115 ads2;

// Address
#define ADS1_ADDRESS 0x48
#define ADS2_ADDRESS 0x49

class setup_ads {
    public:
        void setup_all_ads();

    private:

};

#endif

/*

ADS1 pin connection Spesification:
* A0 - Turbidity1
* A1 - Turbidity2
* A2 - Turbidity3
* A3 - pH Temp (TO)

* ADS1 (ADS1115) ADDR pin Connected to GND, and the Address is 0x48 at Hex Format

ADS2 pin connection Spesification:
* A0 - TDS1
* A1 - TDS2
* A2 - TDS3
* A3 - Water pH (PO)

* ADS2 (ADS1115) ADDR pin Connected to VCC, and the Address is 0x49 at Hex Format

*/