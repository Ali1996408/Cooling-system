#include "main.h"
#include "7_Segment.h"
#include <stdlib.h>

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
    char isNegative = 0;
    if (temp < 0) {
        
        isNegative = 1;
    }

    // Break number into individual digits
    part0 = abs(temp) % 10;           // Ones
    part1 = (abs(temp) / 10) % 10;    // Tens
    part2 = (abs(temp) / 100) % 10;   // Hundreds

    // Use a state machine or call this function sequentially to display digits
		
						
		
    static int currentStep = 0;
		
		

    switch (currentStep) {
			
			 case 0:
		if (isNegative) {
         displayDigit(-1, 10, 2); // Display Negative sign for Tens
			
					
            }
		       
		  currentStep++;
            break;  
			 case 1:
				displayDigit(-1, 9, 1); 
			  currentStep++;
            break; 
			 
						
        case 2: // Third digit (Hundreds)
            displayDigit(part2, 11, 3); // Third digit
            currentStep++;
            break;

        case 3: // Second digit (Tens)
						
					 
                displayDigit(part1, 10, 2); // Second digit
            
            
					
					currentStep++;
				
            break;
				

        case 4: // First digit (Ones)
            displayDigit(part0, 9, 1); // First digit
            currentStep++;
            break;

        case 5: // Fourth digit (Custom)
            displayDigit(part0, 8, 4); // Fourth digit (Custom pattern)
            currentStep = 0; // Reset to the first digit
            break;
				 default:
            // Reset the state machine to restart display
            currentStep = 0;
            break;
				
				
			
			
    }
}
