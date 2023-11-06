#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "SDL\include\SDL.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
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
		LOG("Лорем Ипсум");
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
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



