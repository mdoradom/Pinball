#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

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


	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	player = App->player;

	//// Create flip flops
	//flipFlopLeft = App->physics->CreateRectangle(400, 650, 80, 20, b2_dynamicBody);
	//flipFlopRight = App->physics->CreateRectangle(600, 650, 80, 20, b2_dynamicBody);

	//// Create anchors
	//flipFlopLeftAnchor = App->physics->CreateRectangle(400, 650, 1, 1, b2_staticBody);
	//flipFlopRightAnchor = App->physics->CreateRectangle(600, 650, 1, 1, b2_staticBody);
	//launcherAnchor = App->physics->CreateRectangle(400, 400, 1, 1, b2_staticBody);

		// Deactivate anchors collisions
	//flipFlopLeftAnchor->body->GetFixtureList()->SetSensor(true);
	//flipFlopRightAnchor->body->GetFixtureList()->SetSensor(true);
	//launcherAnchor->body->GetFixtureList()->SetSensor(true);

	//// Create Joints
	//App->physics->CreateRevoluteJoint(flipFlopLeftAnchor, {0,0}, flipFlopLeft, {-1,0}, 30, 15, 0, false, true);
	//App->physics->CreateRevoluteJoint(flipFlopRightAnchor, {0,0}, flipFlopRight, {1,0}, 15, 30, 0, false, true);

	float lowerLimit = 350;
	float upperLimit = 450;
	//App->physics->CreatePrismaticJoint(launcherAnchor, { 0,0 }, App->scene_intro->launcher, { 0,0 }, lowerLimit, upperLimit, false, false, {0,1});

	App->audio->PlayMusic("pinball/audio/background-music.wav", 0.0f);

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

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	// Draw map
	App->renderer->Blit(map, 0, 0, NULL, 1.0f, 0);

	// Draw flip flops
	//int flipFlopLeftX, flipFlopLeftY, flipFlopRightX, flipFlopRightY;
	//flipFlopLeft->GetPosition(flipFlopLeftX, flipFlopLeftY);
	//flipFlopRight->GetPosition(flipFlopRightX, flipFlopRightY);

	//App->renderer->Blit(flipFlopLeftTexture, flipFlopLeftX, flipFlopLeftY, NULL, 1.0f, flipFlopLeft->GetRotation());
	//App->renderer->Blit(flipFlopRightTexture, flipFlopRightX, flipFlopRightY, NULL, 1.0f, flipFlopRight->GetRotation());

	//player->moveFlipFlops();
	player->launchBall();

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25, b2_dynamicBody ));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50, b2_dynamicBody));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
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

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
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
