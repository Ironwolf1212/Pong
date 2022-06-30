#pragma once
#include "raylib.h"
class Paddle {
private:
	float x = 0, y = 0;
	float speed = 0;
	float width = 0, height = 0;
public:
	void initialize(float x, float y, float speed, float width, float height);
	Rectangle getRec();
	void Draw();
	float getY();
	void setY(float y);
	float getSpeed();
	float getHeight();
};