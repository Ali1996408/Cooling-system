#include "main.h"
#include "7_Segment.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


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
int mask_charecters(char index) {
       switch (index) {
           case 'r': return 0b01010000;
           case 'A': return 0b01110111;
           case 'F': return 0b01110001;
           case 'O': return 0b01011100;
           case 'N': return 0b01110110;
           case 'd': return 0b01011110;
           case 'u': return 0b00111110;
           case 's': return 0b01101101;
           case 'n': return 0b01010100;
           case 'P': return 0b01110011;
           case '-': return 0b01000000;  // Dash
				   case 'c': return 0b01011000; // Example mask for 'c'
           case 't': return 0b01110000; // Example mask for 't'
           case 'o': return 0b01011100; // Example mask for 'o'

           default: return 0b00000000;  // All segments off
       }
   }
 bool isNumeric(char c) {
       return c >= '0' && c <= '9'; // Check if it's a digit
   }



void disableAllDigits() {
    // Disable all digit enables
    GPIOB->ODR &= ~(0xF << 8); // Clear bits 8, 9, 10, 11
}

void displayDigit(int digitValue, int enablePin, int segmentType) {
    disableAllDigits();
    GPIOB->ODR &= ~(0xFF); // Clear segment outputs
  
    // Select appropriate mask function
    int maskValue=0;
    // Decide which mask to use
    if (segmentType == 5) { // Mixed characters/numbers
        if (isNumeric(digitValue)) {
            maskValue = mask(digitValue - '0'); // Use numeric mask
        } else {
            maskValue = mask_charecters(digitValue); // Use character mask
        }
    } else if (segmentType == 4) { // Fourth digit, custom logic
        maskValue = maskFourthDigit(digitValue);
    } else { // Default to numeric
        maskValue = mask(digitValue);
    }
    GPIOB->ODR |= (~maskValue & 0xFF); // Assuming active-low
    GPIOB->ODR |= (1 << enablePin); // Enable display
}

void temprature_display(int temp) {
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

void parameter_display(int index) {
    static int currentStep = 0;
    static char displayBuffer[5] = {0};  // Increased buffer size to 4 + null terminator
  
    const char *parameters[] = {
        "---", "r01", "r02", "r03", "r04", "r05", "r09", "r12", "r13", "r39", 
        "r40", "A03", "A04", "A12", "A13", "A14", "A27", "A37", "c01", "c02", 
        "c30", "c70", "d01", "d02", "d03", "d04", "d05", "d06", "d07", "d08", 
        "d09", "d10", "d18", "d19", "F01", "F02", "F04", "t01", "t02", "t03", 
        "t04", "t05", "t06", "t11", "t12", "t13", "t14", "t15", "t16", "t45",
        "t46", "t47", "o01", "o02", "o03", "o04", "o05", "o06", "o15", "o16", 
        "o38", "o39", "o46", "o64", "o65", "o66", "o67", "o70", "o72", "u09", 
        "u10", "u13", "u28", "u58", "u59", "u60", "u69", "u71"
    };
  
    // Verify index is in bounds
    if (index < 0 || index >= (sizeof(parameters)/sizeof(parameters[0]))) {
        index = sizeof(parameters)/sizeof(parameters[0]) - 1;
    }

    // Store the current parameter into the buffer
    strncpy(displayBuffer, parameters[index], 3);
   displayBuffer[3] = '\0'; // Explicitly terminate the string
  
    // Display the current step
    switch (currentStep) {
        case 0: 
            displayDigit(displayBuffer[0], 11, 5); // Third digit
            currentStep++;
            break;
        case 1: 
            displayDigit(displayBuffer[1], 10, 5); // Second digit
            currentStep++;
            break;
        case 2:
            displayDigit(displayBuffer[2], 9, 5); // First digit
            currentStep = 0; // Reset step
            break;
        default:
            currentStep = 0;
            break;  
    }
}


	
	
	
	
	
	
	
	