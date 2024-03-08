#include "PNG.hpp"
#include "zlib.h"
#include <fstream>
#include <sstream>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout << "no input file provided\n";
		return 1;
	}
	
	//reading the file
	std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	
	uint8_t* buffer = new uint8_t[size];
	for(std::streamsize i = 0; i < size; i++) buffer[i] = file.get();
	file.close();
	
	//reading png
	PNG png;
	if(!png.load(buffer, size))
	{
		std::cout << "failed to load png\n";
		return 1;
	}
	
	std::cout << "\nInformations gotten:\n";
	std::cout << "width: " << png.getWidth() << "\n";
	std::cout << "height: " << png.getHeight() << "\n";
	std::cout << "bit depth: " << (int)png.getBitDepth() << "\n";
	std::cout << "color type: " << (int)png.getColorType() << "\n";
	std::cout << "compression method: " << (int)png.getCompressionMethod() << "\n";
	std::cout << "filter method: " << (int)png.getFilterMethod() << "\n";
	std::cout << "interlace method: " << (int)png.getInterlaceMethod() << "\n\n";
	
	size_t counter = 0, idat_index = 0, plte_index = 0;
	for(Chunk& chunk : png.chunks)
	{
		if(memcmp(chunk.type, "PLTE", 4) == 0) plte_index = counter;
		if(memcmp(chunk.type, "IDAT", 4) == 0) idat_index = counter;
		counter++;
	}
	size_t sample_size = (plte_index == 0 ? (png.getColorType() == 6 ? 4 : 3) : 1);
	uLong decompressed_size = (png.getWidth() * (png.getBitDepth() / 8.f) * sample_size + 1)* png.getHeight();
	
	uint8_t decompressed_data[decompressed_size];
	int res = uncompress(decompressed_data, &decompressed_size, png.chunks[idat_index].data, png.chunks[idat_index].length);
	
	if(res != Z_OK)
	{
		std::cout << "failed to decompress\n";
		return 1;
	}
	
	size_t row_length = decompressed_size/png.getHeight();
	/*
	std::cout << "decompressed data:\n";
	for(size_t i = 0; i < decompressed_size; i++){
		for(size_t k = i; k < i + row_length; k++)
			std::cout << (int)(decompressed_data[k]) << " ";
		i += row_length-1;
		std::cout << "\n";
	}
	*/
	
	std::cout << "\nfiltered data:\n";
	
	unsigned char pixels[png.getHeight()][row_length-1][sample_size];
	unsigned char row_filter;
	for(size_t i = 0; i < decompressed_size; i++){
		unsigned char pixel[4] = {0, 0, 0, 0};
		row_filter = decompressed_data[i];
		for(size_t k = i+1; k < i + row_length; )
		{
			for(size_t l = 0; l < sample_size; l++)
			{
				if(row_filter == 1) pixel[l] += decompressed_data[k+l];
				if(row_filter == 2) pixel[l] = pixels[i / row_length - 1][(k-i-1)/sample_size][l] + decompressed_data[k+l];
				pixels[i / row_length][(k-i-1)/sample_size][l] = pixel[l];
				//std::cout << (int)pixel[l] << " ";
			}
			k += sample_size;
			//std::cout << "\n";
		}
		//std::cout << "\n";
		i += row_length-1;
	}
	
	size_t temp_n = (row_length-1) / sample_size;
	for(size_t i = 0; i < png.getHeight(); i++){
		for(size_t j = 0; j < temp_n; j++){
			for(size_t k = 0; k < sample_size; k++)
				std::cout << (int)pixels[i][j][k] << ", ";
			std::cout << "\t";
		}
		std::cout << "\n";
	}
	
	return 0;
}