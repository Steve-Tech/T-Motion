#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <TimeLib.h>

#include "board-config.h"
#include "gps.h"

#define ENABLE_GPS_DEBUG 0

HardwareSerial SERIAL_GPS(SOC_GPIO_PIN_GNSS_RX, SOC_GPIO_PIN_GNSS_TX);
TinyGPSPlus gps;

bool gpsStarted = false;

void setup_gps() {
    SERIAL_GPS.begin(GPS_BAUDRATE);

    /* drive GNSS RST pin low */
    pinMode(SOC_GPIO_PIN_GNSS_RST, OUTPUT);
    digitalWrite(SOC_GPIO_PIN_GNSS_RST, LOW);

    /* activate 1.8V<->3.3V level shifters */
    pinMode(SOC_GPIO_PIN_GNSS_LS,  OUTPUT);
    digitalWrite(SOC_GPIO_PIN_GNSS_LS,  HIGH);

    /* keep RST low to ensure proper IC reset */
    delay(200);

    /* release */
    digitalWrite(SOC_GPIO_PIN_GNSS_RST, HIGH);

    /* give Sony GNSS few ms to warm up */
    delay(100);

    /* Leave pin floating */
    pinMode(SOC_GPIO_PIN_GNSS_RST, INPUT);

    // SERIAL_GPS.write("@VER\r\n");

    /* Idle */
    // SERIAL_GPS.write("@GSTP\r\n");      delay(250);

    /* GGA + GSA + RMC */
    SERIAL_GPS.write("@BSSL 0x25\r\n");
    Serial.println(SERIAL_GPS.readStringUntil('\n'));
    // delay(100);

    /* GPS + GLONASS + SBAS + QZSS */
    SERIAL_GPS.write("@GNS 0xF\r\n");
    Serial.println(SERIAL_GPS.readStringUntil('\n'));
    // delay(100);

    /* Auto backup every 10 minutes */
    SERIAL_GPS.write("@ABPT 10\r\n");
    Serial.println(SERIAL_GPS.readStringUntil('\n'));
    SERIAL_GPS.write("@ABUP 1\r\n");
    Serial.println(SERIAL_GPS.readStringUntil('\n'));
    // delay(100);

    // SERIAL_GPS.write("@GCD\r\n"); /* cold start */
    // SERIAL_GPS.write("@GSW\r\n"); /* warm start */
    // SERIAL_GPS.write("@GSP\r\n"); /* hot start for position */
    // SERIAL_GPS.write("@GSR\r\n"); /* hot start for first fix */

    // delay(250);

}

void warm_start_gps() {
    gpsStarted = true;
    SERIAL_GPS.write("@GSW\r\n"); /* warm start */
    Serial.println(SERIAL_GPS.readStringUntil('\n'));
    // delay(100);
}

void hot_start_gps() {
    gpsStarted = true;
    // Set time from LoRaWAN for faster fix
    char gtim_cmd[32];
    snprintf(gtim_cmd, sizeof(gtim_cmd),
             "@GTIM %04d %02d %02d %02d %02d %02d\r\n", year(), month(), day(),
             hour(), minute(), second());
    SERIAL_GPS.write(gtim_cmd);
    Serial.println(SERIAL_GPS.readStringUntil('\n'));
    // delay(100);

    SERIAL_GPS.write("@GSR\r\n"); /* hot start for first fix */
    Serial.println(SERIAL_GPS.readStringUntil('\n'));
    // delay(100);
}

#if ENABLE_GPS_DEBUG
void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
#endif

void loop_gps() {
    if (gpsStarted) {
        while (SERIAL_GPS.available() > 0)
#if ENABLE_GPS_DEBUG
            if (gps.encode(SERIAL_GPS.read()))
                displayInfo();
#else
            gps.encode(SERIAL_GPS.read());
#endif
    } else {
        if (millis() > 60000) {
            Serial.println(F("GPS not started within 60s, starting now"));
            warm_start_gps();
        }
    }
}
