#ifndef CAPSULE_H
#define CAPSULE_H

#include "geometry3d.h"
#include "sphere.h"
#include "cylinder.h"

class Capsule : public Geometry3D
{

private:
    Sphere *sphere;
    Cylinder *cylinder;

    GLfloat radius;
    GLfloat height;
    GLint slices,  stacks;

public:
    Capsule(GLfloat radius, GLfloat height, GLint slices, GLint stacks);
    ~Capsule();
    virtual void draw();

    GLfloat getRadius();
    GLfloat getHeight();

};

#endif // CAPSULE_H
