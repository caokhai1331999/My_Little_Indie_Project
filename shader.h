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

#include "win32Game.h"
#include <glm/glm.hpp>

typedef bool VertexType;
#define vertex (bool)true
#define fragment (bool)false

struct Shader{
    HANDLE shader_file;
    LARGE_INTEGER file_size;
    unsigned int shaderID;
    void* SourceCode;
    void* SourcePath;
};

void loadShader(Shader* shader, char* name, VertexType type);
char* loadCurrentErr();

// Set Int, bool, float and Vector
void setVec2(GLuint programID, const char* name, glm::vec2 &value);
void setVec2(GLuint programID, const char* name, float x, float y);

void setVec3(GLuint programID, const char* name, glm::vec3 &value);
void setVec3(GLuint programID, const char* name, float x, float y, float z);

void setBool(GLuint programID, const char* name, bool value);
void setInt(GLuint programID, const char* name, int value);

// Set Matrix
void setMat3(GLuint programID, const char* name, glm::mat3 &value);
void setMat4(GLuint programID, const char* name, glm::mat4 &value);

// Use the shader
void useProgram(GLuint programID);

// Attaching shader to program
GLuint setupGLprogram(Shader* vshader, Shader* fshader);
void checkCompileErrors(GLuint shader, char* type);

#define SHADER_H
#endif
