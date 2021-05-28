#!/bin/bash -x
#
# USAGE:
#     bash -ex /usr/lib/selenium/headlessSelenium.sh [WORKSPACE_ROOT]
# whereas WORKSPACE_ROOT points to workspace root in the jenkins workspace
# which contains the whole project
#
if [ -z "$WORKSPACE_ROOT" ]; then
    WORKSPACE_ROOT=$1
fi

FIREFOX_BIN="/usr/lib/firefox-esr/firefox-bin"

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

cd $WORKSPACE_ROOT

if [ -n $2 ]; then
  CONF_FILE_OPTION="-Ddataportal.test.conf=$2"
fi

echo "cleaning up old screenshots"
rm -fr screenshots
mkdir screenshots

mvn -Dwebdriver.firefox.bin=${FIREFOX_BIN} -DargLine="${CONF_FILE_OPTION}" clean test
