/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>  //printf
#include <string.h>
#include <time.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
	
	typedef enum {
	FAILED = 0,
	PASSED = !FAILED
} TestStatus; // Typedef for correct read/write tests

#define SRAM_BANK_ADDR		((uint32_t)0x64000000)		// We use Bank 3, Block 2, thus FMC_NE2 and 0x64000000
//#define SRAM_SIZE			((uint32_t)0x1000) 
#define SRAM_SIZE			((uint32_t)0x40000) 		// AS7C34098A has 2^18 address indices
//#define SRAM_SIZE			((uint32_t)0x80000) 		// Cypress CY62157EV30 has 2^19 address indices
//#define SRAM_SIZE			((uint32_t)0x100000) 		// 2^20 address indices
#define BUFFER_SIZE			100							// buffer size for the global string buffer
#define SRAM_BUFFER_SIZE	40960						// buffer size for the sram buffer
#define COMMAND_COUNT		15

	uint32_t sram_start = SRAM_BANK_ADDR;
	//uint32_t sram_end = 0x64040000;
	uint32_t sram_end = SRAM_BANK_ADDR + SRAM_SIZE;
	//uint32_t SRAM_SIZE = (uint32_t)sram_end - (uint32_t)sram_start;
	
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
	int expression;
	int counter;
	clock_t tic = 0;
	clock_t toc = 0;
	
	//uint32_t sram_start = 0x64000000;
	//uint32_t sram_end = 0x64800000;
	//uint32_t sram_end = (uint32_t)sram_start - (uint32_t)SRAM_SIZE;   //2^19 (1mb)
	//uint32_t SRAM_SIZE = (uint32_t)sram_end - (uint32_t)sram_start;
	uint32_t *ptr = (uint32_t *)SRAM_BANK_ADDR;
	//uint32_t sram_end = 0x64080000;   //2^19 (1mb)
	uint32_t zero = 0x00000000;
	uint32_t ones = 0xFFFFFFFF;
	uint32_t write = 0x12345678;
	
	
	
	uint32_t total_one;
	uint32_t total_zero;
	uint32_t flipped_one;
	uint32_t flipped_zero;
	
	uint32_t write_mode = 0xFFFFFFFF;
	uint32_t start_value = 0x0;
	uint32_t start_adr = 0x0;	// start address for writeSRAMRange
	uint32_t end_adr = 0x0;		// end address for writeSRAMRange
	
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void write_all_zero(void);				//writes zero to entire memory location
void write_all_ones(void);				//writes ones to entire memory location
void write_all_12345678(void);		//writes 12345678 to entire memory location
void sram_read(void);									//reads entire memory location
void sram_read_1(void);						//latency_read
void SRAM_Get_Performance_Measures();
//void SRAM_Get_Values();
void write_all_zero_check(void);
//uint32_t hamming_weight_32b(uint32_t);
//uint16_t hamming_weight_16b(uint16_t);
//uint32_t SRAM_Read_32b(uint32_t adr);
void init_counter();
void init_arguments();
//uint16_t get_num_one_16b(uint16_t bitstring);
//uint16_t get_num_zero_16b(uint16_t bitstring);

void SRAM_Write_32b(uint32_t adr, uint32_t value);
void SRAM_Write_16b(uint32_t adr, uint16_t value);
uint16_t SRAM_Read_16b(uint32_t adr);
uint32_t SRAM_Read_32b(uint32_t adr);
void SRAM_Fill_With_Zeros();
void SRAM_Fill_With_Ones();
void SRAM_Get_Values();
uint16_t flipped_one_16b(uint16_t expected_value, uint16_t real_value);
uint16_t flipped_zero_16b(uint16_t expected_value, uint16_t real_value);
uint16_t get_num_one_16b(uint16_t bitstring);
uint16_t get_num_zero_16b(uint16_t bitstring);
uint16_t get_total_flip_probability_16b(uint16_t expected_value, uint16_t real_value);
uint16_t hamming_weight_16b(uint16_t bitstring);

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
  MX_FMC_Init();
  MX_UART4_Init();
  MX_USB_DEVICE_Init();
  MX_TIM2_Init();
	
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Init(&htim2);
  HAL_TIM_Base_Start(&htim2);
  /* USER CODE END 2 */

	char address_setup_time[2];
	char data_setup_time[2];
	
	HAL_Delay(3000);
	
	printf("\n\rWelcome to sram PUF Experiments\r\n");
//	printf("\n\rEnter address_setup_time for SRAM FMC (00-99)\t:");
//	HAL_UART_Receive (&huart4,(uint8_t *)address_setup_time, 2, 30000);
//	printf("\n\rEnter data_setup_time for SRAM FMC (00-99)\t:");
//	HAL_UART_Receive (&huart4, (uint8_t *)data_setup_time, 2, 30000);
//	
//	
//	printf("\n\raddress setup time : %s",address_setup_time);
//	printf("\n\rdata setup time : %s\r\n",data_setup_time);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
	
	//Non user interactive function

	//////print entire SRAM
	//printf("\r\nReading SRAM memory from %p to %p...\r\n",(uint32_t *)sram_start,(uint32_t *)sram_end);
	printf("\r\nReading SRAM memory\r\n");
	sram_read();
	printf("\r\nSRAM memory from %p to %p is read successfully... %d bit memory read\n\n\r",(uint32_t *)sram_start,(uint32_t *)sram_end,counter);
	//printf("\r\nSRAM read successfully... %d bit memory read\n\r",counter);

	//////calculate hamming weight
	//printf("\r\nCalculating SRAM zero and ones fr/om %p to %p...\r\n",(uint32_t *)sram_start,(uint32_t *)sram_end);
	SRAM_Get_Values();
	//printf("\r\nSRAM zero and ones from %p to %p is calculated successfully... \n\n\r",(uint32_t *)sram_start,(uint32_t *)sram_end);

	//////write to SRAM
	printf("\r\nSetting SRAM values to %x ...",(uint32_t)zero);
	//SRAM_Fill_With_Zeros();
	SRAM_Fill_With_Ones();
	//printf("\r\nValues in memory location from %p to %p is set to %x\n\n\r",(uint32_t *)sram_start,(uint32_t *)sram_end,(uint32_t)zero);
	//printf("\r\nDone...");

	//////calculate hamming weight
	//printf("\r\nCalculating SRAM zero and ones from %p to %p...\r\n",(uint32_t *)sram_start,(uint32_t *)sram_end);
	SRAM_Get_Values();
	//printf("\r\nSRAM zero and ones from %p to %p is calculated successfully... \n\n\r",(uint32_t *)sram_start,(uint32_t *)sram_end);


	//////stop string
	printf("\r\n~~~~~~~~~~~~~~~\r\n");
	
	
	
	/* while(1)
	{
  	
	//int counter = 0;
	//uint32_t *ptr = (uint32_t *)0x60000000;
  //uint32_t write = 0x12345678;	
	

		
  printf("\n\rSelect any of the function below\r\n");
	printf("\t1. Write zero (0x00000000) to entire memory location.\r\n");
	printf("\t2. Write ones (0xffffffff) to entire memory location.\r\n"); 
	printf("\t3. Write random numbers (0x12345678) to entire memory location.\r\n");
	printf("\t4. Read entire memory location...\r\n");
	printf("\t5. Read entire memory location(latency)...\r\n");
	printf("\t6. get performance...\r\n");
	printf("\t7. calculate zeros and ones(Hamming weight)...\r\n");
	printf("\t8. Write zero (0x00000000) to entire memory location and check.\r\n");
		
	//scanf("%d", &expression);
	uint8_t Rx_sram_function[1];		//array of input element
	HAL_UART_Receive (&huart4, Rx_sram_function, 1, 30000);
	//printf("\n\ryou have selected %s...\r\n",Rx_data);
	
		
	printf("\n\ryou have selected %s...\r\n",Rx_sram_function);
	

	for(int i=0;i<1;i++)		//user selection assign to an expression
	{
		expression = Rx_sram_function[i];
	}
	
	//printf("\n\ryou have selected %d...expression\r\n",expression);
	
	switch(expression)
		{
			case 49:		// write all zeros		//ASCII values
			{
				printf("\r\nSetting SRAM values to %x ...",(uint32_t)zero);
				//write_all_zero();
				SRAM_Fill_With_Zeros();
				printf("\r\nValues in memory location from %p to %p is set to %x\n\n\r",(uint32_t *)sram_start,(uint32_t *)sram_end,(uint32_t)zero);
				//printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
				//printf("\n\ryou have selected %d...write_all_zero\r\n",expression);
				break;
			} 

			case 50:		// write all ones		//ASCII values
			{
				printf("\r\nSetting SRAM values to %x ...",(uint32_t)ones);
				//write_all_ones();
				SRAM_Fill_With_Ones();
				printf("\r\nValues in memory location from %p to %p is set to %x\n\n\r",(uint32_t *)sram_start,(uint32_t *)sram_end,(uint32_t)ones);
				//printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
				//printf("\n\ryou have selected %d...write_all_ones\r\n",expression);
				break;
			} 
 
			case 51:		// write all 12345678		//ASCII values
			{
				printf("\r\nSetting SRAM values to %x ...",(uint32_t)write);
				write_all_12345678();
				printf("\r\nValues in memory location from %p to %p is set to %x\n\n\r",(uint32_t *)sram_start,(uint32_t *)sram_end,(uint32_t)write);
				//printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
				//printf("\n\ryou have selected %d...write_all_12345678\r\n",expression);
				break;
			} 
			
			case 52:		// Read entire SRAM		//ASCII values
			{
				printf("\r\nReading SRAM memory from %p to %p...\r\n",(uint32_t *)sram_start,(uint32_t *)sram_end);
				sram_read();
				printf("\r\nSRAM memory from %p to %p is read successfully... %d bit memory read\n\n\r",(uint32_t *)sram_start,(uint32_t *)sram_end,counter);
				//printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
				//printf("\n\ryou have selected %d...Read entire SRAM\r\n",expression);
				break;
			} 
			
			case 53:		// Read entire SRAM_latency		//ASCII values
			{
				printf("\r\nReading SRAM memory from %p to %p...\r\n",(uint32_t *)sram_start,(uint32_t *)sram_end);
				sram_read_1();
				printf("\r\nSRAM memory from %p to %p is read successfully... %d bit memory read\n\n\r",(uint32_t *)sram_start,(uint32_t *)sram_end,counter);
				//printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
				//printf("\n\ryou have selected %d...Read entire SRAM\r\n",expression);
				break;
			} 
			
			case 54:		// SRAM performance		//ASCII values
			{
				printf("\r\nCalculating SRAM performance from %p to %p...\r\n",(uint32_t *)sram_start,(uint32_t *)sram_end);
				SRAM_Get_Performance_Measures();
				printf("\r\nSRAM performance from %p to %p is calculated successfully... \n\n\r",(uint32_t *)sram_start,(uint32_t *)sram_end);
				//printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
				//printf("\n\ryou have selected %d...Read entire SRAM\r\n",expression);
				break;
			}
			
			case 55:		// get sram values		//ASCII values
			{
				printf("\r\nCalculating SRAM zero and ones from %p to %p...\r\n",(uint32_t *)sram_start,(uint32_t *)sram_end);
				SRAM_Get_Values();
				printf("\r\nSRAM zero and ones from %p to %p is calculated successfully... \n\n\r",(uint32_t *)sram_start,(uint32_t *)sram_end);
				//printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
				//printf("\n\ryou have selected %d...Read entire SRAM\r\n",expression);
				break;
			}
			
			case 56:		// write all zeros		//ASCII values
			{
				printf("\r\nSetting SRAM values to %x ...",(uint32_t)zero);
				write_all_zero_check();
				printf("\r\nValues in memory location from %p to %p is set to %x\n\n\r",(uint32_t *)sram_start,(uint32_t *)sram_end,(uint32_t)zero);
				//printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
				//printf("\n\ryou have selected %d...write_all_zero\r\n",expression);
				break;
			} 
			
			default:
			{
				printf("\n\rCommand not found...\r\nPlease select a valid function after reset...\r\n");
				break;
			} 
		}

	}*/ //--commented this to create a non user interactive function
	
	
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart4, (uint8_t *)&ch, 1, 0xFFFF);
	

  return ch;
}

//2)//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
//{
//  HAL_UART_Receive_IT(&huart4, Rx_data, 1); 
//}

//1)//void receive(UART_HandleTypeDef *huart, uint8_t *dstBuffer, uint32_t bufferSize){
//	HAL_UART_Receive_IT(huart, dstBuffer, bufferSize);
//	HAL_Delay(10);
//}

void write_all_zero(void)  	//writes zero to entire memory location
{
	//uint32_t *ptr = (uint32_t *)0x64000000;
  //uint32_t zero = 0x00000000;
	//tic = clock();
	write_mode = 1;
	uint32_t now = HAL_GetTick();
	for(ptr=(uint32_t *)sram_start; ptr < (uint32_t *)sram_end; ptr++ )
  {		
		*ptr = zero;
  }
	uint32_t then = HAL_GetTick();
	printf("\r\ninterrupt 1:%f ms",(double)now);
	printf("\r\ninterrupt 2:%f ms",(double)then);
	printf("\r\ndiff = %f ms\n\r", ((double) (then - now)));
	printf("\r\nElapsed: %f seconds\n", (double)(then - now) / 1000);		//convert milliseconds to seconds
	//printf("\r\nElapsed: %f seconds\n", (double)(then - now) / 0.09523);
	//toc = clock();
	//printf("\r\n%f",(double)tic);
	//printf("\r\n%f",(double)toc);
	//printf("\r\nElapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
}	

void write_all_zero_check(void)  	//writes zero to entire memory location
{
	//uint32_t *ptr = (uint32_t *)0x64000000;
  //uint32_t zero = 0x00000000;
	write_mode = 1;
	uint32_t ret_wert;
	uint16_t real_value = 0xFFFF;
	uint32_t now = HAL_GetTick();
	for(uint32_t adr = 0; adr < SRAM_SIZE; adr++)
  {		
		//SRAM_Write_16b(start_adr, start_value);
		real_value = SRAM_Read_16b(adr);
		//*ptr = zero;
		//SRAM_Read_32b((uint32_t)ptr);
		if(real_value != zero && real_value != ones)
		{
			printf("%x - failed\r\n",adr+SRAM_BANK_ADDR);
		}
		ptr++;	
  }
	uint32_t then = HAL_GetTick();
	printf("\r\ninterrupt 1:%f ms",(double)now);
	printf("\r\ninterrupt 2:%f ms",(double)then);
	printf("\r\ndiff = %f ms\n\r", ((double) (then - now)));
	printf("\r\nElapsed: %f seconds\n", (double)(then - now) / 1000);		//convert milliseconds to seconds

}	

void write_all_ones(void)		//writes ones to entire memory location
{
	//uint32_t *ptr = (uint32_t *)0x64000000;
  //uint32_t ones = 0x11111111;
	//tic = clock();
	write_mode = 2;
	uint32_t now = HAL_GetTick();
	for(ptr=(uint32_t *)sram_start; ptr < (uint32_t *)sram_end; ptr++ )
  {		
		*ptr = ones;
  }
	uint32_t then = HAL_GetTick();
	printf("\r\ninterrupt 1:%f ms",(double)now);
	printf("\r\ninterrupt 2:%f ms",(double)then);
	printf("\r\ndiff = %f ms\n\r", ((double) (then - now)));
	printf("\r\nElapsed: %f seconds\n", (double)(then - now) / 1000);		//convert milliseconds to seconds
	//printf("\r\nElapsed: %f seconds\n", (double)(then - now) / 0.09523);
	
//	toc = clock();
//	printf("\r\n%f",(double)tic);
//	printf("\r\n%f",(double)toc);
//	printf("\r\nElapsed: %f seconds\n", (double)(toc - tic) / 1000);
}	

void write_all_12345678(void)		//writes 12345678 to entire memory location
{
	//uint32_t *ptr = (uint32_t *)0x64000000;
  //uint32_t write = 0x12345678;
	//tic = clock();
	write_mode = 3;
	uint32_t now = HAL_GetTick();
	for(ptr=(uint32_t *)sram_start; ptr < (uint32_t *)sram_end; ptr++ )
  {		
		*ptr = write;	
			
  }
	uint32_t then = HAL_GetTick();
	printf("\r\ninterrupt 1:%f ms",(double)now);
	printf("\r\ninterrupt 2:%f ms",(double)then);
	printf("\r\ndiff = %f ms\n\r", ((double) (then - now)));
	printf("\r\nElapsed: %f seconds\n", (double)(then - now) / 1000);		//convert milliseconds to seconds
	//printf("\r\nElapsed: %f seconds\n", (double)(then - now) / 0.09523);
	//toc = clock();
	
//	printf("\r\n%f",(double)tic);
//	printf("\r\n%f",(double)toc);
//	printf("\r\nElapsed: %f seconds\n", (double)(toc - tic) / 1000);
}	

void sram_read(void)		//read entire sram and print in console
{
	counter = 0;
	int tabs = 0;
	write_mode = 4;	
	uint32_t *ptr = (uint32_t *)0x64000000;
	//printf("%x:",SRAM_BANK_ADDR);
  for(uint32_t adr = 0; adr < SRAM_SIZE; adr=adr+4)
	{		
		counter++;
		
		printf("%x:%x\r\n",adr+SRAM_BANK_ADDR,SRAM_Read_32b(adr));
		
		
//		if(tabs < 8)
//		{
//				printf("%x:%x\n\r",adr+SRAM_BANK_ADDR,SRAM_Read_32b(adr));
//				//printf("%x\t",SRAM_Read_32b(adr));
//				//printf("%x:%x\r\n",adr+SRAM_BANK_ADDR,SRAM_Read_16b(adr));
//				//printf("%x:%x\r\n",SRAM_Read_16b(adr),SRAM_Read_32b(adr));
//				//printf("%x:%x\r\n",adr+SRAM_BANK_ADDR,SRAM_Read_32b(adr));
//				tabs++;
//		}
//		else
//		{
//			//printf("\r\n");
//			//printf("\r\n%x:",adr+SRAM_BANK_ADDR);
//			printf("%x\t",SRAM_Read_32b(adr));
//			tabs = 1;
//		}
		
//		if(tabs < 8)
//		{
//				//printf("%x:%x\n\r",adr+SRAM_BANK_ADDR,SRAM_Read_32b(adr));
//				printf("%x\t",SRAM_Read_32b(adr));
//				//printf("%x:%x\r\n",adr+SRAM_BANK_ADDR,SRAM_Read_16b(adr));
//				//printf("%x:%x\r\n",SRAM_Read_16b(adr),SRAM_Read_32b(adr));
//				//printf("%x:%x\r\n",adr+SRAM_BANK_ADDR,SRAM_Read_32b(adr));
//				tabs++;
//		}
//		else
//		{
//			printf("\r\n");
//			//printf("\r\n%x:",adr+SRAM_BANK_ADDR);
//			printf("%x\t",SRAM_Read_32b(adr));
//			tabs = 1;
//		}
		
  }

}

void sram_read_old(void)		//read entire sram and print in console
{
	counter = 0;
	write_mode = 4;
	//tic = clock();
	//uint32_t now = HAL_GetTick();
	uint32_t *ptr = (uint32_t *)0x64000000;
	//uint16_t *ptr1 = (uint16_t *)0x64000000;
	for(ptr=(uint32_t *)sram_start; ptr < (uint32_t *)sram_end; ptr++ )
  //for(uint32_t adr = 0; adr < SRAM_SIZE; adr++)
	{		
		counter++;
		//printf("%p:%d:%p\n",(void*)&ptr,(void*)*ptr,start);
		//printf("%p:%x\r\n",adr+SRAM_BANK_ADDR,SRAM_Read_16b(adr));
		//printf("%x:%x\r\n",SRAM_Read_16b(adr),SRAM_Read_32b(adr));
		//printf("%x:%x\r\n",adr+SRAM_BANK_ADDR,SRAM_Read_32b(adr));
    //printf("%p:%x\r\n",ptr,*ptr);
		printf("%p:%x\r\n",ptr,*ptr);
		//printf("%4x\r\n",*ptr);
    //printf("%x\r\n",*ptr);
		//HAL_Delay(500);
       
  }
	//uint32_t then = HAL_GetTick();
	//printf("\r\ninterrupt 1:%f ms",(double)now);
	//printf("\r\ninterrupt 2:%f ms",(double)then);
	//printf("\r\ndiff = %f ms\n\r", ((double) (then - now)));
	//printf("\r\nElapsed: %f seconds\n", (double)(then - now) / 1000);		//convert milliseconds to seconds
	//printf("\r\nElapsed: %f seconds\n", (double)(then - now) / 0.09523);
	
//	toc = clock();
//	printf("\r\n%f",(double)tic);
//	printf("\r\n%f",(double)toc);
//	printf("\r\nElapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
}

void sram_read_1(void)		//read entire sram with delay and print in console
{
	counter = 0;
	write_mode = 5;
	uint32_t value;
	uint32_t now = HAL_GetTick();
	//tic = clock();
	printf("delay start\n\r");
	HAL_Delay(5000);
	printf("delay end\n\r");
	//uint32_t *ptr = (uint32_t *)0x64000000;
	for(ptr=(uint32_t *)sram_start; ptr < (uint32_t *)sram_end; ptr++ )
  {		
		counter++;
		value=*ptr;
		//value = SRAM_Read_32b(ptr)
		//printf("%p:%d:%p\n",(void*)&ptr,(void*)*ptr,start);
    //printf("%p:%x\r\n",ptr,*ptr);
    //printf("%x\n",*ptr);
		//HAL_Delay(500);
       
  }
	uint32_t then = HAL_GetTick();
	printf("\r\ninterrupt 1:%f ms",(double)now);
	printf("\r\ninterrupt 2:%f ms",(double)then);
	printf("\r\ndiff = %f ms\n\r", ((double) (then - now)));
	printf("\r\nElapsed: %f seconds\n", (double)(then - now) / 1000);  //convert milliseconds to seconds
	//printf("\r\nElapsed: %f seconds\n", (double)(then - now) / 0.09523);
	//toc = clock();
	
	//uint32_t now = __HAL_TIM_GET_COUNTER(&htim2);
	//uint32_t then = __HAL_TIM_GET_COUNTER(&htim2);
	//printf("start = %.20f\n\rend   = %.20f\n\r", (double)tic, (double)toc);
  //printf("delta = %.20f\n\r", ((double) (toc - tic)));
  //printf("cpu_time_used  = %.15f seconds\n\r", (double)(toc - tic) / CLOCKS_PER_SEC);
  //printf("CLOCKS_PER_SEC = %i\n\n\r", CLOCKS_PER_SEC);
	
	//printf("\r\n%f",(double)tic);
	//printf("\r\n%f",(double)toc);
	//printf("\r\nElapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
}

void SRAM_Write_32b(uint32_t adr, uint32_t value){
	// multiply address by 2 due to 2-byte-access (address is given as one byte)
	adr = adr << 1;

	*(__IO uint32_t *) (sram_start + adr) = value;
}

void SRAM_Write_16b(uint32_t adr, uint16_t value){
	// multiply address by 2 due to 2-byte-access (address is given as one byte)
	adr = adr << 1;

	*(__IO uint16_t *) (sram_start + adr) = value;
}

uint16_t SRAM_Read_16b(uint32_t adr){
	// initialize the return value
	uint16_t ret_wert = 0;
	// multiply address by 2 due to 2-byte-access (address is given as one byte)
	adr = adr << 1;

	ret_wert = *(__IO uint16_t *) (sram_start + adr);

	return (ret_wert);
}

uint32_t SRAM_Read_32b(uint32_t adr){
	// initialize the return value
	uint32_t ret_wert = 0;
	// multiply address by 2 due to 2-byte-access (address is given as one byte)
	adr = adr << 1;

	//ret_wert = *(__IO uint32_t *) (sram_start + adr);
	ret_wert = *(__IO uint32_t *) (sram_start + adr);

	return (ret_wert);
}

/*
 * @brief								fills the whole SRAM with 0's
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 */

void SRAM_Fill_With_Zeros(){
	write_mode = 0x1;
	// reset the counter for statistical analysis
	init_counter();
	// reset the arguments
	init_arguments();

	uint16_t real_value = 0xFFFF;
	//uint32_t real_value_32 = 0xFFFFFFFF;
	TestStatus state = PASSED;
	for(uint32_t adr = 0; adr < SRAM_SIZE; adr++){
		SRAM_Write_16b(adr, 0x0);
		real_value = SRAM_Read_16b(adr);
		// test, if the written value equals the expected value
		// if something went wrong, break here and display an error message
		if(real_value != 0x0){
			state = FAILED;
			break;
		}
	}
	//SRAM_Write_16b(0x0, 0x0001);
	// display return message
	/*
	if(state == PASSED){
		sprintf(STRING_BUFFER, "'writeZeroAll' was successful\r\n\n");
	}else{
		sprintf(STRING_BUFFER, "'writeZeroAll' failed\r\n\n");
	}
	*/
	printf("\n\r--------End of write zero--------\n\r");
	
}

void SRAM_check(){
	uint16_t real_value = 0xFFFF;
	TestStatus state = PASSED;
	for(uint32_t adr = sram_start; adr < sram_end; adr++){
		real_value = SRAM_Read_16b(adr);
		// test, if the written value equals the expected value
		// if something went wrong, break here and display an error message
		if(real_value != 0x0){
			//print();

		}}}

/*
 * @brief								fills the whole SRAM with 1's
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 */
void SRAM_Fill_With_Ones(){
	// reset the counter for statistical analysis
	init_counter();
	// reset the arguments
	init_arguments();

	uint16_t real_value = 0x0;
	TestStatus state = PASSED;
	for(uint32_t adr = 0; adr < SRAM_SIZE; adr++){
		SRAM_Write_16b(adr, 0xFFFF);
		real_value = SRAM_Read_16b(adr);
		// test, if the written value equals the expected value
		// if something went wrong, break here and display an error message
		if(real_value != 0xFFFF){
			state = FAILED;
			break;
		}
	}
	// display return message
	/*
	if(state == PASSED){
		sprintf(STRING_BUFFER, "'writeOneAll' was successful\r\n\n");
	}else{
		sprintf(STRING_BUFFER, "'writeOneAll' failed\r\n\n");
	}
	*/
	printf("\n\r--------End of write ones--------\n\r");

}

void SRAM_Get_Values(){
	total_one = 0;
	total_zero = 0;
	for(uint32_t adr = 0; adr < SRAM_SIZE; adr++){
		uint16_t real_value;
		real_value = SRAM_Read_16b(adr);
		total_one += get_num_one_16b(real_value);
		total_zero += get_num_zero_16b(real_value);
	}


	// displays the total 1's
	printf("\nTotal number of ones in SRAM:  %lu\r\n", (unsigned long)total_one);
	


	// displays the total 0's
	printf("Total number of zeros in SRAM:  %lu\r\n", (unsigned long)total_zero);


	// displays the total 0's
	printf("Percent zeros:  %.3f%%\r\n", (float)total_zero / (SRAM_SIZE * 16) * 100);


	// displays the total 0's
	printf("Percent ones:  %.3f%%\r\n", (float)total_one / (SRAM_SIZE * 16) * 100);

}


uint16_t flipped_one_16b(uint16_t expected_value, uint16_t real_value){
	uint16_t flipped = expected_value ^ real_value;
	flipped = expected_value & flipped;

	return hamming_weight_16b(flipped);
}

uint16_t flipped_zero_16b(uint16_t expected_value, uint16_t real_value){
	uint16_t flipped = expected_value ^ real_value;
	flipped = flipped & real_value;

	return hamming_weight_16b(flipped);
}

uint16_t get_num_one_16b(uint16_t bitstring){
	return hamming_weight_16b(bitstring);
}

uint16_t get_num_zero_16b(uint16_t bitstring){
	return (16 - hamming_weight_16b(bitstring));
}

uint16_t get_total_flip_probability_16b(uint16_t expected_value, uint16_t real_value){
	uint16_t bitstring = expected_value ^ real_value;

	return (hamming_weight_16b(bitstring));
}

uint16_t hamming_weight_16b(uint16_t bitstring){
	uint16_t result;
	result = bitstring;
	uint16_t a = (result >> 0) & 0x5555;
	uint16_t b = (result >> 1) & 0x5555;
	result = a + b;
	a = (result >> 0) & 0x3333;
	b = (result >> 2) & 0x3333;
	result = a + b;
	a = (result >> 0) & 0x0F0F;
	b = (result >> 4) & 0x0F0F;
	result = a + b;
	a = (result >> 0) & 0x00FF;
	b = (result >> 8) & 0x00FF;
	result = a + b;

	return result;
}

void SRAM_Get_Performance_Measures(){
	// if no write access was performed => warning
	if(write_mode == 0xFF){
		printf("Caution. No write operation was performed. Therefore we have no values to compare with...\r\nExit\r\n\n");
		
		return;
	}

	// reset the counter for statistical analysis
	
	init_counter();
	
//	total_one = 0;
//	total_zero = 0;
//	flipped_one = 0;
//	flipped_zero = 0;

	uint32_t expected_value;
	uint32_t start_local = 0;
	//uint32_t SRAM_SIZE = (uint32_t)sram_end - (uint32_t)sram_start;
	uint32_t end_local = SRAM_SIZE;

	// switch between the different command modes to set the correct values to compare with
	
	switch(write_mode){
	case 1:
		// writeZeroAll
		expected_value = 0x0000000;
		break;
	case 2:
		// writeOneAll
		expected_value = 0xFFFFFFFF;
		break;
	case 3:
		// writeOneAll
		expected_value = 0x12345678;
		break;
	case 4:
		// writeSRAM
		expected_value = start_value;
		start_local = sram_start;
		end_local = sram_start;
		break;
	case 5:
		// writeSRAM
		expected_value = start_value;
		start_local = sram_start;
		end_local = sram_start;
		break;
	}
//	switch(write_mode){
//	case 1:
//		// writeZeroAll
//		expected_value = 0x0;
//		break;
//	case 2:
//		// writeOneAll
//		expected_value = 0xFFFF;
//		break;
//	case 3:
//		// writeValueAsc
//		expected_value = start_value;
//		break;
//	case 4:
//		// writeAlternateZeroOne
//		expected_value = 0x5555;
//		break;
//	case 5:
//		// writeAlternateOneZero
//		expected_value = 0xAAAA;
//		break;
//	case 6:
//		// writeSRAM
//		expected_value = start_value;
//		start_local = start_adr;
//		end_local = end_adr;
//		break;
//	case 7:
//		// writeSRAMRange
//		expected_value = start_value;
//		start_local = start_adr;
//		end_local = end_adr;
//		break;
//	}
	//expected_value = 0x00FF;
	printf("Expected value : %x\r\n",expected_value);
	for(uint32_t adr = start_local; adr < end_local; adr++){
		uint16_t real_value;
		real_value = SRAM_Read_16b(adr);
		flipped_one += flipped_one_16b(expected_value, real_value);
		flipped_zero += flipped_zero_16b(expected_value, real_value);
		total_one += get_num_one_16b(expected_value);
		total_zero += get_num_zero_16b(expected_value);
		// if 'writeValueAsc' increment the expected_value
		if(write_mode == 3){
			expected_value++;
		}
	}

	// prevent division by zero
	if(total_one == 0){
		printf( "P(1->0) = NaN\r\n");

	}else{
		// displays the probability that a 1 changes to a 0
		float p_one_to_zero = ((float)flipped_one / (float)total_one) * 100;
		printf( "P(1->0) = %.16f%%\r\n", p_one_to_zero);
		
	}

	if(total_zero == 0){
		printf( "P(0->1) = NaN\r\n");
		
	}else{
		// displays the probability that a 0 changes to a 1
		float p_zero_to_one = ((float)flipped_zero / (float)total_zero) * 100;
		printf( "P(0->1) = %.16f%%\r\n", p_zero_to_one);
		
	}
	float p_total_flip_probability = ((float)(flipped_one + flipped_zero) / (float)(total_one + total_zero)) * 100;

	// displays the total flip probability
	printf( "P(flip) = %.16f%%\r\n", p_total_flip_probability);
	

	// displays the total flip 1's
	printf( "Total number of flipped 1->0:  %lu\r\n", (unsigned long)flipped_one);
	

	// displays the total 1's
	printf( "Total number of ones in expected value:  %lu\r\n", (unsigned long)total_one);
	

	// displays the total flip 0's
	printf( "Total number of flipped 0->1:  %lu\r\n", (unsigned long)flipped_zero);
	

	// displays the total 0's
	printf( "Total number of zeros in expected value:  %lu\r\n", (unsigned long)total_zero);
	

	// displays the total flipped bits
	printf( "Total number of flipped bits:  %lu\r\n\n", (unsigned long)(flipped_one + flipped_zero));
	

	/*
	//sprintf(STRING_BUFFER, "total_one = %lu\r\n\n", (unsigned long)total_one);
	sprintf(STRING_BUFFER, "write_mode = %d\r\n\n", write_mode);
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
	 */

}

//void SRAM_Get_Values(UART_HandleTypeDef *huart){
//	total_one = 0;
//	total_zero = 0;
//	uint32_t SRAM_SIZE = (uint32_t)sram_end - (uint32_t)sram_start;
//	for(uint32_t adr = 0; adr < SRAM_SIZE; adr++){
//		uint16_t real_value;
//		real_value = SRAM_Read_16b(adr);
//		total_one += get_num_one_16b(real_value);
//		total_zero += get_num_zero_16b(real_value);
//	}


//	// displays the total 1's
//	printf( "Total number of ones in SRAM:  %lu\r\n", (unsigned long)total_one);
//	
//	// displays the total 0's
//	printf( "Total number of zeros in SRAM:  %lu\r\n", (unsigned long)total_zero);
//	
//	// displays the total 0's
//	printf( "Percent zeros:  %.16f%%\r\n", (float)total_zero / (SRAM_SIZE * 16) * 100);
//	
//	// displays the total 0's
//	printf( "Percent ones:  %.16f%%\r\n", (float)total_one / (SRAM_SIZE * 16) * 100);
//	
//}

/*
 * @brief							resets the counters for statistical analysis
 */
void init_counter(){
	total_one = 0;
	total_zero = 0;
	flipped_one = 0;
	flipped_zero = 0;
}

/*
 * @brief							resets the custom start/end addresses and the custom start value
 * 									these variables are set in a write method
 */
void init_arguments(){
	start_adr = 0;
	end_adr = 0;
	start_value = 0;
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
