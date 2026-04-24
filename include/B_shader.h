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

#include "handmade.h"
#include "Camera.h"
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
    GLuint ShaderID;
    ShaderType ShaderType_;
public:
    void loadShader(char* name, ShaderType type);    
    GLuint GetShaderID(){return ShaderID;};
};

class B_shader_program{
private:
    GLuint ProgramID;
    char* programName_;
public:
    B_shader shaders[2];
    B_shader_program(char* vertex_file_name, char* fragment_file_name, char* programName = nullptr){
        programName_ = nullptr;
        programName_ = new char;
        programName_ = programName;

        ProgramID = 0;
        shaders[vertex_].loadShader(vertex_file_name, vertex_);
        shaders[fragment_].loadShader(fragment_file_name, fragment_);
        setupGLprogram(programName);
    };

    ~B_shader_program(){
        GLint result;
        delete programName_;
        programName_ = nullptr;
        glDeleteProgram(ProgramID);
        glGetProgramiv(ProgramID, GL_DELETE_STATUS, &result);
        if(result == GL_TRUE){
            printf("Program ID: %d was successfully deleted\n", ProgramID);
        } else {
            printf("Failed to remove Program ID: %d\n", ProgramID);
        };
    };

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

    GLuint GetProgramID() const {return ProgramID;};
    char* GetShaderName() const {return programName_;};
// Attaching shader to program
    GLuint setupGLprogram(const char* programName);

// Load Current Errors
};

void useProgram(GLuint programId){glUseProgram(programId);};
char* loadCurrentErr();
void checkCompileErrors(GLuint shader, const ShaderType type, const char* programName);   
//void* GetAnyGLFuncAddress(const char *name);
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

void CheckShader(GLuint shaderId = 0, GLuint programId = 0, char* name = nullptr);
void tempSetEnviLight(B_shader_program* shader = nullptr, Camera* camera = nullptr);

/*
          delete basic_shader_;
          basic_shader_ = nullptr;

          delete quad_shader_;
          quad_shader_ = nullptr;

          delete model_shader_;
          model_shader_ = nullptr;

          delete animating_shader_;
          animating_shader_ = nullptr;
 */

#define SHADER_H
#endif
