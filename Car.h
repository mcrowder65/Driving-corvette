//
//  Car.h
//  Tutorials
//
//  Created by Matt Crowder on 10/4/16.
//
//
#include "Object.h"
#include "FrontTire.h"
#ifndef Car_h
#define Car_h

class Car {
private:
    FrontTire frontLeftTire;
    FrontTire frontRightTire;
    Object backLeftTire;
    Object backRightTire;
    Object baymax;
    Object car;
    Object reference;
public:
    Car(FrontTire frontLeftTire, FrontTire frontRightTire, Object backLeftTire, Object backRightTire, Object baymax, Object car) {
        this->frontLeftTire = frontLeftTire;
        this->frontRightTire = frontRightTire;
        this->backLeftTire = backLeftTire;
        this->backRightTire = backRightTire;
        this->baymax = baymax;
        this->car = car;
    }
    Car(){}
    void draw(Params fLParams, Params fRParams, Params bLParams, Params bRParams, Params baymaxParams, Params carParams) {
        car.draw(carParams);
        frontLeftTire.draw(fLParams, car.modelMatrix);
        frontRightTire.draw(fRParams, car.modelMatrix);
        backLeftTire.draw(bLParams, car.modelMatrix);
        backRightTire.draw(bRParams, car.modelMatrix);
        baymax.draw(baymaxParams, car.modelMatrix);
    }
};
#endif /* Car_h */
