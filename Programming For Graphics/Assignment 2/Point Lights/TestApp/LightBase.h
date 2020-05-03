#pragma once
#ifndef LIGHTBASE_H
#define LIGHTBASE_H
#include "Transform.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glew.h>

class LightBase
{
public:
	LightBase();
	~LightBase();

	void Draw(Camera* Cam);

	Transform m_Transform;

	vec3 M_Color;


private:
};

#endif // !LIGHTBASE_H

