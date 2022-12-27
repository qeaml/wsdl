#define IMPLEMENT_WSDL
#include "wsdl.hpp"
#include <cstring>
#include <iostream>

#define expect_str(got, exp) {                         \
  if(std::strcmp(got, exp) != 0) {                     \
    std::cerr << "Read incorrect string" << std::endl; \
    std::cerr << "expected:" << std::endl;             \
    std::cerr << exp << std::endl;                     \
    std::cerr << "got:" << std::endl;                  \
    std::cerr << got << std::endl;                     \
    return 2;                                          \
  }                                                    \
}

#define init_sdl_with(initflags)                  \
  auto sdl = sdl::SDL(sdl::InitFlags()initflags); \
  if(!sdl.ok()) {                                 \
    return -1;                                    \
  }

#define init_sdl()                       \
  auto sdl = sdl::SDL(sdl::InitFlags()); \
  if(!sdl.ok()) {                        \
    return -1;                           \
  }
