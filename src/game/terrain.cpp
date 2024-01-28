#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "terrain.h"
#include "renderer/mesh.h"
#include "util/perlin.h"

TerrainGenerator::TerrainGenerator() {
    constexpr int W = 64;
    constexpr int subdiv = 2;
    constexpr int N = W * subdiv;

    float heights[N][N] {};
    std::vector<Vertex> vs((N-1) * (N-1) * 6);
    std::vector<unsigned int> is((N-1)*(N-1)*6);
    std::vector<Texture> ts {};

    // Perlin parameters: 
    float w0 = 1.0f/W; // Base Frequency
    int octaves = 20;  // Octaves
    float pers = 0.5;  // Persistance

    // Generate heightmap
    for(int z = 0; z < N; ++z) {
        for(int x = 0; x < N; ++x) {
            for (int o = 0; o < octaves; ++o) {
                heights[z][x] += 20*pow(pers,o)*perlin(x*w0*pow(2,o)/(float)subdiv,z*w0*pow(2,o)/(float)subdiv);
            }
        }
    }

    // Convert heightmap to vertices
    for(int iz = 0; iz < N-1; ++iz) {
        for(int ix = 0; ix<N-1; ++ix) {
            float x = (float)ix / subdiv;
            float z = (float)iz / subdiv;
            
            glm::vec3 c00 = glm::vec3(x,heights[iz][ix],z);
            glm::vec3 c01 = glm::vec3((x+1.0f/subdiv),heights[iz][ix+1],z);
            glm::vec3 c10 = glm::vec3(x,heights[iz+1][ix],(z+1.0f/subdiv));
            glm::vec3 c11 = glm::vec3((x+1.0f/subdiv),heights[iz+1][ix+1],(z+1.0f/subdiv));

            glm::vec3 n1 = glm::normalize(-glm::cross(c01-c00,c10-c00));
            glm::vec3 n2 = glm::normalize(glm::cross(c11-c01, c11-c10));
            //glm::vec3 n = glm::vec3(0,0,0);
            int i = (iz*(N-1) + ix)*6;
            vs[i]   = {c00,n1,glm::vec3(0,0,0)}; 
            vs[i+1] = {c01,n1,glm::vec3(0,1,0)}; 
            vs[i+2] = {c10,n1,glm::vec3(1,0,0)}; 

            vs[i+3] = {c01,n2,glm::vec3(0,1,0)}; 
            vs[i+4] = {c11,n2,glm::vec3(1,1,0)};
            vs[i+5] = {c10,n2,glm::vec3(1,0,0)}; 

            int ii = (iz*(N-1)+ix)*6;
            is[ii] = i;
            is[ii+1] = i+1;
            is[ii+2] = i+2;
            is[ii+3] = i+3;
            is[ii+4] = i+4;
            is[ii+5] = i+5;
        }
    }

    this->landscape = new Mesh(vs,is,ts);

}

TerrainGenerator::~TerrainGenerator() {
    delete landscape;
    landscape = nullptr;
}

void TerrainGenerator::terminate() {
    landscape->terminate();
}

void TerrainGenerator::draw(Shader s) {
    landscape->Draw(s);
}


