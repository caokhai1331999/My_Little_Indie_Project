/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "shader.h"

// NOTE: Load shader code from source file to empty shader object
// then compile it , next attach it to empty program finally link
// that small program and delete the shader
void loadShader(Shader* shader, char* path){
    //Create File handle to current file for reading
    char* Error;
    shader->shader_file = CreateFileA(
                                     "basic_shader.vs",
                                     GENERIC_READ,
                                     0,0,
                                     OPEN_EXISTING,
                                     0,0);

        if(shader->shader_file != INVALID_HANDLE_VALUE)
    {
        DWORD ByteRead;

        if(GetFileSizeEx(shader->shader_file, &shader->file_size)){
            shader->file_size.QuadPart = safetruncateUint64(shader->file_size.QuadPart);
        }

        if((ReadFile(shader->shader_file, shader->SourcePath, shader->file_size.QuadPart, &ByteRead, NULL)) && (shader->file_size.QuadPart == ByteRead)){

            //First Create an empty shader object by glCreateShader 
            shader->shaderID = glCreateShader(GL_VERTEX_SHADER);

            // Then Sourcing it with glShaderSource
            // Seemed like glShaderSource doesn't relate to file content
            // It just need path
            // 2nd arg is number of shader in source, 4th is an array of string length
            glShaderSource(shader->shaderID, 1, (GLchar*)(shader->path), NULL);
            //Next compile this shader with glCompileShader
            if(glCompileShader(shader->shaderID)){
                //Attach it(glAttachShader) with the already created(glCreateProgram) empty program
                unsigned int ProgramID = glCreateProgram();
                glAttachShader(ProgramID, shader->shaderID);
                glLinkProgram(ProgramID);
                //Finally delete already attached shader
                glDeleteShader();                            
            }
            else {
                //Show error
            }
        } else {
            Error = loadCurrentErr();
            printf("error : %s\n", Error);
        }

    } else {
        Error = loadCurrentErr();
        printf("error : %s\n", Error);        
    }
};

char* loadCurrentErr(){
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError(); 
    char* errorContent;
        
    if (FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, NULL) == 0) {
        errorContent = (char*)lpMsgBuf;
        ExitProcess(dw);
    }
        
    LocalFree(lpMsgBuf);
    ExitProcess(dw);
    return errorContent;
}

void checkCompileError(GLuint shader, char* type){
    
}

void use(Shader* shader){
    glUseProgram(shader->shaderID);
}

// Set Int, bool,
void setBool(Shader* shader, const char* name, const bool value){
    glUniform1i(glGetUniformLocation(shader->shaderID,(GLchar* ) name), (int)value);
};

void setInt(Shader* shader, const  int value){
    glUniform1i(glGetUniformLocation(shader->shaderID,(GLchar* ) name), value);
};

//Vector 2nd argument is number of vector
void setVec2(Shader* shader, const char* name, const  glm::vec2 &value){
    glUniform2fv(glGetUniformLocation(shader->shaderID,(GLchar* ) name), 1, &value[0])
}

void setVec2(Shader* shader, const char* name, float x, float y){
    glm::vec2 value = glm::vec2(x, y);
    glUniform2f(glGetUniformLocation(shader->shaderID,(GLchar* ) name), x, y)
}

void setVec3(Shader* shader, const char* name, const glm::vec3 &value){
    glUniform3fv(glGetUniformLocation(shader->shaderID,(GLchar* ) name), 1 &value[0]);
}
void setVec3(Shader* shader, const char* name, float x, float y, float z){
    glUniform3f(glGetUniformLocation(shader->shaderID,(GLchar* ) name), x, y, z);
}

// Set Matrix, 3rd is GL_Boolean transpose
void setMat3(Shader* shader, const glm::mat3& value){
    glUniformMatrix3(glGetUniformLocation(shader->shaderID,(GLchar* ) name), 1, GL_FALSE, &value[0]);
};

void setMat4(Shader* shader, const glm::mat4& value){
    glUniformMatrix3(glGetUniformLocation(shader->shaderID,(GLchar* ) name), 1, GL_FALSE, &value[0]);
};
