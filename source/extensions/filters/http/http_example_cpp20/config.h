#pragma once

//#include "envoy/extensions/filters/http/aws_request_signing/v3/aws_request_signing.pb.h"
//#include "envoy/extensions/filters/http/aws_request_signing/v3/aws_request_signing.pb.validate.h"
#include "envoy/extensions/filters/http/http_example_cpp20/v3/http_example_cpp20.pb.h"
#include "envoy/extensions/filters/http/http_example_cpp20/v3/http_example_cpp20.pb.validate.h"

#include "extensions/filters/http/common/factory_base.h"
#include "extensions/filters/http/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HttpExampleCpp20 {

/**
 * Config registration for the AWS request signing filter.
 */
class HttpExampleCpp20FilterFactory
    : public Common::FactoryBase<
          envoy::extensions::filters::http::http_example_cpp20::v3::HttpExampleCpp20> {
public:
  HttpExampleCpp20FilterFactory() : FactoryBase(HttpFilterNames::get().HttpExampleCpp20) {}

private:
  Http::FilterFactoryCb createFilterFactoryFromProtoTyped(
      const envoy::extensions::filters::http::http_example_cpp20::v3::HttpExampleCpp20&
          proto_config,
      const std::string& stats_prefix, Server::Configuration::FactoryContext& context) override;
};

} // namespace AwsRequestSigningFilter
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
