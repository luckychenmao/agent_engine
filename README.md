## 目录结构
.
├── agent_engine
│   └── network
│       ├── BUILD
│       └── grpc_demo.cpp
├── BUILD
├── doc
│   ├── agent_engine.md
│   └── agent_interface.md
├── docker
│   ├── after_start.sh
│   ├── bin
│   │   ├── after_start.sh
│   │   └── create.sh
│   ├── Dockerfile
│   └── version
├── env
│   ├── config
│   └── env_init.sh
├── examples
│   ├── BUILD
│   ├── gtest_demo.h
│   ├── gtest_demo_test.cpp
│   ├── main.cpp
│   └── README.md
├── MODULE.bazel
└── README.md

### 主要目录
    - agent_engine: 主工程目录
    - doc: 文档
    - docker: 镜像容器
    - env: 环境配置
    - examples: 示例代码
    - MODULE.bazel: bazel 模块依赖

## 简易开发文档
0. 本例中代码默认存放至 ~/agent_engine 目录
1. 编译镜像
    - 进入 docker 目录 `cd ~/agent_engine/docker/`
    - 使用 `docker -t [tag] -f ./Dockerfile .` 编译镜像，或者直接使用已编译镜像库 
    `docker pull registry.cn-hangzhou.aliyuncs.com/agent_dev/agent_hub:1.0.0`
2. 创建容器
    - 使用 `sh ～/agent_engine/docker/bin/create.sh [USERNAME] [DOCKER_IMAGE_TAG]` 创建容器
    - 容器创建结束后会在当前目录在新增目录 USERNAME 
3. 进入容器
    - 使用 `./USERNAME/sshme` 进入到容器内，开始开发
4. 使用 bazel 编译项目
    - 容器内进入开发环境 `cd ~/agent_engine` 
    - 使用 `bazel build //example:bin_demo` 进行编译， 编译结束后产生可执行文件 `~/agent_engine/bazel-bin/examples/bin_demo` 可直接运行
