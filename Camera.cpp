/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Camera.h"

void UpdateCamera (Camera* camera, float DelayRatio) {
    //Update the camera front based on yaw/bearing (around the y axe), pitch/elevation (around the x axe), roll (around the z axe)

    // First take the perspective from above we have the x is sin(yaw) and z is
    // cos(yaw)
    // Then do the similar look on pitch(y and z) we have y equal to sin(pitch)
    // and z is cos(pitch)
    
    if(camera->mouse.moved){
        camera->mouse.MouseXOffset = camera->mouse.xPos - camera->mouse.LastX;
        // NOTE: Still don't know why we have to do oppositely for Y offset cause of
        // the y axe go down
        camera->mouse.MouseYOffset = camera->mouse.yPos - camera->mouse.LastY;

        camera->mouse.LastX = camera->mouse.xPos;
        camera->mouse.LastY = camera->mouse.yPos;        
            
        camera->Yaw += camera->mouse.MouseXOffset * camera->speed * SENSITIVITY;
        camera->Pitch += camera->mouse.MouseYOffset * camera->speed * SENSITIVITY;

        if(camera->Pitch > 90.0f){
            camera->Pitch = 90.0f;
        }

        if(camera->Pitch < 0.0f){
            camera->Pitch = 0.0f;
        }

        if(camera->Yaw > 360.0f){
            camera->Yaw = 360.0f;
        }

        if(camera->Yaw < 0.0f){
            camera->Yaw = 0.0f;
        }

        // Direction based on mouse
    camera->Direction.x = glm::sin((glm::radians(camera->Yaw)));  
    camera->Direction.z = glm::cos(glm::radians(camera->Yaw)) + glm::cos(glm::radians(camera->Pitch));
    camera->Direction.y = glm::sin((glm::radians(camera->Pitch)));  

//Camera->LastFrameTime = FrameTime; ?? ??
        camera->mouse.moved = false;
    }
        
    //if(camera->moved){
        // Cross product between the constant Up Vector which pointing upward and
        // cammera Direction in world space we have camera Right vector
        camera->Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera->Direction));
        camera->Up = glm::normalize(glm::cross(camera->Right, camera->Direction));
        std::string UpContent = glm::to_string(camera->Up);
        //printf("%s \n", UpContent.c_str());
        glm::vec3 LookedPoint =  camera->Position + camera->Direction;

        camera->view = glm::lookAt(
//Camera Position
            camera->Position,
//camera->Position + camera->Front,
            //glm::vec3(0.0f),
            // One above make camera rotate around center
            !camera->focusCenter?LookedPoint:glm::vec3(0.0f),
//Position Where the camera is looking at (formular is AB-> = B - A(we can apply basic number subtraction rule))
            camera->Up
//How camera is oriented (Normalized up vector of camera NOT WORLD)
                                   );
        //camera->moved = false;
    //}
};

void Zoom(Camera* camera, float offset){
    camera->fov -= offset;

    if(camera->fov < 1.0f){
        camera->fov = 1.0f;
    };

    if(camera->fov > 45.0f){
        camera->fov = 45.0f;
    };
}

