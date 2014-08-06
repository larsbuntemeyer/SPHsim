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

using namespace std;

//////////////////////////////////////////
// A structure for holding two neighboring particles and their weighted distances
struct neighbor { int i, j; float q, q2; };

// The particle structure holding all of the relevant information.
struct particle { 
	Vec2 pos, pos_old, vel, force; 
	float mass, rho, rho_near, press, press_near, sigma, beta; 
	vector<neighbor> neighbors;  
};

#endif /* defined(__SPHsim__particleSystem__) */
