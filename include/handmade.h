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
#include <glad/gl.h>
#include <glad/wgl.h>
#endif

#include <Windows.h>
#include <xinput.h>
#include <stdio.h>
#include <cmath>
#include <iostream>

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
#define global_variable static

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
// ==================================================================

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

#define DEBUG_READ_WHOLE_FILE(name) void* name(char* filename);
typedef DEBUG_READ_WHOLE_FILE(debug_read_whole_file);
#define DEBUG_WRITE_WHOLE_FILE(name) bool32 name(char* filename, uint32 memorysize, void* memory);
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

// TODO: Allow the sample offset here for more robust platform options
uint32 safetruncateUint64(uint64 value);
real32 saferatioN(real32 numerator, real32 divisor);
real32 saferatio0(real32 numerator, real32 divisor);
real32 saferatio1(real32 numerator, real32 divisor);

void* PlatformLoadFile(char* FileName);
debug_read_file_result* DEBUGReadFileWhole(char* filename);
imagee_content* DEBUGReadBMP(char* filename, debug_read_file_result* result);
//BMP_content* DEBUGReadJPG(char* filename, debug_read_file_result* result);
bool32 DEBUGWriteWholeFile(char* filename, uint32 memorysize, void* memory);
void DEBUGFreeFileMemory(void* memory);

void win32LoadXInput(void);
void ProcessXinputDigitalButton(DWORD XInputButtonState ,Game_Button_State* OldState ,DWORD ButtonBit, Game_Button_State* NewState);
void ProcessInput(int maxControllerCount, Game_Input* OldInput, Game_Input* NewInput);

#define HANDMADE_H
#endif
