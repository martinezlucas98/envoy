#pragma once

//#include "envoy/extensions/filters/http/aws_request_signing/v3/aws_request_signing.pb.h"
#include <string>

#include "envoy/extensions/filters/http/http_example_cpp20/v3/http_example_cpp20.pb.h"
#include "envoy/http/filter.h"
#include "envoy/stats/scope.h"
#include "envoy/stats/stats_macros.h"

#include "extensions/filters/http/common/pass_through_filter.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HttpExampleCpp20 {

/**
 * All stats for the AWS request signing filter. @see stats_macros.h
 */
// clang-format off
#define ALL_HTTP_EXAMPLE_CPP20_FILTER_STATS(COUNTER)                                                           \
  COUNTER(signing_added)                                                                        \
  COUNTER(signing_failed)
// clang-format on

/**
 * Wrapper struct filter stats. @see stats_macros.h
 */
struct FilterStats {
  ALL_HTTP_EXAMPLE_CPP20_FILTER_STATS(GENERATE_COUNTER_STRUCT)
};

/**
 * Abstract filter configuration.
 */
class FilterConfig {
public:
  virtual ~FilterConfig() = default;

  /**
   * @return the filter stats.
   */
  virtual FilterStats& stats() PURE;

  /**
   * @return the host rewrite value.
   */
  //virtual const std::string& hostRewrite() const PURE;

  // FilterConfig(const sample::Decoder& proto_config);
  virtual const std::string& key() const PURE;
  virtual const std::string& val() const PURE;

  virtual const bool& associativeContainerUseContains() const PURE;
  virtual const bool& enumMembersInScope() const PURE;
  virtual const bool& strStartsWith() const PURE;
  virtual const bool& strEndsWith() const PURE;
};

using FilterConfigSharedPtr = std::shared_ptr<FilterConfig>;

/**
 * Configuration for the HTTP Example cpp20 filter.
 */
class FilterConfigImpl : public FilterConfig {
public:
  FilterConfigImpl(const std::string& stats_prefix, Stats::Scope& scope,
                   /*const std::string& host_rewrite,*/ const std::string& key,
                   const std::string& value, const bool& associative_container_use_contains,
                   const bool& enum_members_in_scope, const bool& str_starts_with,
                   const bool& str_ends_with);

  FilterStats& stats() override;
  //const std::string& hostRewrite() const override;

  // Header variables
  const std::string& key() const override;
  const std::string& val() const override;

  // Variables to enable C++20 features
  const bool& associativeContainerUseContains() const override;
  const bool& enumMembersInScope() const override;
  const bool& strStartsWith() const override;
  const bool& strEndsWith() const override;

private:
  FilterStats stats_;
  //std::string host_rewrite_;

  std::string key_;
  std::string val_;

  bool associative_container_use_contains_;
  bool enum_members_in_scope_;
  bool str_starts_with_;
  bool str_ends_with_;
};

/**
 * HTTP Example cpp20 auth filter.
 */
class Filter : public Http::PassThroughDecoderFilter, Logger::Loggable<Logger::Id::filter> {
public:
  Filter(const std::shared_ptr<FilterConfig>& config);

  static FilterStats generateStats(const std::string& prefix, Stats::Scope& scope);

  Http::FilterHeadersStatus decodeHeaders(Http::RequestHeaderMap& headers,
                                          bool end_stream) override;

private:
  std::shared_ptr<FilterConfig> config_;
  const Http::LowerCaseString headerKey() const;
  const std::string headerValue() const;
};

} // namespace HttpExampleCpp20
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
