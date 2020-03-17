#include "Mesh.h"
#include "Transform.h"
#include <string>
#include <iostream>

Mesh::Mesh(Vertex* verts, unsigned int vertCount, unsigned int* indices, unsigned int numIndices)
{
	m_drawCount = numIndices;

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glGenBuffers(2, m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
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
