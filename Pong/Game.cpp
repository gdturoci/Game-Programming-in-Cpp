#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
	:Window(nullptr),
	Renderer(nullptr),
	TicksCount(0),
	IsRunning(true),
	Paddle1Dir(0),
	Paddle2Dir(0) {}

bool Game::Initialize() {

	//initialize the SDL library
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	//if sdlResult != 0, then initialization failed and the game should quit
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	Window = SDL_CreateWindow(
		"Pong", //Window Title
		100, //Top left x-coordinate of window
		100, //Top left y-coordinate of window
		1024, //Width of window
		768, //Height of window
		0 //Flags (0 for no flags set)
	);
	//If SDL_CreateWindow fails then print error message and quit
	if (!Window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//After creating the window, create the renderer
	Renderer = SDL_CreateRenderer(
		Window, //Window to create renderer for
		-1, //Usually -1 when only 1 window (lets SDL decide)
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	//If SDL_CreateRenderer fails then print error message and quit
	if (!Renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	//Initialize Paddles (middle left and middle right) and ball positions (center)
	Paddle1Pos.x = 10.0f;
	Paddle1Pos.y = 768.0f/2.0f;
	Paddle2Pos.x = 1014.0f;
	Paddle2Pos.y = 768.0f/2.0f;
	BallPos.x = 1024.0f/2.0f;
	BallPos.y = 768.0f/2.0f;
	BallVel.x = -200.0f;
	BallVel.y = 235.0f;
	return true;
}

void Game::RunLoop() {

	//While the game is running
	while (IsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	
	//Returns true if it finds an event in the queue
	SDL_Event event;
	
	//While there are still events in the queue
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			
			//If the user wants to quit then quit
			case SDL_QUIT:
				IsRunning = false;
				break;
		}
	}

	//Get the state of the keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	//If escape is pressed, end the game
	if (state[SDL_SCANCODE_ESCAPE]) IsRunning = false;

	//Update left paddle direction using W (up)/S (down)
	Paddle1Dir = 0;
	if (state[SDL_SCANCODE_W]) Paddle1Dir -= 1;
	if (state[SDL_SCANCODE_S]) Paddle1Dir += 1;

	//Update right paddle direction using ^/v
	Paddle2Dir = 0;
	if (state[SDL_SCANCODE_UP]) Paddle2Dir -= 1;
	if (state[SDL_SCANCODE_DOWN]) Paddle2Dir += 2;
}

void Game::UpdateGame() {
	
	//Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), TicksCount + 16));
	
	//Deltatime is the difference in ticks from last from last frame (converted to seconds)
	float deltaTime = (SDL_GetTicks() - TicksCount) / 1000.0f;

	//Clamp maximum delta time value
	if (deltaTime > .05f) deltaTime = .05f;

	//Update tick counts for next frame
	TicksCount = SDL_GetTicks();

	//Update the paddles' positions (speed is 300)
	if (Paddle1Dir != 0) {
		Paddle1Pos.y += Paddle1Dir * 300.0f * deltaTime;

		//Boundary conditions for the paddle; prevent it from moving offscreen
		if (Paddle1Pos.y < (paddleH/2.0f + thickness)) Paddle1Pos.y = paddleH/2.0f + thickness;
		else if (Paddle1Pos.y > (768.0f - paddleH/2.0f - thickness)) Paddle1Pos.y = 768.0f - paddleH/2.0f - thickness;
	} if (Paddle2Dir != 0) {
		Paddle2Pos.y += Paddle2Dir * 300.0f * deltaTime;

		//Boundary conditions for the paddle; prevent it from moving offscreen
		if (Paddle2Pos.y < (paddleH/2.0f + thickness)) Paddle2Pos.y = paddleH/2.0f + thickness;
		else if (Paddle2Pos.y > (768.0f - paddleH/2.0f - thickness)) Paddle2Pos.y = 768.0f - paddleH/2.0f - thickness;
	}

	//Update the ball's position via velocity
	BallPos.x += BallVel.x*deltaTime;
	BallPos.y += BallVel.y*deltaTime;

	//Ball collision
	//Walls
	//If the ball collides with the top wall, negate y velocity
	if (BallPos.y <= thickness && BallVel.y < 0.0f) BallVel.y *= -1;
	//bottom wall, negate y velocity
	if (BallPos.y >= 768.0f-thickness && BallVel.y > 0.0f) BallVel.y *= -1;

	//Paddles
	//Left Paddle, negate x velocity
	float diff1 = Paddle1Pos.y - BallPos.y;
	diff1 = (diff1>0.0f) ? diff1 : -diff1;
	if (diff1 <= paddleH/2.0f && //our y-difference is small enough
		BallPos.x <= 25.0f && BallPos.x >= 20.0f && //Ball is at the correct x-position
		BallVel.x < 0.0f) //The ball is moving left
		BallVel.x *= -1.0f;

	//Right Paddle, negate x velocity
	float diff2 = Paddle2Pos.y - BallPos.y;
	diff2 = (diff2>0.0f) ? diff2 : -diff2;
	if (diff2 <= paddleH/2.0f && BallPos.x &&
		BallPos.x >= 999.0f && BallPos.x <= 1004.0f &&
		BallVel.x > 0.0f)
		BallVel.x *= -1;
	

	//If the ball leaves the screen, the game ends
	else if (BallPos.x <= 0.0f || BallPos.x >= 1024.0f) IsRunning = false;
}

void Game::GenerateOutput() {

	//First clear the back bufferer by specifying a color (Black)
	SDL_SetRenderDrawColor(
		Renderer, //Pointer to the renderer
		0, 0, 0, 255 //RGBA
	);

	//Clear the back buffer to the current draw color
	SDL_RenderClear(Renderer);

	//Set the draw color to white
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);

	//Draw top wall
	SDL_Rect wall{
		0, //Top left x
		0, //Top left y
		1024, //Width
		thickness //Height
	};
	SDL_RenderFillRect(Renderer, &wall);

	//Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(Renderer, &wall);

	//Draw the left paddle
	SDL_Rect paddle {
		static_cast<int>(Paddle1Pos.x),
		static_cast<int>(Paddle1Pos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(Renderer, &paddle);

	//Draw right paddle
	paddle.x = static_cast<int>(Paddle2Pos.x - thickness);
	paddle.y = static_cast<int>(Paddle2Pos.y - paddleH/2);
	SDL_RenderFillRect(Renderer, &paddle);

	//Draw the ball
	SDL_Rect ball {
		static_cast<int>(BallPos.x - thickness/2), //Converts from floas into ints
		static_cast<int>(BallPos.y - thickness/2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(Renderer, &ball);

	//Finally, swap the front and back buffers
	SDL_RenderPresent(Renderer);
}

void Game::Shutdown() {

	//First destroy the window
	SDL_DestroyWindow(Window);
	//Then destroy the renderer
	SDL_DestroyRenderer(Renderer);

	//Then close SDL
	SDL_Quit();
}