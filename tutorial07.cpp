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
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
using namespace std;
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

    
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //start of car***************************
    // Get a handle for our "MVP" uniform
    GLuint carMatrixID = glGetUniformLocation(programID, "MVP");
    
    // Load the texture
    GLuint carTexture = loadPNG("car.png");
    // Get a handle for our "myTextureSampler" uniform
    GLuint carTextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    // Read our .obj file
    std::vector<glm::vec3> carVertices;
    std::vector<glm::vec2> carUVS;
    std::vector<glm::vec3> carNormals; // Won't be used at the moment.
    bool carRes = loadOBJ("car.obj", carVertices, carUVS, carNormals);
    
    
    // Load it into a VBO
    
    GLuint carVertexBuffer;
    glGenBuffers(1, &carVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, carVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, carVertices.size() * sizeof(glm::vec3), &carVertices[0], GL_STATIC_DRAW);
    
    GLuint carUVBuffer;
    glGenBuffers(1, &carUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, carUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, carUVS.size() * sizeof(glm::vec2), &carUVS[0], GL_STATIC_DRAW);
	//end of car***************************
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    
    
    //start of parking lot **********************
    GLuint parkingLotMatrixID = glGetUniformLocation(programID, "MVP");
    
    // Load the texture
    GLuint parkingLotTexture = loadPNG("ParkingLot.png");
    // Get a handle for our "myTextureSampler" uniform
    GLuint parkingLotTextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    // Read our .obj file
    std::vector<glm::vec3> parkingLotVertices;
    std::vector<glm::vec2> parkingLotUVS;
    std::vector<glm::vec3> parkingLotNormals; // Won't be used at the moment.
    bool resParkingLot = loadOBJ("ParkingLot.obj", parkingLotVertices, parkingLotUVS, parkingLotNormals);
    
    
    // Load it into a VBO
    
    GLuint parkingLotVertexBuffer;
    glGenBuffers(1, &parkingLotVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, parkingLotVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, parkingLotVertices.size() * sizeof(glm::vec3), &parkingLotVertices[0], GL_STATIC_DRAW);
    
    GLuint parkingLotUVBuffer;
    glGenBuffers(1, &parkingLotUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, parkingLotUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, parkingLotUVS.size() * sizeof(glm::vec2), &parkingLotUVS[0], GL_STATIC_DRAW);
    //end of parking lot
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //start of FRONT LEFT TIRE
    // Get a handle for our "MVP" uniform
    GLuint frontLeftTireMatrixID = glGetUniformLocation(programID, "MVP");
    
    	// Load the texture
    GLuint frontLeftTireTexture = loadPNG("tire.png");
    	// Get a handle for our "myTextureSampler" uniform
    GLuint frontLeftTireTextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    	// Read our .obj file
    	std::vector<glm::vec3> frontLeftTireVertices;
    	std::vector<glm::vec2> frontLeftTireUVS;
    	std::vector<glm::vec3> frontLeftTireNormals; // Won't be used at the moment.
    	bool frontLeftTireRes = loadOBJ("tire.obj", frontLeftTireVertices, frontLeftTireUVS, frontLeftTireNormals);
    
    
    	// Load it into a VBO
    
    	GLuint frontLeftTireVertexBuffer;
    	glGenBuffers(1, &frontLeftTireVertexBuffer);
    	glBindBuffer(GL_ARRAY_BUFFER, frontLeftTireVertexBuffer);
    	glBufferData(GL_ARRAY_BUFFER, frontLeftTireVertices.size() * sizeof(glm::vec3), &frontLeftTireVertices[0], GL_STATIC_DRAW);
    
    	GLuint frontLeftTireUVBuffer;
    	glGenBuffers(1, &frontLeftTireUVBuffer);
    	glBindBuffer(GL_ARRAY_BUFFER, frontLeftTireUVBuffer);
    	glBufferData(GL_ARRAY_BUFFER, frontLeftTireUVS.size() * sizeof(glm::vec2), &frontLeftTireUVS[0], GL_STATIC_DRAW);
    
    //end of FRONT LEFT TIRE
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //START OF FRONT RIGHT TIRE
    	// Get a handle for our "MVP" uniform
    	GLuint frontRightTireMatrixID = glGetUniformLocation(programID, "MVP");
    
    	// Load the texture
        GLuint frontRightTireTexture = loadPNG("tire.png");
    	// Get a handle for our "myTextureSampler" uniform
    	GLuint frontRightTireTextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    	// Read our .obj file
    	std::vector<glm::vec3> frontRightTireVertices;
    	std::vector<glm::vec2> frontRightTireUVS;
    	std::vector<glm::vec3> frontRightTireNormals; // Won't be used at the moment.
    	bool frontRightTireRes = loadOBJ("tire.obj", frontRightTireVertices, frontRightTireUVS, frontRightTireNormals);
    
    
    	// Load it into a VBO
    
    	GLuint frontRightTireVertexbuffer;
    	glGenBuffers(1, &frontRightTireVertexbuffer);
    	glBindBuffer(GL_ARRAY_BUFFER, frontRightTireVertexbuffer);
    	glBufferData(GL_ARRAY_BUFFER, frontRightTireVertices.size() * sizeof(glm::vec3), &frontRightTireVertices[0], GL_STATIC_DRAW);
    
        GLuint frontRightTireUVBuffer;
        glGenBuffers(1, &frontRightTireUVBuffer);
    	glBindBuffer(GL_ARRAY_BUFFER, frontRightTireUVBuffer);
    	glBufferData(GL_ARRAY_BUFFER, frontRightTireUVS.size() * sizeof(glm::vec2), &frontRightTireUVS[0], GL_STATIC_DRAW);
    //END OF FRONT RIGHT TIRE
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //BACK RIGHT TIRE
      // Get a handle for our "MVP" uniform
      GLuint backRightTireMatrixID = glGetUniformLocation(programID, "MVP");
    
      // Load the texture
        GLuint backRightTireTexture = loadPNG("tire.png");
      // Get a handle for our "myTextureSampler" uniform
      GLuint backRightTireTextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
      // Read our .obj file
      std::vector<glm::vec3> backRightTireVertices;
      std::vector<glm::vec2> backRightTireUVS;
      std::vector<glm::vec3> backRightTireNormals; // Won't be used at the moment.
      bool backRightTireRes = loadOBJ("tire.obj", backRightTireVertices, backRightTireUVS, backRightTireNormals);
    
    
      // Load it into a VBO
    
      GLuint backRightTireVertexbuffer;
      glGenBuffers(1, &backRightTireVertexbuffer);
      glBindBuffer(GL_ARRAY_BUFFER, backRightTireVertexbuffer);
      glBufferData(GL_ARRAY_BUFFER, backRightTireVertices.size() * sizeof(glm::vec3), &backRightTireVertices[0], GL_STATIC_DRAW);
    
      GLuint backRightTireUVBuffer;
      glGenBuffers(1, &backRightTireUVBuffer);
      glBindBuffer(GL_ARRAY_BUFFER, backRightTireUVBuffer);
      glBufferData(GL_ARRAY_BUFFER, backRightTireUVS.size() * sizeof(glm::vec2), &backRightTireUVS[0], GL_STATIC_DRAW);
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //BACK LEFT TIRE

    
      GLuint backLeftTireMatrixID = glGetUniformLocation(programID, "MVP");
    
      // Load the texture
        GLuint backLeftTireTexture = loadPNG("tire.png");
      // Get a handle for our "myTextureSampler" uniform
      GLuint backLeftTireTextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
      // Read our .obj file
    
    
      std::vector<glm::vec3> backLeftTireVertices;
      std::vector<glm::vec2> backLeftTireUVs;
      std::vector<glm::vec3> backLeftTireNormals; // Won't be used at the moment.
      bool backLeftTireRes = loadOBJ("tire.obj", backLeftTireVertices, backLeftTireUVs, backLeftTireNormals);
    GLuint backLeftTireVertexbuffer;
    glGenBuffers(1, &backLeftTireVertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, backLeftTireVertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, backLeftTireVertices.size() * sizeof(glm::vec3), &backLeftTireVertices[0], GL_STATIC_DRAW);
    
    GLuint backLeftTireUVBuffer;
    glGenBuffers(1, &backLeftTireUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, backLeftTireUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, backLeftTireUVs.size() * sizeof(glm::vec2), &backLeftTireUVs[0], GL_STATIC_DRAW);
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //start of baymax
    GLuint bayMaxMatrixID = glGetUniformLocation(programID, "MVP");
    
    // Load the texture
    GLuint bayMaxTexture = loadPNG("Solid_white.png");
    // Get a handle for our "myTextureSampler" uniform
    GLuint bayMaxTextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    // Read our .obj file
    
    
    std::vector<glm::vec3> bayMaxVertices;
    std::vector<glm::vec2> bayMaxUVS;
    std::vector<glm::vec3> bayMaxNormals; // Won't be used at the moment.
    bool bayMaxRes = loadOBJ("baymax.obj", bayMaxVertices, bayMaxUVS, bayMaxNormals);
    
    GLuint bayMaxVertexBuffer;
    glGenBuffers(1, &bayMaxVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bayMaxVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, bayMaxVertices.size() * sizeof(glm::vec3), &bayMaxVertices[0], GL_STATIC_DRAW);
    
    GLuint bayMaxUVBuffer;
    glGenBuffers(1, &bayMaxUVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bayMaxUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, bayMaxUVS.size() * sizeof(glm::vec2), &bayMaxUVS[0], GL_STATIC_DRAW);
    
    //END OF BAYMAX
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    //******************************************************************************
    float carScaleArray[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    float carTheta = 2.1;
    float carRotationArray[16] = {
        cos(carTheta),       0,        sin(carTheta), 0,
        0, 1, 0, 0,
        -sin(carTheta), 0  ,  cos(carTheta), 0,
        0,       0,        0, 1
    };
    float carTranslationArray[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -2.5, 0, -7.5, 1
    };
    glm::mat4 carScale = glm::make_mat4(carScaleArray);
    glm::mat4 carRotation = glm::make_mat4(carRotationArray);
    glm::mat4 carTranslation = glm::make_mat4(carTranslationArray);
      // Load it into a VBO
    
    
      float tireScaleArray[16] = {
        .25, 0, 0, 0,
        0, .25, 0, 0,
        0, 0, .25, 0,
        0, 0, 0,   1
      };
    glm::mat4 tireScale = glm::make_mat4(tireScaleArray);
    float leftTiresTheta = 2.11;
        float backLeftTireRotationArray[16] = {
        cos(leftTiresTheta),       0,        sin(leftTiresTheta), 0,
        0, 1, 0, 0,
        -sin(leftTiresTheta), 0  ,  cos(leftTiresTheta), 0,
        0,       0,        0, 1
    };
    
    

    glm::mat4 backLeftTireRotation = glm::make_mat4(backLeftTireRotationArray);
    
    
    float backLeftTireTranslationArray[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -3.1, 0.15, -7.4, 1
    };
    
    glm::mat4 backLeftTireTranslation = glm::make_mat4(backLeftTireTranslationArray);
    

    
    
    float frontLeftTireTranslationArray[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -2.22, 0.15, -6.9, 1
    };

    glm::mat4 frontLeftTireTranslation = glm::make_mat4(frontLeftTireTranslationArray);
    
    
    float backRightTireTranslationArray[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -2.7, 0.15, -8.1, 1
    };
    
    glm::mat4 backRightTireTranslation = glm::make_mat4(backRightTireTranslationArray);
    
    float frontRightTireTranslationArray[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -1.85, 0.15, -7.55, 1
    };
    
    glm::mat4 frontRightTireTranslation = glm::make_mat4(frontRightTireTranslationArray);
    
    float backrightTiresTheta = 5.28;
    
    float backRightTireRotationArray[16] = {
        cos(backrightTiresTheta),       0,        sin(backrightTiresTheta), 0,
        0, 1, 0, 0,
        -sin(backrightTiresTheta), 0  ,  cos(backrightTiresTheta), 0,
        0,       0,        0, 1
    };
    
    glm::mat4 backRightTireRotation = glm::make_mat4(backRightTireRotationArray);
    
    float bayMaxTheta = 0.5;
    float bayMaxRotationArray[16] = {
        cos(bayMaxTheta),       0,        sin(bayMaxTheta), 0,
        0, 1, 0, 0,
        -sin(bayMaxTheta), 0  ,  cos(bayMaxTheta), 0,
        0,       0,        0, 1

    };
    float bayMaxScaleArray[16] = {
        0.04, 0, 0, 0,
        0, 0.04, 0, 0,
        0, 0, 0.04, 0,
        0, 0, 0, 1
    };
    glm::mat4 bayMaxScale = glm::make_mat4(bayMaxScaleArray);
    glm::mat4 bayMaxRotation = glm::make_mat4(bayMaxRotationArray);
    
    float bayMaxTranslationArray[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -2.5, 0.5, -7.5, 1
    };
    glm::mat4 bayMaxTranslation = glm::make_mat4(bayMaxTranslationArray);
    float frontLeftTireTheta = 2.11;
    float frontRightTireTheta = 5.28;

    do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        
       
        
        float frontLeftTireRotationArray[16] = {
            cos(frontLeftTireTheta),       0,        sin(frontLeftTireTheta), 0,
            0, 1, 0, 0,
            -sin(frontLeftTireTheta), 0  ,  cos(frontLeftTireTheta), 0,
            0,       0,        0, 1
        };
        glm::mat4 frontLeftTireRotation = glm::make_mat4(frontLeftTireRotationArray);
        
        
                float frontRightTireRotationArray[16] = {
            cos(frontRightTireTheta),       0,        sin(frontRightTireTheta), 0,
            0, 1, 0, 0,
            -sin(frontRightTireTheta), 0  ,  cos(frontRightTireTheta), 0,
            0,       0,        0, 1
        };
        glm::mat4 frontRightTireRotation = glm::make_mat4(frontRightTireRotationArray);
        // Strafe left
        
        if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS) {
            if(frontLeftTireTheta <= 2.91 && frontRightTireTheta <= 6.08) {
                frontLeftTireTheta += .2;
                frontRightTireTheta +=.2;
            }
            
        } else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            if(frontLeftTireTheta >= 1.31 && frontRightTireTheta >= 4.48) {
                frontLeftTireTheta -= .2;
                frontRightTireTheta -=.2;
            }
            
        }
        
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        
        //START OF BAYMAX
        glm::mat4 bayMaxModelMatrix = glm::mat4(1.0);
         bayMaxModelMatrix = bayMaxTranslation * bayMaxRotation * bayMaxScale;
        glm::mat4 bayMaxMVP = ProjectionMatrix * ViewMatrix * bayMaxModelMatrix;
        
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(bayMaxMatrixID, 1, GL_FALSE, &bayMaxMVP[0][0]);
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bayMaxTexture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(bayMaxTextureID, 0);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, bayMaxVertexBuffer);
        glVertexAttribPointer(
                              0,                  // attribute
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, bayMaxUVBuffer);
        glVertexAttribPointer(
                              1,                                // attribute
                              2,                                // size
                              GL_FLOAT,                         // type
                              GL_FALSE,                         // normalized?
                              0,                                // stride
                              (void*)0                          // array buffer offset
                              );
        
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, bayMaxVertices.size() );
        //        END OF BAYMAX
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        
        //START OF FRONT LEFT TIRE
        glm::mat4 frontLeftTireModelMatrix = glm::mat4(1.0);
        frontLeftTireModelMatrix = frontLeftTireTranslation * frontLeftTireRotation * tireScale;
        glm::mat4 frontLeftTireMVP = ProjectionMatrix * ViewMatrix * frontLeftTireModelMatrix;
        
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(frontLeftTireMatrixID, 1, GL_FALSE, &frontLeftTireMVP[0][0]);
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, frontLeftTireTexture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(frontLeftTireTextureID, 0);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, frontLeftTireVertexBuffer);
        glVertexAttribPointer(
                              0,                  // attribute
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, frontLeftTireUVBuffer);
        glVertexAttribPointer(
                              1,                                // attribute
                              2,                                // size
                              GL_FLOAT,                         // type
                              GL_FALSE,                         // normalized?
                              0,                                // stride
                              (void*)0                          // array buffer offset
                              );
        
        // Draw the triangle !
                glDrawArrays(GL_TRIANGLES, 0, frontLeftTireVertices.size() );
        //        END OF FRONT LEFT TIRE
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        //START OF BACK LEFT TIRE
        
        glm::mat4 backLeftTireModelMatrix = glm::mat4(1.0);
        
        
        
        backLeftTireModelMatrix = backLeftTireTranslation * backLeftTireRotation * tireScale;
        glm::mat4 backLeftTireMVP = ProjectionMatrix *  ViewMatrix * backLeftTireModelMatrix ;
        // in the "MVP" uniform
        glUniformMatrix4fv(backLeftTireMatrixID, 1, GL_FALSE, &backLeftTireMVP[0][0]);
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, backLeftTireTexture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(backLeftTireTextureID, 0);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, backLeftTireVertexbuffer);
        glVertexAttribPointer(
                              0,                  // attribute
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, backLeftTireUVBuffer);
        glVertexAttribPointer(
                              1,                                // attribute
                              2,                                // size
                              GL_FLOAT,                         // type
                              GL_FALSE,                         // normalized?
                              0,                                // stride
                              (void*)0                          // array buffer offset
                              );
        
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, backLeftTireVertices.size() );
        
        
        
        
        
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************

        
        
        
        
        //CAR**********************************************
        glm::mat4 carModelMatrix = glm::mat4(1.0);
        
        carModelMatrix = carTranslation * carRotation * carScale;
        glm::mat4 carMVP = ProjectionMatrix * ViewMatrix * carModelMatrix;
        
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(carMatrixID, 1, GL_FALSE, &carMVP[0][0]);
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, carTexture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(carTextureID, 0);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, carVertexBuffer);
        glVertexAttribPointer(
                              0,                  // attribute
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, carUVBuffer);
        glVertexAttribPointer(
                              1,                                // attribute
                              2,                                // size
                              GL_FLOAT,                         // type
                              GL_FALSE,                         // normalized?
                              0,                                // stride
                              (void*)0                          // array buffer offset
                              );
        
        // Draw the triangle !
         glDrawArrays(GL_TRIANGLES, 0, carVertices.size() );
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        
        
        
        
        
        
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        //START OF BACK RIGHT TIRE
        glm::mat4 backRightTireModelMatrix = glm::mat4(1.0);
        backRightTireModelMatrix = backRightTireTranslation * backRightTireRotation * tireScale;
              glm::mat4 backRightTireMVP = ProjectionMatrix * ViewMatrix * backRightTireModelMatrix;
        
              // Send our transformation to the currently bound shader,
              // in the "MVP" uniform
              glUniformMatrix4fv(backRightTireMatrixID, 1, GL_FALSE, &backRightTireMVP[0][0]);
        
              // Bind our texture in Texture Unit 0
              glActiveTexture(GL_TEXTURE0);
              glBindTexture(GL_TEXTURE_2D, backRightTireTexture);

              glUniform1i(backRightTireTextureID, 0);
        
              // 1rst attribute buffer : frontRightTireVertices
              glEnableVertexAttribArray(0);
              glBindBuffer(GL_ARRAY_BUFFER, backRightTireVertexbuffer);
              glVertexAttribPointer(
                  0,                  // attribute
                  3,                  // size
                  GL_FLOAT,           // type
                  GL_FALSE,           // normalized?
                  0,                  // stride
                  (void*)0            // array buffer offset
              );
        
              // 2nd attribute buffer : UVs
              glEnableVertexAttribArray(1);
              glBindBuffer(GL_ARRAY_BUFFER, backRightTireUVBuffer);
              glVertexAttribPointer(
                  1,                                // attribute
                  2,                                // size
                  GL_FLOAT,                         // type
                  GL_FALSE,                         // normalized?
                  0,                                // stride
                  (void*)0                          // array buffer offset
              );
        
              // Draw the triangle !
              glDrawArrays(GL_TRIANGLES, 0, backRightTireVertices.size() );
//               //END OF BACK RIGHT TIRE
//        //******************************************************************************
//        //******************************************************************************
//        //******************************************************************************
//        //******************************************************************************
//
//        //******************************************************************************
//        //******************************************************************************
//        //******************************************************************************
//        //******************************************************************************
        //START OF FRONT RIGHT TIRE
        		glm::mat4 frontRightTireModelMatrix = glm::mat4(1.0);
        frontRightTireModelMatrix = frontRightTireTranslation * frontRightTireRotation * tireScale;
        		glm::mat4 frontRightTireMVP = ProjectionMatrix * ViewMatrix * frontRightTireModelMatrix;
        
        		// Send our transformation to the currently bound shader,
        		// in the "MVP" uniform
        		glUniformMatrix4fv(frontRightTireMatrixID, 1, GL_FALSE, &frontRightTireMVP[0][0]);
        
        		// Bind our texture in Texture Unit 0
        		glActiveTexture(GL_TEXTURE0);
        		glBindTexture(GL_TEXTURE_2D, frontRightTireTexture);
        		// Set our "myTextureSampler" sampler to user Texture Unit 0
        		glUniform1i(frontRightTireTextureID, 0);
        
        		// 1rst attribute buffer : vertices
        		glEnableVertexAttribArray(0);
        		glBindBuffer(GL_ARRAY_BUFFER, frontRightTireVertexbuffer);
        		glVertexAttribPointer(
        			0,                  // attribute
        			3,                  // size
        			GL_FLOAT,           // type
        			GL_FALSE,           // normalized?
        			0,                  // stride
        			(void*)0            // array buffer offset
        		);
        
        		// 2nd attribute buffer : UVs
        		glEnableVertexAttribArray(1);
        		glBindBuffer(GL_ARRAY_BUFFER, frontRightTireUVBuffer);
        		glVertexAttribPointer(
        			1,                                // attribute
        			2,                                // size
        			GL_FLOAT,                         // type
        			GL_FALSE,                         // normalized?
        			0,                                // stride
        			(void*)0                          // array buffer offset
        		);
        
        		// Draw the triangle !
        		glDrawArrays(GL_TRIANGLES, 0, frontRightTireVertices.size() );
//        //END OF FRONT RIGHT TIRE
        
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        //START OF PARKING LOT*********************************
        glm::mat4 parkingLotModelMatrix = glm::mat4(1.0);
        glm::mat4 parkingLotMVP = ProjectionMatrix * ViewMatrix * parkingLotModelMatrix;
        
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(parkingLotMatrixID, 1, GL_FALSE, &parkingLotMVP[0][0]);
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, parkingLotTexture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(parkingLotTextureID, 0);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, parkingLotVertexBuffer);
        glVertexAttribPointer(
                              0,                  // attribute
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, parkingLotUVBuffer);
        glVertexAttribPointer(
                              1,                                // attribute
                              2,                                // size
                              GL_FLOAT,                         // type
                              GL_FALSE,                         // normalized?
                              0,                                // stride
                              (void*)0                          // array buffer offset
                              );
        
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, parkingLotVertices.size() );
        //END OF PARKING LOT*********************************
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************
        //******************************************************************************

     
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS  && glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &carVertexBuffer);
	glDeleteBuffers(1, &carUVBuffer);


	glDeleteProgram(programID);
	glDeleteTextures(1, &carTextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

