#pragma once
#include <glew.h>
#include <string>
#include <fstream>
#include "Transform.h"
#include "Camera.h"
#include <iostream>
#include "LightBase.h"

using namespace std;

class Shader
{
public:
	Shader(const string FileLocation, Camera& camera);
	~Shader();
	void Update(Transform& transform, LightBase& light);
	void Bind();
	GLuint GetProgram()
	{
		return m_Program;
	}

private:
	string Name;
	GLuint m_Program;
	Camera* m_Camera;

	enum ShaderTypes 
	{
		VERTEX,
		FRAGMENT,
		NUM_SHADER_TYPES
	};
	GLuint m_shaders[NUM_SHADER_TYPES];

	enum UniformNames 
	{
		MODEL_U,
		PROJECTION_U,
		VIEW_U,
		FRAG_LIGHTCOLOR_U,
		FRAG_LIGHTPOS_U,
		FRAG_CAMERAPOS_U,
		NUM_UNIFORMS
	};
	GLuint m_Uniforms[NUM_UNIFORMS];
};

