#pragma once
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "Vertex.h"
#include <map>

using namespace std;

class OBJLoader
{
public:
	static vector<Vertex> LoadOBJ(const string& FolderLocconst, const string FileLoc, string& AmbiantLoc, string& DiffLoc, string& specLoc, string& NormalLoc, vector<glm::uint>& indicies);

	static void LoadMaterial(const string& MatLibLoc, string& AmbiantLoc, string& DiffLoc, string& specLoc, string& NormalLoc);

};

#endif // !OBJLOADER_H
