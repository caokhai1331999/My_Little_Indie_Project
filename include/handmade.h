#if !defined(HANDMADE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

/*

  TODO: Services that the platform layer apply to the game

*/

/*

  NOTE: Services that the platform layer provide to the game layer

*/

#if !defined
#define GLAD_LIB
#include <glad/glad_wgl.h>
#include <glad/glad.h>
//#include <glad/wgl.h>
#endif

#include <Windows.h>
#include <Windowsx.h>
#include <wingdi.h>
#include <xinput.h>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <iostream>

#if !defined
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#endif

using namespace std;

#define ArrayCount(Array) (sizeof(Array) / sizeof(Array[0]))

#if DEBUG
#define Assert(Expression) if(!Expression){* (int* ) 0 = 0;}
#else
#define Assert(Expression)
#endif

#define Kilobytes(data) (data*1024)
#define Megabytes(data) (Kilobytes(data)*1024)
#define Gigabytes(data) (Megabytes(data)*1024)

#define internal static
#define local_persist static
#define global_variable extern

#define Pi32 3.14159265359f

typedef int16_t int16;
typedef int8_t int8;
typedef int32_t int32;
typedef int64_t int64;
typedef uint64_t uint64;

typedef bool bool16;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

typedef float real32;
typedef double real64;

const global_variable int Height = 720;
const global_variable int Width = 1280;

// NOTE: This is all about calling the function in the Xinput.h without the noticing from the compiler
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex,XINPUT_STATE *pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
// NOTE: The second line will be expand out to be like this :
// typedef x_input_get_state(DWORD dwUserIndex,XINPUT_STATE *pState)
// This is to turn on the compiler strict type checking
// And to DECLARE A FUNCTION SIGNATURE AS A TYPE
// for example: x_input_get_state _XinputgetState()
X_INPUT_GET_STATE(XinputGetStateStub) {
    return (ERROR_DEVICE_NOT_CONNECTED);
// NOTE: But the rules of C does not allow this(x_input_get_state _XinputGetStateStub() {//do something;})
}
// so we use this for function pointer
global_variable x_input_get_state* XinputGetState_  = XinputGetStateStub;
// So finally we have a pointer name XinputGetState point to the function
// XinputGetStateStub(DWORD ....) which basically X_INPUT_GET_STATE() function
#define XinputGetState XinputGetState_

// This one is to replace the XinputGetState which already been called in Xinput.h
// with the XinputGetState                                                 
   // ==================================================================
#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex,XINPUT_VIBRATION *pVibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XinputSetStateStub) {
    return (ERROR_DEVICE_NOT_CONNECTED);
}
global_variable x_input_set_state* XinputSetState_  = XinputSetStateStub;
#define XinputSetState XinputSetState_

//NOTE: Get mouse pos
//
//#define GET_X_LPARAM(name) int name(LPARAM lparam)
//typedef GET_X_LPARAM(get_x_lparam);
//GET_X_LPARAM(getxlparam) {
//return (NULL);
//}
//global_variable get_x_lparam* getxlparam_  = getxlparam;
//#define get_x_lparam getxlparam_
//
//#define GET_Y_LPARAM(name) int name(LPARAM lparam)
//typedef GET_Y_LPARAM(get_y_lparam);
//GET_Y_LPARAM(getylparam) {
//return (NULL);
//}
//global_variable get_y_lparam* getylparam_  = getylparam;
//#define get_y_lparam getylparam_
//

// ==================================================================

/*For Bone*/
#include <list>
#include <sstream>
#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL

typedef struct debug_read_file_result{
    uint32 Size;
    void* Content;
}debug_read_file_result;

//NOTE: Pragma pack is to back data of the struct to one byte aligned structure

#pragma pack(push, 1)
struct BITMAP_HEADER{
    // BMP INFO header
    //DWORD biSize;
    //LONG  biWidth;
    //LONG  biHeight;
    //WORD  biPlanes;
    //WORD  biBitCount;
    //DWORD biCompression;
    //DWORD biSizeImage;
    //LONG  biXPelsPerMeter;
    //LONG  biYPelsPerMeter;
    //DWORD biClrUsed;
    //DWORD biClrImportant;

    //BMP header
    uint16 bfType;
    uint32 bfSize;
    uint16  bfReserved1;
    uint16  bfReserved2;
    uint32  bfOffBits;

    uint32 Size;
    int32 Width;
    int32 Height;
    uint16 Planes;
    uint16 BitsPerPixel;
};
#pragma pack(pop)

#define DEBUG_READ_WHOLE_FILE(name) void* name(const char* filename);
typedef DEBUG_READ_WHOLE_FILE(debug_read_whole_file);
#define DEBUG_WRITE_WHOLE_FILE(name) bool32 name(const char* filename, uint32 memorysize, void* memory);
typedef DEBUG_WRITE_WHOLE_FILE(debug_write_whole_file);
#define DEBUG_FREE_FILE_MEMORY(name) void name(void* memory);
typedef  DEBUG_FREE_FILE_MEMORY(debug_free_file_memory);

struct Game_Sound_OutPut{  
    int16 SamplePerSecond;
    int SampleCounts;
    int16* Samples;
};

struct Game_Button_State{
    int HalfTransitionCount;
    bool32 EndedDown;
};

union Game_Controller_Input{
    // NOTE: The union make it child var lie in one place and can be found in one
    // address
    bool32 IsAnalog;
    Game_Button_State State[6]; // This is C type pointer remember that
    
    real32 StartX;
    real32 StartY;

    real32 MinX;
    real32 MinY;

    real32 MaxX;
    real32 MaxY;

    real32 EndX;
    real32 EndY;
    
    struct {
        Game_Button_State Up;
        Game_Button_State Down;
        Game_Button_State Left;
        Game_Button_State Right;
        Game_Button_State LeftShoulder;
        Game_Button_State RightShoulder;
    };
    
};


struct Game_Input{
    Game_Controller_Input Controller[4];
};

struct Game_Memory{
    bool32 IsInitialized; 
    uint64 PermanentStorageSize;
    uint64 TransientStorageSize;
    void* PermanentStorage;
    void* TransientStorage;
};

struct imagee_content{
    int32 Width;
    int32 Height;
    uint32* ImageContent;
};

struct Game_State{
    int BlueOffset = 0;
    int GreenOffset = 0;
    int Hz = 256;    
};

struct win32Dimension{
    int PosX{0};
    int PosY{0};
    int Height{720};
    int Width{1280};
}Dimens;


struct Clock_Set{
  // This one is how many count per Second
    LARGE_INTEGER PerfCountFrequencyResult = {};
    int64 PerfCountFrequency = 0;

    LARGE_INTEGER LastCounter = {};
    LARGE_INTEGER EndCounter = {};

    LARGE_INTEGER previous_collided = {};
    LARGE_INTEGER current_collided = {};
    float collided_time = 0.0f;
    float color_switch_dur = 0.0f;

    uint64 LastCycleCounts = 0;
    uint64 EndCycleCounts = 0;


    float TimeCounter = 0.0f;
    float WaitTimeCounter = 0.0f;

    uint64 TicksPerFrame = 0;
    uint64 TicksPerS = 0;

    int64 CountsPerFrame = 0;
    //So what will be less than 0 must be float type
    float MsPerFrame = 0.0f;
    float SPerFrame = 0.0f;
    float FramePerS = 0.0f;
    // Time elapsed of one cycle/frame in second

    //bool RatioCalculated = false;

    float DelayedRatio = 0.0f;
    float StandardMSperFrame = 16.67f;

    float ChangeAxisCounter = 0.0f;
    float TimeToChangeAxis = 0.0f;
    int64 ViewRotateCount = 0;
};

struct Per_Win_Properties{
    WNDCLASSEXA WindowClass;
    HWND Window;
    Per_Win_Properties(){
        WindowClass = {};
        Window = {};
    };
};

struct Glyph_Map{
    GLuint TextureID;
    stbtt_fontinfo FontInfo;
    unsigned char* bitmap;
    //unsigned char* upside_down_bitmap;
    int w,h,i,j,c, Xoffset, Yoffset;
};

Glyph_Map Glyphs_Map = {};


// TODO: Allow the sample offset here for more robust platform options
uint32 safetruncateUint64(uint64 value);
uint32 safetruncateUint64(uint64 value){
    //Assert(value <= 0xFFFFFFFF);
    uint32 result = value;
    return result;
}

real32 saferatioN(real32 numerator, real32 divisor);
real32 saferatio0(real32 numerator, real32 divisor);
real32 saferatio1(real32 numerator, real32 divisor);

void CalEarlyFrameTime(Clock_Set* Time_Set = nullptr);
void CalColliInterv(Clock_Set* Time_Set = nullptr);
void CalStaticColorDur(Clock_Set* Time_Set, const bool32 On_Flag);

void* PlatformLoadFile(char* FileName);
debug_read_file_result* DEBUGReadFileWhole(const char* filename);
imagee_content* DEBUGReadBMP(const char* filename, debug_read_file_result* result);
//BMP_content* DEBUGReadJPG(char* filename, debug_read_file_result* result);
bool32 DEBUGWriteWholeFile(char* filename, uint32 memorysize, void* memory);
void DEBUGFreeFileMemory(void* memory);

global_variable debug_read_file_result* DEBUGReadFileWhole(const char* filename){
    debug_read_file_result* result = nullptr;
    result = (debug_read_file_result*)malloc(sizeof(struct debug_read_file_result));
    HANDLE FileHandle = CreateFileA( filename, GENERIC_READ, 0,  NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(FileHandle != INVALID_HANDLE_VALUE){
        LARGE_INTEGER filesize;
// NOTE: This should read the right size here
        if(GetFileSizeEx(FileHandle,  &filesize))
        {
            result->Size = safetruncateUint64(filesize.QuadPart);
            result->Content = VirtualAlloc(0, result->Size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
            if(result->Content)
            {
                DWORD BytesRead;
                if(ReadFile(FileHandle, result->Content, result->Size, &BytesRead,0) && ( BytesRead == result->Size))
                {
                    printf("Read file successfully\n");
                }
                else
                {
                    // debug                        
                }
            }
            else
            {
                DEBUGFreeFileMemory(result->Content);
                result = nullptr;
                // debug
            }
        }
        CloseHandle(FileHandle);
    } else {
        // logging
    }
    return result;

}
bool32 DEBUGWriteWholeFile(char* filename, uint32 memorysize, void* memory){
    bool32 result = false;
    HANDLE FileHandle = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(FileHandle != INVALID_HANDLE_VALUE){
            if(result){
                DWORD bytewritten;
                    if(WriteFile(FileHandle, memory, memorysize, &bytewritten, 0))
                    {
                        result = (memorysize == bytewritten);
                    } else {
                        // debug                        
                    }
                } else {
                    // debug
                }
        CloseHandle(FileHandle);
    }else{

        // logging
    }
    return result;
}

void DEBUGFreeFileMemory(void* memory){
    VirtualFree(memory, 0, MEM_RELEASE);
}

imagee_content* DEBUGReadBMP(const char* filename, debug_read_file_result* ReadResult){
    imagee_content* result = (imagee_content*)malloc(sizeof(struct imagee_content));
    ReadResult = DEBUGReadFileWhole(filename);
     if(ReadResult->Size != 0){

         BITMAP_HEADER *HeadResult = (BITMAP_HEADER*)ReadResult->Content;
         //Why plus ???

         uint32* pixels = (uint32* )((uint8 *)ReadResult->Content + HeadResult->bfOffBits);

         result->Width = HeadResult->Width;
         result->Height = HeadResult->Height;
         result->ImageContent = pixels;

         
         
         //uint32* SourceDest = pixels;
//NOTE: For OpenGL this original arranging order work but not for passing directly to window graphic
         
         
/*
         //Why Height is inside the width loop
         for(uint32 Y = 0; Y < HeadResult->Width; ++Y){
             for(uint32 X = 0; X < HeadResult->Height; ++X){
                 //How to reverse byte order from AA RR GG BB (image byte order)
                 //                            => RR GG BB AA (little endiendness order)
                 *SourceDest = ((*SourceDest) >> 8) | ((*SourceDest) << 24);
                 ++SourceDest;
             }
         };
*/
         }
     return result;
}

void win32LoadXInput(void);
void ProcessXinputDigitalButton(DWORD XInputButtonState ,Game_Button_State* OldState ,DWORD ButtonBit, Game_Button_State* NewState);
void ProcessInput(int maxControllerCount, Game_Input* OldInput, Game_Input* NewInput);

void* GetAllFunctionPointerFromLib(HMODULE lib = NULL, const char* name = nullptr);
void *GetAnyGLFuncAddress(const char *name);
// NOTE: Why this function can replace the wglGetProcaddress
void* GetAnyGLFuncAddress(const char* name)
{
    void* p = (void*)wglGetProcAddress(name);
    if (!p)
    {
        static HMODULE module =
           LoadLibraryA("opengl32.dll");
        p = (void*)GetProcAddress(module, name);
        //p = (void*)wglGetProcAddress(name);
    }
    return p;
    //May be the wglgetprocaddress doesn't on pc case work I have to use getprocaddress instead
}
;
void* LoadFunctionFromDLL(const char* DLLName = nullptr, const char* FuncName = nullptr);
void SetUpWindow();
glm::vec3 randomRotateAxis_(int rollIndex);
bool32 string_contain(std::string* string = nullptr, char* substr = nullptr);

bool32 string_contain(std::string* string, char* substr){
    int substrIndex = 0;
    while (substrIndex < sizeof(*substr)){
        if (string->find(substr[substrIndex])==string->npos){
            return false;
        } else {
            substrIndex++;
        }
    };
    return true;
};

std::string* load_bin_map(const char* name = nullptr);

#define HANDMADE_H
#endif
