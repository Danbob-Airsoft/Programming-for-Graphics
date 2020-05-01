// TestApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
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
#include "Shader.h"
#include "LightBase.h"

GLuint LoadTexture(string TextureLocation) 
{
	GLuint textureID;

	int width, height, numComponents;
	unsigned char* ImageData = stbi_load(TextureLocation.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);
	if (ImageData == NULL) 
	{
		std::cerr << "Texture Loading Failed for Texture: " << TextureLocation << std::endl;
	}
	GLenum format;
	if (numComponents == 1) 
	{
		format = GL_RED;
	}
	else if (numComponents == 3)
	{
		format = GL_RGB;
	}
	else if (numComponents == 4)
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(ImageData);

	return textureID;
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
	SquareVerticies.push_back(Vertex (vec3(-0.5f, 0.5f, 1.0f), vec2(0,0)));
	SquareVerticies.push_back(Vertex(vec3(0.5f, 0.5f, 1.0f), vec2(1, 0)));
	SquareVerticies.push_back(Vertex(vec3(0.5f, -0.5f, 1.0f), vec2(1, 1)));
	SquareVerticies.push_back(Vertex(vec3(-0.5f, -0.5f, 1.0f), vec2(0,1)));

	unsigned int SquareIndecies[]
	{
		0,1,2,0,2,3
	};

	//------------------------------------------ Mesh Setup -----------------------------------
	//Mesh Tril(Verticies, 3);
	//Mesh Tri2(Verticies2, 3);
	Mesh Square1(&SquareVerticies[0], SquareVerticies.size(), &SquareIndecies[0], 6);

	//---------------------------------- Make Camera ----------------------------------------------
	Camera* Camera1 = new Camera(vec3(0,0,1.5f));

	float XRotator;
	float YRotator;
	float ZRotator;

	//------------------------------------- New Shader Object -----------------------------------
	Shader* basicShader = new Shader("Resources\\Basic",* Camera1);
	GLuint DiffuseTextureID = LoadTexture("brickwall.jpg");
	GLuint NormalTextureID = LoadTexture("brickwall_normal.jpg");

	//------------------------------------ New Light --------------------------------------------
	LightBase* light = new LightBase();
	light->m_Transform.SetPosition(Camera1->CameraTransform->GetPosition());

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
					break;
				}
				else if (event.key.keysym.sym == SDLK_s)
				{
					//Set X rotator to be 1
					XRotator = -0.1;
					break;
				}
				if (event.key.keysym.sym == SDLK_a)
				{
					//Set X rotator to be 1
					YRotator = -0.1;
					break;
				}
				else if (event.key.keysym.sym == SDLK_d)
				{
					//Set X rotator to be 1
					YRotator = 0.1;
					break;
				}
				if (event.key.keysym.sym == SDLK_q)
				{
					//Set X rotator to be 1
					ZRotator = -0.1;
					break;
				}
				else if (event.key.keysym.sym == SDLK_e)
				{
					//Set X rotator to be 1
					ZRotator = 0.1;
					break;
				}
				if (event.key.keysym.sym == SDLK_r)
				{
					Square1.m_transform->SetRotation(vec3(0, 0, 0));
					break;
				}
				if (event.key.keysym.sym == SDLK_c) 
				{
					float xPos = Square1.m_transform->GetPosition().x;
					float yPos = Square1.m_transform->GetPosition().y;
					float zPos = Square1.m_transform->GetPosition().z - 0.2f;
					Square1.m_transform->SetPosition(vec3(xPos, yPos, zPos));
				}
				if (event.key.keysym.sym == SDLK_v)
				{
					if (Square1.m_transform->GetPosition().z < 0)
					{
						float xPos = Square1.m_transform->GetPosition().x;
						float yPos = Square1.m_transform->GetPosition().y;
						float zPos = Square1.m_transform->GetPosition().z + 0.2f;
						Square1.m_transform->SetPosition(vec3(xPos, yPos, zPos));
					}
					else if (Square1.m_transform->GetPosition().z > 0) 
					{
						float xPos = Square1.m_transform->GetPosition().x;
						float yPos = Square1.m_transform->GetPosition().y;
						float zPos = 0.0f;
						Square1.m_transform->SetPosition(vec3(xPos, yPos, zPos));
					}
				}
			}
		}

		glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, 800, 800);

		basicShader->Bind();
		glActiveTexture(GL_TEXTURE0);
		GLuint TextureLoc = glGetUniformLocation(basicShader->GetProgram(), "texture_diffuse");
		glUniform1i(TextureLoc, 0);
		glBindTexture(GL_TEXTURE_2D, DiffuseTextureID);

		glActiveTexture(GL_TEXTURE1);
		TextureLoc = glGetUniformLocation(basicShader->GetProgram(), "texture_normal");
		glUniform1i(TextureLoc, 1);
		glBindTexture(GL_TEXTURE_2D, NormalTextureID);

		basicShader->Update(*Square1.m_transform, *light);

		//Square Draw
		Square1.Draw(XRotator, YRotator, ZRotator);
		light->Draw(Camera1);

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

