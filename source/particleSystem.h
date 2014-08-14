//
//  particleSystem.h
//  SPHsim
//
//  Created by Lars on 02.08.14.
//  Copyright (c) 2014 Lars. All rights reserved.
//

#ifndef __SPHsim__particleSystem__
#define __SPHsim__particleSystem__

#include <iostream>

#include "math.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

//////////////////////////////////////////
// A structure for holding two neighboring particles and their weighted distances
struct neighbor { int i, j; float q, q2; };

// The particle structure holding all of the relevant information.
struct particle { 
	Vec2 pos, pos_old, vel, force; 
	float mass, rho, rho_near, press, press_near, sigma, beta;
	float lifetime;
	vector<neighbor> neighbors;  
};


enum _particleType {
   basic,sph
};
enum _setup {
   empty,
   cube,
   chaos,
   dambreak
};

class particleSystem{
protected:
    int numberOfParticles;
    int sizeX, sizeY;
    float particleSize;
    vector<particle> particles;
    void init();
    float spacing;   //spacing of particles
    Vec2 globalForce;
    float simW;
    float bottom;
    float dt;
    float lifetime;
    _particleType particleType;
    bool kill;
    void eraseParticle(int i);
    _setup setup;
    void setupParticles();
public:
    particleSystem();
    particleSystem(int n);
    void addParticle(Vec2 x);
    void setNumberOfParticles(int x){numberOfParticles=x;};
    void setBottom(float b){bottom=b;};
    void addGlobalForce(Vec2 f);
    void advance(float timestep);
    vector<particle> getParticles(){return particles;};
    void draw();
    float getSpacing(){return spacing;};
    void setLifetime(float t){lifetime=t;};
    void setWorldSize(float x){simW=x;};
    void setSetup(_setup s){setup=s;};
    void setType(_particleType p){particleType=p;};
    void killParticles(bool k){kill=k;};
    int getType(){return particleType;};
    int getSize(){return particles.size();};
};

#endif /* defined(__SPHsim__particleSystem__) */
