/**
  ******************************************************************************
  * File Name          : tud_seceng.h
  * Description        : This file contains the common defines of the methods
  ******************************************************************************
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRAM_FUNCTION_H
#define __SRAM_FUNCTION_H

#include "stm32f4xx_hal.h"

typedef enum {
	FAILED = 0,
	PASSED = !FAILED
} TestStatus; // Typedef for correct read/write tests

// private defines
	int expression;
	int counter;
	clock_t tic = 0;
	clock_t toc = 0;
	
	uint32_t sram_start = 0x64000000;
	uint32_t sram_end = 0x64800000;
	uint32_t *ptr = (uint32_t *)0x64000000;
	//uint32_t sram_end = 0x64800000;
	uint32_t zero = 0x00000000;
	uint32_t ones = 0xffffffff;
	uint32_t write = 0x12345678;

// global string buffer


// receive buffer


// probability counter
// 1 MB RAM => 1 million 1's and 0's possible => uint32_t




// functions to access the SRAM
//void SRAM_Write_16b(uint32_t adr, uint16_t value);
//uint16_t SRAM_Read_16b(uint32_t adr);

// uart transmit and receive functions


// user functions
void write_all_zero(void);			//writes zero to entire memory location
void write_all_ones(void);			//writes ones to entire memory location
void write_all_12345678(void);		//writes 12345678 to entire memory location
void sram_read(void);				//reads entire memory location
void sram_read_1(void);				//latency_read


// helper functions

#endif /* __SRAM_FUNCTION_H */
