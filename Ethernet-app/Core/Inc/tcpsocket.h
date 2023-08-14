/*
 * tcpsocket.h
 *
 *  Created on: May 21, 2023
 *      Author: Saeed
 */

#ifndef SRC_TCPSOCKET_H_
#define SRC_TCPSOCKET_H_

#include "w5500.h"
#include "socket.h"
#include "wizchip_conf.h"
#include <vector>
#include <string>

class tcpsocket {
public:
	tcpsocket();
	bool w5500device_init(uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb),void (*csEnable)(void),void (*csDisable)(void));
	bool disconnectS(const unsigned char socket);
	bool connectS(const unsigned char socket, uint8_t flag, uint8_t addr, uint16_t port);
	bool listenS(const unsigned char sockets);
	bool closeS(const unsigned char sockets);
	uint8_t listenWait(const unsigned char sockets);
	bool connected(const unsigned char sockets);
	bool setKeepAlive(const unsigned char sockets, uint8_t time);
	bool Ready(const unsigned char sockets);
	bool connectTo(const unsigned char sockets, const unsigned char ip[4], int port);
	void printInformation();
	bool configSocket(const unsigned char socket, uint8_t flag, uint16_t port);
	bool readS(const unsigned char &socket, std::vector<unsigned char> &data);
	bool readString(const unsigned char &socket, std::string &data);
	bool writeS(const unsigned char &socket, std::vector<unsigned char> &data);
	bool writeS(const unsigned char &socket, std::string &data);
	void config(wiz_NetInfo &config);
	virtual ~tcpsocket();
private:
	std::vector<unsigned char> buffer;
	uint8_t bufSize[8] = {2, 2, 2, 2, 2, 2, 2, 2};
};

#endif /* SRC_TCPSOCKET_H_ */
