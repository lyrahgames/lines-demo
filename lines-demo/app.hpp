#pragma once
#include <xstd/io.hpp>
//
#include <sol/sol.hpp>

/// Check whether the application has been requested to quit.
///
bool done() noexcept;

/// Request to quit the application.
///
void quit() noexcept;

/// Add Lua tables and functions to allow basic control
/// over the application to the given Lua table.
///
auto basic_lua_module(sol::table) -> sol::table;

/// Add the default Lua tables and functions for
/// application control to the given Lua table.
///
auto default_lua_module(sol::table) -> sol::table;

/// The Application's Lua Features for Scripting
///
namespace lua {

/// Evaluate the given string as Lua code inside the global Lua state.
///
void eval(std::string_view str);

/// Read and evaluate the given file as Lua code inside the global Lua state.
///
void eval_file(std::filesystem::path const& path);

/// Returns the current recursion level of Lua execution, ie.,
/// the number `eval` or `eval_file` have been called recursively.
///
auto level() noexcept -> int;

/// Returns the maximum recursion level allowed for Lua evaluation calls.
///
auto max_level() noexcept -> int;

/// Sets the maximum recursion level allowed for Lua evaluation calls.
/// Used to prevent stack overflows and infinite recursion for Lua eval.
/// Already a few hundred levels can lead to a stack overflow.
///
void set_max_level(int);

///
///
auto basic_lua_module(sol::table) -> sol::table;

///
///
auto default_lua_module(sol::table) -> sol::table;

/// Lua Live Scripting Features
///
namespace live {

/// Try to receive a message for the given address/file path domain.
/// If successful, change the current path to the address' parent path and
/// evaluate the given message as Lua code in the global Lua state.
///
void listen(xstd::fdm::address const& domain);

/// Listen to all Lua files recursively contained in the stored live paths.
///
void watch();

///
///
auto lua_module(sol::table) -> sol::table;

}  // namespace live

/// This is a helper structure making use of RAII
/// to initialize a global Lua state with proper defaults.
///
struct default_state : sol::state {
  using base = sol::state;
  constexpr default_state() : base{} {
    open_libraries(           //
        sol::lib::base,       //
        sol::lib::package,    //
        sol::lib::coroutine,  //
        sol::lib::string,     //
        sol::lib::os,         //
        sol::lib::math,       //
        sol::lib::table,      //
        sol::lib::io,         //
        sol::lib::debug,      //
        sol::lib::bit32,      //
        sol::lib::utf8);
    // Include global default Lua functionality.
    ::default_lua_module(globals());
  }
};

/// The global Lua state used to evaluate basically all the Lua code.
///
extern default_state state;

///
///
// constexpr void load(std::invocable<sol::state_view> auto&& module) {
//   std::invoke(std::forward<decltype(module)>(module), state);
// }

///
///
// constexpr void load(std::invocable<sol::table> auto&& module) {
//   std::invoke(std::forward<decltype(module)>(module), state.globals());
// }

}  // namespace lua
