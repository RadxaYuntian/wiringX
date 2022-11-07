/*
	Copyright (c) 2022  Radxa Ltd.
        Author: Nascs <nascs@radxa.com>

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef __WIRINGX_RADXA_CM3_IO_H_
#define __WIRINGX_RADXA_CM3_IO_H_

#include "../platform.h"

extern struct platform_t *radxa_cm3_io;

void radxa_cm3_io_init(void);

#endif
