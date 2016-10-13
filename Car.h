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
    void draw(Params fLParams, Params fRParams, Params bLParams, Params bRParams, Params baymaxParams, Params carParams) {
        frontLeftTire.draw(fLParams);
        frontRightTire.draw(fRParams);
        backLeftTire.draw(bLParams);
        backRightTire.draw(bRParams);
        baymax.draw(baymaxParams);
        car.draw(carParams);
    }
};
#endif /* Car_h */
