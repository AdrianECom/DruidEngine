#include "Memory/Memory.hpp"
#include "Memory/FreeListAllocator.hpp"
#include "Memory/LinearAllocator.hpp"
#include "Log/Log.hpp"

namespace DE {

std::map<String, i32> Memory::memoryMapCounter;

std::map<String, std::function<ObjectBase*()>> Memory::classNamesMap;

FreeListAllocator Memory::smGlobal;
//LinearAllocator Memory::smGlobal;

Memory::Memory() {

}

Memory::~Memory() {

}

void Memory::init() {

	// NOTE: Log starts here!!
	Log::init();

	TRACE()

	smGlobal.init(/*2 * GB*/ 500 * MB);

	ECHO("TOTAL MEMORY (IN MB)");
	VAL(f32,(smGlobal.mTotalSize / MB));
}

void Memory::init(u32 size) {
	TRACE()

	smGlobal.init(size);
}

void Memory::flush() {
	smGlobal.flush();
}

Allocator& Memory::getGlobal() {
	return smGlobal;
}

void Memory::free() {

	ECHO("MEMORY SIZE (IN MB)");
	VAL(f32,(smGlobal.mTotalSize / MB));

	ECHO("MAX ALLOCATED MEMORY(IN MB)");
	VAL(f32,(smGlobal.mDebugMaxAllocatedSize / MB));

	ECHO("REMAINING ALLOCATED MEMORY (IN MB)");
	VAL(f32,(smGlobal.mAllocatedSize / MB));

	ECHO("REMAINING ALLOCATED MEMORY (BYTES)");
	VAL(f32,(smGlobal.mAllocatedSize));

	smGlobal.terminate();

    std::map<String, i32>::iterator it = memoryMapCounter.begin();
    while(it != memoryMapCounter.end())
    {
        std::cout<< (it->second == 0 ? "OK" : "!!") << " " << it->second<<" :: "<<it->first<<std::endl;
        it++;
    }

	// NOTE: Log ends here!!
	Log::terminate();

	ECHO("TERMINATE - OK")
}

ObjectBase* Memory::internalFromClassName(StringRef className) {
	ASSERT(classNamesMap.find(className) != classNamesMap.end() , "Class name is not registered: " + className)
	return classNamesMap[className]();
}

void Memory::internalRegisterClassName(StringRef className, std::function<ObjectBase*()> instantiationFunction) {
	classNamesMap[className] = instantiationFunction;
}
}
