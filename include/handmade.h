#if !defined(HANDMADE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

/*

  TODO: Services that the platform layer apply to the game

*/

/*

  NOTE: Services that the platform layer provide to the game layer

*/

#if !defined
#define GLAD_LIB
#include <glad/glad_wgl.h>
#include <glad/glad_wgl.c>
//#include <glad/glad.h>
//#include <glad/glad.c>
#endif

#include <Windows.h>
#include <Windowsx.h>
#include <wingdi.h>
#include <xinput.h>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <iostream>

#if !defined
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#endif

using namespace std;

#define ArrayCount(Array) (sizeof(Array) / sizeof(Array[0]))

#if DEBUG
#define Assert(Expression) if(!Expression){* (int* ) 0 = 0;}
#else
#define Assert(Expression)
#endif

#define KILOBYTES(data) (data*1024)
#define MEGABYTES(data) (KILOBYTES(data)*1024)
#define GIGABYTES(data) (MEGABYTES(data)*1024)

#define internal static
#define local_persist static
#define global_variable extern

#define Pi32 3.14159265359f

typedef int16_t int16;
// the number is the max bit count in binary form
// for ex: uint8 max value is : 0x11111111 = 255
typedef int8_t int8;
typedef int32_t int32;
typedef int64_t int64;
typedef uint64_t uint64;

typedef bool bool16;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

typedef float real32;
typedef double real64;

typedef size_t memory_index;

const global_variable int Height = 720;
const global_variable int Width = 1280;

// NOTE: This is all about calling the function in the Xinput.h without the noticing from the compiler
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex,XINPUT_STATE *pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
// NOTE: The second line will be expand out to be like this :
// typedef x_input_get_state(DWORD dwUserIndex,XINPUT_STATE *pState)
// This is to turn on the compiler strict type checking
// And to DECLARE A FUNCTION SIGNATURE AS A TYPE
// for example: x_input_get_state _XinputgetState()
X_INPUT_GET_STATE(XinputGetStateStub) {
    return (ERROR_DEVICE_NOT_CONNECTED);
// NOTE: But the rules of C does not allow this(x_input_get_state _XinputGetStateStub() {//do something;})
}
// so we use this for function pointer
global_variable x_input_get_state* XinputGetState_  = XinputGetStateStub;
// So finally we have a pointer name XinputGetState point to the function
// XinputGetStateStub(DWORD ....) which basically X_INPUT_GET_STATE() function
#define XinputGetState XinputGetState_

// This one is to replace the XinputGetState which already been called in Xinput.h
// with the XinputGetState                                                 
   // ==================================================================
#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex,XINPUT_VIBRATION *pVibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XinputSetStateStub) {
    return (ERROR_DEVICE_NOT_CONNECTED);
}
global_variable x_input_set_state* XinputSetState_  = XinputSetStateStub;
#define XinputSetState XinputSetState_

//NOTE: Get mouse pos
//
//#define GET_X_LPARAM(name) int name(LPARAM lparam)
//typedef GET_X_LPARAM(get_x_lparam);
//GET_X_LPARAM(getxlparam) {
//return (NULL);
//}
//global_variable get_x_lparam* getxlparam_  = getxlparam;
//#define get_x_lparam getxlparam_
//
//#define GET_Y_LPARAM(name) int name(LPARAM lparam)
//typedef GET_Y_LPARAM(get_y_lparam);
//GET_Y_LPARAM(getylparam) {
//return (NULL);
//}
//global_variable get_y_lparam* getylparam_  = getylparam;
//#define get_y_lparam getylparam_
//

// ==================================================================

/*For Bone*/
#include <list>
#include <sstream>
#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#if !defined
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

typedef struct debug_read_file_result{
    uint32 Size;
    void* Content;
}debug_read_file_result;

//NOTE: Pragma pack is to back data of the struct to one byte aligned structure

#pragma pack(push, 1)
struct BITMAP_HEADER{
    // BMP INFO header
    //DWORD biSize;
    //LONG  biWidth;
    //LONG  biHeight;
    //WORD  biPlanes;
    //WORD  biBitCount;
    //DWORD biCompression;
    //DWORD biSizeImage;
    //LONG  biXPelsPerMeter;
    //LONG  biYPelsPerMeter;
    //DWORD biClrUsed;
    //DWORD biClrImportant;

    //BMP header
    uint16 bfType;
    uint32 bfSize;
    uint16  bfReserved1;
    uint16  bfReserved2;
    uint32  bfOffBits;

    uint32 Size;
    int32 Width;
    int32 Height;
    uint16 Planes;
    uint16 BitsPerPixel;
};
#pragma pack(pop)

#define DEBUG_READ_WHOLE_FILE(name) void* name(const char* filename);
typedef DEBUG_READ_WHOLE_FILE(debug_read_whole_file);
#define DEBUG_WRITE_WHOLE_FILE(name) bool32 name(const char* filename, uint32 memorysize, void* memory);
typedef DEBUG_WRITE_WHOLE_FILE(debug_write_whole_file);
#define DEBUG_FREE_FILE_MEMORY(name) void name(void* memory);
typedef  DEBUG_FREE_FILE_MEMORY(debug_free_file_memory);

struct Game_Sound_OutPut{  
    int16 SamplePerSecond;
    int SampleCounts;
    int16* Samples;
};

struct Game_Button_State{
    int HalfTransitionCount;
    bool32 EndedDown;
};

union Game_Controller_Input{
    // NOTE: The union make it child var lie in one place and can be found in one
    // address
    bool32 IsAnalog;
    Game_Button_State State[6]; // This is C type pointer remember that
    
    real32 StartX;
    real32 StartY;

    real32 MinX;
    real32 MinY;

    real32 MaxX;
    real32 MaxY;

    real32 EndX;
    real32 EndY;
    
    struct {
        Game_Button_State Up;
        Game_Button_State Down;
        Game_Button_State Left;
        Game_Button_State Right;
        Game_Button_State LeftShoulder;
        Game_Button_State RightShoulder;
    };
    
};


struct Game_Input{
    Game_Controller_Input Controller[4];
};

//======================MEMORY_PART=========================
#define minimun(a, b) return (a > b)?b:a

// Do I understand how #define keyword work
// so actually the type and arena is indicating the variable
#define DEFAULT_BLOCK_SIZE MEGABYTES(30)

struct ticket_mutex{
    // keep in mind that the volatile is type that can be delared as an object and modified by hardware
    uint64 volatile ticket;
    // serving is current intercepting thread which id is taken from getthreadid.
    uint64 volatile serving;
    // the ticket loop is just waiting until the thread left/retire before the other get in to execute that line of code again.
};

struct memory_arena{
    size_t size;
    size_t used;
    void* base;
    // In term of linear data arrangement the Pad itself is to just separate the memory_block memory address from what come after it.
    uint64 Pad[6];
};

struct memory_block{
    memory_block* prev;
    memory_block* next;

    size_t size;
    size_t used;
    void* base;
    // In term of linear data arrangement the Pad itself is to just separate the memory_block memory address from what come after it.
    uint64 Pad[6];
};


struct Platform_Properties{
        
    BITMAPINFO Bitmapinfo;
    HBITMAP BitmapHandle;

    //Game_State state;

    bool32 SwitchCamera = false;
    bool transferNeed;    
    bool GLImageRendered = false;

    int BitmapWidth;
    int BitmapHeight;
    int Pitch;
    int BitmapMemorySize; 

    HWND Window;
    RECT ClientRect;
    void* BitmapMemory;
    void* BitmapMemoryForDirectBlit;

    MSG Message;
    WNDPROC wndproc;

    ticket_mutex ticket;
};

global_variable Platform_Properties Game_Platform = {};

local_persist uint64 AtomicAddUint64(uint64* addend, uint64 value);
global_variable void begin_ticket_mutex(ticket_mutex* mutex);
global_variable void end_ticket_mutex(ticket_mutex* mutex);

local_persist uint64 AtomicAddUint64(uint64 volatile *addend, uint64 value){
// use this to create threadId based ticket and loop through them.
    // until it retire in order.
    // Cause this one very fast(cpu level). --> it ensure that no 2 threads can have the same ticket numbers
    // This one is just the order that a thread hit this line, all of these satisfy the M.E.S.I protocol
    uint64 value_ = _InterlockedExchangeAdd((long*)addend, value);
    return value_;
}

global_variable void begin_ticket_mutex(ticket_mutex* mutex){
    uint64 ticket = AtomicAddUint64(&mutex->ticket, 1);
    // mutex->ticket is now auto change
    // But why when the ticket equal to ticket thread id that we know it get out.
    // 
        while(ticket != mutex->serving);
}

global_variable void end_ticket_mutex(ticket_mutex* mutex){
    AtomicAddUint64(&mutex->serving, 1);
    // Whenever the ticket equal to the threadId that mean the thread get out of code lines and bring instruction to the core
}
// ZII
void init_arena_memory(memory_arena* arena, size_t init_size){
    arena->size = init_size;
    arena->used = 0;
}

// apply to grow vertex array
void ALLOCATE_BLOCK_MEMORY(memory_arena* arena, size_t size){
        // why plus one
        begin_ticket_mutex(&Game_Platform.ticket);
        uint8* result = (uint8*)VirtualAlloc(arena->base, size + sizeof(memory_arena), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
        end_ticket_mutex(&Game_Platform.ticket);
        arena->base = result;
}

/*
void* ALLOCATE_BLOCK_MEMORY(memory_block* mem, size_t size){
        memory_block* block = (memory_block*)VirtualAlloc(block->base, size + sizeof(memory_block), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
        // why plus one

        block->next = mem->next; 
        block->prev = mem; 

        begin_ticket_mutex(&BackBuffer.mutex);
        block->next->prev = block;
        block->prev->next = block;
        end_ticket_mutex(&BackBuffer.mutex);

        void* result = block->base + 1;
        return result;
}
*/


bool32 DEALLOCATE_BLOCK_MEMORY(memory_arena* arena){
    bool32 result;
    if(arena->base){
        result = VirtualFree(arena->base, arena->size, MEM_COMMIT|MEM_RESERVE);
    };
    return result;
}

/*
void DEALLOCATE_BLOCK_MEMORY(memory_block* mem){
    if(mem){
        memory_block* block = ((memory_block*)mem - 1);
        block->prev->next = block->next;
        block->next->prev = block->prev;

        VirtualFree(block->base, block->size, MEM_COMMIT|MEM_RESERVE);
    };
}
 */
// apply to grow vertex array

void init_mem_region(size_t size, memory_block * arena){
    arena->size = size;
    arena->base = (uint8*)VirtualAlloc(arena->base, arena->size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE );
}

void free_mem_region(memory_block* arena){
    if(arena->base)
    VirtualFree(arena->base, arena->size, MEM_COMMIT|MEM_RESERVE);
}

//inline memory_index Get_Alignment_Offset(memory_arena* block, size_t alignment){
    //;
//}

/*
void* push_size_(size_t size, memory_arena* arena, ticket_mutex* mutex){
    // whenever the total requested size if bigger than the current block size: allocate new space and copymemory of the old block
    memory_index alignment_mask = Get_Alignment_Offset(, 4);
    if(arena->used + size >= arena->size){
        begin_ticket_mutex(mutex);
        arena->size += minimum_block_size;
        arean->used += size;
        ALLOCATE_BLOCK_MEMORY(arena->base, minimum_block_size);
        void* result = arena->base + used;
        end_ticket_mutex(mutex);
        assert(result);
        //CopyMemory();
    return result;
}
*/

/*
void* push_size_(size_t size, memory_block* sentinel, ticket_mutex* mutex){
    void* result;
    // whenever the total requested size if bigger than the current block size: allocate new space and copymemory of the old block
    if(sentinel->used + size >= sentinel->size){
        begin_ticket_mutex(mutex);
        memory_block* new_block = (memory_block*)ALLOCATE_BLOCK_MEMORY(sentinel, (size_t)DEFAULT_BLOCK_SIZE);
        // casey lock it inside the something call tick mutex, to prevent any one/app else use these kind of thread while it's on working.
// This one is not thread-safe
        // so currently, we haven't touch this growing aray yet.
        // focus on draw scene and load gl pointer on little beast.

        result = new_block->base + size;
        new_block->used += size;
        end_ticket_mutex(mutex);
        // How can i access these memory in pool using index
        //CopyMemory();
    } else {
        sentinel->used += size;
        result = sentinel->base + sentinel->used;
    };
    return result;
}
*/

struct Game_Memory{

    bool32 IsInitialized; 
    uint64 PermanentStorageSize;
    void* PermanentStorage;
    uint64 TransientStorageSize;
    void* TransientStorage;
};

// set memory here
// replace by copy_memory of window.

#define push_size(type, arena) (type* )push_size_(sizeof(type), arena)
#define push_array(type, count, arena) (type* )push_size_(count * sizeof(type), arena)

struct imagee_content{
    int32 Width;
    int32 Height;
    uint32* ImageContent;
};

struct Game_State{
    int BlueOffset = 0;
    int GreenOffset = 0;
    // Memory_Block sentinel;
    int Hz = 256;    
};

struct win32Dimension{
    int PosX{0};
    int PosY{0};
    int Height{720};
    int Width{1280};
}Dimens;


struct Clock_Set{
  // This one is how many count per Second
    LARGE_INTEGER PerfCountFrequencyResult = {};
    int64 PerfCountFrequency = 0;

    LARGE_INTEGER LastCounter = {};
    LARGE_INTEGER EndCounter = {};

    LARGE_INTEGER previous_collided = {};
    LARGE_INTEGER current_collided = {};

    float collided_time = 0.0f;
    float color_switch_dur = 0.0f;

    uint64 LastCycleCounts = 0;
    uint64 EndCycleCounts = 0;


    float TimeCounter = 0.0f;
    float WaitTimeCounter = 0.0f;

    uint64 TicksPerFrame = 0;
    uint64 TicksPerS = 0;

    int64 CountsPerFrame = 0;
    //So what will be less than 0 must be float type
    float MsPerFrame = 0.0f;
    float SPerFrame = 0.0f;
    float FramePerS = 0.0f;
    // Time elapsed of one cycle/frame in second

    //bool RatioCalculated = false;

    float DelayedRatio = 0.0f;
    float StandardMSperFrame = 16.67f;

    float ChangeAxisCounter = 0.0f;
    float TimeToChangeAxis = 0.0f;
    int64 ViewRotateCount = 0;

    uint64 Pad[6];
};


struct Rect_{
    int x, y, w, h;
};

struct OpenGLData{
    unsigned int VAOs;
    unsigned int PlaneVAOs;

    unsigned int VBO;
    unsigned int ColorVBO;
    unsigned int PlaneVBO;  

    std::vector<GLuint> ProgramIDs = {};
    //std::vector<*B_shader_program> ProgramIDs = {};
    
    HGLRC openglRC;
    HGLRC defaultContext;

    unsigned int textureHandle;
    std::vector<unsigned int>* texture_id_list;
    OpenGLData(){
        //Maybe buggy this part
        // need to be careful
        VAOs     = 0;       
        PlaneVAOs = 0;
        VBO      = 0;     
        ColorVBO = 0;
        PlaneVBO = 0;
        textureHandle = 0;
    }
};

bool isNull(GLuint* member = nullptr);
void PassGLData(OpenGLData* BackData, OpenGLData* FrontData);

global_variable bool GlobalRunning = true;

struct Per_Win_Properties{
    WNDCLASSEXA WindowClass;
    HWND Window;
    Per_Win_Properties(){
        WindowClass = {};
        Window = {};
    };
};

struct Glyph_Property{
    //unsigned char* bitmap;
    void* upside_down_bitmap;
    int w,h,i,j,c , Xoffset, Yoffset;
};

struct Glyph_Map{
    std::vector<Glyph_Property*>Glyph_list;
    stbtt_fontinfo FontInfo;
    unsigned int TextureID;
    Glyph_Map(){
        Glyph_list.reserve(100);
        TextureID = 0;
        FontInfo = {};
    }
};

//Glyph_Map Glyphs_Map = {};


// TODO: Allow the sample offset here for more robust platform options
uint32 safetruncateUint64(uint64 value);
uint32 safetruncateUint64(uint64 value){
    //Assert(value <= 0xFFFFFFFF);
    uint32 result = value;
    return result;
}

real32 saferatioN(real32 numerator, real32 divisor);
real32 saferatio0(real32 numerator, real32 divisor);
real32 saferatio1(real32 numerator, real32 divisor);


void CalEarlyFrameTime(Clock_Set* Time_Set = nullptr);
void CalColliInterv(Clock_Set* Time_Set = nullptr);
void CalStaticColorDur(Clock_Set* Time_Set, const bool32 On_Flag);

void* PlatformLoadFile(char* FileName);
debug_read_file_result* DEBUGReadFileWhole(const char* filename);
imagee_content* DEBUGReadBMP(const char* filename, debug_read_file_result* result);
//BMP_content* DEBUGReadJPG(char* filename, debug_read_file_result* result);
bool32 DEBUGWriteWholeFile(char* filename, uint32 memorysize, void* memory);
void DEBUGFreeFileMemory(void* memory);

global_variable debug_read_file_result* DEBUGReadFileWhole(const char* filename){
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
                    printf("Read file successfully\n");
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

imagee_content* DEBUGReadBMP(const char* filename, debug_read_file_result* ReadResult){
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

void win32LoadXInput(void);
void ProcessXinputDigitalButton(DWORD XInputButtonState ,Game_Button_State* OldState ,DWORD ButtonBit, Game_Button_State* NewState);
void ProcessInput(int maxControllerCount, Game_Input* OldInput, Game_Input* NewInput);

void* GetAllFunctionPointerFromLib(HMODULE lib = NULL, const char* name = nullptr);
void *GetAnyGLFuncAddress(const char *name);
// NOTE: Why this function can replace the wglGetProcaddress
void* GetAnyGLFuncAddress(const char* name)
{
    void* p = (void*)wglGetProcAddress(name);
    if (!p)
    {
        static HMODULE module =
           LoadLibraryA("opengl32.dll");
        p = (void*)GetProcAddress(module, name);
        //p = (void*)wglGetProcAddress(name);
    }
    return p;
    //May be the wglgetprocaddress doesn't on pc case work I have to use getprocaddress instead
};

void* LoadFunctionFromDLL(const char* DLLName = nullptr, const char* FuncName = nullptr);
void SetUpWindow();
glm::vec3 randomRotateAxis_(int rollIndex);
bool32 string_contain(std::string* string = nullptr, char* substr = nullptr);

bool32 string_contain(std::string* string, char* substr){
    int substrIndex = 0;
    while (substrIndex < sizeof(*substr)){
        if (string->find(substr[substrIndex])==string->npos){
            return false;
        } else {
            substrIndex++;
        }
    };
    return true;
};

std::string* load_bin_map(const char* name = nullptr);

#define HANDMADE_H
#endif
