#pragma once
#include "raylib.h"
#include <cmath>
class Ball {
private:	
	float x = 0, y = 0;
	float speedX = 0, speedY = 0;
	float radius = 0;
public:
	void initialize(float x, float y, float speedX, float speedY, float radius);
	void Draw();
	float getX();
	void setX(float x);
	float getY();
	void setY(float y);
	float getSpeedX();
	void setSpeedX(float speedX);
	float getSpeedY();
	void setSpeedY(float speedY);
	float getRadius();
};