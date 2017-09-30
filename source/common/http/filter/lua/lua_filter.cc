#include "common/http/filter/lua/lua_filter.h"

#include "common/common/assert.h"
#include "common/lua/wrappers.h"

namespace Envoy {
namespace Http {
namespace Filter {
namespace Lua {

int HeaderMapWrapper::iterate(lua_State* state) {
  luaL_checktype(state, 2, LUA_TFUNCTION);
  headers_.iterate(
      [](const HeaderEntry& header, void* context) -> void {
        // fixfix
        lua_State* state = static_cast<lua_State*>(context);
        lua_pushvalue(state, -1);
        lua_pushstring(state, header.key().c_str());
        lua_pushstring(state, header.value().c_str());
        lua_pcall(state, 2, 0, 0);
      },
      state);

  return 0;
}

StreamHandleWrapper::StreamHandleWrapper(Envoy::Lua::CoroutinePtr&& coroutine, HeaderMap& headers,
                                         bool end_stream)
    : coroutine_(std::move(coroutine)), headers_(headers), end_stream_(end_stream) {
  // We are on the top of the stack.
  coroutine_->start("envoy_on_request", 1);
}

int StreamHandleWrapper::headers(lua_State* state) {
  if (headers_wrapper_.get() != nullptr) {
    headers_wrapper_.pushStack();
  } else {
    headers_wrapper_.reset(HeaderMapWrapper::create(state, headers_), true);
  }
  return 1;
}

int StreamHandleWrapper::bodyChunks(lua_State* state) {
  if (state_ != State::Headers) {
    ASSERT(false); // fixfix
  }

  state_ = State::WaitForBodyChunk;

  // fixfix
  lua_pushcclosure(state, static_bodyIterator, 1);
  return 1;
}

int StreamHandleWrapper::bodyIterator(lua_State* state) {
  if (state_ != State::WaitForBodyChunk) {
    ASSERT(false); // fixfix
  }

  if (end_stream_) {
    ENVOY_LOG(debug, "end stream. no more body chunks");
    return 0;
  } else {
    ENVOY_LOG(debug, "yielding for next body chunk");
    return lua_yield(state, 0);
  }
}

int StreamHandleWrapper::trailers(lua_State* state) {
  // if (state_ != State::Headers) {
  //  ASSERT(false); // fixfix
  //}

  if (end_stream_ && trailers_ == nullptr) {
    ENVOY_LOG(debug, "end stream. no trailers");
    return 0;
  } else if (trailers_ != nullptr) {
    ASSERT(false); // fixfix
  } else {
    ENVOY_LOG(debug, "yielding for next body chunk");
    return lua_yield(state, 0);
  }

  return 0;
}

int StreamHandleWrapper::log(lua_State* state) {
  int level = luaL_checkint(state, 2);
  const char* message = luaL_checkstring(state, 3);
  // fixfix levels
  switch (level) {
  default: {
    ENVOY_LOG(debug, "script log: {}", message);
    break;
  }
  }

  return 0;
}

void StreamHandleWrapper::onData(Buffer::Instance& data, bool end_stream) {
  end_stream_ = end_stream;
  if (coroutine_->state() == Envoy::Lua::Coroutine::State::Finished) {
    ASSERT(false); // fixfix
  }

  ASSERT(coroutine_->state() == Envoy::Lua::Coroutine::State::Yielded);

  // fixfix lifetime.
  ENVOY_LOG(debug, "resuming for next body chunk");
  Envoy::Lua::LuaDeathRef<Envoy::Lua::BufferWrapper> wrapper(
      Envoy::Lua::BufferWrapper::create(coroutine_->luaState(), data), true);
  coroutine_->resume(1);
}

FilterConfig::FilterConfig(const std::string& lua_code) : lua_state_(lua_code) {
  Envoy::Lua::BufferWrapper::registerType(lua_state_.state());
  HeaderMapWrapper::registerType(lua_state_.state());
  StreamHandleWrapper::registerType(lua_state_.state());
}

FilterHeadersStatus Filter::decodeHeaders(HeaderMap& headers, bool end_stream) {
  Envoy::Lua::CoroutinePtr coroutine = config_->createCoroutine();
  request_stream_wrapper_.reset(
      StreamHandleWrapper::create(coroutine->luaState(), std::move(coroutine), headers, end_stream),
      true);

  // fixfix error handling
  // fixfix unexpected yield

  return FilterHeadersStatus::Continue;
}

FilterDataStatus Filter::decodeData(Buffer::Instance& data, bool end_stream) {
  request_stream_wrapper_.get()->onData(data, end_stream);
  return FilterDataStatus::Continue;
}

FilterTrailersStatus Filter::decodeTrailers(HeaderMap&) { return FilterTrailersStatus::Continue; }

} // namespace Lua
} // namespace Filter
} // namespace Http
} // namespace Envoy
