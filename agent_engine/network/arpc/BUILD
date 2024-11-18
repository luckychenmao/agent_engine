load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_proto_grpc_cpp//:defs.bzl", "cc_proto_library")

proto_library(
    name='proto',
    srcs=['arpc/proto/msg_header.proto', 'arpc/proto/rpc_extensions.proto'],
)

cc_proto_library(
    name = "proto_cc_proto",
    protos = [":proto"],
    visibility=['//visibility:public'],
    import_prefix='arpc',
    strip_import_prefix='arpc'
)

cc_library(
    name='arpc',
    visibility=['//visibility:public'],
    srcs=(([
        'arpc/DataBufferOutputStream.cpp', 'arpc/MessageSerializable.cpp',
        'arpc/RPCServerAdapter.cpp', 'arpc/UtilFun.cpp',
        'arpc/RPCServerWorkItem.cpp', 'arpc/RPCChannelBase.cpp',
        'arpc/SyncClosure.cpp', 'arpc/RPCMessageSerializable.cpp',
        'arpc/RPCServer.cpp', 'arpc/RPCChannelManagerBase.cpp',
        'arpc/MessageCodec.cpp', 'arpc/RPCServerClosure.cpp', 'arpc/Tracer.cpp',
        'arpc/AdminService.cpp', 'arpc/RPCServerList.cpp'
    ] + [
        'arpc/anet/ANetApp.cpp', 'arpc/anet/ANetRPCMessageCodec.cpp',
        'arpc/anet/ANetRPCServerAdapter.cpp',
        'arpc/anet/ANetRPCServerClosure.cpp',
        'arpc/anet/ANetRPCServerWorkItem.cpp',
        'arpc/anet/ClientPacketHandler.cpp',
        'arpc/anet/SharedClientPacketHandler.cpp', 'arpc/ANetRPCServer.cpp',
        'arpc/ANetRPCController.cpp', 'arpc/ANetRPCChannel.cpp',
        'arpc/ANetRPCChannelManager.cpp', 'arpc/PooledChannelManager.cpp'
    ]) + glob(['arpc/common/*.cpp', 'arpc/util/*.cpp', 'arpc/anet/*.h'])),
    hdrs=glob(['arpc/*.h', 'arpc/anet/*.h', 'arpc/util/*.h',
               'arpc/common/*.h']),
    deps=[
        ':proto_cc_proto', '//agent_engine/network/anet:anet',
        '//agent_engine/network/arpc/arpc/metric:arpc_metric', '//agent_engine/alog:alog',
        '//agent_engine/util:thread', '//agent_engine/util:env_util', '//agent_engine/util:lock_free'
    ],
    copts=['-Wformat-truncation=0'],
    alwayslink=True
)
test_copts = ['-Wno-deprecated-declarations']
# py_library(
#     name='arpc_python',
#     srcs=glob(['arpc/python/*.py']),
#     visibility=['//visibility:public']
# )
# filegroup(
#     name='py_init_file',
#     srcs=['arpc/python/__init__.py'],
#     visibility=['//visibility:public']
# )
# bundle_files(
#     name='arpc_init_package',
#     prefix='usr/local/lib/python/site-packages/arpc',
#     strip_prefix='arpc/python',
#     srcs=[':py_init_file'],
#     tags=['manual']
# )
# bundle_files(
#     name='arpc_proto_init_package',
#     prefix='usr/local/lib/python/site-packages/arpc/proto',
#     strip_prefix='arpc/python',
#     srcs=[':py_init_file'],
#     tags=['manual']
# )
# bundle_files(
#     name='arpc_python_inner_package',
#     prefix='usr/local/lib/python/site-packages',
#     srcs=[':arpc_python'],
#     deps=[':arpc_init_package'],
#     tags=['manual']
# )
# bundle_files(
#     name='arpc_proto_inner_package',
#     prefix='usr/local/lib/python/site-packages',
#     srcs=[':proto_py'],
#     deps=[':arpc_proto_init_package'],
#     tags=['manual']
# )
# bundle_files(
#     name='arpc_python_package',
#     prefix='usr/local/lib/python/site-packages',
#     strip_prefix='arpc/python',
#     srcs=[':py_init_file'],
#     deps=[':arpc_python_inner_package', ':arpc_proto_inner_package'],
#     tags=['manual'],
#     visibility=['//visibility:public']
# )
