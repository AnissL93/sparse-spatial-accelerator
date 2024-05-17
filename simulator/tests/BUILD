load("//:build_tools/bazel_tool.bzl", "cc_gtest")

cc_library(
    name = "utils",
    srcs = [
        "utils.cc",
        "utils.h",
    ],
    deps = [
        "//:taco",
    ],
)

cc_gtest(
    name = "test_tensor",
    srcs = ["test_tensor.cc"],
    deps = [
        ":utils",
        "//:taco",
        "//src:core",
    ],
)

cc_gtest(
    name = "test_task",
    srcs = ["test_task.cc"],
    deps = [
        ":utils",
        "//src:core",
    ],
)

cc_gtest(
    name = "test_memory",
    srcs = [
        "test_memory.cc",
    ],
    deps = [
        ":utils",
        "//src:core",
    ],
)
