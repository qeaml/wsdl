#include "../common.hpp"

#define expect(exp) {       \
  char got[sizeof(exp)];    \
  f.read(got, sizeof(exp)); \
  expect_str(got, exp);     \
}

int main(int argc, char *argv[]) {
  init_sdl()

  auto f = sdl.open("file.txt", "rt");
  if(!f.ok()) {
    return 1;
  }

  expect("Hello, world!\n");

  f.seek(7, sdl::RWOps::Whence::Set);
  expect("world!\n");

  f.close();
  return 0;
}
