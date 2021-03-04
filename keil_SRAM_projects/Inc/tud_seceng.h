/**
  ******************************************************************************
  * File Name          : tud_seceng.h
  * Description        : This file contains the common defines of the methods
  ******************************************************************************
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TUD_SECENG_H
#define __TUD_SECENG_H

#include "stm32f4xx_hal.h"

typedef enum {
	FAILED = 0,
	PASSED = !FAILED
} TestStatus; // Typedef for correct read/write tests

// private defines
#define SRAM_BANK_ADDR		((uint32_t)0x64000000)		// We use Bank 3, Block 2, thus FMC_NE2 and 0x64000000
#define SRAM_SIZE			((uint32_t)0x40000) 		// AS7C34098A has 2^18 address indices
//#define SRAM_SIZE			((uint32_t)0x80000) 		// Cypress CY62157EV30 has 2^19 address indices
#define BUFFER_SIZE			100							// buffer size for the global string buffer
#define SRAM_BUFFER_SIZE	40960						// buffer size for the sram buffer
#define COMMAND_COUNT		15

// global string buffer
char STRING_BUFFER[BUFFER_SIZE];
char SRAM_BUFFER[SRAM_BUFFER_SIZE];
char *srambp;

uint16_t len;
uint16_t old_len;

// receive buffer
uint8_t Rx_Index;
char Rx_Data[2];
char Rx_Buffer[100];
char Transfer_cplt;

// probability counter
// 1 MB RAM => 1 million 1's and 0's possible => uint32_t
uint32_t total_one;
uint32_t total_zero;
uint32_t flipped_one;
uint32_t flipped_zero;



// functions to access the SRAM
void SRAM_Write_16b(uint32_t adr, uint16_t value);
uint16_t SRAM_Read_16b(uint32_t adr);

// uart transmit and receive functions
extern void send(UART_HandleTypeDef *huart, uint8_t *srcBuffer, uint32_t bufferSize);
extern void receive(UART_HandleTypeDef *huart, uint8_t *dstBuffer, uint32_t bufferSize);

extern void showHelp(UART_HandleTypeDef *huart);
extern void executeCommand(UART_HandleTypeDef *huart);

// user functions
void SRAM_Fill_With_Zeros(UART_HandleTypeDef *huart);
void SRAM_Fill_With_Ones(UART_HandleTypeDef *huart);
void SRAM_Get_Performance_Measures(UART_HandleTypeDef *huart);
void SRAM_Write_Ascending(UART_HandleTypeDef *huart, uint32_t *arguments);
void SRAM_Write_Alternate_Zero_One(UART_HandleTypeDef *huart);
void SRAM_Write_Alternate_One_Zero(UART_HandleTypeDef *huart);
void SRAM_Check_Read_Write_Status(UART_HandleTypeDef *huart);
void SRAM_Get_Whole_Content(UART_HandleTypeDef *huart);
void SRAM_Get_Values(UART_HandleTypeDef *huart);
void SRAM_Check_Address(UART_HandleTypeDef *huart, uint32_t *arguments);
void SRAM_Check_Address_Range(UART_HandleTypeDef *huart, uint32_t *arguments);


// helper functions
void clearBuffer(uint8_t index);
uint16_t hamming_weight_16b(uint16_t bitstring);
void init_counter(void);
void tokenize_arguments(char *args);
uint16_t get_num_one_16b(uint16_t bitstring);
uint16_t get_num_zero_16b(uint16_t bitstring);
uint16_t flipped_one_16b(uint16_t expected_value, uint16_t real_value);
uint16_t flipped_zero_16b(uint16_t expected_value, uint16_t real_value);
uint16_t get_total_flip_probability_16b(uint16_t expected_value, uint16_t real_value);
void init_arguments(void);
uint8_t get_space(char *rx_buffer);
#endif /* __TUD_SECENG_H */
