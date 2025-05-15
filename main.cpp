/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "win32Game.h"

int CALLBACK WinMain
(HINSTANCE Instance,
 HINSTANCE hInstPrev,
 PSTR cmdline,
 int cmdshow)
{    
    printf("Track to here\n");
    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceCounter(&PerfCountFrequencyResult);
    // NOTE: Actually, this the counts per second
    // TODO: Try to find out why the PerfCountFrequency is too large.

    // NOTE: This one count is for counting the frame
    int64 PerfCountFrequency = (int64)(PerfCountFrequencyResult.QuadPart);                
    win32LoadXInput();
    WNDCLASSA WindowClass = {};
    WindowClass.lpfnWndProc = MainWindowCallBack;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "First Game Window Class";
    Win32ResizeDIBSection(&BackBuffer, Dimens.Height, Dimens.Width);

    if(RegisterClassA(&WindowClass)) {
        
        Window = CreateWindowExA(
            // NOTE: The window didn't show up is because the first argument
            0,
            WindowClass.lpszClassName,
            "win32GameWithoutEngine",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            Instance ,
            0);
        // Init here
        InitOpenGL(Window);

        if(Window) {
            GlobalRunning = true; 
            //NOTE: we create a second buffer last for 2 second with

            // int16* SSamples = nullptr;
            // NOTE: Don't call _alloc in the app loop it cause bug (it doesn't clean up entirely but just barely in the function)

            
            // win32_Sound_OutPut SoundOutPut = {};
            SoundOutPut.SamplePerSecond = 48000;
            SoundOutPut.RunningSampleIndex = 0;
            // SoundOutPut.tsine = 0.0f;
            // SoundOutPut.hz = 128;
            // SoundOutPut.WavePeriod = SoundOutPut.SamplePerSecond/SoundOutPut.hz;
            SoundOutPut.LatencySampleCount = SoundOutPut.SamplePerSecond / 15;
            // SoundOutPut.SquareWaveCount = 0;
            SoundOutPut.ToneVolume = 3500;
            SoundOutPut.BytesPerSample = sizeof(int16)*2;
            // Hert(hz) is cycles per second
            SoundOutPut.SecondBufferSize = 2*SoundOutPut.BytesPerSample*SoundOutPut.SamplePerSecond;
            
            int16* SSamples = (int16* )VirtualAlloc(0 , SoundOutPut.SecondBufferSize ,MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
            
            win32InitDSound(Window, SoundOutPut.SamplePerSecond, SoundOutPut.SecondBufferSize);
            Win32ClearSoundBuffer(&SoundOutPut);
            OutputDebugStringA("Sound is playing");

            LARGE_INTEGER LastCounter;
            QueryPerformanceCounter(&LastCounter);
            uint64 LastCycleCounts;
            
            Game_Input Input[2] = {};
            Game_Input* OldInput = &Input[0];
            Game_Input* NewInput = &Input[1];

            LastCycleCounts = __rdtsc();
            
            while(GlobalRunning) {
                MSG Message;
                // NOTE: This is where receiving the message to change
                // for any change in window
                while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE)) {
                    if (Message.message == WM_QUIT) {
                        GlobalRunning = false;
                    }
                    DispatchMessage(&Message);
                    TranslateMessage(&Message);
                }

                int MaxControllerCount = XUSER_MAX_COUNT;
                if( MaxControllerCount > ArrayCount(Input->Controller)) {
                    MaxControllerCount = ArrayCount(Input->Controller);   
                }
                
                // NOTE: The update window function must afoot outside the getting
                // message block and inside the running block
                for(DWORD ControllerIndex{0}; ControllerIndex < MaxControllerCount;
                    ControllerIndex++)
                {
                    XINPUT_STATE ControllerState;
                    
                    if(XinputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS) {
                        // NOTE: The controller is plugged in
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

                        // TODO: Mix/Max macros
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
                        // NOTE: The controller is not available
                    };
                        
                    
                }
                XINPUT_VIBRATION Vibration;
                Vibration.wLeftMotorSpeed = 350;
                Vibration.wRightMotorSpeed = 350;
                XinputSetState(0, &Vibration);

                // NOTE: Check whether OpenGL work or not
                // Define the boundary of what we want to render
                printf("Draw something here\n");
                glViewport(0, 0, 70, 40);
                glClearColor(1.0f, 0.5f, 0.75f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                // Display on the screen
                SwapBuffers(DeviceContext);

                // RenderSplendidGradient(&BackBuffer, XOffset, YOffset);

                // ===========================================================
                // NOTE: The writting cursor create data and the play one will pick
                // everyone of them and send to sound card to make sound .
                // One write one read just like a chase between a cat and a mouse.
                // Once you hit play 'cursor position right now you have to stop
                // the writting somewhere otherwise the newly date will overwrite
                // whatever the play cursor want to read

                
                // NOTE: We need to constantly ask the where we are in sound chase
                // and fill properly in the regions
                DWORD PlayCursor;
                DWORD WriteCursor;
                DWORD ByteToLock;
                DWORD ByteToWrite;
                DWORD TargetCursor;
                bool32 SoundIsValid = false ;
                if(SUCCEEDED(GlobalSecondBuffer->GetCurrentPosition(&PlayCursor,            &WriteCursor))) {
                    
                    ByteToLock = (SoundOutPut.RunningSampleIndex* SoundOutPut.BytesPerSample)% SoundOutPut.SecondBufferSize;
                    // % for the secondBufferSize is the move to wrap around the buffer
                    TargetCursor = ((PlayCursor + (SoundOutPut.LatencySampleCount * SoundOutPut.BytesPerSample)) % SoundOutPut.SecondBufferSize);

                    // TODO: Collapse these two loops
                    // The bugs is we didn't catch up the play cursor yet
                    // TODO: Change this to using a lower latency offset from the
                    // playcursor when we actually start having sound effect
                    
                    if(ByteToLock > TargetCursor){
                        // 
                        ByteToWrite = (SoundOutPut.SecondBufferSize - ByteToLock); // Region 1
                        ByteToWrite += TargetCursor;                   // Region 2
                    } else {
                        // when the requested size fit in buffer when there only region 1 is active
                        // In this case,one situation is that The ByteToWrite is 0 and wait for the next turn of the loop                        
                        ByteToWrite = TargetCursor - ByteToLock; // Region 1
                    }                    
                    SoundIsValid  = true; 
                }                

                // NOTE: this function throw this memory on the stack and I know
                // it will go away when it function is done
                Game_Sound_OutPut SoundBuffer = {};
                SoundBuffer.SamplePerSecond = SoundOutPut.SamplePerSecond;
                SoundBuffer.SampleCounts = ByteToWrite/SoundOutPut.BytesPerSample;
                SoundBuffer.Samples = nullptr;
                SoundBuffer.Samples = SSamples;
                
                Game_OffScreen_Buffer ScreenBuffer = {};
                ScreenBuffer.BitmapMemory = BackBuffer.BitmapMemory;
                ScreenBuffer.BitmapWidth = BackBuffer.BitmapWidth;
                ScreenBuffer.BitmapHeight = BackBuffer.BitmapHeight;
                ScreenBuffer.Pitch = BackBuffer.Pitch;
                GlobalSecondBuffer->Play( 0, 0, DSBPLAY_LOOPING);
                // NOTE: Don't know why compiler couldn't find this function
                // implementation after a little remove of few arguments
                
                GameUpdateAndRender(NULL, NewInput, &ScreenBuffer, &SoundBuffer);
                
                // TODO: This function just being called once
                

                if (SoundIsValid){
                    // TODO: Devle more about why I had to mod SecondBufferSize
                    // to Byte based on index to create bytetolock

                    Win32FillSoundBuffer(&SoundOutPut, ByteToLock, ByteToWrite, &SoundBuffer);
                }                                                    
                                
                // =============================================================
                if(Message.message != WM_KEYDOWN && Message.message != WM_KEYUP)
                {
                    // XOffset++;
                }
                
                DeviceContext = GetDC(Window);                                    
                // RenderSplendidGradient(&BackBuffer, XOffset, YOffset);
                Win32DisplayBufferWindow(DeviceContext, Dimens.Width, Dimens.Height, &BackBuffer);
                
                LARGE_INTEGER EndCounter;
                QueryPerformanceCounter(&EndCounter);

                uint64 EndCycleCounts;
                EndCycleCounts = __rdtsc();

                //  __rdtsc() is an intrinsict (the one which looked like a function call
                // but it actually a hint to the compiler to a specific dissembly language intstruction)
                //
                // S : Single 
                // I : Instruction
                // M : Multiple
                // D : Data
                
                
                uint64 CyclesElapsed = EndCycleCounts - LastCycleCounts;
                // NOTE: It based on the var type to decide what kind of the substraction to do
                real32 ElapsedCounter = (real32)((real32)(EndCounter.QuadPart) - (real32)(LastCounter.QuadPart));
                real32 McPerFrame = (real32)((real32)CyclesElapsed/(1000.f * 1000.f));
                real32 MsPerFrame = (real32)((1000 * (real32)ElapsedCounter) / (real32)PerfCountFrequency);
                real32 FPS = (real32)((real32)PerfCountFrequency/(real32)ElapsedCounter);
#if 0                
                char Buffer[256];
                // NOTE: The '%' is to decide the format of the next thing to print
                // for example: %d is the 32 bit integer
                sprintf(Buffer, "%f Miliseconds/Frame, %f FPS, %f Mc/f \n ", MsPerFrame, FPS, MsPerFrame);
                OutputDebugStringA(Buffer);
#endif                
                LastCounter = EndCounter;
                LastCycleCounts = EndCycleCounts;
                // MULPD -> real32 ==> 128 bits / 32 bits -> 4 real32 packs per register 
                // MULPS -> real64 ==> 128 bits / 64 bits -> 2 real32 packs per register  
                // ReleaseDC(Window, DeviceContext);

                Game_Input* Temp = NewInput;
                NewInput = OldInput; // ???? still don't understand
                OldInput = Temp;
            }
            
        }else{
            // TODO: Logging
        }
    } else {
        // TODO: Logging
    }   
    return (0);
}

