#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>

struct Vertex
{
    Vertex(const glm::vec3& position, const glm::vec2& texCoord, const glm::vec3& normal, const glm::vec3& tangent = glm::vec3(0)) :
        Position(position), TexCoord(texCoord), Normal(normal), Tangent(tangent) {}

    glm::vec3 Position;
    glm::vec2 TexCoord;
    glm::vec3 Normal;
    glm::vec3 Tangent;
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
    static std::shared_ptr<Mesh> CreateQuad();
    static std::shared_ptr<Mesh> LoadTerrain(const std::string& fileName);
    static std::shared_ptr<Mesh> Load(const std::string& fileName);

    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
    ~Mesh();

    void Draw();
};