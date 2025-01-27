/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_0
#define LED2_GPIO_Port GPIOF
#define LED3_Pin GPIO_PIN_1
#define LED3_GPIO_Port GPIOF
#define LED4_Pin GPIO_PIN_0
#define LED4_GPIO_Port GPIOA
#define BUTTON4_Pin GPIO_PIN_1
#define BUTTON4_GPIO_Port GPIOB
#define BUTTON4_EXTI_IRQn EXTI0_1_IRQn
#define BUTTON2_Pin GPIO_PIN_2
#define BUTTON2_GPIO_Port GPIOB
#define BUTTON2_EXTI_IRQn EXTI2_3_IRQn
#define SHCP_Pin GPIO_PIN_12
#define SHCP_GPIO_Port GPIOB
#define STCP_Pin GPIO_PIN_13
#define STCP_GPIO_Port GPIOB
#define DS_Pin GPIO_PIN_15
#define DS_GPIO_Port GPIOB
#define BUTTON1_Pin GPIO_PIN_6
#define BUTTON1_GPIO_Port GPIOB
#define BUTTON1_EXTI_IRQn EXTI4_15_IRQn
#define BUTTON3_Pin GPIO_PIN_7
#define BUTTON3_GPIO_Port GPIOB
#define BUTTON3_EXTI_IRQn EXTI4_15_IRQn

/* USER CODE BEGIN Private defines */
void toggle(void);
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/



/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void debounce_buttons(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

extern volatile int button_debounce_timer[4]; 
extern volatile char buttonFlagForward; 
extern volatile char buttonFlagBackward;
extern volatile char buttonFlagCancel;
extern volatile char buttonFlagConfirm;
extern volatile uint32_t inactivity_timer ;

//  ********   global_variables    ********** 

extern signed int r01_Differencial;
extern signed int r02_Maksimalnoe_ogranichenie_ustavki;
extern signed int r03_Minimalnoe_ogranichenie_ustavki;
extern signed int r04_Edinica_izmereniya_temperatury_C_F;  
extern signed int r05_Korrekciya_pokazanii_temperatury;
extern signed int r09_Korrekciya_signala_s_Sair;
extern signed int r12_Ruchnoe_upravlenie_ostanovka_regulirovaniya_pusk_regulirovaniya;
extern signed int r13_Smeschenie_ustavki_vo_vremya_nochnogo_rejima_raboty;
extern signed int r39_Vklyuchenie_smescheniya_ustavki;
extern signed int r40_Velichina_smescheniya_ustavki;

// ******* Avarii ****//

extern signed int A03_Zaderjka_avariinogo_signala_temperaturi;
extern signed int A04_Zaderjka_avariinogo_signala_dveri;
extern signed int A12_Zaderjka_avariinogo_signala_temperatury_pri_nachale_ohlajdeniya;
extern signed int A13_Nijnii_predel_avariinogo_signala;
extern signed int A14_Verhnii_predel_avariinogo_signala;
extern signed int A27_Zaderjka_avariinogo_signala_DI;
extern signed int A37_Avariinyi_verhnii_predel_dlya_temperatury_kondensatora;

// ******* Kompressor ****//

extern signed int c01_Min_vremya_raboty;
extern signed int c02_Min_vremya_stoyanki;
extern signed int c30_Rele_kompressora_doljno_vklyuchatsya_i_vyklyuchatsya_inversno;
extern signed int c70_Vneshnie_rele;


// ******* Ottaika ****// 
extern signed int d01_Temperatura_ostanovki_ottaiki;
extern signed int d02_Sposob_ottaiki; 
extern signed int d03_Maksimalnaya_dlitelnost_ottaiki;
extern signed int d04_Interval_mejdu_zapuskami_ottaiki;
extern signed int d05_Smeschenie_vklyucheniya_ottaiki_vo_vremya_zapuska;
extern signed int d06_Zaderjka_zapuska_ventilyatora_posle_ottaiki;
extern signed int d07_Vremya_kapleobrazovaniya;
extern signed int d08_Temperatura_nachala_raboty_ventilyatora;
extern signed int d09_Datchik_ottaiki;
extern signed int d10_Rabota_ventilyatora_vo_vremya_ottaiki;
extern signed int d18_Maksimalnoe_summarnoe_vremya_ohlajdeniya_mejdu_dvumya_ottaikami;
extern signed int d19_Ottaika_po_neobhodimosti; 
//_ _dopustimye_kolebaniya_temperatury_S5_pri_obmerzanii._Na_centralizovannoi_ustanovke_vyberite_20K_(=_Off)

// ******* Ventilyator ****//

extern signed int F01_Ostanovka_ventilyatora_pri_otklyuchenii_kompressora;
extern signed int F02_Zaderjka_ventilyatora_pri_ostanovke_kompressora;
extern signed int F04_Temperatura_ostanovki_ventilyatora;

// ******* Kompressor ****//

extern signed int c01_Min_vremya_raboty;
extern signed int c02_Min_vremya_stoyanki;
extern signed int c30_Rele_kompressora_doljno_vklyuchatsya_i_vyklyuchatsya_inversno;
extern signed int c70_Vneshnie_rele;



// ******* Ventilyator ****//

extern signed int F01_Ostanovka_ventilyatora_pri_otklyuchenii_kompressora;
extern signed int F02_Zaderjka_ventilyatora_pri_ostanovke_kompressora;
extern signed int F04_Temperatura_ostanovki_ventilyatora;

//chasi realnogo vremeni

extern signed int t01_nastroika_chasov;
extern signed int t02_nastroika_chasov;
extern signed int t03_nastroika_chasov;
extern signed int t04_nastroika_chasov;
extern signed int t05_nastroika_chasov;
extern signed int t06_nastroika_chasov;
extern signed int t11_nastroika_minut;
extern signed int t12_nastroika_minut;
extern signed int t13_nastroika_minut;
extern signed int t14_nastroika_minut;
extern signed int t15_nastroika_minut;
extern signed int t16_nastroika_minut;
extern signed int t07_ustanovka_chasov;
extern signed int t08_ustanovka_minut;
extern signed int t45_ustanovka_dati;
extern signed int t46_ustanovka_myasitsa;
extern signed int t47_ustanovka_goda;



// ******* Raznoe ****// 

extern signed int o01_Zaderjka_vyhodnogo_signala_posle_zapuska;
extern signed int o03_Setevoi_adres;
extern signed int o06_Servisnoe_soobschenie;
extern signed int o04_Parol_1;
extern signed int o05_Ispolzuemyi_tip_datchika;
extern signed int o15_Delenie_displeya;
extern signed int o16_Maksimalnoe_vremya_ojidaniya_posle_koordinirovannoi_ottaiki;
extern signed int o38_Konfiguraciya_funkcii_osvescheniya;
extern signed int o39_Ruchnoe_vklyuchenie_rele_osvescheniya;


// *******  Obsluzhivanie****//  


extern signed int u10_Status_vhoda_DI;
extern signed int u13_Status_nochnogo_rejima;
extern signed int u28_Schitat_tekuschuyu_nastroiku_regulirovaniya;
extern signed int u58_Sostoyanie_rele_ohlajdeniya;
extern signed int u60_Sostoyanie_rele_ventilyatora;
extern signed int u59_Sostoyanie_rele_ottaiki;

extern signed int u71_Sostoyanie_rele_4;


extern signed int u69_Temperatura_izmerennaya_datchikom_Sair;
extern signed int u09_Temperatura_izmerennaya_datchikom_S5;
extern signed int Temperatura; 
extern signed int Ustavka;
           

//**************** Flags  ***************** 

extern char fl_Kompressor_On;
extern char fl_Ventilator_On;


extern char fl_Ventilator_Off;


extern char fl_Button_Ok_On;
extern char fl_Button_Ok_Long_On;
extern char fl_Button_Cancel_On;
extern char fl_Button_Up_On;
extern char fl_Button_Down_On;


//**************** Systick Flags *****************

extern char fl_SysTick_Main_Cycle;
extern char fl_SysTick_Vetilator;
//extern char fl_SysTick_LEDs;  
//extern char fl_SysTick_TCP232; 
//extern char fl_SysTick_RS485;  
//extern char fl_SysTick_ADC;   

enum Message_Codes{
   r01 = 1000,
   r02,
   r03,
   r04,
};






                                                  
                                                  

void TimerHandler(void);
 void handle_menu_logic(void);
void check_inactivity(void);
void toggle (void);
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/* USER CODE END Private defines */


