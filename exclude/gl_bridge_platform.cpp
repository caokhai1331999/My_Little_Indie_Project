/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "gl_bridge_platform_h"

void init_volume_map(simple_map* map, std::vector<Mesh*>*Mesh_Group){
    map->height = ROOM_HEIGHT;
    map->breadth = ROOM_BREADTH;
    map->length = ROOM_LENGTH;
    map->map_size = (size_t)(map->height * map->breadth * map->length)+1;
    map->map_content = (unsigned int*)VirtualAlloc(map->map_content, map_size);
}

// We need a pre-created Texture group
void sketch_map(simple_map* map, Mesh* mesh_group){
    srand(time(NULL));
    int x, y, z;
    //for(int y = 0; y < map->height; y++){
        //for(int x = 0; x < map->breadth; x++){
            //for(int z = 0; z < map->length; z++)
// what we have:
    //
    //Spawn and Store specs of entities
    for(size_t int i = 0; i < map->size; i++)
    {
                //// we do every single cube here which have the size of 1,1,1
                //
                // have to be more specificly rational about this one
                // ground first then up
                // what to store OMG we just need to store the mesh ID
        if(i > w)
                *map->map_content++ = rand()%(mesh_group->size()-1);
    }
 //}        
    //};
}

void render_in_group (Graphic_Properties* Graphic, simple_volume_map* world_map, Camera* chosen_camera){    
    //for(int i = 0; i < (int)objects_group->size()-1; i++){
    // we can use instance
    // first feed shader with mesh data (Store with VAO)
    // Then update the relative position of the obj(with world, or just mere screen space with text)
    // Then use each brush(shader) in brush_set to draw object

    Graphic->shader[i]->setMat4("projection", chosen_camera->projection); 
    Graphic->shader[i]->setMat4("view", chosen_camera->view);
    for(size_t i = 0; i < world_map->size; i++){
// Each shader represent for one layer of effect at least.
        if(!world_map->map_content[i]){
            //By modding for that dimension we always have a number in its range;
            // I think I done interpret the index to the entities postion in world
            // space
            glm::vec3 postion = {(float)i%world_map->w, (i>(world_map->w*world_map->l))?(float)(i/(world_map->w*world_map->h)):0.0f, i>world_map->w?(i/world_map->w)%world_map->l:0.0f};
            // now we decide how to add matched id in Graphic object;
            // replace i with some thing
            glBindVertexArray(Graphics->mesh_group[map->map_content[i]]);
            Graphic->shader[i]->use();
            // Postion may be we use i * w * l * h
            Graphic->shader[i]->setVec3("Postion", map->map_content[i+1]);
            Draw(obj->graphic_->mesh, obj->graphic_->shader[i]);
        }
    };
    // Then draw post effect here.
    glUseProgram(0);
}
