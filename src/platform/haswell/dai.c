// SPDX-License-Identifier: BSD-3-Clause
//
// Copyright(c) 2016 Intel Corporation. All rights reserved.
//
// Author: Liam Girdwood <liam.r.girdwood@linux.intel.com>

#include <sof/alloc.h>
#include <sof/common.h>
#include <sof/dai.h>
#include <sof/dma.h>
#include <sof/drivers/interrupt.h>
#include <sof/ssp.h>
#include <ipc/dai.h>
#include <ipc/stream.h>
#include <ipc/topology.h>

static struct dai ssp[2];

static void ssp_init(void)
{
	int i;

	/* init ssp */
	for (i = 0; i < ARRAY_SIZE(ssp); i++) {
		ssp[i].index = i;
		ssp[i].drv = &ssp_driver;
		/* Allocate 2 fifos (one for each direction) */
		ssp[i].plat_data.fifo =
			rzalloc(RZONE_SYS, SOF_MEM_CAPS_RAM,
				sizeof(struct dai_plat_fifo_data) * 2);
	}

	ssp[0].plat_data.base = SSP0_BASE;
	ssp[0].plat_data.irq = IRQ_NUM_EXT_SSP0;
	ssp[0].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].offset =
		SSP0_BASE + SSDR;
	ssp[0].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].offset =
		SSP0_BASE + SSDR;
	ssp[0].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].handshake =
		DMA_HANDSHAKE_SSP0_TX;
	ssp[0].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].handshake =
		DMA_HANDSHAKE_SSP0_RX;

	ssp[1].plat_data.base = SSP1_BASE;
	ssp[1].plat_data.irq = IRQ_NUM_EXT_SSP1;
	ssp[1].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].offset =
		SSP1_BASE + SSDR;
	ssp[1].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].offset =
		SSP1_BASE + SSDR;
	ssp[1].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].handshake =
		DMA_HANDSHAKE_SSP1_TX;
	ssp[1].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].handshake =
		DMA_HANDSHAKE_SSP1_RX;
}

static struct dai_type_info dti[] = {
	{
		.type = SOF_DAI_INTEL_SSP,
		.dai_array = ssp,
		.num_dais = ARRAY_SIZE(ssp)
	}
};

int dai_init(void)
{
	ssp_init();

	dai_install(dti, ARRAY_SIZE(dti));
	return 0;
}
