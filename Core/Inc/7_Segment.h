#ifndef SevenSegment_H
#define SevenSegment_H

#include <string.h>


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
		
extern volatile int savedParameterValues[80];
// Function declarations related to parameters


// Function prototype for Timer3 initialization
int mask(int num);
void temprature_display(int temp);
void parameter_display (int index);
void parameter_adjust_and_display(int paramIndex, int action);
#endif /* TIMER3_H */
