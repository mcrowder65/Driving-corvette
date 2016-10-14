// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
// Include GLEW
#include <GL/glew.h>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>
// Include GLFW
#include <glfw3.h>
GLFWwindow* window;
#include <glm/gtc/type_ptr.hpp>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <string>
#include <iostream>
#include "utilities/shader.hpp"
#include "utilities/texture.hpp"
#include "utilities/controls.hpp"
#include "utilities/objloader.hpp"
#include "Car.h"
using namespace std;
#include "FrontTire.h"
#include "Object.h"
#include "Params.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


const float tireScale = 0.25;
const float baymaxScale = 0.04;


const float backLeftTireTheta = 0;
const float backLeftTireX = 0.41;
const float backLeftTireY = 0.15;
const float backLeftTireZ = 0.48;

float carTheta = 2.1;
float carX = -2.5;
float carY = 0;
float carZ = -7.5;
const float carScale = 1;

float tireTheta = carTheta;
float frontLeftTireTheta = 0;

const float frontLeftTireX = 0.41;
const float frontLeftTireY = 0.15;
const float frontLeftTireZ = -0.55;
const float backRightTireTheta = M_PI;
const float backRightTireX = -0.41;
const float backRightTireY = 0.15;
const float backRightTireZ = 0.48;

float frontRightTireTheta = M_PI;
const float frontRightTireX = -0.41;
const float frontRightTireY = 0.15;
const float frontRightTireZ = -0.55;
const float baymaxTheta = carTheta;
const float baymaxX = 0;
const float baymaxY = 0.5;
const float baymaxZ = 0;

const float translateX = 0.061111;
const float FRONT_LEFT_TIRE_MAX = 0.81;
const float FRONT_LEFT_TIRE_MIN = 0.79;
const float FRONT_RIGHT_TIRE_MAX = 3.98;
const float FRONT_RIGHT_TIRE_MIN = 2.38;

void rotateTires(float);
void translateCar(float, float, float);
void rotateCar(float);
int rotateCounter = 0;
const int rotateMax = 9;
void moveCar(FrontTire frontLeftTire, FrontTire frontRightTire) {
    const float rotateConst = 0.05;

    if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS) {
        //right
        
        if(rotateCounter < rotateMax) {
            rotateCounter++;
            rotateTires(rotateConst);
        }

        
        
    } else if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        //left
        if(rotateCounter > -rotateMax) {
            rotateCounter--;
            rotateTires(-rotateConst);
        }
    }
    
    
    float rotate = tireTheta - carTheta;
    float angle = carTheta;
    float sinAngle = angle != 0 ? sin(angle) : 1;
    float cosAngle = angle != 0 ? cos(angle) : -1;
    if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        float x = translateX * sinAngle;
        float z = -1 * translateX * cosAngle;
        tireTheta += rotate/10;
        rotateCar(rotate/10);
        translateCar(x, 0, z);
    } else if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        float x = -1 * translateX * sinAngle;
        float z = translateX * cosAngle;
        tireTheta -= rotate/10;
        rotateCar(-rotate/10);
        translateCar(x, 0, z);
    }

}
void rotateTires(float theta) {
    frontLeftTireTheta += theta;
    frontRightTireTheta += theta;
    tireTheta += theta;
}
void rotateCar(float theta) {
    carTheta += theta;
}

void translateCar(float x, float y, float z) {
    carX += x;
    carY += y;
    carZ += z;
}

void reset() {
     carTheta = 2.1;
     carX = -2.5;
     carY = 0;
     carZ = -7.5;
}
bool init();
int main( void )
{
    //TODO handle translation of objects separately.
    if(!init()) {
        return -1;
    }
    GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/TransformVertexShader.vertexshader", "shaders/TextureFragmentShader.fragmentshader" );

    
    
    Object carObj = Object(programID, "textures/car.png", "objects/car.obj");
    Object parkingLot = Object(programID, "textures/ParkingLot.png", "objects/ParkingLot.obj");
    FrontTire frontLeftTire = FrontTire(programID, "textures/tire.png", "objects/tire.obj");
    FrontTire frontRightTire = FrontTire(programID, "textures/tire.png", "objects/tire.obj");
    
    Object backRightTire = Object(programID, "textures/tire.png", "objects/tire.obj");
    Object backLeftTire = Object(programID, "textures/tire.png", "objects/tire.obj");
    Object baymax = Object(programID, "textures/Solid_white.png", "objects/baymax.obj");
    
    frontLeftTire.setThetaMinAndMax(FRONT_LEFT_TIRE_MIN, FRONT_LEFT_TIRE_MAX);
    frontRightTire.setThetaMinAndMax(FRONT_RIGHT_TIRE_MIN, FRONT_RIGHT_TIRE_MAX);
    
    Car car = Car(frontLeftTire, frontRightTire, backLeftTire, backRightTire, baymax, carObj);
    
    
    do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();

        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            reset();
        }
        if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            resetCamera();
        }

        
        Params baymaxParams = Params(ProjectionMatrix, ViewMatrix, baymaxX, baymaxY, baymaxZ, baymaxTheta, baymaxScale);
        
        Params frontLeftTireParams = Params(ProjectionMatrix, ViewMatrix, frontLeftTireX, frontLeftTireY, frontLeftTireZ, frontLeftTireTheta, tireScale);
        
        
        Params backLeftTireParams = Params(ProjectionMatrix, ViewMatrix, backLeftTireX, backLeftTireY, backLeftTireZ, backLeftTireTheta, tireScale);
        
        Params backRightTireParams = Params(ProjectionMatrix, ViewMatrix, backRightTireX, backRightTireY, backRightTireZ, backRightTireTheta, tireScale);
        
        Params frontRightTireParams = Params(ProjectionMatrix, ViewMatrix, frontRightTireX, frontRightTireY, frontRightTireZ, frontRightTireTheta, tireScale);
        
        Params carParams = Params(ProjectionMatrix, ViewMatrix, carX, carY, carZ, carTheta, carScale);
        
        car.draw(frontLeftTireParams, frontRightTireParams, backLeftTireParams, backRightTireParams, baymaxParams, carParams);
        
        moveCar(frontLeftTire, frontRightTire);
        Params parkingLotParams = Params(ProjectionMatrix, ViewMatrix, 0, 0, 0, 0, 1);
        parkingLot.draw(parkingLotParams);
        
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS  && glfwWindowShouldClose(window) == 0 );

	glfwTerminate();

	return 0;
}
bool init() {
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return false;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Tutorial 07 - Model Loading", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return false;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);
    
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS); 
    
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    return true;
    

}
