#!/bin/bash

# 获取当前目录
current_dir=$(pwd)

# 遍历当前目录下所有子文件夹
for dir in "$current_dir"/*; do
    if [ -d "$dir" ]; then
        # 在子文件夹中查找孙文件夹并执行孙文件夹中的zip.sh脚本
        for sub_dir in "$dir"/*; do
            if [ -d "$sub_dir" ]; then
                zip_script="$sub_dir/zip.sh"
                if [ -f "$zip_script" ]; then
                    cd "$sub_dir"
                    chmod +x "$zip_script"
                    "$zip_script"
                    cd "$current_dir"
                fi
            fi
        done
    fi
done

echo "全部压缩完成"
