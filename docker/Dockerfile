# Use Ubuntu 22.04 as the base image
FROM ubuntu:22.04

# Set environment variables to ensure non-interactive installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary packages and tools
RUN sed -i 's|http://archive.ubuntu.com/ubuntu/|http://mirrors.aliyun.com/ubuntu/|g' /etc/apt/sources.list && \
    apt update && \
    apt install -y --no-install-recommends \
    apt-utils \
    pkg-config \
    software-properties-common \
    wget \
    tree \
    curl \
    git \
    build-essential \
    texinfo \
    gcc \
    g++ \
    python3 \
    python3-dev \
    python3-pip \
    screen \
    gdb \
    linux-tools-common \
    linux-tools-generic \
    texinfo \
    cmake \
    libssl-dev \
    protobuf-compiler \
    lld \
    clang \
    unzip \
    acl \
    clang-format \
    openssh-server \
    clang-format \
    libncurses-dev \
    libgnutls28-dev \
    texinfo \
    gnutls-bin \
    && apt clean && rm -rf /var/lib/apt/lists/*

#  增加hook, 开启ssh
RUN mkdir /var/run/sshd /etc/my_init.d
COPY hook.sh /etc/my_init.d/hook.sh
RUN chmod +x /etc/my_init.d/hook.sh
CMD ["/etc/my_init.d/hook.sh"]

# Install Bazel from official binary installer
# https://bazel.build/install/ubuntu?hl=zh-cn#binary-installer
RUN wget https://github.com/bazelbuild/bazel/releases/download/7.4.0rc4/bazel-7.4.0rc4-installer-linux-x86_64.sh && \
    chmod +x bazel-7.4.0rc4-installer-linux-x86_64.sh && \
    ./bazel-7.4.0rc4-installer-linux-x86_64.sh

# Add and install Emacs from source
RUN wget https://mirrors.aliyun.com/gnu/emacs/emacs-29.4.tar.gz && \
    tar -xzf emacs-29.4.tar.gz && \
    cd emacs-29.4 && \
    ./configure && \
    make && \
    make install && \
    cd .. && \
    rm -rf emacs-29.4 emacs-29.4.tar.gz
