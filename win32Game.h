#if !defined(WIN32GAME_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <DSound.h> 
#include <combaseapi.h>
#include <endpointvolume.h>
#include <strmif.h>
#include <initguid.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <xinput.h>
#include "handmade.h"

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


struct win32Dimension{
    int PosX{0};
    int PosY{0};
    int Height{720};
    int Width{1280};
}Dimens;

struct Win32_OffScreen_Buffer{  
    BITMAPINFO Bitmapinfo;
    HBITMAP BitmapHandle;

    void* BitmapMemory = nullptr;
    int BitmapWidth;
    int BitmapHeight;
    int Pitch;
    const int BytesPerPixel = 4;
};

struct win32_Sound_OutPut{
    int SamplePerSecond;
    int BytesPerSample;
    // Hert(hz) is cycles per second
    int32 SecondBufferSize;
    uint32 RunningSampleIndex;
    real32 tsine;
    int hz;
    int LatencySampleCount;
    int WavePeriod;
    int SquareWaveCount;
    int ToneVolume;
    // Sample per cycle is SquareWave Period    
}SoundOutPut;



global_variable bool  GlobalRunning;
global_variable HWND Window;
global_variable RECT ClientRect;
global_variable HDC DeviceContext;
// global_variable int  XOffset{0}, YOffset{0};
global_variable LPDIRECTSOUNDBUFFER GlobalSecondBuffer;
global_variable Win32_OffScreen_Buffer BackBuffer = {};

global_variable Game_State State = {};
const global_variable int Height = 720;
const global_variable int Width = 1280;

void* PlatformLoadFile(char* FileName);

debug_read_file_result* DEBUGReadFileWhole(char* filename);
uint32* DEBUGReadBMP(char* filename, debug_read_file_result* result);
bool32 DEBUGWriteWholeFile(char* filename, uint32 memorysize, void* memory);
void DEBUGFreeFileMemory(void* memory);

void win32LoadXInput(void);
void ProcessXinputDigitalButton(DWORD XInputButtonState ,Game_Button_State* OldState ,DWORD ButtonBit, Game_Button_State* NewState);

void GetWindowDimension(HWND Window);
void Win32ResizeDIBSection(Win32_OffScreen_Buffer* OBuffer, int Width, int Height);
void RenderSplendidGradient(Win32_OffScreen_Buffer* OBuffer, uint32* ImageContent, int XOffset, int YOffset);

void Win32DisplayBufferWindow (HDC DeviceContext, int WindowWidth, int WindowHeight, Win32_OffScreen_Buffer* OBuffer);

void GameOutputSound(Game_Sound_OutPut* SecondSoundBuffer, int Hz);

void GameUpdateAndRender(Game_Memory* Memory = nullptr, uint32* ImageContent = nullptr ,Game_Input* Input = nullptr, Game_State* State = nullptr ,Win32_OffScreen_Buffer* OBuffer = nullptr, Game_Sound_OutPut* SoundBuffer = nullptr, HDC DeviceContext = NULL);

void OpenConsole();
void InitOpenGL(HWND window, Win32_OffScreen_Buffer* OBuffer, uint32* imageContent);
#define WIN32GAME_H
#endif
