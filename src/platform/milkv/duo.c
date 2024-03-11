/*
  Copyright (c) 2024 Shenzhen Milk-V Technology Co., Ltd
  Author: Willian <willian@milkv.io>
          Carbon <carbon@milkv.io>

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
#include "duo.h"

struct platform_t *milkv_duo = NULL;

static int map[] = {
	/* XGPIOA[28]	XGPIOA[29]	PWR_GPIO[26]	PWR_GPIO[25]	*/
			0,			1,			2,			3,

	/* PWR_GPIO[19]	PWR_GPIO[20]	PWR_GPIO[23]	PWR_GPIO[22]	*/
			4,			5,			6,			7,

	/* PWR_GPIO[21]	PWR_GPIO[18]	XGPIOC[9]	XGPIOC[10]	*/
	   	8,			9,			10,			11,

	/* XGPIOA[16]	XGPIOA[17]	XGPIOA[14]	XGPIOA[15]	*/
	  	12,			13,			14,			15,

	/* XGPIOA[23]	XGPIOA[24]	XGPIOA[22]	XGPIOA[25]	*/
	  	16,			17,			18,			19,

	/* XGPIOA[27]	XGPIOA[26]	PWR_GPIO[4]   NULL	*/
	  	20,			21,			22,			-1,

	/* NULL	XGPIOC[24]	XGPIOB[3]	XGPIOB[6]	*/
	  	-1,			25,			23,			24,

	/* NULL	NULL	NULL	NULL 	*/
	  	-1,			-1,			-1,			-1
};

#define _sizeof(arr) (sizeof(arr) / sizeof(arr[0]))

static int duoValidGPIO(int pin) {
	if(pin >= 0 && pin < _sizeof(map)) {
		if(map[pin] == -1) {
			return -1;
		}
		return 0;
	} else {
		return -1;
	}
}

static int duoSetup(void) {
	milkv_duo->soc->setup();
	milkv_duo->soc->setMap(map, _sizeof(map));
	milkv_duo->soc->setIRQ(map, _sizeof(map));
	return 0;
}

void milkv_duoInit(void) {
	platform_register(&milkv_duo, "milkv_duo");

	milkv_duo->soc = soc_get("Sophgo", "CV180");
	milkv_duo->soc->setMap(map, _sizeof(map));

	milkv_duo->digitalRead = milkv_duo->soc->digitalRead;
	milkv_duo->digitalWrite = milkv_duo->soc->digitalWrite;
	milkv_duo->pinMode = milkv_duo->soc->pinMode;
	milkv_duo->setup = &duoSetup;

	milkv_duo->isr = milkv_duo->soc->isr;
	milkv_duo->waitForInterrupt = milkv_duo->soc->waitForInterrupt;

	milkv_duo->selectableFd = milkv_duo->soc->selectableFd;
	milkv_duo->gc = milkv_duo->soc->gc;

	milkv_duo->validGPIO = &duoValidGPIO;
}
