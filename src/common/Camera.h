//
// Created by dudu on 2023/2/1.
//

#ifndef SPARROW_CAMERA_H
#define SPARROW_CAMERA_H

// Include GLM for Math computing
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {


public:

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float yaw;
    float pitch;
    float FOV;
    float keySensitivity;

    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;


    Camera():
        cameraPos(glm::vec3(0.0f, 0.0f,  3.0f)),
        cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
        cameraUp(glm::vec3(0.0f, 1.0f,  0.0f)),
        yaw(270.0f),pitch(0.0f),FOV(45.0f),keySensitivity(1.0f)
    {

    }


    void UpdateCameraMatrix();

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjMatrix() const;
};


#endif //SPARROW_CAMERA_H
