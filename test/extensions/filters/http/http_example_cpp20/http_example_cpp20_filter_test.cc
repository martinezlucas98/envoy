#include "extensions/filters/http/http_example_cpp20/http_example_cpp20_filter.h"

#include "test/mocks/http/mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace HttpExampleCpp20Filter {
namespace {

class MockFilterConfig : public HttpExampleCpp20::FilterConfig {
public:
  HttpExampleCpp20::FilterStats& stats() override { return stats_; }
  // const std::string& hostRewrite() const override { return host_rewrite_; }

  const std::string& key() const override { return key_; }
  const std::string& val() const override { return val_; }

  const bool& associativeContainerUseContains() const override {
    return associative_container_use_contains_;
  }
  const bool& enumMembersInScope() const override { return enum_members_in_scope_; }
  const bool& strStartsWith() const override { return str_starts_with_; }
  const bool& strEndsWith() const override { return str_ends_with_; }

  Stats::IsolatedStoreImpl stats_store_;
  HttpExampleCpp20::FilterStats stats_{
      HttpExampleCpp20::Filter::generateStats("test", stats_store_)};
  // std::string host_rewrite_;
  std::string key_;
  std::string val_;

  bool associative_container_use_contains_;
  bool enum_members_in_scope_;
  bool str_starts_with_;
  bool str_ends_with_;
};

class HttpExampleCpp20FilterTest : public testing::Test {
public:
  void setup() {
    filter_config_ = std::make_shared<MockFilterConfig>();
    filter_ = std::make_unique<HttpExampleCpp20::Filter>(filter_config_);
  };

  std::shared_ptr<MockFilterConfig> filter_config_;
  std::unique_ptr<HttpExampleCpp20::Filter> filter_;
};

TEST_F(HttpExampleCpp20FilterTest, RequestWithHeader) {
  setup();
  // filter_config_->host_rewrite_ = "foo";
  filter_config_->key_ = "fooKey";
  filter_config_->val_ = "fooVal";

  const Http::LowerCaseString test_header(filter_config_->key_);
  Http::TestRequestHeaderMapImpl headers; //{{"fooKey", "fooVal"}}
  EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
  const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(headers, test_header);
  EXPECT_EQ("fooVal", ret.result().value());
}

//#if defined(__cpp_lib_generic_unordered_lookup)
#if defined(__cpp_lib_generic_associative_lookup)
TEST_F(HttpExampleCpp20FilterTest, AssociativeContainerUseContains) {
  setup();
  filter_config_->associative_container_use_contains_ = true;
  // EXPECT_CALL(*(filter_config_->associative_container_use_contains_),
  // associativeContainerUseContains());
  // feature is on
  {
    Http::TestRequestHeaderMapImpl headers;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        headers, Http::LowerCaseString("x-cpp20-associative-container-use-contains"));
    EXPECT_TRUE(ret.result().has_value());
    EXPECT_EQ("true", ret.result().value());
  }

  // feature is off
  {
    filter_config_->associative_container_use_contains_ = false;
    Http::TestRequestHeaderMapImpl header;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(header, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        header, Http::LowerCaseString("x-cpp20-associative-container-use-contains"));
    EXPECT_FALSE(ret.result().has_value());
  }
}
#endif
//#endif

#if defined(__cpp_using_enum)
TEST_F(HttpExampleCpp20FilterTest, EnumMembersInScope) {
  setup();

  // feature is on
  {
    filter_config_->enum_members_in_scope_ = true;
    Http::TestRequestHeaderMapImpl headers;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        headers, Http::LowerCaseString("x-cpp20-enum-members-in-scope"));
    EXPECT_TRUE(ret.result().has_value());
    EXPECT_EQ("true", ret.result().value());
  }

  // feature is off
  {
    filter_config_->enum_members_in_scope_ = false;
    Http::TestRequestHeaderMapImpl header;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(header, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        header, Http::LowerCaseString("x-cpp20-enum-members-in-scope"));
    EXPECT_FALSE(ret.result().has_value());
  }
}
#endif

#if defined(__cpp_lib_starts_ends_with)
TEST_F(HttpExampleCpp20FilterTest, StrStartsWith) {
  setup();

  // feature is on
  {
    filter_config_->str_starts_with_ = true;
    Http::TestRequestHeaderMapImpl headers;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        headers, Http::LowerCaseString("x-cpp20-str-starts-with"));
    EXPECT_TRUE(ret.result().has_value());
    EXPECT_EQ("true", ret.result().value());
  }

  // feature is off
  {
    filter_config_->str_starts_with_ = false;
    Http::TestRequestHeaderMapImpl headers;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        headers, Http::LowerCaseString("x-cpp20-str-starts-with"));
    EXPECT_FALSE(ret.result().has_value());
  }
}

TEST_F(HttpExampleCpp20FilterTest, StrEndsWith) {
  setup();

  // feature is on
  {
    filter_config_->str_ends_with_ = true;
    Http::TestRequestHeaderMapImpl headers;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        headers, Http::LowerCaseString("x-cpp20-str-ends-with"));
    EXPECT_TRUE(ret.result().has_value());
    EXPECT_EQ("true", ret.result().value());
  }

  // feature is off
  {
    filter_config_->str_ends_with_ = false;
    Http::TestRequestHeaderMapImpl header;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(header, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        header, Http::LowerCaseString("x-cpp20-str-ends-with"));
    EXPECT_FALSE(ret.result().has_value());
  }
}
#endif

} // namespace
} // namespace HttpExampleCpp20Filter
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
