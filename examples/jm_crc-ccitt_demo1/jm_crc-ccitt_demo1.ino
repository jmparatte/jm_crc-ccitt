#include "jm_crc-ccitt.h"

void sizeof_print( const char * msg, size_t len ) {
	Serial.print(msg);
	Serial.print(F("="));
	Serial.print(len);
	Serial.println();
}

void crc_challenge( const char * crc_input, uint16_t crc_check, uint16_t crc_result ) {
	Serial.print(crc_input);
	Serial.print(F(", check="));
	Serial.print(crc_check, HEX);
	Serial.print(F(", result="));
	Serial.print(crc_result, HEX);
	Serial.print(F(" -> "));
	if (crc_check==crc_result)
		Serial.print(F("OK"));
	else
		Serial.print(F("ER"));
	Serial.println();
}

void setup() {

	Serial.begin(115200); // Serial baudrate=115200, databits=8, parity=None, stopbits=1
	for (int i=3000; Serial.println()==0 && i>0; i--) delay(1); // wait max 3s for USB Serial ready

	Serial.println(F(__FILE__)); // display program signature

	Serial.println();

	Serial.println(F("size of common types:"));
	Serial.println(F("---------------------"));
	sizeof_print("__CHAR_BIT__", __CHAR_BIT__);
	sizeof_print("sizeof(char)", sizeof(char));
	sizeof_print("sizeof(int)", sizeof(int));
	sizeof_print("sizeof(short)", sizeof(short));
	sizeof_print("sizeof(long)", sizeof(long));
	sizeof_print("sizeof(long long)", sizeof(long long));
	sizeof_print("sizeof(byte)", sizeof(byte));
	sizeof_print("sizeof(word)", sizeof(word));
	sizeof_print("sizeof(float)", sizeof(float));
	sizeof_print("sizeof(double)", sizeof(double));

	Serial.println();

	Serial.println(F("(char *) challenge:"));
	Serial.println(F("-------------------"));
	crc_challenge("crc_ccitt(\"\")", 0xFFFF, crc_ccitt(""));
	crc_challenge("CRC_CCITT(\"\")", 0xFFFF, CRC_CCITT(""));
	crc_challenge("crc_ccitt(\"1\")", 0x2F8D, crc_ccitt("1"));
	crc_challenge("CRC_CCITT(\"1\")", 0x2F8D, CRC_CCITT("1"));
	crc_challenge("crc_ccitt(\"123456789\")", 0x6F91, crc_ccitt("123456789"));
	crc_challenge("CRC_CCITT(\"123456789\")", 0x6F91, CRC_CCITT("123456789"));

	Serial.println();

	Serial.println(F("(char) challenge:"));
	Serial.println(F("-----------------"));
	crc_challenge("crc_ccitt('1')", 0x2F8D, crc_ccitt('1'));
	crc_challenge("CRC_CCITT('1')", 0x2F8D, CRC_CCITT('1'));

// https://ww1.microchip.com/downloads/en/DeviceDoc/40232J.pdf
// https://ww1.microchip.com/downloads/en/AppNotes/00752a.pdf
// AN752: TABLE 1: CRC WORKOUT EXAMPLE FOR DATA = 8552F189 (HEX)

// running in CRC Online:
// Step Bit No. 8: (uint16_t)0xD14A, check of (uint8_t)0x85 (0x58)
// Step Bit No.16: (uint16_t)0x9B20, check of (uint16_t)0x8552 (0x5825)
// Step Bit No.32: (uint16_t)0x07F1, check of (uint32_t)0x8552F189 (0x58251F98)

// crc_ccitt():
// Step Bit No. 8: (uint16_t)0xD14A, check of (uint8_t)0x85 (0x58)
// Step Bit No.16: (uint16_t)0x9B20, check of (uint16_t)0x8552 (0x2558)
// Step Bit No.32: (uint16_t)0x07F1, check of (uint32_t)0x8552F189 (0x981F2558)

	uint32_t u32 = (uint32_t)0x981F2558; // bytes and nibbles reversed of 0x8552F189

	Serial.println();

	Serial.println(F("(uint8_t) challenge:"));
	Serial.println(F("--------------------"));
	crc_challenge("crc_ccitt((uint8_t)0x58)", 0xD14A, crc_ccitt((uint8_t)0x58));
	crc_challenge("CRC_CCITT((uint8_t)0x58)", 0xD14A, CRC_CCITT((uint8_t)0x58));
	crc_challenge("crc_ccitt((uint8_t)u32)", 0xD14A, crc_ccitt((uint8_t)u32));
	crc_challenge("crc_ccitt((uint8_t*)&u32, 1)", 0xD14A, crc_ccitt((uint8_t*)&u32, 1));

	Serial.println();

	Serial.println(F("(uint16_t) challenge:"));
	Serial.println(F("---------------------"));
	crc_challenge("crc_ccitt((uint16_t)0x2558)", 0x9B20, crc_ccitt((uint16_t)0x2558));
	crc_challenge("CRC_CCITT((uint16_t)0x2558)", 0x9B20, CRC_CCITT((uint16_t)0x2558));
	crc_challenge("crc_ccitt((uint16_t)u32)", 0x9B20, crc_ccitt((uint16_t)u32));
	crc_challenge("crc_ccitt((uint8_t*)&u32, 2)", 0x9B20, crc_ccitt((uint8_t*)&u32, 2));

	Serial.println();

	Serial.println(F("(uint32_t) challenge:"));
	Serial.println(F("---------------------"));
	crc_challenge("crc_ccitt((uint32_t)0x981F2558)", 0x07F1, crc_ccitt((uint32_t)0x981F2558));
	crc_challenge("CRC_CCITT((uint32_t)0x981F2558)", 0x07F1, CRC_CCITT((uint32_t)0x981F2558));
	crc_challenge("crc_ccitt((uint32_t)u32)", 0x07F1, crc_ccitt((uint32_t)u32));
	crc_challenge("crc_ccitt((uint8_t*)&u32, 4)", 0x07F1, crc_ccitt((uint8_t*)&u32, 4));

	Serial.println();

#if __SIZEOF_FLOAT__ == 4
	Serial.println(F("(float) challenge:"));
	Serial.println(F("------------------"));
	crc_challenge("crc_ccitt((float)0.0)", 0x0321, crc_ccitt((float)0.0));
	crc_challenge("CRC_CCITT((float)0.0)", 0x0321, CRC_CCITT((float)0.0));
	crc_challenge("crc_ccitt((float)NAN)", 0x42FB, crc_ccitt((float)NAN));
	crc_challenge("CRC_CCITT((float)NAN)", 0x42FB, CRC_CCITT((float)NAN));
	crc_challenge("crc_ccitt((float)INFINITY)", 0x049D, crc_ccitt((float)INFINITY));
	crc_challenge("CRC_CCITT((float)INFINITY)", 0x049D, CRC_CCITT((float)INFINITY));
#else
	Serial.println(F("This processor does not have a regular FLOAT type."));
#endif

	Serial.println();

#if __SIZEOF_DOUBLE__ == 8
	Serial.println(F("(double) challenge:"));
	Serial.println(F("-------------------"));
	crc_challenge("crc_ccitt((double)0.0)", 0x7C8C, crc_ccitt((double)0.0));
	crc_challenge("CRC_CCITT((double)0.0)", 0x7C8C, CRC_CCITT((double)0.0));
	crc_challenge("crc_ccitt((double)NAN)", 0x4534, crc_ccitt((double)NAN));
	crc_challenge("CRC_CCITT((double)NAN)", 0x4534, CRC_CCITT((double)NAN));
	crc_challenge("crc_ccitt((double)INFINITY)", 0x8BF4, crc_ccitt((double)INFINITY));
	crc_challenge("CRC_CCITT((double)INFINITY)", 0x8BF4, CRC_CCITT((double)INFINITY));
#else
	Serial.println(F("This processor does not have a regular DOUBLE type."));
#endif
}

void loop() {
}
