#ifndef MESH_LOADER_H
#define MESH_LOADER_H
#include <unordered_map>
#include <glm/glm.hpp>
#include <vector>
#include <glm/glm.hpp>


    class MeshLoader
    {
        public:
            static std::pair<unsigned int,unsigned int> LoadMeshFromFile(const std::string& name);
            static std::pair<float*, unsigned int> LoadArrayFromFile(const std::string& name);
            static std::pair<unsigned int, unsigned int> LoadColoredMeshFromFile(const std::string& name);
            static std::pair<float*, unsigned int> LoadColoredArrayFromFile(const std::string& name);
            static void InitializeColorMap(void);
            static unsigned int GetNumVertices(const std::string& name);
            
            
            static std::unordered_map<unsigned int,std::string> vaoDictionary;
            static std::unordered_map<unsigned int, std::string> coloredVAODictionary;
            static std::unordered_map<unsigned int, glm::vec3> colorMap;

    private:
        static unsigned int NUMCOLORS;
            

        public:
            struct Vertex
            {
                glm::vec3 position;
                glm::vec3 normal;
                glm::vec2 uv;

                bool operator==(const Vertex& other)const
                {
                    return position == other.position && normal == other.normal && uv == other.uv;
                }

            };

            struct ColoredVertex
            {
                glm::vec3 position;
                glm::vec3 color;

                bool operator==(const ColoredVertex& other)const
                {
                    return position == other.position && color == other.color;
                }
            };

            struct PosVertex
            {
                glm::vec3 position;

                bool operator==(const PosVertex& other)const
                {
                    return position == other.position;
                }

            };


        

    };


#endif