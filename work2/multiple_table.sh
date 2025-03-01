#!/bin/bash
# Program: 输出九九乘法表

# 使用双重循环来生成乘法表
for ((i=1; i<=9; i++))
do
    for ((j=1; j<=i; j++))
    do
        # 计算乘积
        result=$((i*j))
        # 使用printf格式化输出，保证对齐
        printf "%d×%d=%-2d " $j $i $result
    done
    # 每行结束后换行
    echo ""
done