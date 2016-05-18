#include "geometry3d.h"

Geometry3D::Geometry3D()
{
    vertices = new std::vector<GLfloat>();
    normals = new std::vector<GLfloat>();
    vertIdxs = new std::vector<GLushort>();

}

Geometry3D::~Geometry3D()
{
    delete vertices;
    delete vertIdxs;
    delete normals;
}
