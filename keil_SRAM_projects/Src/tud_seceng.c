/**
  ******************************************************************************
  * File Name          : tud_seceng.c
  * Description        : Methods for experiments
  ******************************************************************************
*/
#include "tud_seceng.h"
#include <string.h>
#include <stdlib.h>

// commands
uint8_t command_mode = 0xFF;
uint8_t write_mode = 0xFF;
uint16_t start_value = 0x0; // start value for ascending writing
uint32_t start_adr = 0x0;	// start address for writeSRAMRange
uint32_t end_adr = 0x0;		// end address for writeSRAMRange
uint32_t arguments[3];
// commands initialization
/*
 * @brief: 	to add new commands, add the command to the array 'command'
 * 			increase the variable 'COMMAND_COUNT' in 'tud_seceng.h'
 * 			add a brief description for the command at the corresponding array index in the array 'command_help'
 *
 * 			Note that the value 0xFF is reserved in the arrays 'command_mode' and 'write_mode'
 *
 * 			If you need more than 3 arguments, increase the array size of 'arguments' to the required value
 */
char * command[] = {
							"help",
							"0",
							"1",
							"writeValueAsc",
							"writeAlternateZeroOne",
							"writeAlternateOneZero",
							"writeSRAM",
							"writeSRAMRange",
							"getPerformanceMeasures",
							"getAddress",
							"readSRAM",
							"checkSRAM",
							"checkAddress",
							"checkAddressRange",
							"v"
						 };

// command help initialization
const char *command_help[] = {
							"'help' Shows this site\r",
							"'0' writes 0's to the whole SRAM\r",
							"'1' writes 1's to the whole SRAM\r",
							"'writeValueAsc val' writes the 16-bit value 'val' to the first address and increments the value by 1\r",
							"'writeAlternateZeroOne' writes alternating 01010101... to the whole SRAM\r",
							"'writeAlternateOneZero' writes alternating 10101010... to the whole SRAM\r",
							"'writeSRAM adr val' writes the 16-bit value 'val' to address 'adr'\r",
							"'writeSRAMRange start end val' writes the 16-bit value 'val' to addresses 'start' to 'end'\r",
							"'getPerformanceMeasures' returns the probabilities P(0->1), P(1->0) and total flip probability (= HammingDistance / SRAM_size)\r",
							"'getAddress adr' returns the value at the address 'adr'\r",
							"'readSRAM' prints the whole content of the SRAM as hexadecimal value to the console (use Putty-logging for file-saving)\r",
							"'checkSRAM' checks, whether the expected value matches the real values of the whole SRAM\r",
							"'checkAddress adr val' checks, whether the value 'val' is equal to the content at address 'adr'\r",
							"'checkAddressRange start end val' checks, whether the value 'val' is equal to the content at the addresses in range 'start' to 'end'\r",
							"'v' returns the number of zeros and ones\n\r"
};




void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);


// implementation of the functions
/*
 * @brief					writes a 16-Bit(2-byte-word) value to the specified address to SRAM
 * @param uint32_t adr		relative address to the base address (specified as macro 'SRAM_BANK_ADDR') of SRAM
 * 							to be written to
 * @param uint16_t value	value to be written to the specified address in SRAM
 * @retval					None
 */
void SRAM_Write_16b(uint32_t adr, uint16_t value){
	// multiply address by 2 due to 2-byte-access (address is given as one byte)
	adr = adr << 1;

	*(__IO uint16_t *) (SRAM_BANK_ADDR + adr) = value;
}

/*
 * @brief					reads a 16-Bit(2-byte-word) value from the specified address from SRAM
 * @param uint32_t adr		relative address to the base address (specified as macro 'SRAM_BANK_ADDR') of SRAM
 * 							to be read from
 * @retval uint16_t			value at address in SRAM
 */
uint16_t SRAM_Read_16b(uint32_t adr){
	// initialize the return value
	uint16_t ret_wert = 0;
	// multiply address by 2 due to 2-byte-access (address is given as one byte)
	adr = adr << 1;

	ret_wert = *(__IO uint16_t *) (SRAM_BANK_ADDR + adr);

	return (ret_wert);
}

/*
 * @brief								fills the whole SRAM with 0's
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 */
void SRAM_Fill_With_Zeros(UART_HandleTypeDef *huart){
	write_mode = 0x1;
	// reset the counter for statistical analysis
	init_counter();
	// reset the arguments
	init_arguments();

	uint16_t real_value = 0xFFFF;
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
	sprintf(STRING_BUFFER, "\n\r~~~~~~~~~~~~~~~\n\r");
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
}

/*
 * @brief								fills the whole SRAM with 1's
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 */
void SRAM_Fill_With_Ones(UART_HandleTypeDef *huart){
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
	sprintf(STRING_BUFFER, "\n\r~~~~~~~~~~~~~~~\n\r");
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
}

void SRAM_Get_Values(UART_HandleTypeDef *huart){
	total_one = 0;
	total_zero = 0;
	for(uint32_t adr = 0; adr < SRAM_SIZE; adr++){
		uint16_t real_value;
		real_value = SRAM_Read_16b(adr);
		total_one += get_num_one_16b(real_value);
		total_zero += get_num_zero_16b(real_value);
	}


	// displays the total 1's
	sprintf(STRING_BUFFER, "Total number of ones in SRAM:  %lu\r\n", (unsigned long)total_one);
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);

	// displays the total 0's
	sprintf(STRING_BUFFER, "Total number of zeros in SRAM:  %lu\r\n", (unsigned long)total_zero);
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);

	// displays the total 0's
	sprintf(STRING_BUFFER, "Percent zeros:  %.16f%%\r\n", (float)total_zero / (SRAM_SIZE * 16) * 100);
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);

	// displays the total 0's
	sprintf(STRING_BUFFER, "Percent ones:  %.16f%%\r\n", (float)total_one / (SRAM_SIZE * 16) * 100);
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
}

/*
 * @brief								gets the probability of flipped 0's and 1's for the written values
 * 										displays an error message and exits, if no write operation was performed
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 */
void SRAM_Get_Performance_Measures(UART_HandleTypeDef *huart){
	// if no write access was performed => warning
	if(write_mode == 0xFF){
		sprintf(STRING_BUFFER, "Caution. No write operation was performed. Therefore we have no values to compare with...\r\nExit\r\n\n");
		len = strlen(STRING_BUFFER);
		send(huart, (uint8_t *)STRING_BUFFER, len);
		return;
	}

	// reset the counter for statistical analysis
	init_counter();

	uint16_t expected_value;
	uint32_t start_local = 0;
	uint32_t end_local = SRAM_SIZE;

	// switch between the different command modes to set the correct values to compare with
	switch(write_mode){
	case 1:
		// writeZeroAll
		expected_value = 0x0;
		break;
	case 2:
		// writeOneAll
		expected_value = 0xFFFF;
		break;
	case 3:
		// writeValueAsc
		expected_value = start_value;
		break;
	case 4:
		// writeAlternateZeroOne
		expected_value = 0x5555;
		break;
	case 5:
		// writeAlternateOneZero
		expected_value = 0xAAAA;
		break;
	case 6:
		// writeSRAM
		expected_value = start_value;
		start_local = start_adr;
		end_local = end_adr;
		break;
	case 7:
		// writeSRAMRange
		expected_value = start_value;
		start_local = start_adr;
		end_local = end_adr;
		break;
	}
	//expected_value = 0x00FF;
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
		sprintf(STRING_BUFFER, "P(1->0) = NaN\r\n");
		len = strlen(STRING_BUFFER);
		send(huart, (uint8_t *)STRING_BUFFER, len);
	}else{
		// displays the probability that a 1 changes to a 0
		float p_one_to_zero = ((float)flipped_one / (float)total_one) * 100;
		sprintf(STRING_BUFFER, "P(1->0) = %.16f%%\r\n", p_one_to_zero);
		len = strlen(STRING_BUFFER);
		send(huart, (uint8_t *)STRING_BUFFER, len);
	}

	if(total_zero == 0){
		sprintf(STRING_BUFFER, "P(0->1) = NaN\r\n");
		len = strlen(STRING_BUFFER);
		send(huart, (uint8_t *)STRING_BUFFER, len);
	}else{
		// displays the probability that a 0 changes to a 1
		float p_zero_to_one = ((float)flipped_zero / (float)total_zero) * 100;
		sprintf(STRING_BUFFER, "P(0->1) = %.16f%%\r\n", p_zero_to_one);
		len = strlen(STRING_BUFFER);
		send(huart, (uint8_t *)STRING_BUFFER, len);
	}
	float p_total_flip_probability = ((float)(flipped_one + flipped_zero) / (float)(total_one + total_zero)) * 100;

	// displays the total flip probability
	sprintf(STRING_BUFFER, "P(flip) = %.16f%%\r\n", p_total_flip_probability);
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);

	// displays the total flip 1's
	sprintf(STRING_BUFFER, "Total number of flipped 1->0:  %lu\r\n", (unsigned long)flipped_one);
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);

	// displays the total 1's
	sprintf(STRING_BUFFER, "Total number of ones in expected value:  %lu\r\n", (unsigned long)total_one);
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);

	// displays the total flip 0's
	sprintf(STRING_BUFFER, "Total number of flipped 0->1:  %lu\r\n", (unsigned long)flipped_zero);
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);

	// displays the total 0's
	sprintf(STRING_BUFFER, "Total number of zeros in expected value:  %lu\r\n", (unsigned long)total_zero);
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);

	// displays the total flipped bits
	sprintf(STRING_BUFFER, "Total number of flipped bits:  %lu\r\n\n", (unsigned long)(flipped_one + flipped_zero));
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);

	/*
	//sprintf(STRING_BUFFER, "total_one = %lu\r\n\n", (unsigned long)total_one);
	sprintf(STRING_BUFFER, "write_mode = %d\r\n\n", write_mode);
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
	 */

}

/*
 * @brief								fills the first address SRAM with the argument and increment it by one
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 * @param uint32_t *arguments			the start value to count up
 */
void SRAM_Write_Ascending(UART_HandleTypeDef *huart, uint32_t *arguments){
	// reset the counter for statistical analysis
	init_counter();
	// reset the arguments
	init_arguments();

	start_value = (uint16_t)arguments[0];
	uint16_t real_value = 0x0;
	TestStatus state = PASSED;
	for(uint32_t adr = 0; adr < SRAM_SIZE; adr++){
		SRAM_Write_16b(adr, start_value);
		real_value = SRAM_Read_16b(adr);
		if(real_value != start_value){
			state = FAILED;
			break;
		}
		start_value++;
	}
	if(state == PASSED){
		sprintf(STRING_BUFFER, "'writeValueAsc' was successful\r\n\n");
	}else{
		sprintf(STRING_BUFFER, "'writeValueAsc' failed\r\n\n");
	}
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
}

/*
 * @brief								fills the whole SRAM with alternating 010101010....
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 */
void SRAM_Write_Alternate_Zero_One(UART_HandleTypeDef *huart){
	// reset the counter for statistical analysis
	init_counter();
	// reset the arguments
	init_arguments();

	uint16_t real_value = 0x0;
	TestStatus state = PASSED;
	for(uint32_t adr = 0; adr < SRAM_SIZE; adr++){
		SRAM_Write_16b(adr, 0x5555);
		real_value = SRAM_Read_16b(adr);
		if(real_value != 0x5555){
			state = FAILED;
			break;
		}
	}
	if(state == PASSED){
		sprintf(STRING_BUFFER, "'writeAlternateZeroOne' was successful\r\n\n");
	}else{
		sprintf(STRING_BUFFER, "'writeAlternateZeroOne' failed\r\n\n");
	}
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
}

/*
 * @brief								fills the whole SRAM with alternating 101010101....
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 */
void SRAM_Write_Alternate_One_Zero(UART_HandleTypeDef *huart){
	// reset the counter for statistical analysis
	init_counter();
	// reset the arguments
	init_arguments();

	uint16_t real_value = 0x0;
	TestStatus state = PASSED;
	for(uint32_t adr = 0; adr < SRAM_SIZE; adr++){
		SRAM_Write_16b(adr, 0xAAAA);
		real_value = SRAM_Read_16b(adr);
		if(real_value != 0xAAAA){
			state = FAILED;
			break;
		}
	}
	if(state == PASSED){
		sprintf(STRING_BUFFER, "'writeAlternateOneZero' was successful\r\n\n");
	}else{
		sprintf(STRING_BUFFER, "'writeAlternateOneZero' failed\r\n\n");
	}
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
}

/*
 * @brief								writes the value (second entry in parameter arguments) to the
 * 										specified address (first entry in parameter arguments)
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 * @param uint32_t *arguments			first element is the address, second element is the value
 */
void SRAM_Write_Address(UART_HandleTypeDef *huart, uint32_t *arguments){
	// reset the counter for statistical analysis
	init_counter();
	// reset the arguments
	init_arguments();

	// set the variables for checking and determining the statistical values
	start_adr = arguments[0];
	end_adr = start_adr + 1;
	start_value = (uint16_t)arguments[1];
	SRAM_Write_16b(start_adr, start_value);
	uint16_t real_value = SRAM_Read_16b(start_adr);
	TestStatus state = PASSED;
	if(real_value != start_value){
		state = FAILED;
	}
	if(state == PASSED){
		sprintf(STRING_BUFFER, "'writeSRAM' was successful\r\n\n");
	}else{
		sprintf(STRING_BUFFER, "'writeSRAM' failed\r\n\n");
	}
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
}

/*
 * @brief								writes the value (third entry in parameter arguments) to the
 * 										specified addresses (from first entry to second entry
 * 										in parameter arguments)
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 * @param uint32_t *arguments			first element is the start address, second element is the end address
 * 										third element is the value
 */
void SRAM_Write_Address_Range(UART_HandleTypeDef *huart, uint32_t *arguments){
	// reset the counter for statistical analysis
	init_counter();
	// reset the arguments
	init_arguments();

	// set the variables for checking and determining the statistical values
	// correct the end_adr for the loop (< not <=)
	start_adr = arguments[0];
	end_adr = arguments[1] + 1;
	start_value = (uint16_t)arguments[2];
	uint16_t real_value;
	TestStatus state = PASSED;
	if(start_adr < SRAM_SIZE && end_adr <= SRAM_SIZE){
		start_adr = arguments[0];
		end_adr = arguments[1] + 1;
		for(uint32_t adr = start_adr; adr < end_adr; adr++){
			SRAM_Write_16b(adr, start_value);
			real_value = SRAM_Read_16b(adr);
			if(real_value != start_value){
				state = FAILED;
				break;
			}
		}
	}else{
		state = FAILED;
	}
	if(state == PASSED){
		sprintf(STRING_BUFFER, "'writeSRAMRange' was successful\r\n\n");
	}else{
		sprintf(STRING_BUFFER, "'writeSRAMRange' failed\r\n\n");
	}
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
}

/*
 * @brief								reads the whole SRAM and print it as hexadecimal values
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 */
void SRAM_Read_SRAM(UART_HandleTypeDef *huart){
	uint16_t real_value;
	uint16_t counter = 0;
	srambp = SRAM_BUFFER;
	for(uint32_t adr = 0; adr < SRAM_SIZE; adr++){
		real_value = SRAM_Read_16b(adr);
		srambp += sprintf(srambp, "%04X\t", real_value);
		counter++;
		if(counter == 8192){
			//STRING_BUFFER + sprintf(STRING_BUFFER, (char *)output, real_value);
			len = strlen(SRAM_BUFFER);
			send(huart, (uint8_t *)SRAM_BUFFER, len);
			counter = 0;
			srambp = SRAM_BUFFER;
		}
	}
}

/*
 * @brief								prints the address and the value at address (first entry in
 * 										parameter arguments)
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 * @param uint32_t *arguments			the address to read from
 */
void SRAM_Get_Address(UART_HandleTypeDef *huart, uint32_t *arguments){
	uint32_t adr = arguments[0];
	uint16_t real_value = 0;
	real_value = SRAM_Read_16b(adr);

	sprintf(STRING_BUFFER, "Address: %lu\tValue: %#06X\n\n\r", (unsigned long)adr, real_value);
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
}

/*
 * @brief								checks if the expected value (second entry in parameter arguments)
 * 										equals the value stored at address specified in first entry in
 * 										parameter arguments
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 * @param uint32_t *arguments			the address to read from, the value to compare with
 */
void SRAM_Check_Address(UART_HandleTypeDef *huart, uint32_t *arguments){
	uint32_t adr = arguments[0];
	uint16_t expected_value = (uint16_t)arguments[1];
	uint16_t real_value = SRAM_Read_16b(adr);
	TestStatus state = PASSED;

	if(real_value != expected_value){
		state = FAILED;
	}

	if(state == PASSED){
		//sprintf(STRING_BUFFER, "%#06X == %#06X\n\rTRUE\n\n\r", real_value, expected_value);
		sprintf(STRING_BUFFER, "TRUE\n\n\r");
	}else{
		sprintf(STRING_BUFFER, "%#06X != %#06X\n\rFALSE\n\n\r", real_value, expected_value);
	}
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
}

/*
 * @brief								checks if the expected value (third entry in parameter arguments)
 * 										equals the value stored at address specified in first entry in
 * 										parameter arguments to address specified in second entry in
 * 										parameter arguments
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 * @param uint32_t *arguments			the start address to read from, the end address to read from
 * 										the value to compare with
 */
void SRAM_Check_Address_Range(UART_HandleTypeDef *huart, uint32_t *arguments){
	uint32_t start_local = arguments[0];
	uint32_t end_local = arguments[1];
	uint16_t expected_value_local = (uint16_t)arguments[2];
	uint16_t real_value_local = 0;
	TestStatus state = PASSED;
	if(start_local <= end_local){
		if(start_local < SRAM_SIZE && end_local <= SRAM_SIZE){
			for(uint32_t adr = start_local; adr < end_local; adr++){
				real_value_local = SRAM_Read_16b(adr);
				if(real_value_local != expected_value_local){
					state = FAILED;
					sprintf(STRING_BUFFER, "Address: %lu\t%#06X != %#06X\n\r", (unsigned long)adr, real_value_local, expected_value_local);
					len = strlen(STRING_BUFFER);
					send(huart, (uint8_t *)STRING_BUFFER, len);
				}
			}
		}else{
			state = FAILED;
		}
	}else{
		state = FAILED;
	}

	if(state == PASSED){
		//sprintf(STRING_BUFFER, "%#06X == %#06X\n\rTRUE\n\n\r", real_value, expected_value);
		sprintf(STRING_BUFFER, "TRUE\n\n\r");
	}else{
		sprintf(STRING_BUFFER, "FALSE\n\n\r");
	}
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
}


/*
 * @brief								checks if the expected value (third entry in parameter arguments)
 * 										equals the value stored at address specified in first entry in
 * 										parameter arguments to address specified in second entry in
 * 										parameter arguments
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 * @param uint32_t *arguments			the start address to read from, the end address to read from
 * 										the value to compare with
 */
void SRAM_Check_Read_Write_Status(UART_HandleTypeDef *huart){

	// if no write access was performed => warning
	if(write_mode == 0xFF){
		sprintf(STRING_BUFFER, "Caution. No write access was performed. Therefore we have no values to compare with...\r\nExit\r\n\n");
		len = strlen(STRING_BUFFER);
		send(huart, (uint8_t *)STRING_BUFFER, len);
		return;
	}

	uint16_t real_value = 0;

	uint16_t expected_value;
	uint32_t start_local = 0;
	uint32_t end_local = SRAM_SIZE;

	// switch between the different command modes to set the correct values to compare with
	switch(write_mode){
	case 1:
		// writeZeroAll
		expected_value = 0x0;
		break;
	case 2:
		// writeOneAll
		expected_value = 0xFFFF;
		break;
	case 3:
		// writeValueAsc
		expected_value = start_value;
		break;
	case 4:
		// writeAlternateZeroOne
		expected_value = 0x5555;
		break;
	case 5:
		// writeAlternateOneZero
		expected_value = 0xAAAA;
		break;
	case 6:
		// writeSRAM
		expected_value = start_value;
		start_local = start_adr;
		end_local = end_adr;
		break;
	case 7:
		// writeSRAMRange
		expected_value = start_value;
		start_local = start_value;
		end_local = end_adr;
		break;
	}

	TestStatus state = PASSED;
	if(start_local <= SRAM_SIZE && end_local <= SRAM_SIZE){
		for(uint32_t adr = start_local; adr < end_local; adr++){
			real_value = SRAM_Read_16b(adr);
			if(real_value != expected_value){
				state = FAILED;
				sprintf(STRING_BUFFER, "Address: %lu\t%#06X != %#06X\n\r", (unsigned long)adr, real_value, expected_value);
				len = strlen(STRING_BUFFER);
				send(huart, (uint8_t *)STRING_BUFFER, len);
			}
			if(write_mode == 3){
				expected_value++;
			}
		}
	}else{
		state = FAILED;
	}

	if(state == PASSED){
		//sprintf(STRING_BUFFER, "%#06X == %#06X\n\rTRUE\n\n\r", real_value, expected_value);
		sprintf(STRING_BUFFER, "TRUE\n\n\r");
	}else{
		sprintf(STRING_BUFFER, "FALSE\n\n\r");
	}
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
}

/*
 * @brief								rewritten function to receive with a delay of 10ms
 * @param UART_HandleTypeDef *huart		the UART handler to communicate with the user
 * @param uint8_t *dstBuffer			the destination buffer
 * @param uint32_t bufferSize			the buffer size
 */
void receive(UART_HandleTypeDef *huart, uint8_t *dstBuffer, uint32_t bufferSize){
	HAL_UART_Receive_IT(huart, dstBuffer, bufferSize);
	HAL_Delay(10);
}

/*
 * @brief								rewritten function to transmit with a delay of 10ms
 * @param UART_HandleTypeDef *huart		the UART handler to communicate with the user
 * @param uint8_t *dstBuffer			the source buffer
 * @param uint32_t bufferSize			the buffer size
 */
void send(UART_HandleTypeDef *huart, uint8_t *srcBuffer, uint32_t bufferSize){
	HAL_UART_Transmit_IT(huart, srcBuffer, bufferSize);
	if(command_mode == 0xA){
		HAL_Delay(3500);
	}else{
		HAL_Delay(20);
	}
}

/*
 * @brief								prints the help for the commands
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 */
void showHelp(UART_HandleTypeDef *huart){
	sprintf(STRING_BUFFER, "\rThis program provides the following commands:\n\n\r");
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);
	for(uint8_t i = 0; i < COMMAND_COUNT; i++){
		sprintf(STRING_BUFFER, "%s\r\n", command_help[i]);
		len = strlen(STRING_BUFFER);
		send(huart, (uint8_t *)STRING_BUFFER, len);
	}
}

/*
 * @brief								parse the command, set the required variables, parse the arguments
 * @param UART_HandleTypeDef huart*		the UART handler to communicate with the user
 */
void executeCommand(UART_HandleTypeDef *huart){
	command_mode = 0xFF; // invalid command
	// parse command
	for(uint8_t i = 0; i < COMMAND_COUNT; i++){
		// check if the command equals a command specified in the array 'command'
		// if so set command_mode different from 0xFF
		uint8_t command_end_index = get_space(Rx_Buffer);
		if((uint8_t)strlen(command[i]) == command_end_index && strncmp(command[i], Rx_Buffer, command_end_index) == 0){
			command_mode = i;
			char tmp[30];
			uint16_t len_rx_buffer = strlen(Rx_Buffer);
			uint16_t len_command = strlen(command[i]);
			// if there are arguments after the command
			if(len_rx_buffer - len_command > 0){
				// extract the arguments from the string
				strncpy(tmp, Rx_Buffer + len_command, len_rx_buffer - len_command);
				// tokenize the arguments and fill the array 'arguments'
				tokenize_arguments(tmp);
			}
			break;
		}
	}

	//sprintf(STRING_BUFFER, "Length: %d, String: %d %d %d %d\r\n", (uint16_t)strlen(Rx_Buffer), Rx_Buffer[0], Rx_Buffer[1], Rx_Buffer[2], Rx_Buffer[3]);
	// prints the command
	sprintf(STRING_BUFFER, ">\r\n");
	len = strlen(STRING_BUFFER);
	send(huart, (uint8_t *)STRING_BUFFER, len);

	// reset the counter before every write execution
	//
	switch(command_mode){
	case 0x0:
		// no write operation will be performed in this method
		//write_mode = 0xFF;
		return showHelp(huart);
	case 0x1:
		// write operation in mode 1 will be performed in this method
		// therefore reset the counters/arguments
		// they will be set in the function
		write_mode = 0x1;
		return SRAM_Fill_With_Zeros(huart);
	case 0x2:
		// write operation in mode 2 will be performed in this method
		// therefore reset the counters/arguments
		// they will be set in the function
		write_mode = 0x2;
		return SRAM_Fill_With_Ones(huart);
	case 0x3:
		// write operation in mode 3 will be performed in this method
		// therefore reset the counters/arguments
		// they will be set in the function
		write_mode = 0x3;
		return SRAM_Write_Ascending(huart, arguments);
	case 0x4:
		// write operation in mode 4 will be performed in this method
		// therefore reset the counters/arguments
		// they will be set in the function
		write_mode = 0x4;
		return SRAM_Write_Alternate_Zero_One(huart);
	case 0x5:
		// write operation in mode 5 will be performed in this method
		// therefore reset the counters/arguments
		// they will be set in the function
		write_mode = 0x5;
		return SRAM_Write_Alternate_One_Zero(huart);
	case 0x6:
		// write operation in mode 6 will be performed in this method
		// therefore reset the counters/arguments
		// they will be set in the function
		write_mode = 0x6;
		return SRAM_Write_Address(huart, arguments);
	case 0x7:
		// write operation in mode 7 will be performed in this method
		// therefore reset the counters/arguments
		// they will be set in the function
		write_mode = 0x7;
		return SRAM_Write_Address_Range(huart, arguments);
	case 0x8:
		// no write operation will be performed in this method
		// reset the counter for statistical analysis
		//write_mode = 0xFF;
		return SRAM_Get_Performance_Measures(huart);
	case 0x9:
		// no write operation will be performed in this method
		// reset the counter for statistical analysis
		//write_mode = 0xFF;
		return SRAM_Get_Address(huart, arguments);
	case 0xA:
		// no write operation will be performed in this method
		//write_mode = 0xFF;
		return SRAM_Read_SRAM(huart);
		break;
	case 0xB:
		// no write operation will be performed in this method
		//write_mode = 0xFF;
		return SRAM_Check_Read_Write_Status(huart);
	case 0xC:
		// no write operation will be performed in this method
		//write_mode = 0xFF;
		return SRAM_Check_Address(huart, arguments);
	case 0xD:
		// no write operation will be performed in this method
		//write_mode = 0xFF;
		return SRAM_Check_Address_Range(huart, arguments);
	case 0xE:
		return SRAM_Get_Values(huart);
	default:
		sprintf(STRING_BUFFER, "Command not found. Type 'help' to show all valid commands.\n\n\r");
		len = strlen(STRING_BUFFER);
		send(huart, (uint8_t *)STRING_BUFFER, len);
	}
}


/*
 * @brief								callback function called if something is received via UART
 * @param UART_HandleTypeDef *huart		the UART handler to communicate with the user
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	// clear Rx_Buffer before receiving new data
	if(huart->Instance == UART4){
		if(Rx_Index == 0){
			clearBuffer(0);
		}
		// if received data different from ascii 13 (Enter)
		if(Rx_Data[0] != 13){
			// only allow 0-9, A-Z, a-z, SP (32), DEL (127)
			if(Rx_Data[0] == 32 || Rx_Data[0] == 127 || (Rx_Data[0] > 47 && Rx_Data[0] < 58) || (Rx_Data[0] > 64 && Rx_Data[0] < 91) ||
					(Rx_Data[0] > 96 && Rx_Data[0] < 123)){
				if(Rx_Data[0] == 127){
					if(Rx_Index > 0){
						Rx_Index--;
					}
					// correct extended ascii chars which uses two bytes when press 'Delete'
					if(Rx_Buffer[Rx_Index] > 127 && Rx_Index > 0){
						Rx_Index--;
					}
					//clearBuffer(Rx_Index);
				}else{
					Rx_Buffer[Rx_Index++] = Rx_Data[0];
				}
				// print input char by char
				HAL_UART_Transmit_IT(huart, (uint8_t *)Rx_Data, 1);
			}
		}else{
			// if received data = 13
			Rx_Index = 0;
			Transfer_cplt = 1; // transfer complete, data is ready to read
		}
		HAL_UART_Receive_IT(huart, (uint8_t *)Rx_Data, 1);
	}
}

/*
 * @brief					clears the rx_buffer starting from address specified in param index
 * @param uint8_t index		index to start from clearing the buffer
 */
void clearBuffer(uint8_t index){
	for(uint8_t i = index; i < BUFFER_SIZE; i++){
		Rx_Buffer[i] = 0;
	}
}

/*
 * @brief					tokenizes the arguments from the commands and fills the array 'arguments'
 * @param					the string with the arguments
 */
void tokenize_arguments(char* args){
	char* token = strtok(args, " ");
	uint8_t i = 0;
	while(token){
		arguments[i] = (uint32_t)strtol(token, NULL, 16);
		token = strtok(NULL, " ");
		i++;
	}
}

/*
 * @brief								determines the number of flipped ones in a 16 bit string compared
 * 										to the expected value
 * @param uint16_t expected_value		the expected value
 * @param uint16_t real_value			the (possibly changed) value from the SRAM
 */
uint16_t flipped_one_16b(uint16_t expected_value, uint16_t real_value){
	uint16_t flipped = expected_value ^ real_value;
	flipped = expected_value & flipped;

	return hamming_weight_16b(flipped);
}

/*
 * @brief								determines the number of flipped zeros in a 16 bit string compared
 * 										to the expected value
 * @param uint16_t expected_value		the expected value
 * @param uint16_t real_value			the (possibly changed) value from the SRAM
 */
uint16_t flipped_zero_16b(uint16_t expected_value, uint16_t real_value){
	uint16_t flipped = expected_value ^ real_value;
	flipped = flipped & real_value;

	return hamming_weight_16b(flipped);
}

/*
 * @brief							determines the number of ones in a 16 bit string
 * @param uint16_t bitstring		the bitstring
 */
uint16_t get_num_one_16b(uint16_t bitstring){
	return hamming_weight_16b(bitstring);
}

/*
 * @brief							determines the number of zeros in a 16 bit string
 * @param uint16_t bitstring		the bitstring
 */
uint16_t get_num_zero_16b(uint16_t bitstring){
	return (16 - hamming_weight_16b(bitstring));
}

/*
 * @brief								determines the number of different bits in a 16 bit string compared
 * 										to the expected value
 * @param uint16_t expected_value		the expected value
 * @param uint16_t real_value			the (possibly changed) value from the SRAM
 */
uint16_t get_total_flip_probability_16b(uint16_t expected_value, uint16_t real_value){
	uint16_t bitstring = expected_value ^ real_value;

	return (hamming_weight_16b(bitstring));
}

/*
 * @brief							determines the number of ones in a 16 bit string
 * @param uint16_t bitstring		the bit string to be counted
 */
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

uint8_t get_space(char *rx_buffer){
	for(uint8_t i = 0; i < strlen(rx_buffer); i++){
		if(rx_buffer[i] == 32){
			return i;
		}
	}
	return (uint8_t)strlen(rx_buffer);
}
