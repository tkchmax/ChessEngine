#include "Misc.h"

#include <iostream>

void misc::ShowBits(const U64& bitboard)
{
	int bit;
	int count = 0;

	for (int i = 64 - 8; i >= 0; i++) {
		bit = (bitboard >> i) & 1ULL;
		std::cout << bit << '\t';
		count++;
		if (count == 8)
		{
			count = 0;
			i -= 16;
			std::cout << std::endl;
		}
	}
	std::cout << std::endl << std::endl;
}
