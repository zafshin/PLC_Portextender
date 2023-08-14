/*
 * uservice.cpp
 *
 *  Created on: Jun 15, 2023
 *      Author: Afshin
 */

#include "uservice.h"
#include <sstream>
#include <iomanip>

#define CRC32_POLYNOMIAL 0xEDB88320
uservice::uservice(SPI_HandleTypeDef *spic) :
		eflash(spic) {
	check_eflash();
}

uservice::~uservice() {
}
std::vector<uint8_t> hexStringToByteArray(const std::string &hexString) {
	std::vector<uint8_t> byteArray;

	// Iterate over the hex string, converting two characters (1 byte) at a time
	for (size_t i = 0; i < hexString.length(); i += 2) {
		// Extract two characters from the hex string
		std::string byteString = hexString.substr(i, 2);
		// Convert the byte string to an actual byte
		uint8_t byte = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
		// Add the byte to the byte array
		byteArray.push_back(byte);
	}

	return byteArray;
}

uservice::decodePkg_t uservice::decode(std::string &input) {
	decodePkg_t result;
	if (input.find(std::string("POST")) != std::string::npos) {
		std::string path;
		std::string c;
		std::string to;
		std::size_t found;
		if (input != "") {
			{
				std::stringstream ss(input);
				while (std::getline(ss, to, '\n')) {
					found = to.find("POST ");
					if (found != std::string::npos)
						break;
				}
			}
			{
				std::stringstream ss(to);
				while (std::getline(ss, path, ' ')) {
					found = path.find("POST");
					if (found == std::string::npos)
						break;
				}
			}
			{
				unsigned int pos = input.find("\r\n\r\n");
				if ((pos + 4) < (input.length() - 1))
					c = input.substr(input.find("\r\n\r\n") + 4,
							input.length() - 1);
			}

		}

		if (path.find("/update/add/") != std::string::npos) {
			c = c.substr(1, c.length() - 2);
			result.data = hexStringToByteArray(c);
			result.t = edata;
			result.num = std::stoul(path.substr(12, path.length() - 1));
		} else if (path.find("/update/crc/") != std::string::npos) {
			c = c.substr(1, c.length() - 2);
			std::vector<uint8_t> tmp = hexStringToByteArray(c);
			result.num = (tmp[0] << 24) | (tmp[1] << 16) | (tmp[2] << 8)
					| tmp[3];
			result.t = crc;
		} else if (path.find("/update/size/") != std::string::npos) {
			result.num = std::stoul(c);
			result.t = size;
		} else if (path.find("/update/cmd/") != std::string::npos) {
			c = c.substr(1, c.length() - 2);
			result.cmd = c;
			result.t = command;
		}
	}
	return result;
}
void uservice::ex(std::string &input, httpRes &res) {
	decodePkg_t pkg = decode(input);
	uint32_t addr = sAddr + pkg.num;
	switch (pkg.t) {
	case typec_t::edata:
		writeEnable_eflash();
		write_eflash(pkg.data.data(), &addr);
		res.mode = false;
		res.code = 200;
		res.res = "OK";
		break;
	case typec_t::command:
		if (pkg.cmd == "erase") {
			uint32_t from = sAddr;
			uint32_t to = sAddr + 256000;
			Erase_eflash_sectors(&from, &to);
		} else if (pkg.cmd == "reboot") {
			NVIC_SystemReset();
		} else if (pkg.cmd == "uflag") {
			writeEnable_eflash();
			uint8_t uflag[256];
			uflag[0] = 0;
			for (int i = 1; i < 256; i++)
				uflag[i] = 0xFF;
			uint32_t addrf = 0xFF000;
			write_eflash((uint8_t*) &uflag, &addrf);
		}
		res.mode = false;
		res.code = 200;
		res.res = "OK";
		break;
	case crc:
		if (checkcrc(pkg.num, fwSize)) {
			res.mode = false;
			res.code = 200;
			res.res = "OK";
		} else {
			res.mode = false;
			res.code = 405;
			res.res = "Not match";
		}
		break;
	case size:
		fwSize = pkg.num;
		res.mode = false;
		res.code = 200;
		res.res = "OK";
		break;
	case none:
		res.mode = false;
		res.code = 404;
		res.res = "Error";
		break;
	}
}

void uservice::flash(std::vector<unsigned char> &data, uint32_t address) {
	writeEnable_eflash();
	write_eflash(data.data(), &address);
}
// Table for CRC32 lookup
static uint32_t crc32_table[256];

// Function to initialize the CRC32 lookup table
void crc32_init_table() {
	for (uint32_t i = 0; i < 256; i++) {
		uint32_t crc = i;
		for (uint32_t j = 0; j < 8; j++) {
			if (crc & 1) {
				crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
			} else {
				crc >>= 1;
			}
		}
		crc32_table[i] = crc;
	}
}
bool uservice::checkcrc(uint32_t crce, uint32_t size) {
	crc32_init_table();
	uint32_t crc = 0xFFFFFFFF;
	uint8_t data = 0;
	uint32_t addrt = sAddr;
	for (uint32_t i = 0; i < size; i++) {
		read_eflash(&data, &addrt, (uint8_t) READ_INS, 1);
		uint8_t index = (crc ^ data) & 0xFF;
		crc = (crc >> 8) ^ crc32_table[index];
		addrt++;
	}
	crc = ~crc;
	return (crc == crce);
}

void uservice::set_cs_flash(void) {
	HAL_GPIO_WritePin(CS_flash_GPIO_Port, CS_flash_Pin, GPIO_PIN_RESET); //CS LOW
}

void uservice::deset_cs_flash(void) {
	HAL_GPIO_WritePin(CS_flash_GPIO_Port, CS_flash_Pin, GPIO_PIN_SET); //CS HIGH
}
void uservice::spi_r(uint8_t *data, uint16_t size) {
	HAL_SPI_Receive(eflash, data, size, HAL_MAX_DELAY);
}

void uservice::spi_w(uint8_t *b, uint16_t size) {
	HAL_SPI_Transmit(eflash, b, size, HAL_MAX_DELAY);
}
uint8_t uservice::check_eflash_status() {
	set_cs_flash();
	uint8_t frm = 0x05;
	spi_w(&frm, 1);
	uint8_t status = 0;
	spi_r(&status, 1);
	deset_cs_flash();
	return (status & 0x01);
}
void uservice::Erase_eflash_sectors(uint32_t *from_address,
		uint32_t *to_address) {
	uint32_t address = *from_address;
	while (true) {
		uint8_t *addr16 = ((uint8_t*) &address) + 2;
		uint8_t *addr8 = ((uint8_t*) &address) + 1;
		uint8_t *addr0 = ((uint8_t*) &address);
		uint8_t frm[4] = { 0x20, *addr16, *addr8, *addr0 };
		writeEnable_eflash();
		set_cs_flash();
		spi_w((uint8_t*) &frm, 4);
		deset_cs_flash();
		waitforeflash();
		if ((*to_address <= (address + 4096)))
			break;
		address += 4096;
	}
}
void uservice::waitforeflash() {
	uint8_t stats = check_eflash_status();
	uint16_t timeout = 0;
	while (stats != 0) {
		stats = check_eflash_status();
		if (timeout > 1000000) {
			timeout = 0;
			writeEnable_eflash();
		}
		timeout++;
	}
}
void uservice::read_eflash(uint8_t *data, uint32_t *address, uint8_t ins,
		uint16_t size) {
	uint8_t *addr16 = ((uint8_t*) address) + 2;
	uint8_t *addr8 = ((uint8_t*) address) + 1;
	uint8_t *addr0 = ((uint8_t*) address);
	uint8_t frm[4] = { ins, *addr16, *addr8, *addr0 };
	set_cs_flash();
	spi_w((uint8_t*) &frm, 4);
	spi_r(data, size);
	deset_cs_flash();
}
void uservice::writeEnable_eflash() {
	uint8_t frm = WRITEEN_INS;
	set_cs_flash();
	spi_w(&frm, 1);
	deset_cs_flash();
}
void uservice::Erase_eflash() {
	waitforeflash();
	writeEnable_eflash();
	uint8_t frm = 0xc7;
	set_cs_flash();
	spi_w(&frm, 1);
	deset_cs_flash();
	waitforeflash();
}
void uservice::writeDisable_eflash() {
	uint8_t frm[1] = { WRITEDS_INS };
	waitforeflash();
	set_cs_flash();
	spi_w((uint8_t*) &frm, 1);
	deset_cs_flash();
}

void uservice::write_eflash(uint8_t *data, uint32_t *address) {
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
	waitforeflash();
	writeEnable_eflash();
	set_cs_flash();
	spi_w((uint8_t*) &buff, 260);
	deset_cs_flash();
}
uint8_t uservice::check_eflash() {
	uint8_t data[2] = { 0, 0 };
	uint32_t addr = 0x0;
	read_eflash((uint8_t*) &data, &addr, (uint8_t) READID_INS, 2);
	if (data[0] == 0xef)
		return 1;
	else
		return 0;
}
