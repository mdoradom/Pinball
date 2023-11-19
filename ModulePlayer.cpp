#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModulePhysics.h"
#include "SDL\include\SDL.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	// Cargar Fx
	flopSound = App->audio->LoadFx("pinball/audio/Fx/flops-sound.ogg");
	boostSound = App->audio->LoadFx("pinball/audio/Fx/boost.ogg");
	pointSound = App->audio->LoadFx("pinball/audio/Fx/bonus.ogg");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

void ModulePlayer::moveFlipFlops() {


	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {

		App->scene_intro->flipFlopLeft->body->ApplyForce(
			{ 0,120 }, 
			{ 0,0 }, 
			true
		);

		App->audio->PlayFx(flopSound);

		LOG("Лорем Ипсум");
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {

		App->scene_intro->flipFlopRight->body->ApplyForce(
			{ 0,-80 }, 
			{ 0,0 },
			true
		);

		App->audio->PlayFx(flopSound);

		LOG("ole la derecha ARRIBA ESPAÑIA");
	}

}

void ModulePlayer::launchBall() {

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		App->audio->PlayFx(boostSound);
		App->scene_intro->launcher->ApplyVerticalImpulse(50);
		LOG("Pulsasto espacio ermano");
	}

}

// Update: draw background
update_status ModulePlayer::Update()
{
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB) {

}