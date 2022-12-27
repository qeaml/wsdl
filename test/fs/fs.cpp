#include "../common.hpp"

int main(int argc, char *argv[]) {
  init_sdl()

  auto p = sdl.basePath();
  if(!std::filesystem::exists(p)) {
    std::cerr << "Expected base path " << p << " to exist!" << std::endl;
    return 1;
  }

  return 0;
}
