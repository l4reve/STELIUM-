#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <cstring>
#include "Button.hpp"
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Button.hpp"


using namespace std;

// Constants
const int ROW_SIZE = 10;
const int COLUMN_SIZE = 10;
const int MINE_COUNT = 10;
const int TILE_SIZE = 32;
const int DISTANCE_BETWEEN = 4;
const int BUTTON_SPRITE_TOTAL = 3;
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 400;
LButton gButtons[ROW_SIZE][COLUMN_SIZE];
#pragma once

//Tile's constants
const int TILE_SIZE = 32;
const int BUTTON_SPRITE_TOTAL = 12;

//Number of mines
const int MINE_COUNT = 12;

//Table's size
const int ROW_SIZE = 10;
const int COLUMN_SIZE = 10;

//Screen dimension constants
const int SCREEN_WIDTH = 460;
const int SCREEN_HEIGHT = 460;
const int DISTANCE_BETWEEN = (SCREEN_WIDTH - ROW_SIZE * TILE_SIZE) / 2;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//The sound effects that will be used
Mix_Music* winner = NULL;
Mix_Music* loser = NULL;
Mix_Chunk* click = NULL;

//Globally used font
TTF_Font* gGameOver = NULL;
TTF_Font* gPlayAgainWin = NULL;
TTF_Font* gPlayAgainLose = NULL;

//Mouse button texture
SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
LTexture gButtonSpriteSheetTexture;

//Screen texture
LTexture gWinningTexture;
LTexture gBackgroundTexture;

//Rendered texture
LTexture gTextTexture;
LTexture gMineLeftTexture;
LTexture gPlayAgainWinTexture;
LTexture gPlayAgainLoseTexture;

//Gameplay variables
int countMineLeft = MINE_COUNT;
int countTileLeft = ROW_SIZE * COLUMN_SIZE;
bool gameOver = false;
bool isWinning = false;

//In memory text stream
stringstream mineLeft;

//Board with mine
int board[ROW_SIZE][COLUMN_SIZE];
int sBoard[ROW_SIZE][COLUMN_SIZE];

// Function declarations
bool init();
bool loadMedia();
void close();
void createTableWithMine();
bool checkWinning();
void mineManager();
void flagManager();
void playAgainManager(bool& quitGame);


class LTexture {
public:
    LTexture();
    ~LTexture();
    bool loadFromRenderedText(string textureText, SDL_Color textColor);
    bool loadFromFile(string path);
    void free();
    void render(int x, int y, SDL_Rect* clip = NULL);
    int getWidth();
    int getHeight();

private:
    SDL_Texture* mTexture;
    int mWidth, mHeight;
};

LTexture::LTexture() {
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    free();
}

bool LTexture::loadFromRenderedText(string textureText, SDL_Color textColor) {
    free();
    SDL_Surface* textSurface = TTF_RenderText_Solid(gGameOver, textureText.c_str(), textColor);
    if (textSurface == NULL) {
        cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
    }
    else {
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture == NULL) {
            cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << endl;
        }
        else {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }
    return mTexture != NULL;
}

bool LTexture::loadFromFile(string path) {
    free();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << endl;
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
        }
        else {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free() {
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(int x, int y, SDL_Rect* clip) {
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}


int main(int argc, char* args[])
{
    if (!init())
    {
        cout << "Failed to initialize!\n";
    }
    else
    {
        if (!loadMedia())
        {
            cout << "Failed to load media!\n";
        }
        else
        {
            bool quit = false;
            SDL_Event e;

            while (!quit)
            {
                createTableWithMine();

                while (!gameOver && !quit && !isWinning)
                {
                    while (SDL_PollEvent(&e) != 0)
                    {
                        if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
                        {
                            quit = true;
                        }

                        for (int i = 0; i < ROW_SIZE; i++)
                        {
                            for (int j = 0; j < COLUMN_SIZE; j++)
                            {
                                gButtons[i][j].handleEvent(&e);
                            }
                        }
                        isWinning = checkWinning();
                    }

                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(gRenderer);

                    gBackgroundTexture.render(0, 0);

                    for (int i = 0; i < ROW_SIZE; i++)
                    {
                        for (int j = 0; j < COLUMN_SIZE; j++)
                        {
                            gButtons[i][j].render(i, j);
                        }
                    }

                    mineManager();
                    flagManager();

                    SDL_RenderPresent(gRenderer);
                }

                playAgainManager(quit);
            }
        }
    }

    close();

    return 0;
}

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            cout << "Warning: Linear texture filtering not enabled!";
        }

        gWindow = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
                success = false;
            }
            else
            {
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
                    success = false;
                }

                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
                    success = false;
                }

                if (TTF_Init() == -1)
                {
                    cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    bool success = true;

    // Loading fonts
    gGameOver = TTF_OpenFont("Font/DTM-Sans.ttf", 40);
    gPlayAgainWin = TTF_OpenFont("Font/DTM-Sans.ttf", 40);
    gPlayAgainLose = TTF_OpenFont("Font/DTM-Sans.ttf", 40);

    // Loading textures
    if (!gWinningTexture.loadFromFile("Image/Winner.png"))
    {
        cout << "Failed to load winning texture!\n";
        success = false;
    }
    if (!gBackgroundTexture.loadFromFile("Image/Background.png"))
    {
        cout << "Failed to load background texture!\n";
        success = false;
    }
    if (!gButtonSpriteSheetTexture.loadFromFile("Image/Tiles.png"))
    {
        cout << "Failed to load sprites texture!\n";
        success = false;
    }

    // Loading sounds
    winner = Mix_LoadMUS("Sounds/winner.wav");
    loser = Mix_LoadMUS("Sounds/loser.wav");
    click = Mix_LoadWAV("Sounds/click.wav");

    return success;
}

void createTableWithMine()
{
    srand(time(NULL));
    int mine = 0;
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COLUMN_SIZE; j++)
        {
            sBoard[i][j] = 10;
            board[i][j] = 0;
        }
    }
    while (mine < MINE_COUNT)
    {
        int i = rand() % ROW_SIZE;
        int j = rand() % COLUMN_SIZE;
        if (board[i][j] == 9)
        {
            continue;
        }
        else
        {
            board[i][j] = 9;
            mine++;
            if (board[i - 1][j] != 9 && i > 0)
                board[i - 1][j]++;
            if (board[i][j - 1] != 9 && j > 0)
                board[i][j - 1]++;
            if (board[i + 1][j] != 9 && i < ROW_SIZE - 1)
                board[i + 1][j]++;
            if (board[i][j + 1] != 9 && j < COLUMN_SIZE - 1)
                board[i][j + 1]++;
            if (board[i - 1][j - 1] != 9 && i > 0 && j > 0)
                board[i - 1][j - 1]++;
            if (board[i - 1][j + 1] != 9 && i > 0 && j < COLUMN_SIZE - 1)
                board[i - 1][j + 1]++;
            if (board[i + 1][j - 1] != 9 && j > 0 && i < ROW_SIZE - 1)
                board[i + 1][j - 1]++;
            if (board[i + 1][j + 1] != 9 && i < ROW_SIZE - 1 && j < COLUMN_SIZE - 1)
                board[i + 1][j + 1]++;
        }
    }
}

bool checkWinning()
{
    bool win = false;
    if (countTileLeft == MINE_COUNT)
    {
        win = true;
    }
    return win;
}

void mineManager()
{
    if (!gameOver && !isWinning)
    {
        SDL_Color textColor = { 140, 140, 140, 255 };

        mineLeft.str("");
        mineLeft << " " << countMineLeft;
        if (!gMineLeftTexture.loadFromRenderedText(mineLeft.str().c_str(), textColor))
        {
            cout << "Unable to render mine left texture!\n";
        }

        gMineLeftTexture.render((SCREEN_WIDTH - gMineLeftTexture.getWidth()) / 2, 0);
    }
}

void flagManager()
{
    if (isWinning && !gameOver)
    {
        SDL_RenderPresent(gRenderer);
        SDL_Delay(500);
        Mix_PlayMusic(winner, 0);
        gWinningTexture.render(0, 0);
        gPlayAgainWinTexture.render((SCREEN_WIDTH - gPlayAgainWinTexture.getWidth()) / 2, SCREEN_HEIGHT - gPlayAgainWinTexture.getHeight());
    }

    if (gameOver)
    {
        gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth()) / 2, 0);
        Mix_PlayMusic(loser, 0);

        for (int i = 0; i < ROW_SIZE; i++)
        {
            for (int j = 0; j < COLUMN_SIZE; j++)
            {
                sBoard[i][j] = board[i][j];
                gButtons[i][j].render(i, j);
            }
        }

        gPlayAgainLoseTexture.render((SCREEN_WIDTH - gPlayAgainLoseTexture.getWidth()) / 2, SCREEN_HEIGHT - gPlayAgainLoseTexture.getHeight());
    }
}

void playAgainManager(bool& quitGame)
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.key.keysym.sym == SDLK_s)
        {
            Mix_HaltMusic();
            countMineLeft = MINE_COUNT;
            countTileLeft = ROW_SIZE * COLUMN_SIZE;
            gameOver = false;
            isWinning = false;
            quitGame = false;
        }
        else if (e.key.keysym.sym == SDLK_ESCAPE)
            quitGame = true;
    }
}

void close()
{
    gButtonSpriteSheetTexture.free();
    gMineLeftTexture.free();
    gBackgroundTexture.free();
    gWinningTexture.free();
    gTextTexture.free();

    TTF_CloseFont(gGameOver);
    TTF_CloseFont(gPlayAgainLose);
    TTF_CloseFont(gPlayAgainWin);
    gGameOver = NULL;
    gPlayAgainLose = NULL;
    gPlayAgainWin = NULL;

    Mix_FreeMusic(winner);
    Mix_FreeMusic(loser);
    Mix_FreeChunk(click);
    winner = NULL;
    loser = NULL;
    click = NULL;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}
