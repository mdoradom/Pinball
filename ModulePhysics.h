#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define RAD_PER_DEG ((float)180/b2_pi)
#define DEG_PER_RAD ((float)b2_pi/180)

#define RAD_TO_DEG(r) ((float) RAD_PER_DEG * r)
#define DEG_TO_RAD(r) ((float) DEG_PER_RAD * r)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

// types of bodies
enum bodyType {
	DYNAMIC,
	STATIC,
	KINEMATIC
};

enum class ColliderType {
	POINTS,
	UNKNOWN
	// ..
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	ColliderType ctype;

	void ApplyBoost(float boostForce) {
		
		if (body) {
			// Obtener la velocidad actual del cuerpo
			b2Vec2 currentVelocity = body->GetLinearVelocity();

			// Normalizar la velocidad actual (manteniendo la dirección) y luego ajustar la magnitud con el impulso
			b2Vec2 newVelocity = currentVelocity;
			newVelocity.Normalize();
			newVelocity *= boostForce;

			// Establecer la nueva velocidad al cuerpo
			body->SetLinearVelocity(newVelocity);
		}
	}

	void PhysBody::ApplyVerticalImpulse(float impulse) {
		if (body) {
			b2Vec2 currentVelocity = body->GetLinearVelocity();
			b2Vec2 newVelocity = b2Vec2(currentVelocity.x, -impulse); // Invertir el impulso
			body->SetLinearVelocity(newVelocity);
		}
	}

};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType bodyType);
	PhysBody* CreateCirclePoints(int x, int y, int radious, bodyType type);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType bodyType);
	PhysBody* CreateRectanglePoints(int x, int y, int width, int height, bodyType bodyType);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size);
	PhysBody* CreateChainPoints(int x, int y, int* points, int size, bodyType bodyType);
	b2RevoluteJoint* CreateRevoluteJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, float lowerAngle, float upperAngle, float referenceAngle, bool collideConnected, bool enableLimit);
	b2PrismaticJoint* CreatePrismaticJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, float lowerTranslation, float upperTranslation, bool collideConnected, bool enableLimit, b2Vec2 axis);



	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

private:

	bool debug;

	b2World* world;
	b2Body* ground;

	// Mouse joint
	b2MouseJoint* mouse_joint;
	b2Body* mouse_body;
};