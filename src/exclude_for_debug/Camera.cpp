/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Camera.h"

void UpdateCamera (Camera* camera, float* DelayRatio) {
    //Update the camera front based on yaw/bearing (around the y axe), pitch/elevation (around the x axe), roll (around the z axe)

    // First take the perspective from above we have the x is sin(yaw) and z is
    // cos(yaw)
    // Then do the similar look on pitch(y and z) we have y equal to sin(pitch)
    // and z is cos(pitch)
    
    if(camera->mouse.moved && camera->focusCenter){
        
        //We'll use quaternion here;
        // by using slerp 
        // NOTE: Still don't know why we have to do oppositely for Y offset cause of
        // the y axe go down

         glm::vec2 xy = glm::normalize(glm::vec2(camera->mouse.xPos, camera->mouse.yPos));
         glm::vec2 xy_ = glm::normalize(glm::vec2(camera->mouse.LastX, camera->mouse.LastY));
         //glm::vec3 xyz = glm::normalize(glm::vec3(camera->mouse.xPos, camera->mouse.yPos));
         //glm::vec3 xyz_ = glm::normalize(glm::vec3(camera->mouse.LastX, camera->mouse.LastY));
         float radAngle = (glm::acos(glm::dot(xy_, xy))) * camera->speed * SENSITIVITY;
         glm::fquat offsetOrientation = {};

         offsetOrientation.w = glm::cos(radAngle/2.0f);
         offsetOrientation.x = camera->Direction.x * glm::sin(radAngle/2.0f);
         offsetOrientation.y = camera->Direction.y * glm::sin(radAngle/2.0f);
         offsetOrientation.z = camera->Direction.z * glm::sin(radAngle/2.0f);

         camera->orientation = camera->orientation * offsetOrientation;

         camera->Direction.x = camera->orientation.x;
         camera->Direction.y = camera->orientation.y;
         camera->Direction.z = camera->orientation.z;
         
        //camera->mouse.MouseXOffset = camera->mouse.LastX - camera->mouse.xPos;
        //camera->mouse.MouseYOffset = camera->mouse.LastY - camera->mouse.yPos;
//
        camera->mouse.LastX = camera->mouse.xPos;
        camera->mouse.LastY = camera->mouse.yPos;        
            
        //camera->Yaw += camera->mouse.MouseXOffset * camera->speed * SENSITIVITY;
        //camera->Pitch += camera->mouse.MouseYOffset * camera->speed * SENSITIVITY;

        //if(camera->Pitch > 90.0f){
            //camera->Pitch = 90.0f;
        //}
//
        //if(camera->Pitch < 0.0f){
            //camera->Pitch = 0.0f;
        //}
//
        //if(camera->Yaw > 360.0f){
            //camera->Yaw = 360.0f;
        //}
//
        //if(camera->Yaw < 0.0f){
            //camera->Yaw = 0.0f;
        //}

        // Direction based on mouse
        // yaw is formed by x and x1
       // similarly Pitch formed by y and y1

        
    //camera->Direction.x = glm::cos((glm::radians(camera->Yaw)));
    //camera->Direction.z = glm::sin(glm::radians(camera->Yaw)) + glm::sin(glm::radians(camera->Pitch));
    //camera->Direction.y = glm::cos((glm::radians(camera->Pitch)));

    //std::cout<<"Direction is: "<<glm::to_string(camera->Direction)<<std::endl;
//Camera->LastFrameTime = FrameTime; ?? ??

        
        //if(camera->moved){
        // Cross product between the constant Up Vector which pointing upward and
        // cammera Direction in world space we have camera Right vector
        
        camera->focusCenter = false;
        camera->mouse.moved = false;
    }

        camera->Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera->Direction));
        camera->Up = glm::normalize(glm::cross(camera->Right, camera->Direction));
        std::string UpContent = glm::to_string(camera->Up);
        //printf("%s \n", UpContent.c_str());
        glm::vec3 LookedPoint =  camera->Position + camera->Direction;


        camera->view = glm::lookAt(
//Camera Position
            camera->Position,
            //One above make camera rotate around center
            //!camera->focusCenter?LookedPoint:glm::vec3(-4.0f, 4.0f, 0.0f),
            //glm::vec3(-4.0f, 4.0f, 0.0f),
            LookedPoint,
//Position Where the camera is looking at (formular is AB-> = B - A(we can apply basic number subtraction rule))
            camera->Up
//How camera is oriented (Normalized up vector of camera NOT WORLD)
                                   );


    
    //}
         //camera->view = glm::mat4_cast(&Camera->orientation);
//===================================================
        ////ViewRotateCount++;
        //float CamX = sin(ViewRotateCount)*10.0f;
        //float CamZ = cos(ViewRotateCount)*10.0f;
        //BackBuffer.camera.view = glm::lookAt(glm::vec3(CamX, 0.0f, CamZ), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //setMat4(ScreenBuffer.glData.ProgramID, "view", BackBuffer.camera.view);
        

};

void updateCamera_ (Camera* camera, float* DelayRatio){
    UpdateCamera(camera, DelayRatio);
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
