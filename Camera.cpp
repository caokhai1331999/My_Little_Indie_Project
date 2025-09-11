/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Camera.h"

void UpdateCamera (Camera* camera) {
    //Update the camera front based on yaw/bearing (around the y axe), pitch/elevation (around the x axe), roll (around the z axe)

    // First take the perspective from above we have the x is sin(yaw) and z is
    // cos(yaw)
    // Then do the similar look on pitch(y and z) we have y equal to sin(pitch)
    // and z is cos(pitch)
    Yaw = XOffset*sensitivity*FrameTime;
    Pitch = YOffset*sensitivity*FrameTime;

    camera->Direction.x = glm::sin((glm::radians(camera->Yaw))) *;  
    camera->Direction.z = glm::cos(glm::radians(camera->Yaw)) * glm::cos(glm::radians(camera->Pitch));
    camera->Direction.y = glm::sin((glm::radians(camera->Pitch)));  
    //camera->Up = glm::cos();  
    //camera->Right = glm::cos();  
    
    camera->view = glm::lookAt(
        camera->Position,  //Camera Position
        camera->Position + camera->Front,  //Camera Direction
        camera->Up         //How camera is oriented (Normalized up vector)
                           );

    //camera->view = glm::translate(camera->view, camera->Position);
};

void Zoom(Camera* camera, float offset){
    camera->zoom -= offset;

    if(camera->zoom < 1.0f){
        camera->zoom = 1.0f;
    };

    if(camera->zoom > 45.0f){
        camera->zoom = 45.0f;
    };
}

