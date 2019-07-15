// SPDX-License-Identifier: BSD-3-Clause
//
// Copyright(c) 2019 Intel Corporation. All rights reserved.
//
// Author: Slawomir Blauciak <slawomir.blauciak@linux.intel.com>

#ifndef __PLATFORM_ALH_H__
#define __PLATFORM_ALH_H__

#include <stdint.h>

/**
 * \brief ALH Handshakes
 * Stream ID -> DMA Handshake map
 * -1 identifies invalid handshakes/streams
 */
const uint8_t alh_handshake_map[] = {
	-1,	/* 0  */
	-1,	/* 1  */
	-1,	/* 2  */
	-1,	/* 3  */
	-1,	/* 4  */
	-1,	/* 5  */
	-1,	/* 6  */
	22,	/* 7  */
	23,	/* 8  */
	24,	/* 9  */
	25,	/* 10 */
	26,	/* 11 */
	27,	/* 12 */
	-1,	/* 13 */
	-1,	/* 14 */
	-1,	/* 15 */
	-1,	/* 16 */
	-1,	/* 17 */
	-1,	/* 18 */
	-1,	/* 19 */
	-1,	/* 20 */
	-1,	/* 21 */
	-1,	/* 22 */
	32,	/* 23 */
	33,	/* 24 */
	34,	/* 25 */
	35,	/* 26 */
	36,	/* 27 */
	37,	/* 28 */
	-1,	/* 29 */
	-1,	/* 30 */
	-1,	/* 31 */
	-1,	/* 32 */
	-1,	/* 33 */
	-1,	/* 34 */
	-1,	/* 35 */
	-1,	/* 36 */
	-1,	/* 37 */
	-1,	/* 38 */
	42,	/* 39 */
	43,	/* 40 */
	44,	/* 41 */
	45,	/* 42 */
	46,	/* 43 */
	47,	/* 44 */
	-1,	/* 45 */
	-1,	/* 46 */
	-1,	/* 47 */
	-1,	/* 48 */
	-1,	/* 49 */
	-1,	/* 50 */
	-1,	/* 51 */
	-1,	/* 52 */
	-1,	/* 53 */
	-1,	/* 54 */
	52,	/* 55 */
	53,	/* 56 */
	54,	/* 57 */
	55,	/* 58 */
	56,	/* 59 */
	57,	/* 60 */
	-1,	/* 61 */
	-1,	/* 62 */
	-1,	/* 63 */
};

#endif /* __PLATFORM_ALH_H__ */
