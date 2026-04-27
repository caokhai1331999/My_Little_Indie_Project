/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "psuedo.h"

bool32 check_collision( space_box* box1 = nullptr, space_box* box2 = nullptr){
    //AABB BBAA
    //assume that the position point is in the center of the box
    bool32 collided = false;

    glm::vec3 spaceforBox1;
    //ON X-axis
    if (((box1->position[3][0] < box2->position[3][0])&&(box1->position[3][0] + box1->size.x > box2->position[3][0])) || ((box2->position[3][0] < box1->position[3][0])&&(box2->position[3][0] + box2->size.x > box1->position[3][0]))){
        //implement here
        spaceforBox1.x = box1->position[3][0] - box2->position[3][0];
        !collided?collided = true:collided=false;     
    }
    //ON y-axis
    else if (((box1->position[3][1] < box2->position[3][1])&&(box1->position[3][1] + box1->size.y > box2->position[3][1])) || ((box2->position[3][1] < box1->position[3][1])&&(box2->position[3][1] + box2->size.y > box1->position[3][1]))){
        spaceforBox1.y = box1->position[3][1] - box1->position[3][1];
        !collided?collided = true:collided=false;        
    }
    //ON z-axis
    else if (((box1->position[3][2] < box2->position[3][2])&&(box1->position[3][2] + box1->size.z > box2->position[3][2])) || ((box2->position[3][2] < box1->position[3][2])&&(box2->position[3][2] + box2->size.z > box1->position[3][2]))){
        spaceforBox1.z = box1->position[3][2] - box1->position[3][2];
        !collided?collided = true:collided=false;        
    }

    box2->collide_list->push_back(collided_space(spaceforBox1));

    glm::vec3 spaceforBox2 = glm::vec3(-1 * spaceforBox1.x, -1 * spaceforBox1.y, -1 * spaceforBox1.z);
    box1->collide_list->push_back(collided_space(spaceforBox2));

    return collided;
};

int main(){
    std::string Mname = "terrain";                
    Model_* land = nullptr;
    land = new Model_(false, &Mname);
    std::string terrain_path = "C:/Users/klove/Documents/repos/GLFW2/Vulkan_Learning_Project/build/source/stylised_terrain_tile_1011124259_texture_fbx/stylised_terrain_tile_1011124259_texture.fbx";
    loadModel_(land, terrain_path);

    
};

//What if we do this on the complex model this ain't work so well cause this is just a rough collision checking method;
glm::mat4 CamCoor_core = glm::mat4(1.0f);
CamCoor_core = glm::scale(CamCoor_core, glm::vec3(0.01, 0.01, 0.01 ));


Model_* CamCoor_ = nullptr;
loadModel_(CamCoor_, "");
model_shader_->setMat4(CamCoor_, "");
// Game Physics
void CalculateGravity(float* weight){
    ;
}

// Game Logics
struct physics{
    float weight = 0.0f;
    float FallingSpeed = 0.0f;
};
// What we do is just affect the model matrix in world space.
//================================

class level_feature{
    ;
};

class entity{
private:
    space_box collided_box;
    // This matrix will contain anything about
    //the entity in the made-up world like:
    //position, rotation, scale
    glm::mat4 world_present_model;
    
    bool32 is_moving;
    bool32 is_dead;

    int current_level;
    int storage;
    skills skill;
    int stregth;

    int Heath_Bar;
//
    controller driver;
};

// Handshake cross check for collision between entities and entities with background

// TileMap contain array of positions in xz plane
// wherein x is analogous to y and z is similar to x in 2D coordinates
//


void drawTile(const unsigned int VaoID = 0, const B_shader_program* Brush = nullptr, const Tile_Map* Map);

// In order to save performance we just draw tile that is in viewing space
std::vector<Tile*>ConstructTileMap(int width, int length, int height){
    for(int h = 0; h < height; h++){
        for(int l = 0; l < length; l+=TILE_WIDTH)
            
        };
    }
;}

void drawTile(const unsigned int VaoID = 0, const B_shader_program* Brush = nullptr, const Tile_Map* Map){
    glBindVertexArray(VaoID);
    Brush->use();
    for(*Tile const &tile: Map->map){
        
    };
};

class Tile_Map{
    std::vector<*Tile>map;
}
