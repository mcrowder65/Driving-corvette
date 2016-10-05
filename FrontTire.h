//
//  Tire.h
//  Tutorials
//
//  Created by Matt Crowder on 10/4/16.
//
//
#include "Object.h"
#ifndef Front_Tire_h
#define Front_Tire_h

class FrontTire : public Object
{
public:
    void setThetaMinAndMax(float min, float max) {
        this->THETA_MIN = min;
        this->THETA_MAX = max;
    }
    float getMin() {
        return this->THETA_MIN;
    }
    float getMax() {
        return this->THETA_MAX;
    }
    FrontTire(){}
    FrontTire(GLuint programID, string textureFileName, string objectFileName) : Object( programID,  textureFileName, objectFileName) {};
private:
    float THETA_MIN;
    float THETA_MAX;
};
#endif /* Tire_h */
