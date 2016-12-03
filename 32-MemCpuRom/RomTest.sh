#!/system/bin/sh

ROM_LOG_FILE="/data/local/romlog.txt"
rom_enable=1

if [ -f ${ROM_LOG_FILE} ]; then
    chmod 0666 ${ROM_LOG_FILE}
else
    touch ${ROM_LOG_FILE}
    chmod 0644 ${ROM_LOG_FILE}
fi

function capture_rom()
{
    echo
    echo "`date`"
    echo "`df`"
}

while [ $rom_enable -eq 1 ]
do
    echo "Test ROM Consumption Test"
    capture_rom >> /data/local/romlog.txt
    sleep 1
done
