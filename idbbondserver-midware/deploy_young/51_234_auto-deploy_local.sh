#!/bin/sh

echo '******************************************AUTO-DEPLOY.SH STATR*******************************************'
cd `dirname $0`
echo "pwd: "`pwd`

CUR_PATH=$(pwd)
BOND_BASE=${CUR_PATH}/1.7.5_Release/sumscope

echo '------call build.sh------'
./2_build.sh $1

echo '------call gen_package.sh------'
#./3_gen_package.sh

cp -fv ../linux/BasicDataServer ${BOND_BASE}/BasicDataServer/bin/BasicDataServer
cp -fv ../linux/IDBBondServer ${BOND_BASE}/IDBBondServer/bin/IDBBondServer
cp -fv ../linux/MarketDataServer ${BOND_BASE}/MarketDataServer/bin/MarketDataServer

cp -fv ../BasicDataServer/Resource/log4cxx.properties ${BOND_BASE}/BasicDataServer/bin/
cp -fv ../IDBBondServer/Resource/log4cxx.properties ${BOND_BASE}/IDBBondServer/bin/
cp -fv ../MarketDataServer/Resource/log4cxx.properties ${BOND_BASE}/MarketDataServer/bin/

cp -fv ../BasicDataServer/Resource/sqlquery.xml ${BOND_BASE}/BasicDataServer/bin/
cp -fv ../IDBBondServer/Resource/sqlquery.xml ${BOND_BASE}/IDBBondServer/bin/
cp -fv ../MarketDataServer/Resource/sqlquery.xml ${BOND_BASE}/MarketDataServer/bin/

COMPRESS_FILE=1.7.5_Release_`date "+%Y%m%d"`.tar.gz
tar -zcvf ${COMPRESS_FILE} 1.7.5_Release/

./deploy_1.7.5_remote.sh ${COMPRESS_FILE}

echo '******************************************AUTO-DEPLOY.SH END**********************************************'

