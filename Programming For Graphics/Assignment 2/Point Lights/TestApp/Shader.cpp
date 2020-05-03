#include "Shader.h"

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

	m_Uniforms[FRAG_CAMERAPOS_U] = glGetUniformLocation(m_Program, "FragCamPos");
	m_Uniforms[FRAG_LIGHTCOLOR_U] = glGetUniformLocation(m_Program, "FragLightColor");
	m_Uniforms[FRAG_LIGHTPOS_U] = glGetUniformLocation(m_Program, "FragLightPos");

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

void Shader::Update(Transform& transform, LightBase& light)
{
	//Calculate distance from Mesh to Light
	double Distance = std::sqrt(std::pow(transform.GetPosition().x - light.m_Transform.GetPosition().x, 2.0f) + 
		std::pow(transform.GetPosition().y - light.m_Transform.GetPosition().y, 2.0f)
		+ std::pow(transform.GetPosition().z - light.m_Transform.GetPosition().z, 2.0f));
	//Decrease into ofset for light intensity
	float LightIntensity = (Distance - 0.5) / 10.0f;
	std::cout << LightIntensity << std::endl;

	mat4 projection = m_Camera->GetPerspective();
	mat4 view = m_Camera->CalculateViewMatrix();
	mat4 model = transform.GetModel();

	glUniformMatrix4fv(m_Uniforms[MODEL_U], 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m_Uniforms[PROJECTION_U], 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(m_Uniforms[VIEW_U], 1, GL_FALSE, &view[0][0]);

	glUniform3f(m_Uniforms[FRAG_CAMERAPOS_U], m_Camera->CameraTransform->GetPosition().x, m_Camera->CameraTransform->GetPosition().y, m_Camera->CameraTransform->GetPosition().z);

	glUniform3f(m_Uniforms[FRAG_LIGHTPOS_U], light.m_Transform.GetPosition().x, light.m_Transform.GetPosition().y, light.m_Transform.GetPosition().z);

	glUniform3f(m_Uniforms[FRAG_LIGHTCOLOR_U], light.M_Color.x, light.M_Color.y, light.M_Color.z);

	
}

void Shader::Bind()
{
	glUseProgram(m_Program);
}
