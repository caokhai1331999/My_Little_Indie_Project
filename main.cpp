/*======================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
  ================================================================================*/
#include <glm/gtx/string_cast.hpp>
#include "win32Game.h"
#include "shader.h"
#include "SoundMaker.h"
#include <ctime>
#include <cstdlib>

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
// What is DeviceContext for in this case??
            GetWindowDimension(Window);
            //NOTE: Whenever the window is resized, this function capture the size
            //of the new window and update a new proper DIB for that
            //DIB is a table where store BIT color infor
            Win32ResizeDIBSection(&BackBuffer, Dimens.Width, Dimens.Height);
            if(!BackBuffer.transferNeed){
                BackBuffer.transferNeed = true;
            }           
            //glViewport(0, 0, BackBuffer.BitmapWidth, BackBuffer.BitmapHeight);
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
            bool WasDown = ((Lparam &(1 << 30)) != 0);

            uint32 vkCode = Wparam;
            if(IsDown){
                if(vkCode == VK_UP) {
                    State.BlueOffset+= 10;
                    BackBuffer.camera.Position += BackBuffer.camera.Front *BackBuffer.camera.speed;
                    printf("Up is HIT\n");
                }

                else if(vkCode == VK_DOWN) {
                    State.GreenOffset+= 10;
                    BackBuffer.camera.Position -= BackBuffer.camera.Front *BackBuffer.camera.speed;
                    printf("Down is HIT\n");
                }

                else if(vkCode == VK_LEFT) {
                    //XOffset -= 10;
                    OutputDebugStringA("Left Button :");
                    //if(WasDown) {                    
                    BackBuffer.camera.Position -=  glm::normalize(glm::cross(BackBuffer.camera.Front, BackBuffer.camera.Up)) * BackBuffer.camera.speed;
                    //OutputDebugStringA(" Was Down");
                    //}
                    printf("LEFT is HIT\n");
                }

                else if(vkCode == VK_RIGHT) {
                    BackBuffer.camera.Position += glm::normalize(glm::cross(BackBuffer.camera.Front, BackBuffer.camera.Up)) * BackBuffer.camera.speed;
                    printf("LEFT is HIT\n");
                    //XOffset += 10;                    
                }

                else if(vkCode == VK_SPACE) {
                    BackBuffer.camera.Position += BackBuffer.camera.Up * BackBuffer.camera.speed;
                    printf("Space is HIT\n");
                    //XOffset += 10;                    
                }

                else if(vkCode == VK_SHIFT) {
                    BackBuffer.camera.Position -= BackBuffer.camera.Up * BackBuffer.camera.speed;
                    printf("Shift is HIT\n");
                    //XOffset += 10;                    
                }

                if (!BackBuffer.camera.moved){
                    BackBuffer.camera.moved = true;
                }

                //if(vkCode == VK_LEFT) {
                //
                //OutputDebugStringA("Left Button :");
                //if(IsDown) {                    
                //OutputDebugStringA(" Is Down");
                //
                //}
                //OutputDebugStringA("\n");
                //}

                if(vkCode == VK_ESCAPE){
                    if(GlobalRunning){
                        GlobalRunning = false;
                    }
                }                
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
            //if (WasDown != IsDown) {
                
            //else
                if(vkCode == VK_TAB) {
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

            //}                
        }break;
        
        case WM_DESTROY:
        {
            GlobalRunning = false;
            PostQuitMessage(0);
            OutputDebugStringA("WM_DESTROY\n");            
        }break;
        
        case WM_PAINT:            
        {
 
            BeginPaint(Window, NULL);
            HDC tempDC = GetDC(Window);
            // Start to save bit drawing data to the current HDC
            RenderSplendidGradient(&BackBuffer, NULL, BMPContent, 0, 0, 4);
            Win32DisplayBufferWindow(tempDC, Dimens.Width, Dimens.Height, &BackBuffer);

            if(glGetError() != GL_NO_ERROR){
                printf("OpenGL Error: %d\n", glGetError());
            };

            //glBindVertexArray(BackBuffer.glData.VAOs);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            SwapBuffers(tempDC);            
            EndPaint(Window, NULL);
            ReleaseDC(Window, tempDC);

            OutputDebugStringA("WM_PAINT\n");
        }break;

        case WM_LBUTTONDOWN:{
            fDraw = TRUE; 
            ptPrevious.x = LOWORD(Lparam); 
            ptPrevious.y = HIWORD(Lparam);
            return 0L;             
        } break;
        case WM_LBUTTONUP:{
            if (fDraw) 
            { 
                DeviceContext = GetDC(Window); 
                MoveToEx(DeviceContext, ptPrevious.x, ptPrevious.y, NULL); 
                LineTo(DeviceContext, LOWORD(Lparam), HIWORD(Lparam)); 
                ReleaseDC(Window, DeviceContext); 
            } 
            fDraw = FALSE; 
            return 0L;             
        }break; 
        case WM_MOUSEMOVE:{
            if (fDraw) 
            { 
                DeviceContext = GetDC(Window); 
                MoveToEx(DeviceContext, ptPrevious.x, ptPrevious.y, NULL); 
                LineTo(DeviceContext, ptPrevious.x = LOWORD(Lparam), 
                       ptPrevious.y = HIWORD(Lparam)); 
                ReleaseDC(Window, DeviceContext); 
            }
            return 0L;             

            BackBuffer.camera.mouse.xPos =  GET_X_LPARAM(lParam); 
            BackBuffer.camera.mouse.yPos = GET_Y_LPARAM(lParam); 

            if(!BackBuffer.camera.mouse.moved){
                
            }

        }break;            
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

            if(!GlobalRunning){
                GlobalRunning = true; 
            }

            HDC DeviceContext = GetDC(Window);
            int refreshRate = GetDeviceCaps(DeviceContext, VREFRESH);
            ReleaseDC(Window, DeviceContext);
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
            debug_read_file_result result2;
            if(game_memory.TransientStorage && game_memory.PermanentStorage){
                //printf("About to read image\n");
                // NOTE: ???? Why when I change to different bmp image it crashed
                //byte order: AA BB GG RR bottom up                  
                BMPContent = DEBUGReadBMP("Harry and Accomplices_rescaled.bmp", &result);
                //JPGContent = DEBUGReadJPG("Harry and Accomplices.jpg", &result2);
                //OpenGL part
                Win32_Front_Buffer ScreenBuffer = Win32_Front_Buffer(BackBuffer.BitmapWidth, BackBuffer.BitmapHeight, &BackBuffer.glData, BackBuffer.BitmapMemory);
// Cause the ScreenData will be deleted out of the loop so
                // We have to assign address of memory and glData to
                //InitOpenGL(Window, &BackBuffer, &ScreenBuffer, JPGContent);
                std::srand(std::time(NULL));
                RenderSplendidGradient(&BackBuffer, &ScreenBuffer, BMPContent, 0, 0, 4);
                InitOpenGL(Window, &BackBuffer, &ScreenBuffer, BMPContent);

                Shader vshader;
                Shader fshader;

                loadShader(&vshader, "shader.vs", (VertexType)vertex);
                loadShader(&fshader, "shader.fs", (VertexType)fragment);
                ScreenBuffer.glData.ProgramID = setupGLprogram(&vshader, &fshader);                
                copyBufferData(&BackBuffer, &ScreenBuffer);
                //????
                //glm::mat4 View = glm::mat4(1.0f);
                glm::mat4 Model = glm::mat4(1.0f);
                glm::mat4 Model2 = glm::mat4(1.0f);
                Model2 = glm::translate(Model2, glm::vec3(-4.0f, 0.0f, 0.0f));
                std::cout<<"Stand still model 2 matrix is :"<<glm::to_string(Model2)<<std::endl;
                glm::mat4 Projection = glm::mat4(1.0f);

                //View = glm::translate(View, glm::vec3(0.0f, 0.0f, -0.3f));
                glm::vec3 Point = glm::vec3(0.0f, 0.0f, 0.0f);

                glm::vec3 Position = glm::vec3(4.0f, 3.0f, 3.0f);
                glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
                glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

                glm::vec3 Right = glm::normalize(glm::cross(Front, Up));

                //set camera view here
                BackBuffer.camera = Camera(BackBuffer.BitmapWidth, BackBuffer.BitmapHeight, Position, Point, Front, Up, Right);
                //std::cout<<"View matrix from camera: "<<glm::to_string(BackBuffer.camera.view)<<std::endl;

                // This will be replaced by camera.view matrix
                glm::mat4 View = glm::lookAt(Position, glm::vec3(0,0,0), Up);
                //std::cout<<"View matrix: "<<glm::to_string(View)<<std::endl;
                float fov = 45.0f;
                Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));
                std::cout<<"Central rotating model is"<<glm::to_string(Model)<<std::endl;
                Projection = glm::perspective(glm::radians(fov), (float)ScreenBuffer.BitmapWidth / (float)ScreenBuffer.BitmapHeight, 0.1f, 100.0f);
                //printf("Part of Projection matrix:%f %f %f\n", Projection[0][1], Projection[1][1], Projection[2][1]);
                std::cout<<"Perspective matrix: "<<glm::to_string(Projection)<<std::endl;

                
                if(glIsProgram(ScreenBuffer.glData.ProgramID)){
                    useProgram(ScreenBuffer.glData.ProgramID);
                    printf("Program ID: %d\n", ScreenBuffer.glData.ProgramID);
                } else {
                    glDebugMessageCallback(MessageCallback, 0);
                    checkCompileErrors(vshader.shaderID, "Vertex");
                    checkCompileErrors(fshader.shaderID, "Fragment");
                    checkCompileErrors(ScreenBuffer.glData.ProgramID, "Program");
                    printf("NO program object created before\n");
                }

                setInt(ScreenBuffer.glData.ProgramID, "ttexture1", ScreenBuffer.glData.textureHandle);                
                setMat4(ScreenBuffer.glData.ProgramID, "view", BackBuffer.camera.view);
                setMat4(ScreenBuffer.glData.ProgramID, "projection", Projection);
                
                printf("texture id:%d\n", ScreenBuffer.glData.textureHandle);
                printf("vertex array :%d\n", ScreenBuffer.glData.VAOs);

                // =============================================
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

                //Why InitOpenGl only work in the app loop
            
                int MaxControllerCount = XUSER_MAX_COUNT;
/*
  Init here
  NOTE: Why InitOpenGL only work while in window loop
  May be this is related to Window and DC that hasn't been
  initialized yet
*/            
                WaitTimeCounter = 0.0f;
                float ChangeAxisCounter = 0.0f;
                int64 ViewRotateCount = 0;

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
                // Attach VAO
                if(BackBuffer.transferNeed){
                    int count = 0;
                    while (count < 6){
                        count++;
                    };
                    if (count >= 5){
                        copyBufferData(&BackBuffer, &ScreenBuffer);
                        BackBuffer.transferNeed = false;                        
                        displayBufferData(&BackBuffer, &ScreenBuffer);
                        glViewport(0, 0, ScreenBuffer.BitmapWidth, ScreenBuffer.BitmapHeight);
                    };
                }

                DeviceContext = GetDC(Window);
                // use shader program
                //printf("texture id:%d\n", ScreenBuffer.glData.textureHandle[0]);
                //DeviceContext = GetDC(Window);
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);             
                //printf("Program ID:%d \n", ScreenBuffer.glData.ProgramID);
                //else {
                    //printf("NO program object created before\n");
                //}
                //glActiveTexture(GL_TEXTURE0);
                //glBindTexture(GL_TEXTURE_2D, 1);
                //printf("Texture ID: %d\n", ScreenBuffer.glData.textureHandle[0]);
                GameUpdateAndRender(&game_memory, BMPContent, NewInput, &State, &ScreenBuffer , &SoundBuffer, NULL);                
                // camera/view transformation
                //Model = glm::rotate(Model, glm::radians(fov)*0.5f, glm::vec3(1.0f, 0.0f, 0.5f));


                // Display on the screen
                // The glitching sound driven me nearly crazy so I decided to turn it off                
                // Ah got it. Texture data pass directly to shader(NOPE)
                // We define that through setInt
                //Why the &ScreenBuffer data doesn't show on the direct screen

                LARGE_INTEGER EndCounter;
                //In 1us(1/1000000)
                QueryPerformanceCounter(&EndCounter);

                // Frame(cycles)
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

                // Number of ticks/frame
                uint64 CyclesElapsed = EndCycleCounts - LastCycleCounts;
                //NOTE: It based on the var type to decide what kind of the substraction to do

                // Time elapsed of one cycle/frame in 1/000000 s
                real32 ElapsedCounter = (real32)((real32)(EndCounter.QuadPart) - (real32)(LastCounter.QuadPart));
                real32 McPerFrame = (real32)((real32)CyclesElapsed/(1000.f * 1000.f));
                // Time elapsed of one cycle/frame in second
                //real32 MsPerFrame = (real32)((1000 * (real32)ElapsedCounter) / (real32)PerfCountFrequency);
                real32 MsPerFrame = (real32)((1000  * (real32)ElapsedCounter) / (real32)CyclesElapsed);
                real32 FPS = (real32)((real32)PerfCountFrequency/(real32)ElapsedCounter);

#if 0                
                char Buffer[256];
                //NOTE: The '%' is to decide the format of the next thing to print
                 for example: %d is the 32 bit integer
                sprintf(Buffer, "%f Miliseconds/Frame, %f FPS, %f Mc/f \n ", MsPerFrame, FPS, MsPerFrame);
                OutputDebugStringA(Buffer);
 #endif                
                glm::vec3 randomRotateAxis = glm::vec3(0.4f*(float)(std::rand()*2),0.4f*(float)(std::rand()*2),0.4f*(float)(std::rand()*2));

                int ChosenAxis = 0;

                    if(BackBuffer.camera.moved){
                        UpdateCamera(&BackBuffer.camera);
                        setMat4(ScreenBuffer.glData.ProgramID, "view", BackBuffer.camera.view);
                        BackBuffer.camera.moved = false;
                    }
                    
                if(WaitTimeCounter >= 16.67f){
                    //else {
                        //ViewRotateCount++;
                        //float CamX = sin(ViewRotateCount)*10.0f;
                        //float CamZ = cos(ViewRotateCount)*10.0f;
                        //BackBuffer.camera.view = glm::lookAt(glm::vec3(CamX, 0.0f, CamZ), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                        //setMat4(ScreenBuffer.glData.ProgramID, "view", BackBuffer.camera.view);
                    //}


                    if(ChangeAxisCounter >= 1000.0f){
                        ChangeAxisCounter = 0.0f;
                        ChosenAxis = std::rand()*2;
                        switch(ChosenAxis){
                            case 0:
                                randomRotateAxis = glm::vec3(0.4f*(float)(std::rand()*2), 0, 0);
                                break;
                            case 1:
                                randomRotateAxis = glm::vec3(0, 0.4f*(float)(std::rand()*2), 0);
                                break;
                            case 2:
                                randomRotateAxis = glm::vec3(0, 0, 0.4f*(float)(std::rand()*2));
                                break;
                            default:
                                randomRotateAxis = glm::vec3(0.4f*(float)(std::rand()*2),0.4f*(float)(std::rand()*2), 0.4f*(float)(std::rand()*2));
                                break;
                        };

                    } else {
                        ChangeAxisCounter += WaitTimeCounter;
                    }

                    Model = glm::rotate(Model, glm::radians(20.0f), randomRotateAxis);

                    // Wait to 17 milli s perframe for model to rotate
                    WaitTimeCounter = 0.0f;
                } else {
                    WaitTimeCounter += MsPerFrame;
                    //printf("WaitTimeCounter: %f\n", WaitTimeCounter);
                }
                
                LastCounter = EndCounter;
                LastCycleCounts = EndCycleCounts;
                        
                glBindVertexArray(ScreenBuffer.glData.VAOs);

                setMat4(ScreenBuffer.glData.ProgramID, "model", Model2);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                setMat4(ScreenBuffer.glData.ProgramID, "model", Model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
/*
                MULPD -> real32 ==> 128 bits / 32 bits -> 4 real32 packs per register 
                MULPS -> real64 ==> 128 bits / 64 bits -> 2 real32 packs per register  
*/
                Game_Input* Temp = NewInput;
                NewInput = OldInput;  //???? still don't understand
                OldInput = Temp;
                SwapBuffers(DeviceContext);
                ReleaseDC(Window, DeviceContext);
            }
            }                
            glDeleteVertexArrays(1, &BackBuffer.glData.VAOs);
            glDeleteBuffers(1, &BackBuffer.glData.VBO);
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
    wglDeleteContext(BackBuffer.glData.openglRC);
    return (0);
}
