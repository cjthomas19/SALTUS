#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

#include "shader_s.h"
#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::Draw(Shader &shader) {
    unsigned int nDiffuse = 1;
    unsigned int nSpecular = 1;
    for(unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse") {
            number = std::to_string(nDiffuse++);
        } else if(name == "texture_specular") {
            number = std::to_string(nSpecular++);
        }

        shader.setInt(("material." + name + number).c_str(),i);
        glBindTexture(GL_TEXTURE_2D,textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,static_cast<unsigned int>(indices.size()),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}

void Mesh::setupMesh() {
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glGenVertexArrays(1,&VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,Normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,TexCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::terminate() {
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}

