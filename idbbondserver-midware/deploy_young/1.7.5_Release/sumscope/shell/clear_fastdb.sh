#!/bin/sh

CUR_USER=$(whoami)
for i in `ipcs -m | grep ${CUR_USER} | awk '{print $2}'`
do
ipcrm -m $i
done

for i in `ipcs -s | grep ${CUR_USER} | awk '{print $2}'`
do
ipcrm -s $i
done

rm -fv /tmp/*.bce
rm -fv /tmp/*.cs
rm -fv /tmp/*.dm
rm -fv /tmp/*.fdb
rm -fv /tmp/*.in
rm -fv /tmp/*.rs
rm -fv /tmp/*.us
rm -fv /tmp/*.ws

