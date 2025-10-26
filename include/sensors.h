#pragma once
#include <math.h>

void setup_sensors();
void update_sensors();

struct SensorData {
    float temperature;
    float pressure;
    float humidity;
};
extern SensorData sensorData;
