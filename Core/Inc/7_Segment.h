#ifndef SevenSegment_H
#define SevenSegment_H

#include <string.h>

#include "stm32f0xx_ll_bus.h"


 typedef struct {
        char code[4]; // Parameter code (e.g., "r01")
        int type; // 0 for numeric, 1 for categorical
        int minValue; // Minimum value (for numeric parameters)
        int maxValue; // Maximum value (for numeric parameters)
        char *options[2]; // Options for categorical parameters (e.g., {"Off", "On"})
        int currentValue; // Current value (or index for categorical)
    } Parameter;

    // Example parameters (expand this with your full parameter set)
 extern   Parameter parameters[];
extern volatile int temprature;	
extern volatile int savedParameterValues[80];
// Function declarations related to parameters
extern const uint32_t INACTIVITY_TIME;//time in ms for inactivity
extern const char DEBOUNCE_THRESHOLD_MS;//debounce time

// Function prototype for Timer3 initialization
void temprature_display(int temp);
int mask(int num);
//void temprature_display(int temp);
void parameter_display (int index);
void parameter_adjust_and_display(int paramIndex, int action);
void handle_menu_logic(void);
void check_inactivity(void);
void toggle (void);
#endif /* TIMER3_H */
