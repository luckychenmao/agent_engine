package(default_visibility=['//agent_engine/engine:__subpackages__'])

cc_library(
    name='admin_lib',
    srcs=glob(['admin_worker.cc']),
    hdrs=glob(['admin_worker.h']),
    copts=['-Werror'],
    include_prefix='engine/admin',
    deps=[
        '//agent_engine/engine/common:worker_base_lib',
        '//agent_engine/engine/modules:admin_modules_lib',
    ],
    alwayslink=1
)

cc_binary(
    name='admin_bin',
    srcs=['admin_worker_creator.cc'],
    copts=['-Werror'],
    visibility=['//visibility:public'],
    deps=[
        ':admin_lib',
        '//agent_engine/engine/common:binary_main_lib',
    ]
)
