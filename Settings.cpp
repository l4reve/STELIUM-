#include "Settings.hpp"
#include "Constances.hpp"
#include <iostream>

Settings::Settings()
    : showMenu(false), volumeOn(true), gFont(nullptr),
    clickSound(nullptr), winnerSound(nullptr), loserSound(nullptr) {
    settingsButton = { SCREEN_WIDTH - 60, 10, 50, 50 }; // Position and size of the settings button
    menuRect = { SCREEN_WIDTH - 190, 70, 180, 200 }; // Position and size of the menu
}


Settings::~Settings() {
    gSettingsButtonTexture.free();
    gMenuTexture.free();
    gNewGameTexture.free();
    gVolumeTexture.free();
    gQuitTexture.free();
    TTF_CloseFont(gFont);
    gFont = nullptr;

    Mix_FreeChunk(clickSound);
    Mix_FreeChunk(winnerSound);
    Mix_FreeChunk(loserSound);
    clickSound = nullptr;
    winnerSound = nullptr;
    loserSound = nullptr;
}

bool Settings::loadMedia() {
    gFont = TTF_OpenFont("Font/DTM-Sans.ttf", 28);
    if (gFont == nullptr) {
        std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    if (!loadButtonTextures()) return false;

    clickSound = Mix_LoadWAV("CLCK.wav");
    if (clickSound == nullptr) {
        std::cout << "Failed to load click sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    winnerSound = Mix_LoadWAV("WIN.wav");
    if (winnerSound == nullptr) {
        std::cout << "Failed to load winner sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    loserSound = Mix_LoadWAV("LOST.wav");
    if (loserSound == nullptr) {
        std::cout << "Failed to load loser sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

bool Settings::loadButtonTextures() {
    if (!gSettingsButtonTexture.loadFromFile("Image/Settings.png")) {
        std::cout << "Failed to load settings button texture!\n";
        return false;
    }
    if (!gMenuTexture.loadFromFile("Image/Menu.png")) {
        std::cout << "Failed to load menu texture!\n";
        return false;
    }

    SDL_Color textColor = { 255, 0, 0, 255 }; // Dark red color
    if (!gNewGameTexture.loadFromRenderedText("New Game", textColor)) {
        std::cout << "Failed to load new game texture!\n";
        return false;
    }
    if (!gVolumeTexture.loadFromRenderedText("Volume: On", textColor)) {
        std::cout << "Failed to load volume texture!\n";
        return false;
    }
    if (!gQuitTexture.loadFromRenderedText("Quit", textColor)) {
        std::cout << "Failed to load quit texture!\n";
        return false;
    }
    return true;
}

void Settings::handleEvent(SDL_Event* e, bool& quit, bool& newGame) {
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (x >= settingsButton.x && x <= settingsButton.x + settingsButton.w &&
            y >= settingsButton.y && y <= settingsButton.y + settingsButton.h) {
            showMenu = !showMenu;
        }

        if (showMenu) {
            if (x >= menuRect.x && x <= menuRect.x + menuRect.w) {
                if (y >= menuRect.y && y < menuRect.y + 40) {
                    newGame = true;
                    showMenu = false;
                    playSound("click.wav");
                }
                else if (y >= menuRect.y + 40 && y < menuRect.y + 80) {
                    toggleVolume();
                    playSound("click.wav");
                }
                else if (y >= menuRect.y + 80 && y < menuRect.y + 120) {
                    quit = true;
                    playSound("click.wav");
                }
            }
        }
    }
}

void Settings::render(const SDL_Rect& settingsButton) { // Updated function signature
    gSettingsButtonTexture.render(settingsButton.x, settingsButton.y);
    if (showMenu) {
        renderMenu(menuRect);
    }
}

void Settings::renderMenu(const SDL_Rect& menuRect) { // Updated function signature
    gMenuTexture.render(menuRect.x, menuRect.y);
    gNewGameTexture.render(menuRect.x + 10, menuRect.y + 10);
    gVolumeTexture.render(menuRect.x + 10, menuRect.y + 50);
    gQuitTexture.render(menuRect.x + 10, menuRect.y + 90);
}


void Settings::toggleVolume() {
    volumeOn = !volumeOn;
    if (volumeOn) {
        Mix_Volume(-1, MIX_MAX_VOLUME);
        gVolumeTexture.loadFromRenderedText("Volume: On", { 255, 0, 0, 255 });
    }
    else {
        Mix_Volume(-1, 0);
        gVolumeTexture.loadFromRenderedText("Volume: Off", { 255, 0, 0, 255 });
    }
}

void Settings::playSound(const std::string& soundFile) {
    if (volumeOn) {
        Mix_Chunk* sound = nullptr;
        if (soundFile == "CLCK.wav") {
            sound = clickSound;
        }
        else if (soundFile == "WIN.wav") {
            sound = winnerSound;
        }
        else if (soundFile == "LOST.wav") {
            sound = loserSound;
        }
        if (sound) {
            Mix_PlayChannel(-1, sound, 0);
        }
    }
}
bool Settings::isMenuOpen() {
    return showMenu;
}
