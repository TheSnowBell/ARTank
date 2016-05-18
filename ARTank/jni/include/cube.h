#ifndef CUBE_H
#define CUBE_H

#include "geometry3d.h"
#include <stdio.h>
#include <math.h>

class Cube : public Geometry3D
{

private:
    GLfloat dSize;
    GLfloat x, y, z;

public:
    Cube(GLfloat dSize);
    Cube(GLfloat x, GLfloat y, GLfloat z);
    void draw();
    ~Cube();

};

#endif // CUBE_H
