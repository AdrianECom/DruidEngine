#pragma once

#include "Core/ObjectBase.hpp"
#include "Events/Event.hpp"
#include "Core/Assert/Assert.hpp"
#include "Core/Singleton.hpp"

namespace DE {

/*
  Macros for (un)susbscribing and sending events.
*/

#define SUBSCRIBE_TO_EVENT(EventClassName, owner, receiver, eventCallback) EventsManager::getInstance()->subscribe<EventClassName>(owner, receiver, eventCallback);
#define UNSUBSCRIBE_TO_EVENT(EventClassName, owner, receiver) EventsManager::getInstance()->unsubscribe<EventClassName>(owner, receiver);
#define SEND_EVENT(owner, instigator, event) EventsManager::getInstance()->send(owner, instigator, &event);

template<class K, class V> class HashMap;
template<class T> class List;

class EventsManager : public ObjectBase, public Singleton<EventsManager>{

private:

	using ReceiverFunctorsMap = HashMap<ObjectBase*, EventFunctor<Event>>;
	using EventReceiversMap = HashMap<ClassId, ReceiverFunctorsMap*>;
	using OwnersReceiversMap = HashMap<ObjectBase*, EventReceiversMap*>;

PRIVATE(OwnersReceiversMap, NONE, OwnersReceiversMap*)

	void removeMapContent();

	void subscribe(ClassId eventClassId, ObjectBase* eventOwner, ObjectBase* eventReceiver, EventCallback eventCallback);
	void unsubscribe(ClassId eventClassId, ObjectBase* eventOwner, ObjectBase* eventReceiver);

public:
	GENERATE_METADATA(CONSTRUCTOR, EventsManager)

	void init();
	void terminate();

	template<class E>
	void subscribe(ObjectBase* eventOwner, ObjectBase* eventReceiver, EventCallback eventCallback){
		if(std::is_base_of<Event, E>::value){
			subscribe(E::getClassIdStatic(), eventOwner, eventReceiver, eventCallback);
		} else {
			ASSERT(false, "ObjectBase must be derived from Event.");
		}
	}

	template<class E>
	void unsubscribe(ObjectBase* eventOwner, ObjectBase* eventReceiver){
		if(std::is_base_of<Event, E>::value){
			unsubscribe(E::getClassIdStatic(), eventOwner, eventReceiver);
		} else {
			ASSERT(false, "ObjectBase must be derived from Event.");
		}
	}

	void send(ObjectBase* eventOwner, ObjectBase* eventInstigator, Event* event);
};
}

