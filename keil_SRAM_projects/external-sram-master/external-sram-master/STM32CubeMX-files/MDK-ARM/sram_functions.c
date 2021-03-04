#include "main.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "fmc.h"
#include "sram_functions.h"

#include <stdio.h>  //printf
#include <string.h>
#include <time.h>

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

void write_all_zero(void)  	//writes zero to entire memory location
{
	//uint32_t *ptr = (uint32_t *)0x64000000;
  //uint32_t zero = 0x00000000;
	//tic = clock();
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

void write_all_ones(void)		//writes ones to entire memory location
{
	//uint32_t *ptr = (uint32_t *)0x64000000;
  //uint32_t ones = 0x11111111;
	//tic = clock();
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

void sram_read(void)
{
	counter = 0;
	//tic = clock();
	uint32_t now = HAL_GetTick();
	//uint32_t *ptr = (uint32_t *)0x64000000;
	for(ptr=(uint32_t *)0x64000000; ptr < (uint32_t *)0x64000fff; ptr++ )
  {		
		counter++;
		//printf("%p:%d:%p\n",(void*)&ptr,(void*)*ptr,start);
    printf("%p:%x\r\n",ptr,*ptr);
    //printf("%x\n",*ptr);
		//HAL_Delay(500);
       
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
//	printf("\r\nElapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
}

void sram_read_1(void)
{
	counter = 0;
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
