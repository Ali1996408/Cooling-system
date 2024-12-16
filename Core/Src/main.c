/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "7_Segment.h"
#include "stdlib.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */
#define TOTAL_PARAMETERS 78

volatile int button_debounce_timer[4] = {0, 0, 0, 0}; // Debounce timers for 4 buttons
volatile char buttonFlagForward = 0; // forward button flag
volatile char buttonFlagBackward = 0; // backward button flag
volatile char buttonFlagCancel = 0; // cancel button flag
volatile char buttonFlagConfirm = 0; // confirm button flag
volatile uint32_t inactivity_timer = 0;

typedef struct {
//  ********   global_variables    ********** 
 int tempratura;  
signed int r01_Differencial;
signed int r02_Maksimalnoe_ogranichenie_ustavki;
signed int r03_Minimalnoe_ogranichenie_ustavki;
signed int r04_Edinica_izmereniya_temperatury_C_F;
signed int r05_Korrekciya_pokazanii_temperatury;  
signed int r09_Korrekciya_signala_s_Sair;
int r12_Ruchnoe_upravlenie_ostanovka_regulirovaniya_pusk_regulirovaniya;
signed int r13_Smeschenie_ustavki_vo_vremya_nochnogo_rejima_raboty;
signed int r39_Vklyuchenie_smescheniya_ustavki;
signed int r40_Velichina_smescheniya_ustavki;



// ******* Avarii ****//

signed int A03_Zaderjka_avariinogo_signala_temperaturi;
signed int A04_Zaderjka_avariinogo_signala_dveri;
signed int A12_Zaderjka_avariinogo_signala_temperatury_pri_nachale_ohlajdeniya;
signed int A13_Nijnii_predel_avariinogo_signala;
signed int A14_Verhnii_predel_avariinogo_signala;
signed int A27_Zaderjka_avariinogo_signala_DI;
signed int A37_Avariinyi_verhnii_predel_dlya_temperatury_kondensatora;


// ******* Kompressor ****//

signed int c01_Min_vremya_raboty;
signed int c02_Min_vremya_stoyanki;
signed int c30_Rele_kompressora_doljno_vklyuchatsya_i_vyklyuchatsya_inversno;
signed int c70_Vneshnie_rele;

// ******* Ottaika ****// 
signed int d01_Temperatura_ostanovki_ottaiki;
signed int d02_Sposob_ottaiki; 
signed int d03_Maksimalnaya_dlitelnost_ottaiki;
signed int d04_Interval_mejdu_zapuskami_ottaiki;
signed int d05_Smeschenie_vklyucheniya_ottaiki_vo_vremya_zapuska;
signed int d06_Zaderjka_zapuska_ventilyatora_posle_ottaiki;
signed int d07_Vremya_kapleobrazovaniya;
signed int d08_Temperatura_nachala_raboty_ventilyatora;
signed int d09_Datchik_ottaiki;
signed int d10_Rabota_ventilyatora_vo_vremya_ottaiki;
signed int d18_Maksimalnoe_summarnoe_vremya_ohlajdeniya_mejdu_dvumya_ottaikami;
signed int d19_Ottaika_po_neobhodimosti; //_ _dopustimye_kolebaniya_temperatury_S5_pri_obmerzanii._Na_centralizovannoi_ustanovke_vyberite_20K_(=_Off)


// ******* Ventilyator ****//

signed int F01_Ostanovka_ventilyatora_pri_otklyuchenii_kompressora;
signed int F02_Zaderjka_ventilyatora_pri_ostanovke_kompressora;
signed int F04_Temperatura_ostanovki_ventilyatora;



//chasi realnogo vremeni

signed int t01_nastroika_chasov;
signed int t02_nastroika_chasov;
signed int t03_nastroika_chasov;
signed int t04_nastroika_chasov;
signed int t05_nastroika_chasov;
signed int t06_nastroika_chasov;
signed int t11_nastroika_minut;
signed int t12_nastroika_minut;
signed int t13_nastroika_minut;
signed int t14_nastroika_minut;
signed int t15_nastroika_minut;
signed int t16_nastroika_minut;
signed int t07_ustanovka_chasov;
signed int t08_ustanovka_minut;
signed int t45_ustanovka_dati;
signed int t46_ustanovka_myasitsa;
signed int t47_ustanovka_goda;









// ******* Raznoe ****// 

signed int o01_Zaderjka_vyhodnogo_signala_posle_zapuska;
signed int o01_Setevoi_adres;
signed int o06_Servisnoe_soobschenie;
signed int o04_Parol_1;
signed int o05_Ispolzuemyi_tip_datchika;
signed int o15_Delenie_displeya;
signed int o16_Maksimalnoe_vremya_ojidaniya_posle_koordinirovannoi_ottaiki;
signed int o38_Konfiguraciya_funkcii_osvescheniya;
signed int o39_Ruchnoe_vklyuchenie_rele_osvescheniya;


// *******  Obsluzhivanie****//  


signed int u10_Status_vhoda_DI;
signed int u13_Status_nochnogo_rejima;
signed int u28_Schitat_tekuschuyu_nastroiku_regulirovaniya;
signed int u58_Sostoyanie_rele_ohlajdeniya;
signed int u60_Sostoyanie_rele_ventilyatora;
signed int u59_Sostoyanie_rele_ottaiki;

signed int u71_Sostoyanie_rele_4;


signed int u69_Temperatura_izmerennaya_datchikom_Sair;
signed int u09_Temperatura_izmerennaya_datchikom_S5;
signed int Temperatura;
signed int Ustavka;
                    
 
//**************** Flags  ***************** 

char fl_Kompressor_On;
char fl_Ventilator_On;

char fl_Ventilator_Off;

char fl_Button_Ok_On;
char fl_Button_Ok_Long_On;
char fl_Button_Cancel_On;
char fl_Button_Up_On;
char fl_Button_Down_On;



//**************** Systick Flags ***************** 

char fl_SysTick_Main_Cycle; 
char fl_SysTick_Vetilator;
//char fl_SysTick_LEDs;
//char fl_SysTick_TCP232;
//char fl_SysTick_RS485;
//char fl_SysTick_ADC;   


}  Settings;

Settings currentSettings;
const Settings factorySettings = {
	2,
	2,
	50,
	-50,
	0,
	0,
	0,
	1,
	0,
	0,
	0,
	30,
	60,
	90,
	8,
	-30,
	30,
	50,
	0,
	0,
	0,
	1,
	2,
	6,
	8,
	45,
	0,
	0,
	0,
	-5,
	1,
	0,
	0,
	20,
	0,
	0,
	50,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1,
	1,
	0,
	5,
	0,
	0,
	0,
	0,
	0,
	0,
	20,
	1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	2
	
	
	
};

void factoryReset(){
	
	
	 currentSettings = factorySettings; 
	
	
}



enum MenuLevel {
    MAIN_SCREEN = 0,
    PARAMETER_SELECTION = 1,
    PARAMETER_ADJUSTMENT = 2
};

static enum MenuLevel menu_position = MAIN_SCREEN;
static int selected_parameter = 0;  // Tracks which parameter you're adjusting


/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_TIM3_Init(void);
void Error_Handler(void);
void debounce_buttons(void);

//void TimerHandler() {
//    static int lastUpdateTime = 0;
//  if ((HAL_GetTick() - lastUpdateTime) >= 1) {
//        SevenSegment(temprature); // Call with updated temperature value
//        lastUpdateTime = HAL_GetTick();
//    }
//}  
 int temprature=-335; 
void handle_menu_logic(void) {
    switch (menu_position) {
        
    case MAIN_SCREEN:
        // Display the temperature
        temprature_display(temprature);

        // Check navigation buttons
            if (buttonFlagForward || buttonFlagBackward) {
         menu_position = PARAMETER_SELECTION;
         selected_parameter = 0;
         buttonFlagForward = 0;
         buttonFlagBackward = 0;
     }
						if(buttonFlagConfirm){
			      buttonFlagConfirm=0;
		}
		 if (buttonFlagCancel) {
            
			       buttonFlagCancel=0;
        }
     

				
				
        break;

    case PARAMETER_SELECTION:
//        // Display parameter index and value
//        display_parameter(selected_parameter, get_parameter_value(selected_parameter));
		       parameter_display(selected_parameter);
		
		 if (buttonFlagForward){
            selected_parameter++;
		  buttonFlagForward = 0;
		 }
        else if (buttonFlagBackward)
				{
            selected_parameter--;
					  buttonFlagBackward = 0;
					
				}
				 if (selected_parameter < 0) selected_parameter = 0;
         if (selected_parameter >= TOTAL_PARAMETERS) selected_parameter = TOTAL_PARAMETERS - 1;
		if(buttonFlagConfirm){
			 menu_position = PARAMETER_ADJUSTMENT;
			buttonFlagConfirm=0;
		}
		else if (buttonFlagCancel) {
            menu_position = MAIN_SCREEN;
			       buttonFlagCancel=0;
        }
		 break;
//        // Navigation
//        if (buttonFlagForward)
//            selected_parameter++;
//        else if (buttonFlagBackward)
//            selected_parameter--;

//        // Constrain parameter selection
//        if (selected_parameter < 0) selected_parameter = 0;
//        if (selected_parameter >= TOTAL_PARAMETERS) selected_parameter = TOTAL_PARAMETERS - 1;

//        // Confirm selection
//        if (buttonFlagConfirm) {
//            menu_position = PARAMETER_ADJUSTMENT;
//        }
//        // Cancel to return to main screen
//        else if (buttonFlagCancel) {
//            menu_position = MAIN_SCREEN;
//        }
       

    case PARAMETER_ADJUSTMENT:
			
			parameter_adjust_and_display(selected_parameter,3);
		
			if(buttonFlagForward)
			{
				parameter_adjust_and_display(selected_parameter,0);
				buttonFlagForward=0;
				
			}
			if(buttonFlagBackward)
			{
				parameter_adjust_and_display(selected_parameter,1);
				buttonFlagBackward=0;
				
			}
			if(buttonFlagConfirm)
			{
				parameter_adjust_and_display(selected_parameter,2);
				buttonFlagConfirm=0;
						
			}
			
			if (buttonFlagCancel)
			{
				menu_position = MAIN_SCREEN;
			  buttonFlagCancel=0;
				
				
			}
				
			
//			temprature=+1;
//		temprature_display(temprature);
//		menu_position=MAIN_SCREEN;
		
		break;
		
        // Display current selected parameter value
//        display_parameter(selected_parameter, get_parameter_value(selected_parameter));

//        // Adjust value with forward/backward
//        if (buttonFlagForward)
//       //     adjust_parameter_value(selected_parameter, 1);  // Increment
//        else if (buttonFlagBackward)
//            adjust_parameter_value(selected_parameter, -1);  // Decrement

        // Confirm change or cancel
//        if (buttonFlagConfirm) {
//            save_parameter_value(selected_parameter);  // Save the new value
//            menu_position = PARAMETER_SELECTION;  // Return to selection
//        } else if (buttonFlagCancel) {
//            menu_position = PARAMETER_SELECTION;  // Cancel adjustment
//        }
		default:
			menu_position= MAIN_SCREEN;
        
    }
}



/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */
//int  counter = 0;
//static int temperature = 123; // Example temperature to display
//static int parameter = 1;    // Example custom parameter
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */ 
	
	     // Example number to display
    // Pass the number to be displayed // Prepare digits of initial number

  while (1)
  {
			

	
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */


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

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
 void MX_TIM3_Init(void)
{

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);  // Enable TIM3 clock
    LL_TIM_SetPrescaler(TIM3, 3999);  // Prescaler value
    LL_TIM_SetAutoReload(TIM3, 1);  // Auto-reload value (period)
    LL_TIM_SetCounterMode(TIM3, LL_TIM_COUNTERMODE_UP);  // Counter mode
    LL_TIM_SetClockDivision(TIM3, LL_TIM_CLOCKDIVISION_DIV1);  // Clock division
    LL_TIM_EnableIT_UPDATE(TIM3);  // Enable update interrupt
    LL_TIM_EnableCounter(TIM3);  // Start the timer
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
 void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED1_Pin|LED2_Pin|LED4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED4_Pin */
 GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED3_Pin */
//   GPIO_InitStruct.Pin = LED3_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Output mode
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED3_GPIO_Port, &GPIO_InitStruct);

 

  /*Configure GPIO pins : PB0 PB1 PB2 PB10
                           PB11 PB3 PB4 PB5
                           PB6 PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

 /*Configure GPIO pins : S1_Pin S3_Pin S2_Pin S4_Pin */
  GPIO_InitStruct.Pin = S1_Pin|S3_Pin|S2_Pin|S4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;  // Set all button pins to interrupt mode
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
     HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);  // Higher priority
   HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0); // Lower priority
     HAL_NVIC_EnableIRQ(TIM3_IRQn);

  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
