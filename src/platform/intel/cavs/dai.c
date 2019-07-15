// SPDX-License-Identifier: BSD-3-Clause
//
// Copyright(c) 2018 Intel Corporation. All rights reserved.
//
// Author: Liam Girdwood <liam.r.girdwood@linux.intel.com>
//         Keyon Jie <yang.jie@linux.intel.com>
//         Rander Wang <rander.wang@intel.com>
//         Janusz Jankowski <janusz.jankowski@linux.intel.com>

#include <sof/alloc.h>
#include <sof/common.h>
#include <sof/dai.h>
#include <sof/dma.h>
#include <sof/drivers/interrupt.h>
#include <sof/hda.h>
#include <sof/memory.h>
#include <sof/spinlock.h>
#include <ipc/dai.h>
#include <ipc/stream.h>
#include <ipc/topology.h>
#include <config.h>

#if CONFIG_CAVS_SSP

#include <sof/ssp.h>

static struct dai ssp[(DAI_NUM_SSP_BASE + DAI_NUM_SSP_EXT)];

#endif

#if CONFIG_CAVS_DMIC

#include <sof/dmic.h>

static struct dai dmic[2];
#endif

#if CONFIG_CAVS_ALH

#include <sof/alh.h>

static struct dai alh[DAI_NUM_ALH_BI_DIR_LINKS];
#endif

static struct dai hda[(DAI_NUM_HDA_OUT + DAI_NUM_HDA_IN)];

static struct dai_type_info dti[] = {
#if CONFIG_CAVS_SSP
	{
		.type = SOF_DAI_INTEL_SSP,
		.dai_array = ssp,
		.num_dais = ARRAY_SIZE(ssp)
	},
#endif
#if CONFIG_CAVS_DMIC
	{
		.type = SOF_DAI_INTEL_DMIC,
		.dai_array = dmic,
		.num_dais = ARRAY_SIZE(dmic)
	},
#endif
	{
		.type = SOF_DAI_INTEL_HDA,
		.dai_array = hda,
		.num_dais = ARRAY_SIZE(hda)
	},
#if CONFIG_CAVS_ALH
	{
		.type = SOF_DAI_INTEL_ALH,
		.dai_array = alh,
		.num_dais = ARRAY_SIZE(alh)
	}
#endif
};

static void ssp_init(void)
{
#if CONFIG_CAVS_SSP
	int i;

	/* init ssp */
	for (i = 0; i < ARRAY_SIZE(ssp); i++) {
		ssp[i].index = i;
		ssp[i].drv = &ssp_driver;
		ssp[i].plat_data.base = SSP_BASE(i);
		ssp[i].plat_data.irq = IRQ_EXT_SSPx_LVL5(i, 0);
		/* Allocate 2 fifos (one for each direction) */
		ssp[i].plat_data.fifo =
			rzalloc(RZONE_SYS, SOF_MEM_CAPS_RAM,
				sizeof(struct dai_plat_fifo_data) * 2);
		ssp[i].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].offset =
			SSP_BASE(i) + SSDR;
		ssp[i].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].handshake =
			DMA_HANDSHAKE_SSP0_TX + 2 * i;
		ssp[i].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].offset =
			SSP_BASE(i) + SSDR;
		ssp[i].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].handshake =
			DMA_HANDSHAKE_SSP0_RX + 2 * i;
		/* initialize spin locks early to enable ref counting */
		spinlock_init(&ssp[i].lock);
	}
#endif
}

static void hda_init(void)
{
	int i;

	/* init hd/a, note that size depends on the platform caps */
	for (i = 0; i < ARRAY_SIZE(hda); i++) {
		hda[i].index = i;
		hda[i].drv = &hda_driver;
		spinlock_init(&hda[i].lock);
	}
}

static void dmic_init(void)
{
#if CONFIG_CAVS_DMIC
	int i;

	/* init dmic */
	for (i = 0; i < ARRAY_SIZE(dmic); i++) {
		dmic[i].index = i;
		dmic[i].drv = &dmic_driver;
		dmic[i].plat_data.base = DMIC_BASE;
		dmic[i].plat_data.irq = IRQ_EXT_DMIC_LVL5(i, 0);
		/* Allocate one fifo (capture only) */
		dmic[i].plat_data.fifo =
			rzalloc(RZONE_SYS, SOF_MEM_CAPS_RAM,
				sizeof(struct dai_plat_fifo_data));
		spinlock_init(&dmic[i].lock);
	}

	/* Testing idea if DMIC FIFOs A and B to access the same microphones
	 * with two different sample rate and PCM format could be presented
	 * similarly as SSP0..N. The difference however is that the DMIC
	 * programming is global and not per FIFO.
	 */

	/* Primary FIFO A */
	dmic[0].plat_data.fifo[0].offset = DMIC_BASE + OUTDATA0;
	dmic[0].plat_data.fifo[0].handshake = DMA_HANDSHAKE_DMIC_CH0;

	/* Secondary FIFO B */
	dmic[1].plat_data.fifo[0].offset = DMIC_BASE + OUTDATA1;
	dmic[1].plat_data.fifo[0].handshake = DMA_HANDSHAKE_DMIC_CH1;
#endif
}

static void alh_init(void)
{
#if CONFIG_CAVS_ALH
	int i;

	for (i = 0; i < ARRAY_SIZE(alh); i++) {
		alh[i].index = i;
		alh[i].drv = &alh_driver;
		spinlock_init(&alh[i].lock);
	}
#endif
}

int dai_init(void)
{
	ssp_init();
	hda_init();
	dmic_init();
	alh_init();

	dai_install(dti, ARRAY_SIZE(dti));
	return 0;
}
