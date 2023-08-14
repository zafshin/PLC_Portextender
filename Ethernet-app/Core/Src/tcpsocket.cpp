/*
 * tcpsocket.cpp
 *
 *  Created on: May 21, 2023
 *      Author: Saeed
 */

#include "tcpsocket.h"
#include "main.h"
#include "w5500.h"
#include "socket.h"
#include "wizchip_conf.h"
#include <string>

tcpsocket::tcpsocket() {
	// TODO Auto-generated constructor stub
}

tcpsocket::~tcpsocket() {
	// TODO Auto-generated destructor stub
}

void tcpsocket::config(wiz_NetInfo &config) {
	wizchip_setnetinfo(&config);
}
void tcpsocket::printInformation() {

}
bool tcpsocket::Ready(const unsigned char sockets) {
	uint16_t ss;
	getsockopt(sockets, SO_REMAINSIZE, &ss);
	if (ss > 0)
		return true;
	else
		return false;
}
bool tcpsocket::setKeepAlive(const unsigned char sockets, uint8_t time) {
	if (setsockopt(sockets, SO_KEEPALIVEAUTO, &time) == SOCK_OK) {
		setsockopt(sockets, SO_KEEPALIVESEND, nullptr);
		return true;
	} else
		return false;
}
bool tcpsocket::connected(const unsigned char sockets) {
	if (getSn_IR(sockets) & Sn_IR_TIMEOUT)
		return false;
	else
		return true;
}
bool tcpsocket::connectTo(const unsigned char sockets,
		const unsigned char ip[4], int port) {
	switch (connect(sockets, (uint8_t*) ip, port)) {
	case SOCK_OK:
		return true;
		break;
	case SOCKERR_PORTZERO:
		return false;
		break;
	case SOCKERR_IPINVALID:
		return false;
		break;
	case SOCKERR_TIMEOUT:
		return false;
		break;
	case SOCKERR_SOCKCLOSED:
		return false;
		break;
	case SOCK_BUSY:
		return false;
		break;
	case SOCK_ESTABLISHED:
		return true;
		break;
	}
	return false;
}

bool tcpsocket::connectS(const unsigned char socket, uint8_t flag, uint8_t addr,
		uint16_t port) {
	switch (connect(socket, &addr, port)) {
	case SOCK_OK:
		return true;
		break;
	case SOCKERR_SOCKNUM:
		return false;
		break;
	case SOCKERR_SOCKMODE:
		return false;
		break;
	case SOCKERR_TIMEOUT:
		return false;
		break;
	case SOCK_BUSY:
		return false;
		break;
	}
	return false;
}
bool tcpsocket::configSocket(const unsigned char sockets, uint8_t flag,
		uint16_t port) {

	if (socket(sockets, flag, port, SF_TCP_NODELAY) == sockets)
		return true;
	else
		return false;

}
uint8_t tcpsocket::listenWait(const unsigned char sockets) {
	uint8_t status = getSn_SR(sockets);
	//log(std::to_string(status).c_str());
	if (status == SOCK_LISTEN)
		return 0;
	else if (status == SOCK_INIT)
		return 1;
	else if (status == SOCK_ESTABLISHED)
		return 2;
	else
		return 3;
}
bool tcpsocket::closeS(const unsigned char sockets) {
	close(sockets);
	return true;
}
bool tcpsocket::disconnectS(const unsigned char socket) {
	switch (disconnect(socket)) {
	case SOCK_OK:
		return true;
		break;
	case SOCKERR_TIMEOUT:
		return false;
		break;
	case SOCK_BUSY:
		return false;
		break;
	default:
		return false;
	}
}
bool tcpsocket::listenS(const unsigned char socket) {
	switch (listen(socket)) {
	case SOCK_OK:
		return true;
		break;
	case SOCKERR_SOCKNUM:
		return false;
		break;
	case SOCKERR_SOCKMODE:
		return false;
		break;
	case SOCKERR_TIMEOUT:
		return false;
		break;
	case SOCK_BUSY:
		return false;
		break;
	}

	return false;
}
bool tcpsocket::readString(const unsigned char &socket, std::string &data) {
	unsigned char buf[1024];

	bool flag = true;
	while (flag) {
		int16_t size = recv(socket, buf, 1024);
		switch (size) {
		case SOCKERR_SOCKNUM:
			return false;
			break;
		case SOCKERR_SOCKMODE:
			return false;
			break;
		case SOCKERR_SOCKSTATUS:
			return false;
			break;
		case SOCKERR_DATALEN:
			return false;
			break;
		case SOCKERR_IPINVALID:
			return false;
			break;
		case SOCKERR_PORTZERO:
			return false;
			break;
		case SOCKERR_SOCKCLOSED:
			return false;
			break;
		case SOCKERR_TIMEOUT:
			return false;
			break;
		case SOCK_BUSY:
			return false;
			break;
		default:
			if (size <= 1024) {
				for (auto i = 0; i < size; i++) {
					data.push_back(buf[i]);
				}
				flag = false;
			} else {
				for (auto i = 0; i < 1024; i++) {
					data.push_back(buf[i]);
				}
			}
			break;
		}
	}
	return true;
}
bool tcpsocket::readS(const unsigned char &socket,
		std::vector<unsigned char> &data) {
	unsigned char buf[1024];
	bool flag = true;
	while (flag) {
		int16_t size = recv(socket, buf, 1024);
		switch (size) {
		case SOCKERR_SOCKNUM:
			return false;
			break;
		case SOCKERR_SOCKMODE:
			return false;
			break;
		case SOCKERR_SOCKSTATUS:
			return false;
			break;
		case SOCKERR_DATALEN:
			return false;
			break;
		case SOCKERR_IPINVALID:
			return false;
			break;
		case SOCKERR_PORTZERO:
			return false;
			break;
		case SOCKERR_SOCKCLOSED:
			return false;
			break;
		case SOCKERR_TIMEOUT:
			return false;
			break;
		case SOCK_BUSY:
			return false;
			break;
		default:
			if (size <= 1024) {
				for (auto i = 0; i < size; i++) {
					data.push_back(buf[i]);
				}
				flag = false;
			} else {
				for (auto i = 0; i < 1024; i++) {
					data.push_back(buf[i]);
				}
			}
		}
	}
	return true;

}
bool tcpsocket::writeS(const unsigned char &socket,
		std::vector<unsigned char> &data) {

	const size_t chunkSize = 1024;
	size_t remainingBytes = data.size();
	size_t offset = 0;

	while (remainingBytes > 0) {
		size_t chunkBytes = std::min(chunkSize, remainingBytes);
		switch (send(socket, (uint8_t*) &data[offset], chunkBytes)) {
		case SOCKERR_SOCKSTATUS:
			return false;
			break;
		case SOCKERR_SOCKMODE:
			return false;
			break;
		case SOCKERR_SOCKNUM:
			return false;
			break;
		case SOCKERR_DATALEN:
			return false;
			break;
		case SOCK_BUSY:
			continue;
			break;
		}
		remainingBytes -= chunkBytes;
		offset += chunkBytes;
	}

	return true;
}
bool tcpsocket::writeS(const unsigned char &socket, std::string &data) {
	const size_t chunkSize = 1024;
	size_t remainingBytes = data.size();
	size_t offset = 0;

	while (remainingBytes > 0) {
		size_t chunkBytes = std::min(chunkSize, remainingBytes);
		switch (send(socket, (uint8_t*) &data[offset], chunkBytes)) {
		case SOCKERR_SOCKSTATUS:
			return false;
			break;
		case SOCKERR_SOCKMODE:
			return false;
			break;
		case SOCKERR_SOCKNUM:
			return false;
			break;
		case SOCKERR_DATALEN:
			return false;
			break;
		case SOCK_BUSY:
			continue;
			break;
		}
		remainingBytes -= chunkBytes;
		offset += chunkBytes;
	}
	return true;
}
bool tcpsocket::w5500device_init(uint8_t (*spi_rb)(void),
		void (*spi_wb)(uint8_t wb), void (*csEnable)(void),
		void (*csDisable)(void)) {
	unsigned char W5500_AdrSet[2][8] = { { 2, 2, 2, 2, 2, 2, 2, 2 }, { 2, 2, 2,
			2, 2, 2, 2, 2 } }; //for 5500
	reg_wizchip_spi_cbfunc(spi_rb, spi_wb);
	reg_wizchip_cs_cbfunc(csEnable, csDisable);
	wizchip_init(bufSize, bufSize);

	/* W5500 Chip Initialization */
	if (ctlwizchip(CW_INIT_WIZCHIP, (void*) W5500_AdrSet) == -1) {
		return false;
	}
	return true;
}
