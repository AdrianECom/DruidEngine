#ifndef BASIC_H_
#define BASIC_H_

#include "Basic.h"
#include "Hash.h"
#include <type_traits>

#define id_to_str(str) #str
#define CLASS_ID(classname) id_to_str(id_##classname)

#define DE_CLASS(classname) class classname : public Hash
#define DE_CLASS_EXTENDS ,

#endif /* BASIC_H_ */