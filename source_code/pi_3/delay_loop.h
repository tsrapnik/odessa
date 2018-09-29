#include "type_definitions.h"

void delay_loop(volatile u64 cycles)
{
	/*u64 count = 0;
	asm volatile
	(
		".text \n\t"
		"mov %1, %0\n\t"
		"1:\n\t"
		"subs %1, %1, #1\n\t"
		"bne 1b"
		:
		: "r" (cycles), "r" (count)
		: "memory"
	);*/
	for(; cycles > 0; cycles--);
}