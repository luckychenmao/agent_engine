#!/bin/bash

# 切换到脚本目录
cd /etc/my_init.d

# 如果存在自定义的钩子脚本，执行它
if [ -f "/tmp/custom_after_start.sh" ]; then
    bash /tmp/custom_after_start.sh
else
    echo "ignore start hook!"
fi
