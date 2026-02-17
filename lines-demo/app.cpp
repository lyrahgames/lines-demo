#include "app.hpp"

// Variable used to indicate request to quit the application.
static bool _done = false;

bool done() noexcept {
  return _done;
}

void quit() noexcept {
  _done = true;
}

namespace lua {

// Implementation of the global Lua state variable uses
// the default constructor of the `default_state` structure.
default_state state{};

// Static ("private") Variables
namespace {
int _level = 0;       // Current Lua Eval Recursion Level
int _max_level = 32;  // Maximum Lua Eval Recursion Level
// bool waiting = false;
}  // namespace

auto level() noexcept -> int {
  return _level;
}

auto max_level() noexcept -> int {
  return _max_level;
}

void set_max_level(int level) {
  _max_level = level;
}

void eval(std::string_view str) {
  if (level() >= max_level()) {
    std::println(
        "ERROR:\nMaximum level ({}) of Lua recursion reached. Consider raising "
        "the maximum allowed level.\n",
        max_level());
    return;
  }
  ++_level;
  const auto result = state.safe_script(str, sol::script_pass_on_error);
  --_level;
  if (not result.valid())
    std::println("ERROR:\n{}\n", sol::error{result}.what());
  // waiting = false;
}

void eval_file(std::filesystem::path const& path) {
  if (level() >= max_level()) {
    std::println(
        "ERROR:\nMaximum level ({}) of Lua recursion reached. Consider raising "
        "the maximum allowed level.\n",
        max_level());
    return;
  }
  // Build absolute path first to prevent empty strings that lead to exceptions.
  xstd::scoped_chdir _{absolute(path).parent_path()};
  ++_level;
  const auto result = state.safe_script_file(path, sol::script_pass_on_error);
  --_level;
  if (not result.valid())
    std::println("ERROR:\n{}\n", sol::error{result}.what());
  // waiting = false;
}

namespace live {

static std::vector<std::filesystem::path> _paths{};

void listen(xstd::fdm::address const& domain) {
  auto msg = xstd::fdm::recv(domain);
  if (not msg) return;
  std::println("\n{}:\n---\n{}---", proximate(domain).string(), msg.value());
  xstd::scoped_chdir _{domain.parent_path()};
  eval(std::move(msg).value());
}

void watch() {
  for (auto const& path : _paths) {
    if (not is_directory(path)) {
      listen(path);
      continue;
    }
    for (auto const& entry :
         std::filesystem::recursive_directory_iterator(path)) {
      if (not entry.is_regular_file()) continue;
      if (entry.path().extension() != ".lua") continue;
      listen(entry.path());
    }
  }
}

}  // namespace live

}  // namespace lua

auto basic_lua_module(sol::table state) -> sol::table {
  state["done"] = done;
  state["quit"] = quit;
  return state;
}

auto default_lua_module(sol::table state) -> sol::table {
  auto table = basic_lua_module(state);
  lua::default_lua_module(table);
  return table;
}

namespace lua {

auto basic_lua_module(sol::table) -> sol::table {
  auto lua_table = state["lua"].get_or_create<sol::table>();
  lua_table["level"] = level;
  lua_table["eval"] = eval;
  lua_table["eval_file"] = [](std::string_view str) { eval_file(str); };
  return lua_table;
}

auto default_lua_module(sol::table state) -> sol::table {
  auto table = basic_lua_module(state);
  live::lua_module(table);
  return table;
}

namespace live {

auto lua_module(sol::table state) -> sol::table {
  auto live = state["live"].get_or_create<sol::table>();
  auto paths = live["paths"].get_or_create<sol::table>();

  paths["assign"] = [](sol::table const& table) {
    _paths.clear();
    for (auto const& [k, v] : table) {
      _paths.push_back(
          weakly_canonical(std::filesystem::path(v.as<std::string_view>())));
    }
  };

  paths["append"] = [](sol::table const& table) {
    for (auto const& [k, v] : table) {
      _paths.push_back(
          weakly_canonical(std::filesystem::path(v.as<std::string_view>())));
    }
  };

  paths["clear"] = [] { _paths.clear(); };

  paths["print"] = [] {
    for (auto const& path : _paths) std::println("{}", path.string());
  };

  return live;
}

}  // namespace live

}  // namespace lua
