#ifndef HOSTORDER32_H
#define HOSTORDER32_H

#include <limits.h>
#include <stdint.h>

#if CHAR_BIT != 8
#error CHAR_BIT
#error "unsupported char size"
#endif


static const union { unsigned char bytes[4]; uint32_t value; } o32_host_order =
    { { 0, 1, 2, 3 } };

#define O32_HOST_ORDER (o32_host_order.value)

enum O32_ENDIANESS
{
    O32_LITTLE_ENDIAN = 0x03020100ul,
    O32_BIG_ENDIAN = 0x00010203ul,
    O32_PDP_ENDIAN = 0x01000302ul,      /* DEC PDP-11 (aka ENDIAN_LITTLE_WORD) */
    O32_HONEYWELL_ENDIAN = 0x02030001ul /* Honeywell 316 (aka ENDIAN_BIG_WORD) */
};

std::string O32_ENDIANESS_TO_STRING(uint32_t s){
	switch(s){
		case O32_LITTLE_ENDIAN:
			return "O32_LITTLE_ENDIAN";
		break;
		case O32_BIG_ENDIAN:
			return "O32_BIG_ENDIAN";
			break;
		case O32_PDP_ENDIAN:
			return "O32_PDP_ENDIAN";
			break;
		case O32_HONEYWELL_ENDIAN:
			return "O32_HONEYWELL_ENDIAN";
			break;
		default:
			std::runtime_error("Could not determine Endianess");
			break;
	}
}

O32_ENDIANESS STRING_TO_032_ENDIANESS(std::string s)
{
	if(s == "O32_LITTLE_ENDIAN")
		return O32_LITTLE_ENDIAN;
	if(s == "O32_BIG_ENDIAN")
		return O32_BIG_ENDIAN;
	if(s == "O32_PDP_ENDIAN")
		return O32_PDP_ENDIAN;
	if(s == "O32_HONEYWELL_ENDIAN")
		return O32_HONEYWELL_ENDIAN;

	std::runtime_error("Could not convert Endianess");
}

#endif
