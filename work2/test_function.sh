#!/bin/bash
#Program:测试Shell自定义函数
#History:2013/10/22 Version1.0
#foo 3 2
function foo(){
  #echo $1,$2
  return $(($1+$2))
}
foo 4 5
total=$(foo 3 2)
foo 4 5
total2=$?
echo $total2
#1、必须在调用函数地方之前，声明函数，shell脚本是逐行运行。不会像其它语言一样先预编译。一次必须在使用函数前先声明函数。

#2、total=$(foo 3 2);  通过这种调用方法，我们清楚知道，在shell 中 单括号里面，可以是：命令语句。 因此，我们可以将shell中函数，看作是定义一个新的命令，它是命令，因此 各个输入参数直接用 空格分隔。 一次，命令里面获得参数方法可以通过：$0…$n得到。 $0代表函数本身。

#3、函数返回值，只能通过$? 系统变量获得，直接通过=,获得是空值。其实，我们按照上面一条理解，知道函数是一个命令，在shell获得命令返回值，都需要通过$?获得。