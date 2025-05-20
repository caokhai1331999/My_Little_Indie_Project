/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
/*
  TODO: THIS IS NOT A FINAL PLATFORM LAYER
   - Saved the game location
   - Get a handle to our own executable file
   - Asset loading path
   - Threading (launch a thread)
   - Raw Input (Support multiple keyboards)
   - Sleep/TimeBeginPeriod
   - ClipCursor() (for multiple monitors)
   - FullScreen Support
   - WM_SetCursor ( Control Cursor Visibility)
   - QueryCancelAutoplay
   - WM_ActivateApp ( For when we are not active application)
   - Blit speed improvement (using BitBlt)
   - Hardware Acceleration (OpenGl or Direct3D or Both)
   - Get Keyboard layout (For French layout, international WASD support)

   Just a partial list if you want to get the game in a complete shipping state
   
 */
#include "win32Game.h"

void
win32LoadXInput(void) {
    HMODULE XInputLibrary = LoadLibrary("xinput1_4.dll");
    if (!XInputLibrary) {
        // TODO: Do a diagnostic
        XInputLibrary = LoadLibrary("xinput1_3.dll");
    }
    // somehow it couldn't find the dll file maybe due to the function or
    // it's just not there therefore I used xinput1_4.dll instead
    if(XInputLibrary) {
        // retrieve the address of the exported function in dll file
        XinputGetState = (x_input_get_state *)GetProcAddress(XInputLibrary, "XInputGetState");
        if (!XinputGetState) {XinputGetState = XinputGetStateStub;}
        XinputSetState = (x_input_set_state *)GetProcAddress(XInputLibrary, "XInputSetState");
        if (!XinputSetState) {XinputSetState = XinputSetStateStub;}
    } else {
        // TODO: Do a diagnostic
    }
}

void RenderSplendidGradient(Game_OffScreen_Buffer* OBuffer, int XOffset, int YOffset) {
    // RR GG BB
    // Row is a pointer to every line of bitmapMemory
    // While pitch is data length of everyline of bitmap
    int Width = OBuffer->BitmapWidth;
    int Height = OBuffer->BitmapHeight;    
    int Pitch = OBuffer->Pitch;
    uint8* Row = (uint8 *)OBuffer->BitmapMemory;
    
    for (int Y{0}; Y < Height; Y++) {
        uint32* Pixel = (uint32 *)Row;
        for(int X{0}; X < Width; X++) {
            uint8 Blue = ( X + XOffset);
            uint8 Green = ( Y + YOffset);
            // NOTE: AA RR GG BB()
            // Because I limit the size of Pixels so it can not add Green color to its storage
            *Pixel++ = ( (Green<<8) | Blue);
        }
        // Instead of manually move row pointer every y axis (by add it to the pitch)
        // we just need to reuse the Pixel pointer pass it to row where it was already moved
        Row = (uint8 *)Pixel;        
    }

}

void GameOutPutSound(Game_Sound_OutPut* SecondSoundBuffer, int Hz) {
    char Output[256];
    local_persist real32 tsine = 0;
    local_persist int ToneVolume = 3000;
    
    // NOTE: The issued happened because I created and assigned value directly to  WavePeriod everytime I call the function without inittializing it properly
    // Initialize it and then assign the value solved the problem
    
    local_persist int WavePeriod = 0;
    WavePeriod = SecondSoundBuffer->SamplePerSecond/Hz;
    
    // int16 SampleValue = ((RunningSampleIndex++/          (SquareWavePeriod/2))% 2) ? ToneVolume : -ToneVolume;
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


void GetWindowDimension(HWND Window) {
    GetClientRect(Window, &ClientRect);
    Dimens.Width = ClientRect.right - ClientRect.left;
    Dimens.Height= ClientRect.bottom - ClientRect.top;
}

void Win32ResizeDIBSection(Win32_OffScreen_Buffer* OBuffer, int Width, int Height) {
    
    if(OBuffer->BitmapMemory) {
        VirtualFree(OBuffer->BitmapMemory, 0, MEM_RELEASE);
    }
    // NOTE: The BitmapWidth change every time we resize the window
    OBuffer->BitmapWidth = Width;
    OBuffer->BitmapHeight = Height;
    OBuffer->Pitch = OBuffer->BytesPerPixel * OBuffer->BitmapWidth *    OBuffer->BitmapHeight;
    
    int BitMapMemorySize;

    OBuffer->Bitmapinfo.bmiHeader.biSize = sizeof(OBuffer->Bitmapinfo.bmiHeader);
    OBuffer->Bitmapinfo.bmiHeader.biWidth = OBuffer->BitmapWidth;
    OBuffer->Bitmapinfo.bmiHeader.biHeight = -OBuffer->BitmapHeight;
    OBuffer->Bitmapinfo.bmiHeader.biPlanes = 1;
    OBuffer->Bitmapinfo.bmiHeader.biBitCount = 32;
    OBuffer->Bitmapinfo.bmiHeader.biCompression = BI_RGB;
        
             
    BitMapMemorySize = OBuffer->BytesPerPixel*(OBuffer->BitmapWidth*OBuffer->BitmapHeight);
    OBuffer->BitmapMemory = VirtualAlloc(0 ,BitMapMemorySize ,MEM_COMMIT, PAGE_READWRITE);
}

// NOTE: Keep in mind that try to all what you need to release back to memory
// in a total thing so that I can release it in aggregate

void ProcessXinputDigitalButton(DWORD XInputButtonState ,Game_Button_State* OldState ,DWORD ButtonBit, Game_Button_State* NewState){
    NewState->EndedDown = ((XInputButtonState & ButtonBit) == ButtonBit);
    NewState->HalfTransitionCount = ((OldState->EndedDown == NewState->EndedDown)? 1 : 0);       
}

void Win32DisplayBufferWindow(HDC DeviceContext, int WindowWidth, int WindowHeight, Win32_OffScreen_Buffer* OBuffer ) {
    
    StretchDIBits(
        DeviceContext,
        0,0,OBuffer->BitmapWidth, OBuffer->BitmapHeight, // Source rectangle
        0,0,WindowWidth, WindowHeight,                 // Destination Rectangle
        // const VOID* lpBits,
        OBuffer->BitmapMemory,
        &OBuffer->Bitmapinfo,
        DIB_RGB_COLORS,
        SRCCOPY
                  );    
    
    printf("Draw something here\n");
    glViewport(0, 0, 70, 40);
    glClearColor(1.0f, 0.5f, 0.75f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    // Upper triangle
    glVertex2i(0, 0);
    glVertex2i(Width, Height);
    glVertex2i(0, Height);
    // Below triangle
    glVertex2i(0, 0);
    glVertex2i(Width, Height);
    glVertex2i(Width, 0);
    glEnd();
    // Display on the screen
    SwapBuffers(DeviceContext);

}


void GameUpdateAndRender(Game_Memory* Memory ,Game_Input* Input, Game_State* State, Game_OffScreen_Buffer* OBuffer,  Game_Sound_OutPut* SoundBuffer){

    if(Memory->IsInitialized){
        State->Hz = 256;
        // State->BlueOffset = 0;
        // State->GreenOffset = 0;
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
    HGLRC openglRC = wglCreateContext(windowDC);
    // Then create rendering context of opengl from it
    // Create the pixel format features
    PIXELFORMATDESCRIPTOR desiredPixelFormat = {};
    desiredPixelFormat.nSize = sizeof(desiredPixelFormat);
    desiredPixelFormat.nVersion =  1;
    desiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
    desiredPixelFormat.cColorBits = 24;
    desiredPixelFormat.cAlphaBits = 8;
    desiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;

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
