#!/bin/bash

# 检查参数数量
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 DOCKER_NAME IMAGE_ID"
    exit 1
fi

DOCKER_NAME=$1
IMAGE_ID=$2
USER_NAME=`echo $USER`
HOME_DIR=$(eval echo ~$USER)
CUR_DIR=`pwd`/$DOCKER_NAME

# 创建名为 DOCKER_NAME 的文件夹
mkdir -p "$DOCKER_NAME"
rm -rf $CUR_DIR/.passwd
rm -rf $CUR_DIR/.group
rm -rf $CUR_DIR/.ssh

prepare_account()
{
    local userline=$(getent passwd "$USER")
    echo "$userline" > "$CUR_DIR/.passwd"
    local gids=($(id -G "$USER"))
    for gid in "${gids[@]}"; do
        local groupline=$(getent group | awk -v gid="$gid" -F: '$3 == gid {print}')
        if [[ -n $groupline ]]; then
            local group_name=$(echo "$groupline" | awk -F: '{print $1}')
            echo "$groupline" >> "$CUR_DIR/.group"
            # 只导出第一个组名
            if [[ -z $GROUP ]]; then
                export GROUP="$group_name"
            fi
        fi
    done
    chown "$USER:$GROUP" "$CUR_DIR/.passwd"
    chown "$USER:$GROUP" "$CUR_DIR/.group"
}

prepare_ssh()
{
    rm -rf $CUR_DIR/.ssh
    if  ! `mkdir -p $CUR_DIR/.ssh ` || ! `chmod 700 $CUR_DIR/.ssh` ; then
	echo "ERROR: mkdir $CUR_DIR/.ssh failed."
	exit 3
    fi
    if [ -f $HOME_DIR/.ssh/authorized_keys ]; then
	cp -f $HOME_DIR/.ssh/authorized_keys $CUR_DIR/.ssh/authorized_keys
    fi
    if [ ! -f $HOME_DIR/.ssh/id_rsa.pub ]; then
	echo "error, public key $HOME_DIR/.ssh/id_rsa.pub not exist, auto login can't be enabled, use ssh-keygen to generate"
	exit 3
    fi
    if [ -f $HOME_DIR/.ssh/id_rsa.pub ]; then
	if ! `cat $HOME_DIR/.ssh/id_rsa.pub >> $CUR_DIR/.ssh/authorized_keys` || ! `chmod 600 $CUR_DIR/.ssh/authorized_keys` ; then
	    echo "error, copy home rsa key to $CUR_DIR/.ssh/authorized_keys failed"
	fi
	if ! `cp $HOME_DIR/.ssh/id_rsa $CUR_DIR/.ssh/` ; then
	    echo "error, cp private key failed, git clone may failed"
	fi
    fi
    if [ -f $HOME_DIR/.ssh/id_dsa.pub ]; then
	if ! `cat $HOME_DIR/.ssh/id_dsa.pub >> $CUR_DIR/.ssh/authorized_keys`; then
	    echo "error, copy home dsa key to $CUR_DIR/.ssh/authorized_keys failed"
	fi
    fi
    chown -R $USER:$GROUP $CUR_DIR/.ssh
}

cat > ${DOCKER_NAME}/custom_after_start.sh <<'EOF'
#!/bin/bash

cat <<EOF9 >>/etc/profile
export TERM="screen"
export HOSTNAME=`/usr/bin/hostname 2>/dev/null`
EOF9

if [ -f /tmp/.passwd ]; then
    hippouser=`cat /tmp/.passwd | awk -F: '{print $1}'`
    cat /etc/passwd | awk -F: '{print $1}' | grep -w -q $hippouser
    if [ $? -eq 0 ]; then
        sed -i "/$hippouser:x:/d" /etc/passwd
    fi
    echo `cat /tmp/.passwd` >> /etc/passwd

    cat /etc/passwd | awk -F: '{print $1}' | grep -w -q $hippouser
    if [ $? -eq 0 ]; then
        sed -i "/$hippouser:/d" /etc/shadow
    fi
    echo "$hippouser:x:17000:0:99999:7:::" >> /etc/shadow
fi

if [ -f /tmp/.group ]; then
    while read -r line; do
        hippogroup=$(echo "$line" | awk -F: '{print $1}')
        cat /etc/group | awk -F: '{print $1}' | grep -w -q $hippogroup
        if [ $? -eq 0 ]; then
            sed -i "/$hippogroup:x:/d" /etc/group
        fi
        echo "$line" >> /etc/group
    done <<< /tmp/.group
fi

echo -e "\nUSER_NAME ALL=(ALL) NOPASSWD:ALL\n" >> /etc/sudoers

for i in {1..5}; do
    /usr/sbin/sshd -p RANDOM_PORT $*
    RETVAL=$?
    if [ $RETVAL -eq 0 ] ; then
        echo "Port RANDOM_PORT" >> /etc/ssh/sshd_config
        break
    fi
done

# change home dir ownership to make sshd work
if [ -d /home/$USER ]; then
    chown $USER:$GROUP /home/$USER
    chmod 755 /home/$USER
fi

if [ -f /etc/bash.bashrc ]; then
    echo -e "\nPS1='\\n[\\[\\e[m\\e[1;32m\\]\\u\\[\\e[m\\e[1;33m\\]@\\[\\e[m\\e[1;35m\\]\\h \\[\\e[4m\\]\\w\\[\\e[m\\e[1;37m\\]]\\[\\e[m\\]\\n\\$ '" >> /etc/bash.bashrc
    echo -e "\nalias ll='ls -l'" >> /etc/bash.bashrc
fi

# 启动 SSH 服务
service ssh start
EOF

SSHPORT=`python3 -c 'import socket; s=socket.socket(); s.bind(("", 0)); print(s.getsockname()[1]); s.close()'`
sed -i "s/\RANDOM_PORT/$SSHPORT/g" ${DOCKER_NAME}/custom_after_start.sh
sed -i "s/\USER_NAME/$USER/g" ${DOCKER_NAME}/custom_after_start.sh

# 生成 sshme.sh
cat <<EOL > "${DOCKER_NAME}/sshme.sh"
ssh $USER@`hostname` -o ConnectTimeout=1 -p $SSHPORT \$*
EOL

# 生成 stop.sh
cat <<EOL > "${DOCKER_NAME}/stop.sh"
#!/bin/bash
docker stop $DOCKER_NAME
docker rm $DOCKER_NAME
EOL

prepare_ssh
prepare_account

# 修改脚本权限
chmod +x "${DOCKER_NAME}/sshme.sh" "${DOCKER_NAME}/stop.sh"
chmod a+x ${DOCKER_NAME}/custom_after_start.sh
chown  $USER:$GROUP ${CUR_DIR}/custom_after_start.sh


docker run --ulimit nofile=655350:655350 \
       --cap-add SYS_ADMIN -d --net=host \
       --name "$DOCKER_NAME" \
       -v "$HOME_DIR":"$HOME_DIR" \
       -v $CUR_DIR/.ssh/:$HOME_DIR/.ssh \
       -v $CUR_DIR/.passwd:/tmp/.passwd \
       -v $CUR_DIR/.group:/tmp/.group \
       -v $CUR_DIR/custom_after_start.sh:/tmp/custom_after_start.sh \
       "$IMAGE_ID"

echo "容器[$DOCKER_NAME]创建成功，请在目录 $CUR_DIR 中使用 sshme 进入容器."
