//
//  sphFluid.cpp
//  SPHsim
//
//  Created by Lars on 09.08.14.
//  Copyright (c) 2014 Lars. All rights reserved.
//

#include "sphFluid.h"

sphFluid::sphFluid(){
   printf("Creating empty Fluid\n");
   k = spacing / 1000.0;			   // Far pressure weight
   k_near = k*10;							// Near pressure weight
   rest_density = 3;						// Rest Density
   r=spacing*1.25;						// Radius of Support
   rsq=r*r;									// ... squared for performance stuff
}

sphFluid::sphFluid(int n): particleSystem(n){
   printf("Creating new Fluid\n");
}

void sphFluid::updateDensity(){
    //
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
	for(int i=0; i < numberOfParticles; ++i)
	{
		particles[i].rho = particles[i].rho_near = 0;

		// We will sum up the 'near' and 'far' densities.
		float d=0, dn=0;

		// Now look at every other particle
		for(int j = i + 1; j < numberOfParticles; ++j)
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
    
}

void sphFluid::updateViscosity(){
    
}

void sphFluid::advanceFluid(float timeStep){
    dt = timeStep;
    updateDensity();
    updatePressure();
    updateViscosity();
}