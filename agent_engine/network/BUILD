package(default_visibility = ["//visibility:public",])

cc_library(
    name = "grpc_server",
    srcs = glob([
        "grpc_server.cc",
        "server_config.cc",
    ]),
    hdrs = glob([
        "grpc_server.h",
        "server_config.h",
    ]),
    visibility = ["//visibility:public"],
    include_prefix='network',
    deps = [
        "@grpc//:grpc++",
        "//agent_engine/network/proto:agent_service_grpc",
        "//agent_engine/util:json",
        "//agent_engine/util:scope",
        "//agent_engine/util:thread",
        "//agent_engine/util:string_helper",
    ],
)

cc_binary(
    name = "demo",
    srcs = glob([
        "grpc_demo.cc",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        ":grpc_server",
    ],
)
