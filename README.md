# jm_crc-ccitt CRC-CCITT Arduino Library

2023-11-18: v1.0.0 - Initial commit.

### CRC-CCITT

_jm_crc-ccitt_ CRC-CCITT Arduino Library computes the CRC-CCITT values of various data.

The library includes also _constexpr_ definitions to compute CRC values of constants at compile time.

CRC-CCITT is a 16-bit CRC.

_jm_crc-ccitt_ is based and uses the AVR optimized library <util/crc16.h> and its generic implementation for non-AVR architectures, which is also the CRC algorithm used by the Bridge Arduino Library.

To compute online CRC values, you can click the link https://crccalc.com/?method=CRC-16/MCRF4XX.

More explanations about MCFF4XX can be found in document:
- https://ww1.microchip.com/downloads/en/DeviceDoc/40232J.pdf

and the Application Note 752:
- https://ww1.microchip.com/downloads/en/AppNotes/00752a.pdf (take care of particular byte/nibble orders).
