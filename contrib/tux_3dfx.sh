#!/bin/sh

declare -x MESA_GLX_FX="fullscreen"
declare -x FX_GLIDE_SWAPINTERVAL=1

declare -x SST_GAMMA=1.4
declare -x SST_SCREENREFRESH=60
declare -x SST_SWA_EN_WAIT_IN_VSYNC=1

declare -x SSTV2_GAMMA=1.4
declare -x SSTV2_SCREENREFRESH=60
declare -x SSTV2_SWA_EN_WAIT_IN_VSYNC=1

src/tux_aqfh $1 $2 $3

