#if !defined(SOUNDMAKER_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "handmade.h"

#include <combaseapi.h>
#include <strmif.h>
#include <DSound.h> 
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <endpointvolume.h>
#include <uuids.h>

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

global_variable LPDIRECTSOUNDBUFFER GlobalSecondBuffer;

void Win32FillSoundBuffer(win32_Sound_OutPut* SoundOutPut, DWORD ByteToLock, DWORD ByteToWrite, Game_Sound_OutPut* SoundSourceBuffer);
void Win32ClearSoundBuffer(win32_Sound_OutPut* SoundOutPut);
void win32InitDSound(HWND window, int32 SamplePerSecond, int32 SecondBufferSize);
void win32InitCoreAudioSound(HWND window, int32 SamplePerSecond, int32 SecondBufferSize);


#define SOUNDMAKER_H
#endif
