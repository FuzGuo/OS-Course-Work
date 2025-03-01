#!/bin/bash
#Program:测试for语句 输出1到10
#History:2013/10/22 Version1.0
for i in $(seq 10)
#for i in $* #运行的时候带参数
do
  echo $i
done

