#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	// Load textures
	map = App->textures->Load("pinball/textures/map.png");
	flipFlopLeftTexture = App->textures->Load("pinball/textures/flipflop_left.png");
	flipFlopRightTexture = App->textures->Load("pinball/textures/flipflop_right.png");
	ball = App->textures->Load("pinball/textures/ball.png");
	elevator = App->textures->Load("pinball/textures/elevator.png");


	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	pointSound = App->audio->LoadFx("pinball/audio/Fx/bonus.ogg");
	boostSound = App->audio->LoadFx("pinball/audio/Fx/boost.ogg");

	App->audio->PlayMusic("pinball/audio/background-music-01.ogg", 0.0f);

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	player = App->player;

	// Create flip flops
	flipFlopLeft = App->physics->CreateRectangle(178, 670, 90, 10, b2_dynamicBody);
	flipFlopRight = App->physics->CreateRectangle(404, 670, 90, 10, b2_dynamicBody);

	//// Create anchors
	flipFlopLeftAnchor = App->physics->CreateRectangle(178, 670, 1, 1, b2_staticBody);
	flipFlopRightAnchor = App->physics->CreateRectangle(404, 670, 1, 1, b2_staticBody);
	launcherAnchor = App->physics->CreateRectangle(575, 890, 1, 1, b2_staticBody);

	// Deactivate anchors collisions
	flipFlopLeftAnchor->body->GetFixtureList()->SetSensor(true);
	flipFlopRightAnchor->body->GetFixtureList()->SetSensor(true);
	launcherAnchor->body->GetFixtureList()->SetSensor(true);

	//// Create Joints
	App->physics->CreateRevoluteJoint(flipFlopLeftAnchor, {0,0}, flipFlopLeft, {-1,0}, 30, 15, 0, false, true);
	App->physics->CreateRevoluteJoint(flipFlopRightAnchor, {0,0}, flipFlopRight, {1,0}, 15, 30, 0, false, true);

	float lowerLimit = 350;
	float upperLimit = 450;
	App->physics->CreatePrismaticJoint(launcherAnchor, { 0,0 }, App->scene_intro->launcher, { 0,0 }, lowerLimit, upperLimit, false, false, {0,1});

	int curvedRect_top_left_pos[16] = {
	160, 171,
	150, 176,
	140, 185,
	132, 195,
	138, 201,
	145, 192,
	152, 184,
	165, 177
	};

	int curvedRect_top_right_pos[16] = {
	465, 165,
	480, 169,
	495, 180,
	503, 199,
	494, 201,
	487, 187,
	476, 177,
	464, 173
	};

	{
		PhysBody* curvedRect_top_left = App->physics->CreateChainScore(0, 0, curvedRect_top_left_pos, 16, bodyType::STATIC);
		curvedRect_top_left->ctype = ColliderType::SCORE15;
		PhysBody* curvedRect_top_right = App->physics->CreateChainScore(0, 0, curvedRect_top_right_pos, 16, bodyType::STATIC);
		curvedRect_top_right->ctype = ColliderType::SCORE15;
		PhysBody* midBall_top_left = App->physics->CreateCircleScore(102, 96, 25, bodyType::STATIC);
		midBall_top_left->ctype = ColliderType::SCORE10;
		PhysBody* smallBall_top_left = App->physics->CreateCircleScore(165, 253, 10, bodyType::STATIC);
		smallBall_top_left->ctype = ColliderType::SCORE20;
		PhysBody* smallBall_mid_left = App->physics->CreateCircleScore(187, 410, 10, bodyType::STATIC);
		smallBall_mid_left->ctype = ColliderType::SCORE20;
		PhysBody* bigBall_mid_left = App->physics->CreateCircleScore(300, 278, 34, bodyType::STATIC);
		bigBall_mid_left->ctype = ColliderType::SCORE5;
		PhysBody* bigBall_top_right = App->physics->CreateCircleScore(410, 246, 34, bodyType::STATIC);
		bigBall_top_right->ctype = ColliderType::SCORE5;
		PhysBody* bigBall_mid_right = App->physics->CreateCircleScore(382, 355, 34, bodyType::STATIC);
		bigBall_mid_right->ctype = ColliderType::SCORE5;
		PhysBody* midRect_top_left = App->physics->CreateRectangleScore(214, 161, 48, 10, bodyType::STATIC);
		midRect_top_left->ctype = ColliderType::SCORE15;
		PhysBody* midRect_mid_left_vertical = App->physics->CreateRectangleScore(162, 296, 10, 28, bodyType::STATIC);
		midRect_mid_left_vertical->ctype = ColliderType::SCORE15;
		PhysBody* midRect_mid_left = App->physics->CreateRectangleScore(148, 309, 36, 10, bodyType::STATIC);
		midRect_mid_left->ctype = ColliderType::SCORE15;
		PhysBody* smallRect_bot_left = App->physics->CreateRectangleScore(193, 550, 30, 10, bodyType::STATIC);
		smallRect_bot_left->ctype = ColliderType::SCORE25;
		PhysBody* smallRect_bot_right = App->physics->CreateRectangleScore(388, 550, 30, 10, bodyType::STATIC);
		smallRect_bot_right->ctype = ColliderType::SCORE25;
	}

	// Create boost colliders
	PhysBody* boostCollider = App->physics->CreateRectangle(95, 250, 40, 100, b2_staticBody);
	boostCollider->body->GetFixtureList()->SetSensor(true);
	boostCollider->ctype = ColliderType::BOOST;

	// Create death colliders
	PhysBody* deathRect = App->physics->CreateRectangle(70+ 220, 892, 440, 2, b2_staticBody);
	deathRect->ctype = ColliderType::DEATH;

	// Create ball
	circles.add(App->physics->CreateCircle(575, 870, 16, b2_dynamicBody));
	circles.getLast()->data->listener = this;
	circles.getLast()->data->ctype = ColliderType::BALL;

	scoreTexture = App->fonts->LoadText("0", { 255, 255, 255 });
	ballTexture = App->fonts->LoadText("5", { 255, 255, 255 });

	score = 0;
	bolas = 5;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	// Unload textures
	App->textures->Unload(ball);
	App->textures->Unload(box);
	App->textures->Unload(rick);

	App->textures->Unload(map);
	App->textures->Unload(flipFlopLeftTexture);
	App->textures->Unload(flipFlopRightTexture);
	App->textures->Unload(elevator);

	if (scoreTexture != nullptr) {
		App->textures->Unload(scoreTexture);
		scoreTexture = nullptr;
	}


	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	// Draw maps
	App->renderer->Blit(map, 0, 0, NULL, 1.0f, 0);

	// Dibujar la textura del marcador en la esquina superior derecha
	App->renderer->BlitText(scoreTexture, SCREEN_WIDTH - 210, 85);
	App->renderer->BlitText(ballTexture, SCREEN_WIDTH - 210, 270);

	// Draw flip flops
	int flipFlopLeftX, flipFlopLeftY, flipFlopRightX, flipFlopRightY;
	flipFlopLeft->GetPosition(flipFlopLeftX, flipFlopLeftY);
	flipFlopRight->GetPosition(flipFlopRightX, flipFlopRightY);

	App->renderer->Blit(flipFlopLeftTexture, flipFlopLeftX, flipFlopLeftY-18, NULL, 1.0f, flipFlopLeft->GetRotation());
	App->renderer->Blit(flipFlopRightTexture, flipFlopRightX, flipFlopRightY-18, NULL, 1.0f, flipFlopRight->GetRotation());

	int launcherX, launcherY;
	launcher->GetPosition(launcherX, launcherY);
	App->renderer->Blit(elevator, launcherX, launcherY, NULL, 1.0f, launcher->GetRotation());

	player->moveFlipFlops();
	player->launchBall();

	if (spawnOneBall) {
		if (bolas > 0) {
			spawnOneBall = false;
			circles.add(App->physics->CreateCircle(575, 870, 16, b2_dynamicBody));
			circles.getLast()->data->listener = this;
			circles.getLast()->data->ctype = ColliderType::BALL;
		}
	}

	if (App->physics->debug) {

		if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 16, b2_dynamicBody ));
			circles.getLast()->data->listener = this;
			circles.getLast()->data->ctype = ColliderType::BALL;
		}

	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);	
		App->renderer->Blit(ball, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}


	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	if (bodyA->ctype == ColliderType::BALL)
	{
		switch (bodyB->ctype)
		{
		case ColliderType::SCORE5:
			LOG("Collision SCORE5");
			IncreaseScore(5);
			LOG("Score: %i", score);
			App->audio->PlayFx(pointSound);

			break;

		case ColliderType::SCORE10:
			LOG("Collision SCORE10");
			IncreaseScore(10);
			LOG("Score: %i", score);
			App->audio->PlayFx(pointSound);

			break;

		case ColliderType::SCORE15:
			LOG("Collision SCORE15");
			IncreaseScore(15);
			LOG("Score: %i", score);
			App->audio->PlayFx(pointSound);

			break;

		case ColliderType::SCORE20:
			LOG("Collision SCORE20");
			IncreaseScore(20);
			LOG("Score: %i", score);	
			App->audio->PlayFx(pointSound);

			break;

		case ColliderType::SCORE25:
			LOG("Collision SCORE25");
			IncreaseScore(25);
			LOG("Score: %i", score);
			App->audio->PlayFx(pointSound);

			break;

		case ColliderType::DEATH:
			LOG("Collision DEATH")
			BallCounter(1);

			break;

		case ColliderType::BOOST:

			App->audio->PlayFx(boostSound);

			bodyA->ApplyVerticalImpulse(-50);

			LOG("Collision BOOST")

			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");

			break;
	}
	}

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}

void ModuleSceneIntro::IncreaseScore(int points) {

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

// pa los comentarios mira lo de arriba que es lo mismo bro
void ModuleSceneIntro::BallCounter(int balls) {

	bolas -= balls;

	LOG("has perdido una bola subnormal ajaajajj, te quedan : %d", bolas);

	char ballText[10];  
	sprintf_s(ballText,"%d", bolas);  

	if (ballTexture != nullptr) {
		App->textures->Unload(ballTexture);
		LOG("ballTexture liberado antes de cargar el nuevo");
	}

	ballTexture = App->fonts->LoadText(ballText, { 255, 255 , 255 });

	spawnOneBall = true;

}