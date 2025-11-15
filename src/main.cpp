/*======================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
  ================================================================================*/
#include <ctime>
#include <cstdlib>
#include "SoundMaker.h"
#include "Tile.h"

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
                if(vkCode == 'W') {
                    //Actually the front vec is at the back of the camera
                    //State.BlueOffset+= 10;
                    BackBuffer.camera.Position +=  glm::normalize(BackBuffer.camera.Direction) * BackBuffer.camera.speed;
                    printf("Up is HIT\n");
                }

                else if(vkCode == 'S') {
                    State.GreenOffset+= 10;
                    BackBuffer.camera.Position -= glm::normalize(BackBuffer.camera.Direction) * BackBuffer.camera.speed;
                    printf("Down is HIT\n");
                }

                else if(vkCode == 'A') {
                    //XOffset -= 10;
                    OutputDebugStringA("Left Button :");
                    //if(WasDown) {                    
                    // Not Camera front and up
                        BackBuffer.camera.Position -=  glm::normalize(glm::cross(BackBuffer.camera.Direction, BackBuffer.camera.Up)) * BackBuffer.camera.speed;
                    //OutputDebugStringA(" Was Down");
                    //}
                    printf("LEFT is HIT\n");
                }

                else if(vkCode == 'D') {
                    BackBuffer.camera.Position += glm::normalize(glm::cross(BackBuffer.camera.Direction, BackBuffer.camera.Up)) * BackBuffer.camera.speed;
                    printf("Right is HIT\n");
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

                else if(vkCode == VK_BACK) {
                    BackBuffer.camera.Direction = glm::vec3(-4.0f, 4.0f, 0.0f) - BackBuffer.camera.Position;

                    BackBuffer.camera.mouse.LastX = BackBuffer.camera.mouse.xPos;
                    BackBuffer.camera.mouse.LastY = BackBuffer.camera.mouse.yPos;
                    BackBuffer.camera.mouse.MouseXOffset = 0;
                    BackBuffer.camera.mouse.MouseYOffset = 0;

                    printf("Direction X is %f\n", BackBuffer.camera.Direction.x);
                    printf("Direction Y is %f\n", BackBuffer.camera.Direction.y);
                    
                    BackBuffer.camera.Yaw = glm::degrees(glm::acos(glm::clamp(BackBuffer.camera.Direction.x, -1.0f, 1.0f)));
                    BackBuffer.camera.Pitch = glm::degrees(glm::acos(glm::clamp(BackBuffer.camera.Direction.y, -1.0f, 1.0f)));
                    printf("Yaw is %f\n", BackBuffer.camera.Yaw);
                    printf("Pitch is %f\n", BackBuffer.camera.Pitch);

                    if(BackBuffer.camera.Yaw > 360.0f){
                        BackBuffer.camera.Yaw -= 360.0f;
                    }

                    if(BackBuffer.camera.Pitch > 90.0f){
                        BackBuffer.camera.Yaw -= 90.0f;
                    }
                    
                    printf("Back to point at the backpack\n");

                    std::cout<<"Direction is: "<< glm::to_string(BackBuffer.camera.Direction)<<std::endl;
                    //XOffset += 10;                    
                }
                
                }

                if(!BackBuffer.camera.moved){
                    //std::cout<<"Camera Position is"<<glm::to_string(BackBuffer.camera.Position)<<std::endl;
                    //std::cout<<"Camera Direction is"<<glm::to_string(BackBuffer.camera.Direction)<<std::endl;
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
        }break;

        case WM_MOUSEWHEEL:
        {
            float wheelPos = GET_WHEEL_DELTA_WPARAM(Wparam)*0.1f;
            //printf("Wheel delta: %d\n", (GET_WHEEL_DELTA_WPARAM(Wparam)));
            BackBuffer.camera.fov += wheelPos * BackBuffer.camera.speed;

            if(BackBuffer.camera.fov < 1.0f){
                BackBuffer.camera.fov = 1.0f;
            }

            if(BackBuffer.camera.fov > 45.0f){
                BackBuffer.camera.fov = 45.0f;
            }
            printf("Mouse Wheel is rolling, Wheel: %f, fov: %f\n", wheelPos, BackBuffer.camera.fov);

            if(!BackBuffer.camera.mouse.Wheeled){
                BackBuffer.camera.mouse.Wheeled = true;
            }

        }break;

        case WM_MOUSELEAVE:
        {
            if(BackBuffer.camera.mouse.LastX != BackBuffer.BitmapWidth/2){
                BackBuffer.camera.mouse.LastX = BackBuffer.BitmapWidth/2;
            }

            if(BackBuffer.camera.mouse.LastY != BackBuffer.BitmapHeight/2){
                BackBuffer.camera.mouse.LastY = BackBuffer.BitmapHeight/2;
            }
            
                //printf("Mouse Pos X: %d, Y: %d\n", BackBuffer.camera.mouse.LastX, BackBuffer.camera.mouse.LastY);
                if(BackBuffer.camera.mouse.moved){
                    //std::cout<<"Camera Position is"<<glm::to_string(BackBuffer.camera.Position)<<std::endl;
                    //std::cout<<"Camera Direction is"<<glm::to_string(BackBuffer.camera.Direction)<<std::endl;
                    BackBuffer.camera.mouse.moved = false;
                }
            
        }break;
        
        case WM_LBUTTONDOWN:
        {
            uint32 vkCode = Wparam;
                //if(vkCode == VK_LBUTTON) {
                    if(!BackBuffer.camera.focusCenter){
                        BackBuffer.camera.focusCenter = true;
                    }
                    printf("Mouse LButton is HIT\n");
                //}
                //return 0;
        }break;

        case WM_LBUTTONUP:
        {
            //uint32 vkCode = Wparam;
            //if(vkCode == VK_LBUTTON) {
                if(BackBuffer.camera.focusCenter){
                    BackBuffer.camera.focusCenter = false;
                }
                printf("Mouse LButton is released\n");
            //}
            //return 0;
        }break;

        case WM_MOUSEHOVER:{
            if(TrackMouseEvent(BackBuffer.camera.mouse.mouseEvent)){
                printf("Mouse event is being tracked\n");
            } else {
                printf("Can not track Mouse event\n");                
            };
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

            BackBuffer.camera.mouse.xPos =  GET_X_LPARAM(Lparam); 
//
            //if(BackBuffer.camera.mouse.xPos > BackBuffer.BitmapWidth){
                //BackBuffer.camera.mouse.xPos = BackBuffer.BitmapWidth;
            //}
//
            //if(BackBuffer.camera.mouse.xPos < 0){
                //BackBuffer.camera.mouse.xPos = 0;
            //}
//
            BackBuffer.camera.mouse.yPos = GET_Y_LPARAM(Lparam); 
//
            //if(BackBuffer.camera.mouse.yPos > BackBuffer.BitmapHeight){
                //BackBuffer.camera.mouse.yPos = BackBuffer.BitmapHeight;
            //}
//
            //if(BackBuffer.camera.mouse.yPos < 0){
                //BackBuffer.camera.mouse.yPos = 0;
            //}
            //
            //printf("Mouse x pos: %d\n", BackBuffer.camera.mouse.xPos);
            //printf("Mouse y pos: %d\n", BackBuffer.camera.mouse.yPos);

            if(!BackBuffer.camera.mouse.moved){
                BackBuffer.camera.mouse.moved = true;
            }
            //return 0L;             

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
            int displayCount = ShowCursor(true);
            printf("display count: %d\n", displayCount);
            
            if(!GlobalRunning){
                GlobalRunning = true; 
            }

            HDC DeviceContext = GetDC(Window);
            int refreshRate = GetDeviceCaps(DeviceContext, VREFRESH);
            ReleaseDC(Window, DeviceContext);

            MSG message;
            
            if(refreshRate > 1){
                printf("Refresh rate is : %dHz\n", refreshRate);
            };

#if INTERNAL
            LPVOID BaseAddress = megabytes(5);
#else
            LPVOID BaseAddress = 0;
#endif

            //=======================================================
            std::srand(std::time(NULL));
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

                // Randomize cube direction
                std::srand(std::time(0));
                float direction = 0.0f;
                for (int x = 0 ; x < 100; x++){
                    direction = ((std::rand()%3)*1.0f);
                    fluxY[x+100] = direction;
                    printf("cube index %d Y: %f, with direction %f %s\n", x, fluxY[x],fluxY[x+100],fluxY[x+100]==UPP_?"UP":fluxY[x+100]==DOWNN_?"DOWNN":"ROLL");
                }
                
// Cause the ScreenData will be deleted out of the loop so
                // We have to assign address of memory and glData to
                //InitOpenGL(Window, &BackBuffer, &ScreenBuffer, JPGContent);
                RenderSplendidGradient(&BackBuffer, &ScreenBuffer, BMPContent, 0, 0, 4);
                InitOpenGL(Window, &BackBuffer, &ScreenBuffer, BMPContent);

                B_shader vshader;
                B_shader fshader;

                loadShader(&vshader, "shader.vs", (VertexType)vertex_);
                loadShader(&fshader, "shader.fs", (VertexType)fragment_);

                setupGLprogram(&vshader, &fshader, &ScreenBuffer.glData.ProgramIDs[0]);

                B_shader Mvshader;
                B_shader Mfshader;

                loadShader(&Mvshader, "1.model.vs", (VertexType)vertex_);
                loadShader(&Mfshader, "1.model.fs", (VertexType)fragment_);

                setupGLprogram(&Mvshader, &Mfshader, &ScreenBuffer.glData.ProgramIDs[1]);

                copyBufferData(&BackBuffer, &ScreenBuffer);
                //????
                //glm::mat4 View = glm::mat4(1.0f);
                float UpdatedDegree = 0.0f;

                glm::mat4 Model = glm::mat4(1.0f);
                glm::mat4 Model2 = glm::mat4(1.0f);
                glm::mat4 Plane = glm::mat4(1.0f);
                Plane = glm::translate(Plane, glm::vec3(0.0f));

                glm::mat4 Projection = glm::mat4(1.0f);

                //View = glm::translate(View, glm::vec3(0.0f, 0.0f, -0.3f));
                glm::vec3 Position = glm::vec3(2.0f, -8.0f, 0.0f);
                glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
                glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

                glm::vec3 Right =  glm::vec3(1.0f, 0.0f, 0.0f);
                glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

                //set camera view here
                //std::cout<<"View matrix from camera: "<<glm::to_string(BackBuffer.camera.view)<<std::endl;
                BackBuffer.camera = Camera(BackBuffer.BitmapWidth, BackBuffer.BitmapHeight, Position, Front, Right, Up);

                TRACKMOUSEEVENT mouseEventVar = {};
                mouseEventVar.cbSize = sizeof(TRACKMOUSEEVENT);
                mouseEventVar.dwFlags = TME_HOVER|TME_LEAVE;
                mouseEventVar.hwndTrack = Window;
                mouseEventVar.dwHoverTime = 1000;

                BackBuffer.camera.mouse.mouseEvent = &mouseEventVar;

                //if(BackBuffer.camera.mouse.mouseEvent == NULL){
                    //printf("Can't initialize mouse Event, hoverTime\n");
                //} else {
                    //printf("Succeed initialize mouse Event\n");                    
                //}
                
                // This will be replaced by camera.view matrix
                std::cout<<"View matrix: "<<glm::to_string(BackBuffer.camera.view)<<std::endl;
                std::cout<<"Front vec: "<<glm::to_string(BackBuffer.camera.Front)<<std::endl;
                std::cout<<"Right vec: "<<glm::to_string(BackBuffer.camera.Right)<<std::endl;
                std::cout<<"Up vec: "<<glm::to_string(BackBuffer.camera.Up)<<std::endl;

                BackBuffer.camera.fov = 45.0f;
                Model = glm::translate(Model, glm::vec3(2.0f, -4.0f, 0.0f));
                std::cout<<"Central rotating model is"<<glm::to_string(Model)<<std::endl;
                Model2 = glm::translate(Model2, glm::vec3(-4.0f, 4.0f, 0.0f));
                std::cout<<"Stand still model 2 matrix is :"<<glm::to_string(Model2)<<std::endl;

                BackBuffer.camera.projection = glm::perspective(glm::radians(BackBuffer.camera.fov), (float)ScreenBuffer.BitmapWidth / (float)ScreenBuffer.BitmapHeight, 0.1f, 100.0f);
                
                if(glIsProgram(ScreenBuffer.glData.ProgramIDs[0])){
                    useProgram(ScreenBuffer.glData.ProgramIDs[0]);
                    printf("Program ID: %d\n", ScreenBuffer.glData.ProgramIDs[0]);
                } else {
                    glDebugMessageCallback(MessageCallback, 0);
                    checkCompileErrors(vshader.shaderID, "Vertex");
                    checkCompileErrors(fshader.shaderID, "Fragment");
                    checkCompileErrors(ScreenBuffer.glData.ProgramIDs[0], "Program");
                    printf("NO program object created before\n");
                }

                if(glIsProgram(ScreenBuffer.glData.ProgramIDs[1])){
                    useProgram(ScreenBuffer.glData.ProgramIDs[1]);
                    printf("Program ID: %d\n", ScreenBuffer.glData.ProgramIDs[1]);
                } else {
                    glDebugMessageCallback(MessageCallback, 0);
                    checkCompileErrors(vshader.shaderID, "Vertex");
                    checkCompileErrors(fshader.shaderID, "Fragment");
                    checkCompileErrors(ScreenBuffer.glData.ProgramIDs[1], "Program");
                    printf("NO program object created before\n");
                }

                //setInt(ScreenBuffer.glData.ProgramIDs[0], "ttexture1", ScreenBuffer.glData.textureHandle);                

                useProgram(ScreenBuffer.glData.ProgramIDs[0]);
                setMat4(ScreenBuffer.glData.ProgramIDs[0], "projection", BackBuffer.camera.projection);
                std::cout<<"Projection mat: "<<glm::to_string(BackBuffer.camera.projection)<<std::endl;                
                setMat4(ScreenBuffer.glData.ProgramIDs[0], "view", BackBuffer.camera.view);

                useProgram(ScreenBuffer.glData.ProgramIDs[1]);
                setMat4(ScreenBuffer.glData.ProgramIDs[1], "projection", BackBuffer.camera.projection);
                setMat4(ScreenBuffer.glData.ProgramIDs[1], "view", BackBuffer.camera.view);
                setVec3(ScreenBuffer.glData.ProgramIDs[1], "ViewPos", BackBuffer.camera.Position);
                setVec3(ScreenBuffer.glData.ProgramIDs[1], "lightPos", glm::vec3(4.0f, 3.0f, 0.0f));

                Model_* modell = nullptr;
                modell = new Model_();
                //std::string path = "C:/Users/klove/Documents/repos/GLFW2/Vulkan_Learning_Project/build/backpack.obj";
                std::string path = "C:/Users/klove/Documents/repos/GLFW2/Vulkan_Learning_Project/build/source/stylised_terrain_tile_1011124259_texture_fbx/stylised_terrain_tile_1011124259_texture.fbx";
                loadModel_(modell, path);
                
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

                bool RatioCalculated = false;
                float DelayedRatio = 0.0f;                
                //Window = SetCapture(Window);

                while(GlobalRunning) {
                MSG Message;
                //NOTE: This is where receiving the message to change
                // for any change in window
                //
                //INPUT
                while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE)) {
                    if(Message.message == WM_QUIT){
                        if(GlobalRunning){
                            GlobalRunning = false;
                        }
                    }
                    DispatchMessage(&Message);
                    TranslateMessage(&Message);
                }
                if( MaxControllerCount > ArrayCount(Input->Controller)) {
                    MaxControllerCount = ArrayCount(Input->Controller);   
                }
                TrackMouseEvent(BackBuffer.camera.mouse.mouseEvent);
                //if(TrackMouseEvent(BackBuffer.camera.mouse.mouseEvent)){
                    //printf("Mouse event is being tracked\n");
                //} else {
                    //printf("Can not track Mouse event\n");                
                //};                

                //UPDATE
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

                int ChosenAxis = 0;

                if(!RatioCalculated){
                    printf("Ms per frame :%f \n", MsPerFrame);
                    DelayedRatio = MsPerFrame/16.67f;
                    printf("Delay Ratio: %f\n", DelayedRatio);
                    BackBuffer.camera.speed = (1.5f * DelayedRatio);                
                    printf("camera speed: %f\n", BackBuffer.camera.speed);
                    RatioCalculated = true;
                }

                //if(BackBuffer.camera.moved || BackBuffer.camera.mouse.moved){
                        UpdateCamera(&BackBuffer.camera, DelayedRatio);                    
                    //}
                        if(BackBuffer.camera.mouse.Wheeled)
                        {
                            BackBuffer.camera.projection = glm::perspective(glm::radians(BackBuffer.camera.fov), (float)ScreenBuffer.BitmapWidth / (float)ScreenBuffer.BitmapHeight, 0.1f, 100.0f);
                            glUseProgram(ScreenBuffer.glData.ProgramIDs[0]);
                            setMat4(ScreenBuffer.glData.ProgramIDs[0], "projection", BackBuffer.camera.projection);
                            glUseProgram(ScreenBuffer.glData.ProgramIDs[1]);
                            setMat4(ScreenBuffer.glData.ProgramIDs[1], "projection", BackBuffer.camera.projection);
                            BackBuffer.camera.mouse.Wheeled = false;
                        }

                        useProgram(ScreenBuffer.glData.ProgramIDs[0]);
                        setMat4(ScreenBuffer.glData.ProgramIDs[0], "view", BackBuffer.camera.view);

                        useProgram(ScreenBuffer.glData.ProgramIDs[1]);
                        setMat4(ScreenBuffer.glData.ProgramIDs[1], "view", BackBuffer.camera.view);
                        
                glm::vec3 randomRotateAxis = glm::vec3(0.4f* DelayedRatio*(float)(std::rand()*2),0.4f*DelayedRatio*(float)(std::rand()*2),0.4f*DelayedRatio*(float)(std::rand()*2));

                //setMat4(ScreenBuffer.glData.ProgramIDs[0], "model", Model);
                //glBindVertexArray(ScreenBuffer.glData.VAOs);
                //glDrawArrays(GL_TRIANGLES, 0, 36);
                
                if(WaitTimeCounter >= 16.67f){
                    //else {
                        //ViewRotateCount++;
                        //float CamX = sin(ViewRotateCount)*10.0f;
                        //float CamZ = cos(ViewRotateCount)*10.0f;
                        //BackBuffer.camera.view = glm::lookAt(glm::vec3(CamX, 0.0f, CamZ), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                        //setMat4(ScreenBuffer.glData.ProgramID, "view", BackBuffer.camera.view);
                    //}
                    //Set vectices and color for plane
                    UpdatedDegree += 5.0f;
                    //printf("updated angle :%f\n", UpdatedDegree);
                    if(UpdatedDegree*(float)BackBuffer.camera.speed > 360.0f){
                        UpdatedDegree -= 360.0f/(float)BackBuffer.camera.speed;
                    };

                    if(ChangeAxisCounter >= 1000.0f){
                        ChosenAxis = std::rand()*2;
                        switch(ChosenAxis){
                            case 0:
                                randomRotateAxis = glm::vec3(0.4f*(float)(std::rand()*2), 0, 0);
                                break;
                            case 1:
                                randomRotateAxis = glm::vec3(0, 0.4f*(float)(std::rand()*2), 0);
                                break;
                            case 2:
                                randomRotateAxis = glm::vec3(0, 0, 0.4f* (float)(std::rand()*2));
                                break;
                            default:
                                randomRotateAxis = glm::vec3(0.4f*(float)(std::rand()*2),0.4f*(float)(std::rand()*2), 0.4f*(float)(std::rand()*2));
                                break;
                        };
                        ChangeAxisCounter = 0.0f;
                        //printf("ChangeAxisCounter: %f\n", WaitTimeCounter);            
                    } else {
                        ChangeAxisCounter += WaitTimeCounter;
                    }
                    Model = glm::rotate(Model, glm::radians(10.0f) * (float)BackBuffer.camera.speed, randomRotateAxis);
                    // Wait to 17 milli s perframe for model to rotate
                    WaitTimeCounter = 0.0f;
                } else {
                    WaitTimeCounter += MsPerFrame;
                    //printf("WaitTimeCounter: %f\n", WaitTimeCounter);
                }

                //RENDER =====================================
                useProgram(ScreenBuffer.glData.ProgramIDs[0]);
                glBindVertexArray(ScreenBuffer.glData.PlaneVAOs);
                setMat4(ScreenBuffer.glData.ProgramIDs[0], "model", Plane);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                setMat4(ScreenBuffer.glData.ProgramIDs[0], "model", Model);
                glBindVertexArray(ScreenBuffer.glData.VAOs);
                glDrawArrays(GL_TRIANGLES, 0, 36);                

                drawTile(ScreenBuffer.glData.VAOs, ScreenBuffer.glData.ProgramIDs[0], BackBuffer.camera.speed, &UpdatedDegree);

                
                useProgram(ScreenBuffer.glData.ProgramIDs[1]);
                glBindVertexArray(ScreenBuffer.glData.VAOs);
                setMat4(ScreenBuffer.glData.ProgramIDs[1], "model", Model2);
                DDraw(modell, &ScreenBuffer.glData.ProgramIDs[1]);
                
                LastCounter = EndCounter;
                LastCycleCounts = EndCycleCounts;
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
