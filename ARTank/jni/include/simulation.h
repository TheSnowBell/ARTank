#ifndef SIMULATION_H
#define SIMULATION_H

#include <ode/ode.h>

#define WBOXSIZE 1.0		// size of wall boxes
#define CANNON_X 10		// x position of cannon
#define CANNON_Y 0		// y position of cannon
#define CANNON_BALL_MASS 10	// mass of the cannon ball
#define CANNON_BALL_RADIUS 0.5

class Simulation
{
    dGeomID ground;
    dBodyID wall_bodies[10000];
    dGeomID cannon_ball_geom;
    dMass m;

    static dWorldID world;
    static dSpaceID space;
    static dJointGroupID contactgroup;
    static void nearCallback (void *data, dGeomID o1, dGeomID o2);
    dMatrix3 cannoCylinder, cannoBox;

public:
    Simulation();
    ~Simulation();
    void setupSimulation();
    void shutdownSimulation();
    void simLoop (bool pause);

    void atirar();
    void reset();
    void setMatrixCannoBox(dMatrix3 cannoBox);
    void setMatrixCannoCylinder(dMatrix3 cannoCylinder);
    void AttMatrixCanno(dMatrix3 markerBaseMat, dMatrix3 markerGeomMat, dReal cannon_elevation);
    dGeomID wall_boxes[10000];
    int wb;
    dBodyID cannon_ball_body;
};

#endif // SIMULATION_H

