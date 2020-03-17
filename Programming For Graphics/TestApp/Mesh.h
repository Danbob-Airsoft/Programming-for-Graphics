#ifndef MESH_H
#define MESH_H

#include "Transform.h"
#include <glm/glm.hpp>
#include <glew.h>
#include "Vertex.h"

class Mesh
{
public:
	Mesh(Vertex* verts, unsigned int vertCount, unsigned int *indices, unsigned int numIndices);
	void Draw(float XRotator, float YRotator, float ZRotator);
	~Mesh();

	int m_drawCount;
	Transform* m_transform = new Transform();
private:
	GLuint m_vertexBufferObject[2];
	GLuint m_vertexArrayObject = 0;

};

#endif // !MESH_H


