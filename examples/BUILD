# 编译 binary 目标
cc_binary(
    name = "bin_demo",
    srcs = glob([
        "main.cpp",
    ]),
    visibility = ["//visibility:public"],
    deps = [
    ],
)

cc_binary(
    name = "bin_demo_libcurl_multi",
    srcs = glob([
        "gtest_demo_libcurl_multi_test.cpp",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "@curl//:curl",  # bazel query @curl//...
        "@googletest//:gtest_main",
    ],
)

# 编译 library 目标
cc_library(
    name = "gtest_demo_lib",
    hdrs = glob([
        "gtest_demo.h"
    ]),
    visibility = ["//visibility:public"],
)

# 编译 test 目标
cc_test(
    name = "gtest_demo",
    srcs = glob([
        "gtest_demo_test.cpp",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "gtest_demo_lib",
         "@googletest//:gtest_main",
    ],
)


cc_test(
    name = "gtest_demo_rapidjson_test",
    srcs = glob([
        "gtest_demo_rapidjson_test.cpp",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "@rapidjson//:rapidjson",
        "@googletest//:gtest_main",
    ],
)
