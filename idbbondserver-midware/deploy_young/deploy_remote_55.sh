#!/bin/sh
echo '++++++++++++++++++++++++++start deploy_remote_55.sh+++++++++++++++++++++++++++++++++++++++++++++'
cd `dirname $0`
echo "pwd: "`pwd`

#replace database
echo "start to replace datebase"
sed -i "s/tcp:\/\/192.168.1.50/tcp:\/\/192.168.1.59/g" `grep tcp:\/\/192.168.1.50 -rl ./IDBBond/*/bin/*.xml`
sed -i "s/tcp:\/\/192.168.1.50/tcp:\/\/192.168.1.59/g" `grep tcp:\/\/192.168.1.50 -rl ./IDBBond/MarketDataServer/*/bin/*.xml`

#replace company info: id/name
echo "start to replace company info: id/name"
sed -i "s~<ID>402880f034219aed0134219e083906e7</ID>~<ID>20</ID>~g" `grep "<ID>402880f034219aed0134219e083906e7</ID>" -rl ./IDBBond/*/bin/*Cfg.xml`
sed -i "s~<ID>402880f034219aed0134219e083906e7</ID>~<ID>20</ID>~g" `grep "<ID>402880f034219aed0134219e083906e7</ID>" -rl ./IDBBond/MarketDataServer/*/bin/*Cfg.xml`
sed -i "s~<Name>OTHER</Name>~<Name>国海证券深圳</Name>~g" `grep "<Name>OTHER</Name>" -rl ./IDBBond/*/bin/*Cfg.xml`
sed -i "s~<Name>OTHER</Name>~<Name>国海证券深圳</Name>~g" `grep "<Name>OTHER</Name>" -rl ./IDBBond/MarketDataServer/*/bin/*Cfg.xml`


expect -c " 
   set timeout 100
   spawn scp -r IDBBond root@192.168.1.55:/sumscope
   expect yes/no { send yes\r ; exp_continue }
   expect password: { send centos\r }
   expect 100%
   sleep 1
" 
echo -e '\ncopy bz2 to 192.168.1.55'

expect -c "
   set timeout 1200 
   spawn ssh root@192.168.1.55
   expect *password* { send centos\r }
   expect \"]#\"
   send \"cd /sumscope/IDBBond\r\"
   send \"./stop-all.sh\r\"
   expect \"]#\"
   send \"./auto-start-all.sh\r\"
   expect \"*]#*\"
   send \"exit\r\"
   expect eof;"

echo '++++++++++++++++++++++++++end deploy_remote_55.sh+++++++++++++++++++++++++++++++++++++++++++++'
