#include "main.h"

int number=0;

int mask (int num)
{
	switch(num)
	{
		case 0: return 0x003f;
		case 1: return 0x0006;
		case 2: return 0x005b;
		case 3: return 0x004f;
		case 4: return 0x0066;
		case 5: return 0x006d;
		case 6: return 0x007d;
		case 7: return 0x0007;
		case 8: return 0x007f;
		case 9: return 0x006f;
		default: return 0x0000;
		
		
		
	}
	
	
	
}

