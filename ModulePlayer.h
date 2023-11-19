#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void moveFlipFlops();
	void launchBall();

public:

	// Fx
	unsigned int flopSound;
	unsigned int boostSound;
	unsigned int pointSound;

	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	
	int score;  // Variable para almacenar la puntuaci�n.
};