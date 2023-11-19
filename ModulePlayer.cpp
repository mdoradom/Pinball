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
	const float initialImpulse = 5.0f;
	const float maxImpulse = 100.0f;
	const float impulseIncrement = 0.2f;

	static float currentImpulse = initialImpulse;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		currentImpulse = initialImpulse;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		if (currentImpulse < maxImpulse) {
			currentImpulse += impulseIncrement;
		}
		LOG("Impulso actual: %f", currentImpulse);
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
		//currentImpulse / 10;
		App->scene_intro->launcher->ApplyVerticalImpulse(currentImpulse);
		LOG("Impulso aplicado: %f", currentImpulse);
		App->audio->PlayFx(boostSound);
	}
}

// Update: draw background
update_status ModulePlayer::Update()
{
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB) {

}