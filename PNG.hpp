#ifndef PNG_HPP
#define PNG_HPP

#include <vector>
#include <iostream>
#include <stdint.h>
#include <string>
#include <cstring>

#define TYPE_SIZE 4
#define CRC_SIZE 4
#define LENGTH_SIZE 8
const uint8_t png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};

struct Chunk
{
	size_t length = 0;
	uint8_t *data;
	uint8_t type[4], crc[4];
};

struct Pixel
{
	uint8_t r, g, b, a;
};

std::string hex(int, size_t = 0);
std::string bit(uint8_t);

class PNG
{
	public:
		bool load(uint8_t*, size_t);
		bool check();
		std::vector<Pixel> getData();
		
		size_t getWidth();
		size_t getHeight();
		uint8_t getBitDepth();
		uint8_t getColorType();
		uint8_t getCompressionMethod();
		uint8_t getFilterMethod();
		uint8_t getInterlaceMethod();
		std::vector<Chunk> chunks;
	private:
		size_t width, height;
		uint8_t bit_depth, color_type, compression_method, filter_method, interlace_method;
};

#endif