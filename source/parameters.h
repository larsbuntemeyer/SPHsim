//
//  parameters.h
//  SPHsim
//
//  Created by Lars on 02.08.14.
//  Copyright (c) 2014 Lars. All rights reserved.
//

#ifndef SPHsim_parameters_h
#define SPHsim_parameters_h

int window_w=512, window_h=512;				// Initial Size of the Window
static float eangle = 0.0;
static float worldSize = 40.0;
static float bottom = 0.0;
int mainWindow;

static string filename = "test_";
static float G = 0.02f*.25;
static int plotIntervall = 2;
static int numberOfParticles = 10000;
static float timestep = 1.0;
static float lifetime = 2000;
static float spacing = 1.f;
static bool kill = false;
unsigned char particleColor[] = { 0,0,255 };  // make particles blue
#define setup column

#endif
