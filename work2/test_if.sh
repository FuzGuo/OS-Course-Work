#!/bin/bash
#Program:测试if语句,判断输入是否是正数
#History:2013/10/22 Version1.0
read -p "请输入一个数:" temp
#if test $temp -gt 0 #测试tset
if [ $temp -gt 0 ] #测试[]
then
  echo "您输入是正数"
else
  echo "您输入是非正数"
fi

