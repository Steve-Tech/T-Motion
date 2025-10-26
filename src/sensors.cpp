#include <Arduino.h>
#include <Wire.h>
#include <BMx280.h>

#include "board-config.h"
#include "sensors.h"

static BMx280 bmx;

const uint8_t I2C_ADDR = 0x76; // Default address

static bool bmxFound = false;

struct SensorData sensorData = {
    .temperature = NAN,
    .pressure = NAN,
    .humidity = NAN
};

void setup_sensors() {
    Wire.setSDA(SOC_GPIO_PIN_SDA);
    Wire.setSCL(SOC_GPIO_PIN_SCL);
    if (!bmx.beginI2C(I2C_ADDR, &Wire)) {
        Serial.println("BMx280 not found");
        bmxFound = false;
        return;
    }
    bmxFound = true;

    bmx.setSampling(
        1, // temp oversampling x1
        1, // pressure oversampling x1
        1, // humidity oversampling x1 (ignored on BMP280)
        0, // IIR filter off
        0, // standby
        BMx280::MODE_NORMAL // start in continuous mode
    );
}

void update_sensors() {
    if (bmxFound) {
        float T, P_hPa, H;
        if (bmx.read280(T, P_hPa, H)) {
            sensorData.temperature = T;
            sensorData.pressure = P_hPa;
            if (bmx.hasHumidity())
                sensorData.humidity = H;
        }
    }
}
