#include "main.h"
#include "7_Segment.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "7_Segment.h"
#include "stdlib.h"
// Define shift register pins
#define DATA_PIN  DS_Pin
#define CLOCK_PIN SHCP_Pin
#define LATCH_PIN STCP_Pin
#define TOTAL_PARAMETERS 80
volatile int button_debounce_timer[4] = {0, 0, 0, 0}; // Debounce timers for 4 buttons

const uint32_t INACTIVITY_TIME = 300000;//time in ms for inactivity
const char DEBOUNCE_THRESHOLD_MS=50;//debounce time
volatile uint32_t inactivity_timer = 0;
volatile int temprature; 
void sendToShiftRegister(uint8_t data);
// volatile int temprature; 

int  part0, part1, part2, counter = 0, tempType=0;

volatile int savedParameterValues[80];
enum MenuLevel {
    MAIN_SCREEN = 0,
    PARAMETER_SELECTION = 1,
    PARAMETER_ADJUSTMENT = 2
};

static enum MenuLevel menu_position = MAIN_SCREEN;
static int selected_parameter = 0;  // Tracks which parameter you're adjusting


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
        case 0: return 0b00100000; // Segments: ABCDEF
        case 1: return 0b01000000; // Segments: BC
//        case 2: return 0b01011011; // Segments: ABDEG
//        case 3: return 0b01001111; // Segments: ABCDG
//        case 4: return 0b01100110; // Segments: BCFG
//        case 5: return 0b01101101; // Segments: ACFDG
//        case 6: return 0b01111101; // Segments: ACDEFG
//        case 7: return 0b00000111; // Segments: ABC
//        case 8: return 0b01111111; // Segments: ABCDEFG
//        case 9: return 0b01101111; // Segments: ABCDFG
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
				   case 'E': return 0b01111001;
				   case 'L': return 0b00111000;
				   case 'g': return 0b01101111;
				   case 'a': return 0b01011111;
				   case 'y': return 0b01101110;
				   case 'e': return 0b01111011;
				 
				  

           default: return 0b00000000;  // All segments off
       }
   }
 bool isNumeric(char c) {
       return c >= '0' && c <= '9'; // Check if it's a digit
   }

void sendToShiftRegister(uint8_t data) {
    HAL_GPIO_WritePin(GPIOB, LATCH_PIN, GPIO_PIN_RESET);

    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(GPIOB, DATA_PIN, ((data & (1 << (7 - i))) ? GPIO_PIN_RESET : GPIO_PIN_SET)); // Invertir datos para cátodo común
        HAL_GPIO_WritePin(GPIOB, CLOCK_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, CLOCK_PIN, GPIO_PIN_RESET);
    }

    HAL_GPIO_WritePin(GPIOB, LATCH_PIN, GPIO_PIN_SET);
}

void disableAllDigits() {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // Deshabilitar primer dígito
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); // Deshabilitar segundo dígito
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // Deshabilitar tercer dígito
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // Deshabilitar cuarto dígito
}

void displayDigit(int digitValue, int enablePin, int segmentType) {
    disableAllDigits();
    
    // Selecciona la máscara apropiada
    int maskValue;
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
  
    
    sendToShiftRegister(maskValue);  
    
    HAL_GPIO_WritePin(GPIOB, (1 << enablePin), GPIO_PIN_SET);
}

void temprature_display(int temp) {
    // Preprocess variables to hold digit values and flags
    int segments[3] = {0};   // Digits: Hundreds = segments[2], Tens = segments[1], Ones = segments[0]
    int IsNegative = 0;    // Symbol: -1 = dot (.), -2 = negative sign (-)
    int displayUnit = 0;     // Default unit (Celsius = 0, Fahrenheit = 1)
    static int currentStep = 0;
		float fahrenheit;
		int int_part;
		int frac_part;
		
		temp*=10;
		
		displayUnit=r05_Korrekciya_pokazanii_temperatury;
		if(displayUnit)
			{
				 fahrenheit = (temp * 9.0 / 5.0)/10.0 + 32.0;
			
			
	  int_part = (int)fahrenheit;                 // Integer part of the result
    frac_part = (int)((fahrenheit  * 10))%10;  // First digit after 
				
				segments[0]=frac_part;
				segments[1]=int_part % 10;
				segments[2]=(int_part / 10) % 10;
				
			}
			else
			{
				 // Extract digits
    segments[0] = abs(temp) % 10;                // Ones place
    segments[1] = (abs(temp) / 10) % 10;         // Tens place
    segments[2] = (abs(temp) / 100) % 10;        // Hundreds place
			}
    // Handle negative temperatures
    if (temp < 0) {
        IsNegative = 1;  // Set negative sign
               // Convert to positive for further processing
    }

   

   

    // Ensure the step loops between 0 and 5
    currentStep %= 6;

    // Multiplexing logic
    switch (currentStep) {
        case 0:
					if(IsNegative==1)
					{
            // This case is for negative sign only
            displayDigit(-1, 10, 2); // Display "-" (only if temp < 0)
					}
            break;
        case 1:
            displayDigit(segments[1], 10, 2);  // Tens place
            break;
        case 2:
            displayDigit(segments[0], 9, 1);   // Ones place
            break;
        case 3:
            displayDigit(segments[2], 11, 3);  // Hundreds place
            break;
        case 4:
            displayDigit(displayUnit, 8, 4);   // Celsius/Fahrenheit indicator
            break;
        case 5:
            displayDigit(-1, 9, 1);            // Display dot (. symbol)
            break;
    }

    // Increment the current step for the next cycle
    currentStep++;
}

void parameter_display(int index) {
    static int currentStep = 0;
    static char displayBuffer[5] = {0};  // Increased buffer size to 4 + null terminator
  
    const char *parameters[] = {
        "---", "r01", "r02", "r03", "r04", "r05", "r09", "r12", "r13", "r39", 
        "r40", "A03", "A04", "A12", "A13", "A14", "A27", "A37", "c01", "c02", 
        "c30", "c70", "d01", "d02", "d03", "d04", "d05", "d06", "d07", "d08", 
        "d09", "d10", "d18", "d19", "F01", "F02", "F04", "t01", "t02", "t03", 
        "t04", "t05", "t06","t07","t08", "t11", "t12", "t13", "t14", "t15", "t16", "t45",
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
// {-500, 500},   // Index 0: Range 0–99
//    {1, 200},   // Index 1: Range 0–31
//    {-490, 500},   // Index 2: Range 0–59
//    {-500, 490}, // Index 3: Range -50–50
//    {-200, 200},
//    {0, 10},
//    {-100, 100},		// Index 4: Range 0–240
//		{-10,10},
//		{-100,100},
//		{-500,500},
//		{0,2400},
//		{0,990},
//		{0,300},
//		{0,300},
//		{0,250},
//		{0,480},
//		{0,1800},
//		{0,2400},
//		{0,600},
//		{0,600},
//		{-150,0},
//		{0,20},
//		{0,20},
//		{0,480},
//		{0,200},
//		{0,300},
//		{-500,500},
//		{0,230},
//		{0,230},
//		{0,230},
//		{0,230},
//		{0,230},
//		{0,230},
//		{0,590},
//		{0,590},
//		{0,590},
//		{0,590},
//		{0,590},
//		{0,590},
//		{0,230},
//		{0,590},
//		{10,310},
//		{10,120},
//		{0,990},
//		{0,6000},
//		{0,110},
//		{0,240},
//		{0,1000},
//		{0,600},
//		{10,30},
//		{0,20},
//		{0,1000},
//		{0,250},
//		{0,250},
//		{0,20},
//		{10,20},
//		 {"OFF", "ON"},  // Index 5: Text values
//    {"OFF", "ON"},        // Index 6: Text values
//    {"no", "gas"},
//    {"no", "yes"},
//    {"OFF", "ON"},	
//    {"Pt","ntc"},
//		{"OFF", "ON"},
//		{"OFF", "ON"},

// Parameter adjustment function
void parameter_adjust_and_display(int paramIndex, int action) {
    // Define your Parameter structure
    typedef struct {
        char code[4];           // Parameter code (e.g., "r01")
        int type;               // 0 for numeric, 1 for categorical
        int minValue;           // Minimum value (for numeric parameters)
        int maxValue;           // Maximum value (for numeric parameters)
        char *options[3];       // Options for categorical parameters (e.g., {"Off", "On"})
        int currentValue;       // Current value (or index for categorical)
    } Parameter;

    // Static array of parameters
    static Parameter parameters[] = {
        {"---", 0, -50, 50, {NULL, NULL}, 2}, // Numeric parameter, range 0-100
        {"r01", 0, 1, 200, {NULL, NULL}, 20}, // Numeric parameter, range 0.1-20K
        {"r02", 0, -49, 50, {NULL, NULL}, 50}, // Numeric parameter, range 10-50
        {"r03", 0, -50, 49, {NULL, NULL}, -50},
        {"r04", 0, -20, 20, {NULL, NULL}, 0},	
        {"r05", 1, 0, 0, {" ", " "}, 0},
        {"r09", 0, -10, 10, {NULL, NULL}, 0},
        {"r12", 0, -1, 1, {NULL, NULL}, 0},		
        {"r13", 0, -10, 10, {NULL, NULL}, 0},						
        {"r39", 1, 0, 0, {"OFF", "On"}, 0},	// Categorical parameter (Off/On)				
				{"r40", 0, -50, 50, {NULL, NULL}, 0},
        {"A03", 0 , 0, 240, {NULL, NULL}, 30},	
        {"A04", 0, 0, 240, {NULL, NULL}, 60},	
        {"A12", 0, 0, 240, {NULL, NULL}, 90},	
				{"A13", 0, -50, 50, {NULL, NULL}, 8},
        {"A14", 0, -50, 50, {NULL, NULL}, -30},	
        {"A27", 0, 0, 240, {NULL, NULL}, 30},	
        {"A37", 0, 0, 99, {NULL, NULL}, 50},
        {"c01", 0, 0, 30, {NULL, NULL}, 0},
        {"c02", 0, 0, 30, {NULL, NULL}, 0},
        {"c30", 1, 0, 0, {"OFF", "On"}, 0},
        {"c70", 1, 0, 0, {"OFF", "On"}, 0},
        {"d01", 1, 0, 2, {"no","EL", "gas"}, 1},	
        {"d02", 0, 0, 25, {NULL, NULL}, 6},	
        {"d03", 0, 0, 48, {NULL, NULL}, 8},
        {"d04", 0, 0, 180, {NULL, NULL}, 45},
        {"d05", 0, 0, 240, {NULL, NULL}, 0},
        {"d06", 0, 0, 60, {NULL, NULL}, 0},	
        {"d07", 0, 0, 60, {NULL, NULL}, 0},					
        {"d08", 0, -15, 0, {NULL, NULL}, -5},
        {"d09", 0, 0, 2, {NULL, NULL}, 1},
        {"d10", 0, 0, 2, {NULL, NULL}, 0},
        {"d18", 0, 0, 48, {NULL, NULL}, 0},	
        {"d19", 0, 0, 20, {NULL, NULL}, 20},	
        {"F01", 1, 0, 0, {"no", "yes"}, 0},
        {"F02", 0, 0, 30, {NULL, NULL}, 0},
        {"F04", 0, -50, 50, {NULL, NULL}, 50},
        {"t01", 0, 0, 23, {NULL, NULL}, 0},	
				{"t02", 0, 0, 23, {NULL, NULL}, 0},
        {"t03", 0, 0, 23, {NULL, NULL}, 0},	
        {"t04", 0, 0, 23, {NULL, NULL}, 0},
        {"t05", 0, 0, 23, {NULL, NULL}, 0},	
        {"t06", 0, 0, 23, {NULL, NULL}, 0},	
        {"t07", 0, 0, 23, {NULL, NULL}, 0},
        {"t08", 0, 0, 59, {NULL, NULL}, 0},
        {"t11", 0, 0, 59, {NULL, NULL}, 0},
        {"t12", 0, 0, 59, {NULL, NULL}, 0},				
   			{"t13", 0, 0, 59, {NULL, NULL}, 0},	
				{"t14", 0, 0, 59, {NULL, NULL}, 0},
        {"t15", 0, 0, 59, {NULL, NULL}, 0},	
        {"t16", 0, 0, 59, {NULL, NULL}, 0},
        {"t45", 0, 1, 31, {NULL, NULL}, 1},
        {"t46", 0, 1, 12, {NULL, NULL}, 1},
        {"t47", 0, 0, 99, {NULL, NULL}, 0},	
        {"o01", 0, 0, 600, {NULL, NULL}, 5},
        {"o02", 0, 11, 0, {NULL, NULL}, 0},
				{"o03", 0, 0, 240, {NULL, NULL}, 0},
				{"o04", 1, 0, 0, {"OFF", "On"}, 0},
				{"o05", 0, 0, 100, {NULL, NULL}, 0},
        {"o06", 1, 0, 0, {"Pt", "ntc"}, 0},
        {"o15", 1, 0, 0, {"no", "yes"}, 0},
        {"o16", 0, 0, 60, {NULL, NULL}, 20},
        {"o38", 0, 1, 3, {NULL, NULL}, 1},
        {"o39", 1, 0, 0, {"OFF", "On"}, 0},
        {"o46", 0, 0, 2, {NULL, NULL}, 0},
        {"o64", 0, 0, 100, {NULL, NULL}, 0},
        {"o65", 0, 0, 25, {NULL, NULL}, 0},
        {"o66", 0, 0, 25, {NULL, NULL}, 0},	
        {"o67",  1, 0, 0, {"OFF", "On"}, 0},
				{"o70",  0, 0, 2, {NULL, NULL}, 0},
				{"o72",  0, 1, 2, {NULL, NULL}, 2},
        				
				
    };

    // Ensure the provided index is valid
    int parameterCount = sizeof(parameters) / sizeof(parameters[0]);
    if (paramIndex < 0 || paramIndex >= parameterCount) {
        return; // Invalid index, exit function
    }

    // Get reference to the specific parameter
    Parameter *param = &parameters[paramIndex];

    // Static variables to manage display and multiplexing
    static char displayBuffer[4] = "   "; // Temporary display content
    static int currentStep = 0;          // Display digit step for multiplexing

switch (action) {
    case 0: // Increment
        if (param->type == 0 && param->currentValue < param->maxValue) { 
            // Numeric: increment within range
            param->currentValue++;
        } else if (param->type == 1) { // Categorical: increment
            // Determine the number of options
            int numOptions = (param->options[2] == NULL) ? 2 : 3;

            // Increment only if not at the last option
            if (param->currentValue < numOptions - 1) {
                param->currentValue++;
            }
        }
        break;

    case 1: // Decrement
        if (param->type == 0 && param->currentValue > param->minValue) { 
            // Numeric: decrement within range
            param->currentValue--;
        } else if (param->type == 1) { // Categorical: decrement
            // Decrement only if not at the first option
            if (param->currentValue > 0) {
                param->currentValue--;
            }
        }
        break;




        case 2: // Confirm and save
            // save_parameter_value(param); // Save value if implemented
				// Confirm and save
            savedParameterValues[paramIndex] = param->currentValue;
            break;
						
            return; // No further display update needed
           
        case 3: // Refresh display without changing the parameter

            break; // No parameter adjustment, just refresh the display
           
        default:
					action=0;
            break;
    }

    // Always update display buffer (no need for needsUpdate flag)
    if (param->type == 0) { // Numeric values
        snprintf(displayBuffer, sizeof(displayBuffer), "%03d", param->currentValue);
    } else if (param->type == 1) { // Categorical values
        strncpy(displayBuffer, param->options[param->currentValue], 3);
        displayBuffer[3] = '\0'; // Ensure null termination
    }

    // Handle multiplexed digit display
    char charToDisplay = displayBuffer[currentStep]; // Get the current digit/character to show
		

    // Call the displayDigit function for the corresponding digit
	  bool isNegative = (displayBuffer[0] == '-'); // Check if negative
		 if (isNegative) {
			 displayBuffer[0]='0';
//			   displayDigit(-1, 10, 2); 
        }
		
bool isR01 = (strcmp(param->code, "r01") == 0);
bool isR05 = (strcmp(param->code, "r05") == 0);
//				 if (strcmp(param->code, "r01") == 0)
//						{
//							displayDigit(-1, 9, 1); 
//						 
//						
//					   }
	

bool tempType=0;
				

if(param->currentValue == 0)
{
tempType=0;
	


}	

else 
{
	tempType=1;
}
//				if (isR05)

//            
//          {
//						 displayDigit(tempType,8,4);

//            
//          }			 
//						 if (strcmp(param->code, "r05") == 0)

//						
//					{
//						if(param->currentValue == 0)
//						{
//							
//							displayDigit(0,8,4);
//						}
//						else
//						{
//								
//							displayDigit(1,8,4);
//						
//						
//					}
//				}
			  disableAllDigits();
		
    switch (currentStep) {
      
          case 0: // First step: Show the negative sign separately
        if (isNegative) {
					displayBuffer[0]='0';
            displayDigit(-1, 10, 2); // Display the negative sign (-) in a specific position
					
        } 
            currentStep++;
            break;

        case 1:
          displayDigit(displayBuffer[0] , 11, 5);
           // Display the digit/character
            currentStep++;
            break;

        case 2: // Second digit (middle)
					
            displayDigit(displayBuffer[1] , 10, 5); // Display the digit/character
            currentStep++;
            break;

        case 3: // Third digit (right-most)
            
            displayDigit(displayBuffer[2] , 9, 5);        // Display the digit/character
            currentStep++;
            break;
				
				case 4:
					if (isR01)
						{
							displayDigit(-1, 9, 1); 
						 
						
					   }
						currentStep++;
						   break;
				case 5:
		  if (isR05)

            
          {
						 displayDigit(tempType,8,4);

            
          }
					currentStep=0;
					break;
          
						
					
					
				
					
//if (!(isR01) )
//                    {
//                      displayDigit(0, 8, 4);
//                      }

//				currentStep=0;
//				  break;
//					

        default:
            currentStep = 0;
            break;
				
			 
			
    }
		

}
 
void handle_menu_logic(void) {
    // Timestamps for button auto-repeat logic
    static uint32_t forward_hold_time = 0;
    static uint32_t backward_hold_time = 0;
    static uint32_t last_repeat_time = 0;

    // Auto-reset repeat delay (in milliseconds)
    uint32_t repeat_delay = 300;  // Initial delay before auto-repeat
    uint32_t repeat_rate = 100;   // Auto-repeat action every 100ms

    // Handle different menu positions
    switch (menu_position) {
        
        case MAIN_SCREEN:
            // 1. Display the current temperature
            temprature_display(temprature);

            // 2. Navigate to PARAMETER_SELECTION on forward/backward press
            if (buttonFlagForward || buttonFlagBackward) {
                menu_position = PARAMETER_SELECTION;
                selected_parameter = 0;
                buttonFlagForward = 0;
                buttonFlagBackward = 0;
            } else if (buttonFlagConfirm) {
                buttonFlagConfirm = 0;
            } else if (buttonFlagCancel) {
                buttonFlagCancel = 0;
            }
            break;

        case PARAMETER_SELECTION:
            // 1. Display the currently selected parameter
				
            parameter_display(selected_parameter);

            // 2. Handle button "Forward" with auto-repeat
            if (HAL_GPIO_ReadPin(BUTTON3_GPIO_Port, BUTTON3_Pin) == GPIO_PIN_RESET) // FORWARD button held
            {
                uint32_t now = HAL_GetTick(); // Get current time
                if (forward_hold_time == 0) forward_hold_time = now; // Start hold timer

                // Check for initial delay or repeat interval
                if (((now - forward_hold_time > repeat_delay) && (now - last_repeat_time > repeat_rate)) ||
                    (buttonFlagForward)) {
                    
                    selected_parameter++;
                    last_repeat_time = now; // Update last repeat time
                    buttonFlagForward = 0; // Clear normal flag
                }
            } else {
                forward_hold_time = 0; // Reset hold timer on release
            }

            // 3. Handle button "Backward" with auto-repeat
            if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == GPIO_PIN_RESET) // BACKWARD button held
            {
                uint32_t now = HAL_GetTick(); // Get current time
                if (backward_hold_time == 0) backward_hold_time = now; // Start hold timer

                // Check for initial delay or repeat interval
                if (((now - backward_hold_time > repeat_delay) && (now - last_repeat_time > repeat_rate)) || 
                    (buttonFlagBackward)) {
                    
                    selected_parameter--;
                    last_repeat_time = now; // Update last repeat time
                    buttonFlagBackward = 0; // Clear normal flag
                }
            } else {
                backward_hold_time = 0; // Reset hold timer on release
            }

            // Prevent out-of-bound parameter selections
            if (selected_parameter < 0) selected_parameter = 0;
            if (selected_parameter >= TOTAL_PARAMETERS) selected_parameter = TOTAL_PARAMETERS - 1;

            // 4. Change state with CONFIRM or CANCEL
            if (buttonFlagConfirm) {
                menu_position = PARAMETER_ADJUSTMENT;
                buttonFlagConfirm = 0;
            } else if (buttonFlagCancel) {
                menu_position = MAIN_SCREEN;
                buttonFlagCancel = 0;
            }
            break;

        case PARAMETER_ADJUSTMENT:
					
				parameter_adjust_and_display(selected_parameter,3);
    // Forward button functionality (Auto-repeat enabled)
    if (HAL_GPIO_ReadPin(BUTTON3_GPIO_Port, BUTTON3_Pin) == GPIO_PIN_RESET) {
        uint32_t now = HAL_GetTick(); // Get current time
        if (forward_hold_time == 0) {
            forward_hold_time = now;     // Start hold timer
        }

        // If holding button, trigger action after initial delay or repeat rate
        if (((now - forward_hold_time > repeat_delay) && (now - last_repeat_time > repeat_rate)) ||
            buttonFlagForward) {
            parameter_adjust_and_display(selected_parameter, 0); // Increment parameter
            last_repeat_time = now;     // Update repeat timer
            buttonFlagForward = 0;      // Clear normal flag
        }
    } else {
        forward_hold_time = 0; // Reset hold timer when button released
	
			
    }

    // Backward button functionality (Auto-repeat enabled)
    if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == GPIO_PIN_RESET) {
        uint32_t now = HAL_GetTick(); // Get current time
        if (backward_hold_time == 0) {
            backward_hold_time = now;  // Start hold timer
        }

        // If holding button, trigger action after initial delay or repeat rate
        if (((now - backward_hold_time > repeat_delay) && (now - last_repeat_time > repeat_rate)) ||
            buttonFlagBackward) {
            parameter_adjust_and_display(selected_parameter, 1); // Decrement parameter
            last_repeat_time = now;      // Update repeat timer
            buttonFlagBackward = 0;      // Clear normal flag
        }
    } else {
        backward_hold_time = 0; // Reset hold timer when button released
			
    }

    // Confirm adjustment when confirm button is pressed
    if (buttonFlagConfirm) {
        parameter_adjust_and_display(selected_parameter, 2); // Confirm adjustment
			menu_position=MAIN_SCREEN;
        buttonFlagConfirm = 0;
    }

    // Cancel adjustment and return to MAIN_SCREEN
    if (buttonFlagCancel) {
        menu_position = MAIN_SCREEN;
        buttonFlagCancel = 0;
    }

    break;
		
				default:
					menu_position=MAIN_SCREEN;

    }
}

void debounce_buttons() {
    for (int i = 0; i < 4; i++) {
        if (button_debounce_timer[i] > 0)
					button_debounce_timer[i]--;
    }
}

void check_inactivity() {
    if (inactivity_timer > 0) {
        inactivity_timer--;
    } else {
      
            menu_position = MAIN_SCREEN;
			      inactivity_timer=0;
        
    }
}

void toggle ()
{
static	int toggle_time =1000;
	if(toggle_time>0)
	{
		toggle_time--;
		
	}
	else
	{
		    LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		    toggle_time = 1000;

	}
}