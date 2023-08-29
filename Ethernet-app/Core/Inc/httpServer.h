/*
 * httpServer.h
 *
 *  Created on: 23 May 2023
 *      Author: Afshin
 */

#ifndef SRC_HTTPSERVER_H_
#define SRC_HTTPSERVER_H_
#include <string>
#include "tcpsocket.h"
#include "uservice.h"
#include "eflashset.h"
#include <vector>
#include "ethtypes.h"

std::string getPath(std::string &input);
http_t processHttp(std::string &input);
class httpServer {
public:
	httpServer(unsigned char sNum, tcpsocket *rinstance, uint16_t p,
			uservice *us, eflash_t *eflashi);
	virtual ~httpServer();
	void start();
	bool sendHTTP(std::string &input, int &code, bool &mode);
	void setCallback(httpRes (*callb)(std::string&, uservice *userver, eflash_t *eflash));
	bool updateNow();
private:
	httpRes (*refCallback)(std::string&, uservice *userver, eflash_t *eflash) = nullptr;
	uservice *userver;
	tcpsocket *refInstance;
	unsigned char socknum;
	uint16_t port;
	eflash_t *eflash;
};

#endif /* SRC_HTTPSERVER_H_ */
