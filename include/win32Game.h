#if !defined(WIN32GAME_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "B_shader.h"
#include "entity.h"
#include "SoundMaker.h"
#include "utility"
#include <vector>

#pragma pack(push, 1)
struct Win32_Front_Buffer{  
    //BITMAPINFO Bitmapinfo;
    //HBITMAP BitmapHandle;
    void* BitmapMemory;
    void* BitmapMemoryForDirectBlit;

    //HWND Window;
    
    int BitmapWidth;
    int BitmapHeight;
    int Pitch;

    bool GLDataPassed = false;
    OpenGLData glData;
//, Window(Window_)
    Win32_Front_Buffer(int PassedWidth, int PassedHeight, OpenGLData* PassedglData, void* PassedMem, HWND Window_):BitmapWidth(PassedWidth), BitmapHeight(PassedHeight), BitmapMemory(PassedMem)
    {
        //isNull(&glData.VAOs)?glData.VAOs=0:printf("Front Buffer VAOs is not NUll\n");
        //isNull(&glData.ColorVAOs)?glData.ColorVAOs=0:printf("Front Buffer ColorVAOs is not NUll\n");
        //isNull(&glData.VBO)?glData.VBO=0:printf("Front Buffer VBO is not NUll\n");
        //isNull(&glData.ColorVBO)?glData.ColorVBO=0:printf("Front Buffer ColorVBO is not NUll\n");
        PassGLData(PassedglData, &glData);
    }
};
#pragma pack(pop)


#pragma pack(push, 1)
struct Win32_OffScreen_Buffer{  
    TileGLObject TileProperty;    
    Camera camera;
    std::vector<B_shader_program*> shaders_list;
    std::vector<Camera*> camera_set;

    const int BytesPerPixel = 4;
    
    uint64 Pad[6];
};
#pragma pack(pop)

extern "C" {global_variable Win32_OffScreen_Buffer BackBuffer = {};}
extern "C" global_variable Game_State State = {};

glm::mat4 dancing_vampire_core = glm::mat4(1.0f);
bool32 is_moving = false;
float DelayedRatio = 0.5f;

void GetWindowDimension(Platform_Properties* GamePlatform = nullptr);
void Win32ResizeDIBSection(Platform_Properties* Game_Platform, Win32_OffScreen_Buffer* OBuffer, int Width, int Height);

//void RenderSplendidGradient(Win32_Front_Buffer* OBuffer, imagee_content* BMPContent, int XOffset, int YOffset);
void RenderSplendidGradient(Platform_Properties* Game_Platform = nullptr, Win32_Front_Buffer* FBuffer = nullptr, imagee_content* BMPContent = nullptr);
void Win32DisplayBufferWindow (HDC DeviceContext, int WindowWidth, int WindowHeight, Platform_Properties* Game_Platform, Win32_OffScreen_Buffer* BackBuffer);

void GameUpdateAndRender(Game_Memory* Memory = nullptr, imagee_content* BMPContent = nullptr ,Game_Input* Input = nullptr, Game_State* State = nullptr ,Win32_Front_Buffer* OBuffer = nullptr, Game_Sound_OutPut* SoundBuffer = nullptr, HDC DeviceContextt = NULL);

void OpenConsole();
bool InitOpenGL(Platform_Properties* Game_Platform = nullptr, Win32_OffScreen_Buffer* OBuffer = nullptr, Win32_Front_Buffer* FBuffer = nullptr, imagee_content* bmpContent = nullptr);

void copyBufferData(Platform_Properties* Game_Platform, Win32_OffScreen_Buffer* BackBuffer, Win32_Front_Buffer* ScreenBuffer);
void displayBufferData(Platform_Properties* Game_Platform, Win32_OffScreen_Buffer* BackBuffer, Win32_Front_Buffer* ScreenBuffer);

//void APIENTRY MessageCallback(GLenum source,
                              //GLenum type,
                              //GLuint id,
                              //GLenum severity,
                              //GLsizei length,
                              //const GLchar* message,
                              //const void* userParam);
//

void ResetGLState(Win32_OffScreen_Buffer* BackBuffer = nullptr, Platform_Properties* Game_Platform = nullptr);
void InitCamera(Platform_Properties* Game_Platform = nullptr, Win32_OffScreen_Buffer* BackBuffer = nullptr);
void Set_Projection_View(Win32_OffScreen_Buffer* BackBuffer = nullptr);

LRESULT CALLBACK MainWindowCallBack(HWND Window, UINT Message, WPARAM Wparam ,LPARAM Lparam);

void CalDelayedRatio(float* DelayedRatio = nullptr, Clock_Set* Time_Set = nullptr, Win32_OffScreen_Buffer* BackBuffer = nullptr);
WNDCLASSEXA SetUpWindowClass(Platform_Properties* Game_Platform, HINSTANCE Instance);
void CleanUpandExit(Platform_Properties* Game_Platform, Win32_OffScreen_Buffer* BackBuffer = nullptr, Glyph_Map* map = nullptr);
void ErrorExit();

#define WIN32GAME_H
#endif
