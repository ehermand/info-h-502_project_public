#include "PhysicsEngine.hpp"

#include <glm/gtc/matrix_transform.hpp>

PhysicsEngine::PhysicsEngine() {
	// Bullet initialization
	init();
}

PhysicsEngine::~PhysicsEngine() {
	// Clean up
	delete mDynamicsWorld;
	delete mSolver;
	delete mCollisionDispatcher;
	delete mCollisionConfiguration;
	delete mBroadphase;
}

void PhysicsEngine::update(float stepSize) {
	mDynamicsWorld->stepSimulation(stepSize, 10);
	
	for (int i = 0; i < mEntities.size(); i++) {
		btTransform trans;
		glm::mat4 mat;
		mRigidBodies[i]->getMotionState()->getWorldTransform(trans);
		trans.getOpenGLMatrix(glm::value_ptr(mat));
		mEntities[i]->updatePose(mat);

	}
}

void PhysicsEngine::resetDefaultState() {
	for (int i = 0; i < mEntities.size(); i++) {
		btTransform trans;
		glm::mat4 mat = mEntities[i]->getDefaultModelMatrix();
		trans.setFromOpenGLMatrix(glm::value_ptr(mat));

		mRigidBodies[i]->btCollisionObject::activate();
		mRigidBodies[i]->setWorldTransform(trans);
		mRigidBodies[i]->setAngularVelocity(btVector3(0, 0, 0));
		mRigidBodies[i]->setLinearVelocity(btVector3(0, 0, 0));
	}
}

void PhysicsEngine::addGround() {
	// Infinite plane defined by normal
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

	btDefaultMotionState* groundMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	mDynamicsWorld->addRigidBody(groundRigidBody);
}

void PhysicsEngine::addLane() {
	btCollisionShape* lane1Shape = new btStaticPlaneShape(btVector3(1, 0, 0), 0);

	btDefaultMotionState* lane1MotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-0.53728, 0, 0)));

	btRigidBody::btRigidBodyConstructionInfo lane1RigidBodyCI(0, lane1MotionState, lane1Shape, btVector3(0, 0, 0));
	btRigidBody* lane1RigidBody = new btRigidBody(lane1RigidBodyCI);

	btCollisionShape* lane2Shape = new btStaticPlaneShape(btVector3(-1, 0, 0), 0);

	btDefaultMotionState* lane2MotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0), btVector3(0.53865, 0, 0)));

	btRigidBody::btRigidBodyConstructionInfo lane2RigidBodyCI(0, lane2MotionState, lane2Shape, btVector3(0, 0, 0));
	btRigidBody* lane2RigidBody = new btRigidBody(lane2RigidBodyCI);

	btCollisionShape* lane3Shape = new btStaticPlaneShape(btVector3(0, 0, 1), 0);

	btDefaultMotionState* lane3MotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -12.4681)));

	btRigidBody::btRigidBodyConstructionInfo lane3RigidBodyCI(0, lane3MotionState, lane3Shape, btVector3(0, 0, 0));
	btRigidBody* lane3RigidBody = new btRigidBody(lane3RigidBodyCI);

	mDynamicsWorld->addRigidBody(lane1RigidBody);
	mDynamicsWorld->addRigidBody(lane2RigidBody);
	mDynamicsWorld->addRigidBody(lane3RigidBody);
}

void PhysicsEngine::addBowlingPin(Entity *entity) {
	// Create collision shape
	btCollisionShape* collisionShape = new btCylinderShape(btVector3(0.121f/2, 0.381f/2, 0.121f/2));

	btCompoundShape* compoundShape = new btCompoundShape();
	btTransform localTrans;
	localTrans.setIdentity();
	// To shift the center of mass
	glm::vec3 cmOffset(0.0f, 0.381f/6, 0.0f);
	localTrans.setOrigin(btVector3(cmOffset.x, cmOffset.y, cmOffset.z));
	compoundShape->addChildShape(localTrans, collisionShape);

	// Motion state
	btTransform trans;
	glm::mat4 mat = entity->getDefaultModelMatrix();
	trans.setFromOpenGLMatrix(glm::value_ptr(mat));

	btDefaultMotionState* motionState = new btDefaultMotionState(trans);

	// Rigid body
	float mass = 1.5f;
	btVector3 localInertia(0, 0, 0);
	compoundShape->calculateLocalInertia(mass, localInertia); // Calculate local inertia
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, compoundShape, localInertia);
	btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);

	// Add rigid body to the world
	mDynamicsWorld->addRigidBody(rigidBody);

	mRigidBodies.push_back(rigidBody);
	mEntities.push_back(entity);
}

void PhysicsEngine::addBall(Entity *entity) {
	glm::vec3 position = entity->getPosition();
	// Create collision shape
	btCollisionShape* collisionShape = new btSphereShape(0.1091565f);

	// Motion state
	glm::quat orientation(0.0f, 0.0f, 0.0f, 1.0f);
	btDefaultMotionState* motionState =
		new btDefaultMotionState(btTransform(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w),
			btVector3(position.x, position.y, position.z)));

	// Rigid body
	float mass = 7.26f;
	btVector3 localInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, localInertia); // Calculate local inertia
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, localInertia);
	btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);

	// Add rigid body to the world
	mDynamicsWorld->addRigidBody(rigidBody);
	mBall = rigidBody; mBallEntity = entity;
	mRigidBodies.push_back(rigidBody);
	mEntities.push_back(entity);
}

void PhysicsEngine::launchBall(float force, glm::vec3 position, glm::vec3 direction) {
	// Reset default state
	btTransform trans;
	glm::mat4 mat = glm::translate(glm::mat4(1.0f), position);
	trans.setFromOpenGLMatrix(glm::value_ptr(mat));

	mBall->btCollisionObject::activate();
	mBall->setWorldTransform(trans);
	mBall->setAngularVelocity(btVector3(0, 0, 0));
	mBall->setLinearVelocity(btVector3(0, 0, 0));

	// Apply impulse
	mBall->applyCentralImpulse(btVector3(direction.x, direction.y, direction.z)*force);
}

void PhysicsEngine::init() {
	// Build the broadphase
	mBroadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mCollisionDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

	// Set up the actual physics solver
	mSolver = new btSequentialImpulseConstraintSolver;

	// Set up the world
	mDynamicsWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
	mDynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
}
