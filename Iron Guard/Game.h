#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
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

	class Grid* GetGrid() {return myGrid;}
	vector<class Enemy*>& GetEnemies() {return myEnemies;}
	class Enemy* GetNearestEnemy(const Vector2& pos);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	//Map of textures loaded
	unordered_map<string, SDL_Texture*> myTextures;

	//All the actors in the game
	vector<class Actor*> myActors;
	//Any pending actors
	vector<class Actor*> myPendingActors;

	//All the sprite components drawn
	vector<class SpriteComponent*> mySprites;

	SDL_Window* myWindow;
	SDL_Renderer* myRenderer;
	Uint32 myTicksCount;
	bool myIsRunning;
	//Track if we're updating actors right now
	bool myUpdatingActors;

	//Game-specific
	vector<class Enemy*> myEnemies;
	class Grid* myGrid;
	float myNextEnemy;

};