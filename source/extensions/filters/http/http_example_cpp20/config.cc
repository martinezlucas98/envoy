#include "extensions/filters/http/http_example_cpp20/config.h"

//#include "envoy/extensions/filters/http/aws_request_signing/v3/aws_request_signing.pb.h"
//#include "envoy/extensions/filters/http/aws_request_signing/v3/aws_request_signing.pb.validate.h" // api/*
#include "envoy/extensions/filters/http/http_example_cpp20/v3/http_example_cpp20.pb.h"
#include "envoy/extensions/filters/http/http_example_cpp20/v3/http_example_cpp20.pb.validate.h" // api/*
#include "envoy/registry/registry.h"

#include "extensions/common/aws/credentials_provider_impl.h"
#include "extensions/common/aws/utility.h"
#include "extensions/filters/http/http_example_cpp20/http_example_cpp20_filter.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HttpExampleCpp20 {

Http::FilterFactoryCb HttpExampleCpp20FilterFactory::createFilterFactoryFromProtoTyped(
    const envoy::extensions::filters::http::http_example_cpp20::v3::HttpExampleCpp20& config,
    const std::string& stats_prefix, Server::Configuration::FactoryContext& context) {

  auto filter_config = std::make_shared<FilterConfigImpl>(stats_prefix,context.scope(),
                                                          config.host_rewrite(),
                                                          config.key(), config.value());

  //auto filter_config = std::make_shared<FilterConfigImpl>(config.key(), config.value());

  return [filter_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    auto filter = std::make_shared<Filter>(filter_config);
    callbacks.addStreamDecoderFilter(filter);
  };
}

/**
 * Static registration for the AWS request signing filter. @see RegisterFactory.
 */
REGISTER_FACTORY(HttpExampleCpp20FilterFactory,
                 Server::Configuration::NamedHttpFilterConfigFactory);

} // namespace AwsRequestSigningFilter
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
