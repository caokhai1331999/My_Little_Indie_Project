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

#include <Windows.h>
#include <xinput.h>
#include <GL/gl.h>

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

//struct Game_OffScreen_Buffer{  
     //BITMAPINFO Bitmapinfo;
     //HBITMAP BitmapHandle;
    //void* BitmapMemory;
    //int BitmapWidth;
    //int BitmapHeight;
    //int Pitch;
    //const int BytesPerPixel = 4;
//};

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

struct Game_State{
    int BlueOffset = 0;
    int GreenOffset = 0;
    int Hz = 256;    
};

// TODO: Allow the sample offset here for more robust platform options

internal void GameOutputSound(Game_Sound_OutPut* SecondSoundBuffer, int Hz);
// internal void RenderSplendidGradient(Game_OffScreen_Buffer* OBuffer, int XOffset, int YOffset);

uint32 safetruncateUint64(uint64 value);
real32 saferatioN(real32 numerator, real32 divisor);
real32 saferatio0(real32 numerator, real32 divisor);
real32 saferatio1(real32 numerator, real32 divisor);

#define HANDMADE_H
#endif
