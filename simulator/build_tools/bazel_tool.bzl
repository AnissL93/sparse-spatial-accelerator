load("@rules_cc//cc:defs.bzl", "cc_test")

def cc_gtest(name, srcs, deps = []):
    cc_test(
        name = name,
        srcs = srcs,
        deps = deps + [
            "@com_google_googletest//:gtest_main",
        ],
    )
