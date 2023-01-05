#include "Mesh.hpp"

#include "IndexedModel.hpp"

#include <stb_image.h>

void CalcNormals(std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
	for(size_t i = 0; i < indices.size(); i += 3)
	{
		GLuint i0 = indices[i    ];
		GLuint i1 = indices[i + 1];
		GLuint i2 = indices[i + 2];

		glm::vec3 v1 = vertices[i0].Position - vertices[i1].Position;
		glm::vec3 v2 = vertices[i0].Position - vertices[i2].Position;

		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		vertices[i0].Normal -= normal;
		vertices[i1].Normal -= normal;
		vertices[i2].Normal -= normal;
	}

	for(size_t i = 0; i < vertices.size(); i++)
		vertices[i].Normal = glm::normalize(vertices[i].Normal);
}

void CalcTangents(std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
	for(size_t i = 0; i < indices.size(); i += 3)
	{
		GLuint i0 = indices[i    ];
		GLuint i1 = indices[i + 1];
		GLuint i2 = indices[i + 2];

		glm::vec3 edge1 = vertices[i1].Position - vertices[i0].Position;
		glm::vec3 edge2 = vertices[i2].Position - vertices[i0].Position;

		glm::vec2 delta1 = vertices[i1].TexCoord - vertices[i0].TexCoord;
		glm::vec2 delta2 = vertices[i2].TexCoord - vertices[i0].TexCoord;

		float dividend = (delta1.x * delta2.y - delta2.x * delta1.y);
		float f = 1.0f / dividend;

		glm::vec3 tangent(0, 0, 0);
		tangent.x = f * (delta2.y * edge1.x - delta1.y * edge2.x);
		tangent.y = f * (delta2.y * edge1.y - delta1.y * edge2.y);
		tangent.z = f * (delta2.y * edge1.z - delta1.y * edge2.z);

		vertices[i0].Tangent += tangent;
		vertices[i1].Tangent += tangent;
		vertices[i2].Tangent += tangent;
	}
}

std::shared_ptr<Mesh> Mesh::CreateCube()
{
	glm::vec2 bl(0, 0);
	glm::vec2 br(1, 0);
	glm::vec2 tl(0, 1);
	glm::vec2 tr(1, 1);

	std::vector<Vertex> vertices = 
	{
		Vertex(glm::vec3(-1, -1, -1), br, glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1,  1, -1), tr, glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1, -1,  1), bl, glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1,  1,  1), tl, glm::vec3(-1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), bl, glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, -1,  1), br, glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1,  1, -1), tl, glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1,  1,  1), tr, glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(-1, -1, -1), tl, glm::vec3(0, -1, 0)),
		Vertex(glm::vec3(-1, -1,  1), bl, glm::vec3(0, -1, 0)),
		Vertex(glm::vec3( 1, -1, -1), tr, glm::vec3(0, -1, 0)),
		Vertex(glm::vec3( 1, -1,  1), br, glm::vec3(0, -1, 0)),

		Vertex(glm::vec3(-1, 1, -1), bl, glm::vec3(0, 1, 0)),
		Vertex(glm::vec3( 1, 1, -1), br, glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, 1,  1), tl, glm::vec3(0, 1, 0)),
		Vertex(glm::vec3( 1, 1,  1), tr, glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), bl, glm::vec3(0, 0, -1)),
		Vertex(glm::vec3( 1, -1, -1), br, glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(-1,  1, -1), tl, glm::vec3(0, 0, -1)),
		Vertex(glm::vec3( 1,  1, -1), tr, glm::vec3(0, 0, -1)),

		Vertex(glm::vec3(-1, -1, 1), br, glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1,  1, 1), tr, glm::vec3(0, 0, 1)),
		Vertex(glm::vec3( 1, -1, 1), bl, glm::vec3(0, 0, 1)),
		Vertex(glm::vec3( 1,  1, 1), tl, glm::vec3(0, 0, 1))
	};

	GLuint faceIndices[] =
	{
		0, 3, 2,
		3, 0, 1,
	};

	std::vector<GLuint> indices(36);

	for(size_t i = 0; i < 6; i++)
	{
		for(int j = 0; j < 6; j++)
			indices.push_back(faceIndices[j] + i * 4);
	}

	CalcTangents(vertices, indices);

	return std::make_shared<Mesh>(vertices, indices);
}


std::shared_ptr<Mesh> Mesh::LoadTerrain(const std::string& fileName)
{
	int width;
	int height;
	int channels;
	uint8_t* pixelData = stbi_load(("./res/textures/" + fileName).c_str(), &width, &height, &channels, 0);

	std::vector<Vertex> vertices;

	for(size_t z = 0; z < height; z++)
	{
		for(size_t x = 0; x < width; x++)
		{
			size_t index = (z * width + x) * channels;

			float u = x / float(width);
			float v = z / float(height);

			float heightValue = pixelData[index] / 255.0f;

			vertices.emplace_back(glm::vec3(u * 2 - 1, heightValue * 2 - 1, v * 2 - 1), glm::vec2(u, v), glm::vec3(0));
		}
	}

	std::vector<GLuint> indices;

	for(GLuint z = 0; z < height - 1ULL; z++)
	{
		for(GLuint x = 0; x < width - 1ULL; x++)
		{
			GLuint index = z * width + x;

			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index + width);

			indices.push_back(index + 1);
			indices.push_back(index + width + 1);
			indices.push_back(index + width);
			
		}
	}

	CalcNormals(vertices, indices);
	CalcTangents(vertices, indices);

	return std::make_shared<Mesh>(vertices, indices);
}

std::shared_ptr<Mesh> Mesh::Load(const std::string& fileName)
{
    OBJModel objModel("./res/meshes/" + fileName);

    IndexedModel indexedModel = objModel.ToIndexedModel();

    std::vector<Vertex> vertices;

    for(size_t i = 0; i < indexedModel.positions.size(); i++)
    {
        Vertex vertex(indexedModel.positions[i], indexedModel.texCoords[i], indexedModel.normals[i]);
        vertices.push_back(vertex);
    }

    std::vector<GLuint> indices;
    for(GLuint index : indexedModel.indices)
        indices.push_back(index);

    CalcTangents(vertices, indices);

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
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
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