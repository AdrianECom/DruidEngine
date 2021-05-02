#pragma once

#include "Core/ObjectBase.hpp"
#include "Scene/Component.hpp"
#include "Events/Event.hpp"
#include "Config/ConfigMap.hpp"
namespace DE {

template<class K, class V> class HashMap;
template<class T> class List;
class Component;
class Transform;
class Scene;

EVENT_DECLARATION_BEGIN(EventOnDestroy)
EVENT_DECLARATION_END(EventOnDestroy)

class GameObject : public ObjectBase {
private:

	using ComponentsMap = HashMap<ClassId, List<Component*>*>;
	ComponentsMap* mComponentsMap;

	List<Component*>* getComponents(ClassId classId);
	Component* getFirstComponent(ClassId classId);

	 bool mIsActive;

	 Scene* mScene;
	 bool mIsStatic;
	 Transform* mTransform;
	 String mTag;
	 bool mIsPendingToBeDestroyed;
	 bool mIsDestroyed;
	 bool mShouldPersist;

public:

	GENERATE_METADATA(GameObject);

	GameObject();
	virtual ~GameObject() override;;

	GET_SET(Scene);
	GET_SET(IsStatic);
	GET(Transform);
	GET_SET(Tag);
	GET(IsPendingToBeDestroyed);
	GET(IsDestroyed);
	GET_SET(ShouldPersist);

	virtual void init();

	void addComponent(Component* component, ClassId classId);
	void removeComponent(Component* component, ClassId classId);

	template<class T>
	void addComponent(T* component) {
		GameObject::addComponent(component, T::getClassIdStatic());
	}

	template<class T>
	void removeComponent(T* component) {
		GameObject::removeComponent(component, T::getClassIdStatic());
	}

	template<class T>
	List<T*>* getComponents() {
		return reinterpret_cast<List<T*>*>(GameObject::getComponents(T::getClassIdStatic()));
	}

	template<class T>
	T* getFirstComponent() {
		return dynamic_cast<T*>(GameObject::getFirstComponent(T::getClassIdStatic()));
	}

	bool isActive() const {
		return (mIsDestroyed || mIsPendingToBeDestroyed) ? false : mIsActive;
	};

	void setIsActive(bool isActive);

	void finallyDestroy() {
		mIsDestroyed = true;
		mIsPendingToBeDestroyed = false;
	};

	void destroy();

	virtual void onDestroy() { /*TODO: call script onDestroy here??*/ };

	virtual void save(ConfigMap* configMap, StringRef objectName);
	virtual void load(ConfigMap* configMap, StringRef objectName);
};
}

