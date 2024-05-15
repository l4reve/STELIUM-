#ifndef TIMER_HPP
#define TIMER_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Texture.hpp"

class Timer {
public:
    Timer();

    void start();
    void stop();
    void reset();
    void render();

private:
    Uint32 mStartTicks;
    Uint32 mPausedTicks;
    bool mPaused;
    bool mStarted;

    LTexture gTimerTexture;
};

#endif
