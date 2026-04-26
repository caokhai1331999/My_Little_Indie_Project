#if !defined(PSUEDO_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

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

bool32 check_collision(collision_box* box1 = nullptr, collision_box* box2 = nullptr);

#define PSUEDO_H
#endif
