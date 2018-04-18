#!/bin/sh
echo '******************************************AUTO-DEPLOY-REMOTE.SH STATR*******************************************'
cd `dirname $0`
echo "pwd: "`pwd`
echo '------call build.sh------'
./build.sh
echo '------call gen_package.sh------'
./gen_package.sh

echo "------generate IDBBond.tar.bz2------"
#tar jcvf IDBBond.tar.bz2 IDBBond

echo '------call deploy_remote.sh------'
./deploy_remote.sh
echo '******************************************AUTO-DEPLOY-REMOTE.SH END**********************************************'

