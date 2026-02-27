#include <sandbox/lua.hpp>
import sandbox;

using namespace sandbox;

int main(int argc, char* argv[]) {
  lua::live_state this_lua{};
  for (int i = 1; i < argc; ++i) this_lua.eval_file(argv[i]);
  while (not application::done()) {
    this_lua.update();
  }
}
