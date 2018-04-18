#!/bin/sh

ulimit -c unlimited

cd ${SHELL_PATH}/..
CUR_PATH=$(pwd)

function start_server()
{
    RETURN_VAL=0
    SERVER_NAME=$1
    SERVER_PATH=${CUR_PATH}/${SERVER_NAME}
#    export LD_LIBRARY_PATH=${SERVER_PATH}/lib:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=${SERVER_PATH}/lib:/usr/local/lib64:/usr/local/lib:/usr/lib64:/usr/lib:/lib64:/lib:$LD_LIBRARY_PATH	 
    program=${SERVER_PATH}/bin/${SERVER_NAME}
    process=`ps -ef | grep $program | grep -v grep`;
    if [ "$process" == "" ]; then
        echo $program start.
        cd ${SERVER_PATH}/bin
        nohup $program  > nohup.out &
        sleep 1s
	expect -c "
	    set timeout 890
	    spawn tail -1f nohup.out
	    expect -re \"server start success|currentNodeStatus:STANDBY\" {send \003}
            expect timeout {exit 1}
	" > /dev/null 2>&1
        if [ $? == 0 ]; then
            RETURN_VAL=0
        else
            RETURN_VAL=1
        fi
        ps -ef | grep $program | grep -v grep | awk '{print $2}' > ${SERVER_PATH}/PID
    else
        echo $program already run.
        RETURN_VAL=0
    fi

    return ${RETURN_VAL}
}

function stop_server()
{
    SERVER_NAME=$1
    SERVER_PATH=${CUR_PATH}/${SERVER_NAME}
    program=${SERVER_PATH}/bin/${SERVER_NAME}
    process=`ps -ef | grep $program | grep -v grep | awk '{print $2}'`;
    if [ "$process" != "" ]; then
        kill -9 $process
        echo $program stop.
        return 0
    else 
        return 1
    fi
}

function status_server()
{
    SERVER_NAME=$1
    SERVER_PATH=${CUR_PATH}/${SERVER_NAME}
    program=${SERVER_PATH}/bin/${SERVER_NAME}
    FASTDB_FLAG=${SERVER_PATH}/bin/log/FastDB.log
    test -f ${FASTDB_FLAG} && FASTDB_SIZE=`awk -F ' ' '{print $1;exit}' $FASTDB_FLAG` || FASTDB_SIZE=-1
    process=`ps -ef | grep $program | grep -v grep | awk '{print $2}'`;
    if [ "$process" != "" ]; then
	CURRHOUR=$(date +%H)
	if [ ${CURRHOUR} -gt 04 ] && [ "$FASTDB_SIZE" == "-1" ]; then
		echo $program is failure. Fastdb is closed
		return 1
	fi
	RETVAL=0
        echo $program is running.
        return 0
    else
        echo $program is stopped.
        return 1
    fi
}

function clear_pid_fastdb()
{
    SERVER_NAME=$1
    SERVER_PATH=${CUR_PATH}/${SERVER_NAME}
    SERVER_PID=$(cat ${SERVER_PATH}/PID)
    CUR_USER=$(whoami)

    for i in `ipcs -p | grep ${CUR_USER} | grep ${SERVER_PID} | awk '{print $1}'`
    do
        ipcrm -m $i
    done
}

function clear_log()
{
    SERVER_NAME=$1
    SERVER_PATH=${CUR_PATH}/${SERVER_NAME}
    rm -rfv ${SERVER_PATH}/bin/*.log
    rm -rfv ${SERVER_PATH}/bin/nohup.out
    rm -rfv ${SERVER_PATH}/bin/log/*.log
    rm -rfv ${SERVER_PATH}/bin/core.*
    rm -rfv ${SERVER_PATH}/PID
}


