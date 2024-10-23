#!/bin/bash

tdir=$(dirname "$(readlink -f "\$0")")/..

cd "$rootdir"

# init
init_yum() {
    #sudo yum update -y
    #sudo yum upgrade -y
    sudo yum install texinfo -y
    sudo yum install ncurses-devel -y
    sudo yum install epel-release -y
    sudo yum install gnutls-devel -y
}

copy_config_files() {
    echo "Copying .screenrc and .emacs to home directory..."
    cp ./config/.screenrc ~/
    cp ./config/.emacs ~/
    if [ $? -eq 0 ]; then
        echo "Files copied successfully."
    else
        echo "Error copying files." >&2
        exit 1
    fi
}

install_emacs() {
    echo "Checking for existing Emacs tar..."
    EMACS_URL="https://mirrors.aliyun.com/gnu/emacs/emacs-29.4.tar.gz"
    EMACS_TAR="emacs-29.4.tar.gz"
    EMACS_MD5="82a95b3e488cace8e5edd1dade8b49ee"

    # 检查文件是否存在，并计算 MD5 值
    if [ -f "$EMACS_TAR" ]; then
        echo "Tar found, checking MD5..."
	MD5_SUM=$(md5sum $EMACS_TAR | awk '{print $1}')
        echo $MD5_SUM
	echo $EMACS_MD5
        if [ "$MD5_SUM" = "$EMACS_MD5" ]; then
            echo "MD5 verified, no need to download."
        else
            echo "MD5 mismatch, re-downloading..."
            wget $EMACS_URL -O $EMACS_TAR
        fi
    else
        echo "Tar not found, downloading..."
        wget $EMACS_URL -O $EMACS_TAR
    fi

    # 检查下载是否成功
    if [ $? -ne 0 ]; then
        echo "Error downloading Emacs." >&2
        exit 1
    fi

    # 解压和安装
    tar -xzf $EMACS_TAR
    cd emacs-29.4
    ./configure
    make
    sudo make install

    if [ $? -eq 0 ]; then
        echo "Emacs installed successfully."
    else
        echo "Error installing Emacs." >&2
        exit 1
    fi

    cd ..
    rm -rf emacs-29.4*
}

install_screen() {
    echo "安装screen..."
    sudo yum install -y screen
    if [ $? -eq 0 ]; then
            echo "安装screen完成"
    else
        echo "安装screen失败" >&2
        exit 1
    fi
}

install_docker() {
    echo "安装docker..."
    sudo yum install -y docker
    if [ $? -eq 0 ]; then
            echo "安装docker完成"
    else
        echo "安装docker失败" >&2
        exit 1
    fi
}

install_bazel() {
    echo "Checking for existing bazel tar..."
    BAZEL_URL="https://mirror.moson.org/arch/extra/os/x86_64/bazel-7.3.2-1-x86_64.pkg.tar.zst"
    BAZEL_TAR="bazel-7.3.2-1-x86_64.pkg.tar.zst"
    BAZEL_MD5="82a95b3e488cace8e5edd1dade8b49ee"

    # 检查文件是否存在，并计算 MD5 值
    if [ -f "$BAZEL_TAR" ]; then
        echo "Tar found, checking MD5..."
	MD5_SUM=$(md5sum $BAZEL_TAR | awk '{print $1}')
        echo $MD5_SUM
	echo $BAZEL_MD5
        if [ "$MD5_SUM" = "$BAZEL_MD5" ]; then
            echo "MD5 verified, no need to download."
        else
            echo "MD5 mismatch, re-downloading..."
            wget $BAZEL_URL -O $BAZEL_TAR
        fi
    else
        echo "Tar not found, downloading..."
        wget $BAZEL_URL -O $BAZEL_TAR
    fi

    # 检查下载是否成功
    if [ $? -ne 0 ]; then
        echo "Error downloading Emacs." >&2
        exit 1
    fi

    # 解压和安装

    tar -xzf $BAZEL_TAR
}
# 执行脚本
init_yum
copy_config_files
install_emacs
install_screen
install_docker

echo "环境初始化完成."
