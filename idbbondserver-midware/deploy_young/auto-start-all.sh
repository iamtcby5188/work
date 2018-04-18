#!/bin/sh

ulimit -c unlimited

CUR_PATH=$(pwd)

./stop-all.sh

cd ${CUR_PATH}
./clear_fastdb.sh
echo "clear fastdb"

cd ${CUR_PATH}/BasicDataServer
./start.sh
expect -c "
set timeout 1000
spawn tail -1f ./bin/nohup.out
expect \"server start success\" {send \003}
expect eof;
"
echo "|||||||||||||||||||||||||||||||||||||||||||||BASIC SERVER STARTED SUCCESSFULLY||||||||||||||||||||||||||||||||||||||||||||||||||||||||"

cd ${CUR_PATH}/IDBBondServer
./start.sh
expect -c "
set timeout 600
spawn tail -1f ./bin/nohup.out
expect \"server start success\" {send \003}
expect eof;
"
echo "|||||||||||||||||||||||||||||||||||||||||||||BOND SERVER STARTED SUCCESSFULLY||||||||||||||||||||||||||||||||||||||||||||||||||||||||"

cd ${CUR_PATH}/MarketDataServer/1
./start.sh
expect -c "
set timeout 600
spawn tail -1f ./bin/nohup.out
expect \"server start success\" {send \003}
expect eof;
"
echo "|||||||||||||||||||||||||||||||||||||||||||||MARKET SERVER 1 STARTED SUCCESSFULLY||||||||||||||||||||||||||||||||||||||||||||||||||||||||"

cd ${CUR_PATH}/MarketDataServer/2
./start.sh
expect -c "
set timeout 600
spawn tail -1f ./bin/nohup.out
expect \"server start success\" {send \003}
expect eof;
"
echo "|||||||||||||||||||||||||||||||||||||||||||||MARKET SERVER 2 STARTED SUCCESSFULLY||||||||||||||||||||||||||||||||||||||||||||||||||||||||"

cd ${CUR_PATH}/ProductFavorServer
./start.sh

cd ${CUR_PATH}
./start-other.sh
echo -e "\n"

ps -ef | grep "sumscope"

