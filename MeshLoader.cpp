#include <glad/glad.h>
#include "MeshLoader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <iostream>
#include <limits>
#include "MeshLibrary.h"


namespace std
{
    template<> struct hash<MeshLoader::Vertex> {
        size_t operator()(MeshLoader::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position) ^
                   (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.uv) << 1);
        }
    };

    template<> struct hash<MeshLoader::ColoredVertex> {
        size_t operator()(MeshLoader::ColoredVertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position)));
        }
    };

    template<> struct hash<MeshLoader::PosVertex> {
        size_t operator()(MeshLoader::PosVertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position)));
        }
    };
}



    std::unordered_map<unsigned int,std::string> MeshLoader::vaoDictionary;
    std::unordered_map<unsigned int, std::string> MeshLoader::coloredVAODictionary;
    std::unordered_map<unsigned int, glm::vec3> MeshLoader::colorMap;
    unsigned int MeshLoader::NUMCOLORS = 10;

    std::pair<unsigned int,unsigned int> MeshLoader::LoadMeshFromFile(const std::string& name)
    {
       
        const std::string filepath = name+".obj";

       

        std::pair<unsigned int,unsigned int> mesh;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<Vertex,uint32_t> uniqueVertices{};

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn,err;

        if(!tinyobj::LoadObj(&attrib,&shapes,&materials,&warn,&err,filepath.c_str()))
        {
            std::cout << (filepath + "  OBJ file not loaded") << std::endl;
            return std::pair<unsigned int,unsigned int>();
        }

       

       
        for(const auto& shape:shapes)
        {
            
            
            for(const auto& index:shape.mesh.indices)
            {
                Vertex vertex{};

                vertex.position = {
                    attrib.vertices[3*index.vertex_index+0],
                    attrib.vertices[3*index.vertex_index+1],
                    attrib.vertices[3*index.vertex_index+2]
                };

                if (attrib.normals.size() > 0)
                {
                    vertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]

                    };

                }


                if (attrib.texcoords.size() > 0)
                {
                    vertex.uv =
                    {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]

                    };

                }

                if(uniqueVertices.count(vertex)==0)
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }

        

        unsigned int VAO;
        unsigned int VBO,EBO;
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1,&VBO);
        glGenBuffers(1,&EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*vertices.size(),vertices.data(),GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);
        unsigned int VBO2;
        
        
        glEnableVertexAttribArray(3);
        
        glEnableVertexAttribArray(4);
       
        glEnableVertexAttribArray(5);
        
        glEnableVertexAttribArray(6);
        glVertexAttribDivisor(3,1);
        glVertexAttribDivisor(4,1);
        glVertexAttribDivisor(5,1);
        glVertexAttribDivisor(6,1);
        
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(uint32_t)*indices.size(),indices.data(),GL_STATIC_DRAW);

       
        

        mesh.first = VAO;
        mesh.second = indices.size();

        vaoDictionary[VAO] = name;
       
        return mesh;
    }

    std::pair<float*, unsigned int> MeshLoader::LoadArrayFromFile(const std::string& name)
    {
        const std::string filepath = name + ".obj";



        std::pair<unsigned int, unsigned int> mesh;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
        {
            std::cout << (filepath + "  OBJ file not loaded") << std::endl;
            return std::pair<float*, unsigned int>();
        }




        for (const auto& shape : shapes)
        {


            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex{};

                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]

                };

                vertex.uv =
                {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]

                };

                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }

        float* vertexArray = new float[vertices.size()*3];
        for (int i = 0; i < vertices.size(); i++)
        {
            vertexArray[i * 3] = vertices[i].position.x;
            vertexArray[(i * 3) + 1] = vertices[i].position.y;
            vertexArray[(i * 3) + 2] = vertices[i].position.z;
            
        }

        std::pair<float*, unsigned int> result;

        result.first = vertexArray;
        result.second = vertices.size();

        return result;




    }

    std::pair<unsigned int, unsigned int> MeshLoader::LoadColoredMeshFromFile(const std::string& name)
    {
        const std::string filepath = name + ".obj";



        std::pair<unsigned int, unsigned int> mesh;

        std::vector<ColoredVertex> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<ColoredVertex, uint32_t> uniqueVertices{};

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
        {
            std::cout << (filepath + "  OBJ file not loaded") << std::endl;
            return std::pair<unsigned int, unsigned int>();
        }


        unsigned int colorCounter = 0;

        for (const auto& shape : shapes)
        {


            for (const auto& index : shape.mesh.indices)
            {
                ColoredVertex vertex{};

                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };


                vertex.color = glm::vec3(1,1,1);

                

                if (uniqueVertices.count(vertex) == 0)
                {
                    vertex.color = colorMap[colorCounter];
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                    colorCounter++;
                    colorCounter = colorCounter % NUMCOLORS;
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }



        unsigned int VAO;
        unsigned int VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
       
        


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);




        mesh.first = VAO;
        mesh.second = indices.size();

        coloredVAODictionary[VAO] = name;
        

        return mesh;

    }

    std::pair<float*, unsigned int> MeshLoader::LoadColoredArrayFromFile(const std::string& name)
    {
        const std::string filepath = name + ".obj";



        std::pair<unsigned int, unsigned int> mesh;

        std::vector<ColoredVertex> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<ColoredVertex, uint32_t> uniqueVertices{};

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
        {
            std::cout << (filepath + "  OBJ file not loaded") << std::endl;
            return std::pair<float*, unsigned int>();
        }



        unsigned int colorCounter = 0;

        for (const auto& shape : shapes)
        {


            for (const auto& index : shape.mesh.indices)
            {
                ColoredVertex vertex{};

                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                

                vertex.color = glm::vec3(1,1,1);
                

                if (uniqueVertices.count(vertex) == 0)
                {
                    
                    vertex.color = colorMap[colorCounter];
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                    colorCounter++;
                    colorCounter = colorCounter % NUMCOLORS;
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }

        float* vertexArray = new float[vertices.size() * 6];
        for (int i = 0; i < vertices.size(); i++)
        {
            vertexArray[i * 6] = vertices[i].position.x;
            vertexArray[(i * 6) + 1] = vertices[i].position.y;
            vertexArray[(i * 6) + 2] = vertices[i].position.z;
            vertexArray[(i * 6)+3] = vertices[i].color.r;
            vertexArray[(i * 6) + 4] = vertices[i].color.g;
            vertexArray[(i * 6) + 5] = vertices[i].color.b;


        }

        std::pair<float*, unsigned int> result;

        result.first = vertexArray;
        result.second = vertices.size();

        return result;


    }


    void MeshLoader::InitializeColorMap(void)
    {
        NUMCOLORS = 10;

        colorMap[0] = glm::vec3(1, 0, 0);
        colorMap[1] = glm::vec3(0, 1, 0);
        colorMap[2] = glm::vec3(0, 0, 1);
        colorMap[3] = glm::vec3(1, 1, 0);
        colorMap[4] = glm::vec3(1, 0, 1);
        colorMap[5] = glm::vec3(0, 1, 1);
        colorMap[6] = glm::vec3(0.3, 1, 0.3);
        colorMap[7] = glm::vec3(0.5, 0, 1);
        colorMap[8] = glm::vec3(1, 0.5, 0);
        colorMap[9] = glm::vec3(0.5, 0.5, .2);
    }

    unsigned int MeshLoader::GetNumVertices(const std::string& name)
    {
        const std::string filepath = name + ".obj";



        std::pair<unsigned int, unsigned int> mesh;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
        {
            std::cout << (filepath + "  OBJ file not loaded") << std::endl;
            return 0;
        }



        unsigned int colorCounter = 0;

        for (const auto& shape : shapes)
        {


            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex{};

                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                if (attrib.normals.size() > 0)
                {
                    vertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]

                    };

                }


                if (attrib.texcoords.size() > 0)
                {
                    vertex.uv =
                    {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]

                    };

                }



               

                if (uniqueVertices.count(vertex) == 0)
                {

                    
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                    
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }

        

        return vertices.size();

    }

   
