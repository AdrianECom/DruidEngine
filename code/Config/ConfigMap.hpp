#pragma once

#include "Core/Basic.hpp"
#include "Containers/HashMap.hpp"

namespace DE {

class ConfigMap : public ObjectBase {

private:
	HashMap<String, String> mMap;

	static String trueString;
	static String falseString;

public:

	GENERATE_METADATA(CONSTRUCTOR, ConfigMap)

	void init();
	void clear();
	bool contains(StringRef key);
	void readConfigFile(const String &path);
	void writeConfigFile(const String &path);
	String getString(const String &key);
	u32 getU32(const String &key);
	f32 getF32(const String &key);
	bool getBool(const String &key);
	void setString(const String &key, const String &value);
	void setU32(const String &key, u32 value);
	void setF32(const String &key, f32 value);
	void setBool(const String &key, bool value);

	/*template<class T>
	const List<T>* getList(const String &key);*/
};
}
