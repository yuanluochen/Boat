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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "bsp.h"
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
uint8_t lora_rx_buff[LORA_RX_BUFF_LEN] = { 0 };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void servo_init(void)
{
  servo_ctrl(&SERVO_TIM, SERVO_FRONT_CHANNEL, SERVO_FRONT_ANGLE_FRONT);
  servo_ctrl(&SERVO_TIM, SERVO_BACK_CHANNEL, SERVO_BACK_ANGLE_FRONT);
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
  int SwitchFlag = 0;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1, lora_rx_buff, LORA_RX_BUFF_LEN);
	
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	
  //舵机自动回正,电调停转
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ESC_MID);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, ESC_MID);
  servo_init();
	HAL_Delay(2000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#if TEST
  //  //两者正向
  //  servo_ctrl(&SERVO_TIM, SERVO_FRONT_CHANNEL, SERVO_FRONT_ANGLE_FRONT);
  //  servo_ctrl(&SERVO_TIM, SERVO_BACK_CHANNEL, SERVO_BACK_ANGLE_FRONT);

  //  //控制电机转速,正向
  //  __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_FRONT_CHANNEL, 1200);
  //  __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_BACK_CHANNEL, 1200);
		
	// 	HAL_Delay(5);
 
		//boat_ctrl.motion = motion_TurnRight;

#endif
#if BOAT_MOTION

    if(boat_ctrl.motion == motion_front)//前进
    {
      
      servo_ctrl(&SERVO_TIM, SERVO_BACK_CHANNEL, SERVO_BACK_ANGLE_FRONT);
      //HAL_Delay(SERVO_DELAY);
      servo_ctrl(&SERVO_TIM, SERVO_FRONT_CHANNEL, SERVO_FRONT_ANGLE_FRONT);

      //HAL_Delay(MOTOR_DELAY);

      //控制电机转速,正向
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_FRONT_CHANNEL, MOTOR_FRONT);
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_BACK_CHANNEL, MOTOR_BACK);
    }
    else if(boat_ctrl.motion == motion_back)//后退
    {
      //两者正向
      servo_ctrl(&SERVO_TIM, SERVO_BACK_CHANNEL, SERVO_BACK_ANGLE_FRONT);
      //HAL_Delay(SERVO_DELAY);
      servo_ctrl(&SERVO_TIM, SERVO_FRONT_CHANNEL, SERVO_FRONT_ANGLE_FRONT);

      //HAL_Delay(MOTOR_DELAY);

      //控制电机转速，反向
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_FRONT_CHANNEL, MOTOR_BACK);
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_BACK_CHANNEL, MOTOR_FRONT);
    }
    else if(boat_ctrl.motion == motion_left)//左平移
    {
      //前后舵机左转
      servo_ctrl(&SERVO_TIM, SERVO_BACK_CHANNEL, SERVO_BACK_ANGLE_LEFT);
      //HAL_Delay(SERVO_DELAY);      
      servo_ctrl(&SERVO_TIM, SERVO_FRONT_CHANNEL, SERVO_FRONT_ANGLE_LEFT);

      //HAL_Delay(MOTOR_DELAY);

      //控制电机转速,正向
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_FRONT_CHANNEL, MOTOR_FRONT);
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_BACK_CHANNEL, MOTOR_BACK);
    }
    else if (boat_ctrl.motion == motion_right) //右平移
    {
      //前后舵机右转
      servo_ctrl(&SERVO_TIM, SERVO_BACK_CHANNEL, SERVO_BACK_ANGLE_RIGHT);
      //HAL_Delay(SERVO_DELAY);
      servo_ctrl(&SERVO_TIM, SERVO_FRONT_CHANNEL, SERVO_FRONT_ANGLE_RIGHT);

      //HAL_Delay(MOTOR_DELAY);

      //控制电机转速,正向
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_FRONT_CHANNEL, MOTOR_FRONT);
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_BACK_CHANNEL, MOTOR_BACK);

    }
    else if(boat_ctrl.motion == motion_TurnLeft)//平移
    {
      //前舵机左，后舵机右
      servo_ctrl(&SERVO_TIM, SERVO_BACK_CHANNEL, SERVO_BACK_ANGLE_RIGHT);
      //HAL_Delay(SERVO_DELAY);
      servo_ctrl(&SERVO_TIM, SERVO_FRONT_CHANNEL, SERVO_FRONT_ANGLE_LEFT);

      //HAL_Delay(MOTOR_DELAY);

      //控制电机转速,正向
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_FRONT_CHANNEL, MOTOR_FRONT);
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_BACK_CHANNEL, MOTOR_BACK);

    }
    else if(boat_ctrl.motion == motion_TurnRight)//右转
    {
      //前舵机右，后舵机左
      servo_ctrl(&SERVO_TIM, SERVO_BACK_CHANNEL, SERVO_BACK_ANGLE_LEFT);
      //HAL_Delay(SERVO_DELAY);
      servo_ctrl(&SERVO_TIM, SERVO_FRONT_CHANNEL, SERVO_FRONT_ANGLE_RIGHT);

      //HAL_Delay(MOTOR_DELAY);

      //控制电机转速,正向
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_FRONT_CHANNEL, MOTOR_FRONT);
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_BACK_CHANNEL, MOTOR_BACK);
    }
    else
    {
      //两者正向
      servo_ctrl(&SERVO_TIM, SERVO_BACK_CHANNEL, SERVO_BACK_ANGLE_FRONT);
      //HAL_Delay(SERVO_DELAY);
      servo_ctrl(&SERVO_TIM, SERVO_FRONT_CHANNEL, SERVO_FRONT_ANGLE_FRONT);

      //HAL_Delay(MOTOR_DELAY);

      //控制电机转速停止
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_FRONT_CHANNEL, MOTOR_STOP);
      __HAL_TIM_SET_COMPARE(&MOTOR_TIM, MOTOR_BACK_CHANNEL, MOTOR_STOP);
    }


    //继电器控制
    if(boat_ctrl.idleControl.idle == IdleOpen)//继电器开
    {
      HAL_GPIO_WritePin(SWITCH_GPIO_Port, SWITCH_Pin, GPIO_PIN_SET);
    }
    else if(boat_ctrl.idleControl.idle == IdleClose)
    {
      HAL_GPIO_WritePin(SWITCH_GPIO_Port, SWITCH_Pin, GPIO_PIN_RESET);
    }
    else
    {
      HAL_GPIO_WritePin(SWITCH_GPIO_Port, SWITCH_Pin, GPIO_PIN_RESET);
    }
    HAL_Delay(5);
#endif
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
  // /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
