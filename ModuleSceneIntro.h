#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePlayer.h"
#include "SDL/include/SDL_render.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void IncreaseScore(int points);
	void BallCounter(int balls);

public:
	p2List<PhysBody*> circles;

	ModulePlayer* player;

	PhysBody* sensor;

	PhysBody* scene;
	PhysBody* launcher;

	bool sensed;

	SDL_Texture* box;
	SDL_Texture* rick;

	SDL_Texture* map;
	SDL_Texture* flipFlopLeftTexture;
	SDL_Texture* flipFlopRightTexture;
	SDL_Texture* ball;
	SDL_Texture* elevator;

	PhysBody* flipFlopLeft;
	PhysBody* flipFlopRight;
	PhysBody* flipFlopLeftAnchor;
	PhysBody* flipFlopRightAnchor;
	PhysBody* launcherAnchor;

	uint bonus_fx;
	int score;
	int bolas;
	p2Point<int> ray;
	bool ray_on;
	unsigned int pointSound;
	unsigned int boostSound;
	bool spawnOneBall = false;

	// Puntero a scoreTexture
	SDL_Texture* scoreTexture = nullptr;
	SDL_Texture* ballTexture = nullptr;
};
