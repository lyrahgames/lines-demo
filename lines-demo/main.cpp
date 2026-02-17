#include "app.hpp"

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) lua::eval_file(argv[i]);
  while (not done()) {
    lua::live::watch();
  }
}
