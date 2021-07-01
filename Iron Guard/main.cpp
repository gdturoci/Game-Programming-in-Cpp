#include "Game.h"

int main(int argc, char** argv) {

	//Create the game
	Game game;

	//If startup was successful run the game
	if (game.Initialize()) game.RunLoop();

	//End the game
	game.Shutdown();

	return 0;
}