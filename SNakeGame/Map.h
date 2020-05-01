#pragma once
#include "Texture.h"
#include <SDL_mixer.h>
#include <string>
using namespace std;
const int numberOfMap = 6;

// ng
class Map
{
public:
	Texture maptexture[numberOfMap]; // 6 anh
	Mix_Chunk *mapEnterSound[numberOfMap];//7
	int maptoDisplay = 0;
	void randomMap()
	{
		
		int x = rand() % numberOfMap;

		while (x == maptoDisplay) 
		{
			x = rand() % numberOfMap;
		}

		maptoDisplay = x;

		Mix_PlayChannel(-1, mapEnterSound[maptoDisplay], 0); // chay nhac

	}
	void load()
	{
		for (int i = 0; i < numberOfMap; i++)
		{
			string namepath = "image/map" + to_string(i + 1) + ".png"; 

			maptexture[i].loadFromFile(namepath);

			string nameSoundPath = "sound/map" + to_string(i + 1) + ".wav";
			mapEnterSound[i] = Mix_LoadWAV(nameSoundPath.c_str());

		}
	}
	void mapRender()
	{
		maptexture[maptoDisplay].render(0, 0);
	}

};

