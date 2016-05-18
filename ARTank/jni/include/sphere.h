#ifndef SPHERE_H
#define SPHERE_H

#include "geometry3d.h"
#include <stdio.h>
#include <math.h>

class Sphere : public Geometry3D
{

private:
    GLint slices, stacks;
    GLfloat radius;

public:
    Sphere(GLfloat radius, GLint slices, GLint stacks);
    virtual void draw();
    ~Sphere();

    GLfloat getRadius();

};

#endif // SPHERE_H
