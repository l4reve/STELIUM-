#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include "Texture.hpp"

class Settings {
public:
    Settings();
    ~Settings();

    bool loadMedia();
    void handleEvent(SDL_Event* e, bool& quit, bool& newGame);
    void render(const SDL_Rect& settingsButton); // Updated to accept settingsButton
    void renderMenu(const SDL_Rect& menuRect);   // Updated to accept menuRect
    void playSound(const std::string& soundFile);
    bool isMenuOpen(); // Declare isMenuOpen member function
private:
    bool loadButtonTextures();
    void toggleVolume();

    LTexture gSettingsButtonTexture;
    LTexture gMenuTexture;
    LTexture gNewGameTexture;
    LTexture gVolumeTexture;
    LTexture gQuitTexture;

    SDL_Rect settingsButton; 
    SDL_Rect menuRect;       // Moved here from Settings constructor

    bool showMenu;
    bool volumeOn;

    TTF_Font* gFont;
    Mix_Chunk* clickSound;
    Mix_Chunk* winnerSound;
    Mix_Chunk* loserSound;
};

#endif
