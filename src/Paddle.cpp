#pragma once
#include"Paddle.h"
void Paddle::initialize(float x, float y, float speed, float width, float height) {
	Paddle::x = x;
	Paddle::y = y;
	Paddle::width = width;
	Paddle::height = height;
	Paddle::speed = speed;
}
//Obtiene rectángulo de contorno
Rectangle Paddle::getRec() {
	return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
}

//Dibuja el palo
void Paddle::Draw() {
	DrawRectangle((int)(x - width / 2), (int)(y - height / 2), 10, 100, WHITE);
}

float Paddle::getY() {
	return y;
}

void Paddle::setY(float y) {
	this->y = y;
}
float Paddle::getSpeed() {
	return speed;
}

float Paddle::getHeight() {
	return height;
}