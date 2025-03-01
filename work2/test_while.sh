#!/bin/bash
#Program:测试while语句 智能欢迎程序
#History:2013/10/22 Version1.0
echo "欢迎使用智能欢迎程序,输入q退出！"
read -p "请输入您的姓名：" name
while [ "$name" != "" ] && [ "$name" != "q" ]
#while [ "$name" != "" ]
do
#	if [ "$name" = "q" ]
#		then break
#	fi
  echo "欢迎您，$name"
  read -p "请输入您的姓名：" name
done
