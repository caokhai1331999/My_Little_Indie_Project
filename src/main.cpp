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
//#include "handmade.h"
#include "animator.h"

bool32 first_size = true;
bool32 first_announce = true;
bool32 Load_Lib = false;
bool32 showMsPF = false;

LRESULT CALLBACK MainWindowCallBack(HWND Window, UINT Message, WPARAM Wparam,
                                    LPARAM Lparam) {
  LRESULT result;
  bool fDraw = false;
  POINT ptPrevious = {};
  switch (Message) {

  case WM_CREATE: {
    printf("On Window creating stage\n");
  } break;
  // What is DeviceContext for in this case??
  // NOTE: Whenever the window is resized, this function capture the size
  // of the new window and update a new proper DIB for that
  // DIB is a table where store BIT color infor
  case WM_SIZE: {
    if (first_size) {
      first_size = false;
    } else {
      GetWindowDimension(Window);
      Win32ResizeDIBSection(&BackBuffer, Dimens.Width, Dimens.Height);
      if (!BackBuffer.transferNeed) {
        BackBuffer.transferNeed = true;
      }
      glViewport(0, 0, BackBuffer.BitmapWidth, BackBuffer.BitmapHeight);
      OutputDebugStringA("WM_SIZE\n");
    }
  } break;

  case WM_CLOSE: {
    GlobalRunning = false;
    OutputDebugStringA("WM_CLOSE\n");
  } break;

  case WM_KEYDOWN: {
    bool IsDown = ((Lparam & (1 << 31)) == 0);
    bool WasDown = ((Lparam & (1 << 30)) != 0);

    uint32 vkCode = Wparam;
    if (IsDown) {
      if (vkCode == 'W') {
        // Actually the front vec is at the back of the camera
        // State.BlueOffset+= 10;
        BackBuffer.camera.Position +=
            glm::normalize(BackBuffer.camera.Direction) *
            (float)BackBuffer.camera.speed;
        if (!WasDown) {
            printf("Up is HIT\n");
        }
      }

      else if (vkCode == 'S') {
        State.GreenOffset += 10;
        BackBuffer.camera.Position -=
            glm::normalize(BackBuffer.camera.Direction) *
            (float)BackBuffer.camera.speed;
        if (!WasDown) {
            printf("Down is HIT\n");
        }
      }

      else if (vkCode == 'A') {
        // XOffset -= 10;
        OutputDebugStringA("Left Button :");
        // if(WasDown) {
        //  Not Camera front and up
        BackBuffer.camera.Position -=
            glm::normalize(
                glm::cross(BackBuffer.camera.Direction, BackBuffer.camera.Up)) *
            (float)BackBuffer.camera.speed;
        // OutputDebugStringA(" Was Down");
        // }
        if (!WasDown) {
            printf("LEFT is HIT\n");
        }
      }

      else if (vkCode == 'L') {
        // XOffset -= 10;
        OutputDebugStringA("L Button :");
        // if(WasDown) {
        if (!Load_Lib) {
            Load_Lib = true;
        }
        // OutputDebugStringA(" Was Down");
        // }
        if (!WasDown) {
            printf("L is HIT\n");
        }
      }

      else if (vkCode == 'D') {
        BackBuffer.camera.Position +=
            glm::normalize(
                glm::cross(BackBuffer.camera.Direction, BackBuffer.camera.Up)) *
            (float)BackBuffer.camera.speed;
        if (!WasDown) {
            printf("Right is HIT\n");
        }
        // XOffset += 10;
      }

      else if (vkCode == VK_SPACE) {
        BackBuffer.camera.Position +=
            BackBuffer.camera.Up * (float)BackBuffer.camera.speed;
        if (!WasDown) {
            printf("Space is HIT\n");
        }
        // XOffset += 10;
      }

      else if (vkCode == VK_SHIFT) {
        BackBuffer.camera.Position -=
            BackBuffer.camera.Up * (float)BackBuffer.camera.speed;
        if (!WasDown) {
            printf("Left Shift is HIT\n");
        }
        // XOffset += 10;
      }

      else if (vkCode == VK_CONTROL) {

        if (!showMsPF) {
            showMsPF = true;
        }
        printf("Right Shift is HIT\n");
        // XOffset += 10;
      }

      else if (vkCode == VK_BACK) {
       BackBuffer.camera.Direction =
            glm::vec3(-4.0f, 4.0f, 0.0f) - BackBuffer.camera.Position;

        BackBuffer.camera.mouse.LastX = BackBuffer.camera.mouse.xPos;
        BackBuffer.camera.mouse.LastY = BackBuffer.camera.mouse.yPos;
        BackBuffer.camera.mouse.MouseXOffset = 0;
        BackBuffer.camera.mouse.MouseYOffset = 0;

        printf("Direction X is %f\n", BackBuffer.camera.Direction.x);
        printf("Direction Y is %f\n", BackBuffer.camera.Direction.y);

        BackBuffer.camera.Yaw = glm::degrees(
            glm::acos(glm::clamp(BackBuffer.camera.Direction.x, -1.0f, 1.0f)));
        BackBuffer.camera.Pitch = glm::degrees(
            glm::acos(glm::clamp(BackBuffer.camera.Direction.y, -1.0f, 1.0f)));

        printf("Yaw is %f\n", BackBuffer.camera.Yaw);
        printf("Pitch is %f\n", BackBuffer.camera.Pitch);

        if (BackBuffer.camera.Yaw > 360.0f) {
          BackBuffer.camera.Yaw -= 360.0f;
        }

        if (BackBuffer.camera.Pitch > 90.0f) {
          BackBuffer.camera.Yaw -= 90.0f;
        }

        printf("Back to point at the backpack\n");

        std::cout << "Direction is: "
                  << glm::to_string(BackBuffer.camera.Direction) << std::endl;

        WINDOWPLACEMENT windowstatus = {};
        windowstatus.length = sizeof(WINDOWPLACEMENT);

        if (GetWindowPlacement(Window, &windowstatus)) {
          printf("Window position(x, y) is: %d %d\n",
                 windowstatus.rcNormalPosition.left,
                 windowstatus.rcNormalPosition.top);
        } else {
          printf("Failed to get window status\n");
        }

        SetCursorPos(
            BackBuffer.camera.mouse.xPos + windowstatus.rcNormalPosition.left,
            BackBuffer.camera.mouse.yPos + windowstatus.rcNormalPosition.top);
        // XOffset += 10;
      }
    }

    if (!BackBuffer.camera.moved) {
      // std::cout<<"Camera Position
      // is"<<glm::to_string(BackBuffer.camera.Position)<<std::endl;
      // std::cout<<"Camera Direction
      // is"<<glm::to_string(BackBuffer.camera.Direction)<<std::endl;
      BackBuffer.camera.moved = true;
    }

    // if(vkCode == VK_LEFT) {
    //
    // OutputDebugStringA("Left Button :");
    // if(IsDown) {
    // OutputDebugStringA(" Is Down");
    //
    // }
    // OutputDebugStringA("\n");
    // }

    if (vkCode == VK_ESCAPE) {
      if (GlobalRunning) {
        GlobalRunning = false;
      }
    }
  } break;

  case WM_MOUSEWHEEL: {
    float wheelPos = GET_WHEEL_DELTA_WPARAM(Wparam) * 0.1f;
    // printf("Wheel delta: %d\n", (GET_WHEEL_DELTA_WPARAM(Wparam)));
    BackBuffer.camera.fov += wheelPos * BackBuffer.camera.speed;

    if (BackBuffer.camera.fov < 1.0f) {
      BackBuffer.camera.fov = 1.0f;
    }

    if (BackBuffer.camera.fov > 45.0f) {
      BackBuffer.camera.fov = 45.0f;
    }
    printf("Mouse Wheel is rolling, Wheel: %f, fov: %f\n", wheelPos,
           BackBuffer.camera.fov);

    if (!BackBuffer.camera.mouse.Wheeled) {
      BackBuffer.camera.mouse.Wheeled = true;
    }

  } break;

  case WM_MOUSELEAVE: {
    if (BackBuffer.camera.mouse.LastX != BackBuffer.BitmapWidth / 2) {
      BackBuffer.camera.mouse.LastX = BackBuffer.BitmapWidth / 2;
    }

    if (BackBuffer.camera.mouse.LastY != BackBuffer.BitmapHeight / 2) {
      BackBuffer.camera.mouse.LastY = BackBuffer.BitmapHeight / 2;
    }

    // printf("Mouse Pos X: %d, Y: %d\n", BackBuffer.camera.mouse.LastX,
    // BackBuffer.camera.mouse.LastY);
    if (BackBuffer.camera.mouse.moved) {
      // std::cout<<"Camera Position
      // is"<<glm::to_string(BackBuffer.camera.Position)<<std::endl;
      // std::cout<<"Camera Direction
      // is"<<glm::to_string(BackBuffer.camera.Direction)<<std::endl;
      BackBuffer.camera.mouse.moved = false;
    }

  } break;

  case WM_LBUTTONDOWN: {
    uint32 vkCode = Wparam;
    // if(vkCode == VK_LBUTTON) {
    if (!BackBuffer.camera.focusCenter) {
      BackBuffer.camera.focusCenter = true;
    }
    printf("Mouse LButton is HIT\n");
    //}
    // return 0;
  } break;

  case WM_LBUTTONUP: {
    // uint32 vkCode = Wparam;
    // if(vkCode == VK_LBUTTON) {
    if (BackBuffer.camera.focusCenter) {
      BackBuffer.camera.focusCenter = false;
    }
    printf("Mouse LButton is released\n");
    //}
    // return 0;
  } break;

  case WM_MOUSEHOVER: {
    if (TrackMouseEvent(BackBuffer.camera.mouse.mouseEvent)) {
      printf("Mouse event is being tracked\n");
    } else {
      printf("Can not track Mouse event\n");
    };
  } break;

  case WM_MOUSEMOVE: {
    if (fDraw) {
      DeviceContext = GetDC(Window);
      MoveToEx(DeviceContext, ptPrevious.x, ptPrevious.y, NULL);
      LineTo(DeviceContext, ptPrevious.x = LOWORD(Lparam),
             ptPrevious.y = HIWORD(Lparam));
      ReleaseDC(Window, DeviceContext);
    }

    BackBuffer.camera.mouse.xPos = GET_X_LPARAM(Lparam);
    //
    // if(BackBuffer.camera.mouse.xPos > BackBuffer.BitmapWidth){
    // BackBuffer.camera.mouse.xPos = BackBuffer.BitmapWidth;
    //}
    //
    // if(BackBuffer.camera.mouse.xPos < 0){
    // BackBuffer.camera.mouse.xPos = 0;
    //}
    //
    BackBuffer.camera.mouse.yPos = GET_Y_LPARAM(Lparam);
    //
    // if(BackBuffer.camera.mouse.yPos > BackBuffer.BitmapHeight){
    // BackBuffer.camera.mouse.yPos = BackBuffer.BitmapHeight;
    //}
    //
    // if(BackBuffer.camera.mouse.yPos < 0){
    // BackBuffer.camera.mouse.yPos = 0;
    //}
    //
    // printf("Mouse x pos: %d\n", BackBuffer.camera.mouse.xPos);
    // printf("Mouse y pos: %d\n", BackBuffer.camera.mouse.yPos);

    if (!BackBuffer.camera.mouse.moved) {
      BackBuffer.camera.mouse.moved = true;
    }
    // return 0L;

  } break;

  case WM_SYSKEYDOWN: {
    uint32 vkCode = Wparam;
    bool AltkeyisDown = ((Lparam & (1 << 29)) != 0);
    if ((vkCode == VK_F4) && AltkeyisDown) {
      GlobalRunning = false;
    }
    OutputDebugStringA("WM_SYSKEYDOWN\n");
  } break;

  case WM_SYSKEYUP: {
    OutputDebugStringA("WM_SYSKEYUP\n");
  } break;

  case WM_KEYUP: {
    uint32 vkCode = Wparam;
    // NOTE: This is whether bit 30 or 0 (never 1).
    //  So if it is bit 30 it is down
    bool WasDown = ((Lparam & (1 << 30)) != 0);
    bool IsDown = ((Lparam & (1 << 31)) == 0);
    // if (WasDown != IsDown) {

    // else
    if (vkCode == VK_TAB) {
      if (SoundOutPut.hz == 128) {
        SoundOutPut.hz = 256;
      } else if (SoundOutPut.hz == 256) {
        SoundOutPut.hz = 512;
      } else {
        SoundOutPut.hz = 128;
      }
      char Output[256];
      sprintf(Output, "TAB button hitted, Current Hert is: %d\n",
              SoundOutPut.hz);
      SoundOutPut.WavePeriod = SoundOutPut.SamplePerSecond / SoundOutPut.hz;

      OutputDebugStringA("TAB button hitted");
    }

  } break;

  case WM_DESTROY: {
    GlobalRunning = false;
    PostQuitMessage(0);
    OutputDebugStringA("WM_DESTROY\n");
  } break;

  //case WM_PAINT: {
//
    //BeginPaint(Window, NULL);
    //HDC tempDC = GetDC(Window);
    // Start to save bit drawing data to the current HDC
    //RenderSplendidGradient(&BackBuffer, NULL, BMPContent, 0, 0, 4);
    //Win32DisplayBufferWindow(tempDC, Dimens.Width, Dimens.Height, &BackBuffer);

    //if (glGetError() != GL_NO_ERROR) {
      //printf("OpenGL Error: %d\n", glGetError());
    //};

    // glBindVertexArray(BackBuffer.glData.VAOs);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    //SwapBuffers(tempDC);
    //EndPaint(Window, NULL);
    //ReleaseDC(Window, tempDC);
//
    //OutputDebugStringA("WM_PAINT\n");
  //} break;

  default: {
    OutputDebugStringA("DEFAULT\n");
    result = DefWindowProcA(Window, Message, Wparam, Lparam);
  } break;
  }
  // return 0L;
  return result;
}

int CALLBACK WinMain
(HINSTANCE Instance,
 HINSTANCE hInstPrev,
 PSTR cmdline,
 int cmdshow)
{
// Ticks per second/microS
LARGE_INTEGER PerfCountFrequencyResult;
//We have one standard var here
  QueryPerformanceFrequency(&PerfCountFrequencyResult);
  // NOTE: Actually, this the counts per second
  // TODO: Try to find out why the PerfCountFrequency is too large.
  // NOTE: This one count is for counting the frame
  int64 PerfCountFrequency = (int64)(PerfCountFrequencyResult.QuadPart);
  // This one is how many count per Second

  LARGE_INTEGER LastCounter = {};
  LARGE_INTEGER EndCounter = {};

  uint64 LastCycleCounts = 0;
  uint64 EndCycleCounts = 0;

  float TimeCounter = 0.0f;
  float WaitTimeCounter = 0.0f;
  float ColorOffset = 0.0f;

  uint64 TicksPerFrame = 0;
  int64 TicksPerMicroS = 0;
  int64 CountsPerFrame = 0;
  real64 MsPerFrame = 0.0f;
  int64 SPerFrame = 0;
  int64 FPS = 0;
  // Time elapsed of one cycle/frame in second

  bool RatioCalculated = false;
  real64 DelayedRatio = 0.0f;
  real64 StandardMSperFrame = 16.67f;
  
  win32LoadXInput();
  WNDCLASSEXA WindowClass = {};
  WindowClass.cbSize = sizeof(WNDCLASSEXA);
  WindowClass.style = CS_HREDRAW|CS_VREDRAW;
  HWND Window = {};
  WindowClass.lpfnWndProc = MainWindowCallBack;
  WindowClass.hInstance = Instance;
  WindowClass.lpszClassName = "First Game Window Class";
  Win32ResizeDIBSection(&BackBuffer, Dimens.Height, Dimens.Width);
  // NOTE: I forgot to init window
  HMODULE AniLib;
  
  AniClassSpawner CreateAnimation = NULL;
  AniClassSlainer KillAninmation = NULL;

  AniUserClassSpawner SpawnAnimator = NULL;
  AniUserClassSlayer SlayAnimator = NULL;
  AniTimeUpdater AniUpdate = NULL;
  PlayAni__ PlayAnimation = NULL;
  //setupMeshh setupMesh = NULL;
  //MDraw Draw = NULL;
      //AniTimeUpdater AniUpdate_;

  Animation* danceAnimation = nullptr;
  Animator* animator = nullptr;
  int delay_count = 0;
  if (RegisterClassExA(&WindowClass)) {

    Window = CreateWindowExA(
        // NOTE: The window didn't show up is because the first argument
        WS_EX_APPWINDOW, WindowClass.lpszClassName, "win32GameWithoutEngine",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, Instance, 0);

    if (Window) {

      OpenConsole();
      int displayCount = ShowCursor(true);
      printf("display count: %d\n", displayCount);
      printf("counter per 1s: %I64d\n",  PerfCountFrequency);

      if (!GlobalRunning) {
        GlobalRunning = true;
      }

      HDC DeviceContext = GetDC(Window);
      int refreshRate = GetDeviceCaps(DeviceContext, VREFRESH);
      ReleaseDC(Window, DeviceContext);


      if (CopyFile("skeletalAni32.dll", "skeletalAni32_copy.dll",
                   false))
      {
          AniLib = LoadLibraryA("skeletalAni32_copy.dll");
      }
      //

      // Animation
      if(AniLib != NULL){
          CreateAnimation = (AniClassSpawner)GetProcAddress(AniLib, "CreateAniClass");
          KillAninmation = (AniClassSlainer)GetProcAddress(AniLib, "DestroysAniClass");

          SpawnAnimator = (AniUserClassSpawner)GetProcAddress(AniLib, "CreateAnimatorClass");
          SlayAnimator = (AniUserClassSlayer)GetProcAddress(AniLib, "DestroyAnimatorClass");
          AniUpdate = (AniTimeUpdater)GetProcAddress(AniLib, "updateAnimationTime_");
          PlayAnimation = (PlayAni__)GetProcAddress(AniLib, "PlayAni_");
          //setupMesh = (setupMeshh)GetProcAddress(AniLib, "setupMesh");
          //Draw = (MDraw)GetProcAddress(AniLib, "Draw");
          //Load_Lib = false;
      }

      
      MSG message;

      if (refreshRate > 1) {
        printf("Refresh rate is : %dHz\n", refreshRate);
      };

#ifdef INTERNAL
      LPVOID BaseAddress = (LPVOID)Megabytes(5);
#else
      LPVOID BaseAddress = 0;
#endif

            //=======================================================
            std::srand(std::time(NULL));

            glm::vec3 randomRotateAxis = glm::vec3(0.4f*(float)(std::rand()*2),0.4f*(float)(std::rand()*2), 0.4f*(float)(std::rand()*2));

            Game_Memory game_memory = {};
            game_memory.PermanentStorageSize = Megabytes(128);
            game_memory.TransientStorageSize = Megabytes((uint64)6);

            uint64 TotalSize = game_memory.PermanentStorageSize + game_memory.TransientStorageSize;
            
            game_memory.PermanentStorage = VirtualAlloc(BaseAddress , TotalSize,  MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
            game_memory.TransientStorage = ((uint8*)game_memory.PermanentStorage + game_memory.PermanentStorageSize);
            //=====================================================

          if(game_memory.TransientStorage && game_memory.PermanentStorage){
            debug_read_file_result result2;
            debug_read_file_result result;
            // BMPContent = new imagee_content;
            // BMPContent = DEBUGReadBMP("Harry_and_Accomplices_rescaled.bmp",
            // &result);
            //  =============================================
            //BMPContent = DEBUGReadBMP("adventure_.jpg", &result);
            // printf("About to read image\n");
            //  NOTE: ???? Why when I change to different bmp image it crashed
            // byte order: AA BB GG RR bottom up
            // JPGContent = DEBUGReadJPG("Harry and Accomplices.jpg", &result2);
            // OpenGL part
            Win32_Front_Buffer ScreenBuffer = Win32_Front_Buffer(
                BackBuffer.BitmapWidth, BackBuffer.BitmapHeight,
                &BackBuffer.glData, BackBuffer.BitmapMemory);

            // Randomize cube direction
            std::srand(std::time(0));

            std::vector<rollCubeInfo>rollCubeMap;
            float direction = 0.0f;

            for (int x = 0; x < 100; x++) {
              direction = ((std::rand() % 3) * 1.0f);
              fluxY[x + 100] = direction;
              if ((float)fluxY[x + 100] == (float)ROLL_) {
                  rollCubeMap.push_back({x, std::rand() % 3});
                }
              printf("cube index %d Y: %f, with direction %f %s\n", x, fluxY[x],
                     fluxY[x + 100],
                     fluxY[x + 100] == UPP_     ? "UP"
                     : fluxY[x + 100] == DOWNN_ ? "DOWNN"
                                                : "ROLL");
            }
                
// Cause the ScreenData will be deleted out of the loop so
                // We have to assign address of memory and glData to
                //InitOpenGL(Window, &BackBuffer, &ScreenBuffer, JPGContent);
                //RenderSplendidGradient(&BackBuffer, &ScreenBuffer, BMPContent, 0, 0, 4);
                InitOpenGL(Window, &BackBuffer, &ScreenBuffer, BMPContent);

                GLenum err = glGetError();
                if (err != GL_NO_ERROR) {
                    std::cerr << "OpenGL Error: " << err << std::endl;
                }

                glViewport(0, 0, BackBuffer.BitmapWidth, BackBuffer.BitmapHeight);
                // Basic shader
                std::string shader_name = "basic brush";
                B_shader_program* basic_shader_ = new B_shader_program("shader.vs", "shader.fs", shader_name.c_str());             ScreenBuffer.glData.ProgramIDs.push_back(basic_shader_->GetProgramID());

                //shader_name.clear();
                shader_name = "model drawing brush";
                //basic model shader
                B_shader_program* model_shader_ = new B_shader_program("1.model.vs", "1.model.fs", shader_name.c_str());                ScreenBuffer.glData.ProgramIDs.push_back(model_shader_->GetProgramID());

                //shader_name.clear();                
                shader_name = "animating sketching brush";
                B_shader_program* animating_shader_ = new B_shader_program("2.skeletal_animation.vs", "2.skeletal_animation.fs", shader_name.c_str());                ScreenBuffer.glData.ProgramIDs.push_back(animating_shader_->GetProgramID());
                
                copyBufferData(&BackBuffer, &ScreenBuffer);
                //????
                //glm::mat4 View = glm::mat4(1.0f);
                real32 UpdatedAngle = 0.0f;
                real32 updateDegreeInPi = 0.0f;
                float threshHold = glm::radians((float)360.0f);

                glm::mat4 basic_cube_core = glm::mat4(1.0f);
                glm::mat4 backpack_core = glm::mat4(1.0f);
                glm::mat4 dancing_vampire_core = glm::mat4(1.0f);

                glm::mat4 Plane = glm::mat4(1.0f);
                Plane = glm::translate(Plane, glm::vec3(0.0f));

                glm::mat4 Projection = glm::mat4(1.0f);

                //View = glm::translate(View, glm::vec3(0.0f, 0.0f, -0.3f));
                glm::vec3 Position = glm::vec3(2.0f, -8.0f, 0.0f);
                glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
                glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

                glm::vec3 Right =  glm::vec3(1.0f, 0.0f, 0.0f);
                glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

                // positions of the point lights
                glm::vec3 pointLightPositions[] = {
                    glm::vec3( 0.7f,  0.2f,  2.0f),
                    glm::vec3( 2.3f, -3.3f, -4.0f),
                    glm::vec3(-4.0f,  2.0f, -12.0f),
                    glm::vec3( 0.0f,  0.0f, -3.0f)
                };
                
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
                basic_cube_core = glm::translate(basic_cube_core, glm::vec3(2.0f, -4.0f, 0.0f));
                std::cout<<"Central rotating model is"<<glm::to_string(basic_cube_core)<<std::endl;
                backpack_core = glm::translate(backpack_core, glm::vec3(-4.0f, 4.0f, 0.0f));
                std::cout<<"Stand still model 2 matrix is :"<<glm::to_string(backpack_core)<<std::endl;

                // Set containing model for dancing vampire
                dancing_vampire_core = glm::translate(dancing_vampire_core, glm::vec3(0.0f, -0.4f, 0.0f));
                dancing_vampire_core = glm::scale(dancing_vampire_core, glm::vec3(0.5f,0.5f,0.5f));
                
                BackBuffer.camera.projection = glm::perspective(glm::radians(BackBuffer.camera.fov), (float)ScreenBuffer.BitmapWidth / (float)ScreenBuffer.BitmapHeight, 0.1f, 100.0f);
                
                if(glIsProgram(ScreenBuffer.glData.ProgramIDs[0])){
                    useProgram(ScreenBuffer.glData.ProgramIDs[0]);
                    printf("Program ID: %d\n", ScreenBuffer.glData.ProgramIDs[0]);
                } else {
                  glDebugMessageCallback(MessageCallback, 0);
                  checkCompileErrors(ScreenBuffer.glData.ProgramIDs[0], programme_, "basic brush");
                    printf("NO program object created before\n");
                }

                if(glIsProgram(ScreenBuffer.glData.ProgramIDs[1])){
                    model_shader_->use();
                    printf("Program ID: %d\n", ScreenBuffer.glData.ProgramIDs[1]);
                } else {
                  glDebugMessageCallback(MessageCallback, 0);
                    checkCompileErrors(ScreenBuffer.glData.ProgramIDs[1], programme_, "model drawing brush");
                    printf("model drawing program object is buggy\n");
                }

                if(glIsProgram(ScreenBuffer.glData.ProgramIDs[2])){
                    animating_shader_->use();
                    printf("Program ID: %d\n", ScreenBuffer.glData.ProgramIDs[2]);
                } else {
                    glDebugMessageCallback(MessageCallback, 0);
                    printf("animating sketching program object is buggy \n");
                }

                //setInt(ScreenBuffer.glData.ProgramIDs[0], "ttexture1", ScreenBuffer.glData.textureHandle);                

                useProgram(ScreenBuffer.glData.ProgramIDs[0]);
                basic_shader_->setMat4("projection", BackBuffer.camera.projection);
                std::cout<<"Projection mat: "<<glm::to_string(BackBuffer.camera.projection)<<std::endl;                
                setMat4(ScreenBuffer.glData.ProgramIDs[0], "view", BackBuffer.camera.view);


                
                Model_* backpack = nullptr;
                backpack = new Model_();
                std::string backpack_path = "./media/backpack.obj";
                loadModel_(backpack, backpack_path);

                //std::string path = "C:/Users/klove/Documents/repos/GLFW2/Vulkan_Learning_Project/build/source/stylised_terrain_tile_1011124259_texture_fbx/stylised_terrain_tile_1011124259_texture.fbx";

//NOW THE ANIMATING PART

                Model_* dancing_vampire = nullptr;
                dancing_vampire = new Model_();
                std::string dancing_vampire_path = "./media/dancing_vampire.dae";
                loadModel_(dancing_vampire, dancing_vampire_path);

                    danceAnimation = CreateAnimation((char* )dancing_vampire_path.c_str(), dancing_vampire);
                    animator = SpawnAnimator(danceAnimation);
                    
//modell->Texturedirectory = texpath.substr(0, path.find_last_of('/'));
                printf("texture id:%d\n", ScreenBuffer.glData.textureHandle);
                printf("vertex array :%d\n", ScreenBuffer.glData.VAOs);

                

                Game_Input Input[2] = {};
                Game_Input* OldInput = &Input[0];
                Game_Input* NewInput = &Input[1];


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
                float ChangeAxisCounter = 0.0f;
                int64 ViewRotateCount = 0;

                //Window = SetCapture(Window);

                useProgram(ScreenBuffer.glData.ProgramIDs[1]);
                //setInt(ScreenBuffer.glData.ProgramIDs[1], "material.diffuse", 0);
                //setInt(ScreenBuffer.glData.ProgramIDs[1], "material.specular", 1);
                model_shader_->setFloat("material.shininess", 32.0f);
                //
                model_shader_->setMat4( "projection", BackBuffer.camera.projection);
                model_shader_->setMat4( "view", BackBuffer.camera.view);
                model_shader_->setVec3( "ViewPos", BackBuffer.camera.Position);
                //model_shader_->setVec3( "lightPos", glm::vec3(4.0f, 3.0f, 0.0f));                

                // directional light
                model_shader_->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
                model_shader_->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
                model_shader_->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
                model_shader_->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

                
                //Model_Shader_->Set point light
                model_shader_->setVec3("pointLights[0].position", pointLightPositions[0]);
                model_shader_->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
                model_shader_->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
                model_shader_->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
                model_shader_->setFloat("pointLights[0].constant", 1.0f);
                model_shader_->setFloat("pointLights[0].linearTerm", 0.09f);
                model_shader_->setFloat("pointLights[0].quadraticTerm", 0.032f);

                 //point light 2
                model_shader_->setVec3("pointLights[1].position", pointLightPositions[1]);
                model_shader_->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
                model_shader_->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
                model_shader_->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
                model_shader_->setFloat("pointLights[1].constant", 1.0f);
                model_shader_->setFloat("pointLights[1].linearTerm", 0.09f);
                model_shader_->setFloat("pointLights[1].quadraticTerm", 0.032f);

                // point light 3
                 //model_shader_->setVec3("pointLights[2].position", pointLightPositions[2]);
                 //model_shader_->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
                 //model_shader_->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
                 //model_shader_->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
                 //model_shader_->setFloat("pointLights[2].constant", 1.0f);
                 //model_shader_->setFloat("pointLights[2].linear", 0.09f);
                 //model_shader_->setFloat("pointLights[2].quadratic", 0.032f);

                // point light 4
                 //model_shader_->setVec3("pointLights[3].position", pointLightPositions[3]);
                 //model_shader_->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
                 //model_shader_->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
                 //model_shader_->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
                 //model_shader_->setFloat("pointLights[3].constant", 1.0f);
                 //model_shader_->setFloat("pointLights[3].linear", 0.09f);
                 //model_shader_->setFloat("pointLights[3].quadratic", 0.032f);

                useProgram(ScreenBuffer.glData.ProgramIDs[0]);
                QueryPerformanceCounter(&LastCounter);

                while (GlobalRunning) {

                  //if (first_announce) {
                      //LastCycleCounts = __rdtsc();
                  //} else {
                    // NOTE: Why this stay the same over and equal to zero the
                    // the next 8 frames
                    // This one is buggy somehow
                    //TicksPerFrame = EndCycleCounts - LastCycleCounts;
                  //}

                    //printf("Count from start of frame: %I64d\n",LastCycleCounts);

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
                    //}
                    if (Load_Lib) {
                      if (AniLib != NULL) {
                        // if (FreeLibrary(AniLib)) {
                        // printf("Succeed free current lib\n");
                        //} else {
                        // printf("fail to free current lib
                        // %s\n",GetLastError());
                        //}
                        FreeLibrary(AniLib);
                        if (CopyFile("skeletalAni32.dll",
                                     "skeletalAni32_copy.dll", false)) {

                          // printf("Succeed copy dll file\n");
                          AniLib = LoadLibraryA("skeletalAni32_copy.dll");
                        }
                        // Animation
                        if (AniLib != NULL) {
                          //printf("Succeed load code from dll\n");
                          KillAninmation = (AniClassSlainer)GetProcAddress(
                              AniLib, "DestroysAniClass");

                          SlayAnimator = (AniUserClassSlayer)GetProcAddress(
                              AniLib, "DestroyAnimatorClass");

                          AniUpdate = (AniTimeUpdater)GetProcAddress(
                              AniLib, "updateAnimationTime_");

                          PlayAnimation = (PlayAni__)GetProcAddress(AniLib, "PlayAni_");                          
                          //setupMesh =
                              //(setupMeshh)GetProcAddress(AniLib, "setMesh");
                          //Draw = (MDraw)GetProcAddress(AniLib, "Draw");
                          }

                        if (Load_Lib) {
                          Load_Lib = false;
                        }

                        }
                    }

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
                    // We define that through setInt
                    //Why the &ScreenBuffer data doesn't show on the direct screen


 
                    //__rdtsc() is an intrinsict (the one which looked like a function call
                    //but it actually a hint to the compiler to a specific dissembly language intstruction)

/*                
                  S : Single 
                  I : Instruction
                  M : Multiple
                  D : Data
                
*/
#if DISPLAY_TIME                
                    //char Buffer[256];
                    // NOTE: The '%' is to decide the format of the next thing
                    // to print for example: %d is the 32 bit
                    //sprintf(Buffer, "%f Miliseconds/Frame, %f FPS, %f Ms/f \n ",
                            //MsPerFrame, FPS, MsPerFrame);
                    //OutputDebugStringA(Buffer);
#endif                

                    int ChosenAxis = 0;

                    //Ratio is based on miscalculated Msperframe
                    //if (RatioCalculated) {
                     DelayedRatio =
                         (real64)(MsPerFrame/StandardMSperFrame);
                        DelayedRatio>0.0f?BackBuffer.camera.speed = (2.5f * DelayedRatio):BackBuffer.camera.speed = (2.5f  * 0.17f);                
                        ColorOffset += (float)0.1*DelayedRatio;
                        if(ColorOffset > 1.0f){
                            ColorOffset -= 1.0f;  
                        };
                        //RatioCalculated = false;
                    //}

                    // Wait to 17 milli s perframe for model to rotate

                    // Update animating model based on frame time
                    // => calculate finalTransformmatrices and set to
                    
                    
                    //if(BackBuffer.camera.moved || BackBuffer.camera.mouse.moved){
                    UpdateCamera(&BackBuffer.camera, DelayedRatio);                    
                    //}
                    if(BackBuffer.camera.mouse.Wheeled)
                    {
                        BackBuffer.camera.projection = glm::perspective(glm::radians(BackBuffer.camera.fov), (float)ScreenBuffer.BitmapWidth / (float)ScreenBuffer.BitmapHeight, 0.1f, 100.0f);
                        glUseProgram(ScreenBuffer.glData.ProgramIDs[0]);
                        basic_shader_->setMat4("projection", BackBuffer.camera.projection);
                        glUseProgram(ScreenBuffer.glData.ProgramIDs[1]);
                        model_shader_->setMat4("projection", BackBuffer.camera.projection);
                        BackBuffer.camera.mouse.Wheeled = false;
                    }

                    basic_shader_->use();
                    basic_shader_->setMat4("view", BackBuffer.camera.view);

                    model_shader_->use();
                    model_shader_->setMat4("view", BackBuffer.camera.view);

                    // Start to add some basic lighting to the model
                    useProgram(ScreenBuffer.glData.ProgramIDs[1]);
                    //setMat4(ScreenBuffer.glData.ProgramIDs[0], "model", Model);
                    //glBindVertexArray(ScreenBuffer.glData.VAOs);
                    //glDrawArrays(GL_TRIANGLES, 0, 36);

                    // NOTE: Thing went wrong inside this function
                    // whether the waittimecounter or the function itself
                    // produce bugs
                    real64 updateTime;                    
                    if (first_size) {
                        printf("counter: %f\n", WaitTimeCounter);
                    }

                    bool TimeToChangeAxis = false;
                    if(WaitTimeCounter >= 16.67f){                        
                        WaitTimeCounter = 0.0f;
                    } else {
                        WaitTimeCounter += (float)MsPerFrame;
                        //printf("WaitTimeCounter: %f\n", WaitTimeCounter);
                    }
                        //else {
                        //ViewRotateCount++;
                        //float CamX = sin(ViewRotateCount)*10.0f;
                        //float CamZ = cos(ViewRotateCount)*10.0f;
                        //BackBuffer.camera.view = glm::lookAt(glm::vec3(CamX, 0.0f, CamZ), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                        //setMat4(ScreenBuffer.glData.ProgramID, "view", BackBuffer.camera.view);
                        //}
                        // Update animation

                        //Set vectices and color for plane


                        updateDegreeInPi += 0.087f * DelayedRatio;                        
                        UpdatedAngle += 10.0f * DelayedRatio;

                        if(UpdatedAngle > 360.0f){
                            UpdatedAngle -= 360.0f;
                        };
                        
                        if(updateDegreeInPi > threshHold){
                            updateDegreeInPi -= threshHold;
                        };

                        if(ChangeAxisCounter >= 1000.0f){
                            int axisIndex = std::rand()%3;
                            randomRotateAxis = randomRotateAxis_(axisIndex);
                            ChangeAxisCounter = 0.0f;
                            //printf("ChangeAxisCounter: %f\n", WaitTimeCounter);
                            //
                            if(!TimeToChangeAxis){
                                TimeToChangeAxis = true;
                            }
                        } else {
                            ChangeAxisCounter += WaitTimeCounter;
                        }

                        
                        //BUGGY
                        basic_cube_core = glm::rotate(basic_cube_core, glm::radians(10.0f) * (float)DelayedRatio, randomRotateAxis);

                    //RENDER =====================================
                    basic_shader_->use();
                    glBindVertexArray(ScreenBuffer.glData.PlaneVAOs);
                    basic_shader_->setMat4("model", Plane);
                    //why it only show half the plane
                    glDrawArrays(GL_TRIANGLES, 0, 6);

                    basic_shader_->setFloat("colorOffset", ColorOffset);
                    glBindVertexArray(ScreenBuffer.glData.VAOs);
                    basic_shader_->setMat4("model", basic_cube_core);
                    glDrawArrays(GL_TRIANGLES, 0, 36);


                    drawTile(ScreenBuffer.glData.VAOs, ScreenBuffer.glData.ProgramIDs[0], DelayedRatio, &UpdatedAngle, TimeToChangeAxis, &rollCubeMap);
                    //drawTile(ScreenBuffer.glData.VAOs, ScreenBuffer.glData.ProgramIDs[0], DelayedRatio, &updateDegreeInPi, TimeToChangeAxis, &rollCubeMap);

                    if(TimeToChangeAxis){
                        TimeToChangeAxis = false;
                    }

                    //Draw the backpack
                    model_shader_->use();
                    //glBindVertexArray(ScreenBuffer.glData.VAOs);
                    GLuint brushID = model_shader_->GetProgramID();
                    model_shader_->setMat4("model", backpack_core);
                    DDraw(backpack, &brushID);
                    //DDraw(dancing_vampire, &brushID);                    
// Now Draw the vampire
                    Game_Input* Temp = NewInput;
                    NewInput = OldInput;  //???? still don't understand
                    OldInput = Temp;
                    SwapBuffers(DeviceContext);
                    ReleaseDC(Window, DeviceContext);

                    //real32 endFrame = EndCycleCounts;
                    //LastCycleCounts = EndCycleCounts;
                    //EndCycleCounts = __rdtsc();
                    //printf("Count by the end of frame: %I64d\n", EndCycleCounts);
                    // Why this produce same result
//Two different approaching ways

                    
                      if (!QueryPerformanceCounter(&EndCounter)) {
                        printf("Failed to call performancecounter function\n");
                      };

                      if (EndCounter.QuadPart > LastCounter.QuadPart) {
                          // We got how many count per frame
                          CountsPerFrame =
                              (int64)(EndCounter.QuadPart - LastCounter.QuadPart);
                          MsPerFrame =
                              (real64)((1000.0f * (real64)CountsPerFrame) / (real64)PerfCountFrequency);
                          updateTime = ((real64)MsPerFrame/1000.0f)>0.0f?((real64)MsPerFrame/1000.0f):updateTime;
                          if (MsPerFrame > 0.0f) {
                              //SPerFrame = MsPerFrame / 1000;
                              // deltaTime = (float)(1 / 60);
                              FPS = (real64)(1000.0f / MsPerFrame);
                          }
                      }

                      if (updateTime > 0.0f) {
                            AniUpdate(animator, &updateTime);
                            PlayAnimation(animator, danceAnimation);
                      }


                      animating_shader_->use();
                      animating_shader_->setMat4( "model", dancing_vampire_core);
                      animating_shader_->setMat4( "projection", Projection);
                      animating_shader_->setMat4( "view", BackBuffer.camera.view);

                      std::unordered_map<std::string, Bone_Info>* boneMapClone = danceAnimation->GetBoneIDMap();
                      std::vector<glm::mat4>* Transform = animator->getFinalBoneMatrices();

                      //if(first_announce){
                      int i = 0;
                          if (boneMapClone->size() > 1) {
                              //for(const std::pair<std::string, Bone_Info>&bone : (*boneMapClone))
                              //{
                                  //animating_shader_->setMat4(
                                      //"finalBoneMatrices[boneIds[" + std::to_string(bone.second.id) +
                                      //"]]",
                                      //(*Transform)[bone.second.id]);
                                  // what is boneIds actually;
                                  //if (first_announce) {
                                      //printf("finalBoneMatrices[boneIds[%d]]: %s\n", (int)bone.second.id, glm::to_string((*Transform)[bone.second.id]).c_str());
                                  //}
                              //};
                              for(const glm::mat4&matrix_ : (*Transform)){
                                  animating_shader_->setMat4(
                                      "finalBoneMatrices[" + std::to_string(i) +
                                      "]", matrix_);
                                  i++;
                                  if (first_announce) {
                                  printf("Bone Matrix index %d is: %s\n", (int)i, glm::to_string(matrix_).c_str());
                                  };
                              };
                          }
                      //}

                      brushID = animating_shader_->GetProgramID();
                      DDraw(dancing_vampire, &brushID);                    
                    

                      
                      if (showMsPF) {
                          printf("[LastFrameCount:%f,EndFrameCount:%f, "
                                 "CounterPerFrame : %I64d], MiliS per frame: "
                                 "%f, real FPS: %I64d \n",
                                 (real32)LastCounter.QuadPart,
                                 (real32)EndCounter.QuadPart, CountsPerFrame,
                                 MsPerFrame, FPS);
                          printf("Delay Ratio: %f, msPerframe: %f, SPF: %f\n",
                                 DelayedRatio, MsPerFrame, updateTime);
                          printf("WaitTimeCounter: %f, Axis changing counter: %f\n", WaitTimeCounter, ChangeAxisCounter);
                          printf("ColorOffset is:%f\n", ColorOffset);

                          printf("updated angle :%f\n", UpdatedAngle);
                          std::cout<<"Center Cube Matrix is: "<<glm::to_string(basic_cube_core)<<std::endl;
                          std::cout<<"Current rotating axis is: "<<glm::to_string(randomRotateAxis)<<std::endl;
                          showMsPF = false;
                      };
                      LastCounter = EndCounter;

                    if (first_announce) {
                       first_announce = false;
                      }
                    //char Buffers[256];
                    //sprintf(Buffers,
                            //"[LastFrameCount: %f,EndFrameCount:%f, CounterPerFrame : %I64d], MiliS per frame: %I64d, real FPS: %I64d \n",(real32)LastCounter.QuadPart,(real32)EndCounter.QuadPart, CountsPerFrame,MsPerFrame, FPS);
                    //printf("[LastFrameCount:%f,EndFrameCount:%f, CounterPerFrame : %I64d], MiliS per frame: %I64d, real FPS: %I64d \n",(real32)LastCounter.QuadPart,(real32)EndCounter.QuadPart, CountsPerFrame, MsPerFrame, FPS);
                    //OutputDebugStringA(Buffers);
                    }
          }

/*
MULPD -> real32 ==> 128 bits / 32 bits -> 4 real32 packs per registerMULPS -> real64 ==> 128 bits / 64 bits -> 2 real32 packs per register
*/
          SlayAnimator(animator);
          KillAninmation(danceAnimation);
          
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
