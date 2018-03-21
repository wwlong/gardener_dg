#cnt=0
#while true
#do
#    sleep 1
#    echo "Hello World !"
#    shotwell -video_size 1920*1080 $cnt.jpg & 
#    cnt=`expr $var + 1`
#    sleep 2
#    killall shotwell
#done

##!/bin/sh
#num1=0
#num2=1
#while true
#do
#if [ -f "$num2.jpg" ]
#then
#echo "exec $num1.jpg "
#./test $num1.jpg 1920 1080 100
#mv 0.jpg /tmp/$num1.jpg
#rm $num1.jpg -rf
#
#sn=`ps -ef | grep shotwell | grep -v grep | awk '{print $2}'`
#if [ "${sn}" = "" ]
#then
#shotwell  /tmp/$num1.jpg &
#else
#kill $sn 
#shotwell  /tmp/$num1.jpg &
#fi
#
#num1=`expr $num1 + 1`
#num1=`expr $num1 % 10`
#num2=`expr $num2 + 1`
#num2=`expr $num2 % 10`
#fi
#echo "sleep for $num2.jpg data"
#sleep 0.4
#done
#
#

#!/bin/sh
jpg_prefix=""
num1=0
num2=1
while true
do
if [ -f "$jpg_prefix$num1.jpg" ]
then
echo "exec $jpg_prefix$num1.jpg "
#./test $num1.jpg 1920 1080 100
# mv $jpg_prefix$num1.jpg /tmp/$jpg_prefix$num1.jpg -f
# rm $jpg_prefix$num1.jpg -rf

sn=`ps -ef | grep shotwell | grep -v grep |awk '{print $2}'`
if [ "${sn}" = "" ]
then
    shotwell $jpg_prefix$num1.jpg &
else
    sudo kill -9 $sn 
    shotwell $jpg_prefix$num1.jpg &
fi

num1=`expr $num1 + 1`
#num1=`expr $num1 % 10`
#num2=`expr $num2 + 1`
#num2=`expr $num2 % 10`
fi
echo "sleep for $jpg_prefix$num1.jpg data"
sleep 1
done

