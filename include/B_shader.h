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
    char* vertex_file_path;
    char* fragment_file_path;
public:
    B_shader shaders[2];
    B_shader_program(char* vertex_file_name, char* fragment_file_name, char* programName = nullptr):vertex_file_path{vertex_file_name},fragment_file_path{fragment_file_name}{
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

    char* GetVertexFilePath(){return vertex_file_path;};
    char* GetFragmentFilePath(){return fragment_file_path;};
    char* GetProgramName(){return programName_;};
    GLuint GetProgramID(){return ProgramID;};
    void ReLoadShaderCode();
// Set Int, bool, float and Vector
    void setVec2(const char* name, const glm::vec2 &value);
    void setVec2(const char* name, float x, float y);

    void setVec3(const char* name, const glm::vec3 &value);
    void setVec3(const char* name,  float x, float y, float z);

    void setBool(const char* name, const bool value);
    void setInt(const std::string name, const int value);
    void setFloat(const char* name, const float value);
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

void B_shader_program::use(){
    glUseProgram(ProgramID);
}

// Set Int, bool,
void B_shader_program::setBool(const char* name, const bool value){
    glUniform1i(glGetUniformLocation(ProgramID, name), (int)value);
};

void B_shader_program::setInt(const std::string name, const int value){

    glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), value);
    if(name.c_str()==""){
        printf("name content is NULL\n");
    }
    if(&value == 0x00){
        printf("value content is NULL\n");        
    }
};

void B_shader_program::setFloat(const std::string name, const float value){

    std::string test_name = name;
   
    if(string_contain(&test_name, "animating")){

        GLint location = glGetUniformLocation(ProgramID, name.c_str());

        if(location == -1){
            printf("This uniform %s in %s is not found\n", name.c_str(), GetProgramName());
        }  else {
            printf("This uniform %s location in %s location is %d\n", name.c_str(), GetProgramName(), location);
        };
    }

    glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value);
    if(name.c_str()==""){
        printf("name content is NULL\n");
    }
    if(&value == 0x00){
        printf("value content is NULL\n");        
    }
};

void B_shader_program::setFloat(const char* name, const float value){

    std::string test_name{name};
   
    if(string_contain(&test_name, "animating")){
        GLint location = glGetUniformLocation(ProgramID, name);

        if(location == -1){
            printf("This uniform %s in %s is not found\n", name, GetProgramName());
        }  else {
            printf("This uniform %s location in %s location is %d\n", name, GetProgramName(), location);
        };
    }

    glUniform1f(glGetUniformLocation(ProgramID, name), value);
    if(name==""){
        printf("name content is NULL\n");
    }
    if(&value == 0x00){
        printf("value content is NULL\n");        
    }
};

//Vector 2nd argument is number of vector
void B_shader_program::setVec2(const char* name, const glm::vec2 &value){
    glUniform2fv(glGetUniformLocation(ProgramID, name), 1, &value[0]);
}

void B_shader_program::setVec2(const char* name, float x, float y){
    glm::vec2 value = glm::vec2(x, y);
    glUniform2f(glGetUniformLocation(ProgramID, name), x, y);
}

void B_shader_program::setVec3(const char* name, const glm::vec3 &value){

    std::string test_name{name};
   
    if(string_contain(&test_name, "animating")){
        GLint location = glGetUniformLocation(ProgramID, name);

        if(location == -1){
            printf("This uniform %s in %s is not found\n", name, GetProgramName());
        }  else {
            printf("This uniform %s location in %s location is %d\n", name, GetProgramName(), location);
        };
    }
    
    if(name == ""){
        printf("name content is NULL\n");
    }
    if(&value == 0x00){
        printf("value content is NULL\n");        
    }
    glUniform3fv(glGetUniformLocation(ProgramID, name), 1, &value[0]);
}

void B_shader_program::setVec3(const char* name, float x, float y, float z){
    GLint location = glGetUniformLocation(ProgramID, name);

    if(location == -1)
        printf("This uniform %s in %s is not found\n", name, GetProgramName());
    
    if(name==""){
        printf("name content is NULL\n");
    }
    glUniform3f(glGetUniformLocation(ProgramID, name), x, y, z);
}

// Set Matrix, 3rd is GL_Boolean transpose
void B_shader_program::setMat3(const char* name, const glm::mat3 &value){
    glUniformMatrix3fv(glGetUniformLocation(ProgramID, name), 1, GL_FALSE, &value[0][0]);
};

void B_shader_program::setMat4(const std::string name, const glm::mat4 &value){
    glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    //printf("pointer to matrix is: 0x%hx\n",&value[0][0]);
 };


// Set Int, bool,
void setBool( GLuint ShaderID, const char* name, const bool value){
    glUniform1i(glGetUniformLocation(ShaderID, name), (int)value);
};

void setInt( GLuint ShaderID, const std::string name, const int value){
    glUniform1i(glGetUniformLocation(ShaderID, name.c_str()), value);
    if(name.c_str()==""){
        printf("name content is NULL\n");
    }
    if(&value == 0x00){
        printf("value content is NULL\n");        
    }
};

void setFloat( GLuint ShaderID, const std::string name, const float value){
    glUniform1f(glGetUniformLocation(ShaderID, name.c_str()), value);
    if(name.c_str()==""){
        printf("name content is NULL\n");
    }
    if(&value == 0x00){
        printf("value content is NULL\n");        
    }
};

//Vector 2nd argument is number of vector
void setVec2( GLuint ShaderID, const char* name, const glm::vec2 &value){
    glUniform2fv(glGetUniformLocation(ShaderID, name), 1, &value[0]);
}

void setVec2( GLuint ShaderID, const char* name, float x, float y){
    glm::vec2 value = glm::vec2(x, y);
    glUniform2f(glGetUniformLocation(ShaderID, name), x, y);
}

void setVec3( GLuint ShaderID, const char* name, const glm::vec3 &value){
    glUniform3fv(glGetUniformLocation(ShaderID, name), 1, &value[0]);
}
void setVec3( GLuint ShaderID, const char* name, float x, float y, float z){
    glUniform3f(glGetUniformLocation(ShaderID, name), x, y, z);
}

// Set Matrix, 3rd is GL_Boolean transpose
void setMat3( GLuint ShaderID, const char* name, const glm::mat3 &value){
    glUniformMatrix3fv(glGetUniformLocation(ShaderID, name), 1, GL_FALSE, &value[0][0]);
};

void setMat4( GLuint ShaderID, const std::string name, const glm::mat4 &value){
    glUniformMatrix4fv(glGetUniformLocation(ShaderID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    //printf("pointer to matrix is: 0x%hx\n",&value[0][0]);
 };

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
