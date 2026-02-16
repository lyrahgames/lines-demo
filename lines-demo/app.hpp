#pragma once
#include <filesystem>
#include <print>
#include <string_view>
//
//
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

///
///
bool done() noexcept;

///
///
void quit() noexcept;

namespace lua {

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
    set_function("done", done);
    set_function("quit", quit);
  }
};

extern default_state state;

void eval(std::string_view str);
void eval_file(std::filesystem::path const& path);

}  // namespace lua
