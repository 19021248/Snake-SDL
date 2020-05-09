#pragma once
#include "Texture.h"

enum direction{
	UP = 0,
	RIGHT = 90,
	DOWN = 180,
	LEFT = 270
};
class MapThing{
private:
	int x = -mapPixel;
	int y = -mapPixel;
	int size = mapPixel;
	direction dir = UP;
public:
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}
	int getSize() {
		return size;
	}
	direction getDir() {
		return dir;
	}
	void resetPos(){
		x = -mapPixel;
		y = -mapPixel;
	}
	void setDir(direction direct) {
		dir = direct;
	}
	void setX(int xx){
		if (xx < 0){
			xx = SCREEN_WIDTH - mapPixel;//O ben trai sang ben phai
		}
		else
			if (xx > SCREEN_WIDTH){
				xx = 0;//o ben phai sang ben trai
			}
		x = xx;
	}
	void setSize(int sizee) {
		size = sizee;
	}
	void setY(int yy) {
		if (yy < 0){
			yy = SCREEN_HEIGHT - mapPixel;
		}
		else
			if (yy > SCREEN_HEIGHT){
				yy = 0;
			}
		y = yy;
	}
	void setXY(int xx, int yy) {
		x = xx;
		y = yy;
	}
	bool collisionDetection(MapThing other)
	{
		bool touched = false;
		if (x < other.getX() + size && x + size>other.getX() && y<other.getY() + size && y + size>other.getY())
		{
			touched = true;
		}

		return touched;
	}
};


