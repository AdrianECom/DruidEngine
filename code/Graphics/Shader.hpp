#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/BasicTypes.hpp"
#include "Maths/Vector4.hpp"

namespace DE {

template<class T> class Array;

class Matrix4;

class Shader : public ObjectBase {
private:
	PRIVATE(VertexShader, NONE, u32)
	PRIVATE(FragmentShader, NONE, u32)
	PRIVATE(Program, NONE, u32)

	static Shader* msShaderDefault;
	static Shader* msShaderDebug;

	void initInternal(const String &vertex, const String &fragment);

public:

	GENERATE_METADATA(CONSTRUCTOR, Shader)

	static Shader* getDefaultShader();
	static Shader* getDebugShader();

	static void freeStaticShaders();

	void init();
	void initDebug();
	void use();

	void addMatrix(const Matrix4 &matrix, const String &name);
	void addInt(i32 value, const String &name);
	void addUInt(u32 value, const String &name);
	void addFloat(f32 value, const String &name);
	void addVector4(Array<f32> *value, const String &name);
	void addVector3(Array<f32> *value, const String &name);
	void addBool(bool value, const String &name);
};
}

