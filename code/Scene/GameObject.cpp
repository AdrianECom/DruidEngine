#include "Scene/GameObject.hpp"

#include "Containers/List.hpp"
#include "Containers/HashMap.hpp"

#include "Scene/Component.hpp"
#include "Scene/Transform.hpp"

#include "Events/EventsManager.hpp"
#include "Config/ConfigMap.hpp"
#include "Physics/RigidBody.hpp"
#include "Physics/Collider.hpp"
#include "Graphics/Renderer.hpp"

namespace DE {

GameObject::GameObject() : ObjectBase() {
	mComponentsMap = nullptr;
	mTransform = nullptr;
	mScene = nullptr;
	mIsActive = true;
	mShouldPersist = false;
}

GameObject::~GameObject() {
	Memory::free(mComponentsMap);
	Memory::free(mTransform);
}

void GameObject::addComponent(Component *component, ClassId classId) {
	List<Component*>* list = nullptr;

	if (!mComponentsMap->contains(classId)) {
		list = Memory::allocate<List<Component*>>();
		list->init();
		mComponentsMap->set(classId, list);
	}

	mComponentsMap->get(classId)->pushBack(component);

	component->setGameObject(this);
	component->init();
}

void GameObject::removeComponent(Component *component, ClassId classId) {
	if (mComponentsMap->contains(classId) && ! component->getIsPendingToBeDestroyed() && ! component->getIsDestroyed()) {
		List<Component*>* list = mComponentsMap->get(classId);
		list->remove(list->find(component));
		if (!(component->getIsDestroyed() || component->getIsPendingToBeDestroyed())) {
			component->destroy();
		}
	}
}

void GameObject::init() {
	// TRACE();

	mComponentsMap = Memory::allocate<HashMap<ClassId, List<Component*>*>>();
	mComponentsMap->init();

	mTransform = Memory::allocate<Transform>();
	addComponent(mTransform);

	mTag = "";
}

List<Component*>* GameObject::getComponents(ClassId classId) {
	List<Component*>* components = nullptr;

	if(mComponentsMap->contains(classId)) {
		components = mComponentsMap->get(classId);

		if(components->isEmpty()){
			components = nullptr;
		}
	}

	return components;
}

Component* GameObject::getFirstComponent(ClassId classId) {
	Component* component = nullptr;
	List<Component*>* components = getComponents(classId);

	if(components && !components->isEmpty()) {
		component = components->getFirst().get();
	}

	return component;
}

void GameObject::setIsActive(bool isActive) {
	mIsActive = mIsDestroyed || mIsPendingToBeDestroyed ? false : isActive;

	FOR_LIST(it, mComponentsMap->getValues()) {
		FOR_LIST(itComponent, it.get())
		{
			itComponent.get()->setIsActive(isActive);
		}
	}
};

void GameObject::destroy() {
	mIsPendingToBeDestroyed = true;
	mIsActive = false;

	EventOnDestroy event;
	SEND_EVENT(this, this, event);

	onDestroy();

	FOR_LIST(it, mComponentsMap->getKeys()) {

		ClassId id = it.get();
		auto list = mComponentsMap->get(id);

		FOR_LIST(itComponent, list)
		{
			itComponent.get()->setGameObject(nullptr);
			itComponent.get()->setIsActive(false);
			removeComponent(itComponent.get(), id);
		}

		Memory::free(list);
	}

	mComponentsMap->clear();
};

void GameObject::save(ConfigMap* configMap, StringRef objectName) {

	configMap->setString(objectName + ".class", getClassName());

	configMap->setBool(objectName + ".isStatic", getIsStatic());
	configMap->setBool(objectName + ".shouldPersist", getShouldPersist());

	configMap->setString(objectName + ".tag", getTag());

	Transform *t = getTransform();
	Vector3 worldPosition = t->getWorldPosition();
	Vector3 scale = t->getScale();

	configMap->setF32(objectName + ".worldPosition.x", worldPosition.x);
	configMap->setF32(objectName + ".worldPosition.y", worldPosition.y);
	configMap->setF32(objectName + ".size.width", scale.x);
	configMap->setF32(objectName + ".size.height", scale.y);

	// TODO : this property is not needed.
	Collider *collider = getFirstComponent<Collider>();
	configMap->setBool(objectName + ".hasCollider", collider ? true : false);

	FOR_LIST(it, mComponentsMap->getKeys()) {

		ClassId id = it.get();
		FOR_LIST(itComponent, mComponentsMap->get(id))
		{
			itComponent.get()->save(configMap, objectName);
		}
	}
}

void GameObject::load(ConfigMap* configMap, StringRef objectName) {
	setIsStatic(configMap->getBool(objectName + ".isStatic"));
	setShouldPersist(configMap->getBool(objectName + ".shouldPersist"));

	if(configMap->contains(objectName + ".tag")){
		setTag(configMap->getString(objectName + ".tag"));
	}

	Vector2 worldPosition(configMap->getF32(objectName + ".worldPosition.x"),
			configMap->getF32(objectName + ".worldPosition.y"));

	Vector2 size(configMap->getF32(objectName + ".size.width"), configMap->getF32(objectName + ".size.height"));
	
	getTransform()->setLocalPosition(Vector3(worldPosition.x, worldPosition.y, 0));
	getTransform()->setScale(Vector3(size.x, size.y, 1));

	Renderer* renderer = Memory::allocate<Renderer>();
	addComponent<Renderer>(renderer);
	renderer->load(configMap, objectName);

	if(configMap->getBool(objectName + ".hasCollider")) {
		RigidBody* rigidBody = Memory::allocate<RigidBody>();
		addComponent<RigidBody>(rigidBody);

		Collider* collider = Memory::allocate<Collider>();
		addComponent<Collider>(collider);
		collider->load(configMap, objectName);
	}
}

}
