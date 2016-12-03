#!/bin/bash
COUNT=10

startapp()
{
    echo start app
    res=`adb shell am start -W  $1/$2|grep TotalTime | awk '{print int($2)}'`
    echo boot time  : ${res}
    thisTime=${res}
    if [ $thisTime -eq 0 ];then
        echo -e ----------may be screen off, keep screen turn on while test running!!!!-----------
        exit -1
    fi
    totalTime=`expr $thisTime + $totalTime`
}

killapp()
{
    pid=`adb shell ps|grep $1| awk '{print $2}'`
    adb shell kill -9 ${pid}
    echo kill app
}

testfunc()
{
    echo -----------$1 START------------------
    adb uninstall  $1
    adb install  $2
    totalTime=0
    i=0
    while [ $i -lt $COUNT ]; do
        startapp $3 $4
        killapp $3
        i=`expr $i + 1`
        sleep 10
    done

    echo name : $t1
    echo 测试包  :  $PACKAGENAME
    echo 测试次数:  $COUNT
    echo 平均值是:  `expr $totalTime / $COUNT`
    sleep 10
    echo ------------END---------------------
}

NAME="微信"
APKNAME="com.tencent.mm-1.apk"
PACKAGENAME=com.tencent.mm
ACTIVITYNAME=.ui.LauncherUI
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="支付宝钱包"
APKNAME="com.eg.android.AlipayGphone-1.apk"
PACKAGENAME=com.eg.android.AlipayGphone
ACTIVITYNAME=.AlipayLogin
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="携程无限"
APKNAME="ctrip.android.view-1.apk"
PACKAGENAME=ctrip.android.view
ACTIVITYNAME=.home.CtripSplashActivity
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="wps"
APKNAME="cn.wps.moffice_eng-1.apk"
PACKAGENAME=cn.wps.moffice_eng
ACTIVITYNAME=cn.wps.moffice.documentmanager.PreStartActivity
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="凤凰电视台"
APKNAME="com.ifeng.newvideo-1.apk"
PACKAGENAME=com.ifeng.newvideo
ACTIVITYNAME=.ui.ActivitySplash
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="KingSoft"
APKNAME="com.kingsoft-1.apk"
PACKAGENAME=com.kingsoft
ACTIVITYNAME=.StartActivity
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="QQ同步助手"
APKNAME="com.tencent.qqpim-1.apk"
PACKAGENAME=com.tencent.qqpim
ACTIVITYNAME=.ui.QQPimAndroid
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="大众点评"
APKNAME="com.dianping.v1-1.apk"
PACKAGENAME=com.dianping.v1
ACTIVITYNAME=com.dianping.main.guide.SplashScreenActivity
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="人民日报"
APKNAME="com.peopledailychina.activity-1.apk"
PACKAGENAME=com.peopledailychina.activity
ACTIVITYNAME=.ui.CoverPageActivity
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

# NAME="百度翻译"
# APKNAME="com.baidu.baidutranslate-1.apk"
# PACKAGENAME=com.baidu.baidutranslate
# ACTIVITYNAME=.activity.SplashActivity
# testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="切水果"
APKNAME="com.halfbrick.fruitninja-1.apk"
PACKAGENAME=com.halfbrick.fruitninja
ACTIVITYNAME=com.halfbrick.mortar.MortarGameActivity
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="愤怒的小鸟"
APKNAME="com.rovio.angrybirds-1.apk"
PACKAGENAME=com.rovio.angrybirds
ACTIVITYNAME=com.rovio.fusion.App
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="汇率转换"
APKNAME="com.mango.conversion-1.apk"
PACKAGENAME=com.mango.conversion
ACTIVITYNAME=.CurrencyConversion
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="疯狂赛车"
APKNAME="rangeracer_v1.0.7.apk"
PACKAGENAME=gm.rangeracer
ACTIVITYNAME=.RangeRacerActivity
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="生理测试"
APKNAME="sheliceshi.apk"
PACKAGENAME=com.qoocc.zn.sl
ACTIVITYNAME=com.qoocc.zn.view.StartActivity
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="小提琴"
APKNAME="com.zeugmangel.Violin62_1.apk"
PACKAGENAME=com.zeugmangel.Violin62
ACTIVITYNAME=com.zeugmangel.violinss.anasayfa
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="日语速记"
APKNAME="riyushuji.apk"
PACKAGENAME=cn.word
ACTIVITYNAME=.activity.CategoryActivity
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="占卜"
APKNAME="com.digitalpress.ouijatable_6.apk"
PACKAGENAME=com.digitalpress.ouijatable
ACTIVITYNAME=.OuijaTable
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="曲棍球"
APKNAME="teamcanadatablehockey_1.apk"
PACKAGENAME=com.StingerGames.TeamCanadaTableHockey
ACTIVITYNAME=.TeamCanadaTableHockeyActivity
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="疯狂的钢琴"
APKNAME="fengkuangdegangqin.apk"
PACKAGENAME=com.zhangda.gangqin.er.view
ACTIVITYNAME=.MainView
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="计算器"
APKNAME="jisuanqi_540.apk"
PACKAGENAME=com.ibox.calculators
ACTIVITYNAME=.CalculatorActivity
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME

NAME="数位时钟"
APKNAME="digitalclockcn.xiaoxiaotu.com_1.0CN_liqucn.com.apk"
PACKAGENAME=digitalclockcn.xiaoxiaotu.com
ACTIVITYNAME=.MainScreen
testfunc $NAME $APKNAME $PACKAGENAME $ACTIVITYNAME
