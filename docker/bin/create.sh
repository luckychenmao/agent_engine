#!/bin/bash

# 检查参数数量
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 DOCKER_NAME IMAGE_ID"
    exit 1
fi

DOCKER_NAME=$1
IMAGE_ID=$2
USER_NAME=`echo $USER`
USER_ID=$(id -u)
GROUP_ID=$(id -g)
HOME_DIR=$(eval echo ~$USER)
CUR_DIR=`pwd`/$DOCKER_NAME

# 创建名为 DOCKER_NAME 的文件夹
mkdir -p "$DOCKER_NAME"

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

# 生成 sshme.sh
cat <<EOL > "${DOCKER_NAME}/sshme.sh"
ssh $USER@`hostname` -o ConnectTimeout=1 -p 22 \$*
EOL

# 生成 stop.sh
cat <<EOL > "${DOCKER_NAME}/stop.sh"
#!/bin/bash
docker stop $DOCKER_NAME
docker rm $DOCKER_NAME
EOL

cat <<EOL > "${CUR_DIR}/custom_after_start.sh"
#!/bin/bash
groupadd -g $GROUP_ID $USER
useradd -m -u $USER_ID -g $GROUP_ID -s /bin/bash $USER
# 启动 SSH 服务
service ssh start
if [ -f /etc/bash.bashrc ]; then
    echo -e "\nPS1='\\n\\[\\e[1;37m\\][\\[\\e[m\\e[1;36m\\]$DOCKER_NAME\\[\\e[m\\e[1;37m\\]][\\[\\e[m\\e[1;32m\\]\\u\\[\\e[m\\e[1;33m\\]@\\[\\e[m\\e[1;35m\\]\\h \\[\\e[4m\\]\\w\\[\\e[m\\e[1;37m\\]]\\[\\e[m\\]\\n\\$ '" >> /etc/bash.bashrc
    # 设置 ll 的别名
    echo -e "\nalias ll='ls -l'" >> /etc/bash.bashrc
fi
source /etc/bash.bashrc
EOL

# 修改脚本权限
chmod +x "${DOCKER_NAME}/sshme.sh" "${DOCKER_NAME}/stop.sh"
chmod a+x ${CUR_DIR}/custom_after_start.sh
chown  $USER:$GROUP ${CUR_DIR}/custom_after_start.sh

prepare_ssh
# 创建 Docker 容器
docker run -it --ulimit nofile=655350:655350 \
       --cap-add SYS_ADMIN -d --net=host \
       --name "$DOCKER_NAME" \
       -v "$HOME_DIR":"$HOME_DIR" \
       -v $CUR_DIR/.ssh/:$HOME_DIR/.ssh \
       -v $CUR_DIR/custom_after_start.sh:/tmp/custom_after_start.sh \
       "$IMAGE_ID" bash
echo "Container $DOCKER_NAME created and scripts generated in $DOCKER_NAME directory."
