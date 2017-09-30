#pragma once

#include "envoy/buffer/buffer.h"

#include "common/lua/lua.h"

namespace Envoy {
namespace Lua {

/**
 * fixfix
 */
class BufferWrapper : public BaseLuaObject<BufferWrapper> {
public:
  BufferWrapper(Buffer::Instance& data) : data_(data) {}

  static ExportedFunctions exportedFunctions() { return {{"byteSize", static_byteSize}}; }

private:
  DECLARE_LUA_FUNCTION(BufferWrapper, byteSize);

  Buffer::Instance& data_;
};

} // namespace Lua
} // namespace Envoy
