package(default_visibility=['//agent_engine/engine:__subpackages__'])

cc_library(
    name='worker_modules_lib',
    srcs=glob([
        '*_module.cc',
    ]),
    hdrs=glob([
        '*_module.h',
    ]),
    copts=['-Werror'],
    include_prefix='engine/worker',
    deps=[
        '//agent_engine/engine/common:common_lib',
        '//agent_engine/engine/common:worker_base_lib',
        '//agent_engine/network:grpc_server',
        "//agent_engine/engine/proto:agent_service_grpc",
    ],
    alwayslink=1
)

cc_library(
    name='worker_lib',
    srcs=glob(['worker.cc']),
    hdrs=glob(['worker.h']),
    copts=['-Werror'],
    include_prefix='engine/worker',
    deps=[
        '//agent_engine/engine/common:worker_base_lib',
        '//agent_engine/engine/modules:worker_modules_lib',
    ],
    alwayslink=1
)

cc_binary(
    name='worker_bin',
    srcs=['worker_creator.cc'],
    copts=['-Werror'],
    visibility=['//visibility:public'],
    deps=[
        ':worker_lib',
        '//agent_engine/engine/common:binary_main_lib',
    ]
)
