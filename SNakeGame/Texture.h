
#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>
#ifndef Texture_HEADER
#define Texture_HEADER

extern SDL_Window* window;

extern SDL_Renderer* renderer;

extern const int mapPixel;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Texture
{
private:
	int width;
	int height;
	SDL_Texture* texture;
public:
	Texture();
	~Texture();
	bool loadFromFile(std::string path);

	bool loadFromRenderedText(std::string textureText, TTF_Font* font, SDL_Color textColor = { 0, 0, 0, 255 });

	void free();
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void render(int x, int y, double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect * clip = NULL, SDL_Point * center = NULL);
	int getWidth();
	int getHeight();

};
#endif // !Texture_HEADER
