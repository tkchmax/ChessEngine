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

unsigned long misc::BitScanForward(const U64& mask)
{
	unsigned long index;
	unsigned char isNonzero;
	isNonzero = _BitScanForward64(&index, mask);

	if (!isNonzero)
		throw std::exception("empty mask");
	return index;
}

unsigned long misc::BitScanReverse(const U64& mask)
{
	unsigned long index;
	unsigned char isNonzero;
	isNonzero = _BitScanReverse64(&index, mask);

	if (!isNonzero)
		throw std::exception("empty mask");
	return index;
}
