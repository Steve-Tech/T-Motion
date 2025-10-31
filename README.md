# Steve's T-Motion Tracker

This is the code for a LoRaWAN-based GPS tracker using the T-Motion board from LilyGO. It uses OTAA to join The Things Network (TTN) and sends its GPS location every 60 seconds. A BME280 sensor is also supported for sending temperature, pressure, and humidity data.

This project is optimised for fast startup, and uses LoRaWAN's network time syncronisation to hot start the GPS module with accurate time. GPS data (e.g. last position, almanac) is also backed up to the GPS's flash every 10 minutes to further speed up GPS fixes. Most GPS boards have a small battery to retain this data when powered off, however the T-Motion does not, so this software-based solution helps to improve time to first fix (TTFF) significantly.

## Hardware

- T-Motion board with GPS and LoRa (e.g. [LilyGO T-Motion](https://lilygo.cc/products/t-motion-s76g-stm32-lora))
- Optional: BME280 (or BMP280) environmental sensor for temperature, pressure, and (if supported) humidity data.

    The BME/BMP280 sensor should be connected to the PB6 (SCL) and PB7 (SDA) pins on the T-Motion board. The 4 pin BME/BMP280 boards usually line up perfectly with the 3V3, GND, PB6, and PB7 pins on the T-Motion.

## Setup

1. [Set your region](https://github.com/mcci-catena/arduino-lmic#selecting-the-lorawan-region-configuration) in `platformio.ini`, .e.g. `-D CFG_au915` for Australia.
2. Update your LoRaWAN credentials in `include/lorawan-config.h`:
   - `APPEUI`: Application/Join EUI in little-endian format.
   - `DEVEUI`: Device EUI in little-endian format.
   - `APPKEY`: Application Key in big-endian format.

   If you are using TTN and don't have an JoinEUI, you can use a value of all zeros. You can generate the DevEUI and AppKey after entering a JoinEUI in the TTN console. **This project requires LoRaWAN 1.0.3**, for time sync support.
3. Upload the code to your T-Motion board using PlatformIO. Hold the BOOT button while plugging it in to enter programming  mode. Then upload the code from PlatformIO.
4. Add `decoder.js` to the Uplink Payload formatter in the TTN console. This project uses a customised Cayenne LPP format, and is not compatible with TTN's built-in Cayenne LPP decoder.
5. Done! Your device should join the network and start sending GPS data.

    The payloads will look something like this when decoded:

    ```json
    {
        "altitude": 28.8,
        "barometer_0": 996.3,
        "hdop": 1.2,
        "heading": 293,
        "humidity_0": 72.5,
        "latitude": -27.4689,
        "longitude": 153.0234,
        "sats": 9,
        "speed": 0,
        "temperature_0": 31.3,
        "time": 1761445517
      }
    ```

## Notes

- You many need to apply `0001-Add-SX1276-version-0x13.patch` to the LMIC library or use my PR https://github.com/mcci-catena/arduino-lmic/pull/1017 if your radio is not detected correctly.
- The code is configured to use FSB 2 for the US/AU region to speed up the join process with TTN. If you are using a different region or network, you may need to adjust this in `src/lorawan.cpp`.
- GPS debugging can be enabled by setting `ENABLE_GPS_DEBUG` to `1` in `src/gps.cpp`. This will print GPS information to the serial console.
