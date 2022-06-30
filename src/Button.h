#pragma once
#include "raylib.h"
class Button {
private:
	float x, y;
	float width, height;
	float roundness;
	int segments, lineThickness;
	Color color;
	const char* text; Color textColor; Font textFont; int textSize;

public:
	Button(float posX, float posY, const char* text, Font textFont, int textSize, Color textColor, float roundness, int segments, int lineThick, Color color);
	void DrawButtonText();
	void changeColor(Color buttonColor, Color textColor);
	float getX();
	float getY();
	float getWidth();
	float getHeight();
};
