#!/bin/sh
echo '++++++++++++++++++++++++++start deploy_remote.sh+++++++++++++++++++++++++++++++++++++++++++++'
cd `dirname $0`
echo "pwd: "`pwd`

echo "deploy 40"
#./deploy_remote_40.sh

echo "deploy 55"
./deploy_remote_55.sh
 '++++++++++++++++++++++++++end deploy_remoe.sh+++++++++++++++++++++++++++++++++++++++++++++'
