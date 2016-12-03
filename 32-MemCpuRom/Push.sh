#!/bin/bash

PushBin()
{
    adb push $1 /system/bin
    adb shell chmod 777 /system/bin/$1
}

adb remount
PushBin AutoTest.sh
PushBin RomTest.sh
PushBin CpuTest.sh
PushBin MemoryTest
PushBin TaskTest
