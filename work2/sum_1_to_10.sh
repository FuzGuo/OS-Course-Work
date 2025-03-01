#!/bin/bash
# Program: 计算1到10的和
sum=0
for i in {1..10}; do
  sum=$((sum + i))
done
echo "1到10的和为: $sum"
