#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstring>

using namespace std;
SDL_Texture* loadTexture(const std::string& path);

class LTexture
{
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
	int mWidth;
	int mHeight;
};


