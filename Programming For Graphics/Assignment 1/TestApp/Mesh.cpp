#include "Mesh.h"
#include "Transform.h"
#include <string>
#include <iostream>

Mesh::Mesh(float* verts, unsigned int vertCount)
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(float), verts, GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);
}

void Mesh::Draw(float XRotator, float YRotator, float ZRotator)
{
	m_transform->SetRotation(vec3(m_transform->GetRotation().x + XRotator, m_transform->GetRotation().y + YRotator, m_transform->GetRotation().z + ZRotator));
	glBindVertexArray(m_vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}
