`# Topology for generic' PLATFORM `board with' CODEC `on SSP' SSP_NUM

# Include topology builder
include(`utils.m4')
include(`dai.m4')
include(`pipeline.m4')
include(`ssp.m4')

# Include TLV library
include(`common/tlv.m4')

# Include Token library
include(`sof/tokens.m4')

# Include DSP configuration
include(`platform/intel/'PLATFORM`.m4')

#
# Define the pipelines
#
# PCM0 -----> volume ----> SRC ------low latency mixer ----> volume ---->  SSP2
#
# PCM0 <---- Volume <---- SSP2
#

# Low Latency capture pipeline 2 on PCM 0 using max 2 channels of s32le.
# 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-low-latency-capture.m4,
	2, 0, 2, s32le,
	1000, 0, 0,
	48000, 48000, 48000)

#
# DAI configuration
#
# SSP port 2 is our only pipeline DAI
#

# playback DAI is SSP2 using 2 periods
# Buffers use s24le format, 1000us deadline on core 0 with priority 1
# this defines pipeline 1. The 'NOT_USED_IGNORED' is due to dependencies
# and is adjusted later with an explicit dapm line.
DAI_ADD(sof/pipe-mixer-dai-playback.m4,
	1, SSP, SSP_NUM, SSP2-Codec,
	NOT_USED_IGNORED, 2, s24le,
	1000, 1, 0, SCHEDULE_TIME_DOMAIN_DMA,
	2, 48000)

# PCM Media Playback pipeline 3 on PCM 0 using max 2 channels of s32le.
# 1000us deadline on core 0 with priority 0
# this is connected to pipeline DAI 1
PIPELINE_PCM_ADD(sof/pipe-pcm-media.m4,
	3, 0, 2, s32le,
	1000, 0, 0,
	8000, 48000, 48000,
	SCHEDULE_TIME_DOMAIN_DMA,
	PIPELINE_PLAYBACK_SCHED_COMP_1)

# Connect pipelines together
SectionGraph."PIPE_NAME" {
	index "0"

	lines [
		# PCM pipeline 3 to DAI pipeline 1
		dapm(PIPELINE_MIXER_1, PIPELINE_SOURCE_3)
	]
}

# capture DAI is SSP2 using 2 periods
# Buffers use s24le format, 1000us deadline on core 0 with priority 0
# this is part of pipeline 2
DAI_ADD(sof/pipe-dai-capture.m4,
	2, SSP, SSP_NUM, SSP2-Codec,
	PIPELINE_SINK_2, 2, s24le,
	1000, 0, 0, SCHEDULE_TIME_DOMAIN_DMA)


# PCM Media
PCM_DUPLEX_ADD(Media, 0, PIPELINE_PCM_3, PIPELINE_PCM_2)

#
# BE configurations - overrides config in ACPI if present
#
DAI_CONFIG(SSP, SSP_NUM, 0, SSP2-Codec,
	   SSP_CONFIG(I2S, SSP_CLOCK(mclk, 19200000, codec_mclk_in),
		      SSP_CLOCK(bclk, 2400000, codec_slave),
		      SSP_CLOCK(fsync, 48000, codec_slave),
		      SSP_TDM(2, 25, 3, 3),
		      SSP_CONFIG_DATA(SSP, SSP_NUM, 24)))
