/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "gl_extensions.h"

// Define all function pointers
PFNGLCREATESHADERPROC        glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC        glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC       glCompileShader = nullptr;
PFNGLGETSHADERIVPROC         glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC    glGetShaderInfoLog = nullptr;
PFNGLCREATEPROGRAMPROC       glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC        glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC         glLinkProgram = nullptr;
PFNGLUSEPROGRAMPROC          glUseProgram = nullptr;
PFNGLGETPROGRAMIVPROC        glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC   glGetProgramInfoLog = nullptr;

bool LoadOpenGLFunctions() {
    // Must have a valid OpenGL context before calling this!
    glCreateShader        = (PFNGLCREATESHADERPROC)        wglGetProcAddress("glCreateShader");
    glShaderSource        = (PFNGLSHADERSOURCEPROC)        wglGetProcAddress("glShaderSource");
    glCompileShader       = (PFNGLCOMPILESHADERPROC)       wglGetProcAddress("glCompileShader");
    glGetShaderiv         = (PFNGLGETSHADERIVPROC)         wglGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog    = (PFNGLGETSHADERINFOLOGPROC)    wglGetProcAddress("glGetShaderInfoLog");
    glCreateProgram       = (PFNGLCREATEPROGRAMPROC)       wglGetProcAddress("glCreateProgram");
    glAttachShader        = (PFNGLATTACHSHADERPROC)        wglGetProcAddress("glAttachShader");
    glLinkProgram         = (PFNGLLINKPROGRAMPROC)         wglGetProcAddress("glLinkProgram");
    glUseProgram          = (PFNGLUSEPROGRAMPROC)          wglGetProcAddress("glUseProgram");
    glGetProgramiv        = (PFNGLGETPROGRAMIVPROC)        wglGetProcAddress("glGetProgramiv");
    glGetProgramInfoLog   = (PFNGLGETPROGRAMINFOLOGPROC)   wglGetProcAddress("glGetProgramInfoLog");

    // Check if any failed (optional)
    return glCreateShader && glShaderSource && glCompileShader;
}
