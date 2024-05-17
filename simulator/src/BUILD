cc_library(
    name = "tensor",
    srcs = [
        "tensor.cc",
        "tensor.h",
    ],
    deps = ["//:taco"],
)

cc_library(
    name = "memory",
    srcs = [
        "base_unit.cc",
        "base_unit.h",
        "memory.cc",
        "memory.h",
    ],
    deps = [
        ":tensor",
        "//:taco",
        "//src/config:arch_config",
        "//src/config:event",
    ],
)

cc_library(
    name = "pe",
    srcs = [
        "pe.cc",
        "pe.h",
    ],
    deps = [
        ":memory",
    ],
)

cc_library(
    name = "core",
    srcs = [
        "chip.cc",
        "chip.h",
        "execute.cc",
        "execute.h",
        "task.cc",
        "task.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "tensor",
        ":pe",
    ],
)
