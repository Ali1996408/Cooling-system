#ifndef DELAY_H
#define DELAY_H

#include "stm32f0xx_hal.h"

// Инициализация задержки
void Delay_Init(void);

// Задержка в миллисекундах
void Delay_ms(uint32_t ms);

// Задержка в микросекундах
void Delay_us(uint32_t us);

#endif // DELAY_H
