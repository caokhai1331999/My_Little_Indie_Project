/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>

#pragma comment(lib, "opengl32.lib")
void OpenConsole() {
    AllocConsole();                             // Allocate a new console
    freopen("CONOUT$", "w", stdout);            // Redirect printf to console
    // freopen("CONOUT$", "w", stderr);            // Redirect stderr
    // freopen("CONIN$", "r", stdin);              // Redirect stdin (optional)
}


int CALLBACK WinMain
(HINSTANCE Instance,
 HINSTANCE hInstPrev,
 PSTR cmdline,
 int cmdshow)
{
    HDC hdc;
    HGLRC hglrc;
    PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1 };
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;

    bool DONE = false;
    bool printed = false;
    unsigned int Count = 0;
    OpenConsole();
    HWND hwnd = CreateWindowA("STATIC", "Dummy", WS_OVERLAPPEDWINDOW,
        0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
    if(hwnd){
        hdc = GetDC(hwnd);
        int pf = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pf, &pfd);
        hglrc = wglCreateContext(hdc);
        wglMakeCurrent(hdc, hglrc);

        while(!DONE){
            const GLubyte* version = glGetString(GL_VERSION);

            if(!printed){
                printf("OpenGL version: %s\n", version);
                printed = true;
            };

            if(Count <= 30){
                Count++;
            } else {
                Count = 0;
                DONE = true;
            }
        }

        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hglrc);
        ReleaseDC(hwnd, hdc);
        DestroyWindow(hwnd);        
    } else {

        while(!DONE){
            printf("Failed to create window\n");

            if(Count <= 30){
                Count++;
            } else {
                Count = 0;
                DONE = true;
            }
        }
        
    }

    return 0;
}
