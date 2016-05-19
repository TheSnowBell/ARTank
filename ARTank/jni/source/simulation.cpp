#include "simulation.h"

#include <android/log.h>

#ifdef DEBUG
#define  LOG_TAG    "ARTank"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif

#define WALLMASS 1		// wall box mass
#define ITERS 20		// number of iterations

#define WALLWIDTH 5	// width of wall
#define WALLHEIGHT 5		// height of wall

#define                 SCALAR           30.0

dWorldID Simulation::world = 0;
dSpaceID Simulation::space = 0;
dJointGroupID Simulation::contactgroup = 0;

static void rotR (dMatrix3 T, float x, float y, float z);
static void transR (dMatrix3 T, float x, float y, float z);
static void scaleR(dMatrix3 T, float sx, float sy, float sz);
static void scaleL(dMatrix3 T, float sx, float sy, float sz);
static void atualizarRotacao(dMatrix3 atual, dMatrix3 nova);
static void atualizarPosicao(dMatrix3 atual, dVector3 nova);
dReal grauToRad(dReal grau) { return (M_PI/180.0)*grau; }


Simulation::Simulation()
{
#ifdef DEBUG
	LOGI("Creating simulation...");
#endif
    dInitODE();
    setupSimulation();
}

Simulation::~Simulation()
{
#ifdef DEBUG
	LOGI("Deleting simulation...");
#endif
    shutdownSimulation();
    dCloseODE();
}

void Simulation::setupSimulation()
{

    world = dWorldCreate();
    space = dSweepAndPruneSpaceCreate( 0, dSAP_AXES_XYZ );
    contactgroup = dJointGroupCreate (0);

    dWorldSetGravity (world,0,0,-9.8);
    dWorldSetCFM (world, 1e-5);
    dWorldSetERP (world, 0.2);
    dWorldSetQuickStepNumIterations (world,ITERS);

    ground = dCreatePlane (space,0,0,1,0);
    wb = 0;

    bool offset = false;
    for (dReal z = WBOXSIZE/2.0; z <= WALLHEIGHT; z+=WBOXSIZE)
    {
        offset = !offset;
        for (dReal y = (-WALLWIDTH+z)/2; y <= (WALLWIDTH-z)/2; y+=WBOXSIZE)
        {
            wall_bodies[wb] = dBodyCreate (world);
            dBodySetPosition (wall_bodies[wb],0,y,z);
            dMassSetBox (&m,1,WBOXSIZE,WBOXSIZE,WBOXSIZE);
            dMassAdjust (&m, WALLMASS);
            dBodySetMass (wall_bodies[wb],&m);
            wall_boxes[wb] = dCreateBox (space,WBOXSIZE,WBOXSIZE,WBOXSIZE);
            dGeomSetBody (wall_boxes[wb],wall_bodies[wb]);
            wb++;
        }
    }

    dRSetIdentity(cannoBox);
    dRSetIdentity(cannoCylinder);
    cannon_ball_body = dBodyCreate (world);
    cannon_ball_geom = dCreateSphere (space,CANNON_BALL_RADIUS);
    dMassSetSphereTotal (&m,CANNON_BALL_MASS,CANNON_BALL_RADIUS);
    dBodySetMass (cannon_ball_body,&m);
    dGeomSetBody (cannon_ball_geom,cannon_ball_body);
    dBodySetPosition (cannon_ball_body,0,0,0);
    dBodyDisable(cannon_ball_body);
    dGeomDisable(cannon_ball_geom);

}

void Simulation::shutdownSimulation()
{
    dSpaceDestroy (space);
    dWorldDestroy (world);
    dJointGroupDestroy (contactgroup);
}

void Simulation::simLoop(bool pause)
{
    if (!pause) {
        dSpaceCollide (space,0,&nearCallback);
        dWorldQuickStep (world,0.05);
        dJointGroupEmpty (contactgroup);
    }
}

void Simulation::nearCallback(void *data, dGeomID o1, dGeomID o2)
{
    int i,n;

    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    if (b1 && b2 && dAreConnected(b1, b2))
        return;
    const int N = 4;
    dContact contact[N];
    n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));

    if (n > 0) {
        for (i=0; i<n; i++) {
          //  contact[i].surface.mode = dContactBounce;
            contact[i].surface.mu = dInfinity;
            //contact[i].surface.bounce = 0;
            //contact[i].surface.bounce_vel = 1;
            dJointID c = dJointCreateContact (world,contactgroup,contact+i);
            dJointAttach (c,dGeomGetBody(o1),dGeomGetBody(o2));
        }
    }
}

void Simulation::atirar()
{
    dBodyEnable(cannon_ball_body);
    dGeomEnable(cannon_ball_geom);
    dReal cpos[3] = {cannoBox[3],cannoBox[7],cannoBox[11]};
    for (int i=0; i<3; i++) cpos[i] += 3*cannoCylinder[i*4+2];
    dBodySetPosition (cannon_ball_body,cpos[0],cpos[1],cpos[2]);
    dReal force = 10;
    dBodySetLinearVel (cannon_ball_body,force*cannoCylinder[2],force*cannoCylinder[6],force*cannoCylinder[10]);
    dBodySetAngularVel (cannon_ball_body,0,0,0);
}

void Simulation::reset()
{
#ifdef DEBUG
	LOGI("Reseting simulation...");
#endif
    shutdownSimulation();
    setupSimulation();
}

void Simulation::setMatrixCannoBox(dMatrix3 cannoBox)
{
    for(int i=0; i<12; i++){
        this->cannoBox[i] = cannoBox[i];
    }
}

void Simulation::setMatrixCannoCylinder(dMatrix3 cannoCylinder)
{
    for(int i=0; i<12; i++){
        this->cannoCylinder[i] = cannoCylinder[i];
    }
}

void Simulation::AttMatrixCanno(dMatrix3 markerBaseMat, dMatrix3 markerGeomMat, dReal cannon_elevation)
{
     dMatrix3 dTransMat;

     dMULTIPLY1_333(dTransMat,markerBaseMat,markerGeomMat); //dTransMat = (object[1].trans)^-1.(object[0].trans)

     //translacao (vetor de translacao - parte 3x1 - direita)
     dVector3 markerBaseVec, markerGeomVec;
     dVector3 transMarkerBaseVec, transMarkerGeomVec;
     markerBaseVec[0] = markerBaseMat[0*4+3]; //object[1].trans[0][3];
     markerBaseVec[1] = markerBaseMat[1*4+3]; //object[1].trans[1][3];
     markerBaseVec[2] = markerBaseMat[2*4+3]; //object[1].trans[2][3];

     markerGeomVec[0] = markerGeomMat[0*4+3];
     markerGeomVec[1] = markerGeomMat[1*4+3];
     markerGeomVec[2] = markerGeomMat[2*4+3];

     dMULTIPLY1_331(transMarkerBaseVec,markerBaseMat,markerBaseVec); //transMarkerBaseVec = markerBaseMat^t.markerBaseVec
     dMULTIPLY1_331(transMarkerGeomVec,markerBaseMat,markerGeomVec); //transMarkerGeomVec = markerBaseMat^t.markerGeomVec

     dTransMat[3]  = transMarkerGeomVec[0] - transMarkerBaseVec[0];
     dTransMat[7]  = transMarkerGeomVec[1] - transMarkerBaseVec[1];
     dTransMat[11]  = transMarkerGeomVec[2] - transMarkerBaseVec[2];

     //Global
     scaleL(dTransMat,SCALAR,SCALAR,SCALAR);

     scaleR(dTransMat,SCALAR,SCALAR,SCALAR);

     transR(dTransMat,0,0,1);
     Simulation::setMatrixCannoBox(dTransMat);
     rotR(dTransMat,0,SCALAR*cannon_elevation,0);
     transR(dTransMat,0,0,1);
     Simulation::setMatrixCannoCylinder(dTransMat);
}

static void rotR (dMatrix3 T, float x, float y, float z){
    dMatrix3 rot,auxRotacao;

    dRFromEulerAngles (rot,grauToRad(-x), grauToRad(-y), grauToRad(-z));
    dMULTIPLY0_333(auxRotacao,T,rot);
    atualizarRotacao(T,auxRotacao);
}

static void transR (dMatrix3 T, float x, float y, float z){
    dVector3 trans;
    dVector3 auxPosicao;
    trans[0] = x;
    trans[1] = y;
    trans[2] = z;

    dMULTIPLY0_331(auxPosicao,T,trans);
    for (int i = 0; i<3; i++) T[i*4+3] = auxPosicao[i] + T[i*4+3];
}

static void scaleR(dMatrix3 T, float sx, float sy, float sz){
    dMatrix3 Mscalar,auxScalar;

    for(int i=0;i<12;i++){
        Mscalar[i] = 0;
    }

    Mscalar[0*4+0] = sx;
    Mscalar[1*4+1] = sy;
    Mscalar[2*4+2] = sz;

    dMULTIPLY0_333(auxScalar,T,Mscalar);

    atualizarRotacao(T,auxScalar);
}

//A Letra L so indica a ordem de multiplicação e não a direção da rotação

static void scaleL(dMatrix3 T,float sx, float sy, float sz){
    dMatrix3 auxRotacao,Mscalar,auxPosicao;
    dVector3 posicao;
    for (int i = 0; i<3; i++) posicao[i] = T[i*4+3];

    for(int i=0;i<12;i++){
        Mscalar[i] = 0;
    }

    Mscalar[0*4+0] = 1/sx;
    Mscalar[1*4+1] = 1/sy;
    Mscalar[2*4+2] = 1/sz;

    dMULTIPLY0_333(auxRotacao,Mscalar,T);
    dMULTIPLY0_331(auxPosicao,Mscalar,posicao);

    atualizarRotacao(T,auxRotacao);
    atualizarPosicao(T,auxPosicao);
}

static void atualizarRotacao(dMatrix3 atual, dMatrix3 nova){
    for (int i=0; i<3;i++){
        for(int j=0;j<3;j++){
            atual[i*4+j] = nova[i*4+j];
        }
    }
}

static void atualizarPosicao(dMatrix3 atual, dVector3 nova){
    for (int i = 0; i<3; i++) atual[i*4+3] = nova[i];
}

