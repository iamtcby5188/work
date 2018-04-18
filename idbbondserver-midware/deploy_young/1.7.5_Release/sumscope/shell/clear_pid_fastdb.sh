#!/bin/sh

SHELL_PATH=/opt/sumscope/newbond/shell
source ${SHELL_PATH}/func.sh

SERVER_NAME=$1
clear_pid_fastdb ${SERVER_NAME}

