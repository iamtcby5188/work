#!/bin/sh
echo '++++++++++++++++++++++++++start gen_package.sh+++++++++++++++++++++++++++++++++++++++++++++'

cd `dirname $0`
echo "pwd: "`pwd`
source ./config.cfg

function cpProgram()
{
  PGM=$1
  PGM_DES=$1
  DEV=$2
  DEP=$3
  NUM=$4
  PGM_SUB_DIR=bin

  if [ -n "$NUM" ]; then 
    PGM_SUB_DIR="${NUM}/${PGM_SUB_DIR}";
    PGM_DES="${NUM}_${PGM}";
  fi

  test -d ${DEP}/IDBBond/${PGM}/${PGM_SUB_DIR} || mkdir -p ${DEP}/IDBBond/${PGM}/${PGM_SUB_DIR}

  \cp -f ${DEV}/linux/${PGM}                               ${DEP}/IDBBond/${PGM}/${PGM_SUB_DIR}/${PGM_DES}
  \cp -f ${DEV}/${PGM}/Resource/*.xml                      ${DEP}/IDBBond/${PGM}/${PGM_SUB_DIR}
  \cp -f ${DEV}/IDBCommon/log4cxx/log4cxx.properties       ${DEP}/IDBBond/${PGM}/${PGM_SUB_DIR}

  
  #replace database
  echo "replace database address in directory ${DEP}/IDBBond/${PGM}/${PGM_SUB_DIR}/"
  sed -i "s/tcp:\/\/192.168.1.102/tcp:\/\/172.16.66.33/g" `grep tcp:\/\/192.168.1.102 -rl ${DEP}/IDBBond/${PGM}/${PGM_SUB_DIR}/*.xml`

  #replace company info: id/name
  echo "replace company id/name in directory ${DEP}/IDBBond/${PGM}/${PGM_SUB_DIR}"
  sed -i "s~<ID>1</ID>~<ID>20</ID>~g"                      `grep "<ID>1</ID>"          -rl ${DEP}/IDBBond/${PGM}/${PGM_SUB_DIR}/*Cfg.xml`
  sed -i "s~<Name>TPSC</Name>~<Name>OTHER</Name>~g"        `grep "<Name>TPSC</Name>"   -rl ${DEP}/IDBBond/${PGM}/${PGM_SUB_DIR}/*Cfg.xml`
  echo "copy ${PGM} to ${PGM_DES} finished!"
}

cpProgram "IDBBondServer"      ${DEV_DIR} ${DEP_DIR};
cpProgram "BasicDataServer"    ${DEV_DIR} ${DEP_DIR};
cpProgram "MarketDataServer"   ${DEV_DIR} ${DEP_DIR} 1;
cpProgram "MarketDataServer"   ${DEV_DIR} ${DEP_DIR} 2;
cpProgram "ProductFavorServer" ${DEV_DIR} ${DEP_DIR};
cpProgram "QQ2IDBServer"       ${DEV_DIR} ${DEP_DIR};

mkdir -p ${DEP}/IDBBondServer/bin/excel
cp -fv ${DEV}/IDBCommon/util/excel/excel.config ${DEP}/IDBBondServer/bin/excel/

#replace cache name
echo "start to replace cache name"
sed -i "s~<Main>Queue.Bond.MarketData.1~<Main>Queue.Bond.MarketData.2~g" `grep "<Main>Queue.Bond.MarketData.1" -rl ${DEP_DIR}/IDBBond/MarketDataServer/2/bin/*Cfg.xml`
sed -i "s~<CacheName>MarketDataCache.1~<CacheName>MarketDataCache.2~g" `grep "<CacheName>MarketDataCache.1" -rl ${DEP_DIR}/IDBBond/MarketDataServer/2/bin/*Cfg.xml`

echo '++++++++++++++++++++++++++end gen_package.sh+++++++++++++++++++++++++++++++++++++++++++++'
