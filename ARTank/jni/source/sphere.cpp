#include "sphere.h"

static void circleTableSphere(GLfloat **sint, GLfloat **cost, const int n, const GLboolean halfCircle)
{
    int i;

    /* Table size, the sign of n flips the circle direction */
    const int size = abs(n);

    /* Determine the angle between samples */
    const GLfloat angle = (halfCircle?1:2)*(GLfloat)M_PI/(GLfloat)( ( n == 0 ) ? 1 : n );

    /* Allocate memory for n samples, plus duplicate of first entry at the end */
    *sint = (GLfloat*) malloc(sizeof(GLfloat) * (size+1));
    *cost = (GLfloat*) malloc(sizeof(GLfloat) * (size+1));

    /* Bail out if memory allocation fails, fgError never returns */
    if (!(*sint) || !(*cost))
    {
        free(*sint);
        free(*cost);
        printf("Failed to allocate memory in fghCircleTable");
    }

    /* Compute cos and sin around the circle */
    (*sint)[0] = 0.0;
    (*cost)[0] = 1.0;

    for (i=1; i<size; i++)
    {
        (*sint)[i] = (GLfloat)sin(angle*i);
        (*cost)[i] = (GLfloat)cos(angle*i);
    }


    if (halfCircle)
    {
        (*sint)[size] =  0.0f;  /* sin PI */
        (*cost)[size] = -1.0f;  /* cos PI */
    }
    else
    {
        /* Last sample is duplicate of the first (sin or cos of 2 PI) */
        (*sint)[size] = (*sint)[0];
        (*cost)[size] = (*cost)[0];
    }

}

static void generateSphere(GLfloat radius, GLint slices, GLint stacks,
                    std::vector<GLfloat> *vertices, std::vector<GLfloat> *normals, int *numVertices){

    int i,j;
    GLfloat x,y,z;

    /* Pre-computed circle */
    GLfloat *sint1,*cost1;
    GLfloat *sint2,*cost2;

    /* number of unique vertices */
    if (slices==0 || stacks<2)
    {
        /* nothing to generate */
        *numVertices = 0;
        return;
    }
    *numVertices = slices*(stacks-1)+2;
    if ((*numVertices) > 65535)
        /*
            * limit of glushort, thats 256*256 subdivisions, should be enough in practice. See note above
            */
        printf("too many slices or stacks requested, indices will wrap");

    /* precompute values on unit circle */
    circleTableSphere(&sint1,&cost1,-slices,GL_FALSE);
    circleTableSphere(&sint2,&cost2, stacks,GL_TRUE);

    /* top */
    vertices->push_back(0.f);
    vertices->push_back(0.f);
    vertices->push_back(radius);

    normals->push_back(0.f);
    normals->push_back(0.f);
    normals->push_back(1.f);

    /* each stack */
    for( i=1; i<stacks; i++ )
    {
        for(j=0; j<slices; j++)
        {
            x = cost1[j]*sint2[i];
            y = sint1[j]*sint2[i];
            z = cost2[i];

            vertices->push_back(x*radius);
            vertices->push_back(y*radius);
            vertices->push_back(z*radius);

            normals->push_back(x);
            normals->push_back(y);
            normals->push_back(z);
        }
    }

    /* bottom */
    vertices->push_back(0.f);
    vertices->push_back(0.f);
    vertices->push_back(-radius);

    normals->push_back(0.f);
    normals->push_back(0.f);
    normals->push_back(-1.f);

    /* Done creating vertices, release sin and cos tables */
    free(sint1);
    free(cost1);
    free(sint2);
    free(cost2);
}

static void createSphere(GLfloat radius, GLint slices, GLint stacks,
               std::vector<GLfloat> *vertices, std::vector<GLfloat> *normals, std::vector<GLushort> *vertIdxs){

    int i,j,numVertices;

    /* Generate vertices and normals */
    generateSphere(radius, slices, stacks, vertices, normals, &numVertices);

    if (numVertices==0)
        /* nothing to draw */
        return;

    /*
    * First, generate vertex index arrays for drawing with glDrawElements
    * All stacks, including top and bottom are covered with a triangle
    * strip.
    */
    GLushort offset;

    /* top stack */
    for (j=0;  j<slices;  j++)
    {
        vertIdxs->push_back(j+1);
        vertIdxs->push_back(0);
    }

    vertIdxs->push_back(1);
    vertIdxs->push_back(0);

    /* middle stacks: */
    /* Strip indices are relative to first index belonging to strip, NOT relative to first vertex/normal pair in array */
    for (i=0; i<stacks-2; i++)
    {
        offset = 1+i*slices;                    /* triangle_strip indices start at 1 (0 is top vertex), and we advance one stack down as we go along */
        for (j=0; j<slices; j++)
        {
            vertIdxs->push_back(offset+j+slices);
            vertIdxs->push_back(offset+j);
        }
        vertIdxs->push_back(offset+slices);
        vertIdxs->push_back(offset);
    }

    /* bottom stack */
    offset = 1+(stacks-2)*slices;               /* triangle_strip indices start at 1 (0 is top vertex), and we advance one stack down as we go along */
    for (j=0; j<slices; j++)
    {
        vertIdxs->push_back(numVertices-1);
        vertIdxs->push_back(offset+j);
    }
    vertIdxs->push_back(numVertices-1);
    vertIdxs->push_back(offset);

}

Sphere::Sphere(GLfloat radius, GLint slices, GLint stacks)
{
    this->radius = radius;
    this->slices = slices;
    this->stacks = stacks;

    createSphere(radius, slices, stacks, vertices, normals, vertIdxs);
}

void Sphere::draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices->data());
    glNormalPointer(GL_FLOAT, 0, normals->data());

    glDrawElements(GL_TRIANGLE_STRIP, vertIdxs->size(), GL_UNSIGNED_SHORT, vertIdxs->data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

Sphere::~Sphere()
{
    //Geometry3D faz o free nos vectores
}

GLfloat Sphere::getRadius()
{
    return this->radius;
}
