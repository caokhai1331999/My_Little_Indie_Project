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

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include <GL/gl.h>

using namespace std;

#define ArrayCount(Array) (sizeof(Array) / sizeof(Array[0]))

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
typedef bool bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

typedef float real32;
typedef double real64;



struct Game_OffScreen_Buffer{  
    // BITMAPINFO Bitmapinfo;
    // HBITMAP BitmapHandle;
    void* BitmapMemory;
    int BitmapWidth;
    int BitmapHeight;
    int Pitch;
    const int BytesPerPixel = 4;
};

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
    uint32 MemorySize;
    void* PermanentStorage;
}GMemory;

struct Game_State{
    int BlueOffset = 0;
    int GreenOffset = 0;
    int Hz = 256;    
};

// TODO: Allow the sample offset here for more robust platform options

internal void GameOutputSound(Game_Sound_OutPut* SecondSoundBuffer, int Hz);
internal void RenderSplendidGradient(Game_OffScreen_Buffer* OBuffer, int XOffset, int YOffset);

#define HANDMADE_H
#endif
