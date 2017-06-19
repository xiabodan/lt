#!/bin/bash

PROJECT_ROOT=$(dirname $_)

ADB=$PROJECT_ROOT/out/host/linux-x86/bin/adb
GDB=$PROJECT_ROOT/prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin/arm-eabi-gdb

TARGET="app_process_gaia"
GDB_PORT=5039
GDBINIT=/tmp/gdbinit.`whoami`

GDBSERVER_PID=`$ADB shell toolbox ps |
               grep "gdbserver" | awk '{ print \$2; }'`

for p in $GDBSERVER_PID ; do
   $ADB shell cat /proc/$p/cmdline | grep -q :$GDB_PORT && ( \
      echo ..killing gdbserver pid $p
      $ADB shell kill -9 $p
   ) || echo ..ignoring gdbserver pid $p

done

$ADB forward tcp:$GDB_PORT tcp:$GDB_PORT

#TARGET_PID=`$ADB shell toolbox ps | grep "$TARGET" | awk '{ print \$2; }' | tail -n1`
TARGET_PID=$1

if [ -z $TARGET_PID ]; then
    echo starting $TARGET
    $ADB shell gdbserver :$GDB_PORT /system/bin/$TARGET &
else
    echo attaching $TARGET_PID
    $ADB shell gdbserver :$GDB_PORT --attach $TARGET_PID &
fi

sleep 1

echo "set solib-absolute-prefix $PROJECT_ROOT/out/target/product/qilianp2/symbols/" > $GDBINIT
echo "set solib-search-path $PROJECT_ROOT/out/target/product/qilianp2/symbols/system/lib/" >> $GDBINIT
echo "target extended-remote :$GDB_PORT" >> $GDBINIT

echo $GDB -x $GDBINIT out/target/product/qilianp2/symbols/system/bin/$TARGET   
$GDB -x $GDBINIT out/target/product/qilianp2/symbols/system/bin/$TARGET   
