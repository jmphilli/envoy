licenses(["notice"])  # Apache 2

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_test",
    "envoy_cc_test_library",
    "envoy_package",
)

envoy_package()

envoy_cc_test(
    name = "filesystem_subscription_impl_test",
    srcs = ["filesystem_subscription_impl_test.cc"],
    deps = [
        ":filesystem_subscription_test_harness",
    ],
)

envoy_cc_test_library(
    name = "filesystem_subscription_test_harness",
    srcs = ["filesystem_subscription_test_harness.h"],
    external_deps = ["envoy_eds"],
    deps = [
        ":subscription_test_harness",
        "//source/common/config:filesystem_subscription_lib",
        "//source/common/config:utility_lib",
        "//source/common/event:dispatcher_lib",
        "//test/mocks/config:config_mocks",
        "//test/test_common:environment_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "grpc_mux_impl_test",
    srcs = ["grpc_mux_impl_test.cc"],
    external_deps = [
        "envoy_discovery",
        "envoy_eds",
    ],
    deps = [
        "//source/common/config:grpc_mux_lib",
        "//source/common/config:resources_lib",
        "//source/common/protobuf",
        "//test/mocks/config:config_mocks",
        "//test/mocks/event:event_mocks",
        "//test/mocks/grpc:grpc_mocks",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "grpc_subscription_impl_test",
    srcs = ["grpc_subscription_impl_test.cc"],
    deps = [
        ":grpc_subscription_test_harness",
    ],
)

envoy_cc_test_library(
    name = "grpc_subscription_test_harness",
    hdrs = ["grpc_subscription_test_harness.h"],
    external_deps = ["envoy_eds"],
    deps = [
        ":subscription_test_harness",
        "//source/common/common:hash_lib",
        "//source/common/config:grpc_subscription_lib",
        "//source/common/config:resources_lib",
        "//test/mocks/config:config_mocks",
        "//test/mocks/event:event_mocks",
        "//test/mocks/grpc:grpc_mocks",
        "//test/mocks/upstream:upstream_mocks",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "http_subscription_impl_test",
    srcs = ["http_subscription_impl_test.cc"],
    deps = [
        ":http_subscription_test_harness",
    ],
)

envoy_cc_test_library(
    name = "http_subscription_test_harness",
    srcs = ["http_subscription_test_harness.h"],
    external_deps = ["envoy_eds"],
    deps = [
        ":subscription_test_harness",
        "//include/envoy/http:async_client_interface",
        "//source/common/common:utility_lib",
        "//source/common/config:http_subscription_lib",
        "//source/common/config:utility_lib",
        "//source/common/http:message_lib",
        "//test/mocks/config:config_mocks",
        "//test/mocks/event:event_mocks",
        "//test/mocks/runtime:runtime_mocks",
        "//test/mocks/upstream:upstream_mocks",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "subscription_factory_test",
    srcs = ["subscription_factory_test.cc"],
    external_deps = ["envoy_eds"],
    deps = [
        "//source/common/config:subscription_factory_lib",
        "//test/mocks/config:config_mocks",
        "//test/mocks/event:event_mocks",
        "//test/mocks/filesystem:filesystem_mocks",
        "//test/mocks/runtime:runtime_mocks",
        "//test/mocks/stats:stats_mocks",
        "//test/mocks/upstream:upstream_mocks",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "subscription_impl_test",
    srcs = ["subscription_impl_test.cc"],
    deps = [
        ":filesystem_subscription_test_harness",
        ":grpc_subscription_test_harness",
        ":http_subscription_test_harness",
        ":subscription_test_harness",
    ],
)

envoy_cc_test_library(
    name = "subscription_test_harness",
    srcs = ["subscription_test_harness.h"],
    deps = [
        "//source/common/config:utility_lib",
        "//test/mocks/stats:stats_mocks",
    ],
)

envoy_cc_test(
    name = "metadata_test",
    srcs = ["metadata_test.cc"],
    deps = [
        "//source/common/config:metadata_lib",
        "//source/common/config:well_known_names",
    ],
)

envoy_cc_test(
    name = "utility_test",
    srcs = ["utility_test.cc"],
    external_deps = ["envoy_eds"],
    deps = [
        "//source/common/config:cds_json_lib",
        "//source/common/config:lds_json_lib",
        "//source/common/config:rds_json_lib",
        "//source/common/config:utility_lib",
        "//source/common/stats:stats_lib",
        "//test/mocks/local_info:local_info_mocks",
        "//test/test_common:utility_lib",
    ],
)
