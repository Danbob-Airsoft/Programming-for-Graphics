#pragma once
#include "Transform.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class Camera
{
public:
	Transform* CameraTransform = new Transform();

	glm::vec3 ForwardVec;
	glm::vec3 UpVec;
	glm::vec3 RightVec;

	float fov = 120.0f;
	float aspect = 800.0f / 800.0f;
	float znear = 0.1f;
	float zfar = 100.0f;

	void UpdateVectors();

	mat4 m_PerspectiveProjection;

	mat4 CalculateViewMatrix();

	Camera(glm::vec3 _pos) 
	{
		CameraTransform->SetPosition(_pos);
		m_PerspectiveProjection = glm::perspective(glm::radians(fov), aspect, znear, zfar);
	}

	mat4 GetPerspective() 
	{
		return m_PerspectiveProjection;
	}

};

