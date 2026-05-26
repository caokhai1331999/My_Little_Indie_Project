/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "handmade.h"

uint32 safetruncateUint64(uint64 value){
    //Assert(value <= 0xFFFFFFFF);
    uint32 result = value;
    return result;
}

real32 saferatioN(real32 numerator, real32 divisor, real32 N){
    real32 result = N;
    if(divisor!=0.0f){
        result = (real32)(numerator/divisor);
    }
    return result;
}

real32 saferatio0(real32 numberator, real32 divisor){
    real32 result = saferatioN(numberator, divisor, 0.0f);
    return result;
}

real32 saferatio1(real32 numberator, real32 divisor){
    real32 result = saferatioN(numberator, divisor, 1.0f);
    return result;        
}

void CalEarlyFrameTime(Clock_Set* Time_Set){
                    // NOTE: Why this stay the same over and equal to zero the
                    // the next 8 frames
                    // This one is buggy somehow
                    if (Time_Set->EndCounter.QuadPart > Time_Set->LastCounter.QuadPart) {
                          // We got how many count per frame
                        Time_Set->CountsPerFrame = (int64)(Time_Set->EndCounter.QuadPart - Time_Set->LastCounter.QuadPart);
                      }

                    Time_Set->MsPerFrame = (real64)((1000.0f * (real64)Time_Set->CountsPerFrame) / Time_Set->PerfCountFrequency);

                    // The value of s per frame is too small for float to hold
                    Time_Set->FramePerS = 1000.0f/((real64)Time_Set->MsPerFrame);

                    //Time_Set->SPerFrame = (Time_Set->SPerFrame>0.0f)?Time_Set->SPerFrame:0.0167;

                      if (Time_Set->MsPerFrame > 0.0f) {
                          //SPerFrame = MsPerFrame / 1000;
                          // deltaTime = (float)(1 / 60);
                          Time_Set->SPerFrame = (float)(Time_Set->MsPerFrame/1000);
                      }

                      //Cause the ms per frame is always less than 1000ms that mean it will be less than 0 after being converted to second then;
                      Time_Set->TicksPerFrame = Time_Set->EndCycleCounts - Time_Set->LastCycleCounts;

                      if(Time_Set->SPerFrame > 0.0f){
                          Time_Set->TicksPerS = Time_Set->TicksPerFrame/Time_Set->SPerFrame;
                      }else{
                          Time_Set->TicksPerS = Time_Set->TicksPerFrame*Time_Set->FramePerS;                          
                      };

                      Time_Set->LastCycleCounts = Time_Set->EndCycleCounts;
                      Time_Set->LastCounter = Time_Set->EndCounter;
}

void CalColliInterv(Clock_Set* Time_Set){
    if(Time_Set->current_collided.QuadPart > Time_Set->previous_collided.QuadPart){
        Time_Set->collided_time = ((float)(Time_Set->current_collided.QuadPart - Time_Set->previous_collided.QuadPart))/Time_Set->PerfCountFrequency;
        Time_Set->previous_collided = Time_Set->current_collided;
    }
};

void CalStaticColorDur(Clock_Set* Time_Set, const bool32 On_Flag){
    if(On_Flag){
        if(Time_Set->color_switch_dur >= 0.0f && Time_Set->color_switch_dur < 0.1f){
            Time_Set->color_switch_dur += Time_Set->SPerFrame;
        }else if(Time_Set->color_switch_dur > 0.094f){
            Time_Set->color_switch_dur = 0.0f;
        }
    }
}

// NOTE: Remember premature api optimization is a destructive way to code an api
// because is will cause difficulties for shipping code or changing platform
// and u will waste alot of time in a unnecessary big chunk of code

// TODO: Swap min, max, macros?

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

// NOTE: Keep in mind that try to do all what you need to release back to memory
// in a total thing so that I can release it in aggregate

void ProcessXinputDigitalButton(DWORD XInputButtonState ,Game_Button_State* OldState ,DWORD ButtonBit, Game_Button_State* NewState){
    NewState->EndedDown = ((XInputButtonState & ButtonBit) == ButtonBit);
    NewState->HalfTransitionCount = ((OldState->EndedDown == NewState->EndedDown)? 1 : 0);       
}

void ProcessInput(int maxControllerCount, Game_Input* OldInput, Game_Input* NewInput){
                
    //NOTE: The update window function must afoot outside the getting
    // message block and inside the running block
    for(DWORD ControllerIndex{0}; ControllerIndex < maxControllerCount;
        ControllerIndex++)
    {
        XINPUT_STATE ControllerState;
                    
        if(XinputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS) {
            //NOTE: The controller is plugged in
            Game_Controller_Input* Old_Controller = &OldInput->Controller[ControllerIndex];
            Game_Controller_Input* New_Controller = &NewInput->Controller[ControllerIndex];
                        
            XINPUT_GAMEPAD* Pad = &ControllerState.Gamepad;

            bool32 Up =  (Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
            bool32 Down =  (Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
            bool32 Left =  (Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
            bool32 Right =  (Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);

            New_Controller->IsAnalog = true;
            New_Controller->StartX = Old_Controller->EndX;
            New_Controller->StartY = Old_Controller->EndY;

            //TODO: Mix/Max macros
            real32 X;

            if (Pad->sThumbLX < 0){
                X = (real32)Pad->sThumbLX/ -32768.0f;
            } else {
                X = (real32)Pad->sThumbLX/ 32768.0f;                            
            }

            real32 Y;
            if (Pad->sThumbLY < 0){
                Y = (real32)Pad->sThumbLY/ -32768.0f;
            } else {
                Y = (real32)Pad->sThumbLY/ 32768.0f;                            
            }
                        
            New_Controller->MinY = New_Controller->MaxY = New_Controller->EndY = Y;
                        
            ProcessXinputDigitalButton(Pad->wButtons ,&Old_Controller-> Down ,XINPUT_GAMEPAD_A, &New_Controller-> Down);
            ProcessXinputDigitalButton(Pad->wButtons ,&Old_Controller-> Right ,XINPUT_GAMEPAD_B, &New_Controller-> Right);
            ProcessXinputDigitalButton(Pad->wButtons ,&Old_Controller-> Left ,XINPUT_GAMEPAD_X, &New_Controller-> Left);
            ProcessXinputDigitalButton(Pad->wButtons ,&Old_Controller-> Up ,XINPUT_GAMEPAD_Y, &New_Controller-> Up);
            ProcessXinputDigitalButton(Pad->wButtons ,&Old_Controller-> LeftShoulder ,XINPUT_GAMEPAD_LEFT_SHOULDER, &New_Controller-> LeftShoulder);
            ProcessXinputDigitalButton(Pad->wButtons ,&Old_Controller-> Right ,XINPUT_GAMEPAD_RIGHT_SHOULDER, &New_Controller-> RightShoulder);

                        
            int16 StickX = Pad->sThumbLX;
            int16 StickY = Pad->sThumbLY;
                        
            bool32 B = (Pad->wButtons &XINPUT_GAMEPAD_B);
            bool32 A = (Pad->wButtons &XINPUT_GAMEPAD_A);

        } else {
            //NOTE: The controller is not available
        };
                        
                    
    }
    XINPUT_VIBRATION Vibration;
    Vibration.wLeftMotorSpeed = 350;
    Vibration.wRightMotorSpeed = 350;
    XinputSetState(0, &Vibration);
    
};

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

imagee_content* DEBUGReadBMP(char* filename, debug_read_file_result* ReadResult){
    imagee_content* result = (imagee_content*)malloc(sizeof(struct imagee_content));
    ReadResult = DEBUGReadFileWhole(filename);
     if(ReadResult->Size != 0){

         BITMAP_HEADER *HeadResult = (BITMAP_HEADER*)ReadResult->Content;
         //Why plus ???

         uint32* pixels = (uint32* )((uint8 *)ReadResult->Content + HeadResult->bfOffBits);

         result->Width = HeadResult->Width;
         result->Height = HeadResult->Height;
         result->ImageContent = pixels;

         
         
         //uint32* SourceDest = pixels;
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

glm::vec3 randomRotateAxis_(int rollIndex) {
    std::srand(std::time(0));
    glm::vec3 randomRotateAxis = glm::vec3(0.0f);
  switch (rollIndex) {
  case 0:
    randomRotateAxis = glm::vec3(0.4f * (float)(std::rand() * 2), 0.4f * (float)(std::rand() * 2), 0.0f);
    break;
  case 1:
    randomRotateAxis = glm::vec3(0.4f * (float)(std::rand() * 2.0f), 0.0f, 0.4f * (float)(std::rand() * 2.0f));
    break;
  case 2:
    randomRotateAxis =
        glm::vec3(0.0f, 0.4f * (float)(std::rand() * 2.0f), 0.4f * (float)(std::rand() * 2.0f));
    break;
  default:
    randomRotateAxis = glm::vec3(0.4f * (float)(std::rand() * 2),
                                 0.4f * (float)(std::rand() * 2),
                                 0.4f * (float)(std::rand() * 2));
    break;
  };
  return randomRotateAxis;
}

// NOTE: Why this function can replace the wglGetProcaddress
void* GetAnyGLFuncAddress(const char* name)
{
    void* p = (void*)wglGetProcAddress(name);
    if (!p)
    {
        static HMODULE module =
            LoadLibraryA("opengl32.dll");
        p = (void*)GetProcAddress(module, name);
    }
    return p;
    //May be the wglgetprocaddress doesn't on pc case work I have to use getprocaddress instead
}

void* LoadFunctionFromDLL(const char *DLLName,
                          const char *FuncName) {
    // Create function pointer that needed hot loading for debug
  // LoadDLL
  // Load function address
  // free DLL
    HMODULE  LoadedLib;
    void* function;
    LoadedLib = LoadLibraryA(DLLName);
    if(LoadedLib != NULL){
        function = (void*)GetProcAddress(LoadedLib, FuncName);
    }
    return function;    
//Then turn the void* type to any function type using static converter

};

std::string* load_bin_map(const char* name){

    std::string map_name = name;
    std::ifstream bin_map_reader;
    std::string* map_content = new string;

    bin_map_reader.open(map_name, std::ios::binary | std::ios::ate);
    
    if(bin_map_reader.is_open()){
        int file_size = bin_map_reader.tellg();
        printf("The size of the stream: %d\n", file_size);
        
        bin_map_reader.seekg(bin_map_reader.std::ios::beg);
        map_content->reserve(file_size);
        bin_map_reader.read(&(*map_content)[0], file_size);

        printf("Succeed loading map content\n");
    }else{
        printf("Failed opening map content\n");
    }

    bin_map_reader.close();

    return map_content;
}

