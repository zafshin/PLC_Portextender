/*
 * eflash.c
 *
 *  Created on: Jun 15, 2023
 *      Author: Afshin
 */
#include "eflash.h"
void set_cs_flash(struct eflash_t *flash) {
	HAL_GPIO_WritePin(flash->CS_Port, flash->CS_Pin, GPIO_PIN_RESET); //CS LOW
}

void deset_cs_flash(struct eflash_t *flash) {
	HAL_GPIO_WritePin(flash->CS_Port, flash->CS_Pin, GPIO_PIN_SET); //CS HIGH
}
void spi_r(struct eflash_t *flash,uint8_t *data, uint16_t size) {
	HAL_SPI_Receive(flash->spi, data, size, HAL_MAX_DELAY);
}

void spi_w(struct eflash_t *flash,uint8_t *b, uint16_t size) {
	HAL_SPI_Transmit(flash->spi, b, size, HAL_MAX_DELAY);
}
uint8_t check_eflash_status(struct eflash_t *flash) {
	set_cs_flash(flash);
	uint8_t frm = 0x05;
	spi_w(flash, &frm, 1);
	uint8_t status = 0;
	spi_r(flash, &status, 1);
	deset_cs_flash(flash);
	return (status & 0x01);
}
void Erase_eflash_sectors(struct eflash_t *flash, uint32_t *from_address, uint32_t *to_address) {
	uint32_t address = *from_address;
	while (1) {
		uint8_t *addr16 = ((uint8_t*) &address) + 2;
		uint8_t *addr8 = ((uint8_t*) &address) + 1;
		uint8_t *addr0 = ((uint8_t*) &address);
		uint8_t frm[4] = { 0x20, *addr16, *addr8, *addr0 };
		writeEnable_eflash(flash);
		set_cs_flash(flash);
		spi_w(flash, (uint8_t*) &frm, 4);
		deset_cs_flash(flash);
		waitforeflash(flash);
		if ((*to_address <= (address + 4096)))
			break;
		address += 4096;
	}
}
void waitforeflash(struct eflash_t *flash) {
	uint8_t stats = check_eflash_status(flash);
	uint16_t timeout = 0;
	while (stats != 0) {
		stats = check_eflash_status(flash);
		if (timeout > 1000000) {
			timeout = 0;
			writeEnable_eflash(flash);
		}
		timeout++;
	}
}
void read_eflash(struct eflash_t *flash, uint8_t *data, uint32_t *address, uint8_t ins, uint16_t size) {
	uint8_t *addr16 = ((uint8_t*) address) + 2;
	uint8_t *addr8 = ((uint8_t*) address) + 1;
	uint8_t *addr0 = ((uint8_t*) address);
	uint8_t frm[4] = { ins, *addr16, *addr8, *addr0 };
	set_cs_flash(flash);
	spi_w(flash, (uint8_t*) &frm, 4);
	spi_r(flash, data, size);
	deset_cs_flash(flash);
}
void writeEnable_eflash(struct eflash_t *flash) {
	uint8_t frm = WRITEEN_INS;
	set_cs_flash(flash);
	spi_w(flash, &frm, 1);
	deset_cs_flash(flash);
}
void Erase_eflash(struct eflash_t *flash) {
	waitforeflash(flash);
	writeEnable_eflash(flash);
	uint8_t frm = 0xc7;
	set_cs_flash(flash);
	spi_w(flash, &frm, 1);
	deset_cs_flash(flash);
	waitforeflash(flash);
}
void writeDisable_eflash(struct eflash_t *flash) {
	uint8_t frm[1] = { WRITEDS_INS };
	waitforeflash(flash);
	set_cs_flash(flash);
	spi_w(flash, (uint8_t*) &frm, 1);
	deset_cs_flash(flash);
}

void write_eflash(struct eflash_t *flash, uint8_t *data, uint32_t *address) {
	uint8_t *addr16 = ((uint8_t*) address) + 2;
	uint8_t *addr8 = ((uint8_t*) address) + 1;
	uint8_t *addr0 = ((uint8_t*) address);
	uint8_t buff[260];
	buff[0] = WRITEPAGE_INS;
	buff[1] = *addr16;
	buff[2] = *addr8;
	buff[3] = *addr0;
	for (int i = 0; i < 256; i++)
		buff[i + 4] = *(data + i);
	waitforeflash(flash);
	writeEnable_eflash(flash);
	set_cs_flash(flash);
	spi_w(flash, (uint8_t*) &buff, 260);
	deset_cs_flash(flash);
}
uint8_t check_eflash(struct eflash_t *flash) {
	uint8_t data[2] = { 0, 0 };
	uint32_t addr = 0x0;
	read_eflash(flash, (uint8_t*) &data, &addr, (uint8_t) READID_INS, 2);
	if (data[0] == 0xef)
		return 1;
	else
		return 0;
}
