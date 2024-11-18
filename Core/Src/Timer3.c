#include "main.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "Timer3.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_TIM3_Init(void);
void Error_Handler(void);

int main(void)
{
    HAL_Init();  // Initialize the HAL Library

    SystemClock_Config();  // Configure the system clock

    // Initialize GPIO and Timer peripherals
    MX_GPIO_Init(); 
    MX_TIM3_Init();

    NVIC_SetPriority(TIM3_IRQn, 0);  // Set interrupt priority
    NVIC_EnableIRQ(TIM3_IRQn);       // Enable the TIM3 interrupt

    while (1)
    {
        // Main loop - your application code
    }
}

void MX_GPIO_Init(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);  // Enable the GPIOA clock
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);  // Configure GPIOA pin 5 as output
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);  // Make sure the pin starts low
}

void MX_TIM3_Init(void)
{
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);  // Enable TIM3 clock
    LL_TIM_SetPrescaler(TIM3, 7999);  // Prescaler value
    LL_TIM_SetAutoReload(TIM3, 999);  // Auto-reload value (period)
    LL_TIM_SetCounterMode(TIM3, LL_TIM_COUNTERMODE_UP);  // Counter mode
    LL_TIM_SetClockDivision(TIM3, LL_TIM_CLOCKDIVISION_DIV1);  // Clock division
    LL_TIM_EnableIT_UPDATE(TIM3);  // Enable update interrupt
    LL_TIM_EnableCounter(TIM3);  // Start the timer
}



void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();  // Error Handler if clock configuration fails
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();  // Error Handler if clock configuration fails
    }
}

void Error_Handler(void)
{
    while (1)
    {
        // Optionally toggle an LED or log an error message to indicate a system error
    }
}