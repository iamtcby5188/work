#!/bin/sh

echo "config.cfg"
source ./config.cfg
echo ${DEV_DIR}
echo '++++++++++++++++++++++++++start build.sh+++++++++++++++++++++++++++++++++++++++++++++'
unset SSH_ASKPASS
cd `dirname $0`
echo "pwd: "`pwd`
echo 'start 1_checkout.sh'
./1_checkout.sh $1
if [ $? != 0 ] ; then
  echo 'checkout.sh failed!'
  exit 1
fi 

LD_LIBRARY_PATH=$(pwd)/../../ssmq/lib/linux:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH

echo 'start make'
cd ${DEV_DIR}
pwd

ls | grep aclocal.m4
if [ $? != 0 ] ; then
  echo 'start aclocal'
  aclocal
  echo 'start autoconf'
  autoconf
  echo 'start automake'
  automake -a
  echo 'start configure'
  ./configure
fi

ls | grep linux
if [ $? != 0 ] ; then
  mkdir linux
fi

make
if [ $? != 0 ] ; then
  echo 'make failed!'
  exit 2 
fi
echo '++++++++++++++++++++++++++end build.sh+++++++++++++++++++++++++++++++++++++++++++++'
