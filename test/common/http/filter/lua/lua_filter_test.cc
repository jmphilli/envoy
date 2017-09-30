#include "common/buffer/buffer_impl.h"
#include "common/http/filter/lua/lua_filter.h"

#include "test/test_common/utility.h"

#include "gmock/gmock.h"

namespace Envoy {
namespace Http {
namespace Filter {
namespace Lua {

class LuaHttpFilterTest : public testing::Test {
public:
  void setup(const std::string& lua_code) {
    config_.reset(new FilterConfig(lua_code));
    filter_.reset(new Filter(config_));
  }

  std::shared_ptr<FilterConfig> config_;
  std::unique_ptr<Filter> filter_;
};

TEST_F(LuaHttpFilterTest, FixFix) {
  const std::string code(R"EOF(
    function envoy_on_request(request_handle)
      if headers ~= nil then
        headers:iterate(
          function(key, value)
            request_handle:log(0, string.format("'%s' '%s'", key, value))
          end
        )
      end

      local headers = request_handle:headers()
      headers:iterate(
        function(key, value)
          request_handle:log(0, string.format("'%s' '%s'", key, value))
        end
      )

      for chunk in request_handle:bodyChunks() do
        request_handle:log(0, chunk:byteSize())
      end

      local trailers = request_handle:trailers()
      if trailers ~= nil then
        trailers:iterate(
          function(key, value)
            request_handle:log(0, string.format("'%s' '%s'", key, value))
          end
        )
      end

      request_handle:log(0, "all done!")
    end
  )EOF");

  setup(code);

  TestHeaderMapImpl request_headers{{":path", "/"}};
  filter_->decodeHeaders(request_headers, false);

  Buffer::OwnedImpl data("hello");
  filter_->decodeData(data, true);
  filter_.reset();
  config_->gc();

  Filter filter2(config_);
  filter2.decodeHeaders(request_headers, false);
}

} // namespace Lua
} // namespace Filter
} // namespace Http
} // namespace Envoy
