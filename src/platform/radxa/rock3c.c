/*
	Copyright (c) 2023 Radxa Ltd.
	Author: Nascs <nascs@radxa.com>

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
#include "rock3c.h"

struct platform_t *rock3c = NULL;

static int map[] = {
	/*  GPIO3_A1	GPIO3_A3	GPIO3_A2	GPIO3_B0  */
			97,     99,     98,     104,
	/*  GPIO3_B1	GPIO3_B2	GPIO3_C1	GPIO3_C4  */
			105,    106,    113,    116,
	/*  GPIO1_A0	GPIO1_A1	GPIO4_C6	GPIO4_D1  */
			32,     33,     150,    153,
	/*  GPIO4_C3	GPIO4_C5	GPIO4_C2	GPIO0_D1  */
			147,    149,    146,    25,
	/*  GPIO0_D0                                */
			24,     -1,     -1,     -1,
	/*              GPIO3_B3	GPIO3_B4	GPIO3_C3  */
			-1,     107,    108,    115,
	/*  GPIO3_A4	GPIO1_A4	GPIO3_C2	GPIO3_A7  */
			100,    36,     114,    103,
	/*  GPIO3_A6	GPIO3_A5	GPIO4_B2	GPIO4_B3  */
			102,    101,    138,    139
};

#define _sizeof(arr) (sizeof(arr) / sizeof(arr[0]))

static int rock3cValidGPIO(int pin) {
	if(pin >= 0 && pin < _sizeof(map)) {
		if(map[pin] == -1) {
			return -1;
		}
		return 0;
	} else {
		return -1;
	}
}

static int rock3cSetup(void) {
	rock3c->soc->setup();
	rock3c->soc->setMap(map,_sizeof(map));
	rock3c->soc->setIRQ(map,_sizeof(map));
	return 0;
}

void rock3cInit(void) {
	platform_register(&rock3c,"rock3c");

	rock3c->soc = soc_get("Rockchip","RK356X");
	rock3c->soc->setMap(map,_sizeof(map));

	rock3c->digitalRead = rock3c->soc->digitalRead;
	rock3c->digitalWrite = rock3c->soc->digitalWrite;
	rock3c->pinMode = rock3c->soc->pinMode;
	rock3c->setup = &rock3cSetup;

	rock3c->isr = rock3c->soc->isr;
	rock3c->waitForInterrupt = rock3c->soc->waitForInterrupt;

	rock3c->selectableFd = rock3c->soc->selectableFd;
	rock3c->gc = rock3c->soc->gc; 

	rock3c->validGPIO = &rock3cValidGPIO;
}
