#include "stm32f1xx_hal.h"
#include "tim.h" // Для доступа к предварительно сгенерированному коду (htim6)

// Функция для старта таймера
void Delay_Init(void)
{
    HAL_TIM_Base_Start(&htim6); // Запуск таймера 6
}
void Delay_ms(uint16_t ms)
{
    while (ms > 0)
    {
        // Ждём, пока таймер не сбросится (переполнится)
        if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE)) // Проверяем флаг переполнения
        {
            __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE); // Сбрасываем флаг
            ms--; // Уменьшаем оставшееся время
        }
    }
}
