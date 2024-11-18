#include "main.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "Timer3.h"

/* Private function prototypes -----------------------------------------------*/
//void SystemClock_Config(void);
//void MX_GPIO_Init(void);
//void MX_TIM3_Init(void);
//void Error_Handler(void);



//void MX_GPIO_Init(void)
//{
//    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);  // Enable the GPIOA clock
//    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);  // Configure GPIOA pin 5 as output
//    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);  // Make sure the pin starts low
//}

//void MX_TIM3_Init(void)
//{
//    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);  // Enable TIM3 clock
//    LL_TIM_SetPrescaler(TIM3, 7999);  // Prescaler value
//    LL_TIM_SetAutoReload(TIM3, 999);  // Auto-reload value (period)
//    LL_TIM_SetCounterMode(TIM3, LL_TIM_COUNTERMODE_UP);  // Counter mode
//    LL_TIM_SetClockDivision(TIM3, LL_TIM_CLOCKDIVISION_DIV1);  // Clock division
//    LL_TIM_EnableIT_UPDATE(TIM3);  // Enable update interrupt
//    LL_TIM_EnableCounter(TIM3);  // Start the timer
//}




//void Error_Handler(void)
//{
//    while (1)
//    {
//        // Optionally toggle an LED or log an error message to indicate a system error
//    }
//}