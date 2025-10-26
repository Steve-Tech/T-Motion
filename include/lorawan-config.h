#pragma once

// // LoRaWAN NwkSKey, network session key
// // This should be in big-endian (aka msb).
// // static const PROGMEM u1_t NWKSKEY[16] = { FILLMEIN };
// static const PROGMEM u1_t NWKSKEY[16] = { 0xB3, 0x73, 0x54, 0x24, 0x2F, 0x99, 0x5A, 0x2A, 0x0A, 0xA1, 0x46, 0x43, 0x55, 0x8B, 0x5A, 0x85 };;

// // LoRaWAN AppSKey, application session key
// // This should also be in big-endian (aka msb).
// // static const u1_t PROGMEM APPSKEY[16] = { FILLMEIN };
// static const u1_t PROGMEM APPSKEY[16] = { 0x17, 0x19, 0xC1, 0xE0, 0xF2, 0xD2, 0xE3, 0xC0, 0x7F, 0x9E, 0x27, 0x2D, 0x5E, 0x3F, 0xD5, 0x2C };

// // LoRaWAN end-device address (DevAddr)
// // See http://thethingsnetwork.org/wiki/AddressSpace
// // The library converts the address to network byte order as needed, so this should be in big-endian (aka msb) too.
// // static const u4_t DEVADDR = FILLMEIN ; // <-- Change this address for every node!
// static const u4_t DEVADDR = 0x260D2B12;


// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
static const u1_t PROGMEM APPEUI[8]={ FILLMEIN };

// This should also be in little endian format, see above.
static const u1_t PROGMEM DEVEUI[8]={ FILLMEIN };

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
static const u1_t PROGMEM APPKEY[16] = { FILLMEIN };
