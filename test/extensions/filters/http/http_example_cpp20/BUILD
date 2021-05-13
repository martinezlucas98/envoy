load(
    "//bazel:envoy_build_system.bzl",
    "envoy_package",
)
load(
    "//test/extensions:extensions_build_system.bzl",
    "envoy_extension_cc_test",
)

licenses(["notice"])  # Apache 2

envoy_package()

envoy_extension_cc_test(
    name = "http_example_cpp20_filter_test",
    srcs = ["http_example_cpp20_filter_test.cc"],
    extension_name = "envoy.filters.http.http_example_cpp20",
    deps = [
        "//source/extensions/filters/http/http_example_cpp20:http_example_cpp20_filter_lib",
        "//test/mocks/http:http_mocks",
    ],
)

envoy_extension_cc_test(
    name = "config_test",
    srcs = ["config_test.cc"],
    extension_name = "envoy.filters.http.http_example_cpp20",
    deps = [
        "//source/extensions/filters/http/http_example_cpp20:config",
        "//test/mocks/server:factory_context_mocks",
        "@envoy_api//envoy/extensions/filters/http/http_example_cpp20/v3:pkg_cc_proto",
    ],
)
