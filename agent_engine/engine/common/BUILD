package(default_visibility=['//agent_engine/engine:__subpackages__'])

cc_library(
    name='common_lib',
    srcs=glob([
        'common.cc',
    ]),
    hdrs=glob([
        'common.h',
        'creator_factory.h',
    ]),
    copts=['-Werror'],
    include_prefix='engine/common',
    deps=[
        '//agent_engine/util:singleton',
    ],
    alwayslink=1
)

cc_library(
    name='binary_main_lib',
    srcs=glob([
        'main.cc',
    ]),
    copts=['-Werror'],
    include_prefix='engine/common',
    deps=[
        ':worker_base_lib',
    ],
    alwayslink=1
)


cc_library(
    name='worker_base_lib',
    srcs=glob([
        'worker_base.cc',
        'base_module.cc',
        'module_manager.cc',
    ]),
    hdrs=glob([
        'worker_base.h',
        'base_module.h',
        'module_manager.h',
    ]),
    copts=['-Werror'],
    include_prefix='engine/common',
    deps=[
        '//agent_engine/util:log',
        '//agent_engine/util:scope',
        '//agent_engine/util:option_parser',
        '//agent_engine/util:string_helper',
        '//agent_engine/util:file_util',
        '//agent_engine/util:singleton',
        ':common_lib',
    ],
    alwayslink=1
)

cc_test(
    name = "engine_test",
    srcs = glob([
        "test/*_test.cc",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        ":worker_base_lib",
         "@googletest//:gtest_main",
    ],
    copts = [
        "-fno-access-control",
    ],
)
