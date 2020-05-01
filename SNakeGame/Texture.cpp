#include "Texture.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
const int mapPixel = 32;
const int SCREEN_WIDTH = 992;
const int SCREEN_HEIGHT = 576;

Texture::Texture()
{
	texture = NULL;
	width = 0;
	height = 0;
}

Texture::~Texture()
{
	free();
}

bool Texture::loadFromFile(std::string path)
{
	free();
	SDL_Texture* tempTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL)
	{
		printf("%s %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		tempTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

		if (tempTexture == NULL)
		{
			printf("%s %s\n", path.c_str(), IMG_GetError());
		}
		else
		{
			width = loadedSurface->w;
			height = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	texture = tempTexture;


	return texture != NULL;
}
bool Texture::loadFromRenderedText(std::string textureText, TTF_Font* font, SDL_Color textColor)
{
	free();
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);

	if (textSurface != NULL)
	{
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL)
		{
			printf("Failed to create image from text!!! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			width = textSurface->w;
			height = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Failed to render image from text!!! SDL Error: %s\n", SDL_GetError());
	}
	return texture != NULL;
};
void Texture::free()
{
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(texture, red, green, blue);
}

void Texture::render(int x, int y, double angle, SDL_RendererFlip flip, SDL_Rect* clip, SDL_Point* center)

{
	SDL_Rect renderQuad = { x, y, width, height };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}

void close()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;
	IMG_Quit();
	SDL_Quit();
}