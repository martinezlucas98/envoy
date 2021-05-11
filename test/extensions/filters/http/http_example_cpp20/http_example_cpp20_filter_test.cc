#include "extensions/common/aws/signer.h"
#include "extensions/filters/http/http_example_cpp20/http_example_cpp20_filter.h"

#include "test/extensions/common/aws/mocks.h"
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
  //MockFilterConfig() { signer_ = std::make_shared<Common::Aws::MockSigner>(); }

  //Common::Aws::Signer& signer() override { return *signer_; }
  HttpExampleCpp20::FilterStats& stats() override { return stats_; }
  const std::string& hostRewrite() const override { return host_rewrite_; }

  const std::string& key() const override { return key_; }
  const std::string& val() const override { return val_; }

  //std::shared_ptr<Common::Aws::MockSigner> signer_;
  Stats::IsolatedStoreImpl stats_store_;
  HttpExampleCpp20::FilterStats stats_{HttpExampleCpp20::Filter::generateStats("test", stats_store_)};
  std::string host_rewrite_;
  std::string key_;
  std::string val_;
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



TEST_F(HttpExampleCpp20FilterTest, GETWithHeader){
setup();
  filter_config_->host_rewrite_ = "foo";
  filter_config_->key_ = "fooKey";
  filter_config_->val_ = "fooVal";
  //EXPECT_CALL(*(filter_config_->signer_), sign(_));

  /*Http::TestRequestHeaderMapImpl headers;
  EXPECT_EQ(Http::FilterHeadersStatus::Continue, filter_->decodeHeaders(headers, false));
  EXPECT_EQ("foo", headers.getHostValue());*/
  //EXPECT_EQ(1UL, filter_config_->stats_.signing_added_.value());
  const Http::LowerCaseString test_header(filter_config_->key_);
  Http::TestRequestHeaderMapImpl headers{
        {"fooKey","fooVal"}};
    const auto ret = Http::HeaderUtility::getAllOfHeaderAsString(headers, test_header);
    EXPECT_EQ("fooVal", ret.result().value());
    //EXPECT_EQ("fooVal", ret.backingString());




}

} // namespace
} // namespace HttpExampleCpp20Filter
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
