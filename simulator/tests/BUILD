load("//:build_tools/bazel_tool.bzl", "cc_gtest")

cc_gtest(
    name = "test_tensor",
    srcs = ["test_tensor.cc"],
    deps = [
        "//:taco",
        "//src:core",
    ],
)

cc_gtest(
    name = "test_task",
    srcs = ["test_task.cc"],
    deps = ["//src:core"],
)
