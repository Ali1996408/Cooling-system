/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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


volatile char buttonFlagForward = 0; // forward button flag
volatile char buttonFlagBackward = 0; // backward button flag
volatile char buttonFlagCancel = 0; // cancel button flag
volatile char buttonFlagConfirm = 0; // confirm button flag



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
signed int o02;
signed int o03_Setevoi_adres;
signed int o06_Servisnoe_soobschenie;
signed int o04_Parol_1;
signed int o05_Ispolzuemyi_tip_datchika;
signed int o15_Delenie_displeya;
signed int o16_Maksimalnoe_vremya_ojidaniya_posle_koordinirovannoi_ottaiki;
signed int o38_Konfiguraciya_funkcii_osvescheniya;
signed int o39_Ruchnoe_vklyuchenie_rele_osvescheniya;
signed int o46;
signed int o64;
signed int o65;
signed int o66;
signed int o67;
signed int o70;
signed int o72;


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
  NVIC_SetPriority(TIM3_IRQn, 0);  // Set interrupt priority
   NVIC_EnableIRQ(TIM3_IRQn);       // Enable the TIM3 interrupt
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
tempratura=savedParameterValues[0];
		  r01_Differencial= savedParameterValues[1];
		  r02_Maksimalnoe_ogranichenie_ustavki=savedParameterValues[2];
		  r03_Minimalnoe_ogranichenie_ustavki=savedParameterValues[3];
		  r04_Edinica_izmereniya_temperatury_C_F=savedParameterValues[4];
		  r05_Korrekciya_pokazanii_temperatury=savedParameterValues[5];
		  r09_Korrekciya_signala_s_Sair=savedParameterValues[6];
		  r12_Ruchnoe_upravlenie_ostanovka_regulirovaniya_pusk_regulirovaniya=savedParameterValues[7];
		  r13_Smeschenie_ustavki_vo_vremya_nochnogo_rejima_raboty=savedParameterValues[8];
		  r39_Vklyuchenie_smescheniya_ustavki=savedParameterValues[9];
		  r40_Velichina_smescheniya_ustavki=savedParameterValues[10];
		  A03_Zaderjka_avariinogo_signala_temperaturi=savedParameterValues[11];
		  A04_Zaderjka_avariinogo_signala_dveri=savedParameterValues[12];
		  A12_Zaderjka_avariinogo_signala_temperatury_pri_nachale_ohlajdeniya=savedParameterValues[13];
		  A13_Nijnii_predel_avariinogo_signala=savedParameterValues[14];
		  A14_Verhnii_predel_avariinogo_signala=savedParameterValues[15];
		  A27_Zaderjka_avariinogo_signala_DI=savedParameterValues[16];
		  A37_Avariinyi_verhnii_predel_dlya_temperatury_kondensatora=savedParameterValues[17];
		  c01_Min_vremya_raboty=savedParameterValues[18];
		  c02_Min_vremya_stoyanki=savedParameterValues[19];
		  c30_Rele_kompressora_doljno_vklyuchatsya_i_vyklyuchatsya_inversno=savedParameterValues[20];
			c70_Vneshnie_rele=savedParameterValues[21];
			d01_Temperatura_ostanovki_ottaiki=savedParameterValues[22];
      d02_Sposob_ottaiki=savedParameterValues[23];
      d03_Maksimalnaya_dlitelnost_ottaiki=savedParameterValues[24];
      d05_Smeschenie_vklyucheniya_ottaiki_vo_vremya_zapuska=savedParameterValues[25];
      d06_Zaderjka_zapuska_ventilyatora_posle_ottaiki=savedParameterValues[26];
      d07_Vremya_kapleobrazovaniya=savedParameterValues[27];
 			d08_Temperatura_nachala_raboty_ventilyatora=savedParameterValues[28];
			d09_Datchik_ottaiki=savedParameterValues[29];
			d10_Rabota_ventilyatora_vo_vremya_ottaiki=savedParameterValues[30];
			d18_Maksimalnoe_summarnoe_vremya_ohlajdeniya_mejdu_dvumya_ottaikami=savedParameterValues[31];
			d19_Ottaika_po_neobhodimosti=savedParameterValues[32];
			F01_Ostanovka_ventilyatora_pri_otklyuchenii_kompressora=savedParameterValues[33];
			F02_Zaderjka_ventilyatora_pri_ostanovke_kompressora=savedParameterValues[34];
			F04_Temperatura_ostanovki_ventilyatora=savedParameterValues[35];
			t01_nastroika_chasov=savedParameterValues[36];
			t02_nastroika_chasov=savedParameterValues[37];
			t03_nastroika_chasov=savedParameterValues[38];
			t04_nastroika_chasov=savedParameterValues[39];
			t05_nastroika_chasov=savedParameterValues[40];
			t06_nastroika_chasov=savedParameterValues[41];
			t07_ustanovka_chasov=savedParameterValues[42];
			t08_ustanovka_minut=savedParameterValues[43];
			t11_nastroika_minut=savedParameterValues[44];
			t12_nastroika_minut=savedParameterValues[45];
			t13_nastroika_minut=savedParameterValues[46];
			t14_nastroika_minut=savedParameterValues[47];
			t15_nastroika_minut=savedParameterValues[48];
			t16_nastroika_minut=savedParameterValues[49];
			t45_ustanovka_dati=savedParameterValues[50];
			t46_ustanovka_myasitsa=savedParameterValues[51];
			t47_ustanovka_goda=savedParameterValues[52];
			o01_Zaderjka_vyhodnogo_signala_posle_zapuska=savedParameterValues[53];
			o02=savedParameterValues[54];
			o03_Setevoi_adres=savedParameterValues[55];
			o04_Parol_1=savedParameterValues[56];
			o05_Ispolzuemyi_tip_datchika=savedParameterValues[57];
			o06_Servisnoe_soobschenie=savedParameterValues[58];
			o15_Delenie_displeya=savedParameterValues[59];
			o16_Maksimalnoe_vremya_ojidaniya_posle_koordinirovannoi_ottaiki=savedParameterValues[60];
			o38_Konfiguraciya_funkcii_osvescheniya=savedParameterValues[61];
			o39_Ruchnoe_vklyuchenie_rele_osvescheniya=savedParameterValues[62];
			o46=savedParameterValues[63];
			o64=savedParameterValues[64];
			o65=savedParameterValues[65];
			o66=savedParameterValues[66];
			o67=savedParameterValues[67];
			o70=savedParameterValues[68];
			o72=savedParameterValues[69];
			
			temprature=tempratura;
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None

  */

	

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, LED2_Pin|LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|SHCP_Pin|STCP_Pin
                          |DS_Pin|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED2_Pin LED3_Pin */
  GPIO_InitStruct.Pin = LED2_Pin|LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : LED4_Pin */
  GPIO_InitStruct.Pin = LED4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON4_Pin BUTTON2_Pin BUTTON1_Pin BUTTON3_Pin */
  GPIO_InitStruct.Pin = BUTTON4_Pin|BUTTON2_Pin|BUTTON1_Pin|BUTTON3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;  // Set all button pins to interrupt mode
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 SHCP_Pin STCP_Pin
                           DS_Pin PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|SHCP_Pin|STCP_Pin
                          |DS_Pin|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
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
