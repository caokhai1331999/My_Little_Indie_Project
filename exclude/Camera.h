#if !defined(CAMERA_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "glm/glm.hpp" 

/*

// Don't know shit about this
glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

// camera/view transformation
glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);

*/


// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


struct Camera{
    // Euler/Tait-Bryan angles
    float Pitch;
    float Yaw;
    float Roll;// May be this one is unecessary

    // Camera attributes
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 Position;
    glm::vec3 WorldUp;

    // Options
    float zoom;
    float sentivity;
    float speed;
};

void UpdateCamera(Camera* camera);
void Zoom(Camera* camera, float offset);

#define CAMERA_H
#endif
