#include <stdio.h>
#include <iostream>
#include "raylib.h"
#include "src/Button.h"
#include "src/Ball.h"
#include "src/Paddle.h"

//Inicializa los Componentes del Pong
void initComponents(Ball* ball, Paddle* leftPaddle, Paddle* rightPaddle) {
	(*ball).initialize(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, 300, 300, 5);
	(*leftPaddle).initialize(50, (float)GetScreenHeight() / 2, 500, 10, 100);
	(*rightPaddle).initialize((float)GetScreenWidth() - 50, (float)GetScreenHeight() / 2, 500, 10, 100);
}

//Renderiza el texto del ganador del Pong
void showWinner(const char** winnerText) {
	int textWidth = MeasureText((*winnerText), 60);
	DrawText((*winnerText), GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
};

//Se encarga de toda la lógica del Pong
void pongLogic(Ball* ball, Paddle* leftPaddle, Paddle* rightPaddle, const char** winnerText, Sound* rightPong, Sound* leftPong, Sound* rightWin, Sound* leftWin, Texture2D* background, float* bgX, float* bgY, int* bgFrameCounter, int* timer) {
	//Mueve la bola en X y Y según velocidad, normaliza con fps
	(*ball).setX((*ball).getX()+ (*ball).getSpeedX() * GetFrameTime());
	(*ball).setY((*ball).getY() + (*ball).getSpeedY() * GetFrameTime());

	//Impide que la bola salga de la pantalla vertical
	if ((*ball).getY() < 0) {
		(*ball).setY(0);
		(*ball).setSpeedY((*ball).getSpeedY() * -1);
	}
	if ((*ball).getY() > GetScreenHeight()) {
		(*ball).setY((float)GetScreenHeight());
		(*ball).setSpeedY((*ball).getSpeedY()*-1);
	}

	//Controles de los palos
	if (IsKeyDown(KEY_W)) {
		if (!((*leftPaddle).getY() < 0 + ((*leftPaddle).getHeight()) / 2)) {
			(*leftPaddle).setY((*leftPaddle).getY() - (*leftPaddle).getSpeed() * GetFrameTime());
		}

	}
	if (IsKeyDown(KEY_S)) {
		if (!((*leftPaddle).getY() > GetScreenHeight() - ((*leftPaddle).getHeight()) / 2)) {
			(*leftPaddle).setY((*leftPaddle).getY() + (*leftPaddle).getSpeed() * GetFrameTime());
		}

	}
	if (IsKeyDown(KEY_UP)) {
		if (!((*rightPaddle).getY() < 0 + ((*rightPaddle).getHeight()) / 2)) {
			(*rightPaddle).setY((*rightPaddle).getY() - (*rightPaddle).getSpeed() * GetFrameTime());
		}

	}
	if (IsKeyDown(KEY_DOWN)) {
		if (!((*rightPaddle).getY() > GetScreenHeight() - ((*rightPaddle).getHeight()) / 2)) {
			(*rightPaddle).setY((*rightPaddle).getY() + (*rightPaddle).getSpeed() * GetFrameTime());
		}

	}

	//Controla colisiones ente la bola y los palos, reproduce sonido de colisión
	if (CheckCollisionCircleRec(Vector2{ (*ball).getX(),(*ball).getY()}, (*ball).getRadius(), (*leftPaddle).getRec())) {
		if ((*ball).getSpeedX() < 0) {
			(*ball).setSpeedX((*ball).getSpeedX()*-1.06f);
			(*ball).setSpeedY((((*ball).getY() - (*leftPaddle).getY()) / ((*leftPaddle).getHeight() / 2)) * (*ball).getSpeedX());
		}
		PlaySound(*rightPong);

	}
	if (CheckCollisionCircleRec(Vector2{ (*ball).getX(),(*ball).getY() }, (*ball).getRadius(), (*rightPaddle).getRec())) {
		if ((*ball).getSpeedX() > 0) {
			(*ball).setSpeedX((*ball).getSpeedX() * -1.06f);
			(*ball).setSpeedY((((*ball).getY() - (*rightPaddle).getY()) / ((*rightPaddle).getHeight() / 2)) * (*ball).getSpeedX());
		}
		PlaySound(*leftPong);

	}
	//Asigna el ganador y reproduce un sonido
	if ((*ball).getX() < 0 && *winnerText == nullptr) {
		(*winnerText) = "Right player wins";
		PlaySound(*rightWin);
	}
	if ((*ball).getX() > GetScreenWidth() && *winnerText == nullptr) {
		(*winnerText) = "Left player wins";
		PlaySound(*leftWin);
	}
	DrawTextureRec(*background, Rectangle{ *bgX,*bgY,500,500 }, Vector2{ 150,50 }, RAYWHITE);
	(*timer)++;
	if ((*timer) > 10) {
		(*bgFrameCounter)++;
		if (((*bgFrameCounter) <= 28) && ((*bgFrameCounter) % 5 != 0)) {
			(*bgX) += 500;
		}
		else if (((*bgFrameCounter) <= 28) && ((*bgFrameCounter) % 5 == 0)) {
			(*bgX) = 0;
			(*bgY) += 500;
		}
		else if ((*bgFrameCounter) > 28) {
			(*bgX) = 0;
			(*bgY) = 0;
			(*bgFrameCounter) = 0;
		}
		(*timer) = 0;
	}
	//Muestra el ganador
	if (winnerText) {
		showWinner(winnerText);
	}

	//Reinicia el juego
	if (*winnerText && IsKeyPressed(KEY_SPACE)) {
		initComponents(ball, leftPaddle, rightPaddle);
		(*winnerText) = nullptr;
		StopSound(*rightWin);
		StopSound(*leftWin);
	}


}

//Inicia el Pong
void renderPong(Ball* ball, Paddle* leftPaddle, Paddle* rightPaddle, const char** winnerText, Sound* rightPong, Sound* leftPong, Sound* rightWin, Sound* leftWin, Music* bgm, Texture2D* background) {
	float bgX = 0;
	float bgY = 0;
	int bgFrameCounter = 0;
	int timer = 0;
	PlayMusicStream(*bgm);
	//Loop del juego, se repite mientras la ventana esté abierta
	while (!WindowShouldClose()) {
		UpdateMusicStream(*bgm);
		//Invoca lógica del juego
		pongLogic(ball, leftPaddle, rightPaddle, winnerText, rightPong, leftPong, rightWin, leftWin, background, &bgX, &bgY, &bgFrameCounter, &timer);
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

//Se encarga de la lógica del Menu Principal
void menuLogic(Button* playButton,int* chosenGame) {
	bool mouseOnPlay = false;
	if ((GetMouseX() >= (int)(*playButton).getX())&&(GetMouseX() <= (int)((*playButton).getWidth() + (*playButton).getX())) && (GetMouseY() >= (int)((*playButton).getY())) && (GetMouseY() <= (int)((*playButton).getHeight() + (*playButton).getY()))) {
		mouseOnPlay = true;
	}
	else {
		mouseOnPlay = false;
	}
	if (mouseOnPlay)
	{
		(*playButton).changeColor(YELLOW, YELLOW);
	}
	else {
		(*playButton).changeColor(WHITE, WHITE);
	}
	if ((mouseOnPlay) && (IsMouseButtonDown(0))) {
		(*chosenGame) = 1;
	}
}

//Inicia el Menu Principal
void renderMenu() {
	Font titleFont = LoadFontEx("Resources/Fonts/Freehand521.ttf",150,0,0);
	Font buttonFont = LoadFontEx("Resources/Fonts/JustMyType.ttf", 55, 0, 0);
	const char* titleTextContent = "Arcade Game";
	Vector2 titleTextSize = MeasureTextEx(titleFont, titleTextContent, 150, 0);
	float titleTextWidth = titleTextSize.x;
	Button play((float)GetScreenWidth()/2,(float)(GetScreenHeight()/2)+50,"Play Pong",buttonFont,50,RAYWHITE,0,0,1,WHITE);
	int chosenGame = 0;
	//std::cout << textWidth << std::endl;
	
	while ((!WindowShouldClose())&&(chosenGame==0)) {
		menuLogic(&play,&chosenGame);
		BeginDrawing();
		ClearBackground(BLACK);
		DrawTextEx(titleFont, titleTextContent, Vector2{ (GetScreenWidth() - titleTextWidth)/2,100}, 150, 0, RAYWHITE);
		play.DrawButtonText();
		EndDrawing();
	};
	UnloadFont(titleFont);
	UnloadFont(buttonFont);
	switch (chosenGame)
	{
	case 1: {
		std::cout << "Chosen game Pong, ID: " + chosenGame << std::endl;
		//Se crean los objetos
		Sound rightPong = LoadSound("Resources/Sounds/RightPong.ogg");
		Sound leftPong = LoadSound("Resources/Sounds/LeftPong.ogg");
		Sound rightWin = LoadSound("Resources/Sounds/RightWin.ogg");
		SetSoundVolume(rightWin, 0.6f);
		Sound leftWin = LoadSound("Resources/Sounds/LeftWin.ogg");
		SetSoundVolume(leftWin, 0.6f);
		Music bgm = LoadMusicStream("Resources/Sounds/BGM.ogg");
		SetMusicVolume(bgm, 0.7f);
		Texture2D background = LoadTexture("Resources/Images/Background.png");
		Ball ball;
		Paddle leftPaddle;
		Paddle rightPaddle;
		//Se inicializan los objetos
		initComponents(&ball, &leftPaddle, &rightPaddle);
		const char* winnerText = nullptr;
		//Empieza el juego
		renderPong(&ball, &leftPaddle, &rightPaddle, &winnerText, &rightPong, &leftPong, &rightWin, &leftWin, &bgm, &background);
		//Desocupa la memoria cuando la ventana se cierra
		UnloadTexture(background);
		UnloadMusicStream(bgm);
		UnloadSound(rightPong);
		UnloadSound(leftPong);
		UnloadSound(rightWin);
		UnloadSound(leftWin);
		UnloadMusicStream(bgm);
		CloseAudioDevice();
	}break;

	default: {
		std::cout << "Something weird happened, invalid chosen game ID: "+chosenGame << std::endl;
	}break;
	}
}

//Crea la ventana y llama al inicio del Menu Principal
int main() {
	//Inicializa el audio
	InitAudioDevice();
	//Crea la ventana
	InitWindow(800, 600, "Pong");
	//Normaliza los fps
	SetWindowState(FLAG_VSYNC_HINT);
	renderMenu();
	CloseWindow();

	return 0;
}
