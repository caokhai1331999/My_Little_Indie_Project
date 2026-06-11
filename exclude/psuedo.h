#if !defined(PSUEDO_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */


typedef int motion_state;
typedef motion_state basic_motion;
typedef motion_state complex_motion;

#define (basic_motion)0 IDLE;
#define (basic_motion)1 MOVING_FORWARD;
#define (basic_motion)2 MOVING_BACKWARD;
#define (basic_motion)3 MOVING_RIGHT;
#define (basic_motion)4 MOVING_LEFT;
#define (complex_motion)5 JUMPING;

// how can gravity affect object

struct object_motion_state_group{
    basic_motion;
    complex_motion;
}

struct rigid_body{
    float acceleration;
    float veclocity;

    object_motion_state_group;
}

void apply_gravity(float mass);

class object{
private:
    space_box box_;
    glm::vec3 position;
    bool destroyed_;
    bool collided_;
// For rigid body
    float acceleration;
    float mass;
    glm::vec3/float veclocity;
public:
    void move(glm::vec3 offset);
    void Check_Collision(World* world, Object* Monster);
}

unsigned int Per_Tile_Vertex[] = {
    ;
}

static const unsigned int planeIndices[] = {
//Even though the vertex 1, 0 will be reused but we have to feed them name for opengl just like this
    2, 0, 1, 1, 0, 4, 1, 7
};

static const float PlaneVertices[] = {
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
    // x,    y,     z
    3.0f, -1.0f, -3.0f, //Vertex[0].Pos
   -1.0f, -1.0f,  3.0f,
    3.0f, -1.0f,  3.0f,

    3.0f, -1.0f, -3.0f,
   -1.0f, -1.0f, -3.0f,
   -1.0f, -1.0f,  3.0f,

    3.0f, -1.0f, -3.0f,
   -1.0f, -1.0f,  3.0f,
   -1.0f, -1.0f, -3.0f
};

struct collision_box{
    float x;
    float y;
    float z;
//Length is always positive
    float length;
    
    collision_box(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f,
                  float length_ = 0.0f): x(x_), y(y_), z(z_), length(length_)
    {
        
    }
};

struct Tile{
    //C_Model* model or model ID to save performance
    // Do we need different size for different type
    glm::vec3 position;
    glm::vec3 scale;

    unsigned int TextureID;
};

bool32 check_collision(collision_box* box1 = nullptr, collision_box* box2 = nullptr);

unsigned int LoadTileTexture(char* path){
    std::string dir = ".\";
    Tile.TextureID = TextureFromFile(path, dir, false);
}

void Jump

#define PSUEDO_H
#endif
