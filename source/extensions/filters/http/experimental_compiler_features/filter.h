#pragma once

#include <string>

#include "envoy/extensions/filters/http/experimental_compiler_features/v3/experimental_compiler_features.pb.h"
#include "envoy/http/filter.h"
#include "envoy/stats/scope.h"
#include "envoy/stats/stats_macros.h"

#include "extensions/filters/http/common/pass_through_filter.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace ExperimentalCompilerFeatures {

// clang-format off
#define ALL_experimental_compiler_features_FILTER_STATS(COUNTER)                                                           \
  COUNTER(signing_added)                                                                        \
  COUNTER(signing_failed)
// clang-format on

/**
 * Wrapper struct filter stats. @see stats_macros.h
 */
struct FilterStats {
  ALL_experimental_compiler_features_FILTER_STATS(GENERATE_COUNTER_STRUCT)
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
  virtual const std::string& key() const PURE;
  virtual const std::string& val() const PURE;

  virtual const bool& associativeContainerUseContains() const PURE;
  virtual const bool& enumMembersInScope() const PURE;
  virtual const bool& strStartsWith() const PURE;
  virtual const bool& strEndsWith() const PURE;

  // These vars are used for testing only.
  virtual const std::string& enumValue() const PURE;
};

using FilterConfigSharedPtr = std::shared_ptr<FilterConfig>;

/**
 * Configuration for the HTTP Example cpp20 filter.
 */
class FilterConfigImpl : public FilterConfig {
public:
  FilterConfigImpl(const std::string& stats_prefix, Stats::Scope& scope, const std::string& key,
                   const std::string& value, const bool& associative_container_use_contains,
                   const bool& enum_members_in_scope, const bool& str_starts_with,
                   const bool& str_ends_with, const std::string enum_value);

  FilterStats& stats() override;

  // Basic key-value header variables
  const std::string& key() const override;
  const std::string& val() const override;

  // Variables to enable C++20 features
  const bool& associativeContainerUseContains() const override;
  const bool& enumMembersInScope() const override;
  const bool& strStartsWith() const override;
  const bool& strEndsWith() const override;

  // These vars are used for testing only.
  const std::string& enumValue() const override;

private:
  FilterStats stats_;

  std::string key_;
  std::string val_;

  bool associative_container_use_contains_;
  bool enum_members_in_scope_;
  bool str_starts_with_;
  bool str_ends_with_;

  // These vars are used for testing only.
  std::string enum_value_;
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
  friend class FilterTest;
  // These vars are used for testing only.
  std::string enum_value_{"foo"};

  std::shared_ptr<FilterConfig> config_;
  const Http::LowerCaseString headerKey() const;
  const std::string headerValue() const;
};

} // namespace ExperimentalCompilerFeatures
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
