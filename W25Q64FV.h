/*
 * W25Q64FV.h
 *
 *  Created on: 16 giu 2020
 *      Author: Alessio
 */

#ifndef W25Q64FV_H_
#define W25Q64FV_H_


#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK82F25615.h"
#include "core_cm4.h"

#define SECTOR_SIZE						4096

#define TOTAL_BYTE_IN_MEMORY 			8388608
#define TOTAL_SECTOR_IN_MEMORY 			2048
#define TOTAL_32K_BLOCK_IN_MEMORY		256
#define TOTAL_64K_BLOCK_IN_MEMORY		128


#define W25Q64FV_SLEEP					0xB9        // Power down active
#define W25Q64FV_WAKE					0xAB        // Power down not active

#define W25Q64FV_WRITEENABLE			0x06        // Write enable
#define W25Q64FV_WRITEDISABLE			0x04        // Write disable

#define W25Q64FV_SECTORERASE_4K			0x20        // Sector erase  (4 KB)
#define W25Q64FV_BLOCKERASE_32K			0x52        // Block erase (32 KB)
#define W25Q64FV_BLOCKERASE_64K			0xD8        // Two Block erase (64 KB)
#define W25Q64FV_CHIPERASE				0xC7        // Chip erase (this operation take some seconds...)

#define W25Q64FV_READ_STAT_1       		0x05        // Read status register 1
#define W25Q64FV_READ_STAT_2      		0x35        // Read status register 2
#define W25Q64FV_WRITE_STAT_1			0x01        // Write status register
#define W25Q64FV_WRITE_STAT_2			0x31        // Write status register

#define W25Q64FV_WRITEBYTE  			0x02        // Write (1 to 256 bytes)
#define W25Q64FV_READBYTE				0x03        // Slow read data
#define W25Q64FV_ARRAYREAD				0x0B        // Fast read data


#define W25Q64FV_JEDEC_READ				0x9F		// Device ID read (2 bytes)
#define W25Q64FV_MAN_READ				0x90    	// Manufacture ID read
#define W25Q64FV_MAC_READ				0x4B        // read unique ID number

#define W25Q64FV_DUMMY					0x00		// Dummy byte

#define W25Q64FV_END_OF_TXFRAME			256			// End Of Frame


void W25Q64FV_init(uint16_t baudRate);

void W25Q64FV_wakeUp(void);
void W25Q64FV_sleep(void);
void W25Q64FV_write_enable(void);
void W25Q64FV_write_disable(void);

uint32_t W25Q64FV_read_JEDECID(void);
uint32_t W25Q64FV_read_uniqueID(void);
uint32_t W25Q64FV_read_manufacture(void);

uint8_t W25Q64_readStatusRegister(uint16_t StatusRegisterNumber);
void W25Q64_writeStatusRegister(uint16_t StatusRegisterNumber);

void W25Q64FV_eraseSector(uint32_t address);
void W25Q64FV_eraseBlock32(uint32_t block_address);
void W25Q64FV_eraseBlock64(uint32_t block_address);
void W25Q64FV_writeByte(uint16_t TXdata, uint32_t address);
void W25Q64FV_writeMoreThanOneByte(uint16_t* TXdata, uint32_t address);
uint8_t W25Q64FV_readByte(uint32_t address);
void W25Q64FV_eraseChip(void);


#endif /* W25Q64FV_H_ */
