#!/bin/bash
#
#
#
DISPLAY=":99"
# 
# This option creates screen screennum and sets its width, height, and depth to W, H, and  D  respectively.
# By default, only screen 0 exists and has the dimensions 1280x1024x8.
#
SCREEN="0 1280x1024x16"

#Use virtual X server
VIRTUAL_X="Xvfb $DISPLAY"

#init
if [ -z "$(pidof Xvfb)" ]; then
        $VIRTUAL_X &
fi
export DISPLAY