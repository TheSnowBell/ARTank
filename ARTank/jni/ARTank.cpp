#include "br_artoolkit_artank_ARTankRenderer.h"
#include "br_artoolkit_artank_Simulation.h"

#include <jni.h>
#include <AR/gsub_es.h>
#include <ARWrapper/ARToolKitWrapperExportedAPI.h>
#include <unistd.h>
#include <android/log.h>

#define                 SCALAR           30.0

//Geomatries3D
#include "sphere.h"
#include "capsule.h"
#include "cube.h"

//ODE
#include "simulation.h"

//Util
static void OpenGLtoODE(GLfloat *matriz, dMatrix3 dTransMat);
static void ODEtoOpenGL(dGeomID object, GLfloat *matriz);
static void ODEtoOpenGL2(dBodyID object, GLfloat *matriz);

static Simulation *simulation;
Cube cube(WBOXSIZE,WBOXSIZE,WBOXSIZE);
Sphere cannoBall(CANNON_BALL_RADIUS,20,20);
Cube cannoBox(2,2,2);
Cylinder cannoCylinder( 0.5, 3, 20, 20);
static dReal cannon_elevation=15;
bool isreset;
bool isatirar;
bool isdeleted;

const GLfloat light_white[]  = {0.9, 0.9, 0.9, 1.0};
const GLfloat light_ambient[]  = {0.2, 0.2, 0.2, 1.0};
const GLfloat light_diffuse[]  = { 0.6, 0.6, 0.6, 1.0 };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0,500.0,0.0,1.0 };

const GLfloat mat_ambient[]    = { 0.360784, 0.301961, 0.196078, 1.0 };
const GLfloat mat_diffuse[]    = { 0.360784, 0.301961, 0.196078, 1.0 };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 25.0f };

static int markers[2] = {0};


JNIEXPORT void JNICALL Java_br_artoolkit_artank_ARTankRenderer_initialiseMarkers
  (JNIEnv *, jobject object){
  
	markers[0] = arwAddMarker("single;Data/primeiro.patt;160");
	markers[1] = arwAddMarker("single;Data/quarto.patt;160");
}

JNIEXPORT void JNICALL Java_br_artoolkit_artank_Simulation_initialise
  (JNIEnv *, jobject object){
#ifdef DEBUG
	ARLOG("Creating.....");
#endif
	isreset = false;
	isatirar = false;
	isdeleted = false;
	
	simulation = new Simulation();
}

JNIEXPORT void JNICALL Java_br_artoolkit_artank_ARTankRenderer_surfaceCreated
  (JNIEnv *env, jobject object){
	glStateCacheFlush(); // Make sure we don't hold outdated OpenGL state.
}

JNIEXPORT void JNICALL Java_br_artoolkit_artank_ARTankRenderer_surfaceChanged
  (JNIEnv *env, jobject object, jint, jint){
	// glViewport(0, 0, w, h) has already been set.
}

static void draw(float gl[16], int id){
	glLoadMatrixf(gl);

	glScalef(SCALAR,SCALAR,SCALAR);
	if(id == markers[0]){
		for (int i = 0; i < simulation->wb; i++){
			glPushMatrix();
			//Desenha Wall
				GLfloat gl[16];
				ODEtoOpenGL(simulation->wall_boxes[i], gl);
				glMultMatrixf(gl);
				glColor4f(0.4,0.4,0.4,1);
				cube.draw();
			glPopMatrix();
		}

		for (int i=0; i < simulation->bullets->size(); i++){
		    glPushMatrix();
			    GLfloat gl[16];
  			    ODEtoOpenGL2(simulation->bullets->at(i).body, gl);
			    glMultMatrixf(gl);
			    cannoBall.draw();
		    glPopMatrix();
		}
	}else if(id == markers[1]){

		glPushMatrix();
			glTranslatef(0,0,1);
			glColor4f(1,1,0,1);
			cannoBox.draw();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0,0,1);
			glRotatef(SCALAR*cannon_elevation,0,1,0);
			glTranslatef(0,0,1);
			glColor4f(1,1,0,1);
			cannoCylinder.draw();
		glPopMatrix();
	}

}

JNIEXPORT void JNICALL Java_br_artoolkit_artank_ARTankRenderer_drawFrame
  (JNIEnv *env, jobject object){

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the projection matrix to that provided by ARToolKit.
	float proj[16];
	arwGetProjectionMatrix(proj);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(proj);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
    glLoadIdentity();
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT0);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT,   light_white);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   light_white);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

	int visi = 0;
    for(int i=0; i<2; i++){
    	if (arwQueryMarkerVisibility(markers[i])) {
	    	glStateCacheEnableDepthTest();
			glStateCacheDisableLighting();
			glStateCacheDisableTex2D();

    		visi++;
		    float trans[16];
		    arwQueryMarkerTransformation(markers[i], trans);
		    draw(trans,markers[i]);
		}
	}

}

JNIEXPORT void JNICALL Java_br_artoolkit_artank_Simulation_shutdown
  (JNIEnv *, jobject object){
#ifdef DEBUG
   	ARLOG("Shutdown.....");
#endif
	delete simulation;
}

JNIEXPORT void JNICALL Java_br_artoolkit_artank_Simulation_shot
  (JNIEnv *, jobject object){
#ifdef DEBUG
  	ARLOG("Shoting.....");
#endif
	int count=0;
	for(int i=0; i<2; i++){
		if (arwQueryMarkerVisibility(markers[i])) {
			count++;
		}
	}

	if(count==2){
	   	dMatrix3 markerBaseMat, markerGeomMat;
	    //convertendo em dMatrix3
	   	float trans[16];
	    arwQueryMarkerTransformation(markers[0], trans);
	    OpenGLtoODE(trans, markerBaseMat);

	    arwQueryMarkerTransformation(markers[1], trans);
	    OpenGLtoODE(trans, markerGeomMat);

	    simulation->AttMatrixCanno(markerBaseMat,markerGeomMat, cannon_elevation);
	 	isatirar=true;
//		simulation->atirar();
	}
	
}

JNIEXPORT void JNICALL Java_br_artoolkit_artank_Simulation__1cannonUp
  (JNIEnv *, jobject object){
#ifdef DEBUG
  	ARLOG("Cannon Up.....");
#endif
	cannon_elevation = cannon_elevation - 0.5;
	if(cannon_elevation == -12.0){
		cannon_elevation = 0.0;
	}
}

JNIEXPORT void JNICALL Java_br_artoolkit_artank_Simulation__1cannonDown
  (JNIEnv *, jobject object){
#ifdef DEBUG
  	ARLOG("Cannon Down.....");
#endif
	cannon_elevation = cannon_elevation + 0.5;
	if(cannon_elevation == 12.0){
		cannon_elevation = 0.0;
	}
}

JNIEXPORT void JNICALL Java_br_artoolkit_artank_Simulation_reset
  (JNIEnv *, jobject object){
	isreset = true;
}

JNIEXPORT void JNICALL Java_br_artoolkit_artank_Simulation_running
  (JNIEnv *, jobject object){
	if(isreset){
		simulation->reset();
		isreset=false;
	}
  
    if(isatirar){
		simulation->atirar();
		isatirar=false;
	}
	
 	simulation->simLoop(false);
}

static void OpenGLtoODE(GLfloat *matriz, dMatrix3 dTransMat){

   dTransMat[0] = matriz[0];  dTransMat[4] = matriz[1];  dTransMat[8] = matriz[2];

   dTransMat[1] = matriz[4];  dTransMat[5] = matriz[5];  dTransMat[9] = matriz[6];

   dTransMat[2] = matriz[8];  dTransMat[6] = matriz[9];  dTransMat[10] = matriz[10];

   dTransMat[3] = matriz[12]; dTransMat[7] = matriz[13]; dTransMat[11] = matriz[14];
}

static void ODEtoOpenGL(dGeomID object, GLfloat *matriz){
    const dReal *rotation = dGeomGetRotation(object);
    const dReal *position = dGeomGetPosition(object);

    //Matriz 4x4 OpenGL adicionando a matriz de rotação 3x3 do ODE transposta.
    matriz[0]  = rotation[0];  matriz[1] = rotation[4];  matriz[2] = rotation[8];  matriz[3] = 0;

    matriz[4]  = rotation[1];  matriz[5] = rotation[5];  matriz[6] = rotation[9];  matriz[7] = 0;

    matriz[8]  = rotation[2];  matriz[9] = rotation[6]; matriz[10] = rotation[10];matriz[11] = 0;

    matriz[12] = position[0]; matriz[13] = position[1]; matriz[14] = position[2]; matriz[15] = 1;
}

static void ODEtoOpenGL2(dBodyID object, GLfloat *matriz){
    const dReal *rotation = dBodyGetRotation(object);
    const dReal *position = dBodyGetPosition(object);

    //Matriz 4x4 OpenGL adicionando a matriz de rotação 3x3 do ODE transposta.
    matriz[0]  = rotation[0];  matriz[1] = rotation[4];  matriz[2] = rotation[8];  matriz[3] = 0;

    matriz[4]  = rotation[1];  matriz[5] = rotation[5];  matriz[6] = rotation[9];  matriz[7] = 0;

    matriz[8]  = rotation[2];  matriz[9] = rotation[6]; matriz[10] = rotation[10];matriz[11] = 0;

    matriz[12] = position[0]; matriz[13] = position[1]; matriz[14] = position[2]; matriz[15] = 1;
}

