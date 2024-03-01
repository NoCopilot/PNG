#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

const int png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};

std::string hex(int n, int s = 0)
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

int _256(int c)
{
	if(c < 0) c += 256;
	return c;
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout << "no input file provided\n";
		return 1;
	}
	
	std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);
	file.read(buffer.data(), size);
	
	file.close();
	
	size_t ch = 0;
	size_t k = 0;
	
	std::cout << "PNG file signature:\n";
	for(int i = 0; i < 8; i++)
	{
		if(_256(buffer[ch]) != png_signature[k])
		{
			std::cout << "not valid signature\n";
			return 1;
		}
		std::cout << (int)png_signature[k++] << " ";
		ch++;
	}
	std::cout << "\n\n";
	
	while(ch < buffer.size())
	{
		size_t n = 0;
		std::cout << "chunk size: ";
		for(int i = 3; i >= 0; i--)
		{
			n += _256(buffer[ch]) << (i*8);
			ch++;
		}
		std::cout << n << "\n";		
		
		std::cout << "chunk type: ";
		for(int i = 0; i < 4; i++)
		{
			std::cout << (char)_256(buffer[ch]);
			ch++;
		}
		std::cout << "\n";
		
		std::cout << "chunk data: ";
		for(int i = 0; i < n; i++)
		{
			std::cout << hex(_256(buffer[ch]), 2) << " ";
			ch++;
		}
		std::cout << "\n";
		
		std::cout << "CRC: ";
		for(int i = 0; i < 4; i++)
		{
			std::cout << hex(_256(buffer[ch]));
			ch++;
		}
		std::cout << "\n";
		
		std::cout << "\n";
		system("pause");
	}
	
	/*
	for(auto ch : buffer)
	{
		if(ch < 0) std::cout << (int)ch + 256 << " ";
		else std::cout << (int)ch << " ";
		i++;
		if(i == 8)
		{
			std::cout << "\n";
			system("pause");
			std::cout << "\n";
			i = 0;
		}
	}
	*/
}