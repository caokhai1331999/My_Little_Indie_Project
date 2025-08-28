#if !defined(WIN32GAME_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "handmade.h"
#include "SoundMaker.h"

struct win32Dimension{
    int PosX{0};
    int PosY{0};
    int Height{720};
    int Width{1280};
}Dimens;

struct OpenGLData{
    unsigned int VAOs;
    unsigned int ColorVAOs;
    unsigned int VBO;
    unsigned int ColorVBO;

    unsigned int ProgramID;
    
    HGLRC openglRC;
    unsigned int textureHandle = 0;
};

struct Win32_OffScreen_Buffer{  
    BITMAPINFO Bitmapinfo;
    HBITMAP BitmapHandle;

    void* BitmapMemory;
    void* BitmapMemoryForDirectBlit;

    int BitmapWidth;
    int BitmapHeight;
    int Pitch;
    int BitmapMemorySize;
    
    bool transferNeed;    
    bool GLImageRendered = false;
    OpenGLData glData;
    const int BytesPerPixel = 4;
};

struct Win32_Front_Buffer{  
    //BITMAPINFO Bitmapinfo;
    //HBITMAP BitmapHandle;
    void* BitmapMemory;
    void* BitmapMemoryForDirectBlit;

    int BitmapWidth;
    int BitmapHeight;
    int Pitch;

    bool GLDataPassed = false;
    OpenGLData glData;
};

global_variable bool  GlobalRunning;
global_variable HWND Window;
global_variable RECT ClientRect;
global_variable HDC DeviceContext;
// global_variable int  XOffset{0}, YOffset{0};
global_variable Win32_OffScreen_Buffer BackBuffer = {};
global_variable Game_State State = {};
global_variable imagee_content* BMPContent;
const global_variable int Height = 720;
const global_variable int Width = 1280;

void GetWindowDimension(HWND Window);
void Win32ResizeDIBSection(Win32_OffScreen_Buffer* OBuffer, int Width, int Height);

//void RenderSplendidGradient(Win32_Front_Buffer* OBuffer, imagee_content* BMPContent, int XOffset, int YOffset);
void RenderSplendidGradient(Win32_OffScreen_Buffer* OBuffer, Win32_Front_Buffer* FBuffer, imagee_content* BMPContent, int XOffset, int YOffset, int ByteCount);
void Win32DisplayBufferWindow (HDC DeviceContext, int WindowWidth, int WindowHeight, Win32_OffScreen_Buffer* OBuffer);

void GameUpdateAndRender(Game_Memory* Memory = nullptr, imagee_content* BMPContent = nullptr ,Game_Input* Input = nullptr, Game_State* State = nullptr ,Win32_Front_Buffer* OBuffer = nullptr, Game_Sound_OutPut* SoundBuffer = nullptr, HDC DeviceContextt = NULL);

void OpenConsole();
bool InitOpenGL(HWND window, Win32_OffScreen_Buffer* OBuffer, Win32_Front_Buffer* FBuffer, imagee_content* bmpContent);

void copyBufferData(Win32_OffScreen_Buffer* BackBuffer, Win32_Front_Buffer* ScreenBuffer);
void displayBufferData(Win32_OffScreen_Buffer* BackBuffer, Win32_Front_Buffer* ScreenBuffer);
void APIENTRY MessageCallback(GLenum source,
                              GLenum type,
                              GLuint id,
                              GLenum severity,
                              GLsizei length,
                              const GLchar* message,
                              const void* userParam);
#define WIN32GAME_H
#endif
