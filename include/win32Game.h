#if !defined(WIN32GAME_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "B_shader.h"
//#include "handmade.h"
#include "entity.h"
#include "SoundMaker.h"

#include "utility"
#include <vector>

struct Rect_{
    int x, y, w, h;
};

struct OpenGLData{
    unsigned int VAOs;
    unsigned int PlaneVAOs;

    unsigned int VBO;
    unsigned int ColorVBO;
    unsigned int PlaneVBO;  

    std::vector<GLuint> ProgramIDs = {};
    //std::vector<*B_shader_program> ProgramIDs = {};
    
    HGLRC openglRC;
    HGLRC defaultContext;

    unsigned int textureHandle;
    std::vector<unsigned int>* texture_id_list;

    OpenGLData(){
        //Maybe buggy this part
        // need to be careful
        VAOs     = 0;       
        PlaneVAOs = 0;
        VBO      = 0;     
        ColorVBO = 0;
        PlaneVBO = 0;
        textureHandle = 0;
    }
};

struct Win32_OffScreen_Buffer{  
    BITMAPINFO Bitmapinfo;
    HBITMAP BitmapHandle;

    void* BitmapMemory;
    void* BitmapMemoryForDirectBlit;

    HWND Window;
    RECT ClientRect;

    MSG Message;
    WNDPROC wndproc;
    
    int BitmapWidth;
    int BitmapHeight;
    int Pitch;
    int BitmapMemorySize;

    std::vector<B_shader_program*> shaders_list;

    bool SwithCamera = false;
    bool transferNeed;    
    bool GLImageRendered = false;
    OpenGLData glData;

    Tile TileProper;
    
    Camera camera;
    std::vector<Camera*> camera_set;
    const int BytesPerPixel = 4;
};


bool isNull(GLuint* member = nullptr);
void PassGLData(OpenGLData* BackData, OpenGLData* FrontData);

global_variable bool GlobalRunning = true;
global_variable HDC DeviceContext;
// global_variable int  XOffset{0}, YOffset{0};
global_variable Win32_OffScreen_Buffer BackBuffer = {};
global_variable Game_State State = {};
global_variable imagee_content* BMPContent = nullptr;
global_variable real32 WaitTimeCounter = 0.0f;
global_variable glm::vec3 moving_vector;

struct Win32_Front_Buffer{  
    //BITMAPINFO Bitmapinfo;
    //HBITMAP BitmapHandle;
    void* BitmapMemory;
    void* BitmapMemoryForDirectBlit;

    //HWND Window;
    
    int BitmapWidth;
    int BitmapHeight;
    int Pitch;

    std::vector<B_shader_program* > shaders_list;

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


bool32 first_size = true;
bool32 first_announce = true;
bool32 Load_Lib = false;
bool32 showMsPF = false;

glm::mat4 dancing_vampire_core = glm::mat4(1.0f);
bool32 is_moving = false;
float DelayedRatio = 0.5f;


void GetWindowDimension(Win32_OffScreen_Buffer* BackBuffer);
void Win32ResizeDIBSection(Win32_OffScreen_Buffer* OBuffer, int Width, int Height);

//void RenderSplendidGradient(Win32_Front_Buffer* OBuffer, imagee_content* BMPContent, int XOffset, int YOffset);
void RenderSplendidGradient(Win32_OffScreen_Buffer* OBuffer = nullptr, Win32_Front_Buffer* FBuffer = nullptr, imagee_content* BMPContent = nullptr);
void Win32DisplayBufferWindow (HDC DeviceContext, int WindowWidth, int WindowHeight, Win32_OffScreen_Buffer* OBuffer);

void GameUpdateAndRender(Game_Memory* Memory = nullptr, imagee_content* BMPContent = nullptr ,Game_Input* Input = nullptr, Game_State* State = nullptr ,Win32_Front_Buffer* OBuffer = nullptr, Game_Sound_OutPut* SoundBuffer = nullptr, HDC DeviceContextt = NULL);

void OpenConsole();
bool InitOpenGL(Win32_OffScreen_Buffer* OBuffer, Win32_Front_Buffer* FBuffer, imagee_content* bmpContent);

void copyBufferData(Win32_OffScreen_Buffer* BackBuffer, Win32_Front_Buffer* ScreenBuffer);
void displayBufferData(Win32_OffScreen_Buffer* BackBuffer, Win32_Front_Buffer* ScreenBuffer);

void APIENTRY MessageCallback(GLenum source,
                              GLenum type,
                              GLuint id,
                              GLenum severity,
                              GLsizei length,
                              const GLchar* message,
                              const void* userParam);

void ReloadGLFunction(const Win32_OffScreen_Buffer* BackBuffer_ = nullptr){
    //HDC tempDC = GetDC(BackBuffer.Window);
    //if(wglMakeCurrent(tempDC, BackBuffer->glData.openglRC)){
        bool success = gladLoadGLLoader((GLADloadproc)wglGetProcAddress);
        if (!success)
            bool success = gladLoadGLLoader((GLADloadproc)GetAnyGLFuncAddress);
        assert(success);
    //};
}

void ResetGLState(Win32_OffScreen_Buffer* BackBuffer = nullptr);
void SetEnvironmentLights();
void InitCamera(Win32_OffScreen_Buffer* BackBuffer = nullptr);
void Set_Projection_View(Win32_OffScreen_Buffer* BackBuffer = nullptr);

LRESULT CALLBACK MainWindowCallBack(HWND Window, UINT Message, WPARAM Wparam ,LPARAM Lparam);

void CalDelayedRatio(float* DelayedRatio = nullptr, Clock_Set* Time_Set = nullptr, Win32_OffScreen_Buffer* BackBuffer = nullptr);
WNDCLASSEXA SetUpWindowClass(Win32_OffScreen_Buffer* BackBuffer, HINSTANCE Instance);
void CleanUpandExit(Win32_OffScreen_Buffer* BackBuffer = nullptr, Glyph_Map* map = nullptr);
void ErrorExit();

#define WIN32GAME_H
#endif
