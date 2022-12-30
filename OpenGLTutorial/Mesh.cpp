#include "Mesh.hpp"

std::shared_ptr<Mesh> Mesh::CreateCube()
{
	glm::vec2 bl(0, 0);
	glm::vec2 br(1, 0);
	glm::vec2 tl(0, 1);
	glm::vec2 tr(1, 1);

	std::vector<Vertex> vertices = 
	{
		Vertex(glm::vec3(-1, -1, -1), br),
		Vertex(glm::vec3(-1,  1, -1), tr),
		Vertex(glm::vec3(-1, -1,  1), bl),
		Vertex(glm::vec3(-1,  1,  1), tl),

		Vertex(glm::vec3(1, -1, -1), bl),
		Vertex(glm::vec3(1, -1,  1), br),
		Vertex(glm::vec3(1,  1, -1), tl),
		Vertex(glm::vec3(1,  1,  1), tr),

		Vertex(glm::vec3(-1, -1, -1), tl),
		Vertex(glm::vec3(-1, -1,  1), bl),
		Vertex(glm::vec3( 1, -1, -1), tr),
		Vertex(glm::vec3( 1, -1,  1), br),

		Vertex(glm::vec3(-1, 1, -1), bl),
		Vertex(glm::vec3( 1, 1, -1), br),
		Vertex(glm::vec3(-1, 1,  1), tl),
		Vertex(glm::vec3( 1, 1,  1), tr),

		Vertex(glm::vec3(-1, -1, -1), bl),
		Vertex(glm::vec3( 1, -1, -1), br),
		Vertex(glm::vec3(-1,  1, -1), tl),
		Vertex(glm::vec3( 1,  1, -1), tr),

		Vertex(glm::vec3(-1, -1, 1), br),
		Vertex(glm::vec3(-1,  1, 1), tr),
		Vertex(glm::vec3( 1, -1, 1), bl),
		Vertex(glm::vec3( 1,  1, 1), tl)
	};

	size_t faceIndices[] =
	{
		0, 2, 3,
		3, 1, 0,
	};

	std::vector<GLuint> indices(36);

	for(size_t i = 0; i < 6; i++)
	{
		for(int j = 0; j < 6; j++)
			indices.push_back(faceIndices[j] + i * 4);
	}

	return std::make_shared<Mesh>(vertices, indices);
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) : m_indexCount(indices.size())
{
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
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