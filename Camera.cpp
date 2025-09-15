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
        // NOTE: Still don't know why the do opposite for Y offset cause of
        // the y axe go down
        camera->mouse.MouseYOffset = camera->mouse.LastY - camera->mouse.yPos;    

        camera->mouse.LastX = camera->mouse.xPos;
        camera->mouse.LastY = camera->mouse.yPos;        
            
        camera->Yaw += camera->mouse.MouseXOffset * DelayRatio * 0.1f;
        camera->Pitch += camera->mouse.MouseYOffset * DelayRatio * 0.1f;

        if(camera->Pitch >= 89.0f){
            camera->Pitch = 89.0f;
        }

        if(camera->Pitch <= 0.0f){
            camera->Pitch = 0.0f;
        }
        //Camera->LastFrameTime = FrameTime; ?? ??
    
        camera->Front.x = glm::sin((glm::radians(camera->Yaw)));  
        camera->Front.z = glm::cos(glm::radians(camera->Yaw)) * glm::cos(glm::radians(camera->Pitch));
        camera->Front.y = glm::sin((glm::radians(camera->Pitch)));  
        
        camera->mouse.moved = false;
    }

    camera->Direction = camera->Position + camera->Front;
    
    camera->view = glm::lookAt(
        camera->Position,  //Camera Position
        //camera->Position + camera->Front,
        camera->Direction, //Camera Direction
        camera->Up         //How camera is oriented (Normalized up vector)
                           );
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

