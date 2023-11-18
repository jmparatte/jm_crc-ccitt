
#ifndef jm_crc_ccitt_h
#define jm_crc_ccitt_h

#include <stddef.h> // size_t
#include <stdint.h> // int8_t, uint8_t,...
#include <inttypes.h> // scanf and printf formats
#include <WString.h> // Arduino String

//------------------------------------------------------------------------------

#if __CHAR_BIT__ != 8
#error This library does not support any number of bits in char other than 8.
#endif

//------------------------------------------------------------------------------

// https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html

/*
__BYTE_ORDER__
__ORDER_LITTLE_ENDIAN__
__ORDER_BIG_ENDIAN__
__ORDER_PDP_ENDIAN__

    __BYTE_ORDER__ is defined to one of the values __ORDER_LITTLE_ENDIAN__, __ORDER_BIG_ENDIAN__, or __ORDER_PDP_ENDIAN__ to reflect the layout of multi-byte and multi-word quantities in memory. If __BYTE_ORDER__ is equal to __ORDER_LITTLE_ENDIAN__ or __ORDER_BIG_ENDIAN__, then multi-byte and multi-word quantities are laid out identically: the byte (word) at the lowest address is the least significant or most significant byte (word) of the quantity, respectively. If __BYTE_ORDER__ is equal to __ORDER_PDP_ENDIAN__, then bytes in 16-bit words are laid out in a little-endian fashion, whereas the 16-bit subwords of a 32-bit quantity are laid out in big-endian fashion.

    You should use these macros for testing like this:

    // Test for a little-endian machine
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

__FLOAT_WORD_ORDER__

    __FLOAT_WORD_ORDER__ is defined to one of the values __ORDER_LITTLE_ENDIAN__ or __ORDER_BIG_ENDIAN__ to reflect the layout of the words of multi-word floating-point quantities.
*/

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error This library does not support byte orders other than __ORDER_LITTLE_ENDIAN__.
#endif

#if __FLOAT_WORD_ORDER__ != __BYTE_ORDER__
#error This library does not support __FLOAT_WORD_ORDER__ different from __BYTE_ORDER__.
#endif

//------------------------------------------------------------------------------

#if __SIZEOF_FLOAT__ == 4
// include CRC calculation of FLOAT only if sizeof FLOAT is exactly 4 bytes.
#else
// it's not a regular sizeof FLOAT and CRC will be wrong compared with other CPU models!
#endif

#if __SIZEOF_DOUBLE__ == 8
// include CRC calculation of DOUBLE only if sizeof DOUBLE is exactly 8 bytes.
#else
// it's not a regular sizeof DOUBLE and CRC will be wrong compared with other CPU models!
#endif

//------------------------------------------------------------------------------

// https://forums.ni.com/t5/Example-Code/16-bit-CRC-CCITT/ta-p/3492270
// https://ww1.microchip.com/downloads/en/DeviceDoc/40232J.pdf
// https://ww1.microchip.com/downloads/en/AppNotes/00752a.pdf
// https://infineon.github.io/mtb-pdl-cat2/pdl_api_reference_manual/html/group__group__crypto__lld__crc__functions.html
// https://docs.rs/crc/latest/crc/struct.Algorithm.html
// https://crccalc.com/

// https://crccalc.com/?crc=123456789&method=CRC-16/MCRF4XX&datatype=ascii&outtype=0
//
// Processed data
// ASCII	123456789
// HEX		0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39
//
// Algorithm 		Result 	Check 	Poly 	Init 	RefIn 	RefOut 	XorOut
// CRC-16/MCRF4XX	0x6F91 	0x6F91 	0x1021 	0xFFFF 	true 	true 	0x0000

/*
Algorithm 			Result 	Check 	Poly 	Init 	RefIn 	RefOut 	XorOut
CRC-16/ARC			0xBB3D 	0xBB3D 	0x8005 	0x0000 	true 	true 	0x0000
CRC-16/AUG-CCITT	0xE5CC 	0xE5CC 	0x1021 	0x1D0F 	false 	false 	0x0000
CRC-16/BUYPASS		0xFEE8 	0xFEE8 	0x8005 	0x0000 	false 	false 	0x0000
CRC-16/CCITT-FALSE	0x29B1 	0x29B1 	0x1021 	0xFFFF 	false 	false 	0x0000
CRC-16/CDMA2000		0x4C06 	0x4C06 	0xC867 	0xFFFF 	false 	false 	0x0000
CRC-16/DDS-110		0x9ECF 	0x9ECF 	0x8005 	0x800D 	false 	false 	0x0000
CRC-16/DECT-R		0x007E 	0x007E 	0x0589 	0x0000 	false 	false 	0x0001
CRC-16/DECT-X		0x007F 	0x007F 	0x0589 	0x0000 	false 	false 	0x0000
CRC-16/DNP			0xEA82 	0xEA82 	0x3D65 	0x0000 	true 	true 	0xFFFF
CRC-16/EN-13757		0xC2B7 	0xC2B7 	0x3D65 	0x0000 	false 	false 	0xFFFF
CRC-16/GENIBUS		0xD64E 	0xD64E 	0x1021 	0xFFFF 	false 	false 	0xFFFF
CRC-16/KERMIT		0x2189 	0x2189 	0x1021 	0x0000 	true 	true 	0x0000
CRC-16/MAXIM		0x44C2 	0x44C2 	0x8005 	0x0000 	true 	true 	0xFFFF
CRC-16/MCRF4XX		0x6F91 	0x6F91 	0x1021 	0xFFFF 	true 	true 	0x0000
CRC-16/MODBUS		0x4B37 	0x4B37 	0x8005 	0xFFFF 	true 	true 	0x0000
CRC-16/RIELLO		0x63D0 	0x63D0 	0x1021 	0xB2AA 	true 	true 	0x0000
CRC-16/T10-DIF		0xD0DB 	0xD0DB 	0x8BB7 	0x0000 	false 	false 	0x0000
CRC-16/TELEDISK		0x0FB3 	0x0FB3 	0xA097 	0x0000 	false 	false 	0x0000
CRC-16/TMS37157		0x26B1 	0x26B1 	0x1021 	0x89EC 	true 	true 	0x0000
CRC-16/USB			0xB4C8 	0xB4C8 	0x8005 	0xFFFF 	true 	true 	0xFFFF
CRC-16/X-25			0x906E 	0x906E 	0x1021 	0xFFFF 	true 	true 	0xFFFF
CRC-16/XMODEM		0x31C3 	0x31C3 	0x1021 	0x0000 	false 	false 	0x0000
CRC-A				0xBF05 	0xBF05 	0x1021 	0xC6C6 	true 	true 	0x0000
*/

//------------------------------------------------------------------------------

// https://crccalc.com/?crc=123456789&method=CRC-16/MCRF4XX&datatype=ascii&outtype=0
// =====================================
// Online CRC-8 CRC-16 CRC-32 Calculator
// =====================================
// to run in CRC Online:
// select CRC-16/MCRF4XX
// !!! bytes are reversed
// nibbles are not swapped
// least significant bit first

// https://ww1.microchip.com/downloads/en/AppNotes/00752a.pdf
// ================================
// Microchip Application Note AN752
// CRC Algorithm for MCRF45X Read/Write Device
// ================================
// to run in CRC Online:
// select CRC-16/MCRF4XX
// !!! bytes are reversed
// !!! nibbles are swapped
// least significant bit first

// ===========
// crc_ccitt()
// ===========
// !!! bytes are reversed
// !!! nibbles are swapped
// least significant bit first

#define CRC_CCITT_INIT (uint16_t)0xFFFF
#define CRC_CCITT_POLY (uint16_t)0x1021 // CRC Polynomial:
// X^0+X^5+X^12+X^16 = 1000-0100-0000-1000-(1) = 0x8408 => reversed = 0x1021
// This polynomial is also known as CRC CCITT-16.
#define CRC_CCITT_CHECK (uint16_t)0x6F91 // CRC of ascii string "123456789"

// AN752: TABLE 1: CRC WORKOUT EXAMPLE FOR DATA = 8552F189 (HEX)
// running in CRC Online:
// Step Bit No. 8: (uint16_t)0xD14A, check of (uint8_t)0x85 (0x58)
// Step Bit No.16: (uint16_t)0x9B20, check of (uint16_t)0x8552 (0x5825)
// Step Bit No.32: (uint16_t)0x07F1, check of (uint32_t)0x8552F189 (0x58251F98)

// crc_ccitt():
// Step Bit No. 8: (uint16_t)0xD14A, check of (uint8_t)0x85 (0x58)
// Step Bit No.16: (uint16_t)0x9B20, check of (uint16_t)0x8552 (0x2558)
// Step Bit No.32: (uint16_t)0x07F1, check of (uint32_t)0x8552F189 (0x981F2558)

//------------------------------------------------------------------------------

uint16_t _crc_ccitt_update(uint16_t crc, uint8_t data);
// AVR use an optimized implementation of CRC, watch "util/crc16.h" for details.
// jm_crc-ccitt use this optimized implementation for AVR
// and a generic implementation for non-AVR architectures.

//------------------------------------------------------------------------------

uint16_t crc_ccitt_init(uint16_t crc=CRC_CCITT_INIT);

uint16_t crc_ccitt(const uint8_t data, uint16_t crc=CRC_CCITT_INIT);
uint16_t crc_ccitt(const int8_t data, uint16_t crc=CRC_CCITT_INIT);
uint16_t crc_ccitt(const char data, uint16_t crc=CRC_CCITT_INIT);

uint16_t crc_ccitt(const uint8_t buf[], size_t len, uint16_t crc=CRC_CCITT_INIT);
uint16_t crc_ccitt(const char * str, uint16_t crc=CRC_CCITT_INIT);
uint16_t crc_ccitt(const String & str, uint16_t crc=CRC_CCITT_INIT);

uint16_t crc_ccitt(const uint16_t data, uint16_t crc=CRC_CCITT_INIT);
uint16_t crc_ccitt(const int16_t data, uint16_t crc=CRC_CCITT_INIT);

uint16_t crc_ccitt(const uint32_t data, uint16_t crc=CRC_CCITT_INIT);
uint16_t crc_ccitt(const int32_t data, uint16_t crc=CRC_CCITT_INIT);

uint16_t crc_ccitt(const uint64_t data, uint16_t crc=CRC_CCITT_INIT);
uint16_t crc_ccitt(const int64_t data, uint16_t crc=CRC_CCITT_INIT);

#if __SIZEOF_FLOAT__ == 4
uint16_t crc_ccitt(const float data, uint16_t crc=CRC_CCITT_INIT);
#endif
#if __SIZEOF_DOUBLE__ == 8
uint16_t crc_ccitt(const double data, uint16_t crc=CRC_CCITT_INIT);
#endif

//------------------------------------------------------------------------------

constexpr uint8_t _CRC_CCITT_UPDATE2(uint16_t crc, uint8_t data)
{
	return (data ^ (crc & 0xff));
}

constexpr uint8_t _CRC_CCITT_UPDATE1(uint16_t crc, uint8_t data)
{
	return (_CRC_CCITT_UPDATE2(crc, data) ^ (_CRC_CCITT_UPDATE2(crc, data) << 4));
}

constexpr uint16_t _CRC_CCITT_UPDATE(uint16_t crc, uint8_t data)
{
	return (
		(((uint16_t)_CRC_CCITT_UPDATE1(crc, data) << 8) | ((crc >> 8) & 0xff)) ^
		(uint8_t)(_CRC_CCITT_UPDATE1(crc, data) >> 4) ^
		((uint16_t)_CRC_CCITT_UPDATE1(crc, data) << 3)
	);
}

//------------------------------------------------------------------------------

constexpr uint16_t CRC_CCITT(const uint8_t data, uint16_t crc=CRC_CCITT_INIT)
{
	return _CRC_CCITT_UPDATE(crc, data);
}

constexpr uint16_t CRC_CCITT(const int8_t data, uint16_t crc=CRC_CCITT_INIT)
{
	return CRC_CCITT((uint8_t) data, crc);
}

constexpr uint16_t CRC_CCITT(const char data, uint16_t crc=CRC_CCITT_INIT)
{
	return CRC_CCITT((uint8_t) data, crc);
}

//------------------------------------------------------------------------------

constexpr uint16_t CRC_CCITT(const uint8_t buf[], size_t len, uint16_t crc=CRC_CCITT_INIT)
{
	return (len ? CRC_CCITT((buf + 1), (len - 1), _CRC_CCITT_UPDATE(crc, *buf)) : crc);
}

//------------------------------------------------------------------------------

constexpr int _STRLEN(const char * str, size_t idx=0)
{
	return (str && *str ? _STRLEN((str + 1), (idx + 1)) : idx);
}

constexpr size_t STRLEN(const char * str)
{
	return (_STRLEN(str));
}

constexpr uint16_t CRC_CCITT(const char * str, uint16_t crc=CRC_CCITT_INIT)
{
	return (str && *str ? CRC_CCITT((str + 1), _CRC_CCITT_UPDATE(crc, str[0])) : crc);
}

//------------------------------------------------------------------------------

constexpr uint16_t CRC_CCITT(const uint16_t data, uint16_t crc=CRC_CCITT_INIT)
{
	return CRC_CCITT((uint8_t *) &data, sizeof(data), crc);
}

constexpr uint16_t CRC_CCITT(const int16_t data, uint16_t crc=CRC_CCITT_INIT)
{
	return CRC_CCITT((uint16_t) data, crc);
}

//------------------------------------------------------------------------------

constexpr uint16_t CRC_CCITT(const uint32_t data, uint16_t crc=CRC_CCITT_INIT)
{
	return CRC_CCITT((uint8_t *) &data, sizeof(data), crc);
}

constexpr uint16_t CRC_CCITT(const int32_t data, uint16_t crc=CRC_CCITT_INIT)
{
	return CRC_CCITT((uint32_t) data, crc);
}

//------------------------------------------------------------------------------

constexpr uint16_t CRC_CCITT(const uint64_t data, uint16_t crc=CRC_CCITT_INIT)
{
	return CRC_CCITT((uint8_t *) &data, sizeof(data), crc);
}

constexpr uint16_t CRC_CCITT(const int64_t data, uint16_t crc=CRC_CCITT_INIT)
{
	return CRC_CCITT((uint64_t) data, crc);
}

//------------------------------------------------------------------------------

#if __SIZEOF_FLOAT__ == 4
constexpr uint16_t CRC_CCITT(const float data, uint16_t crc=CRC_CCITT_INIT)
{
	return CRC_CCITT((uint8_t *) &data, sizeof(data), crc);
}
#endif

#if __SIZEOF_DOUBLE__ == 8
constexpr uint16_t CRC_CCITT(const double data, uint16_t crc=CRC_CCITT_INIT)
{
	return CRC_CCITT((uint8_t *) &data, sizeof(data), crc);
}
#endif

//------------------------------------------------------------------------------

#endif // jm_crc_ccitt_h
