#include "app.hpp"

int main(int argc, char* argv[]) {
  std::string line{};
  while (not done()) {
    std::getline(std::cin, line);
    lua::eval(line);
  }
}
