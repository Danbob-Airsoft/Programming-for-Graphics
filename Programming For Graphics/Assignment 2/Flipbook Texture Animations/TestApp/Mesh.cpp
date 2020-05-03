#include "Mesh.h"
#include <string>

Mesh::Mesh(Vertex* verts, unsigned int vertCount, unsigned int* indices, unsigned int numIndices)
{
	CalculateTangentsBitangent(verts, vertCount, indices, numIndices);

	m_drawCount = numIndices;

	std::vector<vec3> positions;
	std::vector<vec2> texCoords;
	std::vector<vec3> Tangents;
	std::vector<vec3> BiTangents;

	for (unsigned int i = 0; i < vertCount; i++) 
	{
		positions.push_back(verts[i].Position);
		texCoords.push_back(verts[i].TextureCoord);
		Tangents.push_back(verts[i].Tangent);
		BiTangents.push_back(verts[i].BiTangent);
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

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[TANGENT_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Tangents[0]), &Tangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(TANGENT_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(TANGENT_VB);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[BITANGENT_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(BiTangents[0]), &BiTangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(BITANGENT_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(BITANGENT_VB);

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

void Mesh::CalculateTangentsBitangent(Vertex* verticies, unsigned int vertCount, unsigned int* indicies, unsigned int numIndices)
{
	for (unsigned int i = 0; i < numIndices; i += 3) 
	{
		Vertex v0 = verticies[indicies[i]];
		Vertex v1 = verticies[indicies[i + 1]];
		Vertex v2 = verticies[indicies[i + 2]];

		vec3 edge1 = v1.Position - v0.Position;
		vec3 edge2 = v2.Position - v0.Position;

		GLfloat deltaU1 = v1.TextureCoord.x - v0.TextureCoord.x;
		GLfloat deltaV1 = v1.TextureCoord.y - v0.TextureCoord.y;
		GLfloat deltaU2 = v2.TextureCoord.x - v0.TextureCoord.x;
		GLfloat deltaV2 = v2.TextureCoord.y - v0.TextureCoord.y;

		GLfloat f = 1.0f / (deltaU1 * deltaV2 - deltaU2 - deltaV1);

		vec3 tangent;
		vec3 biTangent;

		tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
		tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
		tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

		biTangent.x = f * (-deltaU2 * edge1.x + deltaU1 * edge2.x);
		biTangent.y = f * (-deltaU2 * edge1.y + deltaU1 * edge2.y);
		biTangent.z = f * (-deltaU2 * edge1.z + deltaU1 * edge2.z);

		v0.Tangent += tangent;
		v1.Tangent += tangent;
		v2.Tangent += tangent;

		v0.BiTangent += biTangent;
		v1.BiTangent += biTangent;
		v2.BiTangent += biTangent;

		verticies[indicies[i]] = v0;
		verticies[indicies[i + 1]] = v1;
		verticies[indicies[i + 2]] = v2;
	}

	for (unsigned int i = 0; i < vertCount; i++) 
	{
		verticies[i].Tangent = normalize(verticies[i].Tangent);
		verticies[i].BiTangent = normalize(verticies[i].BiTangent);
	}

}
