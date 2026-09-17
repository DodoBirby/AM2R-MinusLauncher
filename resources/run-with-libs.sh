#!/usr/bin/sh

# This script will open AM2R while loading the libraries shipped with the AM2R Linux Autopatcher
# Both of those can be found here: https://github.com/AM2R-Community-Developers/AM2R-Autopatcher-Linux/tree/master/data
export radeonsi_sync_compile="true"
export LD_LIBRARY_PATH="../../libs"
./runner
