//
//  math.h
//  SPHsim
//
//  Created by Lars on 02.08.14.
//  Copyright (c) 2014 Lars. All rights reserved.
//

#ifndef SPHsim_includes_h
#define SPHsim_includes_h

#include <vector>
#include <cmath>

struct Vec2 {
    float x,y;
    Vec2() :x(0),y(0) { }
    Vec2(float a, float b) : x(a), y(b) { }
    Vec2 operator+(const Vec2& b) const { return Vec2(x+b.x, y+b.y); }
    Vec2 operator-(const Vec2& b) const { return Vec2(x-b.x, y-b.y); }
    Vec2 & operator=(const Vec2& b) { x=b.x; y=b.y; return *this; }
    Vec2 & operator+=(const Vec2& b) { return *this = *this + b; }
    Vec2 & operator-=(const Vec2& b) { return *this = *this - b; }
    
    float operator*(const Vec2& b) const { return x*b.x + y*b.y; }
    Vec2 operator*(float b) const { return Vec2(x * b, y * b); }
    Vec2 operator/(float b) const { return Vec2(x / b, y / b); }
    float len2() const { return *this * *this; }
    float len() const { return sqrt(len2()); }
    Vec2 normal() const { return *this / len(); }
    };
    //Vec2 operator*(float b, const Vec2& a) { return Vec2(a.x * b, a.y * b); }
    

#endif

