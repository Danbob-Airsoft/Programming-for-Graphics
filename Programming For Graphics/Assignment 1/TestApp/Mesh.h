#ifndef MESH_H
#define MESH_H

#include "Transform.h"
#include <glm/glm.hpp>
#include <glew.h>

class Mesh
{
public:
	Mesh(float* verts, unsigned int vertCount);
	void Draw(float XRotator, float YRotator, float ZRotator);
	~Mesh();

	Transform* m_transform = new Transform();
private:
	GLuint m_vertexBufferObject = 0;
	GLuint m_vertexArrayObject = 0;

};

#endif // !MESH_H


