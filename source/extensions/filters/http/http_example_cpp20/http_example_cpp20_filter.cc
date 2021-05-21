#include "extensions/filters/http/http_example_cpp20/http_example_cpp20_filter.h"

#include "envoy/extensions/filters/http/http_example_cpp20/v3/http_example_cpp20.pb.h"

#include <string>

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HttpExampleCpp20 {


FilterConfigImpl::FilterConfigImpl(const std::string& stats_prefix, Stats::Scope& scope,
                                   const std::string& host_rewrite, const std::string& key,
                                   const std::string& value,
                                   const bool& associative_container_use_contains,
                                   const bool& enum_members_in_scope,
                                   const bool& str_starts_with, const bool& str_ends_with)
    : stats_(Filter::generateStats(stats_prefix, scope)), host_rewrite_(host_rewrite), key_(key),
      val_(value), associative_container_use_contains_(associative_container_use_contains),
      enum_members_in_scope_(enum_members_in_scope), str_starts_with_(str_starts_with), str_ends_with_(str_ends_with) {}

Filter::Filter(const std::shared_ptr<FilterConfig>& config) : config_(config) {}

FilterStats& FilterConfigImpl::stats() { return stats_; }

const std::string& FilterConfigImpl::hostRewrite() const { return host_rewrite_; }
const std::string& FilterConfigImpl::key() const { return key_; }
const std::string& FilterConfigImpl::val() const { return val_; }

const bool& FilterConfigImpl::associativeContainerUseContains() const { return associative_container_use_contains_; }
const bool& FilterConfigImpl::enumMembersInScope() const { return enum_members_in_scope_; }
const bool& FilterConfigImpl::strStartsWith() const { return str_starts_with_; }
const bool& FilterConfigImpl::strEndsWith() const { return str_ends_with_; }

FilterStats Filter::generateStats(const std::string& prefix, Stats::Scope& scope) {
  const std::string final_prefix = prefix + "http_example_cpp20.";
  return {ALL_HTTP_EXAMPLE_CPP20_FILTER_STATS(POOL_COUNTER_PREFIX(scope, final_prefix))};
}

const Http::LowerCaseString Filter::headerKey() const {
  return Http::LowerCaseString(config_->key());
}

const std::string Filter::headerValue() const { return config_->val(); }

Http::FilterHeadersStatus Filter::decodeHeaders(Http::RequestHeaderMap& headers, bool) {



    // add a header
    headers.addCopy(headerKey(), headerValue());


    // C++20 features if enabled
    if (config_->associativeContainerUseContains()){
      std::map<std::string, std::string> map {{"val1", "key1"}, {"val2", "key2"}, {"val3", "key3"}};
      map.contains("val1"); // true
      map.contains("val99"); // false

      std::set<std::string> set {"value1", "value2", "value3", "value4"};
      set.contains("value3"); // true

      headers.addCopy(Http::LowerCaseString("x-cpp20-associative-container-use-contains"), "true");

    }

    #if defined(__cpp_using_enum)
      if (config_->enumMembersInScope()){
        enum class rgba_color_channel { red, green, blue, alpha };

        using enum rgba_color_channel;

        rgba_color_channel my_channel {red};

        switch (my_channel) {

          case red:   std::cout<<"red"<<std::endl;break;
          case green: std::cout<<"green"<<std::endl;break;
          case blue:  std::cout<<"blue"<<std::endl;break;
          case alpha: std::cout<<"alpha"<<std::endl;break;
        }

        headers.addCopy(Http::LowerCaseString("x-cpp20-enum-members-in-scope"), "true");

      }

    #endif

    if (config_->strStartsWith()){
      std::string foo_header_val = "server:latam_FazaNA1vkPlFPyyF4T2F";

      if (foo_header_val.starts_with("server:latam_")) { // true
        // we can do something here
      }
        headers.addCopy(Http::LowerCaseString("x-cpp20-str-starts-with"), "true");
    }

    if (config_->strEndsWith()){
      std::string foo_allowed_token = "bfFw1DCsranQ6x2zZKYGYVc0zqW99UB05IZPuQjv";
      std::string foo_header_val = "ip:127.0.0.1_tk:bfFw1DCsranQ6x2zZKYGYVc0zqW99UB05IZPuQjv";

      if (foo_header_val.ends_with("_tk:"+foo_allowed_token)) { // true
        // we can do something here
      } else {
        // we can do something else here
      }

      headers.addCopy(Http::LowerCaseString("x-cpp20-str-ends-with"), "true");

    }

    config_->stats().signing_added_.inc();


  return Http::FilterHeadersStatus::Continue;
}

} // namespace HttpExampleCpp20
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
