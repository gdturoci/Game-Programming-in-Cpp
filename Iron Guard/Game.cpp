#include "Game.h"
#include "SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Grid.h"
#include "Enemy.h"
#include "AIComponent.h"
#include "AIState.h"
using namespace std;

Game::Game() :
	myWindow(nullptr),
	myRenderer(nullptr),
	myIsRunning(true),
	myUpdatingActors(false) {}

bool Game::Initialize() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	myWindow = SDL_CreateWindow("Iron Guard", 100, 100, 1024, 768, 0);
	if (!myWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!myRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	IMG_Init(IMG_INIT_PNG);
	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();
	myTicksCount = SDL_GetTicks();
	return true;
}

void Game::RunLoop() {
	while (myIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			myIsRunning = false;
			break;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE]) myIsRunning = false;

	//Build Towers
	if (keyState[SDL_SCANCODE_B]) myGrid->BuildTower();

	//Process mouse
	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	if (SDL_BUTTON(buttons) & SDL_BUTTON_LEFT) myGrid->ProcessClick(x, y);

	myUpdatingActors = true;
	for (auto actor:myActors) actor->ProcessInput(keyState);
	myUpdatingActors = false;
}

void Game::UpdateGame() {
	//Compute delta Time
	//Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), myTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - myTicksCount) / 1000.0f;
	if (deltaTime > 0.05f) deltaTime = .05f;
	myTicksCount = SDL_GetTicks();

	//Update all actors
	myUpdatingActors = true;
	for (auto actor : myActors) actor->Update(deltaTime);
	myUpdatingActors = false;

	//Move any pending actors to Actors
	for (auto pending : myPendingActors) myActors.emplace_back(pending);
	myPendingActors.clear();

	//Add any dead actors to a temp vector
	vector<Actor*> deadActors;
	for (auto actor : myActors) {
		if (actor->GetState() == Actor::EDead) deadActors.emplace_back(actor);
	}

	//Delete dead actors, removing them from myActors
	for (auto actor : deadActors) delete actor;
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(myRenderer, 250, 250, 250, 255);
	SDL_RenderClear(myRenderer);

	//Draw all sprite components
	for (auto sprite : mySprites) sprite->Draw(myRenderer);

	SDL_RenderPresent(myRenderer);
}

void Game::LoadData() {
	myGrid = new Grid(this);

	// For testing AIComponent
	//Actor* a = new Actor(this);
	//AIComponent* aic = new AIComponent(a);
	//// Register states with AIComponent
	//aic->RegisterState(new AIPatrol(aic));
	//aic->RegisterState(new AIDeath(aic));
	//aic->RegisterState(new AIAttack(aic));
	//// Start in patrol state
	//aic->ChangeState("Patrol");
}

void Game::UnloadData() {
	//Delete actors
	while (!myActors.empty()) delete myActors.back();

	//Destroy textures
	for (auto x : myTextures) SDL_DestroyTexture(x.second);
	myTextures.clear();
}

SDL_Texture* Game::GetTexture(const string& fileName) {

	SDL_Texture* tex = nullptr;

	//Is the texture already in the map?
	auto iter = myTextures.find(fileName);
	if (iter != myTextures.end()) tex = iter->second;
	else {
		//Loads an image from a file
		//Returns a pointer to an SDL_Surface if successful
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf) {
			SDL_Log("Failed to load texture file: %s", fileName.c_str());
			return nullptr;
		}

		//Converts an SDL_Surface to an SDL_Texture
		//Returns a pointer to an SDL_Texture if successful
		tex = SDL_CreateTextureFromSurface(myRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex) {
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		myTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::Shutdown() {
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(myRenderer);
	SDL_DestroyWindow(myWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {
	//If updating actors, need to add to pending
	if (myUpdatingActors) myPendingActors.emplace_back(actor);
	else myActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor) {
	//Check pending actors
	auto iter = find(myPendingActors.begin(), myPendingActors.end(), actor);
	if (iter != myPendingActors.end()) {
		//Swap to end of vector and pop off (avoid erase copies)
		iter_swap(iter, myPendingActors.end() - 1);
		myPendingActors.pop_back();
	}

	//Check actors
	iter = find(myActors.begin(), myActors.end(), actor);
	if (iter != myActors.end()) {
		//Swap to end of vector and pop off (avoid erase copies)
		iter_swap(iter, myActors.end() - 1);
		myActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	//Find the insertion point in the sorted vector
	//The first element with a higher draw order than me
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mySprites.begin();
	for (; iter != mySprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) break;
	}

	//Inserts element before position of iterator
	mySprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	//We can't swap because it ruins ordering
	auto iter = find(mySprites.begin(), mySprites.end(), sprite);
	mySprites.erase(iter);
}

Enemy* Game::GetNearestEnemy(const Vector2& pos) {
	Enemy* best = nullptr;

	if (myEnemies.size() > 0) {
		best = myEnemies[0];
		//Save the distance squared of the first enemy, and test if others are closer
		float bestDistSq = (pos - myEnemies[0]->GetPosition()).LengthSq();
		for (size_t x = 1; x < myEnemies.size(); ++x) {
			float newDistSq = (pos - myEnemies[x]->GetPosition()).LengthSq();
			if (newDistSq < bestDistSq) {
				bestDistSq = newDistSq;
				best = myEnemies[x];
			}
		}
	}

	return best;
}