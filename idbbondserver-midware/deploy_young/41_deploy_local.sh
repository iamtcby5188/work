#!/bin/sh
echo '++++++++++++++++++++++++++start deploy_local.sh+++++++++++++++++++++++++++++++++++++++++++++'
cd `dirname $0`
echo "pwd: "`pwd`
source ./config.cfg
#remove IDBBond 
expect -c "
set timeout 1000
spawn rm -rf /sumscope/data/IDBBond
expect \"]#\"
"
echo "remove /sumscope/data/IDBBond"
#copy  IDBbond
expect -c "
set timeout 1000
spawn cp -r ${DEP_DIR}/IDBBond /sumscope/data
expect \"]#\"
"
echo "copy IDBbond to /sumscope/data"

echo '++++++++++++++++++++++++++end deploy_local.sh+++++++++++++++++++++++++++++++++++++++++++++'

#tar jcvf IDBBond.tar.bz2 IDBBond
#echo generate IDBBond.tar.bz2

