#pragma once

#include "Memory/Allocator.hpp"
#include <map>
#include <typeinfo>       // operator typeid
#include <string>
#include <functional>

#define KB (1024.0f)
#define MB (KB*1024.0f)
#define GB (MB*1024.0f)

namespace DE {

class Allocator;
class FreeListAllocator;
class LinearAllocator;

class Memory : public ObjectBase {

private:

	/*
	* Global Heap for general purposes
	* Special Heap for gameObjects (spawn and destroy)
	* Level-Loading Heap
	* Stack-allocator for single-frame allocations (cleared every frame)
	* Debug memory Heap, only for allocations that will not be needed in final game.
	*/

	static FreeListAllocator smGlobal;
	// static LinearAllocator smGlobal;

	// static DE::Array<bool>* mFlags;
	// static DE::Array<PoolAllocator>* mPools;

	//std::map <string, PoolAllocator> mPools;

	Memory();
	~Memory();

	static std::map<String, i32> memoryMapCounter;

	// Instantiation by ObjectBase Name
	static std::map<String, std::function<ObjectBase*()>> classNamesMap;
	static ObjectBase* internalFromClassName(StringRef className);
	static void internalRegisterClassName(StringRef className, std::function<ObjectBase*()> instantiationFunction);

public:

	// static FreeListAllocator smGlobal;

	static void init();

	static void init(u32 size);

	static void flush();

	static Allocator& getGlobal();

	template<class T>
	static T* allocate(u32 alignment) {
		return Allocator::internalAllocate<T>((Allocator*) (&smGlobal), alignment);
	}

	template<class T>
	static T* allocate() {
		String className(T::getClassNameStatic());

		if(memoryMapCounter.find(className) == memoryMapCounter.end()){
			memoryMapCounter.insert(std::make_pair(className, 0));
		}

		memoryMapCounter[className] = memoryMapCounter[className] + 1;

		return Allocator::internalAllocate<T>((Allocator*) (&smGlobal));
	}

	template<class T>
	static void free(T *pointer) {
		if (pointer) {
			String className(pointer->getClassName());

			if(memoryMapCounter.find(className) != memoryMapCounter.end()){
				//if(memoryMapCounter[className] > 0){
					memoryMapCounter[className] = memoryMapCounter[className] - 1;
				//}
			}

			Allocator::internalFree(pointer, (Allocator*) (&smGlobal));
		}
	}

	static void free();

	template <class T>
	static void registerClassName(StringRef className) {
		internalRegisterClassName(className, [](){
			return (ObjectBase*)Memory::allocate<T>();
		});
	}

	template <class T>
	static T* fromClassName(StringRef className) {
		return (T*) internalFromClassName(className);
	}
};
}

