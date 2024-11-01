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

cc_library(
    name='option_parser',
    srcs=[
        'CommandLineParameter.cpp',
        'OptionParser.cpp'
    ],
    hdrs=[
        'CommandLineParameter.h',
        'OptionParser.h'
    ],
    include_prefix='util',
    visibility=['//visibility:public'],
    deps=[
        ':string_helper',
        ':log'
    ],
    alwayslink=True
)

cc_library(
    name='string_helper',
    srcs=[
        'StringTokenizer.cpp',
        'StringUtil.cpp'
    ],
    hdrs=[
        'StringTokenizer.h',
        'StringUtil.h'
    ],
    include_prefix='util',
    visibility=['//visibility:public'],
    deps=[
        ':common_macros',
        ':log',
    ],
    alwayslink=True,
)

cc_library(
    name='file_util',
    srcs=['FileUtil.cpp'],
    hdrs=['FileUtil.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
)

cc_library(
    name='common_macros',
    hdrs=['CommonMacros.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
    alwayslink=True
)
