#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Mesh
{
private:
    GLuint m_vaoID;
    GLuint m_vboID;
    GLuint m_iboID;

    size_t m_indexCount;
public:
    static std::shared_ptr<Mesh> CreateCube();

    Mesh(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices);
    ~Mesh();

    void Draw();
};