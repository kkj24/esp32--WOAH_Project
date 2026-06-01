#include "Sensor_Setup.h"

Adafruit_ADS1115 ads1;
Adafruit_ADS1115 ads2;

bool sample_data = false;

void setup_ads::setup_all_ads() {
    Wire.begin();

    // Initialize ADS1
    while(ads1.begin(ADS1_ADDRESS)) {
        ads1.begin(ADS1_ADDRESS);
        Serial.println(F("ADS1 Initializing..."));
        delay(1000);
    }
    Serial.println(F("ADS1 Success Initializing..."));
    
    // Initialize ADS2
    while(ads2.begin(ADS2_ADDRESS)) {
        ads1.begin(ADS2_ADDRESS);
        Serial.println(F("ADS2 Initializing..."));
        delay(1000);
    }
    Serial.println(F("ADS2 Success Initializing..."));
    delay(500);
    Serial.println(F("All of ADS Normal!"));

    Serial.println(F("Configure All ADS!"));

    // Set Gain (4096)
    ads1.setGain(GAIN_ONE);
    ads2.setGain(GAIN_ONE);

    // Set Data Rate (16 Sample per Seconds)
    ads1.setDataRate(RATE_ADS1115_16SPS);
    ads2.setDataRate(RATE_ADS1115_16SPS);
}