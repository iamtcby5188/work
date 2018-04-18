#!/bin/sh
echo '******************************************AUTO-DEPLOY.SH STATR*******************************************'
cd `dirname $0`
echo "pwd: "`pwd`
echo '------call build.sh------'
./2_build.sh $1

echo '------call gen_package.sh------'
./3_gen_package.sh

#echo '------call deploy_local.sh------'
#./41_deploy_local.sh

sed -i "s/tcp:\/\/172.16.66.33/tcp:\/\/127.0.0.1/g" ./IDBBond/BasicDataServer/bin/BasicDataServerCfg.xml
sed -i "s/tcp:\/\/172.16.66.33/tcp:\/\/127.0.0.1/g" ./IDBBond/IDBBondServer/bin/IDBBondServerCfg.xml
sed -i "s/tcp:\/\/172.16.66.33/tcp:\/\/127.0.0.1/g" ./IDBBond/MarketDataServer/1/bin/MarketDataServerCfg.xml
sed -i "s/tcp:\/\/172.16.66.33/tcp:\/\/127.0.0.1/g" ./IDBBond/MarketDataServer/2/bin/MarketDataServerCfg.xml
sed -i "s/tcp:\/\/172.16.66.33/tcp:\/\/127.0.0.1/g" ./IDBBond/ProductFavorServer/bin/ProductFavorServerCfg.xml

expect -c "
   spawn ssh root@192.168.1.231
   expect *password* { send 123456\r }
   expect \"]#\"
   send \"cd /home/sumscope\r\"
   expect \"]#\"
   send \"./stop-all.sh\r\"
   expect \"*]#*\"
   send \"exit\r\"
   expect eof;
"

expect -c " 
spawn scp ./IDBBond/BasicDataServer/bin/BasicDataServer root@192.168.1.231:/home/sumscope/BasicDataServer/bin/
expect password { send 123456\r }
expect eof;
spawn scp ./IDBBond/IDBBondServer/bin/IDBBondServer root@192.168.1.231:/home/sumscope/IDBBondServer/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/MarketDataServer/1/bin/1_MarketDataServer root@192.168.1.231:/home/sumscope/MarketDataServer/1/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/MarketDataServer/2/bin/2_MarketDataServer root@192.168.1.231:/home/sumscope/MarketDataServer/2/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/ProductFavorServer/bin/ProductFavorServer root@192.168.1.231:/home/sumscope/ProductFavorServer/bin/
expect password: { send 123456\r }
expect eof;

spawn scp ./IDBBond/BasicDataServer/bin/BasicDataServerCfg.xml root@192.168.1.231:/home/sumscope/BasicDataServer/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/BasicDataServer/bin/backup_tables.sh root@192.168.1.231:/home/sumscope/BasicDataServer/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/BasicDataServer/bin/restore_tables.sh root@192.168.1.231:/home/sumscope/BasicDataServer/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/IDBBondServer/bin/IDBBondServerCfg.xml root@192.168.1.231:/home/sumscope/IDBBondServer/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/MarketDataServer/1/bin/MarketDataServerCfg.xml root@192.168.1.231:/home/sumscope/MarketDataServer/1/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/MarketDataServer/2/bin/MarketDataServerCfg.xml root@192.168.1.231:/home/sumscope/MarketDataServer/2/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/ProductFavorServer/bin/ProductFavorServerCfg.xml root@192.168.1.231:/home/sumscope/ProductFavorServer/bin/
expect password: { send 123456\r }
expect eof;

spawn scp ./IDBBond/BasicDataServer/bin/sqlquery.xml root@192.168.1.231:/home/sumscope/BasicDataServer/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/BasicDataServer/bin/sqlquery.xml root@192.168.1.231:/home/sumscope/IDBBondServer/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/BasicDataServer/bin/sqlquery.xml root@192.168.1.231:/home/sumscope/MarketDataServer/1/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/BasicDataServer/bin/sqlquery.xml root@192.168.1.231:/home/sumscope/MarketDataServer/2/bin/
expect password: { send 123456\r }
expect eof;
spawn scp ./IDBBond/BasicDataServer/bin/sqlquery.xml root@192.168.1.231:/home/sumscope/ProductFavorServer/bin/
expect password: { send 123456\r }
expect eof;
"

expect -c "
   set timeout 1200 
   spawn ssh root@192.168.1.231
   expect *password* { send 123456\r }
   expect \"]#\"
   send \"cd /home/sumscope\r\"
   expect \"]#\"
   send \"./stop-all.sh\r\"
   expect \"]#\"
   send \"cd /home/sumscope\r\"
   expect \"]#\"
   send \"./auto-start-all.sh\r\"
   expect \"*]#*\"
   send \"exit\r\"
   expect eof;
"

echo '******************************************AUTO-DEPLOY.SH END**********************************************'

