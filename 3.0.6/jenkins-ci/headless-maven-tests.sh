#!/bin/bash
#
# USAGE:
#     bash -ex /usr/lib/selenium/headlessSelenium.sh $WORKSPACE 
#
#
WORKSPACE=$1
FIREFOX_BIN="/usr/lib/iceweasel/firefox-bin"

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
else
        echo "Xvfb already running with pid $(pidof Xvfb)"
fi
export DISPLAY

cd $WORKSPACE/test/java/dataportal-selenium-tests/
mvn -Dwebdriver.firefox.bin=$FIREFOX_BIN test
