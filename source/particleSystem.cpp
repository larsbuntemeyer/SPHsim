//
//  particleSystem.cpp
//  SPHsim
//
//  Created by Lars on 02.08.14.
//  Copyright (c) 2014 Lars. All rights reserved.
//

#include "particleSystem.h"

particleSystem::particleSystem(){
    printf("Creating empty Particle System\n");
    numberOfParticles = 0;
    init();
}

particleSystem::particleSystem(int n){
    printf("Creating new Particle System\n");
    numberOfParticles = n;
    printf("number of particles: %d\n", numberOfParticles);
    init();
}

void particleSystem::init(){
    spacing = 2.f;
    particleSize = spacing;
    globalForce = Vec2(0.0,0.0);
    for (int i=0; i<numberOfParticles; i++) {
        particle p;
        particles.push_back(p);
    }
}

void particleSystem::addParticle(float x, float y){
    particle p;
    p.pos = Vec2(x,y);
    particles.push_back(p);
    numberOfParticles=particles.size();
}

int particleSystem::getSize(){
    return numberOfParticles;
}

void particleSystem::addGlobalForce(Vec2 f){
    for (int i=0; i<numberOfParticles; i++) {
        particles[i].force += f;
    }
}

void particleSystem::eraseParticle(int i){
    particles.erase(particles.begin()+i);
    numberOfParticles=particles.size();
}

void particleSystem::draw(){
	// Draw Fluid Particles
	glPointSize(particleSize*2);
	glBegin(GL_POINTS);
	for(int i=0; i < particles.size(); ++i)
	{
			// We'll let the color be determined by 
			// ... pressure for the blue component
			// ... x-velocity for the red component
			// ... y-velocity for the green-component
        float c = .1 * particles[i].rho;
        float p = log(particles[i].press);
        float x = 20 * fabs(particles[i].vel.x);
        float y = 20 * fabs(particles[i].vel.y);

		glColor3f(.3+x,.3+y,.3+c);
		glVertex2f(particles[i].pos.x, particles[i].pos.y);
	}	
	glEnd();
}