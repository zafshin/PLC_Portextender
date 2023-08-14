/*
 * eflash.c
 *
 *  Created on: Jun 15, 2023
 *      Author: Afshin
 */
#include "eflash.h"
#include "main.h"
void set_cs_flash(void) {
	HAL_GPIO_WritePin(GPIOA, CS_flash_Pin, GPIO_PIN_RESET); //CS LOW
}

void deset_cs_flash(void) {
	HAL_GPIO_WritePin(GPIOA, CS_flash_Pin, GPIO_PIN_SET); //CS HIGH
}
void spi_r(uint8_t *data, uint16_t size) {
	HAL_SPI_Receive(nullptr, data, size, HAL_MAX_DELAY);
}

void spi_w(uint8_t *b, uint16_t size) {
	HAL_SPI_Transmit(nullptr, b, size, HAL_MAX_DELAY);
}
uint8_t check_eflash_status(){
	set_cs_flash();
	spi_w((uint8_t*)0x05,1);
	uint8_t status = 0;
	spi_r(&status,1);
	deset_cs_flash();
	return (status ^ 0x1);
}
void waitforeflash(){
	while(check_eflash_status() != 0)
		HAL_Delay(1);
}
void read_eflash(uint8_t *data, uint32_t *address,uint8_t ins ,uint16_t size){
	uint8_t *addr16 = ((uint8_t*)address) + 2;
	uint8_t *addr8 = ((uint8_t*)address) + 1;
	uint8_t *addr0 = ((uint8_t*)address);
	uint8_t frm[4] ={ins,*addr16,*addr8,*addr0};
	//uint8_t frm[4] ={*addr0,*addr8,*addr16,ins};
	waitforeflash();
	set_cs_flash();
	spi_w((uint8_t*)&frm,4);
	spi_r(data,size);
	deset_cs_flash();
}
void writeEnable_eflash(){
	uint8_t frm =WRITEEN_INS;
	set_cs_flash();
	spi_w(&frm,1);
	deset_cs_flash();
}
void Erase_eflash(){
	waitforeflash();
	writeEnable_eflash();
	uint8_t frm =0xc7;
	set_cs_flash();
	spi_w(&frm,1);
	deset_cs_flash();
}
void writeDisable_eflash(){
	uint8_t frm[1] ={WRITEDS_INS};
	waitforeflash();
	set_cs_flash();
	spi_w((uint8_t*)&frm,1);
	deset_cs_flash();
}

void write_eflash(uint8_t *data, uint32_t *address){
		uint8_t *addr16 = ((uint8_t*)address) + 2;
		uint8_t *addr8 = ((uint8_t*)address) + 1;
		uint8_t *addr0 = ((uint8_t*)address);
		uint8_t buff[260];
		buff[0]= WRITEPAGE_INS;
		buff[1]= *addr16;
		buff[2]= *addr8;
		buff[3]= *addr0;
		for(int i =0; i< 256;i++)
			buff[i+4] = *(data+i);
		waitforeflash();
		writeEnable_eflash();
		set_cs_flash();
		spi_w((uint8_t*)&buff,260);
		deset_cs_flash();
}
uint8_t check_eflash(){
	uint8_t data[2] = {0,0};
	uint32_t addr= 0x0;
	read_eflash((uint8_t*)&data,&addr,(uint8_t)READID_INS,2);
	if(data[0] == 0xef)
		return 1;
	else
		return 0;
}