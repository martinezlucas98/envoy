#include "envoy/extensions/filters/http/http_example_cpp20/v3/http_example_cpp20.pb.h"
#include "envoy/extensions/filters/http/http_example_cpp20/v3/http_example_cpp20.pb.validate.h"

#include "extensions/filters/http/http_example_cpp20/config.h"

#include "test/mocks/server/factory_context.h"
#include "test/test_common/utility.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HttpExampleCpp20Filter {

using HttpExampleCpp20ProtoConfig =
    envoy::extensions::filters::http::http_example_cpp20::v3::HttpExampleCpp20;

TEST(HttpExampleCpp20FilterConfigTest, SimpleConfig) {
  const std::string yaml = R"EOF(
service_name: s3
region: us-west-2
key: fooKey
value: fooValue
associative_container_use_contains: true
enum_members_in_scope: true
str_starts_with: true
str_ends_with: true
  )EOF";

  HttpExampleCpp20ProtoConfig proto_config;
  TestUtility::loadFromYamlAndValidate(yaml, proto_config);

  testing::NiceMock<Server::Configuration::MockFactoryContext> context;
  HttpExampleCpp20::HttpExampleCpp20FilterFactory factory;

  Http::FilterFactoryCb cb = factory.createFilterFactoryFromProto(proto_config, "stats", context);
  Http::MockFilterChainFactoryCallbacks filter_callbacks;
  EXPECT_CALL(filter_callbacks, addStreamDecoderFilter(_));
  cb(filter_callbacks);
}

} // namespace HttpExampleCpp20Filter
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
