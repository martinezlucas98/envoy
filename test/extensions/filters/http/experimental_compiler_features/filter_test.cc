#include "extensions/filters/http/experimental_compiler_features/filter.h"

#include "test/mocks/http/mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace ExperimentalCompilerFeatures {
// namespace {

class MockFilterConfig : public ExperimentalCompilerFeatures::FilterConfig {
public:
  ExperimentalCompilerFeatures::FilterStats& stats() override { return stats_; }

  const std::string& key() const override { return key_; }
  const std::string& val() const override { return val_; }

  const bool& associativeContainerUseContains() const override {
    return associative_container_use_contains_;
  }
  const bool& enumMembersInScope() const override { return enum_members_in_scope_; }
  const bool& strStartsWith() const override { return str_starts_with_; }
  const bool& strEndsWith() const override { return str_ends_with_; }

  Stats::IsolatedStoreImpl stats_store_;
  ExperimentalCompilerFeatures::FilterStats stats_{
      ExperimentalCompilerFeatures::Filter::generateStats("test", stats_store_)};

  std::string key_;
  std::string val_;

  bool associative_container_use_contains_;
  bool enum_members_in_scope_;
  bool str_starts_with_;
  bool str_ends_with_;

  // These vars are used for testing only.
  const std::string& enumValue() const override { return enum_value_; }
  std::string enum_value_;
};

class FilterTest : public testing::Test {
public:
  void setup() {
    filter_config_ = std::make_shared<MockFilterConfig>();
    filter_ = std::make_unique<ExperimentalCompilerFeatures::Filter>(filter_config_);
  };

  void set_enum_value(std::string val) { filter_->enum_value_ = val; };

  std::string get_enum_value() { return (filter_->enum_value_); };

  std::shared_ptr<MockFilterConfig> filter_config_;
  std::unique_ptr<ExperimentalCompilerFeatures::Filter> filter_;
};

TEST_F(FilterTest, RequestWithHeader) {
  setup();

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
TEST_F(FilterTest, AssociativeContainerUseContains) {
  // setup();
  // filter_config_->associative_container_use_contains_ = true;
  // EXPECT_CALL(*(filter_config_->associative_container_use_contains_),
  // associativeContainerUseContains());

  // feature is on
  {
    setup();
    filter_config_->associative_container_use_contains_ = true;
    Http::TestRequestHeaderMapImpl headers;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        headers, Http::LowerCaseString("x-cpp20-associative-container-use-contains"));
    EXPECT_TRUE(ret.result().has_value());
    EXPECT_EQ("true", ret.result().value());
  }

  // feature is off
  {
    setup();
    filter_config_->associative_container_use_contains_ = false;
    Http::TestRequestHeaderMapImpl header;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(header, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        header, Http::LowerCaseString("x-cpp20-associative-container-use-contains"));
    EXPECT_FALSE(ret.result().has_value());
  }
}
#endif

#if defined(__cpp_using_enum)
TEST_F(FilterTest, EnumMembersInScope) {

  // feature is on
  // default enum_value = foo which is not in the enum class {red, green, blue, alpha}
  {
    setup();
    filter_config_->enum_value_ = get_enum_value();
    filter_config_->associative_container_use_contains_ = true;
    Http::TestRequestHeaderMapImpl headers;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        headers, Http::LowerCaseString("x-cpp20-enum-members-in-scope"));
    EXPECT_TRUE(ret.result().has_value());
    EXPECT_EQ("invalid enum value", ret.result().value());
  }

  // test for enum_value = red
  {
    setup();
    set_enum_value("red");
    filter_config_->enum_value_ = get_enum_value();
    filter_config_->associative_container_use_contains_ = true;
    Http::TestRequestHeaderMapImpl headers;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        headers, Http::LowerCaseString("x-cpp20-enum-members-in-scope"));
    EXPECT_TRUE(ret.result().has_value());
    EXPECT_EQ("red", ret.result().value());
  }

  // test for enum_value = green
  {
    setup();
    set_enum_value("green");
    filter_config_->enum_value_ = get_enum_value();
    filter_config_->associative_container_use_contains_ = true;
    Http::TestRequestHeaderMapImpl headers;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        headers, Http::LowerCaseString("x-cpp20-enum-members-in-scope"));
    EXPECT_TRUE(ret.result().has_value());
    EXPECT_EQ("green", ret.result().value());
  }

  // test for enum_value = blue
  {
    setup();
    set_enum_value("blue");
    filter_config_->enum_value_ = get_enum_value();
    filter_config_->associative_container_use_contains_ = true;
    Http::TestRequestHeaderMapImpl headers;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        headers, Http::LowerCaseString("x-cpp20-enum-members-in-scope"));
    EXPECT_TRUE(ret.result().has_value());
    EXPECT_EQ("blue", ret.result().value());
  }

  // test for enum_value = alpha
  {
    setup();
    set_enum_value("alpha");
    filter_config_->enum_value_ = get_enum_value();
    filter_config_->associative_container_use_contains_ = true;
    Http::TestRequestHeaderMapImpl headers;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        headers, Http::LowerCaseString("x-cpp20-enum-members-in-scope"));
    EXPECT_TRUE(ret.result().has_value());
    EXPECT_EQ("alpha", ret.result().value());
  }

  // feature is off
  {
    setup();
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
TEST_F(FilterTest, StrStartsWith) {

  // feature is on
  {
    setup();
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
    setup();
    filter_config_->str_starts_with_ = false;
    Http::TestRequestHeaderMapImpl headers;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        headers, Http::LowerCaseString("x-cpp20-str-starts-with"));
    EXPECT_FALSE(ret.result().has_value());
  }
}

TEST_F(FilterTest, StrEndsWith) {

  // feature is on
  {
    setup();
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
    setup();
    filter_config_->str_ends_with_ = false;
    Http::TestRequestHeaderMapImpl header;
    EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(header, false));
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(
        header, Http::LowerCaseString("x-cpp20-str-ends-with"));
    EXPECT_FALSE(ret.result().has_value());
  }
}
#endif

//} // namespace
} // namespace ExperimentalCompilerFeatures
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
