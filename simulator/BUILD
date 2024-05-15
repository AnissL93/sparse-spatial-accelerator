#load("@rules_foreign_cc//:tools/build_defs/cmake.bzl", "cmake_external")
#load("@rules_foreign_cc//foreign_cc:cmake.bzl", "cmake")
load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

cmake(
    name = "taco",
    # These options help CMake to find prebuilt OpenBLAS, which will be copied into
    # $EXT_BUILD_DEPS/openblas by the cmake_external script
    lib_source = "@taco//:all",
    # Dependency on other cmake_external rule; can also depend on cc_import, cc_library rules
    out_shared_libs = ["libtaco.so"],
    visibility = ["//visibility:public"],
)

cmake(
    name = "rapidjson",
    cache_entries = {
        "RAPIDJSON_BUILD_DOC": "off",
        "RAPIDJSON_BUILD_EXAMPLE": "off",
        "RAPIDJSON_BUILD_TESTS": "off",
    },
    lib_source = "@rapidjson//:all",
    out_headers_only = True,
    visibility = ["//visibility:public"],
)
