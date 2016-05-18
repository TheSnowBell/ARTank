#include "capsule.h"

Capsule::Capsule(GLfloat radius, GLfloat height, GLint slices, GLint stacks)
{
    this->radius = radius;
    this->height = height;
    this->slices = slices;
    this->stacks = stacks;

    sphere = new Sphere(radius, slices, stacks);
    cylinder = new Cylinder(radius, height, slices, stacks);
}

Capsule::~Capsule()
{
    delete sphere;
    delete cylinder;
}

void Capsule::draw()
{
    glPushMatrix();
        cylinder->draw();
        glTranslatef(0,0,-height*0.5f);
        sphere->draw();
        glTranslatef(0,0,height);
        sphere->draw();
    glPopMatrix();
}

GLfloat Capsule::getRadius()
{
    return this->radius;
}

GLfloat Capsule::getHeight()
{
    return this->height;
}

