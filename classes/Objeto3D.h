#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glsl.h"
#include "../glm/glm.h"
#include <string>
#include <string.h>

class Objeto3D
{
private:
	GLMmodel* objmodel_ptr;
	bool hasTexture;
public:
	Objeto3D();
	void SetHasTexture(bool flag);
	void AbrirMalla(std::string pathMalla);
	void DibujarMalla(float x, float y, float z, GLuint texid = 0);
};

