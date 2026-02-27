module;
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

module console;

import std;

namespace console::log {

// format
// format_to

// debug / DEBUG
// info
// warn
// error

static void stdout(std::string_view str) {
  std::osyncstream(std::cout) << str << std::endl;
}

static void stderr(std::string_view str) {
  std::osyncstream(std::cerr) << str << std::endl;
}

void text(std::string_view str) {
  stdout(str);
}

void debug(std::string_view str) {
  const auto severity = fmt::format("{:<10}", fmt::format("{}:", "DEBUG"));
  stdout(fmt::format(fg(fmt::color::gray), "{}{}", severity, str));
}

void info(std::string_view str) {
  const auto severity = fmt::format("{:<10}", fmt::format("{}:", "INFO"));
  stdout(fmt::format("{}{}", fmt::format(fg(fmt::color::green), "{}", severity),
                     str));
}

void warn(std::string_view str) {
  const auto severity = fmt::format("{:<10}", fmt::format("{}:", "WARNING"));
  stderr(fmt::format("{}{}",
                     fmt::format(fg(fmt::color::orange), "{}", severity), str));
}

void error(std::string_view str) {
  const auto severity = fmt::format("{:<10}", fmt::format("{}:", "ERROR"));
  stderr(fmt::format("{}{}", fmt::format(fg(fmt::color::red), "{}", severity),
                     str));
}

}  // namespace console::log
