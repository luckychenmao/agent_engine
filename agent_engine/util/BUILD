cc_library(
    name='log',
    hdrs=['Log.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
    deps=[
        ':time',
        '//agent_engine/alog'
    ],
    alwayslink=True
)

cc_library(
    name='time',
    srcs=['TimeUtility.cpp'],
    hdrs=['TimeUtility.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
    alwayslink=True
)

cc_library(
    name='scope',
    hdrs=['Scope.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
    alwayslink=True
)

cc_test(
    name = "util_test",
    srcs = glob([
        "test/*.cpp",
    ]),
    copts = ["-fno-access-control"],
    visibility = ["//visibility:public"],
    deps = [
        ":time",
         "@googletest//:gtest_main",
    ],
)
