#if !defined(CAMERA_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "handmade.h"
#include "glm/glm.hpp" 
#include <glm/gtx/string_cast.hpp>
#include <glm/ext/matrix_transform.hpp>
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
const float SENSITIVITY =  0.2f;
const float ZOOM        =  45.0f;

struct MouseInfo{
    int xPos;
    int yPos;

    int LastX;
    int LastY;

    int MouseXOffset;
    int MouseYOffset;

    float lastWheel;
    
    bool moved;
    bool Wheeled;
    LPTRACKMOUSEEVENT mouseEvent = NULL;

    MouseInfo(int xPoss = 0, int yPoss = 0, int ScreentWidth = 0, int ScreentHeight = 0): xPos(xPoss), yPos(yPoss){

        mouseEvent = new TRACKMOUSEEVENT();

        LastX = ScreentWidth/2;
        LastY = ScreentHeight/2;

        MouseYOffset = 0;
        MouseXOffset = 0;

        lastWheel = 0.0f;
        moved = false;
        Wheeled = false;
    }
    
};

struct Camera{
    bool moved;
    bool focusCenter;

    float LastFrameTime;    
    // Euler/Tait-Bryan angles
    float Pitch;
    float Yaw;

    //float Roll;// May be this one is unecessary

    // Camera attributes
    glm::vec3 Position;
    glm::vec3 WorldUp;

    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Direction;
    glm::vec3 Right;

    glm::mat4 projection;
    glm::mat4 view;
    
    // Options
    float fov;
    float sentivity;
    float speed;

    MouseInfo mouse;

    Camera(int ScreenWidth = 0, int ScreenHeight = 0, glm::vec3 position = glm::vec3(0.0f), glm::vec3 front = glm::vec3(0.0f), glm::vec3 right = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f)): Front(front), Position(position), Right(right), Up(up)
    {
        // We just could take Front as default
        // The Up have to be calculated from Right(Which is calculated from Direction)
        Direction = glm::vec3(0.0f) - Position;
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        
        view = glm::lookAt(
            Position, //Camera Position
            Position + Direction, // Pointed object Position
            Up        // How camera is oriented (Normalized up vector)
                           );
        fov = 45.0f;

        Pitch = PITCH;
        Yaw = YAW;

        focusCenter = false;
        moved = false;
        sentivity = 0.1f;
        LastFrameTime = 0.0f;

        speed = 2.5f;        
        mouse = MouseInfo(0, 0, ScreenWidth, ScreenHeight);
    }
};

void UpdateCamera (Camera* camera = NULL, float DelayRatio = 0.0f);
void Zoom (Camera* camera, float offset);

#define CAMERA_H
#endif
