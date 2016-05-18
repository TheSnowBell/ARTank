#ifndef CYLINDER_H
#define CYLINDER_H

#include "geometry3d.h"
#include <stdio.h>
#include <math.h>

class Cylinder : public Geometry3D
{

private:
    GLfloat radius, height;
    GLint slices, stacks;

public:
    Cylinder(GLfloat radius, GLfloat height, GLint slices, GLint stacks);
    ~Cylinder();
    virtual void draw();

    GLfloat getRadius();
    GLfloat getHeight();

};

#endif // CYLINDER_H
