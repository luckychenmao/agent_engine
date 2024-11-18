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

cc_library(
    name='lock',
    srcs=['lock.cc'],
    hdrs=['lock.h', 'thread_annotations.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
    deps=[':time', 'common_macros'],
    alwayslink=True
)

cc_library(
    name='lock_free',
    hdrs=[
        'lock_free.h',
        'lock_free_queue.h',
    ],
    include_prefix='util',
    visibility=['//visibility:public'],
    alwayslink=True
)

cc_library(
    name='thread',
    srcs=[
        'thread.cc',
        'loop_thread.cc',
        'thread_pool.cc',
        'lock_free_thread_pool.cc',
        'thread_auto_scaler.cc',
    ],
    hdrs=[
        'thread.h',
        'loop_thread.h',
        'thread_pool.h',
        'lock_free_thread_pool.h',
        'circular_queue.h',
        'workitem.h',
        'workitem_queue.h',
        'lock_free_queue.h',
        'lock_free.h',
        'thread_auto_scaler.h',
    ],
    include_prefix='util',
    visibility=['//visibility:public'],
    deps=[':exception', ':log', ':string_helper', 'lock', 'lock_free'],
    alwayslink=True
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

cc_library(
    name='singleton',
    hdrs=['singleton.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
    alwayslink=True
)

cc_library(
    name='env_util',
    srcs=['env_util_impl.cc'],
    hdrs=['env_util.h', 'env_util_impl.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
    deps=[':string_helper', ':log', ':singleton'],
    alwayslink=True
)

cc_library(
    name='backtrace',
    hdrs=['backtrace.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
    deps=[]
)

cc_library(
    name='exception',
    srcs=['legacy/exception.cpp'],
    hdrs=['legacy/exception.h'],
    include_prefix='util',
    visibility=['//visibility:public'],
    deps=[':backtrace', ':env_util'],
    alwayslink=True
)

cc_library(
    name='json_base',
    srcs=[
        'legacy/astream.cpp',
        'legacy/json.cpp',
        'legacy/json_string_enhanced.cpp',
        'legacy/string_conversion.cpp',
        'legacy/string_utf8.cpp'
    ],
    hdrs=[
        'legacy/any.h',
        'legacy/astream.h',
        'legacy/json.h',
        'legacy/jsonizable_exception.h',
        'legacy/string_tools.h'
    ],
    include_prefix='util',
    visibility=['//visibility:public'],
    deps=[
        ':exception',
        ':string_helper'
    ],
    alwayslink=True
)

cc_library(
    name='fast_json',
    srcs=['legacy/fast_jsonizable.cpp'],
    hdrs=[
        'legacy/fast_jsonizable.h',
        'legacy/fast_jsonizable_dec.h'
    ],
    include_prefix='util',
    visibility=['//visibility:public'],
    deps=[
        ':json_base',
        '@rapidjson'
    ],
    alwayslink=True
)

cc_library(
    name='json',
    hdrs=[
        'legacy/jsonizable.h',
        'legacy/legacy_jsonizable.h',
        'legacy/legacy_jsonizable_dec.h'
    ],
    include_prefix='util',
    visibility=['//visibility:public'],
    deps=[
        ':fast_json', ':json_base', '@rapidjson'
    ],
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
