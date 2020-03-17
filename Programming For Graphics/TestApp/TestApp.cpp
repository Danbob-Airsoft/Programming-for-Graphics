// TestApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define GLEW_STATIC

#include <glew.h>
#include <SDL_opengl.h>
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>
#include <string>
#include <iostream>
#include "Mesh.h"
#include "Camera.h"
#include <vector>
#include "Vertex.h"

void CheckShaderError(GLuint Shader, GLuint Flag, bool IsProgram, const std::string& ErrorMessage)
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
		std::cerr << ": '" << error << "'" << std::endl;
	}

	glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
	glViewport(0, 0, 800, 800);
}

int main(int argc, char *argv[])
{
	//------------------------------------ OpenGL Setup ----------------------------------
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	//---------------------------------------- Window Creation --------------------------------------
	SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_GLContext GLContext = SDL_GL_CreateContext(window);

	//-------------------------------------------- More GL Setup -------------------------------
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//------------------------------------------- Initialization Check ------------------------------
	if (status != GLEW_OK) 
	{
		std::cout << "Glew Failed to Initilaize" << std::endl;
	}

	//------------------------------------------ Triangle Setup ------------------------------------
	/*float Verticies[]
	{
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	};*/

	/*float Verticies2[]
	{
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};*/

	std::vector<Vertex> SquareVerticies;
	SquareVerticies.push_back(Vertex(-0.5f, 0.5f, -1.0f));
	SquareVerticies.push_back(Vertex(0.5f, 0.5f, -1.0f));
	SquareVerticies.push_back(Vertex(0.5f, -0.5f, -1.0f));
	SquareVerticies.push_back(Vertex(-0.5f, -0.5f, -1.0f));

	unsigned int SquareIndecies[]
	{
		0,1,2,0,2,3
	};

	//------------------------------------------ Mesh Setup -----------------------------------
	//Mesh Tril(Verticies, 3);
	//Mesh Tri2(Verticies2, 3);
	Mesh Square1(&SquareVerticies[0], SquareVerticies.size(), &SquareIndecies[0], 6);

	//--------------------------------------- Write Shader Code -----------------------------------
	const char* VertexShaderCode =
		"#version 450\n"
		"in vec3 vp;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"void main() {"
		" gl_Position = projection * view * model * vec4(vp,1.0);"
		"}";

	const char* FragmentShaderCode =
		"#version 450\n"
		"out vec4 frag_colour;"
		"void main() {"
		" frag_colour = vec4(1.0, 0.0, 0.0, 1.0);"
		"}";

	//------------------------------------- Linking and Compiling the Shaders --------------------
	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderCode, NULL);
	glCompileShader(VertexShader);
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderCode, NULL);
	glCompileShader(FragmentShader);

	//----------------------------------- Connect Shaders to a Shader Program ---------------------
	GLuint ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);

	//---------------------------------- Link and Validate Shader Program -------------------------
	glLinkProgram(ShaderProgram);
	CheckShaderError(ShaderProgram, GL_LINK_STATUS, true, "Error: Program linking failed: ");
	glValidateProgram(ShaderProgram);
	CheckShaderError(ShaderProgram, GL_VALIDATE_STATUS, true, "Error: program is invalid: ");

	//---------------------------------- Make Camera ----------------------------------------------
	std::vector<Camera*> ListOfCameras;
	Camera* Camera1 = new Camera(vec3(0,0,-3));
	ListOfCameras.push_back(Camera1);
	Camera* Camera2 = new Camera(vec3(-3, 0, 0));
	ListOfCameras.push_back(Camera2);
	int ActiveCamera = 0;

	float XRotator;
	float YRotator;
	float ZRotator;
	//------------------------------------------- Main Loop -----------------------------------------
	while (true) 
	{
		XRotator = 0;
		YRotator = 0;
		ZRotator = 0;
		//Get Input
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_KEYDOWN)
			{
				//If W is down
				if (event.key.keysym.sym == SDLK_w)
				{
					//Set X rotator to be 1
					XRotator = 0.1;
				}
				else if (event.key.keysym.sym == SDLK_s)
				{
					//Set X rotator to be 1
					XRotator = -0.1;
				}
				if (event.key.keysym.sym == SDLK_a)
				{
					//Set X rotator to be 1
					YRotator = -0.1;
				}
				else if (event.key.keysym.sym == SDLK_d)
				{
					//Set X rotator to be 1
					YRotator = 0.1;
				}
				if (event.key.keysym.sym == SDLK_q)
				{
					//Set X rotator to be 1
					ZRotator = -0.1;
				}
				else if (event.key.keysym.sym == SDLK_e)
				{
					//Set X rotator to be 1
					ZRotator = 0.1;
				}
				if (event.key.keysym.sym == SDLK_r)
				{
					//Tril.m_transform->SetRotation(vec3(0, 0, 0));
					//Tri2.m_transform->SetRotation(vec3(0, 0, 0));
					Square1.m_transform->SetRotation(vec3(0, 0, 0));
				}
				if (event.key.keysym.sym == SDLK_1) 
				{
					std::cout << "C Pressed" << std::endl;
					ActiveCamera++;
					if (ActiveCamera > ListOfCameras.size() - 1) 
					{
						ActiveCamera = 0;
					}
				}
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(ShaderProgram);
		GLint modelLoc = glGetUniformLocation(ShaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1 , GL_FALSE, &Square1.m_transform->GetModel()[0][0]);
		GLint viewLoc = glGetUniformLocation(ShaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &ListOfCameras[ActiveCamera]->CalculateViewMatrix()[0][0]);
		GLint PersLoc = glGetUniformLocation(ShaderProgram, "projection");
		glUniformMatrix4fv(PersLoc, 1, GL_FALSE, &ListOfCameras[ActiveCamera]->GetPerspective()[0][0]);
		//glBindVertexArray(VertexArrayObject);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//Tril.Draw(XRotator, YRotator, ZRotator);
		//Tri2.Draw(XRotator, YRotator, ZRotator);
		Square1.Draw(XRotator, YRotator, ZRotator);

		SDL_Delay(16);

		SDL_GL_SwapWindow(window);
	}

	//------------------------------------------- Clean Up ------------------------------------------
	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();

	return 0;
}

