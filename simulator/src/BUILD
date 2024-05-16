cc_library(
    name = "pe",
    srcs = [
        "pe.cc",
        "pe.h",
    ],
    deps = [
        "//:taco",
        "//src/config:arch_config",
        "//src/config:event",
    ],
)

cc_library(
    name = "core",
    srcs = glob([
        "*.cc",
        "*.h",
    ]),
    visibility = ["//visibility:public"],
    deps = [
        "//:taco",
        "//src/config:arch_config",
        "//src/config:event",
    ],
)
