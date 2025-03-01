#!/bin/bash
current_minute=$(date +%M)
if [ ! -d "backup" ]; then
    mkdir backup
fi
remainder=$(expr $current_minute % 5)
if [ $remainder -eq 0 ]; then
    find . -maxdepth 1 -type f -name "*.png" ! -path "./backup/*" -exec cp -p {} ./backup/ \;
    echo "备份完成，时间: $(date '+%Y-%m-%d %H:%M:%S')"
else
    echo "当前分钟($current_minute)不是5的倍数，不执行备份"
fi