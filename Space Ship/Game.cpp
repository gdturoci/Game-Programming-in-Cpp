#include "Game.h"
#include "SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "BGSpriteComponent.h"

Game::Game():
	Window(nullptr),
	Renderer(nullptr),
	IsRunning(true),
	UpdatingActors(false) {}

bool Game::Initialize() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	Window = SDL_CreateWindow("Space Ship", 100, 100, 1024, 768, 0);
	if (!Window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!Renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	IMG_Init(IMG_INIT_PNG);
	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();
	TicksCount = SDL_GetTicks();
	return true;
}

void Game::RunLoop() {
	while (IsRunning) {
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
				IsRunning = false;
				break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) IsRunning = false;

	mShip->ProcessKeyboard(state);
}

void Game::UpdateGame() {

	//Compute delta Time
	//Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), TicksCount+16));

	float deltaTime = (SDL_GetTicks() - TicksCount)/1000.0f;
	if (deltaTime>0.05f) deltaTime = .05f;
	TicksCount = SDL_GetTicks();

	//Update all actors
	UpdatingActors = true;
	for (auto actor : Actors) actor->Update(deltaTime);
	UpdatingActors = false;

	//Move any pending actors to Actors
	for (auto pending : PendingActors) Actors.emplace_back(pending);
	PendingActors.clear();

	//Add any dead actors to a temp vector
	vector<Actor*> deadActors;
	for (auto actor : Actors) {
		if (actor->GetState() == Actor::EDead) deadActors.emplace_back(actor);
	}

	//Delete dead actors, removing them from Actors
	for (auto actor : deadActors) delete actor;
}

void Game::GenerateOutput() {
	
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);

	//Draw all sprite components
	for (auto sprite : Sprites) sprite->Draw(Renderer);

	SDL_RenderPresent(Renderer);
}

void Game::LoadData() {
	// Create player's ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);

	// Create actor for the background (this doesn't need a subclass)
	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));

	// Create the "far back" background
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);
	// Create the closer background
	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);
}

void Game::UnloadData() {
	//Delete actors
	while (!Actors.empty()) delete Actors.back();

	//Destroy textures
	for (auto x : Textures) SDL_DestroyTexture(x.second);
	Textures.clear();
}

SDL_Texture* Game::GetTexture(const string& fileName) {

	SDL_Texture* tex = nullptr;

	//Is the texture already in the map?
	auto iter = Textures.find(fileName);
	if (iter != Textures.end()) tex = iter->second;
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
		tex = SDL_CreateTextureFromSurface(Renderer, surf);
		SDL_FreeSurface(surf);
		if (!tex) {
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		Textures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::Shutdown() {
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {

	//If updating actors, need to add to pending
	if (UpdatingActors) PendingActors.emplace_back(actor);
	else Actors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor) {

	//Check pending actors
	auto iter = find(PendingActors.begin(), PendingActors.end(), actor);
	if (iter != PendingActors.end()) {
		//Swap to end of vector and pop off (avoid erase copies)
		iter_swap(iter, PendingActors.end()-1);
		PendingActors.pop_back();
	}

	//Check actors
	iter = find(Actors.begin(), Actors.end(), actor);
	if (iter != Actors.end()) {
		//Swap to end of vector and pop off (avoid erase copies)
		iter_swap(iter, Actors.end()-1);
		Actors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	//Find the insertion point in the sorted vector
	//The first element with a higher draw order than me
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = Sprites.begin();
	for (; iter != Sprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) break;
	}

	//Inserts element before position of iterator
	Sprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	//We can't swap because it ruins ordering
	auto iter = find(Sprites.begin(), Sprites.end(), sprite);
	Sprites.erase(iter);
}