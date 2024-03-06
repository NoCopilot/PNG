#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include "zlib.h"

int decompress(unsigned char compressed[], unsigned char buffer[], uLongf buffer_size, uLongf compressed_size)
{
	if(uncompress(buffer, &buffer_size, compressed, compressed_size) == Z_OK)	
		return 0;
	return -1;
}

int main()
{
	unsigned char b[] = {
		0x08,
		0x1D,
		0x63, 
		0x20, 
		0x11,
		0x00,
		0x00,
		0x00,
		0x30,
		0x00,
		0x01
		/*0x38,
		0x4F,
		0x63,
		0xFC,
		0xCF,
		0x40,
		0x1A,
		0x60,
		0x82,
		0xD2,
		0x44,
		0x83,
		0x51,
		0x0D,
		0xC4,
		0x80,
		0x51,
		0x0D,
		0xC4,
		0x00,
		0x5A,
		0x6B,
		0x60,
		0x60,
		0x00,
		0x00,
		0x40,
		0x2E,
		0x01,
		0x1F*/
	}, c[1000];
	
	uLongf dest_size = sizeof(c);
	
	int res = decompress(b, c, dest_size, sizeof(b));
	
	//int res = uncompress(c, &dest_size, b, sizeof(b));
	
	if(res == Z_OK)
		std::cout << "ok\n";
	else
		std::cout << "not ok\n";

	for(int i = 0; i < 48; i++){
		for(int j = i; j < (i+3); j++)
			std::cout << (int)c[j] << " ";
		std::cout << "\n\n";
		i += 2;
	}
}