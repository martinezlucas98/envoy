#pragma once

#include "envoy/common/optref.h"
#include "envoy/config/typed_config.h"

namespace Envoy {
namespace Http {

/**
 * fixfix
 */
class HeaderKeyFormatter {
public:
  virtual ~HeaderKeyFormatter() = default;

  /**
   * fixfix
   */
  virtual std::string format(absl::string_view key) const PURE;
};

using HeaderKeyFormatterConstPtr = std::unique_ptr<const HeaderKeyFormatter>;
using HeaderKeyFormatterOptConstRef = OptRef<const HeaderKeyFormatter>;

/**
 * fixfix
 */
class StatefulHeaderKeyFormatter : public HeaderKeyFormatter {
public:
  /**
   * fixfix
   */
  virtual void rememberOriginalReceivedHeader(absl::string_view key, absl::string_view value) PURE;
};

using StatefulHeaderKeyFormatterPtr = std::unique_ptr<StatefulHeaderKeyFormatter>;
using StatefulHeaderKeyFormatterOptRef = OptRef<StatefulHeaderKeyFormatter>;

/**
 * fixfix
 */
class StatefulHeaderKeyFormatterFactory {
public:
  virtual ~StatefulHeaderKeyFormatterFactory() = default;

  /**
   * fixfix
   */
  virtual StatefulHeaderKeyFormatterPtr create() PURE;
};

using StatefulHeaderKeyFormatterFactoryPtr = std::unique_ptr<StatefulHeaderKeyFormatterFactory>;

/**
 * fixfix
 */
class StatefulHeaderKeyFormatterFactoryConfig : public Config::TypedFactory {
public:
  virtual StatefulHeaderKeyFormatterFactoryPtr
  createFromProto(const Protobuf::Message& config) PURE;

  std::string category() const override { return "envoy.http.header_formatter"; }
};

} // namespace Http
} // namespace Envoy