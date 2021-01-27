#pragma once

#include "Core/Basic.hpp"
#include "Containers/HashMap.hpp"

namespace DE {

class ConfigMap: public DE_Class {

private:
	using StringStringMap = HashMap<std::string, std::string>;
	DE_M(Map, StringStringMap)

	static std::string trueString;
	static std::string falseString;

public:

	DE_CLASS(ConfigMap)

	void init();
	void readConfigFile(const std::string &path);
	void writeConfigFile(const std::string &path);
	std::string getString(const std::string &key);
	u32 getU32(const std::string &key);
	f32 getF32(const std::string &key);
	bool getBool(const std::string &key);
	void setString(const std::string &key, const std::string &value);
	void setU32(const std::string &key, u32 value);
	void setF32(const std::string &key, f32 value);
	void setBool(const std::string &key, bool value);

	/*template<class T>
	const List<T>* getList(const std::string &key);*/
};

}