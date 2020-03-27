#ifndef MESH_H
#define MESH_H

#include <iostream>
#include "Transform.h"
#include <glm/glm.hpp>
#include <glew.h>
#include "Vertex.h"
#include <vector>
#include <glm/gtx/normal.hpp>

class Mesh
{
public:
	Mesh(Vertex* verts, unsigned int vertCount, unsigned int *indices, unsigned int numIndices);
	void Draw(float XRotator, float YRotator, float ZRotator);
	~Mesh();

	int m_drawCount;
	Transform* m_transform = new Transform();
private:
	enum
	{
		POSITION_VB,
		TEXTCOORD_VB,
		INDEX_VB,
		NORMAL_VB,
		NUM_BUFFERS
	};

	GLuint m_vertexBufferObject[NUM_BUFFERS];
	GLuint m_vertexArrayObject = 0;
};

#endif // !MESH_H


