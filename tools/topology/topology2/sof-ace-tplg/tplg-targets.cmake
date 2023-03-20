# SPDX-License-Identifier: BSD-3-Clause

# Array of "input-file-name;output-file-name;comma separated pre-processor variables"
set(TPLGS

  "cavs-sdw\;sof-lnl-rt5682\;PLATFORM=mtl,NUM_DMICS=0,NUM_HDMIS=0"

  # CAVS SSP/DMIC topology for MTL/LNL


"cavs-nocodec-bt\;sof-tgl-nocodec\;PLATFORM=tgl,NUM_DMICS=0,PDM1_MIC_A_ENABLE=1,PDM1_MIC_B_ENABLE=1,\
PREPROCESS_PLUGINS=nhlt,NHLT_BIN=nhlt-sof-tgl-nocodec.bin"

"cavs-nocodec-bt\;sof-mtl-nocodec\;PLATFORM=mtl,NUM_DMICS=0,PDM1_MIC_A_ENABLE=1,PDM1_MIC_B_ENABLE=1,\
PREPROCESS_PLUGINS=nhlt,NHLT_BIN=nhlt-sof-mtl-nocodec.bin"

"cavs-nocodec-bt\;sof-lnl-nocodec\;PLATFORM=mtl,NUM_DMICS=0,PDM1_MIC_A_ENABLE=1,PDM1_MIC_B_ENABLE=1,\
PREPROCESS_PLUGINS=nhlt,NHLT_BIN=nhlt-sof-lnl-nocodec.bin"

)
