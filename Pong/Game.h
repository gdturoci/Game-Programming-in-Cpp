#pragma once
#include <SDL.h>

//Vector2 struct just stores x/y coordinates
struct Vector2 {
	float x;
	float y;
};

//Game class
class Game {
public:
	Game();

	//Initialize the game
	bool Initialize();

	//Run the game loop until it's over
	void RunLoop();

	//End the game
	void Shutdown();

private:

	//Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//Window created by SDL
	SDL_Window* Window;
	// Renderer for 2D drawing
	SDL_Renderer* Renderer;
	//Number of ticks since start of game
	Uint32 TicksCount;
	//Game should continue running
	bool IsRunning;

	//Direction of paddles
	int Paddle1Dir;
	int Paddle2Dir;
	//Vectors for paddle positions
	Vector2 Paddle1Pos;
	Vector2 Paddle2Pos;
	//Vector for ball position
	Vector2 BallPos;
	//Vector for ball velocity
	Vector2 BallVel;
};