
# 1. 编译可执行文件 binary
bazel build //agent_engine/examples:bin_demo

# 2. 编译gtest unittest
bazel test //agent_engine/examples:gtest_demo

# 3. 编译gtest unittest - gtest_demo_rapidjson_test

如果需要将测试的所有输出（包括标准输出和标准错误）显示在控制台上， 添加参数  --test_output=all， 如

```
bazel test //examples:gtest_demo_rapidjson_test --test_output=all
```


