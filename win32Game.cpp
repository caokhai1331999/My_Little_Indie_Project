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

internal debug_read_file_result* DEBUGReadFileWhole(char* filename){

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
                    printf("Read image successfully\n");
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

BMP_content* DEBUGReadBMP(char* filename, debug_read_file_result* ReadResult){
    BMP_content* result = new BMP_content();
     ReadResult = DEBUGReadFileWhole(filename);
     if(ReadResult->Size != 0){

         BITMAP_HEADER *HeadResult = (BITMAP_HEADER*)ReadResult->Content;
         //Why plus ???

         uint32* pixels = (uint32* )((uint8 *)ReadResult->Content + HeadResult->bfOffBits);

         result->ImageContent = pixels;
         result->Width = HeadResult->Width;
         result->Height = HeadResult->Height;

         uint32* SourceDest = pixels;
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
    OBuffer->Pitch = OBuffer->BytesPerPixel * OBuffer->BitmapWidth;
    
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

// NOTE: Keep in mind that try to do all what you need to release back to memory
// in a total thing so that I can release it in aggregate

void ProcessXinputDigitalButton(DWORD XInputButtonState ,Game_Button_State* OldState ,DWORD ButtonBit, Game_Button_State* NewState){
    NewState->EndedDown = ((XInputButtonState & ButtonBit) == ButtonBit);
    NewState->HalfTransitionCount = ((OldState->EndedDown == NewState->EndedDown)? 1 : 0);       
}


void RenderSplendidGradient(Win32_OffScreen_Buffer* OBuffer, BMP_content* BMPContent, int XOffset, int YOffset) {
    // RR GG BB
    // Row is a pointer to every line of bitmapMemory
    // While pitch is data length of everyline of bitmap
    int32 BlitWidth =  BMPContent->Width;
    int32 BlitHeight = BMPContent->Height;
    int32 ImagePitch = 4 * BlitWidth;    

    int32 Width =  OBuffer->BitmapWidth;
    int32 Height = OBuffer->BitmapHeight;

    //BUG right here
    //if(BlitWidth > Width){
        //BlitWidth = Width;
    //}


    //if(BlitHeight > Height){
        //BlitHeight = Height;
    //}
    
    // We take memory from BitmapMemory of main Bufer to write on it
    uint8* Row = ((uint8 *)OBuffer->BitmapMemory);
    //Change the image row order upside down
    uint8* imageRow = (uint8*)BMPContent->ImageContent;
// ???? What todo if the image is bigger than the 
    imageRow += 4 * ((BlitHeight) * BlitWidth);

    for (int32 Y{0}; Y < BlitHeight; Y++) {
        uint32* Pixel = (uint32 *)Row;
        uint32* imagePixel = (uint32* )imageRow;
        for(int32 X{0}; X < BlitWidth; X++) {

//NOTE: For Gradient version
    //for (int Y = 0; Y < OBuffer->BitmapHeight; Y++) {
        //uint32* Pixel = (uint32 *)Row;
        //uint32* imagePixel = (uint32* )imageRow;
        //for(int X = 0; X < OBuffer->BitmapWidth; X++) {

            //uint8 Blue = ( X + XOffset);
            //uint8 Green = ( Y + YOffset);

            //NOTE: AA RR GG BB()
            // Because I limit the size of Pixels so it can not add Green color to its storage
            //*Pixel++ = (Green << 8|Blue);
            // NOTE: How to turn pixels order from bottom up to top down
            // Pixel :
            // ImagePointer :
            // Why Pixel appear in uint8 not uint32
            *Pixel++ = *imagePixel++;
        }
        // Instead of manually move row pointer every y axis (by add it to the pitch)
        // we just need to reuse the Pixel pointer pass it to row where it was already moved

        // NOTE: This order is for passing to OpenGL
        imageRow+=ImagePitch;

        // And this is for passing directly to the window (RIGHT)
        //imageRow-=ImagePitch;
        Row+=OBuffer->Pitch;
        //NOTE: This incidentally produce right pixel order
    }
}


void Win32DisplayBufferWindow(HDC DeviceContext, int WindowWidth, int WindowHeight, Win32_OffScreen_Buffer* OBuffer ) {

    //int ScannedLine;

    StretchDIBits(
        DeviceContext,
        0,0,OBuffer->BitmapWidth, OBuffer->BitmapHeight, // Source rectangle
        0,0,WindowWidth, WindowHeight,                 // Destination Rectangle
        // const VOID* lpBits,
        OBuffer->BitmapMemory,
        &OBuffer->Bitmapinfo,
        DIB_RGB_COLORS,
        SRCCOPY);    

    //printf("Number of scanned line is: %d\n", ScannedLine);
    
/*
     Why Flickering???
     CAUSE: the pixel drawing fx in the window/app loop
*/
}

bool InitOpenGL(HWND window, Win32_OffScreen_Buffer* OBuffer, uint32* imageContent){
    // first device context gotten from current window
    // printf("Start to init OpenGL\n");
    HDC windowDC = GetDC(window);
    HGLRC openglRC = wglCreateContext(windowDC);
    
    // Then create rendersplendidgradienting context of opengl from it
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
    bool32 initTexture = false;
    GLuint textureHandle = 0;
    GLuint textureHandle_1 = 0;

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
            // Time to create texture
            // Create texture
            // Bind it
            // and set some parameter
        if(wglMakeCurrent(windowDC, openglRC)){

            // printf("Succeed to init OpenGl\n");

            // if(!initTexture){
            //     glGenTextures(1, &textureHandle);
            //     initTexture = true;
            // }

            //printf("Succeed create OpenGL Context\n");
            glGenTextures(1, &textureHandle);
            glBindTexture(GL_TEXTURE_2D, textureHandle);

            glGenTextures(1, &textureHandle_1);
            glBindTexture(GL_TEXTURE_2D, textureHandle_1);

            //last argument This is where point to the image data
            // Why this doesn't work
            glViewport(0, 0, OBuffer->BitmapWidth, OBuffer->BitmapHeight);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, OBuffer->BitmapWidth, OBuffer->BitmapHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, OBuffer->BitmapMemory);

            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, OBuffer->BitmapWidth, OBuffer->BitmapHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, imageContent);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

            glClearColor(1.0f, 0.5f, 0.75f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glMatrixMode(GL_TEXTURE);
            glLoadIdentity();
    
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();    

            glEnable(GL_TEXTURE_2D);
        } else {
            // TODO: Diagnostic
            printf("Failed to init OpenGl\n");            
            return false;
        };   
    } else {
        printf("Failed to init OpenGl\n");
        return false;
    }
    ReleaseDC(window, windowDC);
    return true;
}


void GameUpdateAndRender(Game_Memory* Memory, BMP_content* BMPContent ,Game_Input* Input, Game_State* State, Win32_OffScreen_Buffer* OBuffer,  Game_Sound_OutPut* SoundBuffer, HDC DeviceContext){

    if(!Memory->IsInitialized){
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



    // The flickering bug is due to thes Swapbuffer inside the app
    // loop

    // Display on the screen
    //RenderSplendidGradient(OBuffer, BMPContent, 0, 0);
    //Win32DisplayBufferWindow(DeviceContext, Dimens.Width, Dimens.Height, OBuffer);
    //SwapBuffers(DeviceContext);


    // OPENGL parts ======================================================
    glBegin(GL_TRIANGLES);
    // real32 a =;
    // real32 b =;
    // real32 proj[]={        
    // }

    real32 p = 1.0f;
    //char* FileName = "structured_color_map.bmp";
    //glBitmap(
          //Dimens.Width * 0.9,
          //Dimens.Height * 0.9,
          //Dimens.Width * 0.1,
          //Dimens.Height * 0.1,
         //0,0,
          //(GLubyte *)FileName
              //);

    //Upper triangle
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-p, p);
    // glColor3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-p, -p);
    // glColor3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(p, -p);
    // glColor3f(0.0f, 0.0f, 1.0f);
    // Below triangle
    // glColor3f(p, p, p);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-p, p);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(p, p);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(p, -p);

    // Display on the screen
    RenderSplendidGradient(OBuffer, BMPContent, State->BlueOffset, State->GreenOffset);

if(glGetError() != GL_NO_ERROR){
    printf("OpenGL Error: %d\n", glGetError());
};

    glEnd();
// ==================================================================

    // Display on the screen
    SwapBuffers(DeviceContext);
    GameOutPutSound(SoundBuffer, State->Hz);
}


void OpenConsole() {
    AllocConsole();                             // Allocate a new console
    freopen("CONOUT$", "w", stdout);            // Redirect printf to console
    // freopen("CONOUT$", "w", stderr);            // Redirect stderr
    // freopen("CONIN$", "r", stdin);              // Redirect stdin (optional)
}

// void LoadTileMap(){
//     Tile
// }
