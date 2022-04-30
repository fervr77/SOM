#ifndef MESH_LIBRARY_H
#define MESH_LIBRARY_H
#include <unordered_map>

class MeshLibrary
{

public:
	
	static std::unordered_map<std::string, std::pair<unsigned int, unsigned int>> meshMap;
	static std::unordered_map<std::string, std::pair<float*, unsigned int>> arrayMap;
	static std::vector<std::string> meshList;



private:


};


#endif
