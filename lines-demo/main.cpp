#include <sandbox/lua.hpp>
import sandbox;
import console;

using namespace sandbox;

#ifdef NDEBUG
#define debug(...) ((void)0)
#else
#define debug(...) ::console::log::debug(__VA_ARGS__)
#endif

int main(int argc, char* argv[]) {
  console::log::text("Some simple text!");
  debug("Some debug output!");
  console::log::info("Lines Demo");
  console::log::warn("This is a warning");
  console::log::error("An error!");

  lua::live_state this_lua{};
  for (int i = 1; i < argc; ++i) this_lua.eval_file(argv[i]);
  while (not application::done()) {
    this_lua.update();
  }
}
