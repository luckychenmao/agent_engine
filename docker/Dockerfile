# Use Ubuntu 22.04 as the base image
FROM ubuntu:22.04

# Set environment variables to ensure non-interactive installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary packages and tools
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    apt-utils \
    software-properties-common \
    wget \
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
    cmake \
    libssl-dev \
    protobuf-compiler \
    emacs \
    lld \
    unzip \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

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
RUN wget https://github.com/bazelbuild/bazel/releases/download/7.3.2/bazel-7.3.2-installer-linux-x86_64.sh && \
    chmod +x bazel-7.3.2-installer-linux-x86_64.sh && \
    ./bazel-7.3.2-installer-linux-x86_64.sh

# Install Google Test and Google Mock from source
RUN git clone https://github.com/google/googletest.git /usr/src/googletest && \
    cd /usr/src/googletest && \
    mkdir build && cd build && \
    cmake .. && make && make install && \
    ldconfig

# Set default working directory
WORKDIR /app

# Default command
CMD ["bash"]

