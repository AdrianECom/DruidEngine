#include <TimeUtils.hpp>
#include "PhysicsEngine.hpp"
#include "RigidBody.hpp"
#include "Collider.hpp"
#include "QuadTree.hpp"
#include "GameObject.hpp"
#include "Memory.hpp"
#include "List.hpp"
#include "Settings.hpp"
#include "ContactsManager.hpp"
#include "RenderEngine.hpp"
#include "Renderer.hpp"

namespace DE {

// ---------------------------------------------------------------------------

PhysicsEngine::PhysicsEngine() : DE_Class(), Singleton() {
	mRigidBodies = nullptr;
	mQuadTree = nullptr;
}

// ---------------------------------------------------------------------------

PhysicsEngine::~PhysicsEngine() {
	Memory::free<ContactsManager>(ContactsManager::getInstance());
}

// ---------------------------------------------------------------------------

void PhysicsEngine::addRigidBody(RigidBody *rigidBody) {
	mRigidBodies->pushBack(rigidBody);
}

// ---------------------------------------------------------------------------

void PhysicsEngine::addCollider(RigidBody *rigidBody, Collider *collider) {
	rigidBody->setCollider(collider);
	mQuadTree->addCollider(collider);
}

// ---------------------------------------------------------------------------

void PhysicsEngine::rayCastQuery(const Vector3 &lineStart, const Vector3 &lineEnd, List<GameObject*> *outList) {
	RenderEngine::getInstance()->drawLine(lineStart, lineEnd);
	mQuadTree->rayCastQuery(lineStart, lineEnd, outList);
}

// ---------------------------------------------------------------------------

void PhysicsEngine::internalRemoveRigidBody(const Iterator *it) {
	auto castedIt = it->cast<RigidBody*>();
	mRigidBodies->remove(*castedIt);

	RigidBody* rigidBody = (*castedIt).get();
	rigidBody->setDestroyed();

	/*Memory::free<Collider>(rigidBody->getCollider());

	Memory::free<RigidBody>(rigidBody);*/
}

// ---------------------------------------------------------------------------

void PhysicsEngine::init(f32 sceneSize) {
	TRACE();

	ContactsManager::getInstance()->init();

	mRigidBodies = Memory::allocate<List<RigidBody*>>();
	mRigidBodies->init();

	mRigidBodiesToFree = Memory::allocate<List<RigidBody*>>();
	mRigidBodiesToFree->init();

	mQuadTree = Memory::allocate<QuadTree>();
	mQuadTree->init(sceneSize);

}

// ---------------------------------------------------------------------------

void PhysicsEngine::step(f32 deltaTime) {

	// TODO : Move this to settings.
	u32 maxIterations = 3.0f; // how many times we want to divide dt

	f32 dt = deltaTime / maxIterations;

	FOR_RANGE(iterations, 0, maxIterations){
		FOR_LIST (it, mRigidBodies) {
			//it.get()->getGameObject()->getComponents<Renderer>()->get(0)->setColor(Vector4(0,0,0,1));
			if (it.get()->isActive()) {
				//it.get()->getGameObject()->getComponents<Renderer>()->get(0)->setColor(Vector4(0,0,0,1));

				// TODO: move dynamic objects to a dynamic list. Don't iterate over static objects.
				if (!it.get()->getGameObject()->isStatic() && it.get()->isSimulate()) {
					it.get()->saveState();
					it.get()->integrate(dt);
				}
			} else if (it.get()->isPendingToBeDestroyed()) {
				mRigidBodiesToFree->pushBack(it.get());
				internalRemoveRigidBody(&it);
			}
		}

		mQuadTree->update();

		mQuadTree->setStatus(ColliderStatus::STATUS_NONE); // Reset status and try again.

		updateContacts();

		FOR_LIST (it, mRigidBodiesToFree) {
			Memory::free<Collider>(it.get()->getCollider());
			Memory::free<RigidBody>(it.get());
		}

		mRigidBodiesToFree->clear();

	}


}

// ---------------------------------------------------------------------------

void PhysicsEngine::updateContacts() {
	ContactsManager::getInstance()->updateContacts();
}

// ---------------------------------------------------------------------------

void PhysicsEngine::terminate() {
	TRACE();

	if(mRigidBodies){
		FOR_LIST(it, mRigidBodies) {
			Memory::free<Collider>(it.get()->getCollider());
			Memory::free<RigidBody>(it.get());
		}

		Memory::free<List<RigidBody*>>(mRigidBodies);
	}

	if(mRigidBodiesToFree){
		FOR_LIST(it, mRigidBodiesToFree) {
			Memory::free<Collider>(it.get()->getCollider());
			Memory::free<RigidBody>(it.get());
		}

		Memory::free<List<RigidBody*>>(mRigidBodiesToFree);
	}

	Memory::free<QuadTree>(mQuadTree);
	mQuadTree = nullptr;

	ContactsManager::getInstance()->terminate();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
