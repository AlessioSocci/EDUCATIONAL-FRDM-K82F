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


#define SECTOR_SIZE					4096

#define W25Q64FV_WRITEENABLE		0x06        // write enable
#define W25Q64FV_WRITEDISABLE		0x04        // write disable

#define W25Q64FV_BLOCKERASE_4K		0x20        // Sector erase  (4 KB)
#define W25Q64FV_BLOCKERASE_32K		0x52        // Block erase (32 KB)
#define W25Q64FV_BLOCKERASE_64K		0xD8        // Two Block erase (64 KB)
#define W25Q64FV_CHIPERASE			0x60        // Chip erase (this operation take some seconds...)

#define W25Q64FV_STATUSREAD       	0x05        // read status register
#define W25Q64FV_STATUSWRITE		0x01        // write status register
#define W25Q64FV_ARRAYREAD			0x0B        // Fast read data (fast, need to add 1 dummy byte after 3 address bytes)
#define W25Q64FV_ARRAYREADLOWFREQ	0x03        // Slow read data

#define W25Q64FV_SLEEP				0xB9        // power down active
#define W25Q64FV_WAKE				0xAB        // power down not active
#define W25Q64FV_BYTEPAGEPROGRAM  	0x02        // write (1 to 256bytes)
#define W25Q64FV_JEDEC_READ			0x9F		// Device ID read (2 bytes)
#define W25Q64FV_MAN_READ			0x90    	// Manufacture ID read

#define W25Q64FV_MAC_READ			0x4B        // read unique ID number

#define W25Q64FV_DUMMY				0x00		// dummy byte

#define W25Q64FV_END_OF_TXFRAME		0xFF		// End Of Frame

void W25Q64FV_init(uint16_t baudRate);

uint64_t W25Q64FV_read_JEDECID(void);
uint64_t W25Q64FV_read_uniqueID(void);
uint64_t W25Q64FV_read_manufacture(void);

uint8_t W25Q64_readStatusRegister(uint8_t StatusRegisterNumber);

void W25Q64FV_eraseSector(uint32_t address);
void W25Q64FV_eraseBlock(uint32_t block_address);
void W25Q64FV_writeByte(uint8_t TXdata, uint32_t address);
uint8_t W25Q64FV_readByte(uint32_t address);
void W25Q64FV_eraseChip(void);


#endif /* W25Q64FV_H_ */
