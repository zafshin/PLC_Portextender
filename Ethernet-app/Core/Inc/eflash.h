/*
 * eflash.h
 *
 *  Created on: Jun 15, 2023
 *      Author: Afshin
 */
#pragma once
#ifndef INC_EFLASH_H_
#define INC_EFLASH_H_
#define READ_INS 0x03U
#define READID_INS 0x90U
#define WRITEEN_INS 0x06U
#define WRITEDS_INS 0x04U
#define WRITEPAGE_INS 0x02U

#define UPDATE_FLAG_ADDR 0x0U
#define UPDATE_ADDR 0x100000U
#define FLASH_ADDR 0x8003B00U
#include "main.h"
void spi_w(uint8_t *b, uint16_t size);
void spi_r(uint8_t *data, uint16_t size);
void read_eflash(uint8_t *data, uint32_t *address,uint8_t ins ,uint16_t size);
void writeEnable_eflash();
void Erase_eflash();
void writeDisable_eflash();
void write_eflash(uint8_t *data, uint32_t *address);
uint8_t check_eflash();
uint8_t check_eflash_status();
void waitforeflash();
#endif /* INC_EFLASH_H_ */
