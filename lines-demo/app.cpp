#include "app.hpp"

static bool _done = false;

bool done() noexcept {
  return _done;
}

void quit() noexcept {
  _done = true;
}

namespace lua {

default_state state{};

void eval(std::string_view str) {
  // ++lua_level;
  const auto result = state.safe_script(str, sol::script_pass_on_error);
  // --lua_level;
  if (not result.valid())
    std::println("ERROR:\n{}\n", sol::error{result}.what());
  // waiting = false;
}

void eval_file(std::filesystem::path const& path) {
  // scoped_chdir _{path.parent_path()};
  // ++lua_level;
  const auto result = state.safe_script_file(path, sol::script_pass_on_error);
  // --lua_level;
  if (not result.valid())
    std::println("ERROR:\n{}\n", sol::error{result}.what());
  // waiting = false;
}

}  // namespace lua
