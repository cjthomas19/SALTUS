#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use() const;
    void setBool(const std::string name, bool value) const;
    void setInt(const std::string name, int value) const;
    void setFloat(const std::string name, float value) const;
    void setMatrix4fv(const std::string name, glm::mat4 matrix) const;
    void setVec3(const std::string name, float x,float y,float z) const;
    void setVec3(const std::string name, glm::vec3 vector) const;
};

#endif