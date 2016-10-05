//
//  Params.h
//  Tutorials
//
//  Created by Matt Crowder on 10/4/16.
//
//

#ifndef Params_h
#define Params_h
class Params {
public:
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
    float x;
    float y;
    float z;
    float theta;
    float scale;
    Params(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, float x, float y, float z, float theta, float scale) {
        this->ProjectionMatrix = ProjectionMatrix;
        this->ViewMatrix = ViewMatrix;
        this->x = x;
        this->y = y;
        this->z = z;
        this->theta = theta;
        this->scale = scale;
    }
};

#endif /* Params_h */
