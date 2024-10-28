## 数据结构

    - 关键组成：appid,userid,characterid,chatrecord,memory
    - appid 数据库租户
        - characterid 角色类型，appid 内
        - userid 用户表
        - chatrecord 聊天
        - memory 表



## 对外接口

### worker

    - 对话请求
        - appid 游戏 id
        - userid 用户 id
        - chat 聊天内容
        - llm_model_type llm 语言模型选择
        - memory_rules 记忆策略
        - custom_data 自定义数据
    - 获取历史会话
        - appid 游戏 id
        - userid 用户 id
        - begin_time 可选，起始时间
        - record_count 可选，返回条数
    - 通用图执行run_graph
        - appid 游戏 id
        - userid 用户 id
        - graphid 图 id

### admin

    - 注册图
        - appid 游戏 id
        - graph 执行图具体结构
    - 获取 worker 地址
        - appid 游戏 id
        - graphid
