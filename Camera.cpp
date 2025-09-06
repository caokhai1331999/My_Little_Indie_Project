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
    camera->Front.x = glm::cos((glm::radians(camera->Roll)));  
    camera->Front.y = glm::sin((glm::radians(camera->Pitch)));  
    camera->Front.z = glm::sin(glm::radians(camera->Yaw)) * glm::cos(glm::radians(camera->Pitch));  

    //camera->Up = glm::cos();  
    //camera->Right = glm::cos();  

    camera->view = glm::lookAt(
            camera->Position, //Camera Position
            camera->Pointing, // Position Where Camere pointing to
            camera->Up        // How camera is oriented (Normalized up vector)
                           );
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

