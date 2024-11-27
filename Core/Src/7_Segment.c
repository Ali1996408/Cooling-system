#include "main.h"
#include "7_Segment.h"

int  part0, part1, part2, counter = 0;

// Masks for 7-segment display
int mask(int num) {
    switch (num) {
        case 0: return 0x3F; // 0b00111111
        case 1: return 0x06; // 0b00000110
        case 2: return 0x5B; // 0b01011011
        case 3: return 0x4F; // 0b01001111
        case 4: return 0x66; // 0b01100110
        case 5: return 0x6D; // 0b01101101
        case 6: return 0x7D; // 0b01111101
        case 7: return 0x07; // 0b00000111
        case 8: return 0x7F; // 0b01111111
        case 9: return 0x6F; // 0b01101111
        case -1: return 0x80; // Show a negative sign
        default: return 0x00; // All segments off
    }
}
int maskFourthDigit(int num) {
    switch (num) {
        case 0: return 0b00111111; // Segments: ABCDEF
        case 1: return 0b00000110; // Segments: BC
        case 2: return 0b01011011; // Segments: ABDEG
        case 3: return 0b01001111; // Segments: ABCDG
        case 4: return 0b01100110; // Segments: BCFG
        case 5: return 0b01101101; // Segments: ACFDG
        case 6: return 0b01111101; // Segments: ACDEFG
        case 7: return 0b00000111; // Segments: ABC
        case 8: return 0b01111111; // Segments: ABCDEFG
        case 9: return 0b01101111; // Segments: ABCDFG
        default: return 0b00000000; // All segments off
			  case -1: return 0b11111111;
    }
}
void disableAllDigits() {
    // Disable all digit enables
    GPIOB->ODR &= ~(0xF << 8); // Clear bits 8, 9, 10, 11
}

void displayDigit(int digitValue, int enablePin, int segmentType) {
    disableAllDigits();
    GPIOB->ODR &= ~(0xFF); // Clear segment outputs
  
    // Select appropriate mask function
    int maskValue;
    if (segmentType == 4) {
        maskValue = maskFourthDigit(digitValue);
    } else {
        maskValue = mask(digitValue);
    }
  
    GPIOB->ODR |= (~maskValue & 0xFF); // Assuming active-low
    GPIOB->ODR |= (1 << enablePin); // Enable display
}

void SevenSegment(int temp) {
   char isNegative=0;
	if(temp<0)
	{
		temp*=(-1);
		isNegative=1;
		
	}
//        if (number > 5) {
//            // Display -1 on all digits
//            displayDigit(-1, 11, 3); // Third digit
//            HAL_Delay(1);

////            displayDigit(-1, 10, 2); // Second digit
////            HAL_Delay(1);

            displayDigit(-1, 9, 1); // First digit
            HAL_Delay(1);

//            // Display -1 or another pattern on the fourth digit, if needed
////            displayDigit(-1, 8, 4); // Custom pattern
////            HAL_Delay(1);
//        } else {
            // Standard display logic
            part0 = temp % 10;
            part1 = (temp / 10) % 10;
            part2 = (temp / 100) % 10;

            displayDigit(part2, 11, 3); // Third digit
            HAL_Delay(1);

            displayDigit(part1, 10, 2); // Second digit
            HAL_Delay(1);

            displayDigit(part0, 9, 1); // First digit
            HAL_Delay(1);

            // Display unique characters on the fourth digit
            displayDigit(part0, 8, 4); // Custom pattern
            HAL_Delay(1);
						if(isNegative){
							
							displayDigit(-1, 10, 2); // Second digit
                HAL_Delay(1);
							
						}
						
        //}

    }
