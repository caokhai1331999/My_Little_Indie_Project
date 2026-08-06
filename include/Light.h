#if !defined(LIGHT_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
// First of all we don't use global light
// We used normal map to compenstate for that.
#include "handmade.h"
#include "win32game.h"
#include "B_shader.h"
#include "Camera.h"
#include <vector>

#define NR_POINT_LIGHTS 2

struct light_in_general{
       glm::vec3 ambient;
       glm::vec3 diffuse;
       glm::vec3 specular;
};
/*
struct Material{
// Ambient will be the same even with change in input
   sampler2D texture_diffused1;
   sampler2D texture_specular1;

   glm::vec3 ambient;
   glm::vec3 diffuse;
   glm::vec3 specular;

   float shininess;
};
 */

struct DirLight{
 // Inherent component
 glm::vec3 direction;

 // For Phong Shading
 glm::vec3 ambient;
 glm::vec3 diffuse;
 glm::vec3 specular;
};

struct PointLight{
 // Inherent component
 glm::vec3 position;

 // For Phong Shading
 glm::vec3 ambient;
 glm::vec3 diffuse;
 glm::vec3 specular;

 // For attenuation (Point Light)
 float constant;
 float linearTerm;
 float quadraticTerm;
};

struct SpotLight{
 // Inherent component
 glm::vec3 direction;
 glm::vec3 position;

 // For Phong Shading
 glm::vec3 ambient;
 glm::vec3 diffuse;
 glm::vec3 specular;

 // For attenuation (Point Light)
 float constant;
 float linearTerm;


 float quadraticTerm;

 // For spotlight effect
 // spotlight area defining angle(Phi) maybe with the different name such as cutoff
 float CutOff;
 // Now the smooth/soft edge effect
 float OuterCutOff;
};

struct global_light{
    light_in_general light;
    DirLight dirLight;
    PointLight Point_Lights [NR_POINT_LIGHTS];
};

global_variable global_light envir_light = {};

inline void LoadFont(Win32_OffScreen_Buffer* Backbuffer = nullptr, Glyph_Map* map = nullptr, const char* path = nullptr);
static void IncreaseFontAlpha(const unsigned char* source, void* dest, const Glyph_Property* glyp);
void DrawFont(const  Win32_OffScreen_Buffer* BackBuffer = nullptr, B_shader_program* shader = nullptr, const Glyph_Map* map = nullptr, const char* string = nullptr, const Rect_* rect = nullptr);
static glm::vec4 CalcGlypProperty(const glm::vec4* glyp_specs = nullptr, const Rect_* rect = nullptr);

static void IncreaseFontAlpha_(Glyph_Map* map = nullptr);

extern "C" __declspec(dllexport) void LoadFont_(platform_api* platform = nullptr, Platform_Properties* Game_Platform = nullptr, Win32_OffScreen_Buffer* BackBuffer = nullptr, Glyph_Map* map = nullptr, const char* path = nullptr);
typedef void (*LoadFont__)(platform_api*, Platform_Properties*, Win32_OffScreen_Buffer*, Glyph_Map*, const char*);

extern "C" __declspec(dllexport) void DrawFont_(Platform_Properties* Game_Platform = nullptr, Win32_OffScreen_Buffer* BackBuffer = nullptr, const GLuint VAO = 0, B_shader_program* shader = nullptr, const Glyph_Map* map = nullptr, const char* string = nullptr, const Rect_* rect = nullptr);
typedef void (*DrawFont__) (Platform_Properties*, Win32_OffScreen_Buffer* ,const GLuint, B_shader_program*, const Glyph_Map*, const char* , const Rect_*);
//=====================================
static void setup_pointlight(const Win32_OffScreen_Buffer* BackBuffer = nullptr, global_light* envir_light = nullptr);

inline void set_environmental_light(B_shader_program* shader = nullptr, const global_light* envir_light = nullptr, const Camera* camera = nullptr);

extern "C" __declspec(dllexport) void Move_(glm::mat4* object_matrix, float delta_time, float DelayedRatio);
typedef void (*Move__)(glm::mat4*, float , float);

extern "C" __declspec(dllexport) void setup_pointlight_(Platform_Properties* Game_Platform, Win32_OffScreen_Buffer* BackBuffer, global_light* envir_light);
typedef void (*setup_pointlight__)(Platform_Properties*, Win32_OffScreen_Buffer*, global_light*);

extern "C" __declspec(dllexport) void Set_environmental_light_(Platform_Properties* Game_Platform, Win32_OffScreen_Buffer* BackBuffer, B_shader_program* shader, const global_light* envir_light, const Camera* camera);

typedef void (*Set_Light_) (Platform_Properties* , Win32_OffScreen_Buffer*, B_shader_program* shader, const global_light* , const Camera*);

#define LIGHT_H
#endif
