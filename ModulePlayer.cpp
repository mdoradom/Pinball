#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModulePhysics.h"
#include "SDL\include\SDL.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

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

	// Inicializar scoreTexture
	scoreTexture = App->fonts->LoadText("0", { 255, 255, 255 });

	score = 0;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	// Liberar la textura del puntaje
	if (scoreTexture != nullptr) {
		App->textures->Unload(scoreTexture);
		scoreTexture = nullptr;
	}

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
update_status ModulePlayer::Update(){

	// Dibujar la textura del marcador en la esquina superior derecha
	App->renderer->BlitText(scoreTexture, SCREEN_WIDTH - 210, 85);

	return UPDATE_CONTINUE;
}

void ModulePlayer::IncreaseScore(int points) {

	score += points;

	LOG("sumas puntos uwu, Puntaje actualizado: %d", score);

	// Actualizar la textura del puntaje
	char scoreText[10];  // Asegurarse de q el tamaño sea suficiente
	sprintf_s(scoreText, "%d", score);  // Convierte el puntaje a cadena

	// Libera la textura antigua antes de cargar la nueva
	if (scoreTexture != nullptr) {
		App->textures->Unload(scoreTexture);
		LOG("scoreTexture liberado antes de cargar el nuevo");
	}

	// Carga la nueva textura del puntaje
	scoreTexture = App->fonts->LoadText(scoreText, { 255, 255, 255 });

	App->audio->PlayFx(pointSound);
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB) {

	LOG("Collision detected!");
	//Aumentar la puntuación en cualquier colisión de colliders.
	IncreaseScore(10);
}



