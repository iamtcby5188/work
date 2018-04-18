#!/bin/sh
echo '******************************************AUTO-DEPLOY.SH STATR*******************************************'

DEPLOY_IP1=172.16.96.73
DEPLOY_IP2=172.16.96.74

function stop_server()
{
expect -c "
   spawn ssh root@$1
   expect (yes/no)? { send yes\r }
   expect *password* { send 123456\r }
   expect \"]#\"
   send \"cd /opt/sumscope/1.7.5_Release/sumscope/shell\r\"
   expect \"]#\"
   send \"./stop_server.sh\r\"
   expect \"*]#*\"
   send \"cd /opt/sumscope\r\"
   expect \"]#\"
   send \"rm -rf 1.7.5_*\r\"
   expect \"]#\"
   send \"exit\r\"
   expect eof;
"
}

stop_server $DEPLOY_IP1
stop_server $DEPLOY_IP2

function scp_file()
{
expect -c " 
set timeout 60
spawn scp $1 root@$2:/opt/sumscope/
expect (yes/no)? { send yes\r }
expect password { send 123456\r }
expect eof;
"
}

scp_file $1 $DEPLOY_IP1
scp_file $1 $DEPLOY_IP2


function extract_file()
{
expect -c "
   set timeout 60
   spawn ssh root@$2
   expect *password* { send 123456\r }
   expect \"]#\"
   send \"cd /opt/sumscope\r\"
   expect \"]#\"
   send \"tar -zxvf $1\r\"
   expect \"]#\"
   send \"exit\r\"
   expect eof;
"
}

extract_file $1 $DEPLOY_IP1
extract_file $1 $DEPLOY_IP2

echo '******************************************AUTO-DEPLOY.SH END**********************************************'

