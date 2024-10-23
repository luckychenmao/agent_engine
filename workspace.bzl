load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def agent_workspace():
    http_archive(
        name = "com_github_grpc_grpc",
        urls = ["file:///mnt/package/grpc-1.67.0.tar.gz"],
        strip_prefix = "grpc-1.67.0",
    )
    # http_archive(
    #     name = "com_google_protobuf",
    #     urls = ["file:///mnt/package/protobuf-28.2.tar.gz"],
    #     strip_prefix = "protobuf-28.2",
    # )
