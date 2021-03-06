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
#include "OBJLoader.h"

GLuint LoadTexture(string TextureLocation) 
{
	GLuint textureID;

	int width, height, numComponents;
	unsigned char* ImageData = stbi_load(TextureLocation.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);
	if (ImageData == NULL) 
	{
		std::cerr << "Texture Loading Failed for Texture: " << TextureLocation << std::endl;
	}
	GLenum format = 0;
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


	//---------------------------------- Make Camera ----------------------------------------------
	std::vector<Camera*> ListOfCameras;
	Camera* Camera1 = new Camera(vec3(0,0,70));
	ListOfCameras.push_back(Camera1);
	Camera* Camera2 = new Camera(vec3(0, 0, 0));
	ListOfCameras.push_back(Camera2);
	int ActiveCamera = 0;

	float XRotator;
	float YRotator;
	float ZRotator;

	//------------------------------------- New Shader Object -----------------------------------
	Shader* basicShader = new Shader("Resources\\Basic",* ListOfCameras[ActiveCamera]);

	//---------------------------------------- OBJ File -----------------------------------------
	string AmbiantLoc;
	string DiffuseLoc;
	string SpecLoc;
	string NormalLoc;

	vector<glm::uint> Indecies;

	std::vector<Vertex> LoadedVerts = OBJLoader::LoadOBJ("Resources/Block", "blocks_01.obj", AmbiantLoc, DiffuseLoc, SpecLoc, NormalLoc, Indecies);

	GLuint AmbiantTextureID = LoadTexture("Resources/Block/" + AmbiantLoc);
	GLuint DiffuseTextureID = LoadTexture("Resources/Block/" + DiffuseLoc);
	GLuint SpeculerTextureID = LoadTexture("Resources/Block/" + SpecLoc);
	GLuint NormalTextureID = LoadTexture("Resources/Block/" + NormalLoc);

	//Create Cube Mesh
	Mesh Cube(&LoadedVerts[0], LoadedVerts.size(), &Indecies[0], Indecies.size());
	Cube.m_transform->SetPosition(vec3(0, 0, 0));

	//------------------------------------ New Light --------------------------------------------
	LightBase* light = new LightBase();

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

					Cube.m_transform->SetRotation(vec3(0, 0, 0));
				}
				if (event.key.keysym.sym == SDLK_1) 
				{
					std::cout << "1 Pressed" << std::endl;
					ActiveCamera++;
					if (ActiveCamera > ListOfCameras.size() - 1) 
					{
						ActiveCamera = 0;
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
		
		light->m_Transform.SetPosition(Camera1->CameraTransform->GetPosition());
	
		//Cube Draw
		Cube.Draw(XRotator, YRotator, ZRotator);

		basicShader->Update(*Cube.m_transform, *light);


		light->Draw(ListOfCameras[ActiveCamera]);

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

