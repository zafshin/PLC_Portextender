/*
 * uservice.h
 *
 *  Created on: Jun 15, 2023
 *      Author: Afshin
 */
#include "ethtypes.h"
#include "main.h"

#include "string"
#include <vector>
//#include "eflash.h"
#ifndef SRC_USERVICE_H_
#define SRC_USERVICE_H_
#define READ_INS 0x03U
#define READID_INS 0x90U
#define WRITEEN_INS 0x06U
#define WRITEDS_INS 0x04U
#define WRITEPAGE_INS 0x02U

#define UPDATE_FLAG_ADDR 0x0U
#define UPDATE_ADDR 0x100000U
#define FLASH_ADDR 0x8003B00U
class uservice {
public:
	uservice(SPI_HandleTypeDef *spic);
	virtual ~uservice();
	void ex(std::string &input, httpRes &res);

private:
	enum typec_t{
			crc=0,
			edata=1,
			size=2,
			command=3,
			none =4
		};
	struct decodePkg_t{
		typec_t t = none;
		std::vector<uint8_t> data;
		uint32_t num = 0;
		std::string cmd ="";
	};
	decodePkg_t decode(std::string &input);
	void flash(std::vector<unsigned char> &data, uint32_t address);
	bool checkcrc(uint32_t crce,uint32_t size);
	uint32_t fwSize = 0;
	uint32_t sAddr = 0x100000U;
	uint32_t eAddr = 0x100000U;
	uint32_t ecrc32 = 0;
	SPI_HandleTypeDef *eflash;
	void set_cs_flash(void);
	void deset_cs_flash(void);
	void spi_w(uint8_t *b, uint16_t size);
	void spi_r(uint8_t *data, uint16_t size);
	void read_eflash(uint8_t *data, uint32_t *address,uint8_t ins ,uint16_t size);
	void writeEnable_eflash();
	void Erase_eflash();
	void Erase_eflash_sectors(uint32_t *from_address,uint32_t *to_address);
	void writeDisable_eflash();
	void write_eflash(uint8_t *data, uint32_t *address);
	uint8_t check_eflash();
	uint8_t check_eflash_status();
	void waitforeflash();

};

#endif /* SRC_USERVICE_H_ */
