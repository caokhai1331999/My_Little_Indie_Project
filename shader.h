#if !defined(SHADER_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

// NOTE : We create shader by these component
// first: shader source code (char*)
// glShaderSource -> glCompileShader(CheckCompileErr) -> DeleteShader

// We create shader from source code
// then create a program by GLint ID name -> attach shader -> then link
// the program

// Then set mat4(Model, projection,etc ...) to the program

#include "handmade.h"
#include <glm/glm.hpp>

struct Shader{
    HANDLE shader_file;
    LARGE_INTEGER file_size;
    unsigned int shaderID;
    void* SourceCode;
    char* SourcePath;
};

void *GetAnyGLFuncAddress(const char *name);
#define loadFunc(name) (name*)GetAnyGLFuncAddress(name)
//
//#define GL_VERTEX_SHADER   0x8B31
//#define GL_FRAGMENT_SHADER 0x8B30
//#define GL_COMPILE_STATUS  0x8B81
//#define GL_LINK_STATUS     0x8B82
//
//extern PROC glCreateShader = (PROC)GetAnyGLFuncAddress("glCreateShader");
//extern PROC glShaderSource = (PROC)GetAnyGLFuncAddress("glShaderSource");
//extern PROC glUseProgram = (PROC)GetAnyGLFuncAddress("glUseProgram");
//extern PROC glCompileShader = (PROC)GetAnyGLFuncAddress("glCompileShader");
//extern PROC glCreateProgram = (PROC)GetAnyGLFuncAddress("glCreateProgram");
//extern PROC glAttachShader = (PROC)GetAnyGLFuncAddress("glCreateShader");
//extern PROC glLinkProgram = (PROC)GetAnyGLFuncAddress("glLinkProgram");
//
// Too much function address call it make his file looked messy
void loadShader(Shader* shader,char* path);
char* loadCurrentErr();
void checkCompileError(GLuint shader, char* type);

// Set Int, bool, float and Vector
void setVec2(Shader* shader, const char* name, glm::vec2 &value);
void setVec2(Shader* shader, const char* name, float x, float y);

void setVec3(Shader* shader, const char* name, glm::vec3 &value);
void setVec3(Shader* shader, const char* name, float x, float y, float z);

void setBool(Shader* shader, const char* name, bool value);
void setInt(Shader* shader, const char* name, int value);

// Set Matrix
void setMat3(Shader* shader, const char* name, glm::mat3 &value);
void setMat4(Shader* shader, const char* name, glm::mat4 &value);

// Use the shader
void use(Shader* shader);

#define SHADER_H
#endif
