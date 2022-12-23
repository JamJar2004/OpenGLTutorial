#include "Mesh.hpp"

Mesh::Mesh(const std::vector<glm::vec3>& vertices) : m_vertexCount(vertices.size())
{
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vboID);
    glDeleteVertexArrays(1, &m_vaoID);
}

void Mesh::Draw()
{
    glBindVertexArray(m_vaoID);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
}