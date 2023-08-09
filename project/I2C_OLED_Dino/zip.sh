#!/bin/bash

# 定义文件夹名称和压缩文件名
folder_name="I2C_OLED_Dino"
zip_file_name="${folder_name}.zip"

# 删除已存在的压缩文件
if [ -f "$zip_file_name" ]; then
    rm "$zip_file_name"
fi

# 创建新的压缩文件
zip -rq "$zip_file_name" "$folder_name" -x "$folder_name/build/*" "$folder_name/Debug/*"

echo "${folder_name} 压缩完成"
