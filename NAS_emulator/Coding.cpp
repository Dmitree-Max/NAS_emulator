#include "Coding.h"



int ascii_ti_int(char symbol)
{
	int num = 0;
	if (symbol >= (int)'a')
	{
		num = symbol - (int)'a' + 10;
	}
	else
	{
		num = symbol - (int)'0';
	}
	if (num < 0)
	{
		printf("symbol: %c,  num: %i\n", symbol, num);
	}
	return num;
}


char int_to_ascii(int code)
{
	char result = 0;
	if (code < 10)
	{
		result = code + '0';
	}
	else
	{
		result = code - 10 + 'a';
	}
	return result;
}


void decode_signal(char* src, char* dst, int src_length)
{
	for(int symbol = 0; symbol < src_length; symbol++)
	{
		dst[symbol * 2] = int_to_ascii((src[symbol] + 128) / 16);
		dst[symbol * 2 + 1] = int_to_ascii((src[symbol] + 128) % 16);

	}
}

void code_signal(char* src, char* dst, int src_length)
{
	int first_num = 0;
	int second_num = 0;
	for(int symbol = 0; symbol < src_length / 2; symbol ++)
	{
		first_num = ascii_ti_int(src[symbol * 2]);
		//std::cout << " first_num = " << first_num << '\n';
		second_num = ascii_ti_int(src[symbol * 2 + 1]);
		//std::cout << " second_num = " << second_num << '\n';
		dst[symbol] = 16 * first_num + second_num - 128;
        //printf("%i", 16 * first_num + second_num - 128);
	}
}


