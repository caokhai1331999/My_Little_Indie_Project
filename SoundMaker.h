#if !defined(SOUNDMAKER_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "handmade.h"
#include <initguid.h>
#include <audioclient.h>

#include <combaseapi.h>
#include <strmif.h>
#include <DSound.h> 
#include <mmdeviceapi.h>
#include <uuids.h>
#include <endpointvolume.h>


DEFINE_GUID(IID_IAudioClient,
0x1CB9AD4C, 0xDBFA, 0x4c32, 0xB1, 0x78, 0xC2, 0xF5, 0x9F, 0x64, 0x11, 0x6A);

// =====================================================================
// NOTE: 
#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS,LPUNKNOWN pUnkOuter);
typedef DIRECT_SOUND_CREATE(direct_sound_create);
//==========================================================================

// ======================================================================
// NOTE: 
#define CO_CREATE_INSTANCE(name) HRESULT name(CLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, IID riid, LPVOID* Enumerator);
typedef CO_CREATE_INSTANCE (Co_Create_Instance);
// =====================================================================

// ====================================================================
//NOTE: 
#define ENUM_AUDIO_ENDPOINTS(name) HRESULT name (EDataFlow dataFlow, DWORD        dwStateMask, LPVOID  FAR * ppv);
typedef ENUM_AUDIO_ENDPOINTS (Enum_Audio_Endpoints);
// ===================================================================

LPDIRECTSOUNDBUFFER GlobalSecondBuffer;

struct win32_Sound_OutPut{
    int SamplePerSecond;
    int BytesPerSample;
    // Hert(hz) is cycles per second
    int32 SecondBufferSize;
    uint32 RunningSampleIndex;
    real32 tsine;
    int hz;
    int LatencySampleCount;
    int WavePeriod;
    int SquareWaveCount;
    int ToneVolume;
    // Sample per cycle is SquareWave Period    
}SoundOutPut;


void InitSoundBuffer(HWND* Window, win32_Sound_OutPut* SoundOutPut);

void Win32ClearSoundBuffer(win32_Sound_OutPut* SoundOutPut);
void win32InitDSound(HWND window, int32 SamplePerSecond, int32 SecondBufferSize);
void win32InitCoreAudioSound(HWND window, int32 SamplePerSecond, int32 SecondBufferSize);

void Win32FillSoundBuffer(win32_Sound_OutPut* SoundOutPut, DWORD ByteToLock, DWORD ByteToWrite, Game_Sound_OutPut* SoundSourceBuffer);
void WriteSoundToBuffer(Game_Sound_OutPut* SoundBuffer, win32_Sound_OutPut* SoundOutPut, int16* SSamples);

void GameOutPutSound(Game_Sound_OutPut* SecondSoundBuffer, int Hz);
#define SOUNDMAKER_H
#endif
