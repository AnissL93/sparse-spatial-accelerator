#load("@rules_foreign_cc//:tools/build_defs/cmake.bzl", "cmake_external")
#load("@rules_foreign_cc//foreign_cc:cmake.bzl", "cmake")
load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

cmake(
    name = "openblas",
    # Values to be passed as -Dkey=value on the CMake command line;
    # here are serving to provide some CMake script configuration options
    cache_entries = {
        "NOFORTRAN": "on",
        "BUILD_WITHOUT_LAPACK": "no",
    },
    lib_source = "@openblas//:all",

    # We are selecting the resulting static library to be passed in C/C++ provider
    # as the result of the build;
    # However, the cmake_external dependants could use other artifacts provided by the build,
    # according to their CMake script
    out_static_libs = ["libopenblas.a"],
)

cmake(
    name = "eigen",
    # These options help CMake to find prebuilt OpenBLAS, which will be copied into
    # $EXT_BUILD_DEPS/openblas by the cmake_external script
    cache_entries = {
        "BLA_VENDOR": "OpenBLAS",
        "BLAS_LIBRARIES": "$EXT_BUILD_DEPS/openblas/lib/libopenblas.a",
    },
    lib_source = "@eigen//:all",
    out_headers_only = True,
    # Dependency on other cmake_external rule; can also depend on cc_import, cc_library rules
    deps = [":openblas"],
)

cmake(
    name = "taco",
    # These options help CMake to find prebuilt OpenBLAS, which will be copied into
    # $EXT_BUILD_DEPS/openblas by the cmake_external script
    lib_source = "@taco//:all",
    # Dependency on other cmake_external rule; can also depend on cc_import, cc_library rules
    out_shared_libs = ["libtaco.so"],
    visibility = ["//visibility:public"],
)
