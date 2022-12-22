#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Mesh
{
private:
    GLuint m_vaoID;
    GLuint m_vboID;
public:
    Mesh(const std::vector<glm::vec3>& vertices);
    ~Mesh();

    void Draw();
};