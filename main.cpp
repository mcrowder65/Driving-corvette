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

const float FRONT_LEFT_TIRE_MAX = 2.91;
const float FRONT_LEFT_TIRE_MIN = 1.31;
const float FRONT_RIGHT_TIRE_MAX = 6.08;
const float FRONT_RIGHT_TIRE_MIN = 4.48;
const float tireScale = 0.25;
const float baymaxScale = 0.04;


float backLeftTireTheta = 2.11;
float backLeftTireX = -3.1;
float backLeftTireY = 0.15;
float backLeftTireZ = -7.4;

float frontLeftTireTheta = 2.11;
const float initialFrontLeftTireTheta = frontLeftTireTheta;
float frontLeftTireX = -2.22;
float frontLeftTireY = 0.15;
float frontLeftTireZ = -6.9;

float backRightTireTheta = 5.28;
float backRightTireX = -2.7;
float backRightTireY = 0.15;
float backRightTireZ = -8.1;

float frontRightTireTheta = 5.28;
const float initialFrontRightTireTheta = frontRightTireTheta;
float frontRightTireX = -1.85;
float frontRightTireY = 0.15;
float frontRightTireZ = -7.55;



float carTheta = 2.1;
float carX = -2.5;
float carY = 0;
float carZ = -7.5;
float carScale = 1;

float baymaxTheta = 0.5;
float baymaxX = -2.5;
float baymaxY = 0.5;
float baymaxZ = -7.5;

float referenceTheta = 0;
float referenceX = -1.8;
float referenceY = 0.25;
float referenceZ = -7.1;
float referenceScale = 0.01;

const float translateX = 0.061111;
const float translateZ = 0.03587;

void rotateObjects(float theta) {
    backLeftTireTheta += theta;
    
    backRightTireTheta += theta;
    carTheta += theta;
    baymaxTheta += theta;

}
static double lastTime = glfwGetTime();
static float horizontalAngle = carTheta;
static glm::vec3 position = glm::vec3(carX, carY, carZ);
static float verticalAngle = 0.0f;

static float speed = 3.0f; // 3 units / second
static float mouseSpeed = 0.005f;

void translateObjects(float x, float y, float z) {
    backLeftTireX += x;
    backLeftTireY += y;
    backLeftTireZ += z;
    
    frontLeftTireX += x;
    frontLeftTireY += y;
    frontLeftTireZ += z;
    
    backRightTireX += x;
    backRightTireY += y;
    backRightTireZ += z;
    
    frontRightTireX += x;
    frontRightTireY += y;
    frontRightTireZ += z;
    
    carX += x;
    carY += y;
    carZ += z;
    
    baymaxX += x;
    baymaxY += y;
    baymaxZ += z;
    
    referenceX += x;
    referenceY += y;
    referenceZ += z;
}
void moveCar(FrontTire frontLeftTire, FrontTire frontRightTire) {
    bool somethingPressed = false;

    float preX = 0;
    float preZ = 0;
    if (glfwGetKey( window, GLFW_KEY_J ) == GLFW_PRESS) {
        if(frontLeftTireTheta <= frontLeftTire.getMax() && frontRightTireTheta <= frontRightTire.getMax()) {
            frontLeftTireTheta += .2;
            frontRightTireTheta +=.2;
        }
        
    } else if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if(frontLeftTireTheta >= frontLeftTire.getMin() && frontRightTireTheta >= frontRightTire.getMin()) {
            frontLeftTireTheta -= .2;
            frontRightTireTheta -=.2;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        float adjacentArray[3] = {
            carX + translateX, carY, carZ + translateZ
        };
        vec3 adjacent = glm::make_vec3(adjacentArray);
        float angle = (frontLeftTireTheta * 180) / M_PI;//- initialFrontLeftTireTheta;
        cout << "frontLeftTireTheta: " << angle << endl;
//        ( radians * 180 ) / pi ;
        float cosTheta = cos(angle);
        cout << "cosTheta: " << cosTheta << endl;
        vec3 hypotenuse = adjacent / cosTheta;
        cout << "hypotenuse[0]: " << hypotenuse[0] << " hypotenuse[2]: " << hypotenuse[2] << endl;
        cout << "adjacent[0]: " << adjacent[0] << " adjacent[2]: " << adjacent[2] << endl;
        carX = hypotenuse[0];
        carZ = hypotenuse[2];
//        translateObjects(hypotenuse[0], 0, hypotenuse[2]);
        somethingPressed = true;
    } else if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        preX = carX - translateX;
        preZ = carZ - translateZ;
        float adjacentArray[3] = {
            carX, carY, carZ
        };
        vec3 adjacent = glm::make_vec3(adjacentArray);
        translateObjects(-translateX, 0, -translateZ);
        
        somethingPressed = true;
    }
    
   
//    float postX = carX;
//    float postZ = carZ;
//    if(somethingPressed) {
//        if(preX != postX) {
//            cout << "x is broken " << " preX: " << preX << " postX: " << postX << endl;
//        }
//        if(preZ != postZ) {
//            cout << "z is broken " << " preZ: " << preZ << " postZ: " << postZ << endl;
//        }
//    }
    
    
}

glm::vec3 getReferenceOrientation() {
    float arr[3] = {
        carX - referenceX,
        0,
        carZ - referenceZ
    };
    return glm::make_vec3(arr);
}
//looks at original car to front left tire orientation and makes sure this is consistent throughout translation
glm::vec3 getFrontLeftTireOrientation() {
    glm::vec3 orientation;
    float x = carX - frontLeftTireX;
    float z = carZ - frontLeftTireZ;
    float arr[3] = {
        x,
        0,
        z
    };
    orientation = glm::make_vec3(arr);

    
    
    return orientation;
}
glm::vec3 frontRightTireOrientation() {
    glm::vec3 orientation;
    return orientation;
}


glm::vec3 backRightTireOrientation() {
    glm::vec3 orientation;
    return orientation;
}
glm::vec3 backLeftTireOrientation() {
    glm::vec3 orientation;
    return orientation;
}
const glm::vec3 frontLeftTireOrientation = getFrontLeftTireOrientation();


void reset() {
    
     backLeftTireTheta = 2.11;
     backLeftTireX = -3.1;
     backLeftTireY = 0.15;
     backLeftTireZ = -7.4;
    
     frontLeftTireTheta = 2.11;
     frontLeftTireX = -2.22;
     frontLeftTireY = 0.15;
     frontLeftTireZ = -6.9;
    
     backRightTireTheta = 5.28;
     backRightTireX = -2.7;
     backRightTireY = 0.15;
     backRightTireZ = -8.1;
    
     frontRightTireTheta = 5.28;
     frontRightTireX = -1.85;
     frontRightTireY = 0.15;
     frontRightTireZ = -7.55;
    
     carTheta = 2.1;
     carX = -2.5;
     carY = 0;
     carZ = -7.5;
     carScale = 1;
    
     baymaxTheta = 0.5;
     baymaxX = -2.5;
     baymaxY = 0.5;
     baymaxZ = -7.5;
    
     referenceTheta = 0;
     referenceX = -2.5;
     referenceY = 0.5;
     referenceZ = -7.5;
     referenceScale = 0.01;
}
int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
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
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
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
    Object reference = Object(programID, "textures/Solid_white.png", "objects/reference.obj");
    
    frontLeftTire.setThetaMinAndMax(FRONT_LEFT_TIRE_MIN, FRONT_LEFT_TIRE_MAX);
    frontRightTire.setThetaMinAndMax(FRONT_RIGHT_TIRE_MIN, FRONT_RIGHT_TIRE_MAX);
    
    Car car = Car(frontLeftTire, frontRightTire, backLeftTire, backRightTire, baymax, carObj, reference);
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
        
        
        moveCar(frontLeftTire, frontRightTire);
        
        Params baymaxParams = Params(ProjectionMatrix, ViewMatrix, baymaxX, baymaxY, baymaxZ, baymaxTheta, baymaxScale);
        
        Params frontLeftTireParams = Params(ProjectionMatrix, ViewMatrix, frontLeftTireX, frontLeftTireY, frontLeftTireZ, frontLeftTireTheta, tireScale);
        
        
        Params backLeftTireParams = Params(ProjectionMatrix, ViewMatrix, backLeftTireX, backLeftTireY, backLeftTireZ, backLeftTireTheta, tireScale);
        
        Params backRightTireParams = Params(ProjectionMatrix, ViewMatrix, backRightTireX, backRightTireY, backRightTireZ, backRightTireTheta, tireScale);
        
        Params frontRightTireParams = Params(ProjectionMatrix, ViewMatrix, frontRightTireX, frontRightTireY, frontRightTireZ, frontRightTireTheta, tireScale);
        
        Params carParams = Params(ProjectionMatrix, ViewMatrix, carX, carY, carZ, carTheta, carScale);
        
        Params referenceParams = Params(ProjectionMatrix, ViewMatrix, referenceX, referenceY, referenceZ, referenceTheta, referenceScale);

        car.draw(frontLeftTireParams, frontRightTireParams, backLeftTireParams, backRightTireParams, baymaxParams, carParams, referenceParams);
        
        
        
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

