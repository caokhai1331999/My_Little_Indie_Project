#if !defined(GL_EXTENSIONS_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <GL/gl.h>
#include <GL/glext.h>
#define GL_GLEXT_PROTOTYPES

// You must define missing constants yourself if not using GLEW/GLAD:
#define GL_VERTEX_SHADER   0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS  0x8B81
#define GL_LINK_STATUS     0x8B82

// Function pointer declarations
extern PFNGLCREATESHADERPROC        glCreateShader;
extern PFNGLSHADERSOURCEPROC        glShaderSource;
extern PFNGLCOMPILESHADERPROC       glCompileShader;
extern PFNGLGETSHADERIVPROC         glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC    glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC       glCreateProgram;
extern PFNGLATTACHSHADERPROC        glAttachShader;
extern PFNGLLINKPROGRAMPROC         glLinkProgram;
extern PFNGLUSEPROGRAMPROC          glUseProgram;
extern PFNGLGETPROGRAMIVPROC        glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC   glGetProgramInfoLog;

bool LoadOpenGLFunctions(); // loader


#define GL_EXTENSIONS_H
#endif
