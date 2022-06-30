#include "Button.h"

Button::Button(float posX, float posY, const char* text, Font textFont, int textSize, Color textColor, float roundness, int segments, int lineThick, Color color) {
	this ->textFont = textFont;
	this->textSize = textSize;
	this->width = MeasureTextEx(textFont, text, textSize, 1).x + 20;
	this->height = MeasureTextEx(textFont, text, textSize, 1).y + 20;
	x = posX - width / 2;
	y = posY - height / 2;
	this->roundness = roundness;
	this->segments = segments;
	this->lineThickness = lineThick;
	this->text = text;
	this->textColor = textColor;
	this->color = color;
}
void Button::DrawButtonText() {
	DrawRectangleRoundedLines(Rectangle{ x,y,width,height }, roundness, segments, lineThickness, color);
	DrawTextEx(textFont, text, Vector2{ x + 10,y + 10 }, textSize, 1, textColor);
}

void Button::changeColor(Color buttonColor, Color textColor) {
	color = buttonColor;
	Button::textColor = textColor;
}

float Button::getX() {
	return x;
}
float Button::getY() {
	return y;
}
float Button::getWidth() {
	return width;
}
float Button::getHeight() {
	return height;
}
