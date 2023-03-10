#include "peripheral-ads1015.h"
#include "esp32-hal-log.h"

// Registers
#define CONVERSION_REGISTER		0x00
#define CONFIG_REGISTER			0x01
#define LO_THRESH_REGISTER		0x02
#define HI_THRESH_REGISTER		0x03

// Registers values and masks
#define CONFIG_H_MODE_CONTIN	0x01
#define CONFIG_H_MODE_SINGLE	0x01
#define CONFIG_H_PGA_0			0x00
#define CONFIG_H_PGA_1			0x02
#define CONFIG_H_PGA_2			0x04
#define CONFIG_H_PGA_3			0x06
#define CONFIG_H_PGA_4			0x08
#define CONFIG_H_PGA_5			0x0a
#define CONFIG_H_PGA_6			0x0c
#define CONFIG_H_PGA_7			0x0e
#define CONFIG_H_MUX_0_1		0x00
#define CONFIG_H_MUX_0_3		0x10
#define CONFIG_H_MUX_1_3		0x20
#define CONFIG_H_MUX_2_3		0x30
#define CONFIG_H_MUX_0			0x40
#define CONFIG_H_MUX_1			0x50
#define CONFIG_H_MUX_2			0x60
#define CONFIG_H_MUX_3			0x70
#define CONFIG_H_OS_START		0x80

#define CONFIG_L_CQUE_AFTER_1	0x00
#define CONFIG_L_CQUE_AFTER_2	0x01
#define CONFIG_L_CQUE_AFTER_4	0x02
#define CONFIG_L_CQUE_NONE		0x03
#define CONFIG_L_CLAT_NONE		0x00
#define CONFIG_L_CLAT_COMP		0x04
#define CONFIG_L_CPOL_LOW		0x00
#define CONFIG_L_CPOL_HIGH		0x08
#define CONFIG_L_CMODE_HYST		0x00
#define CONFIG_L_CMODE_WINDOW	0x10
#define CONFIG_L_DR_128			0x00
#define CONFIG_L_DR_250			0x20
#define CONFIG_L_DR_490			0x40
#define CONFIG_L_DR_920			0x60
#define CONFIG_L_DR_1600		0x80
#define CONFIG_L_DR_2400		0xa0
#define CONFIG_L_DR_3300		0xc0

static const uint16_t i2cTimeout = 50;

bool ads1015_init(uint8_t i2cNum, uint8_t addr) {
	return true;
}

uint16_t ads1015_get_input(uint8_t i2cNum, uint8_t addr, uint8_t index) {
	uint8_t mux = 0;
	switch (index) {
		case 0:
			mux = CONFIG_H_MUX_0;
			break;

		case 1:
			mux = CONFIG_H_MUX_1;
			break;

		case 2:
			mux = CONFIG_H_MUX_2;
			break;

		case 3:
			mux = CONFIG_H_MUX_3;
			break;
	}

	uint8_t buffer[3];
	buffer[0] = CONFIG_REGISTER;
	buffer[1] = CONFIG_H_MODE_SINGLE | CONFIG_H_PGA_1 | CONFIG_H_OS_START | mux;
	buffer[2] = CONFIG_L_CQUE_NONE | CONFIG_L_CLAT_NONE | CONFIG_L_CPOL_LOW | CONFIG_L_CMODE_HYST | CONFIG_L_DR_1600;
	if (i2cWrite(i2cNum, addr, buffer, 3, i2cTimeout) != ESP_OK) {
		return 0;
	}

	delay(1);

	uint8_t convReg = CONVERSION_REGISTER;
	size_t len = 0;
	if (i2cWriteReadNonStop(i2cNum, addr, &convReg, 1, buffer, 2, i2cTimeout, &len) != ESP_OK) {
		return 0;
	}

	uint16_t res = ((buffer[0] << 8) | (buffer[1])) >> 4;
	if (res > 0x07ff) {
		res = 0;
	}

	return res;
}
