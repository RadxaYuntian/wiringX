/*
   Copyright (c) 2022 Radxa Ltd.
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
#include "radxa_cm3_io.h"

struct platform_t *radxa_cm3_io = NULL;

static int map[] = {
    /*  GPIO0_C7     GPIO3_C7     GPIO0_B7    GPIO0_C3    */
		 23,			119,		 15,		 19,
    /*  GPIO3_D4     GPIO3_D3     GPIO3_C6    GPIO3_D5    */
		124,			123,		118,        125,
    /*  GPIO0_B6     GPIO0_B5     GPIO4_A6    GPIO4_D1    */
         14,             13,        134,         -1,
    /*  GPIO4_B2     GPIO4_B0     GPIO4_B3    GPIO0_D1    */
        138,            136,        139,         25,
    /*  GPIO0_D0                                          */
         24,             -1,         -1,         -1,
    /*               GPIO4_B1     GPIO0_C5     GPIO0_C6   */
         -1,            137,         21,         22,
    /*  GPIO3_D0     GPIO0_C2     GPIO4_C0     GPIO4_A7   */
        120,             18,        144,        135,
    /*   GPIO3_D2     GPIO3_D1     GPIO4_B4      GPIO4_B5  */
        122,            121,        140,        141
};

#define _sizeof(arr) (sizeof(arr) / sizeof(arr[0]))

static int radxa_cm3_io_ValidGPIO(int pin) {
    if(pin >= 0 && pin < _sizeof(map)) {
        if(map[pin] == -1) {
            return -1;
        }
        return 0;
    } else {
        return -1;
    }
}

static int radxa_cm3_io_Setup(void) {
    radxa_cm3_io->soc->setup();
    radxa_cm3_io->soc->setMap(map,_sizeof(map));
    radxa_cm3_io->soc->setIRQ(map,_sizeof(map));
    return 0;
}

void radxa_cm3_io_init(void) {
    platform_register(&radxa_cm3_io,"radxa_cm3_io");

    radxa_cm3_io->soc = soc_get("Rockchip","RK356X");
    radxa_cm3_io->soc->setMap(map,_sizeof(map));

    radxa_cm3_io->digitalRead = radxa_cm3_io->soc->digitalRead;
    radxa_cm3_io->digitalWrite = radxa_cm3_io->soc->digitalWrite;
    radxa_cm3_io->pinMode = radxa_cm3_io->soc->pinMode;
    radxa_cm3_io->setup = &radxa_cm3_io_Setup;

    radxa_cm3_io->isr = radxa_cm3_io->soc->isr;
    radxa_cm3_io->waitForInterrupt = radxa_cm3_io->soc->waitForInterrupt;

    radxa_cm3_io->selectableFd = radxa_cm3_io->soc->selectableFd;
    radxa_cm3_io->gc = radxa_cm3_io->soc->gc;

    radxa_cm3_io->validGPIO = &radxa_cm3_io_ValidGPIO;
}
