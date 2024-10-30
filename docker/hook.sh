#!/bin/bash

# 切换到脚本目录
cd /etc/my_init.d

echo "running hook.sh..."

# 如果存在自定义的钩子脚本，执行它
if [ -f "/tmp/custom_after_start.sh" ]; then
    bash /tmp/custom_after_start.sh || echo "error executing custom_after_start.sh, continuing..."
else
    echo "ignoring start hook!"
fi

# 持续保持运行
tail -f /dev/null
