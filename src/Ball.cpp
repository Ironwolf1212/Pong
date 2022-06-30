#include "Ball.h"

void Ball::initialize(float x, float y, float speedX, float speedY, float radius) {
	this ->x = x;
	this ->y = y;
	this ->radius = radius;
	//Multiplica la velocidad por un número al azar entre {-1,1} para determinar la dirección de la bola al empezar
	this ->speedX = (float)(speedX * -pow(-1, rand() % 2));
	this ->speedY = speedY;
}

void Ball::Draw() {
	DrawCircle((int)x, (int)y, radius, WHITE);
}
float Ball::getX() {
	return x;
}
void Ball::setX(float x) {
	this->x = x;
}
float Ball::getY() {
	return y;
}
void Ball::setY(float y) {
	this->y = y;
}
float Ball::getSpeedX() {
	return speedX;
}
void Ball::setSpeedX(float speedX) {
	this->speedX = speedX;
}
float Ball::getSpeedY() {
	return speedY;
}
void Ball::setSpeedY(float speedY) {
	this->speedY = speedY;
}
float Ball::getRadius() {
	return radius;
}
