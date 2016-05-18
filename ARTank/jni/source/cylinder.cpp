#include "cylinder.h"

static void circleTableCylinder(GLfloat **sint, GLfloat **cost, const int n, const GLboolean halfCircle)
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
        printf("Failed to allocate memory in CircleTable");
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

static void generateCylinder(
    GLfloat radius, GLfloat height, GLint slices, GLint stacks, /*  input */
    std::vector<GLfloat> *vertices, std::vector<GLfloat> *normals, int* nVert           /* output */
    )
{
    int i,j;

    /* Step in z as stacks are drawn. */
    GLfloat radf = (GLfloat)radius;
    GLfloat z;
    const GLfloat zStep = (GLfloat)height / ( ( stacks > 0 ) ? stacks : 1 );

    /* Pre-computed circle */
    GLfloat *sint,*cost;

    /* number of unique vertices */
    if (slices==0 || stacks<1)
    {
        /* nothing to generate */
        *nVert = 0;
        return;
    }
    *nVert = slices*(stacks+3)+2;   /* need two extra stacks for closing off top and bottom with correct normals */

    if ((*nVert) > 65535)
        /*
             * limit of glushort, thats 256*256 subdivisions, should be enough in practice. See note above
             */
        printf("Too many slices or stacks requested, indices will wrap");

    /* Pre-computed circle */
    circleTableCylinder(&sint,&cost,-slices,GL_FALSE);

    z=0;
    /* top on Z-axis */
    vertices->push_back(0.f);
    vertices->push_back(0.f);
    vertices->push_back(0.f);

    normals->push_back(0.f);
    normals->push_back(0.f);
    normals->push_back(-1.f);

    /* other on top (get normals right) */
    for (j=0; j<slices; j++)
    {
        vertices->push_back(cost[j]*radf);
        vertices->push_back(sint[j]*radf);
        vertices->push_back(z);

        normals->push_back(0.f);
        normals->push_back(0.f);
        normals->push_back(-1.f);
    }

    /* each stack */
    for (i=0; i<stacks+1; i++ )
    {
        for (j=0; j<slices; j++)
        {
            vertices->push_back(cost[j]*radf);
            vertices->push_back(sint[j]*radf);
            vertices->push_back(z);

            normals->push_back(cost[j]);
            normals->push_back(sint[j]);
            normals->push_back(0.f);
        }

        z += zStep;
    }

    /* other on bottom (get normals right) */
    z -= zStep;
    for (j=0; j<slices; j++)
    {
        vertices->push_back(cost[j]*radf);
        vertices->push_back(sint[j]*radf);
        vertices->push_back(z);

        normals->push_back(0.f);
        normals->push_back(0.f);
        normals->push_back(1.f);
    }

    /* bottom */
//    vertices->push_back(0.f);
//    vertices->push_back(0.f);
//    vertices->push_back(height);

//    normals->push_back(0.f);
//    normals->push_back(0.f);
//    normals->push_back(1.f);

    /* Release sin and cos tables */
    free(sint);
    free(cost);
}


static void createCylinder( GLfloat radius, GLfloat height, GLint slices, GLint stacks,
                     std::vector<GLfloat> *vertices, std::vector<GLfloat> *normals,
                     std::vector<GLushort> *vertIdxs)
{
    int i,j, nVert;

    /* Generate vertices and normals */
    /* Note, (stacks+1)*slices vertices for side of object, 2*slices+2 for top and bottom closures */
    generateCylinder(radius,height,slices,stacks,vertices,normals,&nVert);

    if (nVert==0)
        /* nothing to draw */
        return;


    /* First, generate vertex index arrays for drawing with glDrawElements
         * All stacks, including top and bottom are covered with a triangle
         * strip.
         */

    /* Create index vector */
    GLushort offset;

    /* top stack */
    for (j=0;  j<slices;  j++)
    {
        vertIdxs->push_back(0);
        vertIdxs->push_back(j+1);
    }
    vertIdxs->push_back(0);
    vertIdxs->push_back(1);

    /* middle stacks: */
    /* Strip indices are relative to first index belonging to strip, NOT relative to first vertex/normal pair in array */
    for (i=0; i<stacks; i++)
    {
        offset = 1+(i+1)*slices;                /* triangle_strip indices start at 1 (0 is top vertex), and we advance one stack down as we go along */
        for (j=0; j<slices; j++)
        {
            vertIdxs->push_back(offset+j);
            vertIdxs->push_back(offset+j+slices);
        }
        vertIdxs->push_back(offset);
        vertIdxs->push_back(offset+slices);
    }

    /* top stack */
    offset = 1+(stacks+2)*slices;
    for (j=0; j<slices; j++)
    {
        vertIdxs->push_back(offset+j);
        vertIdxs->push_back(nVert-1);
    }
    vertIdxs->push_back(offset);
    vertIdxs->push_back(nVert-1);

}


Cylinder::Cylinder(GLfloat radius, GLfloat height, GLint slices, GLint stacks)
{

    this->radius = radius;
    this->height = height;
    this->slices = slices;
    this->stacks = stacks;

    createCylinder(radius, height, slices, stacks, vertices, normals, vertIdxs);
}

Cylinder::~Cylinder()
{

}

void Cylinder::draw()
{
    glPushMatrix();
        glTranslatef(0,0,-height*0.5f);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, vertices->data());
        glNormalPointer(GL_FLOAT, 0, normals->data());

        glDrawElements(GL_TRIANGLE_STRIP, vertIdxs->size(), GL_UNSIGNED_SHORT, vertIdxs->data());

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    glPopMatrix();
}

GLfloat Cylinder::getRadius()
{
    return this->radius;
}

GLfloat Cylinder::getHeight()
{
    return this->height;
}
