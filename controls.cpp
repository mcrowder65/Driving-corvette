// Include GLFW
#include <glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
#include <iostream>
using namespace std;
#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
    return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position =glm::vec3(3.85178, 0.471703, -3.63897);// glm::vec3( 0, 0, 5 );]
glm::vec3 initialLookAt = glm::vec3(4.69527, 0.431714, -3.10331);
// Initial horizontal angle : toward -Z
float horizontalAngle = 1.04f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

bool initialLookAtBool = true;

void computeMatricesFromInputs(){
    
    cout << "horizontalAngle: " << horizontalAngle << " verticalAngle: " << verticalAngle << endl;
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();
    
    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    
    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    // Reset mouse position for next frame
    glfwSetCursorPos(window, 1024/2, 768/2);
    
    // Compute new orientation
    horizontalAngle += mouseSpeed * float(1024/2 - xpos );
    verticalAngle   += mouseSpeed * float( 768/2 - ypos );
    
    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
                        cos(verticalAngle) * sin(horizontalAngle),
                        sin(verticalAngle),
                        cos(verticalAngle) * cos(horizontalAngle)
                        );
    
    // Right vector
    glm::vec3 right = glm::vec3(
                                sin(horizontalAngle - 3.14f/2.0f),
                                0,
                                cos(horizontalAngle - 3.14f/2.0f)
                                );
    
    // Up vector
    glm::vec3 up = glm::cross( right, direction );
    
    // Move forward
    //glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    // Move backward
    //glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS
    if ( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    //glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS
    if ( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
        position += right * deltaTime * speed;
    }
    // Strafe left
    //glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS
    if ( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
        position -= right * deltaTime * speed;
    }
    
    float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.
    
    // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    //ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
    float xScale = 3;
    float yScale = 4;
    float near = 0.1f;
    float far = 100.f;
    float nearMinusFar = near - far;
    float nearPlusFar = near + far;
    float num = nearPlusFar / nearMinusFar;
    float num2 = 2 * far * near / nearMinusFar;
    float perspectiveMatrixArray[16] = {
        xScale,      0,    0,  0,
        0, yScale,    0,  0,
        0,      0,  num, -1,
        0,      0, num2,  0
    };
    
    ProjectionMatrix = glm::make_mat4(perspectiveMatrixArray);
    // Camera matrix
    //	ViewMatrix       = glm::lookAt(
    //								position,           // Camera is here
    //								target, // and looks here : at the same position, plus "direction"
    //								up                  // Head is up (set to 0,-1,0 to look upside-down)
    //						   );
    glm::vec3 zaxis = glm::normalize(initialLookAtBool == true ? initialLookAt - position : position + direction - position);
    initialLookAtBool = false;
    glm::vec3 xaxis = glm::normalize(glm::cross(up,zaxis));
    glm::vec3 yaxis = glm::cross(zaxis, xaxis);
    float viewMatrixArray[16] = {
        xaxis[0], yaxis[0], zaxis[0], 0,
        xaxis[1], yaxis[1], zaxis[1], 0,
        xaxis[2], yaxis[2], zaxis[2], 0,
        -1 * glm::dot(xaxis, position), -1 * glm::dot(yaxis, position), -1 * glm::dot(zaxis, position), 1
    };
    ViewMatrix = glm::make_mat4(viewMatrixArray);
    
    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}