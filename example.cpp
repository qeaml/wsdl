#define IMPLEMENT_WSDL
#include "wsdl.hpp"

int main(int argc, char *argv[]) {
  auto sdl = sdl::SDL(sdl::InitFlags().video());
  auto window = sdl.createWindow("Example widnow", 640, 480);
  auto render = window.createRenderer();

  bool quit = false;
  while(!quit) {
    // ideally you'd forward this to a proper event handler
    sdl.pollEvents([&quit](sdl::Event evt) {
      if(evt.type == SDL_QUIT)
        quit = true;
    });

    render.color((Uint8)0, 0, 0);
    render.clear();

    render.color((Uint8)255, 0, 0);
    render.line(0, 0, 100, 100);

    render.color((Uint8)0, 255, 0);
    render.emptyRect(100, 100, 100, 100);

    render.color((Uint8)0, 0, 255);
    render.fillRect(200, 200, 100, 100);

    render.present();
  }

  return 0;
}
