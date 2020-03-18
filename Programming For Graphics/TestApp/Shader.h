#pragma once
#include <glew.h>
#include <string>
#include <fstream>
#include "Transform.h"
#include "Camera.h"
#include <iostream>

using namespace std;

class Shader
{
public:
	Shader(const string FileLocation, Camera& camera);
	~Shader();
	void Update(Transform& transform);
	void Bind();

	static void CheckShaderError(GLuint Shader, GLuint Flag, bool IsProgram, const std::string& ErrorMessage)
	{
		GLint Success = 0;
		GLchar error[1024] = { 0 };

		if (IsProgram)
		{
			glGetProgramiv(Shader, Flag, &Success);
		}
		else
		{
			glGetShaderiv(Shader, Flag, &Success);
		}

		if (Success == GL_FALSE)
		{
			if (IsProgram)
			{
				glGetProgramInfoLog(Shader, sizeof(error), NULL, error);
			}
			else
			{
				glGetShaderInfoLog(Shader, sizeof(error), NULL, error);
			}
			std::cerr << ErrorMessage << ": '" << error << "'" << std::endl;
		}

		glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
		glViewport(0, 0, 800, 800);
	}

	static GLuint CreateShader(const string& ShaderSource, GLenum shaderType)
	{
		GLuint shader = glCreateShader(shaderType);

		if (shader == 0)
		{
			std::cerr << "Error: Shader Creation Failed" << std::endl;
		}

		const char* tempSourceCode = ShaderSource.c_str();

		glShaderSource(shader, 1, &tempSourceCode, NULL);
		glCompileShader(shader);

		CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader Failed to Compile");
		return shader;
	}


	static string LoadShader(const string& fileName)
	{
		std::ifstream file;
		const char* fileNameChar = fileName.c_str();
		file.open(fileNameChar, ifstream::in);

		string output;
		string line;

		if (file.is_open())
		{
			while (file.good())
			{
				getline(file, line);
				output.append(line + "\n");
			}
		}
		else
		{
			std::cerr << "Unable to load shader: " << fileName << std::endl;
		}
		return output;
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
		NUM_UNIFORMS
	};
	GLuint m_Uniforms[NUM_UNIFORMS];
};

