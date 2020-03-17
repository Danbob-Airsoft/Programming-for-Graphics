#pragma once
#ifndef VERTEX_H
#define VERTEX_H
struct Vertex
{
	float x;
	float y;
	float z;

	Vertex(float X, float Y, float Z) 
	{
		this->x = X;
		this->y = Y;
		this->z = Z;
	}
};
#endif //!VERTEX_H

