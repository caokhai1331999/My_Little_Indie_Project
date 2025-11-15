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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "win32Game.h"
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

using namespace std;

typedef bool VertexType;
#define vertex_ (bool)true
#define fragment_ (bool)false

struct B_shader{
    HANDLE shader_file;
    LARGE_INTEGER file_size;
    unsigned int shaderID;
    void* SourceCode;
    void* SourcePath;
};

std::vector<GLuint> ProgramIDs;

void loadShader(B_shader* shader, char* name, VertexType type);
char* loadCurrentErr();

// Set Int, bool, float and Vector
void setVec2(GLuint programID, const char* name, const glm::vec2 &value);
void setVec2(GLuint programID, const char* name, float x, float y);

void setVec3(GLuint programID, const char* name, const glm::vec3 &value);
void setVec3(GLuint programID, const char* name,  float x, float y, float z);

void setBool(GLuint programID, const char* name, const bool value);
void setInt(GLuint programID, const std::string name, const int value);

// Set Matrix
void setMat3(GLuint programID, const char* name, const glm::mat3 &value);
void setMat4(GLuint programID, const std::string name, const glm::mat4 &value);

// Use the shader
void useProgram(GLuint programID);

// Attaching shader to program
GLuint setupGLprogram(B_shader* vshader, B_shader* fshader, GLuint* ProgramID );
void checkCompileErrors(GLuint shader, const char* type);

#define SHADER_H
#endif
