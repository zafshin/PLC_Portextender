/*
 * eflashset.h
 *
 *  Created on: Aug 23, 2023
 *      Author: Afshin
 */

#ifndef INC_EFLASHSET_H_
#define INC_EFLASHSET_H_
#include "eflash.h"
#ifdef __cplusplus
 extern "C" {
#endif


struct ipsetting_t{
	uint8_t mac[6];
	uint8_t ip[4];
	uint8_t sn[4];
	uint8_t gw[4];
	char mode;

};

struct mod2serisetting_t{
	char ip[20];
	unsigned short port;
	uint32_t bud;
	char enable;
	char host;

};

struct modbussetting_t{
	char ip[20];
	unsigned short port;
	char enable;
	char host;

};

struct ipsetting_t getipsettings(struct eflash_t *flash);
void setsettings(struct eflash_t *flash, struct ipsetting_t *ip, struct mod2serisetting_t *m2s, struct modbussetting_t *mod);
struct mod2serisetting_t getmod2serisettings(struct eflash_t *flash);
struct modbussetting_t getmodbussettings(struct eflash_t *flash);


#ifdef __cplusplus
}
#endif
#endif /* INC_EFLASHSET_H_ */
