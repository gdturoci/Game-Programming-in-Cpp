#include "Game.h"

int main(int argc, char** argv) {

	//Create the game
	Game game;
	bool success = game.Initialize();

	//If startup was successful run the game
	if (success) game.RunLoop();

	//End the game
	game.Shutdown();

	return 0;
}