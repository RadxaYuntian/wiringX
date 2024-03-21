/*
  Copyright (c) 2024 Shenzhen Milk-V Technology Co., Ltd
  Author: William James <willian@milkv.io>

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "../../soc/soc.h"
#include "../../wiringx.h"
#include "../platform.h"
#include "meles.h"

struct platform_t *milkv_meles = NULL;

static int map[] = {
	/*	GPIO0_17	I2S2_SCLK	GPIO2_25	GPIO2_24	*/
			17,			169,			89,			88,
	/*	GPIO2_22	GPIO2_31	GPIO3_2	GPIO0_16	*/
			86,			95,			98,			16,
	/*	GPIO0_9	GPIO0_8	GPIO2_15	ADC	*/
			8,			9,			79,			-1,
	/*	GPIO2_16	GPIO2_17	GPIO2_14	GPIO2_0	*/
			80,			81,			78,			64,
	/*	GPIO2_1	*/
			65,			-1,			-1,			-1,
	/*						GPIO2_23	GPIO3_1	GPIO0_11	*/
			-1,			87,			97,			11,
	/*	AOGPIO_7	GPIO3_0	GPIO0_10	GPIO3_3	*/
			168,			96,			10,			99,
	/*	AOGPIO_10	AOGPIO_11	GPIO2_12	GPIO2_11	*/
			171,			172,			76,			75
};

#define _sizeof(arr) (sizeof(arr) / sizeof(arr[0]))

static int milkv_melesValidGPIO(int pin) {
	if(pin >= 0 && pin < _sizeof(map)) {
		if(map[pin] == -1) {
			return -1;
		}
		return 0;
	} else {
		return -1;
	}
}

static int milkv_melesSetup(void) {
	milkv_meles->soc->setup();
	milkv_meles->soc->setMap(map, _sizeof(map));
	milkv_meles->soc->setIRQ(map, _sizeof(map));
	return 0;
}

void milkv_melesInit(void) {
	platform_register(&milkv_meles, "milkv_meles");

	milkv_meles->soc = soc_get("T-Head", "TH1520");
	milkv_meles->soc->setMap(map, _sizeof(map));

	milkv_meles->digitalRead = milkv_meles->soc->digitalRead;
	milkv_meles->digitalWrite = milkv_meles->soc->digitalWrite;
	milkv_meles->pinMode = milkv_meles->soc->pinMode;
	milkv_meles->setup = &milkv_melesSetup;

	milkv_meles->isr = milkv_meles->soc->isr;
	milkv_meles->waitForInterrupt = milkv_meles->soc->waitForInterrupt;

	milkv_meles->selectableFd = milkv_meles->soc->selectableFd;
	milkv_meles->gc = milkv_meles->soc->gc;

	milkv_meles->validGPIO = &milkv_melesValidGPIO;
}
