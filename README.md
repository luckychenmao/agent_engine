### 主要目录
    - agent_engine: 主工程目录
    - doc: 文档
    - docker: 镜像容器
    - env: 环境配置
    - examples: 示例代码
    - MODULE.bazel: bazel 模块依赖

## 简易开发文档
0. 本例中代码默认存放至 ~/agent_engine 目录
1. 环境准备参考 `env/README.md`
2. 进入容器后，使用 bazel 编译项目
    - 容器内进入开发环境 `cd ~/agent_engine` 
    - 使用 `bazel build //examples:bin_demo` 进行编译， 编译结束后产生可执行文件 `~/agent_engine/bazel-bin/examples/bin_demo` 可直接运行
