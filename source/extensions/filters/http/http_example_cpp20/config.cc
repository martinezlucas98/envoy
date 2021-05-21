#include "extensions/filters/http/http_example_cpp20/config.h"

#include "envoy/extensions/filters/http/http_example_cpp20/v3/http_example_cpp20.pb.h"
#include "envoy/extensions/filters/http/http_example_cpp20/v3/http_example_cpp20.pb.validate.h"
#include "envoy/registry/registry.h"

#include "extensions/filters/http/http_example_cpp20/http_example_cpp20_filter.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HttpExampleCpp20 {

Http::FilterFactoryCb HttpExampleCpp20FilterFactory::createFilterFactoryFromProtoTyped(
    const envoy::extensions::filters::http::http_example_cpp20::v3::HttpExampleCpp20& config,
    const std::string& stats_prefix, Server::Configuration::FactoryContext& context) {

  auto filter_config = std::make_shared<FilterConfigImpl>(
      stats_prefix, context.scope(), config.host_rewrite(), config.key(), config.value(), config.associative_container_use_contains(), config.enum_members_in_scope(), config.str_starts_with(), config.str_ends_with());

  return [filter_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    auto filter = std::make_shared<Filter>(filter_config);
    callbacks.addStreamDecoderFilter(filter);
  };
}

/**
 * Static registration for the HTTP example cpp20 filter. @see RegisterFactory.
 */
REGISTER_FACTORY(HttpExampleCpp20FilterFactory,
                 Server::Configuration::NamedHttpFilterConfigFactory);

} // namespace HttpExampleCpp20
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
