#include <string>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>>
#include <time.h>
#include "Texture.h"
#include "MapThing.h"
#include "Map.h"
using namespace std;

string gamename = "simple snake";






int score = 0;
int game = 0;
TTF_Font *font;
TTF_Font *font1;

Texture scoreTexture;
Texture scoreBackground;
Texture startMenu;
Texture loseMenu;

class Fruit : public MapThing{
public:
	Mix_Chunk *eatSound;
	Texture texture;
	int scoreGain = 100;
	bool firstTime = true;
	void load(){
		texture.loadFromFile("image/food.png");
		eatSound = Mix_LoadWAV("sound/food.wav");
	}
	void fruitRender()
	{
		texture.render(this->getX(), this->getY());
	}
	void changeLocation()
	{
		if (!firstTime)
			Mix_PlayChannel(-1, eatSound, 0);
		srand(time(0) + rand());
		int randomX = rand() % (SCREEN_WIDTH / mapPixel);
		int randomY = rand() % (SCREEN_HEIGHT / mapPixel);
		setXY(randomX * mapPixel, randomY * mapPixel);
		score += scoreGain;
		firstTime = false;
	}
};
Fruit fruits[4];

Map map;


class Snake{
private:
	int bodyWidth, bodyHeight, numberOfSegment = 4;
	int size = mapPixel, movingFrame = 0, eatCounter = 0;;
	direction inputDir = UP;
	Texture head, body, tail, turn;
	MapThing bodyLocation[99];
	bool death = false;
	Mix_Chunk *deathSound;
public:
	void snakeStart() {
		head.loadFromFile("image/head.png");
		body.loadFromFile("image/body.png");
		tail.loadFromFile("image/tail.png");
		turn.loadFromFile("image/turn.png");
		deathSound = Mix_LoadWAV("sound/death.wav");
		bodyLocation[0].setXY(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	}
	bool deathBool() {
		return death;
	}
	void snakeBodyControl(SDL_Event e){
		if (!death){
			for (int i = 0; i < 4; i++){
				if (bodyLocation[0].collisionDetection(fruits[i])){
					numberOfSegment++; // tăng lên
					fruits[i].changeLocation();
					eatCounter++;
					if (eatCounter > 4) {
						eatCounter = 0;
						map.randomMap();
					}
				}
			}
			movingFrame++;
			switch (e.key.keysym.sym)
			{
			case SDLK_UP:
				if (bodyLocation[0].getDir() != DOWN)
					inputDir = UP;
				break;
			case SDLK_DOWN:
				if (bodyLocation[0].getDir() != UP)
					inputDir = DOWN;
				break;
			case SDLK_LEFT:
				if (bodyLocation[0].getDir() != RIGHT)
					inputDir = LEFT;
				break;
			case SDLK_RIGHT:
				if (bodyLocation[0].getDir() != LEFT)
					inputDir = RIGHT; // chỉnh hướng cho cái đầu
				break;
			default:
				break;
			}
			int maxmove = 6;
			int score2 = score;
			for (int i = 0; i < 5; i++) {
				if (score2 >= 2000) {
					maxmove--;
					score2 = score2 - 2000;
					if (maxmove <= 1) {
						maxmove = 1;
					}
				}
			}
			if (movingFrame >= maxmove) //game chay dc maxmove tick tốc độ đi 1 ô
			{
				for (int i = 0; i < numberOfSegment; i++)
				{
					int prevX = bodyLocation[i].getX(), prevY = bodyLocation[i].getY();
					int afterX, afterY;
					if (i == 0) // chỉnh hướng cho đầu
					{
						switch (inputDir)
						{
						case UP:
								bodyLocation[0].setDir(UP);
							break;
						case DOWN:
								bodyLocation[0].setDir(DOWN);
							break;
						case LEFT:
								bodyLocation[0].setDir(LEFT);
							break;
						case RIGHT:
								bodyLocation[0].setDir(RIGHT);
							break;
						default:
							break;
						}
						

						switch (bodyLocation[0].getDir())
						{
						case UP:
							bodyLocation[0].setY(bodyLocation[0].getY() - size);
							break;
						case DOWN:
							bodyLocation[0].setY(bodyLocation[0].getY() + size);
							break;
						case LEFT:
							bodyLocation[0].setX(bodyLocation[0].getX() - size);
							break;
						case RIGHT:
							bodyLocation[0].setX(bodyLocation[0].getX() + size);
							break;
						default:
							break;
						}

					}
					else
					{
						if (i == numberOfSegment - 1) // chỉnh hướng cho đuôi
							bodyLocation[i].setDir(bodyLocation[i - 1].getDir());
						else
							switch (bodyLocation[i - 1].getDir()) // chỉnh hướng cho thân
							{
							case UP:
							case DOWN:
								if (bodyLocation[i].getX() == bodyLocation[i - 1].getX())
									bodyLocation[i].setDir(bodyLocation[i - 1].getDir());
								break;
							case RIGHT:
							case LEFT:
								if (bodyLocation[i].getY() == bodyLocation[i - 1].getY())
									bodyLocation[i].setDir(bodyLocation[i - 1].getDir());
								break;
							default:
								break;
							}

						bodyLocation[i].setXY(afterX, afterY);
					}
					afterX = prevX; afterY = prevY;

					for (int j = 1; j < numberOfSegment - 1; j++)
					{
						if (!death&&bodyLocation[0].collisionDetection(bodyLocation[j])) // check va chạm
						{
							death = true;
							Mix_PlayChannel(-1, deathSound, 0);
						}
					}
				}

				movingFrame = 0; 

			}

		}
	}
	void snakeBodyArrange() // cho thẳng ra ( thân đầu đuôi .. )
	{
		death = false;
		bodyLocation[0].setXY(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		for (int i = 1; i < numberOfSegment; i++)
		{
			bodyLocation[i].resetPos();
		}
		numberOfSegment = 4;
		for (int i = 1; i < numberOfSegment; i++)
		{
			int xTemp = 0, yTemp = 0;
			switch (bodyLocation[i].getDir())
			{

			case UP:
				yTemp = size;
				break;
			case RIGHT:
				xTemp = -size;
				break;
			case DOWN:
				yTemp = -size;
				break;
			case LEFT:
				xTemp = size;
				break;
			default:
				break;
			}

			xTemp += bodyLocation[i - 1].getX();
			yTemp += bodyLocation[i - 1].getY();
			bodyLocation[i].setDir(UP);
			bodyLocation[i].setXY(xTemp, yTemp);

		}
	}
	
	
	void snakeRender() { // chỉnh hướng cho đầu t hân đít
		for (int i = 0; i < numberOfSegment; i++)
		{
			if (i == 0)
			{
				head.render(bodyLocation[i].getX(), bodyLocation[i].getY(), bodyLocation[i].getDir());
			}
			else
				if (i == numberOfSegment - 1)
				{
					tail.render(bodyLocation[i].getX(), bodyLocation[i].getY(), bodyLocation[i].getDir());
				}
				else
				{//[next][cur][after]
					int nextX = bodyLocation[i - 1].getX(), nextY = bodyLocation[i - 1].getY();
					int curX = bodyLocation[i].getX(), curY = bodyLocation[i].getY();
					int afterX = bodyLocation[i + 1].getX(), afterY = bodyLocation[i + 1].getY();

					if ((curX<afterX&&curY>nextY) || (curX<nextX&&curY>afterY))
					{
						turn.render(bodyLocation[i].getX(), bodyLocation[i].getY(), 0);
					}
					else
						if ((curX < nextX && curY < afterY) || (curX < afterX && curY < nextY))
						{
							turn.render(bodyLocation[i].getX(), bodyLocation[i].getY(), 90);
						}
						else
							if ((curX > afterX && curY < nextY) || (curX > nextX && curY < afterY))
							{
								turn.render(bodyLocation[i].getX(), bodyLocation[i].getY(), 180);
							}
							else
								if ((curX > nextX && curY > afterY) || (curX > afterX && curY > nextY))
								{
									turn.render(bodyLocation[i].getX(), bodyLocation[i].getY(), 270);
								}

								else
									body.render(bodyLocation[i].getX(), bodyLocation[i].getY(), bodyLocation[i].getDir());

				}
		}
	}
};

Snake mainSnake;
bool started = false;

bool init();
void close();

void startGame();
void endGame();

void loadFile()
{
	mainSnake.snakeStart();
	map.load();
	font = TTF_OpenFont("font.ttf", 28); // font trong game
	font1 = TTF_OpenFont("font.ttf", 50); // font khi end game
	scoreBackground.loadFromFile("image/score.png");
	//startMenu.render(0, 0);
	startMenu.loadFromFile("image/start.png");
	loseMenu.loadFromFile("image/OP6.png");
	for (int i = 0; i < 4; i++)
	{
		fruits[i].load();
		fruits[i].changeLocation();
	}

	fruits[3].texture.loadFromFile("image/goodFood.png");
	fruits[3].eatSound = Mix_LoadWAV("sound/goodFood.wav");
	fruits[3].scoreGain = 300;
}


int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Fail!\n");
	}
	
	else
	{


		loadFile();
		bool quit = false;
		SDL_Event e;
		mainSnake.snakeBodyArrange();

		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT){
					quit = true;
				}
				if (e.key.keysym.sym == SDLK_SPACE){
					startGame();


					if (mainSnake.deathBool()){
						endGame();
					}

				}
			}

			if (started)
			mainSnake.snakeBodyControl(e); 

			 SDL_RenderClear(renderer); 
			map.mapRender(); 


			for (int i = 0; i < 4; i++)
			{
				fruits[i].fruitRender();
			}
			mainSnake.snakeRender(); // hiển thị rắn
			if (started)
			{
				scoreBackground.render(0, 0);
				scoreTexture.loadFromRenderedText("POINT: " + to_string(score), font, { 255, 255, 255, 255 });
				scoreTexture.render(0, 0);
				if (mainSnake.deathBool())
				{
					
					loseMenu.render(0, 0); // start point
					scoreTexture.loadFromRenderedText("YOUR SCORE: " + to_string(score), font1, { 255,255,255,255 });
					scoreTexture.render(320, 420);
					scoreTexture.loadFromRenderedText("YOUR SCORE: " + to_string(score), font1, { 255,255,255,255 });
					scoreTexture.loadFromRenderedText("PRESS SPACE TO PLAY AGAIN", font1, { 200,46,49,255 });
					scoreTexture.render(220, 460);

				}
			}//
			else
			{
				if (game == 0)
					startMenu.render(0, 0);
			}

			
			SDL_RenderPresent(renderer);

		}

	}
	close();
	return 0;
}

void startGame()
{
	if (started == false)
	{
	    mainSnake.snakeBodyArrange();
		printf("game started");
		score = 0;
		started = true;
	}

}
void endGame()
{
	if (started)
	{
		started = false;
		game++; // khi = 0 mới  hiển thị menu
	}
}
bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("%s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Linear texture filtering not enabled!\n");
		}

		window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("%s\n", SDL_GetError());
			success = false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == NULL)
			{
				printf("%s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("%s\n", IMG_GetError());
					success = false;
				}
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("%s\n", Mix_GetError());
					success = false;
				}
				if (TTF_Init() == -1) {
					success = false;
				}
			}
		}
	}

	return success;
}
