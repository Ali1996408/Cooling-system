    #ifndef DELAY_H
    #define DELAY_H

    #include "stm32f0xx_hal.h"

    extern TIM_HandleTypeDef htim6;  // Declare as extern for global access

    void Delay_Init(void);
    void Delay_us(uint16_t us);
    void Delay_ms(uint16_t ms);

    #endif
    
