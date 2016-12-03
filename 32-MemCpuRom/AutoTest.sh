##!/system/bin/sh
#!/bin/bash

BASE=""
function checkHost()
{
    tem=`busybox uname -a`
    arr=($tem)
    if [[ "armv7l" != "${arr[12]}" ]]
    then
        return 0
    else
        return 1
    fi
}
checkHost
num=$?  # check return parameter
if [ $num -eq 0 ]; then
    echo "host is x86"
    BASE=/home/hubert/src/leadcore_os_cert/os_test/32-MemCpuRom
else
    echo "host is arm"
    BASE=/data/local
fi

MEM_LOG_FILE="$BASE/memlog.txt"
ROM_LOG_FILE="$BASE/romlog.txt"
CPU_LOG_FILE="$BASE/cpulog.txt"
ROM_FILE="$BASE/romlog"
ROM_FILE_NUM=12
mem_enable=0
rom_enable=0
cpu_enable=0

print_usage() {
    echo ""
    echo "|Test Usage:-----------------------------------------------------------"
    echo "|Memory consume test please input follow command                       "
    echo "|    $ AutoTest.sh mem                                                 "
    echo "|Rom consume test please input follow command                          "
    echo "|    $ RomTest.sh                                                      "
    echo "|    $ AutoTest.sh rom                                                 "
    echo "|Cpu usage consume test please input follow command                    "
    echo "|    $ CpuTest.sh                                                      "
    echo "|    $ AutoTest.sh cpu                                                 "
    echo "|----------------------------------------------------------------------"
    echo ""
}


function get_mem()
{
    tmp=`cat /proc/meminfo`
}

function get_total_mem()
{
    tmp=`cat /proc/meminfo`
    arr=($tmp)
    echo ${arr[1]}
}

function get_free_mem()
{
    tmp=`cat /proc/meminfo`
    arr=($tmp)
    echo ${arr[4]}
}

function capture_mem()
{
    #echo "`date` memdog end (total:${total_mem} free:${free_mem})"
    #avg=`echo "scale=4;$free_mem/$total_mem" | bc`
    echo
    echo "`date`"
    echo "`cat /proc/meminfo`"
}

function capture_rom()
{
    echo
    echo "`date`"
    echo "`df`"
}

function capture_cpu()
{
    echo
    echo "`date`"
    echo "`top`"
}

if [ $# -ne 1 ]; then
    print_usage
    exit 0
fi

mem=$(get_mem)
total_mem=$(get_total_mem)
free_mem=$(get_free_mem)
last_free_mem=$free_mem

if [ -f ${MEM_LOG_FILE} ]; then
    chmod 0644 ${MEM_LOG_FILE}
else
    touch ${MEM_LOG_FILE}
    chmod 0644 ${MEM_LOG_FILE}
fi

if [ -f ${ROM_LOG_FILE} ]; then
    chmod 0666 ${ROM_LOG_FILE}
else
    touch ${ROM_LOG_FILE}
    chmod 0644 ${ROM_LOG_FILE}
fi

if [ -f ${CPU_LOG_FILE} ]; then
    chmod 0644 ${CPU_LOG_FILE}
else
    touch ${CPU_LOG_FILE}
    chmod 0644 ${CPU_LOG_FILE}
fi

#for((i=0; i<${ROM_FILE_NUM}; i++))
for i in 0 1 2 3 4 5 6 7 8 9 10
do
    if [ -f ${ROM_FILE}${i} ]; then
        chmod 0666 ${ROM_FILE}${i}
    else
        touch ${ROM_FILE}${i}
        chmod 0644 ${ROM_FILE}${i}
    fi
done

function writeFile()
{
    #for (( i=0; i<$1; i++))
    for i in 0 1 2 3 4 5
    do
        dd if=/dev/zero of=${ROM_FILE}${i} bs=$((1024*1024))  count=1000 &
    done
}

case $1 in
    "mem")
        echo "Test Memory Consumption Test"
        mem_enable=1
        /system/bin/MemoryTest 2000000000 &
        ;;
    "rom")
        echo "Test ROM Consumption Test"
        rom_enable=1
        writeFile 5
        ;;
    "cpu")
        echo "Test CPU usage Consumption Test"
        cpu_enable=1
        /system/bin/TaskTest &
        ;;
    *)
        echo "Args Input Error"
        print_usage
        exit 0
        ;;
esac

while [ $mem_enable -eq 1 ]
do
    #avg=`echo "scale=4;$free_mem/$total_mem" | bc`
    #echo "`date` Memory usage :0"$avg
    capture_mem >> ${MEM_LOG_FILE}
    sleep 1
done

while [ $rom_enable -eq 1 ]
do
    echo "Test ROM Consumption Test"
    capture_rom >> ${ROM_LOG_FILE}
    sleep 1
done

while [ $cpu_enable -eq 1 ]
do
    echo "Test CPU usage Consumption Test"
    capture_cpu >> ${CPU_LOG_FILE}
    sleep 1
done

exit 0
