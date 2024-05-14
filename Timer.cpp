#include "Timer.hpp"
#include "Constances.hpp"
#include "Variables.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

Timer::Timer() {
    mStartTicks = 0;
    mPausedTicks = 0;
    mPaused = false;
    mStarted = false;
}

void Timer::start() {
    mStarted = true;
    mPaused = false;
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void Timer::stop() {
    mStarted = false;
    mPaused = false;
    mStartTicks = 0;
    mPausedTicks = 0;
}

void Timer::reset() {
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void Timer::render() {
    if (mStarted) {
        Uint32 time = SDL_GetTicks() - mStartTicks;
        std::stringstream timeText;
        timeText << " " << (time / 1000) / 60 << ":" << std::setfill('0') << std::setw(2) << (time / 1000) % 60;

        SDL_Color textColor = { 139, 0, 0, 255 };
        if (!gTimerTexture.loadFromRenderedText(timeText.str().c_str(), textColor)) {
            std::cout << "Unable to render time texture!" << std::endl;
        }

        gTimerTexture.render(10, 10); // Adjust position as needed
    }
}
