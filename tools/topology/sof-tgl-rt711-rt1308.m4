#
# Topology for Tigerlake with rt711 + rt1308 codec + DMIC + 4 HDMI
#

# Include topology builder
include(`utils.m4')
include(`dai.m4')
include(`pipeline.m4')
include(`ssp.m4')

# Include TLV library
include(`common/tlv.m4')

# Include Token library
include(`sof/tokens.m4')

# Include Tigerlake DSP configuration
include(`platform/intel/tgl.m4')
include(`platform/intel/dmic.m4')

DEBUG_START

#
# Define the pipelines
#

# PCM0 ---> volume ----> ALH 2 BE dailink 0
# PCM1 <--- volume <---- ALH 3 BE dailink 1
# PCM2 ----> volume -----> SSP2 BE dailink 2
# PCM3 <---- volume <----- DMIC01 (dmic0 capture) BE dailink 3
# PCM4 <---------------- DMIC16k (dmic16k, BE dailink 4)

ifelse(HDMI, `1',
`
# PCM5 ----> volume -----> iDisp1 (HDMI/DP playback, BE link 5)
# PCM6 ----> volume -----> iDisp2 (HDMI/DP playback, BE link 6)
# PCM7 ----> volume -----> iDisp3 (HDMI/DP playback, BE link 7)
# PCM8 ----> volume -----> iDisp3 (HDMI/DP playback, BE link 8)
')

dnl PIPELINE_PCM_ADD(pipeline,
dnl     pipe id, pcm, max channels, format,
dnl     period, priority, core,
dnl     pcm_min_rate, pcm_max_rate, pipeline_rate,
dnl     time_domain, sched_comp)

# Low Latency playback pipeline 1 on PCM 0 using max 2 channels of s32le.
# Schedule 48 frames per 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-volume-playback.m4,
	1, 0, 2, s32le,
	1000, 0, 0,
	48000, 48000, 48000)

# Low Latency capture pipeline 2 on PCM 1 using max 2 channels of s32le.
# Schedule 48 frames per 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-volume-capture.m4,
	2, 1, 2, s32le,
	1000, 0, 0,
	48000, 48000, 48000)

# Low Latency playback pipeline 3 on PCM 2 using max 2 channels of s24le.
# Schedule 48 frames per 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-volume-playback.m4,
	3, 2, 2, s24le,
	1000, 0, 0,
	48000, 48000, 48000)

# Passthrough capture pipeline 4 on PCM 3 using max 4 channels.
# Schedule 48 frames per 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-passthrough-capture.m4,
	4, 3, 4, s32le,
	1000, 0, 0,
	48000, 48000, 48000)

# Passthrough capture pipeline 5 on PCM 4 using max 4 channels.
# Schedule 48 frames per 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-passthrough-capture.m4,
	5, 4, 4, s32le,
	1000, 0, 0,
	48000, 48000, 48000)

ifelse(HDMI, `1',
`
# Low Latency playback pipeline 6 on PCM 5 using max 2 channels of s32le.
# Schedule 48 frames per 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-volume-playback.m4,
	6, 5, 2, s32le,
	1000, 0, 0,
	48000, 48000, 48000)

# Low Latency playback pipeline 7 on PCM 6 using max 2 channels of s32le.
# Schedule 48 frames per 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-volume-playback.m4,
	7, 6, 2, s32le,
	1000, 0, 0,
	48000, 48000, 48000)

# Low Latency playback pipeline 8 on PCM 7 using max 2 channels of s32le.
# Schedule 48 frames per 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-volume-playback.m4,
	8, 7, 2, s32le,
	1000, 0, 0,
	48000, 48000, 48000)

# Low Latency playback pipeline 9 on PCM 8 using max 2 channels of s32le.
# Schedule 48 frames per 1000us deadline on core 0 with priority 0
PIPELINE_PCM_ADD(sof/pipe-volume-playback.m4,
	9, 8, 2, s32le,
	1000, 0, 0,
	48000, 48000, 48000)
')

#
# DAIs configuration
#

dnl DAI_ADD(pipeline,
dnl     pipe id, dai type, dai_index, dai_be,
dnl     buffer, periods, format,
dnl     deadline, priority, core, time_domain)

# playback DAI is ALH(SDW0 PIN2) using 2 periods
# Buffers use s32le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-playback.m4,
	1, ALH, 2, SDW0-Playback,
	PIPELINE_SOURCE_1, 2, s32le,
	1000, 0, 0)

# capture DAI is ALH(SDW0 PIN2) using 2 periods
# Buffers use s32le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-capture.m4,
	2, ALH, 3, SDW0-Capture,
	PIPELINE_SINK_2, 2, s32le,
	1000, 0, 0)

# playback DAI is SSP2 using 2 periods
# Buffers use s24le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-playback.m4,
	3, SSP, 2, SSP2-Codec,
	PIPELINE_SOURCE_3, 2, s24le,
	1000, 0, 0)

# capture DAI is DMIC01 using 2 periods
# Buffers use s32le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-capture.m4,
	4, DMIC, 0, dmic01,
	PIPELINE_SINK_4, 2, s32le,
	1000, 0, 0)

# capture DAI is DMIC16k using 2 periods
# Buffers use s16le format, with 16 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-capture.m4,
	5, DMIC, 1, dmic16k,
	PIPELINE_SINK_5, 2, s16le,
	1000, 0, 0)

ifelse(HDMI, `1',
`
# playback DAI is iDisp1 using 2 periods
# Buffers use s32le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-playback.m4,
	6, HDA, 0, iDisp1,
	PIPELINE_SOURCE_6, 2, s32le,
	1000, 0, 0, SCHEDULE_TIME_DOMAIN_TIMER)

# playback DAI is iDisp2 using 2 periods
# Buffers use s32le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-playback.m4,
	7, HDA, 1, iDisp2,
	PIPELINE_SOURCE_7, 2, s32le,
	1000, 0, 0, SCHEDULE_TIME_DOMAIN_TIMER)

# playback DAI is iDisp3 using 2 periods
# Buffers use s32le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-playback.m4,
	8, HDA, 2, iDisp3,
	PIPELINE_SOURCE_8, 2, s32le,
	1000, 0, 0, SCHEDULE_TIME_DOMAIN_TIMER)

# playback DAI is iDisp4 using 2 periods
# Buffers use s32le format, with 48 frame per 1000us on core 0 with priority 0
DAI_ADD(sof/pipe-dai-playback.m4,
	9, HDA, 3, iDisp4,
	PIPELINE_SOURCE_9, 2, s32le,
	1000, 0, 0, SCHEDULE_TIME_DOMAIN_TIMER)
')

# PCM Low Latency, id 0
dnl PCM_PLAYBACK_ADD(name, pcm_id, playback)
PCM_PLAYBACK_ADD(SDW0-headphone, 0, PIPELINE_PCM_1)
PCM_CAPTURE_ADD(SDW0-mics, 1, PIPELINE_PCM_2)
PCM_PLAYBACK_ADD(Speaker, 2, PIPELINE_PCM_3)
PCM_CAPTURE_ADD(DMIC01, 3, PIPELINE_PCM_4)
PCM_CAPTURE_ADD(DMIC16k, 4, PIPELINE_PCM_5)

ifelse(HDMI, `1',
`
PCM_PLAYBACK_ADD(HDMI1, 5, PIPELINE_PCM_6)
PCM_PLAYBACK_ADD(HDMI2, 6, PIPELINE_PCM_7)
PCM_PLAYBACK_ADD(HDMI3, 7, PIPELINE_PCM_8)
PCM_PLAYBACK_ADD(HDMI4, 8, PIPELINE_PCM_9)
')

#
# BE configurations - overrides config in ACPI if present
#

#ALH dai index = ((link_id << 8) | PDI id)
#ALH SDW0 Pin2 (ID: 0)
DAI_CONFIG(ALH, 0x002, 0, SDW0-Playback)

#ALH SDW0 Pin3 (ID: 1)
DAI_CONFIG(ALH, 0x003, 1, SDW0-Capture)

#SSP 2 (ID: 2)
DAI_CONFIG(SSP, 2, 2, SSP2-Codec,
	SSP_CONFIG(I2S, SSP_CLOCK(mclk, 38400000, codec_mclk_in),
		      SSP_CLOCK(bclk, 2400000, codec_slave),
		      SSP_CLOCK(fsync, 48000, codec_slave),
		      SSP_TDM(2, 25, 3, 3),
		      SSP_CONFIG_DATA(SSP, 2, 24)))

# dmic01 (ID: 3)
DAI_CONFIG(DMIC, 0, 3, dmic01,
	   DMIC_CONFIG(1, 500000, 4800000, 40, 60, 48000,
		DMIC_WORD_LENGTH(s32le), 400, DMIC, 0,
		PDM_CONFIG(DMIC, 0, FOUR_CH_PDM0_PDM1)))

# dmic16k (ID: 4)
DAI_CONFIG(DMIC, 1, 4, dmic16k,
	   DMIC_CONFIG(1, 500000, 4800000, 40, 60, 16000,
		DMIC_WORD_LENGTH(s16le), 400, DMIC, 1,
		PDM_CONFIG(DMIC, 1, STEREO_PDM0)))

ifelse(HDMI, `1',
`
# 4 HDMI/DP outputs (ID: 5,6,7,8)
DAI_CONFIG(HDA, 0, 5, iDisp1)
DAI_CONFIG(HDA, 1, 6, iDisp2)
DAI_CONFIG(HDA, 2, 7, iDisp3)
DAI_CONFIG(HDA, 3, 8, iDisp4)
')

DEBUG_END
