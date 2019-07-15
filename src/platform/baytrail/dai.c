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
#include <sof/memory.h>
#include <sof/ssp.h>
#include <ipc/dai.h>
#include <ipc/stream.h>
#include <ipc/topology.h>
#include <config.h>

#if defined CONFIG_CHERRYTRAIL
static struct dai ssp[6];
#else
static struct dai ssp[3];
#endif

static void ssp_init(void)
{
	int i;

	/* init ssp */
	ssp[0].plat_data.base = SSP0_BASE;
	ssp[0].plat_data.irq = IRQ_NUM_EXT_SSP0;

	ssp[1].plat_data.base = SSP1_BASE;
	ssp[1].plat_data.irq = IRQ_NUM_EXT_SSP1;

	ssp[2].plat_data.base = SSP2_BASE;
	ssp[2].plat_data.irq = IRQ_NUM_EXT_SSP2;

#if defined CONFIG_CHERRYTRAIL
	ssp[3].plat_data.base = SSP3_BASE;
	ssp[3].plat_data.irq = IRQ_NUM_EXT_SSP0;

	ssp[4].plat_data.base = SSP4_BASE;
	ssp[4].plat_data.irq = IRQ_NUM_EXT_SSP1;

	ssp[5].plat_data.base = SSP5_BASE;
	ssp[5].plat_data.irq = IRQ_NUM_EXT_SSP2;
#endif

	for (i = 0; i < ARRAY_SIZE(ssp); i++) {
		ssp[i].index = i;
		ssp[i].drv = &ssp_driver;
		/* Allocate 2 fifos (one for each direction) */
		ssp[i].plat_data.fifo =
			rzalloc(RZONE_SYS, SOF_MEM_CAPS_RAM,
				sizeof(struct dai_plat_fifo_data) * 2);

		ssp[i].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].offset =
			ssp[i].plat_data.base + SSDR;
		ssp[i].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].offset =
			ssp[i].plat_data.base + SSDR;
	}

	ssp[0].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].handshake =
		DMA_HANDSHAKE_SSP0_TX;
	ssp[0].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].handshake =
		DMA_HANDSHAKE_SSP0_RX;

	ssp[1].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].handshake =
		DMA_HANDSHAKE_SSP1_TX;
	ssp[1].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].handshake =
		DMA_HANDSHAKE_SSP1_RX;

	ssp[2].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].handshake =
		DMA_HANDSHAKE_SSP2_TX;
	ssp[2].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].handshake =
		DMA_HANDSHAKE_SSP2_RX;

#if defined CONFIG_CHERRYTRAIL
	ssp[3].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].handshake =
		DMA_HANDSHAKE_SSP3_TX;
	ssp[3].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].handshake =
		DMA_HANDSHAKE_SSP3_RX;

	ssp[4].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].handshake =
		DMA_HANDSHAKE_SSP4_TX;
	ssp[4].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].handshake =
		DMA_HANDSHAKE_SSP4_RX;

	ssp[5].plat_data.fifo[SOF_IPC_STREAM_PLAYBACK].handshake =
		DMA_HANDSHAKE_SSP5_TX;
	ssp[5].plat_data.fifo[SOF_IPC_STREAM_CAPTURE].handshake =
		DMA_HANDSHAKE_SSP5_RX;
#endif
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
