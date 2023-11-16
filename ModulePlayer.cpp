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
	flopSound = App->audio->LoadFx("pinball/audio/Fx/bonus.ogg");
	boostSound = App->audio->LoadFx("pinball/audio/Fx/boost.ogg");

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
			{ 10,80 }, 
			{ 0,0 }, 
			true
		);

		App->audio->PlayFx(flopSound);

		LOG("Лорем Ипсум");
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {

		App->scene_intro->flipFlopRight->body->ApplyForce(
			{ -10,-80 }, 
			{ 0,0 }, 
			true
		);

		App->audio->PlayFx(flopSound);

		LOG("ole la derecha ARRIBA ESPAÑIA");
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

		App->scene_intro->launcher->ApplyVerticalImpulse(50);

		App->audio->PlayFx(boostSound);

		LOG("ole la derecha ARRIBA ESPAÑIA");

	}

}

void ModulePlayer::launchBall() {

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		LOG("Pulsasto espacio ermano");
	}

}

// Update: draw background
update_status ModulePlayer::Update()
{
	return UPDATE_CONTINUE;
}



