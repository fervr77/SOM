#include "MeshLibrary.h"

std::unordered_map<std::string, std::pair<unsigned int, unsigned int>> MeshLibrary::meshMap;

std::unordered_map<std::string, std::pair<float*, unsigned int>> MeshLibrary::arrayMap;

std::vector<std::string> MeshLibrary::meshList;