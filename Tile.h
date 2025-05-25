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

struct Tile{
    int x;
    int y;
    int z;
    int ID;
};

// void LoadTileMap();
void DrawTile(std::string path);

#define TILE_H
#endif 
