/*
 * W25Q64FV.c
 *
 *  Created on: 16 giu 2020
 *      Author: Alessio
 */

#include "W25Q64FV.h"
#include "time.h"

uint8_t RX_frame_number_for_ISR = 1;

uint32_t RXdata_from_ISR = 0;


void SPI0_IRQHandler(void)
{
	RXdata_from_ISR = 0;

	delay(1); // delay work with PIT interrupt routine, this is an example in which SPI interrupt routine is preempted by PIT routine (SPI priority is less than PIT priority)

	for(int j = 0; (j < RX_frame_number_for_ISR ) && (j <= 8); j++)
	{
		for(int i = 0; i < 300; i++); // Wait

		RXdata_from_ISR |= (SPI0->POPR) << (((RX_frame_number_for_ISR - j - 1) * 8)); // Read data frame and fit into 64 bit variable
	}

	SPI0->SR |= (1 << 17); // clear FIFO drain flag
}

void SPI_write(uint8_t* TXdata)
{
	SPI0->SR |= (1 << 31); // Clear transmit complete flag
	SPI0->SR |= (1 << 25); // Clear TX FIFO fill flag
	SPI0->SR |= (1 << 17); // Clear RX FIFO drain flag!

	SPI0->PUSHR = (*TXdata) | (1 << 31) | (1 << 17); // PCS1 remain active between transfers, load data in TX FIFO, select the PCS1 assert signal.

	SPI0->MCR &= ~(1 << 0); // Start transfer

	for(int i = 0; i < 200; i++); // Wait

	TXdata++;

	while((*TXdata) != W25Q64FV_END_OF_TXFRAME)
	{
		for(int i = 0; i < 200; i++); // Wait

		SPI0->PUSHR = (*TXdata) | (1 << 31) | (1 << 17); // PCS1 remain active between transfers, load data in TX FIFO, select the PCS1 assert signal.

		TXdata++;
	}

	for(int i = 0; i < 1000; i++); // Wait

	SPI0->PUSHR &= ~(1 << 17); // Set PCS1 inactive

	SPI0->MCR |= (1 << 0); // Stop transfer

	SPI0->SR |= (1 << 31); // Clear transmit complete flag

	SPI0->SR |= (1 << 25); // Clear TX FIFO fill flag

	SPI0->SR |= (1 << 17); // Clear RX FIFO drain flag!
}


uint64_t SPI_write_and_read(uint8_t* TXdata, uint8_t RX_frame_number)
{
	RX_frame_number_for_ISR = RX_frame_number;

	SPI0->SR |= (1 << 31); // Clear transmit complete flag
	SPI0->SR |= (1 << 25); // Clear TX FIFO fill flag
	SPI0->SR |= (1 << 17); // Clear RX FIFO drain flag!

	SPI0->MCR &= ~(1 << 0); // Start transfer

	for(int i = 0; i < 200; i++); // Wait

	SPI0->PUSHR = (*TXdata) | (1 << 31) | (1 << 17); // PCS1 remain active between transfers, load data in TX FIFO, select the PCS1 assert signal.

	TXdata++;

	while((*TXdata) != W25Q64FV_END_OF_TXFRAME)
	{
		for(int i = 0; i < 200; i++); // Wait

		SPI0->PUSHR = (*TXdata) | (1 << 31) | (1 << 17); // PCS1 remain active between transfers, load data in TX FIFO, select the PCS1 assert signal.

		TXdata++;
	}

	/* Here program enter in SPI0 interrupt routine to read device response */

	for(int i = 0; i < 1000; i++); // Wait

	SPI0->PUSHR &= ~(1 << 17); // Set PCS1 inactive

	SPI0->MCR |= (1 << 0); // Stop transfer

	SPI0->SR |= (1 << 31); // Clear transmit complete flag

	SPI0->SR |= (1 << 25); // Clear TX FIFO fill flag

	SPI0->SR |= (1 << 17); // Clear RX FIFO drain flag!

	RX_frame_number_for_ISR = 1;

	return RXdata_from_ISR;
}

void W25Q64FV_write_enable(void)
{
	uint8_t messageToSend[] = {W25Q64FV_WRITEENABLE, W25Q64FV_END_OF_TXFRAME};

	SPI_write(messageToSend);
}

void W25Q64FV_wakeUp(void)
{
	uint8_t messageToSend[] = {W25Q64FV_WAKE, W25Q64FV_END_OF_TXFRAME};

	SPI_write(messageToSend);
}

void W25Q64FV_sleep(void)
{
	uint8_t messageToSend[] = {W25Q64FV_SLEEP, W25Q64FV_END_OF_TXFRAME};

	SPI_write(messageToSend);
}

void W25Q64FV_init(uint16_t baudRate)
{
	SIM->SCGC6 |= (1 << 12); // Clock on SPI0

	PORTD->PCR[4] |= (1 << 9); // PTD4 in Alt2 Mode SPI0_PCS1 (pinout of FRDM K82F board is PCS1, not 0...)
	PORTD->PCR[1] |= (1 << 9); // PTD1 in Alt2 Mode SPI0_SCK
	PORTD->PCR[2] |= (1 << 9); // PTD2 in Alt2 Mode SPI0_SOUT
	PORTD->PCR[3] |= (1 << 9); // PTD3 in Alt2 Mode SPI0_SIN

	uint16_t baudRatePossibleValue[] = {2, 4, 6, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
	uint8_t baudRatePrescalerPossibleValue[] = {2, 3, 5, 7};

	uint16_t resultClock = 0;
	uint8_t scalerValue = 0;
	uint8_t prescalerValue = 0;
	uint16_t error = 0xFFFF;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			resultClock = (75000000 * baudRatePrescalerPossibleValue[i] * baudRatePossibleValue[j]);

			if(baudRate < resultClock)
			{
				if((resultClock - baudRate) < error)
				{
					error = resultClock - baudRate;
					prescalerValue = i;
					scalerValue = j;
				}
			}
			else
			{
				if((baudRate - resultClock) < error)
				{
					error = baudRate - resultClock;
					prescalerValue = i;
					scalerValue = j;
				}
			}
		}
	}

	SPI0->RSER |= (1 << 17); // Enable RX FIFO drain request interrupt

	NVIC->IP[26] |= (1 << 5); // Preemptive Priority (2)! (max priority is 0)

	NVIC->ISER[0] |= (1 << 26); // Enable Interrupt for SPI0

	SPI0->MCR |= (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 0); // Stop Hardware, Disable TX FIFO, disable RX FIFO, clear TX FIFO, clear RX FIFO, Stop transfers

	SPI0->MCR |= (1 << 31) | (1 << 17); // SPI0 in Master Mode, PCS inactive state when High

	SPI0->CTAR[0] = (1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25); // Frame Size of: 7 + 1 = 8 bit

	SPI0->CTAR[0] |= (1 << 23) | (1 << 22); // Set delay between CS assertion and first CLK edge as 7 (prescaler value)

	SPI0->CTAR[0] |= (1 << 21) | (1 << 20); // Set delay between last CLK edge and CS negation as 7 (prescaler value)

//	SPI0->CTAR[0] |= (1 << 31) | (1 << 3); // BR = 6, DBR set as 1, Baud Rate = (fp / prescaler) * [(1 + DBR) / BR scaler], so (75000000 / 2) * [(1+1)/2] = 75000000 / 6 = 12500000 //37500000 Hz (time period ~= 25 ns)

	SPI0->CTAR[0] &= ~((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)) ; // set BR scaler; Baud Rate = (fp / prescaler) * [(1 + double BR) / BR scaler]

	SPI0->CTAR[0] |= (uint32_t)scalerValue; // Baud Rate scaler

	SPI0->CTAR[0] |= (((uint32_t)prescalerValue) << 16); // Baud Rate prescaler

	SPI0->MCR &= ~((1 << 14)); // Start hardware

	W25Q64FV_wakeUp();

	W25Q64FV_write_enable();
}

// work in progress..
uint64_t W25Q64FV_read_JEDECID(void)
{
	uint64_t deviceID = 0;

	W25Q64FV_write_enable();

	uint8_t messageToSend[2] = {W25Q64FV_JEDEC_READ, W25Q64FV_END_OF_TXFRAME};

	deviceID = SPI_write_and_read(messageToSend, 3);

//	uint8_t messageToSend[] = {W25Q64FV_JEDEC_READ, W25Q64FV_END_OF_TXFRAME};
//
//	deviceID = SPI_write_and_read(messageToSend, 1);

	return deviceID;
}

// work in progress..
uint64_t W25Q64FV_read_uniqueID(void)
{
	uint64_t uniqueID = 0;

	W25Q64FV_write_enable();

	uint8_t messageToSend[6] = {W25Q64FV_MAC_READ, W25Q64FV_DUMMY, W25Q64FV_DUMMY, W25Q64FV_DUMMY, W25Q64FV_DUMMY, W25Q64FV_END_OF_TXFRAME};

	uniqueID = SPI_write_and_read(messageToSend, 1);

//	uint8_t messageToSend[] = {W25Q64FV_MAC_READ, W25Q64FV_END_OF_TXFRAME};
//
//	uniqueID = SPI_write_and_read(messageToSend, 1);

	return uniqueID;
}

// work in progress..
uint64_t W25Q64FV_read_manufacture(void)
{
	uint64_t manufacture = 0;

	W25Q64FV_write_enable();

	uint8_t messageToSend[5] = {W25Q64FV_MAN_READ, W25Q64FV_DUMMY, W25Q64FV_DUMMY, 0, W25Q64FV_END_OF_TXFRAME};

	manufacture = SPI_write_and_read(messageToSend, 2);

//	uint8_t messageToSend[] = {W25Q64FV_MAN_READ, W25Q64FV_END_OF_TXFRAME};
//
//	manufacture = SPI_write_and_read(messageToSend, 1);

	return manufacture;
}

// work in progress..
uint8_t W25Q64_readStatusRegister(uint8_t StatusRegisterNumber) // parameter for select status register 1 or 2
{
	uint64_t _status = 0;

	if(StatusRegisterNumber == 1)
	{
		uint8_t messageToSend[] = {0x05, W25Q64FV_END_OF_TXFRAME};

		_status = SPI_write_and_read(messageToSend, 1);
	}
	else if(StatusRegisterNumber == 2)
	{
		uint8_t messageToSend[] = {0x35, W25Q64FV_END_OF_TXFRAME};

		_status = SPI_write_and_read(messageToSend, 1);
	}
	else
	{
		uint8_t messageToSend[] = {0x15, W25Q64FV_END_OF_TXFRAME};

		_status = SPI_write_and_read(messageToSend, 1);
	}

	uint8_t status = (uint8_t)_status;

	return status;
}

 // work in progress..
void W25Q64_writeStatusRegister(uint8_t StatusRegisterNumber) // parameter for select status register 1 or 2
{
	if(StatusRegisterNumber == 1)
	{
		uint8_t messageToSend[] = {0x01, W25Q64FV_END_OF_TXFRAME};

		SPI_write(messageToSend);
	}
	else if(StatusRegisterNumber == 2)
	{
		uint8_t messageToSend[] = {0x31, W25Q64FV_END_OF_TXFRAME};

		SPI_write(messageToSend);
	}
	else
	{
		uint8_t messageToSend[] = {0x11, W25Q64FV_END_OF_TXFRAME};

		SPI_write(messageToSend);
	}
}

void W25Q64FV_eraseSector(uint32_t sector_address) // Erase 4 kB
{
	uint32_t address = sector_address * SECTOR_SIZE;

	W25Q64FV_write_enable();

	uint8_t address_secondByte = (uint8_t)((address & 0x00FF0000) >> 16); // MSB, start of 24 bit addres

	uint8_t address_thirdByte = (uint8_t)((address & 0x0000FF00) >> 8);

	uint8_t address_fourthByte = (uint8_t)(address & 0x000000FF); // the LSB (less significative)

	uint8_t message[5] = {0x20, address_secondByte, address_thirdByte, address_fourthByte, W25Q64FV_END_OF_TXFRAME}; // message to the external device

	SPI_write(message); // function parameter is a pointer, vector passed as a parameter is take by default as a pointer to it's first element
}

void W25Q64FV_eraseBlock(uint32_t block_address) // Erase 64 kB
{
	uint32_t address = block_address * SECTOR_SIZE * 16;

	W25Q64FV_write_enable();

	uint8_t address_secondByte = (uint8_t)((address & 0x00FF0000) >> 16); // MSB, start of 24 bit addres

	uint8_t address_thirdByte = (uint8_t)((address & 0x0000FF00) >> 8);

	uint8_t address_fourthByte = (uint8_t)(address & 0x000000FF); // the LSB (less significative)

	uint8_t message[5] = {0xD8, address_secondByte, address_thirdByte, address_fourthByte, W25Q64FV_END_OF_TXFRAME}; // message to the external device

	SPI_write(message); // function parameter is a pointer, vector passed as a parameter is take by default as a pointer to it's first element
}

void W25Q64FV_writeByte(uint8_t TXdata, uint32_t address)
{
	W25Q64FV_write_enable();


	uint8_t address_secondByte = (uint8_t)((address & 0x00FF0000) >> 16); // MSB, start of 24 bit addres

	uint8_t address_thirdByte = (uint8_t)((address & 0x0000FF00) >> 8);

	uint8_t address_fourthByte = (uint8_t)(address & 0x000000FF); // the LSB (less significative)

	uint8_t message[7] = {0x02, address_secondByte, address_thirdByte, address_fourthByte, TXdata, W25Q64FV_DUMMY, W25Q64FV_END_OF_TXFRAME}; // message to the external device

	SPI_write(message); // function parameter is a pointer, vector passed as a parameter is take by default as a pointer to it's first element
}

uint8_t W25Q64FV_readByte(uint32_t address)
{
	uint64_t _RXdata = 0;

	W25Q64FV_write_enable();

	uint8_t address_secondByte = (uint8_t)((address & 0x00FF0000) >> 16); // MSB, start of 24 bit addres

	uint8_t address_thirdByte = (uint8_t)((address & 0x0000FF00) >> 8);

	uint8_t address_fourthByte = (uint8_t)(address & 0x000000FF); // the LSB (less significative);

	uint8_t message[6] = {0x03, address_secondByte, address_thirdByte, address_fourthByte, W25Q64FV_DUMMY, W25Q64FV_END_OF_TXFRAME};

	_RXdata = SPI_write_and_read(message, 1); // function parameter is a pointer, vector passed as a parameter is take by default as a pointer to it's first element

	uint8_t RXdata = (uint8_t)_RXdata;

	return RXdata;
}

void W25Q64FV_eraseChip(void)
{
	W25Q64FV_write_enable();

	uint8_t message[7] = {0xC7, W25Q64FV_END_OF_TXFRAME}; // message to the external device

	SPI_write(message); // function parameter is a pointer, vector passed as a parameter is take by default as a pointer to it's first element
}


