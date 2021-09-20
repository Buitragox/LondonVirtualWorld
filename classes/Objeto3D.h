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
public:
	Objeto3D();
	void AbrirMalla(std::string pathMalla);
	void DibujarMalla(float x, float y, float z);
};

