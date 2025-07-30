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

void OpenConsole() {
    AllocConsole();                             // Allocate a new console
    freopen("CONOUT$", "w", stdout);            // Redirect printf to console
    // freopen("CONOUT$", "w", stderr);            // Redirect stderr
    // freopen("CONIN$", "r", stdin);              // Redirect stdin (optional)
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


    if(BlitHeight > Height){
        BlitHeight = Height;
    }
    
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
        //imageRow+=ImagePitch;

        // And this is for passing directly to the window (RIGHT)
        imageRow-=ImagePitch;
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
    // Failed to load wglCreateContext function
    HGLRC openglRC;
        // Then create rendersplendidgradienting context of opengl from it
        // Create the pixel format features
        PIXELFORMATDESCRIPTOR desiredPixelFormat = {};
        desiredPixelFormat.nSize = sizeof(desiredPixelFormat);
        desiredPixelFormat.nVersion =  1;
        desiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
        desiredPixelFormat.cColorBits = 32;
        desiredPixelFormat.cAlphaBits = 8;
        desiredPixelFormat.cDepthBits = 24;
        desiredPixelFormat.cStencilBits = 8;
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

            // NOTE: Failed right at the beginning
            bool success = false;
            openglRC = wglCreateContext(windowDC);

            if(wglMakeCurrent(windowDC, openglRC)){

                // NOTE: Failed right at the beginning
                success = gladLoadGL((GLADloadfunc)wglGetProcAddress);

                if(success)
                {
                    //OpenConsole();
                    //printf("GLAD load successfully\n");                    

                    const GLubyte* ver = glGetString(GL_VERSION);
                    if (ver)
                        printf("OpenGL version: %s\n", ver);
                    else
                        printf("glGetString(GL_VERSION) returned NULL\n");
                    
                float trianglesVerticles [] = {
                    //FRONT FACE
                    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f                
                };
             
                float CubeVerticles[] = {
                    // CW
                    // positions          // normals           // texture coords
                    //BACK FACE
                    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,// one stride  
                    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,     
                    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
                    //FRONT FACE
                    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
                    // LEFT FACE
                    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                    // RIGHT FACE
                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                    // BOTTOM FACE
                    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
                    // TOP FACE
                    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,    
                    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
                };
            
                float PlaneVerticles[] = {
                    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
                    //       x,  y,    z
                    -0.5f, -0.5f,  5.0f,  2.0f, 0.0f, // Each verticle
                    5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
                    -0.5f, -0.5f, -5.0f,  0.0f, 2.0f,

                    5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
                    -0.5f, -0.5f, -5.0f,  0.0f, 2.0f,
                    5.0f, -0.5f, -5.0f,  2.0f, 2.0f			        
                };

                OBuffer->OData = {};
            
                glGenBuffers(1, &OBuffer->OData.VBO);
                glGenVertexArrays(1, &OBuffer->OData.VAOs);

                glBindBuffer(GL_ARRAY_BUFFER, OBuffer->OData.VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVerticles), &CubeVerticles, GL_STATIC_DRAW);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
                glEnableVertexAttribArray(0);

                glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 0, (void*)(6*sizeof(float)));
                glEnableVertexAttribArray(2);

                // NOTE: To here we done assigned CubeVerticles data to VAOs and VBO
                // We will call bindbuffer/vertexArray whenever before glDrawArray
            
                //printf("Succeed create OpenGL Context\n");
                glGenTextures(1, &textureHandle);
                glBindTexture(GL_TEXTURE_2D, textureHandle);

                glGenTextures(1, &textureHandle_1);
                glBindTexture(GL_TEXTURE_2D, textureHandle_1);

                //last argument This is where point to the image data
                // Why this doesn't work
                glViewport(0, 0, OBuffer->BitmapWidth, OBuffer->BitmapHeight);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, OBuffer->BitmapWidth, OBuffer->BitmapHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, OBuffer->BitmapMemory);

                //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, OBuffer->BitmapWidth, OBuffer->BitmapHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, imageContent);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                //NOTE: The commented part is one that is deprecated when using the
                // new version of OPENGL
                //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

                glClearColor(1.0f, 0.5f, 0.75f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                glEnable(GL_TEXTURE_2D);
                }
                else
                {

                    DWORD ErrorContent = GetLastError();
                    LPVOID ErrorMsgBuffer;
                    if (FormatMessage(
                            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                            FORMAT_MESSAGE_FROM_SYSTEM |
                            FORMAT_MESSAGE_IGNORE_INSERTS,
                            NULL,
                            ErrorContent,
                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                            (LPTSTR) &ErrorMsgBuffer,
                            0, NULL) == 0){
                        printf("Failed to format message\n");
                    } else

                    {
                        printf("Could n't initialized GLAD first try, Error: %s\n", (char* )ErrorMsgBuffer);                                        

                    }                
                }

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
    //glBegin(GL_TRIANGLES);
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
/*
// NOTE: Old and Deprecated function
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
*/

    //
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Display on the screen
    RenderSplendidGradient(OBuffer, BMPContent, State->BlueOffset, State->GreenOffset);
    
    
if(glGetError() != GL_NO_ERROR){
    printf("OpenGL Error: %d\n", glGetError());
};

    //glEnd();
// ==================================================================

    // Display on the screen
    SwapBuffers(DeviceContext);
    // The glitching sound driven me nearly crazy so I decided to turn it off
    GameOutPutSound(SoundBuffer, State->Hz);
}

// void LoadTileMap(){
//     Tile
// }
