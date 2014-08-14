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
    simW = 50.f;
    bottom = 0.0;
    lifetime = 2000.0;
    kill = false;
    particleType = basic;
    setup = chaos;
    particleSize = spacing;
    globalForce = Vec2(0.0,0.0);
    setupParticles();
}

void particleSystem::setupParticles(){
    float wx;
    float wy;
    switch (setup) {
        case empty:
            break;
        case cube:
            wx = simW/4.f;
            wy = simW;
            for (float y=bottom+1; y<wy; y+=spacing) {
            for (float x=-wx; x<wx; x+=spacing) {
                if (particles.size()>numberOfParticles) break;
                particle p;
                p.pos.x = x;
                p.pos.y = y;
                particles.push_back(p);
            }
            }
            break;
        case chaos:
            wx = simW/4.f;
            wy = simW;
            for (float y=bottom; y<simW; y+=spacing) {
            for (float x=-wx; x<wx; x+=spacing) {
                if (particles.size()>numberOfParticles) break;
                particle p;
                p.pos.x = x;
                p.pos.y = y;
                p.vel = Vec2(randab(-0.1,0.1),randab(-0.1,0.1));
                particles.push_back(p);
            }
            }
            break;
        case dambreak:
            break;
            
        default:
            break;
    }
    for (int i=0; i<particles.size(); i++) {
        particles[i].lifetime = lifetime;
    }
}

void particleSystem::addParticle(Vec2 x){
    particle p;
    p.pos = x;
    p.lifetime = lifetime;
    p.vel = Vec2(randab(-0.1,0.1),randab(-0.1,0.1));
    particles.push_back(p);
}


void particleSystem::addGlobalForce(Vec2 f){
    printf("Setting Global Force: %f, %f\n",f.x,f.y);
    globalForce = f;
}

void particleSystem::eraseParticle(int i){
    particles.erase(particles.begin()+i);
}

void particleSystem::draw(){
	// Draw Particles
	glPointSize(particleSize*2);
	glBegin(GL_POINTS);
	for(int i=0; i < particles.size(); ++i)
	{
			// We'll let the color be determined by 
			// ... pressure for the blue component
			// ... x-velocity for the red component
			// ... y-velocity for the green-component
        float x = 1.f-particles[i].lifetime / lifetime;

		glColor3f(x,x,x);
		glVertex2f(particles[i].pos.x, particles[i].pos.y);
	}	
	glEnd();
}

void particleSystem::advance(float timestep){
   
    dt = timestep;
	// For each particles i ...
	#pragma omp parallel for
	for(int i=0; i < particles.size(); ++i)
	{
		// Normal verlet stuff
		particles[i].pos_old = particles[i].pos;
		particles[i].pos += particles[i].vel * dt;

		// Apply the currently accumulated forces
		particles[i].pos += particles[i].force * dt * dt;

		// Restart the forces with gravity only. We'll add the rest later.
		particles[i].force = globalForce;

		// Calculate the velocity for later.
		particles[i].vel = (particles[i].pos - particles[i].pos_old)/dt;

		// If the velocity is really high, we're going to cheat and cap it.
		// This will not damp all motion. It's not physically-based at all. Just
		// a little bit of a hack.
		float max_vel = 2.f;
		float vel_mag = particles[i].vel.len2();
		// If the velocity is greater than the max velocity, then cut it in half.
		if(vel_mag > max_vel*max_vel)
			particles[i].vel = particles[i].vel * .5f;

		// If the particle is outside the bounds of the world, then
		// Make a little spring force to push it back in.
		if(particles[i].pos.x < -simW) particles[i].force.x -= (particles[i].pos.x - -simW) / 8;
		if(particles[i].pos.x >  simW) particles[i].force.x -= (particles[i].pos.x - simW) / 8;
		if(particles[i].pos.y < bottom) particles[i].force.y -= (particles[i].pos.y - bottom) / 8;
		if(particles[i].pos.y > simW*2) particles[i].force.y -= (particles[i].pos.y - simW*2) / 8;

		// Handle the mouse attractor. 
		// It's a simple spring based attraction to where the mouse is.
       // float attr_dist2 = (particles[i].pos - attractor).len2();
       //const float attr_l = simW/4;
       //if( attracting )
       //      if( attr_dist2 < attr_l*attr_l )
       //        particles[i].force -= (particles[i].pos - attractor) / 256;
	
		// Reset the nessecary items.
		particles[i].rho = particles[i].rho_near = 0;
		particles[i].neighbors.clear();
	}
    if (kill) {
       for (int i = particles.size()-1; i >= 0; i--)
        {
           particles[i].lifetime -= dt;
           if (particles[i].lifetime < 0.0) eraseParticle(i);
       }
    }
}
