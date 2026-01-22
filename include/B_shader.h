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

typedef int ShaderType;

#define vertex_ 0
#define fragment_ 1
#define programme_ 2

struct B_shader{
private:
    HANDLE shader_file;
    LARGE_INTEGER file_size;
    void* SourceCode;
    void* SourcePath;
    unsigned int ShaderID;
    ShaderType ShaderType_;
public:
    void loadShader(char* name, ShaderType type);    
    GLuint GetShaderID(){return ShaderID;};
};

class B_shader_program{
private:
    unsigned int ProgramID;
    
public:
    B_shader shaders[2];
    B_shader_program(char* vertex_file_name, char* fragment_file_name, const char* programName = nullptr){
        shaders[vertex_].loadShader(vertex_file_name, vertex_);
        shaders[fragment_].loadShader(fragment_file_name, fragment_);
        setupGLprogram(programName);
    };

    ~B_shader_program();

// Set Int, bool, float and Vector
    void setVec2(const char* name, const glm::vec2 &value);
    void setVec2(const char* name, float x, float y);

    void setVec3(const char* name, const glm::vec3 &value);
    void setVec3(const char* name,  float x, float y, float z);

    void setBool(const char* name, const bool value);
    void setInt(const std::string name, const int value);
    void setFloat(const std::string name, const float value);

// Set Matrix
    void setMat3(const char* name, const glm::mat3 &value);
    void setMat4(const std::string name, const glm::mat4 &value);

// Return shaderID
    GLuint getVertexShaderID(){return shaders[vertex_].GetShaderID();}; 
    GLuint getFragmentShaderID(){return shaders[fragment_].GetShaderID();}; 
// Use the shader
    void use();

    GLuint GetProgramID(){return ProgramID;};
// Attaching shader to program
    GLuint setupGLprogram(const char* programName);

// Load Current Errors
};

void useProgram(GLuint programId){glUseProgram(programId);};
char* loadCurrentErr();
void checkCompileErrors(GLuint shader, const ShaderType type, const char* programName);   
void* GetAnyGLFuncAddress(const char *name);
std::vector<GLuint> ProgramIDs;


void setVec2(GLuint ShaderID, const char* name, const glm::vec2 &value);
void setVec2(GLuint ShaderID, const char *name, float x, float y);

void setVec3(GLuint ShaderID, const char *name, const glm::vec3 &value);
void setVec3(GLuint ShaderID, const char *name, float x, float y, float z);

void setBool(GLuint ShaderID, const char *name, const bool value);
void setInt(GLuint ShaderID, const std::string name, const int value);
void setFloat(GLuint ShaderID, const std::string name, const float value);

// Set Matrix
void setMat3(GLuint ShaderID, const char *name, const glm::mat3 &value);
void setMat4(GLuint ShaderID, const std::string name, const glm::mat4 &value);


#define SHADER_H
#endif
