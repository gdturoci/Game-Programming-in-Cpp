#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>
#include <vector>
using namespace std;

class Game {
public:
	
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const string& fileName);

private:

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	//Map of textures loaded
	unordered_map<string, SDL_Texture*> Textures;

	//All the actors in the game
	vector<class Actor*> Actors;
	//Any pending actors
	vector<class Actor*> PendingActors;

	//All the sprite components drawn
	vector<class SpriteComponent*> Sprites;

	SDL_Window* Window;
	SDL_Renderer* Renderer;
	Uint32 TicksCount;
	bool IsRunning;

	//Track if we're updating actors right now
	bool UpdatingActors;

	//Game-specific
	class Ship* mShip; //Player's ship
};
