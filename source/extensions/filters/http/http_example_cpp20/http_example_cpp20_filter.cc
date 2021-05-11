#include "extensions/filters/http/http_example_cpp20/http_example_cpp20_filter.h"

//#include "envoy/extensions/filters/http/aws_request_signing/v3/aws_request_signing.pb.h"
#include "envoy/extensions/filters/http/http_example_cpp20/v3/http_example_cpp20.pb.h"

#include <string>

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HttpExampleCpp20 {

FilterConfigImpl::FilterConfigImpl(const std::string& stats_prefix, Stats::Scope& scope,
                                   const std::string& host_rewrite,
                                   const std::string& key, const std::string& value)
    : stats_(Filter::generateStats(stats_prefix, scope)),
      host_rewrite_(host_rewrite), key_(key), val_(value) {}

// FilterConfig::FilterConfig(const sample::Decoder& proto_config)
// : key_(proto_config.key()), val_(proto_config.val()) {}

Filter::Filter(const std::shared_ptr<FilterConfig>& config) : config_(config) {}

FilterStats& FilterConfigImpl::stats() { return stats_; }

const std::string& FilterConfigImpl::hostRewrite() const { return host_rewrite_; }
const std::string& FilterConfigImpl::key() const { return key_; }
const std::string& FilterConfigImpl::val() const { return val_; }

FilterStats Filter::generateStats(const std::string& prefix, Stats::Scope& scope) {
  const std::string final_prefix = prefix + "http_example_cpp20.";
  return {ALL_AWS_REQUEST_SIGNING_FILTER_STATS(POOL_COUNTER_PREFIX(scope, final_prefix))};
}

const Http::LowerCaseString Filter::headerKey() const {
  return Http::LowerCaseString(config_->key());
}

const std::string Filter::headerValue() const { return config_->val(); }

Http::FilterHeadersStatus Filter::decodeHeaders(Http::RequestHeaderMap& headers, bool) {
  const auto& host_rewrite = config_->hostRewrite();
  if (!host_rewrite.empty()) {
    headers.setHost(host_rewrite);
  }

  try {
    // add a header
    headers.addCopy(headerKey(), headerValue());

    config_->stats().signing_added_.inc();
  } catch (const EnvoyException& e) {
    ENVOY_LOG(debug, "signing failed: {}", e.what());
    config_->stats().signing_failed_.inc();
  }

  return Http::FilterHeadersStatus::Continue;
}

} // namespace AwsRequestSigningFilter
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
