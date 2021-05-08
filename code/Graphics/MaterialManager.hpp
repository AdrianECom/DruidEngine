#pragma once

#include "Core/ObjectBase.hpp"
#include "Core/Singleton.hpp"
#include "Containers/HashMap.hpp"

namespace DE {

class MaterialManager: public ObjectBase, public Singleton<MaterialManager> {
	
private:

	HashMap<String, class Texture*>* mTexturesMap;
	HashMap<String, class Material*>* mMaterialsMap;
	class Material* mNoTextureMaterial;

public:

	GENERATE_METADATA(CONSTRUCTOR, MaterialManager)

	void init();
	class Texture* loadTexture(const String &path);
	class Material* loadMaterial(const String &path);
	class Material* loadNoTextureMaterial();
};
}

