load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_extension",
    "envoy_cc_library",
    "envoy_extension_package",
)

licenses(["notice"])  # Apache 2

# HTTP filter example with C++20 optional features
# Public docs: docs/root/configuration/http_filters/http_example_cpp20_filter.rst

envoy_extension_package()

envoy_cc_library(
    name = "http_example_cpp20_filter_lib",
    srcs = ["http_example_cpp20_filter.cc"],
    hdrs = ["http_example_cpp20_filter.h"],
    deps = [
        "//include/envoy/http:filter_interface",
        "//source/extensions/filters/http/common:pass_through_filter_lib",
        "@envoy_api//envoy/extensions/filters/http/http_example_cpp20/v3:pkg_cc_proto",
    ],
)

envoy_cc_extension(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    category = "envoy.filters.http",
    security_posture = "requires_trusted_downstream_and_upstream",
    status = "alpha",
    deps = [
        ":http_example_cpp20_filter_lib",
        "//include/envoy/registry",
        "//source/extensions/common/aws:credentials_provider_impl_lib",
        "//source/extensions/common/aws:signer_impl_lib",
        "//source/extensions/filters/http:well_known_names",
        "//source/extensions/filters/http/common:factory_base_lib",
        "@envoy_api//envoy/extensions/filters/http/http_example_cpp20/v3:pkg_cc_proto",
    ],
)
