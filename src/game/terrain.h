#ifndef TERRAIN_CT
#define TERRAIN_CT

#include "renderer/mesh.h"

class TerrainGenerator {
private:
    Mesh* landscape;
public:
    TerrainGenerator();
    ~TerrainGenerator();
    void draw(Shader s);
    void terminate();
};
#endif
