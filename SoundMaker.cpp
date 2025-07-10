
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "SoundMaker.h"

// ============================================================================
//NOTE: DONE Practice using coreaudio and multimedia api instead of directsound
//Now it's time to write real wave
 void win32InitCoreAudioSound(HWND window, int32 SamplePerSecond, int32 SecondBufferSize) {
    //NOTE:As the mentor said I have the output the sound ahead of a frame
     //to make it work on time
    
    HMODULE CombaseapiLibrary = LoadLibraryA("combase.dll");
    HMODULE MmdeviceapiLibrary = LoadLibraryA("mmdevice.dll");

     //NOTE: Load the library
    if (CombaseapiLibrary && MmdeviceapiLibrary) {
        //NOTE: Get access to the IMMDeviceEnumerator api through..
       //NOTE: Seem like I didn't understand shit. The GetProcAddress must work
       // To retrieve the address and I have to assigned to the pointer

        Co_Create_Instance* CoCreateInstance = (Co_Create_Instance* ) GetProcAddress(CombaseapiLibrary, "CoCreateInstance");

        const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
        const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
        //const IID IID_IAudioClient = __uuidof()
        
        IMMDeviceEnumerator* pEnumerator = nullptr;

 // Initialize COM library
//NOTE: Create a IMMDeviceEnumerator instance
        if (CoCreateInstance && (SUCCEEDED(CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)pEnumerator)))) {
            //NOTE: pEnumerator is a pointer to the IMMDeviceEnumerator
            //NOTE: Get the IMMDeviceCollection api through IMMDeviceEnumerator::
            IMMDeviceCollection *ppDevicesl;
            if (SUCCEEDED(pEnumerator->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE, &ppDevicesl)))
            {   
                IMMDevice *ppDevice;
//NOTE: Get the IMMDevice by calling GetDevice
                if(SUCCEEDED(ppDevicesl->Item(0, &ppDevice))) {
                    //NOTE: Activate the IMMDevice
                    IAudioClient* ppInterface1;
// IID_IAudioClient???
                    if(SUCCEEDED(ppDevice->Activate( IID_IAudioClient, CLSCTX_ALL, NULL, (void**)ppInterface1))) {
                        //NOTE: Initialize the IMMDevice
                        WAVEFORMATEX* pFormat;
                        //NOTE: Now set the format                          
                        pFormat->wFormatTag = WAVE_FORMAT_PCM;
                        pFormat->nChannels = 2;
                        pFormat->nSamplesPerSec = SamplePerSecond;
                        pFormat->wBitsPerSample = 16;
                        //NOTE: Basic thing: Product of is result of multiplying
                        pFormat->nBlockAlign = (pFormat->nChannels * pFormat->wBitsPerSample)/8;
                        pFormat->nAvgBytesPerSec = (pFormat->nSamplesPerSec * pFormat->nBlockAlign); 
                        pFormat->cbSize = 0;

                        if (SUCCEEDED(ppInterface1->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, 0, 2, 0, pFormat, NULL))) {
                            //NOTE: Buffer that has a one second duration
                             // GetBuffer and ReleaseBuffer is correspond to the lock and unlockBuffer in the directSound one
                        } else {
                            //TODO: Do a diagnoses
                        }
                        IAudioClient* ppInterface2 = nullptr;
                         //If I wasn't wrong the interface stand for buffer
                        //NOTE: Then may be get buffer size
                        if(SUCCEEDED(ppDevice->Activate( IID_IAudioClient, CLSCTX_ALL, NULL, (void**)ppInterface2))) {
                
                        } else {
                            //TODO: Do a diagnoses                  
                        }
            
                    }else {
                        //TODO: Do a diagnoses                  
                    }          
                } else {
                    //TODO: Do a diagnoses
                }
            }else{
                //TODO: Do a diagnoses
            }
        
        } else {
            //TODO: Do a diagnostic        
        }

    }
}


// ========================================================================
 void win32InitDSound(HWND window, int32 SamplePerSecond, int32 SecondBufferSize) {
    // NOTE:As the mentor said I have the output the sound ahead of a frame
    // to make it work on time
    
    //NOTE: Load the library
    HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");
    if (DSoundLibrary) {        
        //NOTE: Create the DSound object - cooperative
        direct_sound_create* DirectSoundCreate = (direct_sound_create* )            GetProcAddress(DSoundLibrary, "DirectSoundCreate");
        LPDIRECTSOUND DirectSound ;
        if (DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound,
                                                             0))) {
            WAVEFORMATEX WaveFormat = {};
                    
            WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
            WaveFormat.nChannels = 2;
            WaveFormat.nSamplesPerSec = SamplePerSecond;
            WaveFormat.wBitsPerSample = 16;
            //NOTE: Basic thing: Product of is result of multiplying
            WaveFormat.nBlockAlign = (WaveFormat.nChannels *
                                      WaveFormat.wBitsPerSample)/8;
            WaveFormat.nAvgBytesPerSec = (WaveFormat.nSamplesPerSec *
                                          WaveFormat.nBlockAlign); 
            WaveFormat.cbSize = 0;
            
            //   =======================================================
            //NOTE: Primary Buffer
            if(SUCCEEDED(DirectSound->SetCooperativeLevel(window,
                                                          DSSCL_PRIORITY))) {
                //NOTE: Little trick here to clear all the struct member to zero
                DSBUFFERDESC BufferDescription = {};
                BufferDescription.dwSize = sizeof(BufferDescription);
                BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;    
                LPDIRECTSOUNDBUFFER PrimaryBuffer;
                
                //NOTE: Create a primary buffer
                if(SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription,
                                                            &PrimaryBuffer, 0))) {
                    OutputDebugStringA("Primary sound buffer was create successfully/n");                    
                    BufferDescription.dwBufferBytes = 0;
                    
                    if((PrimaryBuffer->SetFormat(&WaveFormat)) == DS_OK) {
                       //NOTE: Or
                         if(SUCCEEDED(PrimaryBuffer->SetFormat(&WaveFormat))) {
                        OutputDebugStringA("Primary sound buffer was set/n");                        
                        //NOTE: Start it playing
                    }else {
                        //TODO: Do a diagnostic                   
                    }
                }
                        
            } else {
                //TODO: Do a diagnostic
            }
                    
            //   ===========================================================

            //NOTE: Then the second one
            if(SUCCEEDED(DirectSound->SetCooperativeLevel(window,
                                                          DSSCL_PRIORITY))) {
                //NOTE: Create a secondary buffer
                DSBUFFERDESC BufferDescription = {};
                BufferDescription.dwSize = sizeof(BufferDescription);
                BufferDescription.dwFlags = 0;
                BufferDescription.dwBufferBytes = SecondBufferSize;                    
                BufferDescription.lpwfxFormat = &WaveFormat;
                                
                if(SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription,
                                                            &GlobalSecondBuffer, 0))) {
                    OutputDebugStringA("Secondary sound buffer was created successfully/n");                                        
                }
                else {
                    //TODO: Do a diagnostic
                }
                
            } else {
                //TODO: Do a diagnostic
            }
            //  =================================================================
        
        } else {
            //TODO: Do a diagnostic
        }
        
    } else {
         //TODO: Do a diagnostic        
    }
    }
}

void InitSoundBuffer(HWND* Window, win32_Sound_OutPut* SoundOutPut){
    //NOTE: we create a second buffer last for 2 second with
    //NOTE: Don't call _alloc in the app loop it cause bug (it doesn't clean up entirely but just barely in the function)
    SoundOutPut->SamplePerSecond = 48000;
    SoundOutPut->RunningSampleIndex = 0;
    SoundOutPut->tsine = 0.0f;
    SoundOutPut->hz = 128;
    SoundOutPut->WavePeriod = SoundOutPut->SamplePerSecond/SoundOutPut->hz;
    SoundOutPut->LatencySampleCount = SoundOutPut->SamplePerSecond / 15;
    SoundOutPut->SquareWaveCount = 0;
    SoundOutPut->ToneVolume = 3500;
    SoundOutPut->BytesPerSample = sizeof(int16)*2;
    //Hert(hz) is cycles per second
    SoundOutPut->SecondBufferSize = 2*SoundOutPut->BytesPerSample*SoundOutPut->SamplePerSecond;
            
            
    win32InitDSound(*Window, SoundOutPut->SamplePerSecond, SoundOutPut->SecondBufferSize);
    Win32ClearSoundBuffer(SoundOutPut);
    OutputDebugStringA("Sound is playing");
}

void WriteSoundToBuffer(Game_Sound_OutPut* SoundBuffer, win32_Sound_OutPut* SoundOutPut, int16* SSamples){
                    
    //NOTE: The writting cursor create data and the play one will pick
/*               everyone of them and send to sound card to make sound .
                 One write one read just like a chase between a cat and a mouse.
                 Once you hit play 'cursor position right now you have to stop
                 the writting somewhere otherwise the newly date will overwrite
                 whatever the play cursor want to read
*/
                
    //NOTE: We need to constantly ask the where we are in sound chase
    //and fill properly in the regions
    DWORD PlayCursor;
    DWORD WriteCursor;
    DWORD ByteToLock;
    DWORD ByteToWrite;
    DWORD TargetCursor;
    bool32 SoundIsValid = false ;

    
    //Constantly write sound===============================
    if(SUCCEEDED(GlobalSecondBuffer->GetCurrentPosition(&PlayCursor,            &WriteCursor))) {
                    
        ByteToLock = (SoundOutPut->RunningSampleIndex* SoundOutPut->BytesPerSample)% SoundOutPut->SecondBufferSize;
        //% for the secondBufferSize is the move to wrap around the buffer
        TargetCursor = ((PlayCursor + (SoundOutPut->LatencySampleCount * SoundOutPut->BytesPerSample)) % SoundOutPut->SecondBufferSize);

/*
//TODO: Collapse these two loops
The bugs is we didn't catch up the play cursor yet
//TODO: Change this to using a lower latency offset from the
playcursor when we actually start having sound effect
*/                    

        if(ByteToLock > TargetCursor){
                         
            ByteToWrite = (SoundOutPut->SecondBufferSize - ByteToLock);  //Region 1
            ByteToWrite += TargetCursor;
            //Region 2
        } else {
            //when the requested size fit in buffer when there only region 1 is active
            //In this case,one situation is that The ByteToWrite is 0 and wait for the next turn of the loop                        
            ByteToWrite = TargetCursor - ByteToLock;  //Region 1
        }                    
        SoundIsValid  = true; 
    }                


    SoundBuffer->SamplePerSecond = SoundOutPut->SamplePerSecond;
    SoundBuffer->SampleCounts = ByteToWrite/SoundOutPut->BytesPerSample;
    SoundBuffer->Samples = SSamples;                
                
    //NOTE: this function throw this memory on the stack and I know
    //it will go away when it function is done
    GlobalSecondBuffer->Play( 0, 0, DSBPLAY_LOOPING);

    //NOTE: Don't know why compiler couldn't find this function
    //implementation after a little remove of few arguments
                
    //TODO: This function just being called once
    if (SoundIsValid){
        //TODO: Devle more about why I had to mod SecondBufferSize
        //to Byte based on index to create bytetolock
        Win32FillSoundBuffer(SoundOutPut, ByteToLock, ByteToWrite, SoundBuffer);
    }                                                    
}


void Win32ClearSoundBuffer(win32_Sound_OutPut* SoundOutPut){
    VOID* Region1;
    DWORD Region1Size;
    VOID* Region2;
    DWORD Region2Size;

    if(SUCCEEDED(GlobalSecondBuffer->Lock(0, SoundOutPut->SecondBufferSize, &Region1, &Region1Size,&Region2, &Region2Size,DSBLOCK_FROMWRITECURSOR))){
        uint8* SampleOut = (uint8* )Region1;
        //NOTE: Basically what we want to do is remembering where
        //we were and how many sound we're outputting and able
        //to lock the buffer at whatever we left off
                        
        for (DWORD SampleIndex{0};
             SampleIndex < Region1Size;
             SampleIndex++){                            
            *SampleOut++ = 0;                            
        }
                        
        SampleOut = (uint8* )Region2;                        
        for (DWORD SampleIndex{0};
             SampleIndex < Region2Size;
             SampleIndex++){                            
            *SampleOut++ = 0;
        }                                                GlobalSecondBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);        
    }    
}

 void Win32FillSoundBuffer(win32_Sound_OutPut* SoundOutPut, DWORD ByteToLock, DWORD ByteToWrite, Game_Sound_OutPut* SoundSourceBuffer){
                    VOID* Region1;
                    DWORD Region1Size;
                    VOID* Region2;
                    DWORD Region2Size;
                    
                    if(SUCCEEDED(GlobalSecondBuffer->Lock(ByteToLock, ByteToWrite, &Region1, &Region1Size,&Region2, &Region2Size,DSBLOCK_FROMWRITECURSOR))){

                        DWORD Region1SampleCounts = Region1Size/SoundOutPut->BytesPerSample;
                        int16* SampleOut = (int16* )Region1;
                        int16* SourceSample = SoundSourceBuffer->Samples;
                        //SoundSourceBuffer->Samples;
                        //NOTE: Basically what we want to do is remembering where
                        //we were and how many sound we're outputting and able
                        //to lock the buffer at whatever we left off
                        
                        for (DWORD SampleIndex{0};
                             SampleIndex < Region1SampleCounts;
                             SampleIndex++){

                            //if (SquareWaveCounter){
                            //     SquareWaveCounter = SquareWavePeriod;
                            // }
                            //NOTE: This formula is to produce square wave
                            //int16 SampleValue = ((RunningSampleIndex++ /            (SquareWavePeriod/2))% 2) ? ToneVolume : -ToneVolume;
                            
                             real32 SineValue = sinf(SoundOutPut->tsine);            
                            *SampleOut++ = *SourceSample++;
                            *SampleOut++ = *SourceSample++;
                            // --SquareWaveCounter;
/*                            
                              NOTE: In order to avoid glitch instead of using
                              sinewave and then plus it to however far we want
                              GlobalRunning sampleindex (cause this var change over the time) we store where we are in
*/                            
                             SoundOutPut->tsine += 2.0f*Pi32* 1.0f/(real32)SoundOutPut->WavePeriod;
                            ++SoundOutPut->RunningSampleIndex;
                        }
                        
                        SampleOut = (int16* )Region2;
                        DWORD Region2SampleCounts = Region2Size/SoundOutPut->BytesPerSample;                        
                        for (DWORD SampleIndex{0};
                             SampleIndex < Region2SampleCounts;
                             SampleIndex++){
                            
                             //int16 SampleValue = ((RunningSampleIndex++/          (SquareWavePeriod/2))% 2) ? ToneVolume : -ToneVolume;
                             real32 SineValue = sinf(SoundOutPut->tsine);
                            *SampleOut++ = *SourceSample++;
                            *SampleOut++ = *SourceSample++;
                             SoundOutPut->tsine += 2.0f*Pi32* 1.0f/(real32)SoundOutPut->WavePeriod;
                            ++SoundOutPut->RunningSampleIndex;                            
                        }                                                GlobalSecondBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);
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
