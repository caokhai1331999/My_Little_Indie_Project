#if !defined(TILE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <stdint.h>
#include <iostream>

using namespace std;

struct 3DPos{
    real32 x;
    real32 y;
    real32 z;
    3DPos(){
        x = 0;  
        y = 0;  
        z = 0;  
    };
};

struct Tile{
    3DPos Pos;
    int ID;
    Tile(){
        Pos = {};
        ID = 0;;
    };
};

// void LoadTileMap();
void DrawTile(std::string path);

#define TILE_H
#endif 
