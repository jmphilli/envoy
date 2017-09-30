#pragma once

#include "envoy/http/filter.h"

#include "common/lua/lua.h"

namespace Envoy {
namespace Http {
namespace Filter {
namespace Lua {

/**
 * fixfix
 */
class HeaderMapWrapper : public Envoy::Lua::BaseLuaObject<HeaderMapWrapper> {
public:
  HeaderMapWrapper(HeaderMap& headers) : headers_(headers) {}

  static ExportedFunctions exportedFunctions() { return {{"iterate", static_iterate}}; }

private:
  DECLARE_LUA_FUNCTION(HeaderMapWrapper, iterate);

  HeaderMap& headers_;
};

class Filter;

/**
 * fixfix
 */
class StreamHandleWrapper : public Envoy::Lua::BaseLuaObject<StreamHandleWrapper> {
public:
  enum class State { Headers, WaitForBodyChunk, WaitForTrailers };

  StreamHandleWrapper(Envoy::Lua::CoroutinePtr&& coroutine, HeaderMap& headers, bool end_stream);

  static ExportedFunctions exportedFunctions() {
    return {{"headers", static_headers},
            {"bodyChunks", static_bodyChunks},
            {"trailers", static_trailers},
            {"log", static_log}};
  }

  void onData(Buffer::Instance& data, bool end_stream);

private:
  DECLARE_LUA_FUNCTION(StreamHandleWrapper, headers);
  DECLARE_LUA_FUNCTION(StreamHandleWrapper, bodyChunks);
  DECLARE_LUA_FUNCTION(StreamHandleWrapper, trailers);
  DECLARE_LUA_FUNCTION(StreamHandleWrapper, log);
  DECLARE_LUA_CLOSURE(StreamHandleWrapper, bodyIterator);

  void onMarkDead() override {
    if (headers_wrapper_.get()) {
      headers_wrapper_.get()->markDead();
    }
  }

  // Filter& filter_;
  Envoy::Lua::CoroutinePtr coroutine_;
  Envoy::Lua::LuaRef<HeaderMapWrapper> headers_wrapper_;
  HeaderMap& headers_;
  HeaderMap* trailers_{};
  bool end_stream_;
  State state_{State::Headers};
};

/**
 * fixfix
 */
class FilterConfig {
public:
  FilterConfig(const std::string& lua_code);
  Envoy::Lua::CoroutinePtr createCoroutine() { return lua_state_.createCoroutine(); }
  void gc() { return lua_state_.gc(); }

private:
  Envoy::Lua::ThreadLocalState lua_state_;
};

typedef std::shared_ptr<FilterConfig> FilterConfigConstSharedPtr;

/**
 * fixfix
 */
class Filter : public StreamFilter {
public:
  Filter(FilterConfigConstSharedPtr config) : config_(config) {}

  // Http::StreamFilterBase
  void onDestroy() override {}

  // Http::StreamDecoderFilter
  FilterHeadersStatus decodeHeaders(HeaderMap& headers, bool end_stream) override;
  FilterDataStatus decodeData(Buffer::Instance& data, bool end_stream) override;
  FilterTrailersStatus decodeTrailers(HeaderMap&) override;
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) override {
    decoder_callbacks_ = &callbacks;
  }

  // Http::StreamEncoderFilter
  FilterHeadersStatus encodeHeaders(HeaderMap&, bool) override {
    return FilterHeadersStatus::Continue;
  }
  FilterDataStatus encodeData(Buffer::Instance&, bool) override {
    return FilterDataStatus::Continue;
  };
  FilterTrailersStatus encodeTrailers(HeaderMap&) override {
    return FilterTrailersStatus::Continue;
  };
  void setEncoderFilterCallbacks(StreamEncoderFilterCallbacks& callbacks) override {
    encoder_callbacks_ = &callbacks;
  };

private:
  FilterConfigConstSharedPtr config_;
  StreamDecoderFilterCallbacks* decoder_callbacks_{};
  StreamEncoderFilterCallbacks* encoder_callbacks_{};
  Envoy::Lua::LuaDeathRef<StreamHandleWrapper> request_stream_wrapper_;
};

} // namespace Lua
} // namespace Filter
} // namespace Http
} // namespace Envoy
