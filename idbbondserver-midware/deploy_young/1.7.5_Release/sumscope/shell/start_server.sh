#!/bin/sh

SHELL_PATH=/opt/sumscope/newbond/shell
source ${SHELL_PATH}/func.sh

start_server BasicDataServer
echo "|||||||||||||||||||||||||||||||||||||||||||||BASIC SERVER STARTED SUCCESSFULLY||||||||||||||||||||||||||||||||||||||||||||||||||||||||"

start_server IDBBondServer
echo "|||||||||||||||||||||||||||||||||||||||||||||BOND SERVER STARTED SUCCESSFULLY||||||||||||||||||||||||||||||||||||||||||||||||||||||||"

start_server MarketDataServer
echo "|||||||||||||||||||||||||||||||||||||||||||||MARKET SERVER STARTED SUCCESSFULLY||||||||||||||||||||||||||||||||||||||||||||||||||||||||"

echo -e "\n"

ps -ef | grep "sumscope"

