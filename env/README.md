## 环境准备

### 创建用户
   - 创建用户（此处以创建用户 user_a 为例） 
      > `sudo useradd -m user_a`
   - 增加 sudo 权限 
      > `sudo usermod -aG sudo user_a`
   - 获取docker权限（后续无需sudo使用docker指令）
      > `sudo usermod -aG docker user_a`
      >
      > `sudo chmod 666 /var/run/docker.sock`
   - 切换到 user_a 
      > `su user_a; cd ~/`

### 准备代码
   - 使用 ssh-keygen 创建 ssh key
   - 使用git clone 代码到 home 目录 
      > `git clone git@agent_engine.git`
   - 查看 docker images 找到最新版本镜像 tag
      > `docker images`，
   - 使用脚本创建镜像 
      > `~/agent_engine/docker/bin/create.sh [user_a] [tag]`
      >
      > 注意：如果 docker images 无可用镜像，说明环境为第一次创建，可使用`cd ~/agent_engine/docker;sh build_image.sh` 重新 build 镜像
    - 镜像创建后在当前目录下使用 `./user_a/sshme.sh` 进入容器开始开发
