#ifndef TERRAIN_CT
#define TERRAIN_CT

#include "renderer/mesh.h"

class TerrainGenerator {
public:
    TerrainGenerator();
    ~TerrainGenerator();
    void draw(Shader s);
    void terminate();
private:
    Mesh* landscape {};
};
#endif
