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

glm::mat4 dancing_vampire_core = glm::mat4(1.0f);
bool32 is_moving = false;

float DelayedRatio = 0.5f;

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
      GetWindowDimension(Window, &BackBuffer);
    if (first_size) {
      first_size = false;
    } else {
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

      case WM_KEYUP:{

          bool IsUp = ((Lparam & (1 << 31)) != 1);
          bool WasUP = ((Lparam & (1 << 30)) == 0);
          bool WasDown = ((Lparam & (1 << 30)) == 1);

          uint32 vkCode = Wparam;
          
          if (vkCode == VK_UP) {
              // Actually the front vec is at the back of the camera
              // State.BlueOffset+= 10;
                  if(is_moving)
                  is_moving = !is_moving;

                  printf("Up is released\n");
          }

          else if (vkCode == VK_DOWN) {
              // Actually the front vec is at the back of the camera
              // State.BlueOffset+= 10;
                  if(is_moving){
                      is_moving = !is_moving;
                  }
              printf("DOWN is released\n");
          }

          else if (vkCode == VK_LEFT) {
              // Actually the front vec is at the back of the camera
              // State.BlueOffset+= 10;
              printf("LEFT is released\n");
              if(is_moving)
                is_moving = !is_moving;

          }

          else if (vkCode == VK_RIGHT) {
              // Actually the front vec is at the back of the camera
              // State.BlueOffset+= 10;
              if(is_moving)
                is_moving = !is_moving;
              printf("RIGHT is released\n");
          }          

          //else if (vkCode == VK_TAB) {
              //if (SoundOutPut.hz == 128) {
                  //SoundOutPut.hz = 256;
              //} else if (SoundOutPut.hz == 256) {
                  //SoundOutPut.hz = 512;
              //} else {
                  //SoundOutPut.hz = 128;
              //}
              //char Output[256];
              //sprintf(Output, "TAB button hitted, Current Hert is: %d\n",
                      //SoundOutPut.hz);
              //SoundOutPut.WavePeriod = SoundOutPut.SamplePerSecond / SoundOutPut.hz;
//
              //OutputDebugStringA("TAB button hitted");
          //}
//          
      }break;

 case WM_KEYDOWN: {
    bool IsDown = ((Lparam & (1 << 31)) == 0);
    bool WasDown = ((Lparam & (1 << 30)) == 1);
    bool WasUp = ((Lparam & (1 << 30)) == 0);

    uint32 vkCode = Wparam;
    if (IsDown) {
      if (vkCode == VK_UP) {
        // Actually the front vec is at the back of the camera
        // State.BlueOffset+= 10;
            //if (WasUp) {
                dancing_vampire_core = glm::translate(dancing_vampire_core, glm::vec3(0.0f, 0.0f, 5.0f * DelayedRatio));

                if(!is_moving)
                    is_moving = !is_moving;

                printf("Up is hit\n");
            //}
      }

      else if (vkCode == VK_DOWN) {
        // Actually the front vec is at the back of the camera
        // State.BlueOffset+= 10;
            //if (WasUp) {
                dancing_vampire_core = glm::translate(dancing_vampire_core, glm::vec3(0.0f, 0.0f, -(5.0f * DelayedRatio)));
                if(!is_moving)
                    is_moving = !is_moving;
                printf("DOWN is HIT\n");
            //}

      }

      else if (vkCode == VK_LEFT) {
        // Actually the front vec is at the back of the camera
        // State.BlueOffset+= 10;
            //if (WasUp) {
                dancing_vampire_core = glm::translate(dancing_vampire_core, glm::vec3(-(5.0f * DelayedRatio), 0.0f, 0.0f));
                if(!is_moving)
                    is_moving = !is_moving;

                printf("LEFT is HIT\n");
        //}
      }

      else if (vkCode == VK_RIGHT) {
        // Actually the front vec is at the back of the camera
        // State.BlueOffset+= 10;
            //if (WasUp) {
                dancing_vampire_core = glm::translate(dancing_vampire_core, glm::vec3(5.0f * DelayedRatio, 0.0f, 0.0f));
                if(!is_moving)
                    is_moving = !is_moving;
                printf("RIGHT is HIT\n");
            //}
      }
        //====================================================
      else  if (vkCode == 'W') {
        // Actually the front vec is at the back of the camera
        // State.BlueOffset+= 10;
        BackBuffer.camera.Position +=
            glm::normalize(BackBuffer.camera.Direction) *
            (float)BackBuffer.camera.speed;
        if (!WasDown) {
            printf("W is HIT\n");
        }
      }

      else if (vkCode == 'S') {
        State.GreenOffset += 10;
        BackBuffer.camera.Position -=
            glm::normalize(BackBuffer.camera.Direction) *
            (float)BackBuffer.camera.speed;
        if (!WasDown) {
            printf("S is HIT\n");
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
            printf("A is HIT\n");
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
            printf("D is HIT\n");
        }
        // XOffset += 10;
      }

      else if (vkCode == VK_SPACE) {

          BackBuffer.camera.Position += BackBuffer.camera.Up * (float)BackBuffer.camera.speed;

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
    BackBuffer.camera.fov -= wheelPos * BackBuffer.camera.speed;

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
      GetWindowDimension(Window, &BackBuffer);
      
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
    //if (fDraw) {
      //DeviceContext = GetDC(Window);
      //MoveToEx(DeviceContext, ptPrevious.x, ptPrevious.y, NULL);
      //LineTo(DeviceContext, ptPrevious.x = LOWORD(Lparam),
             //ptPrevious.y = HIWORD(Lparam));
      //ReleaseDC(Window, DeviceContext);
    //}
//
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

  case WM_DESTROY: {
    GlobalRunning = false;
    PostQuitMessage(0);
    OutputDebugStringA("WM_DESTROY\n");
  } break;

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

  LARGE_INTEGER previous_collided = {};
  LARGE_INTEGER current_collided = {};
  float collided_time = 0.0f;

  uint64 LastCycleCounts = 0;
  uint64 EndCycleCounts = 0;


  float TimeCounter = 0.0f;
  float WaitTimeCounter = 0.0f;
  float ColorOffset = 0.0f;

  uint64 TicksPerFrame = 0;
  real64 TicksPerS = 0;

  int64 CountsPerFrame = 0;
  //So what will be less than 0 must be float type
  real64 MsPerFrame = 0.0f;
  float SPerFrame = 0.0f;
  int64 FPS = 0;
  // Time elapsed of one cycle/frame in second

  bool RatioCalculated = false;
  real64 DelayedRatio = 0.0f;
  real64 StandardMSperFrame = 16.67f;

  bool32 collided_  = false;
  bool32 along1 = true;  
  bool32 along2 = true;  
  bool32 short_color_change_ = false;
  float color_switch_dur = 0.0f;
  
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
  ShowInfo_ showUniformVarValuePerVertex = NULL;
  check_collision_ check_collision = NULL;

  //setUpUBO__ setUpUBO = NULL;
  //updateUBOData__ updateUBOData = NULL;
  //setupMeshh setupMesh = NULL;
  //MDraw Draw = NULL;
      //AniTimeUpdater AniUpdate_;

  B_shader_program* model_shader_;
  B_shader_program* animating_shader_;
  B_shader_program* basic_shader_;
  B_shader_program* quad_shader_;

  
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

          showUniformVarValuePerVertex = (ShowInfo_)GetProcAddress(AniLib, "ShowInfo");
          check_collision = (check_collision_)GetProcAddress(AniLib, "check_collision_wrapper");
          //setUpUBO = (setUpUBO__)GetProcAddress(AniLib, "setupUBO_");
          //updateUBOData = (updateUBOData__)GetProcAddress(AniLib, "updateUBOData_");

          //setupMesh = (setupMeshh)GetProcAddress(AniLib, "setupMesh");
          //Draw = (MDraw)GetProcAddress(AniLib, "Draw");
          //Load_Lib = false;
      }

      
      MSG message;

      glm::vec3 tempPos = glm::vec3(9.0f, 0.3, 1.0f);
      glm::vec3 size = glm::vec3(1.0f);
      space_box obj1 = space_box(&tempPos, &size);
      tempPos = glm::vec3(-9.0f, 0.3, 1.0f);
      space_box obj2 = space_box(&tempPos, &size);

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

                GetWindowDimension(Window, &BackBuffer);
                glViewport(0, 0, Dimens.Width, Dimens.Height);

                //shader_name.clear();                
                std::string shader_name = "animating sketching brush";
                animating_shader_ = new B_shader_program("2.skeletal_animation.vs", "2.skeletal_animation.fs", shader_name.c_str());
                ScreenBuffer.glData.ProgramIDs.push_back(animating_shader_->GetProgramID());

// Basic shader
                shader_name = "basic brush";
                basic_shader_ = new B_shader_program("shader.vs", "shader.fs", shader_name.c_str());             ScreenBuffer.glData.ProgramIDs.push_back(basic_shader_->GetProgramID());

                shader_name = "quad brush";
                quad_shader_ = new B_shader_program("quad.vs", "quad.fs", shader_name.c_str());             ScreenBuffer.glData.ProgramIDs.push_back(quad_shader_->GetProgramID());

                //shader_name.clear();
                shader_name = "model drawing brush";
                //basic model shader
                model_shader_ = new B_shader_program("1.model.vs", "1.model.fs", shader_name.c_str());                ScreenBuffer.glData.ProgramIDs.push_back(model_shader_->GetProgramID());
                
                copyBufferData(&BackBuffer, &ScreenBuffer);
                //????
                //glm::mat4 View = glm::mat4(1.0f);
                real32 UpdatedAngle = 0.0f;
                real32 updateDegreeInPi = 0.0f;
                float threshHold = glm::radians((float)360.0f);

                glm::mat4 basic_cube_core = glm::mat4(1.0f);
                glm::mat4 backpack_core = glm::mat4(1.0f);

                
                glm::mat4 Plane = glm::mat4(1.0f);
                Plane = glm::translate(Plane, glm::vec3(0.0f));
                glm::mat4 WorldToCamera = glm::mat4(1.0f);
                WorldToCamera = BackBuffer.camera.view * dancing_vampire_core;
                
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
                //
                GetWindowDimension(Window, &BackBuffer);
                InitCamera(&BackBuffer);
                ViewCamera(&BackBuffer.camera);

                basic_cube_core = glm::translate(basic_cube_core, glm::vec3(2.0f, -4.0f, 0.0f));
                std::cout<<"Central rotating model is"<<glm::to_string(basic_cube_core)<<std::endl;
                backpack_core = glm::translate(backpack_core, glm::vec3(-4.0f, 2.0f, 3.0f));
                std::cout<<"Stand still model 2 matrix is :"<<glm::to_string(backpack_core)<<std::endl;

                // Set containing model for dancing vampire
                
                dancing_vampire_core = glm::scale(dancing_vampire_core,glm::vec3( 0.01f));
                dancing_vampire_core = glm::translate(dancing_vampire_core, glm::vec3(-2.0f, 0.0f, 0.0f));

                BackBuffer.camera.projection = glm::perspective(glm::radians(BackBuffer.camera.fov), (float)Dimens.Width / (float)Dimens.Height, 0.1f, 100.0f);
                
                  CheckShader(ScreenBuffer.glData.ProgramIDs[0], programme_, "basic brush");
                  CheckShader(ScreenBuffer.glData.ProgramIDs[1], programme_, "model drawing brush");
                  CheckShader(ScreenBuffer.glData.ProgramIDs[2], programme_, "animating brush");

                basic_shader_->use();
                basic_shader_->setMat4("projection", BackBuffer.camera.projection);

                Set_Projection_View(&BackBuffer);

                std::string Mname = "backpack";                
                Model_* backpack = nullptr;
                backpack = new Model_(false, &Mname);
                std::string backpack_path = "./media/backpack.obj";
                loadModel_(backpack, backpack_path);

                //std::string path = "C:/Users/klove/Documents/repos/GLFW2/Vulkan_Learning_Project/build/source/stylised_terrain_tile_1011124259_texture_fbx/stylised_terrain_tile_1011124259_texture.fbx";

//NOW THE ANIMATING PART

                Model_* dancing_vampire = nullptr;
                Mname = "vampire";
                dancing_vampire = new Model_(false, &Mname);
                std::string dancing_vampire_path = "./media/dancing_vampire.dae";
                loadModel_(dancing_vampire, dancing_vampire_path);

                danceAnimation = CreateAnimation((char* )dancing_vampire_path.c_str(), dancing_vampire);
                animator = SpawnAnimator(danceAnimation);
                    
                Game_Input Input[2] = {};
                Game_Input* OldInput = &Input[0];
                Game_Input* NewInput = &Input[1];


                //win32_Sound_OutPut SoundOutPut = {};
                //Game_Sound_OutPut SoundBuffer = {};
                //InitSoundBuffer(&Window, &SoundOutPut);
                //int16* SSamples = (int16* )VirtualAlloc(0 , SoundOutPut.SecondBufferSize ,MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

                ////Why InitOpenGl only work in the app loop
            
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

//// Set light environment here
                tempSetEnviLight(model_shader_);
                animating_shader_->use();
                animating_shader_->setMat4( "projection", BackBuffer.camera.projection);
                
                GLuint UBO;
                int k = 0;                

                GLuint id = animating_shader_->GetProgramID();
                showUniformVarValuePerVertex(&UBO, &id, &dancing_vampire->meshes[0], false, false, false, false, false, false);

                while (GlobalRunning) {

                  if(first_announce) {
                      QueryPerformanceCounter(&LastCounter);
                      previous_collided = LastCounter;
                      LastCycleCounts = __rdtsc();
                  } else {
                    // NOTE: Why this stay the same over and equal to zero the
                    // the next 8 frames
                    // This one is buggy somehow
                    if (EndCounter.QuadPart > LastCounter.QuadPart) {
                          // We got how many count per frame
                        CountsPerFrame = (int64)(EndCounter.QuadPart - LastCounter.QuadPart);
                      }

                    MsPerFrame = (real64)((1000.0f * (real64)CountsPerFrame) / PerfCountFrequency);


                    // The value of s per frame is too small for float to hold
                    SPerFrame = (float)(MsPerFrame/1000);
                          //UpdateTime is really a pain here
                    real64 FramePerS = 1000.0f/((real64)MsPerFrame);

                    SPerFrame = (SPerFrame>0.0f)?SPerFrame:0.0167;

                      if (MsPerFrame > 0.0f) {
                          //SPerFrame = MsPerFrame / 1000;
                          // deltaTime = (float)(1 / 60);
                          FPS = (real64)(1000.0f / MsPerFrame);
                      }

                      //Cause the ms per frame is always less than 1000ms that mean it will be less than 0 after being converted to second then;
                      TicksPerFrame = EndCycleCounts - LastCycleCounts;

                      if(SPerFrame > 0.0f){
                          TicksPerS = TicksPerFrame/SPerFrame;
                      }else{
                          TicksPerS = TicksPerFrame*FramePerS;                          
                      };

                      LastCycleCounts = EndCycleCounts;
                      LastCounter = EndCounter;
                  }

                    //printf("Count from start of frame: %I64d\n",LastCycleCounts);
                  MSG Message;
                    //NOTE: This is where receiving the message to change
                    // for any change in window
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
                         if (FreeLibrary(AniLib)) {
                         printf("Succeed free current lib\n");
                        } else {
                             printf("fail to free current lib %s\n", GetLastError());
                        }

                        if (CopyFile("skeletalAni32.dll",
                                     "skeletalAni32_copy.dll", false)) {

                           printf("Succeed copy dll file\n");
                          AniLib = LoadLibraryA("skeletalAni32_copy.dll");
                        }
//
                        // Animation
                        if (AniLib != NULL) {
                            printf("Succeed reload code and opengl function from dll\n");
                          KillAninmation = (AniClassSlainer)GetProcAddress(
                              AniLib, "DestroysAniClass");

                          SlayAnimator = (AniUserClassSlayer)GetProcAddress(
                              AniLib, "DestroyAnimatorClass");

                          AniUpdate = (AniTimeUpdater)GetProcAddress(
                              AniLib, "updateAnimationTime_");

                          PlayAnimation = (PlayAni__)GetProcAddress(AniLib, "PlayAni_");                          

                          showUniformVarValuePerVertex = (ShowInfo_)GetProcAddress(AniLib, "ShowInfo");

                          check_collision = (check_collision_)GetProcAddress(AniLib, "check_collision_wrapper");
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
                    //WriteSoundToBuffer(&SoundBuffer, &SoundOutPut, SSamples);
                    //=====================================================================
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
                    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);             

                    //GameUpdateAndRender(&game_memory, BMPContent, NewInput, &State, &ScreenBuffer , &SoundBuffer, NULL);
                    
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

                           collided_ = false;
                    int ChosenAxis = 0;

                    //Ratio is based on miscalculated Msperframe
                    //if (RatioCalculated) {
                     DelayedRatio =
                         (float)(MsPerFrame/StandardMSperFrame);
                        DelayedRatio>0.0f?BackBuffer.camera.speed = (2.5f * DelayedRatio):BackBuffer.camera.speed = (2.5f  * 0.17f);                
                        ColorOffset += (float)0.1*DelayedRatio;
                        if(ColorOffset > 1.0f){
                            ColorOffset -= 1.0f;  
                        };
                        //RatioCalculated = false;
                    //}                    
                    
                    //if(BackBuffer.camera.moved || BackBuffer.camera.mouse.moved){
                    UpdateCamera(&BackBuffer.camera, DelayedRatio);
                    //}
                    
                    if(BackBuffer.camera.mouse.Wheeled)
                    {
                        BackBuffer.camera.projection = glm::perspective(glm::radians(BackBuffer.camera.fov), (float)ScreenBuffer.BitmapWidth / (float)ScreenBuffer.BitmapHeight, 0.1f, 100.0f);

                        BackBuffer.camera.mouse.Wheeled = false;
                    }

                    basic_shader_->use();
                    basic_shader_->setMat4("projection", BackBuffer.camera.projection);

                    quad_shader_->use();
                    quad_shader_->setMat4("projection", BackBuffer.camera.projection);

                    model_shader_->use();
                    model_shader_->setMat4("projection", BackBuffer.camera.projection);

                    animating_shader_->use();
                    animating_shader_->setMat4("projection", BackBuffer.camera.projection);
                    
                    basic_shader_->use();
                    basic_shader_->setMat4("view", BackBuffer.camera.view);

                    quad_shader_->use();
                    quad_shader_->setMat4("view", BackBuffer.camera.view);

                    model_shader_->use();
                    model_shader_->setMat4("view", BackBuffer.camera.view);

                    animating_shader_->use();
                    WorldToCamera = BackBuffer.camera.view * dancing_vampire_core;
                    animating_shader_->setMat4("WorldToCamera", WorldToCamera);
                    
                    glUseProgram(0);
                    Set_Projection_View(&BackBuffer);

                    //// Start to add some basic lighting to the model


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

                        // Move and check collision here
                        glm::vec3 move_right = glm::vec3(DelayedRatio>0.0f?(0.2f * DelayedRatio):0.02f, 0.0f, 0.0f);
                        glm::vec3 move_left = glm::vec3(DelayedRatio>0.0f?(-0.2f * DelayedRatio):-0.02f, 0.0f, 0.0f);

                        collided_ = check_collision(&obj1, &obj2);

                        if(obj1.position[3][0] < -15.0f){
                            if(along1)
                            along1 = !along1;
                        }

                        if(obj2.position[3][0] > 15.0f){
                            if(along2)
                            along2 = !along2;
                        }

                        if(collided_){
                            QueryPerformanceCounter(&current_collided);
                            if(current_collided.QuadPart > previous_collided.QuadPart)
                                collided_time = ((float)(current_collided.QuadPart - previous_collided.QuadPart))/PerfCountFrequency;

                            if(first_announce){
                                printf("Time from begin to first time collide is:%f\n", collided_time);
                            }
                                previous_collided = current_collided;

                            obj1.position = glm::translate(obj1.position, (*obj1.collide_list)[0].space);
                            obj1.collide_list->pop_back();

                            obj2.position = glm::translate(obj2.position, (*obj2.collide_list)[0].space);
                            obj1.collide_list->pop_back();

                            if(!along1)
                            along1 = !along1;
                            if(!along2)
                            along2 = !along2;
                        }

                        obj1.position = glm::translate(obj1.position, along1?move_left:move_right);//along?move_left:move_right
                        obj2.position = glm::translate(obj2.position, along2?move_right:move_left);//along?move_right:move_left
                        
                        
//=========================================================================

                        
                    //RENDER =====================================
                    quad_shader_->use();
                    glBindVertexArray(ScreenBuffer.glData.PlaneVAOs);
                    quad_shader_->setFloat("colorOffset", ColorOffset);
                    quad_shader_->setMat4("model", Plane);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    //===============================================

                    basic_shader_->use();
                    glBindVertexArray(ScreenBuffer.glData.VAOs);

                    bool32 dummyflag = false;
                    basic_shader_->setBool("short_color_change_", dummyflag);
                    basic_shader_->setFloat("colorOffset", ColorOffset);
                    basic_shader_->setMat4("model", basic_cube_core);
                    
                    //Draw colliding objects here
                    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
                    
                    //drawTile(ScreenBuffer.glData.VAOs, basic_shader_->GetProgramID(), DelayedRatio, &UpdatedAngle, TimeToChangeAxis, &rollCubeMap);

                    if(collided_){
                        if(!short_color_change_)
                            short_color_change_=!short_color_change_;
                    }

                    if(short_color_change_){
                        if(color_switch_dur >= 0.0f && color_switch_dur < 0.1f){
                            color_switch_dur += SPerFrame;
                        }else if(color_switch_dur > 0.094f){
                            short_color_change_=!short_color_change_;
                            color_switch_dur = 0.0f;   
                        }
                    }

                    
                    basic_shader_->setBool("short_color_change_", short_color_change_);
                    basic_shader_->setMat4("model", obj1.position);
                    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                    basic_shader_->setMat4("model", obj2.position);
                    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                        if(showMsPF){
                            printf("along1 is %s, along2 is %s\n", along1?"true":"false", along2?"true":"false");
                            printf("obj1 %f %f %f\n",
                                   obj1.position[3][0],
                                   obj1.position[3][1],
                                   obj1.position[3][2]);
                            printf("Collided time: %f, Color switch: %s, duration: %f\n",collided_time, short_color_change_?"true":"false", color_switch_dur );
                        }

                    
                    if(collided_)
                        collided_ = !collided_;
                    
                    glUseProgram(0);
                    glBindVertexArray(0);
                    GLuint brushID;

                    if(TimeToChangeAxis){
                        TimeToChangeAxis = false;
                    }


                    brushID = model_shader_->GetProgramID();
                    //Draw the backpack
                    model_shader_->use();
                    //glBindVertexArray(ScreenBuffer.glData.VAOs);
                    model_shader_->setMat4("model", backpack_core);
                    DDraw(backpack, &brushID);
                    glUseProgram(0);

                    //printf("Count by the end of frame: %I64d\n", EndCycleCounts);

// animation update and render ================================================

                    animating_shader_->use();
                    brushID = animating_shader_->GetProgramID();

                    if(first_announce){
                        //setUpUBO(animator, &brushID);
//Setup UBO
                        glGenBuffers(1, &UBO);
                        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
                        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)* 52, NULL, GL_STREAM_DRAW);
                        glBindBuffer(GL_UNIFORM_BUFFER, 0);

                        //// get Uniform location from ProgramID
                        GLint boneMatricIndex = glGetUniformBlockIndex(brushID, "finalBone");
                        // bind that location to global binding point (using glUniformBlock binding)
                        glUniformBlockBinding(brushID, boneMatricIndex, 1);
                        // then bind the binding point to UBO using glBindbufferrange
                        glBindBufferRange(GL_UNIFORM_BUFFER, 1, UBO, 0, sizeof(glm::mat4)* 52);
                    }

                    //float* Transform;
                    std::vector<glm::mat4>* Transform_;

                    if(is_moving || first_announce){
                        if(animator->GetCurrentTime() > danceAnimation->GetDuration() )
                        {
                            PlayAnimation(animator, danceAnimation);
                            if(showMsPF)
                                printf("animator current time: %f, animation duration: %f", animator->GetCurrentTime(), danceAnimation->GetDuration());

                        } else {

                            if(showMsPF){
                                printf("animator current time: %f, animation duration: %f", animator->GetCurrentTime(), danceAnimation->GetDuration());
                            }

                            if(SPerFrame > 0.0f && SPerFrame < 0.004f ) {
                                //AniUpdate expect S per frame;
                                AniUpdate(animator, &SPerFrame);
                                //updateUBOData(animator);
                                //std::vector<glm::mat4>* Transform = animator->getFinalBoneMatrices();
                            }else{
                                SPerFrame = 0.0035f;
                                AniUpdate(animator, &SPerFrame);                        
                            }                        

                        }   
                    }

                    
                    //Transform = animator->getFinalBoneMatrices();
                        Transform_ = animator->getFinalBoneMatrices();
                        //glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 16 * 52, Transform);
                        //if(Transform_->size() > 0){
                            glBindBuffer(GL_UNIFORM_BUFFER, UBO);
                            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4)* 52, Transform_->data());
                            glBindBuffer(GL_UNIFORM_BUFFER, UBO);
                            glBindBuffer(GL_UNIFORM_BUFFER, 0);
                        //}

                        int i = 0;
                            std::string matrixName_;

                            char index[1];
                            char indexx[2];
//
                            
/*                            
                            if (Transform_ != nullptr){
                                for(const glm::mat4& matrix_ : (*Transform_)) {
                                    //matrixName_ = "finalBoneMatrices["+std::to_string(i)+"]";

                                    matrixName_ = "final.finalBoneMatrices[]";

                                    if(i<10){
                                        sprintf(index, "%d", i);
                                        matrixName_.insert(matrixName_.size()-1, index);
                                    } else {
                                        sprintf(indexx, "%d", i);
                                        matrixName_.insert(matrixName_.size()-1, indexx);                                     
                                    }
                                    //animating_shader_->setMat4(
                                        //matrixName_.c_str(), matrix_);

                                    if(showMsPF){
                                        printf("uniform name %s :%s\n", matrixName_.c_str(), glm::to_string(matrix_).c_str());
                                    }
//
                                    //if(i < Transform->size())
                                    i++;
                                };
                            };

*/

                            //if(is_moving){
                                WorldToCamera = BackBuffer.camera.view * dancing_vampire_core;
                                animating_shader_->setMat4("WorldToCamera", WorldToCamera);                                
                            //}


                    animating_shader_->setBool("is_moving", is_moving);

//Why the later shader texture drawing work but not the one above
                    DDraw(dancing_vampire, &brushID);

                      if(showMsPF){
                          glm::vec2 TexCoordToShow;
                          int TextureID;
                          showUniformVarValuePerVertex(&UBO, &brushID, &dancing_vampire->meshes[0], false, false, false, false, false, false);

                      };

// animation update and render ================================================

                          //}

                    if (first_announce) {
                       first_announce = false;
                      }

//
                    Game_Input* Temp = NewInput;
                    NewInput = OldInput;  //???? still don't understand
                    OldInput = Temp;
                    SwapBuffers(DeviceContext);
                    ReleaseDC(Window, DeviceContext);//maybe this one

                    //End count of frame time
                    if (!QueryPerformanceCounter(&EndCounter)) {
                        printf("Failed to call performancecounter function\n");
                      };
                      EndCycleCounts = __rdtsc();
                }

          }


/*
MULPD -> real32 ==> 128 bits / 32 bits -> 4 real32 packs per registerMULPS -> real64 ==> 128 bits / 64 bits -> 2 real32 packs per register
*/
          SlayAnimator(animator);
          KillAninmation(danceAnimation);          

          glDeleteVertexArrays(1, &BackBuffer.glData.VAOs);
          glDeleteBuffers(1, &BackBuffer.glData.VBO);

          ResetGLState(&BackBuffer, Window);
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
    //wglDeleteContext(BackBuffer.glData.openglRC);
    return (0);
}
