#pragma once
#include <SDL_ttf.h>

extern TTF_Font* gFont;
//Tile's constants
const int TILE_SIZE = 32;
const int BUTTON_SPRITE_TOTAL = 12;

//Number of mines
const int MINE_COUNT = 18;

//Table's size
const int ROW_SIZE = 12;
const int COLUMN_SIZE = 12;

//Screen dimension constants
const int SCREEN_WIDTH = 572;
const int SCREEN_HEIGHT = 572;
const int DISTANCE_BETWEEN = (SCREEN_WIDTH - ROW_SIZE * TILE_SIZE) / 2;
