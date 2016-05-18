#include "cube.h"

static void createCube( GLfloat dSize, std::vector<GLfloat> *vertices,
                     std::vector<GLfloat> *normals, std::vector<GLushort> *vertIdxs)
{
    GLfloat tempVert[] = {  // front
      -.5f, -.5f,  .5f,
       .5f, -.5f,  .5f,
       .5f,  .5f,  .5f,
      -.5f,  .5f,  .5f,
      // top
      -.5f,  .5f,  .5f,
       .5f,  .5f,  .5f,
       .5f,  .5f, -.5f,
      -.5f,  .5f, -.5f,
      // back
       .5f, -.5f, -.5f,
      -.5f, -.5f, -.5f,
      -.5f,  .5f, -.5f,
       .5f,  .5f, -.5f,
      // bottom
      -.5f, -.5f, -.5f,
       .5f, -.5f, -.5f,
       .5f, -.5f,  .5f,
      -.5f, -.5f,  .5f,
      // left
      -.5f, -.5f, -.5f,
      -.5f, -.5f,  .5f,
      -.5f,  .5f,  .5f,
      -.5f,  .5f, -.5f,
      // right
       .5f, -.5f,  .5f,
       .5f, -.5f, -.5f,
       .5f,  .5f, -.5f,
       .5f,  .5f,  .5f,
       };

    GLfloat tempNormals[]  = { 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0,v1,v2,v3 (front)
                               0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0,v5,v6,v1 (top)
                               0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1,   // v4,v7,v6,v5 (back)
                               0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7,v4,v3,v2 (bottom)
                               -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1,v6,v7,v2 (left)
                               1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0   // v0,v3,v4,v5 (right)
                             };

    GLubyte tempIndices[]  = { 0, 1, 2,   2, 3, 0,      // front
                             4, 5, 6,   6, 7, 4,      // right
                             8, 9,10,  10,11, 8,      // top
                            12,13,14,  14,15,12,      // left
                            16,17,18,  18,19,16,      // bottom
                            20,21,22,  22,23,20 };    // back

    //72 = Tamanho do vetor tempVert e tempNormals
    for (int i=0; i<72; i++){
        vertices->push_back(dSize*tempVert[i]); //Calculo do tamanho do cubo eh feito aqui
        normals->push_back(tempNormals[i]);
    }

    //36 = Tamanho do vetor tempIndices
    for(int i=0;i<36;i++){
        vertIdxs->push_back(tempIndices[i]);
    }
}

static void createBox( GLfloat x, GLfloat y, GLfloat z, std::vector<GLfloat> *vertices,
                     std::vector<GLfloat> *normals, std::vector<GLushort> *vertIdxs){

    GLfloat lx = x * 0.5f;
    GLfloat ly = y * 0.5f;
    GLfloat lz = z * 0.5f;

    GLfloat tempVert[] = {  // front
      -lx, -ly,  lz,
       lx, -ly,  lz,
       lx,  ly,  lz,
      -lx,  ly,  lz,
      // top
      -lx,  ly,  lz,
       lx,  ly,  lz,
       lx,  ly, -lz,
      -lx,  ly, -lz,
      // back
       lx, -ly, -lz,
      -lx, -ly, -lz,
      -lx,  ly, -lz,
       lx,  ly, -lz,
      // bottom
      -lx, -ly, -lz,
       lx, -ly, -lz,
       lx, -ly,  lz,
      -lx, -ly,  lz,
      // left
      -lx, -ly, -lz,
      -lx, -ly,  lz,
      -lx,  ly,  lz,
      -lx,  ly, -lz,
      // right
       lx, -ly,  lz,
       lx, -ly, -lz,
       lx,  ly, -lz,
       lx,  ly,  lz,
       };

    GLfloat tempNormals[]  = { 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0,v1,v2,v3 (front)
                                1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0,v3,v4,v5 (right)
                                0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0,v5,v6,v1 (top)
                               -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1,v6,v7,v2 (left)
                                0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7,v4,v3,v2 (bottom)
                                0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1 }; // v4,v7,v6,v5 (back)

    GLubyte tempIndices[]  = { 0, 1, 2,   2, 3, 0,      // front
                             4, 5, 6,   6, 7, 4,      // right
                             8, 9,10,  10,11, 8,      // top
                            12,13,14,  14,15,12,      // left
                            16,17,18,  18,19,16,      // bottom
                            20,21,22,  22,23,20 };    // back

    //72 = Tamanho do vetor tempVert e tempNormals
    for (int i=0; i<72; i++){
        vertices->push_back(tempVert[i]); //Calculo do tamanho do cubo eh feito aqui
        normals->push_back(tempNormals[i]);
    }

    //36 = Tamanho do vetor tempIndices
    for(int i=0;i<36;i++){
        vertIdxs->push_back(tempIndices[i]);
    }
}

Cube::Cube(GLfloat dSize)
{
    this->dSize = dSize;

    createCube( dSize, vertices, normals, vertIdxs );
}

Cube::Cube(GLfloat x, GLfloat y, GLfloat z)
{
    this->dSize = dSize;

    createBox( x, y, z, vertices, normals, vertIdxs );
}

void Cube::draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices->data());
    glNormalPointer(GL_FLOAT, 0, normals->data());

    glDrawElements(GL_TRIANGLES, vertIdxs->size(), GL_UNSIGNED_SHORT, vertIdxs->data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

Cube::~Cube()
{

}


