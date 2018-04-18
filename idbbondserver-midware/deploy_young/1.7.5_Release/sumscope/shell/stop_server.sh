#!/bin/sh

SHELL_PATH=/opt/sumscope/newbond/shell
source ${SHELL_PATH}/func.sh

stop_server BasicDataServer
stop_server IDBBondServer IDBBondServer
stop_server MarketDataServer

${SHELL_PATH}/clear_fastdb.sh

