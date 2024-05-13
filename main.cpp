#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <sstream>
#include <string>
#include "Texture.hpp"
#include "Constances.hpp"
#include "Variables.hpp"

using namespace std;

LButton gButtons[ROW_SIZE][COLUMN_SIZE];

bool init();
bool loadMedia();
void close();
void createTableWithMine();
bool checkWinning();
void mineManager();
void flagManager();
void playAgainManager(bool& quitGame);

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

	gGameOver = TTF_OpenFont("Font/DTM-Sans.ttf", 40);
	if (gGameOver == NULL)
	{
		cout << "Failed to load DTM-Sans font! SDL_ttf Error: " << TTF_GetError() << endl;
		success = false;
	}
	else
	{
		SDL_Color textColor = { 140, 140, 140 };
		if (!gTextTexture.loadFromRenderedText("GAME OVER :(", textColor))
		{
			cout << "Failed to render text texture!\n";
			success = false;
		}
	}

	gPlayAgainWin = TTF_OpenFont("Font/DTM-Sans.ttf", 40);
	if (gPlayAgainWin == NULL)
	{
		cout << "Failed to load DTM-Sans font! SDL_ttf Error: " << TTF_GetError() << endl;
		success = false;
	}
	else
	{
		SDL_Color playAgainWin = { 30, 100, 100 };
		if (!gPlayAgainWinTexture.loadFromRenderedText("Press s to play again!", playAgainWin))
		{
			cout << "Failed to render text texture!\n";
			success = false;
		}
	}

	gPlayAgainLose = TTF_OpenFont("Font/DTM-Sans.ttf", 40);
	if (gPlayAgainLose == NULL)
	{
		cout << "Failed to load DTM-Sans font! SDL_ttf Error: " << TTF_GetError() << endl;
		success = false;
	}
	else
	{
		SDL_Color playAgainLose = { 140, 140, 140 };
		if (!gPlayAgainLoseTexture.loadFromRenderedText("Press s to play again!", playAgainLose))
		{
			cout << "Failed to render text texture!\n";
			success = false;
		}
	}

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
	else
	{
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; i++)
		{
			gSpriteClips[i].x = i * 32;
			gSpriteClips[i].y = 0;
			gSpriteClips[i].w = TILE_SIZE;
			gSpriteClips[i].h = TILE_SIZE;
		}

		for (int i = 0; i < ROW_SIZE; i++)
		{
			for (int j = 0; j < COLUMN_SIZE; j++)
			{
				gButtons[i][j].setPosition(j * TILE_SIZE + DISTANCE_BETWEEN, i * TILE_SIZE + DISTANCE_BETWEEN);
			}
		}
	}

	winner = Mix_LoadMUS("Sounds/winner.wav");
	if (winner == NULL)
	{
		cout << "Failed to load winner sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	loser = Mix_LoadMUS("Sounds/loser.wav");
	if (loser == NULL)
	{
		cout << "Failed to load loser sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	click = Mix_LoadWAV("Sounds/click.wav");
	if (click == NULL)
	{
		cout << "Failed to load click sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

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
		else if (e.key.keysym.sym == SDLK_ESCAPE) quitGame = true;
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
