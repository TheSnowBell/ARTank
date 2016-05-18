#ifndef GEOMETRY3D_H
#define GEOMETRY3D_H

#include <GLES/gl.h>
#include <vector>
#include <stdlib.h>

class Geometry3D
{

public:
    Geometry3D();
    virtual ~Geometry3D();
    virtual void draw() = 0;

protected:
    std::vector<GLfloat> *vertices;
    std::vector<GLfloat> *normals;
    std::vector<GLushort> *vertIdxs;

};

#endif // GEOMETRY3D_H
