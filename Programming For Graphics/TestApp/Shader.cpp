#include "Shader.h"

Shader::Shader(const string FileLocation, Camera& camera)
{
	m_Camera = &camera;
	Name = FileLocation;
	m_Program = glCreateProgram();

	m_shaders[VERTEX] = CreateShader(LoadShader(FileLocation + ".vert"), GL_VERTEX_SHADER);
	m_shaders[FRAGMENT] = CreateShader(LoadShader(FileLocation + ".frag"), GL_FRAGMENT_SHADER);

	for (GLuint i = 0; i < NUM_SHADER_TYPES; i++) 
	{
		glAttachShader(m_Program, m_shaders[i]);
	}

	glLinkProgram(m_Program);
	CheckShaderError(m_Program, GL_LINK_STATUS, true, "Error: Program Linking Failed: ");

	glValidateProgram(m_Program);
	CheckShaderError(m_Program, GL_VALIDATE_STATUS, true, "Error: Program is Invalid: ");

	m_Uniforms[MODEL_U] = glGetUniformLocation(m_Program, "model");
	m_Uniforms[PROJECTION_U] = glGetUniformLocation(m_Program, "projection");
	m_Uniforms[VIEW_U] = glGetUniformLocation(m_Program, "view");

	for (GLuint i = 0; i < NUM_UNIFORMS; i++) 
	{
		if (m_Uniforms[i] == GL_INVALID_INDEX) 
		{
			std::cout << "Shader " << Name << " Uniform invlid index:" << static_cast<UniformNames>(i) << "(Might be optimised out if not used)" << std::endl;
		}
	}
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADER_TYPES; i++) 
	{
		glDetachShader(m_Program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	glDeleteProgram(m_Program);
}

void Shader::Update(Transform& transform)
{
	mat4 projection = m_Camera->GetPerspective();
	mat4 view = m_Camera->CalculateViewMatrix();
	mat4 model = transform.GetModel();

	glUniformMatrix4fv(m_Uniforms[MODEL_U], 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m_Uniforms[PROJECTION_U], 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(m_Uniforms[VIEW_U], 1, GL_FALSE, &view[0][0]);
}

void Shader::Bind()
{
	glUseProgram(m_Program);
}
