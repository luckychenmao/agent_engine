cc_library(
    name='log',
    hdrs=['log.h'],
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
    srcs=['time_utility.cc'],
    hdrs=['time_utility.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
    alwayslink=True
)

cc_library(
    name='scope',
    hdrs=['scope.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
    alwayslink=True
)

cc_test(
    name = "util_test",
    srcs = glob([
        "test/*.cc",
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
        'command_line_parameter.cc',
        'option_parser.cc'
    ],
    hdrs=[
        'command_line_parameter.h',
        'option_parser.h'
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
        'string_tokenizer.cc',
        'string_util.cc'
    ],
    hdrs=[
        'string_tokenizer.h',
        'string_util.h'
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
    srcs=['file_util.cc'],
    hdrs=['file_util.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
)

cc_library(
    name='common_macros',
    hdrs=['common_macros.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
    alwayslink=True
)
