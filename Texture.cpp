#include "Texture.hpp"
#include "Variables.hpp"
#include "Constances.hpp"

SDL_Texture* loadTexture(const std::string& path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << endl;
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			cout << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << endl;
		}
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}
LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;


}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromRenderedText(string textureText, SDL_Color textColor)
{
	free();
	SDL_Surface* textSurface = TTF_RenderText_Solid(gGameOver, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
	}
	else
	{
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << endl;
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	return mTexture != NULL;
}

bool LTexture::loadFromFile(string path)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << endl;
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Rect* clip)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

