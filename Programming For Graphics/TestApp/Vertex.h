#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TextureCoord;

	Vertex(const float X, const float Y, const float Z) 
	{
		this->Position.x = X;
		this->Position.y = Y;
		this->Position.z = Z;

		TextureCoord = { 0, 0 };
	}

	Vertex(const glm::vec3 Position): Vertex(Position.x, Position.y, Position.z) 
	{
	}

	Vertex(const glm::vec3 (Position), const glm::vec2 (texCoord)) 
	{
		this->Position = Position;
		this->TextureCoord = texCoord;
	}
};
#endif //!VERTEX_H

