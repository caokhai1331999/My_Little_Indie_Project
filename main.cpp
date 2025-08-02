/*======================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
  ================================================================================*/

#include "win32Game.h"
#include "Shader.h"
#include "SoundMaker.h"

LRESULT CALLBACK MainWindowCallBack(
    HWND Window,
    UINT Message,
    WPARAM Wparam,
    LPARAM Lparam    
                                    )
{
    LRESULT result;
    bool fDraw = false;
    POINT ptPrevious = {};
    switch(Message) {
        //case WM_CREATE:
            //printf("On Window creating stage\n");
            //break;
        case WM_SIZE:
        {
            DeviceContext = GetDC(Window);
            GetWindowDimension(Window);
            //NOTE: Whenever the window is resized, this function capture the size
             //of the new window and update a new proper DIB for that
             //DIB is a table where store BIT color infor
            Win32ResizeDIBSection(&BackBuffer, Dimens.Width, Dimens.Height);
            //Win32DisplayBufferWindow(DeviceContext, Dimens.Width, Dimens.Height,  &BackBuffer);
            OutputDebugStringA("WM_SIZE\n");
        }break;
        
        case WM_CLOSE:
        {
            GlobalRunning = false;
            OutputDebugStringA("WM_CLOSE\n");
        }break;

        case WM_KEYDOWN:
        {            
            bool IsDown = ((Lparam &(1 << 31)) == 0);
            uint32 vkCode = Wparam;
            if(vkCode == VK_LEFT) {
                
                OutputDebugStringA("Left Button :");
                if(IsDown) {                    
                    OutputDebugStringA(" Is Down");
                }
                OutputDebugStringA("\n");
            }

            if(vkCode == VK_ESCAPE){
                GlobalRunning = false;
            }
            
        }break;

        case WM_SYSKEYDOWN:
        {
            uint32 vkCode = Wparam;
            bool AltkeyisDown = ((Lparam &(1 << 29)) != 0);
            if((vkCode == VK_F4) && AltkeyisDown) {
                GlobalRunning = false;
            }                                        
            OutputDebugStringA("WM_SYSKEYDOWN\n");            
        }break;

        case WM_SYSKEYUP:
        {            
            OutputDebugStringA("WM_SYSKEYUP\n");            
        }break;

        case WM_KEYUP:
        {
            uint32 vkCode = Wparam;
            //NOTE: This is whether bit 30 or 0 (never 1).
            // So if it is bit 30 it is down 
            bool WasDown = ((Lparam &(1 << 30)) != 0);
            bool IsDown = ((Lparam &(1 << 31)) == 0);            
            if (WasDown != IsDown) {

                if(vkCode == VK_UP) {
                    State.BlueOffset+= 10;
                    printf("YOffset is %d\n", State.BlueOffset);
                }

                else if(vkCode == VK_DOWN) {
                    State.GreenOffset+= 10;
                    printf("YOffset is %d\n", State.GreenOffset);
                }

                else if(vkCode == VK_LEFT) {
                     //XOffset -= 10;
                    OutputDebugStringA("Left Button :");
                    //if(WasDown) {                    
                    //    OutputDebugStringA(" Was Down");
                    //}
                    OutputDebugStringA("\n");
                }

                else if(vkCode == VK_RIGHT) {
                    //XOffset += 10;                    
                }
                
                else if(vkCode == VK_TAB) {
                    if(SoundOutPut.hz == 128){
                        SoundOutPut.hz = 256;
                    } else if (SoundOutPut.hz == 256) {
                        SoundOutPut.hz = 512;
                    } else {
                        SoundOutPut.hz = 128;                        
                    }
                     char Output[256];
                     sprintf(Output, "TAB button hitted, Current Hert is: %d\n", SoundOutPut.hz);
                    SoundOutPut.WavePeriod = SoundOutPut.SamplePerSecond/SoundOutPut.hz;
                    
                    OutputDebugStringA("TAB button hitted");                  
                }
            }                
            }break;
        case WM_DESTROY:
        {
            GlobalRunning = false;
            PostQuitMessage(0);
            OutputDebugStringA("WM_DESTROY\n");            
        }break;
        
        case WM_PAINT:            
        {
            PAINTSTRUCT Paint;
            DeviceContext = BeginPaint(Window, &Paint);

             int X = Paint.rcPaint.left;
             int Y = Paint.rcPaint.top;
            
             int width = Paint.rcPaint.right - Paint.rcPaint.left;
             int height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            GetWindowDimension(Window);
             local_persist DWORD Operation = WHITENESS;

             if (Operation == WHITENESS) {
                 Operation = BLACKNESS;
             }else {
                 Operation = WHITENESS;
             }

             Win32DisplayBufferWindow( DeviceContext ,Dimens.Width, Dimens.Height, &BackBuffer);
             EndPaint(Window, &Paint);
            OutputDebugStringA("WM_PAINT\n");
        }
        break;
        case WM_LBUTTONDOWN: 
            fDraw = TRUE; 
            ptPrevious.x = LOWORD(Lparam); 
            ptPrevious.y = HIWORD(Lparam);
            return 0L; 
             break;
        case WM_LBUTTONUP: 
            if (fDraw) 
            { 
                DeviceContext = GetDC(Window); 
                MoveToEx(DeviceContext, ptPrevious.x, ptPrevious.y, NULL); 
                LineTo(DeviceContext, LOWORD(Lparam), HIWORD(Lparam)); 
                ReleaseDC(Window, DeviceContext); 
            } 
            fDraw = FALSE; 
            return 0L; 
             break; 
        case WM_MOUSEMOVE: 
            if (fDraw) 
            { 
                DeviceContext = GetDC(Window); 
                MoveToEx(DeviceContext, ptPrevious.x, ptPrevious.y, NULL); 
                LineTo(DeviceContext, ptPrevious.x = LOWORD(Lparam), 
                ptPrevious.y = HIWORD(Lparam)); 
                ReleaseDC(Window, DeviceContext); 
            }
            return 0L; 
             break;            
        default:
        {
            OutputDebugStringA("DEFAULT\n");
            result = DefWindowProcA(Window, Message, Wparam, Lparam);
        }break;
    }
     //return 0L;         
    return result;
}

void ErrorExit() 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    DWORD dw = GetLastError(); 

    if (FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL) == 0) {
        MessageBox(NULL, TEXT("FormatMessage failed"), TEXT("Error"), MB_OK);
        ExitProcess(dw);
    }

    MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    ExitProcess(dw); 
}

int CALLBACK WinMain
(HINSTANCE Instance,
 HINSTANCE hInstPrev,
 PSTR cmdline,
 int cmdshow)
{
    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceCounter(&PerfCountFrequencyResult);
    //NOTE: Actually, this the counts per second
    //TODO: Try to find out why the PerfCountFrequency is too large.
    //NOTE: This one count is for counting the frame
    int64 PerfCountFrequency = (int64)(PerfCountFrequencyResult.QuadPart);                
    win32LoadXInput();
    WNDCLASSA WindowClass = {};
    //WindowClass.style = CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallBack;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "First Game Window Class";
    Win32ResizeDIBSection(&BackBuffer, Dimens.Height, Dimens.Width);
    OpenConsole();
    HWND Window;
    if(RegisterClassA(&WindowClass)) {
        Window = CreateWindowExA(
            //NOTE: The window didn't show up is because the first argument
            WS_EX_APPWINDOW,
            WindowClass.lpszClassName,
            "win32GameWithoutEngine",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            Instance,
            0);

        if(Window) {
            GlobalRunning = true; 

            HDC devicecontext = GetDC(Window);
            int refreshRate = GetDeviceCaps(devicecontext, VREFRESH);
            ReleaseDC(Window, devicecontext);
            if(refreshRate > 1){
                printf("Refresh rate is : %dHz\n", refreshRate);
            };

#if INTERNAL
                LPVOID BaseAddress = megabytes(5);
#else
                LPVOID BaseAddress = 0;
#endif

            //=======================================================
            Game_Memory game_memory = {};
            game_memory.PermanentStorageSize = Megabytes(128);
            game_memory.TransientStorageSize = Megabytes((uint64)6);

            uint64 TotalSize = game_memory.PermanentStorageSize + game_memory.TransientStorageSize;
            
            game_memory.PermanentStorage = VirtualAlloc(BaseAddress , TotalSize,  MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
            game_memory.TransientStorage = ((uint8*)game_memory.PermanentStorage + game_memory.PermanentStorageSize);
            //======================================================
            debug_read_file_result result;
            BMP_content* BMPContent;
            if(game_memory.TransientStorage && game_memory.PermanentStorage){
                //printf("About to read image\n");
                // NOTE: ???? Why when I change to different bmp image it crashed
                //byte order: AA BB GG RR bottom up  

                //if (!gladLoadGLLoader((GLADloadproc)GetProcAddress))
                //{
                    //std::cout << "Failed to initialize GLAD" << std::endl;
                //}
//
                //if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress))
                //{
                    //std::cout << "Failed to initialize GLAD" << std::endl;
                //}
                InitOpenGL(Window, &BackBuffer, nullptr);

                Shader vshader;
                Shader fshader;

                loadShader(&fshader, "shader.fs");
                loadShader(&vshader, "shader.vs");
                
                BMPContent = DEBUGReadBMP("Harry and Accomplices_rescaled.bmp", &result);

                
                LARGE_INTEGER LastCounter;
                QueryPerformanceCounter(&LastCounter);
                uint64 LastCycleCounts;
            
            Game_Input Input[2] = {};
            Game_Input* OldInput = &Input[0];
            Game_Input* NewInput = &Input[1];

            LastCycleCounts = __rdtsc();

            win32_Sound_OutPut SoundOutPut = {};
            Game_Sound_OutPut SoundBuffer = {};
            InitSoundBuffer(&Window, &SoundOutPut);
            int16* SSamples = (int16* )VirtualAlloc(0 , SoundOutPut.SecondBufferSize ,MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

            Win32_OffScreen_Buffer ScreenBuffer = {};

            //Why InitOpenGl only work in the app loop

            //Pass BackBuffer data
            ScreenBuffer.BitmapMemory = BackBuffer.BitmapMemory;
            ScreenBuffer.BitmapWidth = BackBuffer.BitmapWidth;
            ScreenBuffer.BitmapHeight = BackBuffer.BitmapHeight;
            ScreenBuffer.Pitch = BackBuffer.Pitch;
            ScreenBuffer.Bitmapinfo = BackBuffer.Bitmapinfo;
            ScreenBuffer.BitmapHandle = BackBuffer.BitmapHandle;
            
            int MaxControllerCount = XUSER_MAX_COUNT;
/*
             Init here
             NOTE: Why InitOpenGL only work while in window loop
             May be this is related to Window and DC that hasn't been
             initialized yet
*/            
            //OpenGLInited = InitOpenGL(Window, &ScreenBuffer, nullptr);
            while(GlobalRunning) {
                MSG Message;
                //NOTE: This is where receiving the message to change
                // for any change in window
                while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE)) {
                    if (Message.message == WM_QUIT) {
                        GlobalRunning = false;
                    }
                    DispatchMessage(&Message);
                    TranslateMessage(&Message);
                }

             // Console Input part
                if( MaxControllerCount > ArrayCount(Input->Controller)) {
                    MaxControllerCount = ArrayCount(Input->Controller);   
                }
                
                ProcessInput(MaxControllerCount, OldInput, NewInput);
                
             // ================================================================
            // NOTE: Sounding Part
                WriteSoundToBuffer(&SoundBuffer, &SoundOutPut, SSamples);
                //=====================================================================
                 //WHY????
                 //Update here                
                //printf("Just before Game update and render\n");                

                InitOpenGL(Window, &ScreenBuffer, nullptr);                
                DeviceContext = GetDC(Window);
                use(&vshader);
                use(&fshader);                

                // Attach VAO
                // use shader
                glBindVertexArray(0);
                // Ah got it. Texture data pass directly to shader
                glDrawArrays(GL_TRIANGLES, 0, 3);
                //Why the &ScreenBuffer data doesn't show on the direct screen
                GameUpdateAndRender(&game_memory, BMPContent, NewInput, &State, &ScreenBuffer, &SoundBuffer, DeviceContext);

                //NOTE: Check whether OpenGL work or not
                 //Define the boundary of what we want to rende
                
                LARGE_INTEGER EndCounter;
                QueryPerformanceCounter(&EndCounter);

                uint64 EndCycleCounts;
                EndCycleCounts = __rdtsc();
 
                  //__rdtsc() is an intrinsict (the one which looked like a function call
                 //but it actually a hint to the compiler to a specific dissembly language intstruction)

/*                
                 S : Single 
                 I : Instruction
                 M : Multiple
                 D : Data
                
*/                
                uint64 CyclesElapsed = EndCycleCounts - LastCycleCounts;
                //NOTE: It based on the var type to decide what kind of the substraction to do
                real32 ElapsedCounter = (real32)((real32)(EndCounter.QuadPart) - (real32)(LastCounter.QuadPart));
                real32 McPerFrame = (real32)((real32)CyclesElapsed/(1000.f * 1000.f));
                real32 MsPerFrame = (real32)((1000 * (real32)ElapsedCounter) / (real32)PerfCountFrequency);
                real32 FPS = (real32)((real32)PerfCountFrequency/(real32)ElapsedCounter);
 #if 0                
                char Buffer[256];
                //NOTE: The '%' is to decide the format of the next thing to print
                 for example: %d is the 32 bit integer
                sprintf(Buffer, "%f Miliseconds/Frame, %f FPS, %f Mc/f \n ", MsPerFrame, FPS, MsPerFrame);
                OutputDebugStringA(Buffer);
 #endif                
                LastCounter = EndCounter;
                LastCycleCounts = EndCycleCounts;
/*
                MULPD -> real32 ==> 128 bits / 32 bits -> 4 real32 packs per register 
                MULPS -> real64 ==> 128 bits / 64 bits -> 2 real32 packs per register  
*/
                Game_Input* Temp = NewInput;
                NewInput = OldInput;  //???? still don't understand
                OldInput = Temp;
                ReleaseDC(Window, DeviceContext);
            }            
            }                
        }
        else{
            //TODO: Logging

            if(!IsWindow(Window)){
                printf("Window is NULL\n");
                DWORD errorCode = GetLastError();
                char buffer[256] = {};
                FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, buffer, sizeof(buffer), NULL);
                printf("%s\n", buffer);
            }

            if (!GetProcessId(NULL)){
                    ErrorExit();
            }

        }
    } else {
        //TODO: Logging
        DWORD errorCode = GetLastError();
        char buffer[256] = {};
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, buffer, sizeof(buffer), NULL);
        printf("%s\n", buffer);
    }   
        return (0);
}
