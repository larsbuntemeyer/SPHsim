//
//  sphFluid.cpp
//  SPHsim
//
//  Created by Lars on 09.08.14.
//  Copyright (c) 2014 Lars. All rights reserved.
//

#include "sphFluid.h"

sphFluid::sphFluid(): particleSystem(){
    printf("Creating empty Fluid\n");
    init();
}

sphFluid::sphFluid(int n): particleSystem(n){
    printf("Creating new Fluid\n");
    init();
}

void sphFluid::init(){
    printf("Initiating sphFluid... ");
    k = spacing / 1000.0;			   // Far pressure weight
    k_near = k*10;							// Near pressure weight
    rest_density = 3;						// Rest Density
    r=1.25*spacing;						// Radius of Support
    rsq=r*r;									// ... squared for performance stuff
    particleType = sph;
    printf("done \n");
}

void sphFluid::addParticle(Vec2 x){
    particleSystem::addParticle(x);
    particles[particles.size()-1].force = Vec2(0,0);
    particles[particles.size()-1].sigma = 0.2f;
    particles[particles.size()-1].beta  = 0.2f;
}

void sphFluid::updateDensity(){

// DENSITY
//
// Calculate the density by basically making a weighted sum
// of the distances of neighboring particles within the radius of support (r)

// For each particle ...
//#pragma omp parallel for
// DENSITY 
//
// Calculate the density by basically making a weighted sum
// of the distances of neighboring particles within the radius of support (r)

// For each particle ...
    #pragma omp parallel for
    for(int i=0; i < particles.size(); ++i)
    {
        particles[i].rho = particles[i].rho_near = 0;
        
        // We will sum up the 'near' and 'far' densities.
        float d=0, dn=0;
        
        // Now look at every other particle
        for(int j = i + 1; j < particles.size(); ++j)
        {
            // The vector seperating the two particles
            Vec2 rij = particles[j].pos - particles[i].pos;
            
            // Along with the squared distance between
            float rij_len2 = rij.len2();            
            
            // If they're within the radius of support ...
            if(rij_len2 < rsq)
            {
                // Get the actual distance from the squared distance.
                float rij_len = sqrt(rij_len2);
                
                // And calculated the weighted distance values
                float q = 1 - rij_len / r;
                float q2 = q*q;
                float q3 = q2*q;
                
                d += q2;
                dn += q3;
                
                // Accumulate on the neighbor
                particles[j].rho += q2;
                particles[j].rho_near += q3;
                
                // Set up the neighbor list for faster access later.
                neighbor n;
                n.i = i; n.j = j;
                n.q = q; n.q2 = q2;              
                particles[i].neighbors.push_back(n);
            }
        }
        
        particles[i].rho        += d;
        particles[i].rho_near   += dn;
    }
}

void sphFluid::updatePressure(){
    //
    // PRESSURE
	//
	// Make the simple pressure calculation from the equation of state.
	#pragma omp parallel for
	for(int i=0; i < particles.size(); ++i)
	{
		particles[i].press = k * (particles[i].rho - rest_density);
		particles[i].press_near = k_near * particles[i].rho_near;
	}
    //
	// PRESSURE FORCE
	//
	// We will force particles in or out from their neighbors
	// based on their difference from the rest density.

	// For each particle ...
	#pragma omp parallel for
	for(int i=0; i < particles.size(); ++i)
	{
		Vec2 dX = Vec2();

		// For each of the neighbors
		int ncount = particles[i].neighbors.size();
		for(int ni=0; ni < ncount; ++ni)
		{
			neighbor n = particles[i].neighbors[ni];
			int j = n.j;     
			float q(n.q);
			float q2(n.q2);       

			// The vector from particle i to particle j
			Vec2 rij = particles[j].pos - particles[i].pos;

			// calculate the force from the pressures calculated above
			float dm = (particles[i].press + particles[j].press) * q +
				(particles[i].press_near + particles[j].press_near) * q2;

			// Get the direction of the force
			Vec2 D = rij.normal() * dm;
			dX += D;
			particles[j].force += D;
		}

		particles[i].force -= dX;
	}
}

void sphFluid::updateViscosity(){
    //
	// VISCOSITY
	//
	// This simulation actually may look okay if you don't compute 
	// the viscosity section. The effects of numerical damping and 
	// surface tension will give a smooth appearance on their own.
	// Try it.

	// For each particle
	#pragma omp parallel for
	for(int i=0; i < particles.size(); ++i)
	{
		// For each of that particles neighbors
		for(int ni=0; ni < particles[i].neighbors.size(); ++ni)
		{
			neighbor n = particles[i].neighbors[ni];

			Vec2 rij = particles[n.j].pos - particles[i].pos;
			float l = (rij).len();
			float q = l / r;

			Vec2 rijn = (rij / l);
			// Get the projection of the velocities onto the vector between them.
			float u = (particles[n.i].vel - particles[n.j].vel) * rijn;
			if(u > 0)
			{
				// Calculate the viscosity impulse between the two particles
				// based on the quadratic function of projected length.
				Vec2 I = rijn *((1 - q) * (particles[n.j].sigma * u + particles[n.j].beta * u*u));

				// Apply the impulses on the two particles
				particles[n.i].vel -= I * 0.5;
				particles[n.j].vel += I * 0.5;
			}

		}
	}
    
}


void sphFluid::advance(float timestep){
    particleSystem::advance(timestep);
    updateDensity();
    updatePressure();
    updateViscosity();
}
