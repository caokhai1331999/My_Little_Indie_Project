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
    
    //if(camera->mouse.moved){
        camera->mouse.MouseXOffset = camera->mouse.xPos - camera->mouse.LastX;
        // NOTE: Still don't know why we have to do oppositely for Y offset cause of
        // the y axe go down
        camera->mouse.MouseYOffset = camera->mouse.yPos - camera->mouse.LastY;

        camera->mouse.LastX = camera->mouse.xPos;
        camera->mouse.LastY = camera->mouse.yPos;        
            
        camera->Yaw += camera->mouse.MouseXOffset * DelayRatio * SENSITIVITY;
        camera->Pitch += camera->mouse.MouseYOffset * DelayRatio * SENSITIVITY;

        if(camera->Pitch >= 89.0f){
            camera->Pitch = 89.0f;
        }

        if(camera->Pitch <= 0.0f){
            camera->Pitch = 0.0f;
        }
        //Camera->LastFrameTime = FrameTime; ?? ??
    
        camera->Direction.x = glm::sin((glm::radians(camera->Yaw)));  
        camera->Direction.z = glm::cos(glm::radians(camera->Yaw)) * glm::cos(glm::radians(camera->Pitch));
        camera->Direction.y = glm::sin((glm::radians(camera->Pitch)));  
        
        //camera->mouse.moved = false;
    //}
        camera->Front = glm::normalize(camera->Direction);

        camera->view = glm::lookAt(
            camera->Position,  //Camera Position
            //camera->Position + camera->Front,
            camera->Position + camera->Front, //Camera Direction
            camera->Up         //How camera is oriented (Normalized up vector)
                                   );
// Why I have to plus Position with Front
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

