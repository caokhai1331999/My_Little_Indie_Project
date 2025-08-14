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
void *GetAnyGLFuncAddress(const char *name)
{
  void *p = (void *)wglGetProcAddress(name);
  if(p == 0 ||
    (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
    (p == (void*)-1) )
  {
    HMODULE module = LoadLibraryA("opengl32.dll");
    if(module!=NULL){
        p = (void *)GetProcAddress(module, name);
    } else {
        printf("Couldn't load OpenGl library: %s\n", loadCurrentErr());
    }
  } else {
      printf("Can find function with given name\n"); \
  }

  return p;
}
// =============== let aside this alone touch it when it's time================

void loadShader(Shader* shader, char* name, VertexType type){
    //Create File handle to current file for reading
    char* Error;
    shader->shader_file = CreateFileA(
                                     name,
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

        shader->SourceCode = VirtualAlloc(0, shader->file_size.QuadPart, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

        if(shader->SourceCode){

            if((ReadFile(shader->shader_file, shader->SourceCode, shader->file_size.QuadPart, &ByteRead, NULL))){

                printf("Load file: %s successfully, File size is %d\n", name, ByteRead);
            
                //First Create an empty shader object by glCreateShader 
                // BUG here
                if(type == vertex){
                    shader->shaderID = glCreateShader(GL_VERTEX_SHADER);
                } else {
                    shader->shaderID = glCreateShader(GL_FRAGMENT_SHADER);
                }

                // Then Sourcing it with glShaderSource
                // Seemed like glShaderSource doesn't relate to file content
                // It just need path
                // 2nd arg is number of shader in source, 4th is an array of string length
                const char* CodeContent = (char* )shader->SourceCode;
                glShaderSource(shader->shaderID, 1, &CodeContent, NULL);
                //Next compile this shader with glCompileShader
                glCompileShader(shader->shaderID);
                //Attach it(glAttachShader) with the already created(glCreateProgram) empty program
                //Finally delete already attached shader

                checkCompileErrors(shader->shaderID, type?"Vertex":"Fragment");
                const GLubyte* ver = glGetString(GL_VERSION);
                if (ver)
                    printf("OpenGL version: %s\n", ver);
                else
                    printf("glGetString(GL_VERSION) returned NULL\n");

                //glDeleteShader(shader->shaderID);
                DEBUGFreeFileMemory(shader->SourceCode);
                CloseHandle(shader->shader_file);
            } else {
                Error = loadCurrentErr();
                DWORD err = GetLastError();
                printf("error : %s\n", Error);
            }            
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
        //ExitProcess(dw);
    }
        
    LocalFree(lpMsgBuf);
    //ExitProcess(dw);
    return errorContent;
}

void useProgram(GLuint programID){
    glUseProgram(programID);
}

// Set Int, bool,
void setBool(GLuint programID, const char* name, const bool value){
    glUniform1i(glGetUniformLocation(programID, name), (int)value);
};

void setInt(GLuint programID, const char* name, const int value){
    glUniform1i(glGetUniformLocation(programID, name), value);
};

//Vector 2nd argument is number of vector
void setVec2(GLuint programID, const char* name, const  glm::vec2 &value){
    glUniform2fv(glGetUniformLocation(programID, name), 1, &value[0]);
}

void setVec2(GLuint programID, const char* name, float x, float y){
    glm::vec2 value = glm::vec2(x, y);
    glUniform2f(glGetUniformLocation(programID, name), x, y);
}

void setVec3(GLuint programID, const char* name, const glm::vec3 &value){
    glUniform3fv(glGetUniformLocation(programID, name), 1, &value[0]);
}
void setVec3(GLuint programID, const char* name, float x, float y, float z){
    glUniform3f(glGetUniformLocation(programID, name), x, y, z);
}

// Set Matrix, 3rd is GL_Boolean transpose
void setMat3(GLuint programID, const char* name, const glm::mat3 &value){
    glUniformMatrix3fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &value[0][0]);
};

void setMat4(GLuint programID, const char* name, const glm::mat4 &value){
    glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &value[0][0]);
};

void checkCompileErrors(GLuint shader, char* type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n %s", type, infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n %s", type, infoLog);
        }
    }
};

void setupGLprogram(Win32_Front_Buffer* buffer, Shader* vshader, Shader* fshader){
    buffer->glData.ProgramID = glCreateProgram();

    glAttachShader(buffer->glData.ProgramID, vshader->shaderID);
    glAttachShader(buffer->glData.ProgramID, fshader->shaderID);
    glLinkProgram(buffer->glData.ProgramID);

    checkCompileErrors(buffer->glData.ProgramID, "PROGRAM");
    
    if(glGetError() != GL_NO_ERROR){
    printf("OpenGL Error: %d\n", glGetError());
    };

    glDeleteShader(vshader->shaderID);
    glDeleteShader(fshader->shaderID);
}
