//
//  Object.h
//  Tutorials
//
//  Created by Matt Crowder on 10/4/16.
//
//
#include <string>
#include <iostream>
using namespace std;
#include "stb_image.h"
#include "Params.h"
#ifndef Object_h
#define Object_h

class Object
{
private:
    GLuint loadPNG(const char* path) {
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
    void setScale(float s) {
        float arr[16] = {
            s, 0, 0, 0,
            0, s, 0, 0,
            0, 0, s, 0,
            0, 0, 0, 1
        };
        this->scale = glm::make_mat4(arr);
    }
    void setTranslation(float x, float y, float z) {
        float arr[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            x, y, z, 1
        };
        this->translation = glm::make_mat4(arr);
    }
    void setRotation(float theta) {
        float c = cos(theta);
        float s = sin(theta);
        float b = -sin(theta);
        float rotationArray[16] = {
            c, 0, s, 0,
            0, 1, 0, 0,
            b, 0, c, 0,
            0, 0, 0, 1
        };
        this->rotation = glm::make_mat4(rotationArray);
        

    }
    GLuint matrixID;
    GLuint texture;
    GLuint textureID;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    GLuint vertexBuffer;
    GLuint uvBuffer;
    glm::mat4 translation;
    glm::mat4 rotation;
    glm::mat4 scale;
    glm::mat4 modelMatrix;
    glm::mat4 mvp;
public:
    Object(GLuint programID, string textureFileName, string objectFileName) {
        this->matrixID = glGetUniformLocation(programID, "MVP");
        this->texture = loadPNG(textureFileName.c_str());
        this->textureID  = glGetUniformLocation(programID, "myTextureSampler");
        bool res = loadOBJ(objectFileName.c_str(), vertices, uvs, normals);
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        
        glGenBuffers(1, &uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
        
    }
    Object() {}
    ~Object() {}
    void draw(Params params) {
        this->setScale(params.scale);
        this->setTranslation(params.x, params.y, params.z);
        this->setRotation(params.theta);
        this->modelMatrix = glm::mat4(1.0);
        this->modelMatrix = this->translation * this->rotation * this->scale;
        this->mvp = params.ProjectionMatrix * params.ViewMatrix * this->modelMatrix;
    
        glUniformMatrix4fv(this->matrixID, 1, GL_FALSE, &this->mvp[0][0]);
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glUniform1i(this->textureID, 0);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, this->uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() );
    }
    
};

#endif /* Object_h */
