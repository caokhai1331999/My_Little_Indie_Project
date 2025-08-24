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

    if(OBuffer->BitmapMemoryForDirectBlit) {
        VirtualFree(OBuffer->BitmapMemoryForDirectBlit, 0, MEM_RELEASE);
    }
    // NOTE: The BitmapWidth change every time we resize the window
    OBuffer->BitmapWidth = Width;
    OBuffer->BitmapHeight = Height;
    //OBuffer->Pitch = OBuffer->BytesPerPixel * OBuffer->BitmapWidth;
    OBuffer->Pitch = 4 * OBuffer->BitmapWidth;
    
    int BitMapMemorySize;

    OBuffer->Bitmapinfo.bmiHeader.biSize = sizeof(OBuffer->Bitmapinfo.bmiHeader);
    OBuffer->Bitmapinfo.bmiHeader.biWidth = OBuffer->BitmapWidth;
    OBuffer->Bitmapinfo.bmiHeader.biHeight = -OBuffer->BitmapHeight;
    OBuffer->Bitmapinfo.bmiHeader.biPlanes = 1;
    OBuffer->Bitmapinfo.bmiHeader.biBitCount = 32;
    OBuffer->Bitmapinfo.bmiHeader.biCompression = BI_RGB;
        
             
    BitMapMemorySize = OBuffer->BytesPerPixel*(OBuffer->BitmapWidth*OBuffer->BitmapHeight);

    OBuffer->BitmapMemory = VirtualAlloc(0 ,BitMapMemorySize ,MEM_COMMIT, PAGE_READWRITE);
    OBuffer->BitmapMemoryForDirectBlit = VirtualAlloc(0 ,BitMapMemorySize ,MEM_COMMIT, PAGE_READWRITE);

}

//void RenderSplendidGradient(Win32_Front_Buffer* OBuffer, imagee_content* BMPContent, int XOffset, int YOffset) {
void RenderSplendidGradient(Win32_OffScreen_Buffer* OBuffer, Win32_Front_Buffer* FBuffer, imagee_content* BMPContent, int XOffset, int YOffset, int ByteCount) {
    // RR GG BB
    // Row is a pointer to every line of bitmapMemory
    // While pitch is data length of everyline of bitmap
    int32 BlitWidth =  BMPContent->Width;
    int32 BlitHeight = BMPContent->Height;
    int32 WidthOffset = 0;
    int32 ImagePitch = 4 * BlitWidth;    

    int32 Height;
    int32 Width;

    if (OBuffer!=NULL){
        Height = OBuffer->BitmapHeight;
        Width =  OBuffer->BitmapWidth;
    } else {
        Height = FBuffer->BitmapHeight;
        Width =  FBuffer->BitmapWidth;        
    }
    
    //BUG right here
    if(BlitWidth > Width){
        WidthOffset = BlitWidth - Width;
        BlitWidth = Width;
    }

    if(BlitHeight > Height){
        BlitHeight = Height;
    }
    
    // We take memory from BitmapMemory of main Bufer to write on it
    uint8* Row;
    uint8* DirectRow;

    if (OBuffer!=NULL){
        Row = ((uint8 *)OBuffer->BitmapMemory);
    } else {
        Row = ((uint8 *)FBuffer->BitmapMemory);
    }

    if (OBuffer!=NULL){
        if(OBuffer->BitmapMemoryForDirectBlit != NULL){
            DirectRow = ((uint8 *)OBuffer->BitmapMemoryForDirectBlit);
        } else {
            printf("Bitmap Memory for direct blit is empty\n");
        }
    } else {
        DirectRow = ((uint8 *)FBuffer->BitmapMemoryForDirectBlit);
    }
    //Change the image row order upside down
    uint8* imageRow = (uint8*)BMPContent->ImageContent;
    uint8* imageRowForDirectBlit = (uint8*)BMPContent->ImageContent;
// ???? What todo if the image is bigger than the 
    imageRowForDirectBlit += 4*((BlitHeight - 1) * BlitWidth);

    for (int32 Y{0}; Y < Height; Y++) {
        uint32* Pixel = (uint32 *)Row;
        uint32* DirectPixel = (uint32 *)DirectRow;
        
        uint32* imagePixel = (uint32* )imageRow;
        if(Y == BlitHeight){
            imageRowForDirectBlit += 4*((BlitHeight - 1) * BlitWidth);            
        }
        uint32* imagePixelForDirect = (uint32* )imageRowForDirectBlit;

        for(int32 X{0}; X < Width; X++) {

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
            if(X >= BlitWidth){
                *DirectPixel++ = 0xffffffff;
            } else {
                *DirectPixel++ = *imagePixelForDirect++;
            }
        }
        // Instead of manually move row pointer every y axis (by add it to the pitch)
        // we just need to reuse the Pixel pointer pass it to row where it was already moved

        // NOTE: This order is for passing to OpenGL
        imageRow+=ImagePitch;

        // And this is for passing directly to the window (RIGHT)
        imageRowForDirectBlit-=ImagePitch;
//For GL
        if(OBuffer != NULL){
            Row+=OBuffer->Pitch;
        } else {
            Row+=FBuffer->Pitch;            
        }
// For direct blit
        if(OBuffer != NULL){
            DirectRow+=OBuffer->Pitch;
        } else {
            DirectRow+=FBuffer->Pitch;            
        }
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
        OBuffer->BitmapMemoryForDirectBlit,
        &OBuffer->Bitmapinfo,
        DIB_RGB_COLORS,
        SRCCOPY);    

    //OBuffer->BitmapMemoryForDirectBlit!=NULL?printf("Memory for direct blit was not NULL but screen still being black\n"):printf("Memory Pool is empty\n");
    
/*
     Why Flickering???
     CAUSE: the pixel drawing fx in the window/app loop
*/
}

bool InitOpenGL(HWND window, Win32_OffScreen_Buffer* OBuffer, Win32_Front_Buffer* FBuffer, imagee_content* bmpContent){
    // first device context gotten from current window
    // printf("Start to init OpenGL\n");
    HDC windowDC = GetDC(window);
        // Create the pixel format features
       // Then describe it
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
        DescribePixelFormat(
            windowDC,
            suggestedPixelFormatIndex,
            sizeof(suggestedPixelFormat),
            &suggestedPixelFormat);

        // Then set pixel format
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
// Next create OPENGL context
            OBuffer->glData.openglRC = wglCreateContext(windowDC);
            if(wglMakeCurrent(windowDC, OBuffer->glData.openglRC)){

                // NOTE: Failed right at the beginning
                success = gladLoadGL((GLADloadfunc)wglGetProcAddress);

                if(success)
                {
                    //OpenConsole();
                    //printf("GLAD load successfully\n");                    
                    //
                //float trianglesVerticles [] = {
                    //FRONT FACE
                   //-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
                    //0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
                   //-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
                    //0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
                    //0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
                   //-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f
                //};
             //
                float Vertices[] = {
                   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
                };
            
                float PlaneVerticles[] = {
                    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
                    //        x,     y,           z
                   -1.0f, -1.0f,  5.0f,  2.0f, 0.0f, // Each verticle
                    5.0f, -1.0f,  5.0f,  0.0f, 0.0f,
                   -1.0f, -1.0f, -5.0f,  0.0f, 2.0f,

                    5.0f, -1.0f,  5.0f,  2.0f, 0.0f,
                   -1.0f, -1.0f, -5.0f,  0.0f, 2.0f,
                    5.0f, -1.0f, -5.0f,  2.0f, 2.0f			        
                };

                glGenBuffers(1, &OBuffer->glData.VBO);
                glGenVertexArrays(1, &OBuffer->glData.VAOs);

                glBindBuffer(GL_ARRAY_BUFFER, OBuffer->glData.VBO);
                //glBufferData(GL_ARRAY_BUFFER, sizeof(trianglesVerticles), &trianglesVerticles, GL_STATIC_DRAW);

                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);

                printf("Size of Vertices:%d\n", sizeof(Vertices));

                glBindVertexArray(OBuffer->glData.VAOs);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                //glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8*sizeof(float), (void*)(3*sizeof(float)));
//
                //glEnableVertexAttribArray(1);

                //  index, size, type, .., stride, pointer
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
                glEnableVertexAttribArray(2);
                
                // NOTE: To here we done assigned CubeVerticles data to VAOs and VBO
                // We will call bindbuffer/vertexArray whenever before glDrawArray
// NOTE: We delve into Buffer drawing later!!!!
                //===============================================================
                
                //printf("Succeed create OpenGL Context\n");
                //OBuffer->glData.textureHandle = (unsigned int*)malloc(sizeof(unsigned int));

                // NOTE: Found it: The temptexture is local to this fx so its
                // data and address turn to null after the fx called

                glGenTextures(1, &OBuffer->glData.textureHandle);
                glBindTexture(GL_TEXTURE_2D, 0);
// OBuffer->glData.textureHandle is the name of the texture
                //last argument This is where point to the image data
                // Why this doesn't work
                glViewport(0, 0, OBuffer->BitmapWidth, OBuffer->BitmapHeight);
                
                //if(OBuffer->BitmapMemory != NULL){
                    //printf("We have Image content but somehow it wasn't shown on screen\n");                    
                //} else {
                    //RenderSplendidGradient(OBuffer, NULL, BMPContent, 0, 0, 3);                    
                    //printf("Image content is NULL\n");
                //}
// NOTE: Focus on this

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, OBuffer->BitmapWidth, OBuffer->BitmapHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, OBuffer->BitmapMemory);
 
                //glPixelStorei(GL_UNPACK_ROW_LENGTH, OBuffer->Pitch / 4);
                //
                glBindTexture(GL_TEXTURE_2D, OBuffer->glData.textureHandle);
                glGenerateMipmap(GL_TEXTURE_2D);
                
   //Wrapping
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   //Filter
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                GLenum err = glGetError();

                if (err != GL_NO_ERROR) {
                    printf("OpenGL Error after glTexImage2D: %x\n", err);
                }

                if(OBuffer->glData.textureHandle!=NULL){
                    printf("Texture name is: %d\n", OBuffer->glData.textureHandle);
                } else {
                    printf("Some How texture is NULL???\n");
                }
                //NOTE: The commented part is one that is deprecated when using the
                // new version of OPENGL
                //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

                // Deprecated
                //glEnable(GL_TEXTURE_2D);
                glEnable(GL_DEPTH_TEST);


                glClearColor(1.0f, 1.0f, 0.75f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

                const GLubyte* ver = glGetString(GL_VERSION);
                if (ver)
                    printf("OpenGL version: %s\n", ver);
                else
                    printf("glGetString(GL_VERSION) returned NULL\n");

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


void GameUpdateAndRender(Game_Memory* Memory, imagee_content* BMPContent ,Game_Input* Input, Game_State* State, Win32_Front_Buffer* OBuffer,  Game_Sound_OutPut* SoundBuffer, HDC DeviceContextt){

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
    //Win32DisplayBufferWindow(DeviceContext, Dimens.Width, Dimens.Height, OBuffer);
    //SwapBuffers(DeviceContextt);    
    // Display on the screen

    //NOTE: I did this the wrong way
    //RenderSplendidGradient(OBuffer, BMPContent, 0, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    //SwapBuffers(DeviceContextt);    

    //if(glGetError() != GL_NO_ERROR){
        //printf("OpenGL Error: %d\n", glGetError());
    //};

    // Display on the screen
    // The glitching sound driven me nearly crazy so I decided to turn it off
    GameOutPutSound(SoundBuffer, State->Hz);
}

void copyBufferData(Win32_OffScreen_Buffer* BackBuffer, Win32_Front_Buffer* ScreenBuffer){
    
    ScreenBuffer->BitmapWidth = ScreenBuffer->BitmapWidth!=BackBuffer->BitmapWidth?BackBuffer->BitmapWidth:printf("W didn't change\n");
    ScreenBuffer->BitmapHeight = ScreenBuffer->BitmapHeight!=BackBuffer->BitmapHeight?BackBuffer->BitmapHeight:printf("H didn't change\n");
    ScreenBuffer->Pitch = ScreenBuffer->Pitch!=BackBuffer->Pitch?BackBuffer->Pitch:printf("Pitch didn't change\n");

    // Why if I don't pass this type of data the app will collapse as the conflict of memory
    if( ScreenBuffer->glData.VAOs != BackBuffer->glData.VAOs){
        ScreenBuffer->glData.VAOs = BackBuffer->glData.VAOs;
    }

    if(ScreenBuffer->glData.VBO != BackBuffer->glData.VBO){
        ScreenBuffer->glData.VBO = BackBuffer->glData.VBO;
    }

    if(ScreenBuffer->glData.ProgramID != BackBuffer->glData.ProgramID){
        ScreenBuffer->glData.ProgramID = BackBuffer->glData.ProgramID;
    }

    if(ScreenBuffer->glData.VAOs != BackBuffer->glData.VAOs){
        ScreenBuffer->glData.VAOs = BackBuffer->glData.VAOs;
    }

    if(ScreenBuffer->glData.textureHandle != BackBuffer->glData.textureHandle){
        ScreenBuffer->glData.textureHandle = BackBuffer->glData.textureHandle; 
    }
        //ScreenBuffer->Bitmapinfo = BackBuffer->Bitmapinfo;
//
        //if(ScreenBuffer->BitmapHandle != BackBuffer->BitmapHandle && BackBuffer->BitmapHandle != NULL){
        //ScreenBuffer->BitmapHandle = BackBuffer->BitmapHandle;
    //}

        ScreenBuffer->BitmapMemory = BackBuffer->BitmapMemory;
}

void displayBufferData(Win32_OffScreen_Buffer* BackBuffer, Win32_Front_Buffer* FrontBuffer){
    printf("=====================================\n");
    printf("              |  BackBuffer | FrontBuffer\n");
    printf("VAOS          |  %d        |  %d\n", BackBuffer->glData.VAOs, FrontBuffer->glData.VAOs);
    printf("TextureID     |%d| %d\n", BackBuffer->glData.textureHandle, FrontBuffer->glData.textureHandle);
    printf("Memory Address|0x%x|0x%x\n", BackBuffer->BitmapMemory, FrontBuffer->BitmapMemory);
    printf("=====================================\n");
}

// void LoadTileMap(){
//     Tile
// }
