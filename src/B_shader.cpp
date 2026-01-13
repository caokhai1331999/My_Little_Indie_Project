 /* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "B_shader.h"

// NOTE: Load shader code from source file to empty shader object
// then compile it , next attach it to empty program finally link
// that small program and delete the shader
void* GetAnyGLFuncAddress(const char *name)
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

void B_shader::loadShader(char* name, ShaderType type){
    //Create File handle to current file for reading
    char* Error;
    shader_file = CreateFileA(
                                     name,
                                     GENERIC_READ,
                                     0,0,
                                     OPEN_EXISTING,
                                     0,0);

        if(shader_file != INVALID_HANDLE_VALUE)
    {
        DWORD ByteRead;

        if(GetFileSizeEx(shader_file, &file_size)){
            file_size.QuadPart = safetruncateUint64(file_size.QuadPart);
        }

        SourceCode = VirtualAlloc(0, file_size.QuadPart, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

        if(SourceCode){

            if((ReadFile(shader_file, SourceCode, file_size.QuadPart, &ByteRead, NULL))){

                printf("Load file: %s successfully, File size is %d\n", name, ByteRead);
            
                //First Create an empty shader object by glCreateShader 
                // BUG here
                if(type == vertex_){
                    ShaderID = glCreateShader(GL_VERTEX_SHADER);
                } else {
                    ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
                }

                // Then Sourcing it with glShaderSource
                // Seemed like glShaderSource doesn't relate to file content
                // It just need path
                // 2nd arg is number of shader in source, 4th is an array of string length
                const char* CodeContent = (char* )SourceCode;
                glShaderSource(ShaderID, 1, &CodeContent, NULL);
                //Next compile this shader with glCompileShader
                glCompileShader(ShaderID);
                //Attach it(glAttachShader) with the already created(glCreateProgram) empty program
                //Finally delete already attached shader

                checkCompileErrors(ShaderID, type?"Vertex":"Fragment");
                const GLubyte* ver = glGetString(GL_VERSION);
                if (ver)
                    printf("OpenGL version: %s\n", ver);
                else
                    printf("glGetString(GL_VERSION) returned NULL\n");

                //glDeleteShader(shaderID);
                DEBUGFreeFileMemory(SourceCode);
                CloseHandle(shader_file);
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
    glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value);
    if(name.c_str()==""){
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
    glUniform3fv(glGetUniformLocation(ProgramID, name), 1, &value[0]);
}
void B_shader_program::setVec3(const char* name, float x, float y, float z){
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

void checkCompileErrors(GLuint shader, const char* type)
{
    GLint success;
    GLint infoLength = 0;
    GLchar* infoLog;
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
        if(infoLog != nullptr){
            free(infoLog);
        }
            infoLog = (GLchar*)malloc(infoLength);
        if (!success )
        {
            if(infoLength > 1){
                glGetShaderInfoLog(shader, infoLength, NULL, infoLog);
                printf("ERROR::SHADER_COMPILATION_ERROR of type: %s is %s\n", type, infoLog);
            }
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLength);        
        if (!success|| (infoLength > 0))
        {
            if(infoLog != nullptr){
                free(infoLog);
            }
            infoLog = (GLchar*)malloc(infoLength);
            glGetProgramInfoLog(shader, infoLength, NULL, infoLog);
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s is %s\n", type, infoLog);
        }
    }
};

GLuint B_shader_program::setupGLprogram(){
    //buffer->glData.ProgramID = glCreateProgram();
    unsigned int tempProgramID = glCreateProgram();
    
    glAttachShader(tempProgramID, shaders[vertex_].GetShaderID());
    glAttachShader(tempProgramID, shaders[fragment_].GetShaderID());
    glLinkProgram(tempProgramID);

    ProgramID = tempProgramID;
    printf("Program ID: %d %d\n", ProgramID, tempProgramID);
    checkCompileErrors(shaders[vertex_].GetShaderID(), "VERTEX");
    checkCompileErrors(shaders[fragment_].GetShaderID(), "FRAGMENT");
    checkCompileErrors(tempProgramID, "PROGRAM");
    
    if(glGetError() != GL_NO_ERROR){
    //printf("OpenGL Error: %d\n", glGetError());
        cout<<"OpenGL Error: "<< glGetError()<<endl;
    };
    glDetachShader(tempProgramID, shaders[vertex_].GetShaderID()); 
    glDetachShader(tempProgramID, shaders[fragment_].GetShaderID()); 

    glDeleteShader(shaders[vertex_].GetShaderID());
    glDeleteShader(shaders[fragment_].GetShaderID());

    return tempProgramID;
}
