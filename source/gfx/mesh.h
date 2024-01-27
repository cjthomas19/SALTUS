#ifndef MESH_CT_H
#define MESH_CT_H

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader_s.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
};

class Mesh {
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;    
    std::string type;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
    void Draw(Shader &shader);
    void terminate();
private: 
    unsigned int VAO,VBO,EBO;
    void setupMesh();
};

#endif