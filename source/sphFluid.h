//
//  sphFluid.h
//  SPHsim
//
//  Created by Lars on 09.08.14.
//  Copyright (c) 2014 Lars. All rights reserved.
//

#ifndef __SPHsim__sphFluid__
#define __SPHsim__sphFluid__

#include <iostream>
#include "particleSystem.h"


class sphFluid: public particleSystem{
public:
    sphFluid();
    sphFluid(int n);
    void advance(float timestep);
    void addParticle(Vec2 x);
private:
    float k;			   // Far pressure weight
    float k_near;							// Near pressure weight
    float rest_density;						// Rest Density
    float r;						// Radius of Support
    float rsq;									// ... squared for performance stuff
    void updateDensity();
    void updatePressure();
    void updateViscosity();
    void updateForces();
    void init();
};



#endif /* defined(__SPHsim__sphFluid__) */
