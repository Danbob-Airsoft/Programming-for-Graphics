#include "Mesh.h"
#include <string>

Mesh::Mesh(Vertex* verts, unsigned int vertCount, unsigned int* indices, unsigned int numIndices)
{
	m_drawCount = numIndices;

	std::vector<vec3> positions;
	std::vector<vec2> texCoords;

	for (unsigned int i = 0; i < vertCount; i++) 
	{
		positions.push_back(verts[i].Position);
		texCoords.push_back(verts[i].TextureCoord);
	}

	std::vector<vec3> Normals;
	Normals.resize(vertCount);

	for (int i = 0; i < numIndices; i += 3) 
	{
		vec3 Vert1 = positions[indices[i]];
		vec3 Vert2 = positions[indices[i + 1]];
		vec3 Vert3 = positions[indices[i + 2]];

		vec3 normal = triangleNormal(Vert1, Vert2, Vert3);
		Normals[indices[i]] += normal;
		Normals[indices[i + 1]] += normal;
		Normals[indices[i + 2]] += normal;
	}

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	
	glGenBuffers(NUM_BUFFERS, m_vertexBufferObject);

	//Position
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(POSITION_VB, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(POSITION_VB);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[TEXTCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(TEXTCOORD_VB, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(TEXTCOORD_VB);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Normals[0]), &Normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(NORMAL_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(NORMAL_VB);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferObject[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Draw(float XRotator, float YRotator, float ZRotator)
{
	m_transform->SetRotation(vec3(m_transform->GetRotation().x + XRotator, m_transform->GetRotation().y + YRotator, m_transform->GetRotation().z + ZRotator));
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}
