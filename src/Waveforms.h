//
//  Waveforms.h
//  example
//
//  Created by Joshua Batty on 20/9/17.
//
//

#pragma once 

class Waveform{
public:
    float result(float x);
};

float fract(float f) {
    float temp;
    return modf(f, &temp);
}

class Sine : public Waveform{
public:
    float result(float x){
        return sin(x);
    }
};

class Triangle : public Waveform{
public:
    float result(float x){
        return asin(sin(x))/(PI/2.);
    }
};

class Pulse : public Waveform{
public:
    float result(float x){
        return (floor(sin(x))+0.5)*2.;
    }
};

class Saw : public Waveform{
public:
    float result(float x){
        return (fract((x/2.)/PI)-0.5)*2.;
    }
};

class Noise : public Waveform{
public:
    float result(float x){
        return (fract(sin((x*2.) *(12.9898+78.233)) * 43758.5453)-0.5)*2.;
    }
};


class Waveforms{
public:
    //--------- LFO's --------------
    Sine sine;
    Triangle tri;
    Pulse pulse;
    Saw saw;
    Noise noise;
};
