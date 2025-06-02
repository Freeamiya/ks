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
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tm1637.h"
#include "sr04.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
sr04_t sr04;
uint8_t echo_flag = 0;
uint16_t rising_cnt = 0, falling_cnt = 0;
uint8_t CurrentDisplay[4];
uint8_t tm1637_Segments[8] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void display_distance_cm(uint32_t mm_distance);
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
    tm1637_Segments[0] = A_SEG;
    tm1637_Segments[1] = B_SEG;
    tm1637_Segments[2] = C_SEG;
    tm1637_Segments[3] = D_SEG;
    tm1637_Segments[4] = E_SEG;
    tm1637_Segments[5] = F_SEG;
    tm1637_Segments[6] = G_SEG;
    tm1637_Segments[7] = DP_SEG;
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
  MX_RTC_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
    HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SDO_GPIO_Port, SDO_Pin, GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//      HAL_GPIO_WritePin(SR04_TRIG_GPIO_Port, SR04_TRIG_Pin, 1);
//      HAL_Delay(5);
//      HAL_GPIO_WritePin(SR04_TRIG_GPIO_Port, SR04_TRIG_Pin, 0);
//      // 清零
//      rising_cnt = 0;
//      falling_cnt = 0;
//      echo_flag = 0;
//      __HAL_TIM_SET_COUNTER(&htim1, 0);
//      // 开始捕获
//      __HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
//      HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_3);
//      // 等待完成
//      for (uint16_t i = 0; i < 150; i++)
//      {
//          if (rising_cnt != 0 && falling_cnt != 0)
//          {
//              // 计算距离
//              // 定时器每1us计数1次，因此 距离=计数*0.34/2（毫米）
//              float distance = (falling_cnt - rising_cnt) * 0.17;
//              display_distance_cm((uint32_t)distance);  // 转换为毫米并显示
//              break;
//          }
//          HAL_Delay(1);
//      }
//      // 停止捕获
//      HAL_TIM_IC_Stop_IT(&htim1, TIM_CHANNEL_3);
//      HAL_Delay(300);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void display_distance_cm(uint32_t mm_distance)
{
    uint32_t cm = mm_distance / 10;  // mm → cm，保留整数部分

    CurrentDisplay[0] = char2segments('0' + (cm / 1000) % 10);
    CurrentDisplay[1] = char2segments('0' + (cm / 100) % 10);
    CurrentDisplay[2] = char2segments('0' + (cm / 10) % 10);
    CurrentDisplay[3] = char2segments('0' + (cm % 10));

    // 去除前导 0：替换为空格段码
    if (cm < 1000) CurrentDisplay[0] = char2segments(' ');
    if (cm < 100) CurrentDisplay[1] = char2segments(' ');
    if (cm < 10) CurrentDisplay[2] = char2segments(' ');

    // 发送到显示器
    tm1637_DisplayHandle(7, CurrentDisplay);  // 亮度等级7
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
        // 捕获到上升沿
        if (!echo_flag)
        {
            rising_cnt = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
            echo_flag = 1;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
        }
            // 捕获到下降沿
        else
        {
            falling_cnt = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
            echo_flag = 0;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
        }
    }
}
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
