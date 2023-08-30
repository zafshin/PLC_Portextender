/*
 * app.cpp
 *
 *  Created on: May 23, 2023
 *      Author: Afshin
 */

#include "app.h"
#include "uservice.h"
#include <sstream>
SPI_HandleTypeDef *spiaa;
//w5500
struct ip_t{
	unsigned char ip[4]={0};
};
ip_t str2ip(std::string ip){
	ip_t result;
	std::string to;
	std::stringstream ss(ip);
	int i = 0;
	while (std::getline(ss, to, '.')) {
		result.ip[i] = std::stoul(to, nullptr, 0);
		i++;
		if (i > 3)
		break;
	}
	return result;
}
void cs_sel(void) {
	HAL_GPIO_WritePin(eth_cs_GPIO_Port, eth_cs_Pin, GPIO_PIN_RESET); //CS LOW
}

void cs_desel(void) {
	HAL_GPIO_WritePin(eth_cs_GPIO_Port, eth_cs_Pin, GPIO_PIN_SET); //CS HIGH
}

uint8_t spi_rb(void) {
	uint8_t rbuf;
	HAL_SPI_Receive(spiaa, &rbuf, 1, HAL_MAX_DELAY);
	return rbuf;
}

void spi_wb(uint8_t b) {
	HAL_SPI_Transmit(spiaa, &b, 1, HAL_MAX_DELAY);
}
//eFlash

httpRes callback(std::string &input, uservice *userver, eflash_t *eflash) {
	http_t req = processHttp(input);
	std::string msg = "New request \"";
	httpRes res;
	if (req.p == "/") {
		res.mode = false;
		res.code = 200;
		ipsetting_t ip = getipsettings(eflash);
		modbussetting_t mb = getmodbussettings(eflash);
		mod2serisetting_t m2s = getmod2serisettings(eflash);
		std::string mben = "checked";
		std::string m2sen = "checked";
		std::string mbmod = "Client";
		std::string m2smod = "Client";
		if (mb.enable == 0)
			mben = "";
		if (m2s.enable == 0)
			m2sen = "";
		if (mb.host == 1)
			mbmod = "Host";
		if (m2s.host == 1)
			m2smod = "Host";
		res.res =
				"<!DOCTYPE html>\n"
						"<html lang=\"en\">\n"
						"  <head>\n"
						"    <meta charset=\"UTF-8\" />\n"
						"    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />\n"
						"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n"
						"    <style>\n"
						"      body {\n"
						"        margin: 0;\n"
						"        padding: 0;\n"
						"        background-color: black;\n"
						"        text-shadow: 0.1em 0.1em 0.1em black;\n"
						"        color: white;\n"
						"        font-family: Arial, Helvetica, sans-serif;\n"
						"      }\n"
						"      input {\n"
						"        width: 8em;\n"
						"        border-radius: 0.5em;\n"
						"        padding: 0.3em;\n"
						"        margin-left: 0.2em;\n"
						"        box-shadow: 0.1em 0.1em 0.1em black;\n"
						"      }\n"
						"      .logst {\n"
						"        margin: 0;\n"
						"        padding: 0;\n"
						"      }\n"
						"      .container {\n"
						"        text-align: center;\n"
						"        padding: 1em;\n"
						"        margin: 0.5em;\n"
						"        border-radius: 1em;\n"
						"        box-shadow: 0.2em 0.2em 0.2em black;\n"
						"        text-shadow: 0.1em 0.1em 0.1em black;\n"
						"        display: block;\n"
						"        width: 20em;\n"
						"        color: white;\n"
						"        background-color: rgb(73, 79, 85);\n"
						"        left: 100px;\n"
						"        right: 100px;\n"
						"        border: 1px solid black;\n"
						"      }\n"
						"      .containerlog {\n"
						"        box-shadow: 0.2em 0.2em 0.2em black;\n"
						"        background-color: rgb(73, 79, 85);\n"
						"        padding: 1em;\n"
						"        margin: 0.5em;\n"
						"        overflow: auto;\n"
						"        border-radius: 1em;\n"
						"        height: 10em;\n"
						"      }\n"
						"      .containermain {\n"
						"        padding: 1em;\n"
						"        margin: 0.5em;\n"
						"        border-radius: 1em;\n"
						"        display: block;\n"
						"        background-color: rgb(147, 11, 38);\n"
						"        left: 100px;\n"
						"        right: 100px;\n"
						"        border: 1px solid black;\n"
						"      }\n"
						"      .containerch {\n"
						"        text-align: center;\n"
						"        padding: 1em;\n"
						"        margin: 0.5em;\n"
						"        border-radius: 1em;\n"
						"        box-shadow: 0.2em 0.2em 0.2em black;\n"
						"        text-shadow: 0.1em 0.1em 0.1em black;\n"
						"        display: inline-grid;\n"
						"        width: 24em;\n"
						"        color: white;\n"
						"        background-color: rgb(73, 79, 85);\n"
						"        left: 100px;\n"
						"        right: 100px;\n"
						"        border: 1px solid black;\n"
						"      }\n"
						"\n"
						"      .chb {\n"
						"        text-shadow: 0 0 0 black;\n"
						"        box-shadow: 0 0 0 black;\n"
						"        width: 1em;\n"
						"        text-align: center;\n"
						"        border-radius: 70%;\n"
						"        height: 20px;\n"
						"        width: 20px;\n"
						"      }\n"
						"      .inlinec {\n"
						"        display: inline-block;\n"
						"      }\n"
						"    </style>\n"
						"    <script>\n"
						"      function clickHandle() {\n"
						"        let ln = \";\";\n"
						"        let ip = document.getElementById(\"ip\").value;\n"
						"        let sn = document.getElementById(\"sn\").value;\n"
						"        let gw = document.getElementById(\"gw\").value;\n"
						"        let ips = ip.split(\".\");\n"
						"        let sns = sn.split(\".\");\n"
						"        let gws = gw.split(\".\");\n"
						"        let firstSets = ips[0] + ln + ips[1] + ln + ips[2] + ln + ips[3] + ln;\n"
						"        firstSets += sns[0] + ln + sns[1] + ln + sns[2] + ln + sns[3] + ln;\n"
						"        firstSets += gws[0] + ln + gws[1] + ln + gws[2] + ln + gws[3] + ln;\n"
						"        let mbip = document.getElementById(\"modbusIP\").value;\n"
						"        let mbport = document.getElementById(\"modbusPORT\").value;\n"
						"        let mbmod = document.getElementById(\"modbusMod\").value;\n"
						"        let mbEN = document.getElementById(\"modbusEn\").checked;\n"
						"        let mb2sbud = document.getElementById(\"modbus2sbud\").value;\n"
						"        let mb2sip = document.getElementById(\"modbus2sIP\").value;\n"
						"        let mb2sport = document.getElementById(\"modbus2sPort\").value;\n"
						"        let mb2smod = document.getElementById(\"modbus2sMod\").value;\n"
						"        let mb2sEN = document.getElementById(\"modbus2sEn\").checked;\n"
						"        sendSettings(\n"
						"          String(\n"
						"            firstSets +\n"
						"              mbip +\n"
						"              ln +\n"
						"              mbport +\n"
						"              ln +\n"
						"              mbmod +\n"
						"              ln +\n"
						"              mbEN +\n"
						"              ln +\n"
						"              mb2sbud +\n"
						"              ln +\n"
						"              mb2sip +\n"
						"              ln +\n"
						"              mb2sport +\n"
						"              ln +\n"
						"              mb2smod +\n"
						"              ln +\n"
						"              mb2sEN\n"
						"          )\n"
						"        );\n"
						"      }\n"
						"      function log(tag, msg) {\n"
						"        let lastlog = document.getElementById(\"log\").innerHTML;\n"
						"        document.getElementById(\"log\").innerHTML =\n"
						"          lastlog + tag + \": \" + msg + \"<br>\";\n"
						"      }\n"
						"      function sendSettings(payload) {\n"
						"        fetch(\"/settings\", {\n"
						"          method: \"POST\",\n"
						"          headers: {\n"
						"            \"Content-Type\": \"application/json\",\n"
						"          },\n"
						"          body: JSON.stringify(payload),\n"
						"        })\n"
						"          .then((response) => {\n"
						"            if (response.ok) {\n"
						"              // Request was successful\n"
						"              log(\"Info\", \"Command sent successfully\");\n"
						"            } else {\n"
						"              // Request failed\n"
						"              log(\"Error\", \"Failed to send command\");\n"
						"            }\n"
						"          })\n"
						"          .catch((error) => {\n"
						"            log(\"Error\", error);\n"
						"          });\n"
						"      }\n"
						"    </script>\n"
						"    <title>Settings</title>\n"
						"  </head>\n"
						"  <body>\n"
						"    <div class=\"containermain\">\n"
						"      <div class=\"containerch\">\n"
						"        <p class=\"inlinec\">Network settings</p>\n"
						"        <div class=\"container\">\n"
						"          <p class=\"inlinec\">Local IP:</p>\n"
						"          <input\n"
						"            class=\"inlinec\"\n"
						"            type=\"text\"\n"
						"            name=\"ipAddress\"\n"
						"            placeholder=\"192.168.1.10\"\n"
						"            id=\"ip\"\n"
						"            value=\""
						+ std::to_string((unsigned int) ip.ip[0]) + "."
						+ std::to_string((unsigned int) ip.ip[1]) + "."
						+ std::to_string((unsigned int) ip.ip[2]) + "."
						+ std::to_string((unsigned int) ip.ip[3]) + "\"\n"
								"          />\n"
								"        </div>\n"
								"        <div class=\"container\">\n"
								"          <p class=\"inlinec\">Submask:</p>\n"
								"          <input\n"
								"            class=\"inlinec\"\n"
								"            type=\"text\"\n"
								"            name=\"Submask\"\n"
								"            placeholder=\"255.255.255.0\"\n"
								"            value=\""
						+ std::to_string((unsigned int) ip.sn[0]) + "."
						+ std::to_string((unsigned int) ip.sn[1]) + "."
						+ std::to_string((unsigned int) ip.sn[2]) + "."
						+ std::to_string((unsigned int) ip.sn[3]) + "\"\n"
								"            id=\"sn\"\n"
								"          />\n"
								"        </div>\n"
								"        <div class=\"container\">\n"
								"          <p class=\"inlinec\">Gateway:</p>\n"
								"          <input\n"
								"            class=\"inlinec\"\n"
								"            type=\"text\"\n"
								"            name=\"Gateway\"\n"
								"            value=\""
						+ std::to_string((unsigned int) ip.gw[0]) + "."
						+ std::to_string((unsigned int) ip.gw[1]) + "."
						+ std::to_string((unsigned int) ip.gw[2]) + "."
						+ std::to_string((unsigned int) ip.gw[3])
						+ "\"\n"
								"            placeholder=\"192.168.1.1\"\n"
								"            id=\"gw\"\n"
								"          />\n"
								"        </div>\n"
								"      </div>\n"
								"      <div class=\"containerch\">\n"
								"        <p class=\"inlinec\">Modbus settings</p>\n"
								"        <div class=\"container\">\n"
								"          <p class=\"inlinec\">Host IP:</p>\n"
								"          <input\n"
								"            class=\"inlinec\"\n"
								"            type=\"text\"\n"
								"            name=\"modbusIP\"\n"
								"            placeholder=\"192.168.1.20\"\n"
								"            value=\""
						+ std::string((const char*) &mb.ip) + "\"\n"
								"            id=\"modbusIP\"\n"
								"          />\n"
								"        </div>\n"
								"        <div class=\"container\">\n"
								"          <p class=\"inlinec\">Port:</p>\n"
								"          <input\n"
								"            class=\"inlinec\"\n"
								"            type=\"text\"\n"
								"            name=\"modbusPort\"\n"
								"            value=\"" + std::to_string(mb.port)
						+ "\"\n"
								"            placeholder=\"502\"\n"
								"            id=\"modbusPORT\"\n"
								"          />\n"
								"        </div>\n"
								"        <div class=\"container\">\n"
								"          <p class=\"inlinec\">Mode:</p>\n"
								"          <select name=\"modbusMod\" id=\"modbusMod\">\n"
								"			 <option selected hidden>" + mbmod
						+ "</option>\n"
								"            <option value=\"Client\">Client</option>\n"
								"            <option value=\"Host\">Host</option>\n"
								"          </select>\n"
								"        </div>\n"
								"        <div class=\"container\">\n"
								"          <label for=\"modbusEn\">Enable:</label>\n"
								"          <input\n"
								"            class=\"chb\"\n"
								"            type=\"checkbox\"\n"
								"            id=\"modbusEn\"\n"
								"            name=\"modbusEn\"\n"
								"            value=\"modbusEn\"\n"
								"            " + mben
						+ "\n"
								"          />\n"
								"        </div>\n"
								"      </div>\n"
								"      <div class=\"containerch\">\n"
								"        <p class=\"inlinec\">Ethernet to RS485 settings</p>\n"
								"        <div class=\"container\">\n"
								"          <p class=\"inlinec\">Baudrate:</p>\n"
								"          <input\n"
								"            class=\"inlinec\"\n"
								"            type=\"text\"\n"
								"            name=\"modbus2sbud\"\n"
								"            placeholder=\"115200\"\n"
								"            value=\"" + std::to_string(m2s.bud)
						+ "\"\n"
								"            id=\"modbus2sbud\"\n"
								"          />\n"
								"        </div>\n"
								"        <div class=\"container\">\n"
								"          <p class=\"inlinec\">Host IP:</p>\n"
								"          <input\n"
								"            class=\"inlinec\"\n"
								"            type=\"text\"\n"
								"            name=\"modbus2sIP\"\n"
								"            placeholder=\"192.168.1.20\"\n"
								"            value=\""
						+ std::string((const char*) &m2s.ip) + "\"\n"
								"            id=\"modbus2sIP\"\n"
								"          />\n"
								"        </div>\n"
								"        <div class=\"container\">\n"
								"          <p class=\"inlinec\">Port:</p>\n"
								"          <input\n"
								"            class=\"inlinec\"\n"
								"            type=\"text\"\n"
								"            name=\"modbus2sPort\"\n"
								"            placeholder=\"502\"\n"
								"            value=\""
						+ std::to_string(m2s.port)
						+ "\"\n"
								"            id=\"modbus2sPort\"\n"
								"          />\n"
								"        </div>\n"
								"        <div class=\"container\">\n"
								"          <p class=\"inlinec\">Mode:</p>\n"
								"          <select name=\"modbus2sMod\" id=\"modbus2sMod\">\n"
								"			 <option selected hidden>" + m2smod
						+ "</option>\n"
								"            <option value=\"Client\">Client</option>\n"
								"            <option value=\"Host\">Host</option>\n"
								"          </select>\n"
								"        </div>\n"
								"        <div class=\"container\">\n"
								"          <label for=\"modbusEn\">Enable:</label>\n"
								"          <input\n"
								"            class=\"chb\"\n"
								"            type=\"checkbox\"\n"
								"            id=\"modbus2sEn\"\n"
								"            name=\"modbus2sEn\"\n"
								"            value=\"modbus2sEn\"\n"
								"            " + m2sen
						+ "\n"
								"          />\n"
								"        </div>\n"
								"      </div>\n"
								"      <div class=\"container\">\n"
								"        <button onclick=\"clickHandle()\">Save</button>\n"
								"      </div>\n"
								"      <div class=\"container\">\n"
								"        <button onclick=\"location.href='fw'\">Upgrade</button>\n"
								"      </div>\n"
								"    </div>\n"
								"    <div class=\"containermain\">\n"
								"      <h2>Log:</h2>\n"
								"      <div class=\"containerlog\">\n"
								"        <p id=\"log\" class=\"logst\"></p>\n"
								"      </div>\n"
								"    </div>\n"
								"  </body>\n"
								"</html>\n"
								"";
	} else if (req.p == "/main") {
		res.mode = false;
		res.code = 200;
		res.res = "<!DOCTYPE html>\n";
		res.res += "<html>\n";
		res.res += "<head>\n";
		res.res += "<title>Example Page</title>\n";
		res.res += "</head>\n";
		res.res += "<body>\n";
		res.res += "<h1>This is main</h1>\n";
		res.res += "<p>The path is :" + req.p + "</p>\n";
		res.res += "</body>\n";
		res.res += "</html>\n";
	} else if (req.p.find("/update") != std::string::npos) {
		res.mode = false;
		userver->ex(input, res);
	} else if (req.p == "/settings") {
		res.mode = false;
		res.code = 503;
		if (req.t == "POST") {
			res.code = 200;
			res.res = "OK";
			int l = 0;

			ipsetting_t s1;
			modbussetting_t s2;
			mod2serisetting_t s3;

			std::stringstream streamData(req.c);
			std::string val;
			const char separator = ';';
			while (std::getline(streamData, val, separator)) {
				size_t size = val.size();
				switch (l) {
				case 0:
					s1.ip[0] = std::stoul(val);
					break;
				case 1:
					s1.ip[1] = std::stoul(val);
					break;
				case 2:
					s1.ip[2] = std::stoul(val);
					break;
				case 3:
					s1.ip[3] = std::stoul(val);
					break;
				case 4:
					s1.sn[0] = std::stoul(val);
					break;
				case 5:
					s1.sn[1] = std::stoul(val);
					break;
				case 6:
					s1.sn[2] = std::stoul(val);
					break;
				case 7:
					s1.sn[3] = std::stoul(val);
					break;
				case 8:
					s1.gw[0] = std::stoul(val);
					break;
				case 9:
					s1.gw[1] = std::stoul(val);
					break;
				case 10:
					s1.gw[2] = std::stoul(val);
					break;
				case 11:
					s1.gw[3] = std::stoul(val);
					break;
				case 12:
					if (sizeof(s2.ip) < size)
						size = sizeof(s2.ip);
					memcpy(&s2.ip, val.c_str(), size+1);
					break;
				case 13:
					s2.port = std::stoul(val);
					break;
				case 14:
					if (val == "Host")
						s2.host = 1;
					else
						s2.host = 0;
					break;
				case 15:
					if (val == "true")
						s2.enable = 1;
					else
						s2.enable = 0;
					break;
				case 16:
					s3.bud = std::stoul(val);
					break;
				case 17:
					if (sizeof(s3.ip) < size)
						size = sizeof(s3.ip);
					memcpy(&s3.ip, val.c_str(), size+1);
					break;
				case 18:
					s3.port = std::stoul(val);
					break;
				case 19:
					if (val == "Host")
						s3.host = 1;
					else
						s3.host = 0;
					break;
				case 20:
					if (val == "true")
						s3.enable = 1;
					else
						s3.enable = 0;
					break;
				}
				l++;
			}
			if(l == 21){
				setsettings(eflash, &s1, &s3, &s2);
				res.reboot= true;
			}
		}
	} else if (req.p == "/fw") {
		res.mode = false;
		res.code = 200;
		if (req.t == "GET")
			res.res =
					"<!DOCTYPE html>\n<html lang=\"en\">\n  <head>\n    <meta charset=\"UTF-8\" />\n    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n    <title>Update firmware</title>\n    <style>\n      * {\n        margin: 0;\n        padding: 0;\n        background-color: black;\n        color: white;\n      }\n\n      .bb {\n        border-radius: 1em;\n        margin: 1em;\n        padding: 2em;\n        background-color: brown;\n        display: flex;\n        align-items: center;\n      }\n\n      .elem {\n        padding-right: 1em;\n        display: block;\n        padding-top: 0.8em;\n        height: 3em;\n        background-color: transparent;\n      }\n      input[type=\"file\"]::file-selector-button {\n        background-color: rgb(67, 100, 161);\n        border-radius: 0.5em;\n        margin-right: 0.5em;\n        box-shadow: 0.2em 0.2em 0.2em black;\n        padding: 0.5em;\n        color: white;\n      }\n      input[type=\"file\"]::file-selector-button:hover {\n        background-color: rgb(57, 86, 139);\n      }\n      input[type=\"file\"]::file-selector-button:active {\n        background-color: rgb(43, 66, 107);\n      }\n      input[type=\"file\"][disabled]::file-selector-button {\n        background-color: rgb(112, 21, 21);\n      }\n      input[type=\"file\"][disabled]::file-selector-button:hover {\n        background-color: rgb(112, 21, 21);\n      }\n      input[type=\"file\"][disabled]::file-selector-button:active {\n        background-color: rgb(112, 21, 21);\n      }\n      .btn {\n        background-color: rgb(67, 100, 161);\n        border-radius: 0.5em;\n        margin-right: 0.5em;\n        box-shadow: 0.2em 0.2em 0.2em black;\n        padding: 0.5em;\n      }\n      .btn:hover {\n        background-color: rgb(57, 86, 139);\n      }\n      .btn:active {\n        background-color: rgb(43, 66, 107);\n      }\n      .btn:disabled {\n        background-color: rgb(112, 21, 21);\n      }\n    </style>\n    <script>\n      let per = document.getElementById(\"per\");\n      let ff = document.getElementById(\"file-input\");\n      function readFileByteByByte(file) {\n        return new Promise((resolve, reject) => {\n          let reader = new FileReader();\n          reader.onloadend = function () {\n            const arrayBuffer = reader.result;\n            const byteArray = new Uint8Array(arrayBuffer);\n            resolve(byteArray);\n          };\n          reader.onerror = function () {\n            reject(reader.error);\n          };\n          reader.readAsArrayBuffer(file);\n        });\n      }\n      function calculateCRC32(byteArray) {\n        const crcTable = new Uint32Array(256);\n        const polynomial = 0xedb88320;\n        let crc = 0xffffffff;\n        for (let i = 0; i < 256; i++) {\n          let crcValue = i;\n          for (let j = 0; j < 8; j++) {\n            if ((crcValue & 1) === 1) {\n              crcValue = (crcValue >>> 1) ^ polynomial;\n            } else {\n              crcValue >>>= 1;\n            }\n          }\n          crcTable[i] = crcValue;\n        }\n        for (let i = 0; i < byteArray.length; i++) {\n          const byte = byteArray[i];\n          crc = (crc >>> 8) ^ crcTable[(crc ^ byte) & 0xff];\n        }\n        crc = (crc ^ 0xffffffff) >>> 0;\n        const checksum = crc.toString(16).toUpperCase().padStart(8, \"0\");\n        return checksum;\n      }\n      function uint8ArrayToHexString(uint8Array) {\n        return Array.from(uint8Array)\n          .map((byte) => byte.toString(16).padStart(2, \"0\"))\n          .join(\"\");\n      }\n      function uint32ToHexString(uint32Value) {\n        return uint32Value.toString(16).toUpperCase().padStart(8, \"0\");\n      }\n      function postDataWithTimeout(data, cm, tries, timeoutMs) {\n        let url = cm;\n        let xhr = new XMLHttpRequest();\n        xhr.open(\"POST\", url, false);\n        xhr.setRequestHeader(\"Content-Type\", \"application/json\");\n        let isTimedOut = false;\n        if (timeoutMs) {\n          setTimeout(function () {\n            isTimedOut = true;\n            xhr.abort();\n          }, timeoutMs);\n        }\n        xhr.send(JSON.stringify(data));\n        if (isTimedOut) {\n          return false;\n        }\n        if (xhr.status === 200) {\n          return true;\n        } else {\n          if (tries > 0)\n            return postDataWithTimeout(data, cm, tries - 1, timeoutMs);\n          else return false;\n        }\n      }\n      function getData(data, cm) {\n        fetch(url)\n          .then((response) => response.json())\n          .then((data) => {\n            console.log(data);\n          })\n          .catch((error) => {\n            console.error(\"Error:\", error);\n          });\n      }\n      let file;\n      function openfile() {\n        const fileInput = document.getElementById(\"file-input\");\n        fileInput.addEventListener(\"change\", (event) => {\n          file = event.target.files[0];\n        });\n      }\n      let i1 = 0;\n      let oko = false;\n      let byteArrayo;\n      let checksum;\n      function chunk() {\n        const per = document.getElementById(\"per\");\n        if (oko) {\n          const bytes = new Uint8Array(256);\n          let b;\n          for (b = i1; (b < i1 + 256) & (b < byteArrayo.length); b++) {\n            bytes[b - i1] = byteArrayo[b];\n          }\n          let str = uint8ArrayToHexString(bytes);\n          if (\n            postDataWithTimeout(str, \"/update/add/\" + i1, 20, 5000) == false\n          ) {\n            oko = false;\n            per.value = \"Error\";\n          } else {\n            i1 = b;\n            per.value = Math.round((i1 / byteArrayo.length) * 100) + \"%\";\n          }\n        }\n        if (!(i1 < byteArrayo.length)) {\n          oko = false;\n          finn();\n        } else {\n          requestAnimationFrame(chunk);\n        }\n      }\n      function finn() {\n        postDataWithTimeout(byteArrayo.length, \"/update/size/\", 20, 5000);\n        if (postDataWithTimeout(checksum, \"/update/crc/\", 20, 5000)) {\n          uflag();\n          per.value = \"Finished\";\n          rebootc();\n        } else {\n          per.value = \"CRC check failed\";\n        }\n        ff.disabled = false;\n        per.disabled = true;\n      }\n      function uploadfile() {\n        per = document.getElementById(\"per\");\n        ff = document.getElementById(\"file-input\");\n        i1 = 0;\n        if (file) {\n          readFileByteByByte(file)\n            .then((byteArray) => {\n              per.disabled = true;\n              ff.disabled = true;\n              checksum = calculateCRC32(byteArray);\n              per.value = \"Starting\";\n              console.log(\"CRC-32 checksum:\", checksum);\n              byteArrayo = byteArray;\n              postDataWithTimeout(\"erase\", \"/update/cmd/\", 0, 5000);\n              oko = true;\n              requestAnimationFrame(chunk);\n            })\n            .catch((error) => {\n              console.error(\"Error reading file:\", error);\n              per.disabled = false;\n              ff.disabled = false;\n            });\n        }\n      }\n      async function startup() {\n        setTimeout(uploadfile(), 200000);\n      }\n      function uflag() {\n        postDataWithTimeout(\"uflag\", \"/update/cmd/\", 0, 5000);\n      }\n      function rebootc() {\n        postDataWithTimeout(\"reboot\", \"/update/cmd/\", 0, 500);\n      }\n    </script>\n  </head>\n\n  <body>\n    <div class=\"bb\">\n      <input class=\"elem\" type=\"file\" onclick=\"openfile()\" id=\"file-input\" />\n      <input\n        class=\"btn\"\n        type=\"button\"\n        onclick=\"startup()\"\n        id=\"per\"\n        value=\"Upload\"\n      />\n    </div>\n  </body>\n</html>\n";
	} else {
		res.mode = false;
		res.code = 404;
		res.res = "<!DOCTYPE html>\n";
		res.res += "<html>\n";
		res.res += "<head>\n";
		res.res += "<title>404 Page not found</title>\n";
		res.res += "</head>\n";
		res.res += "<body>\n";
		res.res += "<h1>404 Page not found</h1>\n";
		res.res += "</body>\n";
		res.res += "</html>\n";
	}
	return res;
}

app::app(SPI_HandleTypeDef &hspi1, SPI_HandleTypeDef &hspi2,
		UART_HandleTypeDef &huart5) :
		spieflash(&hspi1), spia(&hspi2), uarta(&huart5) {
	spiaa = spia;
	flash.CS_Pin = CS_flash_Pin;
	flash.CS_Port = CS_flash_GPIO_Port;
	flash.spi = spieflash;
	HAL_GPIO_WritePin(R_LED_GPIO_Port, R_LED_Pin, GPIO_PIN_SET); //LED HIGH
	HAL_GPIO_WritePin(eth_rst_GPIO_Port, eth_rst_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(eth_rst_GPIO_Port, eth_rst_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	uservice *us = new uservice(spieflash);
	network.w5500device_init(spi_rb, spi_wb, cs_sel, cs_desel);
	std::string strd;
	ipsetting_t s = getipsettings(&flash);
	modbussetting_t s2 = getmodbussettings(&flash);
	mod2serisetting_t s3 = getmod2serisettings(&flash);
	if (s.ip[0] == 255 && s.ip[1] == 255 && s.ip[2] == 255 && s.ip[3] == 255) {
		s.ip[0] = 192;
		s.ip[1] = 168;
		s.ip[2] = 1;
		s.ip[3] = 10;
		s.sn[0] = 255;
		s.sn[1] = 255;
		s.sn[2] = 255;
		s.sn[3] = 0;
		s.gw[0] = 192;
		s.gw[1] = 168;
		s.gw[2] = 1;
		s.gw[3] = 1;
		s.mode = 1;
		const char cip[] = "192.168.1.100\0";
		s2.enable = 0;
		s2.host = 1;
		s2.port = 100;
		s3.bud = 115200;
		s3.enable = 0;
		s3.host = 1;
		s3.port = 101;
		memcpy(s2.ip, cip, sizeof(cip));
		memcpy(s3.ip, cip, sizeof(cip));
		setsettings(&flash, &s, &s3, &s2);
	}
	wiz_NetInfo netInfo = { .mac = { 0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef } };
	netInfo.dhcp = (dhcp_mode) s.mode;
	memcpy(netInfo.ip, s.ip, sizeof(s.ip));
	memcpy(netInfo.sn, s.sn, sizeof(s.sn));
	memcpy(netInfo.gw, s.gw, sizeof(s.gw));

	network.config(netInfo);
	wizchip_getnetinfo(&netInfo);

	strd = std::to_string(netInfo.ip[0]) + "." + std::to_string(netInfo.ip[1])
			+ "." + std::to_string(netInfo.ip[2]) + "."
			+ std::to_string(netInfo.ip[3]) + "\r\n\0";
	server1 = new httpServer(0, &network, 80, us, &flash);
	server1->start();
	server1->setCallback(callback);
	//send_uart("Starting modbus server...\r\n\0");
	mb1 = new modbus(1, &network);

	if (s2.enable == 1) {
		mb1->enable = true;
		if (s2.host)
			mb1->host(s2.port);
//		else
//			mb1->connect((const char*) &s2.ip, s2.port);
	}

	m2s = new mosbus2serial(&network, uarta, 2);

	if (s3.enable == 1) {
		uarta->Init.BaudRate = s3.bud;
		if (HAL_UART_Init(uarta) != HAL_OK)
		  {
			uarta->Init.BaudRate = 115200;
			if (HAL_UART_Init(uarta) != HAL_OK)
				Error_Handler();
		  }
		if(s3.host==1)
			m2s->config(s3.port);
		else{
			ip_t ii = str2ip(s3.ip);
			m2s->connect(ii.ip, s3.port);
		}
		m2s->start();
	}
	HAL_GPIO_WritePin(G_LED_GPIO_Port, G_LED_Pin, GPIO_PIN_SET); //LED HIGH
	HAL_GPIO_WritePin(R_LED_GPIO_Port, R_LED_Pin, GPIO_PIN_RESET); //LED HIGH
}

app::~app() {
	// TODO Auto-generated destructor stub
}

std::string vectorToHexString(const std::vector<unsigned char> &vec) {
	std::stringstream ss;
	ss << std::hex << std::setfill('0');

	for (const auto &val : vec) {
		ss << std::setw(2) << static_cast<int>(val);
	}
	return ss.str();
}

void app::serial_int() {
	m2s->irq();
}
void app::readMB() {
	std::vector<unsigned char> req = { 0x2, 0x4, 0x0, 0x0, 0x8 };
	mb1->sentReq(req);
	int i = 0;
	while (i < 2000) {
		i++;
		if (mb1->updateNow())
			break;
//		if (i > 1998)
//			mb1->connect("192.168.1.5", 502);
	}
	std::vector<unsigned char> res = mb1->getRes();
	std::string a = vectorToHexString(res);
	a += "\r\n\0";
}
void app::writeMB() {
	std::vector<unsigned char> req = { 0x1, 0x5, 0x0, 0x4, 0x8 };
	mb1->sentReq(req);
	int i = 0;
	while (i < 2000) {
		i++;
		if (mb1->updateNow())
			break;
//		if (i > 1998)
//			mb1->connect("192.168.1.5", 502);
	}
	std::vector<unsigned char> res = mb1->getRes();
	std::string a = vectorToHexString(res);
	a += "\r\n\0";
}

void app::loopModbus() {
	readMB();
	writeMB();
}

void app::loop() {
	//loopModbus();
	server1->updateNow();
	m2s->refresh();
	if (HAL_GPIO_ReadPin(reset_factory_GPIO_Port, reset_factory_Pin)) {
		NVIC_SystemReset();
	}

}
