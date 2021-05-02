#pragma once

#include "Core/BasicTypes.hpp"
#include <type_traits>

// --------------------------------------------------------
// CLASS - METADATA MACROS
// --------------------------------------------------------

#define GENERATE_ID_STATIC(...)\
	static ClassId getClassIdStatic(){\
		static ClassId classId = Hash::hash(# __VA_ARGS__); return classId;\
	};

#define GENERATE_ID_VIRTUAL(...)\
	ClassId getClassId() const override {\
		return __VA_ARGS__::getClassIdStatic();\
	};

#define GENERATE_NAME_STATIC(...)\
	static String getClassNameStatic(){\
		static String className = String(# __VA_ARGS__);\
		return className;\
	};

#define GENERATE_NAME_VIRTUAL(...)\
	String getClassName() const override {\
		return __VA_ARGS__::getClassNameStatic();\
	};

#define GENERATE_DYNAMIC_DESTRUCTOR_VIRTUAL(...)\
	virtual void dynamicDestructor() override { this->~__VA_ARGS__(); };

#define GENERATE_METADATA(...)\
	GENERATE_NAME_STATIC(__VA_ARGS__);\
	GENERATE_NAME_VIRTUAL(__VA_ARGS__);\
	GENERATE_ID_STATIC(__VA_ARGS__);\
	GENERATE_ID_VIRTUAL(__VA_ARGS__);\
	GENERATE_DYNAMIC_DESTRUCTOR_VIRTUAL(__VA_ARGS__); 

#define INSTANCEABLE_BY_CLASSNAME(...)\
	Memory::registerClassName<__VA_ARGS__>(__VA_ARGS__::getClassNameStatic());

// --------------------------------------------------------
// MEMBERS, GETTERS AND SETTERS
// --------------------------------------------------------

#define IS_POINTER(Class) std::is_pointer<std::remove_reference<Class>::type>::value
#define IS_ARITHMETIC(Class) std::is_arithmetic<std::remove_reference<Class>::type>::value

#define MEMBER_BASE(BaseName, ...) __VA_ARGS__ m ## BaseName = {};

#define COND_TYPE(Bool, T1, T2) std::conditional<Bool, T1, T2>::type

#define GETTER_TYPE(Var)\
	COND_TYPE(\
		IS_POINTER(decltype(Var)),\
		std::add_const<std::remove_reference<decltype(Var)>::type>::type,\
		COND_TYPE(\
			IS_ARITHMETIC(decltype(Var)),\
			std::remove_reference<decltype(Var)>::type,\
			std::add_const<decltype(Var)>::type\
		)\
	)

#define SETTER_TYPE(Var) GETTER_TYPE(Var) // NOTE: It's the same as the getter.

#define GET(BaseName)\
	GETTER_TYPE(m ## BaseName) get ## BaseName() const { return m ## BaseName; };

#define SET(BaseName)\
	void set ## BaseName (SETTER_TYPE(m ## BaseName) new ## BaseName){ m ## BaseName = new ## BaseName; };

#define GET_SET(BaseName) GET(BaseName) SET(BaseName)

// --------------------------------------------------------
// FOR LOOPS
// --------------------------------------------------------

#define FOR_RANGE(it, start, size) for (i32 it = start; it < size; ++it)
#define FOR_RANGE_COND(it, start, size, condition) for (i32 it = start; it < size && condition; ++it)

#define FOR_ARRAY(it, array) FOR_RANGE(it, 0, array->getLength())
#define FOR_ARRAY_COND(it, array, condition) for (u32 it = 0; it < array->getLength() && condition; ++it)

#define FOR_LIST(it, list) for (auto it = list->getIterator(); !it.isNull(); it.next())
#define FOR_LIST_COND(it, list, cond) for (auto it = list->getIterator(); (!it.isNull()) && cond; it.next())

