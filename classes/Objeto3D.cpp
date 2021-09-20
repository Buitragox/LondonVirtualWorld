#include "Objeto3D.h"

Objeto3D::Objeto3D()
{
    objmodel_ptr = nullptr;
}

void Objeto3D::AbrirMalla(std::string malla)
{
    char path[200];
    strcpy(path, malla.c_str());
    if (!objmodel_ptr)
    {
        objmodel_ptr = glmReadOBJ(path);
        if (!objmodel_ptr)
            exit(0);

        glmUnitize(objmodel_ptr);
        glmFacetNormals(objmodel_ptr);
        glmVertexNormals(objmodel_ptr, 90.0);
    }
}

void Objeto3D::DibujarMalla(float x, float y, float z)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glmDraw(objmodel_ptr, GLM_SMOOTH | GLM_MATERIAL);
    glPopMatrix();
}
