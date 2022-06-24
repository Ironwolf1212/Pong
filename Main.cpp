#include <stdio.h>
#include <iostream>
#include "raylib.h"


struct Ball {
	float x=0, y=0;
	float speedX=0, speedY=0;
	float radius=0;

	void initialize(float x, float y, float speedX, float speedY, float radius) {
		Ball::x = x;
		Ball::y = y;
		Ball::radius = radius;
		Ball::speedX = speedX;
		Ball::speedY = speedY;
	}
	//Dibuja el cículo en pantalla
	void Draw() {
		DrawCircle((int)x,(int)y,radius, WHITE);
	}
};

struct Paddle {
	float x=0, y=0;
	float speed=0;
	float width=0, height=0;

	void initialize(float x, float y,float speed, float width,float height) {
			Paddle::x = x;
			Paddle::y = y;
			Paddle::width = width;
			Paddle::height = height;
			Paddle::speed = speed;
	}
	//Obtiene rectángulo de contorno
	Rectangle getRec() {
		return Rectangle{x - width / 2, y - height / 2, 10, 100};
	}

	//Dibuja el palo
	void Draw() {
		DrawRectangle((int)(x-width/2), (int)(y-height/2), 10, 100, WHITE);
	}
};

//Inicializa las variables para empezar un nuevo juego
void initComponents(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle) {
	(*ball).initialize(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, 300, 300, 5);
	(* leftPaddle).initialize(50, (float)GetScreenHeight() / 2, 500, 10, 100);
	(* rightPaddle).initialize((float)GetScreenWidth() - 50, (float)GetScreenHeight() / 2, 500, 10, 100);
}

//Renderiza el texto del ganador
void showWinner(const char** winnerText) {
	int textWidth = MeasureText((*winnerText), 60);
	DrawText((*winnerText), GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
};

//Se encarga de toda la lógica
void doLogic(Ball* ball, Paddle* leftPaddle, Paddle* rightPaddle, const char** winnerText, Sound* rightPong, Sound* leftPong) {

	//Mueve la bola en X y Y según velocidad, normaliza con fps
	(*ball).x += (*ball).speedX * GetFrameTime();
	(*ball).y += (*ball).speedY * GetFrameTime();

	//Impide que la bola salga de la pantalla vertical
	if ((*ball).y < 0) {
		(*ball).y = 0;
		(*ball).speedY *= -1;
	}
	if ((*ball).y > GetScreenHeight()) {
		(*ball).y = (float)GetScreenHeight();
		(*ball).speedY *= -1;
	}

	//Controles de los palos
	if (IsKeyDown(KEY_W)) {
		(*leftPaddle).y -= (*leftPaddle).speed * GetFrameTime();
	}
	if (IsKeyDown(KEY_S)) {
		(*leftPaddle).y += (*leftPaddle).speed * GetFrameTime();
	}
	if (IsKeyDown(KEY_UP)) {
		(*rightPaddle).y -= (*rightPaddle).speed * GetFrameTime();
	}
	if (IsKeyDown(KEY_DOWN)) {
		(*rightPaddle).y += (*rightPaddle).speed * GetFrameTime();
	}

	//Controla colisiones ente la bola y los palos, reproduce sonido de colisión
	if (CheckCollisionCircleRec(Vector2{ (*ball).x,(*ball).y }, (*ball).radius, (*leftPaddle).getRec())) {
		if ((*ball).speedX < 0) {
			(*ball).speedX *= -1.1f;
			(*ball).speedY = (((*ball).y - (*leftPaddle).y) / ((*leftPaddle).height / 2)) * (*ball).speedX;
		}
		PlaySound(*rightPong);

	}
	if (CheckCollisionCircleRec(Vector2{ (*ball).x,(*ball).y }, (*ball).radius, (*rightPaddle).getRec())) {
		if ((*ball).speedX > 0) {
			(*ball).speedX *= -1.1f;
			(*ball).speedY = (((*ball).y - (*rightPaddle).y) / ((*rightPaddle).height / 2)) * -(*ball).speedX;
		}
		PlaySound(*leftPong);
	}
	//Se encarga de mostrar el texto corecto del ganador
	if ((*ball).x < 0) {
		(*winnerText) = "Right player wins";
		showWinner(winnerText);
	}
	if ((*ball).x > GetScreenWidth()) {
		(*winnerText) = "Left player wins";
		showWinner(winnerText);
	}

	//Reinicia el juego
	if (winnerText && IsKeyPressed(KEY_SPACE)) {
		initComponents(ball, leftPaddle, rightPaddle);
		(*winnerText) = nullptr;
	}

}



void renderGame(Ball *ball, Paddle *leftPaddle, Paddle *rightPaddle, const char* *winnerText, Sound *rightPong, Sound *leftPong) {
	//Loop del juego, se repite mientras la ventana esté abierta
	while (!WindowShouldClose()) {
		//Invoca lógica del juego
		doLogic(ball, leftPaddle, rightPaddle, winnerText, rightPong, leftPong);
		//Inicializa la pantalla, dibuja elementos
		BeginDrawing();
		ClearBackground(BLACK);
		(*ball).Draw();
		(*leftPaddle).Draw();
		(*rightPaddle).Draw();
		DrawFPS(10, 10);
		//Desocupa memoria
		EndDrawing();
	}
}
int main() {
	//Inicializa el audio
	InitAudioDevice();
	//Crea la ventana
	InitWindow(800, 600, "Pong");
	//Normaliza los fps
	SetWindowState(FLAG_VSYNC_HINT);
	//Se crean los objetos
	Sound rightPong = LoadSound("Resources/Sounds/RightPong.ogg");
	Sound leftPong = LoadSound("Resources/Sounds/LeftPong.ogg");
	Ball ball;
	Paddle leftPaddle;
	Paddle rightPaddle;
	//Se inicializan los objetos
	initComponents(&ball, &leftPaddle, &rightPaddle);
	const char* winnerText = nullptr;
	//Empieza el juego
	renderGame(&ball, &leftPaddle, &rightPaddle, &winnerText, &rightPong, &leftPong);
	//Desocupa la memoria cuando la ventana se cierra
	UnloadSound(rightPong);
	UnloadSound(leftPong);
	CloseAudioDevice();
	CloseWindow();

	return 0;
}
