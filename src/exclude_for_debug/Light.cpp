/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "Light.h"


void IncreaseFontAlpha(const unsigned char* source, void* dest, const Glyph_Property* glyp){
    uint8 *Source = (uint8*)source;
    // move the destination pointer to the head of the last row
    uint8* DestRow = (uint8*)dest + (glyp->w * (glyp->h - 1));
    //Turn the bitmap upside down
    for(uint8 y = 0; y < glyp->h; y++){
        //uint32* Dest = (uint32*)DestRow;
        uint8* Dest = DestRow;
        for(uint8 x = 0; x < glyp->w; x++){
            //uint32 alpha = *Source++;
            ////*Dest++ = ((alpha << 24)|
                       //(alpha << 16)|
                       //(alpha <<  8)|
                       //(alpha <<  0));
            *Dest++ = *Source++;
        }
            DestRow -= glyp->w;
    };
}
// Init
// Load
// Render}

void LoadFont(const Win32_OffScreen_Buffer* Backbuffer, Glyph_Map* map, const char* path){
// Load File + Init
        debug_read_file_result* TTFfile = DEBUGReadFileWhole(path);
        stbtt_InitFont(&map->FontInfo, (unsigned char*)TTFfile->Content, stbtt_GetFontOffsetForIndex((unsigned char*)TTFfile->Content, 0));

        Glyph_Property* glyph;
        unsigned char* bitmap;

        for(char c  = 'A'; c < 'Z'; c++){
            glyph = (Glyph_Property*)malloc(sizeof(Glyph_Property));
            glyph-> i = 0;
            glyph-> j = 0;
            glyph-> c = c;

            bitmap = stbtt_GetCodepointBitmap(&map->FontInfo, 0, stbtt_ScaleForPixelHeight(&map->FontInfo, 128.0f), c, &glyph->w, &glyph->h, &glyph->Xoffset, &glyph->Yoffset);
            assert(bitmap);

            //glyph->upside_down_bitmap = VirtualAlloc(0, sizeof(uint32) * glyph->h * glyph->w, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
            glyph->upside_down_bitmap = (uint32* )malloc(sizeof(uint32) * glyph->h * glyph->w);
            IncreaseFontAlpha(bitmap, glyph->upside_down_bitmap, glyph);

            assert(glyph->upside_down_bitmap);
            map->Glyph_list.push_back(glyph);

            //free(glyph);
            stbtt_FreeBitmap(bitmap, nullptr);
        }
    if(TTFfile->Content)
            DEBUGFreeFileMemory(TTFfile->Content);
// Set texture config
            glGenTextures(1, &(map->TextureID));
            glActiveTexture(GL_TEXTURE0+map->TextureID);
            glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0+map->TextureID);

            glGenerateMipmap(GL_TEXTURE_2D);
            // can free temp_bitmap at this point
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);        
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//==========================================

        delete TTFfile;
        TTFfile = nullptr;        
}

void LoadFont_(const Win32_OffScreen_Buffer* BackBuffer, Glyph_Map* map , const char* path){
        if(wglMakeCurrent(GetDC(BackBuffer->Window), BackBuffer->glData.openglRC)){
            ReloadGLFunction(BackBuffer);
        }
        LoadFont(BackBuffer, map, path);
};

glm::vec4 CalcGlypProperty(const glm::vec4* previous_glyp_specs, const Rect_* rect, const Glyph_Property* glyph){
    glm::vec4 font_specs;
    if(previous_glyp_specs->x - previous_glyp_specs->w > 0){
        font_specs.x = (float)((float)previous_glyp_specs->x - (float )previous_glyp_specs->w / 1009);
    }else{
        font_specs.x = 0;
        // specs.z is h
        font_specs.y = (float)((float)previous_glyp_specs->y - (float)previous_glyp_specs->z)/600;
    }
    font_specs.z = glyph->h/600;
    font_specs.w = glyph->w/1000;
    return font_specs;
}


// How to create pos based on each of glyph was draw before
void DrawFont(const GLuint VAO, B_shader_program* shader, const Glyph_Map* map, const char* string, const Rect_* rect){
    shader->use();
    glBindVertexArray(VAO);
    // L, R, B, T
    glm::mat4 othorForGlyph = glm::ortho(0.0f, (float)20.0f, 0.0f, (float)20.0f);
    shader->setMat4("projection", othorForGlyph);

    glActiveTexture(GL_TEXTURE0+map->TextureID);
    glBindTexture(GL_TEXTURE_2D, map->TextureID);

    int i = 0;

    int decreased_width = rect->w/1000;
    int decreased_height = rect->h/600;

    Glyph_Property* glyp_p;
    glm::vec4 current_glyp_specs;
    std::string name;
    while(string[i] != '\0'){
        // underlying argument is the i (index)
        for(Glyph_Property* const &iter: map->Glyph_list){
            if(iter->c == string[i]){
                glyp_p = iter;
                break;
            }else{
                glyp_p = map->Glyph_list[0];
            }
        }
        // wrong here
        // w -> 0
        //current_glyp_specs = i==0?glm::vec4(rect->w, rect->h, glyp_p->h, glyp_p->w):CalcGlypProperty(&glm::vec4((float)decreased_width, (float)decreased_height, (float)glyp_p->h, (float)glyp_p->w), rect, glyp_p);
        current_glyp_specs = CalcGlypProperty(&glm::vec4((float)decreased_width, (float)decreased_height, (float)glyp_p->h, (float)glyp_p->w), rect, glyp_p);

        //name = "GlyphPoses["+to_string(i)+"]";
        name = "GlyphPos";
        //printf("glypos %d is %s\n", i, glm::to_string(current_glyp_specs).c_str());
        shader->setVec4(name.c_str(), current_glyp_specs);        
        glActiveTexture(GL_TEXTURE0+map->TextureID);
        glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0+map->TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, glyp_p->w, glyp_p->h, 0, GL_RG, GL_UNSIGNED_BYTE, glyp_p->upside_down_bitmap);        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // currently
        decreased_width -= glyp_p->w/1000;

        if(decreased_width <= 0){
            decreased_height -= glyp_p->h/600;
            decreased_width = rect->w/1000;
        }
        i++;
    }
    glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);
    glUseProgram(0);
}
 
void DrawFont_(const Win32_OffScreen_Buffer* BackBuffer, const GLuint VAO, B_shader_program* shader, const Glyph_Map* map, const char* string, const Rect_* rect){
    ReloadGLFunction(BackBuffer);
    DrawFont(VAO, shader, map, string, rect);
}

/*
void LoadFont(Glyph_Map* map, const char* path){
    debug_read_file_result* TTFfile = nullptr;

    if(first_announce){
// Load File + Init
        TTFfile = new debug_read_file_result;
        debug_read_file_result* TTFfile = DEBUGReadFileWhole(path);
        stbtt_InitFont(&map->FontInfo, (unsigned char*)TTFfile->Content, stbtt_GetFontOffsetForIndex((unsigned char*)TTFfile->Content, 0));
    }
// This function load every character
    // For/while
    //
    map->bitmap = stbtt_GetCodepointBitmap(&map->FontInfo, 0, stbtt_ScaleForPixelHeight(&map->FontInfo, 128.0f), 'A', &map->w, &map->h, &map->Xoffset, &map->Yoffset);

    if(map->bitmap){

        if(first_announce){
            InitBitmap(map);
            glGenTextures(1, &map->TextureID);
        }

        IncreaseFontAlpha(map);
//Render
        // Actually we still don't know the relative size of character to the
        // screen size
        glActiveTexture(GL_TEXTURE0+map->TextureID);
        glBindTexture(GL_TEXTURE_2D, map->TextureID);
        if(map->upside_down_bitmap){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, map->w, map->h, 0, GL_RG, GL_UNSIGNED_BYTE, map->upside_down_bitmap);
        }else{
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, map->w, map->h, 0, GL_RG, GL_UNSIGNED_BYTE, map->bitmap);
        }

        if(first_announce){
        // Why there are no pointer of this
        glGenerateMipmap(GL_TEXTURE_2D);
        // can free temp_bitmap at this point
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
    
        printf("load Font successfully\n");
    }else{
        printf("Failed Loading font\n");
    }       
    printf("Created Font Texture :%d\n", map->TextureID);

    if(first_announce){
        if(TTFfile->Content)
            DEBUGFreeFileMemory(TTFfile->Content);

        delete TTFfile;
        TTFfile = nullptr;
    }
}
*/

void setup_pointlight(global_light* envir_light){
    glm::vec3 pointlight_Pos[] =
        {
            glm::vec3(-0.2f, -1.0f, -0.3f),
            glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.5f, 0.5f, 0.5f)
        };

    envir_light->Point_Lights[0].position = glm::vec3(7.0f,  5.0f,  3.0f);
    envir_light->Point_Lights[0].ambient = glm::vec3( 0.05f, 0.05f, 0.05f);
    envir_light->Point_Lights[0].diffuse = glm::vec3( 0.8f, 0.8f, 0.8f);
    envir_light->Point_Lights[0].specular = glm::vec3( 1.0f, 1.0f, 1.0f);
    envir_light->Point_Lights[0].constant = 1.0f;
    envir_light->Point_Lights[0].linearTerm = 0.09f;
    envir_light->Point_Lights[0].quadraticTerm = 0.032f;
          
    envir_light->Point_Lights[1].position = glm::vec3(-5.0f, 5.0f, 3.0f);    
    envir_light->Point_Lights[1].ambient = glm::vec3(0.05f, 0.05f, 0.05f);        
    envir_light->Point_Lights[1].diffuse = glm::vec3(0.8f, 0.8f, 0.8f);           
    envir_light->Point_Lights[1].specular = glm::vec3(1.0f, 1.0f, 1.0f);          
    envir_light->Point_Lights[1].constant = 1.0f;                     
    envir_light->Point_Lights[1].linearTerm = 0.09f;                  
    envir_light->Point_Lights[1].quadraticTerm = 0.032f;

    envir_light->dirLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    envir_light->dirLight.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    envir_light->dirLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);
}

void set_environmental_light(B_shader_program* shader, global_light* envir_light, Camera* camera){

    shader->use();

    std::string test_name {shader->GetProgramName()};
    shader->setFloat("material.shininess", 25.0f);

    glm::vec3 lightPos (-4.0f, 10.0f, -15.0f);
        shader->setVec3("lightPos", lightPos);

        shader->setVec3("dirLight.ambient", envir_light->dirLight.ambient);
        shader->setVec3("dirLight.diffuse", envir_light->dirLight.diffuse);
        shader->setVec3("dirLight.specular", envir_light->dirLight.specular);

    
    std::string light_name;
    for(int i = 0; i < 2; i++){

        light_name = "pointLight_Pos[]";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);

        shader->setVec3(light_name.c_str(), envir_light->Point_Lights[i].position);
        light_name = "pointLights[].diffuse";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);

        shader->setVec3(light_name.c_str(), envir_light->Point_Lights[i].diffuse);

        light_name = "pointLights[].ambient";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setVec3(light_name.c_str(), envir_light->Point_Lights[i].ambient);

        light_name = "pointLights[].specular";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setVec3(light_name.c_str(), envir_light->Point_Lights[i].specular);

        light_name = "pointLights[].constant";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setFloat(light_name.c_str(), 1.0f);

        light_name = "pointLights[].linearTerm";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setFloat(light_name.c_str(), 0.09f);

        light_name = "pointLights[].quadraticTerm";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setFloat(light_name.c_str(), 0.032f);

        light_name = "spotlight[0].direction";
        //light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setVec3(light_name.c_str(), glm::vec3(0.0f, 1.0f, 0.0f));

        light_name = "spotlight[].ambient";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        // Pointlight[0] data  is wrong somewhere
        //shader->setVec3(light_name.c_str(), envir_light->Point_Lights[0].ambient);
        shader->setVec3(light_name.c_str(), glm::vec3(0.5f));

        light_name = "spotlight[].diffuse";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setVec3(light_name.c_str(), envir_light->Point_Lights[0].diffuse);

        light_name = "spotlight[].specular";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setVec3(light_name.c_str(), envir_light->Point_Lights[0].specular);

        light_name = "spotlight[].constant";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setFloat(light_name.c_str(), 1.0f);

        light_name = "spotlight[].linearTerm";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setFloat(light_name.c_str(), 0.09f);

        light_name = "spotlight[].quadraticTerm";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setFloat(light_name.c_str(), 0.032f);

        light_name = "spotlight[].CutOff";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setFloat(light_name.c_str(), 0.3f);

        light_name = "spotlight[].OuterCutOff";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setFloat(light_name.c_str(), 0.4f);
    }
    glUseProgram(0);
};

void setup_pointlight_(global_light* envir_light){
    ReloadGLFunction(&BackBuffer);
    setup_pointlight(envir_light);
}

void Set_environmental_light_(B_shader_program* shader, global_light* envir_light, Camera* camera){
    //ReloadGLFunction(&BackBuffer);
    set_environmental_light(shader, envir_light, camera);
};
