// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

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
#include "shader.hpp"
#include "texture.hpp"
#include "controls.hpp"
#include "objloader.hpp"
//#include "Car.h"
using namespace std;
#include "FrontTire.h"
#include "Object.h"
#include "Params.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static GLuint loadPNG(const char* path)
{
    int w;
    int h;
    int comp;
    unsigned char* image = stbi_load(path, &w, &h, &comp, STBI_rgb);
    
    if (image == nullptr)
        throw(std::string("Failed to load texture"));
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(image);
    
    return textureID;
};
const float baymaxScale = 0.04;
const float FRONT_LEFT_TIRE_MAX = 2.91;
const float FRONT_LEFT_TIRE_MIN = 1.31;
const float FRONT_RIGHT_TIRE_MAX = 6.08;
const float FRONT_RIGHT_TIRE_MIN = 4.48;
int main( void )
{
    //a car is an object... which contains 4 objects(tires), and an object (baymax)
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

	// Dark blue background
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
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );

    
    
    Object carObj = Object(programID, "car.png", "car.obj");
    Object parkingLot = Object(programID, "ParkingLot.png", "ParkingLot.obj");
    FrontTire frontLeftTire = FrontTire(programID, "tire.png", "tire.obj");
    FrontTire frontRightTire = FrontTire(programID, "tire.png", "tire.obj");
    
    Object backRightTire = Object(programID, "tire.png", "tire.obj");
    Object backLeftTire = Object(programID, "tire.png", "tire.obj");
    Object baymax = Object(programID, "Solid_white.png", "baymax.obj");

    
    frontLeftTire.setThetaMinAndMax(FRONT_LEFT_TIRE_MIN, FRONT_LEFT_TIRE_MAX);
    frontRightTire.setThetaMinAndMax(FRONT_RIGHT_TIRE_MIN, FRONT_RIGHT_TIRE_MAX);
    
    const float tireScale = 0.25;
    
    
    
    float backLeftTireTheta = 2.11;
    float backLeftTireX = -3.1;
    float backLeftTireY = 0.15;
    float backLeftTireZ = -7.4;
    
    float frontLeftTireTheta = 2.11;
    float frontLeftTireX = -2.22;
    float frontLeftTireY = 0.15;
    float frontLeftTireZ = -6.9;
    
    float backRightTireTheta = 5.28;
    float backRightTireX = -2.7;
    float backRightTireY = 0.15;
    float backRightTireZ = -8.1;
    
    float frontRightTireTheta = 5.28;
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
    float baymaxScale = 0.04;
    
 
    //left joystick = wasd
    //right joystick = arrow keys
    //move tires right = j
    //move tires left = k

    do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
        
       
       
        if (glfwGetKey( window, GLFW_KEY_J ) == GLFW_PRESS) {
            if(frontLeftTireTheta <= frontLeftTire.getMax() && frontRightTireTheta <= frontRightTire.getMax()) {
                frontLeftTireTheta += .2;
                frontRightTireTheta +=.2;
            }
            
        } else if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            if(frontLeftTireTheta >= frontLeftTire.getMin() && frontRightTireTheta >= frontRightTire.getMin()) {
                frontLeftTireTheta -= .2;
                frontRightTireTheta -=.2;
            }
            
        }

        Params baymaxParams = Params(ProjectionMatrix, ViewMatrix, baymaxX, baymaxY, baymaxZ, baymaxTheta, baymaxScale);
        baymax.draw(baymaxParams);
        
        Params frontLeftTireParams = Params(ProjectionMatrix, ViewMatrix, frontLeftTireX, frontLeftTireY, frontLeftTireZ, frontLeftTireTheta, tireScale);
        frontLeftTire.draw(frontLeftTireParams);
        
        
        Params backLeftTireParams = Params(ProjectionMatrix, ViewMatrix, backLeftTireX, backLeftTireY, backLeftTireZ, backLeftTireTheta, tireScale);
        backLeftTire.draw(backLeftTireParams);
        
        Params backRightTireParams = Params(ProjectionMatrix, ViewMatrix, backRightTireX, backRightTireY, backRightTireZ, backRightTireTheta, tireScale);
        
        backRightTire.draw(backRightTireParams);
        
        Params frontRightTireParams = Params(ProjectionMatrix, ViewMatrix, frontRightTireX, frontRightTireY, frontRightTireZ, frontRightTireTheta, tireScale);
        frontRightTire.draw(frontRightTireParams);
        Params parkingLotParams = Params(ProjectionMatrix, ViewMatrix, 0, 0, 0, 0, 1);
        parkingLot.draw(parkingLotParams);

        Params carParams = Params(ProjectionMatrix, ViewMatrix, carX, carY, carZ, carTheta, carScale);
        carObj.draw(carParams);
        
        
        
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

