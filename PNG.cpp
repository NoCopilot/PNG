#include "PNG.hpp"

std::string hex(int n, size_t s)
{
	std::string res = "";
	while(n > 0)
	{
		int c_res = n%16;
		if(c_res > 9) c_res = 65 + c_res - 10;
		else c_res = 48 + c_res;
		res = (char)(c_res) + res;
		n /= 16;
	}
	while(res.size() < s)
	{
		res = "0" + res;
	}
	return res;
}

std::string bit(uint8_t n)
{
	std::string res = "";
	while(n > 0)
	{
		res = (char)(n%2 + 48) + res;
		n /= 2;
	}
	while(res.size() < 8)
	{
		res = "0" + res;
	}
	return res;
}

bool PNG::load(uint8_t* buffer, size_t size)
{
	chunks.clear();
	
	for(int i = 0; i < 8; i++)
	{
		if(*buffer != png_signature[i])
		{
			std::cout << "signature not valid\n";
			return false;
		}
		buffer++;
	}
	size_t k = 8;
	std::cout << "---------------------------------------------------------------------\n";
	while(k < size)
	{
		Chunk chunk;
		std::cout << "chunk size: ";
		for(int i = 3; i >= 0; i--)
		{
			chunk.length += (*buffer) << (i*8);
			buffer++;
		}
		std::cout << chunk.length << "\n";		
		
		std::cout << "chunk type: ";
		for(int i = 0; i < 4; i++)
		{
			chunk.type[i] = (uint8_t)*buffer;
			std::cout << chunk.type[i];
			buffer++;
		}
		std::cout << "\n";
		
		std::cout << "chunk data: ";
		chunk.data = new uint8_t[chunk.length];
		for(size_t i = 0; i < chunk.length; i++)
		{
			chunk.data[i] = *buffer;
			std::cout << hex(chunk.data[i], 2) << " ";
			buffer++;
		}
		std::cout << "\n";
		
		std::cout << "CRC: ";
		for(int i = 0; i < 4; i++)
		{
			chunk.crc[i] = *buffer;
			std::cout << hex(chunk.crc[i]);
			buffer++;
		}
		std::cout << "\n";
		
		k += 12 + chunk.length;
		
		chunks.push_back(chunk);
		if(chunk.type == (uint8_t*)"IEND") break;
		
		std::cout << "---------------------------------------------------------------------\n";
	}
	
	if(std::memcmp(chunks[0].type, "IHDR", 4) != 0)
	{
		std::cout << "first chunk must be IHDR type\n";
		return false;
	}
	
	if(std::memcmp(chunks[chunks.size()-1].type, "IEND", 4) != 0)
	{
		std::cout << "last chunk must be IEND type\n";
		return false;
	}
	
	//getting IHDR chunk data
	width = 0;
	for(int i = 3; i >= 0; i--)
		width += (chunks[0].data[3 - i]) << (i*8);
	
	height = 0;
	for(int i = 3; i >= 0; i--)
		height += (chunks[0].data[4 + 3 - i]) << (i*8);
	
	bit_depth = chunks[0].data[8];
	color_type = chunks[0].data[9];
	compression_method = chunks[0].data[10];
	filter_method = chunks[0].data[11];
	interlace_method = chunks[0].data[12];
	
	
	
	return true;
}

std::vector<Pixel> PNG::getData()
{
	
	return {};
}

bool PNG::check()
{
	
	return true;
}

size_t PNG::getWidth()
{
	return width;
}

size_t PNG::getHeight()
{
	return height;
}

uint8_t PNG::getBitDepth()
{
	return bit_depth;
}

uint8_t PNG::getColorType()
{
	return color_type;
}

uint8_t PNG::getCompressionMethod()
{
	return compression_method;
}

uint8_t PNG::getFilterMethod()
{
	return filter_method;
}

uint8_t PNG::getInterlaceMethod()
{
	return interlace_method;
}
