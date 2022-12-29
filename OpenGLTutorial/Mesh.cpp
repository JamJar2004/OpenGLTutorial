#include "Mesh.hpp"

std::shared_ptr<Mesh> Mesh::CreateCube()
{
    std::vector<glm::vec3> vertices =
    {
        glm::vec3(-1, -1,  1),
        glm::vec3( 1, -1,  1),
        glm::vec3(-1,  1,  1),
        glm::vec3( 1,  1,  1),
        glm::vec3(-1, -1, -1),
        glm::vec3( 1, -1, -1),
        glm::vec3(-1,  1, -1),
        glm::vec3( 1,  1, -1),
    };

    std::vector<GLuint> indices = 
    { 
        0, 2, 3,
        3, 1, 0,
        6, 4, 5,
        7, 6, 5,
        1, 3, 7,
        7, 5, 1,
        4, 6, 2,
        0, 4, 2,
        3, 2, 6,
        6, 7, 3,
        5, 4, 0,
        0, 1, 5 
    };

    return std::make_shared<Mesh>(vertices, indices);
}

Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices) : m_indexCount(indices.size())
{
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vboID);
    glDeleteBuffers(1, &m_iboID);
    glDeleteVertexArrays(1, &m_vaoID);
}

void Mesh::Draw()
{
    glBindVertexArray(m_vaoID);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
}