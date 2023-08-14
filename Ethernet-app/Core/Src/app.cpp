/*
 * app.cpp
 *
 *  Created on: May 23, 2023
 *      Author: Afshin
 */

#include "app.h"
#include "uservice.h"
SPI_HandleTypeDef *spiaa;
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

httpRes callback(std::string &input, uservice *userver) {
	http_t req = processHttp(input);
	std::string msg = "New request \"";
	httpRes res;
	if (req.p == "/") {
		res.mode = false;
		res.code = 200;
		res.res =
				"<!DOCTYPE html>\n<html lang=\"en\">\n  <head>\n    <meta charset=\"UTF-8\" />\n    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n    <style>\n      body {\n        margin: 0;\n        padding: 0;\n        background-color: black;\n        text-shadow: 0.1em 0.1em 0.1em black;\n        color: white;\n        font-family: Arial, Helvetica, sans-serif;\n      }\n      input {\n        width: 8em;\n        border-radius: 0.5em;\n        padding: 0.3em;\n        margin-left: 0.2em;\n        box-shadow: 0.1em 0.1em 0.1em black;\n      }\n      .logst {\n        margin: 0;\n        padding: 0;\n      }\n      .container {\n        text-align: center;\n        padding: 1em;\n        margin: 0.5em;\n        border-radius: 1em;\n        box-shadow: 0.2em 0.2em 0.2em black;\n        text-shadow: 0.1em 0.1em 0.1em black;\n        display: block;\n        width: 20em;\n        color: white;\n        background-color: rgb(73, 79, 85);\n        left: 100px;\n        right: 100px;\n        border: 1px solid black;\n      }\n      .containerlog {\n        box-shadow: 0.2em 0.2em 0.2em black;\n        background-color: rgb(73, 79, 85);\n        padding: 1em;\n        margin: 0.5em;\n        overflow: auto;\n        border-radius: 1em;\n        height: 10em;\n      }\n      .containermain {\n        padding: 1em;\n        margin: 0.5em;\n        border-radius: 1em;\n        display: block;\n        background-color: rgb(147, 11, 38);\n        left: 100px;\n        right: 100px;\n        border: 1px solid black;\n      }\n      .inlinec {\n        display: inline-block;\n      }\n    </style>\n    <script>\n      function clickHandle() {\n        let ip = document.getElementById(\"ip\").value;\n        let port = document.getElementById(\"mbp1\").value;\n        let lastlog = document.getElementById(\"log\").innerHTML;\n        log(\"Setting\", \"Local ip set to \" + ip);\n        log(\"Setting\", \"Modbus port set to \" + port);\n      }\n      function log(tag, msg) {\n        let lastlog = document.getElementById(\"log\").innerHTML;\n        document.getElementById(\"log\").innerHTML =\n          lastlog + tag + \": \" + msg + \"<br>\";\n      }\n      function sendSettings(payload) {\n        fetch(\"/backend-url\", {\n          method: \"POST\",\n          headers: {\n            \"Content-Type\": \"application/json\",\n          },\n          body: JSON.stringify(payload),\n        })\n          .then((response) => {\n            if (response.ok) {\n              // Request was successful\n              log(\"Info\", \"Command sent successfully\");\n            } else {\n              // Request failed\n              log(\"Error\", \"Failed to send command\");\n            }\n          })\n          .catch((error) => {\n            log(\"Error\", error);\n          });\n      }\n    </script>\n    <title>Settings</title>\n  </head>\n  <body>\n    <div class=\"containermain\">\n      <div class=\"container\">\n        <p class=\"inlinec\">Local IP:</p>\n        <input\n          class=\"inlinec\"\n          type=\"text\"\n          name=\"ipAddress\"\n          placeholder=\"192.168.1.1\"\n          id=\"ip\"\n        />\n      </div>\n      <div class=\"container\">\n        <p class=\"inlinec\">Modbus port:</p>\n        <input\n          class=\"inlinec\"\n          type=\"text\"\n          name=\"modbusPort\"\n          placeholder=\"502\"\n          id=\"mbp1\"\n        />\n      </div>\n      <div class=\"container\">\n        <p class=\"inlinec\">Registry address:</p>\n        <input\n          class=\"inlinec\"\n          type=\"text\"\n          name=\"modbusPort\"\n          placeholder=\"4000\"\n          id=\"mbp1\"\n        />\n      </div>\n      <div class=\"container\">\n        <button onclick=\"clickHandle()\">Save</button>\n      </div>\n      <div class=\"container\">\n        <button onclick=\"location.href='fw'\">Upgrade</button>\n      </div>\n    </div>\n    <div class=\"containermain\">\n      <h2>Log:</h2>\n      <div class=\"containerlog\">\n        <p id=\"log\" class=\"logst\"></p>\n      </div>\n    </div>\n  </body>\n</html>\n";
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
	} else if (req.p == "/fw") {
		res.mode = false;
		res.code = 200;
		if (req.t == "GET")
			res.res =
					"<!DOCTYPE html>\n<html lang=\"en\">\n  <head>\n    <meta charset=\"UTF-8\" />\n    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n    <title>Update firmware</title>\n    <style>\n      * {\n        margin: 0;\n        padding: 0;\n        background-color: black;\n        color: white;\n      }\n\n      .bb {\n        border-radius: 1em;\n        margin: 1em;\n        padding: 2em;\n        background-color: brown;\n        display: flex;\n        align-items: center;\n      }\n\n      .elem {\n        padding-right: 1em;\n        display: block;\n        padding-top: 0.8em;\n        height: 3em;\n        background-color: transparent;\n      }\n      input[type=\"file\"]::file-selector-button {\n        background-color: rgb(67, 100, 161);\n        border-radius: 0.5em;\n        margin-right: 0.5em;\n        box-shadow: 0.2em 0.2em 0.2em black;\n        padding: 0.5em;\n        color: white;\n      }\n      input[type=\"file\"]::file-selector-button:hover {\n        background-color: rgb(57, 86, 139);\n      }\n      input[type=\"file\"]::file-selector-button:active {\n        background-color: rgb(43, 66, 107);\n      }\n      input[type=\"file\"][disabled]::file-selector-button {\n        background-color: rgb(112, 21, 21);\n      }\n      input[type=\"file\"][disabled]::file-selector-button:hover {\n        background-color: rgb(112, 21, 21);\n      }\n      input[type=\"file\"][disabled]::file-selector-button:active {\n        background-color: rgb(112, 21, 21);\n      }\n      .btn {\n        background-color: rgb(67, 100, 161);\n        border-radius: 0.5em;\n        margin-right: 0.5em;\n        box-shadow: 0.2em 0.2em 0.2em black;\n        padding: 0.5em;\n      }\n      .btn:hover {\n        background-color: rgb(57, 86, 139);\n      }\n      .btn:active {\n        background-color: rgb(43, 66, 107);\n      }\n      .btn:disabled {\n        background-color: rgb(112, 21, 21);\n      }\n    </style>\n    <script>\n      let per = document.getElementById(\"per\");\n      let ff = document.getElementById(\"file-input\");\n      function readFileByteByByte(file) {\n        return new Promise((resolve, reject) => {\n          let reader = new FileReader();\n          reader.onloadend = function () {\n            const arrayBuffer = reader.result;\n            const byteArray = new Uint8Array(arrayBuffer);\n            resolve(byteArray);\n          };\n          reader.onerror = function () {\n            reject(reader.error);\n          };\n          reader.readAsArrayBuffer(file);\n        });\n      }\n      function calculateCRC32(byteArray) {\n        const crcTable = new Uint32Array(256);\n        const polynomial = 0xedb88320;\n        let crc = 0xffffffff;\n        for (let i = 0; i < 256; i++) {\n          let crcValue = i;\n          for (let j = 0; j < 8; j++) {\n            if ((crcValue & 1) === 1) {\n              crcValue = (crcValue >>> 1) ^ polynomial;\n            } else {\n              crcValue >>>= 1;\n            }\n          }\n          crcTable[i] = crcValue;\n        }\n        for (let i = 0; i < byteArray.length; i++) {\n          const byte = byteArray[i];\n          crc = (crc >>> 8) ^ crcTable[(crc ^ byte) & 0xff];\n        }\n        crc = (crc ^ 0xffffffff) >>> 0;\n        const checksum = crc.toString(16).toUpperCase().padStart(8, \"0\");\n        return checksum;\n      }\n      function uint8ArrayToHexString(uint8Array) {\n        return Array.from(uint8Array)\n          .map((byte) => byte.toString(16).padStart(2, \"0\"))\n          .join(\"\");\n      }\n      function uint32ToHexString(uint32Value) {\n        return uint32Value.toString(16).toUpperCase().padStart(8, \"0\");\n      }\n      function postDataWithTimeout(data, cm, tries, timeoutMs) {\n        let url = cm;\n        let xhr = new XMLHttpRequest();\n        xhr.open(\"POST\", url, false);\n        xhr.setRequestHeader(\"Content-Type\", \"application/json\");\n        let isTimedOut = false;\n        if (timeoutMs) {\n          setTimeout(function () {\n            isTimedOut = true;\n            xhr.abort();\n          }, timeoutMs);\n        }\n        xhr.send(JSON.stringify(data));\n        if (isTimedOut) {\n          return false;\n        }\n        if (xhr.status === 200) {\n          return true;\n        } else {\n          if (tries > 0)\n            return postDataWithTimeout(data, cm, tries - 1, timeoutMs);\n          else return false;\n        }\n      }\n      function getData(data, cm) {\n        fetch(url)\n          .then((response) => response.json())\n          .then((data) => {\n            console.log(data);\n          })\n          .catch((error) => {\n            console.error(\"Error:\", error);\n          });\n      }\n      let file;\n      function openfile() {\n        const fileInput = document.getElementById(\"file-input\");\n        fileInput.addEventListener(\"change\", (event) => {\n          file = event.target.files[0];\n        });\n      }\n      let i1 = 0;\n      let oko = false;\n      let byteArrayo;\n      let checksum;\n      function chunk() {\n        const per = document.getElementById(\"per\");\n        if (oko) {\n          const bytes = new Uint8Array(256);\n          let b;\n          for (b = i1; (b < i1 + 256) & (b < byteArrayo.length); b++) {\n            bytes[b - i1] = byteArrayo[b];\n          }\n          let str = uint8ArrayToHexString(bytes);\n          if (\n            postDataWithTimeout(str, \"/update/add/\" + i1, 20, 5000) == false\n          ) {\n            oko = false;\n            per.value = \"Error\";\n          } else {\n            i1 = b;\n            per.value = Math.round((i1 / byteArrayo.length) * 100) + \"%\";\n          }\n        }\n        if (!(i1 < byteArrayo.length)) {\n          oko = false;\n          finn();\n        } else {\n          requestAnimationFrame(chunk);\n        }\n      }\n      function finn() {\n        postDataWithTimeout(byteArrayo.length, \"/update/size/\", 20, 5000);\n        if (postDataWithTimeout(checksum, \"/update/crc/\", 20, 5000)) {\n          uflag();\n          per.value = \"Finished\";\n          rebootc();\n        } else {\n          per.value = \"CRC check failed\";\n        }\n        ff.disabled = false;\n        per.disabled = false;\n      }\n      function uploadfile() {\n        per = document.getElementById(\"per\");\n        ff = document.getElementById(\"file-input\");\n        i1 = 0;\n        if (file) {\n          readFileByteByByte(file)\n            .then((byteArray) => {\n              per.disabled = true;\n              ff.disabled = true;\n              checksum = calculateCRC32(byteArray);\n              per.value = \"Starting\";\n              console.log(\"CRC-32 checksum:\", checksum);\n              byteArrayo = byteArray;\n              postDataWithTimeout(\"erase\", \"/update/cmd/\", 0, 5000);\n              oko = true;\n              requestAnimationFrame(chunk);\n            })\n            .catch((error) => {\n              console.error(\"Error reading file:\", error);\n              per.disabled = false;\n              ff.disabled = false;\n            });\n        }\n      }\n      async function startup() {\n        setTimeout(uploadfile(), 200000);\n      }\n      function uflag() {\n        postDataWithTimeout(\"uflag\", \"/update/cmd/\", 0, 5000);\n      }\n      function rebootc() {\n        postDataWithTimeout(\"reboot\", \"/update/cmd/\", 0, 500);\n      }\n    </script>\n  </head>\n\n  <body>\n    <div class=\"bb\">\n      <input class=\"elem\" type=\"file\" onclick=\"openfile()\" id=\"file-input\" />\n      <input\n        class=\"btn\"\n        type=\"button\"\n        onclick=\"startup()\"\n        id=\"per\"\n        value=\"Upload\"\n      />\n    </div>\n  </body>\n</html>\n";
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

app::app(SPI_HandleTypeDef &hspi1, SPI_HandleTypeDef &hspi2,UART_HandleTypeDef &huart5): spieflash(&hspi1), spia(&hspi2), uarta(&huart5) {
	spiaa = spia;
	HAL_GPIO_WritePin(R_LED_GPIO_Port, R_LED_Pin, GPIO_PIN_SET); //LED HIGH
	HAL_GPIO_WritePin(eth_rst_GPIO_Port, eth_rst_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(eth_rst_GPIO_Port, eth_rst_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	uservice *us = new uservice(spieflash);
	//bool result =
	network.w5500device_init(spi_rb, spi_wb, cs_sel, cs_desel);
	std::string strd;
	wiz_NetInfo netInfo = { .mac = { 0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef },// Mac address
			.ip = { 192, 168, 1, 3 },			// IP address
			.sn = { 255, 255, 255, 0 },			// Subnet mask
			.gw = { 192, 168, 1, 1 }			// Gateway address
	};
	network.config(netInfo);
	wizchip_getnetinfo(&netInfo);

	strd = std::to_string(netInfo.ip[0]) + "." + std::to_string(netInfo.ip[1])
			+ "." + std::to_string(netInfo.ip[2]) + "."
			+ std::to_string(netInfo.ip[3]) + "\r\n\0";
	server1 = new httpServer(0, &network, 80, us);
	server1->start();
	server1->setCallback(callback);
	//send_uart("Starting modbus server...\r\n\0");
	mb1 = new modbus(1,&network);
	//mb1->enable = true;
	//mb1->connect("192.168.1.5", 502);

	m2s = new mosbus2serial(&network,uarta,2);
	m2s->config(100);
	m2s->start();
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

void app::serial_int(){
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
		if (i > 1998)
			mb1->connect("192.168.1.5", 502);
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
		if (i > 1998)
			mb1->connect("192.168.1.5", 502);
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
