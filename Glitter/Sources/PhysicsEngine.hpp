#pragma once

#include <btBulletDynamicsCommon.h>

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Entity.hpp"

class PhysicsEngine {
public:
	PhysicsEngine();
	~PhysicsEngine();

	void update(float stepSize);
	void resetDefaultState();

	// Add bottom limit
	void addGround();
	// Add left, right, back limits
	void addLane();
	void addBowlingPin(Entity *entity);
	void addBall(Entity *entity);

	void launchBall(float force, glm::vec3 position = glm::vec3(0.0f, 0.3f, -10.0f), glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f));

private:
	btBroadphaseInterface* mBroadphase;
	btDefaultCollisionConfiguration* mCollisionConfiguration;
	btCollisionDispatcher* mCollisionDispatcher;
	btSequentialImpulseConstraintSolver* mSolver;
	btDiscreteDynamicsWorld* mDynamicsWorld;

	std::vector<btRigidBody*> mRigidBodies;
	std::vector<Entity*> mEntities;

	btRigidBody* mBall;
	Entity* mBallEntity;

	void init();
};
