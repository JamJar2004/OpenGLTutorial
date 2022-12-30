#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

struct Vertex
{
    Vertex(const glm::vec3& position, const glm::vec2& texCoord) :
        Position(position), TexCoord(texCoord) {}

    glm::vec3 Position;
    glm::vec2 TexCoord;
};

class Mesh
{
private:
    GLuint m_vaoID;
    GLuint m_vboID;
    GLuint m_iboID;

    size_t m_indexCount;
public:
    static std::shared_ptr<Mesh> CreateCube();

    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
    ~Mesh();

    void Draw();
};