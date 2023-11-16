#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	// MAP CREATION ----------------------------------
	
	// Pivot -512, -635
	int scenario[182] = {
		-445, 261,
		1, 262,
		1, 87,
		-8, 103,
		-20, 120,
		-39, 141,
		-65, 163,
		-90, 178,
		-110, 187,
		-128, 193,
		-146, 197,
		-164, 198,
		-164, 168,
		-151, 166,
		-136, 163,
		-117, 156,
		-100, 148,
		-86, 140,
		-74, 132,
		-64, 124,
		-48, 110,
		-29, 87,
		-18, 68,
		-5, 42,
		1, 22,
		1, -459,
		-9, -470,
		-21, -479,
		-32, -483,
		-45, -485,
		-58, -485,
		-58, -521,
		-45, -521,
		-30, -521,
		-13, -516,
		3, -508,
		15, -498,
		25, -486,
		33, -475,
		38, -461,
		38, 262,
		91, 262,
		91, -524,
		86, -536,
		77, -550,
		65, -562,
		50, -571,
		35, -577,
		22, -578,
		-371, -578,
		-401, -576,
		-420, -570,
		-435, -560,
		-441, -554,
		-446, -542,
		-446, -265,
		-442, -259,
		-437, -252,
		-433, -247,
		-429, -242,
		-429, -231,
		-446, -231,
		-446, 22,
		-439, 40,
		-434, 52,
		-427, 65,
		-419, 80,
		-406, 98,
		-392, 114,
		-377, 127,
		-362, 138,
		-343, 149,
		-332, 154,
		-316, 160,
		-299, 164,
		-282, 168,
		-282, 199,
		-299, 197,
		-317, 193,
		-338, 185,
		-353, 178,
		-367, 170,
		-380, 161,
		-395, 150,
		-406, 140,
		-419, 126,
		-428, 115,
		-436, 104,
		-443, 90,
		-446, 84,
		-446, 261
	};

	// Pivot -510, -638
	// Pivot -512, -635
	int collider1[40] = {
		-383, -336,
		-359, -336,
		-359, -388,
		-354, -408,
		-346, -422,
		-336, -432,
		-322, -439,
		-264, -439,
		-264, -464,
		-294, -464,
		-311, -464,
		-326, -463,
		-336, -460,
		-346, -453,
		-358, -443,
		-369, -431,
		-377, -416,
		-382, -403,
		-384, -390,
		-384, -336
	};

	// Pivot -512, -635
	int collider2[28] = {
		-378, -93,
		-384, -96,
		-388, -100,
		-390, -105,
		-390, -175,
		-386, -180,
		-381, -185,
		-376, -186,
		-367, -186,
		-362, -183,
		-300, -106,
		-301, -97,
		-306, -93,
		-375, -93
	};

	// Pivot -512, -635
	int collider3[10] = {
		-396, -13,
		-385, -23,
		-336, 26,
		-346, 36,
		-395, -13
	};

	// Pivot -512, -635
	int collider4[28] = {
		-142, -96,
		-145, -102,
		-143, -107,
		-80, -185,
		-75, -186,
		-65, -186,
		-59, -181,
		-55, -175,
		-55, -106,
		-57, -100,
		-61, -96,
		-65, -93,
		-136, -93,
		-141, -95
	};

	// Pivot -512, -635
	int collider5[10] = {
		-107, 26,
		-98, 36,
		-48, -14,
		-59, -21,
		-107, 26
	};

	App->scene_intro->scene = App->physics->CreateChain(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5f, scenario, 181);
	App->scene_intro->scene = App->physics->CreateChain(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5f, collider1, 39);
	App->scene_intro->scene = App->physics->CreateChain(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5f, collider2, 27);
	App->scene_intro->scene = App->physics->CreateChain(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5f, collider3, 9);
	App->scene_intro->scene = App->physics->CreateChain(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5f, collider4, 27);
	App->scene_intro->scene = App->physics->CreateChain(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5f, collider5, 9);

	App->scene_intro->launcher = App->physics->CreateRectangle(575, 890, 45, 20, b2_dynamicBody);

	return true; 
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType bodyType)
{
	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 0.5f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType bodyType)
{
	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_staticBody; // TODO poner esto como parámetro para poder crear chains dinámicas y estáticas
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, float lowerAngle, float upperAngle, float referenceAngle, bool collideConnected, bool enableLimit)
{
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = A->body;
	revoluteJointDef.bodyB = B->body;
	revoluteJointDef.collideConnected = collideConnected;
	revoluteJointDef.localAnchorA.Set(anchorA.x, anchorA.y);
	revoluteJointDef.localAnchorB.Set(anchorB.x, anchorB.y);
	revoluteJointDef.referenceAngle = DEG_TO_RAD(referenceAngle);
	revoluteJointDef.enableLimit = enableLimit;
	revoluteJointDef.lowerAngle = -DEG_TO_RAD(lowerAngle);
	revoluteJointDef.upperAngle = DEG_TO_RAD(upperAngle);

	return (b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef);
}

// TODO crear la funcion para crear el joint que restrinja el movimiento sobre un eje
// b2Vec2(0, 1) eje Y
// b2Vec2(1, 0) eje X
b2PrismaticJoint* ModulePhysics::CreatePrismaticJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, float lowerTranslation, float upperTranslation, bool collideConnected, bool enableLimit, b2Vec2 axis) {

	b2PrismaticJointDef prismaticJointDef;
	prismaticJointDef.bodyA = A->body;
	prismaticJointDef.bodyB = B->body;
	prismaticJointDef.collideConnected = collideConnected;
	prismaticJointDef.localAnchorA.Set(anchorA.x, anchorA.y);
	prismaticJointDef.localAnchorB.Set(anchorB.x, anchorB.y);
	prismaticJointDef.enableLimit = enableLimit;
	prismaticJointDef.lowerTranslation = lowerTranslation;
	prismaticJointDef.upperTranslation = upperTranslation;
	prismaticJointDef.localAxisA = axis;

	return (b2PrismaticJoint*)world->CreateJoint(&prismaticJointDef);
}


// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
					break;
				}
				

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
					break;
				}

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					break;
				}
				

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
					break;
				}
				
			}

			// DONE 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				// test if the current body contains mouse position
				b2Vec2 p = { PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) };
				if (f->GetShape()->TestPoint(b->GetTransform(), p) == true)
				{

					// If a body was selected we will attach a mouse joint to it
					// so we can pull it around

					// The variable "b2Body* mouse_body;" is defined in the header ModulePhysics.h 
					// We need to keep this body throughout several game frames; you cannot define it as a local variable here. 
					mouse_body = b;

					// Get current mouse position
					b2Vec2 mousePosition;
					mousePosition.x = p.x;
					mousePosition.y = p.y;

					// Define new mouse joint
					b2MouseJointDef def;
					def.bodyA = ground; // First body must be a static ground
					def.bodyB = mouse_body; // Second body will be the body to attach to the mouse
					def.target = mousePosition; // The second body will be pulled towards this location
					def.dampingRatio = 0.5f; // Play with this value
					def.frequencyHz = 2.0f; // Play with this value
					def.maxForce = 200.0f * mouse_body->GetMass(); // Play with this value

					// Add the new mouse joint into the World
					mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
				}
			}
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// DONE 2: If a body was selected, create a mouse joint
	// using mouse_joint class property
	if (mouse_body != nullptr && mouse_joint != nullptr)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			// Get new mouse position and re-target mouse_joint there
			b2Vec2 mousePosition;
			mousePosition.x = PIXEL_TO_METERS(App->input->GetMouseX());
			mousePosition.y = PIXEL_TO_METERS(App->input->GetMouseY());
			mouse_joint->SetTarget(mousePosition);

			// DONE 3: If the player keeps pressing the mouse button, update
			// target position and draw a red line between both anchor points
			// Draw a red line between both anchor points
			App->renderer->DrawLine(METERS_TO_PIXELS(mouse_body->GetPosition().x), METERS_TO_PIXELS(mouse_body->GetPosition().y), App->input->GetMouseX(), App->input->GetMouseY(), 255, 0, 0);
		}
	}

	// DONE 4: If the player releases the mouse button, destroy the joint
	if (mouse_body != nullptr && mouse_joint != nullptr)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
		{
			// Tell Box2D to destroy the mouse_joint
			world->DestroyJoint(mouse_joint);

			// DO NOT FORGET THIS! We need it for the "if (mouse_body != nullptr && mouse_joint != nullptr)"
			mouse_joint = nullptr;
			mouse_body = nullptr;
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}