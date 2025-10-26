#include <Arduino.h>

#include "board-config.h"
#include "lorawan.h"
#include "gps.h"

void setup() {
    Serial.begin(9600);
    Serial.println("--- Startup ---");
    setup_gps();
    setup_lorawan();
}

void loop() {
    loop_lorawan();
    loop_gps();
}
