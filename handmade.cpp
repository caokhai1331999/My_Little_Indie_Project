/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "handmade.h"

// NOTE: Remember premature api optimization is a destructive way to code an api
// because is will cause difficulties for shipping code or changing platform
// and u will waste alot of time in a unnecessary big chunk of code

// TODO: Swap min, max, macros?

internal void GameOutPutSound(Game_Sound_OutPut* SecondSoundBuffer, int Hz) {
    char Output[256];
    local_persist real32 tsine = 0;
    local_persist int ToneVolume = 3000;
    
    // NOTE: The issued happened because I created and assigned value directly to  WavePeriod everytime I call the function without inittializing it properly
    // Initialize it and then assign the value solved the problem
    
    local_persist int WavePeriod = 0;
    WavePeriod = SecondSoundBuffer->SamplePerSecond/Hz;
    
    // int16 SampleValue = ((RunningSampleIndex++/(SquareWavePeriod/2))% 2) ? ToneVolume : -ToneVolume;
    int16* SampleOut = nullptr;
    SampleOut = SecondSoundBuffer->Samples;

    for (int SampleIndex{0};
         SampleIndex < SecondSoundBuffer->SampleCounts;
         SampleIndex++){

        real32 SineValue = 0;
        SineValue = sinf(tsine);            
        int16 SampleValue = (int16)(SineValue * ToneVolume);

        // NOTE: These two lines caused memory leaked
        *SampleOut++ = SampleValue;
        *SampleOut++ = SampleValue;
        // NOTE: But Game didn't show any graphics because of different issues
        // And that issues is I initualize SSample before the component value
        tsine += 2.0f*Pi32* 1.0f/(real32)WavePeriod;                            
    }                                               
    sprintf(Output, "Current Hert is: %d, Current WavePeriod is: %d \n", Hz, WavePeriod);
    OutputDebugStringA(Output);                      
}

void GameUpdateAndRender(Game_Memory* Memory ,Game_Input* Input, Game_OffScreen_Buffer* OBuffer,  Game_Sound_OutPut* SoundBuffer){

    Game_State* State = new Game_State;

    if(Memory->IsInitialized){
        State->Hz = 256;
        State->BlueOffset = 0;
        State->GreenOffset = 0;
    }
    
    Game_Controller_Input* Input0 = &Input->Controller[0];
    
    if(Input0->IsAnalog){
    State->Hz = (int)(128.0f*(Input0->EndX));
    State->BlueOffset = (int)(4.0f*(Input0->EndY));        
    } else {
        
    }
    
    if(Input0->Down.EndedDown){
        State->GreenOffset += 1;
    }
    
    RenderSplendidGradient(OBuffer, State->BlueOffset, State->GreenOffset);
    GameOutPutSound(SoundBuffer, State->Hz);
}

void InitOpenGL(HWND window){
    // first device context gotten from current window
    HDC windowDC = GetDC(window);
    // Then create rendering context of opengl from it
    HGLRC openglRC = wglCreateContext(windowDC);
    // Create the pixel format features
    PIXELFORMATDESCRIPTOR desiredPixelFormat = {};
    desiredPixelFormat.nSize = sizeof(desiredPixelFormat);
    desiredPixelFormat.nVersion =  1;
    desiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
    desiredPixelFormat.cColorBits = 24;
    desiredPixelFormat.cAlphaBits = 8;
    desiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW;

    // Assign to an index
    int suggestedPixelFormatIndex = ChoosePixelFormat(windowDC, &desiredPixelFormat);

    // Create a format from that index
    PIXELFORMATDESCRIPTOR suggestedPixelFormat;

    DescribePixelFormat(
        windowDC,
        suggestedPixelFormatIndex,
        sizeof(suggestedPixelFormat),
        &suggestedPixelFormat);

    if ( SetPixelFormat(
             windowDC,
             suggestedPixelFormatIndex,
             &suggestedPixelFormat)
         )
    {
         // Then init it
        if(wglMakeCurrent(windowDC, openglRC)){
            printf("Succeed to init OpenGl\n");
        } else {
            // TODO: Diagnostic
        };   

    } else {
        printf("Failed to init OpenGl\n");
    }
    // Release unused DC
    ReleaseDC(window, windowDC);
}
