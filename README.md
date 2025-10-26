# Steve's T-Motion Tracker

This is the code for a LoRaWAN-based GPS tracker using the T-Motion board from LilyGO. It uses OTAA to join The Things Network (TTN) and periodically sends GPS location data. This project is optimised for fast startup, and uses LoRaWAN's network time syncronisation to hot start the GPS module with accurate time; GPS data (e.g. last position, almanac) is also backed up every 10 minutes to further speed up GPS fixes.

## Setup

1. Set your region in `platformio.ini`, .e.g. `-D CFG_au915` for Australia.
2. Update your LoRaWAN credentials in `include/lorawan-config.h`:
   - `APPEUI`: Application/Join EUI in little-endian format.
   - `DEVEUI`: Device EUI in little-endian format.
   - `APPKEY`: Application Key in big-endian format.

   If you are using TTN and don't have an JoinEUI, you can use a value of all zeros. You can generate the DevEUI and AppKey after entering a JoinEUI in the TTN console. This project requires LoRaWAN 1.0.3, for time sync support.
3. Upload the code to your T-Motion board using PlatformIO. Hold the BOOT button while plugging it in to enter programming  mode. Then upload the code from PlatformIO.
4. Add `decoder.js` to the Uplink Payload formatter in the TTN console. This project uses a customised Cayenne LPP format, and is not compatible with TTN's built-in Cayenne LPP decoder.
5. Done! Your device should join the network and start sending GPS data.

    The payloads will look something like this when decoded:

    ```json
    {
        "altitude": 28.8,
        "hdop": 1.2,
        "heading": 293,
        "latitude": -27.4689,
        "longitude": 153.0234,
        "sats": 9,
        "speed": 0,
        "time": 1761445517
      }
    ```

## Notes

- You many need to apply `0001-Add-SX1276-version-0x13.patch` to the LMIC library or use my PR https://github.com/mcci-catena/arduino-lmic/pull/1017 if your radio is not detected correctly.
- The code is configured to use sub-band 1 (channels 8-15) for the US/AU region to speed up the join process with TTN. If you are using a different region or network, you may need to adjust this in `src/lorawan.cpp`.
- GPS debugging can be enabled by setting `ENABLE_GPS_DEBUG` to `1` in `src/gps.cpp`. This will print GPS information to the serial console.
