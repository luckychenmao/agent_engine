
cc_test(
    name = "engine_test",
    srcs = glob([
        "*_test.cc",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "//agent_engine/engine:worker_base_lib",
        "//agent_engine/engine:worker_lib",
        "//agent_engine/engine:admin_lib",
         "@googletest//:gtest_main",
    ],
    copts = [
        "-fno-access-control",
    ],
)
