/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled_init.h"
#include "button.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//uint8_t buff[1024];
const uint8_t danji[][32] ={
0xE7,0xE7,0xCF,0xF3,0x9F,0xF9,0x03,0xC0,0x73,0xCE,0x73,0xCE,0x03,0xC0,0x73,0xCE,
0x73,0xCE,0x03,0xC0,0x7F,0xFE,0x7F,0xFE,0x00,0x00,0x7F,0xFE,0x7F,0xFE,0x7F,0xFE,/*"单",0*/
0x7F,0xFE,0x7F,0xFE,0x7F,0xFE,0x03,0xC0,0x7F,0xFE,0x7F,0xFE,0x7F,0xFE,0x00,0x00,
0x7F,0xFE,0x7F,0xFE,0x73,0xCE,0x73,0xCE,0x73,0xCE,0x73,0xCE,0x03,0xC0,0xFF,0xCF,/*"击",1*/
};
const uint8_t changan[][32] ={
0xCF,0xFF,0xCF,0xE7,0xCF,0xF3,0xCF,0xF9,0xCF,0xFC,0x4F,0xFE,0xCF,0xFF,0x00,0x00,
0x0F,0xFF,0x4F,0xFE,0xCF,0xFC,0xCF,0xF9,0x4F,0xF2,0x0F,0xC7,0x8F,0x1F,0xCF,0xFF,/*"长",0*/
0xE7,0xF9,0xE7,0xF3,0xE7,0xF3,0x27,0x00,0x00,0x3F,0x87,0x99,0xE7,0xF9,0x07,0x00,
0xE3,0xCC,0xE0,0xCC,0x67,0xCE,0xE7,0xE0,0xE7,0xF3,0xE7,0xE1,0xE1,0xCC,0x33,0x9E,/*"按",1*/
};
const uint8_t qiehuan[][28] ={
0x04,0x00,0xC4,0x1F,0x04,0x11,0x04,0x11,0x3C,0x11,0x07,0x11,0x04,0x11,0x04,0x11,
0x24,0x11,0x94,0x10,0x8C,0x10,0x44,0x10,0x20,0x10,0x10,0x0C,/*"切",0*/
0x44,0x00,0x44,0x00,0xC4,0x07,0x2F,0x04,0x14,0x02,0xE4,0x0F,0x24,0x09,0x2C,0x09,
0x27,0x09,0xF4,0x1F,0x84,0x02,0x44,0x04,0x24,0x08,0x17,0x10,/*"换",1*/
};
const uint8_t shuchu[][28] ={
0x02,0x01,0x82,0x02,0x4F,0x04,0x22,0x08,0xD1,0x17,0x05,0x00,0xEF,0x11,0x24,0x15,
0xE4,0x15,0x2C,0x15,0xE7,0x15,0x24,0x15,0x24,0x11,0xA4,0x1D,/*"输",0*/
0x40,0x00,0x42,0x08,0x42,0x08,0x42,0x08,0x42,0x08,0xFE,0x0F,0x40,0x00,0x40,0x00,
0x41,0x10,0x41,0x10,0x41,0x10,0x41,0x10,0xFF,0x1F,0x00,0x10,/*"出",1*/
};
const uint8_t dianya[][30] ={
0xFF,0xFF,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0x03,0xF8,0xBB,0xFB,0x03,0xF8,0xBB,0xFB,
0xBB,0xFB,0x03,0xF8,0xBB,0xFB,0xBF,0xEF,0xBF,0xEF,0x3F,0xE0,0xFF,0xFF,/*"电",0*/
0xFF,0xFF,0xFB,0xF7,0x03,0xF8,0x7B,0xFF,0x7B,0xFF,0x7B,0xFF,0x7B,0xFF,0x1B,0xF8,
0x7B,0xFF,0x7B,0xF9,0x7B,0xFB,0x7B,0xFF,0x79,0xEF,0x87,0xF0,0xFF,0xFF,/*"压",1*/
};
const uint8_t dianliu[][30] ={
0xFF,0xFF,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0x03,0xF8,0xBB,0xFB,0x03,0xF8,0xBB,0xFB,
0xBB,0xFB,0x03,0xF8,0xBB,0xFB,0xBF,0xEF,0xBF,0xEF,0x3F,0xE0,0xFF,0xFF,/*"电",0*/
0xFF,0xFF,0xFF,0xFE,0x33,0xF0,0xF7,0xFE,0x7F,0xFF,0xAD,0xF3,0x33,0xF4,0xD7,0xFB,
0xD7,0xFA,0xDB,0xFA,0xD9,0xFA,0xDB,0xFA,0xEB,0xEA,0xE3,0xE3,0xFF,0xFF,/*"流",1*/
};
const uint8_t gonglv[][30] ={
0xFF,0xFF,0xFF,0xFD,0xFF,0xFD,0x81,0xFD,0xF7,0xED,0x77,0xE0,0xF7,0xEE,0xF7,0xEE,
0xF7,0xEE,0x97,0xEE,0x63,0xF7,0x7D,0xF7,0xBF,0xF7,0xDF,0xF1,0xFF,0xFF,/*"功",0*/
0xFF,0xFF,0xBF,0xFF,0x43,0xF0,0xBF,0xFF,0xDB,0xF6,0x07,0xFB,0x7F,0xFF,0xB7,0xFB,
0x0B,0xF6,0x7F,0xFF,0x03,0xE0,0xBF,0xFF,0xBF,0xFF,0xBF,0xFF,0xFF,0xFF,/*"率",1*/
};
//const  uint8_t ONF[][64] ={
// 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0xF8,0x8F,0xE3,
// 0xC7,0xC7,0xE7,0xCF,0xE3,0x8F,0xE3,0x8F,0xF1,0x9F,0xF1,0x1F,0xF1,0x1F,0xF1,0x1F,
// 0xF1,0x1F,0xF1,0x1F,0xF1,0x1F,0xF1,0x1F,0xF1,0x1F,0xF3,0x1F,0xE3,0x8F,0xE3,0x8F,
// 0xE7,0xCF,0xC7,0xC7,0x8F,0xE3,0x3F,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,/*"O",0*/
// 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x07,0xC3,0x9F,
// 0xC3,0x9F,0x83,0x9F,0x83,0x9F,0x13,0x9F,0x13,0x9F,0x33,0x9E,0x33,0x9E,0x73,0x9C,
// 0x73,0x9C,0xF3,0x98,0xF3,0x98,0xF3,0x91,0xF3,0x91,0xF3,0x83,0xF3,0x83,0xF3,0x87,
// 0xF3,0x87,0xF3,0x8F,0xF3,0x8F,0xC0,0x9F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,/*"N",1*/
// 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,0x80,0xC7,0x87,
// 0xC7,0x9F,0xC7,0x3F,0xC7,0x3F,0xC7,0xFF,0xC7,0xFF,0xC7,0xE7,0xC7,0xE7,0xC7,0xE3,
// 0x07,0xE0,0xC7,0xE3,0xC7,0xE7,0xC7,0xE7,0xC7,0xE7,0xC7,0xFF,0xC7,0xFF,0xC7,0xFF,
// 0xC7,0xFF,0xC7,0xFF,0xC7,0xFF,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,/*"F",2*/
// };
const uint8_t ONF[][64] ={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x07,0x70,0x1C,
0x38,0x38,0x18,0x30,0x1C,0x70,0x1C,0x70,0x0E,0x60,0x0E,0xE0,0x0E,0xE0,0x0E,0xE0,
0x0E,0xE0,0x0E,0xE0,0x0E,0xE0,0x0E,0xE0,0x0E,0xE0,0x0C,0xE0,0x1C,0x70,0x1C,0x70,
0x18,0x30,0x38,0x38,0x70,0x1C,0xC0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"O",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xF8,0x3C,0x60,
0x3C,0x60,0x7C,0x60,0x7C,0x60,0xEC,0x60,0xEC,0x60,0xCC,0x61,0xCC,0x61,0x8C,0x63,
0x8C,0x63,0x0C,0x67,0x0C,0x67,0x0C,0x6E,0x0C,0x6E,0x0C,0x7C,0x0C,0x7C,0x0C,0x78,
0x0C,0x78,0x0C,0x70,0x0C,0x70,0x3F,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"N",1*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x7F,0x38,0x78,
0x38,0x60,0x38,0xC0,0x38,0xC0,0x38,0x00,0x38,0x00,0x38,0x18,0x38,0x18,0x38,0x1C,
0xF8,0x1F,0x38,0x1C,0x38,0x18,0x38,0x18,0x38,0x18,0x38,0x00,0x38,0x00,0x38,0x00,
0x38,0x00,0x38,0x00,0x38,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"F",2*/
};
const uint8_t danwei[][14] ={
// 0xFF,0xFF,0xFF,0x18,0xBD,0xBD,0xDD,0xDB,0xDB,0xEB,0xEB,0xE7,0xF7,0xF7,0xFF,0xFF,/*"V",0*/
// 0xFF,0xFF,0xFF,0xF7,0xF7,0xE7,0xEB,0xEB,0xDB,0xC3,0xDD,0xBD,0xBD,0x18,0xFF,0xFF,/*"A",1*/
// 0xFF,0xFF,0xFF,0x94,0xD5,0xD5,0xD5,0xD5,0xD5,0xC9,0xEB,0xEB,0xEB,0xEB,0xFF,0xFF,/*"W",2*/
0x00,0x00,0x00,0x77,0x22,0x22,0x14,0x14,0x14,0x14,0x08,0x08,0x00,0x00,/*"V",0*/
0x00,0x00,0x00,0x08,0x08,0x14,0x14,0x14,0x1C,0x22,0x22,0x77,0x00,0x00,/*"A",1*/
0x00,0x00,0x00,0x6B,0x2A,0x2A,0x2A,0x2A,0x36,0x14,0x14,0x14,0x00,0x00,/*"W",2*/
};
const uint8_t shuzi[][12] ={
0x00,0x00,0x0E,0x11,0x11,0x11,0x11,0x11,0x11,0x0E,0x00,0x00,/*"0",0*/
0x00,0x00,0x04,0x06,0x04,0x04,0x04,0x04,0x04,0x0E,0x00,0x00,/*"1",1*/
0x00,0x00,0x0E,0x11,0x11,0x08,0x04,0x02,0x01,0x1F,0x00,0x00,/*"2",2*/
0x00,0x00,0x0E,0x11,0x10,0x0C,0x10,0x10,0x11,0x0E,0x00,0x00,/*"3",3*/
0x00,0x00,0x08,0x0C,0x0C,0x0A,0x09,0x1F,0x08,0x1C,0x00,0x00,/*"4",4*/
0x00,0x00,0x1F,0x01,0x01,0x0F,0x11,0x10,0x11,0x0E,0x00,0x00,/*"5",5*/
0x00,0x00,0x0C,0x12,0x01,0x0D,0x13,0x11,0x11,0x0E,0x00,0x00,/*"6",6*/
0x00,0x00,0x1E,0x10,0x08,0x08,0x04,0x04,0x04,0x04,0x00,0x00,/*"7",7*/
0x00,0x00,0x0E,0x11,0x11,0x0E,0x11,0x11,0x11,0x0E,0x00,0x00,/*"8",8*/
0x00,0x00,0x0E,0x11,0x11,0x19,0x16,0x10,0x09,0x06,0x00,0x00,/*"9",9*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,/*".",10*/
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
KEY_HandleTypeDef key;
volatile uint32_t tim6Tick = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM6){
		tim6Tick ++;
		if(tim6Tick % 4 == 0){
			KEY_Scan(&key);
		}
    }
}
unsigned  int millis(void){
	return 10 * tim6Tick;
}
unsigned  int micros(void){
	return 10000 * tim6Tick;
}
void setCh224kVoltage(int v){
  switch(v){
    case 5:
    	HAL_GPIO_WritePin(CH224K_CFG1_GPIO_Port, CH224K_CFG1_Pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(CH224K_CFG2_GPIO_Port, CH224K_CFG2_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(CH224K_CFG3_GPIO_Port, CH224K_CFG3_Pin, GPIO_PIN_RESET);
    	break;
    case 9:
		HAL_GPIO_WritePin(CH224K_CFG1_GPIO_Port, CH224K_CFG1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CH224K_CFG2_GPIO_Port, CH224K_CFG2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CH224K_CFG3_GPIO_Port, CH224K_CFG3_Pin, GPIO_PIN_RESET);
		break;
    case 12:
		HAL_GPIO_WritePin(CH224K_CFG1_GPIO_Port, CH224K_CFG1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CH224K_CFG2_GPIO_Port, CH224K_CFG2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CH224K_CFG3_GPIO_Port, CH224K_CFG3_Pin, GPIO_PIN_SET);
		break;
    case 15:
		HAL_GPIO_WritePin(CH224K_CFG1_GPIO_Port, CH224K_CFG1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CH224K_CFG2_GPIO_Port, CH224K_CFG2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CH224K_CFG3_GPIO_Port, CH224K_CFG3_Pin, GPIO_PIN_SET);
		break;
    case 20:
		HAL_GPIO_WritePin(CH224K_CFG1_GPIO_Port, CH224K_CFG1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CH224K_CFG2_GPIO_Port, CH224K_CFG2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CH224K_CFG3_GPIO_Port, CH224K_CFG3_Pin, GPIO_PIN_RESET);
		break;
  }
}
void powerSwitch(int sta){
  if(sta == 1) HAL_GPIO_WritePin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin, GPIO_PIN_SET);
  else if(sta == 0) HAL_GPIO_WritePin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin, GPIO_PIN_RESET);
  else if(sta == -1){
    if(HAL_GPIO_ReadPin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin))
    	HAL_GPIO_WritePin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin, GPIO_PIN_RESET);
    else HAL_GPIO_WritePin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin, GPIO_PIN_SET);
  }
}
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
  MX_ADC_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  setCh224kVoltage(5);
  powerSwitch(0);
  key.KeyId = Gpio_pin2Index(GPIOB, GPIO_PIN_12);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	u8g2_t u8g2;
	u8g2Init(&u8g2);
	int show_len = 7;
	char buf[20]={"\0"};
	char *str1;
	//HAL_ADCEx_Calibration_Start(&hadc);
	const int CFG_VOLTAGE[5] = {5, 9, 12, 15, 20};
	int cfg_index = 0;
	int adc_v=0, adc_c=0;
	float volage=0, current=0, power=0;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	uint32_t t = millis();

	/** 按键事件处理 */
	KEY_FsmTypeDef fsm = KEY_Get_Fsm(&key);
	if(fsm == KEY_FSM_SHORT_PRESS){
		powerSwitch(0);
		if(cfg_index >= 4) cfg_index = 0;
		else cfg_index++;
		setCh224kVoltage(CFG_VOLTAGE[cfg_index]);
		printf("KEY_FSM_SHORT_PRESS\n");
	}else if(fsm == KEY_FSM_DOUBLE_PRESS){
		powerSwitch(0);
		if(cfg_index <= 0) cfg_index = 4;
		else cfg_index--;
		setCh224kVoltage(CFG_VOLTAGE[cfg_index]);
		printf("KEY_FSM_DOUBLE_PRESS\n");
	}else if(fsm == KEY_FSM_LONG_PRESS){
		powerSwitch(-1);
		printf("KEY_FSM_LONG_PRESS\n");
	}

   /** ADC采集转换 */
	HAL_ADCEx_Calibration_Start(&hadc);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 50);
	adc_c = 0.6f * adc_c + 0.4f * HAL_ADC_GetValue(&hadc);
	HAL_ADCEx_Calibration_Start(&hadc);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 50);
	adc_v = 0.5f * adc_v + 0.5f * HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);
    volage = ((3.3f * adc_v / 4096.0)) * 10.1f;
    current = (3.3f * adc_c / 4096.0) * 2.0f;
    power = volage * current;

    /** Oled显示 */
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawXBM(&u8g2,0,0,16,16,danji[0]);
    u8g2_DrawXBM(&u8g2,16,0,16,16,danji[1]);
    u8g2_DrawXBM(&u8g2,66,0,16,16,changan[0]);
    u8g2_DrawXBM(&u8g2,82,0,16,16,changan[1]);
    u8g2_DrawXBM(&u8g2,34,2,14,14,qiehuan[0]);
    u8g2_DrawXBM(&u8g2,48,2,14,14,qiehuan[1]);
    u8g2_DrawXBM(&u8g2,100,2,14,14,shuchu[0]);
    u8g2_DrawXBM(&u8g2,114,2,14,14,shuchu[1]);

	u8g2_DrawXBM(&u8g2,0,16,15,15,dianya[0]);
	u8g2_DrawXBM(&u8g2,15,16,15,15,dianya[1]);
	u8g2_DrawXBM(&u8g2,0,33,15,15,dianliu[0]);
	u8g2_DrawXBM(&u8g2,15,33,15,15,dianliu[1]);
	u8g2_DrawXBM(&u8g2,0,50,15,15,gonglv[0]);
	u8g2_DrawXBM(&u8g2,15,50,15,15,gonglv[1]);

	u8g2_DrawXBM(&u8g2,70,16,7,14,danwei[0]);
	u8g2_DrawXBM(&u8g2,70,33,7,14,danwei[1]);
	u8g2_DrawXBM(&u8g2,70,50,7,14,danwei[2]);

	int w = 6;
	int h = 12;
	buf[0] = '0';buf[1] = '.';buf[2] = '0';buf[3] = '0';buf[4] = '0';buf[5] = '0';buf[6] = '0';buf[7] = '0';
	if(volage > 0.001) str1 = gcvt(volage, show_len, buf);
	for(int i=0;i<5;i++){
		if(buf[i] != '.')
			u8g2_DrawXBM(&u8g2,36 + w*i, 16, w, h, shuzi[buf[i] - '0']);
		else u8g2_DrawXBM(&u8g2,36 + w*i, 16, w, h, shuzi[10]);
	}
	buf[0] = '0';buf[1] = '.';buf[2] = '0';buf[3] = '0';buf[4] = '0';buf[5] = '0';buf[6] = '0';buf[7] = '0';
	if(current > 0.001) str1 = gcvt(current, show_len, buf);
	for(int i=0;i<5;i++){
		if(buf[i] != '.')
			u8g2_DrawXBM(&u8g2,36 + w*i, 33, w, h, shuzi[buf[i] - '0']);
		else u8g2_DrawXBM(&u8g2,36 + w*i, 33, w, h, shuzi[10]);
	}
	buf[0] = '0';buf[1] = '.';buf[2] = '0';buf[3] = '0';buf[4] = '0';buf[5] = '0';buf[6] = '0';buf[7] = '0';
	if(power > 0.001) str1 = gcvt(power, show_len, buf);
	for(int i=0;i<5;i++){
		if(buf[i] != '.')
			u8g2_DrawXBM(&u8g2,36 + w*i, 50, w, h, shuzi[buf[i] - '0']);
		else u8g2_DrawXBM(&u8g2,36 + w*i, 50, w, h, shuzi[10]);
	}

	if(HAL_GPIO_ReadPin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin)){
		 u8g2_DrawXBM(&u8g2,88,24,16,32,ONF[0]);
		 u8g2_DrawXBM(&u8g2,104,24,16,32,ONF[1]);
	}else {
		u8g2_DrawXBM(&u8g2,82,24,16,32,ONF[0]);
		u8g2_DrawXBM(&u8g2,98,24,16,32,ONF[2]);
		u8g2_DrawXBM(&u8g2,113,24,16,32,ONF[2]);
	}
	u8g2_SendBuffer(&u8g2);

//	u8g2_ClearBuffer(&u8g2);
//	//u8g2_SetFont(&u8g2, u8g2_font_unifont_t_chinese1);
//	u8g2_SetFont(&u8g2, u8g2_font_DigitalDiscoThin_tu);
//	//u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_gb2312);
//	u8g2_DrawStr(&u8g2, 0, 14, "click");
//	u8g2_DrawStr(&u8g2, 64, 14, "long");
//	u8g2_DrawStr(&u8g2, 0, 22, "----------------------------");
//	//u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_gb2312);
//	u8g2_DrawStr(&u8g2, 32, 14, ":sw");
//	u8g2_DrawStr(&u8g2, 96, 14, ":out");
//	//u8g2_SetFont(&u8g2, u8g2_font_wqy14_t_gb2312);
//	u8g2_DrawStr(&u8g2, 0, 32, "vol:");
//	str1 = gcvt(volage, show_len, buf);
//	u8g2_DrawStr(&u8g2, 35, 32, str1);
//	u8g2_DrawStr(&u8g2, 66, 32, " V");
//	u8g2_DrawStr(&u8g2, 0, 48, "cur:");
//	str1 = gcvt(current, show_len, buf);
//	u8g2_DrawStr(&u8g2, 35, 48, str1);
//	u8g2_DrawStr(&u8g2, 65, 48, " A");
//
//	u8g2_DrawStr(&u8g2, 0, 64, "pow:");
//	str1 = gcvt(power, show_len, buf);
//	u8g2_DrawStr(&u8g2, 35, 64, str1);
//	u8g2_DrawStr(&u8g2, 64, 64, " W");
//
//	//u8g2_SetFont(&u8g2, u8g2_font_logisoso24_tr);
//	if(HAL_GPIO_ReadPin(POWER_SWITCH_GPIO_Port, POWER_SWITCH_Pin))
//		u8g2_DrawStr(&u8g2, 88, 52, "ON");
//	else u8g2_DrawStr(&u8g2, 82, 52, "OFF");
//	u8g2_SendBuffer(&u8g2);

	 printf("%d \n",millis() - t);
	 HAL_Delay(10);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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

