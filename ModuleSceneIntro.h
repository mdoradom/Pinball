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

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	ModulePlayer* player;

	PhysBody* sensor;

	PhysBody* scene;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;

	PhysBody* flipFlopLeft;
	PhysBody* flipFlopRight;
	PhysBody* flipFlopLeftAnchor;
	PhysBody* flipFlopRightAnchor;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
