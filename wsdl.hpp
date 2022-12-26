/*
wsdl.hpp
--------
Minimal header-only C++ wrapper for SDL2. Uses object lifetimes to automatically
deinitialize allocated resources.

To include the implementation with the definitions, define the IMPLEMENT_WSDL
macro:

  #define IMPLEMENT_WSDL
  #include "wsdl.hpp"

You can also use the provided wsdl.cpp file to compile the implementation into
an object file for linking later.

qeaml 26.12.2022
*/

#pragma once

#include <SDL2/SDL.h>
#include <functional>

#ifdef WSDL_DEBUG
#include <iostream>
#endif

namespace sdl {

// define classes up here so they can be referred to down there
class SDL;
class WindowFlags;
class Window;
class RendererFlags;
class Renderer;

// contains flags deciding which subsystems should be initialized by the
// sdl::SDL(InitFlags) constructor
class InitFlags {
private:
  bool mEverything: 1,
       mTimer: 1,
       mAudio: 1,
       mVideo: 1,
       mJoystick: 1,
       mHaptic: 1,
       mGamecontroller: 1,
       mEvents: 1,
       mSensor: 1;

public:
  InitFlags() = default;

  InitFlags everything();
  InitFlags timer();
  InitFlags audio();
  InitFlags video();
  InitFlags joystick();
  InitFlags haptic();
  InitFlags gamecontroller();
  InitFlags events();
  InitFlags sensor();

  // returns the current init flags as a bitfield, this is used internally when
  // calling SDL_Init
  Uint32 get();
};

typedef SDL_Event Event;

class SDL {
private:
  bool mOK;
  Event mEvent;

public:
  SDL(InitFlags);
  ~SDL();
  bool ok();

  void pollEvents(std::function<void(Event)>);

  Window createWindow(WindowFlags, const char *title, int x, int y, int w, int h);
  Window createWindow(WindowFlags, const char *title, int w, int h);
  Window createWindow(const char *title, int x, int y, int w, int h);
  Window createWindow(const char *title, int w, int h);
};

class WindowFlags {
private:
  bool mFullscreen: 1,
       mOpenGL: 1,
       mShown: 1,
       mHidden: 1,
       mBorderless: 1,
       mResizable: 1,
       mMinimized: 1,
       mMaximized: 1,
       mMouseGrabbed: 1,
       mInputFocus: 1,
       mMouseFocus: 1,
       mFullscreenDesktop: 1,
       mForeign: 1,
       mAllowHighDPI: 1,
       mMouseCapture: 1,
       mAlwaysOnTop: 1,
       mSkipTaskbar: 1,
       mIsUtility: 1,
       mIsTooltip: 1,
       mIsPopup: 1,
       mKeyboardGrabbed: 1,
       mVulkan: 1,
       mMetal: 1;

public:
  WindowFlags() = default;

  WindowFlags fullscreen();
  WindowFlags openGL();
  WindowFlags shown();
  WindowFlags hidden();
  WindowFlags borderless();
  WindowFlags resizable();
  WindowFlags minimized();
  WindowFlags maximized();
  WindowFlags mouseGrabbed();
  WindowFlags inputFocus();
  WindowFlags mouseFocus();
  WindowFlags fullscreenDesktop();
  WindowFlags foreign();
  WindowFlags allowHighDPI();
  WindowFlags mouseCapture();
  WindowFlags alwaysOnTop();
  WindowFlags skipTaskbar();
  WindowFlags isUtility();
  WindowFlags isTooltip();
  WindowFlags isPopup();
  WindowFlags keyboardGrabbed();
  WindowFlags vulkan();
  WindowFlags metal();

  Uint32 get();
};

class Window {
private:
  SDL& mSDL;
  SDL_Window *mIntern;
  bool mOK;

public:
  Window(SDL&, WindowFlags, const char *title, int x, int y, int w, int h);
  Window(SDL&, WindowFlags, const char *title, int w, int h);
  Window(SDL&, const char *title, int x, int y, int w, int h);
  Window(SDL&, const char *title, int w, int h);
  ~Window();
  bool ok();
  SDL_Window *intern();

  Renderer createRenderer(RendererFlags);
  Renderer createRenderer();
};

class RendererFlags {
private:
  bool mSoftware: 1,
       mAccelerated: 1,
       mPresentVSync: 1,
       mTargetTexture: 1;

public:
  RendererFlags() = default;

  RendererFlags software();
  RendererFlags accelerated();
  RendererFlags presentVSync();
  RendererFlags targetTexture();

  Uint32 get();
};

class Renderer {
private:
  SDL& mSDL;
  Window& mWindow;
  SDL_Renderer *mIntern;
  bool mOK;

public:
  Renderer(SDL&, Window&, RendererFlags);
  Renderer(SDL&, Window&);
  ~Renderer();
  bool ok();
  SDL_Renderer *intern();

  void color(SDL_Color c);
  void color(Uint8 r, Uint8 g, Uint8 b);
  void color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
  void color(float r, float g, float b);
  void color(float r, float g, float b, float a);

  void line(int x1, int y1, int x2, int y2);

  void emptyRect(int x, int y, int w, int h);
  void emptyRect(SDL_Rect);

  void fillRect(int x, int y, int w, int h);
  void fillRect(SDL_Rect);

  void clear();
  void present();
};





#ifdef IMPLEMENT_WSDL

InitFlags InitFlags::everything()     { mEverything = true;     return *this; }
InitFlags InitFlags::timer()          { mTimer = true;          return *this; }
InitFlags InitFlags::audio()          { mAudio = true;          return *this; }
InitFlags InitFlags::video()          { mVideo = true;          return *this; }
InitFlags InitFlags::joystick()       { mJoystick = true;       return *this; }
InitFlags InitFlags::haptic()         { mHaptic = true;         return *this; }
InitFlags InitFlags::gamecontroller() { mGamecontroller = true; return *this; }
InitFlags InitFlags::events()         { mEvents = true;         return *this; }
InitFlags InitFlags::sensor()         { mSensor = true;         return *this; }

Uint32 InitFlags::get() {
  if(mEverything)
    return SDL_INIT_EVERYTHING;

  Uint32 flags = 0;
  flags |= SDL_INIT_TIMER * mTimer;
  flags |= SDL_INIT_AUDIO * mAudio;
  flags |= SDL_INIT_VIDEO * mVideo;
  flags |= SDL_INIT_JOYSTICK * mJoystick;
  flags |= SDL_INIT_HAPTIC * mHaptic;
  flags |= SDL_INIT_GAMECONTROLLER * mGamecontroller;
  flags |= SDL_INIT_SENSOR * mSensor;
  return flags;
}

SDL::SDL(InitFlags flags) {
  mOK = SDL_Init(flags.get()) == 0;
}

SDL::~SDL() {
#ifdef WSDL_DEBUG
  std::cerr<<"~SDL()"<<std::endl;
#endif
  SDL_Quit();
}

void SDL::pollEvents(std::function<void(Event)> f) {
  Event e;
  while(SDL_PollEvent(&e))
    f(e);
}

Window SDL::createWindow(WindowFlags flags, const char *title, int x, int y, int w, int h) {
  return Window(*this, flags, title, x, y, w, h);
}

Window SDL::createWindow(WindowFlags flags, const char *title, int w, int h) {
  return Window(*this, flags, title, w, h);
}

Window SDL::createWindow(const char *title, int x, int y, int w, int h) {
  return Window(*this, title, x, y, w, h);
}

Window SDL::createWindow(const char *title, int w, int h) {
  return Window(*this, title, w, h);
}

WindowFlags WindowFlags::fullscreen()        { mFullscreen = true;        return *this; }
WindowFlags WindowFlags::openGL()            { mOpenGL = true;            return *this; }
WindowFlags WindowFlags::shown()             { mShown = true;             return *this; }
WindowFlags WindowFlags::hidden()            { mHidden = true;            return *this; }
WindowFlags WindowFlags::borderless()        { mBorderless = true;        return *this; }
WindowFlags WindowFlags::resizable()         { mResizable = true;         return *this; }
WindowFlags WindowFlags::minimized()         { mMinimized = true;         return *this; }
WindowFlags WindowFlags::maximized()         { mMaximized = true;         return *this; }
WindowFlags WindowFlags::mouseGrabbed()      { mMouseGrabbed = true;      return *this; }
WindowFlags WindowFlags::inputFocus()        { mInputFocus = true;        return *this; }
WindowFlags WindowFlags::mouseFocus()        { mMouseFocus = true;        return *this; }
WindowFlags WindowFlags::fullscreenDesktop() { mFullscreenDesktop = true; return *this; }
WindowFlags WindowFlags::foreign()           { mForeign = true;           return *this; }
WindowFlags WindowFlags::allowHighDPI()      { mAllowHighDPI = true;      return *this; }
WindowFlags WindowFlags::mouseCapture()      { mMouseCapture = true;      return *this; }
WindowFlags WindowFlags::alwaysOnTop()       { mAlwaysOnTop = true;       return *this; }
WindowFlags WindowFlags::skipTaskbar()       { mSkipTaskbar = true;       return *this; }
WindowFlags WindowFlags::isUtility()         { mIsUtility = true;         return *this; }
WindowFlags WindowFlags::isTooltip()         { mIsTooltip = true;         return *this; }
WindowFlags WindowFlags::isPopup()           { mIsPopup = true;           return *this; }
WindowFlags WindowFlags::keyboardGrabbed()   { mKeyboardGrabbed = true;   return *this; }
WindowFlags WindowFlags::vulkan()            { mVulkan = true;            return *this; }
WindowFlags WindowFlags::metal()             { mMetal = true;             return *this; }

Uint32 WindowFlags::get() {
  Uint32 flags = 0;
  flags |= SDL_WINDOW_FULLSCREEN * mFullscreen;
  flags |= SDL_WINDOW_OPENGL * mOpenGL;
  flags |= SDL_WINDOW_SHOWN * mShown;
  flags |= SDL_WINDOW_HIDDEN * mHidden;
  flags |= SDL_WINDOW_BORDERLESS * mBorderless;
  flags |= SDL_WINDOW_RESIZABLE * mResizable;
  flags |= SDL_WINDOW_MINIMIZED * mMinimized;
  flags |= SDL_WINDOW_MAXIMIZED * mMaximized;
  flags |= SDL_WINDOW_MOUSE_GRABBED * mMouseGrabbed;
  flags |= SDL_WINDOW_INPUT_FOCUS * mInputFocus;
  flags |= SDL_WINDOW_MOUSE_FOCUS * mMouseFocus;
  flags |= SDL_WINDOW_FULLSCREEN_DESKTOP * mFullscreenDesktop;
  flags |= SDL_WINDOW_FOREIGN * mForeign;
  flags |= SDL_WINDOW_ALLOW_HIGHDPI * mAllowHighDPI;
  flags |= SDL_WINDOW_MOUSE_CAPTURE * mMouseCapture;
  flags |= SDL_WINDOW_ALWAYS_ON_TOP * mAlwaysOnTop;
  flags |= SDL_WINDOW_SKIP_TASKBAR * mSkipTaskbar;
  flags |= SDL_WINDOW_UTILITY * mIsUtility;
  flags |= SDL_WINDOW_TOOLTIP * mIsTooltip;
  flags |= SDL_WINDOW_POPUP_MENU * mIsPopup;
  flags |= SDL_WINDOW_KEYBOARD_GRABBED * mKeyboardGrabbed;
  flags |= SDL_WINDOW_VULKAN * mVulkan;
  flags |= SDL_WINDOW_METAL * mMetal;
  return flags;
}

Window::Window(SDL& sdl, WindowFlags flags, const char *title, int x, int y, int w, int h)
  : mSDL(sdl)
{
  mIntern = SDL_CreateWindow(title, x, y, w, h, flags.get());
  mOK = mIntern != NULL;
}

Window::Window(SDL& sdl, WindowFlags flags, const char *title, int w, int h)
  : mSDL(sdl)
{
  mIntern = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags.get());
  mOK = mIntern != NULL;
}

Window::Window(SDL& sdl, const char *title, int x, int y, int w, int h)
  : mSDL(sdl)
{
  mIntern = SDL_CreateWindow(title, x, y, w, h, 0);
  mOK = mIntern != NULL;
}

Window::Window(SDL& sdl, const char *title, int w, int h)
  : mSDL(sdl)
{
  mIntern = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);
  mOK = mIntern != NULL;
}

Window::~Window() {
#ifdef WSDL_DEBUG
  std::cerr<<"~Window()"<<std::endl;
#endif
  if(mOK)
    SDL_DestroyWindow(mIntern);
}

bool Window::ok() { return mOK; }

SDL_Window *Window::intern() { return mIntern; }

Renderer Window::createRenderer(RendererFlags flags) {
  return Renderer(mSDL, *this, flags);
}

Renderer Window::createRenderer() {
  return Renderer(mSDL, *this);
}

RendererFlags RendererFlags::software()      { mSoftware = true;      return *this; }
RendererFlags RendererFlags::accelerated()   { mAccelerated = true;   return *this; }
RendererFlags RendererFlags::presentVSync()  { mPresentVSync = true;  return *this; }
RendererFlags RendererFlags::targetTexture() { mTargetTexture = true; return *this; }

Uint32 RendererFlags::get() {
  Uint32 flags = 0;
  flags |= SDL_RENDERER_SOFTWARE * mSoftware;
  flags |= SDL_RENDERER_ACCELERATED * mAccelerated;
  flags |= SDL_RENDERER_PRESENTVSYNC * mPresentVSync;
  flags |= SDL_RENDERER_TARGETTEXTURE * mTargetTexture;
  return flags;
}

Renderer::Renderer(SDL& sdl, Window& window, RendererFlags flags)
  : mSDL(sdl), mWindow(window)
{
  mIntern = SDL_CreateRenderer(mWindow.intern(), -1, flags.get());
  mOK = mIntern != NULL;
}

Renderer::Renderer(SDL& sdl, Window& window)
  : mSDL(sdl), mWindow(window)
{
  mIntern = SDL_CreateRenderer(mWindow.intern(), -1, 0);
  mOK = mIntern != NULL;
}

Renderer::~Renderer() {
#ifdef WSDL_DEBUG
  std::cerr<<"~Renderer()"<<std::endl;
#endif
  if(mOK)
    SDL_DestroyRenderer(mIntern);
}

bool Renderer::ok() { return mOK; }

SDL_Renderer *Renderer::intern() { return mIntern; }

void Renderer::color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  SDL_SetRenderDrawColor(mIntern, r, g, b, a);
}

void Renderer::color(SDL_Color c) {
  color(c.r, c.g, c.b, c.a);
}
void Renderer::color(Uint8 r, Uint8 g, Uint8 b) {
  color(r, g, b, 255);
}

#define wsdl_float2uint8(f) (Uint8)((int)(f*255.0f)&0xFF)

void Renderer::color(float r, float g, float b) {
  color(wsdl_float2uint8(r),
        wsdl_float2uint8(g),
        wsdl_float2uint8(b));
}

void Renderer::color(float r, float g, float b, float a) {
  color(wsdl_float2uint8(r),
        wsdl_float2uint8(g),
        wsdl_float2uint8(b),
        wsdl_float2uint8(a));
}

#undef wsdl_float2uint8

void Renderer::line(int x1, int y1, int x2, int y2) { SDL_RenderDrawLine(mIntern, x1, y1, x2, y2); }

void Renderer::emptyRect(SDL_Rect r) { SDL_RenderDrawRect(mIntern, &r); }
void Renderer::emptyRect(int x, int y, int w, int h) { emptyRect(SDL_Rect{ x, y, w, h }); }

void Renderer::fillRect(SDL_Rect r) { SDL_RenderFillRect(mIntern, &r); }
void Renderer::fillRect(int x, int y, int w, int h) { fillRect(SDL_Rect{ x, y, w, h }); }

void Renderer::clear() { SDL_RenderClear(mIntern); }
void Renderer::present() { SDL_RenderPresent(mIntern); }

#endif

} // namespace sdl
