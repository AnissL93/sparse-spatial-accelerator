load("@rules_proto//proto:defs.bzl", "proto_library")

cc_library(
    name = "arch_config",
    srcs = [
        "arch_config.cc",
        "arch_config.h",
    ],
    visibility = ["//visibility:public"],
    deps = [":arch_cc_proto"],
)

cc_proto_library(
    name = "arch_cc_proto",
    deps = [":arch_proto"],
)

proto_library(
    name = "arch_proto",
    srcs = [":arch.proto"],
    deps = [
        "@com_google_protobuf//:any_proto",
    ],
)

cc_library(
    name = "event",
    srcs = [
        "event.cc",
        "event.h",
    ],
    visibility = ["//visibility:public"],
    deps = [":event_cc_proto"],
)

cc_proto_library(
    name = "event_cc_proto",
    deps = [":event_proto"],
)

proto_library(
    name = "event_proto",
    srcs = [":event.proto"],
    deps = [
        "@com_google_protobuf//:any_proto",
    ],
)
