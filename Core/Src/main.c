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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "../../hall_sensor/hall_sensor.h"
#include "../../motor_utils/motor_driver.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* ICU PD */
#define ICU_TIMCLOCK 48000000
#define ICU_TIMEOUT_COUNTS 2

/* TIM2 PD*/
#define TIM2_CLOCKRATE 48000000
#define TIM2_ARR 733

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/*
 * Fetches speed % reading from throttle piece
 * Reads ADC data and remaps it to a percentage
 * @retval
 * */
uint8_t poll_speed();

/*
 * Initializes the pinouts and necessary software configs
 * of electrical phases and gate drivers of the motor inverter
 */
void init_inverter();
/**
 * Initializes the required configurations for 180 Commutation
 */
void init_commutation180();
// GLOBAL VARIABLES
HALL_SENSOR hs1, hs2, hs3;
HALL_STATE hstate;

volatile uint8_t Throttle_percent = 0;

/* ICU Variables */
volatile float hall_frequency = 0;
volatile float motor_rpm = 0;
volatile uint8_t ICU_timeout_count = 0;

/* Electrical objects */
// 6 mosfet gate drivers..
MOS_gate high_sides[3];
MOS_gate low_sides[3];

// 3 phases
Phase phases[3];

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
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

	init_hall_sensor(&hs1, GPIOA, GPIO_PIN_15);
	init_hall_sensor(&hs2, GPIOB, GPIO_PIN_3);
	init_hall_sensor(&hs3, GPIOB, GPIO_PIN_4);

	HAL_TIM_Base_Start(&htim3); // Start Timer3 (Trigger Source For ADC1)
	HAL_ADC_Start_IT(&hadc1);   // 	Start ADC Conversion
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_3);

	init_inverter();
	init_commutation180();
	//start_pwm(&phases[0]);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//set_phase_pwm_DC(&phases[0], 50);
    	//HAL_Delay(100);
    	/*set_phase_pwm_DC(&phases[0], 60);
    	HAL_Delay(100);
    	set_phase_pwm_DC(&phases[0], 100);
    	HAL_Delay(100);*/
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 4800;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 733-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 63;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 49999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 734-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */
	// Enable update interrupt
	__HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Debugging_indicator_GPIO_Port, Debugging_indicator_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SIM_HALL_A_Pin|SIM_HALL_B_Pin|SIM_HALL_C_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : Debugging_indicator_Pin */
  GPIO_InitStruct.Pin = Debugging_indicator_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Debugging_indicator_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : HALL_A_EXTI_15_Pin */
  GPIO_InitStruct.Pin = HALL_A_EXTI_15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(HALL_A_EXTI_15_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : HALL_B_EXTI_3_Pin HALL_C_EXTI_4_Pin */
  GPIO_InitStruct.Pin = HALL_B_EXTI_3_Pin|HALL_C_EXTI_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SIM_HALL_A_Pin SIM_HALL_B_Pin SIM_HALL_C_Pin */
  GPIO_InitStruct.Pin = SIM_HALL_A_Pin|SIM_HALL_B_Pin|SIM_HALL_C_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

uint8_t poll_speed()
{
	//gonna use it for naive digital filter
	static uint16_t last_val = 0;
	uint16_t adc_val;
	/*
	 HAL_ADC_Start(&hadc1); // start the adc

	 HAL_ADC_PollForConversion(&hadc1, 100); // poll for conversion

	 adc_val = HAL_ADC_GetValue(&hadc1); // get the adc value

	 HAL_ADC_Stop(&hadc1); // stop adc
	 */
	adc_val = HAL_ADC_GetValue(&hadc1);
	// steps range : 496 (400 mv) => 1539 (1.24v)
	if (adc_val < 496)
		adc_val = 496;
	if (adc_val > 1539)
		adc_val = 1539;
	if(adc_val >= last_val)
		adc_val = last_val + (adc_val - last_val)/2;
	else
		adc_val = last_val - (last_val - adc_val)/2;
	last_val = adc_val;
	return (adc_val - 496) * (uint16_t) (100) / (1539 - 496);
}

void init_inverter(){
	//High side mosfets A,B,C
	init_mosfet(&high_sides[0], GPIOA, GPIO_PIN_8);//A8
	init_mosfet(&high_sides[1], GPIOA, GPIO_PIN_9);//A9
	init_mosfet(&high_sides[2], GPIOA, GPIO_PIN_10);//A10
	//Low side mosfets A,B,C
	init_mosfet(&low_sides[0], GPIOA, GPIO_PIN_7);//A7
	init_mosfet(&low_sides[1], GPIOB, GPIO_PIN_0);//B0
	init_mosfet(&low_sides[2], GPIOB, GPIO_PIN_1);//B1

	//INIT phase A
	init_phase(&phases[0], &high_sides[0], &low_sides[0], &htim1, TIM_CHANNEL_1);
	//INIT phase B
	init_phase(&phases[1], &high_sides[1], &low_sides[1], &htim1, TIM_CHANNEL_2);
	//INIT phase C
	init_phase(&phases[2], &high_sides[2], &low_sides[2], &htim1, TIM_CHANNEL_3);

}
void init_commutation180(){
	set_phase_to_complementary_pwm(&phases[0]);
	set_phase_to_complementary_pwm(&phases[1]);
	set_phase_to_complementary_pwm(&phases[2]);
}
/*
 * Hall Sensors state change ISR
 * Manages BLDC commutation.
 * 180 Commutation is implemented alongside complementary switching
 * (during unforced period of the PWM , charged coils discharge to ground without Free wheeling diodes)
 * Max Freq = 6 * 220 = ~ 1320 hz
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	update_hall_state(&hstate, &hs1, &hs2, &hs3);
	//{0b101 , 0b001 , 0b011 , 0b010 , 0b110 , 0b100};
	// transition , could be wrong , easily fixed by reordering conditions of the following cases..
	// in other words, set up accordingly..

	switch (hstate.state)
	{
	// 101 => 0 - 60 deg => A+ B- C+
	case 5:
		// A+
		set_phase_pwm_DC(&phases[0], Throttle_percent);
		// C+
		set_phase_pwm_DC(&phases[2], Throttle_percent);
		// B-
		set_phase_pwm_DC(&phases[1], 100);//100 for inverted logic

		break;
		// 60 - 120 => + - -
	case 1:
		// A+
		set_phase_pwm_DC(&phases[0], Throttle_percent);
		set_phase_pwm_DC(&phases[1], 100);//100 for inverted logic
		set_phase_pwm_DC(&phases[2], 100);//100 for inverted logic
		// B-
		// set_pin_as_gpio_high(GPIOB, GPIO_PIN_0);
		// C-
		// set_pin_as_gpio_high(GPIOB, GPIO_PIN_1);
		break;
		// 120 - 180 => + + -
	case 3:
		set_phase_pwm_DC(&phases[0], Throttle_percent);
		set_phase_pwm_DC(&phases[1], Throttle_percent);
		set_phase_pwm_DC(&phases[2], 100);//100 for inverted logic
		// A+
		// B+
		// C-
		break;
		// 180 - 240 => - + -
	case 2:
		set_phase_pwm_DC(&phases[0], 100);
		set_phase_pwm_DC(&phases[1], Throttle_percent);
		set_phase_pwm_DC(&phases[2], 100);//100 for inverted logic
		// B +
		// A -
		// C -
		break;
		// 240 - 300 => - + +
	case 6:
		set_phase_pwm_DC(&phases[0], 100);
		set_phase_pwm_DC(&phases[1], Throttle_percent);
		set_phase_pwm_DC(&phases[2], Throttle_percent);
		// B+
		// C+
		// A-
		break;
		// 300 - 360 => - - +
	case 4:
		set_phase_pwm_DC(&phases[0], 100);
		set_phase_pwm_DC(&phases[1], 100);//100 for inverted logic
		set_phase_pwm_DC(&phases[2], Throttle_percent);//100 for inverted logic
		// C+
		// A-
		// B-
		break;
	default:
		break;
	}
	//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
	return;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	uint8_t speed = poll_speed();
	speed = 50;
	// limit throttle to [10, 90]

	if (speed < 10)
	{
		Throttle_percent = 0;
	}
	else if(speed > 90)
	{
		Throttle_percent = 90;
	}
	else
	{
		Throttle_percent = speed;
	}

	// Map Throttle_percent to frequency ( x6 considering 6 PWM sates )
	float required_frequency = 6;

	// Change simulated Hall sensor frequency
	if ((uint32_t) required_frequency == 0)
	{
		// Set minimum frequency to 1 Hz (6 Hz considering 6 PWM sates)
		required_frequency = 6;
	}

	// Get (PSC * ARR) from required frequency
	float PSC_ARR_product = TIM2_CLOCKRATE / required_frequency;

	// Get PSC value (ARR = TIM2_ARR - 1)
	float PSC_value = PSC_ARR_product / (TIM2_ARR);

	// Set PSC value
	TIM2->PSC = (uint16_t) (PSC_value - 1);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t IC_Val1 = 0;
	static uint16_t IC_Val2 = 0;
	static uint16_t difference = 0;
	static uint8_t captured_first = 0;

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		// Reset timeout counter
		ICU_timeout_count = 0;

		if (!captured_first) // if the first rising edge is not captured
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // read the first value
			captured_first = 1;                // set the first captured as true
		}

		else // If the first rising edge is captured, now we will capture the second edge
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // read second value

			if (IC_Val2 > IC_Val1)
			{
				difference = IC_Val2 - IC_Val1;
			}

			else if (IC_Val1 > IC_Val2)
			{
				// In case of counter overflow before second capture
				difference = (0xFFFF - IC_Val1) + IC_Val2;
			}

			float refClock = (float) ICU_TIMCLOCK / (TIM4->PSC + 1);

			hall_frequency = refClock / difference;
			motor_rpm = 60 * hall_frequency / 23;

			__HAL_TIM_SET_COUNTER(htim, 0); // reset the counter
			captured_first = 0;             // set it back to false
		}
	}

	return;
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
