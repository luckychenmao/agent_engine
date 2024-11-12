package(default_visibility = ["//visibility:public"])

load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_proto_grpc_cpp//:defs.bzl", "cpp_grpc_library", "cpp_proto_library")

proto_library(
    name = "agent_service_proto",
    srcs = ["agent_service.proto"],
)

cpp_proto_library(
    name = "cpp_agent_service_proto",
    strip_include_prefix='agent_service_grpc_pb/agent_engine/engine',
    protos = [":agent_service_proto"],
)

cpp_grpc_library(
    name = "agent_service_grpc",
    strip_include_prefix='agent_service_grpc_pb/agent_engine/engine',
    protos = [":agent_service_proto"],
)
