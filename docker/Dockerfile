# Use Ubuntu 22.04 as the base image
FROM ubuntu:22.04

# Set environment variables to ensure non-interactive installation
ENV DEBIAN_FRONTEND=noninteractive
ENV http_proxy=http://8.209.206.35:3419
ENV https_proxy=http://8.209.206.35:3419

# Install necessary packages and tools
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    apt-utils \
    pkg-config \
    software-properties-common \
    wget \
    tree \
    curl \
    git \
    build-essential \
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
    unzip \
    acl \
    openssh-server \
    clang-format \
    libncurses-dev \
    libgnutls28-dev \
    texinfo \
    gnutls-bin \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

#  增加hook, 开启ssh
RUN mkdir /var/run/sshd /etc/my_init.d
COPY after_start.sh /etc/my_init.d/after_start.sh
RUN chmod +x /etc/my_init.d/after_start.sh
EXPOSE 22
CMD ["/etc/my_init.d/after_start.sh"]


# Add LLVM apt repository and install latest Clang (18)
RUN wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh 18

# Creating a Symbolic Link
RUN ln -s /usr/bin/clang-18 /usr/bin/clang \
    && ln -s /usr/bin/clang++-18 /usr/bin/clang++

# Set Clang 18 as default
RUN update-alternatives --install /usr/bin/cc cc /usr/bin/clang-18 100 \
    && update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-18 100

# Verify Clang installation and C++20 support
RUN clang++-18 --version && clang++-18 -std=c++20 -fsyntax-only -xc++ /dev/null

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

# Install Google Test and Google Mock from source
RUN git clone https://github.com/google/googletest.git /usr/src/googletest && \
    cd /usr/src/googletest && \
    mkdir build && cd build && \
    cmake .. && make && make install && \
    ldconfig
