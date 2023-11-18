
#include <jm_crc-ccitt.h>

//------------------------------------------------------------------------------

#if defined(ARDUINO_ARCH_AVR)
// AVR use an optimized implementation of CRC
#include <util/crc16.h>
#else
// Generic implementation for non-AVR architectures
uint16_t _crc_ccitt_update(uint16_t crc, uint8_t data)
{
	data ^= crc & 0xff;
	data ^= data << 4;
	return (
		(((uint16_t)data << 8) | ((crc >> 8) & 0xff)) ^
		(uint8_t)(data >> 4) ^
		((uint16_t)data << 3)
	);
}
#endif

//------------------------------------------------------------------------------

uint16_t crc_ccitt_init(uint16_t crc)
{
	return crc;
}

//------------------------------------------------------------------------------

uint16_t crc_ccitt(const uint8_t data, uint16_t crc)
{
	return _crc_ccitt_update(crc, data);
}

uint16_t crc_ccitt(const int8_t data, uint16_t crc)
{
	return crc_ccitt((uint8_t) data, crc);
}

uint16_t crc_ccitt(const char data, uint16_t crc)
{
	return crc_ccitt((uint8_t) data, crc);
}

//------------------------------------------------------------------------------

uint16_t crc_ccitt(const uint8_t buf[], size_t len, uint16_t crc)
{
	while (len--) crc = _crc_ccitt_update(crc, *buf++);
	return crc;
}

uint16_t crc_ccitt(const char * str, uint16_t crc)
{
	return (str ? crc_ccitt((uint8_t *) str, strlen(str), crc) : crc);
}

uint16_t crc_ccitt(const String & str, uint16_t crc)
{
	return crc_ccitt((uint8_t *) str.c_str(), str.length(), crc);
}

//------------------------------------------------------------------------------

uint16_t crc_ccitt(const uint16_t data, uint16_t crc)
{
	return crc_ccitt((uint8_t *) &data, sizeof(data), crc);
}

uint16_t crc_ccitt(const int16_t data, uint16_t crc)
{
	return crc_ccitt((uint16_t) data, crc);
}

//------------------------------------------------------------------------------

uint16_t crc_ccitt(const uint32_t data, uint16_t crc)
{
	return crc_ccitt((uint8_t *) &data, sizeof(data), crc);
}

uint16_t crc_ccitt(const int32_t data, uint16_t crc)
{
	return crc_ccitt((uint32_t) data, crc);
}

//------------------------------------------------------------------------------

uint16_t crc_ccitt(const uint64_t data, uint16_t crc)
{
	return crc_ccitt((uint8_t *) &data, sizeof(data), crc);
}

uint16_t crc_ccitt(const int64_t data, uint16_t crc)
{
	return crc_ccitt((uint64_t) data, crc);
}

//------------------------------------------------------------------------------

#if __SIZEOF_FLOAT__ == 4
uint16_t crc_ccitt(const float data, uint16_t crc)
{
	return crc_ccitt((uint8_t *) &data, sizeof(data), crc);
}
#endif

#if __SIZEOF_DOUBLE__ == 8
uint16_t crc_ccitt(const double data, uint16_t crc)
{
	return crc_ccitt((uint8_t *) &data, sizeof(data), crc);
}
#endif

//------------------------------------------------------------------------------

// END.
