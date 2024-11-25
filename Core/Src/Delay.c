#include "delay.h"
#include "stm32f0xx_hal.h" // Подключаем HAL

static volatile uint32_t delayTicks = 0; // Переменная для подсчета тиков

// Инициализация SysTick
void Delay_Init(void)
{
    // SysTick настроен на частоту 1 мс (1000 Гц)
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
}

// Задержка в миллисекундах
void Delay_ms(uint32_t ms)
{
    delayTicks = ms; // Устанавливаем значение для задержки
    while (delayTicks > 0); // Ждем, пока таймер не дойдет до 0
}

// Задержка в микросекундах
void Delay_us(uint32_t us)
{
    uint32_t targetTicks = us * (HAL_RCC_GetHCLKFreq() / 1000000); // Расчет тиков
    uint32_t startTick = SysTick->VAL;

    while ((startTick - SysTick->VAL) < targetTicks); // Ждем завершения
}

// Обработчик SysTick (вызывается автоматически)
void HAL_SYSTICK_Callback(void)
{
    if (delayTicks > 0)
    {
        delayTicks--; // Уменьшаем счетчик
    }
}
