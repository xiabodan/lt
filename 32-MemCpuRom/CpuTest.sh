#!/system/bin/sh

CPU_LOG_FILE="/data/local/cpulog.txt"
cpu_enable=1


if [ -f ${CPU_LOG_FILE} ]; then
    chmod 0644 ${CPU_LOG_FILE}
else
    touch ${CPU_LOG_FILE}
    chmod 0644 ${CPU_LOG_FILE}
fi

function capture_cpu()
{
    echo
    echo "`date`"
    echo "`top`"
}

while [ $cpu_enable -eq 1 ]
do
    echo "Test CPU usage Consumption Test"
    capture_cpu >> ${CPU_LOG_FILE}
    sleep 1
done
