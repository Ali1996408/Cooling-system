#include "main.h"
#include "7_Segment.h"
#include <stdlib.h>

// Define shift register pins
#define DATA_PIN  DS_Pin
#define CLOCK_PIN SHCP_Pin
#define LATCH_PIN STCP_Pin

void sendToShiftRegister(uint8_t data);

int part0, part1, part2;

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
        default: return 0x00; // All segments off
    }
}

void sendToShiftRegister(uint8_t data) {
    HAL_GPIO_WritePin(GPIOB, LATCH_PIN, GPIO_PIN_RESET);

    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(GPIOB, DATA_PIN, ((data & (1 << (7 - i))) ? GPIO_PIN_RESET : GPIO_PIN_SET)); // Invert data for common anode
        HAL_GPIO_WritePin(GPIOB, CLOCK_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, CLOCK_PIN, GPIO_PIN_RESET);
    }

    HAL_GPIO_WritePin(GPIOB, LATCH_PIN, GPIO_PIN_SET);
}

void disableAllDigits() {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // Disable Ones
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // Disable Tens
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // Disable Hundreds
}

void displayDigit(int digitValue, int enablePin) {
    disableAllDigits();
    sendToShiftRegister(mask(digitValue));  // Send the segment data directly
    HAL_GPIO_WritePin(GPIOB, enablePin, GPIO_PIN_SET); // Enable the digit
}

void SevenSegment(int temp) {
    part0 = abs(temp) % 10;          // Ones
    part1 = (abs(temp) / 10) % 10;   // Tens
    part2 = (abs(temp) / 100) % 10;  // Hundreds

    static int currentStep = 0;

    switch (currentStep) {
        case 0:
            displayDigit(part2, GPIO_PIN_11); // Display Hundreds
            currentStep++;
            break;
        case 1:
            displayDigit(part1, GPIO_PIN_10); // Display Tens
            currentStep++;
            break;
        case 2:
            displayDigit(part0, GPIO_PIN_9); // Display Ones
            currentStep = 0; // Reset step
            break;
    }
}
