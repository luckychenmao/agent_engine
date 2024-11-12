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
        "//agent_engine/util:json",
        "//agent_engine/util:scope",
        "//agent_engine/util:thread",
        "//agent_engine/util:string_helper",
    ],
)

cc_library(
    name = "grpc_client",
    srcs = glob([
        "grpc_client.cc",
        "grpc_connection_pool.cc",
        "grpc_connection.cc",
    ]),
    hdrs = glob([
        "grpc_client.h",
        "grpc_connection_pool.h",
        "grpc_connection.h",
        "completion_queue_status.h",
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
    name = "server_demo",
    srcs = glob([
        "example/grpc_server_demo.cc",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        ":grpc_server",
    ],
)

cc_binary(
    name = "client_demo",
    srcs = glob([
        "example/grpc_client_demo.cc",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        ":grpc_client",
    ],
)
