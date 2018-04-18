#!/bin/sh
echo '++++++++++++++++++++++++++start deploy_remote_40.sh+++++++++++++++++++++++++++++++++++++++++++++'
cd `dirname $0`
echo "pwd: "`pwd`

#echo "generate IDBBond.tar.bz2"
#tar jcvf IDBBond.tar.bz2 IDBBond

expect -c " 
   set timeout 100
   spawn scp IDBBond.tar.bz2 root@172.16.8.40:/sumscope
   expect yes/no { send yes\r ; exp_continue }
   expect password: { send Sumscope@2015\r }
   expect 100%
   sleep 1
" 
echo -e '\ncopy bz2 to 172.16.8.40'

expect -c "
   set timeout 1200 
   spawn ssh root@172.16.8.40
   expect *password* { send Sumscope@2015\r }
   expect \"]#\"
   send \"cd /sumscope\r\"
   send \"tar jxvf IDBBond.tar.bz2\r\"
   expect \"]#\"
   send \"cd ./IDBBond\r\"
   send \"./stop-all.sh\r\"
   expect \"]#\"
   send \"./auto-start-all.sh\r\"
   expect \"*]#*\"
   send \"exit\r\"
   expect eof;"

echo '++++++++++++++++++++++++++end deploy_remote_40.sh+++++++++++++++++++++++++++++++++++++++++++++'
