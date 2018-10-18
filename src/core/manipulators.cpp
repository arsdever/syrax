#include <manipulators.h>

int DigitCount(int number)
{
	int digits = 1;
	while (number >= 10) {
		number /= 10;
		++digits;
	}

	return digits;
}